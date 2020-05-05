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

void DPM_Init(UINT8 u8PortNum)
{
    UINT8 u8DPM_Status = gasDPM[u8PortNum].u8DPM_Status;
    UINT8 u8DPM_ConfigData = gasDPM[u8PortNum].u8DPM_ConfigData;
    
    u8DPM_Status |= (CONFIG_PD_DEFAULT_SPEC_REV << DPM_CURR_PD_SPEC_REV_POS);
    u8DPM_ConfigData |= (CONFIG_PD_DEFAULT_SPEC_REV  << DPM_DEFAULT_PD_SPEC_REV_POS);
        
    if((gasCfgStatusData.sPerPortData[u8PortNum].u32CfgData & TYPEC_PORT_TYPE_MASK)== (PD_ROLE_SOURCE))
    {   
        /* Set Port Power Role as Source in DPM Configure variable*/
        u8DPM_ConfigData |= (PD_ROLE_SOURCE << DPM_DEFAULT_POWER_ROLE_POS); 
        
        /* Set Port Data Role as DFP in DPM Configure variable*/
        u8DPM_ConfigData |= (PD_ROLE_DFP << DPM_DEFAULT_DATA_ROLE_POS);
        
        /* Set Port Power Role as Source in DPM Status variable */
        u8DPM_Status |= (PD_ROLE_SOURCE << DPM_CURR_POWER_ROLE_POS);
        
        /* Set Port Data Role as DFP in DPM Status variable */
        u8DPM_Status |= (PD_ROLE_DFP << DPM_CURR_DATA_ROLE_POS);
        
        /* Set Port Power Role as Source in Port Connection Status register */
        gasCfgStatusData.sPerPortData[u8PortNum].u32PortConnectStatus |= DPM_PORT_POWER_ROLE_STATUS; 
        
        /* Set Port Data Role as DFP in Port Connection Status register */
        gasCfgStatusData.sPerPortData[u8PortNum].u32PortConnectStatus |= DPM_PORT_DATA_ROLE_STATUS; 
        
        /* During initialization, Advertised PDOs and Advertised PDO Count are 
           updated with Default Source PDOs and Source PDO Count */
        (void)MCHP_PSF_HOOK_MEMCPY(gasCfgStatusData.sPerPortData[u8PortNum].u32aAdvertisedPDO, 
                gasCfgStatusData.sPerPortData[u8PortNum].u32aSourcePDO, 
                (gasCfgStatusData.sPerPortData[u8PortNum].u8SourcePDOCnt * 4));
        
        gasCfgStatusData.sPerPortData[u8PortNum].u8AdvertisedPDOCnt = \
                        gasCfgStatusData.sPerPortData[u8PortNum].u8SourcePDOCnt;  

    }       
    else
    {
        /* Set the Default Port Power Role as Sink in DPM Status variable */
        u8DPM_ConfigData |= (PD_ROLE_SINK << DPM_DEFAULT_POWER_ROLE_POS);
        
        /* Set the Default Port Data Role as UFP in DPM Status variable */
        u8DPM_ConfigData |= (PD_ROLE_UFP << DPM_DEFAULT_DATA_ROLE_POS);
        
        /* Set the Current Port Power Role as Sink in DPM Status variable */
        u8DPM_Status |= (PD_ROLE_SINK << DPM_CURR_POWER_ROLE_POS);
        
        /* Set the Current  Port Data Role as UFP in DPM Status variable */
        u8DPM_Status |= (PD_ROLE_UFP << DPM_CURR_DATA_ROLE_POS);
        
        /* Set Port Power Role as Sink in Port Connection Status register */
        gasCfgStatusData.sPerPortData[u8PortNum].u32PortConnectStatus &= ~(DPM_PORT_POWER_ROLE_STATUS); 
        
        /* Set Port Data Role as UFP in Port Connection Status register */
        gasCfgStatusData.sPerPortData[u8PortNum].u32PortConnectStatus &= ~(DPM_PORT_DATA_ROLE_STATUS);         
        /*On initialization Advertised PDO is updated to Sink's PDO*/
        (void)MCHP_PSF_HOOK_MEMCPY(gasCfgStatusData.sPerPortData[u8PortNum].u32aAdvertisedPDO, 
            gasCfgStatusData.sPerPortData[u8PortNum].u32aSinkPDO, 
            DPM_4BYTES_FOR_EACH_PDO_OF(gasCfgStatusData.sPerPortData[u8PortNum].u8SinkPDOCnt));
        /*Advertised PDO Count is updated to SinkPDO Count*/
        gasCfgStatusData.sPerPortData[u8PortNum].u8AdvertisedPDOCnt = \
                        gasCfgStatusData.sPerPortData[u8PortNum].u8SinkPDOCnt;
        
        gasDPM[u8PortNum].u16SinkOperatingCurrInmA = DPM_0mA;
    }
    
    gasDPM[u8PortNum].u8DPM_Status =  u8DPM_Status;
    gasDPM[u8PortNum].u8DPM_ConfigData  = u8DPM_ConfigData;
	
#if (TRUE == INCLUDE_POWER_FAULT_HANDLING)
	gasDPM[u8PortNum].u8VBUSPowerGoodTmrID = MAX_CONCURRENT_TIMERS;
	gasDPM[u8PortNum].u8PowerFaultISR = SET_TO_ZERO;
	gasDPM[u8PortNum].u8VBUSPowerFaultCount = RESET_TO_ZERO;
	gasDPM[u8PortNum].u8HRCompleteWait = RESET_TO_ZERO;
    /*VCONN OCS related variables*/
    gasDPM[u8PortNum].u8VCONNGoodtoSupply = TRUE;
    gasDPM[u8PortNum].u8VCONNPowerGoodTmrID = MAX_CONCURRENT_TIMERS;
    gasDPM[u8PortNum].u8VCONNPowerFaultCount = SET_TO_ZERO;

#endif
	
    
}
/********************************************************************************************/

void DPM_StateMachineInit(void)
{
	for (UINT8 u8PortNum = SET_TO_ZERO; u8PortNum < CONFIG_PD_PORT_COUNT; u8PortNum++)
  	{
        
        if (UPD_PORT_ENABLED == ((gasCfgStatusData.sPerPortData[u8PortNum].u32CfgData \
                                    & TYPEC_PORT_ENDIS_MASK) >> TYPEC_PORT_ENDIS_POS))
        {
		  	/* Init UPD350 GPIO */
		  	UPD_GPIOInit(u8PortNum);
			
            /*Type-C UPD350 register configuration for a port*/
            TypeC_InitPort(u8PortNum);
            
            /* Protocol Layer initialization for all the port present */
            PRL_Init (u8PortNum);
        }
    }
}
/*******************************************************************************/

void DPM_RunStateMachine (UINT8 u8PortNum)
{
    MCHP_PSF_HOOK_DPM_PRE_PROCESS(u8PortNum);
    
    /* Run Type C State machine*/
    TypeC_RunStateMachine (u8PortNum);
    
    /* Run Policy engine State machine*/
    PE_RunStateMachine(u8PortNum);
	
    #if (TRUE == INCLUDE_POWER_THROTTLING)
    PT_RunStateMachine(u8PortNum);
    #endif     

	/* Power Fault handling*/
	#if (TRUE == INCLUDE_POWER_FAULT_HANDLING)
		DPM_PowerFaultHandler(u8PortNum);
	#endif
    
    /* Client Request Handling */
    DPM_ClientRequestHandler(u8PortNum); 
        
    /* UPD Power Management */
    #if (TRUE == INCLUDE_POWER_MANAGEMENT_CTRL)
        UPD_PwrManagementCtrl (u8PortNum);
    #endif
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
            TypeC_ConfigureVBUSThr(u8PortNum, u16VoltageInmV, u16CurrentInmA, TYPEC_CONFIG_NON_PWR_FAULT_THR);            
        }
#if (TRUE == INCLUDE_PD_SOURCE_PPS) 
        else if (DPM_PD_PPS_CONTRACT == DPM_GET_CURRENT_EXPLICIT_CONTRACT(u8PortNum))
        {
            /* VBUS Voltage to be driven for PPS is the voltage 
             requested by Sink in the RDO's Output Voltage field */
           u16VoltageInmV = DPM_GET_OP_VOLTAGE_FROM_PROG_RDO_IN_mV(gasCfgStatusData.sPerPortData[u8PortNum].u32RDO);   
           u16CurrentInmA = DPM_GET_PROG_RDO_OPR_CURRENT_IN_mA(gasCfgStatusData.sPerPortData[u8PortNum].u32RDO); 
           /* Note: For PPS, Configuring the VBUS Threshold is not needed 
              since a PPS Source should not rely on checking the voltage on VBUS. */
        }
#endif 
        else
        {
            /* No support for Battery and Variable power supplies */
        }
        PWRCTRL_SetPortPower (u8PortNum, gasDPM[u8PortNum].u8NegotiatedPDOIndex, 
                u16VoltageInmV, u16CurrentInmA);
    }
    else
    {
        u16VoltageInmV = gasCfgStatusData.sPerPortData[u8PortNum].u16NegoVoltageIn50mV *50; 
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
  UINT8 u8VBUSPresence = ((gasTypeCcontrol[u8PortNum].u8IntStsISR & 
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
    UINT16 u16PrevRDOVoltInmV = SET_TO_ZERO; 
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
        
        if (ePDO_FIXED == (ePDOtypes)DPM_GET_CURRENT_EXPLICIT_CONTRACT(u8PortNum))
        {
            /* If previous request is for a fixed PDO, calculate previous output
               voltage from gasDPM[u8PortNum].u32NegotiatedPDO */
            u16PrevRDOVoltInmV = DPM_GET_VOLTAGE_FROM_PDO_MILLI_V(gasDPM[u8PortNum].u32NegotiatedPDO);
        }
        else if (ePDO_PROGRAMMABLE == (ePDOtypes)DPM_GET_CURRENT_EXPLICIT_CONTRACT(u8PortNum))
        {
            /* If previous request is for a PPS APDO, calculate previous output
               voltage from gasCfgStatusData.sPerPortData[u8PortNum].u32RDO */            
            u16PrevRDOVoltInmV = DPM_GET_OP_VOLTAGE_FROM_PROG_RDO_IN_mV(gasCfgStatusData.sPerPortData[u8PortNum].u32RDO);
        }
        else
        {
            /* Do Nothing */
        }
    }
#endif 
    else
    {
        /* No support for Variable and Battery power supplies */
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
        
        if (ePDO_FIXED == (ePDOtypes)DPM_GET_PDO_TYPE(u32PDO))
        {
            /* Set the current Explicit Contract Type as Fixed Supply */
            gasDPM[u8PortNum].u8DPM_Status &= ~(DPM_CURR_EXPLICIT_CONTRACT_TYPE_MASK); 
            
            gasCfgStatusData.sPerPortData[u8PortNum].u16NegoCurrentIn10mA = u16SrcPDOCurrVal; 
            gasCfgStatusData.sPerPortData[u8PortNum].u16NegoVoltageIn50mV = \
                    ((DPM_GET_VOLTAGE_FROM_PDO_MILLI_V(gasDPM[u8PortNum].u32NegotiatedPDO)) / DPM_PDO_VOLTAGE_UNIT);
            /* Update the allocated power in terms of 250mW */
            gasCfgStatusData.sPerPortData[u8PortNum].u16AllocatedPowerIn250mW =    
                    ((gasCfgStatusData.sPerPortData[u8PortNum].u16NegoVoltageIn50mV * gasCfgStatusData.sPerPortData[u8PortNum].u16NegoCurrentIn10mA) / 
                    (DPM_PDO_VOLTAGE_UNIT * DPM_PDO_CURRENT_UNIT));             
        }
#if (TRUE == INCLUDE_PD_SOURCE_PPS)        
        else if (ePDO_PROGRAMMABLE == (ePDOtypes)DPM_GET_PDO_TYPE(u32PDO))
        {
            /* Set the current Explicit Contract Type as PPS */
            gasDPM[u8PortNum].u8DPM_Status |= DPM_CURR_EXPLICIT_CONTRACT_TYPE_MASK;
            
            /* Determine the timer value that must be used for sending PS_RDY 
               in case of Prog RDO based on the voltage step difference */
            DPM_DeterminePPSPSRdyTimer(u8PortNum, u16PrevRDOVoltInmV, u16RDOOpVoltInmV);    
            
            /* To-do: The above 3 parameters need to be updated for PPS, units 
            differ b/w Fixed and PPS voltages, currents. */
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
UINT32 DPM_CurrentCutDown (UINT32 u32PDO)
{
    /* If PDO max current greater than E-Cable supported current, reset the current value */
    if (ePDO_FIXED == (ePDOtypes)DPM_GET_PDO_TYPE(u32PDO))
    {
        if((u32PDO & PE_MAX_CURR_MASK) > DPM_CABLE_CURR_3A_UNIT)
        {
            u32PDO &= ~PE_MAX_CURR_MASK;
            u32PDO |= DPM_CABLE_CURR_3A_UNIT;
        }  
    }
#if (TRUE == INCLUDE_PD_SOURCE_PPS)    
    else if (ePDO_PROGRAMMABLE == (ePDOtypes)DPM_GET_PDO_TYPE(u32PDO))
    {
        if (DPM_GET_APDO_MAX_CURRENT_IN_mA(u32PDO) > DPM_3000mA)
        {
            u32PDO &= ~(DPM_APDO_MAX_CURRENT_MASK);
            u32PDO |= DPM_3A_IN_50mA; 
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
    
    /* The attached USB-C cable does not support the locally defined Source PDOs  */   
    gasCfgStatusData.sPerPortData[u8PortNum].u32PortConnectStatus |= 
                                        DPM_PORT_CABLE_REDUCED_SRC_CAPABILITIES_STATUS;        

    for (UINT8 u8PDOindex = SET_TO_ZERO; u8PDOindex < u8pSrcPDOCnt; u8PDOindex++)
    {   
        /* Reset the current value to E-Cable supported current */
        pu32DataObj[u8PDOindex] = DPM_CurrentCutDown (pu32SrcCaps[u8PDOindex]);
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
        DPM_CalcSrcCapsFromCurrPTBank(u8PortNum); 
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
                            ~(DPM_PORT_PD_BAL_REDUCED_SRC_CAPABILITIES_STATUS);         
    }   
    else
    {
        /* The advertised PDOs have been reduced from default configured values */
        gasCfgStatusData.sPerPortData[u8PortNum].u32PortConnectStatus |= 
                            DPM_PORT_PD_BAL_REDUCED_SRC_CAPABILITIES_STATUS;                 
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

void DPM_DeterminePPSPSRdyTimer (UINT8 u8PortNum, UINT16 u16PrevRDOVoltInmV, UINT16 u16CurrRDOVoltInmV)
{
    /* Calculate the Voltage difference based on whether the transition is 
       positive or negative */
    UINT16 u16vPpsStepmV = ((u16CurrRDOVoltInmV > u16PrevRDOVoltInmV) ? \
         (u16CurrRDOVoltInmV - u16PrevRDOVoltInmV) : (u16PrevRDOVoltInmV - u16CurrRDOVoltInmV)); 
    
    /* Use tPpsSrcTransLarge for voltage steps larger than vPpsSmallStep */
    if (u16vPpsStepmV > DPM_PPS_VOLTAGE_SMALL_STEP)
    {
        gasDPM[u8PortNum].u8DPM_Status |= DPM_PPS_PS_RDY_TIMER_VAL_MASK;         
    }
    /* Else, use tPpsSrcTransSmall */
    else
    {
        gasDPM[u8PortNum].u8DPM_Status &= ~(DPM_PPS_PS_RDY_TIMER_VAL_MASK);         
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
    for (UINT8 u8Index = INDEX_0; u8Index < PE_STATUS_DATA_BLOCK_SIZE; u8Index++)
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
                gasCfgStatusData.sPerPortData[u8PortNum].u16NegoVoltageIn50mV = DPM_GET_PDO_VOLTAGE(u32SinkPDO);
                /*Update Negotiated value*/
                gasCfgStatusData.sPerPortData[u8PortNum].u16NegoCurrentIn10mA = DPM_GET_PDO_CURRENT(u32SinkPDO);
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
                 gasCfgStatusData.sPerPortData[u8PortNum].u16NegoVoltageIn50mV = DPM_GET_PDO_VOLTAGE(u32SinkPDO);
                /*Update Negotiated value*/
                 gasCfgStatusData.sPerPortData[u8PortNum].u16NegoCurrentIn10mA = u16SinkRDOCurIn10mA;              
                
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
                gasCfgStatusData.sPerPortData[u8PortNum].u16NegoVoltageIn50mV = DPM_GET_PDO_VOLTAGE(u32RcvdSrcPDO);
                /*Update Negotiated value*/
                gasCfgStatusData.sPerPortData[u8PortNum].u16NegoCurrentIn10mA = DPM_GET_PDO_CURRENT(u32RcvdSrcPDO);
                
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

/********************************Power Fault API ******************************/

#if (TRUE == INCLUDE_POWER_FAULT_HANDLING)
static void DPM_ClearPowerfaultFlags(UINT8 u8PortNum)
{
    /*ISR flag is cleared by disabling the interrupt*/
    MCHP_PSF_HOOK_DISABLE_GLOBAL_INTERRUPT();
    gasDPM[u8PortNum].u8PowerFaultISR = SET_TO_ZERO;
    MCHP_PSF_HOOK_ENABLE_GLOBAL_INTERRUPT();
}
void DPM_PowerFaultHandler(UINT8 u8PortNum)
{
  	/* Incase detach reset the Power Fault handling variables*/
    if (((gasTypeCcontrol[u8PortNum].u8TypeCState == TYPEC_UNATTACHED_SRC) &&
		    (gasTypeCcontrol[u8PortNum].u8TypeCSubState == TYPEC_UNATTACHED_SRC_INIT_SS))||
				 ((gasTypeCcontrol[u8PortNum].u8TypeCState == TYPEC_UNATTACHED_SNK) &&
				   (gasTypeCcontrol[u8PortNum].u8TypeCSubState == TYPEC_UNATTACHED_SNK_INIT_SS)))
    {
		/* Enable Fault PIO to detect OCS as it would have been disabled as part of
         Power fault handling*/
        UPD_EnableFaultIn(u8PortNum);
		
		/* Reset Wait for HardReset Complete bit*/
        gasDPM[u8PortNum].u8HRCompleteWait = SET_TO_ZERO;
		
		/* Kill the timer*/
        PDTimer_Kill (gasDPM[u8PortNum].u8VBUSPowerGoodTmrID);
		
		/*Setting the u8VBUSPowerGoodTmrID to MAX_CONCURRENT_TIMERS to indicate that
    	TimerID does not hold any valid timer IDs anymore*/
        gasDPM[u8PortNum].u8VBUSPowerGoodTmrID = MAX_CONCURRENT_TIMERS;
		
		/* Setting the power fault count to Zero */
        if(gasDPM[u8PortNum].u8TypeCErrRecFlag == SET_TO_ZERO)
        {
            gasDPM[u8PortNum].u8VBUSPowerFaultCount = SET_TO_ZERO;
        }
        
        gasDPM[u8PortNum].u8TypeCErrRecFlag = SET_TO_ZERO;
        	
        
        /*******Resetting the VCONN OCS related variables************/
        
        /*Setting VCONNGoodtoSupply flag as true*/        
        gasDPM[u8PortNum].u8VCONNGoodtoSupply = TRUE;
        
        /* Killing the VCONN power good timer*/
        PDTimer_Kill (gasDPM[u8PortNum].u8VCONNPowerGoodTmrID);
        
        /*Setting the u8VCONNPowerGoodTmrID to MAX_CONCURRENT_TIMERS to indicate that
    	TimerID does not hold any valid timer IDs anymore*/
        gasDPM[u8PortNum].u8VCONNPowerGoodTmrID = MAX_CONCURRENT_TIMERS;
        
        /*Resetting the VCONN OCS fault count to Zero */
        gasDPM[u8PortNum].u8VCONNPowerFaultCount = SET_TO_ZERO;
        
        /*ISR flag for OVP,UVP,OCP,VCONN OCS is cleared by disabling the interrupt*/
        MCHP_PSF_HOOK_DISABLE_GLOBAL_INTERRUPT();
        gasDPM[u8PortNum].u8PowerFaultISR = SET_TO_ZERO;
        MCHP_PSF_HOOK_ENABLE_GLOBAL_INTERRUPT();
        
    }	
    if(gasDPM[u8PortNum].u8HRCompleteWait) 
    { 
        if((gasPolicy_Engine[u8PortNum].ePESubState == ePE_SRC_TRANSITION_TO_DEFAULT_POWER_ON_SS) ||
				 (gasPolicy_Engine[u8PortNum].ePEState == ePE_SNK_STARTUP))
        {
            if(gasDPM[u8PortNum].u8VCONNPowerFaultCount >= (gasCfgStatusData.sPerPortData[u8PortNum].u8VCONNMaxFaultCnt))
            {            
                /*Setting the VCONN Good to Supply Flag as False*/
                gasDPM[u8PortNum].u8VCONNGoodtoSupply = FALSE;
            }
            if (gasDPM[u8PortNum].u8VBUSPowerFaultCount >= (gasCfgStatusData.sPerPortData[u8PortNum].u8VBUSMaxFaultCnt))
            {
				/* Disable the receiver*/
                PRL_EnableRx (u8PortNum, FALSE);
				
				/* kill all the timers*/
                PDTimer_KillPortTimers (u8PortNum);
				
				/* set the fault count to zero */
                gasDPM[u8PortNum].u8VBUSPowerFaultCount = SET_TO_ZERO;
				
                DEBUG_PRINT_PORT_STR (u8PortNum, "PWR_FAULT: u8HRCompleteWait Reseted ");
				
                if (DPM_GET_CURRENT_POWER_ROLE(u8PortNum) == PD_ROLE_SOURCE)
                {			
					/* Assign an idle state wait for detach*/
                    gasTypeCcontrol[u8PortNum].u8TypeCSubState = TYPEC_ATTACHED_SRC_IDLE_SS;
                }
                else
                { 
					/* Assign an idle state wait for detach*/
                    gasTypeCcontrol[u8PortNum].u8TypeCSubState = TYPEC_ATTACHED_SNK_IDLE_SS;
                }
				/* Assign an idle state wait for detach*/
                gasPolicy_Engine[u8PortNum].ePEState = ePE_INVALIDSTATE;
                DEBUG_PRINT_PORT_STR (u8PortNum, "PWR_FAULT: Entered SRC/SNK Powered OFF state");
            }
            else
            {
                /* Enable Fault PIO to detect OCS as it would have been disabled as part of
                    Power fault handling*/
                UPD_EnableFaultIn(u8PortNum);
            }
			/* Reset Wait for HardReset Complete bit*/
            gasDPM[u8PortNum].u8HRCompleteWait = SET_TO_ZERO;
			
        }
    }
    if (gasDPM[u8PortNum].u8PowerFaultISR)
    {
        DEBUG_PRINT_PORT_STR(u8PortNum, "DPM Fault Handling");
        /*If VCONN OCS is present , kill the VCONN power good timer*/
        if(gasDPM[u8PortNum].u8PowerFaultISR & DPM_POWER_FAULT_VCONN_OCS)
        {
            if(FALSE == DPM_NotifyClient(u8PortNum, eMCHP_PSF_VCONN_PWR_FAULT))
            {
                /*Clear the Power fault flag and return*/
                DPM_ClearPowerfaultFlags(u8PortNum);
                return;
            }
            /*Kill the VCONN Power fault timer*/
            PDTimer_Kill (gasDPM[u8PortNum].u8VCONNPowerGoodTmrID);
        
             /*Setting the u8VCONNPowerGoodTmrID to MAX_CONCURRENT_TIMERS to indicate that
            TimerID does not hold any valid timer IDs anymore*/
            gasDPM[u8PortNum].u8VCONNPowerGoodTmrID = MAX_CONCURRENT_TIMERS;
			
            DEBUG_PRINT_PORT_STR (u8PortNum, "PWR_FAULT: VCONN Power Fault");
        }
        if(gasDPM[u8PortNum].u8PowerFaultISR & ~DPM_POWER_FAULT_VCONN_OCS)
        { 
            #if(TRUE == INCLUDE_PD_SINK)
            /*Resetting EN_SINK IO status here as the EN_SINK is reset at 
               on detection of fault at ISR itself*/
            gasCfgStatusData.sPerPortData[u8PortNum].u16PortIOStatus &= \
                    ~(DPM_PORT_IO_EN_SINK_STATUS);
            #endif     
            if(FALSE == DPM_NotifyClient(u8PortNum, eMCHP_PSF_VBUS_PWR_FAULT))
            {
                /*Clear the Power fault flag and return*/
                DPM_ClearPowerfaultFlags(u8PortNum);
                return;
            }
             /*Toggle DC_DC EN on VBUS fault to reset the DC-DC controller*/
            PWRCTRL_ConfigDCDCEn(u8PortNum, FALSE);    
            
            
            #if (TRUE == INCLUDE_UPD_PIO_OVERRIDE_SUPPORT)
            /*Clear PIO override enable*/
            UPD_RegByteClearBit (u8PortNum, UPD_PIO_OVR_EN,  UPD_PIO_OVR_2);
            #endif
            
            /* Kill Power Good Timer */
            PDTimer_Kill (gasDPM[u8PortNum].u8VBUSPowerGoodTmrID);
        
            /*Setting the u8VBUSPowerGoodTmrID to MAX_CONCURRENT_TIMERS to indicate that
            TimerID does not hold any valid timer IDs anymore*/
            gasDPM[u8PortNum].u8VBUSPowerGoodTmrID = MAX_CONCURRENT_TIMERS;
			
            DEBUG_PRINT_PORT_STR (u8PortNum, "PWR_FAULT: VBUS Power Fault");
        }
        if(PE_GET_PD_CONTRACT(u8PortNum) == PE_IMPLICIT_CONTRACT)
        {
			/* Set it to Type C Error Recovery */
            gasTypeCcontrol[u8PortNum].u8TypeCState = TYPEC_ERROR_RECOVERY;
            gasTypeCcontrol[u8PortNum].u8TypeCSubState = TYPEC_ERROR_RECOVERY_ENTRY_SS;
            
            gasDPM[u8PortNum].u8TypeCErrRecFlag = 0x01;
						
            /*Increment the fault count*/
            gasDPM[u8PortNum].u8VBUSPowerFaultCount++;
            
            if (gasDPM[u8PortNum].u8VBUSPowerFaultCount >= \
                    gasCfgStatusData.sPerPortData[u8PortNum].u8VBUSMaxFaultCnt)
            {
				/* Disable the receiver*/
                //PRL_EnableRx (u8PortNum, FALSE);
				
				/* kill all the timers*/
                PDTimer_KillPortTimers (u8PortNum);
				
				/* set the fault count to zero */
                gasDPM[u8PortNum].u8VBUSPowerFaultCount = SET_TO_ZERO;
				
                DEBUG_PRINT_PORT_STR (u8PortNum, "PWR_FAULT: u8HRCompleteWait Resetted ");
				
                if (DPM_GET_CURRENT_POWER_ROLE(u8PortNum) == PD_ROLE_SOURCE)
                {			
					/* Assign an idle state wait for detach*/
                    gasTypeCcontrol[u8PortNum].u8TypeCState = TYPEC_ATTACHED_SRC;
                    gasTypeCcontrol[u8PortNum].u8TypeCSubState = TYPEC_ATTACHED_SRC_IDLE_SS;
        
                }
                else
                { 
					/* Assign an idle state wait for detach*/
                    gasTypeCcontrol[u8PortNum].u8TypeCState = TYPEC_ATTACHED_SNK;
                    gasTypeCcontrol[u8PortNum].u8TypeCSubState = TYPEC_ATTACHED_SNK_IDLE_SS;
                }
				/* Assign an idle state wait for detach*/
                //gasPolicy_Engine[u8PortNum].ePEState = ePE_INVALIDSTATE;
                DEBUG_PRINT_PORT_STR (u8PortNum, "PWR_FAULT: Entered SRC/SNK Powered OFF state");
                
                gasDPM[u8PortNum].u8TypeCErrRecFlag = 0x00;
            }
			  
			/* Assign an idle state wait for detach*/
            //gasPolicy_Engine[u8PortNum].ePEState = ePE_INVALIDSTATE;
        }
        else
        {          
            if(gasDPM[u8PortNum].u8PowerFaultISR & DPM_POWER_FAULT_VCONN_OCS)
            {           
                /*Increment the VCONN fault count*/
                gasDPM[u8PortNum].u8VCONNPowerFaultCount++;
                
                /*CC comparator will off once VCONN OCS is detected for implicit contract it is 
                 enabled as part of Type C error recovery. For explicit contract it is enabled here*/
                /*Enabling the CC Sampling on CC1 and CC2 lines*/
                TypeC_ConfigCCComp (u8PortNum, TYPEC_CC_COMP_CTL_CC1_CC2);
            }
            if(gasDPM[u8PortNum].u8PowerFaultISR & ~DPM_POWER_FAULT_VCONN_OCS)
            {
                /*Increment the fault count*/
                gasDPM[u8PortNum].u8VBUSPowerFaultCount++;            
            }
			/* Send Hard reset*/
            PE_SendHardResetMsg(u8PortNum);
			
			/* Set Wait for HardReset Complete bit*/
            gasDPM[u8PortNum].u8HRCompleteWait = gasDPM[u8PortNum].u8PowerFaultISR;
        }
        
        /* Enable DC_DC_EN to drive power*/        
        PWRCTRL_ConfigDCDCEn(u8PortNum, TRUE); 

        #if (TRUE == INCLUDE_UPD_PIO_OVERRIDE_SUPPORT)
        /*Enable PIO override enable*/
        UPD_RegByteSetBit (u8PortNum, UPD_PIO_OVR_EN,  UPD_PIO_OVR_2);
        #endif
            
		/*Clear the Power fault flag*/
        DPM_ClearPowerfaultFlags(u8PortNum);
    }
}
void DPM_VCONNPowerGood_TimerCB (UINT8 u8PortNum, UINT8 u8DummyVariable)
{
	/* Set the timer Id to Max Value*/
 	gasDPM[u8PortNum].u8VCONNPowerGoodTmrID = MAX_CONCURRENT_TIMERS;	
	/* Resetting the VCONN fault Count*/
	gasDPM[u8PortNum].u8VCONNPowerFaultCount = RESET_TO_ZERO;
}

void DPM_HandleVBUSFault(UINT8 u8PortNum)
{
    /**VBUS OCS flag is set for DPM to handle the VBUS fault*/
    MCHP_PSF_HOOK_DISABLE_GLOBAL_INTERRUPT();
    gasDPM[u8PortNum].u8PowerFaultISR |= DPM_POWER_FAULT_VBUS_OCS;
    MCHP_PSF_HOOK_ENABLE_GLOBAL_INTERRUPT(); 
} 
#endif 

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

/************************DPM Notification Handler ******************************/
UINT8 DPM_NotifyClient(UINT8 u8PortNum, eMCHP_PSF_NOTIFICATION eDPMNotification)
{
    UINT8 u8Return = TRUE; 
    
#if (TRUE == INCLUDE_POWER_BALANCING)
    if (TRUE == DPM_IS_PB_ENABLED(u8PortNum))
    {
        u8Return = PB_HandleDPMEvents(u8PortNum, (UINT8)eDPMNotification);
    }
#endif
    
    /* DPM notifications that need to be handled by stack applications must
       be added here before calling the user function. */
    
    u8Return &= MCHP_PSF_NOTIFY_CALL_BACK(u8PortNum, (UINT8)eDPMNotification); 
     
    return u8Return;
}

/************************DPM Client Request Handling APIs ******************************/ 
void DPM_ClearAllClientRequests(UINT8 u8PortNum) 
{
    gasCfgStatusData.sPerPortData[u8PortNum].u8ClientRequest = DPM_CLEAR_ALL_CLIENT_REQ; 
}

void DPM_ClientRequestHandler(UINT8 u8PortNum)
{
    /* Check if at least one request is initiated by any application. This 
       check saves code execution time by letting the control not to check 
       for each if-else condition present inside in case this condition
       is false. */ 
    if (DPM_NO_CLIENT_REQ_PENDING != gasCfgStatusData.sPerPortData[u8PortNum].u8ClientRequest)
    {
        /* Check if Policy Engine is Idle. */
        if(TRUE == PE_IsPolicyEngineIdle(u8PortNum))
        {            
            /* Check for renegotiation request */
            if (DPM_CLIENT_REQ_RENEGOTIATE & gasCfgStatusData.sPerPortData[u8PortNum].u8ClientRequest)
            {
                /* Clear the request since the request is accepted and going to be handled */
                gasCfgStatusData.sPerPortData[u8PortNum].u8ClientRequest &= 
                                          ~(DPM_CLIENT_REQ_RENEGOTIATE);                
                
                /* Enable New PDO Select in DPM Config */
                DPM_EnableNewPDO(u8PortNum, DPM_ENABLE_NEW_PDO);
                
                /* Check for Port Power Role */
                if (DPM_GET_CURRENT_POWER_ROLE(u8PortNum) == PD_ROLE_SOURCE)
                {
                    /* Move the Policy Engine to PE_SRC_SEND_CAPABILITIES state */
                    gasPolicy_Engine[u8PortNum].ePEState = ePE_SRC_SEND_CAPABILITIES;
                    gasPolicy_Engine[u8PortNum].ePESubState = ePE_SRC_SEND_CAP_ENTRY_SS;
                }
                else
                {
                    /* TBD for Sink*/
                }
            }
            /* Check for Get Sink caps request */
            else if (DPM_CLIENT_REQ_GET_SINK_CAPS & gasCfgStatusData.sPerPortData[u8PortNum].u8ClientRequest)
            {
                /* Clear the request since the request is accepted and going to be handled */
                gasCfgStatusData.sPerPortData[u8PortNum].u8ClientRequest &= 
                                          ~(DPM_CLIENT_REQ_GET_SINK_CAPS);                
                
                /* Check for Port Power Role */
                if (DPM_GET_CURRENT_POWER_ROLE(u8PortNum) == PD_ROLE_SOURCE)
                {
                    /* Move the Policy Engine to PE_SRC_GET_SINK_CAP state */
                    gasPolicy_Engine[u8PortNum].ePEState = ePE_SRC_GET_SINK_CAP; 
                    gasPolicy_Engine[u8PortNum].ePESubState = ePE_SRC_GET_SINK_CAP_ENTRY_SS;                    
                }
                else
                {
                    /* TBD for Sink */
                }
            }
            /* Check for Send Alert Request */
            else if (DPM_CLIENT_REQ_SEND_ALERT & gasCfgStatusData.sPerPortData[u8PortNum].u8ClientRequest)
            {
                /* This will be taken care of during PPS implementation */
            }
#if (TRUE == INCLUDE_PD_SOURCE_PPS)
            /* Check for Get Status Request */
            else if (DPM_CLIENT_REQ_GET_STATUS & gasCfgStatusData.sPerPortData[u8PortNum].u8ClientRequest)
            {
                /*To-do: Get_Status initiation is now handled through 
                 client request. It needs to be changed once the strategy
                 to initiate Get_Status is finalized. */
                /* Clear the request since the request is accepted and going to be handled */
                gasCfgStatusData.sPerPortData[u8PortNum].u8ClientRequest &= 
                                          ~(DPM_CLIENT_REQ_GET_STATUS);                
                
                /* Check for Port Power Role */
                if (DPM_GET_CURRENT_POWER_ROLE(u8PortNum) == PD_ROLE_SOURCE)
                {
                    /* Move the Policy Engine to PE_SRC_GET_SINK_CAP state */
                    gasPolicy_Engine[u8PortNum].ePEState = ePE_SRC_GET_SINK_STATUS; 
                    gasPolicy_Engine[u8PortNum].ePESubState = ePE_SRC_GET_SINK_STATUS_ENTRY_SS;                    
                }
                else
                {
                    /* TBD for Sink */
                }                
            }
#endif 
            /* Check for Get Sink Caps Extended Request */
            else if (DPM_CLIENT_REQ_GET_SINK_CAPS_EXTD & gasCfgStatusData.sPerPortData[u8PortNum].u8ClientRequest)
            {
                /* This would be needed in future phases while supporting  
                   Power Balancing on PPS ports */
            }
            else
            {
                /* Do Nothing */
            }
        }
        else
        {
            /* Since Policy Engine is not Idle i.e not in PE_SRC_READY state,
               DPM cannot handle any of the Client Requests. So, clear the 
               flag and send Busy notification, so that the application can 
               re-initiate the request on receiving the Busy notification */
            DPM_ClearAllClientRequests(u8PortNum);
            
            (void)DPM_NotifyClient(u8PortNum, eMCHP_PSF_BUSY); 
        }
        
#if (TRUE == INCLUDE_POWER_FAULT_HANDLING)
        /* Check for VBUS Fault Handling request. Policy Engine Idle check 
           is not needed for this. */
        if (DPM_CLIENT_REQ_HANDLE_VBUS_FAULT & gasCfgStatusData.sPerPortData[u8PortNum].u8ClientRequest)
        {
            /* Call the DPM API that sets the VBUS OCS Flag */
            DPM_HandleVBUSFault(u8PortNum); 
        }
#endif         
    }
}

/************************  DPM Renegotiation APIs **********************/
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
        u16CurrentIn10mA = ROUND_OFF_FLOAT_TO_INT((float)(((float)u16PowerIn250mW / fVoltageInmV) * (PB_POWER_UINTS_MILLI_W / DPM_PDO_CURRENT_UNIT))); 
        
        if (TRUE == DPM_IS_PB_ENABLED(u8PortNum))
        {    
            /* In PB, current value of a port should not exceed PORT_MAX_I */ 
            u16CurrentIn10mA = MIN(u16CurrentIn10mA, gasCfgStatusData.sPBPerPortData[u8PortNum].u16MaxPrtCurrentIn10mA); 
        }
        else
        {
            u16CurrentIn10mA = MIN(u16CurrentIn10mA, (gasCfgStatusData.sPerPortData[u8PortNum].u16MaximumOperatingCurInmA / DPM_PDO_CURRENT_UNIT));
        }
        
        /* Load the New PDO registers with the new PDO values */
        gasCfgStatusData.sPerPortData[u8PortNum].u32aNewPDO[u8Index] = \
                    (gasCfgStatusData.sPerPortData[u8PortNum].u32aSourcePDO[u8Index] & ~(PB_FIXED_PDO_CURRENT_MASK)) | u16CurrentIn10mA;  

    }
}

void DPM_CalcSrcCapsFromCurrPTBank(UINT8 u8PortNum)
{
    /* Get current PT Bank */
    UINT8 u8CurrPTBank = DPM_GET_CURRENT_PT_BANK;
    
    if (PD_THROTTLE_BANK_B == u8CurrPTBank)
    {
        DPM_EnableNewPDO(u8PortNum, DPM_ENABLE_NEW_PDO); 
        DPM_UpdatePDO(u8PortNum, gasCfgStatusData.sPBPerPortData[u8PortNum].u16MaxPrtPwrBankBIn250mW);
    }
    else if (PD_THROTTLE_BANK_C == u8CurrPTBank)
    {
        DPM_EnableNewPDO(u8PortNum, DPM_ENABLE_NEW_PDO); 
        DPM_UpdatePDO(u8PortNum, gasCfgStatusData.sPBPerPortData[u8PortNum].u16MaxPrtPwrBankCIn250mW);        
    }
    else
    {
        /* New PDO should not be enabled for Bank A since the source caps
           would be advertised from u32aSourcePDO[7] array */
    }
}