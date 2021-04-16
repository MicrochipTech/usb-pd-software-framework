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
    PE_ResetParams (u8PortNum);
    
    if (DPM_GET_DEFAULT_POWER_ROLE(u8PortNum) != PD_ROLE_SINK)
    {
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

void PE_ResetParams (UINT8 u8PortNum)
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
    
    /*Setting the CapsCounter to 0 */
    gasPolicyEngine[u8PortNum].u8CapsCounter = SET_TO_ZERO;
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
    
    /* Return value */
    UINT8 u8RetVal;
    
    /* PD Specification Revision */
    UINT8 u8PDSpecRev; 
    
    /* Received Message Header */
    UINT32 u32Header;    

#if(TRUE == INCLUDE_PD_DRP)
    if (DPM_GET_CURRENT_POWER_ROLE(u8PortNum) != PD_ROLE_DRP)
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

	    u8RetVal = PRL_ReceiveMsg (u8PortNum, &u8SOPType, &u32Header, u8aDataBuf, NULL);

        /* If any new Msg is received, pass it to the Receive Handler */
        if (u8RetVal & (PRL_RET_MSG_RCVD | PRL_RET_EXT_MSG_RCVD))
        {
            /*Setting the Specification Revision as per section 6.2.1.1.5 from
             * PD Specification 3.0*/
            if (DPM_GET_DEFAULT_PD_SPEC_REV (u8PortNum) > PRL_GET_PD_SPEC_REV (u32Header))
            {
                u8PDSpecRev = PRL_GET_PD_SPEC_REV (u32Header);                 
            }
            else
            {
                u8PDSpecRev = DPM_GET_DEFAULT_PD_SPEC_REV (u8PortNum);                                
            }
            
            DPM_UpdatePDSpecRev (u8PortNum, u8PDSpecRev, u8SOPType);
            
            /* Spec Rev is updated by PRL*/
            PRL_UpdateSpecAndDeviceRoles (u8PortNum);
            
            /*Assign Idle state to PE if AMS is not initiated on TX and message is received
             * before that*/
            if (gasPRL[u8PortNum].u8TxStsDPMSyncISR && gasDPM[u8PortNum].u16InternalEvntInProgress)            
            {
                DEBUG_PRINT_PORT_STR (u8PortNum,"PE: INTERNAL EVT Discarded due to Rcvd Msg\r\n");
                
                if (PD_ROLE_SOURCE == DPM_GET_CURRENT_POWER_ROLE(u8PortNum))
                {
                    gasPolicyEngine[u8PortNum].ePEState = ePE_SRC_READY;
                    gasPolicyEngine[u8PortNum].ePESubState = ePE_SRC_READY_IDLE_SS; 
                    /* Revert the Rp value to SinkTxOK */
                    #if (TRUE == INCLUDE_PD_3_0)
                        PRL_SetCollisionAvoidance (u8PortNum, TYPEC_SINK_TXOK); 
                    #endif 
                }
                else
                {
                    gasPolicyEngine[u8PortNum].ePEState = ePE_SNK_READY;
                    gasPolicyEngine[u8PortNum].ePESubState = ePE_SNK_READY_IDLE_SS;
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
            PE_ReceiveMsgHandler (u8PortNum, u32Header, u8aDataBuf);
        }
        /*Setting Timeout sub-state to invalid state*/
        gasPolicyEngine[u8PortNum].ePETimeoutSubState = ePE_INVALIDSUBSTATE;
        
            /* Run VCONN_Swap State Machine */
        #if (TRUE == INCLUDE_PD_VCONN_SWAP)
            PE_RunVCONNSwapStateMachine (u8PortNum);
        #endif  
         
            /* Run DR_Swap State Machine */
        #if (TRUE == INCLUDE_PD_DR_SWAP)
            PE_RunDRSwapStateMachine (u8PortNum);
        #endif

            /* Run PR_Swap State Machine */
        #if (TRUE == INCLUDE_PD_PR_SWAP)
            PE_RunPRSwapStateMachine (u8PortNum);
        #endif        

            /* Run FR_Swap State Machine */
        #if (TRUE == INCLUDE_PD_FR_SWAP)
            PE_RunFRSwapStateMachine (u8PortNum);
        #endif             

        #if (TRUE == INCLUDE_PD_VDM)
            PE_RunVDMStateMachine (u8PortNum, u8aDataBuf, u32Header);
        #endif             
         
        if (PD_ROLE_SOURCE == DPM_GET_CURRENT_POWER_ROLE(u8PortNum))
        {
            /* Run Source State Machine */
            #if (TRUE == INCLUDE_PD_SOURCE)
                PE_RunSrcStateMachine (u8PortNum, u8aDataBuf, u32Header);
            #endif
        }
        else /* Port Power Role is Sink */
        {
            /* Run Sink State Machine */
            #if (TRUE == INCLUDE_PD_SINK)
                PE_RunSnkStateMachine (u8PortNum, u8aDataBuf, u32Header);
            #endif
        }
        
        /* Run PE State Machine common to Source and Sink */
        PE_RunCommonStateMachine (u8PortNum, u8aDataBuf, u32Header);        
    }
#if(TRUE == INCLUDE_PD_DRP)    
    else
    {
        /* This hook is needed to notify that PE has entered idle 
           after a detach. For a DRP port, current power role will be 
           changed to DRP after detach. So, PE SMs will not run and 
           therefore this notification is needed */
        MCHP_PSF_HOOK_NOTIFY_IDLE(u8PortNum, eIDLE_PE_NOTIFY);
    }  
#endif 
}
/***************************************************************************************/
/*******************PE Support functions to decode and handle received messages**********************/
/****************************************************************************************/
UINT8 PE_IsMsgUnsupported (UINT8 u8PortNum, UINT16 u16Header)
{    
    UINT8 u8RetVal = PE_SUPPORTED_MSG;
    UINT8 u8MsgType = PRL_GET_MESSAGE_TYPE (u16Header);
    UINT8 u8DefaultPwrRole = DPM_GET_DEFAULT_POWER_ROLE(u8PortNum); 
    UINT8 u8CurrentPwrRole = DPM_GET_CURRENT_POWER_ROLE(u8PortNum);

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
        if (PE_OBJECT_COUNT_0 == PRL_GET_OBJECT_COUNT (u16Header))
        {
            /*Control messages in Table 6-5 of PD 3.0 Spec after "Not Supported" message are
            are not supported*/
            /*Any control message with message type between soft reset and Not supported are
            not supported as they are reserved fields*/
            if ((PE_CTRL_GET_STATUS == u8MsgType) || (PE_CTRL_GET_PPS_STATUS == u8MsgType))
            {
                /* Get_Status and Get_PPS_Status messages are supported for Source
                and if any APDOs are advertised to port partner.*/                
                #if (FALSE == INCLUDE_PD_SOURCE_PPS)
                    u8RetVal = PE_UNSUPPORTED_MSG;
                #else
                    if (FALSE == DPM_IsAPDOAdvertised(u8PortNum))
                    {
                        u8RetVal = PE_UNSUPPORTED_MSG;
                    }
                #endif    
            }
            else if (PE_CTRL_DR_SWAP == u8MsgType)
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
            else if (PE_CTRL_FR_SWAP == u8MsgType)
            {            
                #if (FALSE == INCLUDE_PD_FR_SWAP)
                    u8RetVal = PE_UNSUPPORTED_MSG; 
                #else                     
                    if ((PD_ROLE_SINK == u8CurrentPwrRole) || \
                            (FALSE == DPM_GET_PDO_FRS_CURRENT(gasCfgStatusData.sPerPortData[u8PortNum].u32aSinkPDO[INDEX_0])))
                    {
                        u8RetVal = PE_UNSUPPORTED_MSG;
                    }
                #endif 
            }
            else if ((PE_CTRL_GET_SINK_CAP == u8MsgType) || \
                        (PE_CTRL_GET_SINK_CAP_EXTENDED == u8MsgType))
            {
                /* Get Sink Caps shall be supported for Sink only and DRP ports */                
                if (PD_ROLE_SOURCE == u8DefaultPwrRole)
                {
                    u8RetVal = PE_UNSUPPORTED_MSG;
                }
            }
            else if (PE_CTRL_GET_SOURCE_CAP == u8MsgType)
            {
                /* Get Source Caps shall be supported for Source only and DRP ports */
                if ((PD_ROLE_SINK == u8DefaultPwrRole) && \
                        (FALSE == DPM_IS_DRP_SWITCHED_TO_SINK_ROLE(u8PortNum)))
                {
                    u8RetVal = PE_UNSUPPORTED_MSG;
                }                
            }
            else if ((PE_RESERVED_MSG_TYPE == u8MsgType) || (u8MsgType > PE_CTRL_NOT_SUPPORTED) || \
                    ((u8MsgType > PE_CTRL_SOFT_RESET) && (u8MsgType < PE_CTRL_NOT_SUPPORTED)))
            {
                u8RetVal = PE_UNSUPPORTED_MSG;
            }
            else if ((PD_ROLE_SOURCE == u8CurrentPwrRole) && \
                     ((PE_CTRL_PING == u8MsgType) || (PE_CTRL_GOTO_MIN == u8MsgType)))
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
            else if ((PE_RESERVED_MSG_TYPE == u8MsgType) || ((u8MsgType > PE_DATA_SINK_CAP) && \
                    (u8MsgType != PE_DATA_VENDOR_DEFINED)))
            {
                u8RetVal = PE_UNSUPPORTED_MSG;
            }
            else if ((PD_ROLE_SOURCE == u8CurrentPwrRole) && \
                     (PE_DATA_SOURCE_CAP == u8MsgType))
            {
                u8RetVal = PE_UNSUPPORTED_MSG;
            }
            else if ((PD_ROLE_SINK == u8DefaultPwrRole) && \
                    (PE_DATA_SINK_CAP == u8MsgType))
            {
                u8RetVal = PE_UNSUPPORTED_MSG;
            }
            else if ((PD_ROLE_SINK == u8CurrentPwrRole) && \
                    ((PE_DATA_REQUEST == u8MsgType) || \
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
    UINT8 u8RetVal = PE_PROCESS_MSG;

    /*Check whether the message is supported by the policy engine*/
    /*Message header is passed without the extender header bytes*/
    UINT8 u8PEMsgSupport = PE_IsMsgUnsupported (u8PortNum, PRL_GET_MSG_HEADER(u32Header));

    UINT8 u8IsVDMTimerOn = (((ePE_VDM_INITIATE_VDM == gasPolicyEngine[u8PortNum].ePEState) || \
            (ePE_VDM_IDENTITY_REQUEST == gasPolicyEngine[u8PortNum].ePEState)) ? TRUE : FALSE);
    
    if (PE_UNSUPPORTED_MSG == u8PEMsgSupport)
    {
        /* Kill the VDM Sender Response timer when PE is waiting for VDM 
           response, but the AMS is interrupted by an Unsupported AMS */
        if (u8IsVDMTimerOn)
        {
            PE_KillPolicyEngineTimer (u8PortNum);
        }
        /* Unsupported message received in ready state or when an 
           interruptible AMS is in progress */
        if ((ePE_SRC_READY == gasPolicyEngine[u8PortNum].ePEState) ||
           (ePE_SNK_READY == gasPolicyEngine[u8PortNum].ePEState)  || 
           (ePE_VDM_RESPOND_VDM == gasPolicyEngine[u8PortNum].ePEState) ||
           (u8IsVDMTimerOn))
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
        else
        {
            /* Unsupported message received during Non interruptible AMS */
            PE_HandleUnExpectedMsg (u8PortNum);            
        }
        
        /* If there are any Active Modes between the Port Partners when a DR_Swap Message 
           is a received then a Hard Reset Shall be performed. If DR_Swap is supported,
           this will be handled in PE_ReceiveMsgHandler() */        
#if (TRUE == INCLUDE_PD_ALT_MODE)        
        if ((PE_CTRL_DR_SWAP == PRL_GET_MESSAGE_TYPE (u32Header)) && \
                DPM_IS_MODAL_OPERATION_ACTIVE(u8PortNum))
        {
            PE_SendHardReset (u8PortNum);
        }
#endif 
        /* Update the return value as the unsupported message is handled */
        u8RetVal = PE_MSG_HANDLED;
    }

    return u8RetVal;
}

void PE_HandleRcvdMsgAndTimeoutEvents (UINT8 u8PortNum, ePolicyState eNextState , ePolicySubState eNextSubState)
{
    if (ePE_INVALIDSUBSTATE == gasPolicyEngine[u8PortNum].ePETimeoutSubState)
    {
        gasPolicyEngine[u8PortNum].ePEState = eNextState;
        gasPolicyEngine[u8PortNum].ePESubState = eNextSubState;
    }
    else
    {
        if (gasPolicyEngine[u8PortNum].u32TimeoutMsgHeader == gasPolicyEngine[u8PortNum].u32MsgHeader)
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
    DEBUG_PRINT_PORT_STR (u8PortNum,"PE: Handle UnExpected Msg\r\n");
    
    DEBUG_PRINT_PORT_STR ((UINT32)gasPolicyEngine[u8PortNum].ePEState,"PE State\r\n");
    DEBUG_PRINT_PORT_STR ((UINT32)gasPolicyEngine[u8PortNum].ePESubState,"PE Substate\r\n");
        
    /*Send Hard reset if Current State is in power transition*/
    if ((ePE_SRC_TRANSITION_SUPPLY == gasPolicyEngine[u8PortNum].ePEState) || \
      (ePE_SNK_TRANSITION_SINK == gasPolicyEngine[u8PortNum].ePEState))
    {
        PE_SendHardReset (u8PortNum);
    }
    /*Send soft reset if  Current State is not power transitioned*/
    else
    {
        gasPolicyEngine[u8PortNum].ePEState = ePE_SEND_SOFT_RESET;
        gasPolicyEngine[u8PortNum].ePESubState = ePE_SEND_SOFT_RESET_ENTRY_SS;
    }
}

void PE_ReceiveMsgHandler (UINT8 u8PortNum, UINT32 u32Header, UINT8 *pu8DataBuf)
{
    /* Validate the PD Message and get the status whether the message needs to be processed */
    UINT8 u8MsgOperation = PE_ValidateMessage (u8PortNum, u32Header);
    
    /* Is Policy Engine in VDM State */
    UINT8 u8PEInVDMState = (((ePE_VDM_INITIATE_VDM == gasPolicyEngine[u8PortNum].ePEState) || \
          (ePE_VDM_IDENTITY_REQUEST == gasPolicyEngine[u8PortNum].ePEState) || \
          (ePE_VDM_RESPOND_VDM == gasPolicyEngine[u8PortNum].ePEState)) ? TRUE : FALSE);    
    
    UINT8 u8IsVDMTimerOn = (((ePE_VDM_INITIATE_VDM == gasPolicyEngine[u8PortNum].ePEState) || \
            (ePE_VDM_IDENTITY_REQUEST == gasPolicyEngine[u8PortNum].ePEState)) ? TRUE : FALSE);
            
    /* Is PD Contract a PPS contract */
    UINT8 u8IsPPSCommTmrOn = (((DPM_PD_PPS_CONTRACT == DPM_GET_CURRENT_EXPLICIT_CONTRACT(u8PortNum)) && \
                             (ePE_SRC_READY == gasPolicyEngine[u8PortNum].ePEState)) ? TRUE : FALSE);

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
                       Response Timer and change the PE sub-state to ePE_SRC_GET_SINK_STATUS_RESPONSE_RCVD_SS */
                    if ((ePE_SRC_GET_SINK_STATUS_MSG_DONE_SS == gasPolicyEngine[u8PortNum].ePESubState) || \
                        (ePE_SRC_GET_SINK_STATUS_IDLE_SS == gasPolicyEngine[u8PortNum].ePESubState))
                    {                       
                        PE_KillPolicyEngineTimer (u8PortNum);
                        PE_HandleRcvdMsgAndTimeoutEvents(u8PortNum, ePE_SRC_GET_SINK_STATUS, ePE_SRC_GET_SINK_STATUS_RESPONSE_RCVD_SS);
                    }
                    else
                    {
                        /* Handle Unexpected message when Status is received in an
                           incorrect state */
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
                    DEBUG_PRINT_PORT_STR (u8PortNum,"Source Cap message received\r\n");
                    
                    /*Discard the VDM AMS and process the source capability message received*/
                    if ((ePE_SNK_WAIT_FOR_CAPABILITIES == gasPolicyEngine[u8PortNum].ePEState) || \
                        (ePE_SNK_READY == gasPolicyEngine[u8PortNum].ePEState) || \
                        (u8PEInVDMState))
                    {
                        /* Kill VDM Response timer if PE is waiting for a VDM response, but
                           the AMS is interrupted by Source Cap AMS */
                        if (u8IsVDMTimerOn)
                        {
                            PE_KillPolicyEngineTimer (u8PortNum);
                        }
                        if (ePE_SNK_WAIT_FOR_CAPABILITIES == \
                            gasPolicyEngine[u8PortNum].ePEState)
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
                    DEBUG_PRINT_PORT_STR (u8PortNum,"Request message received\r\n");
                    
                    if ((ePE_SRC_SEND_CAPABILITIES == gasPolicyEngine[u8PortNum].ePEState) || \
                       (ePE_SRC_READY == gasPolicyEngine[u8PortNum].ePEState) || \
                       (u8PEInVDMState))
                    {
                        /* Kill SenderResponse timer only in ePE_SRC_SEND_CAPABILITIES 
                           Kill SourcePPSCommTimer only in ePE_SRC_READY state if 
                           the current explicit contract is for a PPS APDO
                           Kill VDM Response timer if PE is waiting for a VDM response, but
                           the AMS is interrupted by an asynchronous request */
                        if ((ePE_SRC_SEND_CAPABILITIES == gasPolicyEngine[u8PortNum].ePEState) || \
                            (u8IsVDMTimerOn)|| (u8IsPPSCommTmrOn))   
                        {
                            PE_KillPolicyEngineTimer (u8PortNum);
                        }
                        
                        PE_HandleRcvdMsgAndTimeoutEvents (u8PortNum,ePE_SRC_NEGOTIATE_CAPABILITY,(ePolicySubState)SET_TO_ZERO);                        
                    }
                    else
                    {
                        PE_HandleUnExpectedMsg (u8PortNum);
                    }

                    break;
                }
                case PE_DATA_BIST:
                {
                    if (((ePE_SNK_READY == gasPolicyEngine[u8PortNum].ePEState) || \
                        (ePE_SRC_READY == gasPolicyEngine[u8PortNum].ePEState)) &&	\
                        (TYPEC_VBUS_5V == DPM_GetVBUSVoltage (u8PortNum)))
                    {
                        /* Kill SourcePPSCommTimer only in ePE_SRC_READY state if 
                           the current explicit contract is for a PPS APDO*/
                        if (u8IsPPSCommTmrOn)
                        {
                            PE_KillPolicyEngineTimer (u8PortNum);
                        }

                        gasPolicyEngine[u8PortNum].ePEState = ePE_BIST_MODE;
                        gasPolicyEngine[u8PortNum].ePESubState = ePE_BIST_MODE_ENTRY_SS;                         
                    }
                    break;
                }
                case PE_DATA_VENDOR_DEFINED:
                {
                    /* VDM Response received from Cable in implicit contract */
                    if (ePE_VDM_IDENTITY_REQUEST == gasPolicyEngine[u8PortNum].ePEState)
                    {
                        PE_KillPolicyEngineTimer (u8PortNum);
                        PE_HandleRcvdMsgAndTimeoutEvents (u8PortNum, ePE_VDM_IDENTITY_ACKED,(ePolicySubState)SET_TO_ZERO);
                    }
                    else if ((ePE_SNK_READY == gasPolicyEngine[u8PortNum].ePEState) || \
                            (ePE_SRC_READY == gasPolicyEngine[u8PortNum].ePEState) || \
                            (u8PEInVDMState))
                    {                        
                        /* In case of PPS, Kill SourcePPSCommTimer only in ePE_SRC_READY  
                           state if the current explicit contract is for a PPS APDO 
                           Kill VDM Response timer if PE is waiting for a VDM response, but
                           the AMS is interrupted by another VDM AMS */
                        if (u8IsVDMTimerOn || u8IsPPSCommTmrOn)                         
                        {
                            PE_KillPolicyEngineTimer (u8PortNum);
                        }   
                        if (DPM_VDM_UNSTRUCTURED_VDM == DPM_GET_VDM_TYPE (*(pu8DataBuf + BYTE_LEN_1)))
                        {
                            /* Unstructured VDMs are unsupported by PSF */
                            PE_HandleUnsupportedVDM (u8PortNum);
                        }
                        else /* DPM_VDM_STRUCTURED_VDM */ 
                        {
                            if (DPM_VDM_REQ == DPM_GET_VDM_CMD_TYPE (*pu8DataBuf))
                            {
                                /* Respond with appropriate VDO if VDM is supported by the port */                            
                                if (DPM_IS_VDM_SUPPORTED(u8PortNum))
                                {
                                    gasPolicyEngine[u8PortNum].ePEState = ePE_VDM_EVALUATE_VDM;  
                                }
                                else
                                {
                                    PE_HandleUnsupportedVDM (u8PortNum);
                                }
                            }
                            else /* VDM message is a response */
                            {
                                if (u8PEInVDMState)
                                {
                                    PE_HandleRcvdMsgAndTimeoutEvents (u8PortNum, ePE_VDM_INITIATE_VDM,
                                            ePE_VDM_INITIATE_VDM_RESPONSE_RCVD_SS);
                                }
                            }                            
                        }
                    }        
                    else
                    {
                        PE_HandleUnExpectedMsg (u8PortNum);
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
#if (TRUE == INCLUDE_PD_SOURCE)                
                case PE_DATA_SINK_CAP:
                {
                    /* Once response for Get_Sink_Cap is received, kill the Sender 
                       Response Timer and change the PE sub-state as  ePE_GET_SINK_CAP_RESPONSE_RCVD_SS */
                    if ((ePE_GET_SINK_CAP_MSG_DONE_SS == gasPolicyEngine[u8PortNum].ePESubState) || \
                        (ePE_GET_SINK_CAP_IDLE_SS == gasPolicyEngine[u8PortNum].ePESubState))
                    {                       
                        PE_KillPolicyEngineTimer (u8PortNum);
                        PE_HandleRcvdMsgAndTimeoutEvents(u8PortNum, ePE_GET_SINK_CAP, ePE_GET_SINK_CAP_RESPONSE_RCVD_SS);
                    }
                    else
                    {
                        /* Handle Unexpected message when Sink capabilities message is 
                           received in incorrect PE State */
                        PE_HandleUnExpectedMsg (u8PortNum);
                    }
                    break; 
                }
#endif 
#if (TRUE == INCLUDE_PD_SOURCE_PPS)
                case PE_DATA_ALERT: 
                {
                    if ((ePE_SRC_READY == gasPolicyEngine[u8PortNum].ePEState) || \
                        (u8PEInVDMState))
                    {
                        /* In case of PPS, Kill SourcePPSCommTimer only in ePE_SRC_READY  
                           state if the current explicit contract is for a PPS APDO 
                           Kill VDM Response timer if PE is waiting for a VDM response, but
                           the AMS is interrupted by an Alert message */
                        if (u8IsPPSCommTmrOn || u8IsVDMTimerOn)
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
                    else if (ePE_SNK_READY_END_AMS_SS == gasPolicyEngine[u8PortNum].ePESubState)
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
                    if ((ePE_SNK_SELECT_CAPABILITY_WAIT_FOR_ACCEPT_SS == \
                        gasPolicyEngine[u8PortNum].ePESubState) || \
                        (ePE_SNK_SELECT_CAPABILITY_REQ_SENT_SS == \
                         gasPolicyEngine[u8PortNum].ePESubState))
                    {
                        /*kill the timer PE_SENDER_RESPONSE_TIMEOUTID*/
                        DEBUG_PRINT_PORT_STR (u8PortNum,"Request accepted by Source\r\n");
                        gasCfgStatusData.sPerPortData[u8PortNum].u32PortConnectStatus |= \
                                    DPM_PORT_AS_SNK_LAST_REQ_ACCEPT_STATUS;
                        PE_KillPolicyEngineTimer (u8PortNum);
                        PE_HandleRcvdMsgAndTimeoutEvents (u8PortNum,ePE_SNK_TRANSITION_SINK,\
                                                                   ePE_SNK_TRANSITION_SINK_ENTRY_SS);
                    }
                    /*Accept message received for soft reset sent to SOP */
                    else if ((ePE_SEND_SOFT_RESET_IDLE_SS == gasPolicyEngine[u8PortNum].ePESubState) || 
                            (ePE_SEND_SOFT_RESET_MSG_DONE_SS == gasPolicyEngine[u8PortNum].ePESubState))
                    {
                        DEBUG_PRINT_PORT_STR (u8PortNum,"Accept received for Soft Reset\r\n");
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
                    /*Accept message received for soft reset sent to SOP_P */
                    else if ((ePE_VCS_CBL_SEND_SOFT_RESET_IDLE_SS == gasPolicyEngine[u8PortNum].ePESubState) || 
                            (ePE_VCS_CBL_SEND_SOFT_RESET_MSG_DONE_SS == gasPolicyEngine[u8PortNum].ePESubState))
                    {
                        DEBUG_PRINT_PORT_STR (u8PortNum,"Accept received for SOP' Soft Reset\r\n");                        
                        /*kill the timer PE_SENDER_RESPONSE_TIMEOUTID*/
                        PE_KillPolicyEngineTimer (u8PortNum);

                        if (PD_ROLE_SOURCE == DPM_GET_CURRENT_POWER_ROLE(u8PortNum))
                        {
                            PE_HandleRcvdMsgAndTimeoutEvents (u8PortNum,ePE_SRC_READY,\
                                                                   ePE_SRC_READY_END_AMS_SS);                             
                        }
                        else
                        {
                            PE_HandleRcvdMsgAndTimeoutEvents (u8PortNum,ePE_SNK_READY,\
                                                                   ePE_SNK_READY_END_AMS_SS);                             
                        }
                    }
#if (TRUE == INCLUDE_PD_VCONN_SWAP)
                    /* Accept message received for VCONN Swap request */
                    else if ((ePE_VCS_SEND_SWAP_IDLE_SS == gasPolicyEngine[u8PortNum].ePESubState) || 
                            (ePE_VCS_SEND_SWAP_MSG_DONE_SS == gasPolicyEngine[u8PortNum].ePESubState)) 
                    {
                        DEBUG_PRINT_PORT_STR (u8PortNum,"VCONN_SWAP Accepted by partner\r\n");
                        /* Kill the Sender Response Timer */
                        PE_KillPolicyEngineTimer (u8PortNum);
                        if (DPM_IsPortVCONNSource (u8PortNum))
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
#if (TRUE == INCLUDE_PD_FR_SWAP)
                    /* Accept message received for FR_Swap request */
                    else if ((ePE_FRS_SNK_SRC_SEND_SWAP_IDLE_SS == gasPolicyEngine[u8PortNum].ePESubState) || 
                            (ePE_FRS_SNK_SRC_SEND_SWAP_MSG_DONE_SS == gasPolicyEngine[u8PortNum].ePESubState)) 
                    {
                        DEBUG_PRINT_PORT_STR (u8PortNum,"FR_Swap Accepted by partner\r\n");
                        /* Kill the Sender Response Timer */
                        PE_KillPolicyEngineTimer (u8PortNum);
                        
                        PE_HandleRcvdMsgAndTimeoutEvents (u8PortNum, ePE_FRS_SNK_SRC_TRANSITION_TO_OFF, 
                                ePE_FRS_SNK_SRC_TRANSITION_TO_OFF_ENTRY_SS);
                    }
#endif                     
#if (TRUE == INCLUDE_PD_PR_SWAP)
                    /* Accept message received for PR_Swap request */
                    else if ((ePE_PRS_SEND_SWAP_IDLE_SS == gasPolicyEngine[u8PortNum].ePESubState) || 
                            (ePE_PRS_SEND_SWAP_MSG_DONE_SS == gasPolicyEngine[u8PortNum].ePESubState)) 
                    {
                        DEBUG_PRINT_PORT_STR (u8PortNum,"PR_SWAP Accepted by partner\r\n");
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
                    else if ((ePE_DRS_SEND_SWAP_IDLE_SS == gasPolicyEngine[u8PortNum].ePESubState) || 
                            (ePE_DRS_SEND_SWAP_MSG_DONE_SS == gasPolicyEngine[u8PortNum].ePESubState))
                    {
						/*Accept handling for DR_SWAP*/
                        DEBUG_PRINT_PORT_STR (u8PortNum,"DR_SWAP Accepted by partner\r\n");
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
                    if ((ePE_SNK_SELECT_CAPABILITY_WAIT_FOR_ACCEPT_SS == \
                        gasPolicyEngine[u8PortNum].ePESubState)  || \
                        (ePE_SNK_SELECT_CAPABILITY_REQ_SENT_SS == \
                         gasPolicyEngine[u8PortNum].ePESubState))
                    {
                        /* Kill the Sender Response Timer */
                        PE_KillPolicyEngineTimer (u8PortNum);
                        
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
                                gasPolicyEngine[u8PortNum].ePESubState = ePE_SNK_READY_END_AMS_SS;
                            }
                            else if (PE_CTRL_REJECT == PRL_GET_MESSAGE_TYPE(u32Header))
                            {
                                /*Set the status that last request was rejected*/
                                gasCfgStatusData.sPerPortData[u8PortNum].u32PortConnectStatus |= \
                                            DPM_PORT_AS_SNK_LAST_REQ_REJECT_STATUS;
                                gasPolicyEngine[u8PortNum].ePEState = ePE_SNK_READY;
                                gasPolicyEngine[u8PortNum].ePESubState = ePE_SNK_READY_END_AMS_SS;
                                DEBUG_PRINT_PORT_STR (u8PortNum,"Request rejected by Source\r\n");
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
                            DEBUG_PRINT_PORT_STR (u8PortNum,"VCONN_SWAP rejected by partner\r\n");
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
                        /* Kill the Sender Response Timer */
                        PE_KillPolicyEngineTimer (u8PortNum);
                        
                        if (PE_CTRL_REJECT == (PRL_GET_MESSAGE_TYPE(u32Header)))
                        {
                            DEBUG_PRINT_PORT_STR (u8PortNum,"PR_SWAP rejected by partner\r\n");
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
                            (ePE_DRS_SEND_SWAP_MSG_DONE_SS == gasPolicyEngine[u8PortNum].ePESubState)) 
                    {
						/*Reject and wait handling for DR_SWAP*/
                        /* Kill the Sender Response Timer */
                        PE_KillPolicyEngineTimer (u8PortNum);
                        
                        if (PE_CTRL_REJECT == (PRL_GET_MESSAGE_TYPE(u32Header)))
                        {
                            DEBUG_PRINT_PORT_STR (u8PortNum,"DR_SWAP rejected by partner\r\n");
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
                    else if ((PD_ROLE_DRP == DPM_GET_CONFIGURED_POWER_ROLE(u8PortNum)) && 
                            (PD_ROLE_SINK == DPM_GET_CURRENT_POWER_ROLE(u8PortNum)) &&
                            ((ePE_GET_SINK_CAP_IDLE_SS == gasPolicyEngine[u8PortNum].ePESubState) || 
                            (ePE_GET_SINK_CAP_MSG_DONE_SS == gasPolicyEngine[u8PortNum].ePESubState))) 
                    {
						/*Reject handling for Get_Sink_Cap only when PSF is 
                          Dual role capable and currently acting as Sink*/
                        
                        /* Kill the Sender Response Timer */
                        PE_KillPolicyEngineTimer (u8PortNum);
                        
                        if (PE_CTRL_REJECT == (PRL_GET_MESSAGE_TYPE(u32Header)))
                        {
                            gasPolicyEngine[u8PortNum].ePESubState = ePE_GET_SINK_CAP_NO_RESPONSE_SS;                                                                            
                        }
                        else
                        {
                            /* Do Nothing */
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
                        DEBUG_PRINT_PORT_STR (u8PortNum,"VCONN_SWAP: PS_RDY received\r\n");
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
#if (TRUE == INCLUDE_PD_FR_SWAP)
                    /* PS_RDY received for Fast Role Swap from partner */
                    else if (ePE_FRS_SRC_SNK_WAIT_SOURCE_ON_WAIT_FOR_PSRDY_SS == \
                                    gasPolicyEngine[u8PortNum].ePESubState)
                    {
                        DEBUG_PRINT_PORT_STR (u8PortNum,"FR_SWAP: PS_RDY received from Initial Sink\r\n");
                         /*Kill the PSSourceOn timer*/
                        PE_KillPolicyEngineTimer (u8PortNum);
                        
                        PE_HandleRcvdMsgAndTimeoutEvents (u8PortNum,ePE_FRS_SRC_SNK_WAIT_SOURCE_ON,\
                                           ePE_FRS_SRC_SNK_WAIT_SOURCE_ON_PSRDY_RCVD_SS);                                                
                    }
                    else if (ePE_FRS_SNK_SRC_TRANSITION_TO_OFF_WAIT_FOR_PSRDY_SS == \
                                    gasPolicyEngine[u8PortNum].ePESubState)
                    {
                        DEBUG_PRINT_PORT_STR (u8PortNum,"FR_SWAP: PS_RDY received from Initial Source\r\n");
                        /*Kill the PSSourceOff timer*/
                        PE_KillPolicyEngineTimer (u8PortNum);                        

                        PE_HandleRcvdMsgAndTimeoutEvents (u8PortNum,ePE_FRS_SNK_SRC_VBUS_APPLIED,\
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
                        (u8PEInVDMState))
                    {                        
                        /* In case of PPS, Kill SourcePPSCommTimer only in ePE_SRC_READY  
                           state if the current explicit contract is for a PPS APDO 
                           Kill VDM Response timer if PE is waiting for a VDM response, but
                           the AMS is interrupted by Get Source Cap AMS */
                        if (u8IsVDMTimerOn || u8IsPPSCommTmrOn)                           
                        {
                            PE_KillPolicyEngineTimer (u8PortNum);
                        }  
                         
                        gasPolicyEngine[u8PortNum].ePEState = ePE_SRC_SEND_CAPABILITIES;
                        gasPolicyEngine[u8PortNum].ePESubState = ePE_SRC_SEND_CAP_ENTRY_SS; 
                    }
#if (TRUE == INCLUDE_PD_DRP)
                    else if (ePE_SNK_READY == gasPolicyEngine[u8PortNum].ePEState)
                    {
                        #if (TRUE == INCLUDE_PD_FR_SWAP)
                        if (DPM_IS_DRP_SWITCHED_TO_SINK_ROLE(u8PortNum))
                        {
                            gasPolicyEngine[u8PortNum].ePEState = ePE_SEND_REJECT;
                            gasPolicyEngine[u8PortNum].ePESubState = ePE_SEND_REJECT_ENTRY_SS;
                        }
                        else
                        #endif 
                        {
                            gasPolicyEngine[u8PortNum].ePEState = ePE_GIVE_CAP;
                            gasPolicyEngine[u8PortNum].ePESubState = ePE_GIVE_CAP_ENTRY_SS;                                                                            
                        }
                    }
#endif 
                    else
                    {
                        PE_HandleUnExpectedMsg (u8PortNum);
                    }

                    break;
                }

                case PE_CTRL_GET_SINK_CAP:
                    /* fallthrough */
                case PE_CTRL_GET_SINK_CAP_EXTENDED:
                {
                    if ((ePE_SRC_READY == gasPolicyEngine[u8PortNum].ePEState) || \
                        (ePE_SNK_READY == gasPolicyEngine[u8PortNum].ePEState) || \
                        (u8PEInVDMState))   
                    {                       
                        /* In case of PPS, Kill SourcePPSCommTimer only in ePE_SRC_READY  
                           state if the current explicit contract is for a PPS APDO 
                           Kill VDM Response timer if PE is waiting for a VDM response, but
                           the AMS is interrupted by Get Sink Cap AMS */
                        if (u8IsVDMTimerOn || u8IsPPSCommTmrOn)
                        {
                            PE_KillPolicyEngineTimer (u8PortNum);
                        }  
                       
                        gasPolicyEngine[u8PortNum].ePEState = ePE_GIVE_CAP;
                        gasPolicyEngine[u8PortNum].ePESubState = ePE_GIVE_CAP_ENTRY_SS;                        
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
                        (u8PEInVDMState))
                    {
                        DEBUG_PRINT_PORT_STR (u8PortNum,"VCONN_Swap Received\r\n");
                                               
                        /* In case of PPS, Kill SourcePPSCommTimer only in ePE_SRC_READY  
                           state if the current explicit contract is for a PPS APDO 
                           Kill VDM Response timer if PE is waiting for a VDM response, but
                           the AMS is interrupted by VCONN Swap AMS */
                        if (u8IsVDMTimerOn || u8IsPPSCommTmrOn)           
                        {
                            PE_KillPolicyEngineTimer (u8PortNum);
                        }  
#if (TRUE == INCLUDE_PD_VCONN_SWAP)
                                          
                        gasPolicyEngine[u8PortNum].ePEState = ePE_VCS_EVALUATE_SWAP;
                        /* Set the timer Id to Max Concurrent Value*/
                        gasDPM[u8PortNum].u8VCONNSwapWaitTmrID = MAX_CONCURRENT_TIMERS;
#else
                        gasPolicyEngine[u8PortNum].ePEState = ePE_SEND_REJECT;
                        gasPolicyEngine[u8PortNum].ePESubState = ePE_SEND_REJECT_ENTRY_SS;
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
                        (u8PEInVDMState))
                    {                                                
                        /* In case of PPS, Kill SourcePPSCommTimer only in ePE_SRC_READY  
                           state if the current explicit contract is for a PPS APDO 
                           Kill VDM Response timer if PE is waiting for a VDM response, but
                           the AMS is interrupted by Get Status AMS */
                        if (u8IsPPSCommTmrOn || u8IsVDMTimerOn)
                        {
                            PE_KillPolicyEngineTimer (u8PortNum);
                        }                         
                        gasPolicyEngine[u8PortNum].ePEState = ePE_SRC_GIVE_SOURCE_STATUS;
                        gasPolicyEngine[u8PortNum].ePESubState = ePE_SRC_GIVE_SOURCE_STATUS_ENTRY_SS;                      
                        
                        /*Kill the DPM_STATUS_FAULT_PERSIST_TIMEOUT_MS timer*/
                        PDTimer_Kill (gasDPM[u8PortNum].u8PPSFaultPersistTmrID);
                        /* Set the timer Id to Max Concurrent Value*/
                        gasDPM[u8PortNum].u8PPSFaultPersistTmrID = MAX_CONCURRENT_TIMERS;
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
                        (u8PEInVDMState))
                    {                                                                      
                        /* In case of PPS, Kill SourcePPSCommTimer only in ePE_SRC_READY  
                           state if the current explicit contract is for a PPS APDO 
                           Kill VDM Response timer if PE is waiting for a VDM response, but
                           the AMS is interrupted by Get PPS Status AMS */                        
                        if (u8IsPPSCommTmrOn || u8IsVDMTimerOn)
                        {
                            PE_KillPolicyEngineTimer (u8PortNum);
                        }
                        gasPolicyEngine[u8PortNum].ePEState = ePE_SRC_GIVE_PPS_STATUS;
                        gasPolicyEngine[u8PortNum].ePESubState = ePE_SRC_GIVE_PPS_STATUS_ENTRY_SS;                        
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
                    DEBUG_PRINT_PORT_STR (u8PortNum,"DR_Swap Received\r\n");
                    
                    /* If there are any Active Modes between the Port Partners when a DR_Swap Message 
                       is a received then a Hard Reset Shall be performed*/
                    if (DPM_IS_MODAL_OPERATION_ACTIVE(u8PortNum))
                    {
                        PE_SendHardReset (u8PortNum);
                    }
                    /*DR_SWAP message is valid only if the PE state is SRC_READY or SNK_READY*/
                    else if ((ePE_SNK_READY == gasPolicyEngine[u8PortNum].ePEState) || \
                        (ePE_SRC_READY == gasPolicyEngine[u8PortNum].ePEState) || \
                        (u8PEInVDMState))
                    {                        
                        /* In case of PPS, Kill SourcePPSCommTimer only in ePE_SRC_READY  
                           state if the current explicit contract is for a PPS APDO 
                           Kill VDM Response timer if PE is waiting for a VDM response, but
                           the AMS is interrupted by DR Swap AMS */
                        if (u8IsVDMTimerOn || u8IsPPSCommTmrOn)
                        {
                            PE_KillPolicyEngineTimer (u8PortNum);
                        }   
 
                        /*Kill the u8DRSwapWaitTmrID timer*/
                        PDTimer_Kill (gasDPM[u8PortNum].u8DRSwapWaitTmrID);
                        gasDPM[u8PortNum].u8DRSwapWaitTmrID = MAX_CONCURRENT_TIMERS;
                        gasPolicyEngine[u8PortNum].ePEState = ePE_DRS_EVALUATE_SWAP;                         
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
                    DEBUG_PRINT_PORT_STR (u8PortNum,"PR_Swap Received\r\n");
                    
                    /*Accept the Power Role swap if current state is READY State or
                    any VDM AMS is active*/
                    if ((ePE_SNK_READY == gasPolicyEngine[u8PortNum].ePEState) || \
                        (ePE_SRC_READY == gasPolicyEngine[u8PortNum].ePEState) || \
                        (u8PEInVDMState))
                    {                       
                        /* In case of PPS, Kill SourcePPSCommTimer only in ePE_SRC_READY  
                           state if the current explicit contract is for a PPS APDO 
                           Kill VDM Response timer if PE is waiting for a VDM response, but
                           the AMS is interrupted by PR Swap AMS */
                        if (u8IsVDMTimerOn || u8IsPPSCommTmrOn)
                        {
                            PE_KillPolicyEngineTimer (u8PortNum);
                        }                                
                        /*Kill the tPRSwapWait timer*/
                        PDTimer_Kill (gasDPM[u8PortNum].u8PRSwapWaitTmrID);
                        /* Set the timer Id to Max Concurrent Value*/
                        gasDPM[u8PortNum].u8PRSwapWaitTmrID = MAX_CONCURRENT_TIMERS;

                        gasPolicyEngine[u8PortNum].ePEState = ePE_PRS_EVALUATE_SWAP; 
                    }
                    else
                    {
                        PE_HandleUnExpectedMsg (u8PortNum);
                    }
                    break;                    
                }
#endif                 
                
#if (TRUE == INCLUDE_PD_FR_SWAP)
                case PE_CTRL_FR_SWAP: 
                {
                    /*Accept the Fast Role swap if current state is READY State or
                      any VDM AMS is active.  */
                    if (((ePE_SRC_READY == gasPolicyEngine[u8PortNum].ePEState) &&
                        (ePE_SRC_READY_IDLE_SS == gasPolicyEngine[u8PortNum].ePESubState))  || \
                        (u8PEInVDMState))
                    {                    
                        /* In case of PPS, Kill SourcePPSCommTimer only in ePE_SRC_READY  
                        state if the current explicit contract is for a PPS APDO 
                        Kill VDM Response timer if PE is waiting for a VDM response, but
                        the AMS is interrupted by DR Swap AMS */
                        if (u8IsVDMTimerOn || u8IsPPSCommTmrOn)
                        {
                            PE_KillPolicyEngineTimer (u8PortNum);
                        }                         
                        DEBUG_PRINT_PORT_STR (u8PortNum,"FR_SWAP Received from Partner\r\n");

                        gasPolicyEngine[u8PortNum].ePEState = ePE_FRS_SRC_SNK_EVALUATE_SWAP; 
                    }
                    else
                    {
                        /* PD Spec reference regarding FR_Swap: This process can occur at any time,
                           even during a Non-interruptible AMS in which case error handling such as
                           Hard Reset or [USB Type-C 2.0] Error Recovery will be triggered.*/
                        if (PE_IMPLICIT_CONTRACT == PE_GET_PD_CONTRACT(u8PortNum))
                        {
                            if (TRUE == DPM_NotifyClient (u8PortNum, eMCHP_PSF_TYPEC_ERROR_RECOVERY))
                            {
                                DPM_SetTypeCState (u8PortNum, TYPEC_ERROR_RECOVERY, TYPEC_ERROR_RECOVERY_ENTRY_SS);
                            }
                            else
                            {
                                /*Do nothing. If User application returns FALSE for 
                                 eMCHP_PSF_TYPEC_ERROR_RECOVERY notification, it is expected that
                                 the user application will raise a Port disable client request*/
                            }
                        }  
                        else /* PE_EXPLICIT_CONTRACT */
                        {
                            PE_SendHardReset (u8PortNum);
                        }
                    }                    
                    break;                    
                }
#endif                 
                case PE_CTRL_NOT_SUPPORTED:
                {
                    if (ePE_VDM_INITIATE_VDM == gasPolicyEngine[u8PortNum].ePEState)
                    {
#if (TRUE == INCLUDE_PD_VDM)
                        /* Kill the VDM Sender Response Timer */
                        PE_KillPolicyEngineTimer (u8PortNum);  
                        
                        PE_HandleRcvdMsgAndTimeoutEvents (u8PortNum,ePE_VDM_INITIATE_VDM,\
                                                                   ePE_VDM_INITIATE_VDM_NO_RESPONSE_SS);                                                     
#endif 
                    }                    
                    else if (ePE_SRC_GET_SINK_STATUS == gasPolicyEngine[u8PortNum].ePEState)
                    {
#if (TRUE == INCLUDE_PD_SOURCE_PPS)                        
                        /* Kill the Sender Response Timer */
                        PE_KillPolicyEngineTimer (u8PortNum);  
                        
                        PE_HandleRcvdMsgAndTimeoutEvents (u8PortNum,ePE_SRC_GET_SINK_STATUS,\
                                                                   ePE_SRC_GET_SINK_STATUS_NO_RESPONSE_SS);                                                                                                    
#endif
                    }
                    else if (ePE_VCS_SEND_SWAP == gasPolicyEngine[u8PortNum].ePEState)
                    {
#if (TRUE == INCLUDE_PD_VCONN_SWAP)                        
                        /* Kill the Sender Response Timer */
                        PE_KillPolicyEngineTimer (u8PortNum);  
                        
                        PE_HandleRcvdMsgAndTimeoutEvents (u8PortNum,ePE_VCS_SEND_SWAP,\
                                                                   ePE_VCS_SEND_SWAP_NO_RESPONSE_SS);                          
#endif 
                    }
                    else if (ePE_PRS_SEND_SWAP == gasPolicyEngine[u8PortNum].ePEState)
                    {
#if (TRUE == INCLUDE_PD_PR_SWAP)
                        /* Kill the Sender Response Timer */
                        PE_KillPolicyEngineTimer (u8PortNum);  
                        
                        PE_HandleRcvdMsgAndTimeoutEvents (u8PortNum,ePE_PRS_SEND_SWAP,\
                                                                   ePE_PRS_SEND_SWAP_NO_RESPONSE_SS);                                                  
#endif 
                    }
                    else if (ePE_DRS_SEND_SWAP == gasPolicyEngine[u8PortNum].ePEState)
                    {
#if (TRUE == INCLUDE_PD_DR_SWAP)                        
                        /* Kill the Sender Response Timer */
                        PE_KillPolicyEngineTimer (u8PortNum);  
                        
                        PE_HandleRcvdMsgAndTimeoutEvents (u8PortNum,ePE_DRS_SEND_SWAP,\
                                                                   ePE_DRS_SEND_SWAP_NO_RESPONSE_SS);                                                                          
#endif
                    }
                    else if (ePE_GET_SINK_CAP == gasPolicyEngine[u8PortNum].ePEState)
                    {
                        /* Kill the Sender Response Timer */
                        PE_KillPolicyEngineTimer (u8PortNum);  
                        
                        PE_HandleRcvdMsgAndTimeoutEvents (u8PortNum,ePE_GET_SINK_CAP,\
                                                                   ePE_GET_SINK_CAP_NO_RESPONSE_SS);                                                                          
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
}

/***************************************************************************************/

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

/***************************************************************************************/
void PE_RunCommonStateMachine (UINT8 u8PortNum, UINT8 *pu8DataBuf, UINT32 u32Header)
{
    /* Receive VDM Header */
    UINT32 u32VDMHeader = SET_TO_ZERO;
    
    /* Transmit SOP type */
    UINT8 u8TransmitSOP = PRL_SOP_TYPE;
    
    /* Transmit message header */
    UINT32 u32TransmitHeader = SET_TO_ZERO;
    
    /* Transmit Data Object */
    UINT32 *u32pTransmitDataObj = SET_TO_ZERO;
    
    /* Transmit CB */
    PRLTxCallback pfnTransmitCB = PE_StateChange_TransmitCB;
    
    /* States to be assigned after transmit complete */
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
        eTxDoneSS = ePE_SNK_READY_END_AMS_SS;     
        eTxHardRstSS = ePE_SNK_HARD_RESET_SEND_SS; 
    }
#if (FALSE != INCLUDE_PDFU)
    static UINT8 u8TempRespBuffer[3];
#endif   
    /* Data Object Array */
    UINT32 u32aDataObj[PRL_MAX_DATA_OBJ_COUNT] = {SET_TO_ZERO};
    
    /* Data Object Count */
    UINT8 u8DataObjCnt = SET_TO_ZERO;
    
    /* Received message type */
    UINT8 u8MessageType = PRL_GET_MESSAGE_TYPE (u32Header);    

    switch (gasPolicyEngine[u8PortNum].ePEState)
    {                
#if (TRUE == (INCLUDE_PD_SOURCE || INCLUDE_PD_VCONN_SWAP))        
        /* Note: SOP' Discover Identity states are handled in PE_RunCommonStateMachine 
           instead of PE_RunVDMStateMachine to reduce code and data size for Source only or 
           Sink only applications that does not require full VDM support */       
                /*************** PE_VDM_IDENTITY_REQUEST *****************/
        case ePE_VDM_IDENTITY_REQUEST:
        {  
            switch(gasPolicyEngine[u8PortNum].ePESubState)
            {
                case ePE_VDM_IDENTITY_REQUEST_ENTRY_SS:
                {
                    DEBUG_PRINT_PORT_STR (u8PortNum,"PE_VDM_IDENTITY_REQUEST_ENTRY_SS\r\n");
                    
                    /* Choosing VDM version as per Current spec revision */
                    if (PD_SPEC_REVISION_2_0 == DPM_GET_CURRENT_PD_SPEC_REV(u8PortNum))
                    {
                        u32VDMHeader = DPM_VDM_HEADER_LOW_VER;
                    }                    
                    else
                    {
                        u32VDMHeader = DPM_VDM_HEADER_HIGH_VER;
                    }
                    
					/* Send VDM Discover Identity message to E-Cable */
                    u32TransmitHeader = PRL_FormNonSOPTypeMsgHeader (u8PortNum, (UINT8)PE_DATA_VENDOR_DEFINED,  \
                                                                        BYTE_LEN_1, PE_NON_EXTENDED_MSG);
                    u8TransmitSOP = PRL_SOP_P_TYPE;
                    u32pTransmitDataObj = &u32VDMHeader;

                    u32TransmitTmrIDTxSt = PRL_BUILD_PKD_TXST_U32( ePE_VDM_IDENTITY_REQUEST, \
                                                ePE_VDM_IDENTITY_REQUEST_MSG_DONE_SS, ePE_VDM_IDENTITY_REQUEST, ePE_VDM_IDENTITY_REQUEST_GOODCRC_NOT_RCVD_SS);
              
                    gasPolicyEngine[u8PortNum].u8DiscoverIdentityCounter++;
                    u8IsTransmit = TRUE;
                    
                    gasPolicyEngine[u8PortNum].ePESubState = ePE_VDM_IDENTITY_REQUEST_IDLE_SS;                    
                    
                    break; 
                }
                case ePE_VDM_IDENTITY_REQUEST_IDLE_SS:
                {
                    /* Hook to notify PE state machine entry into idle sub-state */
                    MCHP_PSF_HOOK_NOTIFY_IDLE(u8PortNum, eIDLE_PE_NOTIFY);
                    
                    break;
                }
                case ePE_VDM_IDENTITY_REQUEST_MSG_DONE_SS:
                {					
                    DEBUG_PRINT_PORT_STR (u8PortNum,"PE_VDM_IDENTITY_REQUEST_MSG_DONE_SS\r\n");                                                            
                    
					/* Start the VDMIDentityRequest Sender Response timer, if timed out set the PE
						sub-state to ePE_VDM_IDENTITY_REQUEST_NO_RESPONSE_SS */
                    gasPolicyEngine[u8PortNum].u8PETimerID = PDTimer_Start (
                                                            (PE_VDMRESPONSE_TIMEOUT_MS),
                                                            PE_SSChngAndTimeoutValidate_TimerCB,u8PortNum,  
                                                            (UINT8)ePE_VDM_IDENTITY_REQUEST_NO_RESPONSE_SS);
                    
                    gasPolicyEngine[u8PortNum].ePESubState = ePE_VDM_IDENTITY_REQUEST_IDLE_SS;
                    
                    break;  
                }     
                case ePE_VDM_IDENTITY_REQUEST_GOODCRC_NOT_RCVD_SS:
                {
                    DEBUG_PRINT_PORT_STR (u8PortNum,"PE_VDM_IDENTITY_REQUEST_GOODCRC_NOT_RCVD_SS\r\n");                    
                    
                    /* When performing cable discovery during an Explicit Contract the 
                       Discover Identity Command request Shall be sent every tDiscoverIdentity.
                       No more than nDiscoverIdentityCount Discover Identity Messages without 
                       a GoodCRC Message response Shall be sent.*/                                          
                    if (PE_EXPLICIT_CONTRACT == PE_GET_PD_CONTRACT(u8PortNum))
                    {                        
                        /* When detach happens without GOODCRC being received for the 
                           SOP' Discover Identity, SOP' packet reception would have been 
                           enabled. So, on the next attach, the port will respond to the
                           SOP's packets sent by partner to the cable. To avoid this, 
                           disable SOP'/SOP'' packet reception */
                		UPD_RegWriteByte (u8PortNum, PRL_RX_CTL_B, 
                    		  (PRL_RX_CTL_B_PD3_AUTO_DECODE | PRL_RX_CTL_B_RX_SOP_ENABLE_SOP));
                                                
                        if (gasPolicyEngine[u8PortNum].u8DiscoverIdentityCounter < PE_N_DISCOVER_IDENTITY_COUNT)
                        {
                            /* Clear the internal event. It has to be initiated only after tDiscoverIdentity */
                            gasDPM[u8PortNum].u16DPMInternalEvents &= ~DPM_INT_EVT_DISCOVER_CABLE_IDENTITY;
                            
                            /* Update the Cable PD Spec Rev as 2.0 */
                            DPM_UpdatePDSpecRev (u8PortNum, PD_SPEC_REVISION_2_0, PRL_SOP_P_TYPE);   
                            
                            /* Start Discover Identity Timer */
                            gasDPM[u8PortNum].u8DiscoverIdentityTmrID = PDTimer_Start (
                                                                  (PE_DISCOVER_IDENTITY_TIMEOUT_MS),
                                                                  DPM_DiscoverIdentity_TimerCB, u8PortNum,  
                                                                  (UINT8)SET_TO_ZERO);                    
                        }        
                        else
                        {
                            /* Good_CRC not received from cable even after PE_N_DISCOVER_IDENTITY_COUNT times.
                               Hence, considering the cable as Non-PD capable */
                            gasDPM[u8PortNum].u32DPMStatus &= ~DPM_CABLE_DISCOVERY_STS_MASK;
                            gasDPM[u8PortNum].u32DPMStatus |= (DPM_CBL_DISCOVERED_AS_NON_PD_CAPABLE << DPM_CABLE_DISCOVERY_STS_POS);
                            
                            /* Reset Discover Identity Counter to 0 */
                            gasPolicyEngine[u8PortNum].u8DiscoverIdentityCounter = RESET_TO_ZERO;
                            
                            /* Post eMCHP_PSF_CABLE_IDENTITY_NAKED notification */
                            (void)DPM_NotifyClient (u8PortNum, eMCHP_PSF_CABLE_IDENTITY_NAKED);                                                      
                        }                        
                                                
                        /* Move to ePE_SRC_READY/ePE_SNK_READY state */
                        gasPolicyEngine[u8PortNum].ePEState = eTxDoneSt; 
                        gasPolicyEngine[u8PortNum].ePESubState = eTxDoneSS;                           
                    }
                    else
                    {
                        gasPolicyEngine[u8PortNum].ePEState = ePE_VDM_IDENTITY_NAKED;
                    }
                    break; 
                }
                case ePE_VDM_IDENTITY_REQUEST_NO_RESPONSE_SS:
                {
                    DEBUG_PRINT_PORT_STR (u8PortNum,"PE_VDM_IDENTITY_REQUEST_NO_RESPONSE_SS\r\n");
                    
                    if (PE_EXPLICIT_CONTRACT == PE_GET_PD_CONTRACT(u8PortNum))
                    {           
                        /* Update DPM Cable Discovery status as Non PD Capable */
                        gasDPM[u8PortNum].u32DPMStatus &= ~DPM_CABLE_DISCOVERY_STS_MASK;
                        gasDPM[u8PortNum].u32DPMStatus |= (DPM_CBL_DISCOVERED_AS_NON_PD_CAPABLE << DPM_CABLE_DISCOVERY_STS_POS);
                        
                        /* Move to ePE_SRC_READY/ePE_SNK_READY state */
                        gasPolicyEngine[u8PortNum].ePEState = eTxDoneSt; 
                        gasPolicyEngine[u8PortNum].ePESubState = eTxDoneSS;                         
                        
                        /* Post eMCHP_PSF_CABLE_IDENTITY_NAKED notification */
                        (void)DPM_NotifyClient (u8PortNum, eMCHP_PSF_CABLE_IDENTITY_NAKED);                          
                    }
                    else
                    {
                        gasPolicyEngine[u8PortNum].ePEState = ePE_VDM_IDENTITY_NAKED; 
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
                /************ PE_VDM_IDENTITY_ACKED ****************/
        case ePE_VDM_IDENTITY_ACKED:
        {
            DEBUG_PRINT_PORT_STR (u8PortNum,"PE_VDM_IDENTITY_ACKED\r\n");                        
            
            if (DPM_VDM_ACK == DPM_StoreCableIdentity (u8PortNum, (UINT16) u32Header, (UINT32*) pu8DataBuf))
            {
                /* A PD-Capable Cable Plug Shall return a Discover Identity Command ACK in 
                   response to a Discover Identity Command request sent to SOP */
                gasDPM[u8PortNum].u32DPMStatus &= ~DPM_CABLE_DISCOVERY_STS_MASK;
                gasDPM[u8PortNum].u32DPMStatus |= (DPM_CBL_DISCOVERED_AS_PD_CAPABLE << DPM_CABLE_DISCOVERY_STS_POS);
                
                /* Reset Discover Identity Counter to 0 */
                gasPolicyEngine[u8PortNum].u8DiscoverIdentityCounter = RESET_TO_ZERO;
                
                if (PE_EXPLICIT_CONTRACT == PE_GET_PD_CONTRACT(u8PortNum))
                {                    
                    /* Move to ePE_SRC_READY/ePE_SNK_READY state */
                    gasPolicyEngine[u8PortNum].ePEState = eTxDoneSt; 
                    gasPolicyEngine[u8PortNum].ePESubState = eTxDoneSS;
                }
                else
                {                                        
                    gasPolicyEngine[u8PortNum].ePEState = ePE_SRC_SEND_CAPABILITIES;
                    gasPolicyEngine[u8PortNum].ePESubState = ePE_SRC_SEND_CAP_ENTRY_SS;
                }
                
                /* Post eMCHP_PSF_CABLE_IDENTITY_DISCOVERED notification */
                (void)DPM_NotifyClient (u8PortNum, eMCHP_PSF_CABLE_IDENTITY_DISCOVERED);
            }            
            else /* NAK response */ 
            {
                if (PE_EXPLICIT_CONTRACT == PE_GET_PD_CONTRACT(u8PortNum))
                {
                    /* Move to ePE_SRC_READY/ePE_SNK_READY state */
                    gasPolicyEngine[u8PortNum].ePEState = eTxDoneSt; 
                    gasPolicyEngine[u8PortNum].ePESubState = eTxDoneSS;
                    
                    /* Post eMCHP_PSF_CABLE_IDENTITY_NAKED notification */
                    (void)DPM_NotifyClient (u8PortNum, eMCHP_PSF_CABLE_IDENTITY_NAKED);                                    
                }
                else
                {
                    gasPolicyEngine[u8PortNum].ePEState = ePE_VDM_IDENTITY_NAKED;
                }
            }                        
            break; 
        }
                /************ PE_VDM_IDENTITY_NAKED ****************/
        case ePE_VDM_IDENTITY_NAKED:
        {
            DEBUG_PRINT_PORT_STR (u8PortNum,"PE_VDM_IDENTITY_NAKED\r\n");
            
            /* VDM NAK received */
			/* If DiscoverIdentityCounter reaches nDiscoverIdentityCount change the PE state to	
				PE_SRC_SEND_CAPABILITIES */
            if (gasPolicyEngine[u8PortNum].u8DiscoverIdentityCounter < PE_N_DISCOVER_IDENTITY_COUNT)
            {
                /* Update the Cable PD Spec Rev as 2.0 */
                DPM_UpdatePDSpecRev (u8PortNum, PD_SPEC_REVISION_2_0, PRL_SOP_P_TYPE);            	                
                
                gasPolicyEngine[u8PortNum].ePEState = ePE_VDM_IDENTITY_REQUEST;
                gasPolicyEngine[u8PortNum].ePESubState = ePE_VDM_IDENTITY_REQUEST_ENTRY_SS;                                
            }       
            else
            {                                
                gasPolicyEngine[u8PortNum].u8PEPortSts |= PE_CABLE_RESPOND_NAK;
                
                /* Update DPM Cable Discovery status as Non PD Capable */
                gasDPM[u8PortNum].u32DPMStatus &= ~DPM_CABLE_DISCOVERY_STS_MASK;
                gasDPM[u8PortNum].u32DPMStatus |= (DPM_CBL_DISCOVERED_AS_NON_PD_CAPABLE << DPM_CABLE_DISCOVERY_STS_POS);
                
                /* Reset Discover Identity Counter to 0 */
                gasPolicyEngine[u8PortNum].u8DiscoverIdentityCounter = RESET_TO_ZERO;                

                gasPolicyEngine[u8PortNum].ePEState = ePE_SRC_SEND_CAPABILITIES;
                gasPolicyEngine[u8PortNum].ePESubState = ePE_SRC_SEND_CAP_ENTRY_SS;                         
                
                /* Post eMCHP_PSF_CABLE_IDENTITY_NAKED notification */
                (void)DPM_NotifyClient (u8PortNum, eMCHP_PSF_CABLE_IDENTITY_NAKED);                
            }                        
            break; 
        }
        
#endif 
       /**************** Soft Reset Reception State ********************/ 
        case ePE_SOFT_RESET:
        {
            /* To save code size, Soft Reset handling is kept common for
              both Source and Sink */
            switch (gasPolicyEngine[u8PortNum].ePESubState)
            {
                case ePE_SOFT_RESET_ENTRY_SS:
                {
                    DEBUG_PRINT_PORT_STR (u8PortNum,"PE_SOFT_RESET_ENTRY_SS\r\n");
                    
                    /* Kill Policy Engine Timer */
                    PE_KillPolicyEngineTimer (u8PortNum);

					/* Reset Protocol Layer */
                    PRL_ResetPRLSpecificSOP (u8PortNum, PRL_SOP_TYPE);

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
       /**************** Soft Reset Transmission State ********************/ 
       case ePE_SEND_SOFT_RESET:
       {
           switch (gasPolicyEngine[u8PortNum].ePESubState)
           {
               case ePE_SEND_SOFT_RESET_ENTRY_SS:
               {
                    DEBUG_PRINT_PORT_STR (u8PortNum,"PE_SEND_SOFT_RESET_SOP_SS\r\n");
                   
                    /* Kill Policy Engine Timer */
                    PE_KillPolicyEngineTimer (u8PortNum);
                   
					/* Reset the Protocol Layer for SOP type message */
                    PRL_ResetPRLSpecificSOP (u8PortNum, (UINT8) PRL_SOP_TYPE);
                    
					/* Send SoftReset message */
                    u32TransmitHeader = PRL_FormSOPTypeMsgHeader (u8PortNum, PE_CTRL_SOFT_RESET, \
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
                    DEBUG_PRINT_PORT_STR (u8PortNum,"PE_SEND_SOFT_RESET_MSG_DONE_SS\r\n");
                    
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
       /**************** Not Supported Transmission State ********************/ 
       case ePE_SEND_NOT_SUPPORTED:
       {
            switch (gasPolicyEngine[u8PortNum].ePESubState)
            {
                case ePE_SEND_NOT_SUPPORTED_ENTRY_SS:
                {
                    DEBUG_PRINT_PORT_STR (u8PortNum,"PE_SEND_NOT_SUPPORTED_ENTRY_SS\r\n");

                    u32TransmitHeader = PRL_FormSOPTypeMsgHeader (u8PortNum, PE_CTRL_NOT_SUPPORTED, \
                                                    PE_OBJECT_COUNT_0, PE_NON_EXTENDED_MSG);
                    u32pTransmitDataObj = NULL;

                    /*Set the transmitter callback to transition to source soft reset state if
                    message transmission fails*/
                    u32TransmitTmrIDTxSt = PRL_BUILD_PKD_TXST_U32 (eTxDoneSt, \
                                                eTxDoneSS, ePE_SEND_SOFT_RESET,\
                                                ePE_SEND_SOFT_RESET_ENTRY_SS);

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
       /**************** Reject Transmission State ********************/ 
       case ePE_SEND_REJECT:
       {           
            switch (gasPolicyEngine[u8PortNum].ePESubState)
            {
                case ePE_SEND_REJECT_ENTRY_SS:
                {
                    DEBUG_PRINT_PORT_STR (u8PortNum,"PE_SEND_REJECT_ENTRY_SS\r\n");

                    u32TransmitHeader = PRL_FormSOPTypeMsgHeader (u8PortNum, PE_CTRL_REJECT, \
                                                    PE_OBJECT_COUNT_0, PE_NON_EXTENDED_MSG);
                    u32pTransmitDataObj = NULL;

                    /*Set the transmitter callback to transition to source soft reset state if
                    message transmission fails*/
                    u32TransmitTmrIDTxSt = PRL_BUILD_PKD_TXST_U32 (ePE_SEND_REJECT, \
                                                ePE_SEND_REJECT_MSG_DONE_SS, ePE_SEND_SOFT_RESET,\
                                                ePE_SEND_SOFT_RESET_ENTRY_SS);
                    u8IsTransmit = TRUE;
                    gasPolicyEngine[u8PortNum].ePESubState = ePE_SEND_REJECT_IDLE_SS;

                    break;
                }
                case ePE_SEND_REJECT_MSG_DONE_SS:
                {
                    /* Move to ePE_SRC_READY/ePE_SNK_READY state */
                    gasPolicyEngine[u8PortNum].ePEState = eTxDoneSt; 
                    gasPolicyEngine[u8PortNum].ePESubState = eTxDoneSS;
                    
                    /* Post the pending notification */
                    if (gasDPM[u8PortNum].eDPMNotification)
                    {
                        (void) DPM_NotifyClient(u8PortNum, gasDPM[u8PortNum].eDPMNotification);
                        gasDPM[u8PortNum].eDPMNotification = (eMCHP_PSF_NOTIFICATION) RESET_TO_ZERO; 
                    }                      
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
                                             PRL_FormSOPTypeMsgHeader (u8PortNum,PE_EXT_FW_UPDATE_RESPONSE,7, /**Standard Msg Header*/
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
                    eSubState = ePE_SNK_READY_END_AMS_SS;     
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
               
        /**************** Give Capabilities State ********************/ 
        /* <ePE_GIVE_CAP>: This PE state can be entered when
           i.   a Dual Role Source Port receives Get Sink Caps message 
           ii.  a Dual Role Sink Port receives Get Source Caps message 
           iii. a Sink only Port receives Get Sink Caps message 
           Note: Scenarios iii is common for Sink only and DRP ports 
           It is handled in a generic manner in this state to reduce code size.
           In future, this same state can be used when a Source only port 
           receives Get Source Caps Extended message or a Sink only or Dual 
           Role Source/Sink Port receives a Get Sink Caps Extended message */

        case ePE_GIVE_CAP:
        {
            switch (gasPolicyEngine[u8PortNum].ePESubState)
            {
                case ePE_GIVE_CAP_ENTRY_SS:
                {             
                    if (PE_CTRL_GET_SOURCE_CAP == u8MessageType)
                    {
#if (TRUE == INCLUDE_PD_SOURCE)                         
                        DEBUG_PRINT_PORT_STR (u8PortNum,"PE_GIVE_CAP_ENTRY_SS:GET_SOURCE_CAP RCVD\r\n");
                                               
                        DPM_GetSourceCapabilities (u8PortNum, &u8DataObjCnt, u32aDataObj);                        

                        u32pTransmitDataObj = u32aDataObj;
                        
                        u32TransmitHeader = PRL_FormSOPTypeMsgHeader (u8PortNum, PE_DATA_SOURCE_CAP,\
                                                         u8DataObjCnt, PE_NON_EXTENDED_MSG);                                                                                       
#endif 
                    }         
#if (TRUE == INCLUDE_PD_SINK)                     
                    /*Request Device Policy Manager for the Capability Message*/
                    else if (PE_CTRL_GET_SINK_CAP == u8MessageType)
                    {                 
                        DEBUG_PRINT_PORT_STR (u8PortNum,"PE_GIVE_CAP_ENTRY_SS:GET_SINK_CAP RCVD\r\n");                        
                        
                        DPM_GetSinkCapabilities (u8PortNum, &u8DataObjCnt, u32aDataObj);                    
                                             
                        u32pTransmitDataObj = u32aDataObj;
                        
                        u32TransmitHeader = PRL_FormSOPTypeMsgHeader (u8PortNum, PE_DATA_SINK_CAP,\
                                                         u8DataObjCnt, PE_NON_EXTENDED_MSG);                                                               
                    }
                    else if (PE_CTRL_GET_SINK_CAP_EXTENDED == u8MessageType)
                    {
                        DEBUG_PRINT_PORT_STR (u8PortNum,"PE_GIVE_CAP_ENTRY_SS:GET_SINK_CAP_EXTD RCVD\r\n");                        
                        
                        u32pTransmitDataObj = (UINT32 *)&gasCfgStatusData.sPerPortData[u8PortNum].u8aSinkCapsExtd[INDEX_0];                          
                        
                        u32TransmitHeader =  /* Combined Message Header */
                            PRL_FORM_COMBINED_MSG_HEADER(((1u << PRL_EXTMSG_CHUNKED_BIT_POS) | (PRL_EXTMSG_DATA_FIELD_MASK & PE_SINK_CAP_EXTD_DATA_BLOCK_SIZE_IN_BYTES)), /* Extended Msg Header*/
                                    PRL_FormSOPTypeMsgHeader (u8PortNum,PE_EXT_SINK_CAPS_EXTD,PE_SINK_CAP_EXTD_DATA_DATA_OBJ_CNT, /* Standard Msg Header */
                                                PE_EXTENDED_MSG));                                                                         
                    }
#endif                    
                    else
                    {
                        /* Do Nothing */
                    }                                        
                    
                    /*Set the transmitter callback to transition to Soft reset state if
                    message transmission fails*/
                    u32TransmitTmrIDTxSt = PRL_BUILD_PKD_TXST_U32(eTxDoneSt,\
                                             eTxDoneSS,ePE_SEND_SOFT_RESET,\
                                             ePE_SEND_SOFT_RESET_ENTRY_SS);
                    u8IsTransmit = TRUE;
                    
                    gasPolicyEngine[u8PortNum].ePESubState = ePE_GIVE_CAP_IDLE_SS;
                                        
                    break;
                }
                case ePE_GIVE_CAP_IDLE_SS:
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
        /************ PE_GET_SINK_CAP *****************/ 
        case ePE_GET_SINK_CAP: 
        {
           switch (gasPolicyEngine[u8PortNum].ePESubState) 
           {
                case ePE_GET_SINK_CAP_ENTRY_SS:
                {
                    DEBUG_PRINT_PORT_STR (u8PortNum,"PE_GET_SINK_CAP_ENTRY_SS\r\n"); 
                    
                    u32TransmitHeader = PRL_FormSOPTypeMsgHeader (u8PortNum, PE_CTRL_GET_SINK_CAP, \
                                            PE_OBJECT_COUNT_0, PE_NON_EXTENDED_MSG);

                    u32pTransmitDataObj = NULL;               
                        
                    u32TransmitTmrIDTxSt = PRL_BUILD_PKD_TXST_U32(ePE_GET_SINK_CAP, ePE_GET_SINK_CAP_MSG_DONE_SS, \
                                                    ePE_SEND_SOFT_RESET, ePE_SEND_SOFT_RESET_ENTRY_SS);
                                                
                    u8IsTransmit = TRUE;                                             
                   
                    gasPolicyEngine[u8PortNum].ePESubState = ePE_GET_SINK_CAP_IDLE_SS;
                    
                    break; 
                }                   
                
                case ePE_GET_SINK_CAP_MSG_DONE_SS: 
                {
                    DEBUG_PRINT_PORT_STR (u8PortNum,"PE_GET_SINK_CAP_MSG_DONE_SS\r\n"); 
                    
                    /* Start Sender Response timer and Set the timer callback to transition to 
					ePE_GET_SINK_CAP_TIMER_TIMEDOUT sub state if timeout happens */
                    gasPolicyEngine[u8PortNum].u8PETimerID = PDTimer_Start (
                                                            PE_SENDERRESPONSE_TIMEOUT_MS,
                                                            PE_SubStateChange_TimerCB, u8PortNum,  
                                                            (UINT8)ePE_GET_SINK_CAP_NO_RESPONSE_SS); 

                    gasPolicyEngine[u8PortNum].ePESubState = ePE_GET_SINK_CAP_IDLE_SS; 

                    break; 
                }
                  
                case ePE_GET_SINK_CAP_NO_RESPONSE_SS:
                {
                   /* Sink caps not received within tSenderResponse. Send 
                      eMCHP_PSF_SINK_CAPS_NOT_RCVD notification and move to 
                      READY state */ 
                    DEBUG_PRINT_PORT_STR (u8PortNum,"PE_GET_SINK_CAP_NO_RESPONSE_SS\r\n"); 
                   
                    gasPolicyEngine[u8PortNum].ePEState = eTxDoneSt;
                    gasPolicyEngine[u8PortNum].ePESubState = eTxDoneSS;                                        
                   
                    (void)DPM_NotifyClient (u8PortNum, eMCHP_PSF_SINK_CAPS_NOT_RCVD);
                   
                    break;  
                }   
                
                case ePE_GET_SINK_CAP_RESPONSE_RCVD_SS:
                {
                    DEBUG_PRINT_PORT_STR (u8PortNum,"PE_GET_SINK_CAP_RESPONSE_RCVD_SS\r\n"); 
                    /* Store the received sink caps and send notification */                                               
                    gasCfgStatusData.sPerPortData[u8PortNum].u8PartnerSinkPDOCnt = PRL_GET_OBJECT_COUNT(u32Header);
    
                    (void)MCHP_PSF_HOOK_MEMCPY(gasCfgStatusData.sPerPortData[u8PortNum].u32aPartnerSinkPDO, pu8DataBuf,
                                 (gasCfgStatusData.sPerPortData[u8PortNum].u8PartnerSinkPDOCnt * BYTE_LEN_4));           
                    
                    /* Move to Ready state */
                    gasPolicyEngine[u8PortNum].ePEState = eTxDoneSt;
                    gasPolicyEngine[u8PortNum].ePESubState = eTxDoneSS;  
                    
                    (void)DPM_NotifyClient (u8PortNum, eMCHP_PSF_SINK_CAPS_RCVD);
                    
                    break; 
                }
                
                case ePE_GET_SINK_CAP_IDLE_SS: 
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

void PE_SSChngAndTimeoutValidate_TimerCB (UINT8 u8PortNum, UINT8 u8PESubState)
{
    gasPolicyEngine[u8PortNum].u32TimeoutMsgHeader = PRL_IsAnyMsgPendinginPRL (u8PortNum);

    /* Check for Msg Header is NULL */
    if (SET_TO_ZERO == gasPolicyEngine[u8PortNum].u32TimeoutMsgHeader)
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

/*******************************************************************************/
UINT8 PE_IsPolicyEngineIdle (UINT8 u8PortNum)
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

void PE_HandleUnsupportedVDM (UINT8 u8PortNum)
{
    /*Send "Not Supported" message for any received VDM message if PD Spec revision is 3.0
     If the Spec Rev is 2.0, ignore the VDM */    
    if (PD_SPEC_REVISION_3_0 == DPM_GET_CURRENT_PD_SPEC_REV (u8PortNum))
    {
        gasPolicyEngine[u8PortNum].ePEState = ePE_SEND_NOT_SUPPORTED;
        gasPolicyEngine[u8PortNum].ePESubState = ePE_SEND_NOT_SUPPORTED_ENTRY_SS;                    
    }
    else
    {
        if (PD_ROLE_SOURCE == DPM_GET_CURRENT_POWER_ROLE(u8PortNum))
        {
           gasPolicyEngine[u8PortNum].ePEState = ePE_SRC_READY;
           gasPolicyEngine[u8PortNum].ePESubState = ePE_SRC_READY_END_AMS_SS;
        }
        else
        {
           gasPolicyEngine[u8PortNum].ePEState = ePE_SNK_READY;
           gasPolicyEngine[u8PortNum].ePESubState = ePE_SNK_READY_END_AMS_SS;                                        
        }                                    
    }                                
}

