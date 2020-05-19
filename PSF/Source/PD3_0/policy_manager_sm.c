/*******************************************************************************
  Policy Manager Source file

  Company:
    Microchip Technology Inc.

  File Name:
    policy_manager_sm.c

  Description:
    This file contains DPM State machine APIs
 *******************************************************************************/
/*******************************************************************************
Copyright © [2020] Microchip Technology Inc. and its subsidiaries.

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
	gasDPM[u8PortNum].u8PowerFaultFlags = SET_TO_ZERO;
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

    /* Handle Power Throttling Bank Switching */
    #if (TRUE == INCLUDE_POWER_THROTTLING)
        PT_HandleBankSwitch(u8PortNum);
    #endif  
        
	/* Power Fault handling*/
	#if (TRUE == INCLUDE_POWER_FAULT_HANDLING)
		DPM_PowerFaultHandler(u8PortNum);
	#endif

    /* Client Request Handling */
    DPM_ClientRequestHandler(u8PortNum);
    
    /*Internal Event Handler*/
    DPM_InternalEventHandler(u8PortNum);
        
    /* UPD Power Management */
    #if (TRUE == INCLUDE_POWER_MANAGEMENT_CTRL)
        UPD_PwrManagementCtrl (u8PortNum);
    #endif
}

/*********************************************************************************/
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
		
		/* Kill the timer*/
        PDTimer_Kill (gasDPM[u8PortNum].u8VBUSPowerGoodTmrID);
		
		/*Setting the u8VBUSPowerGoodTmrID to MAX_CONCURRENT_TIMERS to indicate that
    	TimerID does not hold any valid timer IDs anymore*/
        gasDPM[u8PortNum].u8VBUSPowerGoodTmrID = MAX_CONCURRENT_TIMERS;
		
		/* Setting the power fault count to Zero */
        if(FALSE == (gasDPM[u8PortNum].u8PowerFaultFlags & DPM_TYPEC_ERR_RECOVERY_FLAG_MASK))
        {
            gasDPM[u8PortNum].u8VBUSPowerFaultCount = SET_TO_ZERO;
        }
        
        gasDPM[u8PortNum].u8PowerFaultFlags = SET_TO_ZERO;
        	
        
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
        
    } /*end of if detach check*/
	
    if(TRUE == (gasDPM[u8PortNum].u8PowerFaultFlags & DPM_HR_COMPLETE_WAIT_MASK))
    { 
        if((gasPolicy_Engine[u8PortNum].ePESubState == ePE_SRC_TRANSITION_TO_DEFAULT_POWER_ON_SS) ||
				 (gasPolicy_Engine[u8PortNum].ePEState == ePE_SNK_STARTUP))
        {
            /*Checks whether VCONN max power fault count exceeds*/
            if(gasDPM[u8PortNum].u8VCONNPowerFaultCount >= (gasCfgStatusData.sPerPortData[u8PortNum].u8VCONNMaxFaultCnt))
            {            
                /*Setting the VCONN Good to Supply Flag as False*/
                gasDPM[u8PortNum].u8VCONNGoodtoSupply = FALSE;
            }
            
            /*Checks whether VBUS max power fault count exceeds*/
            if (gasDPM[u8PortNum].u8VBUSPowerFaultCount >= (gasCfgStatusData.sPerPortData[u8PortNum].u8VBUSMaxFaultCnt))
            {
				/* Disable the receiver*/
                PRL_EnableRx (u8PortNum, FALSE);
				/* kill all the timers*/
                PDTimer_KillPortTimers (u8PortNum);
				
				/* set the fault count to zero */
                gasDPM[u8PortNum].u8VBUSPowerFaultCount = SET_TO_ZERO;
				
                DEBUG_PRINT_PORT_STR (u8PortNum, "PWR_FAULT: HRCompleteWait Reseted ");
				
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
            }/*end of if condition of VBUS max count exceed check*/
            else
            {
                /* Enable Fault PIO to detect OCS as it would have been disabled as part of
                    Power fault handling*/
                UPD_EnableFaultIn(u8PortNum);
                #if (TRUE == INCLUDE_PD_SOURCE_PPS)
                /*On completion Hard Reset mechanism for VBUS fault initiate an alert message*/
                if (gasDPM[u8PortNum].u8AlertType & (DPM_ALERT_TYPE_OVP | DPM_ALERT_TYPE_OCP))
                {
                    DPM_RegisterInternalEvent(u8PortNum, DPM_INT_EVT_INITIATE_ALERT);
                }
                #endif
            } /*end of else condition of VBUS max count exceed check*/
			/* Reset Wait for HardReset Complete bit*/
            gasDPM[u8PortNum].u8PowerFaultFlags &= (~DPM_HR_COMPLETE_WAIT_MASK);
        }
    } /*end if DPM_HR_COMPLETE_WAIT_MASK check*/
    
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
        } /*end of if condition of VCONN OCS check*/
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
        } /*end of if condition of VBUS Fault check*/
        
        if(PE_GET_PD_CONTRACT(u8PortNum) == PE_IMPLICIT_CONTRACT)
        {
			/* Set it to Type C Error Recovery */
            gasTypeCcontrol[u8PortNum].u8TypeCState = TYPEC_ERROR_RECOVERY;
            gasTypeCcontrol[u8PortNum].u8TypeCSubState = TYPEC_ERROR_RECOVERY_ENTRY_SS;
            
            gasDPM[u8PortNum].u8PowerFaultFlags |= DPM_TYPEC_ERR_RECOVERY_FLAG_MASK;
						
            /*Increment the fault count*/
            gasDPM[u8PortNum].u8VBUSPowerFaultCount++;
            
            if (gasDPM[u8PortNum].u8VBUSPowerFaultCount >= \
                    gasCfgStatusData.sPerPortData[u8PortNum].u8VBUSMaxFaultCnt)
            {
				/* kill all the timers*/
                PDTimer_KillPortTimers (u8PortNum);
				
				/* set the fault count to zero */
                gasDPM[u8PortNum].u8VBUSPowerFaultCount = SET_TO_ZERO;
				
                DEBUG_PRINT_PORT_STR (u8PortNum, "PWR_FAULT: HRCompleteWait Resetted ");
				
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

                DEBUG_PRINT_PORT_STR (u8PortNum, "PWR_FAULT: Entered SRC/SNK Powered OFF state");
                
                gasDPM[u8PortNum].u8PowerFaultFlags &= (~DPM_TYPEC_ERR_RECOVERY_FLAG_MASK);
            }
        } /*end of if condition that checks implicit contract*/
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
                #if (TRUE == INCLUDE_PD_SOURCE_PPS)
                /*If VBUS fault occurs in a Explicit contract, register the alert
                   type and wait for HardReset completion to initiate the Alert message*/
                if (gasDPM[u8PortNum].u8PowerFaultISR & (DPM_POWER_FAULT_UV | DPM_POWER_FAULT_VBUS_OCS))
                {   
                    gasDPM[u8PortNum].u8AlertType |= DPM_ALERT_TYPE_OCP;
                    gasDPM[u8PortNum].u8StatusEventFlags |= DPM_EVENT_TYPE_OCP;
                }
                else if (gasDPM[u8PortNum].u8PowerFaultISR & DPM_POWER_FAULT_OVP)
                {
                    gasDPM[u8PortNum].u8AlertType |= DPM_ALERT_TYPE_OVP;
                    gasDPM[u8PortNum].u8StatusEventFlags |= DPM_EVENT_TYPE_OVP;
                }
               #endif /*INCLUDE_PD_SOURCE_PPS*/
                /*Increment the fault count*/
                gasDPM[u8PortNum].u8VBUSPowerFaultCount++;            
            }
			/* Send Hard reset*/
            PE_SendHardResetMsg(u8PortNum);
			
			/* Set Wait for HardReset Complete bit*/
            gasDPM[u8PortNum].u8PowerFaultFlags |= DPM_HR_COMPLETE_WAIT_MASK;
        } /* end of else part of implicit contract check*/
        
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

void DPM_HandleExternalVBUSFault(UINT8 u8PortNum, UINT8 u8FaultType)
{
    /*Set power fault due to external source only  if power fault is not
     detected by internal mechanism */
    if (!gasDPM[u8PortNum].u8PowerFaultISR)
    {
        #if (TRUE ==  INCLUDE_PD_SOURCE)
        /*Disable VBUS_EN on detection of external fault*/
        UPD_GPIOUpdateOutput(u8PortNum, gasCfgStatusData.sPerPortData[u8PortNum].u8Pio_EN_VBUS, 
                gasCfgStatusData.sPerPortData[u8PortNum].u8Mode_EN_VBUS, (UINT8)UPD_GPIO_DE_ASSERT);
        #endif
        /*VBUS OCS flag is set for DPM to handle the VBUS fault*/
        MCHP_PSF_HOOK_DISABLE_GLOBAL_INTERRUPT();
        gasDPM[u8PortNum].u8PowerFaultISR |= u8FaultType;
        MCHP_PSF_HOOK_ENABLE_GLOBAL_INTERRUPT(); 
    }
} 
#endif 


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

    #if (TRUE == INCLUDE_POWER_THROTTLING)
    /* Busy is the only notification applicable for PT */
    if (eMCHP_PSF_BUSY == eDPMNotification)
    {
        PT_HandleDPMBusy(u8PortNum); 
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
    gasCfgStatusData.sPerPortData[u8PortNum].u32ClientRequest = DPM_CLEAR_ALL_CLIENT_REQ; 
}

void DPM_ClientRequestHandler(UINT8 u8PortNum)
{
    /* Check if at least one request is initiated by any application. This 
       check saves code execution time by letting the control not to check 
       for each if-else condition present inside in case this condition
       is false. */ 
    if (DPM_NO_CLIENT_REQ_PENDING == gasCfgStatusData.sPerPortData[u8PortNum].u32ClientRequest)
    {
        return;
    }
    
    /* Check if Policy Engine is Idle. */
    if(TRUE == PE_IsPolicyEngineIdle(u8PortNum))
    {            
        /* Check for renegotiation request */
        if (DPM_CLIENT_REQ_RENEGOTIATE & gasCfgStatusData.sPerPortData[u8PortNum].u32ClientRequest)
        {
            /* Clear the request since the request is accepted and going to be handled */
            gasCfgStatusData.sPerPortData[u8PortNum].u32ClientRequest &= 
                                      ~(DPM_CLIENT_REQ_RENEGOTIATE);                
            
#if (TRUE == INCLUDE_POWER_THROTTLING)
            /* Update the renegotiation request status as accepted
               if it was initiated by PT Mngr */
            if (ePT_RENEG_REQ_INITIATED == gasPTPortParam[u8PortNum].ePTRenegSts)
            {
                gasPTPortParam[u8PortNum].ePTRenegSts = ePT_RENEG_REQ_ACCEPTED;
            }
#endif 
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
        } /*DPM_CLIENT_REQ_RENEGOTIATE*/
        /* Check for Get Sink caps request */
        else if (DPM_CLIENT_REQ_GET_SINK_CAPS & gasCfgStatusData.sPerPortData[u8PortNum].u32ClientRequest)
        {
            /* Clear the request since the request is accepted and going to be handled */
            gasCfgStatusData.sPerPortData[u8PortNum].u32ClientRequest &= 
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
                /*When role is sink Get_Sink_Caps request is not applicable*/
            }
        }/*DPM_CLIENT_REQ_GET_SINK_CAPS*/
        /* Check for Get Sink Caps Extended Request */
        else if (DPM_CLIENT_REQ_GET_SINK_CAPS_EXTD & gasCfgStatusData.sPerPortData[u8PortNum].u32ClientRequest)
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
    } /*else part of PE engine idle check*/

#if (TRUE == INCLUDE_POWER_FAULT_HANDLING)
    /* Check for VBUS Fault Handling request. Policy Engine Idle check 
       is not needed for this has fault has to be handled with highest priority*/
    if (DPM_CLIENT_REQ_HANDLE_FAULT_VBUS_OV & gasCfgStatusData.sPerPortData[u8PortNum].u32ClientRequest)
    {
        /* Call the DPM API that sets the VBUS OV flag*/
        DPM_HandleExternalVBUSFault(u8PortNum, DPM_POWER_FAULT_OVP); 
    }
    else if (DPM_CLIENT_REQ_HANDLE_FAULT_VBUS_OCS & gasCfgStatusData.sPerPortData[u8PortNum].u32ClientRequest)
    {
        /*Inform DPM to handle VBUS OCS only if it is Fixed supply else it is operating condition
          change in case of PPS supply*/
        #if (TRUE == INCLUDE_PD_SOURCE_PPS)
        if (DPM_PD_PPS_CONTRACT == DPM_GET_CURRENT_EXPLICIT_CONTRACT(u8PortNum))
        {
            /*Operating condition change  as Alert Type and OMF is set to Current Limit mode*/
            gasDPM[u8PortNum].u8AlertType |= DPM_ALERT_TYPE_OPR_COND_CHANGE;
            gasDPM[u8PortNum].u8StatusEventFlags |= DPM_EVENT_TYPE_CL_MODE;
            gasDPM[u8PortNum].u8RealTimeFlags |= DPM_REAL_TIME_FLAG_OMF_IN_CL_MODE;
            /*Initiate Alert message*/
            DPM_RegisterInternalEvent(u8PortNum, DPM_INT_EVT_INITIATE_ALERT);
            
        }
        else
        #endif /*INCLUDE_PD_SOURCE_PPS*/
        {
            /*Fixed supply*/
            /* Call the DPM API to handle external VBUS fault */
            DPM_HandleExternalVBUSFault(u8PortNum, DPM_POWER_FAULT_VBUS_OCS);
        }   
    }
    else if (DPM_CLIENT_REQ_HANDLE_VBUS_OCS_EXIT & gasCfgStatusData.sPerPortData[u8PortNum].u32ClientRequest)
    {
        #if (TRUE == INCLUDE_PD_SOURCE_PPS)
        if (DPM_PD_PPS_CONTRACT == DPM_GET_CURRENT_EXPLICIT_CONTRACT(u8PortNum))
        {
            /*Operating condition change  as Alert Type and OMF is set to Current Limit mode*/
            gasDPM[u8PortNum].u8AlertType |= DPM_ALERT_TYPE_OPR_COND_CHANGE;
            gasDPM[u8PortNum].u8StatusEventFlags &= ~DPM_EVENT_TYPE_CL_CV_MODE_MASK;
            gasDPM[u8PortNum].u8RealTimeFlags &= ~DPM_REAL_TIME_FLAG_OMF_FIELD_MASK;
            /*Initiate Alert message*/
            DPM_RegisterInternalEvent(u8PortNum, DPM_INT_EVT_INITIATE_ALERT);
        }
        else
        {
            /*no need to handle of fixed supply*/
        }
        #endif
    }
#endif         
}

/**************************************************************************************/
void DPM_RegisterInternalEvent(UINT8 u8PortNum, UINT8 u8EventType)
{
    gasDPM[u8PortNum].u8DPMInternalEvents |= u8EventType;
}

void DPM_InternalEventHandler(UINT8 u8PortNum)
{
    if (gasDPM[u8PortNum].u8DPMInternalEvents)
    {
        if (DPM_INT_EVT_INFORM_DETACH == (gasDPM[u8PortNum].u8DPMInternalEvents\
                                                    & DPM_INT_EVT_INFORM_DETACH))
        {
            /*When detach event is notified, reset the Internal events*/
            gasDPM[u8PortNum].u8DPMInternalEvents = RESET_TO_ZERO;
            #if (TRUE == INCLUDE_PD_SOURCE_PPS)
            /*Kill the DPM_STATUS_FAULT_PERSIST_TIMEOUT_MS timer*/
            PDTimer_Kill(gasDPM[u8PortNum].u8StsClearTmrID);
            /*Clear all the status registers*/
            gasDPM[u8PortNum].u8AlertType = SET_TO_ZERO;
            gasDPM[u8PortNum].u8StatusEventFlags = SET_TO_ZERO;
            gasDPM[u8PortNum].u8PowerStatus = SET_TO_ZERO;
            gasDPM[u8PortNum].u8RealTimeFlags = SET_TO_ZERO;
            /* Set the timer Id to Max Concurrent Value*/
            gasDPM[u8PortNum].u8StsClearTmrID = MAX_CONCURRENT_TIMERS;
            
            #endif
        }
        /* Initiate a sequence only when the Policy Engine is in PS_RDY state*/
        if(TRUE == PE_IsPolicyEngineIdle(u8PortNum))
        {
            #if (TRUE == INCLUDE_PD_SOURCE_PPS)
            if (DPM_INT_EVT_INITIATE_ALERT == (gasDPM[u8PortNum].u8DPMInternalEvents\
                                                    & DPM_INT_EVT_INITIATE_ALERT))
            {
                /*Assign Policy engine state to initiate Alert*/
                //TBD

                /*Clear the Internal event since it is processed*/
                gasDPM[u8PortNum].u8DPMInternalEvents &= ~DPM_INT_EVT_INITIATE_ALERT;
                
                /*start the DPM_STATUS_FAULT_PERSIST_TIMEOUT_MS to clear the status flag
                  on timeout if there is no request for status*/
                gasDPM[u8PortNum].u8StsClearTmrID = PDTimer_Start (DPM_STATUS_FAULT_PERSIST_TIMEOUT_MS,\
                                                          DPM_StatusFaultPersist_TimerCB, u8PortNum, (UINT8)SET_TO_ZERO);
                
            }
            else if (DPM_INT_EVT_INITIATE_GET_STATUS == (gasDPM[u8PortNum].u8DPMInternalEvents\
                                                    & DPM_INT_EVT_INITIATE_GET_STATUS))
            {
                /*Assign Policy engine state to initiate Get_Status */
                //TBD
                /*Clear the Internal event since it is processed*/
                gasDPM[u8PortNum].u8DPMInternalEvents &= ~DPM_INT_EVT_INITIATE_GET_STATUS;
            }
            #endif /*INCLUDE_PD_SOURCE_PPS*/
        } 
    }
}

/*************************************************************************************/