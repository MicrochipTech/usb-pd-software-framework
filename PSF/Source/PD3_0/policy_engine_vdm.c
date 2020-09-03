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
    
    /* VDM Header */
    UINT32 u32VDMHeader = SET_TO_ZERO; 
    
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
        eTxDoneSS = ePE_SNK_READY_AMS_END_SS;
    }          
    
    switch(gasPolicyEngine[u8PortNum].ePEState)
    {
        /* Initiator to Port VDM Discover Identity */
        case ePE_INIT_PORT_VDM_IDENTITY_REQUEST:
        {
            switch(gasPolicyEngine[u8PortNum].ePESubState)
            {
                case ePE_INIT_PORT_VDM_IDENTITY_REQUEST_ENTRY_SS: 
                {
                    DEBUG_PRINT_PORT_STR (u8PortNum,"ePE_INIT_PORT_VDM_IDENTITY_REQUEST_ENTRY_SS\r\n");

                    /* Choose VDM version as per current spec revision */
                    if(PD_SPEC_REVISION_2_0 == DPM_GET_CURRENT_PD_SPEC_REV(u8PortNum))
                    {
                        u32VDMHeader = PE_VDM_HEADER_LOW_VER;
                    }                    
                    else
                    {
                        u32VDMHeader = PE_VDM_HEADER_HIGH_VER;
                    }
                    u32TransmitHeader = PRL_FormSOPTypeMsgHeader(u8PortNum, (UINT8)PE_DATA_VENDOR_DEFINED,  \
                                                                        BYTE_LEN_1, PE_NON_EXTENDED_MSG);
                    u32pTransmitDataObj = &u32VDMHeader;

                    u32TransmitTmrIDTxSt = PRL_BUILD_PKD_TXST_U32( ePE_INIT_PORT_VDM_IDENTITY_REQUEST, \
                                                ePE_INIT_PORT_VDM_IDENTITY_REQUEST_MSG_DONE_SS, \
                                                ePE_SEND_SOFT_RESET, ePE_SEND_SOFT_RESET_SOP_SS);
              
                    u8IsTransmit = TRUE;
                    
                    gasPolicyEngine[u8PortNum].ePESubState = ePE_INIT_PORT_VDM_IDENTITY_REQUEST_IDLE_SS;                                       
                    
                    break; 
                }               
                case ePE_INIT_PORT_VDM_IDENTITY_REQUEST_MSG_DONE_SS:
                {
					/* GoodCRC received for VDM Identity request sent to SOP */
                    DEBUG_PRINT_PORT_STR (u8PortNum,"ePE_INIT_PORT_VDM_IDENTITY_REQUEST_MSG_DONE_SS\r\n");
                    
					/* Start the VDMResponse timer, if timed out set the PE sub-state to 
					   ePE_INIT_PORT_VDM_IDENTITY_REQUEST_NO_RESPONSE_SS */
                    gasPolicyEngine[u8PortNum].u8PETimerID = PDTimer_Start (
                                                            (PE_VDMRESPONSE_TIMEOUT_MS),
                                                            PE_SubStateChange_TimerCB,u8PortNum,  
                                                            (UINT8)ePE_INIT_PORT_VDM_IDENTITY_REQUEST_NO_RESPONSE_SS);
                    
                    gasPolicyEngine[u8PortNum].ePESubState = ePE_INIT_PORT_VDM_IDENTITY_REQUEST_IDLE_SS;
                    break;
                }
                case ePE_INIT_PORT_VDM_IDENTITY_REQUEST_RESP_RCVD_SS:
                {
                    DEBUG_PRINT_PORT_STR (u8PortNum,"ePE_INIT_PORT_VDM_IDENTITY_REQUEST_RESP_RCVD_SS\r\n");

                    /* Store the received VDM response in u32aPartnerIdentity array.
                       If the response is NAK or BUSY, the response will contain
                       only 1 Data Object which is the VDM Header */
                    (void)MCHP_PSF_HOOK_MEMCPY(gasCfgStatusData.sPerPortData[u8PortNum].u32aPartnerIdentity, pu8DataBuf,
                                 (PRL_GET_OBJECT_COUNT(u32Header) * BYTE_LEN_4));                                                   
                                       
                    /* Move the VDM SM based on the type of response received */
                    u8VDMCmdType = DPM_VDM_GET_CMD_TYPE(pu8DataBuf[DPM_VDM_HEADER_POS]);
                    
                    if (PE_VDM_ACK == u8VDMCmdType)
                    {
                        gasPolicyEngine[u8PortNum].ePEState = ePE_INIT_PORT_VDM_IDENTITY_ACKED;                        
                    }
                    else if ((PE_VDM_NAK == u8VDMCmdType) || (PE_VDM_BUSY == u8VDMCmdType))
                    {
                        gasPolicyEngine[u8PortNum].ePEState = ePE_INIT_PORT_VDM_IDENTITY_NAKED;                                                
                    }
                    else /* PE_VDM_REQ */
                    {
                        gasPolicyEngine[u8PortNum].ePEState = ePE_VDM_GET_IDENTITY;
                    }
                    break; 
                }
                case ePE_INIT_PORT_VDM_IDENTITY_REQUEST_NO_RESPONSE_SS:
                {
                    /* No response from port partner for the VDM:DiscIdentity 
                       message sent. Post notification and move to Ready state 
                       based on the current power role */
                    gasPolicyEngine[u8PortNum].ePEState = eTxDoneSt;
                    gasPolicyEngine[u8PortNum].ePESubState = eTxDoneSS;                      
                    
                    (void)DPM_NotifyClient(u8PortNum, eMCHP_PSF_PARTNER_IDENTITY_NOT_RCVD); 
                    break; 
                }
                case ePE_INIT_PORT_VDM_IDENTITY_REQUEST_IDLE_SS:
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
        case ePE_INIT_PORT_VDM_IDENTITY_ACKED:
        {
            /* Move PE to Ready state based on the current power role */
            gasPolicyEngine[u8PortNum].ePEState = eTxDoneSt;
            gasPolicyEngine[u8PortNum].ePESubState = eTxDoneSS;                      
            
            /* Post eMCHP_PSF_PARTNER_IDENTITY_DISCOVERED notification */
            (void)DPM_NotifyClient(u8PortNum, eMCHP_PSF_PARTNER_IDENTITY_DISCOVERED);
            
            break; 
        }
        case ePE_INIT_PORT_VDM_IDENTITY_NAKED:
        {
            /* Policy Engine would enter this state if the response is NAK or BUSY 
               Start VDM Busy Retry Timer only for BUSY response */
            u8VDMCmdType = DPM_VDM_GET_CMD_TYPE(gasCfgStatusData.sPerPortData[u8PortNum].u32aPartnerIdentity[INDEX_0]);
            
            if (PE_VDM_BUSY == u8VDMCmdType)
            {
                gasDPM[u8PortNum].u8VDMBusyTmrID = PDTimer_Start (
                                                    (PE_VDM_BUSY_TIMEOUT_MS),
                                                    DPM_VDMBusy_TimerCB,u8PortNum,  
                                                    (UINT8)SET_TO_ZERO);                                
            }
            /* Move PE to Ready state based on the current power role */
            gasPolicyEngine[u8PortNum].ePEState = eTxDoneSt;
            gasPolicyEngine[u8PortNum].ePESubState = eTxDoneSS;                      
            
            /* Post notification only for NAK response */
            if (PE_VDM_NAK == u8VDMCmdType)
            {
                (void)DPM_NotifyClient(u8PortNum, eMCHP_PSF_PARTNER_IDENTITY_NAKED);
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
