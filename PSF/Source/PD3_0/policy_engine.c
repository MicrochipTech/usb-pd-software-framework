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

void PE_InitPort (UINT8 u8PortNum)
{

    /*Setting the HardResetCounter to 0 */
    gasPolicyEngine[u8PortNum].u8HardResetCounter = SET_TO_ZERO;

    /*Setting Contract to Implicit contract and PD Connected Status to not connected*/
    gasPolicyEngine[u8PortNum].u8PEPortSts = SET_TO_ZERO;
    
    /*Setting the Hard Reset Received Status to False*/
    gasPolicyEngine[u8PortNum].u8HardResetRcvdISR = FALSE;

    /*Setting Timer ID to Max value */
    gasPolicyEngine[u8PortNum].u8PETimerID = MAX_CONCURRENT_TIMERS;

    /*Set Timeout sub-state to invalid state*/
    gasPolicyEngine[u8PortNum].ePETimeoutSubState = ePE_INVALIDSUBSTATE;

    /* Setting Receive Handler message header and Timeout Message header to zero */
    gasPolicyEngine[u8PortNum].u32MsgHeader = SET_TO_ZERO;
    gasPolicyEngine[u8PortNum].u32TimeoutMsgHeader = SET_TO_ZERO;

    gasPolicyEngine[u8PortNum].u8PERuntimeConfig = SET_TO_ZERO; 
    
    if (PD_ROLE_SINK != DPM_GET_DEFAULT_POWER_ROLE(u8PortNum))
    {
        /*Setting the CapsCounter to 0 */
        gasPolicyEngine[u8PortNum].u8CapsCounter = SET_TO_ZERO;

        /*Setting Initial Source Policy Engine State as Startup State*/
        gasPolicyEngine[u8PortNum].ePEState = ePE_SRC_STARTUP;
        gasPolicyEngine[u8PortNum].ePESubState = ePE_SRC_STARTUP_ENTRY_SS;

        /*Setting No Response TimerID to Max Value */
        gasPolicyEngine[u8PortNum].u8PENoResponseTimerID = MAX_CONCURRENT_TIMERS;
    }
    else
    {
        /*Setting Initial Sink Policy Engine State as Startup State*/
        gasPolicyEngine[u8PortNum].ePEState = ePE_SNK_STARTUP;
        gasPolicyEngine[u8PortNum].ePESubState = ePE_INVALIDSUBSTATE;
    }
}
/***********************************************************************************/
/***********************************************************************************/
/*********************PE Run state machine ******************************************/
/************************************************************************************/
void PE_RunStateMachine (UINT8 u8PortNum)
{

    /* Receive Data Buffer */
    UINT8 u8aDataBuf[PRL_MAX_EXTN_MSG_LEN_IN_BYTES] = {SET_TO_ZERO};
    /*Received message type*/
    UINT8 u8SOPType = PRL_SOP_TYPE;
    /* Received Message Header */
    UINT32 u32Header;
    UINT8 u8RetVal;

#if(TRUE == INCLUDE_PD_DRP)
    if(DPM_GET_CURRENT_POWER_ROLE(u8PortNum) != PD_ROLE_DRP)
#endif
    {
        /* Protocol layer Chunk State machine must be ran by PE to receive Chunk message if any*/
    #if (TRUE == INCLUDE_PD_3_0)
        PRL_RunChunkStateMachine (u8PortNum);
    #endif
        
        MCHP_PSF_HOOK_DISABLE_GLOBAL_INTERRUPT ();
        /*Check the HardReset Flag in DPMStatus variable if any hard reset is received*/
        /*State transition for Hard reset reception is done in foreground to avoid the policy
        state and sub-state corruption*/
        
        if (gasPolicyEngine[u8PortNum].u8HardResetRcvdISR)
        {
            if (PD_ROLE_SOURCE == DPM_GET_CURRENT_POWER_ROLE(u8PortNum))
            {
                gasPolicyEngine[u8PortNum].ePEState = ePE_SRC_HARD_RESET_RECEIVED;
                gasPolicyEngine[u8PortNum].ePESubState = ePE_SRC_HARD_RESET_RECEIVED_ENTRY_SS;
            }
            else
            {
                gasPolicyEngine[u8PortNum].ePEState = ePE_SNK_TRANSITION_TO_DEFAULT;
                gasPolicyEngine[u8PortNum].ePESubState = ePE_SNK_TRANSITION_TO_DEFAULT_ENTRY_SS;
            }
            gasPolicyEngine[u8PortNum].u8HardResetRcvdISR = FALSE;
        }

        MCHP_PSF_HOOK_ENABLE_GLOBAL_INTERRUPT ();

        /*Check the current policy engine states and set the VDM state active flag accordingly*/
        PE_SetVDMActiveFlag (u8PortNum);

	    u8RetVal = PRL_ReceiveMsg (u8PortNum, &u8SOPType, &u32Header, u8aDataBuf, NULL);

        /* If any new Msg is received, pass it to the Receive Handler */
        if ((PRL_RET_MSG_RCVD & u8RetVal) || (PRL_RET_EXT_MSG_RCVD & u8RetVal))
        {
            /*Setting the Specification Revision as per section 6.2.1.1.5 from
             * PD Specification 3.0*/
            if(DPM_GET_DEFAULT_PD_SPEC_REV (u8PortNum) > PRL_GET_PD_SPEC_REV (u32Header))
            {
                DPM_UpdatePDSpecRev (u8PortNum, PRL_GET_PD_SPEC_REV (u32Header));
            }
            else
            {
                DPM_UpdatePDSpecRev (u8PortNum, DPM_GET_DEFAULT_PD_SPEC_REV (u8PortNum));
            }
            /* Spec Rev is updated by PRL*/
            PRL_UpdateSpecAndDeviceRoles (u8PortNum);
            
            /*Assign Idle state to PE if AMS is not initiated on TX and message is received
             * before that*/
            if ((TRUE == gasPRL[u8PortNum].u8TxStsDPMSyncISR) && \
                    (gasDPM[u8PortNum].u16InternalEvntInProgress))
            {
                if (PD_ROLE_SOURCE == DPM_GET_CURRENT_POWER_ROLE(u8PortNum))
                {
                    gasPolicyEngine[u8PortNum].ePEState = ePE_SRC_READY;
                    gasPolicyEngine[u8PortNum].ePESubState = ePE_SRC_READY_IDLE_SS;                            
                }
                else
                {
                    gasPolicyEngine[u8PortNum].ePEState = ePE_SNK_READY;
                    gasPolicyEngine[u8PortNum].ePESubState = ePE_SNK_READY_AMS_END_SS;
                }
                MCHP_PSF_HOOK_DISABLE_GLOBAL_INTERRUPT();
                gasPRL[u8PortNum].u8TxStsDPMSyncISR = FALSE;
                MCHP_PSF_HOOK_ENABLE_GLOBAL_INTERRUPT();
                
                /*Restore the internal event that was in progress*/
                gasDPM[u8PortNum].u16DPMInternalEvents |= gasDPM[u8PortNum].u16InternalEvntInProgress;
                gasDPM[u8PortNum].u16InternalEvntInProgress = RESET_TO_ZERO;
            }
            
            gasPolicyEngine[u8PortNum].u32MsgHeader = u32Header;
            /* Process the received PD message */
            PE_ReceiveMsgHandler (u8PortNum, u32Header);
        }
        /*Setting Timeout sub-state to invalid state*/
        gasPolicyEngine[u8PortNum].ePETimeoutSubState = ePE_INVALIDSUBSTATE;
        
        #if (TRUE == INCLUDE_PD_DR_SWAP)
            PE_RunDRSwapStateMachine (u8PortNum);
        #endif 
        if(PD_ROLE_SOURCE == DPM_GET_CURRENT_POWER_ROLE(u8PortNum))
        {
            #if (TRUE == INCLUDE_PD_SOURCE)
            PE_RunSrcStateMachine (u8PortNum, u8aDataBuf, u8SOPType,u32Header);
            #endif
        }
        else if(PD_ROLE_SINK == DPM_GET_CURRENT_POWER_ROLE (u8PortNum))
        {
            #if (TRUE == INCLUDE_PD_SINK)
            PE_RunSnkStateMachine (u8PortNum, u8aDataBuf, u8SOPType,u32Header);
            #endif
        }
        else
        {
            /* Do Nothing */
        }
#if (TRUE == INCLUDE_PD_VCONN_SWAP)
        PE_RunVCONNSwapStateMachine(u8PortNum);
#endif
#if (TRUE == INCLUDE_PD_PR_SWAP)
        PE_RunPRSwapStateMachine (u8PortNum);
#endif        
#if (TRUE == INCLUDE_PD_VDM)
        PE_RunVDMStateMachine (u8PortNum, u8aDataBuf, u32Header);
#endif 
        PE_RunCommonStateMachine (u8PortNum, u8aDataBuf, u8SOPType,u32Header);        
    }
}
/***************************************************************************************/
/*******************PE Support functions to decode and handle received messages**********************/
/****************************************************************************************/
UINT8 PE_IsMsgUnsupported (UINT8 u8PortNum, UINT16 u16Header)
{
    UINT8 u8MsgType;
    UINT8 u8RetVal = PE_SUPPORTED_MSG;

    u8MsgType = PRL_GET_MESSAGE_TYPE (u16Header);

    /*Only Status and FW_Update_Request Extended messages are supported 
      by the policy engine*/
    if (PRL_IS_EXTENDED_MSG (u16Header))
    {
        if (PE_EXT_FW_UPDATE_REQUEST == u8MsgType)
        {
#if (FALSE != INCLUDE_PDFU)
            u8RetVal = PE_SUPPORTED_EXTDMSG;
#else 
            u8RetVal = PE_UNSUPPORTED_MSG; 
#endif 
        }
        else if (PE_EXT_STATUS == u8MsgType)
        {
#if (TRUE == INCLUDE_PD_SOURCE_PPS)
            if (TRUE == DPM_IsAPDOAdvertised(u8PortNum))
            {
                u8RetVal = PE_SUPPORTED_EXTDMSG;
            }
            else
            {
                u8RetVal = PE_UNSUPPORTED_MSG; 
            }
#else 
            u8RetVal = PE_UNSUPPORTED_MSG; 
#endif 
        }
        else
        {
            u8RetVal = PE_UNSUPPORTED_MSG;   
        }
    }
    else
    {
        if(PE_OBJECT_COUNT_0 == PRL_GET_OBJECT_COUNT (u16Header))
        {
            /*Control messages in Table 6-5 of PD 3.0 Spec after "Not Supported" message are
            are not supported*/
            /*Any control message with message type between soft reset and Not supported are
            not supported as they are reserved fields*/
            /* Get_Status and Get_PPS_Status messages are supported for Source
            and if any APDOs are advertised to port partner.*/
            if ((PE_CTRL_GET_STATUS == u8MsgType) || (PE_CTRL_GET_PPS_STATUS == u8MsgType))
            {
                #if (FALSE == INCLUDE_PD_SOURCE_PPS)
                    u8RetVal = PE_UNSUPPORTED_MSG;
                #else
                    if (FALSE == DPM_IsAPDOAdvertised(u8PortNum))
                    {
                        u8RetVal = PE_UNSUPPORTED_MSG;
                    }
                #endif    
            }
            else if(PE_CTRL_DR_SWAP == u8MsgType)
            {
                /* DR Swap will be supported only when Dual Role Data(DRD)
                   capability is enabled */
                #if (TRUE != INCLUDE_PD_DR_SWAP)
                    u8RetVal = PE_UNSUPPORTED_MSG;
                #else 
                    if (FALSE == DPM_GET_PDO_DUAL_DATA(gasCfgStatusData.sPerPortData[u8PortNum].u32aAdvertisedPDO[INDEX_0]))
                    {
                        u8RetVal = PE_UNSUPPORTED_MSG;
                    }
                #endif
            }
            else if (PE_CTRL_PR_SWAP == u8MsgType)
            {
                 /* PR Swap will be supported only when Dual Role Power(DRP)
                    capability is enabled */                
                #if (FALSE == INCLUDE_PD_PR_SWAP)
                    u8RetVal = PE_UNSUPPORTED_MSG; 
                #else 
                    if (FALSE == DPM_GET_PDO_DUAL_POWER(gasCfgStatusData.sPerPortData[u8PortNum].u32aAdvertisedPDO[INDEX_0]))
                    {
                        u8RetVal = PE_UNSUPPORTED_MSG;
                    }
                #endif 
            }
            else if((u8MsgType > PE_CTRL_NOT_SUPPORTED) || ((u8MsgType > PE_CTRL_SOFT_RESET)\
                    && (u8MsgType < PE_CTRL_NOT_SUPPORTED)))
            {
                u8RetVal = PE_UNSUPPORTED_MSG;
            }
            else if ((PD_ROLE_SOURCE == DPM_GET_CURRENT_POWER_ROLE (u8PortNum)) && \
                     ((PE_CTRL_GET_SINK_CAP == u8MsgType) || (PE_CTRL_PING == u8MsgType)\
                        || (PE_CTRL_GOTO_MIN == u8MsgType)))
            {
                u8RetVal = PE_UNSUPPORTED_MSG;
            }

            else if ((PD_ROLE_SINK == DPM_GET_CURRENT_POWER_ROLE (u8PortNum)) && \
                     ((PE_CTRL_GET_SOURCE_CAP == u8MsgType)))
            {
                u8RetVal = PE_UNSUPPORTED_MSG;
            }
            else
            {
                /* Do Nothing */
            }
        }
        else
        {
            /*Message type greater than Sink_Capabilities except Vendor_Defined 
             * and Alert(Source only) are not supported
             * Refer Table 6-6 Data Message Types of PD Specification */
            /* Alert is supported only for PPS Source */
            if (PE_DATA_ALERT == u8MsgType)
            {
                #if (FALSE == INCLUDE_PD_SOURCE_PPS)
                    u8RetVal = PE_UNSUPPORTED_MSG;
                #else
                    if (FALSE == DPM_IsAPDOAdvertised(u8PortNum))
                    {
                        u8RetVal = PE_UNSUPPORTED_MSG;
                    }
                #endif 
            }
            else if ((u8MsgType > PE_DATA_SINK_CAP) && (u8MsgType != PE_DATA_VENDOR_DEFINED))
            {
                u8RetVal = PE_UNSUPPORTED_MSG;
            }
            else if ((PD_ROLE_SOURCE == DPM_GET_CURRENT_POWER_ROLE (u8PortNum)) && \
                    (PE_DATA_SOURCE_CAP == u8MsgType))
            {
                u8RetVal = PE_UNSUPPORTED_MSG;
            }
            else if ((PD_ROLE_SINK == DPM_GET_CURRENT_POWER_ROLE (u8PortNum)) && \
                    ((PE_DATA_SINK_CAP == u8MsgType) || (PE_DATA_REQUEST == u8MsgType) || \
                    (PE_DATA_ALERT == u8MsgType)))
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

UINT8 PE_ValidateMessage (UINT8 u8PortNum, UINT32 u32Header)
{
    UINT8 u8RetVal = FALSE;

    /*Check whether the message is supported by the policy engine*/
    /*Message header is passed without the extender header bytes*/
    UINT8 u8ChkMsgSupport = PE_IsMsgUnsupported (u8PortNum, PRL_GET_MSG_HEADER(u32Header));

    if (PE_UNSUPPORTED_MSG == u8ChkMsgSupport)
    {
        if ((ePE_SRC_READY == gasPolicyEngine[u8PortNum].ePEState) ||
           (ePE_SNK_READY == gasPolicyEngine[u8PortNum].ePEState) || 
           (ePE_INIT_PORT_VDM_IDENTITY_REQUEST == gasPolicyEngine[u8PortNum].ePEState))
        {
            PE_SendNotSupportedOrRejectMsg(u8PortNum);
            u8RetVal = PE_MSG_HANDLED;
        }
        else
        {
            /*AMS Type is interruptible*/
            if ((gasPolicyEngine[u8PortNum].u8PEPortSts & PE_AMS_TYPE))
            {
                u8RetVal = PE_PROCESS_MSG;
            }
            /*AMS Type is Non interruptible*/
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
        /* Do Nothing */
    }

    return u8RetVal;
}

void PE_HandleRcvdMsgAndTimeoutEvents (UINT8 u8PortNum, ePolicyState eNextState , ePolicySubState eNextSubState)
{

    if(ePE_INVALIDSUBSTATE == gasPolicyEngine[u8PortNum].ePETimeoutSubState)
    {
        gasPolicyEngine[u8PortNum].ePEState = eNextState;
        gasPolicyEngine[u8PortNum].ePESubState = eNextSubState;
    }
    else
    {
        if(gasPolicyEngine[u8PortNum].u32TimeoutMsgHeader == gasPolicyEngine[u8PortNum].u32MsgHeader)
        {
            gasPolicyEngine[u8PortNum].ePEState = eNextState;
            gasPolicyEngine[u8PortNum].ePESubState = eNextSubState;
        }

        else
        {
            gasPolicyEngine[u8PortNum].ePESubState = gasPolicyEngine[u8PortNum].ePETimeoutSubState;
        }
    }
}

void PE_HandleUnExpectedMsg (UINT8 u8PortNum)
{
    /*Send Hard reset if Current State is in power transition*/
    if ((ePE_SRC_TRANSITION_SUPPLY == gasPolicyEngine[u8PortNum].ePEState) || \
      (ePE_SNK_TRANSITION_SINK == gasPolicyEngine[u8PortNum].ePEState))
    {
        PE_SendHardReset (u8PortNum);
    }
    /*Send soft reset if  Current State is not power transitioned*/
    else
    {
        PE_SendSoftResetMsg (u8PortNum);
    }
}
/***********************************************************************************/
void PE_ReceiveMsgHandler (UINT8 u8PortNum, UINT32 u32Header)
{
    UINT8 u8MsgOperation = PE_ValidateMessage (u8PortNum, u32Header);
    /* Validate the received message*/
    if (PE_PROCESS_MSG == u8MsgOperation)
    {   
        #if (FALSE != INCLUDE_PDFU)
        if((ePE_PDFU_MODE == PE_FWUP_GET_CURRENT_STATE(u8PortNum)) && (PE_EXT_FW_UPDATE_REQUEST != PRL_GET_MESSAGE_TYPE (u32Header)))
        {
          /** if in PDFU State, any other Standard PD Messages are received,
           then those messages shall be processed as if it is in SRC_READY/SNK_READY state*/
            PE_FwUpdtKillTimer(u8PortNum);
            PE_FwUpdtResetToEnumState(u8PortNum);
            PE_FwUpdtInitialize();
        }
        #endif
        
        if (PRL_IS_EXTENDED_MSG (u32Header))
        {
            switch (PRL_GET_MESSAGE_TYPE (u32Header))
            {
#if (TRUE == INCLUDE_PD_SOURCE_PPS)
                case PE_EXT_STATUS:
                {
                    /* Once response for Get_Status is received, kill the Sender 
                       Response Timer and change the PE sub-state as ePE_SRC_GET_SINK_STATUS_RESPONSE_RECEIVED_SS */
                    if (ePE_SRC_GET_SINK_STATUS == gasPolicyEngine[u8PortNum].ePEState)
                    {                       
                        PE_KillPolicyEngineTimer (u8PortNum);
                        PE_HandleRcvdMsgAndTimeoutEvents(u8PortNum, ePE_SRC_GET_SINK_STATUS, ePE_SRC_GET_SINK_STATUS_RESPONSE_RCVD_SS);
                    }
                    else
                    {
                        /* Handle Unexpected message by sending Soft Reset */
                        PE_HandleUnExpectedMsg (u8PortNum);
                    }
                    break; 
                }
#endif 
                default:
                {
                    break; 
                }
            }
        }
        /* Process Message to be added down*/
        else if (PE_OBJECT_COUNT_0 != PRL_GET_OBJECT_COUNT (u32Header))
        {
            switch (PRL_GET_MESSAGE_TYPE (u32Header))
            {
                case PE_DATA_SOURCE_CAP:
                {
                    /*Discard the VDM AMS and process the source capability message received*/
                    if ((ePE_SNK_WAIT_FOR_CAPABILITIES_WAIT_SS == \
                         gasPolicyEngine[u8PortNum].ePESubState)|| (ePE_SNK_READY_AMS_END_SS == \
                         gasPolicyEngine[u8PortNum].ePESubState) || \
                       (gasDPM[u8PortNum].u16DPMStatus & DPM_VDM_AMS_ACTIVE_MASK))
                    {
                        if (ePE_SNK_WAIT_FOR_CAPABILITIES_WAIT_SS == \
                            gasPolicyEngine[u8PortNum].ePESubState)
                        {
                            /*Kill the PE_SINK_WAIT_CAP_TIMEOUT since the source
                            capability message has been received*/
                            PE_KillPolicyEngineTimer (u8PortNum);
                            PE_HandleRcvdMsgAndTimeoutEvents (u8PortNum,\
                            ePE_SNK_EVALUATE_CAPABILITY,(ePolicySubState)SET_TO_ZERO);
                        }
                        else
                        {
                            gasPolicyEngine[u8PortNum].ePEState = ePE_SNK_EVALUATE_CAPABILITY;
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
                    if ((ePE_SRC_SEND_CAPABILITIES == gasPolicyEngine[u8PortNum].ePEState) || \
                       (ePE_SRC_READY == gasPolicyEngine[u8PortNum].ePEState) || \
                       (gasDPM[u8PortNum].u16DPMStatus & DPM_VDM_AMS_ACTIVE_MASK))
                    {
                        /* Kill SenderResponse timer only in ePE_SRC_SEND_CAPABILITIES 
                           Kill SourcePPSCommTimer only in ePE_SRC_READY state if 
                           the current explicit contract is for a PPS APDO */
                        if ((ePE_SRC_SEND_CAPABILITIES == gasPolicyEngine[u8PortNum].ePEState) || 
                             ((DPM_PD_PPS_CONTRACT == DPM_GET_CURRENT_EXPLICIT_CONTRACT(u8PortNum)) && 
                                (ePE_SRC_READY == gasPolicyEngine[u8PortNum].ePEState)))   
                        {
                            PE_KillPolicyEngineTimer (u8PortNum);
                        }
                        
                        PE_HandleRcvdMsgAndTimeoutEvents (u8PortNum,ePE_SRC_NEGOTIATE_CAPABILITY,(ePolicySubState)SET_TO_ZERO);
                        DEBUG_PRINT_PORT_STR (u8PortNum,"PE_DATA_REQUEST: Request message received\r\n");
                    }
                    else
                    {
                        PE_HandleUnExpectedMsg (u8PortNum);
                    }

                    break;
                }
                case PE_DATA_BIST:
                {
                    if(((ePE_SNK_READY == gasPolicyEngine[u8PortNum].ePEState) || \
                            (ePE_SRC_READY == gasPolicyEngine[u8PortNum].ePEState)) &&	\
                            (TYPEC_VBUS_5V == DPM_GetVBUSVoltage(u8PortNum)))
                    {
                        gasPolicyEngine[u8PortNum].ePEState = ePE_BIST_MODE;
                        gasPolicyEngine[u8PortNum].ePESubState = ePE_BIST_MODE_ENTRY_SS;
                        
#if (TRUE == INCLUDE_PD_SOURCE_PPS)
                        /* Kill SourcePPSCommTimer only in ePE_SRC_READY state if 
                           the current explicit contract is for a PPS APDO*/
                        if((DPM_PD_PPS_CONTRACT == DPM_GET_CURRENT_EXPLICIT_CONTRACT(u8PortNum)) && 
                                (ePE_SRC_READY == gasPolicyEngine[u8PortNum].ePEState))
                        {
                            PE_KillPolicyEngineTimer (u8PortNum);
                        }
#endif 
                    }
                    break;
                }
                case PE_DATA_VENDOR_DEFINED:
                {
                    /*Handle the VDM messages if already in ready state*/
                    /*Also discard the current VDM AMS to handle the new VDM message*/
                    if ((ePE_SNK_READY == gasPolicyEngine[u8PortNum].ePEState) || \
						(ePE_SRC_READY == gasPolicyEngine[u8PortNum].ePEState) || \
                         (gasDPM[u8PortNum].u16DPMStatus & DPM_VDM_AMS_ACTIVE_MASK))
                    {
                        gasPolicyEngine[u8PortNum].ePEState = ePE_VDM_GET_IDENTITY;                       
                        
#if (TRUE == INCLUDE_PD_SOURCE_PPS)                        
                        /* Kill SourcePPSCommTimer only in ePE_SRC_READY state if 
                           the current explicit contract is for a PPS APDO*/
                        if((DPM_PD_PPS_CONTRACT == DPM_GET_CURRENT_EXPLICIT_CONTRACT(u8PortNum)) && 
                                (ePE_SRC_READY == gasPolicyEngine[u8PortNum].ePEState))
                        {
                            PE_KillPolicyEngineTimer (u8PortNum);
                        }  
#endif 
                    }

                    else if (ePE_SRC_VDM_IDENTITY_REQUEST == gasPolicyEngine[u8PortNum].ePEState)
                    {
                        PE_KillPolicyEngineTimer (u8PortNum);
                        PE_HandleRcvdMsgAndTimeoutEvents (u8PortNum, ePE_SRC_VDM_IDENTITY_ACKED,(ePolicySubState)SET_TO_ZERO);
                    }
#if (TRUE == INCLUDE_PD_VDM)
                    else if ((ePE_INIT_PORT_VDM_IDENTITY_REQUEST_IDLE_SS == gasPolicyEngine[u8PortNum].ePESubState) || 
                             (ePE_INIT_PORT_VDM_IDENTITY_REQUEST_MSG_DONE_SS == gasPolicyEngine[u8PortNum].ePESubState))
                    {                        
                        /* Kill VDM Response timer */
                        PE_KillPolicyEngineTimer (u8PortNum);
                        PE_HandleRcvdMsgAndTimeoutEvents (u8PortNum, ePE_INIT_PORT_VDM_IDENTITY_REQUEST,\
                                                ePE_INIT_PORT_VDM_IDENTITY_REQUEST_RESP_RCVD_SS);
                    }
#endif
                    else 
                    {
                        /* Do Nothing */
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
                                    gasPolicyEngine[u8PortNum].ePEState = ePE_PDFU_MODE;
                                    gasPolicyEngine[u8PortNum].ePESubState = ePE_FWUP_ENUMERATION_SS;
                                    PE_FwUpdtSetMsgRcvStatus();
                                }
                                else if(PE_FwUpdtGetTimeoutStatus())
                                {
                                  /**Both Message event and Timeout event Simultaneously present*/
                                    if(gasPolicyEngine[u8PortNum].u32TimeoutMsgHeader == gasPolicyEngine[u8PortNum].u32MsgHeader)
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
                                  /** Post Message received notification */
                                    PE_FwUpdtSetMsgRcvStatus();
                                }
                            }
                            else
                            {
                                gasPolicyEngine[u8PortNum].ePEState = ePE_PDFU_MODE;
                                gasPolicyEngine[u8PortNum].ePESubState = ePE_FWUP_ENUMERATION_SS;
                                gsPdfuInfo.u8EventType |= PE_FWUP_INTERRUPTION_EVT;
                                /** PDFU is active on other port so do not allow this request to PDFU State Machine*/
                            }
                        }
                        else
                        {
                            gasPolicyEngine[u8PortNum].ePEState = ePE_PDFU_MODE;
                            gasPolicyEngine[u8PortNum].ePESubState = ePE_FWUP_ENUMERATION_SS;              
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
                case PE_DATA_SINK_CAP:
                {
                    /* Once response for Get_Sink_Cap is received, kill the Sender 
                       Response Timer and change the PE sub-state as  ePE_SRC_GET_SINK_CAP_RESPONSE_RECEIVED */
                    if (ePE_SRC_GET_SINK_CAP == gasPolicyEngine[u8PortNum].ePEState)
                    {                       
                        PE_KillPolicyEngineTimer (u8PortNum);
                        PE_HandleRcvdMsgAndTimeoutEvents(u8PortNum, ePE_SRC_GET_SINK_CAP, ePE_SRC_GET_SINK_CAP_RESPONSE_RCVD_SS);
                    }
                    else
                    {
                        /* Handle Unexpected message when Sink capabilities message is 
                           received in wrong PE State */
                        PE_HandleUnExpectedMsg (u8PortNum);
                    }
                    break; 
                }
#if (TRUE == INCLUDE_PD_SOURCE_PPS)
                case PE_DATA_ALERT: 
                {
                    if ((ePE_SRC_READY == gasPolicyEngine[u8PortNum].ePEState) || \
                            (gasDPM[u8PortNum].u16DPMStatus & DPM_VDM_AMS_ACTIVE_MASK))
                    {
                        /* Kill SourcePPSCommTimer if the current explicit contract 
                           is for a PPS APDO*/
                        if((DPM_PD_PPS_CONTRACT == DPM_GET_CURRENT_EXPLICIT_CONTRACT(u8PortNum)))                                
                        {
                            PE_KillPolicyEngineTimer (u8PortNum);
                        }                        
                        /*The PE_SRC_Sink_Alert_Received state Shall be 
                         entered from the PE_SRC_Ready state when an Alert 
                         Message is received.*/
                        gasPolicyEngine[u8PortNum].ePEState = ePE_SRC_SINK_ALERT_RECEIVED;
                    }
                    else
                    {
                        /* Send Soft Reset when Alert is received in incorrect state. */
                        PE_HandleUnExpectedMsg (u8PortNum);
                    }
                    break; 
                }
#endif
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
                #if (TRUE == INCLUDE_PD_SINK)
                case PE_CTRL_GOTO_MIN:
                {
                    /*GotoMin message is received for Sink Data request*/
                    if (ePE_SNK_SELECT_CAPABILITY_WAIT_FOR_ACCEPT_SS == \
                        gasPolicyEngine[u8PortNum].ePESubState)
                    {
                        /*kill the timer PE_SENDER_RESPONSE_TIMEOUTID*/
                        DEBUG_PRINT_PORT_STR (u8PortNum,"PE_SNK_SELECT_CAPABILITY: Gotomin Msg Received\r\n");
                        PE_KillPolicyEngineTimer (u8PortNum);

                        PE_HandleRcvdMsgAndTimeoutEvents (u8PortNum,ePE_SNK_TRANSITION_SINK,\
                                                                   ePE_SNK_TRANSITION_SINK_ENTRY_SS);
                    }
                    else if (ePE_SNK_READY_AMS_END_SS == gasPolicyEngine[u8PortNum].ePESubState)
                    {
                        PE_HandleRcvdMsgAndTimeoutEvents (u8PortNum,ePE_SNK_TRANSITION_SINK,\
                                                                   ePE_SNK_TRANSITION_SINK_ENTRY_SS);
                    }
                    else
                    {
                        PE_HandleUnExpectedMsg (u8PortNum);
                    }
                    
                    break; 
                }
                #endif
                  
                case PE_CTRL_ACCEPT:
                {
                    /*Accept message received for Sink Data request*/
                    if (ePE_SNK_SELECT_CAPABILITY_WAIT_FOR_ACCEPT_SS == \
                        gasPolicyEngine[u8PortNum].ePESubState)
                    {
                        /*kill the timer PE_SENDER_RESPONSE_TIMEOUTID*/
                        DEBUG_PRINT_PORT_STR (u8PortNum,"PE_SNK_SELECT_CAPABILITY: Accept Message Received\r\n");
                        gasCfgStatusData.sPerPortData[u8PortNum].u32PortConnectStatus |= \
                                    DPM_PORT_AS_SNK_LAST_REQ_ACCEPT_STATUS;
                        PE_KillPolicyEngineTimer (u8PortNum);
                        PE_HandleRcvdMsgAndTimeoutEvents (u8PortNum,ePE_SNK_TRANSITION_SINK,\
                                                                   ePE_SNK_TRANSITION_SINK_ENTRY_SS);
                    }
                    /*Accept message received for soft reset sent by Sink*/
                    else if ((ePE_SEND_SOFT_RESET_IDLE_SS == gasPolicyEngine[u8PortNum].ePESubState) || 
                            (ePE_SEND_SOFT_RESET_MSG_DONE_SS == gasPolicyEngine[u8PortNum].ePESubState))
                    {
                        DEBUG_PRINT_PORT_STR (u8PortNum,"PE_SEND_SOFT_RESET: Accept Message received\r\n");
                        /*kill the timer PE_SENDER_RESPONSE_TIMEOUTID*/
                        PE_KillPolicyEngineTimer (u8PortNum);

                        if (PD_ROLE_SOURCE == DPM_GET_CURRENT_POWER_ROLE(u8PortNum))
                        {
                            PE_HandleRcvdMsgAndTimeoutEvents (u8PortNum,ePE_SRC_SEND_CAPABILITIES,\
                                                                   ePE_SRC_SEND_CAP_ENTRY_SS);                             
                        }
                        else
                        {
                            PE_HandleRcvdMsgAndTimeoutEvents (u8PortNum,ePE_SNK_WAIT_FOR_CAPABILITIES,\
                                                                   ePE_SNK_WAIT_FOR_CAPABILITIES_ENTRY_SS);                             
                        }
                    }
#if (TRUE == INCLUDE_PD_VCONN_SWAP)
                    /* Accept message received for VCONN Swap request */
                    else if ((ePE_VCS_SEND_SWAP_IDLE_SS == gasPolicyEngine[u8PortNum].ePESubState) || 
                            (ePE_VCS_SEND_SWAP_MSG_DONE_SS == gasPolicyEngine[u8PortNum].ePESubState)) 
                    {
                        DEBUG_PRINT_PORT_STR (u8PortNum,"Accept Received for VCONN_Swap Sent\r\n");
                        /* Kill the Sender Response Timer */
                        PE_KillPolicyEngineTimer (u8PortNum);
                        if (DPM_IsPortVCONNSource(u8PortNum))
                        {
                            PE_HandleRcvdMsgAndTimeoutEvents (u8PortNum, ePE_VCS_WAIT_FOR_VCONN, 
                                ePE_VCS_WAIT_FOR_VCONN_START_TIMER_SS);
                        }
                        else
                        {                      
                            PE_HandleRcvdMsgAndTimeoutEvents (u8PortNum, ePE_VCS_TURN_ON_VCONN, 
                                    (ePolicySubState)SET_TO_ZERO);
                        }
                    }
#endif 
#if (TRUE == INCLUDE_PD_PR_SWAP)
                    /* Accept message received for PR_Swap request */
                    else if ((ePE_PRS_SEND_SWAP_IDLE_SS == gasPolicyEngine[u8PortNum].ePESubState) || 
                            (ePE_PRS_SEND_SWAP_MSG_DONE_SS == gasPolicyEngine[u8PortNum].ePESubState)) 
                    {
                        DEBUG_PRINT_PORT_STR (u8PortNum,"Accept Received for PR_Swap Sent\r\n");
                        /* Kill the Sender Response Timer */
                        PE_KillPolicyEngineTimer (u8PortNum);
                        if (PD_ROLE_SOURCE == DPM_GET_CURRENT_POWER_ROLE(u8PortNum))
                        {
                            PE_HandleRcvdMsgAndTimeoutEvents (u8PortNum, ePE_PRS_SRC_SNK_TRANSITION_TO_OFF, 
                                    ePE_PRS_SRC_SNK_TRANSITION_TO_OFF_ENTRY_SS);
                        }
                        else
                        {
                            PE_HandleRcvdMsgAndTimeoutEvents (u8PortNum, ePE_PRS_SNK_SRC_TRANSITION_TO_OFF, 
                                    ePE_PRS_SNK_SRC_TRANSITION_TO_OFF_ENTRY_SS);
                        }                        
                    }
#endif 
#if (TRUE == INCLUDE_PD_DR_SWAP)
                    else if((ePE_DRS_SEND_SWAP_IDLE_SS == gasPolicyEngine[u8PortNum].ePESubState) || 
                            (ePE_DRS_SEND_SWAP_GOOD_CRC_RCVD_SS == gasPolicyEngine[u8PortNum].ePESubState))
                    {
						/*Accept handling for DR_SWAP*/
                        /* Kill the Sender Response Timer */
                        PE_KillPolicyEngineTimer (u8PortNum);
                        PE_HandleRcvdMsgAndTimeoutEvents (u8PortNum, ePE_DRS_DFP_UFP_ROLE_CHANGE, 
                                    ePE_INVALIDSUBSTATE);
                    }
#endif
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
                        gasPolicyEngine[u8PortNum].ePESubState)
                    {
                        if (PE_EXPLICIT_CONTRACT == (gasPolicyEngine[u8PortNum].u8PEPortSts \
                                & PE_PDCONTRACT_MASK))
                        {
                            /*Go to "ePE_SNK_READY" if Wait/Reject message is received and there is
                            an explicit contract present*/
                            /*TODO: Set the Sink request timer active flag and kill it once the src cap
                            is received before timeout*/
                            if (PE_CTRL_WAIT == PRL_GET_MESSAGE_TYPE (u32Header))
                            {
                                gasTypeCcontrol[u8PortNum].u8TypeCTimerID = PDTimer_Start (PE_SINKREQUEST_TIMEOUT_MS, PE_StateChange_TimerCB,\
                                               u8PortNum,(UINT8) ePE_SNK_SELECT_CAPABILITY);

                                gasPolicyEngine[u8PortNum].ePEState = ePE_SNK_READY;
                                gasPolicyEngine[u8PortNum].ePESubState = ePE_SNK_READY_AMS_END_SS;

                            }
                            else if (PE_CTRL_REJECT == PRL_GET_MESSAGE_TYPE(u32Header))
                            {
                                /*Set the status that last request was rejected*/
                                gasCfgStatusData.sPerPortData[u8PortNum].u32PortConnectStatus |= \
                                            DPM_PORT_AS_SNK_LAST_REQ_REJECT_STATUS;
                                gasPolicyEngine[u8PortNum].ePEState = ePE_SNK_READY;
                                gasPolicyEngine[u8PortNum].ePESubState = ePE_SNK_READY_AMS_END_SS;
                            }
                            else
                            {
                                /* Do Nothing */
                            }
                        }
                        /*Go to "ePE_SNK_WAIT_FOR_CAPABILITIES" if Wait/Reject message is
                        received and there is no explicit contract present*/
                        else
                        {
                            gasPolicyEngine[u8PortNum].ePEState = ePE_SNK_WAIT_FOR_CAPABILITIES;
                            gasPolicyEngine[u8PortNum].ePESubState = ePE_SNK_WAIT_FOR_CAPABILITIES_ENTRY_SS;
                        }
                    }
#if (TRUE == INCLUDE_PD_VCONN_SWAP)
                    /* Reject or Wait message received for VCONN Swap request */
                    else if ((ePE_VCS_SEND_SWAP_IDLE_SS == gasPolicyEngine[u8PortNum].ePESubState) || 
                            (ePE_VCS_SEND_SWAP_MSG_DONE_SS == gasPolicyEngine[u8PortNum].ePESubState)) 
                    {
                        /* Kill the Sender Response Timer */
                        PE_KillPolicyEngineTimer (u8PortNum);
                        
                        if (PE_CTRL_REJECT == (PRL_GET_MESSAGE_TYPE(u32Header)))
                        {
                            DEBUG_PRINT_PORT_STR (u8PortNum,"Reject Received for VCONN_Swap Sent\r\n");
                            /* Move to ePE_SRC_READY/ePE_SNK_READY state */
                            gasPolicyEngine[u8PortNum].ePESubState = ePE_VCS_SEND_SWAP_REJECT_RCVD_SS;                            
                        }
                        else if (PE_CTRL_WAIT == (PRL_GET_MESSAGE_TYPE (u32Header)))
                        {
                            gasPolicyEngine[u8PortNum].ePESubState = ePE_VCS_SEND_SWAP_WAIT_RCVD_SS;                            
                        }
                        else
                        {
                            /* Do Nothing */
                        }
                    }
#endif
#if (TRUE == INCLUDE_PD_PR_SWAP)
                    /* Reject or Wait message received for PR_Swap request */
                    else if ((ePE_PRS_SEND_SWAP_IDLE_SS == gasPolicyEngine[u8PortNum].ePESubState) || 
                            (ePE_PRS_SEND_SWAP_MSG_DONE_SS == gasPolicyEngine[u8PortNum].ePESubState)) 
                    {
                        DEBUG_PRINT_PORT_STR (u8PortNum,"Reject Received for PR_Swap Sent\r\n");
                        /* Kill the Sender Response Timer */
                        PE_KillPolicyEngineTimer (u8PortNum);
                        
                        if (PE_CTRL_REJECT == (PRL_GET_MESSAGE_TYPE(u32Header)))
                        {
                            gasPolicyEngine[u8PortNum].ePESubState = ePE_PRS_SEND_SWAP_REJECT_RCVD_SS;                            
                        }
                        else if (PE_CTRL_WAIT == (PRL_GET_MESSAGE_TYPE (u32Header)))
                        {
                            gasPolicyEngine[u8PortNum].ePESubState = ePE_PRS_SEND_SWAP_WAIT_RCVD_SS;                            
                        }
                        else
                        {
                            /* Do Nothing */
                        }
                    }
#endif
#if (TRUE == INCLUDE_PD_DR_SWAP)
                    else if ((ePE_DRS_SEND_SWAP_IDLE_SS == gasPolicyEngine[u8PortNum].ePESubState) || 
                            (ePE_DRS_SEND_SWAP_GOOD_CRC_RCVD_SS == gasPolicyEngine[u8PortNum].ePESubState)) 
                    {
						/*Reject and wait handling for DR_SWAP*/
                        /* Kill the Sender Response Timer */
                        PE_KillPolicyEngineTimer (u8PortNum);
                        
                        if (PE_CTRL_REJECT == (PRL_GET_MESSAGE_TYPE(u32Header)))
                        {
                            gasPolicyEngine[u8PortNum].ePESubState = ePE_DRS_SEND_SWAP_REJECT_RCVD_SS;
                                                                            
                        }
                        else if (PE_CTRL_WAIT == (PRL_GET_MESSAGE_TYPE (u32Header)))
                        {
                            gasPolicyEngine[u8PortNum].ePESubState = ePE_DRS_SEND_SWAP_WAIT_RCVD_SS;                        
                        }
                        else
                        {
                            /* Do Nothing */
                        }
                    }
#endif
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
                        gasPolicyEngine[u8PortNum].ePESubState)
                    {
                        /*Set the port connect status as PS_RDY received for last request*/
                        gasCfgStatusData.sPerPortData[u8PortNum].u32PortConnectStatus |= \
                                                DPM_PORT_AS_SNK_LAST_REQ_PS_RDY_STATUS;
                        /*Kill the timer PE_PSTRANSITION_TIMEOUTID*/
                        PE_KillPolicyEngineTimer (u8PortNum);

                        PE_HandleRcvdMsgAndTimeoutEvents (u8PortNum,ePE_SNK_READY,\
                                                                   ePE_SNK_READY_ENTRY_SS);
                    }
#if (TRUE == INCLUDE_PD_VCONN_SWAP)
                    /*PS RDY received from VCONN Source partner*/
                    else if (ePE_VCS_WAIT_FOR_VCONN_WAIT_FOR_PS_RDY_SS == \
                             gasPolicyEngine[u8PortNum].ePESubState)
                    {
                        DEBUG_PRINT_PORT_STR (u8PortNum,"VCONN_SWAP: PS_RDY received \r\n");
                        /*Kill the VCONNON timer*/
                        PE_KillPolicyEngineTimer (u8PortNum);

                        PE_HandleRcvdMsgAndTimeoutEvents (u8PortNum,ePE_VCS_TURN_OFF_VCONN,\
                                           (ePolicySubState)SET_TO_ZERO);
                    }
#endif
#if (TRUE == INCLUDE_PD_PR_SWAP)
                    /* PS_RDY received from Power Role Swap partner */
                    else if (ePE_PRS_SRC_SNK_WAIT_SOURCE_ON_WAIT_FOR_PSRDY_SS == \
                                    gasPolicyEngine[u8PortNum].ePESubState)
                    {
                        DEBUG_PRINT_PORT_STR (u8PortNum,"PR_SWAP: PS_RDY received from Initial Sink\r\n");
                         /*Kill the PSSourceOn timer*/
                        PE_KillPolicyEngineTimer (u8PortNum);
                        
                        PE_HandleRcvdMsgAndTimeoutEvents (u8PortNum,ePE_PRS_SRC_SNK_WAIT_SOURCE_ON,\
                                           ePE_PRS_SRC_SNK_WAIT_SOURCE_ON_PSRDY_RCVD_SS);                                                
                    }
                    else if (ePE_PRS_SNK_SRC_TRANSITION_TO_OFF_WAIT_FOR_PSRDY_SS == \
                                    gasPolicyEngine[u8PortNum].ePESubState)
                    {
                        DEBUG_PRINT_PORT_STR (u8PortNum,"PR_SWAP: PS_RDY received from Initial Source\r\n");
                        /*Kill the PSSourceOff timer*/
                        PE_KillPolicyEngineTimer (u8PortNum);                        

                        PE_HandleRcvdMsgAndTimeoutEvents (u8PortNum,ePE_PRS_SNK_SRC_ASSERT_RP,\
                                           (ePolicySubState)SET_TO_ZERO);                                                
                    }
#endif 
                    else
                    {
                        PE_HandleUnExpectedMsg (u8PortNum);
                    }
                    break;
                }
                case PE_CTRL_GET_SOURCE_CAP:
                {
                    if ((ePE_SRC_READY == gasPolicyEngine[u8PortNum].ePEState) || \
                        (gasDPM[u8PortNum].u16DPMStatus & DPM_VDM_AMS_ACTIVE_MASK))
                    {
                        DEBUG_PRINT_PORT_STR (u8PortNum,"PE_CTRL_GET_SOURCE_CAP: Get Source Cap Received\r\n");
                        gasPolicyEngine[u8PortNum].ePEState = ePE_SRC_SEND_CAPABILITIES;
                        gasPolicyEngine[u8PortNum].ePESubState = ePE_SRC_SEND_CAP_ENTRY_SS;

#if (TRUE == INCLUDE_PD_SOURCE_PPS)                        
                        /* Kill SourcePPSCommTimer only in ePE_SRC_READY state if 
                           the current explicit contract is for a PPS APDO*/
                        if((DPM_PD_PPS_CONTRACT == DPM_GET_CURRENT_EXPLICIT_CONTRACT(u8PortNum)))                                
                        {
                            PE_KillPolicyEngineTimer (u8PortNum);
                        }
#endif 
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
                    if ((ePE_SNK_READY == gasPolicyEngine[u8PortNum].ePEState) || \
                      (gasDPM[u8PortNum].u16DPMStatus & DPM_VDM_AMS_ACTIVE_MASK))
                    {
                        /*Go to "ePE_SNK_GIVE_SINK_CAP" state if GET_SINK_CAP message is received*/
                        gasPolicyEngine[u8PortNum].ePEState = ePE_SNK_GIVE_SINK_CAP;
                        gasPolicyEngine[u8PortNum].ePESubState = ePE_SNK_GIVE_SINK_CAP_ENTRY_SS;
                    }
                    else
                    {
                        PE_HandleUnExpectedMsg (u8PortNum);
                    }
                    break;
                }

                case PE_CTRL_VCONN_SWAP:
                {
                    /*Accept the VCONN swap if INCLUDE_PD_VCONN_SWAP is true and
                     if current state is READY State or any VDM AMS is active*/
                    if ((ePE_SNK_READY == gasPolicyEngine[u8PortNum].ePEState) || \
                        (ePE_SRC_READY == gasPolicyEngine[u8PortNum].ePEState) || \
                         (gasDPM[u8PortNum].u16DPMStatus & DPM_VDM_AMS_ACTIVE_MASK))
                    {

#if (TRUE == INCLUDE_PD_VCONN_SWAP)
                                          
                        gasPolicyEngine[u8PortNum].ePEState = ePE_VCS_EVALUATE_SWAP;
                        /* Set the timer Id to Max Concurrent Value*/
                        gasDPM[u8PortNum].u8VCONNSwapWaitTmrID = MAX_CONCURRENT_TIMERS;
#else
                        gasPolicyEngine[u8PortNum].ePEState = ePE_SEND_REJECT;
                        gasPolicyEngine[u8PortNum].ePESubState = ePE_SEND_REJECT_ENTRY_SS;
#endif

#if (TRUE == INCLUDE_PD_SOURCE_PPS)                        
                        /* Kill SourcePPSCommTimer only in ePE_SRC_READY state if 
                           the current explicit contract is for a PPS APDO*/
                        if((DPM_PD_PPS_CONTRACT == DPM_GET_CURRENT_EXPLICIT_CONTRACT(u8PortNum)) && 
                                (ePE_SRC_READY == gasPolicyEngine[u8PortNum].ePEState))
                        {
                            PE_KillPolicyEngineTimer (u8PortNum);
                        } 
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
                    gasPolicyEngine[u8PortNum].ePEState = ePE_SOFT_RESET;
                    gasPolicyEngine[u8PortNum].ePESubState = ePE_SOFT_RESET_ENTRY_SS;                                        
                    break;
                }
#if (TRUE == INCLUDE_PD_SOURCE_PPS)
                /* Get Status message received */
                case PE_CTRL_GET_STATUS:
                {
                    if ((ePE_SRC_READY == gasPolicyEngine[u8PortNum].ePEState) || \
                            (gasDPM[u8PortNum].u16DPMStatus & DPM_VDM_AMS_ACTIVE_MASK))
                    {
                        DEBUG_PRINT_PORT_STR (u8PortNum,"PE_CTRL_GET_STATUS: Get Status Received\r\n");
                        
                        gasPolicyEngine[u8PortNum].ePEState = ePE_SRC_GIVE_SOURCE_STATUS;
                        gasPolicyEngine[u8PortNum].ePESubState = ePE_SRC_GIVE_SOURCE_STATUS_ENTRY_SS;
                      
                        /* Kill SourcePPSCommTimer only in ePE_SRC_READY state if 
                           the current explicit contract is for a PPS APDO*/
                        if((DPM_PD_PPS_CONTRACT == DPM_GET_CURRENT_EXPLICIT_CONTRACT(u8PortNum)))                                
                        {
                            PE_KillPolicyEngineTimer (u8PortNum);
                        }
                        
                        /*Kill the DPM_STATUS_FAULT_PERSIST_TIMEOUT_MS timer*/
                        PDTimer_Kill(gasDPM[u8PortNum].u8StsClearTmrID);
                        /* Set the timer Id to Max Concurrent Value*/
                        gasDPM[u8PortNum].u8StsClearTmrID = MAX_CONCURRENT_TIMERS;
                    }
                    else
                    {
                        PE_HandleUnExpectedMsg (u8PortNum);
                    }
                    break; 
                }
                /* Get PPS Status message received */
                case PE_CTRL_GET_PPS_STATUS:
                {
                    if ((ePE_SRC_READY == gasPolicyEngine[u8PortNum].ePEState) || \
                            (gasDPM[u8PortNum].u16DPMStatus & DPM_VDM_AMS_ACTIVE_MASK))
                    {
                        DEBUG_PRINT_PORT_STR (u8PortNum,"PE_CTRL_GET_PPS_STATUS: Get PPS Status Received\r\n");
                        
                        gasPolicyEngine[u8PortNum].ePEState = ePE_SRC_GIVE_PPS_STATUS;
                        gasPolicyEngine[u8PortNum].ePESubState = ePE_SRC_GIVE_PPS_STATUS_ENTRY_SS;
                      
                        /* Kill SourcePPSCommTimer only in ePE_SRC_READY state if 
                           the current explicit contract is for a PPS APDO*/
                        if((DPM_PD_PPS_CONTRACT == DPM_GET_CURRENT_EXPLICIT_CONTRACT(u8PortNum)))                                
                        {
                            PE_KillPolicyEngineTimer (u8PortNum);
                        }
                    }
                    else
                    {
                        PE_HandleUnExpectedMsg (u8PortNum);
                    }  
                    break; 
                }
#endif 
#if (TRUE == INCLUDE_PD_DR_SWAP)
                case PE_CTRL_DR_SWAP:
                {
                    /*DR_SWAP message is valid only if the PE state is SRC_READY or SNK_READY*/
                    if ((ePE_SNK_READY == gasPolicyEngine[u8PortNum].ePEState) || \
                        (ePE_SRC_READY == gasPolicyEngine[u8PortNum].ePEState) || \
                        (gasDPM[u8PortNum].u16DPMStatus & DPM_VDM_AMS_ACTIVE_MASK))
                    {
                        /*Kill the u8DRSwapWaitTmrID timer*/
                        PDTimer_Kill(gasDPM[u8PortNum].u8DRSwapWaitTmrID);
                        gasDPM[u8PortNum].u8DRSwapWaitTmrID = MAX_CONCURRENT_TIMERS;
                        gasPolicyEngine[u8PortNum].ePEState = ePE_DRS_EVALUATE_SWAP;
#if (TRUE == INCLUDE_PD_SOURCE_PPS)                        
                        /* Kill SourcePPSCommTimer only in ePE_SRC_READY state if 
                           the current explicit contract is for a PPS APDO*/
                        if((DPM_PD_PPS_CONTRACT == DPM_GET_CURRENT_EXPLICIT_CONTRACT(u8PortNum)) && 
                                (ePE_SRC_READY == gasPolicyEngine[u8PortNum].ePEState))
                        {
                            PE_KillPolicyEngineTimer (u8PortNum);
                        } 
#endif                         
                    }
                    else
                    {
                        PE_HandleUnExpectedMsg (u8PortNum);
                    }
                    break;
                }
#endif
                
#if (TRUE == INCLUDE_PD_PR_SWAP)
                case PE_CTRL_PR_SWAP: 
                {
                    /*Accept the Power Role swap if current state is READY State or
                    any VDM AMS is active*/
                    if ((ePE_SNK_READY == gasPolicyEngine[u8PortNum].ePEState) || \
                        (ePE_SRC_READY == gasPolicyEngine[u8PortNum].ePEState) || \
                         (gasDPM[u8PortNum].u16DPMStatus & DPM_VDM_AMS_ACTIVE_MASK))
                    {
                        DEBUG_PRINT_PORT_STR (u8PortNum,"PR_SWAP Received from Partner \r\n");
                        /*Kill the tPRSwapWait timer*/
                        PDTimer_Kill(gasDPM[u8PortNum].u8PRSwapWaitTmrID);
                        /* Set the timer Id to Max Concurrent Value*/
                        gasDPM[u8PortNum].u8PRSwapWaitTmrID = MAX_CONCURRENT_TIMERS;

                        gasPolicyEngine[u8PortNum].ePEState = ePE_PRS_EVALUATE_SWAP;

#if (TRUE == INCLUDE_PD_SOURCE_PPS)                        
                        /* Kill SourcePPSCommTimer only in ePE_SRC_READY state if 
                           the current explicit contract is for a PPS APDO*/
                        if((DPM_PD_PPS_CONTRACT == DPM_GET_CURRENT_EXPLICIT_CONTRACT(u8PortNum)) && 
                                (ePE_SRC_READY == gasPolicyEngine[u8PortNum].ePEState))
                        {
                            PE_KillPolicyEngineTimer (u8PortNum);
                        } 
#endif 
                    }
                    else
                    {
                        PE_HandleUnExpectedMsg (u8PortNum);
                    }
                    break;                    
                }
#endif 
                case PE_CTRL_NOT_SUPPORTED:
                {
                    if ((ePE_SRC_GET_SINK_STATUS == gasPolicyEngine[u8PortNum].ePEState) || 
                    (ePE_VCS_SEND_SWAP == gasPolicyEngine[u8PortNum].ePEState) || 
                    (ePE_PRS_SEND_SWAP == gasPolicyEngine[u8PortNum].ePEState) || 
                    (ePE_DRS_SEND_SWAP == gasPolicyEngine[u8PortNum].ePEState) || 
                    (ePE_INIT_PORT_VDM_IDENTITY_REQUEST == gasPolicyEngine[u8PortNum].ePEState))                     
                    {
                        /* Kill the Sender Response Timer */
                        PE_KillPolicyEngineTimer (u8PortNum);
                        if (PD_ROLE_SOURCE == DPM_GET_CURRENT_POWER_ROLE(u8PortNum))
                        {
                            PE_HandleRcvdMsgAndTimeoutEvents (u8PortNum,ePE_SRC_READY,\
                                                                   ePE_SRC_READY_END_AMS_SS);                             
                        }
                        else
                        {
                            PE_HandleRcvdMsgAndTimeoutEvents (u8PortNum,ePE_SNK_READY,\
                                                                   ePE_SNK_READY_AMS_END_SS);                             
                        }                                            
                    }
                    else
                    {
                        if ((gasPolicyEngine[u8PortNum].ePEState != ePE_SRC_READY) && 
                            (gasPolicyEngine[u8PortNum].ePEState != ePE_SNK_READY))
                        {
                            PE_HandleUnExpectedMsg (u8PortNum);
                        }
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
/***************************************************************************************/
/***************************************************************************************/
/*********************************PE Send Messages APIs********************************/
/****************************************************************************************/
void PE_SendNotSupportedOrRejectMsg (UINT8 u8PortNum)
{
    /*Send Not Supported message if Current PD Specification is 3.0*/
    if (PD_SPEC_REVISION_3_0 == DPM_GET_CURRENT_PD_SPEC_REV (u8PortNum))
    {
        gasPolicyEngine[u8PortNum].ePEState = ePE_SEND_NOT_SUPPORTED;
        gasPolicyEngine[u8PortNum].ePESubState = ePE_SEND_NOT_SUPPORTED_ENTRY_SS;
    }
    /*Send Reject message if Current PD Specification is 2.0*/
    else
    {
        gasPolicyEngine[u8PortNum].ePEState = ePE_SEND_REJECT;
        gasPolicyEngine[u8PortNum].ePESubState = ePE_SEND_REJECT_ENTRY_SS;
    }
}

void PE_SendHardReset (UINT8 u8PortNum)
{
    if (PD_ROLE_SOURCE == DPM_GET_CURRENT_POWER_ROLE(u8PortNum))
    {
        gasPolicyEngine[u8PortNum].ePEState = ePE_SRC_HARD_RESET;
        gasPolicyEngine[u8PortNum].ePESubState = ePE_SRC_HARD_RESET_ENTRY_SS;
    }
    else
    {
        gasPolicyEngine[u8PortNum].ePEState = ePE_SNK_HARD_RESET;
        gasPolicyEngine[u8PortNum].ePESubState = ePE_SNK_HARD_RESET_SEND_SS;
    }
}

void PE_SendSoftResetMsg (UINT8 u8PortNum)
{   
    gasPolicyEngine[u8PortNum].ePEState = ePE_SEND_SOFT_RESET;
    gasPolicyEngine[u8PortNum].ePESubState = ePE_SEND_SOFT_RESET_SOP_SS;
}
/***************************************************************************************/
void PE_RunCommonStateMachine(UINT8 u8PortNum , UINT8 *pu8DataBuf , UINT8 u8SOPType ,UINT32 u32Header)
{
    UINT8 u8TransmitSOP = PRL_SOP_TYPE;
    UINT32 u32TransmitHeader = SET_TO_ZERO;
    UINT32 *u32pTransmitDataObj = SET_TO_ZERO;
    PRLTxCallback pfnTransmitCB = PE_StateChange_TransmitCB;
    UINT32 u32TransmitTmrIDTxSt = SET_TO_ZERO;
    UINT8 u8IsTransmit = FALSE;
    ePolicyState eTxDoneSt; 
    ePolicySubState eTxDoneSS, eTxHardRstSS; 
    UINT8 u8CurrPwrRole = DPM_GET_CURRENT_POWER_ROLE(u8PortNum); 
    if (PD_ROLE_SOURCE == u8CurrPwrRole)
    {
        eTxDoneSt = ePE_SRC_READY;
        eTxDoneSS = ePE_SRC_READY_END_AMS_SS;   
        eTxHardRstSS = ePE_SRC_HARD_RESET_ENTRY_SS; 
    }
    else
    {
        eTxDoneSt = ePE_SNK_READY;
        eTxDoneSS = ePE_SNK_READY_AMS_END_SS;     
        eTxHardRstSS = ePE_SNK_HARD_RESET_SEND_SS; 
    }
#if (FALSE != INCLUDE_PDFU)
    static UINT8 u8TempRespBuffer[3];
#endif
    switch (gasPolicyEngine[u8PortNum].ePEState)
    {
       case ePE_SOFT_RESET:
       {
            switch (gasPolicyEngine[u8PortNum].ePESubState)
            {
                case ePE_SOFT_RESET_ENTRY_SS:
                {
                    DEBUG_PRINT_PORT_STR (u8PortNum,"ePE_SOFT_RESET_ENTRY_SS\r\n");
                    
                    /* Kill Policy Engine Timer */
                    PE_KillPolicyEngineTimer (u8PortNum);

					/* Reset Protocol Layer */
                    PRL_ProtocolSpecificSOPReset(u8PortNum, PRL_SOP_TYPE);

                    /* Send Accept message */
                    u32TransmitHeader = PRL_FormSOPTypeMsgHeader (u8PortNum, PE_CTRL_ACCEPT,
                                            PE_OBJECT_COUNT_0, PE_NON_EXTENDED_MSG);

                    u32pTransmitDataObj = NULL;
                    
                    if (PD_ROLE_SOURCE == u8CurrPwrRole)
                    {
                        u32TransmitTmrIDTxSt = PRL_BUILD_PKD_TXST_U32( ePE_SRC_SEND_CAPABILITIES, \
                                                ePE_SRC_SEND_CAP_ENTRY_SS, \
                                                ePE_SRC_HARD_RESET, ePE_SRC_HARD_RESET_ENTRY_SS);                        
                    }
                    else
                    {     
                        u32TransmitTmrIDTxSt = PRL_BUILD_PKD_TXST_U32(ePE_SNK_WAIT_FOR_CAPABILITIES,\
                                             ePE_SNK_WAIT_FOR_CAPABILITIES_ENTRY_SS,\
                                             ePE_SNK_HARD_RESET, \
                                             ePE_SNK_HARD_RESET_SEND_SS);                        
                    }                                           

                    u8IsTransmit = TRUE;
                    
                    gasPolicyEngine[u8PortNum].ePESubState = ePE_SOFT_RESET_IDLE_SS;
                                        
                    break;
                }
                case ePE_SOFT_RESET_IDLE_SS:
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
       
       case ePE_SEND_SOFT_RESET:
       {
           switch (gasPolicyEngine[u8PortNum].ePESubState)
           {
               case ePE_SEND_SOFT_RESET_SOP_SS:
               {
                    DEBUG_PRINT_PORT_STR (u8PortNum,"ePE_SEND_SOFT_RESET_SOP_SS\r\n");
                   
                    /* Kill Policy Engine Timer */
                    PE_KillPolicyEngineTimer (u8PortNum);
                   
					/* Reset the Protocol Layer for SOP type message */
                    PRL_ProtocolSpecificSOPReset(u8PortNum, (UINT8) PRL_SOP_TYPE);
                    
					/* Send SoftReset message */
                    u32TransmitHeader = PRL_FormSOPTypeMsgHeader(u8PortNum, PE_CTRL_SOFT_RESET, \
																	PE_OBJECT_COUNT_0, PE_NON_EXTENDED_MSG);
                    u32pTransmitDataObj = NULL;
                    
                    if (PD_ROLE_SOURCE == u8CurrPwrRole)
                    {
                        u32TransmitTmrIDTxSt = PRL_BUILD_PKD_TXST_U32( ePE_SEND_SOFT_RESET, \
                                                    ePE_SEND_SOFT_RESET_MSG_DONE_SS, \
                                                    ePE_SRC_HARD_RESET, ePE_SRC_HARD_RESET_ENTRY_SS);
                    }
                    else
                    {
                        u32TransmitTmrIDTxSt = PRL_BUILD_PKD_TXST_U32( ePE_SEND_SOFT_RESET, \
                                                    ePE_SEND_SOFT_RESET_MSG_DONE_SS, \
                                                    ePE_SNK_HARD_RESET, ePE_SNK_HARD_RESET_SEND_SS);                        
                    }
                    u8IsTransmit = TRUE;
                    
                    gasPolicyEngine[u8PortNum].ePESubState = ePE_SEND_SOFT_RESET_IDLE_SS;
                                       
                    break; 
               }
               case ePE_SEND_SOFT_RESET_SOP_P_SS:
               {
                    DEBUG_PRINT_PORT_STR (u8PortNum,"ePE_SEND_SOFT_RESET_SOP_P_SS\r\n");
					
                    /* Kill Policy Engine Timer */
                    PE_KillPolicyEngineTimer (u8PortNum);
                    
					/* Reset the Protocol Layer for SOP_P type message */
                    PRL_ProtocolSpecificSOPReset(u8PortNum, PRL_SOP_P_TYPE);

                    /* Send Soft Reset Message */
                    u8TransmitSOP = PRL_SOP_P_TYPE;
                    u32TransmitHeader = PRL_FormNonSOPTypeMsgHeader(u8PortNum, PE_CTRL_SOFT_RESET, \
																	PE_OBJECT_COUNT_0, PE_NON_EXTENDED_MSG);
                    u32pTransmitDataObj = NULL;
                    
                    if (PD_ROLE_SOURCE == u8CurrPwrRole)
                    {
                        u32TransmitTmrIDTxSt = PRL_BUILD_PKD_TXST_U32( ePE_SEND_SOFT_RESET, \
                                                    ePE_SEND_SOFT_RESET_MSG_DONE_SS, \
                                                    ePE_SRC_HARD_RESET, ePE_SRC_HARD_RESET_ENTRY_SS);
                    }
                    else
                    {
                        u32TransmitTmrIDTxSt = PRL_BUILD_PKD_TXST_U32( ePE_SEND_SOFT_RESET, \
                                                    ePE_SEND_SOFT_RESET_MSG_DONE_SS, \
                                                    ePE_SNK_HARD_RESET, ePE_SNK_HARD_RESET_SEND_SS);                        
                    }
                    
                    u8IsTransmit = TRUE;
                    
                    gasPolicyEngine[u8PortNum].ePESubState = ePE_SEND_SOFT_RESET_IDLE_SS;
                    
                    break;                                       
               }
               case ePE_SEND_SOFT_RESET_MSG_DONE_SS:
               {
                    DEBUG_PRINT_PORT_STR (u8PortNum,"ePE_SEND_SOFT_RESET_MSG_DONE_SS\r\n");
                    gasPolicyEngine[u8PortNum].u8PETimerID = PDTimer_Start (
                                                            PE_SENDERRESPONSE_TIMEOUT_MS,
                                                            PE_SSChngAndTimeoutValidate_TimerCB,u8PortNum,  
                                                            (UINT8)eTxHardRstSS);
                    gasPolicyEngine[u8PortNum].ePESubState = ePE_SEND_SOFT_RESET_IDLE_SS;
                   
                    break; 
               }
               case ePE_SEND_SOFT_RESET_IDLE_SS:
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
#if (TRUE == INCLUDE_PD_3_0)
       case ePE_SEND_NOT_SUPPORTED:
       {
            switch (gasPolicyEngine[u8PortNum].ePESubState)
            {
                case ePE_SEND_NOT_SUPPORTED_ENTRY_SS:
                {
                    DEBUG_PRINT_PORT_STR (u8PortNum,"PE_SEND_NOT_SUPPORTED: Entered the state\r\n");

                    u32TransmitHeader = PRL_FormSOPTypeMsgHeader (u8PortNum, PE_CTRL_NOT_SUPPORTED, \
                                                    PE_OBJECT_COUNT_0, PE_NON_EXTENDED_MSG);
                    u32pTransmitDataObj = NULL;

                    /*Set the transmitter callback to transition to source soft reset state if
                    message transmission fails*/
                    u32TransmitTmrIDTxSt = PRL_BUILD_PKD_TXST_U32 (eTxDoneSt, \
                                                eTxDoneSS, ePE_SEND_SOFT_RESET,\
                                                ePE_SEND_SOFT_RESET_SOP_SS);

                    u8IsTransmit = TRUE;
                    gasPolicyEngine[u8PortNum].ePESubState = ePE_SEND_NOT_SUPPORTED_IDLE_SS;

                    break;
                }
                /*Wait here until the message is sent*/
                case ePE_SEND_NOT_SUPPORTED_IDLE_SS:
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
#endif
       case ePE_SEND_REJECT:
       {
            switch (gasPolicyEngine[u8PortNum].ePESubState)
            {
                case ePE_SEND_REJECT_ENTRY_SS:
                {
                    DEBUG_PRINT_PORT_STR (u8PortNum,"ePE_SEND_REJECT: Entered the state\r\n");

                    u32TransmitHeader = PRL_FormSOPTypeMsgHeader (u8PortNum, PE_CTRL_REJECT, \
                                                    PE_OBJECT_COUNT_0, PE_NON_EXTENDED_MSG);
                    u32pTransmitDataObj = NULL;

                    /*Set the transmitter callback to transition to source soft reset state if
                    message transmission fails*/
                    u32TransmitTmrIDTxSt = PRL_BUILD_PKD_TXST_U32 (eTxDoneSt, \
                                                eTxDoneSS, ePE_SEND_SOFT_RESET,\
                                                ePE_SEND_SOFT_RESET_SOP_SS);
                    u8IsTransmit = TRUE;
                    gasPolicyEngine[u8PortNum].ePESubState = ePE_SEND_REJECT_IDLE_SS;

                    break;
                }
                /*Wait here until the message is sent*/
                case ePE_SEND_REJECT_IDLE_SS:
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

        /* Responder Structured VDM Discover Identity */
        case ePE_VDM_GET_IDENTITY:
        {
            DEBUG_PRINT_PORT_STR (u8PortNum,"PE_RESP_VDM_GET_IDENTITY: Entered the state\r\n");
            
            /*Convert the u8DataBuf into 32 bit header for easy handling*/
            UINT32 *u32RecvdVDMHeader = (UINT32 *)pu8DataBuf;

            if (PE_VDM_REQ == PE_GET_VDM_CMD_TYPE (*u32RecvdVDMHeader))
            {
                /*Send NACK message for any received VDM message if PD Spec revision is 2.0*/
                if (PD_SPEC_REVISION_2_0 == DPM_GET_CURRENT_PD_SPEC_REV (u8PortNum))
                {
                    gasPolicyEngine[u8PortNum].ePEState = eTxDoneSt;
                    gasPolicyEngine[u8PortNum].ePESubState = eTxDoneSS;                    
                }
                 /*Send "Not Supported" message for any received VDM message if PD Spec revision is 3.0*/
                else
                {
                    gasPolicyEngine[u8PortNum].ePEState = ePE_SEND_NOT_SUPPORTED;
                    gasPolicyEngine[u8PortNum].ePESubState = ePE_SEND_NOT_SUPPORTED_ENTRY_SS;
                }
            }              
            break;
        }

		case ePE_BIST_MODE:
		{
            switch (gasPolicyEngine[u8PortNum].ePESubState)
			{
                case ePE_BIST_MODE_ENTRY_SS:
                {
				  /*Convert the u8DataBuf into 32 bit header for easy handling*/
					UINT32 *u32BISTObj = (UINT32 *)pu8DataBuf;

					/* DATA OBJECT is checked for BIST carrier mode */
                    if (PRL_BIST_CARRIER_MODE_OBJ == u32BISTObj[INDEX_0])
                    {
                        /* BIST_CARRIER_MODE is configured*/
                        PRL_ConfigureBISTCarrierMode (u8PortNum, TRUE);
                        /* BISTContModeTimer is started */
                        (void)PDTimer_Start (PRL_BIST_CONTMODE_TIMEOUT_MS, PE_SubStateChange_TimerCB,\
                                u8PortNum,(UINT8)ePE_BIST_MODE_EXIT_SS);
                    }
                    else
                    {
                        /* in case of BIST Test Data mode disable the message processing
                        by FW.*/
                        gasPRL[u8PortNum].u8RxDisable = TRUE;
                    }
                    gasPolicyEngine[u8PortNum].ePESubState = ePE_BIST_MODE_IDLE_SS;
                    break;
                }
                case ePE_BIST_MODE_IDLE_SS:
                {
				  	/*Idle state for BISTContModeTimer timeout*/
                    /* Hook to notify PE state machine entry into idle sub-state */
                    MCHP_PSF_HOOK_NOTIFY_IDLE(u8PortNum, eIDLE_PE_NOTIFY);
                    break;
                }
                case ePE_BIST_MODE_EXIT_SS:
                {
                    /* On BISTContModeTimer timeout BIST is reset*/
                    PRL_ConfigureBISTCarrierMode (u8PortNum, FALSE);
                    
                    /*Transition to ePE_SRC_TRANSITION_TO_DEFAULT state if current role is source*/
                    if (PD_ROLE_SOURCE == u8CurrPwrRole)
                    {
                        gasPolicyEngine[u8PortNum].ePEState = ePE_SRC_TRANSITION_TO_DEFAULT;
                        gasPolicyEngine[u8PortNum].ePESubState = ePE_SRC_TRANSITION_TO_DEFAULT_ENTRY_SS;
                    }
                    /*Transition to ePE_SNK_TRANSITION_TO_DEFAULT state if current role is sink*/
                    else
                    {
                        gasPolicyEngine[u8PortNum].ePEState = ePE_SNK_TRANSITION_TO_DEFAULT;
                        gasPolicyEngine[u8PortNum].ePESubState = ePE_SNK_TRANSITION_TO_DEFAULT_ENTRY_SS;
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
                       
            if(FALSE != (gsPdfuInfo.u8EventType & PE_FWUP_INTERRUPTION_EVT))
            {
                UINT8 u8CurrentPowerRole;
                ePolicySubState eSubState = ePE_INVALIDSUBSTATE;
                ePolicyState eState = ePE_INVALIDSTATE;
                
                u8TempRespBuffer[PE_FWUP_RESPBUFF_PROTOCOL_VERSION_INDEX] = PE_FWUP_PROTOCOL_VERSION;
                u8TempRespBuffer[PE_FWUP_RESPBUFF_RESPONSE_TYPE_INDEX] = ((~0x80) & pu8DataBuf[PE_FWUP_RESPBUFF_REQUEST_TYPE_INDEX]);
                u8TempRespBuffer[PE_FWUP_RESPBUFF_STATUS_INDEX] = (UINT8)ePE_FWUP_errUNKNOWN;
                //u8TempRespBuffer[PE_FWUP_RESPBUFF_STATUS_INDEX + 1u] = SET_TO_ZERO; /** Padding byte */
                /**Clear the Interruption event*/
                gsPdfuInfo.u8EventType &= (~PE_FWUP_INTERRUPTION_EVT);
                
                u32TransmitHeader =  /**Combined Message Header*/
                PRL_FORM_COMBINED_MSG_HEADER(((1u << PRL_EXTMSG_CHUNKED_BIT_POS) | (PRL_EXTMSG_DATA_FIELD_MASK & 0x03)), /**Extended Msg Header*/
                                             PRL_FormSOPTypeMsgHeader(u8PortNum,PE_EXT_FW_UPDATE_RESPONSE,7, /**Standard Msg Header*/
                                                     PE_EXTENDED_MSG));
                /** Update the Pointer to Data Object */
                u32pTransmitDataObj = (UINT32*)&u8TempRespBuffer[0u];
                
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
                    eSubState = ePE_SNK_READY_AMS_END_SS;     
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
                    u32TransmitHeader = u32Header;
                    /** Update the Pointer to Data Object */
                    u32pTransmitDataObj = (UINT32*)&gsPdfuInfo.pu8ResponseBuffer[0u];

                    /** Update the Transmit Call back */
                    pfnTransmitCB = PE_FwUpdtTxDoneCallBack;

                    /**Update the Transmit Completed Callback State Transitions*/
                    u32TransmitTmrIDTxSt = gsPdfuInfo.u32Transmit_TmrID_TxSt;
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
        (void)PRL_TransmitMsg (u8PortNum, u8TransmitSOP, u32TransmitHeader, (UINT8 *)u32pTransmitDataObj,\
                pfnTransmitCB, u32TransmitTmrIDTxSt);
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
            gasPolicyEngine[u8PortNum].ePEState = (ePolicyState) u8TXDoneState;
            gasPolicyEngine[u8PortNum].ePESubState = (ePolicySubState) u8TxDoneSubState;
            break;
        }
        case PRL_TX_ABORTED_ST:
        {
            break;
        }

        case PRL_TX_FAILED_ST:
        {
            gasPolicyEngine[u8PortNum].ePEState = (ePolicyState) u8TxFailedState;
            gasPolicyEngine[u8PortNum].ePESubState = (ePolicySubState) u8TxFailedSubState;
            break;
        }
        
        default:
        {
            break;
        }

    }
}
/************************************************************************************/
/************************************************************************************/
/*******************PE Timer Callback************************************************/
/************************************************************************************/
void PE_StateChange_TimerCB (UINT8 u8PortNum, UINT8 u8PEState)
{
	gasPolicyEngine[u8PortNum].ePEState = (ePolicyState) u8PEState;

    /*Setting the u8PETimerID to MAX_CONCURRENT_TIMERS to indicate that
    Timeout has occurred*/
    gasPolicyEngine[u8PortNum].u8PETimerID = MAX_CONCURRENT_TIMERS;
}

void PE_SubStateChange_TimerCB (UINT8 u8PortNum, UINT8 u8PESubState)
{
    /*Setting the u8PETimerID to MAX_CONCURRENT_TIMERS to indicate that
    Timeout has occurred*/
    gasPolicyEngine[u8PortNum].ePESubState = (ePolicySubState) u8PESubState;
    gasPolicyEngine[u8PortNum].u8PETimerID = MAX_CONCURRENT_TIMERS;
}

void PE_SSChngAndTimeoutValidate_TimerCB(UINT8 u8PortNum, UINT8 u8PESubState)
{
    gasPolicyEngine[u8PortNum].u32TimeoutMsgHeader = PRL_IsAnyMsgPendinginPRL (u8PortNum);

    /* Check for Msg Header is NULL */
    if(SET_TO_ZERO == gasPolicyEngine[u8PortNum].u32TimeoutMsgHeader)
    {
        /* Copy Receive Msg Handler Header to Timeout Header */
        gasPolicyEngine[u8PortNum].u32TimeoutMsgHeader = gasPolicyEngine[u8PortNum].u32MsgHeader;
    }

	/* Set Timeout state to passed timeout sub state */
	gasPolicyEngine[u8PortNum].ePETimeoutSubState = (ePolicySubState) u8PESubState;
    
    /* Set PE sub state to passed timeout sub state */
    gasPolicyEngine[u8PortNum].ePESubState = (ePolicySubState) u8PESubState;
    
    if (ePE_SRC_HARD_RESET_ENTRY_SS == (ePolicySubState)u8PESubState)
    {
    	gasPolicyEngine[u8PortNum].ePEState = ePE_SRC_HARD_RESET;
        
    }
    else if (ePE_SNK_HARD_RESET_SEND_SS == (ePolicySubState)u8PESubState)
    {
         gasPolicyEngine[u8PortNum].ePEState = ePE_SNK_HARD_RESET;
    }
    else
    {
        /* Do Nothing */
    }
   
    /*Setting the u8PETimerID to MAX_CONCURRENT_TIMERS to indicate that
    Timeout has occurred*/
    gasPolicyEngine[u8PortNum].u8PETimerID = MAX_CONCURRENT_TIMERS;

}
void PE_NoResponse_TimerCB (UINT8 u8PortNum, UINT8 u8PE_State)
{
    /* Setting No Response timeout flag */
    gasPolicyEngine[u8PortNum].u8PEPortSts |= PE_NO_RESPONSE_TIMEDOUT;
    
    /*Setting the u8PENoResponseTimerID to MAX_CONCURRENT_TIMERS to indicate that
    Timeout has occurred*/
    gasPolicyEngine[u8PortNum].u8PENoResponseTimerID = MAX_CONCURRENT_TIMERS;
}

void PE_KillPolicyEngineTimer (UINT8 u8PortNum)
{
    PDTimer_Kill (gasPolicyEngine[u8PortNum].u8PETimerID);

    /*Setting the u8PETimerID to MAX_CONCURRENT_TIMERS to indicate that
    TimerID does not hold any valid timer IDs anymore*/
    gasPolicyEngine[u8PortNum].u8PETimerID = MAX_CONCURRENT_TIMERS;

}
/********************************************************************************/
/********************************************************************************/
/*****************VDM Support function*******************************************/
/********************************************************************************/
void PE_SetVDMActiveFlag (UINT8 u8PortNum)
{
    switch (gasPolicyEngine[u8PortNum].ePEState)
    {
        case ePE_VDM_GET_IDENTITY:
        {
            gasDPM[u8PortNum].u16DPMStatus |= DPM_VDM_AMS_ACTIVE_MASK;
            break;
        }
        default:
        {
           gasDPM[u8PortNum].u16DPMStatus &= ~DPM_VDM_AMS_ACTIVE_MASK;
           break;
        }
    }
}
/*******************************************************************************/
UINT8 PE_IsPolicyEngineIdle(UINT8 u8PortNum)
{
    UINT8 u8Return = FALSE;  
    
    if ((PD_ROLE_SOURCE == DPM_GET_CURRENT_POWER_ROLE(u8PortNum)) &&
            (ePE_SRC_READY == gasPolicyEngine[u8PortNum].ePEState) &&
          (ePE_SRC_READY_IDLE_SS == gasPolicyEngine[u8PortNum].ePESubState))  
    {
        u8Return = TRUE;
    }
    else if ((ePE_SNK_READY == gasPolicyEngine[u8PortNum].ePEState) &&
          (ePE_SNK_READY_IDLE_SS == gasPolicyEngine[u8PortNum].ePESubState))  
    {
        u8Return = TRUE;
    }
    else
    {
        /* Do Nothing */
    }
    
    return u8Return;
}
/*******************************************************************************/


