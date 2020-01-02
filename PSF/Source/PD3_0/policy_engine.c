/*******************************************************************************
 PD Policy Engine Source file 

  Company:
    Microchip Technology Inc.
  
  File Name:
    policy_engine.c

  Description:
    This file contains the function definitions for Receive Handler and Policy Engine
    common state-machines for Source and Sink
*******************************************************************************/

/*******************************************************************************
Copyright ©  [2019] Microchip Technology Inc. and its subsidiaries.

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

void PE_InitPort (UINT8 u8PortNum)
{

    /*Setting the HardResetCounter to 0 */
    gasPolicy_Engine[u8PortNum].u8HardResetCounter = SET_TO_ZERO;

    /*Setting Contract to Implicit contract and PD Connected Status to not connected*/
    gasPolicy_Engine[u8PortNum].u8PEPortSts = SET_TO_ZERO;
    
    /*Setting the Hard Reset Received Status to False*/
    gasPolicy_Engine[u8PortNum].u8HardResetRecvdISR = FALSE;

    /*Setting Timer ID to Max value */
    gasPolicy_Engine[u8PortNum].u8PETimerID = MAX_CONCURRENT_TIMERS;

    /*Set Timeout sub-state to invalid state*/
    gasPolicy_Engine[u8PortNum].ePETimeoutSubState = ePE_INVALIDSUBSTATE;

    /* Setting Receive Handler message header and Timeout Message header to zero */
    gasPolicy_Engine[u8PortNum].u32MsgHeader = SET_TO_ZERO;
    gasPolicy_Engine[u8PortNum].u32TimeoutMsgHeader = SET_TO_ZERO;

    if ((gasPortConfigurationData[u8PortNum].u32CfgData & TYPEC_PORT_TYPE_MASK) == PD_ROLE_SOURCE)
    {
        /*Setting the CapsCounter to 0 */
        gasPolicy_Engine[u8PortNum].u8CapsCounter = SET_TO_ZERO;

        /*Setting Initial Soource Policy Engine State as Startup State*/
        gasPolicy_Engine[u8PortNum].ePEState = ePE_SRC_STARTUP;
        gasPolicy_Engine[u8PortNum].ePESubState = ePE_SRC_STARTUP_ENTRY_SS;

        /*Setting No Response TimerID to Max Value */
        gasPolicy_Engine[u8PortNum].u8PENoResponseTimerID = MAX_CONCURRENT_TIMERS;
    }
    else
    {
        /*Setting Initial Sink Policy Engine State as Startup State*/
        gasPolicy_Engine[u8PortNum].ePEState = ePE_SNK_STARTUP;
        gasPolicy_Engine[u8PortNum].ePESubState = ePE_INVALIDSUBSTATE;
    }
}


void PE_RunStateMachine (UINT8 u8PortNum)
{

    /* Receive Data Buffer */
    UINT8 u8DataBuf[260] = {SET_TO_ZERO};
    /*Received message type*/
    UINT8 u8SOPType = PRL_SOP_TYPE;
    /* Received Msg Header */
    UINT32 u32Header;
    UINT8 u8RetVal;

    /* Protocol layer Chunk State machine must be ran by PE to receive Chunk message if any*/
#if INCLUDE_PD_3_0
    PRL_RunChunkStateMachine (u8PortNum);
#endif

    MCHP_PSF_HOOK_DISABLE_GLOBAL_INTERRUPT ();

    /*Check the HardReset Flag in DPMStatus variable if any hard reset is received*/
    /*State transition for Hard reset reception is done in foreground to avoid the policy
    state and substate corruption*/

    if (gasPolicy_Engine[u8PortNum].u8HardResetRecvdISR)
    {
        if (PD_ROLE_SOURCE == DPM_GET_CURRENT_POWER_ROLE(u8PortNum))
        {
            gasPolicy_Engine[u8PortNum].ePEState = ePE_SRC_HARD_RESET_RECEIVED;
            gasPolicy_Engine[u8PortNum].ePESubState = ePE_SRC_HARD_RESET_RECEIVED_ENTRY_SS;
        }
        else
        {
            gasPolicy_Engine[u8PortNum].ePEState = ePE_SNK_TRANSITION_TO_DEFAULT;
            gasPolicy_Engine[u8PortNum].ePESubState = ePE_SNK_TRANSITION_TO_DEFAULT_ENTRY_SS;
        }
        gasPolicy_Engine[u8PortNum].u8HardResetRecvdISR = FALSE;
    }

    MCHP_PSF_HOOK_ENABLE_GLOBAL_INTERRUPT ();

    /*Check the current policy engine states and set the VDM state active flag accordingly*/
    PE_FindVDMStateActiveFlag (u8PortNum);

	u8RetVal = PRL_ReceiveMsg (u8PortNum, &u8SOPType, &u32Header, u8DataBuf, NULL);

    /* If any new Msg is received, pass it to the Receive Handler */
    if ((PRL_RET_MSG_RCVD & u8RetVal) || (PRL_RET_EXT_MSG_RCVD & u8RetVal))
    {
        /*Setting the Specification Revision as per section 6.2.1.1.5 from
        PD Specification 3.0*/
        if (DPM_GET_DEFAULT_PD_SPEC_REV (u8PortNum) > PRL_GET_PD_SPEC_REV (u32Header))
        {
            gasDPM[u8PortNum].u8DPM_Status &= ~DPM_CURR_PD_SPEC_REV_MASK;
            gasDPM[u8PortNum].u8DPM_Status |= ((PRL_GET_PD_SPEC_REV (u32Header)) << \
                                               DPM_CURR_PD_SPEC_REV_POS);
        }
        else
        {
            gasDPM[u8PortNum].u8DPM_Status &= ~DPM_CURR_PD_SPEC_REV_MASK;
            gasDPM[u8PortNum].u8DPM_Status |= ((DPM_GET_DEFAULT_PD_SPEC_REV (u8PortNum)) << \
                                               DPM_CURR_PD_SPEC_REV_POS);
        }

        /* Spec Rev is updated by PRL*/
        PRL_UpdateSpecAndDeviceRoles (u8PortNum);

        gasPolicy_Engine[u8PortNum].u32MsgHeader = u32Header;
        PE_ReceiveMsgHandler (u8PortNum, u32Header);
    }

    /*Setting Timeout sub-state to invalid state*/
    gasPolicy_Engine[u8PortNum].ePETimeoutSubState = ePE_INVALIDSUBSTATE;

    if(DPM_GET_CURRENT_POWER_ROLE (u8PortNum) == PD_ROLE_SOURCE)
    {
        #if INCLUDE_PD_SOURCE
        PE_SrcRunStateMachine (u8PortNum, u8DataBuf, u8SOPType,u32Header);
        #endif
    }
    else if(DPM_GET_CURRENT_POWER_ROLE (u8PortNum) == PD_ROLE_SINK)
    {
        #if INCLUDE_PD_SINK
        PE_SnkRunStateMachine (u8PortNum, u8DataBuf, u8SOPType,u32Header);
        #endif
    }

    PE_RunCommonStateMachine (u8PortNum, u8DataBuf, u8SOPType,u32Header);
}

UINT8 PE_IsMsgUnsupported (UINT8 u8PortNum, UINT16 u16Header)
{
    UINT8 u8MsgType;
    UINT8 u8RetVal = PE_SUPPORTED_MSG;

    u8MsgType = PRL_GET_MESSAGE_TYPE (u16Header);

    /*Extended messages are not supported by the policy engine*/
    if (PRL_IS_EXTENDED_MSG (u16Header))
    {
        if ((PE_EXT_FW_UPDATE_REQUEST != u8MsgType))
        {
            u8RetVal = PE_UNSUPPORTED_MSG;
        }
        else
        {
            #if (FALSE != INCLUDE_PDFU)
            u8RetVal = PE_SUPPORTED_EXTDMSG;
            #else
            u8RetVal = PE_UNSUPPORTED_MSG;
            #endif
        }
    }
    else
    {
        if(PE_OBJECT_COUNT_0 == PRL_GET_OBJECT_COUNT (u16Header))
        {
            /*GotoMin, PR SWAP, DR SWAP are not Supported*/
            /*Control messages in Table 6-5 of PD 3.0 Spec after "Not Supported" message are
            are not supported*/
            /*Any control message with message type between soft reset and Not supported are
            not supported as they are reserved fields*/
            if ((PE_CTRL_GOTO_MIN == u8MsgType) || (PE_CTRL_DR_SWAP == u8MsgType) ||
                (PE_CTRL_PR_SWAP == u8MsgType) || (u8MsgType > PE_CTRL_NOT_SUPPORTED)  \
               || ((u8MsgType > PE_CTRL_SOFT_RESET) && (u8MsgType < PE_CTRL_NOT_SUPPORTED)) )
            {
                u8RetVal = PE_UNSUPPORTED_MSG;
            }
            else if ((DPM_GET_DEFAULT_DATA_ROLE (u8PortNum) == PD_ROLE_SOURCE) && \
                     ((PE_CTRL_GET_SINK_CAP == u8MsgType) || ((PE_CTRL_PING == u8MsgType))))
            {
                u8RetVal = PE_UNSUPPORTED_MSG;
            }

            else if ((DPM_GET_DEFAULT_DATA_ROLE (u8PortNum) == PD_ROLE_SINK) && \
                     ((PE_CTRL_GET_SOURCE_CAP == u8MsgType)))
            {
                u8RetVal = PE_UNSUPPORTED_MSG;
            }
        }
        else
        {
            /*Message type greater than Sink_Capabilities except Vendor_Defined are not supported
            Refer Table 6-6*/
            if ((u8MsgType > PE_DATA_SINK_CAP) && (u8MsgType != PE_DATA_VENDOR_DEFINED) )
            {
                u8RetVal = PE_UNSUPPORTED_MSG;
            }

            else if ((DPM_GET_DEFAULT_DATA_ROLE (u8PortNum) == PD_ROLE_SOURCE) && \
                     (PE_DATA_SOURCE_CAP == u8MsgType))
            {
                u8RetVal = PE_UNSUPPORTED_MSG;
            }

            else if ((DPM_GET_DEFAULT_DATA_ROLE (u8PortNum) == PD_ROLE_SINK) && \
                     ((PE_DATA_SINK_CAP == u8MsgType) || (PE_DATA_REQUEST == u8MsgType)))
            {
                u8RetVal = PE_UNSUPPORTED_MSG;
            }

            else
            {
                /* Do Nothing */
            }
        }
    }

    return u8RetVal;
}

void PE_SendNotSupportedOrRejectMsg (UINT8 u8PortNum)
{
    /*Send Not Supported message if Current PD Specification is 3.0*/
    if ((DPM_GET_CURRENT_PD_SPEC_REV (u8PortNum) == PD_SPEC_REVISION_3_0))
    {
        gasPolicy_Engine[u8PortNum].ePEState = ePE_SEND_NOT_SUPPORTED;
        gasPolicy_Engine[u8PortNum].ePESubState = ePE_SEND_NOT_SUPPORTED_ENTRY_SS;
    }
    /*Send Reject message if Current PD Specification is 2.0*/
    else
    {
        gasPolicy_Engine[u8PortNum].ePEState = ePE_SEND_REJECT;
        gasPolicy_Engine[u8PortNum].ePESubState = ePE_SEND_REJECT_ENTRY_SS;
    }
}

void PE_SendHardResetMsg (UINT8 u8PortNum)
{
    if (DPM_GET_CURRENT_POWER_ROLE(u8PortNum) == PD_ROLE_SOURCE)
    {
        gasPolicy_Engine[u8PortNum].ePEState = ePE_SRC_HARD_RESET;
        gasPolicy_Engine[u8PortNum].ePESubState = ePE_SRC_HARD_RESET_ENTRY_SS;
    }
    else
    {
        gasPolicy_Engine[u8PortNum].ePEState = ePE_SNK_HARD_RESET;
        gasPolicy_Engine[u8PortNum].ePESubState = ePE_SNK_HARD_RESET_SEND_SS;
    }
}

void PE_SendSoftResetMsg (UINT8 u8PortNum)
{
    if (DPM_GET_CURRENT_POWER_ROLE(u8PortNum) == PD_ROLE_SOURCE)
    {
        gasPolicy_Engine[u8PortNum].ePEState = ePE_SRC_SEND_SOFT_RESET;
        gasPolicy_Engine[u8PortNum].ePESubState = ePE_SRC_SEND_SOFT_RESET_SOP_SS;
    }
    else
    {
        gasPolicy_Engine[u8PortNum].ePEState = ePE_SNK_SEND_SOFT_RESET;
        gasPolicy_Engine[u8PortNum].ePESubState = ePE_SNK_SEND_SOFT_RESET_ENTRY_SS;
    }
}

UINT8 PE_ValidateMessage (UINT8 u8PortNum, UINT32 u32Header)
{
    UINT8 u8RetVal = FALSE;

    /*Check whether the message is supported by the policy engine*/
    /*Message header is passed without the extender header bytes*/
    UINT8 u8ChkMsgSupport = PE_IsMsgUnsupported (u8PortNum, PRL_GET_MSG_HEADER(u32Header));

    if (PE_UNSUPPORTED_MSG == u8ChkMsgSupport)
    {
        if ((ePE_SRC_READY == gasPolicy_Engine[u8PortNum].ePEState) ||
           (ePE_SNK_READY == gasPolicy_Engine[u8PortNum].ePEState))
        {
            PE_SendNotSupportedOrRejectMsg(u8PortNum);
            u8RetVal = PE_MSG_HANDLED;
        }
        else
        {
            /*AMS Type is interruptable*/
            if ((gasPolicy_Engine[u8PortNum].u8PEPortSts & PE_AMS_TYPE))
            {
                u8RetVal = PE_PROCESS_MSG;
            }
            /*AMS Type is Non interruptable*/
            else
            {
                PE_HandleUnExpectedMsg (u8PortNum);
            }
        }
    }
    else if(PE_SUPPORTED_MSG == u8ChkMsgSupport)
    {
        u8RetVal = PE_PROCESS_MSG;
    }
    else if(PE_SUPPORTED_EXTDMSG == u8ChkMsgSupport)
    {
        u8RetVal = PE_PROCESS_MSG;
    }
    else
    {

    }

    return u8RetVal;
}

void PE_HandleRcvdMsgAndTimeoutEvents (UINT8 u8PortNum, ePolicyState eNextState , ePolicySubState eNextSubState)
{

    if(gasPolicy_Engine[u8PortNum].ePETimeoutSubState == ePE_INVALIDSUBSTATE)
    {
        gasPolicy_Engine[u8PortNum].ePEState = eNextState;
        gasPolicy_Engine[u8PortNum].ePESubState = eNextSubState;
    }
    else
    {
        if(gasPolicy_Engine[u8PortNum].u32TimeoutMsgHeader == gasPolicy_Engine[u8PortNum].u32MsgHeader)
        {
            gasPolicy_Engine[u8PortNum].ePEState = eNextState;
            gasPolicy_Engine[u8PortNum].ePESubState = eNextSubState;
        }

        else
        {
            gasPolicy_Engine[u8PortNum].ePESubState = gasPolicy_Engine[u8PortNum].ePETimeoutSubState;
        }
    }
}

void PE_HandleUnExpectedMsg (UINT8 u8PortNum)
{
    /*Send Hard reset if Current State is in power transition*/
    if ((ePE_SRC_TRANSITION_SUPPLY == gasPolicy_Engine[u8PortNum].ePEState) || \
      (ePE_SNK_TRANSITION_SINK == gasPolicy_Engine[u8PortNum].ePEState))
    {
        PE_SendHardResetMsg (u8PortNum);
    }
    /*Send soft reset if  Current State is not power transitioned*/
    else
    {
        PE_SendSoftResetMsg (u8PortNum);
    }
}


void PE_ReceiveMsgHandler (UINT8 u8PortNum, UINT32 u32Header)
{
    UINT8 u8MsgOperation = PE_ValidateMessage (u8PortNum, u32Header);
    /* Validate the received message*/
    if (PE_PROCESS_MSG == u8MsgOperation)
    {   
        #if (FALSE != INCLUDE_PDFU)
        if((PE_FWUP_GET_CURRENT_STATE(u8PortNum) == ePE_PDFU_MODE) && (PE_EXT_FW_UPDATE_REQUEST != PRL_GET_MESSAGE_TYPE (u32Header)))
        {
          /** if in PDFU State, any other Standard PD Messages are received,
           then those messages shall be processed as if it is in SRC_READY/SNK_READY state*/
            PE_FwUpdtKillTimer(u8PortNum);
            PE_FwUpdtResetToEnumState(u8PortNum);
            PE_FwUpdtInitialize();
        }
        #endif
        
        /* Process Message to be added down*/
        if (PE_OBJECT_COUNT_0 != PRL_GET_OBJECT_COUNT (u32Header))
        {
            switch (PRL_GET_MESSAGE_TYPE (u32Header))
            {
                case PE_DATA_SOURCE_CAP:
                {
                    /*Discard the VDM AMS and process the source capability message received*/
                    if ((ePE_SNK_WAIT_FOR_CAPABILITIES_WAIT_SS == \
                         gasPolicy_Engine[u8PortNum].ePESubState)|| (ePE_SNK_READY_IDLE_SS == \
                         gasPolicy_Engine[u8PortNum].ePESubState) || \
                       (gasDPM[u8PortNum].u8DPM_Status & DPM_VDM_STATE_ACTIVE_MASK))
                    {

                        if (ePE_SNK_WAIT_FOR_CAPABILITIES_WAIT_SS == \
                            gasPolicy_Engine[u8PortNum].ePESubState)
                        {
                            /*Kill the CONFIG_PE_SINK_WAIT_CAP_TIMEOUT since the source
                            capability message has been received*/
                            PE_KillPolicyEngineTimer (u8PortNum);
                            PE_HandleRcvdMsgAndTimeoutEvents (u8PortNum,\
                            ePE_SNK_EVALUATE_CAPABILITY,(ePolicySubState)0);

                        }
                        else
                        {
                            gasPolicy_Engine[u8PortNum].ePEState = ePE_SNK_EVALUATE_CAPABILITY;

                        }

                    }
                    else
                    {
                         PE_HandleUnExpectedMsg (u8PortNum);
                    }
                    break;
                }

                case PE_DATA_REQUEST:
                {
                    if (ePE_SRC_SEND_CAPABILITIES == gasPolicy_Engine[u8PortNum].ePEState ||
                       ePE_SRC_READY == gasPolicy_Engine[u8PortNum].ePEState)
                    {
                        /* Kill SenderResponse timer only in ePE_SRC_SEND_CAPABILITIES */
                        if (ePE_SRC_SEND_CAPABILITIES == gasPolicy_Engine[u8PortNum].ePEState)
                        {
                            PE_KillPolicyEngineTimer (u8PortNum);
                        }
                        
                        PE_HandleRcvdMsgAndTimeoutEvents (u8PortNum,ePE_SRC_NEGOTIATE_CAPABILITY,(ePolicySubState)0);
                        DEBUG_PRINT_PORT_STR (u8PortNum,"PE_DATA_REQUEST: Source Capability message received\r\n");
                    }

                    else
                    {
                        PE_HandleUnExpectedMsg (u8PortNum);
                    }

                    break;
                }
                case PE_DATA_BIST:
                {
                    if(((ePE_SNK_READY == gasPolicy_Engine[u8PortNum].ePEState) || \
                            (ePE_SRC_READY == gasPolicy_Engine[u8PortNum].ePEState)) &&	\
                            (TYPEC_VBUS_5V == DPM_GetVBUSVoltage(u8PortNum)))
                    {
                        gasPolicy_Engine[u8PortNum].ePEState = ePE_BIST_MODE;
                        gasPolicy_Engine[u8PortNum].ePESubState = ePE_BIST_MODE_ENTRY_SS;
                    }
                    break;
                }
                case PE_DATA_VENDOR_DEFINED:
                {
                    /*Handle the VDM messages if already in ready state*/
                    /*Also discard the current VDM AMS to handle the new VDM message*/
                    if ((ePE_SNK_READY == gasPolicy_Engine[u8PortNum].ePEState) || \
						(ePE_SRC_READY == gasPolicy_Engine[u8PortNum].ePEState) || \
                         (gasDPM[u8PortNum].u8DPM_Status & DPM_VDM_STATE_ACTIVE_MASK))
                    {

                        gasPolicy_Engine[u8PortNum].ePEState = ePE_VDM_GET_IDENTITY;
                    }

                    else if (ePE_SRC_VDM_IDENTITY_REQUEST == gasPolicy_Engine[u8PortNum].ePEState)
                    {
                        PE_KillPolicyEngineTimer (u8PortNum);
                        PE_HandleRcvdMsgAndTimeoutEvents( u8PortNum, ePE_SRC_VDM_IDENTITY_ACKED,(ePolicySubState)0);
                    }

                    break;
                }
                case PE_EXT_FW_UPDATE_REQUEST:
                {
                    #if (FALSE != INCLUDE_PDFU)

                    /***if current state is Source Ready / Sink ready or PDFU Mode**/
                    if((((PD_ROLE_SOURCE == DPM_GET_CURRENT_POWER_ROLE (u8PortNum)) && (ePE_SRC_READY == PE_FWUP_GET_CURRENT_STATE(u8PortNum)))||
                      ((PD_ROLE_SINK == DPM_GET_CURRENT_POWER_ROLE (u8PortNum))&& (ePE_SNK_READY == PE_FWUP_GET_CURRENT_STATE(u8PortNum)))) ||
                      (ePE_PDFU_MODE == PE_FWUP_GET_CURRENT_STATE(u8PortNum)))
                    {
                        if(TRUE == MCHP_PSF_HOOK_IS_PDFU_ALLOWED_NOW())
                        {
                            UINT8 u8PDFUActivePortNum;              
                            u8PDFUActivePortNum = PE_FwUpdtGetPDFUActivePortNum();

                            if ((u8PDFUActivePortNum >= CONFIG_PD_PORT_COUNT) || (u8PDFUActivePortNum == u8PortNum))
                            {
                                /**PD FW UPdate process is not active on any of the ports (or)
                                The PDFU Msg is received on the same port on which PDFU is already active*/
                                if(ePE_PDFU_MODE != PE_FWUP_GET_CURRENT_STATE(u8PortNum))
                                {
                                    gasPolicy_Engine[u8PortNum].ePEState = ePE_PDFU_MODE;
                                    gasPolicy_Engine[u8PortNum].ePESubState = ePE_FWUP_ENUMERATION_SS;
                                    PE_FwUpdtSetMsgRcvStatus();
                                }
                                else if(PE_FwUpdtGetTimeoutStatus())
                                {
                                  /**Both Message event and Timeout event Simultaneously present*/
                                    if(gasPolicy_Engine[u8PortNum].u32TimeoutMsgHeader == gasPolicy_Engine[u8PortNum].u32MsgHeader)
                                    {
                                        /** Before Timeout, message was received.*/
                                        PE_FwUpdtClearTimeoutStatus();

                                        PE_FwUpdtSetMsgRcvStatus();
                                    }
                                    else
                                    {
                                        /**Timeout happened before*/
                                        /** Ignore the message */
                                    }
                                }
                                else
                                {
                                  /** Kill if any timer is running */
                                    PE_FwUpdtKillTimer(u8PortNum);
                                  /** Post Message recieved notification */
                                    PE_FwUpdtSetMsgRcvStatus();
                                }
                            }
                            else
                            {
                                gasPolicy_Engine[u8PortNum].ePEState = ePE_PDFU_MODE;
                                gasPolicy_Engine[u8PortNum].ePESubState = ePE_FWUP_ENUMERATION_SS;
                                gsPdfuInfo.u8EventType |= PE_FWUP_INTERRUPTION_EVT;
                                /** PDFU is active on other port so do not allow this request to PDFU State Machine*/
                            }
                        }
                        else
                        {
                            gasPolicy_Engine[u8PortNum].ePEState = ePE_PDFU_MODE;
                            gasPolicy_Engine[u8PortNum].ePESubState = ePE_FWUP_ENUMERATION_SS;              
                             /** PDFU cannot be allowed currently */
                            gsPdfuInfo.u8EventType |= PE_FWUP_INTERRUPTION_EVT;
                        }
                    }
                    else
                    {
                      
                      PE_HandleUnExpectedMsg (u8PortNum);
                      
                        /** PDFU Message received during Some other PD State
                        Handle the illegal message */
                    }
                    #endif
                    break;
                }                
                default:
                {
                    break;
                }
            }
            
        }
        else
        {
            switch (PRL_GET_MESSAGE_TYPE (u32Header))
            {
                case PE_CTRL_ACCEPT:
                {
                    /*Accept message received for Sink Data request*/
                    if (ePE_SNK_SELECT_CAPABILITY_WAIT_FOR_ACCEPT_SS == \
                        gasPolicy_Engine[u8PortNum].ePESubState)
                    {
                        /*kill the timer CONFIG_PE_SENDER_RESPONSE_TIMEOUTID*/
                        DEBUG_PRINT_PORT_STR (u8PortNum,"PE_SNK_SELECT_CAPABILITY: Accept Msg Received\r\n");
                       PE_KillPolicyEngineTimer (u8PortNum);

                         PE_HandleRcvdMsgAndTimeoutEvents (u8PortNum,ePE_SNK_TRANSITION_SINK,\
                                                                   ePE_SNK_TRANSITION_SINK_ENTRY_SS);
                    }
                    /*Accept message received for soft reset sent by Sink*/
                    else if (ePE_SNK_SEND_SOFT_RESET_WAIT_FOR_ACCEPT_SS == \
                             gasPolicy_Engine[u8PortNum].ePESubState)
                    {
                         DEBUG_PRINT_PORT_STR (u8PortNum,"PE_SNK_SEND_SOFT_RESET: Accept Msg received\r\n");
                        /*kill the timer CONFIG_PE_SENDER_RESPONSE_TIMEOUTID*/
                         PE_KillPolicyEngineTimer (u8PortNum);

                         PE_HandleRcvdMsgAndTimeoutEvents (u8PortNum,ePE_SNK_WAIT_FOR_CAPABILITIES,\
                                                                   ePE_SNK_WAIT_FOR_CAPABILITIES_ENTRY_SS);

                    }
                    else if ((ePE_SRC_SEND_SOFT_RESET_IDLE_SS == gasPolicy_Engine[u8PortNum].ePESubState) ||
                            (ePE_SRC_SEND_SOFT_RESET_GOODCRC_RECEIVED_SS == gasPolicy_Engine[u8PortNum].ePESubState))
                    {
                        DEBUG_PRINT_PORT_STR (u8PortNum,"PE_CTRL_ACCEPT: SenderResponse Timer Killed\r\n");
                        PE_KillPolicyEngineTimer (u8PortNum);

                        gasPolicy_Engine[u8PortNum].ePEState = ePE_SRC_SEND_CAPABILITIES;
                        gasPolicy_Engine[u8PortNum].ePESubState = ePE_SRC_SEND_CAP_ENTRY_SS;

                        /*TODO:Add PE_CheckRcvdMsg_n_TimeoutSynchronication API call*/
                    }
                    else
                    {
                        PE_HandleUnExpectedMsg (u8PortNum);
                    }

                    break;
                }

                case PE_CTRL_WAIT:
                case PE_CTRL_REJECT:
                {
                    if (ePE_SNK_SELECT_CAPABILITY_WAIT_FOR_ACCEPT_SS == \
                        gasPolicy_Engine[u8PortNum].ePESubState)
                    {
                        if ((gasPolicy_Engine[u8PortNum].u8PEPortSts & PE_PDCONTRACT_MASK) == \
                            PE_EXPLICIT_CONTRACT)
                        {
                            /*Go to "ePE_SNK_READY" if Wait/Reject message is received and there is
                            an explicit contract present*/
                            /*TODO: Set the Sink request timer active flag and kill it once the src cap
                            is received before timeout*/
                            if ((PRL_GET_MESSAGE_TYPE (u32Header)) == PE_CTRL_WAIT)
                            {
                                gasTypeCcontrol[u8PortNum].u8TypeC_TimerID = PDTimer_Start (CONFIG_PE_SINKREQUEST_TIMEOUT_MS, PE_StateChange_TimerCB,\
                                               u8PortNum,(UINT8) ePE_SNK_SELECT_CAPABILITY);

                                gasPolicy_Engine[u8PortNum].ePEState = ePE_SNK_READY;
                                gasPolicy_Engine[u8PortNum].ePESubState = ePE_SNK_READY_IDLE_SS;

                            }
                            else if ((PRL_GET_MESSAGE_TYPE(u32Header)) == PE_CTRL_REJECT)
                            {
                                gasPolicy_Engine[u8PortNum].ePEState = ePE_SNK_READY;
                                gasPolicy_Engine[u8PortNum].ePESubState = ePE_SNK_READY_IDLE_SS;
                            }
                        }
                        /*Go to "ePE_SNK_WAIT_FOR_CAPABILITIES" if Wait/Reject message is
                        received and there is no explicit contract present*/
                        else
                        {
                            gasPolicy_Engine[u8PortNum].ePEState = ePE_SNK_WAIT_FOR_CAPABILITIES;
                            gasPolicy_Engine[u8PortNum].ePESubState = ePE_SNK_WAIT_FOR_CAPABILITIES_ENTRY_SS;
                        }
                    }
                    else
                    {
                        PE_HandleUnExpectedMsg (u8PortNum);
                    }
                    break;
                }
                 case PE_CTRL_PS_RDY:
                {
                    /*PS RDY received from source after accept message*/
                    if (ePE_SNK_TRANSITION_SINK_WAIT_FOR_PSRDY_SS == \
                        gasPolicy_Engine[u8PortNum].ePESubState)
                    {
                        /*Kill the timer CONFIG_PE_PSTRANSITION_TIMEOUTID*/
                        PE_KillPolicyEngineTimer (u8PortNum);

                        PE_HandleRcvdMsgAndTimeoutEvents (u8PortNum,ePE_SNK_READY,\
                                                                   ePE_SNK_READY_ENTRY_SS);

                    }
                    /*PS RDY received from VCONN Source partner*/
                    else if (ePE_VCS_WAIT_FOR_VCONN_WAIT_FOR_PS_RDY_SS == \
                             gasPolicy_Engine[u8PortNum].ePESubState)
                    {
                        /*Kill the timer VCONNON timer*/
                        PE_KillPolicyEngineTimer (u8PortNum);

                        PE_HandleRcvdMsgAndTimeoutEvents (u8PortNum,ePE_VCS_TURN_OFF_VCONN,\
                                           ePE_VCS_TURN_OFF_VCONN_ENTRY_SS);
                    }
                    else
                    {
                        PE_HandleUnExpectedMsg (u8PortNum);
                    }
                    break;
                }
                case PE_CTRL_GET_SOURCE_CAP:
                {
                    if (ePE_SRC_READY == gasPolicy_Engine[u8PortNum].ePEState)
                    {
                        DEBUG_PRINT_PORT_STR (u8PortNum,"PE_CTRL_GET_SOURCE_CAP: Get Source Cap Received\r\n");
                        gasPolicy_Engine[u8PortNum].ePEState = ePE_SRC_SEND_CAPABILITIES;
                        gasPolicy_Engine[u8PortNum].ePESubState = ePE_SRC_SEND_CAP_ENTRY_SS;
                    }
                    else
                    {
                        PE_HandleUnExpectedMsg (u8PortNum);
                    }

                    break;
                }

                case PE_CTRL_GET_SINK_CAP:
                {
                    /*Discard VDM AMS and handle the PD message here*/
                    if ((ePE_SNK_READY == gasPolicy_Engine[u8PortNum].ePEState) || \
                      (gasDPM[u8PortNum].u8DPM_Status & DPM_VDM_STATE_ACTIVE_MASK))
                    {
                        /*Go to "ePE_SNK_GIVE_SINK_CAP" state if GET_SINK_CAP message is received*/
                        gasPolicy_Engine[u8PortNum].ePEState = ePE_SNK_GIVE_SINK_CAP;
                        gasPolicy_Engine[u8PortNum].ePESubState = ePE_SNK_GIVE_SINK_CAP_ENTRY_SS;

                    }
                    else
                    {
                        PE_HandleUnExpectedMsg (u8PortNum);
                    }
                    break;
                }

                case PE_CTRL_VCONN_SWAP:
                {

                    /*Accept the VCONN swap if current state is READY State or
                    any VDM AMS is active*/
                    if ((ePE_SNK_READY == gasPolicy_Engine[u8PortNum].ePEState) || \
                        (ePE_SRC_READY == gasPolicy_Engine[u8PortNum].ePEState) || \
                         (gasDPM[u8PortNum].u8DPM_Status & DPM_VDM_STATE_ACTIVE_MASK))
                    {

#if INCLUDE_VCONN_SWAP_SUPPORT
                                          
                        gasPolicy_Engine[u8PortNum].ePEState = ePE_VCS_EVALUATE_SWAP;
#else
                        
                        PE_SendNotSupportedOrRejectMsg(u8PortNum);
#endif

                    }
                    else
                    {
                        PE_HandleUnExpectedMsg (u8PortNum);
                    }
                    break;
                }

                case PE_CTRL_SOFT_RESET:
                {
                    if (DPM_GET_CURRENT_POWER_ROLE(u8PortNum) == PD_ROLE_SOURCE)
                    {
                        gasPolicy_Engine[u8PortNum].ePEState = ePE_SRC_SOFT_RESET;
                        gasPolicy_Engine[u8PortNum].ePESubState = ePE_SRC_SOFT_RESET_ENTRY_SS;
                    }
                    else
                    {
                        /*Go to "ePE_SNK_SOFT_RESET" state if SOFT_RESET message is received*/
                        gasPolicy_Engine[u8PortNum].ePEState = ePE_SNK_SOFT_RESET;
                        gasPolicy_Engine[u8PortNum].ePESubState = ePE_SNK_SOFT_RESET_SEND_ACCEPT_SS;

                    }
                    break;
                }
                default:
                {
                    break;
                }
            }

        }
    }
    else 
    {
        
    }


}

void PE_RunCommonStateMachine(UINT8 u8PortNum , UINT8 *pu8DataBuf , UINT8 u8SOPType ,UINT32 u32Header)
{

    /*Initialize the transmit message type as SOP since only SOP message is transmitted in
    this state machine*/
    UINT8 u8TransmitSOP= PRL_SOP_TYPE;
    UINT32 u32Transmit_Header = SET_TO_ZERO;
    UINT32 *u32pTransmit_DataObj = SET_TO_ZERO;
    PRLTxCallback Transmit_cb = NULL;
    UINT32 u32Transmit_TmrID_TxSt= SET_TO_ZERO;
    UINT8 u8IsTransmit= FALSE;
#if (FALSE != INCLUDE_PDFU)
    static UINT8 u8TempRespBuffer[3];
#endif
    switch (gasPolicy_Engine[u8PortNum].ePEState)
    {

#if INCLUDE_PD_3_0
       case ePE_SEND_NOT_SUPPORTED:
       {
            switch (gasPolicy_Engine[u8PortNum].ePESubState)
            {
                case ePE_SEND_NOT_SUPPORTED_ENTRY_SS:
                {
                    DEBUG_PRINT_PORT_STR (u8PortNum,"PE_SEND_NOT_SUPPORTED: Enterted the state\r\n");

                    u32Transmit_Header = PRL_FormSOPTypeMsgHeader (u8PortNum, PE_CTRL_NOT_SUPPORTED, \
                                                    PE_OBJECT_COUNT_0, PE_NON_EXTENDED_MSG);
                    u8TransmitSOP = PRL_SOP_TYPE;
                    u32pTransmit_DataObj = NULL;

                    Transmit_cb = PE_StateChange_TransmitCB;

                    if ((gasPortConfigurationData[u8PortNum].u32CfgData & TYPEC_PORT_TYPE_MASK) == \
                        PD_ROLE_SOURCE)
                    {
                        /*Set the transmitter callback to transition to source soft reset state if
                        message transmission fails*/
                        u32Transmit_TmrID_TxSt = PRL_BUILD_PKD_TXST_U32 (ePE_SRC_READY, \
                                                 ePE_SRC_READY_END_AMS_SS, ePE_SRC_SEND_SOFT_RESET,\
                                             ePE_SRC_SEND_SOFT_RESET_SOP_SS);
                    }
                    else
                    {
                        /*Set the transmitter callback to transition to sink soft reset state if
                        message transmission fails*/
                        u32Transmit_TmrID_TxSt = PRL_BUILD_PKD_TXST_U32 (ePE_SNK_READY, ePE_SNK_READY_IDLE_SS,\
                                             ePE_SNK_SEND_SOFT_RESET,ePE_SNK_SEND_SOFT_RESET_ENTRY_SS);
                    }

                    u8IsTransmit = TRUE;
                    gasPolicy_Engine[u8PortNum].ePESubState = ePE_SEND_NOT_SUPPORTED_IDLE_SS;

                    break;
                }
                /*Wait here until the message is sent*/
                case ePE_SEND_NOT_SUPPORTED_IDLE_SS:
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
#endif
       case ePE_SEND_REJECT:
       {
            switch (gasPolicy_Engine[u8PortNum].ePESubState)
            {
                case ePE_SEND_REJECT_ENTRY_SS:
                {
                    DEBUG_PRINT_PORT_STR (u8PortNum,"ePE_SEND_REJECT: Enterted the state\r\n");

                    u32Transmit_Header = PRL_FormSOPTypeMsgHeader (u8PortNum, PE_CTRL_REJECT, \
                                                    PE_OBJECT_COUNT_0, PE_NON_EXTENDED_MSG);
                    u8TransmitSOP = PRL_SOP_TYPE;
                    u32pTransmit_DataObj = NULL;

                    Transmit_cb = PE_StateChange_TransmitCB;

                    if ((gasPortConfigurationData[u8PortNum].u32CfgData & TYPEC_PORT_TYPE_MASK) == \
                        PD_ROLE_SOURCE)
                    {
                        /*Set the transmitter callback to transition to source soft reset state if
                        message transmission fails*/
                        u32Transmit_TmrID_TxSt = PRL_BUILD_PKD_TXST_U32 (ePE_SRC_READY,\
                                                 ePE_SRC_READY_END_AMS_SS, ePE_SRC_SEND_SOFT_RESET,\
                                                 ePE_SRC_SEND_SOFT_RESET_SOP_SS);
                    }
                    else
                    {
                        /*Set the transmitter callback to transition to sink soft reset state if
                        message transmission fails*/
                        u32Transmit_TmrID_TxSt = PRL_BUILD_PKD_TXST_U32 (ePE_SNK_READY,\
                                                 ePE_SNK_READY_IDLE_SS,ePE_SNK_SEND_SOFT_RESET,\
                                                 ePE_SNK_SEND_SOFT_RESET_ENTRY_SS);
                    }

                    u8IsTransmit = TRUE;
                    gasPolicy_Engine[u8PortNum].ePESubState = ePE_SEND_REJECT_IDLE_SS;

                    break;
                }
                /*Wait here until the message is sent*/
                case ePE_SEND_REJECT_IDLE_SS:
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

#if INCLUDE_VCONN_SWAP_SUPPORT

        case ePE_VCS_EVALUATE_SWAP:
        {
            /*Transition directly to next state as UPD301A accepts VCONN Swap always*/
            DEBUG_PRINT_PORT_STR (u8PortNum,"PE_VCS_EVALUATE_SWAP: Enterted the state\r\n");

#if INCLUDE_POWER_FAULT_HANDLING
            
            /*Send Not Supported or Reject if Port partner requests VCONN Swap to supply the
            VCONN when the u8VCONNGoodtoSupply is false */
            if((!DPM_IsPort_VCONN_Source(u8PortNum)) && (!gasDPM[u8PortNum].u8VCONNGoodtoSupply))
            {
                PE_SendNotSupportedOrRejectMsg(u8PortNum);
            }
            else
            {
                gasPolicy_Engine[u8PortNum].ePEState = ePE_VCS_ACCEPT_SWAP;
                gasPolicy_Engine[u8PortNum].ePESubState = ePE_VCS_ACCEPT_SWAP_SEND_ACCEPT_SS;
            }
#else 
            gasPolicy_Engine[u8PortNum].ePEState = ePE_VCS_ACCEPT_SWAP;
            gasPolicy_Engine[u8PortNum].ePESubState = ePE_VCS_ACCEPT_SWAP_SEND_ACCEPT_SS;
#endif

            break;
        }
        case ePE_VCS_ACCEPT_SWAP:
        {
            switch (gasPolicy_Engine[u8PortNum].ePESubState)
            {
                case ePE_VCS_ACCEPT_SWAP_SEND_ACCEPT_SS:
                {
                    DEBUG_PRINT_PORT_STR (u8PortNum,"PE_VCS_ACCEPT_SWAP-SEND_ACCEPT_SS: Enterted the SubState \r\n");

                     /*Send Accept message*/
                    /*Set the PD message transmitter API to Send Accept Messsage*/
                    u8TransmitSOP = PRL_SOP_TYPE;
                    u32Transmit_Header = PRL_FormSOPTypeMsgHeader (u8PortNum, PE_CTRL_ACCEPT,\
                                         PE_OBJECT_COUNT_0, PE_NON_EXTENDED_MSG);
                    u32pTransmit_DataObj = NULL;
                    Transmit_cb = PE_StateChange_TransmitCB;

                    if (DPM_GET_CURRENT_POWER_ROLE(u8PortNum) == PD_ROLE_SOURCE)
                    {
                        /*Set the transmitter callback to transition to source soft reset state if
                        message transmission fails*/
                        u32Transmit_TmrID_TxSt = PRL_BUILD_PKD_TXST_U32 (ePE_VCS_ACCEPT_SWAP,\
                                             ePE_VCS_ACCEPT_SWAP_ACCEPT_SENT_SS,\
                                             ePE_SRC_SEND_SOFT_RESET, ePE_SRC_SEND_SOFT_RESET_SOP_SS);

                    }
                    else
                    {
                        /*Set the transmitter callback to transition to sink soft reset state if
                        message transmission fails*/
                        u32Transmit_TmrID_TxSt = PRL_BUILD_PKD_TXST_U32 (ePE_VCS_ACCEPT_SWAP,\
                                             ePE_VCS_ACCEPT_SWAP_ACCEPT_SENT_SS,\
                                             ePE_SNK_SEND_SOFT_RESET, ePE_SNK_SEND_SOFT_RESET_ENTRY_SS);

                    }

                    gasPolicy_Engine[u8PortNum].ePESubState = ePE_VCS_ACCEPT_SWAP_IDLE_SS;
                    u8IsTransmit = TRUE;

                    break;

                }
                /*Wait here until the message is sent*/
                case ePE_VCS_ACCEPT_SWAP_IDLE_SS:
                {
                    break;
                }
                case ePE_VCS_ACCEPT_SWAP_ACCEPT_SENT_SS:
                {
                    DEBUG_PRINT_PORT_STR (u8PortNum,"PE_VCS_ACCEPT_SWAP-ACCEPT_SENT_SS: Enterted the SubState \r\n");

                    /*Check whether the Port is currently sourcing VCONN and
                    transition accordingly*/
                    if (DPM_IsPort_VCONN_Source(u8PortNum))
                    {
                         gasPolicy_Engine[u8PortNum].ePEState = ePE_VCS_WAIT_FOR_VCONN;
                         gasPolicy_Engine[u8PortNum].ePESubState = \
                           ePE_VCS_WAIT_FOR_VCONN_START_TIMER_SS;
                    }
                    else
                    {                      
                        gasPolicy_Engine[u8PortNum].ePEState = ePE_VCS_TURN_ON_VCONN;                        
                        gasPolicy_Engine[u8PortNum].ePESubState = ePE_VCS_TURN_ON_VCONN_ENTRY_SS;
                        
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
        case ePE_VCS_WAIT_FOR_VCONN:
        {
            switch (gasPolicy_Engine[u8PortNum].ePESubState)
            {
                case ePE_VCS_WAIT_FOR_VCONN_START_TIMER_SS:
                {
                     DEBUG_PRINT_PORT_STR (u8PortNum,"PE_VCS_WAIT_FOR_VCONN-START_TIMER_SS: Enterted the SubState \r\n");

                    /*If PS RDY message is not received from port partner, then transition to Hard
                    reset state*/

                    /*Start the VCONN timer*/
                    /*Set the timer callback to transition to ePE_VCS_WAIT_FOR_VCONN_SENDERRES_TO_SS
                    sub state if timeout happens*/
                    gasPolicy_Engine[u8PortNum].u8PETimerID = PDTimer_Start (\
                                                              CONFIG_PE_VCONNON_TIMEOUT_MS,\
                                                              PE_SubStateChangeAndTimeoutValidateCB,\
                                                              u8PortNum,\
                                                              (UINT8) ePE_VCS_WAIT_FOR_VCONN_SENDERRES_TO_SS);


                    gasPolicy_Engine[u8PortNum].ePESubState = ePE_VCS_WAIT_FOR_VCONN_WAIT_FOR_PS_RDY_SS;
                    break;
                }
                case ePE_VCS_WAIT_FOR_VCONN_WAIT_FOR_PS_RDY_SS:
                {
                    /*Wait for PS_RDY message*/
                    break;
                }
                case ePE_VCS_WAIT_FOR_VCONN_SENDERRES_TO_SS:
                {
                    PE_SendHardResetMsg (u8PortNum);
                    break;
                }
                default:
                {
                    break;
                }
            }
            break;
        }

        case ePE_VCS_TURN_OFF_VCONN:
        {          
            switch (gasPolicy_Engine[u8PortNum].ePESubState)
            {
              
                case ePE_VCS_TURN_OFF_VCONN_ENTRY_SS:
                {
                  
                    DEBUG_PRINT_PORT_STR(u8PortNum,"PE_VCS_TURN_OFF_VCONN: Enterted the state\r\n");

                    /*Turn off VCONN since PS RDY message is received from VCONN Source partner*/
                    DPM_VConnOnOff(u8PortNum,DPM_VCONN_OFF);            
                    
                    /*Start the VCONN_OFF timer*/
                    /*This Timeout is implemented outside of the PD Specification to track 
                    VCONN Turn OFF error*/
                    gasPolicy_Engine[u8PortNum].u8PETimerID = PDTimer_Start (\
                                                              CONFIG_PE_VCONNOFF_TIMEOUT_MS,\
                                                              DPM_VCONNOFFErrorTimerCB,\
                                                              u8PortNum,\
                                                              (UINT8)SET_TO_ZERO);
                    
                    gasPolicy_Engine[u8PortNum].ePESubState = ePE_VCS_TURN_OFF_VCONN_CHECK_SS;
            
                    break;            
                }
                case ePE_VCS_TURN_OFF_VCONN_CHECK_SS:
                {
                
                    if(!DPM_IsPort_VCONN_Source(u8PortNum))
                    {
                        /*Stop the VCONN_OFF timer*/
                        PE_KillPolicyEngineTimer (u8PortNum);
                        
                        if (DPM_GET_CURRENT_POWER_ROLE(u8PortNum) == PD_ROLE_SOURCE)
                        {
                            gasPolicy_Engine[u8PortNum].ePEState = ePE_SRC_READY;
                            gasPolicy_Engine[u8PortNum].ePESubState = ePE_SRC_READY_END_AMS_SS;
                        }
                        else
                        {
                            gasPolicy_Engine[u8PortNum].ePEState = ePE_SNK_READY;
                            gasPolicy_Engine[u8PortNum].ePESubState = ePE_SNK_READY_IDLE_SS;
                        }                   
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

        case ePE_VCS_TURN_ON_VCONN:
        {
          
            switch (gasPolicy_Engine[u8PortNum].ePESubState)
            {
                case ePE_VCS_TURN_ON_VCONN_ENTRY_SS:
                {
                  
                    DEBUG_PRINT_PORT_STR(u8PortNum,"PE_VCS_TURN_ON_VCONN: Enterted the state\r\n");
                    
                    /*Turn ON VCONN*/
                    DPM_VConnOnOff (u8PortNum,DPM_VCONN_ON);
                    
                    /*Port Partner maintains the tVCONNSourceOn timer, So setting the VCONN_ON_self
                    timer greater than tVCONNSourceOn to send Hard reset in case of VCONN ON
                    failure*/
                    /*Start the VCONN_ON_self timer*/
                    gasPolicy_Engine[u8PortNum].u8PETimerID = PDTimer_Start (\
                                                              CONFIG_PE_VCONNON_SELF_TIMEOUT_MS,\
                                                              DPM_VCONNONTimerErrorCB,\
                                                              u8PortNum,\
                                                              (UINT8)SET_TO_ZERO);
                    
                    gasPolicy_Engine[u8PortNum].ePESubState = ePE_VCS_TURN_ON_VCONN_CHECK_SS;
                    
                     break;
                  
                }
                
                case ePE_VCS_TURN_ON_VCONN_CHECK_SS:
                {                 
                    if(DPM_IsPort_VCONN_Source(u8PortNum))
                    {    
                      
                        PE_KillPolicyEngineTimer (u8PortNum);
                        gasPolicy_Engine[u8PortNum].ePEState = ePE_VCS_SEND_PS_RDY;
                        gasPolicy_Engine[u8PortNum].ePESubState = ePE_VCS_SEND_PS_RDY_ENTRY_SS;
                                       
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

        case ePE_VCS_SEND_PS_RDY:
        {
            switch (gasPolicy_Engine[u8PortNum].ePESubState)
            {
                case ePE_VCS_SEND_PS_RDY_ENTRY_SS:
                {
                    DEBUG_PRINT_PORT_STR (u8PortNum,"ePE_VCS_SEND_PS_RDY-ENTRY_SS: Enterted the SubState \r\n");
                    /*Send PS_RDY message*/
                    u8TransmitSOP = PRL_SOP_TYPE;
                    u32Transmit_Header = PRL_FormSOPTypeMsgHeader (u8PortNum, PE_CTRL_PS_RDY,\
                                                                   PE_OBJECT_COUNT_0, PE_NON_EXTENDED_MSG);
                    u32pTransmit_DataObj = NULL;
                    Transmit_cb = PE_StateChange_TransmitCB;

                    if (DPM_GET_CURRENT_POWER_ROLE (u8PortNum) == PD_ROLE_SOURCE)
                    {
                        /*Set the transmitter callback to transition to source soft reset state if
                        message transmission fails*/
                        u32Transmit_TmrID_TxSt = PRL_BUILD_PKD_TXST_U32 (ePE_SRC_READY,\
                                                 ePE_SRC_READY_END_AMS_SS,\
                                                 ePE_SRC_SEND_SOFT_RESET,\
                                                 ePE_SRC_SEND_SOFT_RESET_SOP_SS);
                    }

                    else
                    {
                        /*Set the transmitter callback to transition to sink soft reset state if
                        message transmission fails*/
                        u32Transmit_TmrID_TxSt = PRL_BUILD_PKD_TXST_U32 (\
                                                 ePE_SNK_READY,ePE_SNK_READY_IDLE_SS,\
                                                 ePE_SNK_SEND_SOFT_RESET,\
                                                 ePE_SNK_SEND_SOFT_RESET_ENTRY_SS);
                    }

                    u8IsTransmit = TRUE;
                    gasPolicy_Engine[u8PortNum].ePESubState = ePE_VCS_SEND_PS_RDY_IDLE_SS;

                     break;
                }
                /*Wait here until the PS_RDY message is sent*/
                case ePE_VCS_SEND_PS_RDY_IDLE_SS:
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
#endif
        
        case ePE_VDM_GET_IDENTITY:
        {
            DEBUG_PRINT_PORT_STR (u8PortNum,"PE_RESP_VDM_GET_IDENTITY: Entered the state\r\n");

            /*Convert the u8DataBuf into 32 bit header for easy handling*/
            UINT32 *u32RecvdVDMHeader = (UINT32 *)pu8DataBuf;

            if (PE_GET_VDM_CMD_TYPE (*u32RecvdVDMHeader) == PE_GET_VDM_CMD_TYPE_REQ)
            {
                /*Send NACK message for any received VDM message if PD Spec revision is 2.0*/
                if (DPM_GET_CURRENT_PD_SPEC_REV (u8PortNum) == PD_SPEC_REVISION_2_0)
                {                  
                    if(PD_ROLE_SOURCE == (DPM_GET_CURRENT_POWER_ROLE(u8PortNum)))
                    {
                        gasPolicy_Engine[u8PortNum].ePEState = ePE_SRC_READY;
                        gasPolicy_Engine[u8PortNum].ePESubState = ePE_SRC_READY_END_AMS_SS;
                    
                    }
                    else
                    {
                        gasPolicy_Engine[u8PortNum].ePEState = ePE_SNK_READY;
                        gasPolicy_Engine[u8PortNum].ePESubState = ePE_SNK_READY_IDLE_SS;
                    
                    }
                }
                 /*Send "Not Supported" message for any received VDM message if PD Spec revision is 3.0*/
                else
                {
                    gasPolicy_Engine[u8PortNum].ePEState = ePE_SEND_NOT_SUPPORTED;
                    gasPolicy_Engine[u8PortNum].ePESubState = ePE_SEND_NOT_SUPPORTED_ENTRY_SS;
                }
            }              
            break;
        }
		case ePE_BIST_MODE:
		{
            switch (gasPolicy_Engine[u8PortNum].ePESubState)
			{
                case ePE_BIST_MODE_ENTRY_SS:
                {
				  /*Convert the u8DataBuf into 32 bit header for easy handling*/
					UINT32 *u32BISTObj = (UINT32 *)pu8DataBuf;

					/* DATA OBJECT is checked for BIST carrier mode */
                    if (u32BISTObj[0]== PRL_BIST_CARRIER_MODE_OBJ)
                    {
                        /* BIST_CARRIER_MODE is configured*/
                        PRL_ConfigureBISTCarrierMode (u8PortNum, TRUE);
                        /* BISTContModeTimer is started */
                        (void)PDTimer_Start (CONFIG_PRL_BIST_CONTMODE_TIMEOUT_MS, PE_SubStateChange_TimerCB,\
                                u8PortNum,(UINT8)ePE_BIST_MODE_EXIT_SS);
                    }
                    else
                    {
                        /* in case of BIST Test Data mode disable the message processing
                        by FW.*/
                        gasPRL[u8PortNum].u8RxDisable = TRUE;
                    }
                    gasPolicy_Engine[u8PortNum].ePESubState = ePE_BIST_MODE_IDLE_SS;
                    break;
                }
                case ePE_BIST_MODE_IDLE_SS:
                {
				  	/*Idle state for BISTContModeTimer timeout*/
                    break;
                }
                case ePE_BIST_MODE_EXIT_SS:
                {
                    /* On BISTContModeTimer timeout BIST is reset*/
                    PRL_ConfigureBISTCarrierMode (u8PortNum, FALSE);
                    
                    /*Transition to ePE_SRC_TRANSITION_TO_DEFAULT state if current role is source*/
                    if (PD_ROLE_SOURCE == (DPM_GET_CURRENT_POWER_ROLE(u8PortNum)))
                    {
                        gasPolicy_Engine[u8PortNum].ePEState = ePE_SRC_TRANSITION_TO_DEFAULT;
                        gasPolicy_Engine[u8PortNum].ePESubState = ePE_SRC_TRANSITION_TO_DEFAULT_ENTRY_SS;
                    }
                    /*Transition to ePE_SNK_TRANSITION_TO_DEFAULT state if current role is sink*/
                    else
                    {
                        gasPolicy_Engine[u8PortNum].ePEState = ePE_SNK_TRANSITION_TO_DEFAULT;
                        gasPolicy_Engine[u8PortNum].ePESubState = ePE_SNK_TRANSITION_TO_DEFAULT_ENTRY_SS;
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
        #if (FALSE != INCLUDE_PDFU)
        case ePE_PDFU_MODE:
            
            u8TransmitSOP = PRL_SOP_TYPE;
            
            if(FALSE != (gsPdfuInfo.u8EventType & PE_FWUP_INTERRUPTION_EVT))
            {
                UINT8 u8CurrentPowerRole;
                ePolicySubState eSubState = ePE_INVALIDSUBSTATE;
                ePolicyState eState = ePE_INVALIDSTATE;
                
                u8TempRespBuffer[PE_FWUP_RESPBUFF_PROTOCOL_VERSION_INDEX] = PE_FWUP_PROTOCOL_VERSION;
                u8TempRespBuffer[PE_FWUP_RESPBUFF_RESPONSE_TYPE_INDEX] = ((~0x80) & pu8DataBuf[PE_FWUP_RESPBUFF_REQUEST_TYPE_INDEX]);
                u8TempRespBuffer[PE_FWUP_RESPBUFF_STATUS_INDEX] = (UINT8)ePE_FWUP_errUNKNOWN;
                //u8TempRespBuffer[PE_FWUP_RESPBUFF_STATUS_INDEX + 1u] = 0x00u; /** Padding byte */
                /**Clear the Interruption event*/
                gsPdfuInfo.u8EventType &= (~PE_FWUP_INTERRUPTION_EVT);
                
                u32Transmit_Header =  /**Combined Message Header*/
                PRL_FORM_COMBINED_MSG_HEADER(((1u << PRL_EXTMSG_CHUNKED_BIT_POS) | (PRL_EXTMSG_DATA_FIELD_MASK & 0x03)), /**Extended Msg Header*/
                                             PRL_FormSOPTypeMsgHeader(u8PortNum,PE_EXT_FW_UPDATE_RESPONSE,7, /**Standard Msg Header*/
                                                     PE_EXTENDED_MSG));
                /** Update the Pointer to Data Object */
                u32pTransmit_DataObj = (UINT32*)&u8TempRespBuffer[0u];
                
                u8IsTransmit = TRUE;
                
                u8CurrentPowerRole = DPM_GET_CURRENT_POWER_ROLE(u8PortNum);

                if (PD_ROLE_SOURCE == u8CurrentPowerRole)
                {
                    eState  = ePE_SRC_READY;
                    eSubState = ePE_SRC_READY_END_AMS_SS;
                }
                else
                if (PD_ROLE_SINK == u8CurrentPowerRole)
                {
                    eState  = ePE_SNK_READY;
                    eSubState = ePE_SNK_READY_IDLE_SS;     
                }
                else
                { }
                
                PE_FWUP_SET_CURRENT_STATE(u8PortNum, eState);
                PE_FWUP_CHANGE_SUBSTATE(u8PortNum, eSubState);
                
            }
            else
            {
                u8IsTransmit = PE_FwUpdtStateMachine(u8PortNum, &u32Header, pu8DataBuf);
                if(PE_FWUP_NO_RESPONSE != u8IsTransmit)
                {
                    u32Transmit_Header = u32Header;
                    /** Update the Pointer to Data Object */
                    u32pTransmit_DataObj = (UINT32*)&gsPdfuInfo.pu8ResponseBuffer[0u];

                    /** Update the Transmit Call back */
                    Transmit_cb = PE_FwUpdtTxDoneCallBack;

                    /**Update the Transmit Completed Callback State Transitions*/
                    u32Transmit_TmrID_TxSt = gsPdfuInfo.u32Transmit_TmrID_TxSt;
                }
            }
        break;
        #endif
        
        default:
        {
            break;
        }
    }

    if (u8IsTransmit != FALSE)
    {
        (void)PRL_TransmitMsg (u8PortNum, u8TransmitSOP, u32Transmit_Header, (UINT8 *)u32pTransmit_DataObj,\
                Transmit_cb, u32Transmit_TmrID_TxSt);
    }
}

void PE_StateChange_TransmitCB (UINT8 u8PortNum, UINT8 u8TXDoneState, UINT8 u8TxDoneSubState,\
  UINT8 u8TxFailedState, UINT8 u8TxFailedSubState)
{
    /*Policy engine state is assigned based on Transmitted message status*/
    switch (gasPRL[u8PortNum].u8TxStateISR)
    {
        case PRL_TX_DONE_ST:
        {
            gasPolicy_Engine[u8PortNum].ePEState = (ePolicyState) u8TXDoneState;
            gasPolicy_Engine[u8PortNum].ePESubState = (ePolicySubState) u8TxDoneSubState;
            break;
        }
        case PRL_TX_ABORTED_ST:
        {
            break;
        }

        case PRL_TX_FAILED_ST:
        {
            gasPolicy_Engine[u8PortNum].ePEState = (ePolicyState) u8TxFailedState;
            gasPolicy_Engine[u8PortNum].ePESubState = (ePolicySubState) u8TxFailedSubState;
            break;
        }
        
        default:
        {
            break;
        }

    }
}

void PE_SetHardResetReceiveFlag (UINT8 u8PortNum)
{
    /*Setting the Hard reset received flag*/
    gasPolicy_Engine[u8PortNum].u8HardResetRecvdISR = TRUE ;
}

void PE_StateChange_TimerCB (UINT8 u8PortNum, UINT8 u8PEState)
{
	gasPolicy_Engine[u8PortNum].ePEState = (ePolicyState) u8PEState;

    /*Setting the u8PETimerID to MAX_CONCURRENT_TIMERS to indicate that
    Timeout has occured*/
    gasPolicy_Engine[u8PortNum].u8PETimerID = MAX_CONCURRENT_TIMERS;
}

void PE_SubStateChange_TimerCB (UINT8 u8PortNum, UINT8 u8PESubState)
{
    /*Setting the u8PETimerID to MAX_CONCURRENT_TIMERS to indicate that
    Timeout has occured*/
    gasPolicy_Engine[u8PortNum].ePESubState = (ePolicySubState) u8PESubState;
    gasPolicy_Engine[u8PortNum].u8PETimerID = MAX_CONCURRENT_TIMERS;
}

void PE_SubStateChangeAndTimeoutValidateCB(UINT8 u8PortNum, UINT8 u8PESubState)
{
    gasPolicy_Engine[u8PortNum].u32TimeoutMsgHeader = PRL_IsAnyMsgPendinginPRL (u8PortNum);

    /* Check for Msg Header is NULL */
    if(SET_TO_ZERO == gasPolicy_Engine[u8PortNum].u32TimeoutMsgHeader)
    {
        /* Copy Receive Msg Handler Header to Timeout Header */
        gasPolicy_Engine[u8PortNum].u32TimeoutMsgHeader = gasPolicy_Engine[u8PortNum].u32MsgHeader;
    }

    /* Set Timeout state to passed timeout sub state */
    gasPolicy_Engine[u8PortNum].ePETimeoutSubState = (ePolicySubState) u8PESubState;

    /* Set PE sub state to passed timeout sub state */
    gasPolicy_Engine[u8PortNum].ePESubState = (ePolicySubState) u8PESubState;

    /*Setting the u8PETimerID to MAX_CONCURRENT_TIMERS to indicate that
    Timeout has occured*/
    gasPolicy_Engine[u8PortNum].u8PETimerID = MAX_CONCURRENT_TIMERS;

}

void PE_NoResponseTimerCB (UINT8 u8PortNum, UINT8 u8PE_State)
{
    /* Setting No Response timeout flag */
    gasPolicy_Engine[u8PortNum].u8PEPortSts |= PE_NO_RESPONSE_TIMEDOUT;
    
    /*Setting the u8PENoResponseTimerID to MAX_CONCURRENT_TIMERS to indicate that
    Timeout has occured*/
    gasPolicy_Engine[u8PortNum].u8PENoResponseTimerID = MAX_CONCURRENT_TIMERS;
}

void PE_FindVDMStateActiveFlag (UINT8 u8PortNum)
{
    switch (gasPolicy_Engine[u8PortNum].ePEState)
    {
        case ePE_VDM_GET_IDENTITY:
        {
            gasDPM[u8PortNum].u8DPM_Status |= DPM_VDM_STATE_ACTIVE_MASK;
            break;
        }
        default:
        {
           gasDPM[u8PortNum].u8DPM_Status &= ~DPM_VDM_STATE_ACTIVE_MASK;
           break;
        }
    }
}

void PE_KillPolicyEngineTimer (UINT8 u8PortNum)
{
    PDTimer_Kill (gasPolicy_Engine[u8PortNum].u8PETimerID);

    /*Setting the u8PETimerID to MAX_CONCURRENT_TIMERS to indicate that
    TimerID does not hold any valid timer IDs anymore*/
    gasPolicy_Engine[u8PortNum].u8PETimerID = MAX_CONCURRENT_TIMERS;

}
