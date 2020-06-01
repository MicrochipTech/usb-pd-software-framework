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

#if (TRUE == INCLUDE_PD_SINK || TRUE == INCLUDE_PD_DRP)

void PE_SnkRunStateMachine (UINT8 u8PortNum , UINT8 *pu8DataBuf , UINT8 u8SOPType ,UINT32 u32Header)
{
    UINT8 u8TypeCState = TYPEC_INVALID_STATE;
    UINT8 u8TypeCSubState = TYPEC_INVALID_SUBSTATE;
    UINT8 u8TransmitSOP = PRL_SOP_TYPE;
    UINT32 u32DataObj[PRL_MAX_DATA_OBJ_COUNT] = {SET_TO_ZERO};
	UINT16 u16Transmit_Header = SET_TO_ZERO;
	PRLTxCallback Transmit_cb = NULL;
	UINT32 u32Transmit_TmrID_TxSt = SET_TO_ZERO;
	UINT8 u8IsTransmit= FALSE;

#if (TRUE == CONFIG_HOOK_DEBUG_MSG)
    UINT32 u32PDODebug = SET_TO_ZERO;
#endif
    
    /*Getting Type-C state and Type-C Sub State from DPM to find Type C detach event*/
    DPM_GetTypeCStates (u8PortNum, &u8TypeCState, &u8TypeCSubState);
    
    /*If the Device is detached, set the Policy Engine State to PE_SNK_STARTUP */
    if (((TYPEC_UNATTACHED_SNK == u8TypeCState) || ((TYPEC_ATTACHWAIT_SNK == u8TypeCState))) && \
        ((ePE_SNK_STARTUP != gasPolicy_Engine[u8PortNum].ePEState) \
        && (ePE_SNK_DISCOVERY != gasPolicy_Engine[u8PortNum].ePEState)))
    {
        /*Reset the HardResetCounter*/
        gasPolicy_Engine[u8PortNum].u8HardResetCounter = RESET_TO_ZERO;
        gasPolicy_Engine[u8PortNum].ePEState = ePE_SNK_STARTUP;

    }
  
    switch (gasPolicy_Engine[u8PortNum].ePEState)
    {
        case ePE_SNK_STARTUP:
        {         
            DEBUG_PRINT_PORT_STR (u8PortNum,"PE_SNK_STARTUP: Enterted the state\r\n");
 
            /*Reset the Protocol Layer */
             PRL_ProtocolReset(u8PortNum);
             
            /*Clearing the Hard Reset IN Progress bit here because source detach
            can occur while waiting in ePE_SNK_TRANSITION_TO_DEFAULT_WAIT_SS*/
            gasPolicy_Engine[u8PortNum].u8PEPortSts &= ~ PE_HARDRESET_PROGRESS_MASK;

            /*Explicit Contract becomes invaild once this state is reached 
            from Hard Reset or Initial Power up*/
            gasPolicy_Engine[u8PortNum].u8PEPortSts &= (~PE_EXPLICIT_CONTRACT);
            
            /*Clear partner pdo variable*/
            for(UINT8 u8Index = SET_TO_ZERO; u8Index < DPM_MAX_PDO_CNT; u8Index++)
            {
                gasCfgStatusData.sPerPortData[u8PortNum].u32aPartnerPDO[u8Index] = SET_TO_ZERO;
            }
            gasCfgStatusData.sPerPortData[u8PortNum].u8PartnerPDOCnt = SET_TO_ZERO;
            
            /*Reset the all the sink status set*/
            gasCfgStatusData.sPerPortData[u8PortNum].u32PortConnectStatus &= \
                        ~(DPM_PORT_SINK_CAPABILITY_MISMATCH_STATUS |
                            DPM_PORT_AS_SNK_LAST_REQ_PS_RDY_STATUS |
                            DPM_PORT_AS_SNK_LAST_REQ_ACCEPT_STATUS |
                            DPM_PORT_AS_SNK_LAST_REQ_REJECT_STATUS);
            
            gasPolicy_Engine[u8PortNum].ePEState = ePE_SNK_DISCOVERY;
            
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
            if ((DPM_GetVBUSVoltage(u8PortNum) == TYPEC_VBUS_5V) && \
                (u8TypeCState == TYPEC_ATTACHED_SNK))
            {
			  	/* Enable Power fault thresholds for TYPEC_VBUS_5V to detect Power faults*/
                TypeC_ConfigureVBUSThr(u8PortNum, TYPEC_VBUS_5V, \
                    gasDPM[u8PortNum].u16SinkOperatingCurrInmA,TYPEC_CONFIG_PWR_FAULT_THR);
				
                gasPolicy_Engine[u8PortNum].ePEState = ePE_SNK_WAIT_FOR_CAPABILITIES;
                gasPolicy_Engine[u8PortNum].ePESubState = ePE_SNK_WAIT_FOR_CAPABILITIES_ENTRY_SS;
            }
            break;                
        }
           
        case ePE_SNK_WAIT_FOR_CAPABILITIES:
        {
            switch (gasPolicy_Engine[u8PortNum].ePESubState)
            {
                case ePE_SNK_WAIT_FOR_CAPABILITIES_ENTRY_SS:
                {                   
                    DEBUG_PRINT_PORT_STR (u8PortNum,"PE_SNK_WAIT_FOR_CAPABILITIES: Enterted the state\r\n");

                    if (gasPolicy_Engine[u8PortNum].u8HardResetCounter <= PE_N_HARD_RESET_COUNT)
                    {
                        /*Start the CONFIG_PE_SINKWAITCAP_TIMEOUT_MS to wait for the source 
                        capability message*/
                        /*Set the timer callback to transition to 
                        ePE_SNK_HARD_RESET and ePE_SNK_HARD_RESET_SEND_SS sub state if timeout happens*/
                        gasPolicy_Engine[u8PortNum].u8PETimerID = PDTimer_Start(\
                                                                 CONFIG_PE_SINKWAITCAP_TIMEOUT_MS,\
                                                                 PE_SubStateChangeAndTimeoutValidateCB,\
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

                    gasPolicy_Engine[u8PortNum].ePESubState = ePE_SNK_WAIT_FOR_CAPABILITIES_WAIT_SS;                    
                    break;
                }
                /*Wait in this substate for the reception of Source capability message 
                from source partner*/    
                case ePE_SNK_WAIT_FOR_CAPABILITIES_WAIT_SS:
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
        /*This State is set by the PE_ReceiveMsgHandler API if the source capability message has been received*/    
        case ePE_SNK_EVALUATE_CAPABILITY:
        {            
            DEBUG_PRINT_PORT_STR (u8PortNum,"PE_SNK_EVALUATE_CAPABILITY: Entered the state\r\n");
              
            /* Notify the new source capability is received*/
            (void)DPM_NotifyClient(u8PortNum, eMCHP_PSF_NEW_SRC_CAPS_RCVD);
            
            /*Reset the HardResetCounter*/
            gasPolicy_Engine[u8PortNum].u8HardResetCounter = RESET_TO_ZERO;	
            
            /*Ask the Device policy manager to evaluate the received source capability message*/
            DPM_Evaluate_Received_Src_caps(u8PortNum,(UINT16) u32Header ,(UINT32*)pu8DataBuf );

            /*Invalid Source Capability Message results in Sink request object count to be 0*/
           if (gasCfgStatusData.sPerPortData[u8PortNum].u32RDO == SET_TO_ZERO)
           {
                /*Transition to Ready state if already in PD contract*/
                if ((gasPolicy_Engine[u8PortNum].u8PEPortSts & PE_PDCONTRACT_MASK ) == \
                    PE_EXPLICIT_CONTRACT)
                {
                     gasPolicy_Engine[u8PortNum].ePEState = ePE_SNK_READY;
                     gasPolicy_Engine[u8PortNum].ePESubState = ePE_SNK_READY_IDLE_SS;
                }
                 /*Transition to sink wait for capabilities state if not in PD contract*/
                else
                {
                    gasPolicy_Engine[u8PortNum].ePEState = ePE_SNK_WAIT_FOR_CAPABILITIES;
                    gasPolicy_Engine[u8PortNum].ePESubState = ePE_SNK_WAIT_FOR_CAPABILITIES_ENTRY_SS;
                }			
            }
            /*Valid Source capability results in transition to ePE_SNK_SELECT_CAPABILITY state*/
            else
            {     
                gasPolicy_Engine[u8PortNum].ePEState = ePE_SNK_SELECT_CAPABILITY;
                gasPolicy_Engine[u8PortNum].ePESubState = ePE_SNK_SELECT_CAPABILITY_SEND_REQ_SS;            
            }
            break;
        }           
        case ePE_SNK_SELECT_CAPABILITY:
        {
            switch (gasPolicy_Engine[u8PortNum].ePESubState)
            {
                case ePE_SNK_SELECT_CAPABILITY_SEND_REQ_SS:
                {                 
                    DEBUG_PRINT_PORT_STR (u8PortNum,"PE_SNK_SELECT_CAPABILITY: Entered the state\r\n");

                    /*Set the PD message transmitter API variables to send Sink Data request Message*/
                    u8TransmitSOP = PRL_SOP_TYPE;
                    
                    u16Transmit_Header = PRL_FormSOPTypeMsgHeader (u8PortNum, PE_DATA_REQUEST,\
                                            PE_OBJECT_COUNT_1, PE_NON_EXTENDED_MSG);
                    
                    u32DataObj[0] = gasCfgStatusData.sPerPortData[u8PortNum].u32RDO;
                    Transmit_cb = PE_StateChange_TransmitCB;
                    
                    /*Set the transmitter callback to transition to Soft reset state if
                    message transmission fails*/
                    u32Transmit_TmrID_TxSt = PRL_BUILD_PKD_TXST_U32(ePE_SNK_SELECT_CAPABILITY,\
                                             ePE_SNK_SELECT_CAPABILITY_REQ_SENT_SS,\
                                             ePE_SNK_SEND_SOFT_RESET,\
                                             ePE_SNK_SEND_SOFT_RESET_ENTRY_SS );
                    u8IsTransmit = TRUE;
                    
                    gasPolicy_Engine[u8PortNum].ePESubState = ePE_SNK_SELECT_CAPABILITY_SEND_REQ_IDLE_SS;
                    break;                                   
                }
                 /*Wait here until the Sink data request message is sent*/
                case ePE_SNK_SELECT_CAPABILITY_SEND_REQ_IDLE_SS:
                {                  
                    break;
                }
                    
                case ePE_SNK_SELECT_CAPABILITY_REQ_SENT_SS:
                {
                    /*Start the CONFIG_PE_SENDERRESPONSE_TIMEOUT_MS for the Sink Data request message sent*/
                    /*Set the timer callback to transition to 
                    ePE_SNK_HARD_RESET state and ePE_SNK_HARD_RESET_SEND_SS sub state if timeout happens*/
                    gasPolicy_Engine[u8PortNum].u8PETimerID = PDTimer_Start(\
                                                              CONFIG_PE_SENDERRESPONSE_TIMEOUT_MS,\
                                                              PE_SubStateChangeAndTimeoutValidateCB,\
                                                              u8PortNum,(UINT8) ePE_SNK_HARD_RESET_SEND_SS);
                    
                    gasPolicy_Engine[u8PortNum].ePESubState = ePE_SNK_SELECT_CAPABILITY_WAIT_FOR_ACCEPT_SS;
                    break;
                }
                
                /*Wait here for the accept message from Source for the sink data request*/
                case ePE_SNK_SELECT_CAPABILITY_WAIT_FOR_ACCEPT_SS:
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
        /*This State is set by the PE_ReceiveMsgHandler API if the accept message has been received*/
        case ePE_SNK_TRANSITION_SINK:
        {
            switch (gasPolicy_Engine[u8PortNum].ePESubState)
            {
                case ePE_SNK_TRANSITION_SINK_ENTRY_SS:
                {                    
                    DEBUG_PRINT_PORT_STR (u8PortNum,"PE_SNK_TRANSITION_SINK: Enterted the state\r\n");
                    
                    /* Requested current controlling */
                      PWRCTRL_ConfigSinkHW(u8PortNum, \
                            DPM_GET_VOLTAGE_FROM_PDO_MILLI_V(gasDPM[u8PortNum].u32NegotiatedPDO),\
                                gasDPM[u8PortNum].u16SinkOperatingCurrInmA);
                    
                    /*Initialize and run CONFIG_PE_PSTRANSITION_TIMEOUT_MS*/
                    /*Set the timer callback to transition to 
                    ePE_SNK_HARD_RESET and ePE_SNK_HARD_RESET_SEND_SS sub state if timeout happens*/
                    gasPolicy_Engine[u8PortNum].u8PETimerID = PDTimer_Start(\
                                                              CONFIG_PE_PSTRANSITION_TIMEOUT_MS,\
                                                              PE_SubStateChangeAndTimeoutValidateCB,\
                                                              u8PortNum, (UINT8)ePE_SNK_HARD_RESET_SEND_SS);
                    
                    gasPolicy_Engine[u8PortNum].ePESubState = ePE_SNK_TRANSITION_SINK_WAIT_FOR_PSRDY_SS;
                    break;
                }              
                /*Wait here for the PS Ready message from Source for the sink data request*/
                case ePE_SNK_TRANSITION_SINK_WAIT_FOR_PSRDY_SS:
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
        
        /*This State is set by the PE_ReceiveMsgHandler API if the PS_RDY message has been received*/
        case ePE_SNK_READY:
        {
            switch (gasPolicy_Engine[u8PortNum].ePESubState)
            {
                case ePE_SNK_READY_ENTRY_SS:
                {                    
                    DEBUG_PRINT_PORT_STR (u8PortNum,"PE_SNK_READY: Entered the state\r\n");
					/* configure threshold to detect faults*/
                   	DPM_EnablePowerFaultDetection(u8PortNum);
					
                    /*Setting the explicit contract as True*/
                    gasPolicy_Engine[u8PortNum].u8PEPortSts |= (PE_EXPLICIT_CONTRACT);
                    gasPolicy_Engine[u8PortNum].ePESubState = ePE_SNK_READY_IDLE_SS;
                    
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
        /*This State is enterted if hard reset is to be transmitted to the port partner*/   
        case ePE_SNK_HARD_RESET:
        {
           switch (gasPolicy_Engine[u8PortNum].ePESubState)
           {           
               case ePE_SNK_HARD_RESET_SEND_SS:
               { 
                    
                    DEBUG_PRINT_PORT_STR (u8PortNum,"PE_SNK_HARD_RESET_SEND_SS: Enterted the state\r\n");

                    /*Kill All the Active timers of policy engine for the port*/
                    PE_KillPolicyEngineTimer (u8PortNum);
                    
                    /*Setting the Hard Reset IN progress bit to avoid VBUS discharge
                    when VBUS drops below VSinkDisconnnect*/
                    gasPolicy_Engine[u8PortNum].u8PEPortSts |= PE_HARDRESET_PROGRESS_MASK;
                   
                     /*The transmitter callback is set to transition to Startup state 
                      * if message transmission fails*/
                    
                    /* API to send Hardreset is called*/
                    PRL_SendCableorHardReset(u8PortNum, PRL_SEND_HARD_RESET,\
                                             NULL, 0x00);
                    
                    gasPolicy_Engine[u8PortNum].ePEState = ePE_SNK_TRANSITION_TO_DEFAULT;
                    gasPolicy_Engine[u8PortNum].ePESubState = ePE_SNK_TRANSITION_TO_DEFAULT_ENTRY_SS;

                    /*Increment HardReset Counter*/
                    gasPolicy_Engine[u8PortNum].u8HardResetCounter++;             
                    
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
           switch (gasPolicy_Engine[u8PortNum].ePESubState)
           {                  
               case ePE_SNK_TRANSITION_TO_DEFAULT_ENTRY_SS:
               {
            
                    DEBUG_PRINT_PORT_STR (u8PortNum,"PE_SNK_TRANSITION_TO_DEFAULT: Enterted the state\r\n");
                    
                    /*Setting the Hard Reset IN progress bit to avoid VBUS discharge
                    when VBUS drops below VSinkDisconnnect*/
                    gasPolicy_Engine[u8PortNum].u8PEPortSts |= PE_HARDRESET_PROGRESS_MASK;
                     
                    /*Turn OFF VCONN if it sources currently*/
                    if (DPM_IsPort_VCONN_Source(u8PortNum))
                    {
                        DPM_VConnOnOff(u8PortNum,DPM_VCONN_OFF);
                        
                        /*Start the VCONN_OFF timer*/
                        /*This Timeout is implemented outside of the PD Specification to track 
                        VCONN Turn OFF error*/
                        gasPolicy_Engine[u8PortNum].u8PETimerID = PDTimer_Start (\
                                                                  CONFIG_PE_VCONNOFF_TIMEOUT_MS,\
                                                                  DPM_VCONNOFFErrorTimerCB,\
                                                                  u8PortNum,\
                                                                  (UINT8)SET_TO_ZERO);                 
                    
                        gasPolicy_Engine[u8PortNum].ePESubState = ePE_SNK_TRANSITION_TO_DEFAULT_VCONNOFF_CHECK_SS;
                    
                    }
                    else
                    {
                        gasPolicy_Engine[u8PortNum].ePESubState = ePE_SNK_TRANSITION_TO_DEFAULT_RESETHW_SS;
                    
                    }
                                       
                    break;                    
               }
               
               case ePE_SNK_TRANSITION_TO_DEFAULT_VCONNOFF_CHECK_SS:
               {                
                    if(!DPM_IsPort_VCONN_Source(u8PortNum))
                    {
                        
                        /*Stop the VCONN_OFF timer*/
                        PE_KillPolicyEngineTimer (u8PortNum);
                    
                        gasPolicy_Engine[u8PortNum].ePESubState = ePE_SNK_TRANSITION_TO_DEFAULT_RESETHW_SS;
                    
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
                    gasPolicy_Engine[u8PortNum].ePESubState = ePE_SNK_TRANSITION_TO_DEFAULT_WAIT_SS;
                    break;                    
               }
               case ePE_SNK_TRANSITION_TO_DEFAULT_WAIT_SS:
               {                     
                    /*Transition only after the VBUS from Source has gone down to 0V*/
                    if(DPM_GetVBUSVoltage(u8PortNum) == TYPEC_VBUS_0V)
                    {  						
                        /*Inform Protocol Layer about Hard Reset Complete */
                        PRL_HRorCRCompltIndicationFromPE(u8PortNum);
                        
                        /*Clearing the Hard Reset IN Progress status bit since Hard Reset 
                        is complete after the Vsafe0V transition*/
                        gasPolicy_Engine[u8PortNum].u8PEPortSts &= ~ PE_HARDRESET_PROGRESS_MASK;
                 
                        gasPolicy_Engine[u8PortNum].ePEState = ePE_SNK_STARTUP;
                    
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
            switch (gasPolicy_Engine[u8PortNum].ePESubState)
            {
                case ePE_SNK_GIVE_SINK_CAP_ENTRY_SS:
                {
                    DEBUG_PRINT_PORT_STR (u8PortNum,"PE_SNK_GIVE_SINK_CAP: Enterted the state\r\n");

                    UINT16 u16Header;
                    UINT8 u8SinkPDOCnt;

                    /*Request Device policy manager for Sink Capability Messsage*/
                    /*If the Port does not have sink capability, Send Reject/Not Supported message*/
                    DPM_Get_Sink_Capabilities(u8PortNum, &u8SinkPDOCnt, u32DataObj);
                    
                    
                    u16Header = PRL_FormSOPTypeMsgHeader(u8PortNum, PE_DATA_SINK_CAP,\
                                                         u8SinkPDOCnt, PE_NON_EXTENDED_MSG);
                

                    /*Set the PD message transmitter  API to Send Sink Capability Messsage*/
                    u8TransmitSOP = PRL_SOP_TYPE;
                    u16Transmit_Header = u16Header;
                    Transmit_cb = PE_StateChange_TransmitCB;
                    
                    /*Set the transmitter callback to transition to Soft reset state if
                    message transmission fails*/
                    u32Transmit_TmrID_TxSt = PRL_BUILD_PKD_TXST_U32(ePE_SNK_READY,\
                                             ePE_SNK_READY_IDLE_SS,ePE_SNK_SEND_SOFT_RESET,\
                                             ePE_SNK_SEND_SOFT_RESET_ENTRY_SS);
                    u8IsTransmit = TRUE;
                    
                    gasPolicy_Engine[u8PortNum].ePESubState = ePE_SNK_GIVE_SINK_CAP_IDLE_SS;
                    
                    break;

                }
                /*Wait here until the Sink capability message is sent*/
                case ePE_SNK_GIVE_SINK_CAP_IDLE_SS:
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
        
        /*This State is enterted if soft reset is received from the port partner*/
        case ePE_SNK_SOFT_RESET:
        {
            switch (gasPolicy_Engine[u8PortNum].ePESubState)
            {
                case ePE_SNK_SOFT_RESET_SEND_ACCEPT_SS:
                {
                                   
                    DEBUG_PRINT_PORT_STR (u8PortNum,"PE_SNK_SOFT_RESET: Enterted the state\r\n");
                    
                    /*Kill the Policy engine active timer since soft reset is received*/
                    PE_KillPolicyEngineTimer (u8PortNum);
                    
                    /*Reset the UPD Protocol Layer for the received soft reset message*/
                    PRL_ProtocolspecificSOPReset(u8PortNum, PRL_SOP_TYPE);

                    /*Set the PD message transmitter API to Send Accept Messsage*/
                    u8TransmitSOP = PRL_SOP_TYPE;
                    u16Transmit_Header = PRL_FormSOPTypeMsgHeader(u8PortNum, PE_CTRL_ACCEPT,\
                                                                 PE_OBJECT_COUNT_0,\
                                                                 PE_NON_EXTENDED_MSG);
                    
                    Transmit_cb = PE_StateChange_TransmitCB;
                    
                    /*Set the transmitter callback to transition to hard reset state if
                    message transmission fails*/
                    u32Transmit_TmrID_TxSt = PRL_BUILD_PKD_TXST_U32(ePE_SNK_WAIT_FOR_CAPABILITIES,\
                                             ePE_SNK_WAIT_FOR_CAPABILITIES_ENTRY_SS,\
                                             ePE_SNK_HARD_RESET, \
                                             ePE_SNK_HARD_RESET_SEND_SS);
                    u8IsTransmit = TRUE;
                    
                    gasPolicy_Engine[u8PortNum].ePESubState = ePE_SNK_SOFT_RESET_WAIT_SS;
                    break;
                }
                /*Wait here until the accept message for the received soft reset is sent*/
                case ePE_SNK_SOFT_RESET_WAIT_SS:
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
        /*This State is enterted to send soft reset message to the port partner*/
        case ePE_SNK_SEND_SOFT_RESET:
        {
            switch (gasPolicy_Engine[u8PortNum].ePESubState)
            {
                case ePE_SNK_SEND_SOFT_RESET_ENTRY_SS:
                {

                    DEBUG_PRINT_PORT_STR (u8PortNum,"PE_SNK_SEND_SOFT_RESET: Enterted the state\r\n");
                    
                     /*Kill the Policy engine active timer since soft reset is received*/
                    PE_KillPolicyEngineTimer (u8PortNum);

                    /*Reset the UPD Protocol Layer*/
                    PRL_ProtocolspecificSOPReset(u8PortNum, PRL_SOP_TYPE);

                    /*Set the PD message transmitter  API to Send SoftReset message*/
                    u8TransmitSOP = PRL_SOP_TYPE;
                    u16Transmit_Header = PRL_FormSOPTypeMsgHeader(u8PortNum, PE_CTRL_SOFT_RESET,\
                                                                  PE_OBJECT_COUNT_0, \
                                                                  PE_NON_EXTENDED_MSG);
                    Transmit_cb = PE_StateChange_TransmitCB;
                    
                    /*Set the transmitter callback to transition to hard reset state if
                    message transmission fails*/
                    u32Transmit_TmrID_TxSt = PRL_BUILD_PKD_TXST_U32( ePE_SNK_SEND_SOFT_RESET,\
                                             ePE_SNK_SEND_SOFT_RESET_SENT_SS,\
                                             ePE_SNK_HARD_RESET, ePE_SNK_HARD_RESET_SEND_SS);
                    u8IsTransmit = TRUE;
                    gasPolicy_Engine[u8PortNum].ePESubState = ePE_SNK_SEND_SOFT_RESET_SEND_IDLE_SS;
                    break;
                }
                /*Wait here until the Soft reset message is sent*/               
                case ePE_SNK_SEND_SOFT_RESET_SEND_IDLE_SS:
                {                  
                    break;
                }
                /*This Substate is entered once GoodCrc for the Soft reset message is received*/      
                case ePE_SNK_SEND_SOFT_RESET_SENT_SS:
                {
                  
                    /*Start the Sender response timer for the soft reset message being sent*/
                    /*Set the timer callback to transition to ePE_SNK_HARD_RESET state
                     * and ePE_SNK_HARD_RESET_SEND_SS sub state if timeout happens*/
                    gasPolicy_Engine[u8PortNum].u8PETimerID = PDTimer_Start(
                                                              CONFIG_PE_SENDERRESPONSE_TIMEOUT_MS,\
                                                              PE_SubStateChangeAndTimeoutValidateCB,\
                                                              u8PortNum,\
                                                              (UINT8) ePE_SNK_HARD_RESET_SEND_SS);
                    
                    gasPolicy_Engine[u8PortNum].ePESubState = ePE_SNK_SEND_SOFT_RESET_WAIT_FOR_ACCEPT_SS;
                    break;

                }
                /*Wait here for the accept message from source for the soft reset sent*/   
                case ePE_SNK_SEND_SOFT_RESET_WAIT_FOR_ACCEPT_SS:
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
        default:
        {
            break;
        }

     }  

    /*Send PD message if the variable "u8IsTransmit" is set as true inside the state machine*/
	if (u8IsTransmit == TRUE)
	{
		(void) PRL_TransmitMsg(u8PortNum, u8TransmitSOP, u16Transmit_Header, (UINT8 *)u32DataObj,\
                        Transmit_cb, u32Transmit_TmrID_TxSt);
	}

}
#endif
