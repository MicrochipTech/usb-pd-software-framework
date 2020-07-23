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
Copyright �  [2020] Microchip Technology Inc. and its subsidiaries.

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
void PE_DRSwapRunStateMachine(UINT8 u8PortNum)
{
	/* Transmit Message Type - SOP SOP' SOP" */
    UINT8 u8TransmitSOP = PRL_SOP_TYPE;

	/* Transmit Message Header */
	UINT32 u32TransmitHeader = SET_TO_ZERO;

	/* Transmit Data Object */
	UINT32 *u32pTransmitDataObj = NULL; 

	/* Transmit Call back */
	PRLTxCallback pfnTransmitCB = NULL;

	/* Transmit Call back variables */
	UINT32 u32TransmitTmrIDTxSt = SET_TO_ZERO;

	/* Transmit Flag */
	UINT8 u8IsTransmit = FALSE;
    
    UINT8 u8TxFailedSt, u8TxFailedSS;
    
    if (PD_ROLE_SOURCE == DPM_GET_CURRENT_POWER_ROLE(u8PortNum))
    {
        u8TxFailedSt = ePE_SRC_SEND_SOFT_RESET;
        u8TxFailedSS = ePE_SRC_SEND_SOFT_RESET_SOP_SS;
        
    }
    else
    {
        u8TxFailedSt = ePE_SNK_SEND_SOFT_RESET;
        u8TxFailedSS = ePE_SNK_SEND_SOFT_RESET_ENTRY_SS;
    }
    
   switch(gasPolicyEngine[u8PortNum].ePEState)
   {
        case ePE_DRS_EVALUATE_SWAP:
        {
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
                    /* Send the Accept message */
                    u32TransmitHeader = PRL_FormSOPTypeMsgHeader (u8PortNum, PE_CTRL_ACCEPT,
                                            PE_OBJECT_COUNT_0, PE_NON_EXTENDED_MSG);

                    /*u8TransmitSOP is set to PRL_SOP_TYPE by default and u32pTransmitDataObj is
                     assigned to a NULL pointer */
                    pfnTransmitCB = PE_StateChange_TransmitCB;
                    
                    /*If GoodCRC is not received, Soft reset state is set depending on
                     Power role*/
                    u32TransmitTmrIDTxSt = PRL_BUILD_PKD_TXST_U32( ePE_DRS_DFP_UFP_ROLE_CHANGE , \
                                                NULL, \
                                               u8TxFailedSt, u8TxFailedSS);

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
            /*Change the present role*/
            if (PD_ROLE_DFP == DPM_GET_CURRENT_DATA_ROLE(u8PortNum))
            {
                DPM_UpdateDataRole (u8PortNum, PD_ROLE_UFP);
            }
            else if (PD_ROLE_UFP == DPM_GET_CURRENT_DATA_ROLE(u8PortNum))
            {
                DPM_UpdateDataRole (u8PortNum, PD_ROLE_DFP);
            }
            else
            {
                /*Do nothing*/
            }
            
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

                    u8TransmitSOP = PRL_SOP_TYPE;
                    u32pTransmitDataObj = NULL;
                    pfnTransmitCB = PE_StateChange_TransmitCB;
      
                    u32TransmitTmrIDTxSt = PRL_BUILD_PKD_TXST_U32( ePE_DRS_SEND_SWAP , \
                                                ePE_DRS_SEND_SWAP_GOOD_CRC_RCVD_SS, \
                                               u8TxFailedSt, u8TxFailedSS);

                    u8IsTransmit = TRUE;
                    
                    /*Wait in a idle state to get a response for DR_SWAP*/
                    gasPolicyEngine[u8PortNum].ePESubState = ePE_DRS_SEND_SWAP_IDLE_SS;
                   
                    break;
                }
                case ePE_DRS_SEND_SWAP_IDLE_SS:
                {
                    break;
                }
                case ePE_DRS_SEND_SWAP_GOOD_CRC_RCVD_SS:
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
		(void) PRL_TransmitMsg (u8PortNum, (UINT8) u8TransmitSOP, u32TransmitHeader, \
                    (UINT8 *)u32pTransmitDataObj, pfnTransmitCB, u32TransmitTmrIDTxSt); 
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
    
    UINT8 u8TxFailedSt, u8TxFailedSS, u8TxDoneSt, u8TxDoneSS;
    
    if (PD_ROLE_SOURCE == DPM_GET_CURRENT_POWER_ROLE(u8PortNum))
    {
        u8TxDoneSt = ePE_SRC_READY;
        u8TxDoneSS = ePE_SRC_READY_END_AMS_SS;
        u8TxFailedSt = ePE_SRC_SEND_SOFT_RESET;
        u8TxFailedSS = ePE_SRC_SEND_SOFT_RESET_SOP_SS;        
    }
    else
    {
        u8TxDoneSt = ePE_SNK_READY;
        u8TxDoneSS = ePE_SNK_READY_IDLE_SS;
        u8TxFailedSt = ePE_SNK_SEND_SOFT_RESET;
        u8TxFailedSS = ePE_SNK_SEND_SOFT_RESET_ENTRY_SS;
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
                                                ePE_PRS_SEND_SWAP_GOODCRC_RCVD_SS, \
                                                u8TxFailedSt, u8TxFailedSS);

                    u8IsTransmit = TRUE;                                        
                             
                    /* Assign an idle sub-state to wait for message transmit completion */
                    gasPolicyEngine[u8PortNum].ePESubState = ePE_PRS_SEND_SWAP_IDLE_SS;                    
                    break; 
                }
                case ePE_PRS_SEND_SWAP_GOODCRC_RCVD_SS:
                {                   
                    DEBUG_PRINT_PORT_STR (u8PortNum,"ePE_PRS_SEND_SWAP_GOODCRC_RCVD_SS\r\n");
                    /* Start Sender Response Timer */
                    gasPolicyEngine[u8PortNum].u8PETimerID = PDTimer_Start (
                                                            (PE_SENDERRESPONSE_TIMEOUT_MS),
                                                            PE_SubStateChange_TimerCB,u8PortNum,  
                                                            (UINT8)ePE_PRS_SEND_SWAP_NO_RESPONSE_RCVD_SS);
                    
                    /* Assign an idle sub-state to wait for timer expiry */
                    gasPolicyEngine[u8PortNum].ePESubState = ePE_PRS_SEND_SWAP_IDLE_SS;                                            
                    break; 
                }
                case ePE_PRS_SEND_SWAP_NO_RESPONSE_RCVD_SS:
                {
                    /* Response not received within tSenderResponse. Move to 
                       ePE_SRC_READY/ePE_SNK_READY states */
                    gasPolicyEngine[u8PortNum].ePEState = u8TxDoneSt; 
                    gasPolicyEngine[u8PortNum].ePESubState = u8TxDoneSS;

                    break; 
                }
                case ePE_PRS_SEND_SWAP_WAIT_RCVD_SS:
                {
                    /* Start PR Swap Wait timer*/
                    gasDPM[u8PortNum].u8PRSwapWaitTmrID = PDTimer_Start (
                                                          (PE_PR_SWAP_WAIT_TIMEOUT_MS),
                                                          DPM_PRSwapWait_TimerCB,u8PortNum,  
                                                          (UINT8)SET_TO_ZERO);

                    gasPolicyEngine[u8PortNum].ePEState = u8TxDoneSt; 
                    gasPolicyEngine[u8PortNum].ePESubState = u8TxDoneSS;
                                 
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
                        u8TxDoneSt = ePE_PRS_SRC_SNK_TRANSITION_TO_OFF;
                        u8TxDoneSS = ePE_PRS_SRC_SNK_TRANSITION_TO_OFF_ENTRY_SS;
                    }
                    else
                    {
                        u8TxDoneSt = ePE_PRS_SNK_SRC_TRANSITION_TO_OFF;
                        u8TxDoneSS = ePE_PRS_SNK_SRC_TRANSITION_TO_OFF_ENTRY_SS;
                    }
                    
                    u32TransmitTmrIDTxSt = PRL_BUILD_PKD_TXST_U32( u8TxDoneSt, \
                                                u8TxDoneSS, u8TxFailedSt, u8TxFailedSS);

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
                    DEBUG_PRINT_PORT_STR (u8PortNum,"ePE_PRS_SRC_SNK_TRANSITION_TO_OFF_ENTRY_SS\r\n");
                    /* Start Source transition timer */
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
                    DEBUG_PRINT_PORT_STR (u8PortNum,"ePE_PRS_SRC_SNK_TRANSITION_TO_OFF_PWROFF_SS\r\n");
					/* Turn Off Power Supply by driving vSafe0V in VBUS */
                    DPM_TypeCSrcVBus5VOnOff(u8PortNum, DPM_VBUS_OFF);
                    
                     /*Start the VBUS OFF timer for monitoring the time taken for 
                       power module to reach vSafe0V*/
                    gasPolicyEngine[u8PortNum].u8PETimerID = PDTimer_Start (
                                                              (TYPEC_VBUS_OFF_TIMER_MS),
                                                              DPM_VBUSOnOff_TimerCB, u8PortNum,  
                                                              (UINT8)SET_TO_ZERO);
                   
                    gasPolicyEngine[u8PortNum].ePESubState = ePE_PRS_SRC_SNK_TRANSITION_TO_OFF_EXIT_SS;                    
                    break; 
                }
                case ePE_PRS_SRC_SNK_TRANSITION_TO_OFF_EXIT_SS:
                {
                    DEBUG_PRINT_PORT_STR (u8PortNum,"ePE_PRS_SRC_SNK_TRANSITION_TO_OFF_EXIT_SS\r\n");
                    
                    if(TYPEC_VBUS_0V == DPM_GetVBUSVoltage(u8PortNum))
                    {
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
            DEBUG_PRINT_PORT_STR (u8PortNum,"ePE_PRS_SRC_SNK_ASSERT_RD\r\n");
            /* Policy Engine enters this state when the source power supply has been turned off.
               There are no sub-states involved in this state*/
            /* Request DPM to assert Rd pull down on CC wire */
            DPM_SetTypeCState (u8PortNum, TYPEC_ATTACHED_SNK, TYPEC_ATTACHED_SNK_ASSERT_RD_SS);
            
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
                            (TYPEC_ATTACHED_SNK_RUN_SM_SS == gasTypeCcontrol[u8PortNum].u8TypeCSubState))
                    {
                        DEBUG_PRINT_PORT_STR (u8PortNum,"ePE_PRS_SRC_SNK_WAIT_SOURCE_ON_SEND_PSRDY_SS\r\n");
                        /* Port has transitioned into Sink. Update the Power role 
                           and send PS_RDY message */
                        /* Set the Current Port Power Role as Sink in DPM Status variable */
                        DPM_SET_POWER_ROLE_STS(u8PortNum, PD_ROLE_SINK);
                        
                        /* Set Port Power Role as Sink in Port Connection Status register */
                        gasCfgStatusData.sPerPortData[u8PortNum].u32PortConnectStatus &= ~(DPM_PORT_POWER_ROLE_STATUS_MASK);
                        gasCfgStatusData.sPerPortData[u8PortNum].u32PortConnectStatus |= DPM_PORT_POWER_ROLE_STATUS_SINK; 
                        
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
                    /* Start PSSourceOn timer */
                    gasPolicyEngine[u8PortNum].u8PETimerID = PDTimer_Start (
                                                            (PE_PS_SOURCE_ON_TIMEOUT_MS),
                                                            PE_SubStateChange_TimerCB,u8PortNum,  
                                                            (UINT8)ePE_PRS_SRC_SNK_WAIT_SOURCE_ON_ERROR_SS);
                    
                    /* Assign an idle state to wait for timer expiry */
                    gasPolicyEngine[u8PortNum].ePESubState = ePE_PRS_SRC_SNK_WAIT_SOURCE_ON_WAIT_FOR_PSRDY_SS;                                                                                    
                    break; 
                }
                case ePE_PRS_SRC_SNK_WAIT_SOURCE_ON_ERROR_SS:
                {
                    DEBUG_PRINT_PORT_STR (u8PortNum,"ePE_PRS_SRC_SNK_WAIT_SOURCE_ON_ERROR_SS\r\n");
                    /* PS_RDY message transmission failed or PSSourceOn Timer expired. 
                       So, revert the port's power role and invoke Type C Error Recovery */
                    DPM_SET_POWER_ROLE_STS(u8PortNum, PD_ROLE_SOURCE);                       
                    gasCfgStatusData.sPerPortData[u8PortNum].u32PortConnectStatus &= ~(DPM_PORT_POWER_ROLE_STATUS_MASK);
                    gasCfgStatusData.sPerPortData[u8PortNum].u32PortConnectStatus |= DPM_PORT_POWER_ROLE_STATUS_SOURCE; 

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
                    DEBUG_PRINT_PORT_STR (u8PortNum,"ePE_PRS_SRC_SNK_WAIT_SOURCE_ON_PSRDY_RCVD_SS\r\n");
                    /* Send PR_Swap complete notification */
                    DPM_NotifyClient (u8PortNum, eMCHP_PSF_PR_SWAP_COMPLETE); 
                    
                    gasPolicyEngine[u8PortNum].ePEState = ePE_SNK_STARTUP;
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
                    
                    /* Initialize and run PSSourceOffTimer */
                    gasPolicyEngine[u8PortNum].u8PETimerID = PDTimer_Start (
                                                            (PE_PS_SOURCE_OFF_TIMEOUT_MS),
                                                            DPM_PSSourceOff_TimerCB,u8PortNum,  
                                                            (UINT8)SET_TO_ZERO);
               
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
                            (TYPEC_ATTACHED_SRC_RUN_SM_SS == gasTypeCcontrol[u8PortNum].u8TypeCSubState))
                    {
                        DEBUG_PRINT_PORT_STR (u8PortNum,"ePE_PRS_SNK_SRC_SOURCE_ON_SEND_PSRDY_SS\r\n");
                        /* Port has transitioned into Source.  
                           Send PS_RDY message with power role updated as Source */                        
                        
                        /* Inform Protocol Layer of the port power role change */
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
                    
                    /* Start SwapSourceStart timer */
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
                       transmission fails. Revert the power role to Sink and 
                       invoke Type C Error Recovery */
                    DPM_SET_POWER_ROLE_STS(u8PortNum, PD_ROLE_SINK);                       
                    gasCfgStatusData.sPerPortData[u8PortNum].u32PortConnectStatus &= ~(DPM_PORT_POWER_ROLE_STATUS_MASK);
                    gasCfgStatusData.sPerPortData[u8PortNum].u32PortConnectStatus |= DPM_PORT_POWER_ROLE_STATUS_SINK; 
                    
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
                    
                    /* Clear the PR_Swap in progress flag */
                    gasPolicyEngine[u8PortNum].u8PEPortSts &= ~(PE_PR_SWAP_IN_PROGRESS_MASK);
                    
                    /* Move the Policy Engine to PE_SRC_STARTUP state */
                    gasPolicyEngine[u8PortNum].ePEState = ePE_SRC_STARTUP;
                    gasPolicyEngine[u8PortNum].ePESubState = ePE_SRC_STARTUP_ENTRY_SS;
                    
                    /* Send PR_Swap complete notification */
                    DPM_NotifyClient (u8PortNum, eMCHP_PSF_PR_SWAP_COMPLETE); 
                    
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
