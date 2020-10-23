/*******************************************************************************
  Policy Manager Source file

  Company:
    Microchip Technology Inc.

  File Name:
    policy_manager.c

  Description:
    This file contains the function definitions for Policy Manager functions
 *******************************************************************************/
/*******************************************************************************
Copyright ©  [2019-2020] Microchip Technology Inc. and its subsidiaries.

Subject to your compliance with these terms, you may use Microchip software and
any derivatives exclusively with Microchip products. It is your responsibility
to comply with third party license terms applicable to your use of third party
software (including open source software) that may accompany Microchip software.

THIS SOFTWARE IS SUPPLIED BY MICROCHIP "AS IS".  NO WARRANTIES, WHETHER EXPRESS,
IMPLIED OR STATUTORY, APPLY TO THIS SOFTWARE, INCLUDING ANY IMPLIED WARRANTIES
OF NON-INFRINGEMENT, MERCHANTABILITY, AND FITNESS FOR A PARTICULAR PURPOSE. IN
NO EVENT WILL MICROCHIP BE LIABLE FOR ANY INDIRECT, SPECIAL, PUNITIVE,
INCIDENTAL OR CONSEQUENTIAL LOSS, DAMAGE, COST OR EXPENSE OF ANY KIND
WHATSOEVER RELATED TO THE SOFTWARE, HOWEVER CAUSED, EVEN IF MICROCHIP HAS BEEN
ADVISED OF THE POSSIBILITY OR THE DAMAGES ARE FORESEEABLE.  TO THE FULLEST
EXTENT ALLOWED BY LAW, MICROCHIP'S TOTAL LIABILITY ON ALL CLAIMS IN ANY WAY
RELATED TO THIS SOFTWARE WILL NOT EXCEED THE AMOUNT OF FEES, IF ANY, THAT YOU
HAVE PAID DIRECTLY TO MICROCHIP FOR THIS SOFTWARE.
*******************************************************************************/

#include <psf_stdinc.h>

/*************************************VBUS & VCONN on/off Timer APIS*********************************/
void DPM_VBUSorVCONNOnOff_TimerCB (UINT8 u8PortNum, UINT8 u8DummyVariable)
{
    if(TRUE == DPM_NotifyClient(u8PortNum, eMCHP_PSF_TYPEC_ERROR_RECOVERY))
    {
        DPM_SetTypeCState(u8PortNum, TYPEC_ERROR_RECOVERY, TYPEC_ERROR_RECOVERY_ENTRY_SS);
   
        gasPolicyEngine[u8PortNum].ePEState = ePE_INVALIDSTATE;
        gasPolicyEngine[u8PortNum].ePESubState = ePE_INVALIDSUBSTATE;

        gasPolicyEngine[u8PortNum].u8PETimerID = MAX_CONCURRENT_TIMERS;

        #if (TRUE == INCLUDE_PD_PR_SWAP)
        /* Clear the PR_Swap In Progress Flag during PSSourceOff Timer expiry.
           This scenario would get hit only when PS_RDY is not received from the 
           Original Source during Sink to Source PR_Swap
           Note: During normal scenarios other than PR_swap, value of u8DummyVariable
           would be registered as 0 */
        if (DPM_CLR_PR_SWAP_IN_PROGRESS_MASK == u8DummyVariable)
        {
            gasPolicyEngine[u8PortNum].u8PEPortSts &= ~(PE_PR_SWAP_IN_PROGRESS_MASK);
        }
        #endif 
    }
    else
    {
        /*Do nothing. If User application returns FALSE for 
        eMCHP_PSF_TYPEC_ERROR_RECOVERY notification, it is expected that
        the user application will raise a Port disable client request*/
    }
}
void DPM_SrcReady_TimerCB (UINT8 u8PortNum, UINT8 u8DummyVariable)
{
    if(gasPolicyEngine[u8PortNum].u8PEPortSts & PE_EXPLICIT_CONTRACT)
    {
        gasPolicyEngine[u8PortNum].ePEState = ePE_SRC_HARD_RESET;
        gasPolicyEngine[u8PortNum].ePESubState = ePE_SRC_HARD_RESET_ENTRY_SS;
    }    
    else
    {
        DPM_VBUSorVCONNOnOff_TimerCB ( u8PortNum, u8DummyVariable);
    }
    
    gasPolicyEngine[u8PortNum].u8PETimerID = MAX_CONCURRENT_TIMERS;
}

void DPM_VCONNONError_TimerCB (UINT8 u8PortNum , UINT8 u8DummyVariable)
{ 
    gasTypeCcontrol[u8PortNum].u8PortSts &= ~TYPEC_VCONN_ON_REQ_MASK;
    gasPolicyEngine[u8PortNum].u8PETimerID = MAX_CONCURRENT_TIMERS;
    
    if(gasDPM[u8PortNum].u8VCONNErrCounter > (gasCfgStatusData.sPerPortData[u8PortNum].u8VCONNMaxFaultCnt))
    {      
        /*Disable the receiver*/
        PRL_EnableRx (u8PortNum, FALSE);
        
        /*Kill all the Port timers*/
        PDTimer_KillPortTimers (u8PortNum);
        
        /*Disable VCONN by switching off the VCONN FETS which was enabled previously*/
        TypeC_EnabDisVCONN (u8PortNum, TYPEC_VCONN_DISABLE);
        
        if (PD_ROLE_SOURCE == DPM_GET_CURRENT_POWER_ROLE(u8PortNum))
        {		          
            /*Disable VBUS by driving to vSafe0V if port role is a source*/
            DPM_TypeCSrcVBus5VOnOff(u8PortNum, DPM_VBUS_OFF);
        
            /*Assign an idle state to wait for detach*/
            gasTypeCcontrol[u8PortNum].u8TypeCSubState = TYPEC_ATTACHED_SRC_IDLE_SS;
            
            DEBUG_PRINT_PORT_STR(u8PortNum,"VCONN_ON_ERROR: Entered SRC Powered OFF state\r\n");
        }
        else
        { 
            /*Assign an idle state to wait for detach*/
            gasTypeCcontrol[u8PortNum].u8TypeCSubState = TYPEC_ATTACHED_SNK_IDLE_SS;
            
            DEBUG_PRINT_PORT_STR(u8PortNum,"VCONN_ON_ERROR: Entered SNK Powered OFF state\r\n");
        }       
        gasPolicyEngine[u8PortNum].ePEState = ePE_INVALIDSTATE;
        gasPolicyEngine[u8PortNum].ePESubState = ePE_INVALIDSUBSTATE;
        
        (void)DPM_NotifyClient(u8PortNum, eMCHP_PSF_PORT_POWERED_OFF);
    }
    else
    {
        gasDPM[u8PortNum].u8VCONNErrCounter++;    
        PE_SendHardReset(u8PortNum);    
    }    
}
/****************************** DPM APIs Accessing Type C Port Control Module*********************/
void DPM_GetTypeCStates(UINT8 u8PortNum, UINT8 *pu8TypeCState, UINT8 *pu8TypeCSubState)
{
    *pu8TypeCState = gasTypeCcontrol[u8PortNum].u8TypeCState;
    *pu8TypeCSubState = gasTypeCcontrol[u8PortNum].u8TypeCSubState;
}
void DPM_SetTypeCState(UINT8 u8PortNum, UINT8 u8TypeCState, UINT8 u8TypeCSubState)
{
    gasTypeCcontrol[u8PortNum].u8TypeCState = u8TypeCState;
    gasTypeCcontrol[u8PortNum].u8TypeCSubState = u8TypeCSubState;
}

/**************************DPM APIs for VCONN *********************************/
void DPM_VCONNOnOff(UINT8 u8PortNum, UINT8 u8VConnEnable)
{
    if(DPM_VCONN_ON == u8VConnEnable)
    {
        /*Enable VCONN by switching on the VCONN FETS*/
        TypeC_EnabDisVCONN (u8PortNum, TYPEC_VCONN_ENABLE);              
    }    
    else
    {
        /*Disable VCONN by switching off the VCONN FETS*/
        TypeC_EnabDisVCONN (u8PortNum, TYPEC_VCONN_DISABLE);     
    }
}

UINT8 DPM_IsPortVCONNSource(UINT8 u8PortNum)
{ 
    UINT8 u8IsVCONNSrc;
    if(gasTypeCcontrol[u8PortNum].u8IntStsISR & TYPEC_VCONN_SOURCE_MASK)
    {
        u8IsVCONNSrc = TRUE;
    }
    else
    {
        u8IsVCONNSrc = FALSE;
    }
    return u8IsVCONNSrc;
}
/*******************************************************************************/

/**************************DPM APIs for VBUS* *********************************/
void DPM_SetPortPower(UINT8 u8PortNum)
{
    UINT16 u16VoltageInmV = gasCfgStatusData.sPerPortData[u8PortNum].u16NegoVoltageInmV;
    UINT16 u16CurrentInmA = SET_TO_ZERO;
    
    if (PD_ROLE_SOURCE == DPM_GET_CURRENT_POWER_ROLE(u8PortNum))
    {
        u16CurrentInmA = gasCfgStatusData.sPerPortData[u8PortNum].u16NegoCurrentInmA;
                
        TypeC_ConfigureVBUSThr(u8PortNum, u16VoltageInmV, u16CurrentInmA, TYPEC_CONFIG_NON_PWR_FAULT_THR);            
        
        PWRCTRL_SetPortPower (u8PortNum, u16VoltageInmV, u16CurrentInmA);
    }
    else
    {
        u16CurrentInmA = gasDPM[u8PortNum].u16SinkOperatingCurrInmA;
        TypeC_ConfigureVBUSThr(u8PortNum, u16VoltageInmV, u16CurrentInmA, TYPEC_CONFIG_NON_PWR_FAULT_THR);
    }
}

void DPM_TypeCSrcVBus5VOnOff(UINT8 u8PortNum, UINT8 u8VbusOnorOff)
{
	UINT16 u16CurrentInmA, u16VoltageInmV;
    
	if(PD_ROLE_SOURCE == DPM_GET_CURRENT_POWER_ROLE(u8PortNum))
	{		
        if (DPM_VBUS_ON == u8VbusOnorOff)
        {
            /* This API with DPM_VBUS_ON argument will be called from 
               i.  Type C Attached Src state
               ii. PE Transition to default state(after a Hard Reset)
               During both these scenarios, the port will not be in an 
               explicit contract. Therefore, set the current limit based 
               on the Rp value configured by the user */
            u16CurrentInmA = TypeC_ObtainCurrentValueFrmRp(u8PortNum);
            u16VoltageInmV = TYPEC_VBUS_5V;
        }
        else
        {
            u16CurrentInmA = DPM_0mA; 
            u16VoltageInmV = TYPEC_VBUS_0V; 
        }
        
        /* Update power related status registers */
        gasCfgStatusData.sPerPortData[u8PortNum].u16NegoCurrentInmA = u16CurrentInmA; 
        gasCfgStatusData.sPerPortData[u8PortNum].u16NegoVoltageInmV = u16VoltageInmV; 
        gasCfgStatusData.sPerPortData[u8PortNum].u16AllocatedPowerIn250mW = ((u16CurrentInmA * u16VoltageInmV) / DPM_250mW); 
        
        /* Configure VBUS threshold as per the voltage value */
        TypeC_ConfigureVBUSThr(u8PortNum, u16VoltageInmV, u16CurrentInmA, TYPEC_CONFIG_NON_PWR_FAULT_THR);
        /* Drive the VBUS as per the voltage value */
        PWRCTRL_SetPortPower (u8PortNum, u16VoltageInmV, u16CurrentInmA);        
    }
}

UINT16 DPM_GetVBUSVoltage(UINT8 u8PortNum)
{
    UINT8 u8VBUSPresence = SET_TO_ZERO; 
    UINT16 u16VBUSVoltageInmV = SET_TO_ZERO; 
    
    /* u8IntStsISR shall be used only for fixed voltages. In case of PPS contract, 
       voltage that is driven on VBUS is the output voltage requested by Sink 
       in RDO */
    if((gasPolicyEngine[u8PortNum].u8PEPortSts & PE_EXPLICIT_CONTRACT) &&
            (DPM_PD_PPS_CONTRACT == DPM_GET_CURRENT_EXPLICIT_CONTRACT(u8PortNum)))
    {
        u16VBUSVoltageInmV = DPM_GET_OP_VOLTAGE_FROM_PROG_RDO_IN_mV(gasCfgStatusData.sPerPortData[u8PortNum].u32RDO);
    }
    else
    {
        u8VBUSPresence = (gasTypeCcontrol[u8PortNum].u8IntStsISR & TYPEC_VBUS_PRESENCE_MASK);

        if (TYPEC_VBUS_0V_PRES == u8VBUSPresence)
        {
            u16VBUSVoltageInmV = TYPEC_VBUS_0V; 
        }
        else if (TYPEC_VBUS_5V_PRES == u8VBUSPresence)
        {
            u16VBUSVoltageInmV = TYPEC_VBUS_5V; 
        }
        else
        {
            u8VBUSPresence = (u8VBUSPresence >> TYPEC_VBUS_PRESENCE_POS);
            u16VBUSVoltageInmV = DPM_GET_VOLTAGE_FROM_PDO_MILLI_V(gasCfgStatusData.sPerPortData[u8PortNum].u32aAdvertisedPDO[--u8VBUSPresence]);
        }
    }    
    return u16VBUSVoltageInmV; 
}

void DPM_EnablePowerFaultDetection(UINT8 u8PortNum)
{	
#if (TRUE == INCLUDE_POWER_FAULT_HANDLING)
    /* set the threshold to detect fault*/
    TypeC_ConfigureVBUSThr(u8PortNum,
            gasCfgStatusData.sPerPortData[u8PortNum].u16NegoVoltageInmV,
            gasCfgStatusData.sPerPortData[u8PortNum].u16NegoCurrentInmA,
            TYPEC_CONFIG_PWR_FAULT_THR);
    
#endif
}
/*******************************************************************************/
/****DPM APIs to set power,data roles and PD Spec Rev in status variables*******/
/*******************************************************************************/
void DPM_UpdatePowerRole(UINT8 u8PortNum, UINT8 u8NewPowerRole)
{
    /*Set Port Power Role in gasDPM[u8PortNum].u16DPMStatus variable*/
    gasDPM[u8PortNum].u16DPMStatus &= (~DPM_CURR_POWER_ROLE_MASK);
    gasDPM[u8PortNum].u16DPMStatus |= (u8NewPowerRole << DPM_CURR_POWER_ROLE_POS);
    
    /* Set Port Power Role in Port Connection Status register */
    gasCfgStatusData.sPerPortData[u8PortNum].u32PortConnectStatus &= ~(DPM_PORT_POWER_ROLE_STATUS_MASK);
    gasCfgStatusData.sPerPortData[u8PortNum].u32PortConnectStatus |= (u8NewPowerRole << DPM_PORT_POWER_ROLE_STATUS_POS); 

#if(TRUE == INCLUDE_PD_DRP)
    /*Set power role in Port X IO Status register*/
    if(PD_ROLE_SOURCE == u8NewPowerRole)
    {
        MCHP_PSF_HOOK_GPIO_FUNC_DRIVE(u8PortNum, ePOWER_ROLE_FUNC, eGPIO_ASSERT);
        gasCfgStatusData.sPerPortData[u8PortNum].u32PortIOStatus |= (DPM_PORT_IO_POWER_ROLE_STATUS);
    }
    else
    {
        MCHP_PSF_HOOK_GPIO_FUNC_DRIVE(u8PortNum, ePOWER_ROLE_FUNC, eGPIO_DEASSERT);
        gasCfgStatusData.sPerPortData[u8PortNum].u32PortIOStatus &= ~(DPM_PORT_IO_POWER_ROLE_STATUS);        
    }
#endif
}

void DPM_UpdateDataRole(UINT8 u8PortNum, UINT8 u8NewDataRole)
{
    /*Set Port Data Role in gasDPM[u8PortNum].u16DPMStatus variable*/
    gasDPM[u8PortNum].u16DPMStatus &= (~DPM_CURR_DATA_ROLE_MASK); 
    gasDPM[u8PortNum].u16DPMStatus |= (u8NewDataRole << DPM_CURR_DATA_ROLE_POS);
    
    /* Set Port Data Role in Port Connection Status register */
    gasCfgStatusData.sPerPortData[u8PortNum].u32PortConnectStatus &= ~(DPM_PORT_DATA_ROLE_STATUS_MASK);
    gasCfgStatusData.sPerPortData[u8PortNum].u32PortConnectStatus |= (u8NewDataRole << DPM_PORT_DATA_ROLE_STATUS_POS); 

#if(TRUE == INCLUDE_PD_DRP)
    if(PD_ROLE_DFP == u8NewDataRole)
    {
        MCHP_PSF_HOOK_GPIO_FUNC_DRIVE(u8PortNum, eDATA_ROLE_FUNC, eGPIO_ASSERT);
        gasCfgStatusData.sPerPortData[u8PortNum].u32PortIOStatus |= (DPM_PORT_IO_DATA_ROLE_STATUS);
    }
    else
    {
        MCHP_PSF_HOOK_GPIO_FUNC_DRIVE(u8PortNum, eDATA_ROLE_FUNC, eGPIO_DEASSERT);
        gasCfgStatusData.sPerPortData[u8PortNum].u32PortIOStatus &= ~(DPM_PORT_IO_DATA_ROLE_STATUS);   
    }
#endif
}

void DPM_UpdatePDSpecRev(UINT8 u8PortNum, UINT8 u8PDSpecRev)
{
    /* Set PD Spec Rev in gasDPM[u8PortNum].u16DPMStatus variable */
    gasDPM[u8PortNum].u16DPMStatus &= ~DPM_CURR_PD_SPEC_REV_MASK;
	gasDPM[u8PortNum].u16DPMStatus |= (u8PDSpecRev << DPM_CURR_PD_SPEC_REV_POS);    
    
    /* Set PD Spec Rev in Port Connection Status register */
    gasCfgStatusData.sPerPortData[u8PortNum].u32PortConnectStatus &= ~(DPM_PORT_PD_SPEC_REV_STATUS_MASK);
    gasCfgStatusData.sPerPortData[u8PortNum].u32PortConnectStatus |= (u8PDSpecRev << DPM_PORT_PD_SPEC_REV_STATUS_POS);    
}

#if (TRUE == INCLUDE_PD_SOURCE)
/**************************************************************************************************/
/*********************************DPM VDM Cable APIs**************************************/
/**************************************************************************************************/
UINT8 DPM_StoreCableIdentity(UINT8 u8PortNum, UINT8 u8SOPType, UINT16 u16Header, UINT32 *u32DataBuf)
{
    UINT32 u32ProductTypeVDO;
    UINT8 u8RetVal = FALSE;
    UINT8 u8CurVal;
    
    /* Store the received VDM Data Object count in u8CableIdentityCnt.
       Subtracting 1 from the actual count to eliminate the VDM Header */
    gasCfgStatusData.sPerPortData[u8PortNum].u8CableIdentityCnt = \
                                        (PRL_GET_OBJECT_COUNT(u16Header) - BYTE_LEN_1);
    
    /* Store the received VDM Data Objects in u32aCableIdentity array */
    (void)MCHP_PSF_HOOK_MEMCPY(gasCfgStatusData.sPerPortData[u8PortNum].u32aCableIdentity, (u32DataBuf + BYTE_LEN_1),
                 (gasCfgStatusData.sPerPortData[u8PortNum].u8CableIdentityCnt * BYTE_LEN_4));                                                   
    
    /* Get the CMD type from received VDM */
    u8RetVal = DPM_GET_VDM_CMD_TYPE(u32DataBuf[DPM_VDM_HEADER_POS]);
    
    /* if Data object is one, received message is NAK */
    if((DPM_VDM_NAK == u8RetVal) || (DPM_VDM_BUSY == u8RetVal))
    {
        u8RetVal = DPM_VDM_NAK;
    }
    else
    {
        /* Get the product VDO from the received message */
        u32ProductTypeVDO = u32DataBuf[DPM_VMD_PRODUCT_TYPE_VDO_POS];               
       
        /* Get the Cable supported current value */
        u8CurVal = DPM_GET_CABLE_CUR_VAL(u32ProductTypeVDO);
        
        /* Setting E-Cable Max Current Value */
        if(DPM_CABLE_CURR_3A == u8CurVal)
        {
            gasDPM[u8PortNum].u16SrcMaxSupportedCurrInmA = DPM_CABLE_CURR_3A_UNIT;
        }        
        else if(DPM_CABLE_CURR_5A == u8CurVal)
        {
            gasDPM[u8PortNum].u16SrcMaxSupportedCurrInmA = DPM_CABLE_CURR_5A_UNIT;
        }        
        else
        {
           /* Do nothing */ 
        }
        
        /* Received message is ACK */
        u8RetVal = DPM_VDM_ACK;
    }    
    
    return u8RetVal;
}
/*******************************************************************************/

/*******************************************************************************************/
/****************************** DPM Source related APIs*****************************************/
/***********************************************************************************************/
/* Validate the received Request message */
UINT8 DPM_ValidateRequest(UINT8 u8PortNum, UINT16 u16Header, UINT8 *u8DataBuf)
{
    UINT8 u8RetVal = FALSE;
    UINT8 u8SinkReqObjPos= SET_TO_ZERO;
    UINT16 u16SinkReqCurrInmA = SET_TO_ZERO;
    UINT16 u16SrcPDOCurrInmA = SET_TO_ZERO;
    UINT8 u8RaPresence = SET_TO_ZERO;
    UINT32 u32PDO = SET_TO_ZERO; 
    UINT32 u32RDO = SET_TO_ZERO; 
#if (TRUE == INCLUDE_PD_SOURCE_PPS) 
    UINT16 u16RDOOpVoltInmV = SET_TO_ZERO; 
#endif           
    /* Get the VCONN enabled status and ACK status of the port */
    u8RaPresence = (gasTypeCcontrol[u8PortNum].u8IntStsISR & TYPEC_VCONN_SOURCE_MASK) & \
                    (~((gasPolicyEngine[u8PortNum].u8PEPortSts & \
                                            PE_CABLE_RESPOND_NAK) >> PE_CABLE_RESPOND_NAK_POS));
    
    /* Get the Requested PDO object position from received buffer */
    u8SinkReqObjPos = ((u8DataBuf[INDEX_3]) & DPM_RDO_OBJ_MASK) >> DPM_RDO_OBJ_POS;
    
    /* Get the PDO/APDO from Advertised PDO Array */
    u32PDO = gasCfgStatusData.sPerPortData[u8PortNum].u32aAdvertisedPDO[u8SinkReqObjPos-1]; 
    
    /* Form the RDO */
    u32RDO = MAKE_UINT32_FROM_BYTES(u8DataBuf[INDEX_0], u8DataBuf[INDEX_1], 
                                        u8DataBuf[INDEX_2], u8DataBuf[INDEX_3]);
    
    if (ePDO_FIXED == (ePDOType)DPM_GET_PDO_TYPE(u32PDO))
    {
        /* Get the Requested current value */
        u16SinkReqCurrInmA = (UINT16)(((u32RDO & DPM_RDO_OPR_CUR_MASK) >> DPM_RDO_OPR_CUR_START_POS) * DPM_PDO_CURRENT_UNIT);
        
        /* Get the current value of Requested Source PDO */
        u16SrcPDOCurrInmA = ((u32PDO & DPM_RDO_MAX_CUR_MASK) * DPM_PDO_CURRENT_UNIT);       
    }
    
#if (TRUE == INCLUDE_PD_SOURCE_PPS)    
    else if (ePDO_PROGRAMMABLE == (ePDOType)DPM_GET_PDO_TYPE(u32PDO))
    {     
        /* Get the Requested current value */ 
        u16SinkReqCurrInmA = DPM_GET_PROG_RDO_OPR_CURRENT_IN_mA(u32RDO); 
                
        /* Get the current value of Requested Source PDO */
        u16SrcPDOCurrInmA = DPM_GET_APDO_MAX_CURRENT_IN_mA(u32PDO); 
         
        /* Get the RDO Output Voltage */
        u16RDOOpVoltInmV = DPM_GET_OP_VOLTAGE_FROM_PROG_RDO_IN_mV(u32RDO);        
    }
#endif 
    else
    {
        /* No support for Variable and Battery power supplies */
    }
    
    /* Update the Previous VBUS voltage */
    if (ePDO_FIXED == (ePDOType)DPM_GET_CURRENT_EXPLICIT_CONTRACT(u8PortNum))
    {
        /* If previous request is for a fixed PDO, calculate previous output
           voltage from gasDPM[u8PortNum].u32NegotiatedPDO */
        gasDPM[u8PortNum].u16PrevVBUSVoltageInmV = DPM_GET_VOLTAGE_FROM_PDO_MILLI_V(gasDPM[u8PortNum].u32NegotiatedPDO);
    }
    else if (ePDO_PROGRAMMABLE == (ePDOType)DPM_GET_CURRENT_EXPLICIT_CONTRACT(u8PortNum))
    {
        /* If previous request is for a PPS APDO, calculate previous output
           voltage from gasCfgStatusData.sPerPortData[u8PortNum].u32RDO */            
        gasDPM[u8PortNum].u16PrevVBUSVoltageInmV = DPM_GET_OP_VOLTAGE_FROM_PROG_RDO_IN_mV(gasCfgStatusData.sPerPortData[u8PortNum].u32RDO);
    }
    else
    {
        /* Do Nothing */
    }
    
    /* If Requested Max current is greater current value of Requested Source PDO or
        Requested object position is invalid, received request is invalid request */ 
    u8RetVal = (u16SinkReqCurrInmA > u16SrcPDOCurrInmA) ? DPM_INVALID_REQUEST : (((u8SinkReqObjPos <= FALSE) || \
               (u8SinkReqObjPos > gasCfgStatusData.sPerPortData[u8PortNum].u8AdvertisedPDOCnt))) ? \
                DPM_INVALID_REQUEST : (FALSE == u8RaPresence) ? DPM_VALID_REQUEST : \
                (u16SinkReqCurrInmA > gasDPM[u8PortNum].u16SrcMaxSupportedCurrInmA) ? \
                DPM_INVALID_REQUEST : DPM_VALID_REQUEST;   
   
#if (TRUE == INCLUDE_PD_SOURCE_PPS)     
    if (ePDO_PROGRAMMABLE == (ePDOType)DPM_GET_PDO_TYPE(u32PDO))
    {
        if ((u16RDOOpVoltInmV >= DPM_GET_APDO_MIN_VOLTAGE_IN_mV(u32PDO)) && 
                (u16RDOOpVoltInmV <= DPM_GET_APDO_MAX_VOLTAGE_IN_mV(u32PDO)))
        {
            u8RetVal &= DPM_VALID_REQUEST; 
        }
        else 
        {
            u8RetVal &= DPM_INVALID_REQUEST; 
        }
    }
#endif 
    
    /* If request is valid set the Negotiated PDO as requested */
    if(DPM_VALID_REQUEST == u8RetVal)
    {
        gasDPM[u8PortNum].u32NegotiatedPDO = u32PDO; 
        gasDPM[u8PortNum].u8NegotiatedPDOIndex = u8SinkReqObjPos;
        gasCfgStatusData.sPerPortData[u8PortNum].u32RDO = u32RDO;
        
        /* Set Capability mismatch status */
        if (u32RDO & DPM_RDO_CAP_MISMATCH_MASK)
        {
            gasCfgStatusData.sPerPortData[u8PortNum].u32PortConnectStatus |= 
                    DPM_PORT_SRC_CAPABILITY_MISMATCH_STATUS; 
        }
        else
        {
            gasCfgStatusData.sPerPortData[u8PortNum].u32PortConnectStatus &= 
                        ~(DPM_PORT_SRC_CAPABILITY_MISMATCH_STATUS);                 
        }

        if (ePDO_FIXED == (ePDOType)DPM_GET_PDO_TYPE(u32PDO))
        {
            /* Set the current Explicit Contract Type as Fixed Supply */
            gasDPM[u8PortNum].u16DPMStatus &= ~(DPM_CURR_EXPLICIT_CONTRACT_TYPE_MASK); 
            
            /* Update Negotiated current in terms of mA */            
            gasCfgStatusData.sPerPortData[u8PortNum].u16NegoCurrentInmA = u16SrcPDOCurrInmA;
            
            /* Update Negotiated voltage in terms of mV */            
            gasCfgStatusData.sPerPortData[u8PortNum].u16NegoVoltageInmV = \
                    DPM_GET_VOLTAGE_FROM_PDO_MILLI_V(gasDPM[u8PortNum].u32NegotiatedPDO);
            
            /* Update the allocated power in terms of 250mW */
            gasCfgStatusData.sPerPortData[u8PortNum].u16AllocatedPowerIn250mW =    
                    ((gasCfgStatusData.sPerPortData[u8PortNum].u16NegoVoltageInmV * gasCfgStatusData.sPerPortData[u8PortNum].u16NegoCurrentInmA) / 
                    DPM_250mW);             
        }
#if (TRUE == INCLUDE_PD_SOURCE_PPS)        
        else if (ePDO_PROGRAMMABLE == (ePDOType)DPM_GET_PDO_TYPE(u32PDO))
        {
            /* Set the current Explicit Contract Type as PPS */
            gasDPM[u8PortNum].u16DPMStatus |= DPM_CURR_EXPLICIT_CONTRACT_TYPE_MASK;           
            
            /* Update Negotiated current in terms of mA */            
            gasCfgStatusData.sPerPortData[u8PortNum].u16NegoCurrentInmA = \
                    DPM_GET_PROG_RDO_OPR_CURRENT_IN_mA(gasCfgStatusData.sPerPortData[u8PortNum].u32RDO); 
            
            /* Update Negotiated voltage in terms of mV */            
            gasCfgStatusData.sPerPortData[u8PortNum].u16NegoVoltageInmV = \
                    DPM_GET_OP_VOLTAGE_FROM_PROG_RDO_IN_mV(gasCfgStatusData.sPerPortData[u8PortNum].u32RDO);
            
            /* Update the allocated power in terms of 250mW */
            gasCfgStatusData.sPerPortData[u8PortNum].u16AllocatedPowerIn250mW =    
                    ((gasCfgStatusData.sPerPortData[u8PortNum].u16NegoVoltageInmV * gasCfgStatusData.sPerPortData[u8PortNum].u16NegoCurrentInmA) / 
                    DPM_250mW);                         
        }
#endif 
        else
        {
            /* Do Nothing */
        }
        DEBUG_PRINT_PORT_STR (u8PortNum,"DPM-PE: Request is Valid \r\n");
    }

    return u8RetVal;
}

/* Reset the current value in PDO */
UINT32 DPM_CurrentCutDown (UINT8 u8PortNum, UINT32 u32PDO)
{
    /* If PDO max current greater than E-Cable supported current, reset the current value */
    if (ePDO_FIXED == (ePDOType)DPM_GET_PDO_TYPE(u32PDO))
    {
        if((u32PDO & DPM_MAX_CURR_MASK) > (DPM_CABLE_CURR_3A_UNIT / DPM_10mA))
        {
            u32PDO &= ~DPM_MAX_CURR_MASK;
            u32PDO |= (DPM_CABLE_CURR_3A_UNIT / DPM_10mA);
            /* Source PDO Current value reduced due to cable limitation */   
            gasCfgStatusData.sPerPortData[u8PortNum].u32PortConnectStatus |= 
                                        DPM_PORT_CABLE_REDUCED_SRC_CAPABILITIES_STATUS;        
        }  
    }
#if (TRUE == INCLUDE_PD_SOURCE_PPS)    
    else if (ePDO_PROGRAMMABLE == (ePDOType)DPM_GET_PDO_TYPE(u32PDO))
    {
        if (DPM_GET_APDO_MAX_CURRENT_IN_mA(u32PDO) > DPM_3000mA)
        {
            u32PDO &= ~(DPM_APDO_MAX_CURRENT_MASK);
            u32PDO |= DPM_3A_IN_50mA; 
            /* Source APDO Current value reduced due to cable limitation */   
            gasCfgStatusData.sPerPortData[u8PortNum].u32PortConnectStatus |= 
                                        DPM_PORT_CABLE_REDUCED_SRC_CAPABILITIES_STATUS;                    
        }
    }
#endif 
    else
    {
        /* No support for Variable and Battery power supplies */
    }
    
    return u32PDO; 
}

/* Copy the Source capabilities */
void DPM_ChangeCapabilities (UINT8 u8PortNum, UINT32* pu32DataObj, UINT32 *pu32SrcCaps, UINT8 u8pSrcPDOCnt)
{
    for (UINT8 u8PDOindex = SET_TO_ZERO; u8PDOindex < u8pSrcPDOCnt; u8PDOindex++)
    {   
        /* Reset the current value to E-Cable supported current */
        pu32DataObj[u8PDOindex] = DPM_CurrentCutDown (u8PortNum, pu32SrcCaps[u8PDOindex]);
    }
}

/* Get the source capabilities from the port configuration structure */
void DPM_GetSourceCapabilities(UINT8 u8PortNum, UINT8* u8pSrcPDOCnt, UINT32* pu32DataObj)
{   
	UINT32 *u32pSrcCap;
    
#if (TRUE == INCLUDE_POWER_THROTTLING)
    if ((TRUE == DPM_IS_PT_ENABLED) && (FALSE == DPM_IS_PB_ENABLED(u8PortNum)))
    {
        /* If PB is disabled, update the Source caps based on currently 
           active PT Bank. If PB is enabled, this would be taken care by PB */
        PT_CalculateSrcPDOs(u8PortNum); 
    }
#endif 
    
    if (DPM_GET_CONFIGURED_NEW_PDO_STATUS(u8PortNum))
    {
        *u8pSrcPDOCnt = gasCfgStatusData.sPerPortData[u8PortNum].u8NewSourcePDOCnt;
   
        u32pSrcCap = (UINT32 *)&gasCfgStatusData.sPerPortData[u8PortNum].u32aNewSourcePDO[INDEX_0]; 
    }
    else
    {
        *u8pSrcPDOCnt = gasCfgStatusData.sPerPortData[u8PortNum].u8SourcePDOCnt;
   
        u32pSrcCap = (UINT32 *)&gasCfgStatusData.sPerPortData[u8PortNum].u32aSourcePDO[INDEX_0];   
    }
    
    /* E-Cable present */
    if (gasTypeCcontrol[u8PortNum].u8PortSts & TYPEC_PWDCABLE_PRES_MASK)
    {
        /* If E-Cable max current is 5A, pass the capabilities without change */
        if(DPM_CABLE_CURR_5A_UNIT == gasDPM[u8PortNum].u16SrcMaxSupportedCurrInmA)
        {
            /* The attached USB-C cable supports the locally-defined Source PDOs */
            gasCfgStatusData.sPerPortData[u8PortNum].u32PortConnectStatus &= 
                                        ~(DPM_PORT_CABLE_REDUCED_SRC_CAPABILITIES_STATUS);

            (void)MCHP_PSF_HOOK_MEMCPY(&pu32DataObj[INDEX_0], &u32pSrcCap[INDEX_0],\
                    DPM_4BYTES_FOR_EACH_PDO_OF(*u8pSrcPDOCnt) );
        }
        /* If E-Cable max current is 3A and PDO current value is more than 3A, 
            reset the current value of PDOs */
        else
        {
            DPM_ChangeCapabilities (u8PortNum, &pu32DataObj[INDEX_0], &u32pSrcCap[INDEX_0], *u8pSrcPDOCnt);
        }          
    }   
    else
    {
        DPM_ChangeCapabilities (u8PortNum, pu32DataObj, &u32pSrcCap[INDEX_0],*u8pSrcPDOCnt);  
    }     
}

/**********************************************************************************/
#if (TRUE == INCLUDE_PD_SOURCE_PPS)

UINT32 DPM_ReturnPPSSrcTransTmrVal (UINT8 u8PortNum)
{
    UINT16 u16PrevRDOVoltInmV = gasDPM[u8PortNum].u16PrevVBUSVoltageInmV; 
    UINT16 u16CurrRDOVoltInmV = DPM_GET_OP_VOLTAGE_FROM_PROG_RDO_IN_mV(gasCfgStatusData.sPerPortData[u8PortNum].u32RDO); 
    
    /* Calculate the Voltage difference based on whether the transition is 
       positive or negative. Use tPpsSrcTransLarge for voltage steps larger 
       than vPpsSmallStep. Else, use tPpsSrcTransSmall */
    UINT16 u16vPpsStepmV = ((u16CurrRDOVoltInmV > u16PrevRDOVoltInmV) ? \
         (u16CurrRDOVoltInmV - u16PrevRDOVoltInmV) : (u16PrevRDOVoltInmV - u16CurrRDOVoltInmV)); 
    
    if (u16vPpsStepmV > DPM_PPS_VOLTAGE_SMALL_STEP)
    {
        return PE_PPS_SRCTRANSLARGE_TIMEOUT_MS; 
    }
    else
    {
        return PE_PPS_SRCTRANSSMALL_TIMEOUT_MS; 
    }
}

/***********************Status API******************************************/
UINT32 DPM_ObtainAlertDO(UINT8 u8PortNum)
{
    /*Alert Data Object - BIT[31:24] - Type of Alert
                          BIT[23:20] - Fixed Batteries - Always 0x00
                          BIT[19:16] - Hot swappable Batteries -  NA - 0x00
                          BIT[15:0] - Reserved  */
    UINT32 u32AlertDO = (((UINT32)gasDPM[u8PortNum].u8AlertType) << DPM_ALERT_TYPE_FIELD_POS);
    
    /*Clear the gasDPM[u8PortNum].u8AlertType values stored once message is informed
     to PE through this function*/
    gasDPM[u8PortNum].u8AlertType = SET_TO_ZERO;
    
    return u32AlertDO;
}

void DPM_ObtainStatusDB(UINT8 u8PortNum, UINT8 *pau8StatusDO)
{
    /*Byte 0 - Internal Temperature
     Byte 1 - Present Input
     Byte 2 - Present Battery Input
     Byte 3 - Event Flags
     Byte 4 - Temperature Status
     Byte 5 - Power Status*/
    pau8StatusDO[INDEX_0] = SET_TO_ZERO;  /*Not Supported*/
    pau8StatusDO[INDEX_1] = BIT(1);       /*External Power is set and DC*/
    pau8StatusDO[INDEX_2] = SET_TO_ZERO;  /*Present Battery Input not applicable*/
    pau8StatusDO[INDEX_3] = gasDPM[u8PortNum].u8StatusEventFlags;
    pau8StatusDO[INDEX_4] = DPM_ReturnTemperatureStatus();
    pau8StatusDO[INDEX_5] = DPM_ReturnPowerStatus(u8PortNum); 
    
    /*Clear the status variable once the Status details are provided to the policy Engine*/
   gasDPM[u8PortNum].u8StatusEventFlags = RESET_TO_ZERO;
}

UINT32 DPM_ObtainPPSStatusDB (UINT8 u8PortNum) 
{
    /* PPS Status Data Block - Byte[0-1] - Output Voltage
                               Byte[2]   - Output Current
                               Byte[3]   - Real Time Flags */
    
    UINT32 u32OutputVoltageIn20mV = MCHP_PSF_HOOK_GET_OUTPUT_VOLTAGE_IN_mV;
    UINT32 u32OutputCurrentIn50mA = MCHP_PSF_HOOK_GET_OUTPUT_CURRENT_IN_mA;


#if (MCHP_PSF_HOOK_GET_OUTPUT_CURRENT_IN_mA == DPM_PPSSDB_OUTPUT_USER_CONFIGURED_UNSUPPORTED_VAL) 
    u32OutputVoltageIn20mV = DPM_PPSSDB_OUTPUT_VOLT_UNSUPPORTED_VAL;
#else
    u32OutputVoltageIn20mV =  ((MCHP_PSF_HOOK_GET_OUTPUT_VOLTAGE_IN_mV) / DPM_20mV);
#endif
    
#if (MCHP_PSF_HOOK_GET_OUTPUT_CURRENT_IN_mA == DPM_PPSSDB_OUTPUT_USER_CONFIGURED_UNSUPPORTED_VAL)
    u32OutputCurrentIn50mA = DPM_PPSSDB_OUTPUT_CURRENT_UNSUPPORTED_VAL;
#else
    u32OutputCurrentIn50mA =  ((MCHP_PSF_HOOK_GET_OUTPUT_CURRENT_IN_mA) / DPM_50mA);
#endif 
    
    gasDPM[u8PortNum].u8RealTimeFlags |= DPM_ReturnTemperatureStatus();
    
    UINT32 u32PPSStatusDB = (u32OutputVoltageIn20mV | \
                                (u32OutputCurrentIn50mA << DPM_PPSSDB_OUTPUT_CURRENT_FIELD_POS) |\
                                (((UINT32)gasDPM[u8PortNum].u8RealTimeFlags) << DPM_PPSSDB_REAL_TIME_FLAG_FIELD_POS));
    
    return u32PPSStatusDB;
}

void DPM_StatusFaultPersist_TimerCB (UINT8 u8PortNum, UINT8 u8DummyVariable)
{
	/* Set the timer Id to Max Concurrent Value*/
 	gasDPM[u8PortNum].u8StsClearTmrID = MAX_CONCURRENT_TIMERS;
	
	/* Reset the status variable*/
    gasDPM[u8PortNum].u8StatusEventFlags = RESET_TO_ZERO;
	
}

UINT8 DPM_ReturnTemperatureStatus (void)
{
    UINT8 u8TempStatus = SET_TO_ZERO; 
    
/* ----------------------------------------
   Bit    Description 
   ----------------------------------------
    0     Reserved and Shall be set to zero
   1..2   00 - Not Supported
          01 - Normal
          10 - Warning
          11 - Over temperature
   3..7   Reserved and Shall be set to zero 
   ---------------------------------------- */
    
    if (DPM_PD_THROTTLE_BANK_A == DPM_GET_CURRENT_PT_BANK)
    {
        u8TempStatus = DPM_TEMP_STATUS_NORMAL; 
    }
    else if ((DPM_PD_THROTTLE_BANK_B == DPM_GET_CURRENT_PT_BANK) || 
                        (DPM_PD_THROTTLE_BANK_C == DPM_GET_CURRENT_PT_BANK))
    {
        u8TempStatus = DPM_TEMP_STATUS_WARNING;
    }
    else
    {
        u8TempStatus = DPM_TEMP_STATUS_OVER_TEMP; 
    }
    return u8TempStatus; 
}

UINT8 DPM_ReturnPowerStatus (UINT8 u8PortNum)
{
    UINT8 u8PwrStatus = SET_TO_ZERO; 
   
 /* -------------------------------------------------------------
    Bit   Description
    -------------------------------------------------------------
     0    Reserved and Shall be set to zero
     1    Source power limited due to cable supported current
     2    Source power limited due to insufficient power available while sourcing other ports
     3    Source power limited due to insufficient external power
     4    Source power limited due to Event Flags in place (Event Flags must also be set)
     5    Source power limited due to temperature
     6:7  Reserved and Shall be set to zero  
    ------------------------------------------------------------- */
        
    if (gasCfgStatusData.sPerPortData[u8PortNum].u32PortConnectStatus & 
                                DPM_PORT_CABLE_REDUCED_SRC_CAPABILITIES_STATUS)
    {
        u8PwrStatus |= DPM_PWRSTS_SRCPWR_LTD_CABLE_CURR; 
    }

    if (gasCfgStatusData.sPerPortData[u8PortNum].u32PortConnectStatus &
                                DPM_PORT_SRC_CAPABILITIES_REDUCED_STATUS)
    {
        if (TRUE == DPM_IS_PB_ENABLED(u8PortNum))
        {
            u8PwrStatus |= DPM_PWRSTS_SRCPWR_LTD_INSUFF_PWR_AVAIL; 
        }  

        if ((DPM_PD_THROTTLE_BANK_B == DPM_GET_CURRENT_PT_BANK) || 
                            (DPM_PD_THROTTLE_BANK_C == DPM_GET_CURRENT_PT_BANK))
        {
            u8PwrStatus |= DPM_PWRSTS_SRCPWR_LTD_TEMP; 
        }        
    }
    
    if (gasDPM[u8PortNum].u8StatusEventFlags)
    {
        u8PwrStatus |= DPM_PWRSTS_SRCPWR_LTD_EVNT_FLAG;
    }
    
    return u8PwrStatus;  
}

#endif /*INCLUDE_PD_SOURCE_PPS*/ 
#endif /*INCLUDE_PD_SOURCE*/  

void DPM_UpdateAdvertisedPDOParam(UINT8 u8PortNum)
{
    UINT8 u8AdvertisedPDOCnt;
    
    /*Clear u32aAdvertisedPDO initially*/
    for(UINT8 u8Index = SET_TO_ZERO; u8Index < DPM_MAX_PDO_CNT; u8Index++)
    {
        gasCfgStatusData.sPerPortData[u8PortNum].u32aAdvertisedPDO[u8Index] = SET_TO_ZERO;
    }
    
    if(PD_ROLE_SOURCE == DPM_GET_CURRENT_POWER_ROLE(u8PortNum))
    {
#if(TRUE == INCLUDE_PD_SOURCE)
        if (DPM_GET_CONFIGURED_NEW_PDO_STATUS(u8PortNum))
        {
            /* Update Advertised PDO Count */
            u8AdvertisedPDOCnt = gasCfgStatusData.sPerPortData[u8PortNum].u8NewSourcePDOCnt;

           /* Update Advertised PDO Registers with New PDOs if NewPDOSlct is enabled. */
            (void)MCHP_PSF_HOOK_MEMCPY(gasCfgStatusData.sPerPortData[u8PortNum].u32aAdvertisedPDO, 
                gasCfgStatusData.sPerPortData[u8PortNum].u32aNewSourcePDO, DPM_4BYTES_FOR_EACH_PDO_OF(u8AdvertisedPDOCnt));            
        }
        else
        {
            /* Update Advertised PDO Count */
            u8AdvertisedPDOCnt = gasCfgStatusData.sPerPortData[u8PortNum].u8SourcePDOCnt;

            /* Update Advertised PDO Registers with Default Source PDOs if NewPDOSlct is not enabled. */
            (void)MCHP_PSF_HOOK_MEMCPY(gasCfgStatusData.sPerPortData[u8PortNum].u32aAdvertisedPDO, 
                gasCfgStatusData.sPerPortData[u8PortNum].u32aSourcePDO, DPM_4BYTES_FOR_EACH_PDO_OF(u8AdvertisedPDOCnt));           
        }
        
        /*Update Advertised PDO count*/
        gasCfgStatusData.sPerPortData[u8PortNum].u8AdvertisedPDOCnt = u8AdvertisedPDOCnt;

        /* Update the Port Connection Status register by comparing the Fixed and 
           Advertised Source PDOs */    
        if (FALSE == MCHP_PSF_HOOK_MEMCMP(&gasCfgStatusData.sPerPortData[u8PortNum].u32aSourcePDO[INDEX_0],
                        &gasCfgStatusData.sPerPortData[u8PortNum].u32aAdvertisedPDO[INDEX_0], 
                        ((MAX(gasCfgStatusData.sPerPortData[u8PortNum].u8SourcePDOCnt, 
                            gasCfgStatusData.sPerPortData[u8PortNum].u8AdvertisedPDOCnt)) * BYTE_LEN_4)))
        {
            /* The advertised PDOs are equivalent to the default configured values */
            gasCfgStatusData.sPerPortData[u8PortNum].u32PortConnectStatus &= 
                                ~(DPM_PORT_SRC_CAPABILITIES_REDUCED_STATUS);         
        }   
        else
        {
            /* The advertised PDOs have been reduced from default configured values */
            gasCfgStatusData.sPerPortData[u8PortNum].u32PortConnectStatus |= 
                                DPM_PORT_SRC_CAPABILITIES_REDUCED_STATUS;                 
        } 
#endif
    }
    else /*PD_ROLE_SINK*/
    {
#if(TRUE == INCLUDE_PD_SINK)

        if (DPM_GET_CONFIGURED_NEW_PDO_STATUS(u8PortNum))
        {
             /* Update Advertised PDO Count */
            u8AdvertisedPDOCnt = gasCfgStatusData.sPerPortData[u8PortNum].u8NewSinkPDOCnt;

           /* Update Advertised PDO Registers with New PDOs if NewPDOSlct is enabled. */
            (void)MCHP_PSF_HOOK_MEMCPY(gasCfgStatusData.sPerPortData[u8PortNum].u32aAdvertisedPDO, 
                gasCfgStatusData.sPerPortData[u8PortNum].u32aNewSinkPDO, DPM_4BYTES_FOR_EACH_PDO_OF(u8AdvertisedPDOCnt));              
        }
        else
        {   /* Update Advertised PDO Count */
            u8AdvertisedPDOCnt = gasCfgStatusData.sPerPortData[u8PortNum].u8SinkPDOCnt;

            /* Update Advertised PDO Registers with Default Sink PDOs if NewPDOSlct is not enabled. */
            (void)MCHP_PSF_HOOK_MEMCPY(gasCfgStatusData.sPerPortData[u8PortNum].u32aAdvertisedPDO, 
                gasCfgStatusData.sPerPortData[u8PortNum].u32aSinkPDO, DPM_4BYTES_FOR_EACH_PDO_OF(u8AdvertisedPDOCnt));
        }
        
        /*Update Advertised PDO count*/
        gasCfgStatusData.sPerPortData[u8PortNum].u8AdvertisedPDOCnt = u8AdvertisedPDOCnt;
#endif
    }

}

/******************************************************************************/
#if (TRUE == INCLUDE_PD_SINK)
/****************************** DPM Sink related APIs*****************************************/
/**************************************************************************************************/
void DPM_GetSinkCapabilities(UINT8 u8PortNum,UINT8 *u8pSinkPDOCnt, UINT32 * pu32DataObj)
{   
    /* Get Sink Capability from Port Configuration Data Structure */
    *u8pSinkPDOCnt = gasCfgStatusData.sPerPortData[u8PortNum].u8AdvertisedPDOCnt;
    
        (void)MCHP_PSF_HOOK_MEMCPY ( pu32DataObj, gasCfgStatusData.sPerPortData[u8PortNum].u32aAdvertisedPDO, \
                            DPM_4BYTES_FOR_EACH_PDO_OF(gasCfgStatusData.sPerPortData[u8PortNum].u8AdvertisedPDOCnt));
}

void DPM_CalculateAndSortPower(UINT8 u8PDOCount, UINT32 *pu32CapsPayload, UINT8 u8Power[][2], UINT8 u8SinkMode)
{
    UINT8 u8PowerSwap = SET_TO_ZERO;
    UINT8 u8PowerSwapIndex = SET_TO_ZERO;
    UINT8 u8PowerIndex;
    UINT8 u8PDOIndex;
    UINT32 u32PDO;
  
    /* Calculating and storing src power from minimum to maximum */
    for(u8PDOIndex = SET_TO_ZERO; u8PDOIndex < u8PDOCount; u8PDOIndex++)
    {
        u32PDO = pu32CapsPayload[u8PDOIndex];
        
        u8Power[u8PDOIndex][DPM_PDO_PWR] = (UINT8)((float)(DPM_GET_PDO_VOLTAGE(u32PDO) * DPM_PDO_VOLTAGE_UNIT/(float)DPM_1000mA) *\
                                            ((float)((float)(DPM_GET_PDO_CURRENT(u32PDO) * DPM_PDO_CURRENT_UNIT) /(float)DPM_1000mA)));
 
        u8Power[u8PDOIndex][DPM_PDO_INDEX] = u8PDOIndex;
    }
    
    for(u8PDOIndex = SET_TO_ZERO; u8PDOIndex < u8PDOCount; u8PDOIndex++)
    {          
        for(u8PowerIndex = SET_TO_ZERO; u8PowerIndex < (u8PDOCount - u8PDOIndex - BYTE_LEN_1); u8PowerIndex++)
        {
            if(DPM_SINK_MODE_A == u8SinkMode)
            {
                if(u8Power[u8PowerIndex][DPM_PDO_PWR] <= u8Power[u8PowerIndex + BYTE_LEN_1][DPM_PDO_PWR])
                {
                   u8PowerSwap = u8Power[u8PowerIndex][DPM_PDO_PWR];
                   u8PowerSwapIndex = u8Power[u8PowerIndex][DPM_PDO_INDEX];
                   u8Power[u8PowerIndex][DPM_PDO_PWR] = u8Power[DPM_NEXT_PWR_INDEX(u8PowerIndex)][DPM_PDO_PWR];
                   u8Power[u8PowerIndex][DPM_PDO_INDEX] = u8Power[DPM_NEXT_PWR_INDEX(u8PowerIndex)][DPM_PDO_INDEX];
                   u8Power[DPM_NEXT_PWR_INDEX(u8PowerIndex)][DPM_PDO_PWR] = u8PowerSwap; 
                   u8Power[DPM_NEXT_PWR_INDEX(u8PowerIndex)][DPM_PDO_INDEX] = u8PowerSwapIndex;
                }
            }
            else if (DPM_SINK_MODE_B == u8SinkMode)
            {
                if(u8Power[u8PowerIndex][DPM_PDO_PWR] < u8Power[u8PowerIndex + BYTE_LEN_1][DPM_PDO_PWR])
                {
                   u8PowerSwap = u8Power[u8PowerIndex][DPM_PDO_PWR];
                   u8PowerSwapIndex = u8Power[u8PowerIndex][DPM_PDO_INDEX];
                   u8Power[u8PowerIndex][DPM_PDO_PWR] = u8Power[DPM_NEXT_PWR_INDEX(u8PowerIndex)][DPM_PDO_PWR];
                   u8Power[u8PowerIndex][DPM_PDO_INDEX] = u8Power[DPM_NEXT_PWR_INDEX(u8PowerIndex)][DPM_PDO_INDEX];
                   u8Power[DPM_NEXT_PWR_INDEX(u8PowerIndex)][DPM_PDO_PWR] = u8PowerSwap; 
                   u8Power[DPM_NEXT_PWR_INDEX(u8PowerIndex)][DPM_PDO_INDEX] = u8PowerSwapIndex;
                }
                else if(u8Power[u8PowerIndex][DPM_PDO_PWR] == u8Power[u8PowerIndex + BYTE_LEN_1][DPM_PDO_PWR]) 
                {
                    if(DPM_GET_PDO_VOLTAGE(pu32CapsPayload[u8PowerIndex]) > DPM_GET_PDO_VOLTAGE(pu32CapsPayload[u8PowerIndex + 1]))
                    {
                        u8PowerSwap = u8Power[u8PowerIndex][DPM_PDO_PWR];
                        u8PowerSwapIndex = u8Power[u8PowerIndex][DPM_PDO_INDEX];
                        u8Power[u8PowerIndex][DPM_PDO_PWR] = u8Power[DPM_NEXT_PWR_INDEX(u8PowerIndex)][DPM_PDO_PWR];
                        u8Power[u8PowerIndex][DPM_PDO_INDEX] = u8Power[DPM_NEXT_PWR_INDEX(u8PowerIndex)][DPM_PDO_INDEX];
                        u8Power[DPM_NEXT_PWR_INDEX(u8PowerIndex)][DPM_PDO_PWR] = u8PowerSwap; 
                        u8Power[DPM_NEXT_PWR_INDEX(u8PowerIndex)][DPM_PDO_INDEX] = u8PowerSwapIndex;
                    }
                }
                else
                {
                    /* Do Nothing */
                }
            }  
            else
            {
                /* Do Nothing */
            }
        }
    }
}

void DPM_EvaluateReceivedSrcCaps(UINT8 u8PortNum ,UINT16 u16RecvdSrcCapsHeader,
                                     UINT32 *pu32RecvdSrcCapsPayload)
{
    /* Two dimensional array stores the Power and Original PDO index number for each PDO index*/
    UINT8 u8SrcPower[DPM_MAX_PDO_CNT][DPM_2_DIMENSION_TO_STORE_INDEX_AND_PWR] = {SET_TO_ZERO};
    UINT8 u8SinkPower[DPM_MAX_PDO_CNT][DPM_2_DIMENSION_TO_STORE_INDEX_AND_PWR] = {SET_TO_ZERO};
    UINT8 u8SrcIndex = SET_TO_ZERO;
    UINT8 u8SinkIndex = SET_TO_ZERO;
    UINT8 u8CapMismatch = FALSE;   
    UINT8 u8SinkAdvertisedPDOCnt;
    UINT8 u8PartnerPDOCnt;
    UINT32 u32aPartnerPDO[DPM_MAX_PDO_CNT];
    UINT32 u32RcvdSrcPDO;
    UINT8 u8SrcPDOIndex;
    UINT8 u8SinkAdvertisedPDOIndex;
    UINT32 u32SinkAdvertisedPDO;
    UINT16 u16MinPDOPreferredCurIn10mA;
    UINT16 u16SinkRDOCurIn10mA;
    UINT8 u8SinkMode = (gasCfgStatusData.sPerPortData[u8PortNum].u8SinkConfigSel & DPM_SINK_CONFIG_SINK_MODE_SEL_MASK);
    UINT8 u8SinkGiveBackFlag = ((gasCfgStatusData.sPerPortData[u8PortNum].u8SinkConfigSel & \
            DPM_SINK_CONFIG_GIVE_BACK_FLAG_MASK) >> DPM_SINK_CONFIG_GIVE_BACK_FLAG_POS);
    UINT8 u8SinkNoUSBSusp = ((gasCfgStatusData.sPerPortData[u8PortNum].u8SinkConfigSel & \
            DPM_SINK_CONFIG_NO_USB_SUSP_MASK) >> DPM_SINK_CONFIG_NO_USB_SUSP_POS);
    
    if(DPM_INT_EVT_INITIATE_RENEGOTIATION != gasDPM[u8PortNum].u16InternalEvntInProgress)
    {
        /*PDO Count of the source derived from received src caps*/
        UINT8 u8RcvdSrcPDOCnt =  PRL_GET_OBJECT_COUNT(u16RecvdSrcCapsHeader);
    
        /*Clear partner pdo variable initially*/
        for(UINT8 u8Index = SET_TO_ZERO; u8Index < DPM_MAX_PDO_CNT; u8Index++)
        {
            gasCfgStatusData.sPerPortData[u8PortNum].u32aPartnerPDO[u8Index] = SET_TO_ZERO;
        }

        /*Update received source capabilities to partner variable*/
        (void)MCHP_PSF_HOOK_MEMCPY(gasCfgStatusData.sPerPortData[u8PortNum].u32aPartnerPDO, 
                                        pu32RecvdSrcCapsPayload, DPM_4BYTES_FOR_EACH_PDO_OF(u8RcvdSrcPDOCnt));
        gasCfgStatusData.sPerPortData[u8PortNum].u8PartnerPDOCnt = u8RcvdSrcPDOCnt;
    }
    
    u8PartnerPDOCnt = gasCfgStatusData.sPerPortData[u8PortNum].u8PartnerPDOCnt;
    /*Update received source capabilities to partner variable*/
    (void)MCHP_PSF_HOOK_MEMCPY(u32aPartnerPDO, gasCfgStatusData.sPerPortData[u8PortNum].u32aPartnerPDO, 
            DPM_4BYTES_FOR_EACH_PDO_OF(u8PartnerPDOCnt));
        
    DPM_UpdateAdvertisedPDOParam(u8PortNum);
    
    /*PDO Count of the sink*/
	u8SinkAdvertisedPDOCnt = gasCfgStatusData.sPerPortData[u8PortNum].u8AdvertisedPDOCnt;
    
    /* Calculate and sort the power of Sink PDOs */
    DPM_CalculateAndSortPower(u8SinkAdvertisedPDOCnt, gasCfgStatusData.sPerPortData[u8PortNum].u32aAdvertisedPDO, u8SinkPower, u8SinkMode);
    
    /* Calculate and sort the received source PDOs power */
    DPM_CalculateAndSortPower(u8PartnerPDOCnt, u32aPartnerPDO, u8SrcPower, u8SinkMode);
    
    /* Compare Maximum power sink PDO to received source PDOs */
    /* Storing PDO index of the Sink PDO which has maximum power.*/
    u8SinkAdvertisedPDOIndex = u8SinkPower[DPM_PDO_WITH_MAX_PWR][DPM_PDO_INDEX];
    /* Storing Sink PDO which has maximum power*/
    u32SinkAdvertisedPDO = gasCfgStatusData.sPerPortData[u8PortNum].u32aAdvertisedPDO[u8SinkAdvertisedPDOIndex];
    
    for(u8SrcIndex = SET_TO_ZERO; u8SrcIndex < u8PartnerPDOCnt; u8SrcIndex++)
    {
        /* Comparing whether any of Source power is greater than or 
         * equal to maximum Sink power*/
        if(u8SrcPower[u8SrcIndex][DPM_PDO_PWR] >= u8SinkPower[DPM_PDO_WITH_MAX_PWR][DPM_PDO_PWR])
        {
            u8SrcPDOIndex = u8SrcPower[u8SrcIndex][DPM_PDO_INDEX];
            u32RcvdSrcPDO = u32aPartnerPDO[u8SrcPDOIndex];
            if((DPM_GET_PDO_VOLTAGE(u32RcvdSrcPDO) == DPM_GET_PDO_VOLTAGE(u32SinkAdvertisedPDO)) && \
                (DPM_GET_PDO_CURRENT(u32RcvdSrcPDO)) >= DPM_GET_PDO_CURRENT(u32SinkAdvertisedPDO))
            {
                if(u8SinkGiveBackFlag)
                {
                    /*if go to min supported Form request message with minimum current */
                    gasCfgStatusData.sPerPortData[u8PortNum].u32RDO = \
                            DPM_FORM_DATA_REQUEST((u8SrcPDOIndex + BYTE_LEN_1), u8CapMismatch,
                            u8SinkGiveBackFlag, DPM_GET_PDO_USB_COMM_CAP(u32SinkAdvertisedPDO),\
                            u8SinkNoUSBSusp, DPM_GET_PDO_CURRENT(u32SinkAdvertisedPDO),\
                           (gasCfgStatusData.sPerPortData[u8PortNum].u16SnkMinOperatingCurInmA/DPM_10mA));
                }
                else
                {
                    /* Form request message */
                    gasCfgStatusData.sPerPortData[u8PortNum].u32RDO = \
                            DPM_FORM_DATA_REQUEST((u8SrcPDOIndex + BYTE_LEN_1), u8CapMismatch,
                            u8SinkGiveBackFlag, DPM_GET_PDO_USB_COMM_CAP(u32SinkAdvertisedPDO),\
                            u8SinkNoUSBSusp, DPM_GET_PDO_CURRENT(u32SinkAdvertisedPDO),\
                                                DPM_GET_PDO_CURRENT(u32SinkAdvertisedPDO));
                }
           
                gasDPM[u8PortNum].u16SinkOperatingCurrInmA = DPM_GET_PDO_CURRENT(u32SinkAdvertisedPDO)*DPM_10mA;
                /*Updating the globals with Sink PDO selected */
                gasDPM[u8PortNum].u32NegotiatedPDO = u32SinkAdvertisedPDO;
                /*Updating the globals for Sink */
                gasDPM[u8PortNum].u8NegotiatedPDOIndex = u8SinkAdvertisedPDOIndex+1; 
                /*Update Negotiated value*/
                gasCfgStatusData.sPerPortData[u8PortNum].u16NegoVoltageInmV = (DPM_GET_PDO_VOLTAGE(u32SinkAdvertisedPDO) * DPM_PDO_VOLTAGE_UNIT);
                /*Update Negotiated value*/
                gasCfgStatusData.sPerPortData[u8PortNum].u16NegoCurrentInmA = (DPM_GET_PDO_CURRENT(u32SinkAdvertisedPDO) * DPM_10mA);
                /*VBUS Threshold are configured for the requested PDO*/
                DPM_SetPortPower (u8PortNum);                
                return;
            }
        }
    }
    
    /* Check DPM whether minimum PDO current matches*/
    for(u8SrcIndex = SET_TO_ZERO; u8SrcIndex < u8PartnerPDOCnt; u8SrcIndex++)
    {
        u8SrcPDOIndex = u8SrcPower[u8SrcIndex][DPM_PDO_INDEX];
        u32RcvdSrcPDO = u32aPartnerPDO[u8SrcPDOIndex];
        for(u8SinkIndex = SET_TO_ZERO; u8SinkIndex < u8SinkAdvertisedPDOCnt; u8SinkIndex++)
        {
            u8SinkAdvertisedPDOIndex = u8SinkPower[u8SinkIndex][DPM_PDO_INDEX];
            u32SinkAdvertisedPDO = gasCfgStatusData.sPerPortData[u8PortNum].u32aAdvertisedPDO[u8SinkAdvertisedPDOIndex];
            u16MinPDOPreferredCurIn10mA = (gasCfgStatusData.sPerPortData[u8PortNum].u16aMinPDOPreferredCurInmA[u8SinkAdvertisedPDOIndex] /10);
    
            if((DPM_GET_PDO_VOLTAGE(u32RcvdSrcPDO) == DPM_GET_PDO_VOLTAGE(u32SinkAdvertisedPDO))
                    && (DPM_GET_PDO_CURRENT(u32RcvdSrcPDO) >= u16MinPDOPreferredCurIn10mA))
            {
                if(DPM_GET_PDO_CURRENT(u32RcvdSrcPDO) > DPM_GET_PDO_CURRENT(u32SinkAdvertisedPDO))
                {
                    u16SinkRDOCurIn10mA = DPM_GET_PDO_CURRENT(u32SinkAdvertisedPDO);
                }
                else
                {
                    //u16SinkRDOCurIn10mA will be greater than or equal to u16SinkRDOCurIn10mA
                    u16SinkRDOCurIn10mA = DPM_GET_PDO_CURRENT(u32RcvdSrcPDO);
                }
                if(u8SinkGiveBackFlag)
                {
                    /*if go to min supported Form request message with minimum current */
                    /* Form Request message with Capability Mismatch*/
                    gasCfgStatusData.sPerPortData[u8PortNum].u32RDO = \
                            DPM_FORM_DATA_REQUEST((u8SrcPDOIndex + BYTE_LEN_1), u8CapMismatch,
                            u8SinkGiveBackFlag, DPM_GET_PDO_USB_COMM_CAP(u32SinkAdvertisedPDO),\
                            u8SinkNoUSBSusp, u16SinkRDOCurIn10mA,\
                            (gasCfgStatusData.sPerPortData[u8PortNum].u16SnkMinOperatingCurInmA/DPM_10mA));      
                }
                else
                {
                     /* Form Request message with Capability Mismatch*/
                    gasCfgStatusData.sPerPortData[u8PortNum].u32RDO = \
                            DPM_FORM_DATA_REQUEST((u8SrcPDOIndex + BYTE_LEN_1), u8CapMismatch,
                            u8SinkGiveBackFlag, DPM_GET_PDO_USB_COMM_CAP(u32SinkAdvertisedPDO),\
                            u8SinkNoUSBSusp, u16SinkRDOCurIn10mA,\
                                                DPM_GET_PDO_CURRENT(u32SinkAdvertisedPDO));
                }
                gasDPM[u8PortNum].u16SinkOperatingCurrInmA = u16SinkRDOCurIn10mA * DPM_10mA;
                /*Updating the globals with Sink PDO selected */
                gasDPM[u8PortNum].u32NegotiatedPDO = u32RcvdSrcPDO;
                /*Updating the globals with Sink PDO index selected*/
                gasDPM[u8PortNum].u8NegotiatedPDOIndex = (u8SinkAdvertisedPDOIndex+1);
                /*Update Negotiated value*/
                 gasCfgStatusData.sPerPortData[u8PortNum].u16NegoVoltageInmV = (DPM_GET_PDO_VOLTAGE(u32SinkAdvertisedPDO) * DPM_PDO_VOLTAGE_UNIT);
                /*Update Negotiated value*/
                 gasCfgStatusData.sPerPortData[u8PortNum].u16NegoCurrentInmA = (u16SinkRDOCurIn10mA * DPM_10mA);              
                
                 DPM_SetPortPower (u8PortNum);
                
                return;
            }
        }
    }
    
    /*  None of the capability matches thus capability mismatch bit is set*/
    u8CapMismatch = TRUE;   
    for(u8SrcIndex = SET_TO_ZERO; u8SrcIndex < u8PartnerPDOCnt; u8SrcIndex++)
    {
        u8SrcPDOIndex = u8SrcPower[u8SrcIndex][DPM_PDO_INDEX];
        u32RcvdSrcPDO = u32aPartnerPDO[u8SrcPDOIndex];
        
        for(u8SinkIndex = SET_TO_ZERO; u8SinkIndex < u8SinkAdvertisedPDOCnt; u8SinkIndex++)
        {
            u8SinkAdvertisedPDOIndex = u8SinkPower[u8SinkIndex][DPM_PDO_INDEX];
            u32SinkAdvertisedPDO = gasCfgStatusData.sPerPortData[u8PortNum].u32aAdvertisedPDO[u8SinkAdvertisedPDOIndex];
            
            if((DPM_GET_PDO_VOLTAGE(u32RcvdSrcPDO)) == DPM_GET_PDO_VOLTAGE(u32SinkAdvertisedPDO))
            {
                if(u8SinkGiveBackFlag)
                {
                    /*if go to min supported Form request message with minimum current */
                    /* Form Request message with Capability Mismatch*/
                    gasCfgStatusData.sPerPortData[u8PortNum].u32RDO = \
                            DPM_FORM_DATA_REQUEST((u8SrcPDOIndex + BYTE_LEN_1), u8CapMismatch,
                            u8SinkGiveBackFlag, DPM_GET_PDO_USB_COMM_CAP(u32SinkAdvertisedPDO),\
                            u8SinkNoUSBSusp, DPM_GET_PDO_CURRENT(u32RcvdSrcPDO),\
                            (gasCfgStatusData.sPerPortData[u8PortNum].u16SnkMinOperatingCurInmA/DPM_10mA));      
                }
                else
                {
                     /* Form Request message with Capability Mismatch*/
                    gasCfgStatusData.sPerPortData[u8PortNum].u32RDO = \
                            DPM_FORM_DATA_REQUEST((u8SrcPDOIndex + BYTE_LEN_1), u8CapMismatch,
                            u8SinkGiveBackFlag, DPM_GET_PDO_USB_COMM_CAP(u32SinkAdvertisedPDO),\
                            u8SinkNoUSBSusp, DPM_GET_PDO_CURRENT(u32RcvdSrcPDO),\
                                                DPM_GET_PDO_CURRENT(u32SinkAdvertisedPDO));
                }
                gasDPM[u8PortNum].u16SinkOperatingCurrInmA = DPM_GET_PDO_CURRENT(u32RcvdSrcPDO) * DPM_10mA;
                
                /*Updating the globals with Sink PDO selected */
                gasDPM[u8PortNum].u32NegotiatedPDO = u32RcvdSrcPDO;
                /*Updating the globals with Sink PDO index selected*/
                gasDPM[u8PortNum].u8NegotiatedPDOIndex = (u8SinkAdvertisedPDOIndex + BYTE_LEN_1);
   
                /*Update Negotiated value*/
                gasCfgStatusData.sPerPortData[u8PortNum].u16NegoVoltageInmV = (DPM_GET_PDO_VOLTAGE(u32RcvdSrcPDO) * DPM_PDO_VOLTAGE_UNIT);
                /*Update Negotiated value*/
                gasCfgStatusData.sPerPortData[u8PortNum].u16NegoCurrentInmA = (DPM_GET_PDO_CURRENT(u32RcvdSrcPDO) * DPM_10mA);
                
                /*Set the capability mismatch status*/
                gasCfgStatusData.sPerPortData[u8PortNum].u32PortConnectStatus |= DPM_PORT_SINK_CAPABILITY_MISMATCH_STATUS;
                
                DPM_SetPortPower(u8PortNum);                
                return;
            }
        }
    }
          
}
#endif

/************************  DPM Renegotiation APIs **********************/
#if (TRUE == INCLUDE_POWER_THROTTLING || (TRUE == INCLUDE_POWER_BALANCING))

void DPM_UpdateNewPDOFrmSrcPwr(UINT8 u8PortNum, UINT16 u16PowerIn250mW)
{
    float fVoltageInmV = SET_TO_ZERO; 
    UINT16 u16CurrentIn10mA = SET_TO_ZERO; 
    
    /* Load the New PDO Count which is same as Fixed PDO count */
    gasCfgStatusData.sPerPortData[u8PortNum].u8NewSourcePDOCnt = gasCfgStatusData.sPerPortData[u8PortNum].u8SourcePDOCnt; 
    
    for (UINT8 u8Index = INDEX_0; u8Index < gasCfgStatusData.sPerPortData[u8PortNum].u8NewSourcePDOCnt; u8Index++)
    {
        /* Get the voltage value from 32aSourcePDO[7] */
        fVoltageInmV = DPM_GET_VOLTAGE_FROM_PDO_MILLI_V(gasCfgStatusData.sPerPortData[u8PortNum].u32aSourcePDO[u8Index]); 
        
        /* Calculate new current value based on new power */
        u16CurrentIn10mA = ROUND_OFF_FLOAT_TO_INT((float)(((float)u16PowerIn250mW / fVoltageInmV) * (DPM_250mW / DPM_PDO_CURRENT_UNIT))); 
          
        /* Current value of a port should not exceed PORT_MAX_I */ 
        u16CurrentIn10mA = MIN(u16CurrentIn10mA, gasCfgStatusData.sPerPortData[u8PortNum].u16MaxSrcPrtCurrentIn10mA); 
        
        /* Load the New PDO registers with the new PDO values */
        gasCfgStatusData.sPerPortData[u8PortNum].u32aNewSourcePDO[u8Index] = \
                    (gasCfgStatusData.sPerPortData[u8PortNum].u32aSourcePDO[u8Index] & ~(DPM_PDO_CURRENT_MASK)) | u16CurrentIn10mA;  
    }
}

#endif 
/*********************************************************************************/
/***************DPM Common APIs***************************************************/
/*******************************************************************************/
/********************* DPM API for Port Enable/Disable ******************/
void DPM_EnablePort(UINT8 u8PortNum, UINT8 u8Enable)
{
    UINT8 u8ConfiguredRpVal;
    
    if (FALSE == u8Enable)
    {
        /* Disable the port by changing Type C states to 
           TYPEC_DISABLED and TYPEC_DISABLED_ENTRY_SS */
        DPM_SetTypeCState(u8PortNum, TYPEC_DISABLED, TYPEC_DISABLED_ENTRY_SS);
        
        /* Change Policy Engine state and sub-state to invalid state */
        gasPolicyEngine[u8PortNum].ePEState = ePE_INVALIDSTATE;
        gasPolicyEngine[u8PortNum].ePESubState = ePE_INVALIDSUBSTATE; 
    }
    else
    {
        if ((TYPEC_DISABLED == gasTypeCcontrol[u8PortNum].u8TypeCState) && \
                (TYPEC_DISABLED_DONE_SS == gasTypeCcontrol[u8PortNum].u8TypeCSubState))
        {
            /*Reset protocol layer*/
            PRL_Init(u8PortNum);
             
            /*Clear Policy Engine internal variables*/
            PE_InitPort(u8PortNum);
            
            if (PD_ROLE_SOURCE == DPM_GET_DEFAULT_POWER_ROLE(u8PortNum))
            {
                u8ConfiguredRpVal = DPM_GET_CONFIGURED_SOURCE_RP_VAL(u8PortNum);
                
                /*Setting the CC1 and CC2 line as user given Rp value*/
                TypeC_SetCCPowerRole (u8PortNum, PD_ROLE_SOURCE, u8ConfiguredRpVal, TYPEC_ENABLE_CC1_CC2);
                
                /*Setting the Current Rp value status in u8PortSts variable as user given Rp value*/
                gasTypeCcontrol[u8PortNum].u8PortSts &= ~TYPEC_CURR_RPVAL_MASK;
                gasTypeCcontrol[u8PortNum].u8PortSts |= (u8ConfiguredRpVal << TYPEC_CURR_RPVAL_POS);
            
#if(TRUE == INCLUDE_PD_SOURCE)                           
                /*Enable DC_DC EN on VBUS fault to reset the DC-DC controller*/
                PWRCTRL_ConfigDCDCEn(u8PortNum, TRUE);
#endif                        
                #if (TRUE == INCLUDE_UPD_PIO_OVERRIDE_SUPPORT)
                /*Enable PIO override enable*/
                UPD_RegByteSetBit (u8PortNum, UPD_PIO_OVR_EN,  UPD_PIO_OVR_2);
                #endif

                DPM_SetTypeCState(u8PortNum, TYPEC_UNATTACHED_SRC, TYPEC_UNATTACHED_SRC_ENTRY_SS);  

            }
            else if (PD_ROLE_SINK == DPM_GET_DEFAULT_POWER_ROLE(u8PortNum))
            {
                /*Setting the CC1 and CC2 line as sink Rd*/
                TypeC_SetCCPowerRole (u8PortNum,PD_ROLE_SINK, TYPEC_ROLE_SINK_RD, TYPEC_ENABLE_CC1_CC2);                        
                
                gasDPM[u8PortNum].u16SinkOperatingCurrInmA = DPM_0mA;
                    
	            TypeC_ConfigureVBUSThr(u8PortNum, TYPEC_VBUS_5V,
	                    gasDPM[u8PortNum].u16SinkOperatingCurrInmA , TYPEC_CONFIG_NON_PWR_FAULT_THR);
            
				#if (TRUE == INCLUDE_PD_SINK)
	            /*Disable the Sink circuitry to stop sinking the power from source*/
	            PWRCTRL_ConfigSinkHW(u8PortNum, TYPEC_VBUS_0V, gasDPM[u8PortNum].u16SinkOperatingCurrInmA);
	            #endif

                /*Setting CC Comparator ON*/
                TypeC_ConfigCCComp (u8PortNum, TYPEC_CC_COMP_CTL_CC1_CC2);   
    
	            /*Enabling the VBUS discharge functionality for VBUS to go to Vsafe0V*/                  
	            PWRCTRL_ConfigVBUSDischarge (u8PortNum, TRUE);
                
                DPM_SetTypeCState(u8PortNum, TYPEC_UNATTACHED_SNK, TYPEC_UNATTACHED_SNK_ENTRY_SS);
            }
            else /*Power role is DRP*/
            {
#if (TRUE == INCLUDE_PD_DRP)

                TypeC_InitDRPPort(u8PortNum);
                      
                /*Set power and data role status*/
                DPM_UpdatePowerRole(u8PortNum, PD_ROLE_DRP);
                DPM_UpdateDataRole(u8PortNum, PD_ROLE_TOGGLING);
#endif
            }            
        }
        else
        {
            /* Do nothing since the port shall only be enabled if it was 
            previously in the disabled state */ 
        }

    }
}

/*********************************DPM PD negotiation API**************************************/
void DPM_OnPDNegotiationCmplt(UINT8 u8PortNum)
{
    
#if ((TRUE == INCLUDE_PD_VCONN_SWAP) || (TRUE == INCLUDE_PD_DR_SWAP) || (TRUE == INCLUDE_PD_PR_SWAP))
    UINT16 u16DPMStatus = gasDPM[u8PortNum].u16DPMStatus;
#endif  
#if (TRUE == INCLUDE_PD_SOURCE)
    /*On negotiation, initiate Get Sink caps if Get Sink is not initiated already
     and Partner PDO is null; In case of PB enabled, Get Sink caps is initiated 
     by PB layer*/
    if ((!gasCfgStatusData.sPerPortData[u8PortNum].u32aPartnerPDO[INDEX_0]) &&\
          (PD_ROLE_SOURCE == DPM_GET_CURRENT_POWER_ROLE(u8PortNum)) &&\
            (TRUE != DPM_IS_PB_ENABLED(u8PortNum)))
    {
        DPM_RegisterInternalEvent(u8PortNum, DPM_INT_EVT_INITIATE_GET_SINK_CAPS);
    }        
#endif
    /*Evaluate swap and register internal event*/
#if (TRUE == INCLUDE_PD_VCONN_SWAP)
    /*Initiate VCONN Swap only if the swap is not already initiated and rejected*/
    if (!(((TRUE == DPM_IsPortVCONNSource(u8PortNum)) && 
                            (u16DPMStatus& DPM_VCONN_SWAP_REJ_STS_AS_VCONNSRC)) ||
               ((FALSE == DPM_IsPortVCONNSource(u8PortNum)) && 
                            (u16DPMStatus & DPM_VCONN_SWAP_REJ_STS_AS_NOT_VCONNSRC))))
    {
        if (DPM_REQUEST_SWAP == DPM_EvaluateRoleSwap (u8PortNum, eVCONN_SWAP_INITIATE))
        {
            DPM_RegisterInternalEvent(u8PortNum, DPM_INT_EVT_INITIATE_VCONN_SWAP);
        }
    }
#endif
#if (TRUE == INCLUDE_PD_DR_SWAP)
    /*Initiate DR Swap only if the swap is not already initiated and rejected*/
    if (!(((PD_ROLE_DFP == DPM_GET_CURRENT_DATA_ROLE(u8PortNum)) && \
                            (u16DPMStatus& DPM_DR_SWAP_REJ_STS_AS_DFP)) ||\
               ((PD_ROLE_UFP == DPM_GET_CURRENT_DATA_ROLE(u8PortNum)) && \
                            (u16DPMStatus & DPM_DR_SWAP_REJ_STS_AS_UFP))))
    {
        if (DPM_REQUEST_SWAP == DPM_EvaluateRoleSwap (u8PortNum, eDR_SWAP_INITIATE))
        {
            DPM_RegisterInternalEvent(u8PortNum, DPM_INT_EVT_INITIATE_DR_SWAP);
        }
    }
#endif /*INCLUDE_PD_DR_SWAP*/
    
#if (TRUE == INCLUDE_PD_PR_SWAP)
    if (!(((PD_ROLE_SOURCE == DPM_GET_CURRENT_POWER_ROLE(u8PortNum)) && \
                            (u16DPMStatus& DPM_PR_SWAP_REJ_STS_AS_SRC)) ||\
               ((PD_ROLE_SINK == DPM_GET_CURRENT_POWER_ROLE(u8PortNum)) && \
                            (u16DPMStatus & DPM_PR_SWAP_REJ_STS_AS_SNK))))
    {
        if (DPM_REQUEST_SWAP == DPM_EvaluateRoleSwap (u8PortNum, ePR_SWAP_INITIATE))
        {
            DPM_RegisterInternalEvent(u8PortNum, DPM_INT_EVT_INITIATE_PR_SWAP);
        }
    }
#endif /*INCLUDE_PD_PR_SWAP*/
}

/*********************************DPM TypeC Detach API**************************************/
void DPM_OnTypeCDetach(UINT8 u8PortNum)
{
    #if (TRUE == INCLUDE_PD_DR_SWAP)
    
    /* If the port is configured as either Source/DFP or Sink/UFP with 
       DR_SWAP enabled restore the Data Role on detach */
    if (DPM_GET_DEFAULT_DATA_ROLE(u8PortNum) != PD_ROLE_TOGGLING)
    {
        DPM_UpdateDataRole(u8PortNum, DPM_GET_DEFAULT_DATA_ROLE(u8PortNum));
    }
    #endif
    
    gasDPM[u8PortNum].u8VCONNErrCounter = SET_TO_ZERO;
    
    /* Clear the DPM variables whose data is no more valid after a Type C detach */
    gasDPM[u8PortNum].u8NegotiatedPDOIndex = RESET_TO_ZERO;
    gasDPM[u8PortNum].u32NegotiatedPDO = RESET_TO_ZERO;
    gasDPM[u8PortNum].u16PrevVBUSVoltageInmV = RESET_TO_ZERO;
            
    /* Clear the RDO register */
    gasCfgStatusData.sPerPortData[u8PortNum].u32RDO = RESET_TO_ZERO; 
    
    /* Clear the bits that are not valid during detached state in 
       Port Connection Status register */
    gasCfgStatusData.sPerPortData[u8PortNum].u32PortConnectStatus &= 
        ~(DPM_PORT_ATTACHED_STATUS | DPM_PORT_AS_SRC_PD_CONTRACT_GOOD_STATUS | \
        DPM_PORT_AS_SRC_RDO_ACCEPTED_STATUS | DPM_PORT_ORIENTATION_FLIPPED_STATUS | \
        DPM_PORT_CABLE_REDUCED_SRC_CAPABILITIES_STATUS | DPM_PORT_SRC_CAPABILITIES_REDUCED_STATUS |\
        DPM_PORT_SRC_CAPABILITY_MISMATCH_STATUS | DPM_PORT_AS_SRC_RDO_REJECTED_STATUS| \
        DPM_PORT_AS_SNK_LAST_REQ_ACCEPT_STATUS | DPM_PORT_AS_SNK_LAST_REQ_REJECT_STATUS| \
        DPM_PORT_AS_SNK_LAST_REQ_PS_RDY_STATUS | DPM_PORT_SINK_CAPABILITY_MISMATCH_STATUS| \
        DPM_PORT_RP_VAL_DETECT_MASK_STATUS);
                    
    /*Clear Partner PDO, Advertised PDO, Cable Identity and Partner Identity registers */
    for(UINT8 u8Index = SET_TO_ZERO; u8Index < DPM_MAX_PDO_CNT; u8Index++)
    {
        gasCfgStatusData.sPerPortData[u8PortNum].u32aPartnerPDO[u8Index] = RESET_TO_ZERO;
        gasCfgStatusData.sPerPortData[u8PortNum].u32aAdvertisedPDO[u8Index] = RESET_TO_ZERO;        
    }
    gasCfgStatusData.sPerPortData[u8PortNum].u8PartnerPDOCnt = RESET_TO_ZERO; 
    gasCfgStatusData.sPerPortData[u8PortNum].u8AdvertisedPDOCnt = RESET_TO_ZERO; 
    
    #if (TRUE == INCLUDE_PD_SOURCE_PPS)
    /* Clear Partner Alert register */
    gasCfgStatusData.sPPSPerPortData[u8PortNum].u32PartnerAlert = RESET_TO_ZERO; 
    
    /*Clear Partner Status register */
    for(UINT8 u8Index = SET_TO_ZERO; u8Index < PE_STATUS_DATA_BLOCK_SIZE_IN_BYTES; u8Index++)
    {
        gasCfgStatusData.sPPSPerPortData[u8PortNum].u8aPartnerStatus[u8Index] = RESET_TO_ZERO;
    }
    
    /*Kill the DPM_STATUS_FAULT_PERSIST_TIMEOUT_MS timer*/
    PDTimer_Kill(gasDPM[u8PortNum].u8StsClearTmrID);
    /* Set the timer Id to Max Concurrent Value*/
    gasDPM[u8PortNum].u8StsClearTmrID = MAX_CONCURRENT_TIMERS;
    /* Note: It is recognized that it is possible to send an alert to another 
       partner if the current partner is disconnected and a new partner is
       connected. So, no need to clear the other variables - 
       gasDPM[u8PortNum].u8AlertType, gasDPM[u8PortNum].u8StatusEventFlags 
       and gasDPM[u8PortNum].u8RealTimeFlags */     
    #endif

    /*Clear all client requests except Port disable client request*/
    gasCfgStatusData.sPerPortData[u8PortNum].u32ClientRequest &= DPM_CLIENT_REQ_PORT_DISABLE;
    
    /*Clear all Internal events except Alert*/
    gasDPM[u8PortNum].u16DPMInternalEvents = (gasDPM[u8PortNum].u16DPMInternalEvents\
                                            & DPM_INT_EVT_INITIATE_ALERT);
    gasDPM[u8PortNum].u16InternalEvntInProgress = SET_TO_ZERO;
        
    gasDPM[u8PortNum].u16DPMStatus &= ~(DPM_SWAP_REJECT_STS_MASK | \
                                        DPM_PORT_IN_MODAL_OPERATION);    
    
    MCHP_PSF_HOOK_DISABLE_GLOBAL_INTERRUPT();
    gasPRL[u8PortNum].u8TxStsDPMSyncISR = FALSE;
    MCHP_PSF_HOOK_ENABLE_GLOBAL_INTERRUPT(); 
    
    /* Disable Orientation LED */
    MCHP_PSF_HOOK_GPIO_FUNC_INIT(u8PortNum, eORIENTATION_FUNC);

    #if (TRUE == INCLUDE_PD_SINK)
    gasCfgStatusData.sPerPortData[u8PortNum].u32PortIOStatus &=\
            ~DPM_PORT_IO_CAP_MISMATCH_STATUS;
    MCHP_PSF_HOOK_GPIO_FUNC_DRIVE(u8PortNum, eSNK_CAPS_MISMATCH_FUNC, eGPIO_DEASSERT);
    #endif

    /* Clear the VDM registers */    
    for(UINT8 u8Index = SET_TO_ZERO; u8Index < DPM_MAX_VDO_CNT; u8Index++)
    {
        #if (TRUE == INCLUDE_PD_SOURCE)
        gasCfgStatusData.sPerPortData[u8PortNum].u32aCableIdentity[u8Index] = RESET_TO_ZERO;
        #endif 
        #if (TRUE == INCLUDE_PD_VDM)
        gasCfgStatusData.sVDMPerPortData[u8PortNum].u32aPartnerPDIdentity[u8Index] = RESET_TO_ZERO;
        #endif
    }
    #if (TRUE == INCLUDE_PD_SOURCE)
    gasCfgStatusData.sPerPortData[u8PortNum].u8CableIdentityCnt = RESET_TO_ZERO;
    #endif

    #if (TRUE == INCLUDE_PD_VDM)
    gasCfgStatusData.sVDMPerPortData[u8PortNum].u32PartnerVDMHeader = RESET_TO_ZERO;
    gasCfgStatusData.sVDMPerPortData[u8PortNum].u8PartnerPDIdentityCnt = RESET_TO_ZERO;
    #endif      
}

/********************* DPM API to check if PPS APDO is advertised ********************/
UINT8 DPM_IsAPDOAdvertised(UINT8 u8PortNum)
{
    UINT8 u8RetVal = FALSE; 

    for (UINT8 u8Index = INDEX_0; u8Index < gasCfgStatusData.sPerPortData[u8PortNum].u8AdvertisedPDOCnt; u8Index++)
    {
        if (ePDO_PROGRAMMABLE == (ePDOType)DPM_GET_PDO_TYPE(gasCfgStatusData.sPerPortData[u8PortNum].u32aAdvertisedPDO[u8Index]))
        {
            u8RetVal = TRUE; 
            break;
        }
    }
    return u8RetVal; 
}

/********************* Policy Manager APIs for Swap ********************/
UINT8 DPM_EvaluateRoleSwap (UINT8 u8PortNum, eRoleSwapMsgType eRoleSwapMsg)
{
    UINT8 u8RetVal = DPM_REJECT_SWAP;
#if ((TRUE == INCLUDE_PD_VCONN_SWAP) || (TRUE == INCLUDE_PD_DR_SWAP) || (TRUE == INCLUDE_PD_PR_SWAP))
    UINT16 u16SwapPolicy = gasCfgStatusData.sPerPortData[u8PortNum].u16SwapPolicy;
#endif
    
#if (TRUE == INCLUDE_PD_VCONN_SWAP)
    UINT8 u8IsVCONNSource = DPM_IsPortVCONNSource(u8PortNum); 
    UINT8 u8IsRaCable = (gasTypeCcontrol[u8PortNum].u8PortSts & TYPEC_PWDCABLE_PRES_MASK);
#endif

#if ((TRUE == INCLUDE_PD_VCONN_SWAP) || (TRUE == INCLUDE_PD_PR_SWAP))
    UINT8 u8CurrentPwrRole = DPM_GET_CURRENT_POWER_ROLE(u8PortNum);
#endif
    
#if (TRUE == INCLUDE_PD_DR_SWAP)
    UINT8 u8CurrentDataRole = DPM_GET_CURRENT_DATA_ROLE(u8PortNum);
#endif    
    
    switch (eRoleSwapMsg)
    {
#if (TRUE == INCLUDE_PD_VCONN_SWAP)
        case eVCONN_SWAP_RCVD: 
        {
            /*Evaluate whether received VCONN_SWAP message can be accepted or rejected
             based on gasCfgStatusData.sPerPortData[u8PortNum].u16SwapPolicy configuration.
             Also the Swap shall be accepted only if powered cable is present and Port role
             is source */
            if ((((PD_ROLE_SOURCE == u8CurrentPwrRole) && u8IsRaCable) || 
                  (PD_ROLE_SINK == u8CurrentPwrRole))&&
#if (TRUE == INCLUDE_POWER_FAULT_HANDLING)
                ((TRUE == u8IsVCONNSource) || (gasDPM[u8PortNum].u8VCONNGoodtoSupply)) &&
#endif 
                (((TRUE == u8IsVCONNSource) && 
                            (u16SwapPolicy & DPM_AUTO_VCONN_SWAP_ACCEPT_AS_VCONN_SRC)) ||
                ((FALSE == u8IsVCONNSource) && 
                            (u16SwapPolicy & DPM_AUTO_VCONN_SWAP_ACCEPT_AS_NOT_VCONN_SRC))))
            {
                u8RetVal = DPM_ACCEPT_SWAP;
            }
            else
            {
                u8RetVal = DPM_REJECT_SWAP;
            }
            break;
            
        }
        case eVCONN_SWAP_INITIATE:
        {
            /*Evaluate whether to initiate VCONN_SWAP message 
            based on gasCfgStatusData.sPerPortData[u8PortNum].u16SwapPolicy configuration
            Also the Swap shall be accepted only if powered cable is present and Port role
            is source */
            if ((((PD_ROLE_SOURCE == u8CurrentPwrRole) && u8IsRaCable) || 
                  (PD_ROLE_SINK == u8CurrentPwrRole))&&
                (((TRUE == u8IsVCONNSource) && 
                           (u16SwapPolicy & DPM_AUTO_VCONN_SWAP_REQ_AS_VCONN_SRC)) ||
                ((FALSE == u8IsVCONNSource) && 
                           (u16SwapPolicy & DPM_AUTO_VCONN_SWAP_REQ_AS_NOT_VCONN_SRC))))
            {
                u8RetVal = DPM_REQUEST_SWAP;
            }
            else
            {
                u8RetVal = DPM_IGNORE_INITIATE_SWAP;
            }
            break; 
        }
#endif
#if (TRUE == INCLUDE_PD_DR_SWAP)
        case eDR_SWAP_RCVD:
        {     
            /*Evaluate whether received DR_SWAP message can be accepted or rejected
             based on gasCfgStatusData.sPerPortData[u8PortNum].u16SwapPolicy configuration*/
            if (((PD_ROLE_DFP == u8CurrentDataRole) && 
                            (u16SwapPolicy & DPM_AUTO_DR_SWAP_ACCEPT_AS_DFP)) ||
               ((PD_ROLE_UFP == u8CurrentDataRole) && 
                            (u16SwapPolicy & DPM_AUTO_DR_SWAP_ACCEPT_AS_UFP)))
            {
                u8RetVal = DPM_ACCEPT_SWAP;
            }
            else
            {
                u8RetVal = DPM_REJECT_SWAP;
            }
            break;   
        }
        case eDR_SWAP_INITIATE:
        {
            /*Evaluate whether to initiate DR_SWAP message 
                based on gasCfgStatusData.sPerPortData[u8PortNum].u16SwapPolicy configuration*/
               if (((PD_ROLE_DFP == u8CurrentDataRole) && 
                               (u16SwapPolicy & DPM_AUTO_DR_SWAP_REQ_AS_DFP)) ||
                  ((PD_ROLE_UFP == u8CurrentDataRole) && 
                               (u16SwapPolicy & DPM_AUTO_DR_SWAP_REQ_AS_UFP)))
               {
                   u8RetVal = DPM_REQUEST_SWAP;
               }
               else
               {
                   u8RetVal = DPM_IGNORE_INITIATE_SWAP;
               }
            break;
        }
#endif /*INCLUDE_PD_DR_SWAP*/
#if (TRUE == INCLUDE_PD_PR_SWAP)            
        case ePR_SWAP_RCVD:
        {
            /*Evaluate whether received PR_SWAP message can be accepted or rejected
             based on gasCfgStatusData.sPerPortData[u8PortNum].u16SwapPolicy configuration*/
            if (((PD_ROLE_SOURCE == u8CurrentPwrRole) && 
                            (u16SwapPolicy & DPM_AUTO_PR_SWAP_ACCEPT_AS_SRC)) ||
               ((PD_ROLE_SINK == u8CurrentPwrRole) && 
                            (u16SwapPolicy & DPM_AUTO_PR_SWAP_ACCEPT_AS_SNK)))
            {
                u8RetVal = DPM_ACCEPT_SWAP;
            }
            else
            {
                u8RetVal = DPM_REJECT_SWAP;
            }
            break;
            
        }
        case ePR_SWAP_INITIATE:
        {
            /*Evaluate whether to initiate PR_SWAP message 
            based on gasCfgStatusData.sPerPortData[u8PortNum].u16SwapPolicy configuration*/
            if (((PD_ROLE_SOURCE == u8CurrentPwrRole) && 
                           (u16SwapPolicy & DPM_AUTO_PR_SWAP_REQ_AS_SRC)) ||
              ((PD_ROLE_SINK == u8CurrentPwrRole) && 
                           (u16SwapPolicy & DPM_AUTO_PR_SWAP_REQ_AS_SNK)))
            {
                u8RetVal = DPM_REQUEST_SWAP;
            }
            else
            {
                u8RetVal = DPM_IGNORE_INITIATE_SWAP;
            }
            break; 
        }
#endif /*INCLUDE_PD_PR_SWAP*/
        default:
        {
            break; 
        }       
    }
    return u8RetVal; 
}

void DPM_SwapWait_TimerCB (UINT8 u8PortNum, UINT8 u8SwapInitiateType)
{
    switch (u8SwapInitiateType)
    {
#if (TRUE == INCLUDE_PD_VCONN_SWAP)
        case eVCONN_SWAP_INITIATE:
        {
            /* Set the timer Id to Max Concurrent Value*/
            gasDPM[u8PortNum].u8VCONNSwapWaitTmrID = MAX_CONCURRENT_TIMERS;
            break;
        }
#endif /*INCLUDE_PD_VCONN_SWAP*/
#if (TRUE == INCLUDE_PD_PR_SWAP)
        case ePR_SWAP_INITIATE:
        {
            /* Set the timer Id to Max Concurrent Value*/
            gasDPM[u8PortNum].u8PRSwapWaitTmrID = MAX_CONCURRENT_TIMERS;
            break;
        }
#endif /*INCLUDE_PD_PR_SWAP*/
#if (TRUE == INCLUDE_PD_DR_SWAP)
        case eDR_SWAP_INITIATE:
        {
            /* Set the timer Id to Max Concurrent Value*/
            gasDPM[u8PortNum].u8DRSwapWaitTmrID = MAX_CONCURRENT_TIMERS;
            break;
        }
#endif /*INCLUDE_PD_DR_SWAP*/
        default:
            break;
    }

    /* Re-initiate the corresponding Swap on SwapWait timer expiry */
    if (DPM_REQUEST_SWAP == DPM_EvaluateRoleSwap (u8PortNum, (eRoleSwapMsgType)u8SwapInitiateType))
    {
        DPM_RegisterInternalEvent(u8PortNum, u8SwapInitiateType);
    } 
}

/************************************************************************************************************************/

/********************* Policy Manager APIs for VDM ********************/
#if (TRUE == INCLUDE_PD_VDM)

UINT8 DPM_EvaluateVDMRequest (UINT8 u8PortNum, UINT32 *pu32VDMHeader)
{
    /* DPM Response */
    UINT8 u8DPMResponse = DPM_RESPOND_VDM_NAK;   
    
    /* SVID extracted from received VDM Header */
    UINT16 u16SVID = DPM_GET_VDM_SVID(*pu32VDMHeader); 
    
    /* Object Position extracted from received VDM Header */
    UINT8 u8ObjPos = DPM_GET_VDM_OBJ_POS(*pu32VDMHeader);
    
    /* SVIDs Count - Copy of global variable to reduce code size */
#if (TRUE == INCLUDE_PD_ALT_MODE)
    UINT8 u8SVIDsCnt = gasCfgStatusData.sAltModePerPortData[u8PortNum].u8SVIDsCnt;
#endif 
    
    /* Invalid Port State - If Data Role is DFP and negotiated PD Rev is 2.0 */
    UINT8 u8InvalidPortState = (((PD_ROLE_DFP == DPM_GET_CURRENT_DATA_ROLE(u8PortNum)) && 
            (PD_SPEC_REVISION_2_0 == DPM_GET_CURRENT_PD_SPEC_REV(u8PortNum))) ? TRUE : FALSE);
        
    /* Type of SVDM Command */
    eSVDMCmd eVDMCmd = (eSVDMCmd) DPM_GET_VDM_CMD(*pu32VDMHeader); 
            
    switch (eVDMCmd)
    {
        case eSVDM_DISCOVER_IDENTITY:
        {
            if ((DPM_VDM_PD_SID == u16SVID) && !(u8InvalidPortState) && !(u8ObjPos) && \
                     (gasCfgStatusData.sVDMPerPortData[u8PortNum].u8PDIdentityCnt != SET_TO_ZERO))
            {
                u8DPMResponse = DPM_RESPOND_VDM_ACK;
            }
            break; 
        }
#if (TRUE == INCLUDE_PD_ALT_MODE)   
        case eSVDM_DISCOVER_SVIDS:
        {
            if ((DPM_VDM_PD_SID == u16SVID) && !(u8InvalidPortState) && \
                            !(u8ObjPos) && (u8SVIDsCnt != SET_TO_ZERO))
            {
                u8DPMResponse = DPM_RESPOND_VDM_ACK;
            }            
            break; 
        }
        case eSVDM_DISCOVER_MODES:
        {
            if (!(u8InvalidPortState) && !(u8ObjPos))
            {
                for (UINT8 u8Index = INDEX_0; u8Index < u8SVIDsCnt; u8Index++)
                {
                    if (u16SVID == gasCfgStatusData.sAltModePerPortData[u8PortNum].u16aSVIDsTable[u8Index])
                    {
                        u8DPMResponse = DPM_RESPOND_VDM_ACK;
                        break; 
                    }
                }
            }
            break; 
        }
        case eSVDM_EXIT_MODE:
        {
            if (DPM_EXIT_ALL_ACTIVE_MODES == u8ObjPos)
            {
                u8DPMResponse = DPM_RESPOND_VDM_ACK; 
            }
            /* Break statement is left intentionally for the below logic to 
               get executed. Object Position in Exit Mode command can be 7 or 
               any valid index of Modes advertised */
        }
        /* fallthrough */
        case eSVDM_ENTER_MODE:        
        {
            for (UINT8 u8Index = INDEX_0; u8Index < u8SVIDsCnt; u8Index++)
            {
                if (u16SVID == gasCfgStatusData.sAltModePerPortData[u8PortNum].u16aSVIDsTable[u8Index])
                {                   
                    if ((u8ObjPos >= INDEX_1) && \
                            (u8ObjPos <= DPM_GET_NO_OF_MODES(gasCfgStatusData.sAltModePerPortData[u8PortNum].u8aSVIDEntryTable[u8Index])))
                    {
                        u8DPMResponse = DPM_RESPOND_VDM_ACK;                        
                    }                     
                    break;
                }
            }    
            break;
        }
#endif 
        default:
        {
            /* When INCLUDE_PD_ALT_MODE is 0 and INCLUDE_PD_VDM is 1, 
               all VMDs except Discover Identity shall be NAKed*/
#if (TRUE == INCLUDE_PD_ALT_MODE) 
            u8DPMResponse = DPM_IGNORE_VDM_RESPONSE; 
#endif 
            break; 
        }
    }
    
    /* Notify user only if DPM Evaluation results in ACK response or 
       the evaluation of VDM is ignored by DPM since it is dependent on 
       response from the application  */
    if (u8DPMResponse)
    {
        if (FALSE == DPM_NotifyClient (u8PortNum, eMCHP_PSF_VDM_REQUEST_RCVD))
        {
            /* Change the DPM response as NAK if application returns FALSE */
            if (DPM_RESPOND_VDM_ACK == u8DPMResponse)
            {
                u8DPMResponse = DPM_RESPOND_VDM_NAK;
            }
        }
    }
    
    /* Set/Clear Modal Operation Active Status based on the command type
       and DPM Response. Both DPM and User Application has evaluated the
       VDM command. So, it is obvious that a modal operation is going 
       to be established on reception of a Enter mode. So, setting the 
       modal operation active status in this API itself */   
#if (TRUE == INCLUDE_PD_ALT_MODE)  
    
    if (eSVDM_ENTER_MODE == eVDMCmd)
    {
        if (DPM_RESPOND_VDM_ACK == u8DPMResponse)
        {
            gasDPM[u8PortNum].u16DPMStatus |= DPM_PORT_IN_MODAL_OPERATION;
        }
        else
        {
            gasDPM[u8PortNum].u16DPMStatus &= ~DPM_PORT_IN_MODAL_OPERATION;
        }
    }
    else if (eSVDM_EXIT_MODE == eVDMCmd)
    {
        if (DPM_RESPOND_VDM_ACK == u8DPMResponse)
        {
            gasDPM[u8PortNum].u16DPMStatus &= ~DPM_PORT_IN_MODAL_OPERATION;
        }
        else
        {
            gasDPM[u8PortNum].u16DPMStatus |= DPM_PORT_IN_MODAL_OPERATION;
        }
    }
    else
    {
        /* Do Nothing */
    }
#endif 
    
    return u8DPMResponse;
}

void DPM_ReturnVDOs (UINT8 u8PortNum, UINT32 *pu32VDMHeader, UINT8 *pu8VDOCnt, UINT32 *pu32ResponseVDO)
{
    switch (DPM_GET_VDM_CMD(*pu32VDMHeader))
    {
        case eSVDM_DISCOVER_IDENTITY:
        {
            *pu8VDOCnt = gasCfgStatusData.sVDMPerPortData[u8PortNum].u8PDIdentityCnt;
            
            (void)MCHP_PSF_HOOK_MEMCPY(pu32ResponseVDO, gasCfgStatusData.sVDMPerPortData[u8PortNum].u32aPDIdentity,                    
                                                    (*pu8VDOCnt * BYTE_LEN_4));                                                               
            
            /* The Responder Shall restart the list of SVIDs each time a Discover Identity
               Command request is received from the Initiator.*/
            gasDPM[u8PortNum].u8CurrSVIDIndex = SET_TO_ZERO; 
            break; 
        }
#if (TRUE == INCLUDE_PD_ALT_MODE)         
        case eSVDM_DISCOVER_SVIDS:
        {
            UINT8 u8Count, u8NumObjects = SET_TO_ZERO; 
            UINT8 u8SVIDsToCopyCnt = MIN((gasCfgStatusData.sAltModePerPortData[u8PortNum].u8SVIDsCnt - \
                                                gasDPM[u8PortNum].u8CurrSVIDIndex), DPM_MAX_SVID_CNT);                       
            UINT16 u16SVID; 
            
            /* The SVIDs are returned 2 per VDO. If there are an odd number of 
               supported SVIDs, the Discover SVIDs Command is returned ending 
               with a SVID value of 0x0000 in the last part of the last VDO. 
               If there are an even number of supported SVIDs, the Discover SVIDs 
               Command is returned ending with an additional VDO containing two 
               SVIDs with values of 0x0000.*/
            for(u8Count = SET_TO_ZERO; u8Count < u8SVIDsToCopyCnt; u8Count++)
            {
                u16SVID = gasCfgStatusData.sAltModePerPortData[u8PortNum].u16aSVIDsTable[gasDPM[u8PortNum].u8CurrSVIDIndex];
                
                if(u8Count & BYTE_LEN_1)
                {                                       
                    *pu32ResponseVDO |= u16SVID;                    
                    pu32ResponseVDO++;
                    u8NumObjects++; 
                }
                else
                {
                    *pu32ResponseVDO = (u16SVID << BYTE_LEN_16);
                }
                gasDPM[u8PortNum].u8CurrSVIDIndex++;
            }  
            if(u8Count != DPM_MAX_SVID_CNT)
            {
                /* check if count is even then pad with 2 zeros */
                if(!(u8Count & BYTE_LEN_1))
                {
                    *pu32ResponseVDO = SET_TO_ZERO;
                }
                else
                {
                    *pu32ResponseVDO &= DPM_LAST_SVID_MASK; 
                }
                u8NumObjects++;
            }
            *pu8VDOCnt = u8NumObjects;            
                        
            break; 
        }
        case eSVDM_DISCOVER_MODES:
        { 
            UINT8 u8StartModeIdx = SET_TO_ZERO;
            
            for (UINT8 u8Index = INDEX_0; \
                       u8Index < gasCfgStatusData.sAltModePerPortData[u8PortNum].u8SVIDsCnt; \
                       u8Index++)
            {
                if (DPM_GET_VDM_SVID(*pu32VDMHeader) == gasCfgStatusData.sAltModePerPortData[u8PortNum].u16aSVIDsTable[u8Index])
                {
                    *pu8VDOCnt = DPM_GET_NO_OF_MODES(gasCfgStatusData.sAltModePerPortData[u8PortNum].u8aSVIDEntryTable[u8Index]);
                    u8StartModeIdx = DPM_GET_START_MODE_IDX(gasCfgStatusData.sAltModePerPortData[u8PortNum].u8aSVIDEntryTable[u8Index]);
                    break; 
                }
            }      
            
            (void)MCHP_PSF_HOOK_MEMCPY(pu32ResponseVDO, &gasCfgStatusData.sAltModePerPortData[u8PortNum].u32aModesTable[u8StartModeIdx],                    
                                                    (*pu8VDOCnt * BYTE_LEN_4));                                                               

            break; 
        }        
#endif
        default:
        {
            break; 
        }
    }  
}

void DPM_VDMBusy_TimerCB (UINT8 u8PortNum, UINT8 u8DummyVariable)
{
    /* Set the timer Id to Max Concurrent Value*/
    gasDPM[u8PortNum].u8VDMBusyTmrID = MAX_CONCURRENT_TIMERS;

    /* Re-initiate the corresponding VDM command on Busy Timer expiry */
    DPM_RegisterInternalEvent(u8PortNum, DPM_INT_EVT_INITIATE_VDM);
}

#endif 