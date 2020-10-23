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
    /* VDM Data Object Array */
    UINT32 u32aVDMDataObj[PRL_MAX_DATA_OBJ_COUNT] = {SET_TO_ZERO}; 
    
    /* VDM Data Object Count */
    UINT8 u8VDOCnt = SET_TO_ZERO; 
    
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
    
    ePolicyState eTxDoneSt;
    ePolicySubState eTxDoneSS;
    
    if (PD_ROLE_SOURCE == DPM_GET_CURRENT_POWER_ROLE(u8PortNum))
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
        /************************** VDM Initiator State **********************/
        case ePE_SEND_VDM:
        {
            switch(gasPolicyEngine[u8PortNum].ePESubState)
            {
                case ePE_SEND_VDM_ENTRY_SS: 
                {
                    DEBUG_PRINT_PORT_STR (u8PortNum,"ePE_SEND_VDM_ENTRY_SS\r\n");

                    /* Get the VDM Header and VDOs configured by the application */
                    u32aVDMDataObj[INDEX_0] = gasCfgStatusData.sVDMPerPortData[u8PortNum].u32VDMHeader;

#if (TRUE == INCLUDE_PD_ALT_MODE)    
                    /* This is used in cases where application sends responds for SVID 
                       specific commands like DPStatus, DPConfigure, etc., */
                    u8VDOCnt = gasCfgStatusData.sAltModePerPortData[u8PortNum].u8VDOCnt;
                            
                    (void) MCHP_PSF_HOOK_MEMCPY((u32aVDMDataObj + BYTE_LEN_1), gasCfgStatusData.sAltModePerPortData[u8PortNum].u32aVDO, \
                                                                        (u8VDOCnt * BYTE_LEN_4));                                                                       
#endif 
                                        
                    u32pTransmitDataObj = u32aVDMDataObj;
                    
                    u32TransmitHeader = PRL_FormSOPTypeMsgHeader(u8PortNum, (UINT8)PE_DATA_VENDOR_DEFINED,  \
                                                                    (u8VDOCnt + BYTE_LEN_1), PE_NON_EXTENDED_MSG);                    

                    if (DPM_VDM_REQ == DPM_GET_VDM_CMD_TYPE(u32aVDMDataObj[INDEX_0]))
                    {
                        u32TransmitTmrIDTxSt = PRL_BUILD_PKD_TXST_U32( ePE_SEND_VDM, \
                                                    ePE_SEND_VDM_MSG_DONE_SS, \
                                                    eTxDoneSt, eTxDoneSS);                                                
                    }
                    else
                    {
                        /* Move to Ready state since it is VDM response */
                        u32TransmitTmrIDTxSt = PRL_BUILD_PKD_TXST_U32( eTxDoneSt, \
                                                    eTxDoneSS, eTxDoneSt, eTxDoneSS);                                                                        
                        /* Clear the internal event */                        
                        gasDPM[u8PortNum].u16DPMInternalEvents &= ~(DPM_INT_EVT_INITIATE_VDM);                        
                    }
              
                    u8IsTransmit = TRUE;
                    
                    gasPolicyEngine[u8PortNum].ePESubState = ePE_SEND_VDM_IDLE_SS;                                       
                    
                    break; 
                }               
                case ePE_SEND_VDM_MSG_DONE_SS:
                {
					/* GoodCRC received for VDM Identity request sent to SOP */
                    DEBUG_PRINT_PORT_STR (u8PortNum,"ePE_SEND_VDM_MSG_DONE_SS\r\n");
                    
					/* Start the VDMResponse timer, if timed out set the PE sub-state to 
					   ePE_INIT_PORT_VDM_IDENTITY_REQUEST_NO_RESPONSE_SS */
                    gasPolicyEngine[u8PortNum].u8PETimerID = PDTimer_Start (
                                                            (PE_VDMRESPONSE_TIMEOUT_MS),
                                                            PE_SubStateChange_TimerCB,u8PortNum,  
                                                            (UINT8)ePE_SEND_VDM_NO_RESPONSE_SS);
                    
                    gasPolicyEngine[u8PortNum].ePESubState = ePE_SEND_VDM_IDLE_SS;
                    break;
                }
                case ePE_SEND_VDM_RESP_RCVD_SS:
                {
                    DEBUG_PRINT_PORT_STR (u8PortNum,"ePE_SEND_VDM_RESP_RCVD_SS\r\n");

                    /* Store the received VDM Header and VDM response in status registers */                    
                    if (eSVDM_DISCOVER_IDENTITY == (eSVDMCmd) DPM_GET_VDM_CMD(pu8DataBuf[DPM_VDM_HEADER_POS]))
                    {                        
                        (void)MCHP_PSF_HOOK_MEMCPY(&gasCfgStatusData.sVDMPerPortData[u8PortNum].u32PartnerVDMHeader, pu8DataBuf, \
                                                                    BYTE_LEN_4);      
                    
                        gasCfgStatusData.sVDMPerPortData[u8PortNum].u8PartnerPDIdentityCnt = \
                                            (PRL_GET_OBJECT_COUNT(u32Header) - BYTE_LEN_1);
                            
                        (void)MCHP_PSF_HOOK_MEMCPY(gasCfgStatusData.sVDMPerPortData[u8PortNum].u32aPartnerPDIdentity, (pu8DataBuf + BYTE_LEN_4), \
                                            (gasCfgStatusData.sVDMPerPortData[u8PortNum].u8PartnerPDIdentityCnt * BYTE_LEN_4));                                                   
                    }
                    /* Move to Ready state based on the current power role */
                    gasPolicyEngine[u8PortNum].ePEState = eTxDoneSt;
                    gasPolicyEngine[u8PortNum].ePESubState = eTxDoneSS;                      
                   
                    if (DPM_VDM_BUSY == DPM_GET_VDM_CMD_TYPE(pu8DataBuf[DPM_VDM_HEADER_POS]))
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
                case ePE_SEND_VDM_NO_RESPONSE_SS:
                {
                    DEBUG_PRINT_PORT_STR (u8PortNum,"ePE_SEND_VDM_NO_RESPONSE_SS\r\n");
                    
                    /* No response from port partner for the VDM request sent. 
                       Post notification and move to Ready state 
                       based on the current power role */
                    gasPolicyEngine[u8PortNum].ePEState = eTxDoneSt;
                    gasPolicyEngine[u8PortNum].ePESubState = eTxDoneSS; 
                    
                    /* Post eMCHP_PSF_VDM_RESPONSE_NOT_RCVD notification */
                    (void)DPM_NotifyClient(u8PortNum, eMCHP_PSF_VDM_RESPONSE_NOT_RCVD); 
                                        
                    break; 
                }
                case ePE_SEND_VDM_IDLE_SS:
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
        /************************** VDM Responder State **********************/
        case ePE_VDM_REQ_RCVD:
        {
            switch (gasPolicyEngine[u8PortNum].ePESubState)
            {
                case ePE_VDM_REQ_RCVD_ENTRY_SS:
                {                 
                    DEBUG_PRINT_PORT_STR (u8PortNum,"ePE_VDM_REQ_RCVD_ENTRY_SS\r\n");

                    /* Store the received VDM Header in u32PartnerVDMHeader */                    
                    (void) MCHP_PSF_HOOK_MEMCPY(&gasCfgStatusData.sVDMPerPortData[u8PortNum].u32PartnerVDMHeader, \
                                                                        pu8DataBuf, BYTE_LEN_4);      
                    
#if (TRUE == INCLUDE_PD_ALT_MODE)
                    /* For Enter Mode and other SVID specific commands, copy the
                       received VDOs in the status registers for the application to 
                       handle the evaluation and response */
                    gasCfgStatusData.sAltModePerPortData[u8PortNum].u8VDOCnt = \
                                        (PRL_GET_OBJECT_COUNT(u32Header) - BYTE_LEN_1);
                            
                    (void) MCHP_PSF_HOOK_MEMCPY(gasCfgStatusData.sAltModePerPortData[u8PortNum].u32aVDO, (pu8DataBuf + BYTE_LEN_4), \
                                        (gasCfgStatusData.sAltModePerPortData[u8PortNum].u8VDOCnt * BYTE_LEN_4));                                                   
#endif 
                    
                    /* Get Evaluation of VDM request from Device Policy Manager */
                    UINT8 u8DPMResponse = DPM_EvaluateVDMRequest (u8PortNum, (UINT32*)pu8DataBuf);
                    
                    if (DPM_IGNORE_VDM_RESPONSE == u8DPMResponse)
                    {
                        /* It is expected that the user application will evaluate the 
                           VDM request and send response by raising VDM Client request.
                           So, move to Ready state */
                        gasPolicyEngine[u8PortNum].ePEState = eTxDoneSt; 
                        gasPolicyEngine[u8PortNum].ePESubState = eTxDoneSS;                        
                    }
                    else /* DPM_RESPOND_VDM_ACK or DPM_RESPOND_VDM_NAK */
                    {
                        /* Copy the received VDM Header */
                        u32aVDMDataObj[INDEX_0] = gasCfgStatusData.sVDMPerPortData[u8PortNum].u32PartnerVDMHeader; 
                        
                        /* Clear the command type bits in VDM header */
                        u32aVDMDataObj[INDEX_0] &= ~DPM_VDM_CMD_TYPE_MASK; 
                        
                        if (DPM_RESPOND_VDM_ACK == u8DPMResponse)
                        {
                            u32aVDMDataObj[INDEX_0] |= (DPM_VDM_ACK << DPM_VDM_CMD_TYPE_POS);
                            
                            /* Get VDOs from DPM */
                            DPM_ReturnVDOs (u8PortNum, (UINT32*)pu8DataBuf, &u8VDOCnt, (u32aVDMDataObj + BYTE_LEN_1));                                           
                        }
                        else /* DPM_RESPOND_VDM_NAK */
                        {
                            u32aVDMDataObj[INDEX_0] |= (DPM_VDM_NAK << DPM_VDM_CMD_TYPE_POS); 
                        }
                        
                       /* u8VDOCnt returned by DPM during ACK will not include VDM 
                          Header. So, Object count is incremented by 1 to include 
                          VDM Header. In case of NAK, u8VDOCnt will be 0 */
                        u32TransmitHeader = PRL_FormSOPTypeMsgHeader(u8PortNum, PE_DATA_VENDOR_DEFINED, \
                                                                        (u8VDOCnt + BYTE_LEN_1), PE_NON_EXTENDED_MSG);                                        
                        u32pTransmitDataObj = u32aVDMDataObj;
                    
                        u32TransmitTmrIDTxSt = PRL_BUILD_PKD_TXST_U32( eTxDoneSt, \
                                                    eTxDoneSS, eTxDoneSt, eTxDoneSS);              
                        
                        u8IsTransmit = TRUE; 
                        
                        gasPolicyEngine[u8PortNum].ePESubState = ePE_VDM_REQ_RCVD_IDLE_SS;                                                                                       
                    }                                                      
                    break;
                }
                case ePE_VDM_REQ_RCVD_IDLE_SS:
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
                    (UINT8 *)u32pTransmitDataObj, pfnTransmitCB, u32TransmitTmrIDTxSt); 
    }
    
}

#endif /* INCLUDE_PD_VDM */ 

/* *****************************************************************************
 End of File
 */
