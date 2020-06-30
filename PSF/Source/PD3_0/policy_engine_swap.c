/*******************************************************************************
  PD Policy Engine swap Source file

  Company:
    Microchip Technology Inc.

  File Name:
    policy_engine_swap.c

  Description:
    This file contains the function definitions for Swap Policy Engine
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
#if (TRUE == INCLUDE_PD_DR_SWAP) 
void PE_DRSwapRunStateMachine(UINT8 u8PortNum , UINT8 *pu8DataBuf , UINT8 u8SOPType ,UINT32 u32Header)
{
	/* Transmit Message Type - SOP SOP' SOP" */
    UINT8 u8TransmitSOP = PRL_SOP_TYPE;

	/* Transmit Message Header */
	UINT32 u32Transmit_Header = SET_TO_ZERO;

	/* Transmit Data Object */
	UINT32 *u32pTransmit_DataObj = SET_TO_ZERO; 

	/* Transmit Call back */
	PRLTxCallback Transmit_cb = NULL;

	/* Transmit Call back variables */
	UINT32 u32Transmit_TmrID_TxSt = SET_TO_ZERO;

	/* Transmit Flag */
	UINT8 u8IsTransmit = FALSE;
    
    UINT8 u8TxFailedSt, u8TxFailedSS, u8TxDoneSt, u8TxDoneSS;
    
    if (PD_ROLE_SOURCE == DPM_GET_CURRENT_POWER_ROLE(u8PortNum))
    {
        u8TxDoneSt = ePE_SRC_READY;
        u8TxDoneSS = ePE_SRC_READY_ENTRY_SS;
        u8TxFailedSt = ePE_SRC_SEND_SOFT_RESET;
        u8TxFailedSS = ePE_SRC_SEND_SOFT_RESET_SOP_SS;
        
    }
    else
    {
        u8TxDoneSt = ePE_SNK_READY;
        u8TxDoneSS = ePE_SNK_READY_ENTRY_SS;
        u8TxFailedSt = ePE_SNK_SEND_SOFT_RESET;
        u8TxFailedSS = ePE_SNK_SEND_SOFT_RESET_ENTRY_SS;
    }
    
   switch(gasPolicy_Engine[u8PortNum].ePEState)
   {
        case ePE_DRS_EVALUATE_SWAP:
        {
            /*Evaluate swap through DPM function*/
            break;
        }
        case ePE_DRS_ACCEPT_SWAP:
        {
            switch(gasPolicy_Engine[u8PortNum].ePESubState)
            {
                case ePE_DRS_ACCEPT_SWAP_SEND_ACCEPT_SS:
                {
                    /* Send the Accept message */
                    u32Transmit_Header = PRL_FormSOPTypeMsgHeader (u8PortNum, PE_CTRL_ACCEPT,
                                            PE_OBJECT_COUNT_0, PE_NON_EXTENDED_MSG);

                    u8TransmitSOP = PRL_SOP_TYPE;
                    u32pTransmit_DataObj = NULL;
                    Transmit_cb = PE_StateChange_TransmitCB;
      
                    u32Transmit_TmrID_TxSt = PRL_BUILD_PKD_TXST_U32( ePE_DRS_DFP_UFP_ROLE_CHANGE , \
                                                NULL, \
                                               u8TxFailedSt, u8TxFailedSS);

                    u8IsTransmit = TRUE;
                    
                    /*Wait in a idle state to get a response for Accept*/
                    gasPolicy_Engine[u8PortNum].ePESubState = ePE_DRS_ACCEPT_SWAP_IDLE_SS;
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
        case ePE_DRS_REJECT_SWAP:
        {
            switch(gasPolicy_Engine[u8PortNum].ePESubState)
            {
                case ePE_DRS_REJECT_SWAP_SEND_REJECT_SS:
                {
                    /* Send the Reject message */
                    u32Transmit_Header = PRL_FormSOPTypeMsgHeader (u8PortNum, PE_CTRL_REJECT,
                                            PE_OBJECT_COUNT_0, PE_NON_EXTENDED_MSG);

                    u8TransmitSOP = PRL_SOP_TYPE;
                    u32pTransmit_DataObj = NULL;
                    Transmit_cb = PE_StateChange_TransmitCB;
      
                    u32Transmit_TmrID_TxSt = PRL_BUILD_PKD_TXST_U32(u8TxDoneSt, \
                                                u8TxDoneSS, \
                                               u8TxFailedSt, u8TxFailedSS);

                    u8IsTransmit = TRUE;
                    
                    /*Wait in a idle state to get a response for Reject*/
                    gasPolicy_Engine[u8PortNum].ePESubState = ePE_DRS_REJECT_SWAP_IDLE_SS;
                   
                    break;
                }
                case ePE_DRS_REJECT_SWAP_IDLE_SS:
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
            break;
        }
       case ePE_DRS_SEND_SWAP:
       {
            switch(gasPolicy_Engine[u8PortNum].ePESubState)
            {
                case ePE_DRS_SEND_SWAP_ENTRY_SS:
                {
                    /* Send the Reject message */
                    u32Transmit_Header = PRL_FormSOPTypeMsgHeader (u8PortNum, PE_CTRL_DR_SWAP,
                                            PE_OBJECT_COUNT_0, PE_NON_EXTENDED_MSG);

                    u8TransmitSOP = PRL_SOP_TYPE;
                    u32pTransmit_DataObj = NULL;
                    Transmit_cb = PE_StateChange_TransmitCB;
      
                    u32Transmit_TmrID_TxSt = PRL_BUILD_PKD_TXST_U32( ePE_DRS_SEND_SWAP , \
                                                ePE_DRS_SEND_SWAP_GOOD_CRC_RCVD_SS, \
                                               u8TxFailedSt, u8TxFailedSS);

                    u8IsTransmit = TRUE;
                    
                    /*Wait in a idle state to get a response for DR_SWAP*/
                    gasPolicy_Engine[u8PortNum].ePESubState = ePE_DRS_SEND_SWAP_IDLE_SS;
                   
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
    if (u8IsTransmit == TRUE)
    {
		(void) PRL_TransmitMsg (u8PortNum, (UINT8) u8TransmitSOP, u32Transmit_Header, \
                    (UINT8 *)u32pTransmit_DataObj, Transmit_cb, u32Transmit_TmrID_TxSt); 
    }
}
#endif /*INCLUDE_PD_DR_SWAP*/
