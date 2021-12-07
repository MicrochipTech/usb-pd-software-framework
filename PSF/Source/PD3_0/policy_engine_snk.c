/*******************************************************************************
  PD Sink Policy Engine Source file

  Company:
    Microchip Technology Inc.

  File Name:
    policy_engine_snk.c

  Description:
    This file contains the function definitions for Sink Policy Engine state-machine
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

#if (TRUE == INCLUDE_PD_SINK)

void PE_RunSnkStateMachine (UINT8 u8PortNum, UINT8 *pu8DataBuf, UINT32 u32Header)
{
    UINT8 u8TypeCState = TYPEC_INVALID_STATE;
    UINT8 u8TypeCSubState = TYPEC_INVALID_SUBSTATE;
    UINT32 u32aDataObj[PRL_MAX_DATA_OBJ_COUNT] = {SET_TO_ZERO};
	UINT16 u16TransmitHeader = SET_TO_ZERO;
	PRLTxCallback pfnTransmitCB = PE_StateChange_TransmitCB;
	UINT32 u32TransmitTmrIDTxSt = SET_TO_ZERO;
	UINT8 u8IsTransmit = FALSE;
    
    /*Getting Type-C state and Type-C Sub State from DPM to find Type C detach event*/
    DPM_GetTypeCStates (u8PortNum, &u8TypeCState, &u8TypeCSubState);
    
    /*If the Device is detached, set the Policy Engine State to PE_SNK_STARTUP */
    if (((TYPEC_UNATTACHED_SNK == u8TypeCState) || ((TYPEC_ATTACHWAIT_SNK == u8TypeCState))) && \
        ((gasPolicyEngine[u8PortNum].ePEState != ePE_SNK_STARTUP) \
        && (gasPolicyEngine[u8PortNum].ePEState != ePE_SNK_DISCOVERY)))
    {
        /*Reset the HardResetCounter*/
        gasPolicyEngine[u8PortNum].u8HardResetCounter = RESET_TO_ZERO;
        gasPolicyEngine[u8PortNum].ePEState = ePE_SNK_STARTUP;
        
        /* Set cable spec revision to default spec revision on every detach */
        DPM_UpdatePDSpecRev (u8PortNum, CONFIG_PD_DEFAULT_SPEC_REV, PRL_SOP_P_TYPE);        
    }
  
    switch (gasPolicyEngine[u8PortNum].ePEState)
    {
        case ePE_SNK_STARTUP:
        {         
            //DEBUG_PRINT_PORT_STR (u8PortNum,"PE_SNK_STARTUP\r\n");
 
            /*Reset the Protocol Layer */
            PRL_ResetProtocolLayer (u8PortNum);
             
            /*Clearing the Hard Reset IN Progress bit here because source detach
            can occur while waiting in ePE_SNK_TRANSITION_TO_DEFAULT_WAIT_SS*/
            gasPolicyEngine[u8PortNum].u8PEPortSts &= ~PE_HARDRESET_PROGRESS_MASK;

            /*Explicit Contract becomes invalid once this state is reached 
            from Hard Reset or Initial Power up*/
            gasPolicyEngine[u8PortNum].u8PEPortSts &= (~PE_EXPLICIT_CONTRACT);
        
            /*Reset the all the sink status set*/
            gasCfgStatusData.sPerPortData[u8PortNum].u32PortConnectStatus &= \
                        ~(DPM_PORT_SINK_CAPABILITY_MISMATCH_STATUS |
                            DPM_PORT_AS_SNK_LAST_REQ_PS_RDY_STATUS |
                            DPM_PORT_AS_SNK_LAST_REQ_ACCEPT_STATUS |
                            DPM_PORT_AS_SNK_LAST_REQ_REJECT_STATUS);           
            
            #if (FALSE != INCLUDE_PDFU)
            if ((FALSE != gsPdfuInfo.u8IsPDFUActive) && (u8PortNum == gsPdfuInfo.u8PDFUPortNum))
            {
                PE_FwUpdtInitialize();
            }
            #endif

            gasPolicyEngine[u8PortNum].ePEState = ePE_SNK_DISCOVERY;
            
            break;
        }            

        case ePE_SNK_DISCOVERY:
        {
            /*This State will be useful in getting the VBUS status, if the state machine comes 
            from ePE_SNK_TRANSITION_TO_DEFAULT state*/
    
            /*Query the Device policy manager for VBUS of 5V Presence*/
            if ((TYPEC_VBUS_5V == DPM_GetVBUSVoltage (u8PortNum)) && \
                (TYPEC_ATTACHED_SNK == u8TypeCState))
            {
                //DEBUG_PRINT_PORT_STR (u8PortNum,"PE_SNK_DISCOVERY\r\n");
                
			  	/* Enable Power fault thresholds for TYPEC_VBUS_5V to detect Power faults*/
                TypeC_ConfigureVBUSThr (u8PortNum, TYPEC_VBUS_5V, \
                    gasDPM[u8PortNum].u16SinkOperatingCurrInmA, TYPEC_CONFIG_PWR_FAULT_THR);
				
                gasPolicyEngine[u8PortNum].ePEState = ePE_SNK_WAIT_FOR_CAPABILITIES;
                gasPolicyEngine[u8PortNum].ePESubState = ePE_SNK_WAIT_FOR_CAPABILITIES_ENTRY_SS;
            }
            else
            {
                /* Hook to notify PE state machine entry into idle sub-state */
                MCHP_PSF_HOOK_NOTIFY_IDLE(u8PortNum, eIDLE_PE_NOTIFY);
            }
            break;                
        }
           
        case ePE_SNK_WAIT_FOR_CAPABILITIES:
        {
            switch (gasPolicyEngine[u8PortNum].ePESubState)
            {
                case ePE_SNK_WAIT_FOR_CAPABILITIES_ENTRY_SS:
                {                   
                    //DEBUG_PRINT_PORT_STR (u8PortNum,"PE_SNK_WAIT_FOR_CAPABILITIES_ENTRY_SS\r\n");
                    
                    if (gasPolicyEngine[u8PortNum].u8HardResetCounter <= PE_N_HARD_RESET_COUNT)
                    {
                        /*Start the PE_SINKWAITCAP_TIMEOUT_MS to wait for the source 
                        capability message*/
                        /*Set the timer callback to transition to 
                        ePE_SNK_HARD_RESET and ePE_SNK_HARD_RESET_SEND_SS sub state if timeout happens*/
                        gasPolicyEngine[u8PortNum].u8PETimerID = PDTimer_Start (\
                                                                 PE_SINKWAITCAP_TIMEOUT_MS,\
                                                                 PE_SSChngAndTimeoutValidate_TimerCB,\
                                                                 u8PortNum, \
                                                                 (UINT8)ePE_SNK_HARD_RESET_SEND_SS);
                    }
                    else 
                    {
                        /*if u8HardResetCounter is greater than PE_N_HARD_RESET_COUNT*/
                        /*Stay in PE_SNK_Wait_for_Capabilities State if HardReset Counter Overflowed*/
                        /*Update EN_SINK based on implicit current from source 
                          and Wait for Source capability message*/
                        PWRCTRL_ConfigEnSink (u8PortNum, TRUE);
                    }

                    gasPolicyEngine[u8PortNum].ePESubState = ePE_SNK_WAIT_FOR_CAPABILITIES_WAIT_SS;                    
                    
                    break;
                }
                /*Wait in this sub-state for the reception of Source capability message 
                from source partner*/    
                case ePE_SNK_WAIT_FOR_CAPABILITIES_WAIT_SS:
                {
                    /* Hook to notify PE state machine entry into idle sub-state */
                    MCHP_PSF_HOOK_NOTIFY_IDLE(u8PortNum, eIDLE_PE_NOTIFY);
                    
                    break; 
                }
                default:
                {
                    break;
                }
            }
            break;
        }
        /*This State is set by the PE_ReceiveMsgHandler API if the source capability message has been received*/    
        case ePE_SNK_EVALUATE_CAPABILITY:
        {            
            //DEBUG_PRINT_PORT_STR (u8PortNum,"PE_SNK_EVALUATE_CAPABILITY\r\n");
              
            if (gasDPM[u8PortNum].u16InternalEvntInProgress != DPM_INT_EVT_INITIATE_RENEGOTIATION)
            {
                /* Notify the new source capability is received*/
                (void)DPM_NotifyClient (u8PortNum, eMCHP_PSF_NEW_SRC_CAPS_RCVD);
            }

            /*Reset the HardResetCounter*/
            gasPolicyEngine[u8PortNum].u8HardResetCounter = RESET_TO_ZERO;	                        
            
            /*Ask the Device policy manager to evaluate the received source capability message*/
            DPM_EvaluateRcvdSrcCaps (u8PortNum, (UINT16) u32Header, (UINT32*)pu8DataBuf);

            /*Invalid Source Capability Message results in Sink request object count to be 0*/
            if (SET_TO_ZERO == gasCfgStatusData.sPerPortData[u8PortNum].u32RDO)
            {
                /*Transition to Ready state if already in PD contract*/
                if (PE_EXPLICIT_CONTRACT == (gasPolicyEngine[u8PortNum].u8PEPortSts \
                        & PE_PDCONTRACT_MASK))
                {
                     gasPolicyEngine[u8PortNum].ePEState = ePE_SNK_READY;
                     gasPolicyEngine[u8PortNum].ePESubState = ePE_SNK_READY_END_AMS_SS;
                }
                 /*Transition to sink wait for capabilities state if not in PD contract*/
                else
                {
                    gasPolicyEngine[u8PortNum].ePEState = ePE_SNK_WAIT_FOR_CAPABILITIES;
                    gasPolicyEngine[u8PortNum].ePESubState = ePE_SNK_WAIT_FOR_CAPABILITIES_ENTRY_SS;
                }			
            }
            /*Valid Source capability results in transition to ePE_SNK_SELECT_CAPABILITY state*/
            else
            {     
                gasPolicyEngine[u8PortNum].ePEState = ePE_SNK_SELECT_CAPABILITY;
                gasPolicyEngine[u8PortNum].ePESubState = ePE_SNK_SELECT_CAPABILITY_SEND_REQ_SS;            
            }
            break;
        }           
        case ePE_SNK_SELECT_CAPABILITY:
        {
            switch (gasPolicyEngine[u8PortNum].ePESubState)
            {
                case ePE_SNK_SELECT_CAPABILITY_SEND_REQ_SS:
                {                 
                    //DEBUG_PRINT_PORT_STR (u8PortNum,"PE_SNK_SELECT_CAPABILITY_SEND_REQ_SS\r\n");

                    /*Set the PD message transmitter API variables to send Sink Data request Message*/                    
                    u16TransmitHeader = PRL_FormSOPTypeMsgHeader (u8PortNum, PE_DATA_REQUEST,\
                                            PE_OBJECT_COUNT_1, PE_NON_EXTENDED_MSG);
                    
                    u32aDataObj[INDEX_0] = gasCfgStatusData.sPerPortData[u8PortNum].u32RDO;                    
                    
                    /*Set the transmitter callback to transition to Soft reset state if
                    message transmission fails*/
                    u32TransmitTmrIDTxSt = PRL_BUILD_PKD_TXST_U32(ePE_SNK_SELECT_CAPABILITY,\
                                             ePE_SNK_SELECT_CAPABILITY_REQ_SENT_SS,\
                                             ePE_SEND_SOFT_RESET,\
                                             ePE_SEND_SOFT_RESET_ENTRY_SS );
                    
                    u8IsTransmit = TRUE;
                    
                    gasPolicyEngine[u8PortNum].ePESubState = ePE_SNK_SELECT_CAPABILITY_SEND_REQ_IDLE_SS;
                    
                    break;                                   
                }
                 /*Wait here until the Sink data request message is sent*/
                case ePE_SNK_SELECT_CAPABILITY_SEND_REQ_IDLE_SS:
                {    
                    /* Hook to notify PE state machine entry into idle sub-state */
                    MCHP_PSF_HOOK_NOTIFY_IDLE(u8PortNum, eIDLE_PE_NOTIFY);
                    
                    break;
                }
                    
                case ePE_SNK_SELECT_CAPABILITY_REQ_SENT_SS:
                {
                    /*Start the PE_SENDERRESPONSE_TIMEOUT_MS for the Sink Data request message sent*/
                    /*Set the timer callback to transition to 
                    ePE_SNK_HARD_RESET state and ePE_SNK_HARD_RESET_SEND_SS sub state if timeout happens*/
                    gasPolicyEngine[u8PortNum].u8PETimerID = PDTimer_Start (\
                                                              PE_SENDERRESPONSE_TIMEOUT_MS,\
                                                              PE_SSChngAndTimeoutValidate_TimerCB,\
                                                              u8PortNum,(UINT8) ePE_SNK_HARD_RESET_SEND_SS);
                    
                    gasPolicyEngine[u8PortNum].ePESubState = ePE_SNK_SELECT_CAPABILITY_WAIT_FOR_ACCEPT_SS;
                    
                    break;
                }
                
                /*Wait here for the accept message from Source for the sink data request*/
                case ePE_SNK_SELECT_CAPABILITY_WAIT_FOR_ACCEPT_SS:
                {
                    /* Hook to notify PE state machine entry into idle sub-state */
                    MCHP_PSF_HOOK_NOTIFY_IDLE(u8PortNum, eIDLE_PE_NOTIFY);
                    
                    break;
                }
                default:
                {
                    break;
                }
            }
            break;
        }
        /*This State is set by the PE_ReceiveMsgHandler API if the accept message has been received*/
        case ePE_SNK_TRANSITION_SINK:
        {
            switch (gasPolicyEngine[u8PortNum].ePESubState)
            {
                case ePE_SNK_TRANSITION_SINK_ENTRY_SS:
                {                    
                    //DEBUG_PRINT_PORT_STR (u8PortNum,"PE_SNK_TRANSITION_SINK_ENTRY_SS\r\n");                                                       
                    
                    /* Configure VBUS Threshold for the requested PDO */
                    TypeC_ConfigureVBUSThr (u8PortNum, \
                            gasCfgStatusData.sPerPortData[u8PortNum].u16NegoVoltageInmV, \
                                gasDPM[u8PortNum].u16SinkOperatingCurrInmA, \
                                    TYPEC_CONFIG_NON_PWR_FAULT_THR);                    
                
                    /* Requested current controlling */
                    PWRCTRL_ConfigSinkHW (u8PortNum, \
                            DPM_GET_VOLTAGE_FROM_PDO_MILLI_V(gasDPM[u8PortNum].u32NegotiatedPDO), \
                                gasDPM[u8PortNum].u16SinkOperatingCurrInmA);
                    
                    /*Initialize and run PE_PSTRANSITION_TIMEOUT_MS*/
                    /*Set the timer callback to transition to 
                    ePE_SNK_HARD_RESET and ePE_SNK_HARD_RESET_SEND_SS sub state if timeout happens*/
                    gasPolicyEngine[u8PortNum].u8PETimerID = PDTimer_Start (\
                                                              PE_PSTRANSITION_TIMEOUT_MS,\
                                                              PE_SSChngAndTimeoutValidate_TimerCB,\
                                                              u8PortNum, (UINT8)ePE_SNK_HARD_RESET_SEND_SS);
                    
                    gasPolicyEngine[u8PortNum].ePESubState = ePE_SNK_TRANSITION_SINK_WAIT_FOR_PSRDY_SS;
                    
                    break;
                }              
                /*Wait here for the PS Ready message from Source for the sink data request*/
                case ePE_SNK_TRANSITION_SINK_WAIT_FOR_PSRDY_SS:
                {
                    /* Hook to notify PE state machine entry into idle sub-state */
                    MCHP_PSF_HOOK_NOTIFY_IDLE(u8PortNum, eIDLE_PE_NOTIFY);
                    
                    break;
                }
                default:
                {
                    break;
                }               
            }
            break;
        }
        
        /*This State is set by the PE_ReceiveMsgHandler API if the PS_RDY message has been received*/
        case ePE_SNK_READY:
        {
            switch (gasPolicyEngine[u8PortNum].ePESubState)
            {
                case ePE_SNK_READY_ENTRY_SS:
                {                    
                    //DEBUG_PRINT_PORT_STR (u8PortNum,"PE_SNK_READY_ENTRY_SS\r\n");
					
                    /* Configure threshold to detect faults*/
                   	DPM_EnablePowerFaultDetection (u8PortNum);
					
                    /*Setting the explicit contract as True*/
                    gasPolicyEngine[u8PortNum].u8PEPortSts |= (PE_EXPLICIT_CONTRACT);                    
                                    
                    /*Set EN_SINK*/
                    PWRCTRL_ConfigEnSink (u8PortNum, TRUE);

                    gasPolicyEngine[u8PortNum].ePESubState = ePE_SNK_READY_END_AMS_SS;
                    
                    /*Notify that contract is established*/
                    (void)DPM_NotifyClient (u8PortNum, eMCHP_PSF_PD_CONTRACT_NEGOTIATED);
                                        
                    if (DPM_PORT_SINK_CAPABILITY_MISMATCH_STATUS & \
                            gasCfgStatusData.sPerPortData[u8PortNum].u32PortConnectStatus)
                    {
                        /* Notify the capability mismatch*/
                        (void)DPM_NotifyClient (u8PortNum, eMCHP_PSF_CAPS_MISMATCH);
                    }
                    
                    break;
                }
                
                case ePE_SNK_READY_END_AMS_SS:
                {                 
                    //DEBUG_PRINT_PORT_STR (u8PortNum,"PE_SNK_READY_END_AMS_SS\r\n");
                    
                    /* Request DPM to initiate internal events*/
                    DPM_InitiateInternalEvts (u8PortNum);
                    
                    gasDPM[u8PortNum].u16InternalEvntInProgress = RESET_TO_ZERO;                        

                    gasPolicyEngine[u8PortNum].ePESubState = ePE_SNK_READY_IDLE_SS;                                                           
                                        
                    break;
                }
                
                case ePE_SNK_READY_IDLE_SS:
                {
                    /* Hook to notify PE state machine entry into idle sub-state */
                    MCHP_PSF_HOOK_NOTIFY_IDLE(u8PortNum, eIDLE_PE_NOTIFY);
                    
                    break;
                }
                default:
                {
                    break;
                }               
            }
            break;
        }
        /*This State is entered if hard reset is to be transmitted to the port partner*/   
        case ePE_SNK_HARD_RESET:
        {
           switch (gasPolicyEngine[u8PortNum].ePESubState)
           {           
               case ePE_SNK_HARD_RESET_SEND_SS:
               {                     
                    //DEBUG_PRINT_PORT_STR (u8PortNum,"PE_SNK_HARD_RESET_SEND_SS\r\n");

                    /*Kill All the Active timers of policy engine for the port*/
                    PE_KillPolicyEngineTimer (u8PortNum);
                    
                    /*Setting the Hard Reset IN progress bit to avoid VBUS discharge
                    when VBUS drops below VSinkDisconnnect*/
                    gasPolicyEngine[u8PortNum].u8PEPortSts |= PE_HARDRESET_PROGRESS_MASK;                   
                    
                    /* API to send HardReset is called*/
                    PRL_SendCableorHardReset (u8PortNum, PRL_SEND_HARD_RESET,\
                                             NULL, SET_TO_ZERO);
                    
                    /*Increment HardReset Counter*/
                    gasPolicyEngine[u8PortNum].u8HardResetCounter++; 
                    
                    gasPolicyEngine[u8PortNum].ePEState = ePE_SNK_TRANSITION_TO_DEFAULT;
                    gasPolicyEngine[u8PortNum].ePESubState = ePE_SNK_TRANSITION_TO_DEFAULT_ENTRY_SS;            
                    
                    break;
               }
               default:
               {
                    break;
               }
            
            }
            break;               
        }
        /*This state is entered after the completion of Hard reset signal sent or 
        if any hard reset signal is received from port partner*/
        case ePE_SNK_TRANSITION_TO_DEFAULT:
        {            
           switch (gasPolicyEngine[u8PortNum].ePESubState)
           {                  
               case ePE_SNK_TRANSITION_TO_DEFAULT_ENTRY_SS:
               {            
                    //DEBUG_PRINT_PORT_STR (u8PortNum,"PE_SNK_TRANSITION_TO_DEFAULT_ENTRY_SS\r\n");
                    
                    /*Setting the Hard Reset IN progress bit to avoid VBUS discharge
                    when VBUS drops below VSinkDisconnnect*/
                    gasPolicyEngine[u8PortNum].u8PEPortSts |= PE_HARDRESET_PROGRESS_MASK;
                     
                    /*Turn OFF VCONN if it sources currently*/
                    if (DPM_IsPortVCONNSource (u8PortNum))
                    {
                        TypeC_EnabDisVCONN (u8PortNum, TYPEC_VCONN_DISABLE); 
                        
                        /*Start the VCONN_OFF timer*/
                        /*This Timeout is implemented outside of the PD Specification to track 
                        VCONN Turn OFF error*/
                        gasPolicyEngine[u8PortNum].u8PETimerID = PDTimer_Start (\
                                                                  PE_VCONNOFF_TIMEOUT_MS,\
                                                                  DPM_VBUSOnOffOrVCONNOff_TimerCB,\
                                                                  u8PortNum,\
                                                                  (UINT8)SET_TO_ZERO); 
                    
                        gasPolicyEngine[u8PortNum].ePESubState = ePE_SNK_TRANSITION_TO_DEFAULT_WAIT_FOR_VCONN_OFF_SS;                    
                    }
                    else
                    {
                        gasPolicyEngine[u8PortNum].ePESubState = ePE_SNK_TRANSITION_TO_DEFAULT_RESETHW_SS;                    
                    }
                                       
                    break;                    
               }
               
               case ePE_SNK_TRANSITION_TO_DEFAULT_WAIT_FOR_VCONN_OFF_SS:
               {                
                    if(!DPM_IsPortVCONNSource (u8PortNum))
                    {                        
                        /*Stop the VCONN_OFF timer*/
                        PE_KillPolicyEngineTimer (u8PortNum);
                    
                        gasPolicyEngine[u8PortNum].ePESubState = ePE_SNK_TRANSITION_TO_DEFAULT_RESETHW_SS;                    
                    }
                    else
                    {
                        /* Hook to notify PE state machine entry into idle sub-state */
                        MCHP_PSF_HOOK_NOTIFY_IDLE(u8PortNum, eIDLE_PE_NOTIFY);
                    }
                    
                    break;                 
               }
					                                                 
               case ePE_SNK_TRANSITION_TO_DEFAULT_RESETHW_SS:
               {    
                    #if (TRUE == INCLUDE_PD_FR_SWAP)
                    /* Disable FRS Signal Detection */
                    DPM_DISABLE_FRS_DET_EN(u8PortNum);
                    
                    /* De-assert EN_FRS pin */
                    PWRCTRL_DisableEnFRS (u8PortNum);
                    #endif 

                    /* Configuring VBUS threshold to detect VSafe0V and VSafe5V
                     as on reception of HardReset Source will transition to VSafe0V*/
                    TypeC_ConfigureVBUSThr (u8PortNum, TYPEC_VBUS_5V, \
                            gasDPM[u8PortNum].u16SinkOperatingCurrInmA, \
                            TYPEC_CONFIG_NON_PWR_FAULT_THR);
                    
                    gasPolicyEngine[u8PortNum].ePESubState = ePE_SNK_TRANSITION_TO_DEFAULT_WAIT_SS;
                    
                    break;                    
               }
               case ePE_SNK_TRANSITION_TO_DEFAULT_WAIT_SS:
               {                     
                    /*Transition only after the VBUS from Source has gone down to 0V*/
                    if (TYPEC_VBUS_0V == DPM_GetVBUSVoltage (u8PortNum))
                    {
                        /*Disable the VBUS discharge functionality since VBUS has reached vSafe0V*/                  
                        PWRCTRL_ConfigVBUSDischarge (u8PortNum, FALSE);
  						
                        /*Inform Protocol Layer about Hard Reset Complete */
                        PRL_OnHardResetComplete (u8PortNum);
                        
                        /*Clearing the Hard Reset IN Progress status bit since Hard Reset 
                        is complete after the vSafe0V transition*/
                        gasPolicyEngine[u8PortNum].u8PEPortSts &= ~PE_HARDRESET_PROGRESS_MASK;
                 
                        gasPolicyEngine[u8PortNum].ePEState = ePE_SNK_STARTUP;
                        
                        /* Post eMCHP_PSF_HARD_RESET_COMPLETE notification */
                        (void) DPM_NotifyClient (u8PortNum, eMCHP_PSF_HARD_RESET_COMPLETE);
                    }
                    else
                    {
                        /* Hook to notify PE state machine entry into idle sub-state */
                        MCHP_PSF_HOOK_NOTIFY_IDLE(u8PortNum, eIDLE_PE_NOTIFY);                        
                    }
                    break;                                 
               }
               default:
               {
                    break;
               }
           }
           break;        
        }            
  
        default:
        {
            break;
        }
    }  

    /*Send PD message if the variable "u8IsTransmit" is set as true inside the state machine*/
	if (u8IsTransmit)
	{
		(void) PRL_TransmitMsg (u8PortNum, PRL_SOP_TYPE, u16TransmitHeader, (UINT8 *)u32aDataObj,\
                        pfnTransmitCB, u32TransmitTmrIDTxSt);
	}

}
#endif
