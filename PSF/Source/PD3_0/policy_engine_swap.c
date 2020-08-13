/*******************************************************************************
  PD Policy Engine swap Source file

  Company:
    Microchip Technology Inc.

  File Name:
    policy_engine_swap.c

  Description:
    This file contains the function definitions for Swap Policy Engine.
 *******************************************************************************/
/*******************************************************************************
Copyright ©  [2020] Microchip Technology Inc. and its subsidiaries.

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
#if (TRUE == INCLUDE_PD_DR_SWAP) 
void PE_RunDRSwapStateMachine(UINT8 u8PortNum)
{
	/* Transmit Message Header */
	UINT32 u32TransmitHeader = SET_TO_ZERO;

	/* Transmit Call back */
	PRLTxCallback pfnTransmitCB = PE_StateChange_TransmitCB;

	/* Transmit Call back variables */
	UINT32 u32TransmitTmrIDTxSt = SET_TO_ZERO;

	/* Transmit Flag */
	UINT8 u8IsTransmit = FALSE;
    
    ePolicyState eTxDoneSt, eTxFailedSt;
    ePolicySubState eTxDoneSS, eTxFailedSS;
    
    if (PD_ROLE_SOURCE == DPM_GET_CURRENT_POWER_ROLE(u8PortNum))
    {
        eTxDoneSt = ePE_SRC_READY;
        eTxDoneSS = ePE_SRC_READY_END_AMS_SS;
        eTxFailedSt = ePE_SRC_SEND_SOFT_RESET;
        eTxFailedSS = ePE_SRC_SEND_SOFT_RESET_SOP_SS;        
    }
    else
    {
        eTxDoneSt = ePE_SNK_READY;
        eTxDoneSS = ePE_SNK_READY_IDLE_SS;
        eTxFailedSt = ePE_SNK_SEND_SOFT_RESET;
        eTxFailedSS = ePE_SNK_SEND_SOFT_RESET_ENTRY_SS;
    }
    
   switch(gasPolicyEngine[u8PortNum].ePEState)
   {
        case ePE_DRS_EVALUATE_SWAP:
        {
            /* Inform DR_SWAP request from port partner has been received notification*/
            (void)DPM_NotifyClient (u8PortNum, eMCHP_PSF_DR_SWAP_RCVD);
            /*Evaluate swap through DPM function*/
            if (DPM_ACCEPT_SWAP == DPM_EvaluateRoleSwap(u8PortNum, eDR_SWAP_RCVD))
            {
                gasPolicyEngine[u8PortNum].ePEState = ePE_DRS_ACCEPT_SWAP; 
                gasPolicyEngine[u8PortNum].ePESubState = ePE_DRS_ACCEPT_SWAP_SEND_ACCEPT_SS;           
            }
            else
            {
                /* Reject is sent through common PE state machine*/
                gasPolicyEngine[u8PortNum].ePEState = ePE_SEND_REJECT; 
                gasPolicyEngine[u8PortNum].ePESubState = ePE_SEND_REJECT_ENTRY_SS;                         
            }
            break;
            
        }
        case ePE_DRS_ACCEPT_SWAP:
        {
            switch(gasPolicyEngine[u8PortNum].ePESubState)
            {
                case ePE_DRS_ACCEPT_SWAP_SEND_ACCEPT_SS:
                {
                    /*SOP type is hard coded to SOP and Data object pointer to NULL
                     and Transmit call back to PE_StateChange_TransmitCB*/
                    /* Send the Accept message */
                    u32TransmitHeader = PRL_FormSOPTypeMsgHeader (u8PortNum, PE_CTRL_ACCEPT,
                                            PE_OBJECT_COUNT_0, PE_NON_EXTENDED_MSG);
                    
                    /*If GoodCRC is not received, Soft reset state is set depending on
                     Power role*/
                    u32TransmitTmrIDTxSt = PRL_BUILD_PKD_TXST_U32( ePE_DRS_DFP_UFP_ROLE_CHANGE , \
                                                NULL, \
                                               eTxFailedSt, eTxFailedSS);

                    u8IsTransmit = TRUE;        
                    /*Wait in a idle state to get a Good_CRC response for Accept*/
                    gasPolicyEngine[u8PortNum].ePESubState = ePE_DRS_ACCEPT_SWAP_IDLE_SS;
                    break;
                }
                case ePE_DRS_ACCEPT_SWAP_IDLE_SS:
                {
                    /*Idle state*/
                    break;
                }
                default:
                {
                    break;
                }
            }
            break;
        }
        case ePE_DRS_DFP_UFP_ROLE_CHANGE:
        {
            UINT8 u8CurrentDataRole = DPM_GET_CURRENT_DATA_ROLE(u8PortNum);
            /*Change the present role*/
            if (PD_ROLE_DFP == u8CurrentDataRole)
            {
                DPM_UpdateDataRole(u8PortNum, PD_ROLE_UFP);
            }
            else if (PD_ROLE_UFP == u8CurrentDataRole)
            {
                DPM_UpdateDataRole(u8PortNum, PD_ROLE_DFP);
            }
            else
            {
                /*Do nothing*/
            }
            /*Inform Protocol layer of the role change*/
            PRL_UpdateSpecAndDeviceRoles (u8PortNum);
            
            /* Move the Policy Engine to ePE_SRC_READY/ePE_SNK_READY state */
            gasPolicyEngine[u8PortNum].ePEState = eTxDoneSt; 
            gasPolicyEngine[u8PortNum].ePESubState = eTxDoneSS;

            /* Inform DR_SWAP completion notification*/
            (void)DPM_NotifyClient (u8PortNum, eMCHP_PSF_DR_SWAP_COMPLETED);
            
            break;
        }
       case ePE_DRS_SEND_SWAP:
       {
            switch(gasPolicyEngine[u8PortNum].ePESubState)
            {
                case ePE_DRS_SEND_SWAP_ENTRY_SS:
                {
                    /* Send the Reject message */
                    u32TransmitHeader = PRL_FormSOPTypeMsgHeader (u8PortNum, PE_CTRL_DR_SWAP,
                                            PE_OBJECT_COUNT_0, PE_NON_EXTENDED_MSG);
      
                    u32TransmitTmrIDTxSt = PRL_BUILD_PKD_TXST_U32( ePE_DRS_SEND_SWAP , \
                                                ePE_DRS_SEND_SWAP_GOOD_CRC_RCVD_SS, \
                                               eTxFailedSt, eTxFailedSS);

                    u8IsTransmit = TRUE;
                    
                    /*Wait in a idle state to get a response for DR_SWAP*/
                    gasPolicyEngine[u8PortNum].ePESubState = ePE_DRS_SEND_SWAP_IDLE_SS;
                   
                    break;
                }
                case ePE_DRS_SEND_SWAP_GOOD_CRC_RCVD_SS:
                {
                    /* Start Sender Response Timer */
                    gasPolicyEngine[u8PortNum].u8PETimerID = PDTimer_Start (
                                                            (PE_SENDERRESPONSE_TIMEOUT_MS),
                                                            PE_SubStateChange_TimerCB,u8PortNum,  
                                                            (UINT8)ePE_DRS_SEND_SWAP_NO_RESPONSE_SS);
                    
                    /*Wait in a idle state for a response*/
                    gasPolicyEngine[u8PortNum].ePESubState = ePE_DRS_SEND_SWAP_IDLE_SS;                                            
                    break; 
                }
                case ePE_DRS_SEND_SWAP_NO_RESPONSE_SS:
                {
                    /* Response not received within tSenderResponse. Assign
                     * PE_SRC_READY/PE_SNK_READY state*/
                    gasPolicyEngine[u8PortNum].ePEState = eTxDoneSt; 
                    gasPolicyEngine[u8PortNum].ePESubState = eTxDoneSS;
                    /* Notify no response has been received for the DR_SWAP message sent*/
                    (void)DPM_NotifyClient (u8PortNum, eMCHP_PSF_DR_SWAP_NO_RESPONSE_RCVD);
                    break; 
                }
                case ePE_DRS_SEND_SWAP_WAIT_RCVD_SS:
                {
                    /* Start Wait timer*/
                    gasDPM[u8PortNum].u8DRSwapWaitTmrID = PDTimer_Start (
                                                          (PE_DR_SWAP_WAIT_TIMEOUT_MS),
                                                          DPM_SwapWait_TimerCB,u8PortNum,  
                                                          (UINT8)eDR_SWAP_INITIATE);
                    /*PE_SRC_READY/PE_SNK_READY state is set*/
                    gasPolicyEngine[u8PortNum].ePEState = eTxDoneSt; 
                    gasPolicyEngine[u8PortNum].ePESubState = eTxDoneSS;
                                 
                    break; 
                }
                case ePE_DRS_SEND_SWAP_REJECT_RCVD_SS:
                {
                    /*SRC_READY/SNK_READY state is set*/
                    gasPolicyEngine[u8PortNum].ePEState = eTxDoneSt; 
                    gasPolicyEngine[u8PortNum].ePESubState = eTxDoneSS;
                    /* Notify DR_SWAP is completed*/
                    (void)DPM_NotifyClient (u8PortNum, eMCHP_PSF_DR_SWAP_COMPLETED);
                    break;
                }
                case ePE_DRS_SEND_SWAP_IDLE_SS:
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
   
   /* Transmit the message if u8IsTransmit is set */
    if (TRUE == u8IsTransmit)
    {
		(void) PRL_TransmitMsg (u8PortNum, (UINT8) PRL_SOP_TYPE, u32TransmitHeader, \
                    (UINT8 *)NULL, pfnTransmitCB, u32TransmitTmrIDTxSt); 
    }
}
#endif /*INCLUDE_PD_DR_SWAP*/

#if (TRUE == INCLUDE_PD_PR_SWAP)

void PE_RunPRSwapStateMachine (UINT8 u8PortNum)
{
	/* Transmit Message Header */
	UINT32 u32TransmitHeader = SET_TO_ZERO;

	/* Transmit Call back set to PE_StateChange_TransmitCB */
	PRLTxCallback pfnTransmitCB = PE_StateChange_TransmitCB;

	/* Transmit Call back variables */
	UINT32 u32TransmitTmrIDTxSt = SET_TO_ZERO;

	/* Transmit Flag */
	UINT8 u8IsTransmit = FALSE;
    
    ePolicyState eTxDoneSt, eTxFailedSt;
    ePolicySubState eTxDoneSS, eTxFailedSS;
    
    if (PD_ROLE_SOURCE == DPM_GET_CURRENT_POWER_ROLE(u8PortNum))
    {
        eTxDoneSt = ePE_SRC_READY;
        eTxDoneSS = ePE_SRC_READY_END_AMS_SS;
        eTxFailedSt = ePE_SRC_SEND_SOFT_RESET;
        eTxFailedSS = ePE_SRC_SEND_SOFT_RESET_SOP_SS;        
    }
    else
    {
        eTxDoneSt = ePE_SNK_READY;
        eTxDoneSS = ePE_SNK_READY_IDLE_SS;
        eTxFailedSt = ePE_SNK_SEND_SOFT_RESET;
        eTxFailedSS = ePE_SNK_SEND_SOFT_RESET_ENTRY_SS;
    }
        
    switch(gasPolicyEngine[u8PortNum].ePEState)
    {
        case ePE_PRS_SEND_SWAP:
        {
            switch(gasPolicyEngine[u8PortNum].ePESubState)
            {
                case ePE_PRS_SEND_SWAP_ENTRY_SS:
                {                 
                    DEBUG_PRINT_PORT_STR (u8PortNum,"ePE_PRS_SEND_SWAP_ENTRY_SS\r\n");                    
					/* Send the PR_Swap message */
                    u32TransmitHeader = PRL_FormSOPTypeMsgHeader (u8PortNum, PE_CTRL_PR_SWAP,
                                            PE_OBJECT_COUNT_0, PE_NON_EXTENDED_MSG);
      
                    u32TransmitTmrIDTxSt = PRL_BUILD_PKD_TXST_U32( ePE_PRS_SEND_SWAP, \
                                                ePE_PRS_SEND_SWAP_MSG_DONE_SS, \
                                                eTxFailedSt, eTxFailedSS);

                    u8IsTransmit = TRUE;                                        
                             
                    /* Assign an idle sub-state to wait for message transmit completion */
                    gasPolicyEngine[u8PortNum].ePESubState = ePE_PRS_SEND_SWAP_IDLE_SS;                    
                    break; 
                }
                case ePE_PRS_SEND_SWAP_MSG_DONE_SS:
                {                   
                    /* Policy Engine would enter this sub-state on reception of GoodCRC 
                       for the PR_Swap message sent */
                    DEBUG_PRINT_PORT_STR (u8PortNum,"ePE_PRS_SEND_SWAP_MSG_DONE_SS\r\n");
                    /* Start Sender Response Timer. Either Accept, Reject or wait would be 
                       received as response. If no response is received, move to 
                       ePE_SRC_READY or ePE_SNK_READY state */
                    gasPolicyEngine[u8PortNum].u8PETimerID = PDTimer_Start (
                                                            (PE_SENDERRESPONSE_TIMEOUT_MS),
                                                            PE_SubStateChange_TimerCB,u8PortNum,  
                                                            (UINT8)ePE_PRS_SEND_SWAP_NO_RESPONSE_SS);
                    
                    /* Assign an idle sub-state to wait for timer expiry */
                    gasPolicyEngine[u8PortNum].ePESubState = ePE_PRS_SEND_SWAP_IDLE_SS;                                            
                    break; 
                }
                case ePE_PRS_SEND_SWAP_NO_RESPONSE_SS:
                {                    
                    /* Response not received within tSenderResponse. Move to 
                       ePE_SRC_READY/ePE_SNK_READY state */
                    gasPolicyEngine[u8PortNum].ePEState = eTxDoneSt; 
                    gasPolicyEngine[u8PortNum].ePESubState = eTxDoneSS;

                    /* Post PR_SWAP_NO_RESP_RCVD notification*/
                    (void)DPM_NotifyClient (u8PortNum, eMCHP_PSF_PR_SWAP_NO_RESPONSE_RCVD);

                    break; 
                }
                case ePE_PRS_SEND_SWAP_REJECT_RCVD_SS:
                {
                    /* Move to ePE_SRC_READY/ePE_SNK_READY state */
                    gasPolicyEngine[u8PortNum].ePEState = eTxDoneSt; 
                    gasPolicyEngine[u8PortNum].ePESubState = eTxDoneSS;
                    
                    /* Post PR_Swap complete notification as PR_Swap is rejected by partner */
                    (void)DPM_NotifyClient (u8PortNum, eMCHP_PSF_PR_SWAP_COMPLETE); 

                    break;
                }                
                case ePE_PRS_SEND_SWAP_WAIT_RCVD_SS:
                {
                    /* Policy Engine would enter this sub-state on reception of 
                       Wait response from the port partner. Start PR Swap Wait timer
                       and move to ePE_SRC_READY or ePE_SNK_READY state. On PR_Swap
                       wait timer expiry, re-initiate PR_Swap transmission if not 
                       done by the port partner */
                    gasDPM[u8PortNum].u8PRSwapWaitTmrID = PDTimer_Start (
                                                          (PE_PR_SWAP_WAIT_TIMEOUT_MS),
                                                          DPM_SwapWait_TimerCB,u8PortNum,  
                                                          (UINT8)ePR_SWAP_INITIATE);
                    /* Move to ePE_SRC_READY/ePE_SNK_READY state */
                    gasPolicyEngine[u8PortNum].ePEState = eTxDoneSt; 
                    gasPolicyEngine[u8PortNum].ePESubState = eTxDoneSS;
                                 
                    break; 
                }
                case ePE_PRS_SEND_SWAP_IDLE_SS:
                {
                    /* Idle state to wait for message transmit completion or timer expiry */
                    break; 
                }
                default:
                {
                    break; 
                }
            }
            break; 
        }
        
        case ePE_PRS_EVALUATE_SWAP:
        {
            DEBUG_PRINT_PORT_STR (u8PortNum,"ePE_PRS_EVALUATE_SWAP\r\n");
            
            /* Post PR_SWAP_RCVD notification*/
            (void)DPM_NotifyClient (u8PortNum, eMCHP_PSF_PR_SWAP_RCVD);

            /* Get evaluation of swap request from Device Policy Manager */
            if (DPM_ACCEPT_SWAP == DPM_EvaluateRoleSwap(u8PortNum, ePR_SWAP_RCVD))
            {
                gasPolicyEngine[u8PortNum].ePEState = ePE_PRS_ACCEPT_SWAP; 
                gasPolicyEngine[u8PortNum].ePESubState = ePE_PRS_ACCEPT_SWAP_ENTRY_SS;           
            }
            else
            {
                /* Use common state available for sending reject message */
                gasPolicyEngine[u8PortNum].ePEState = ePE_SEND_REJECT; 
                gasPolicyEngine[u8PortNum].ePESubState = ePE_SEND_REJECT_ENTRY_SS;                           
            }
            break; 
        }
        
        case ePE_PRS_ACCEPT_SWAP:
        {
            switch(gasPolicyEngine[u8PortNum].ePESubState)
            {
                case ePE_PRS_ACCEPT_SWAP_ENTRY_SS:
                {
                    DEBUG_PRINT_PORT_STR (u8PortNum,"ePE_PRS_ACCEPT_SWAP_ENTRY_SS\r\n");
					/* Send the Accept message */
                    u32TransmitHeader = PRL_FormSOPTypeMsgHeader (u8PortNum, PE_CTRL_ACCEPT,
                                            PE_OBJECT_COUNT_0, PE_NON_EXTENDED_MSG);
      
                    /* Move to Transition to Off state based on the current power role */
                    if (PD_ROLE_SOURCE == DPM_GET_CURRENT_POWER_ROLE(u8PortNum))
                    {
                        eTxDoneSt = ePE_PRS_SRC_SNK_TRANSITION_TO_OFF;
                        eTxDoneSS = ePE_PRS_SRC_SNK_TRANSITION_TO_OFF_ENTRY_SS;
                    }
                    else
                    {
                        eTxDoneSt = ePE_PRS_SNK_SRC_TRANSITION_TO_OFF;
                        eTxDoneSS = ePE_PRS_SNK_SRC_TRANSITION_TO_OFF_ENTRY_SS;
                    }
                    
                    u32TransmitTmrIDTxSt = PRL_BUILD_PKD_TXST_U32( eTxDoneSt, \
                                                eTxDoneSS, eTxFailedSt, eTxFailedSS);

                    u8IsTransmit = TRUE;
                    
                    /* Move the Policy engine to Idle state*/
                    gasPolicyEngine[u8PortNum].ePESubState = ePE_PRS_ACCEPT_SWAP_IDLE_SS;                    
                    break; 
                }
                case ePE_PRS_ACCEPT_SWAP_IDLE_SS:
                {
                    /* Idle state to wait for accept message transmit completion */
                    break; 
                }
                default:
                {
                    break; 
                }
            }
            break; 
        }
        /********************* Source-to-Sink PR_Swap States **************/
        case ePE_PRS_SRC_SNK_TRANSITION_TO_OFF:
        {
            switch(gasPolicyEngine[u8PortNum].ePESubState)
            {
                case ePE_PRS_SRC_SNK_TRANSITION_TO_OFF_ENTRY_SS:
                {
                    /* Policy Engine would enter this state on reception of Accept 
                       message from port partner or if GoodCRC has been received 
                       for the accept message sent */
                    DEBUG_PRINT_PORT_STR (u8PortNum,"ePE_PRS_SRC_SNK_TRANSITION_TO_OFF_ENTRY_SS\r\n");
                    /* Start Source transition timer - The time the Source Shall wait before 
                       transitioning the power supply to ensure that the Sink has sufficient time to prepare.*/
                    gasPolicyEngine[u8PortNum].u8PETimerID = PDTimer_Start (
                                                            (PE_SRCTRANSISTION_TIMEOUT_MS),
                                                            PE_SubStateChange_TimerCB,u8PortNum,  
                                                            (UINT8)ePE_PRS_SRC_SNK_TRANSITION_TO_OFF_PWROFF_SS);
                    
                    /* Assign an idle state to wait for timer expiry */
                    gasPolicyEngine[u8PortNum].ePESubState = ePE_PRS_SRC_SNK_TRANSITION_TO_OFF_IDLE_SS;                                                                
                    break; 
                }
                case ePE_PRS_SRC_SNK_TRANSITION_TO_OFF_PWROFF_SS:
                {
                    /* tSrcTransition after the GoodCRC Message was received the 
                       power supply starts to change its output power capability to 
                       Swap Standby*/
                    DEBUG_PRINT_PORT_STR (u8PortNum,"ePE_PRS_SRC_SNK_TRANSITION_TO_OFF_PWROFF_SS\r\n");
					/* Turn Off Power Supply by driving vSafe0V in VBUS */
                    DPM_TypeCSrcVBus5VOnOff(u8PortNum, DPM_VBUS_OFF);
                    
                     /*Start the VBUS OFF timer for monitoring the time taken for 
                       power module to reach vSafe0V*/
                    gasPolicyEngine[u8PortNum].u8PETimerID = PDTimer_Start (
                                                              (TYPEC_VBUS_OFF_TIMER_MS),
                                                              DPM_VBUSorVCONNOnOff_TimerCB, u8PortNum,  
                                                              (UINT8)SET_TO_ZERO);
                   
                    gasPolicyEngine[u8PortNum].ePESubState = ePE_PRS_SRC_SNK_TRANSITION_TO_OFF_EXIT_SS;                    
                    break; 
                }
                case ePE_PRS_SRC_SNK_TRANSITION_TO_OFF_EXIT_SS:
                {                              
                    if(TYPEC_VBUS_0V == DPM_GetVBUSVoltage(u8PortNum))
                    {
                        /* The Policy Engine determines its power supply is no longer 
                           supplying VBUS */
                        DEBUG_PRINT_PORT_STR (u8PortNum,"ePE_PRS_SRC_SNK_TRANSITION_TO_OFF_EXIT_SS\r\n");                        
                        /*Kill the VBUS Off timer since vSafe0V is reached*/
                        PE_KillPolicyEngineTimer (u8PortNum);
                        
                        /* Drive the DC_DC_EN pin to low since it is not needed for Sink functionality */
                        PWRCTRL_ConfigDCDCEn(u8PortNum, FALSE);
                        
                        /* Move the Policy Engine to ePE_PRS_SRC_SNK_ASSERT_RD state */
                        gasPolicyEngine[u8PortNum].ePEState = ePE_PRS_SRC_SNK_ASSERT_RD;                     
                    }                    
                    break; 
                }
                case ePE_PRS_SRC_SNK_TRANSITION_TO_OFF_IDLE_SS:
                {
                    /* Idle state to wait for Source transition timer expiry */
                    break; 
                }
                default:
                {
                    break; 
                }                
            }
            break; 
        }
        
        case ePE_PRS_SRC_SNK_ASSERT_RD:
        {
            /* Policy Engine enters this state when the source power supply 
               has been turned off. There are no sub-states involved in this state*/
            DEBUG_PRINT_PORT_STR (u8PortNum,"ePE_PRS_SRC_SNK_ASSERT_RD\r\n");
            /* The Policy Engine requests the Device Policy Manager to assert the Rd 
               pull down on the CC wire.*/
            DPM_SetTypeCState (u8PortNum, TYPEC_ATTACHED_SRC, TYPEC_ATTACHED_SRC_PRS_TRANS_TO_SNK_SS);
            
            gasPolicyEngine[u8PortNum].ePEState = ePE_PRS_SRC_SNK_WAIT_SOURCE_ON; 
            gasPolicyEngine[u8PortNum].ePESubState = ePE_PRS_SRC_SNK_WAIT_SOURCE_ON_SEND_PSRDY_SS;
            
            break; 
        }
        
        case ePE_PRS_SRC_SNK_WAIT_SOURCE_ON:
        {
            switch(gasPolicyEngine[u8PortNum].ePESubState)
            {
                case ePE_PRS_SRC_SNK_WAIT_SOURCE_ON_SEND_PSRDY_SS:
                {
                    if ((TYPEC_ATTACHED_SNK == gasTypeCcontrol[u8PortNum].u8TypeCState) && 
                            (TYPEC_ATTACHED_SNK_PRS_VBUS_PRES_DETECT_SS == gasTypeCcontrol[u8PortNum].u8TypeCSubState))
                    {
                        DEBUG_PRINT_PORT_STR (u8PortNum,"ePE_PRS_SRC_SNK_WAIT_SOURCE_ON_SEND_PSRDY_SS\r\n");
                        
                        /* The Policy Engine then directs the Protocol Layer to generate a 
                           PS_RDY Message, with the Port Power Role bit in the Message 
                           Header set to Sink, to tell its Port Partner that it 
                           can begin to Source VBUS. */  
                        DPM_UpdatePowerRole(u8PortNum, PD_ROLE_SINK); 
                        
                        PRL_UpdateSpecAndDeviceRoles (u8PortNum);
                        
                        u32TransmitHeader = PRL_FormSOPTypeMsgHeader (u8PortNum, PE_CTRL_PS_RDY, \
                                                PE_OBJECT_COUNT_0, PE_NON_EXTENDED_MSG);

                        u32TransmitTmrIDTxSt = PRL_BUILD_PKD_TXST_U32( ePE_PRS_SRC_SNK_WAIT_SOURCE_ON, ePE_PRS_SRC_SNK_WAIT_SOURCE_ON_MSG_DONE_SS, \
                                                        ePE_PRS_SRC_SNK_WAIT_SOURCE_ON, ePE_PRS_SRC_SNK_WAIT_SOURCE_ON_ERROR_SS);                     

                        u8IsTransmit = TRUE;
                        /* Move the Policy Engine to Idle sub-state */
                        gasPolicyEngine[u8PortNum].ePESubState = ePE_PRS_SRC_SNK_WAIT_SOURCE_ON_IDLE_SS;
                    }
                    break; 
                }
                case ePE_PRS_SRC_SNK_WAIT_SOURCE_ON_MSG_DONE_SS:
                {
                    DEBUG_PRINT_PORT_STR (u8PortNum,"ePE_PRS_SRC_SNK_WAIT_SOURCE_ON_MSG_DONE_SS\r\n");
                    
                    /* PS_RDY Message was successfully sent. At this point the 
                       Initial Source is ready to be the new Sink.
                       Start PSSourceOn timer */
                    gasPolicyEngine[u8PortNum].u8PETimerID = PDTimer_Start (
                                                            (PE_PS_SOURCE_ON_TIMEOUT_MS),
                                                            PE_SubStateChange_TimerCB,u8PortNum,  
                                                            (UINT8)ePE_PRS_SRC_SNK_WAIT_SOURCE_ON_ERROR_SS);
                    
                    /* Assign an idle state to wait for PS_RDY from Initial Sink */
                    gasPolicyEngine[u8PortNum].ePESubState = ePE_PRS_SRC_SNK_WAIT_SOURCE_ON_WAIT_FOR_PSRDY_SS;                                                                                    
                    break; 
                }
                case ePE_PRS_SRC_SNK_WAIT_SOURCE_ON_ERROR_SS:
                {
                    /* Policy Engine would enter this sub-state in any one of the 
                       following scenarios:
                       - PR_RDY not sent after retries 
                       - PSSourceOn timer expired */
                    DEBUG_PRINT_PORT_STR (u8PortNum,"ePE_PRS_SRC_SNK_WAIT_SOURCE_ON_ERROR_SS\r\n");
                    
                    /* Setting the u8PETimerID to MAX_CONCURRENT_TIMERS to indicate that
                        TimerID does not hold any valid timer IDs anymore */
                    gasPolicyEngine[u8PortNum].u8PETimerID = MAX_CONCURRENT_TIMERS;
                    
                    /* Revert the port's power role to Source and invoke Type C Error Recovery, 
                       so that Type C SM would move to Unattached Source state after Error 
                       recovery where the u8DRPLastAttachedState condition would pass and DRP 
                       Offload would be enabled */
                    DPM_UpdatePowerRole(u8PortNum, PD_ROLE_SOURCE);
                    
                    DPM_SetTypeCState(u8PortNum, TYPEC_ERROR_RECOVERY, TYPEC_ERROR_RECOVERY_ENTRY_SS);
                    
                    gasPolicyEngine[u8PortNum].ePEState = ePE_INVALIDSTATE;
                    gasPolicyEngine[u8PortNum].ePESubState = ePE_INVALIDSUBSTATE;

                    break; 
                }
                case ePE_PRS_SRC_SNK_WAIT_SOURCE_ON_IDLE_SS:
                {
                    /* Idle state to wait for PS_RDY message transmit completion */
                    break; 
                }
                case ePE_PRS_SRC_SNK_WAIT_SOURCE_ON_WAIT_FOR_PSRDY_SS:
                {
                    /* Idle state to wait for PS_RDY message from port partner */
                    break; 
                }
                case ePE_PRS_SRC_SNK_WAIT_SOURCE_ON_PSRDY_RCVD_SS:
                {
                    /* Policy Engine would enter this sub-state when 
                       PS_RDY is received from port partner within tPSSourceOn */                    
                    
                    /* Move to Sink Startup state after vSafe5V is hit and Type C sink SM 
                       has reached the TYPEC_ATTACHED_SNK_RUN_SM_SS sub-state */
                    if ((TYPEC_ATTACHED_SNK == gasTypeCcontrol[u8PortNum].u8TypeCState) && 
                            (TYPEC_ATTACHED_SNK_RUN_SM_SS == gasTypeCcontrol[u8PortNum].u8TypeCSubState))
                    {
                        DEBUG_PRINT_PORT_STR (u8PortNum,"ePE_PRS_SRC_SNK_WAIT_SOURCE_ON_PSRDY_RCVD_SS\r\n");
                        
                        /* Resetting the Protocol Layer would be taken care by the 
                        ePE_SNK_STARTUP state */
                        gasPolicyEngine[u8PortNum].ePEState = ePE_SNK_STARTUP;
                        
                        /* Send PR_Swap complete notification */
                        (void)DPM_NotifyClient (u8PortNum, eMCHP_PSF_PR_SWAP_COMPLETE);                                         
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
        
        /********************* Sink-to-Source PR_Swap States **************/
        case ePE_PRS_SNK_SRC_TRANSITION_TO_OFF:
        {
            switch(gasPolicyEngine[u8PortNum].ePESubState)
            {
                case ePE_PRS_SNK_SRC_TRANSITION_TO_OFF_ENTRY_SS:
                {
                    DEBUG_PRINT_PORT_STR (u8PortNum,"ePE_PRS_SNK_SRC_TRANSITION_TO_OFF_ENTRY_SS\r\n");
                    /* Set PR_Swap in progress Flag. This would prevent PSF Sink from 
                       detach when VBUS drops below VSinkDisconnect.
                       PD Spec Reference Note: during the Power Role Swap process the 
                       initial Sink does not disconnect even though VBUS drops below vSafe5V */
                    gasPolicyEngine[u8PortNum].u8PEPortSts |= PE_PR_SWAP_IN_PROGRESS_MASK;
                                        
                    /* Transition to Sink Standby.
                       Configure the Type C VBUS threshold for vSafe0v detection */
                    gasDPM[u8PortNum].u16SinkOperatingCurrInmA = DPM_0mA; 
                    TypeC_ConfigureVBUSThr(u8PortNum, TYPEC_VBUS_0V,
                                gasDPM[u8PortNum].u16SinkOperatingCurrInmA, TYPEC_CONFIG_NON_PWR_FAULT_THR);
                    
                    /*Turn off the Sink circuitry to stop sinking the power from source*/
                    PWRCTRL_ConfigSinkHW (u8PortNum, TYPEC_VBUS_0V, gasDPM[u8PortNum].u16SinkOperatingCurrInmA);
                    
                    /* Initialize and run PSSourceOffTimer. 
                       Note: DPM_VBUSorVCONNOnOff_TimerCB API is reused for PSSourceOff 
                       timer call back intentionally, as both the time outs invoke
                       Error Recovery. This would save the usage of code memory.
                       DPM_CLR_PR_SWAP_IN_PROGRESS_MASK is passed as the argument for CB
                       so that PR_Swap In Progress mask would be cleared on Timeout */
                    gasPolicyEngine[u8PortNum].u8PETimerID = PDTimer_Start (
                                                            (PE_PS_SOURCE_OFF_TIMEOUT_MS),
                                                            DPM_VBUSorVCONNOnOff_TimerCB,u8PortNum,  
                                                            (UINT8)DPM_CLR_PR_SWAP_IN_PROGRESS_MASK);
               
                    /* Assign an idle state to wait for PS_RDY reception */
                    gasPolicyEngine[u8PortNum].ePESubState = ePE_PRS_SNK_SRC_TRANSITION_TO_OFF_WAIT_FOR_PSRDY_SS;                                                                                    
                    break;                     
                }
                case ePE_PRS_SNK_SRC_TRANSITION_TO_OFF_WAIT_FOR_PSRDY_SS:
                {
                    /* Idle state to wait for PS_RDY from original Source */
                    break; 
                }
                default:
                {
                    break; 
                }
            }
            break; 
        }
        
        case ePE_PRS_SNK_SRC_ASSERT_RP:
        {
            DEBUG_PRINT_PORT_STR (u8PortNum,"ePE_PRS_SNK_SRC_ASSERT_RP\r\n");
            /* Policy Engine enters this state when the sink circuitry has been 
               turned off and PS_RDY received from original source. There are no 
               sub-states involved in this state. 
               Request DPM to transition Type C Port role from Sink to Source */            
            DPM_SetTypeCState (u8PortNum, TYPEC_ATTACHED_SNK, TYPEC_ATTACHED_SNK_PRS_TRANS_TO_SRC_SS);             
            
            /* Once port role is transitioned to Source, send a PS_RDY message */
            gasPolicyEngine[u8PortNum].ePEState = ePE_PRS_SNK_SRC_SOURCE_ON; 
            gasPolicyEngine[u8PortNum].ePESubState = ePE_PRS_SNK_SRC_SOURCE_ON_SEND_PSRDY_SS;
            
            break; 
        }
        
        case ePE_PRS_SNK_SRC_SOURCE_ON:
        {
            switch(gasPolicyEngine[u8PortNum].ePESubState)
            {
                case ePE_PRS_SNK_SRC_SOURCE_ON_SEND_PSRDY_SS:
                {
                    if ((TYPEC_ATTACHED_SRC == gasTypeCcontrol[u8PortNum].u8TypeCState) && 
                            (TYPEC_ATTACHED_SRC_SET_PRL_SS == gasTypeCcontrol[u8PortNum].u8TypeCSubState))
                    {
                        DEBUG_PRINT_PORT_STR (u8PortNum,"ePE_PRS_SNK_SRC_SOURCE_ON_SEND_PSRDY_SS\r\n");
                        /*  Policy Engine, when its power supply is ready to supply power,
                            tells the Protocol Layer to form a PS_RDY Message */
                            
                        /*  The Port Power Role bit used in this and subsequent Message 
                            Headers is now set to 'Source' */                                                                        
                        PRL_UpdateSpecAndDeviceRoles (u8PortNum);
                        
                        u32TransmitHeader = PRL_FormSOPTypeMsgHeader (u8PortNum, PE_CTRL_PS_RDY, \
                                                PE_OBJECT_COUNT_0, PE_NON_EXTENDED_MSG);

                        u32TransmitTmrIDTxSt = PRL_BUILD_PKD_TXST_U32( ePE_PRS_SNK_SRC_SOURCE_ON, ePE_PRS_SNK_SRC_SOURCE_ON_MSG_DONE_SS , \
                                                        ePE_PRS_SNK_SRC_SOURCE_ON, ePE_PRS_SNK_SRC_SOURCE_ON_MSG_ERROR_SS);                     

                        u8IsTransmit = TRUE;
                        /* Move the Policy Engine to Idle sub-state */
                        gasPolicyEngine[u8PortNum].ePESubState = ePE_PRS_SNK_SRC_SOURCE_ON_IDLE_SS;
                    }                    
                    break; 
                }
                case ePE_PRS_SNK_SRC_SOURCE_ON_MSG_DONE_SS:
                {
                    DEBUG_PRINT_PORT_STR (u8PortNum,"ePE_PRS_SNK_SRC_SOURCE_ON_MSG_DONE_SS\r\n");
                    
                    /* Start SwapSourceStart timer which must timeout before sending 
                       any Source_Capabilities Message */
                    gasPolicyEngine[u8PortNum].u8PETimerID = PDTimer_Start (
                                                            (PE_SWAP_SOURCE_START_TIMEOUT_MS),
                                                            PE_SubStateChange_TimerCB,u8PortNum,  
                                                            (UINT8)ePE_PRS_SNK_SRC_SOURCE_ON_EXIT_SS);
                    
                    /* Assign an idle state to wait for timer expiry */
                    gasPolicyEngine[u8PortNum].ePESubState = ePE_PRS_SNK_SRC_SOURCE_ON_IDLE_SS;                                                                                                        
                    break; 
                }
                case ePE_PRS_SNK_SRC_SOURCE_ON_MSG_ERROR_SS:
                {
                    DEBUG_PRINT_PORT_STR (u8PortNum,"ePE_PRS_SNK_SRC_SOURCE_ON_MSG_ERROR_SS\r\n");
                    /* This sub-state would be entered if PS_Rdy message 
                       is not sent after retries */
                    /* Clear the PR_Swap In Progress Flag since PE would come out of Swap 
                       SM after this and this flag should not cause any issues during the normal 
                       flow */
                    gasPolicyEngine[u8PortNum].u8PEPortSts &= ~(PE_PR_SWAP_IN_PROGRESS_MASK);
                    
                    /* No need to revert the port role to Sink since gasTypeCcontrol[u8PortNum].u8DRPLastAttachedState 
                       would be PD_ROLE_SOURCE. After Error recovery, Type C SM would enter 
                       Unattached Source state and DRP offload would be enabled there.
                       If role is updated as Sink, then Type C SM would move to Unattached 
                       Sink after Error recovery where the u8DRPLastAttachedState condition would fail
                       and DRP Offload will not be enabled */
                    DPM_SetTypeCState(u8PortNum, TYPEC_ERROR_RECOVERY, TYPEC_ERROR_RECOVERY_ENTRY_SS);
                    
                    gasPolicyEngine[u8PortNum].ePEState = ePE_INVALIDSTATE;
                    gasPolicyEngine[u8PortNum].ePESubState = ePE_INVALIDSUBSTATE;
                    
                    break;
                }
                case ePE_PRS_SNK_SRC_SOURCE_ON_IDLE_SS:
                {
                    /* Idle state to wait for PS_RDY message transmit completion 
                       and to wait for SwapSourceStart timer expiry */
                    break; 
                }
                case ePE_PRS_SNK_SRC_SOURCE_ON_EXIT_SS:
                {
                    /* This sub-state would be entered when SwapSourceStart 
                       timer times out */                    
                    DEBUG_PRINT_PORT_STR (u8PortNum,"ePE_PRS_SNK_SRC_SOURCE_ON_EXIT_SS\r\n");
                    
                    /* Clear the PR_Swap in progress flag since the Swap is complete */
                    gasPolicyEngine[u8PortNum].u8PEPortSts &= ~(PE_PR_SWAP_IN_PROGRESS_MASK);
                    
                    /* Move the Policy Engine to PE_SRC_STARTUP state. Resetting the CapsCounter 
                       and Protocol Layer would be taken care by the startup state */
                    gasPolicyEngine[u8PortNum].ePEState = ePE_SRC_STARTUP;
                    gasPolicyEngine[u8PortNum].ePESubState = ePE_SRC_STARTUP_ENTRY_SS;
                    
                    /* Send PR_Swap complete notification */
                    (void)DPM_NotifyClient (u8PortNum, eMCHP_PSF_PR_SWAP_COMPLETE); 
                    
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
    
   /* Transmit the message if u8IsTransmit is set */
    if (TRUE == u8IsTransmit)
    {
		(void) PRL_TransmitMsg (u8PortNum, (UINT8) PRL_SOP_TYPE, u32TransmitHeader, \
                            NULL, pfnTransmitCB, u32TransmitTmrIDTxSt); 
    }    
}
#endif /*INCLUDE_PD_PR_SWAP*/

#if (TRUE == INCLUDE_PD_VCONN_SWAP)
void PE_RunVCONNSwapStateMachine (UINT8 u8PortNum)
{
	/* Transmit Message Header */
	UINT32 u32TransmitHeader = SET_TO_ZERO;

	/* Transmit Call back set to PE_StateChange_TransmitCB */
	PRLTxCallback pfnTransmitCB = PE_StateChange_TransmitCB;

	/* Transmit Call back variables */
	UINT32 u32TransmitTmrIDTxSt = SET_TO_ZERO;

	/* Transmit Flag */
	UINT8 u8IsTransmit = FALSE;
    
    ePolicyState eTxDoneSt, eTxFailedSt;
    ePolicySubState eTxDoneSS, eTxFailedSS,eTxHardRstSS;
    
    if (PD_ROLE_SOURCE == DPM_GET_CURRENT_POWER_ROLE(u8PortNum))
    {
        eTxDoneSt = ePE_SRC_READY;
        eTxDoneSS = ePE_SRC_READY_END_AMS_SS;
        eTxFailedSt = ePE_SRC_SEND_SOFT_RESET;
        eTxFailedSS = ePE_SRC_SEND_SOFT_RESET_SOP_SS; 
        eTxHardRstSS = ePE_SRC_HARD_RESET_ENTRY_SS;
    }
    else
    {
        eTxDoneSt = ePE_SNK_READY;
        eTxDoneSS = ePE_SNK_READY_IDLE_SS;
        eTxFailedSt = ePE_SNK_SEND_SOFT_RESET;
        eTxFailedSS = ePE_SNK_SEND_SOFT_RESET_ENTRY_SS;
        eTxHardRstSS = ePE_SNK_HARD_RESET_SEND_SS;
    }
        
    switch(gasPolicyEngine[u8PortNum].ePEState)
    {
        case ePE_VCS_SEND_SWAP:
        {
            switch(gasPolicyEngine[u8PortNum].ePESubState)
            {
                case ePE_VCS_SEND_SWAP_ENTRY_SS:
                {                 
                    DEBUG_PRINT_PORT_STR (u8PortNum,"ePE_VCS_SEND_SWAP_ENTRY_SS\r\n");                    
					/* Send the VCONN_Swap message */
                    u32TransmitHeader = PRL_FormSOPTypeMsgHeader (u8PortNum, PE_CTRL_VCONN_SWAP,
                                            PE_OBJECT_COUNT_0, PE_NON_EXTENDED_MSG);
      
                    u32TransmitTmrIDTxSt = PRL_BUILD_PKD_TXST_U32( ePE_VCS_SEND_SWAP, \
                                                ePE_VCS_SEND_SWAP_MSG_DONE_SS, \
                                                eTxFailedSt, eTxFailedSS);

                    u8IsTransmit = TRUE;                                        
                             
                    /* Assign an idle sub-state to wait for message transmit completion */
                    gasPolicyEngine[u8PortNum].ePESubState = ePE_VCS_SEND_SWAP_IDLE_SS;                    
                    break; 
                }
                case ePE_VCS_SEND_SWAP_MSG_DONE_SS:
                {                   
                    /* Policy Engine would enter this sub-state on reception of GoodCRC 
                       for the VCONN_Swap message sent */
                    DEBUG_PRINT_PORT_STR (u8PortNum,"ePE_VCS_SEND_SWAP_MSG_DONE_SS\r\n");
                    /* Start Sender Response Timer. Either Accept, Reject or wait would be 
                       received as response. If no response is received, move to 
                       ePE_SRC_READY or ePE_SNK_READY state */
                    gasPolicyEngine[u8PortNum].u8PETimerID = PDTimer_Start (
                                                            (PE_SENDERRESPONSE_TIMEOUT_MS),
                                                            PE_SubStateChange_TimerCB,u8PortNum,  
                                                            (UINT8)ePE_VCS_SEND_SWAP_NO_RESPONSE_SS);
                    
                    /* Assign an idle sub-state to wait for timer expiry */
                    gasPolicyEngine[u8PortNum].ePESubState = ePE_VCS_SEND_SWAP_IDLE_SS;                                            
                    break; 
                }
                case ePE_VCS_SEND_SWAP_NO_RESPONSE_SS:
                {                    
                    /* Response not received within tSenderResponse. Move to 
                       ePE_SRC_READY/ePE_SNK_READY state */
                    gasPolicyEngine[u8PortNum].ePEState = eTxDoneSt; 
                    gasPolicyEngine[u8PortNum].ePESubState = eTxDoneSS;

                    /* Post VCONN_SWAP_NO_RESP_RCVD notification*/
                    (void)DPM_NotifyClient (u8PortNum, eMCHP_PSF_VCONN_SWAP_NO_RESPONSE_RCVD);
                    break; 
                }
                case ePE_VCS_SEND_SWAP_REJECT_RCVD_SS:
                {                    
                    /* Response not received within tSenderResponse. Move to 
                       ePE_SRC_READY/ePE_SNK_READY state */
                    gasPolicyEngine[u8PortNum].ePEState = eTxDoneSt; 
                    gasPolicyEngine[u8PortNum].ePESubState = eTxDoneSS;
                    break; 
                }
                case ePE_VCS_SEND_SWAP_WAIT_RCVD_SS:
                {
                    /* Policy Engine would enter this sub-state on reception of 
                       Wait response from the port partner. Start PR Swap Wait timer
                       and move to ePE_SRC_READY or ePE_SNK_READY state. On PR_Swap
                       wait timer expiry, re-initiate PR_Swap transmission if not 
                       done by the port partner */
                    gasDPM[u8PortNum].u8VCONNSwapWaitTmrID = PDTimer_Start (
                                                          (PE_VCONN_SWAP_WAIT_TIMEOUT_MS),
                                                          DPM_SwapWait_TimerCB,u8PortNum,  
                                                          (UINT8)eVCONN_SWAP_INITIATE);
                    /* Move to ePE_SRC_READY/ePE_SNK_READY state */
                    gasPolicyEngine[u8PortNum].ePEState = eTxDoneSt; 
                    gasPolicyEngine[u8PortNum].ePESubState = eTxDoneSS;
                                 
                    break; 
                }
                case ePE_VCS_SEND_SWAP_ACCEPT_RCVD_SS:
                {
                    DEBUG_PRINT_PORT_STR (u8PortNum,"PE_VCS_ACCEPT_SWAP-ACCEPT_RCVD_SS: Entered the SubState \r\n");

                    /*Check whether the Port is currently sourcing VCONN and
                    transition accordingly*/
                    if (DPM_IsPortVCONNSource(u8PortNum))
                    {
                         gasPolicyEngine[u8PortNum].ePEState = ePE_VCS_WAIT_FOR_VCONN;
                         gasPolicyEngine[u8PortNum].ePESubState = \
                           ePE_VCS_WAIT_FOR_VCONN_START_TIMER_SS;
                    }
                    else
                    {                      
                        gasPolicyEngine[u8PortNum].ePEState = ePE_VCS_TURN_ON_VCONN;                        
                        gasPolicyEngine[u8PortNum].ePESubState = ePE_VCS_TURN_ON_VCONN_ENTRY_SS;
                        
                    }
                    break;
                }
                case ePE_VCS_SEND_SWAP_IDLE_SS:
                {
                    /* Idle state to wait for message transmit completion or timer expiry */
                    break; 
                }
                default:
                {
                    break; 
                }
            }
            break; 
        }
        
        case ePE_VCS_EVALUATE_SWAP:
        {
            /*Transition directly to next state as PSF accepts VCONN Swap always*/
            DEBUG_PRINT_PORT_STR (u8PortNum,"PE_VCS_EVALUATE_SWAP: Entered the state\r\n");
            if ((gasTypeCcontrol[u8PortNum].u8PortSts & TYPEC_PWDCABLE_PRES_MASK) &&
                 (DPM_ACCEPT_SWAP == DPM_EvaluateRoleSwap (u8PortNum, eVCONN_SWAP_RCVD)))
            {
#if (TRUE == INCLUDE_POWER_FAULT_HANDLING)

                /*Send Not Supported or Reject if Port partner requests VCONN Swap to supply the
                VCONN when the u8VCONNGoodtoSupply is false */
                if((!DPM_IsPortVCONNSource(u8PortNum)) && (!gasDPM[u8PortNum].u8VCONNGoodtoSupply))
                {
                    gasPolicyEngine[u8PortNum].ePEState = ePE_SEND_REJECT;
                    gasPolicyEngine[u8PortNum].ePESubState = ePE_SEND_REJECT_ENTRY_SS;
                }
                else
                {
                    gasPolicyEngine[u8PortNum].ePEState = ePE_VCS_ACCEPT_SWAP;
                    gasPolicyEngine[u8PortNum].ePESubState = ePE_VCS_ACCEPT_SWAP_SEND_ACCEPT_SS;
                }
#else 
                gasPolicyEngine[u8PortNum].ePEState = ePE_VCS_ACCEPT_SWAP;
                gasPolicyEngine[u8PortNum].ePESubState = ePE_VCS_ACCEPT_SWAP_SEND_ACCEPT_SS;
#endif
            }
            else
            {
                /* Use common state available for sending reject message */
                gasPolicyEngine[u8PortNum].ePEState = ePE_SEND_REJECT; 
                gasPolicyEngine[u8PortNum].ePESubState = ePE_SEND_REJECT_ENTRY_SS;                           
            }
            break;
        }
        case ePE_VCS_ACCEPT_SWAP:
        {
            switch (gasPolicyEngine[u8PortNum].ePESubState)
            {
                case ePE_VCS_ACCEPT_SWAP_SEND_ACCEPT_SS:
                {
                    DEBUG_PRINT_PORT_STR (u8PortNum,"PE_VCS_ACCEPT_SWAP-SEND_ACCEPT_SS: Entered the SubState \r\n");

                     /*Send Accept message*/
                    /*Set the PD message transmitter API to Send Accept Message*/
                    u32TransmitHeader = PRL_FormSOPTypeMsgHeader (u8PortNum, PE_CTRL_ACCEPT,\
                                         PE_OBJECT_COUNT_0, PE_NON_EXTENDED_MSG);

                    /*Set the transmitter callback to transition to source soft reset state if
                    message transmission fails*/
                    u32TransmitTmrIDTxSt = PRL_BUILD_PKD_TXST_U32 (ePE_VCS_ACCEPT_SWAP,\
                                             ePE_VCS_ACCEPT_SWAP_ACCEPT_SENT_SS,\
                                             eTxFailedSt, eTxFailedSS);

                    gasPolicyEngine[u8PortNum].ePESubState = ePE_VCS_ACCEPT_SWAP_IDLE_SS;
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
                    DEBUG_PRINT_PORT_STR (u8PortNum,"PE_VCS_ACCEPT_SWAP-ACCEPT_SENT_SS: Entered the SubState \r\n");

                    /*Check whether the Port is currently sourcing VCONN and
                    transition accordingly*/
                    if (DPM_IsPortVCONNSource(u8PortNum))
                    {
                        gasPolicyEngine[u8PortNum].ePEState = ePE_VCS_WAIT_FOR_VCONN;
                        gasPolicyEngine[u8PortNum].ePESubState = \
                           ePE_VCS_WAIT_FOR_VCONN_START_TIMER_SS;
                    }
                    else
                    {                      
                        gasPolicyEngine[u8PortNum].ePEState = ePE_VCS_TURN_ON_VCONN;                        
                        gasPolicyEngine[u8PortNum].ePESubState = ePE_VCS_TURN_ON_VCONN_ENTRY_SS;                        
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
            switch (gasPolicyEngine[u8PortNum].ePESubState)
            {
                case ePE_VCS_WAIT_FOR_VCONN_START_TIMER_SS:
                {
                     DEBUG_PRINT_PORT_STR (u8PortNum,"PE_VCS_WAIT_FOR_VCONN-START_TIMER_SS: Entered the SubState \r\n");

                    /*If PS RDY message is not received from port partner, then transition to Hard
                    reset state*/

                    /*Start the VCONN timer*/
                    /*Set the timer callback to transition to ePE_SRC_HARD_RESET state
                     * ePE_SRC_HARD_RESET_ENTRY_SS sub state if timeout happens*/
                    gasPolicyEngine[u8PortNum].u8PETimerID = PDTimer_Start (\
                                                              PE_VCONNON_TIMEOUT_MS,\
                                                              PE_SSChngAndTimeoutValidate_TimerCB,\
                                                              u8PortNum, eTxHardRstSS);


                    gasPolicyEngine[u8PortNum].ePESubState = ePE_VCS_WAIT_FOR_VCONN_WAIT_FOR_PS_RDY_SS;
                    break;
                }
                case ePE_VCS_WAIT_FOR_VCONN_WAIT_FOR_PS_RDY_SS:
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

        case ePE_VCS_TURN_OFF_VCONN:
        {          
            switch (gasPolicyEngine[u8PortNum].ePESubState)
            {
              
                case ePE_VCS_TURN_OFF_VCONN_ENTRY_SS:
                {
                  
                    DEBUG_PRINT_PORT_STR(u8PortNum,"PE_VCS_TURN_OFF_VCONN: Entered the state\r\n");

                    /*Turn off VCONN since PS RDY message is received from VCONN Source partner*/
                    DPM_VCONNOnOff(u8PortNum,DPM_VCONN_OFF);            
                    
                    /*Start the VCONN_OFF timer*/
                    /*This Timeout is implemented outside of the PD Specification to track 
                    VCONN Turn OFF error*/
                    gasPolicyEngine[u8PortNum].u8PETimerID = PDTimer_Start (\
                                                              PE_VCONNOFF_TIMEOUT_MS,\
                                                              DPM_VBUSorVCONNOnOff_TimerCB,\
                                                              u8PortNum,\
                                                              (UINT8)SET_TO_ZERO);
                    
                    gasPolicyEngine[u8PortNum].ePESubState = ePE_VCS_TURN_OFF_VCONN_CHECK_SS;
                    break;            
                }
                case ePE_VCS_TURN_OFF_VCONN_CHECK_SS:
                {                
                    if(!DPM_IsPortVCONNSource(u8PortNum))
                    {
                        /*Stop the VCONN_OFF timer*/
                        PE_KillPolicyEngineTimer (u8PortNum);
                        
                        gasPolicyEngine[u8PortNum].ePEState = eTxDoneSt;
                        gasPolicyEngine[u8PortNum].ePESubState = eTxDoneSS;
                        
                        /* Reset the discover identity counter to 0*/
                        gasPolicyEngine[u8PortNum].u8DiscoverIdentityCounter = SET_TO_ZERO;
                        
                        /* Post eMCHP_PSF_VCONN_SWAP_COMPLETE notification*/
                        (void)DPM_NotifyClient (u8PortNum, eMCHP_PSF_VCONN_SWAP_COMPLETE);
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
          
            switch (gasPolicyEngine[u8PortNum].ePESubState)
            {
                case ePE_VCS_TURN_ON_VCONN_ENTRY_SS:
                {                  
                    DEBUG_PRINT_PORT_STR(u8PortNum,"PE_VCS_TURN_ON_VCONN: Entered the state\r\n");
                    
                    /*Turn ON VCONN*/
                    DPM_VCONNOnOff (u8PortNum,DPM_VCONN_ON);
                    
                    /*Port Partner maintains the tVCONNSourceOn timer, So setting the VCONN_ON_self
                    timer greater than tVCONNSourceOn to send Hard reset in case of VCONN ON
                    failure*/
                    /*Start the VCONN_ON_self timer*/
                    gasPolicyEngine[u8PortNum].u8PETimerID = PDTimer_Start (\
                                                              PE_VCONNON_SELF_TIMEOUT_MS,\
                                                              DPM_VCONNONError_TimerCB,\
                                                              u8PortNum,\
                                                              (UINT8)SET_TO_ZERO);
                    
                    gasPolicyEngine[u8PortNum].ePESubState = ePE_VCS_TURN_ON_VCONN_CHECK_SS;
                    /* Hook to notify PE state machine entry into idle substate */
                    MCHP_PSF_HOOK_NOTIFY_IDLE(u8PortNum, eIDLE_PE_NOTIFY);
                    
                    break;
                  
                }
                
                case ePE_VCS_TURN_ON_VCONN_CHECK_SS:
                {                 
                    if(DPM_IsPortVCONNSource(u8PortNum))
                    {                          
                        PE_KillPolicyEngineTimer (u8PortNum);
                        gasPolicyEngine[u8PortNum].ePEState = ePE_VCS_SEND_PS_RDY;
                        gasPolicyEngine[u8PortNum].ePESubState = ePE_VCS_SEND_PS_RDY_ENTRY_SS;                                       
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
            switch (gasPolicyEngine[u8PortNum].ePESubState)
            {
                case ePE_VCS_SEND_PS_RDY_ENTRY_SS:
                {
                    DEBUG_PRINT_PORT_STR (u8PortNum,"ePE_VCS_SEND_PS_RDY-ENTRY_SS: Entered the SubState \r\n");
                    /*Send PS_RDY message*/
                    u32TransmitHeader = PRL_FormSOPTypeMsgHeader (u8PortNum, PE_CTRL_PS_RDY,\
                                                                   PE_OBJECT_COUNT_0, PE_NON_EXTENDED_MSG);

                    /*Set the transmitter callback to transition to source soft reset state if
                     message transmission fails*/
                    u32TransmitTmrIDTxSt = PRL_BUILD_PKD_TXST_U32 (eTxDoneSt,\
                                             eTxDoneSS,\
                                             eTxFailedSt,\
                                             eTxFailedSS);
                    u8IsTransmit = TRUE;
                    gasPolicyEngine[u8PortNum].ePESubState = ePE_VCS_SEND_PS_RDY_IDLE_SS;
                    
                    /* Reset the discover identity counter to 0*/
                    gasPolicyEngine[u8PortNum].u8DiscoverIdentityCounter = SET_TO_ZERO;

                    /* Post eMCHP_PSF_VCONN_SWAP_COMPLETE notification*/
                    (void)DPM_NotifyClient (u8PortNum, eMCHP_PSF_VCONN_SWAP_COMPLETE);
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
        default:
        {
            break;
        }
    }

    /* Transmit the message if u8IsTransmit is set */
    if (TRUE == u8IsTransmit)
    {
		(void) PRL_TransmitMsg (u8PortNum, (UINT8) PRL_SOP_TYPE, u32TransmitHeader, \
                            NULL, pfnTransmitCB, u32TransmitTmrIDTxSt); 
    }    
}
#endif /*INCLUDE_PD_VCONN_SWAP*/        
        