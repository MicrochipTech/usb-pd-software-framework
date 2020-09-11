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

#if (TRUE == INCLUDE_PD_SINK)

void PE_RunSnkStateMachine (UINT8 u8PortNum , UINT8 *pu8DataBuf , UINT8 u8SOPType ,UINT32 u32Header)
{
    UINT8 u8TypeCState = TYPEC_INVALID_STATE;
    UINT8 u8TypeCSubState = TYPEC_INVALID_SUBSTATE;
    UINT8 u8TransmitSOP = PRL_SOP_TYPE;
    UINT32 u32DataObj[PRL_MAX_DATA_OBJ_COUNT] = {SET_TO_ZERO};
	UINT16 u16TransmitHeader = SET_TO_ZERO;
	PRLTxCallback pfnTransmitCB = NULL;
	UINT32 u32TransmitTmrIDTxSt = SET_TO_ZERO;
	UINT8 u8IsTransmit = FALSE;

#if (TRUE == CONFIG_HOOK_DEBUG_MSG)
    UINT32 u32PDODebug = SET_TO_ZERO;
#endif
    
    /*Getting Type-C state and Type-C Sub State from DPM to find Type C detach event*/
    DPM_GetTypeCStates (u8PortNum, &u8TypeCState, &u8TypeCSubState);
    
    /*If the Device is detached, set the Policy Engine State to PE_SNK_STARTUP */
    if (((TYPEC_UNATTACHED_SNK == u8TypeCState) || ((TYPEC_ATTACHWAIT_SNK == u8TypeCState))) && \
        ((ePE_SNK_STARTUP != gasPolicyEngine[u8PortNum].ePEState) \
        && (ePE_SNK_DISCOVERY != gasPolicyEngine[u8PortNum].ePEState)))
    {
        /*Reset the HardResetCounter*/
        gasPolicyEngine[u8PortNum].u8HardResetCounter = RESET_TO_ZERO;
        gasPolicyEngine[u8PortNum].ePEState = ePE_SNK_STARTUP;

    }
  
    switch (gasPolicyEngine[u8PortNum].ePEState)
    {
        case ePE_SNK_STARTUP:
        {         
            DEBUG_PRINT_PORT_STR (u8PortNum,"PE_SNK_STARTUP: Entered the state\r\n");
 
            /*Reset the Protocol Layer */
             PRL_ProtocolReset(u8PortNum);
             
            /*Clearing the Hard Reset IN Progress bit here because source detach
            can occur while waiting in ePE_SNK_TRANSITION_TO_DEFAULT_WAIT_SS*/
            gasPolicyEngine[u8PortNum].u8PEPortSts &= ~ PE_HARDRESET_PROGRESS_MASK;

            /*Explicit Contract becomes invalid once this state is reached 
            from Hard Reset or Initial Power up*/
            gasPolicyEngine[u8PortNum].u8PEPortSts &= (~PE_EXPLICIT_CONTRACT);
        
            /*Reset the all the sink status set*/
            gasCfgStatusData.sPerPortData[u8PortNum].u32PortConnectStatus &= \
                        ~(DPM_PORT_SINK_CAPABILITY_MISMATCH_STATUS |
                            DPM_PORT_AS_SNK_LAST_REQ_PS_RDY_STATUS |
                            DPM_PORT_AS_SNK_LAST_REQ_ACCEPT_STATUS |
                            DPM_PORT_AS_SNK_LAST_REQ_REJECT_STATUS);
            
            gasPolicyEngine[u8PortNum].ePEState = ePE_SNK_DISCOVERY;
            
            #if (FALSE != INCLUDE_PDFU)
            if((FALSE!=gsPdfuInfo.u8IsPDFUActive) && (u8PortNum == gsPdfuInfo.u8PDFUPortNum))
            {
                PE_FwUpdtInitialize();
            }
            #endif
            break;
        }            

        case ePE_SNK_DISCOVERY:
        {
            /*This State will be useful in getting the VBUS status, if the state machine comes 
            from ePE_SNK_TRANSITION_TO_DEFAULT state*/
    
            /*Query the Device policy manager for VBUS of 5V Presence*/
            if ((TYPEC_VBUS_5V == DPM_GetVBUSVoltage(u8PortNum)) && \
                (TYPEC_ATTACHED_SNK == u8TypeCState) && (TYPEC_ATTACHED_SNK_RUN_SM_SS == u8TypeCSubState))
            {
			  	/* Enable Power fault thresholds for TYPEC_VBUS_5V to detect Power faults*/
                TypeC_ConfigureVBUSThr(u8PortNum, TYPEC_VBUS_5V, \
                    gasDPM[u8PortNum].u16SinkOperatingCurrInmA,TYPEC_CONFIG_PWR_FAULT_THR);
				
                gasPolicyEngine[u8PortNum].ePEState = ePE_SNK_WAIT_FOR_CAPABILITIES;
                gasPolicyEngine[u8PortNum].ePESubState = ePE_SNK_WAIT_FOR_CAPABILITIES_ENTRY_SS;
            }
            break;                
        }
           
        case ePE_SNK_WAIT_FOR_CAPABILITIES:
        {
            switch (gasPolicyEngine[u8PortNum].ePESubState)
            {
                case ePE_SNK_WAIT_FOR_CAPABILITIES_ENTRY_SS:
                {                   
                    DEBUG_PRINT_PORT_STR (u8PortNum,"PE_SNK_WAIT_FOR_CAPABILITIES: Entered the state\r\n");

                    /*Advertised PDO is updated with Sink's PDO*/
                    (void)MCHP_PSF_HOOK_MEMCPY(gasCfgStatusData.sPerPortData[u8PortNum].u32aAdvertisedPDO, 
                            gasCfgStatusData.sPerPortData[u8PortNum].u32aSinkPDO, 
                            DPM_4BYTES_FOR_EACH_PDO_OF(gasCfgStatusData.sPerPortData[u8PortNum].u8SinkPDOCnt));
                    /*Advertised PDO Count is updated to SinkPDO Count*/
                    gasCfgStatusData.sPerPortData[u8PortNum].u8AdvertisedPDOCnt = \
                            gasCfgStatusData.sPerPortData[u8PortNum].u8SinkPDOCnt;
                    
                    if (gasPolicyEngine[u8PortNum].u8HardResetCounter <= PE_N_HARD_RESET_COUNT)
                    {
                        /*Start the PE_SINKWAITCAP_TIMEOUT_MS to wait for the source 
                        capability message*/
                        /*Set the timer callback to transition to 
                        ePE_SNK_HARD_RESET and ePE_SNK_HARD_RESET_SEND_SS sub state if timeout happens*/
                        gasPolicyEngine[u8PortNum].u8PETimerID = PDTimer_Start(\
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

                        PWRCTRL_ConfigEnSink(u8PortNum, TRUE);
                    }

                    gasPolicyEngine[u8PortNum].ePESubState = ePE_SNK_WAIT_FOR_CAPABILITIES_WAIT_SS;                    
                    break;
                }
                /*Wait in this sub-state for the reception of Source capability message 
                from source partner*/    
                case ePE_SNK_WAIT_FOR_CAPABILITIES_WAIT_SS:
                {
                    /* Hook to notify PE state machine entry into idle substate */
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
            DEBUG_PRINT_PORT_STR (u8PortNum,"PE_SNK_EVALUATE_CAPABILITY: Entered the state\r\n");
              
            /* Notify the new source capability is received*/
            (void)DPM_NotifyClient(u8PortNum, eMCHP_PSF_NEW_SRC_CAPS_RCVD);
            
            /*Reset the HardResetCounter*/
            gasPolicyEngine[u8PortNum].u8HardResetCounter = RESET_TO_ZERO;	
            
            /*Ask the Device policy manager to evaluate the received source capability message*/
            DPM_EvaluateReceivedSrcCaps(u8PortNum,(UINT16) u32Header ,(UINT32*)pu8DataBuf );

            /*Invalid Source Capability Message results in Sink request object count to be 0*/
           if (SET_TO_ZERO == gasCfgStatusData.sPerPortData[u8PortNum].u32RDO)
           {
                /*Transition to Ready state if already in PD contract*/
                if (PE_EXPLICIT_CONTRACT == (gasPolicyEngine[u8PortNum].u8PEPortSts \
                        & PE_PDCONTRACT_MASK ))
                {
                     gasPolicyEngine[u8PortNum].ePEState = ePE_SNK_READY;
                     gasPolicyEngine[u8PortNum].ePESubState = ePE_SNK_READY_AMS_END_SS;
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
                    DEBUG_PRINT_PORT_STR (u8PortNum,"PE_SNK_SELECT_CAPABILITY: Entered the state\r\n");

                    /*Set the PD message transmitter API variables to send Sink Data request Message*/
                    u8TransmitSOP = PRL_SOP_TYPE;
                    
                    u16TransmitHeader = PRL_FormSOPTypeMsgHeader (u8PortNum, PE_DATA_REQUEST,\
                                            PE_OBJECT_COUNT_1, PE_NON_EXTENDED_MSG);
                    
                    u32DataObj[INDEX_0] = gasCfgStatusData.sPerPortData[u8PortNum].u32RDO;
                    pfnTransmitCB = PE_StateChange_TransmitCB;
                    
                    /*Set the transmitter callback to transition to Soft reset state if
                    message transmission fails*/
                    u32TransmitTmrIDTxSt = PRL_BUILD_PKD_TXST_U32(ePE_SNK_SELECT_CAPABILITY,\
                                             ePE_SNK_SELECT_CAPABILITY_REQ_SENT_SS,\
                                             ePE_SEND_SOFT_RESET,\
                                             ePE_SEND_SOFT_RESET_SOP_SS );
                    u8IsTransmit = TRUE;
                    
                    gasPolicyEngine[u8PortNum].ePESubState = ePE_SNK_SELECT_CAPABILITY_SEND_REQ_IDLE_SS;
                    break;                                   
                }
                 /*Wait here until the Sink data request message is sent*/
                case ePE_SNK_SELECT_CAPABILITY_SEND_REQ_IDLE_SS:
                {    
                    /* Hook to notify PE state machine entry into idle substate */
                    MCHP_PSF_HOOK_NOTIFY_IDLE(u8PortNum, eIDLE_PE_NOTIFY);
                    break;
                }
                    
                case ePE_SNK_SELECT_CAPABILITY_REQ_SENT_SS:
                {
                    /*Start the PE_SENDERRESPONSE_TIMEOUT_MS for the Sink Data request message sent*/
                    /*Set the timer callback to transition to 
                    ePE_SNK_HARD_RESET state and ePE_SNK_HARD_RESET_SEND_SS sub state if timeout happens*/
                    gasPolicyEngine[u8PortNum].u8PETimerID = PDTimer_Start(\
                                                              PE_SENDERRESPONSE_TIMEOUT_MS,\
                                                              PE_SSChngAndTimeoutValidate_TimerCB,\
                                                              u8PortNum,(UINT8) ePE_SNK_HARD_RESET_SEND_SS);
                    
                    gasPolicyEngine[u8PortNum].ePESubState = ePE_SNK_SELECT_CAPABILITY_WAIT_FOR_ACCEPT_SS;
                    break;
                }
                
                /*Wait here for the accept message from Source for the sink data request*/
                case ePE_SNK_SELECT_CAPABILITY_WAIT_FOR_ACCEPT_SS:
                {
                    /* Hook to notify PE state machine entry into idle substate */
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
                    DEBUG_PRINT_PORT_STR (u8PortNum,"PE_SNK_TRANSITION_SINK: Entered the state\r\n");
                    
                    gasDPM[u8PortNum].u16SinkOperatingCurrInmA = \
                                gasCfgStatusData.sPerPortData[u8PortNum].u16SnkMinOperatingCurInmA;

                    /* Requested current controlling */
                    PWRCTRL_ConfigSinkHW(u8PortNum, \
                            DPM_GET_VOLTAGE_FROM_PDO_MILLI_V(gasDPM[u8PortNum].u32NegotiatedPDO),\
                                gasDPM[u8PortNum].u16SinkOperatingCurrInmA);
                    
                    /*Initialize and run PE_PSTRANSITION_TIMEOUT_MS*/
                    /*Set the timer callback to transition to 
                    ePE_SNK_HARD_RESET and ePE_SNK_HARD_RESET_SEND_SS sub state if timeout happens*/
                    gasPolicyEngine[u8PortNum].u8PETimerID = PDTimer_Start(\
                                                              PE_PSTRANSITION_TIMEOUT_MS,\
                                                              PE_SSChngAndTimeoutValidate_TimerCB,\
                                                              u8PortNum, (UINT8)ePE_SNK_HARD_RESET_SEND_SS);
                    
                    gasPolicyEngine[u8PortNum].ePESubState = ePE_SNK_TRANSITION_SINK_WAIT_FOR_PSRDY_SS;
                    break;
                }              
                /*Wait here for the PS Ready message from Source for the sink data request*/
                case ePE_SNK_TRANSITION_SINK_WAIT_FOR_PSRDY_SS:
                {
                    /* Hook to notify PE state machine entry into idle substate */
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
                    DEBUG_PRINT_PORT_STR (u8PortNum,"PE_SNK_READY: Entered the state\r\n");
					/* configure threshold to detect faults*/
                   	DPM_EnablePowerFaultDetection(u8PortNum);
					
                    /*Setting the explicit contract as True*/
                    gasPolicyEngine[u8PortNum].u8PEPortSts |= (PE_EXPLICIT_CONTRACT);
                    gasPolicyEngine[u8PortNum].ePESubState = ePE_SNK_READY_AMS_END_SS;
                    
#if (TRUE == CONFIG_HOOK_DEBUG_MSG)                    
                    u32PDODebug = gasDPM[u8PortNum].u32NegotiatedPDO;
                    DEBUG_PRINT_PORT_UINT32_STR( u8PortNum, "PDPWR", u32PDODebug, 1, "\r\n");
#endif                  
                    /*Set EN_SINK*/
                    PWRCTRL_ConfigEnSink(u8PortNum, TRUE);

                    /*Notify that contract is established*/
                    (void)DPM_NotifyClient(u8PortNum, eMCHP_PSF_PD_CONTRACT_NEGOTIATED);
                    
                    
                    if ((DPM_PORT_SINK_CAPABILITY_MISMATCH_STATUS & \
                            gasCfgStatusData.sPerPortData[u8PortNum].u32PortConnectStatus))
                    {
                        /* Notify the capability mismatch*/
                        (void)DPM_NotifyClient(u8PortNum, eMCHP_PSF_CAPS_MISMATCH);
                    }
                    break;
                }
                
                case ePE_SNK_READY_AMS_END_SS:
                {
#if (TRUE == INCLUDE_PD_VDM)
                    /* Post not received notification if VDM:Disc Identity was initiated 
                       previously and no response has been received */
                    if (gasPolicyEngine[u8PortNum].u8PERuntimeConfig & PE_DISCOVER_ID_INITIATED)
                    {
                        gasPolicyEngine[u8PortNum].u8PERuntimeConfig &= ~(PE_DISCOVER_ID_INITIATED);
                        if (SET_TO_ZERO == DPM_VDM_GET_CMD_TYPE(gasCfgStatusData.sPerPortData[u8PortNum].u32aPartnerIdentity[INDEX_0]))
                        {
                            (void)DPM_NotifyClient(u8PortNum, eMCHP_PSF_PARTNER_IDENTITY_NOT_RCVD);                                                         
                        }
                    }
#endif
                    
                    gasDPM[u8PortNum].u16InternalEvntInProgress = RESET_TO_ZERO;                        

                    if(SET_TO_ZERO == gasDPM[u8PortNum].u16DPMInternalEvents)
                    {
                        /* Hook to notify PSF is IDLE */
                        (void)DPM_NotifyClient(u8PortNum, eMCHP_PSF_IDLE);                    
                    }
                    else
                    {
                        /*Do nothing*/
                    }
                    
                    gasPolicyEngine[u8PortNum].ePESubState = ePE_SNK_READY_IDLE_SS;
                    
                    /* Hook to notify PE state machine entry into idle sub-state */
                    MCHP_PSF_HOOK_NOTIFY_IDLE(u8PortNum, eIDLE_PE_NOTIFY);
                    break;
                }
                
                case ePE_SNK_READY_IDLE_SS:
                {
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
                    DEBUG_PRINT_PORT_STR (u8PortNum,"PE_SNK_HARD_RESET_SEND_SS: Entered the state\r\n");

                    /*Kill All the Active timers of policy engine for the port*/
                    PE_KillPolicyEngineTimer (u8PortNum);
                    
                    /*Setting the Hard Reset IN progress bit to avoid VBUS discharge
                    when VBUS drops below VSinkDisconnnect*/
                    gasPolicyEngine[u8PortNum].u8PEPortSts |= PE_HARDRESET_PROGRESS_MASK;
                   
                     /*The transmitter callback is set to transition to Startup state 
                      * if message transmission fails*/
                    
                    /* API to send HardReset is called*/
                    PRL_SendCableorHardReset(u8PortNum, PRL_SEND_HARD_RESET,\
                                             NULL, 0x00);
                    
                    gasPolicyEngine[u8PortNum].ePEState = ePE_SNK_TRANSITION_TO_DEFAULT;
                    gasPolicyEngine[u8PortNum].ePESubState = ePE_SNK_TRANSITION_TO_DEFAULT_ENTRY_SS;

                    /*Increment HardReset Counter*/
                    gasPolicyEngine[u8PortNum].u8HardResetCounter++;             
                    
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
            
                    DEBUG_PRINT_PORT_STR (u8PortNum,"PE_SNK_TRANSITION_TO_DEFAULT: Entered the state\r\n");
                    
                    /*Setting the Hard Reset IN progress bit to avoid VBUS discharge
                    when VBUS drops below VSinkDisconnnect*/
                    gasPolicyEngine[u8PortNum].u8PEPortSts |= PE_HARDRESET_PROGRESS_MASK;
                     
                    /*Turn OFF VCONN if it sources currently*/
                    if (DPM_IsPortVCONNSource(u8PortNum))
                    {
                        DPM_VCONNOnOff(u8PortNum,DPM_VCONN_OFF);
                        
                        /*Start the VCONN_OFF timer*/
                        /*This Timeout is implemented outside of the PD Specification to track 
                        VCONN Turn OFF error*/
                        gasPolicyEngine[u8PortNum].u8PETimerID = PDTimer_Start (\
                                                                  PE_VCONNOFF_TIMEOUT_MS,\
                                                                  DPM_VBUSorVCONNOnOff_TimerCB,\
                                                                  u8PortNum,\
                                                                  (UINT8)SET_TO_ZERO); 
                        /* Hook to notify PE state machine entry into idle substate */
                        MCHP_PSF_HOOK_NOTIFY_IDLE(u8PortNum, eIDLE_PE_NOTIFY);
                    
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
                    if(!DPM_IsPortVCONNSource(u8PortNum))
                    {                        
                        /*Stop the VCONN_OFF timer*/
                        PE_KillPolicyEngineTimer (u8PortNum);
                    
                        gasPolicyEngine[u8PortNum].ePESubState = ePE_SNK_TRANSITION_TO_DEFAULT_RESETHW_SS;
                    
                    }
                    
                    break;                 
               }
					                                                 
               case ePE_SNK_TRANSITION_TO_DEFAULT_RESETHW_SS:
               {    
                    /* Configuring VBUS threshold to detect VSafe0V and VSafe5V
                     as on reception of HardReset Source will transition to VSafe0V*/
                    TypeC_ConfigureVBUSThr(u8PortNum, TYPEC_VBUS_5V, \
                            gasDPM[u8PortNum].u16SinkOperatingCurrInmA, \
                            TYPEC_CONFIG_NON_PWR_FAULT_THR);
                    gasPolicyEngine[u8PortNum].ePESubState = ePE_SNK_TRANSITION_TO_DEFAULT_WAIT_SS;
                    break;                    
               }
               case ePE_SNK_TRANSITION_TO_DEFAULT_WAIT_SS:
               {                     
                    /*Transition only after the VBUS from Source has gone down to 0V*/
                    if(TYPEC_VBUS_0V == DPM_GetVBUSVoltage(u8PortNum))
                    {  						
                        /*Inform Protocol Layer about Hard Reset Complete */
                        PRL_OnHardResetComplete(u8PortNum);
                        
                        /*Clearing the Hard Reset IN Progress status bit since Hard Reset 
                        is complete after the vSafe0V transition*/
                        gasPolicyEngine[u8PortNum].u8PEPortSts &= ~ PE_HARDRESET_PROGRESS_MASK;
                 
                        gasPolicyEngine[u8PortNum].ePEState = ePE_SNK_STARTUP;
                    
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
        /*This State is set by PE_ReceiveMsgHandler API if Get Sink Caps is received if SNK ready state*/
        case ePE_SNK_GIVE_SINK_CAP:
        {              
            switch (gasPolicyEngine[u8PortNum].ePESubState)
            {
                case ePE_SNK_GIVE_SINK_CAP_ENTRY_SS:
                {
                    DEBUG_PRINT_PORT_STR (u8PortNum,"PE_SNK_GIVE_SINK_CAP: Entered the state\r\n");

                    UINT16 u16Header;
                    UINT8 u8SinkPDOCnt;

                    /*Request Device policy manager for Sink Capability Message*/
                    /*If the Port does not have sink capability, Send Reject/Not Supported message*/
                    DPM_GetSinkCapabilities(u8PortNum, &u8SinkPDOCnt, u32DataObj);
                    
                    
                    u16Header = PRL_FormSOPTypeMsgHeader(u8PortNum, PE_DATA_SINK_CAP,\
                                                         u8SinkPDOCnt, PE_NON_EXTENDED_MSG);
                

                    /*Set the PD message transmitter  API to Send Sink Capability Message*/
                    u8TransmitSOP = PRL_SOP_TYPE;
                    u16TransmitHeader = u16Header;
                    pfnTransmitCB = PE_StateChange_TransmitCB;
                    
                    /*Set the transmitter callback to transition to Soft reset state if
                    message transmission fails*/
                    u32TransmitTmrIDTxSt = PRL_BUILD_PKD_TXST_U32(ePE_SNK_READY,\
                                             ePE_SNK_READY_AMS_END_SS,ePE_SEND_SOFT_RESET,\
                                             ePE_SEND_SOFT_RESET_SOP_SS);
                    u8IsTransmit = TRUE;
                    
                    gasPolicyEngine[u8PortNum].ePESubState = ePE_SNK_GIVE_SINK_CAP_IDLE_SS;
                    
                    break;

                }
                /*Wait here until the Sink capability message is sent*/
                case ePE_SNK_GIVE_SINK_CAP_IDLE_SS:
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
        default:
        {
            break;
        }
     }  

    /*Send PD message if the variable "u8IsTransmit" is set as true inside the state machine*/
	if (TRUE == u8IsTransmit)
	{
		(void) PRL_TransmitMsg(u8PortNum, u8TransmitSOP, u16TransmitHeader, (UINT8 *)u32DataObj,\
                        pfnTransmitCB, u32TransmitTmrIDTxSt);
	}

}
#endif
