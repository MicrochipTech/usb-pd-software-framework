/*******************************************************************************
  PD VDM Policy Engine Source file

  Company:
    Microchip Technology Inc.

  File Name:
    policy_engine_vdm.c

  Description:
    This file contains the function definition for VDM Policy Engine State Machine.
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

#if (TRUE == INCLUDE_PD_VDM)

void PE_RunVDMStateMachine (UINT8 u8PortNum, UINT8 *pu8DataBuf, UINT32 u32Header)
{
    /* Transmit Message Type - SOP SOP' SOP" */ 
    UINT8 u8TransmitSOP = PRL_SOP_TYPE; 
    
    /* VDM Command Type */
    UINT8 u8VDMCmdType = SET_TO_ZERO; 
    
	/* Transmit Message Header */
	UINT32 u32TransmitHeader = SET_TO_ZERO;

    /* Transmit Data Object */
    UINT32 *u32pTransmitDataObj = SET_TO_ZERO; 
    
	/* Transmit Call back set to PE_StateChange_TransmitCB */
	PRLTxCallback pfnTransmitCB = PE_StateChange_TransmitCB;

	/* Transmit Call back variables */
	UINT32 u32TransmitTmrIDTxSt = SET_TO_ZERO;

	/* Transmit Flag */
	UINT8 u8IsTransmit = FALSE;
    
    /* Current Power Role */
    UINT8 u8CurrPwrRole = DPM_GET_CURRENT_POWER_ROLE(u8PortNum); 
    
    ePolicyState eTxDoneSt;
    ePolicySubState eTxDoneSS;
    
    if (PD_ROLE_SOURCE == u8CurrPwrRole)
    {
        eTxDoneSt = ePE_SRC_READY;
        eTxDoneSS = ePE_SRC_READY_END_AMS_SS;
    }
    else
    {
        eTxDoneSt = ePE_SNK_READY;
        eTxDoneSS = ePE_SNK_READY_END_AMS_SS;
    }          
    
    switch(gasPolicyEngine[u8PortNum].ePEState)
    {
        /* Initiator to Port VDM Discover Identity */
        case ePE_INIT_PORT_VDM_REQUEST:
        {
            switch(gasPolicyEngine[u8PortNum].ePESubState)
            {
                case ePE_INIT_PORT_VDM_REQUEST_ENTRY_SS: 
                {
                    DEBUG_PRINT_PORT_STR (u8PortNum,"ePE_INIT_PORT_VDM_REQUEST_ENTRY_SS\r\n");

                    /* Get the VDM Header configured by the application */
                    u32pTransmitDataObj = &gasCfgStatusData.sVDMPerPortData[u8PortNum].u32VDMHeader;
                    
                    u32TransmitHeader = PRL_FormSOPTypeMsgHeader(u8PortNum, (UINT8)PE_DATA_VENDOR_DEFINED,  \
                                                                        BYTE_LEN_1, PE_NON_EXTENDED_MSG);                    

                    u32TransmitTmrIDTxSt = PRL_BUILD_PKD_TXST_U32( ePE_INIT_PORT_VDM_REQUEST, \
                                                ePE_INIT_PORT_VDM_REQUEST_MSG_DONE_SS, \
                                                eTxDoneSt, eTxDoneSS);
              
                    u8IsTransmit = TRUE;
                    
                    gasPolicyEngine[u8PortNum].ePESubState = ePE_INIT_PORT_VDM_REQUEST_IDLE_SS;                                       
                    
                    break; 
                }               
                case ePE_INIT_PORT_VDM_REQUEST_MSG_DONE_SS:
                {
					/* GoodCRC received for VDM Identity request sent to SOP */
                    DEBUG_PRINT_PORT_STR (u8PortNum,"ePE_INIT_PORT_VDM_REQUEST_MSG_DONE_SS\r\n");
                    
					/* Start the VDMResponse timer, if timed out set the PE sub-state to 
					   ePE_INIT_PORT_VDM_IDENTITY_REQUEST_NO_RESPONSE_SS */
                    gasPolicyEngine[u8PortNum].u8PETimerID = PDTimer_Start (
                                                            (PE_VDMRESPONSE_TIMEOUT_MS),
                                                            PE_SubStateChange_TimerCB,u8PortNum,  
                                                            (UINT8)ePE_INIT_PORT_VDM_REQUEST_NO_RESPONSE_SS);
                    
                    gasPolicyEngine[u8PortNum].ePESubState = ePE_INIT_PORT_VDM_REQUEST_IDLE_SS;
                    break;
                }
                case ePE_INIT_PORT_VDM_REQUEST_RESP_RCVD_SS:
                {
                    DEBUG_PRINT_PORT_STR (u8PortNum,"ePE_INIT_PORT_VDM_REQUEST_RESP_RCVD_SS\r\n");

                    /* Store the received VDM Header and VDM response in status registers */                    
                    (void)MCHP_PSF_HOOK_MEMCPY(&gasCfgStatusData.sVDMPerPortData[u8PortNum].u32PartnerVDMHeader, pu8DataBuf, \
                                                                    BYTE_LEN_4);      
                    
                    gasCfgStatusData.sVDMPerPortData[u8PortNum].u8PartnerPDIdentityCnt = \
                                        (PRL_GET_OBJECT_COUNT(u32Header) - BYTE_LEN_1);
                            
                    (void)MCHP_PSF_HOOK_MEMCPY(gasCfgStatusData.sVDMPerPortData[u8PortNum].u32aPartnerPDIdentity, (pu8DataBuf + BYTE_LEN_4), \
                                        (gasCfgStatusData.sVDMPerPortData[u8PortNum].u8PartnerPDIdentityCnt * BYTE_LEN_4));                                                   
                                       
                    /* Get the VDM response type */
                    u8VDMCmdType = DPM_VDM_GET_CMD_TYPE(pu8DataBuf[DPM_VDM_HEADER_POS]);
                    
                    /* Move to Ready state based on the current power role */
                    gasPolicyEngine[u8PortNum].ePEState = eTxDoneSt;
                    gasPolicyEngine[u8PortNum].ePESubState = eTxDoneSS;                      
                   
                    if (PE_VDM_BUSY == u8VDMCmdType)
                    {                  
                        if(gasPolicyEngine[u8PortNum].u8VDMBusyCounter < PE_N_BUSY_COUNT)
                        {
                            gasPolicyEngine[u8PortNum].u8VDMBusyCounter++; 
                            /* Start the VDM Busy Retry Timer */            
                            gasDPM[u8PortNum].u8VDMBusyTmrID = PDTimer_Start (
                                                                    (PE_VDM_BUSY_TIMEOUT_MS),
                                                                    DPM_VDMBusy_TimerCB,u8PortNum,  
                                                                    (UINT8)SET_TO_ZERO);                                             
                        }
                        else
                        {
                            /* Reset the VDM Busy Counter */
                            gasPolicyEngine[u8PortNum].u8VDMBusyCounter = RESET_TO_ZERO;
                            
                            /* Post eMCHP_PSF_VDM_RESPONSE_NOT_RCVD notification */
                            (void)DPM_NotifyClient(u8PortNum, eMCHP_PSF_VDM_RESPONSE_NOT_RCVD); 
                        }
                    }
                    else /* ACK or NAK response */
                    {
                        /* Post eMCHP_PSF_VDM_RESPONSE_RCVD notification */
                        (void)DPM_NotifyClient(u8PortNum, eMCHP_PSF_VDM_RESPONSE_RCVD);                                                
                    }
                    break; 
                }
                case ePE_INIT_PORT_VDM_REQUEST_NO_RESPONSE_SS:
                {
                    /* No response from port partner for the VDM request sent. 
                       Post notification and move to Ready state 
                       based on the current power role */
                    gasPolicyEngine[u8PortNum].ePEState = eTxDoneSt;
                    gasPolicyEngine[u8PortNum].ePESubState = eTxDoneSS; 
                    
                    /* Post eMCHP_PSF_VDM_RESPONSE_NOT_RCVD notification */
                    (void)DPM_NotifyClient(u8PortNum, eMCHP_PSF_VDM_RESPONSE_NOT_RCVD); 
                                        
                    break; 
                }
                case ePE_INIT_PORT_VDM_REQUEST_IDLE_SS:
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
    
	/* Transmit the message if u8IsTransmit is set */
    if (TRUE == u8IsTransmit)
    {
		(void) PRL_TransmitMsg (u8PortNum, (UINT8) u8TransmitSOP, u32TransmitHeader, \
                    (UINT8 *)u32pTransmitDataObj, pfnTransmitCB, u32TransmitTmrIDTxSt); 
    }
    
}

#endif /* INCLUDE_PD_VDM */ 

/* *****************************************************************************
 End of File
 */
