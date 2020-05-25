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
Copyright �  [2019-2020] Microchip Technology Inc. and its subsidiaries.

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
void DPM_VBUSOnOffTimerCB (UINT8 u8PortNum, UINT8 u8DummyVariable)
{   
    gasTypeCcontrol[u8PortNum].u8TypeCState = TYPEC_ERROR_RECOVERY;
    gasTypeCcontrol[u8PortNum].u8TypeCSubState = TYPEC_ERROR_RECOVERY_ENTRY_SS;
    
    gasPolicy_Engine[u8PortNum].ePEState = ePE_INVALIDSTATE;
    gasPolicy_Engine[u8PortNum].ePESubState = ePE_INVALIDSUBSTATE;
    
    gasPolicy_Engine[u8PortNum].u8PETimerID = MAX_CONCURRENT_TIMERS;
}
void DPM_SrcReadyTimerCB (UINT8 u8PortNum, UINT8 u8DummyVariable)
{
    if(gasPolicy_Engine[u8PortNum].u8PEPortSts & PE_EXPLICIT_CONTRACT)
    {
        gasPolicy_Engine[u8PortNum].ePEState = ePE_SRC_HARD_RESET;
        gasPolicy_Engine[u8PortNum].ePESubState = ePE_SRC_HARD_RESET_ENTRY_SS;
    }
    
    else
    {
        DPM_VBUSOnOffTimerCB ( u8PortNum, u8DummyVariable);
    }
    
    gasPolicy_Engine[u8PortNum].u8PETimerID = MAX_CONCURRENT_TIMERS;
}

void DPM_VCONNONTimerErrorCB (UINT8 u8PortNum , UINT8 u8DummyVariable)
{ 
    gasTypeCcontrol[u8PortNum].u8PortSts &= ~TYPEC_VCONN_ON_REQ_MASK;
    gasPolicy_Engine[u8PortNum].u8PETimerID = MAX_CONCURRENT_TIMERS;
    
    if(gasDPM[u8PortNum].u8VCONNErrCounter > (gasCfgStatusData.sPerPortData[u8PortNum].u8VCONNMaxFaultCnt))
    {      
        /*Disable the receiver*/
        PRL_EnableRx (u8PortNum, FALSE);
        
        /*Kill all the Port timers*/
        PDTimer_KillPortTimers (u8PortNum);
        
        /*Disable VCONN by switching off the VCONN FETS which was enabled previously*/
        TypeC_EnabDisVCONN (u8PortNum, TYPEC_VCONN_DISABLE);
        
        if (DPM_GET_CURRENT_POWER_ROLE(u8PortNum) == PD_ROLE_SOURCE)
        {		          
            /*Disable VBUS by driving to vSafe0V if port role is a source*/
            DPM_TypeCSrcVBus5VOnOff(u8PortNum, DPM_VBUS_OFF);
        
            /*Assign an idle state to wait for detach*/
            gasTypeCcontrol[u8PortNum].u8TypeCSubState = TYPEC_ATTACHED_SRC_IDLE_SS;
            
            DEBUG_PRINT_PORT_STR(u8PortNum,"VCONN_ON_ERROR: Entered SRC Powered OFF state");
        }
        else
        { 
            /*Assign an idle state to wait for detach*/
            gasTypeCcontrol[u8PortNum].u8TypeCSubState = TYPEC_ATTACHED_SNK_IDLE_SS;
            
            DEBUG_PRINT_PORT_STR(u8PortNum,"VCONN_ON_ERROR: Entered SNK Powered OFF state");
        }       
        gasPolicy_Engine[u8PortNum].ePEState = ePE_INVALIDSTATE;
        gasPolicy_Engine[u8PortNum].ePESubState = ePE_INVALIDSUBSTATE;
    }
    else
    {
        gasDPM[u8PortNum].u8VCONNErrCounter++;    
        PE_SendHardResetMsg(u8PortNum);    
    }    
}

void DPM_VCONNOFFErrorTimerCB (UINT8 u8PortNum , UINT8 u8DummyVariable)
{  
    /*Set it to Type C Error Recovery */
    gasTypeCcontrol[u8PortNum].u8TypeCState = TYPEC_ERROR_RECOVERY;
    gasTypeCcontrol[u8PortNum].u8TypeCSubState = TYPEC_ERROR_RECOVERY_ENTRY_SS;
    
    gasPolicy_Engine[u8PortNum].u8PETimerID = MAX_CONCURRENT_TIMERS;
    /* Assign an idle state wait for detach*/
    gasPolicy_Engine[u8PortNum].ePEState = ePE_INVALIDSTATE;
  
}

void DPM_ResetVCONNErrorCnt (UINT8 u8PortNum)
{  
    gasDPM[u8PortNum].u8VCONNErrCounter = SET_TO_ZERO;  
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
void DPM_GetPoweredCablePresence(UINT8 u8PortNum, UINT8 *pu8RaPresence)
{
    *pu8RaPresence = (gasTypeCcontrol[u8PortNum].u8PortSts & TYPEC_PWDCABLE_PRES_MASK);
}
/*******************************************************************************/
/*******************************************************************************/
void DPM_EnableNewPDO(UINT8 u8PortNum, UINT8 u8EnableDisable)
{
    if (DPM_ENABLE_NEW_PDO == u8EnableDisable)
    {
        gasDPM[u8PortNum].u8DPM_ConfigData |= DPM_NEW_PDO_ENABLE_MASK;   
    }
    else
    {
        gasDPM[u8PortNum].u8DPM_ConfigData &= ~(DPM_NEW_PDO_ENABLE_MASK);    
    }
}

/*******************************************************************************/
/**************************DPM APIs for VCONN *********************************/
void DPM_VConnOnOff(UINT8 u8PortNum, UINT8 u8VConnEnable)
{
    if(u8VConnEnable == DPM_VCONN_ON)
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
/*******************************************************************************/

/**************************DPM APIs for VBUS* *********************************/
void DPM_SetPortPower(UINT8 u8PortNum)
{
    UINT16 u16VoltageInmV = SET_TO_ZERO;
    UINT16 u16CurrentInmA = SET_TO_ZERO;
    
    if (DPM_GET_CURRENT_POWER_ROLE(u8PortNum) == PD_ROLE_SOURCE)
    {
        if (DPM_PD_FIXED_SUPPLY_CONTRACT == DPM_GET_CURRENT_EXPLICIT_CONTRACT(u8PortNum))
        {
            u16VoltageInmV = DPM_GET_VOLTAGE_FROM_PDO_MILLI_V(gasDPM[u8PortNum].u32NegotiatedPDO);
            u16CurrentInmA = DPM_GET_CURRENT_FROM_PDO_MILLI_A(gasDPM[u8PortNum].u32NegotiatedPDO);            
        }
#if (TRUE == INCLUDE_PD_SOURCE_PPS) 
        else if (DPM_PD_PPS_CONTRACT == DPM_GET_CURRENT_EXPLICIT_CONTRACT(u8PortNum))
        {
            /* VBUS Voltage to be driven for PPS is the voltage 
             requested by Sink in the RDO's Output Voltage field */
           u16VoltageInmV = DPM_GET_OP_VOLTAGE_FROM_PROG_RDO_IN_mV(gasCfgStatusData.sPerPortData[u8PortNum].u32RDO);   
           u16CurrentInmA = DPM_GET_PROG_RDO_OPR_CURRENT_IN_mA(gasCfgStatusData.sPerPortData[u8PortNum].u32RDO); 
        }
#endif 
        else
        {
            /* No support for Battery and Variable power supplies */
        }
        TypeC_ConfigureVBUSThr(u8PortNum, u16VoltageInmV, u16CurrentInmA, TYPEC_CONFIG_NON_PWR_FAULT_THR);            
        
        PWRCTRL_SetPortPower (u8PortNum, gasDPM[u8PortNum].u8NegotiatedPDOIndex, 
                u16VoltageInmV, u16CurrentInmA);
    }
    else
    {
        u16VoltageInmV = gasCfgStatusData.sPerPortData[u8PortNum].u16NegoVoltageInmV; 
        u16CurrentInmA = gasDPM[u8PortNum].u16SinkOperatingCurrInmA;
        TypeC_ConfigureVBUSThr(u8PortNum, u16VoltageInmV, u16CurrentInmA, TYPEC_CONFIG_NON_PWR_FAULT_THR);
    }
}

void DPM_TypeCSrcVBus5VOnOff(UINT8 u8PortNum, UINT8 u8VbusOnorOff)
{
	UINT16 u16Current;
	if(PD_ROLE_SOURCE == DPM_GET_CURRENT_POWER_ROLE(u8PortNum))
	{
		u16Current = gasDPM[u8PortNum].u16MaxCurrSupportedin10mA * DPM_10mA;
        if (DPM_VBUS_ON == u8VbusOnorOff)
        {
            TypeC_ConfigureVBUSThr(u8PortNum, TYPEC_VBUS_5V, u16Current, TYPEC_CONFIG_NON_PWR_FAULT_THR);
            PWRCTRL_SetPortPower (u8PortNum, DPM_VSAFE5V_PDO_INDEX_1, TYPEC_VBUS_5V, u16Current);
    
        }
        else
        {
            TypeC_ConfigureVBUSThr(u8PortNum, TYPEC_VBUS_0V, u16Current, TYPEC_CONFIG_NON_PWR_FAULT_THR);
		    PWRCTRL_SetPortPower (u8PortNum, DPM_VSAFE0V_PDO_INDEX, TYPEC_VBUS_0V, u16Current);
            PWRCTRL_ConfigVBUSDischarge (u8PortNum, TRUE);
        }
    }
}

UINT16 DPM_GetVBUSVoltage(UINT8 u8PortNum)
{
    UINT8 u8VBUSPresence = SET_TO_ZERO; 
    
    /* u8IntStsISR shall be used only for fixed voltages. In case of PPS contract, 
       voltage that is driven on VBUS is the output voltage requested by Sink 
       in RDO */
    if((gasPolicy_Engine[u8PortNum].u8PEPortSts & PE_EXPLICIT_CONTRACT) &&
            (DPM_PD_PPS_CONTRACT == DPM_GET_CURRENT_EXPLICIT_CONTRACT(u8PortNum)))
    {
        return DPM_GET_OP_VOLTAGE_FROM_PROG_RDO_IN_mV(gasCfgStatusData.sPerPortData[u8PortNum].u32RDO);
    }
    else
    {
        u8VBUSPresence = ((gasTypeCcontrol[u8PortNum].u8IntStsISR & 
                          TYPEC_VBUS_PRESENCE_MASK) >> TYPEC_VBUS_PRESENCE_POS);
        if (u8VBUSPresence!= TYPEC_VBUS_0V_PRES)
        {
            return DPM_GET_VOLTAGE_FROM_PDO_MILLI_V(gasCfgStatusData.sPerPortData[u8PortNum].u32aAdvertisedPDO[--u8VBUSPresence]);
        }
        else
        {
            return TYPEC_VBUS_0V_PRES;
        }     
    }    
}

void DPM_EnablePowerFaultDetection(UINT8 u8PortNum)
{	
#if (TRUE == INCLUDE_POWER_FAULT_HANDLING)
    
    UINT16 u16PDOVoltageInmV = SET_TO_ZERO; 
    UINT16 u16PDOCurrentInmA = SET_TO_ZERO; 
    
    if (DPM_PD_FIXED_SUPPLY_CONTRACT == DPM_GET_CURRENT_EXPLICIT_CONTRACT(u8PortNum))
    {        
        /* Obtain voltage from negotiated PDO*/
        u16PDOVoltageInmV = DPM_GET_VOLTAGE_FROM_PDO_MILLI_V(gasDPM[u8PortNum].u32NegotiatedPDO);
        u16PDOCurrentInmA = DPM_GET_CURRENT_FROM_PDO_MILLI_A(gasDPM[u8PortNum].u32NegotiatedPDO);
    }
#if (TRUE == INCLUDE_PD_SOURCE_PPS) 
    else if (DPM_PD_PPS_CONTRACT == DPM_GET_CURRENT_EXPLICIT_CONTRACT(u8PortNum))
    {
        u16PDOVoltageInmV = DPM_GET_OP_VOLTAGE_FROM_PROG_RDO_IN_mV(gasCfgStatusData.sPerPortData[u8PortNum].u32RDO);
        u16PDOCurrentInmA = DPM_GET_PROG_RDO_OPR_CURRENT_IN_mA(gasCfgStatusData.sPerPortData[u8PortNum].u32RDO); 
    }
#endif 
    else
    {
        /* No support for Battery and Variable power supplies */
    }
    
    /* set the threshold to detect fault*/
    TypeC_ConfigureVBUSThr(u8PortNum, u16PDOVoltageInmV, u16PDOCurrentInmA, TYPEC_CONFIG_PWR_FAULT_THR);
    
#endif
}
/*******************************************************************************/
#if (TRUE == INCLUDE_PD_SOURCE)

/****************************** DPM Source related APIs*****************************************/
/* Validate the received Request message */
UINT8 DPM_ValidateRequest(UINT8 u8PortNum, UINT16 u16Header, UINT8 *u8DataBuf)
{
    UINT8 u8RetVal = FALSE;
    UINT8 u8SinkReqObjPos= SET_TO_ZERO;
    UINT16 u16SinkReqCurrVal = SET_TO_ZERO;
    UINT16 u16SrcPDOCurrVal = SET_TO_ZERO;
    UINT8 u8RaPresence = SET_TO_ZERO;
    UINT32 u32PDO = SET_TO_ZERO; 
    UINT32 u32RDO = SET_TO_ZERO; 
#if (TRUE == INCLUDE_PD_SOURCE_PPS) 
    UINT16 u16RDOOpVoltInmV = SET_TO_ZERO; 
#endif           
    /* Get the status of E-Cable presence and ACK status */
    u8RaPresence = (gasTypeCcontrol[u8PortNum].u8PortSts & TYPEC_PWDCABLE_PRES_MASK) & \
                    (~((gasPolicy_Engine[u8PortNum].u8PEPortSts & \
                                            PE_CABLE_RESPOND_NAK) >> PE_CABLE_RESPOND_NAK_POS));
    
    /* Get the Requested PDO object position from received buffer */
    u8SinkReqObjPos= ((u8DataBuf[INDEX_3]) & PE_REQUEST_OBJ_MASK) >> PE_REQUEST_OBJ_POS;
    
    /* Get the PDO/APDO from Advertised PDO Array */
    u32PDO = gasCfgStatusData.sPerPortData[u8PortNum].u32aAdvertisedPDO[u8SinkReqObjPos-1]; 
    
    /* Form the RDO */
    u32RDO = MAKE_UINT32_FROM_BYTES(u8DataBuf[INDEX_0], u8DataBuf[INDEX_1], 
                                        u8DataBuf[INDEX_2], u8DataBuf[INDEX_3]);
    
    if (ePDO_FIXED == (ePDOtypes)DPM_GET_PDO_TYPE(u32PDO))
    {
        /* Get the Requested current value */
        u16SinkReqCurrVal = (UINT16)((u32RDO & PE_REQUEST_OPR_CUR_MASK) >> PE_REQUEST_OPR_CUR_START_POS);
        
        /* Get the current value of Requested Source PDO */
        u16SrcPDOCurrVal = (u32PDO & PE_REQUEST_MAX_CUR_MASK);       
    }
    
#if (TRUE == INCLUDE_PD_SOURCE_PPS)    
    else if (ePDO_PROGRAMMABLE == (ePDOtypes)DPM_GET_PDO_TYPE(u32PDO))
    {     
        /* Get the Requested current value */ 
        u16SinkReqCurrVal = DPM_GET_PROG_RDO_OPR_CURRENT(u32RDO); 
                
        /* Get the current value of Requested Source PDO */
        u16SrcPDOCurrVal = DPM_GET_APDO_MAX_CURRENT(u32PDO); 
         
        /* Get the RDO Output Voltage */
        u16RDOOpVoltInmV = DPM_GET_OP_VOLTAGE_FROM_PROG_RDO_IN_mV(u32RDO);        
    }
#endif 
    else
    {
        /* No support for Variable and Battery power supplies */
    }
    
    /* Update the Previous VBUS voltage */
    if (ePDO_FIXED == (ePDOtypes)DPM_GET_CURRENT_EXPLICIT_CONTRACT(u8PortNum))
    {
        /* If previous request is for a fixed PDO, calculate previous output
           voltage from gasDPM[u8PortNum].u32NegotiatedPDO */
        gasDPM[u8PortNum].u16PrevVBUSVoltageInmV = DPM_GET_VOLTAGE_FROM_PDO_MILLI_V(gasDPM[u8PortNum].u32NegotiatedPDO);
    }
    else if (ePDO_PROGRAMMABLE == (ePDOtypes)DPM_GET_CURRENT_EXPLICIT_CONTRACT(u8PortNum))
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
    u8RetVal = (u16SinkReqCurrVal > u16SrcPDOCurrVal) ? DPM_INVALID_REQUEST : (((u8SinkReqObjPos<= FALSE) || \
               (u8SinkReqObjPos> gasCfgStatusData.sPerPortData[u8PortNum].u8AdvertisedPDOCnt))) ? \
                DPM_INVALID_REQUEST : (u8RaPresence == FALSE) ? DPM_VALID_REQUEST : \
                (u16SinkReqCurrVal > gasDPM[u8PortNum].u16MaxCurrSupportedin10mA) ? \
                DPM_INVALID_REQUEST : DPM_VALID_REQUEST;   
   
#if (TRUE == INCLUDE_PD_SOURCE_PPS)     
    if (ePDO_PROGRAMMABLE == (ePDOtypes)DPM_GET_PDO_TYPE(u32PDO))
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
    if(u8RetVal == DPM_VALID_REQUEST)
    {
        gasDPM[u8PortNum].u32NegotiatedPDO = u32PDO; 
        gasDPM[u8PortNum].u8NegotiatedPDOIndex = u8SinkReqObjPos;
        gasCfgStatusData.sPerPortData[u8PortNum].u32RDO = u32RDO;
        
        /* Set Capability mismatch status */
        if (u32RDO & PE_REQUEST_CAP_MISMATCH_MASK)
        {
            gasCfgStatusData.sPerPortData[u8PortNum].u32PortConnectStatus |= 
                    DPM_PORT_SRC_CAPABILITY_MISMATCH_STATUS; 
        }
        else
        {
            gasCfgStatusData.sPerPortData[u8PortNum].u32PortConnectStatus &= 
                        ~(DPM_PORT_SRC_CAPABILITY_MISMATCH_STATUS);                 
        }

        if (ePDO_FIXED == (ePDOtypes)DPM_GET_PDO_TYPE(u32PDO))
        {
            /* Set the current Explicit Contract Type as Fixed Supply */
            gasDPM[u8PortNum].u8DPM_Status &= ~(DPM_CURR_EXPLICIT_CONTRACT_TYPE_MASK); 
            
            /* Update Negotiated current in terms of mA */            
            gasCfgStatusData.sPerPortData[u8PortNum].u16NegoCurrentInmA = \
                            (u16SrcPDOCurrVal * DPM_PDO_CURRENT_UNIT); 
            
            /* Update Negotiated voltage in terms of mV */            
            gasCfgStatusData.sPerPortData[u8PortNum].u16NegoVoltageInmV = \
                    DPM_GET_VOLTAGE_FROM_PDO_MILLI_V(gasDPM[u8PortNum].u32NegotiatedPDO);
            
            /* Update the allocated power in terms of 250mW */
            gasCfgStatusData.sPerPortData[u8PortNum].u16AllocatedPowerIn250mW =    
                    ((gasCfgStatusData.sPerPortData[u8PortNum].u16NegoVoltageInmV * gasCfgStatusData.sPerPortData[u8PortNum].u16NegoCurrentInmA) / 
                    DPM_250mW);             
        }
#if (TRUE == INCLUDE_PD_SOURCE_PPS)        
        else if (ePDO_PROGRAMMABLE == (ePDOtypes)DPM_GET_PDO_TYPE(u32PDO))
        {
            /* Set the current Explicit Contract Type as PPS */
            gasDPM[u8PortNum].u8DPM_Status |= DPM_CURR_EXPLICIT_CONTRACT_TYPE_MASK;           
            
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
        DEBUG_PRINT_PORT_STR (u8PortNum,"DPM-PE: Requested is Valid \r\n");
    }

    return u8RetVal;
}

/* Reset the current value in PDO */
UINT32 DPM_CurrentCutDown (UINT8 u8PortNum, UINT32 u32PDO)
{
    /* If PDO max current greater than E-Cable supported current, reset the current value */
    if (ePDO_FIXED == (ePDOtypes)DPM_GET_PDO_TYPE(u32PDO))
    {
        if((u32PDO & PE_MAX_CURR_MASK) > DPM_CABLE_CURR_3A_UNIT)
        {
            u32PDO &= ~PE_MAX_CURR_MASK;
            u32PDO |= DPM_CABLE_CURR_3A_UNIT;
            /* Source PDO Current value reduced due to cable limitation */   
            gasCfgStatusData.sPerPortData[u8PortNum].u32PortConnectStatus |= 
                                        DPM_PORT_CABLE_REDUCED_SRC_CAPABILITIES_STATUS;        
        }  
    }
#if (TRUE == INCLUDE_PD_SOURCE_PPS)    
    else if (ePDO_PROGRAMMABLE == (ePDOtypes)DPM_GET_PDO_TYPE(u32PDO))
    {
        if (DPM_GET_APDO_MAX_CURRENT_IN_mA(u32PDO) > DPM_3000mA)
        {
            u32PDO &= ~(DPM_APDO_MAX_CURRENT_MASK);
            u32PDO |= DPM_3A_IN_50mA; 
            /* Source PDO Current value reduced due to cable limitation */   
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
void DPM_Get_Source_Capabilities(UINT8 u8PortNum, UINT8* u8pSrcPDOCnt, UINT32* pu32DataObj)
{   
  
    UINT8 u8RaPresence = SET_TO_ZERO;
	UINT32 *pu32SrcCap;
    
#if (TRUE == INCLUDE_POWER_THROTTLING)
    if (FALSE == DPM_IS_PB_ENABLED(u8PortNum))
    {
        /* If PB is disabled, update the Source caps based on currently 
           active PT Bank. If PB is enabled, this would be taken care by PB */
        PT_CalculateSrcPDOs(u8PortNum); 
    }
#endif 
    
	/* Get the source PDO count */
    if (DPM_ENABLE_NEW_PDO == DPM_GET_NEW_PDO_STATUS(u8PortNum))
    {
        *u8pSrcPDOCnt = gasCfgStatusData.sPerPortData[u8PortNum].u8NewPDOCnt;
   
        pu32SrcCap = (UINT32 *)&gasCfgStatusData.sPerPortData[u8PortNum].u32aNewPDO[0];                        
    }
    else
    {
        *u8pSrcPDOCnt = gasCfgStatusData.sPerPortData[u8PortNum].u8SourcePDOCnt;
   
        pu32SrcCap = (UINT32 *)&gasCfgStatusData.sPerPortData[u8PortNum].u32aSourcePDO[0];        
    }
 
#if (TRUE == INCLUDE_PD_SOURCE_PPS)    
    if (TRUE == DPM_IS_PPS_ENABLED(u8PortNum))
    {
        DPM_IncludeAPDOs (u8PortNum, u8pSrcPDOCnt, pu32SrcCap); 
    }   
#endif
    
    DPM_GetPoweredCablePresence(u8PortNum, &u8RaPresence);
   
    /* E-Cable presents */
    if((TRUE == u8RaPresence))
    {
        /* If E-Cable max current is 5A, pass the capabilities without change */
        if(gasDPM[u8PortNum].u16MaxCurrSupportedin10mA == DPM_CABLE_CURR_5A_UNIT)
        {
            /* The attached USB-C cable supports the locally-defined Source PDOs */
            gasCfgStatusData.sPerPortData[u8PortNum].u32PortConnectStatus &= 
                                        ~(DPM_PORT_CABLE_REDUCED_SRC_CAPABILITIES_STATUS);

            (void)MCHP_PSF_HOOK_MEMCPY(&pu32DataObj[INDEX_0], &pu32SrcCap[INDEX_0],\
                    DPM_4BYTES_FOR_EACH_PDO_OF(*u8pSrcPDOCnt) );
        }
        /* If E-Cable max current is 3A and PDO current value is more than 3A, 
            reset the current value of PDOs */
        else
        {
            DPM_ChangeCapabilities (u8PortNum, &pu32DataObj[INDEX_0], &pu32SrcCap[INDEX_0], *u8pSrcPDOCnt);
        }
          
    }   
    else
    {
        DPM_ChangeCapabilities (u8PortNum, pu32DataObj, &pu32SrcCap[INDEX_0],*u8pSrcPDOCnt);  
    }     
}

void DPM_ResetNewPDOParameters(UINT8 u8PortNum)
{
    DPM_EnableNewPDO(u8PortNum, DPM_DISABLE_NEW_PDO); 
    
    gasCfgStatusData.sPerPortData[u8PortNum].u8NewPDOCnt = RESET_TO_ZERO; 
        
    for (UINT8 u8PDOIndex = INDEX_0; u8PDOIndex < DPM_MAX_PDO_CNT; u8PDOIndex++)
    {
        gasCfgStatusData.sPerPortData[u8PortNum].u32aNewPDO[u8PDOIndex] = RESET_TO_ZERO; 
    }    
}

void DPM_UpdateAdvertisedPDOParam(UINT8 u8PortNum)
{
    if (DPM_ENABLE_NEW_PDO == DPM_GET_NEW_PDO_STATUS(u8PortNum))
    {
        /* Update Advertised PDO Count */
        gasCfgStatusData.sPerPortData[u8PortNum].u8AdvertisedPDOCnt = \
                            gasCfgStatusData.sPerPortData[u8PortNum].u8NewPDOCnt;

       /* Update Advertised PDO Registers with New PDOs if NewPDOSlct is enabled. */
        (void)MCHP_PSF_HOOK_MEMCPY(gasCfgStatusData.sPerPortData[u8PortNum].u32aAdvertisedPDO, 
            gasCfgStatusData.sPerPortData[u8PortNum].u32aNewPDO, DPM_4BYTES_FOR_EACH_PDO_OF(gasCfgStatusData.sPerPortData[u8PortNum].u8NewPDOCnt));            
    }
    else
    {
        /* Update Advertised PDO Count */
        gasCfgStatusData.sPerPortData[u8PortNum].u8AdvertisedPDOCnt = \
                            gasCfgStatusData.sPerPortData[u8PortNum].u8SourcePDOCnt;

        /* Update Advertised PDO Registers with Default Source PDOs if NewPDOSlct is not enabled. */
        (void)MCHP_PSF_HOOK_MEMCPY(gasCfgStatusData.sPerPortData[u8PortNum].u32aAdvertisedPDO, 
            gasCfgStatusData.sPerPortData[u8PortNum].u32aSourcePDO, DPM_4BYTES_FOR_EACH_PDO_OF(gasCfgStatusData.sPerPortData[u8PortNum].u8SourcePDOCnt));           
    }

#if (TRUE == INCLUDE_PD_SOURCE_PPS)
    if (TRUE == DPM_IS_PPS_ENABLED(u8PortNum))
    {    
        /* Add APDOs and APDO count to the Advertised PDO registers */
        DPM_IncludeAPDOs (u8PortNum, &gasCfgStatusData.sPerPortData[u8PortNum].u8AdvertisedPDOCnt, 
                                    &gasCfgStatusData.sPerPortData[u8PortNum].u32aAdvertisedPDO[INDEX_0]); 
    }
#endif
    
    /* Update the Port Connection Status register by comparing the Fixed and 
       Advertised Source PDOs */
    if (0 == DPM_ComparePDOs(u8PortNum))
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
}

UINT8 DPM_ComparePDOs(UINT8 u8PortNum)
{
    return MCHP_PSF_HOOK_MEMCMP(&gasCfgStatusData.sPerPortData[u8PortNum].u32aSourcePDO[INDEX_0],
                    &gasCfgStatusData.sPerPortData[u8PortNum].u32aAdvertisedPDO[INDEX_0], 
                    ((MAX(gasCfgStatusData.sPerPortData[u8PortNum].u8SourcePDOCnt, 
                        gasCfgStatusData.sPerPortData[u8PortNum].u8AdvertisedPDOCnt)) * 4)); 
}

void DPM_StoreSinkCapabilities(UINT8 u8PortNum, UINT16 u16Header, UINT32* u32DataBuf)
{   
    /* Store the count in Partner PDO Count. */
    gasCfgStatusData.sPerPortData[u8PortNum].u8PartnerPDOCnt = PRL_GET_OBJECT_COUNT(u16Header); 
    
    for (UINT8 u8PDOIndex = INDEX_0; u8PDOIndex < gasCfgStatusData.sPerPortData[u8PortNum].u8PartnerPDOCnt; u8PDOIndex++)
    {
      gasCfgStatusData.sPerPortData[u8PortNum].u32aPartnerPDO[u8PDOIndex] = u32DataBuf[u8PDOIndex];   
    }    
}

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
 
void DPM_IncludeAPDOs(UINT8 u8PortNum, UINT8 *u8pSrcPDOCnt, UINT32 *u32pSrcCap)
{
    UINT8 u8APDOEnDisMask = DPM_PPS_APDO_EN_DIS_MASK; 
   
	for (UINT8 byIndex = INDEX_0; byIndex < DPM_MAX_APDO_COUNT; byIndex++)
    {
        /* At any point PDO + APDO Count should not exceed 7 */
        if ((*u8pSrcPDOCnt) < DPM_MAX_PDO_CNT)
        {
            /* Check if APDO is enabled */
            if (gasCfgStatusData.sPPSPerPortData[u8PortNum].u8PPSCfgData & u8APDOEnDisMask) 
            {
                /* Include the APDO in Source capabilities message */
                *(u32pSrcCap + (*u8pSrcPDOCnt)) = gasCfgStatusData.sPPSPerPortData[u8PortNum].u32aPPSApdo[byIndex];

                /* Increment Source caps PDO count */
                (*u8pSrcPDOCnt)++; 
            }
            /* Left shift the mask to check for next APDO */
            u8APDOEnDisMask <<= 1;
        }
    }      
}

void DPM_StorePartnerAlertInfo(UINT8 u8PortNum, UINT8 *u8DataBuf)
{
    /* Store the Alert Information received from Port Partner */
    gasCfgStatusData.sPPSPerPortData[u8PortNum].u32PartnerAlertInfo = \
                    MAKE_UINT32_FROM_BYTES(u8DataBuf[INDEX_0], u8DataBuf[INDEX_1], \
                                        u8DataBuf[INDEX_2], u8DataBuf[INDEX_3]);
}

void DPM_StoreOrClearPartnerStatus(UINT8 u8PortNum, UINT8 *u8DataBuf, UINT8 u8StrClr)
{
    /* Store/Clear the Status information received from Port Partner */
    for (UINT8 u8Index = INDEX_0; u8Index < PE_STATUS_DATA_BLOCK_SIZE_IN_BYTES; u8Index++)
    {
        if (DPM_STORE_PARTNER_STATUS == u8StrClr)
        {
            gasCfgStatusData.sPPSPerPortData[u8PortNum].u8aPartnerStatus[u8Index] = \
                                                   u8DataBuf[u8Index];   
        }
        else
        {
            gasCfgStatusData.sPPSPerPortData[u8PortNum].u8aPartnerStatus[u8Index] = \
                                                    RESET_TO_ZERO;               
        }
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
    
/* Bit    Description 
    0     Reserved and Shall be set to zero
   1..2   00 - Not Supported
          01 - Normal
          10 - Warning
          11 - Over temperature
   3..7   Reserved and Shall be set to zero */ 
    
    if (PD_THROTTLE_BANK_A == DPM_GET_CURRENT_PT_BANK)
    {
        u8TempStatus = DPM_TEMP_STATUS_NORMAL; 
    }
    else if ((PD_THROTTLE_BANK_B == DPM_GET_CURRENT_PT_BANK) || 
                        (PD_THROTTLE_BANK_C == DPM_GET_CURRENT_PT_BANK))
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
    
    /* Bit 1: Source power limited due to cable supported current */
    if (gasCfgStatusData.sPerPortData[u8PortNum].u32PortConnectStatus & 
                                DPM_PORT_CABLE_REDUCED_SRC_CAPABILITIES_STATUS)
    {
        u8PwrStatus |= DPM_PWRSTS_SRCPWR_LTD_CABLE_CURR; 
    }

    /* Source power limited due to..  */
    if (gasCfgStatusData.sPerPortData[u8PortNum].u32PortConnectStatus &
                                DPM_PORT_SRC_CAPABILITIES_REDUCED_STATUS)
    {
        /* Bit 2: insufficient power available while sourcing other ports */
        if (TRUE == DPM_IS_PB_ENABLED(u8PortNum))
        {
            u8PwrStatus |= DPM_PWRSTS_SRCPWR_LTD_INSUFF_PWR_AVAIL; 
        }  
        /* Bit 5: temperature */
        if ((PD_THROTTLE_BANK_B == DPM_GET_CURRENT_PT_BANK) || 
                            (PD_THROTTLE_BANK_C == DPM_GET_CURRENT_PT_BANK))
        {
            u8PwrStatus |= DPM_PWRSTS_SRCPWR_LTD_TEMP; 
        }        
        
    }
    /* Bit 3: Not Applicable */
    /* Bit 4: Source power limited due to Event Flags in place (Event Flags must also be set) */
    if (gasDPM[u8PortNum].u8StatusEventFlags)
    {
        u8PwrStatus |= DPM_PWRSTS_SRCPWR_LTD_EVNT_FLAG;
    }
    
    /* Bit 7:6 - Reserved */
    
    return u8PwrStatus;  
}

#endif /*INCLUDE_PD_SOURCE_PPS*/ 
#endif /*INCLUDE_PD_SOURCE*/  

/*********************************DPM VDM Cable APIs**************************************/
UINT8 DPM_StoreVDMECableData(UINT8 u8PortNum, UINT8 u8SOPType, UINT16 u16Header, UINT32* u32DataBuf)
{
    UINT32 u32ProductTypeVDO;
    UINT8 u8RetVal = FALSE;
    UINT8 u8CurVal;
    
    /* Get the CMD type from received VDM */
    u8RetVal = DPM_VDM_GET_CMD_TYPE(u32DataBuf[DPM_VDM_HEADER_POS]);
    
    /* if Data object is one, received message is NAK */
    if(u8RetVal == PE_VDM_NAK || u8RetVal == PE_VDM_BUSY)
    {
        u8RetVal = PE_VDM_NAK;
    }
    else
    {
        /* Get the product VDO from the received message */
        u32ProductTypeVDO = u32DataBuf[DPM_VMD_PRODUCT_TYPE_VDO_POS];               
       
        /* Get the Cable supported current value */
        u8CurVal = DPM_GET_CABLE_CUR_VAL(u32ProductTypeVDO);
        
        /* Setting E-Cable Max Current Value */
        if(u8CurVal == DPM_CABLE_CURR_3A)
        {
            gasDPM[u8PortNum].u16MaxCurrSupportedin10mA = DPM_CABLE_CURR_3A_UNIT;
        }
        
        else if(u8CurVal == DPM_CABLE_CURR_5A)
        {
            gasDPM[u8PortNum].u16MaxCurrSupportedin10mA = DPM_CABLE_CURR_5A_UNIT;
        }
        
        else
        {
           /* Do nothing */ 
        }
        
        /* Received message is ACK */
        u8RetVal = PE_VDM_ACK;
    }
    
    return u8RetVal;
}
/*******************************************************************************/
/*****************************DPM API that access Policy Engine************/
UINT8 DPM_IsHardResetInProgress(UINT8 u8PortNum)
{
    UINT8 u8HardResetProgressStatus = ((gasPolicy_Engine[u8PortNum].u8PEPortSts & \
                                        PE_HARDRESET_PROGRESS_MASK) >> PE_HARDRESET_PROGRESS_POS);
    return u8HardResetProgressStatus;

}
/******************************************************************************/

#if (TRUE == INCLUDE_PD_SINK)
/****************************** DPM Sink related APIs*****************************************/
void DPM_Get_Sink_Capabilities(UINT8 u8PortNum,UINT8 *u8pSinkPDOCnt, UINT32 * pu32DataObj)
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
        
        u8Power[u8PDOIndex][DPM_PDO_PWR] = (UINT8)((float)(DPM_GET_PDO_VOLTAGE(u32PDO)*50/(float)1000) *\
                                            ((float)((float)(DPM_GET_PDO_CURRENT(u32PDO) * 10) /(float)1000)));
 
        u8Power[u8PDOIndex][DPM_PDO_INDEX] = u8PDOIndex;
    }
    
    for(u8PDOIndex = SET_TO_ZERO; u8PDOIndex < u8PDOCount; u8PDOIndex++)
    {          
        for(u8PowerIndex = SET_TO_ZERO; u8PowerIndex < (u8PDOCount - u8PDOIndex - 1); u8PowerIndex++)
        {
            if(u8SinkMode == DPM_SINK_MODE_A)
            {
                if(u8Power[u8PowerIndex][DPM_PDO_PWR] <= u8Power[u8PowerIndex + 1][DPM_PDO_PWR])
                {
                   u8PowerSwap = u8Power[u8PowerIndex][DPM_PDO_PWR];
                   u8PowerSwapIndex = u8Power[u8PowerIndex][DPM_PDO_INDEX];
                   u8Power[u8PowerIndex][DPM_PDO_PWR] = u8Power[DPM_NEXT_PWR_INDEX(u8PowerIndex)][DPM_PDO_PWR];
                   u8Power[u8PowerIndex][DPM_PDO_INDEX] = u8Power[DPM_NEXT_PWR_INDEX(u8PowerIndex)][DPM_PDO_INDEX];
                   u8Power[DPM_NEXT_PWR_INDEX(u8PowerIndex)][DPM_PDO_PWR] = u8PowerSwap; 
                   u8Power[DPM_NEXT_PWR_INDEX(u8PowerIndex)][DPM_PDO_INDEX] = u8PowerSwapIndex;
                }
            }
            else if (u8SinkMode == DPM_SINK_MODE_B)
            {
                if(u8Power[u8PowerIndex][DPM_PDO_PWR] < u8Power[u8PowerIndex + 1][DPM_PDO_PWR])
                {
                   u8PowerSwap = u8Power[u8PowerIndex][DPM_PDO_PWR];
                   u8PowerSwapIndex = u8Power[u8PowerIndex][DPM_PDO_INDEX];
                   u8Power[u8PowerIndex][DPM_PDO_PWR] = u8Power[DPM_NEXT_PWR_INDEX(u8PowerIndex)][DPM_PDO_PWR];
                   u8Power[u8PowerIndex][DPM_PDO_INDEX] = u8Power[DPM_NEXT_PWR_INDEX(u8PowerIndex)][DPM_PDO_INDEX];
                   u8Power[DPM_NEXT_PWR_INDEX(u8PowerIndex)][DPM_PDO_PWR] = u8PowerSwap; 
                   u8Power[DPM_NEXT_PWR_INDEX(u8PowerIndex)][DPM_PDO_INDEX] = u8PowerSwapIndex;
                }
                else if(u8Power[u8PowerIndex][DPM_PDO_PWR] == u8Power[u8PowerIndex + 1][DPM_PDO_PWR]) 
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
            }            
        }
    }
}

void DPM_Evaluate_Received_Src_caps(UINT8 u8PortNum ,UINT16 u16RecvdSrcCapsHeader,
                                     UINT32 *pu32RecvdSrcCapsPayload)
{
    /* Two dimensional array stores the Power and Original PDO index number for each PDO index*/
    UINT8 u8SrcPower[DPM_MAX_PDO_CNT][DPM_2_DIMENSION_TO_STORE_INDEX_AND_PWR] = {SET_TO_ZERO};
    UINT8 u8SinkPower[DPM_MAX_PDO_CNT][DPM_2_DIMENSION_TO_STORE_INDEX_AND_PWR] = {SET_TO_ZERO};
    UINT8 u8SrcIndex = SET_TO_ZERO;
    UINT8 u8SinkIndex = SET_TO_ZERO;
    UINT8 u8CapMismatch = FALSE;   
    /*PDO Count of the sink*/
	UINT8 u8SinkPDOCnt = gasCfgStatusData.sPerPortData[u8PortNum].u8SinkPDOCnt;
    /*PDO Count of the source derived from received src caps*/
	UINT8 u8Recevd_SrcPDOCnt =  PRL_GET_OBJECT_COUNT(u16RecvdSrcCapsHeader);
    UINT32 u32RcvdSrcPDO;
    UINT8 u8SrcPDOIndex;
    UINT8 u8SinkPDOIndex;
    UINT32 u32SinkPDO;
    UINT16 u16MinPDOPreferredCurIn10mA;
    UINT16 u16SinkRDOCurIn10mA;
    UINT8 u8SinkMode = (gasCfgStatusData.sPerPortData[u8PortNum].u8SinkConfigSel & DPM_SINK_CONFIG_SINK_MODE_SEL_MASK);
    UINT8 u8SinkGiveBackFlag = ((gasCfgStatusData.sPerPortData[u8PortNum].u8SinkConfigSel & \
            DPM_SINK_CONFIG_GIVE_BACK_FLAG_MASK) >> DPM_SINK_CONFIG_GIVE_BACK_FLAG_POS);
    UINT8 u8SinkNoUSBSusp = ((gasCfgStatusData.sPerPortData[u8PortNum].u8SinkConfigSel & \
            DPM_SINK_CONFIG_NO_USB_SUSP_MASK) >> DPM_SINK_CONFIG_NO_USB_SUSP_POS);
    
    /*Clear partner variable*/
    for(UINT8 u8Index = SET_TO_ZERO; u8Index < DPM_MAX_PDO_CNT; u8Index++)
    {
        gasCfgStatusData.sPerPortData[u8PortNum].u32aPartnerPDO[u8Index] = SET_TO_ZERO;
    }
    
	/*Update received source capabilities to partner variable*/
    (void)MCHP_PSF_HOOK_MEMCPY(gasCfgStatusData.sPerPortData[u8PortNum].u32aPartnerPDO, 
                                    pu32RecvdSrcCapsPayload, DPM_4BYTES_FOR_EACH_PDO_OF(u8Recevd_SrcPDOCnt));
    gasCfgStatusData.sPerPortData[u8PortNum].u8PartnerPDOCnt = u8Recevd_SrcPDOCnt;
    
    /* Calculate and sort the power of Sink PDOs */
    DPM_CalculateAndSortPower(u8SinkPDOCnt, gasCfgStatusData.sPerPortData[u8PortNum].u32aSinkPDO, u8SinkPower, u8SinkMode);
    
    /* Calculate and sort the received source PDOs power */
    DPM_CalculateAndSortPower(u8Recevd_SrcPDOCnt, pu32RecvdSrcCapsPayload, u8SrcPower, u8SinkMode);
    
    /* Compare Maximum power sink PDO to received source PDOs */
    /* Storing PDO index of the Sink PDO which has maximum power.*/
    u8SinkPDOIndex = u8SinkPower[DPM_PDO_WITH_MAX_PWR][DPM_PDO_INDEX];
    /* Storing Sink PDO which has maximum power*/
    u32SinkPDO = gasCfgStatusData.sPerPortData[u8PortNum].u32aSinkPDO[u8SinkPDOIndex];
    
    for(u8SrcIndex = SET_TO_ZERO; u8SrcIndex < u8Recevd_SrcPDOCnt; u8SrcIndex++)
    {
        /* Comparing whether any of Source power is greater than or 
         * equal to maximum Sink power*/
        if(u8SrcPower[u8SrcIndex][DPM_PDO_PWR] >= u8SinkPower[DPM_PDO_WITH_MAX_PWR][DPM_PDO_PWR])
        {
            u8SrcPDOIndex = u8SrcPower[u8SrcIndex][DPM_PDO_INDEX];
            u32RcvdSrcPDO = pu32RecvdSrcCapsPayload[u8SrcPDOIndex];
            if((DPM_GET_PDO_VOLTAGE(u32RcvdSrcPDO) == DPM_GET_PDO_VOLTAGE(u32SinkPDO)) && \
                (DPM_GET_PDO_CURRENT(u32RcvdSrcPDO)) >= DPM_GET_PDO_CURRENT(u32SinkPDO))
            {
                if(u8SinkGiveBackFlag)
                {
                    /*if go to min supported Form request message with minimum current */
                    gasCfgStatusData.sPerPortData[u8PortNum].u32RDO = \
                            DPM_FORM_DATA_REQUEST((u8SrcPDOIndex + 1), u8CapMismatch,
                            u8SinkGiveBackFlag, DPM_GET_PDO_USB_COMM_CAP(u32SinkPDO),\
                            u8SinkNoUSBSusp, DPM_GET_PDO_CURRENT(u32SinkPDO),\
                           (gasCfgStatusData.sPerPortData[u8PortNum].u16MinimumOperatingCurInmA/DPM_10mA));
                }
                else
                {
                    /* Form request message */
                    gasCfgStatusData.sPerPortData[u8PortNum].u32RDO = \
                            DPM_FORM_DATA_REQUEST((u8SrcPDOIndex + 1), u8CapMismatch,
                            u8SinkGiveBackFlag, DPM_GET_PDO_USB_COMM_CAP(u32SinkPDO),\
                            u8SinkNoUSBSusp, DPM_GET_PDO_CURRENT(u32SinkPDO),\
                                                DPM_GET_PDO_CURRENT(u32SinkPDO));
                }
           
                gasDPM[u8PortNum].u16SinkOperatingCurrInmA = DPM_GET_PDO_CURRENT(u32SinkPDO)*DPM_10mA;
                /*Updating the globals with Sink PDO selected */
                gasDPM[u8PortNum].u32NegotiatedPDO = u32SinkPDO;
                /*Updating the globals for Sink */
                gasDPM[u8PortNum].u8NegotiatedPDOIndex = u8SinkPDOIndex+1; 
                /*Update Negotiated value*/
                gasCfgStatusData.sPerPortData[u8PortNum].u16NegoVoltageInmV = (DPM_GET_PDO_VOLTAGE(u32SinkPDO) * DPM_PDO_VOLTAGE_UNIT);
                /*Update Negotiated value*/
                gasCfgStatusData.sPerPortData[u8PortNum].u16NegoCurrentInmA = (DPM_GET_PDO_CURRENT(u32SinkPDO) * DPM_10mA);
                /*VBUS Threshold are configured for the requested PDO*/
                DPM_SetPortPower (u8PortNum);                
                return;
            }
        }
    }
    
    /* Check DPM whether minimum PDO current matches*/
    for(u8SrcIndex = SET_TO_ZERO; u8SrcIndex < u8Recevd_SrcPDOCnt; u8SrcIndex++)
    {
        u8SrcPDOIndex = u8SrcPower[u8SrcIndex][DPM_PDO_INDEX];
        u32RcvdSrcPDO = pu32RecvdSrcCapsPayload[u8SrcPDOIndex];
        for(u8SinkIndex = SET_TO_ZERO; u8SinkIndex < u8SinkPDOCnt; u8SinkIndex++)
        {
            u8SinkPDOIndex = u8SinkPower[u8SinkIndex][DPM_PDO_INDEX];
            u32SinkPDO = gasCfgStatusData.sPerPortData[u8PortNum].u32aSinkPDO[u8SinkPDOIndex];
            u16MinPDOPreferredCurIn10mA = (gasCfgStatusData.sPerPortData[u8PortNum].u16aMinPDOPreferredCurInmA[u8SinkPDOIndex] /10);
    
            if((DPM_GET_PDO_VOLTAGE(u32RcvdSrcPDO) == DPM_GET_PDO_VOLTAGE(u32SinkPDO))
                    && (DPM_GET_PDO_CURRENT(u32RcvdSrcPDO) >= u16MinPDOPreferredCurIn10mA))
            {
                if(DPM_GET_PDO_CURRENT(u32RcvdSrcPDO) > DPM_GET_PDO_CURRENT(u32SinkPDO))
                {
                    u16SinkRDOCurIn10mA = DPM_GET_PDO_CURRENT(u32SinkPDO);
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
                            DPM_FORM_DATA_REQUEST((u8SrcPDOIndex + 1), u8CapMismatch,
                            u8SinkGiveBackFlag, DPM_GET_PDO_USB_COMM_CAP(u32SinkPDO),\
                            u8SinkNoUSBSusp, u16SinkRDOCurIn10mA,\
                            (gasCfgStatusData.sPerPortData[u8PortNum].u16MinimumOperatingCurInmA/DPM_10mA));      
                }
                else
                {
                     /* Form Request message with Capability Mismatch*/
                    gasCfgStatusData.sPerPortData[u8PortNum].u32RDO = \
                            DPM_FORM_DATA_REQUEST((u8SrcPDOIndex + 1), u8CapMismatch,
                            u8SinkGiveBackFlag, DPM_GET_PDO_USB_COMM_CAP(u32SinkPDO),\
                            u8SinkNoUSBSusp, u16SinkRDOCurIn10mA,\
                                                DPM_GET_PDO_CURRENT(u32SinkPDO));
                }
                gasDPM[u8PortNum].u16SinkOperatingCurrInmA = u16SinkRDOCurIn10mA * DPM_10mA;
                /*Updating the globals with Sink PDO selected */
                gasDPM[u8PortNum].u32NegotiatedPDO = u32RcvdSrcPDO;
                /*Updating the globals with Sink PDO index selected*/
                gasDPM[u8PortNum].u8NegotiatedPDOIndex = (u8SinkPDOIndex+1);
                /*Update Negotiated value*/
                 gasCfgStatusData.sPerPortData[u8PortNum].u16NegoVoltageInmV = (DPM_GET_PDO_VOLTAGE(u32SinkPDO) * DPM_PDO_VOLTAGE_UNIT);
                /*Update Negotiated value*/
                 gasCfgStatusData.sPerPortData[u8PortNum].u16NegoCurrentInmA = (u16SinkRDOCurIn10mA * DPM_10mA);              
                
                 DPM_SetPortPower (u8PortNum);
                
                return;
            }
        }
    }
    
    /*  None of the capability matches thus capability mismatch bit is set*/
    u8CapMismatch = TRUE;   
    for(u8SrcIndex = SET_TO_ZERO; u8SrcIndex < u8Recevd_SrcPDOCnt; u8SrcIndex++)
    {
        u8SrcPDOIndex = u8SrcPower[u8SrcIndex][DPM_PDO_INDEX];
        u32RcvdSrcPDO = pu32RecvdSrcCapsPayload[u8SrcPDOIndex];
        
        for(u8SinkIndex = SET_TO_ZERO; u8SinkIndex < u8SinkPDOCnt; u8SinkIndex++)
        {
            u8SinkPDOIndex = u8SinkPower[u8SinkIndex][DPM_PDO_INDEX];
            u32SinkPDO = gasCfgStatusData.sPerPortData[u8PortNum].u32aSinkPDO[u8SinkPDOIndex];
            
            if((DPM_GET_PDO_VOLTAGE(u32RcvdSrcPDO)) == DPM_GET_PDO_VOLTAGE(u32SinkPDO))
            {
                if(u8SinkGiveBackFlag)
                {
                    /*if go to min supported Form request message with minimum current */
                    /* Form Request message with Capability Mismatch*/
                    gasCfgStatusData.sPerPortData[u8PortNum].u32RDO = \
                            DPM_FORM_DATA_REQUEST((u8SrcPDOIndex + 1), u8CapMismatch,
                            u8SinkGiveBackFlag, DPM_GET_PDO_USB_COMM_CAP(u32SinkPDO),\
                            u8SinkNoUSBSusp, DPM_GET_PDO_CURRENT(u32RcvdSrcPDO),\
                            (gasCfgStatusData.sPerPortData[u8PortNum].u16MinimumOperatingCurInmA/DPM_10mA));      
                }
                else
                {
                     /* Form Request message with Capability Mismatch*/
                    gasCfgStatusData.sPerPortData[u8PortNum].u32RDO = \
                            DPM_FORM_DATA_REQUEST((u8SrcPDOIndex + 1), u8CapMismatch,
                            u8SinkGiveBackFlag, DPM_GET_PDO_USB_COMM_CAP(u32SinkPDO),\
                            u8SinkNoUSBSusp, DPM_GET_PDO_CURRENT(u32RcvdSrcPDO),\
                                                DPM_GET_PDO_CURRENT(u32SinkPDO));
                }
                gasDPM[u8PortNum].u16SinkOperatingCurrInmA = DPM_GET_PDO_CURRENT(u32RcvdSrcPDO) * DPM_10mA;
                
                /*Updating the globals with Sink PDO selected */
                gasDPM[u8PortNum].u32NegotiatedPDO = u32RcvdSrcPDO;
                /*Updating the globals with Sink PDO index selected*/
                gasDPM[u8PortNum].u8NegotiatedPDOIndex = (u8SinkPDOIndex+1);
   
                /*Update Negotiated value*/
                gasCfgStatusData.sPerPortData[u8PortNum].u16NegoVoltageInmV = (DPM_GET_PDO_VOLTAGE(u32RcvdSrcPDO) * DPM_PDO_VOLTAGE_UNIT);
                /*Update Negotiated value*/
                gasCfgStatusData.sPerPortData[u8PortNum].u16NegoCurrentInmA = (DPM_GET_PDO_CURRENT(u32RcvdSrcPDO) * DPM_10mA);
                
                /*Set the capability mismatch status*/
                gasCfgStatusData.sPerPortData[u8PortNum].u32PortConnectStatus |= DPM_PORT_SINK_CAPABILITY_MISMATCH_STATUS;

				DPM_SetPortPower (u8PortNum);                
                return;
            }
        }
    }
          
}
#endif

/********************************VCONN Related APIs**********************************************/
UINT8 DPM_Evaluate_VCONN_Swap(UINT8 u8PortNum)
{
    /*As of now, Accept the VCONN Swap without any restriction*/
    return TRUE;   
}
UINT8 DPM_IsPort_VCONN_Source(UINT8 u8PortNum)
{ 
    UINT8 u8IsVCONNSrc;
    if(gasTypeCcontrol[u8PortNum].u8IntStsISR & TYPEC_VCONN_SOURCE_MASK)
    {
        u8IsVCONNSrc =TRUE;
    }
    else
    {
        u8IsVCONNSrc =FALSE;
    }
    return u8IsVCONNSrc;
}

/************************  DPM Renegotiation APIs **********************/
#if (TRUE == INCLUDE_POWER_THROTTLING || (TRUE == INCLUDE_POWER_BALANCING))

void DPM_UpdatePDO(UINT8 u8PortNum, UINT16 u16PowerIn250mW)
{
    float fVoltageInmV = SET_TO_ZERO; 
    UINT16 u16CurrentIn10mA = SET_TO_ZERO; 
    
    /* Load the New PDO Count which is same as Fixed PDO count */
    gasCfgStatusData.sPerPortData[u8PortNum].u8NewPDOCnt = gasCfgStatusData.sPerPortData[u8PortNum].u8SourcePDOCnt; 
    
    for (UINT8 u8Index = INDEX_0; u8Index < gasCfgStatusData.sPerPortData[u8PortNum].u8NewPDOCnt; u8Index++)
    {
        /* Get the voltage value from 32aSourcePDO[7] */
        fVoltageInmV = DPM_GET_VOLTAGE_FROM_PDO_MILLI_V(gasCfgStatusData.sPerPortData[u8PortNum].u32aSourcePDO[u8Index]); 
        
        /* Calculate new current value based on new power */
        u16CurrentIn10mA = ROUND_OFF_FLOAT_TO_INT((float)(((float)u16PowerIn250mW / fVoltageInmV) * (DPM_250mW / DPM_PDO_CURRENT_UNIT))); 
          
        /* In PB, current value of a port should not exceed PORT_MAX_I */ 
        u16CurrentIn10mA = MIN(u16CurrentIn10mA, gasCfgStatusData.sPerPortData[u8PortNum].u16MaxSrcPrtCurrentIn10mA); 
        
        /* Load the New PDO registers with the new PDO values */
        gasCfgStatusData.sPerPortData[u8PortNum].u32aNewPDO[u8Index] = \
                    (gasCfgStatusData.sPerPortData[u8PortNum].u32aSourcePDO[u8Index] & ~(DPM_FIXED_PDO_CURRENT_MASK)) | u16CurrentIn10mA;  

    }
}

#endif 

/********************* DPM API for Port Enable/Disable ******************/
void DPM_EnablePort(UINT8 u8PortNum, UINT8 u8Enable)
{
    if (FALSE == u8Enable)
    {
        /* Disable the port by changing Type C states to 
           TYPEC_DISABLED and TYPEC_DISABLED_ENTRY_SS */
        gasTypeCcontrol[u8PortNum].u8TypeCState = TYPEC_DISABLED;
        gasTypeCcontrol[u8PortNum].u8TypeCSubState = TYPEC_DISABLED_ENTRY_SS;

        /* Change Policy Engine state and sub-state to invalid state */
        gasPolicy_Engine[u8PortNum].ePEState = ePE_INVALIDSTATE;
        gasPolicy_Engine[u8PortNum].ePESubState = ePE_INVALIDSUBSTATE; 
        
        /* Kill all the timers*/
        PDTimer_KillPortTimers (u8PortNum);
    }
    else
    {
        if (TYPEC_DISABLED == gasTypeCcontrol[u8PortNum].u8TypeCState)
        {
            /* Enable the port by changing Type C states to 
               TYPEC_UNATTACHED_SRC and TYPEC_UNATTACHED_SRC_ENTRY_SS  */
            gasTypeCcontrol[u8PortNum].u8TypeCState = TYPEC_UNATTACHED_SRC;
            gasTypeCcontrol[u8PortNum].u8TypeCSubState = TYPEC_UNATTACHED_SRC_ENTRY_SS;        
        }
        else
        {
            /* Do nothing since the port shall only be enabled if it was 
            previously in the disabled state */ 
        }
    }
}
/************************************************************************************************************************/