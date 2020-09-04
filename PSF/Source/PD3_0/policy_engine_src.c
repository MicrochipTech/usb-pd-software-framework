/*******************************************************************************
  PD Source Policy Engine Source file

  Company:
    Microchip Technology Inc.

  File Name:
    policy_engine_src.c

  Description:
    This file contains the function definitions for Source Policy Engine state-machine
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

#if (TRUE == INCLUDE_PD_SOURCE)
/********************************************************************/
/****************Source Policy Engine State Machine******************/
/********************************************************************/
void PE_RunSrcStateMachine(UINT8 u8PortNum , UINT8 *pu8DataBuf , UINT8 u8SOPType ,UINT32 u32Header)
{
    /* Receive VDM Header */
    UINT32 u32VDMHeader = SET_TO_ZERO;

	/* Source Capabilities PDO Count */
    UINT8 u8SrcPDOCnt = SET_TO_ZERO;

	/* Source Cap Objects */
    UINT32 u32aDataObj[DPM_MAX_PDO_CNT] = {SET_TO_ZERO};

	/* Type-C state and sub-state */
    UINT8 u8TypeCState = TYPEC_INVALID_STATE, u8TypeCSubState = TYPEC_INVALID_STATE;

	/* Transmit Message Type - SOP SOP' SOP" */
    UINT8 u8TransmitSOP = PRL_SOP_TYPE;

	/* Transmit Message Header */
	UINT32 u32TransmitHeader = SET_TO_ZERO;

	/* Transmit Data Object */
	UINT32 *u32pTransmitDataObj = SET_TO_ZERO; 

	/* Transmit Call back */
	PRLTxCallback pfnTransmitCB = NULL;

	/* Transmit Call back variables */
	UINT32 u32TransmitTmrIDTxSt = SET_TO_ZERO;

	/* Transmit Flag */
	UINT8 u8IsTransmit = FALSE;

	/* Ra Presence Check */
    UINT8 u8RaPresence = FALSE;
    
#if (TRUE == CONFIG_HOOK_DEBUG_MSG)    
    /* Added for negotiated PDO debug message */
    UINT32 u32PDODebug = SET_TO_ZERO;
#endif
    
#if (TRUE == INCLUDE_PD_SOURCE_PPS)    
    /* PS_RDY Timer value to be used in case of PPS contract */
    UINT32 u32PpsSrcTransTmrMs = SET_TO_ZERO; 
    
    /* Used to get Alert Data Object and PPS_Status Data Block from DPM */
    UINT32 u32DataBlock = SET_TO_ZERO; 
    
    /* Status Data Block */
    UINT8 u8aStatusDB[PE_STATUS_DATA_BLOCK_SIZE_IN_BYTES] = {SET_TO_ZERO};
#endif 
    
    /* Get the Type-C state from DPM */
    DPM_GetTypeCStates(u8PortNum, &u8TypeCState, &u8TypeCSubState);
    
    /* Get Powered Cable Presence */
    u8RaPresence = (gasTypeCcontrol[u8PortNum].u8PortSts & TYPEC_PWDCABLE_PRES_MASK);
	
    /* Setting Timeout sub-state to invalid state */
    gasPolicyEngine[u8PortNum].ePETimeoutSubState = ePE_INVALIDSUBSTATE;
    
    /* If port partner detached set the Policy Engine State to PE_SRC_STARTUP */
    if((TYPEC_UNATTACHED_SRC == u8TypeCState) && (ePE_SRC_STARTUP != gasPolicyEngine[u8PortNum].ePEState))
    {
		/* Kill all the port timers */
        PDTimer_KillPortTimers(u8PortNum);
        gasPolicyEngine[u8PortNum].u8PETimerID = MAX_CONCURRENT_TIMERS;
        gasPolicyEngine[u8PortNum].ePEState = ePE_SRC_STARTUP;
        gasPolicyEngine[u8PortNum].ePESubState = ePE_SRC_STARTUP_ENTRY_SS;        
        gasPolicyEngine[u8PortNum].u8HardResetCounter = RESET_TO_ZERO;
        
        /* Set spec revision to default spec revision on every detach */
        DPM_UpdatePDSpecRev (u8PortNum, CONFIG_PD_DEFAULT_SPEC_REV);
        
        /* Spec Rev is updated by PRL*/
        PRL_UpdateSpecAndDeviceRoles (u8PortNum);
    }
     
    /* Source Policy Engine State Machine */
    switch(gasPolicyEngine[u8PortNum].ePEState)
    {
        /********************PE_SRC_STARTUP*********************/
        case ePE_SRC_STARTUP:
        {
            switch(gasPolicyEngine[u8PortNum].ePESubState)
            {
                case ePE_SRC_STARTUP_ENTRY_SS:
                {
                    DEBUG_PRINT_PORT_STR (u8PortNum,"PE_SRC_STARTUP-ENTRY_SS\r\n");
					
					/* Reset the Caps Counter value */
                    gasPolicyEngine[u8PortNum].u8CapsCounter = FALSE;
					
                    gasPolicyEngine[u8PortNum].u8PEPortSts &= (~PE_PDCONNECTED_STS_MASK);
                    
					/* Set PD Contract as Implicit Contract */
                    gasPolicyEngine[u8PortNum].u8PEPortSts &= (~PE_EXPLICIT_CONTRACT);
					
					/* Reset the Protocol Layer */
                    PRL_ProtocolReset(u8PortNum);
                    
                    /*Reset collision avoidance set*/
                    #if (TRUE == INCLUDE_PD_3_0)                    
                    PRL_SetCollisionAvoidance (u8PortNum, TYPEC_SINK_TXOK);
                    #endif 

                    gasPolicyEngine[u8PortNum].ePESubState = ePE_SRC_STARTUP_IDLE_SS;
                    
                    #if (FALSE != INCLUDE_PDFU)
                    if((FALSE!=gsPdfuInfo.u8IsPDFUActive) && (u8PortNum == gsPdfuInfo.u8PDFUPortNum))
                    {
                        PE_FwUpdtInitialize();
                    }
                    #endif
                    
                    break;
                }
                
                case ePE_SRC_STARTUP_IDLE_SS:
                {
                    if((TYPEC_ATTACHED_SRC == u8TypeCState) && (TYPEC_ATTACHED_SRC_RUN_SM_SS == u8TypeCSubState))
                    {
						/* If E-Cable attached with port partner and the port is sourcing VCONN, before sending the source capabilities message
							Cable discovery identity message will be send to the cable to find the cable capabilities 
                           Spec Ref: A VCONN Source that is also a Source can attempt to send a Discover Identity Command 
                           using SOP' to a Cable Plug prior to the establishment of an Explicit Contract */
                        if(FALSE == DPM_IsPortVCONNSource(u8PortNum))
                        { 
							/* E-Cable not present, Port partner alone attached */
                            DEBUG_PRINT_PORT_STR (u8PortNum,"PE_SRC_STARTUP-IDLE_SS: Device Attached\r\n");
                            gasPolicyEngine[u8PortNum].ePEState = ePE_SRC_SEND_CAPABILITIES;
                            gasPolicyEngine[u8PortNum].ePESubState = ePE_SRC_SEND_CAP_ENTRY_SS;                      
                        }
                        else
                        {
							/* Port partner attached with E-Cable */
                            DEBUG_PRINT_PORT_STR (u8PortNum,"PE_SRC_STARTUP-IDLE_SS: E-Cable and Device Attached\r\n");
                            gasPolicyEngine[u8PortNum].ePEState = ePE_SRC_VDM_IDENTITY_REQUEST;
                            gasPolicyEngine[u8PortNum].ePESubState = ePE_SRC_VDM_IDENTITY_REQUEST_ENTRY_SS;
                            gasDPM[u8PortNum].u16MaxCurrSupportedin10mA = DPM_CABLE_CURR_3A_UNIT;
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
        
		/***************** PE_SRC_SEND_CAPABILITIES *****************/
        case ePE_SRC_SEND_CAPABILITIES:
        {
            switch(gasPolicyEngine[u8PortNum].ePESubState)
            {
                case ePE_SRC_SEND_CAP_ENTRY_SS:
                {
                    /* If HardReset counter reaches nHardResetCount and NoResponseTimer timed out */
                    if((gasPolicyEngine[u8PortNum].u8HardResetCounter > PE_N_HARD_RESET_COUNT) &&
                       (gasPolicyEngine[u8PortNum].u8PEPortSts & PE_NO_RESPONSE_TIMEDOUT))
                    {
						/* If PD Connected change the Type-C state to TYPEC_ERROR_RECOVERY state */
                        if(gasPolicyEngine[u8PortNum].u8PEPortSts & PE_PDCONNECTED_STS_MASK)
                        {
                            DPM_SetTypeCState(u8PortNum, TYPEC_ERROR_RECOVERY, TYPEC_ERROR_RECOVERY_ENTRY_SS);
                        }
                        
						/* If PD not connected change the PE state to ePE_SRC_DISABLED */
                        else
                        {
                            gasPolicyEngine[u8PortNum].ePEState = ePE_SRC_DISABLED;
                            MCHP_PSF_NOTIFY_CALL_BACK (u8PortNum, eMCHP_PSF_PE_SRC_DISABLED);
                        }
                    }                 
                    else
                    {
						/* Send Source Capabilities message to Port partner */
                        DEBUG_PRINT_PORT_STR (u8PortNum,"PE_SRC_SEND_CAP-ENTRY_SS: Send Source Capabilities\r\n");
                        DPM_GetSourceCapabilities(u8PortNum, &u8SrcPDOCnt, u32aDataObj);
                        
                        u32TransmitHeader = PRL_FormSOPTypeMsgHeader(u8PortNum, PE_DATA_SOURCE_CAP,  \
                                                                            u8SrcPDOCnt, PE_NON_EXTENDED_MSG);
                        
                        u8TransmitSOP = PRL_SOP_TYPE;
                        u32pTransmitDataObj = u32aDataObj;
                        pfnTransmitCB = PE_StateChange_TransmitCB;
                        
                        if(gasPolicyEngine[u8PortNum].u8PEPortSts & PE_PDCONNECTED_STS_MASK)
                        {
                            /*The Protocol Layer indicates that the Message has not been sent , so
                             Send Soft reset since PD connected presently*/
                            u32TransmitTmrIDTxSt = PRL_BUILD_PKD_TXST_U32( ePE_SRC_SEND_CAPABILITIES, \
                                                        ePE_SRC_SEND_CAP_GOODCRC_RCVD_SS, \
                                                          ePE_SEND_SOFT_RESET, ePE_SEND_SOFT_RESET_SOP_SS);                                                  
                          
                        }
                        else
                        {                          
                            /*The Policy Engine Shall transition to the PE_SRC_Discovery state when:
                              The Protocol Layer indicates that the Message has not been sent and we are presently not Connected
                             */
                            u32TransmitTmrIDTxSt = PRL_BUILD_PKD_TXST_U32( ePE_SRC_SEND_CAPABILITIES, \
                                                        ePE_SRC_SEND_CAP_GOODCRC_RCVD_SS, \
                                                        ePE_SRC_DISCOVERY, ePE_SRC_DISCOVERY_ENTRY_SS);
                        }

                        u8IsTransmit = TRUE;
                        gasPolicyEngine[u8PortNum].u8CapsCounter ++;
                                            
                        gasPolicyEngine[u8PortNum].ePESubState = ePE_SRC_SEND_CAP_IDLE_SS;
                    }
                    break;  
                }
                
                case ePE_SRC_SEND_CAP_IDLE_SS:
                {
                    /* Hook to notify PE state machine entry into idle substate */
                    MCHP_PSF_HOOK_NOTIFY_IDLE(u8PortNum, eIDLE_PE_NOTIFY);
                    break;  
                }
                
                case ePE_SRC_SEND_CAP_GOODCRC_RCVD_SS:
                {
                    DEBUG_PRINT_PORT_STR (u8PortNum,"PE_SRC_SEND_CAP-GOODCRC_RECEIVED_SS\r\n");
                    
                    /* Reset CapsCounter and HardReset Counter to 0 */
                    gasPolicyEngine[u8PortNum].u8CapsCounter = RESET_TO_ZERO;
                    gasPolicyEngine[u8PortNum].u8HardResetCounter = RESET_TO_ZERO;
                    gasPolicyEngine[u8PortNum].ePESubState = ePE_SRC_SEND_CAP_IDLE_SS;
					
					/* Set PD Status as Connected */
                    gasPolicyEngine[u8PortNum].u8PEPortSts |= PE_PDCONNECTED_STS_MASK;
                    
					/* Start Sender Response timer and Set the timer callback to transition to 
					ePE_SRC_HARD_RESET sate and ePE_SRC_HARD_RESET_ENTRY_SS sub state if timeout happens */
                    gasPolicyEngine[u8PortNum].u8PETimerID = PDTimer_Start (
                                                           PE_SENDERRESPONSE_TIMEOUT_MS,
                                                            PE_SSChngAndTimeoutValidate_TimerCB, u8PortNum,  
                                                            (UINT8)ePE_SRC_HARD_RESET_ENTRY_SS);
#if (TRUE == INCLUDE_PD_SOURCE_PPS)
                    /* Register an internal event for sending Alert for Cable Limitation 
                    This request would be processed once an explicit contract is established */
                    if (gasCfgStatusData.sPerPortData[u8PortNum].u32PortConnectStatus & 
                                    DPM_PORT_CABLE_REDUCED_SRC_CAPABILITIES_STATUS)
                    {
                        gasDPM[u8PortNum].u8AlertType |= DPM_ALERT_TYPE_OPR_COND_CHANGE; 
                        DPM_RegisterInternalEvent(u8PortNum, DPM_INT_EVT_INITIATE_ALERT);
                    }
#endif                    
                    /* Hook to notify PE state machine entry into idle substate */
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
        
		/************ PE_SRC_NEGOTIATE_CAPABILITY *************/
        case ePE_SRC_NEGOTIATE_CAPABILITY:
        {
            /* Set PD Status as Connected as ePE_SRC_SEND_CAP_GOODCRC_RCVD_SS state 
             may be skipped sometime when Request from partner arises quickly after Good_CRC*/
            gasPolicyEngine[u8PortNum].u8PEPortSts |= PE_PDCONNECTED_STS_MASK;
            
           /* Update Advertised PDO registers */
            DPM_UpdateAdvertisedPDOParam(u8PortNum); 

			/* Validate the received Request message by passing the received message to DPM */
            if(DPM_VALID_REQUEST == DPM_ValidateRequest(u8PortNum, (UINT16)u32Header, pu8DataBuf))
            {
				/* Received Request is a valid request */
                DEBUG_PRINT_PORT_STR (u8PortNum,"PE_SRC_NEGOTIATE_CAPABILITY: Request is Valid\r\n");
                
                /* No RDO Reject message has been sent to last Request made by attached Sink */
                gasCfgStatusData.sPerPortData[u8PortNum].u32PortConnectStatus &= 
                                        ~(DPM_PORT_AS_SRC_RDO_REJECTED_STATUS); 
                
                gasPolicyEngine[u8PortNum].ePEState = ePE_SRC_TRANSITION_SUPPLY;
                gasPolicyEngine[u8PortNum].ePESubState = ePE_SRC_TRANSITION_SUPPLY_ENTRY_SS;
            }            
            else
            {
				/* Received Request is invalid */
                DEBUG_PRINT_PORT_STR (u8PortNum,"PE_SRC_NEGOTIATE_CAPABILITY: Request is Invalid\r\n"); 
                
                /* No RDO Accept message has been sent to last Request made by attached Sink */
                gasCfgStatusData.sPerPortData[u8PortNum].u32PortConnectStatus &= 
                                        ~(DPM_PORT_AS_SRC_RDO_ACCEPTED_STATUS); 
                
                gasPolicyEngine[u8PortNum].ePEState = ePE_SRC_CAPABILITY_RESPONSE;
                gasPolicyEngine[u8PortNum].ePESubState = ePE_SRC_CAPABILITY_RESPONSE_ENTRY_SS;
            }
            
            break;
        }
        
		/************** PE_SRC_TRANSITION_SUPPLY ****************/
        case ePE_SRC_TRANSITION_SUPPLY:
        {
            switch(gasPolicyEngine[u8PortNum].ePESubState)
            {
                case ePE_SRC_TRANSITION_SUPPLY_ENTRY_SS:
                {
                    DEBUG_PRINT_PORT_STR (u8PortNum,"PE_SRC_TRANSITION_SUPPLY-ENTRY_SS\r\n");
                    
					/* Send the Accept message */
                    u32TransmitHeader = PRL_FormSOPTypeMsgHeader (u8PortNum, PE_CTRL_ACCEPT,
                                            PE_OBJECT_COUNT_0, PE_NON_EXTENDED_MSG);

                    u8TransmitSOP = PRL_SOP_TYPE;
                    u32pTransmitDataObj = NULL;
                    pfnTransmitCB = PE_StateChange_TransmitCB;
      
                    u32TransmitTmrIDTxSt = PRL_BUILD_PKD_TXST_U32( ePE_SRC_TRANSITION_SUPPLY, \
                                                ePE_SRC_TRANSITION_SUPPLY_GOODCRC_RCVD_SS, \
                                                ePE_SRC_HARD_RESET, ePE_SRC_HARD_RESET_ENTRY_SS);

                    u8IsTransmit = TRUE;                                        
                                                 
                    gasPolicyEngine[u8PortNum].ePESubState = ePE_SRC_TRANSITION_SUPPLY_IDLE_SS;
                    break;  
                }
                
                case ePE_SRC_TRANSITION_SUPPLY_GOODCRC_RCVD_SS:
                {
                    /* Set AS_SOURCE_RDO_ACCEPTED bit in Port Connection Status */
                    gasCfgStatusData.sPerPortData[u8PortNum].u32PortConnectStatus |= DPM_PORT_AS_SRC_RDO_ACCEPTED_STATUS;
                    
                    /* If the request is for a Fixed PDO, Start the 
                       Source Transition timer. */
                    if (DPM_PD_FIXED_SUPPLY_CONTRACT == DPM_GET_CURRENT_EXPLICIT_CONTRACT(u8PortNum))
                    {
                        gasPolicyEngine[u8PortNum].u8PETimerID = PDTimer_Start (
                                                                (PE_SRCTRANSISTION_TIMEOUT_MS),
                                                                PE_SubStateChange_TimerCB,u8PortNum,  
                                                                (UINT8)ePE_SRC_TRANSITION_SUPPLY_DRIVE_POWER_SS);
                        gasPolicyEngine[u8PortNum].ePESubState = ePE_SRC_TRANSITION_SUPPLY_IDLE_SS;                        
                    }
                    
#if (TRUE == INCLUDE_PD_SOURCE_PPS)                    
                    /* In case of programmable RDO, Source transition timer is 
                       not needed. Go ahead and drive the port power. */
                    else if (DPM_PD_PPS_CONTRACT == DPM_GET_CURRENT_EXPLICIT_CONTRACT(u8PortNum))
                    {
                        gasPolicyEngine[u8PortNum].ePESubState = ePE_SRC_TRANSITION_SUPPLY_DRIVE_POWER_SS;
                    }
#endif 
                    else
                    {
                        /* No support for Battery and Variable power supplies */
                    }
                    break;
                }
                
                case ePE_SRC_TRANSITION_SUPPLY_DRIVE_POWER_SS:
                {
                    /* Drive the Power to as requested by port partner*/
                    DPM_SetPortPower(u8PortNum);
                    
                    if (DPM_PD_FIXED_SUPPLY_CONTRACT == DPM_GET_CURRENT_EXPLICIT_CONTRACT(u8PortNum))
                    {
                         /* Start tSrcReady timer */
                        gasPolicyEngine[u8PortNum].u8PETimerID = PDTimer_Start (
                                                              (PE_SRC_READY_TIMEOUT_MS),
                                                              DPM_VBUSorVCONNOnOff_TimerCB, u8PortNum,  
                                                              (UINT8)SET_TO_ZERO);
                        gasPolicyEngine[u8PortNum].ePESubState = ePE_SRC_TRANSITION_SUPPLY_EXIT_SS;  
                        /* Hook to notify PE state machine entry into idle sub-state */
                        MCHP_PSF_HOOK_NOTIFY_IDLE(u8PortNum, eIDLE_PE_NOTIFY);
                    }

#if (TRUE == INCLUDE_PD_SOURCE_PPS)                    
                    else if (DPM_PD_PPS_CONTRACT == DPM_GET_CURRENT_EXPLICIT_CONTRACT(u8PortNum))
                    {
                        /* Get the PPS timer value that needs to be used for sending PS_RDY */
                        u32PpsSrcTransTmrMs = DPM_ReturnPPSSrcTransTmrVal(u8PortNum); 
                        
                        /* Start tPpsSrcTransSmall/tPpsSrcTransLarge timer */
                        gasPolicyEngine[u8PortNum].u8PETimerID = PDTimer_Start (
                                                              u32PpsSrcTransTmrMs,
                                                              PE_SubStateChange_TimerCB, u8PortNum,  
                                                              (UINT8)ePE_SRC_TRANSITION_SUPPLY_EXIT_SS);                                                       
                        
                        gasPolicyEngine[u8PortNum].ePESubState = ePE_SRC_TRANSITION_SUPPLY_IDLE_SS;
                        /* Hook to notify PE state machine entry into idle sub-state */
                        MCHP_PSF_HOOK_NOTIFY_IDLE(u8PortNum, eIDLE_PE_NOTIFY);
                    }
#endif 
                    else
                    {
                        /* No support for Battery and Variable power supplies */
                    }
                    break;
                }
                
                case ePE_SRC_TRANSITION_SUPPLY_EXIT_SS:
                {
                    /* If the voltage reached the driven voltage level send the PS_RDY message 
                       Checking VBUS Voltage is not needed for PPS */
                    if(((DPM_PD_FIXED_SUPPLY_CONTRACT == DPM_GET_CURRENT_EXPLICIT_CONTRACT(u8PortNum)) && 
                            (DPM_GET_VOLTAGE_FROM_PDO_MILLI_V(gasDPM[u8PortNum].u32NegotiatedPDO) == DPM_GetVBUSVoltage(u8PortNum))) || 
                            (DPM_PD_PPS_CONTRACT == DPM_GET_CURRENT_EXPLICIT_CONTRACT(u8PortNum)))
                    {
                        /* Kill tSrcReady timer in case of Fixed supply. 
                           tPpsSrcTransSmall or tPpsSrcTransLarge Timer in case of PPS. */
                        PE_KillPolicyEngineTimer (u8PortNum);
                        DEBUG_PRINT_PORT_STR (u8PortNum,"PE_SRC_TRANSITION_SUPPLY-EXIT_SS\r\n");
                        u32TransmitHeader = PRL_FormSOPTypeMsgHeader (u8PortNum, PE_CTRL_PS_RDY, \
                                                PE_OBJECT_COUNT_0, PE_NON_EXTENDED_MSG);

                        u8TransmitSOP = PRL_SOP_TYPE;
                        u32pTransmitDataObj = NULL;
                        pfnTransmitCB = PE_StateChange_TransmitCB;

                        u32TransmitTmrIDTxSt = PRL_BUILD_PKD_TXST_U32( ePE_SRC_READY, ePE_SRC_READY_ENTRY_SS, \
                                                        ePE_SRC_HARD_RESET, ePE_SRC_HARD_RESET_ENTRY_SS);                     

                        u8IsTransmit = TRUE;
                        gasPolicyEngine[u8PortNum].ePESubState = ePE_SRC_TRANSITION_SUPPLY_IDLE_SS;
                    }
                    
                    break;  
                }
                    
                case ePE_SRC_TRANSITION_SUPPLY_IDLE_SS:
                {
                    /* Hook to notify PE state machine entry into idle substate */
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
        
		/***************** PE_SRC_CAPABILITY_RESPONSE ******************/
        case ePE_SRC_CAPABILITY_RESPONSE:
        {
            switch(gasPolicyEngine[u8PortNum].ePESubState)
            {
                case ePE_SRC_CAPABILITY_RESPONSE_ENTRY_SS:
                {
					/* If Explicit Contract already initiated, Send the reject message and set the
						PE state to PE_SRC_READY*/
                    if((gasPolicyEngine[u8PortNum].u8PEPortSts & PE_EXPLICIT_CONTRACT))
                    {
                        u32TransmitTmrIDTxSt = PRL_BUILD_PKD_TXST_U32( ePE_SRC_READY, \
                                                    ePE_SRC_READY_ENTRY_SS, ePE_SEND_SOFT_RESET, \
                                                    ePE_SEND_SOFT_RESET_SOP_SS);
                    }
                    
					/* If Explicit Contract is not initiated, Send the reject message and set the
						PE state to PE_SRC_WAIT_NEW_CAPABILITIES*/
                    else
                    {
						/* Change the state to PE_SRC_WAIT_NEW_CAPABILITIES to wait for capabilities change */
                        u32TransmitTmrIDTxSt = PRL_BUILD_PKD_TXST_U32( ePE_SRC_WAIT_NEW_CAPABILITIES, \
                                                    NULL, ePE_SEND_SOFT_RESET, \
                                                    ePE_SEND_SOFT_RESET_SOP_SS);
                    }
                    
					/* Send Reject message for Requested Invalid Capability */
                    u32TransmitHeader = PRL_FormSOPTypeMsgHeader (u8PortNum, PE_CTRL_REJECT, \
                                        PE_OBJECT_COUNT_0, PE_NON_EXTENDED_MSG);

                    u8TransmitSOP = PRL_SOP_TYPE;
                    u32pTransmitDataObj = NULL;
                    pfnTransmitCB = PE_StateChange_TransmitCB;
                    
                    u8IsTransmit = TRUE;
                    
                    /* RDO Reject message has been sent to last Request made by attached Sink */
                    gasCfgStatusData.sPerPortData[u8PortNum].u32PortConnectStatus |= 
                                             DPM_PORT_AS_SRC_RDO_REJECTED_STATUS;
                    
                    gasPolicyEngine[u8PortNum].ePESubState = ePE_SRC_CAPABILITY_RESPONSE_IDLE_SS;
                    
                    break;  
                }
                
                case ePE_SRC_CAPABILITY_RESPONSE_IDLE_SS:
                {
                    /* Hook to notify PE state machine entry into idle substate */
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
        
		/************** PE_SRC_READY *****************/
        case ePE_SRC_READY:
        {
            switch(gasPolicyEngine[u8PortNum].ePESubState)
            {
                case ePE_SRC_READY_ENTRY_SS:
                {
                    DEBUG_PRINT_PORT_STR (u8PortNum,"PE_SRC_READY-ENTRY_SS\r\n");
                    
#if (TRUE == CONFIG_HOOK_DEBUG_MSG)
                    u32PDODebug = gasDPM[u8PortNum].u32NegotiatedPDO;
                    DEBUG_PRINT_PORT_UINT32_STR( u8PortNum, "PDPWR", u32PDODebug, 1, "\r\n");
#endif
					
					/* Set the PD contract as Explicit Contract */
                    gasPolicyEngine[u8PortNum].u8PEPortSts |= PE_EXPLICIT_CONTRACT;                    
					
                    /* Update the Port Connection Status register : As Source, 
                       USB PD connection established, Power request has been made, 
                       accepted and PS_RDY message sent */
                    gasCfgStatusData.sPerPortData[u8PortNum].u32PortConnectStatus |= 
                                        DPM_PORT_AS_SRC_PD_CONTRACT_GOOD_STATUS; 
  
                    /* Reset New PDO Parameters if renegotiation was initiated by DPM
                       TODO: <DPM_Policy> Take care of this while implementing DPM Client request
                       functionality */                    
                    if ((TRUE == DPM_GET_NEW_PDO_STATUS(u8PortNum)) && 
                             (DPM_INT_EVT_INITIATE_RENEGOTIATION == (gasDPM[u8PortNum].u8InternalEvntInProgress & DPM_INT_EVT_INITIATE_RENEGOTIATION)))
                    {                                                
                        DPM_ResetNewPDOParameters(u8PortNum);
                    }                    
                    
                    DPM_EnablePowerFaultDetection(u8PortNum);
                    
#if (TRUE == INCLUDE_PD_SOURCE_PPS)                    
                    /* Start the SourcePPSCommTimer in case the current explicit contract 
                       is for a PPS APDO. Send Hard Reset when SourcePPSCommTimer expires*/
                    if (DPM_PD_PPS_CONTRACT == DPM_GET_CURRENT_EXPLICIT_CONTRACT(u8PortNum))
                    {
                        gasPolicyEngine[u8PortNum].u8PETimerID = PDTimer_Start (
                                                                  (PE_SOURCE_PPS_COMM_TIMEOUT_MS),
                                                                  PE_SSChngAndTimeoutValidate_TimerCB,u8PortNum,  
                                                                  (UINT8) ePE_SRC_HARD_RESET_ENTRY_SS);
                        /* Hook to notify PE state machine entry into idle sub-state */
                        MCHP_PSF_HOOK_NOTIFY_IDLE(u8PortNum, eIDLE_PE_NOTIFY);
                    }                    
#endif                     
                    gasPolicyEngine[u8PortNum].ePESubState = ePE_SRC_READY_END_AMS_SS;
                    
                    /* Notify that PD contract is established*/    
                    (void)DPM_NotifyClient(u8PortNum, eMCHP_PSF_PD_CONTRACT_NEGOTIATED);
                           
                    break;
                }
                case ePE_SRC_READY_END_AMS_SS:
                {
                    /*This Sub-state shall be assigned if the AMS is started by PSF*/
#if (TRUE == INCLUDE_PD_3_0)
					/* Collision avoidance - Rp value set to TYPEC_SINK_TXOK */
                    PRL_SetCollisionAvoidance (u8PortNum, TYPEC_SINK_TXOK);
#endif
                    if (gasDPM[u8PortNum].u8InternalEvntInProgress)
                    {
                        gasDPM[u8PortNum].u8InternalEvntInProgress = RESET_TO_ZERO;
                    }
                    else
                    {
                        /* Hook to notify PSF is IDLE */
                        MCHP_PSF_HOOK_NOTIFY_IDLE(u8PortNum, eMCHP_PSF_IDLE);
                    
                    }

                    gasPolicyEngine[u8PortNum].ePESubState = ePE_SRC_READY_IDLE_SS;
                    /* Hook to notify PE state machine entry into idle sub-state */
                    MCHP_PSF_HOOK_NOTIFY_IDLE(u8PortNum, eIDLE_PE_NOTIFY);
                    break;
                }
                case ePE_SRC_READY_IDLE_SS:
                {
                    /*Idle State*/
                    break;
                }
                
                default:
                {
                    break;
                }
            }
            break;
        }
        
		/************* PE_SRC_WAIT_NEW_CAPABILITIES **************/
        case ePE_SRC_WAIT_NEW_CAPABILITIES:
        {
			/* Since our source capabilities are fixed this state will be in idle*/
            /* Hook to notify PE state machine entry into idle substate */
            MCHP_PSF_HOOK_NOTIFY_IDLE(u8PortNum, eIDLE_PE_NOTIFY);
            break;  
        }
        
		/*************** PE_SRC_DISCOVERY **********************/
        case ePE_SRC_DISCOVERY:
        {
            switch(gasPolicyEngine[u8PortNum].ePESubState)
            {
                case ePE_SRC_DISCOVERY_ENTRY_SS:
                {
                    DEBUG_PRINT_PORT_STR (u8PortNum,"PE_SRC_DISCOVERY-ENTRY_SS\r\n");
					
					/* Start Source Capability  Timer, if timed out set the PE sub-state to 
						ePE_SRC_DISCOVERY_EXIT_SS */
                    gasPolicyEngine[u8PortNum].u8PETimerID = PDTimer_Start (
                                                              (PE_SOURCECAPABILITY_TIMEOUT_MS),
                                                              PE_SubStateChange_TimerCB,u8PortNum,  
                                                              (UINT8) ePE_SRC_DISCOVERY_EXIT_SS);
                    
                    gasPolicyEngine[u8PortNum].ePESubState = ePE_SRC_DISCOVERY_IDLE_SS;
                    break;  
                }
                
                case ePE_SRC_DISCOVERY_IDLE_SS:
                { 
                    /* Wait for Timeout */
                    /* Hook to notify PE state machine entry into idle sub-state */
                    MCHP_PSF_HOOK_NOTIFY_IDLE(u8PortNum, eIDLE_PE_NOTIFY);
                    break;  
                }
                
                case ePE_SRC_DISCOVERY_EXIT_SS:
                {
                    DEBUG_PRINT_PORT_STR (u8PortNum,"PE_SRC_DISCOVERY-EXIT_SS\r\n");
					
					/* If Caps Counter value reaches nCapsCount or Hard Reset Counter reaches
						nHardResetCount, NoResponse timer timed out and PD is not connected change the 
						PE state to ePE_SRC_DISABLED*/
                    if((gasPolicyEngine[u8PortNum].u8CapsCounter > PE_N_CAPSCOUNT) ||
                       ((gasPolicyEngine[u8PortNum].u8HardResetCounter > PE_N_HARD_RESET_COUNT) &&
                       (!(gasPolicyEngine[u8PortNum].u8PEPortSts & PE_PDCONNECTED_STS_MASK)) &&
                       (gasPolicyEngine[u8PortNum].u8PEPortSts & PE_NO_RESPONSE_TIMEDOUT)))
                    {
                        gasPolicyEngine[u8PortNum].ePEState = ePE_SRC_DISABLED;
                        MCHP_PSF_NOTIFY_CALL_BACK (u8PortNum, eMCHP_PSF_PE_SRC_DISABLED);
                    }
                    
                    else
                    {
                        gasPolicyEngine[u8PortNum].ePEState = ePE_SRC_SEND_CAPABILITIES;
                        gasPolicyEngine[u8PortNum].ePESubState = ePE_SRC_SEND_CAP_ENTRY_SS;
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
        
		/*************** PE_SRC_HARD_RESET ******************/
        case ePE_SRC_HARD_RESET:
        {
            switch(gasPolicyEngine[u8PortNum].ePESubState)
            {
                case ePE_SRC_HARD_RESET_ENTRY_SS:
                {
                    DEBUG_PRINT_PORT_STR (u8PortNum,"PE_SRC_HARD_RESET-ENTRY_SS\r\n");
                    
                    /* No request has been made by Sink within tSenderResponse */
                    if (gasPolicyEngine[u8PortNum].u8PEPortSts & PE_PDCONNECTED_STS_MASK)
                    {
                        gasCfgStatusData.sPerPortData[u8PortNum].u32PortConnectStatus &= 
                                        ~(DPM_PORT_AS_SRC_RDO_ACCEPTED_STATUS | DPM_PORT_AS_SRC_RDO_REJECTED_STATUS);
                    }
                    
					/* Kill the policy engine timer */
                    PE_KillPolicyEngineTimer (u8PortNum);  
                 
                    /* Send Hard Reset to Port Partner*/
                    PRL_SendCableorHardReset(u8PortNum, PRL_SEND_HARD_RESET, NULL, 0);	

                    /* Increment the HardReset Counter */
                    gasPolicyEngine[u8PortNum].u8HardResetCounter++;
                    
                    /* Start the PSHardResetTimer, if timed out set the PE sub-state to
						ePE_SRC_HARD_RESET_EXIT_SS */
                    gasPolicyEngine[u8PortNum].u8PETimerID = PDTimer_Start (
                                                              (PE_PSHARDRESET_TIMEOUT_MS),
                                                              PE_SubStateChange_TimerCB, u8PortNum,  
                                                              (UINT8) ePE_SRC_HARD_RESET_EXIT_SS);
                    
                    gasPolicyEngine[u8PortNum].ePESubState = ePE_SRC_HARD_RESET_IDLE_SS;
                    
                    break;  
                }
                
                case ePE_SRC_HARD_RESET_IDLE_SS:
                {
                    /* Wait for PSHardResetTimer to times out */
                    /* Hook to notify PE state machine entry into idle sub-state */
                    MCHP_PSF_HOOK_NOTIFY_IDLE(u8PortNum, eIDLE_PE_NOTIFY);
                    break;  
                }
                
                case ePE_SRC_HARD_RESET_EXIT_SS:
                {
                    DEBUG_PRINT_PORT_STR (u8PortNum,"PE_SRC_HARD_RESET-EXIT_SS\r\n");
                    gasPolicyEngine[u8PortNum].ePEState = ePE_SRC_TRANSITION_TO_DEFAULT;
                    gasPolicyEngine[u8PortNum].ePESubState = ePE_SRC_TRANSITION_TO_DEFAULT_ENTRY_SS;
                    break;
                }
                
                default:
                {
                    break;
                }             
            }
            
            break;  
        }
        
		/************** PE_SRC_HARD_RESET_RECEIVED ****************/
         case ePE_SRC_HARD_RESET_RECEIVED:
        {
            switch(gasPolicyEngine[u8PortNum].ePESubState)
            {
                case ePE_SRC_HARD_RESET_RECEIVED_ENTRY_SS:
                {
                    DEBUG_PRINT_PORT_STR (u8PortNum,"PE_SRC_HARD_RESET_RECEIVED-ENTRY_SS\r\n");
                    
                    /* Kill All the Active Policy Engine timers for the port */                   
                    PE_KillPolicyEngineTimer (u8PortNum);  
                                        
                    /* Start PSHardResetTimer, if timed out set PE sub-state to
						ePE_SRC_HARD_RESET_RECEIVED_EXIT_SS */
                    gasPolicyEngine[u8PortNum].u8PETimerID = PDTimer_Start (
                                                              (PE_PSHARDRESET_TIMEOUT_MS),
                                                              PE_SubStateChange_TimerCB, u8PortNum,  
                                                              (UINT8) ePE_SRC_HARD_RESET_RECEIVED_EXIT_SS);
                    
                    gasPolicyEngine[u8PortNum].ePESubState = ePE_SRC_HARD_RESET_RECEIVED_IDLE_SS;
                    
                    break;  
                }
                
                case ePE_SRC_HARD_RESET_RECEIVED_IDLE_SS:
                {
                    /* Wait for PSHardResetTimer */
                    /* Hook to notify PE state machine entry into idle sub-state */
                    MCHP_PSF_HOOK_NOTIFY_IDLE(u8PortNum, eIDLE_PE_NOTIFY);
                    break;
                }
                
                case ePE_SRC_HARD_RESET_RECEIVED_EXIT_SS:
                {
                    DEBUG_PRINT_PORT_STR (u8PortNum,"PE_SRC_HARD_RESET_RECEIVED-EXIT_SS\r\n");
                    gasPolicyEngine[u8PortNum].ePEState = ePE_SRC_TRANSITION_TO_DEFAULT;
                    gasPolicyEngine[u8PortNum].ePESubState = ePE_SRC_TRANSITION_TO_DEFAULT_ENTRY_SS;  
                    break;
                }
                
                default:
                {
                    break;
                }
            }
            break;    
        }
        
		/************ PE_SRC_TRANSITION_TO_DEFAULT *****************/
        case ePE_SRC_TRANSITION_TO_DEFAULT:
        {
            switch(gasPolicyEngine[u8PortNum].ePESubState)
            {
                case ePE_SRC_TRANSITION_TO_DEFAULT_ENTRY_SS:
                {
                    DEBUG_PRINT_PORT_STR (u8PortNum,"PE_SRC_TRANSITION_TO_DEFAULT-ENTRY_SS\r\n");
                    
					/* Turn Off VCONN if the port is currently supplying VCONN */
                    if(DPM_IsPortVCONNSource(u8PortNum))
                    {
                        DPM_VCONNOnOff(u8PortNum, DPM_VCONN_OFF);
                    }
					
					/* Turn Off VBUS */
                    DPM_TypeCSrcVBus5VOnOff(u8PortNum, DPM_VBUS_OFF);
                    
                     /*Start the VBUS OFF timer for monitoring the time taken for 
                    power module to reach vSafe0V*/
                    gasPolicyEngine[u8PortNum].u8PETimerID = PDTimer_Start (
                                                              (TYPEC_VBUS_OFF_TIMER_MS),
                                                              DPM_VBUSorVCONNOnOff_TimerCB, u8PortNum,  
                                                              (UINT8)SET_TO_ZERO);
                   
                    gasPolicyEngine[u8PortNum].ePESubState = ePE_SRC_TRANSITION_TO_DEFAULT_WAIT_FOR_VBUS_OFF_SS;
                    /* Hook to notify PE state machine entry into idle sub-state */
                    MCHP_PSF_HOOK_NOTIFY_IDLE(u8PortNum, eIDLE_PE_NOTIFY);
                
                    break;  
                }
                
                case ePE_SRC_TRANSITION_TO_DEFAULT_WAIT_FOR_VBUS_OFF_SS:
                {                                  
                    if(TYPEC_VBUS_0V == DPM_GetVBUSVoltage(u8PortNum))
                    {
                        /*Kill the VBUS ON timer since vSafe0V is reached*/
                        PE_KillPolicyEngineTimer (u8PortNum);
                        
                        /*Start the VCONN_OFF timer*/
                        /*This Timeout is implemented outside of the PD Specification to track 
                        VCONN Turn OFF error*/
                        gasPolicyEngine[u8PortNum].u8PETimerID = PDTimer_Start (\
                                                                  PE_VCONNOFF_TIMEOUT_MS,\
                                                                  DPM_VBUSorVCONNOnOff_TimerCB,\
                                                                  u8PortNum,\
                                                                  (UINT8)SET_TO_ZERO);
                        /* Hook to notify PE state machine entry into idle sub-state */
                        MCHP_PSF_HOOK_NOTIFY_IDLE(u8PortNum, eIDLE_PE_NOTIFY);
                        
#if (TRUE == CONFIG_HOOK_DEBUG_MSG)
                        u32PDODebug = SET_TO_ZERO;
                        DEBUG_PRINT_PORT_UINT32_STR( u8PortNum, "PDPWR", u32PDODebug, 1, "\r\n");
#endif
                        gasPolicyEngine[u8PortNum].ePESubState = ePE_SRC_TRANSITION_TO_DEFAULT_WAIT_FOR_VCONN_OFF_SS;
                     
                    }
                    break;  
                }
                
                case ePE_SRC_TRANSITION_TO_DEFAULT_WAIT_FOR_VCONN_OFF_SS:
                {                    
                     if(!DPM_IsPortVCONNSource(u8PortNum))
                     {                       
                        /*Stop the VCONN_OFF timer*/
                        PE_KillPolicyEngineTimer (u8PortNum);
                        
                        /* Start tSRCRecovery timer, if timed-out set the PE sub-state to
                        ePE_SRC_TRANSITION_TO_DEFAULT_POWER_ON_SS*/
                        gasPolicyEngine[u8PortNum].u8PETimerID = PDTimer_Start (
                                                            (PE_SRCRECOVER_TIMEOUT_MS),
                                                            PE_SubStateChange_TimerCB,u8PortNum,  
                                                            (UINT8) ePE_SRC_TRANSITION_TO_DEFAULT_SRCRECOVER_TIMEOUT_SS);

                        gasPolicyEngine[u8PortNum].ePESubState = ePE_SRC_TRANSITION_TO_DEFAULT_IDLE_SS;                    
                     }
                    
                    break;            
                }
                
                case ePE_SRC_TRANSITION_TO_DEFAULT_IDLE_SS:
                {
                    /* Wait for SrcRecoverTimer to times out */
                    /* Hook to notify PE state machine entry into idle sub-state */
                    MCHP_PSF_HOOK_NOTIFY_IDLE(u8PortNum, eIDLE_PE_NOTIFY);
                    break;  
                }
                
                case ePE_SRC_TRANSITION_TO_DEFAULT_SRCRECOVER_TIMEOUT_SS:
                {
                    /* SrcRecover timer timed out - Added for Power fault handling*/
                    gasPolicyEngine[u8PortNum].ePESubState = ePE_SRC_TRANSITION_TO_DEFAULT_POWER_ON_SS;
                    break;  
                }
                
                case ePE_SRC_TRANSITION_TO_DEFAULT_POWER_ON_SS:
                {
                    DEBUG_PRINT_PORT_STR (u8PortNum,"PE_SRC_TRANSITION_TO_DEFAULT-POWER_ON_SS\r\n");
 
#if (TRUE == INCLUDE_POWER_FAULT_HANDLING)
                    
					/* Turn On VConn if E-Cable detected and VCONN Good to Supply flag is true*/
                    if(u8RaPresence && gasDPM[u8PortNum].u8VCONNGoodtoSupply)
                    {
#else 
                     /* Turn On VConn if E-Cable detected*/
                    if(u8RaPresence)
                    {
#endif
                        DPM_VCONNOnOff(u8PortNum, DPM_VCONN_ON);
                    }
					
					/* Turn On VBus */
                    DPM_TypeCSrcVBus5VOnOff(u8PortNum, DPM_VBUS_ON);
                    
                     /*Start the VBUS ON timer for monitoring the time taken for 
                    power module to reach vSafe5V*/
                    gasPolicyEngine[u8PortNum].u8PETimerID = PDTimer_Start (
                                                              (TYPEC_VBUS_ON_TIMER_MS),
                                                              DPM_VBUSorVCONNOnOff_TimerCB, u8PortNum,  
                                                              (UINT8)SET_TO_ZERO);
                    /* Hook to notify PE state machine entry into idle sub-state */
                    MCHP_PSF_HOOK_NOTIFY_IDLE(u8PortNum, eIDLE_PE_NOTIFY);
                    
                    gasPolicyEngine[u8PortNum].ePESubState = ePE_SRC_TRANSITION_TO_DEFAULT_WAIT_FOR_VBUS_ON_SS;
                    
                    break;  
                }
                
                case ePE_SRC_TRANSITION_TO_DEFAULT_WAIT_FOR_VBUS_ON_SS:
                {
                    if(TYPEC_VBUS_5V == DPM_GetVBUSVoltage(u8PortNum))
                    {
                        DEBUG_PRINT_PORT_STR (u8PortNum,"ePE_SRC_TRANSITION_TO_DEFAULT_CHECK_VBUS_ON_SS\r\n");
#if (TRUE == CONFIG_HOOK_DEBUG_MSG)                        
                        switch((gasTypeCcontrol[u8PortNum].u8PortSts & TYPEC_CURR_RPVAL_MASK) >> TYPEC_CURR_RPVAL_POS)
                        {
                            case TYPEC_DFP_DEFAULT_CURRENT:
                            {
                                u32PDODebug = DPM_DEBUG_PDO_5V_9MA;
                                break;  
                            }
                            
                            case TYPEC_DFP_1A5_CURRENT:
                            {
                                u32PDODebug = DPM_DEBUG_PDO_5V_1P5A;
                                break;  
                            }
                            
                            case TYPEC_DFP_3A0_CURRENT:
                            {
                                u32PDODebug = DPM_DEBUG_PDO_5V_3A;
                                break;  
                            }
                        }
#endif                        
                        
                        /*Kill the VBUS ON timer since vSafe5V is reached*/
                        PE_KillPolicyEngineTimer (u8PortNum);

#if (TRUE == CONFIG_HOOK_DEBUG_MSG)
                        DEBUG_PRINT_PORT_UINT32_STR( u8PortNum, "PDPWR", u32PDODebug, 1, "\r\n");
#endif
                        /* Inform Protocol Layer about Hard Reset Complete */
                        PRL_OnHardResetComplete(u8PortNum);
						
						/* Enable Power fault thresholds for TYPEC_VBUS_5V*/
                        TypeC_ConfigureVBUSThr(u8PortNum, TYPEC_VBUS_5V,gasDPM[u8PortNum].u16MaxCurrSupportedin10mA*DPM_10mA, TYPEC_CONFIG_PWR_FAULT_THR);
                        
#if (TRUE == INCLUDE_POWER_FAULT_HANDLING)   
                        
                        if(u8RaPresence && gasDPM[u8PortNum].u8VCONNGoodtoSupply)
                        {
#else 
                        /* Turn On VConn if E-Cable detected*/
                        if(u8RaPresence)
                        {
#endif                           
                            /*Start the VCONN ON timer for monitoring the time taken for 
                            VCONN to reach its Min value*/
                            /*This Timeout is implemented outside of the PD Specification to track 
                            VCONN Turn ON error*/
                            gasPolicyEngine[u8PortNum].u8PETimerID = PDTimer_Start (
                                                             TYPEC_VCONNON_TIMEOUT_MS,
                                                             DPM_VCONNONError_TimerCB, u8PortNum,  
                                                             (UINT8)SET_TO_ZERO);
                        
                            gasPolicyEngine[u8PortNum].ePESubState = ePE_SRC_TRANSITION_TO_DEFAULT_WAIT_FOR_VCONN_ON_SS;
                            /* Hook to notify PE state machine entry into idle sub-state */
                            MCHP_PSF_HOOK_NOTIFY_IDLE(u8PortNum, eIDLE_PE_NOTIFY);
                        }
                        else
                        {
                             gasPolicyEngine[u8PortNum].ePESubState = ePE_SRC_TRANSITION_TO_DEFAULT_EXIT_SS;                         
                        }
                        
                    }
                    break;
                }
                
                case ePE_SRC_TRANSITION_TO_DEFAULT_WAIT_FOR_VCONN_ON_SS:
                {                
                    if(DPM_IsPortVCONNSource(u8PortNum))
                    {                          
                        PE_KillPolicyEngineTimer (u8PortNum);                        
                        gasPolicyEngine[u8PortNum].ePESubState = ePE_SRC_TRANSITION_TO_DEFAULT_EXIT_SS;                                                          
                    }                    
                    break;                  
                }
                case ePE_SRC_TRANSITION_TO_DEFAULT_EXIT_SS:
                {                  
                    /*Start NoResponse timer */
                    gasPolicyEngine[u8PortNum].u8PENoResponseTimerID = PDTimer_Start (
                                                        (PE_NORESPONSE_TIMEOUT_MS),
                                                        PE_NoResponse_TimerCB, u8PortNum,  
                                                        (UINT8)SET_TO_ZERO);
                    
                    gasPolicyEngine[u8PortNum].ePEState = ePE_SRC_STARTUP;
                    gasPolicyEngine[u8PortNum].ePESubState = ePE_SRC_STARTUP_ENTRY_SS;
                    break;
                }
               
                default:
                {
                    break;  
                }
            }
            break;  
        }
		/************ PE_SRC_GET_SINK_CAP *****************/ 
        case ePE_SRC_GET_SINK_CAP: 
        {
           switch(gasPolicyEngine[u8PortNum].ePESubState) 
           {
                case ePE_SRC_GET_SINK_CAP_ENTRY_SS:
                {
                    DEBUG_PRINT_PORT_STR (u8PortNum,"ePE_SRC_GET_SINK_CAP_ENTRY_SS\r\n"); 
                    
                    u32TransmitHeader = PRL_FormSOPTypeMsgHeader (u8PortNum, PE_CTRL_GET_SINK_CAP, \
                                            PE_OBJECT_COUNT_0, PE_NON_EXTENDED_MSG);

                    u8TransmitSOP = PRL_SOP_TYPE;
                    u32pTransmitDataObj = NULL;
                    pfnTransmitCB = PE_StateChange_TransmitCB;                 
                        
                    u32TransmitTmrIDTxSt = PRL_BUILD_PKD_TXST_U32(ePE_SRC_GET_SINK_CAP, ePE_SRC_GET_SINK_CAP_GOODCRC_RCVD_SS, \
                                                    ePE_SEND_SOFT_RESET, ePE_SEND_SOFT_RESET_SOP_SS);
                                                
                    u8IsTransmit = TRUE;                                             
                   
                    gasPolicyEngine[u8PortNum].ePESubState = ePE_SRC_GET_SINK_CAP_IDLE_SS;
                    
                    break; 
                }                   
                
                case ePE_SRC_GET_SINK_CAP_GOODCRC_RCVD_SS: 
                {
                    DEBUG_PRINT_PORT_STR (u8PortNum,"ePE_SRC_GET_SINK_CAP_GOODCRC_RECEIVED_SS\r\n"); 
                    gasPolicyEngine[u8PortNum].ePESubState = ePE_SRC_GET_SINK_CAP_IDLE_SS; 
                    
                    /* Start Sender Response timer and Set the timer callback to transition to 
					ePE_SRC_GET_SINK_CAP_TIMER_TIMEDOUT sub state if timeout happens */
                    gasPolicyEngine[u8PortNum].u8PETimerID = PDTimer_Start (
                                                           PE_SENDERRESPONSE_TIMEOUT_MS,
                                                            PE_SubStateChange_TimerCB, u8PortNum,  
                                                            (UINT8)ePE_SRC_GET_SINK_CAP_NO_RESPONSE_SS); 
                    /* Hook to notify PE state machine entry into idle substate */
                    MCHP_PSF_HOOK_NOTIFY_IDLE(u8PortNum, eIDLE_PE_NOTIFY);
                    
                    break; 
                }
                  
                case ePE_SRC_GET_SINK_CAP_NO_RESPONSE_SS:
                {
                   /* Sink caps not received within tSenderResponse. Send 
                      SINK_CAPS_NOT_RECEIVED notification and move to 
                      PE_SRC_READY state */ 
                   DEBUG_PRINT_PORT_STR (u8PortNum,"ePE_SRC_GET_SINK_CAP_TIMER_TIMEDOUT_SS\r\n"); 
                   gasPolicyEngine[u8PortNum].ePEState = ePE_SRC_READY; 
                   gasPolicyEngine[u8PortNum].ePESubState = ePE_SRC_READY_END_AMS_SS;
                   
                   (void)DPM_NotifyClient(u8PortNum, eMCHP_PSF_SINK_CAPS_NOT_RCVD);
                   
                   break;  
                }   
                
                case ePE_SRC_GET_SINK_CAP_RESPONSE_RCVD_SS:
                {
                    DEBUG_PRINT_PORT_STR (u8PortNum,"ePE_SRC_GET_SINK_CAP_RESPONSE_RECEIVED_SS\r\n"); 
                    /* Store the received sink caps and send notification */                                               
                    gasCfgStatusData.sPerPortData[u8PortNum].u8PartnerPDOCnt = PRL_GET_OBJECT_COUNT(u32Header);
    
                    (void)MCHP_PSF_HOOK_MEMCPY(gasCfgStatusData.sPerPortData[u8PortNum].u32aPartnerPDO, pu8DataBuf,
                                 (gasCfgStatusData.sPerPortData[u8PortNum].u8PartnerPDOCnt * BYTE_LEN_4));           
                    
                    gasPolicyEngine[u8PortNum].ePEState = ePE_SRC_READY; 
                    gasPolicyEngine[u8PortNum].ePESubState = ePE_SRC_READY_END_AMS_SS;
                    
                    (void)DPM_NotifyClient(u8PortNum, eMCHP_PSF_SINK_CAPS_RCVD);
                    
                    break; 
                }
                
                case ePE_SRC_GET_SINK_CAP_IDLE_SS: 
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

		/************* PE_SRC_DISABLED ***************/
        case ePE_SRC_DISABLED:
        {
            /* Port Disabled */
            break;  
        }
        
		/*************** PE_SRC_VDM_IDENTITY_REQUEST *****************/
        case ePE_SRC_VDM_IDENTITY_REQUEST:
        {            
            switch(gasPolicyEngine[u8PortNum].ePESubState)
            {
                case ePE_SRC_VDM_IDENTITY_REQUEST_ENTRY_SS:
                {
                    DEBUG_PRINT_PORT_STR (u8PortNum,"PE_SRC_VDM_IDENTITY_REQUEST-ENTRY_SS\r\n");
                    
                    /* Choosing VDM version as per Current spec revision */
                    if(PD_SPEC_REVISION_2_0 == DPM_GET_CURRENT_PD_SPEC_REV(u8PortNum))
                    {
                        u32VDMHeader = PE_VDM_HEADER_LOW_VER;
                    }                    
                    else
                    {
                        u32VDMHeader = PE_VDM_HEADER_HIGH_VER;
                    }
                    
					/* Send VDM Discover Identity message to E-Cable */
                    u32TransmitHeader = PRL_FormNonSOPTypeMsgHeader(u8PortNum, (UINT8)PE_DATA_VENDOR_DEFINED,  \
                                                                        BYTE_LEN_1, PE_NON_EXTENDED_MSG);
                    u8TransmitSOP = PRL_SOP_P_TYPE;
                    u32pTransmitDataObj = &u32VDMHeader;
                    pfnTransmitCB = PE_StateChange_TransmitCB;

                    u32TransmitTmrIDTxSt = PRL_BUILD_PKD_TXST_U32( ePE_SRC_VDM_IDENTITY_REQUEST, \
                                                ePE_SRC_VDM_IDENTITY_REQUEST_GOODCRC_RCVD_SS, ePE_SRC_VDM_IDENTITY_NAKED, NULL);
              
                    gasPolicyEngine[u8PortNum].u8DiscoverIdentityCounter++;
                    u8IsTransmit = TRUE;
                    
                    gasPolicyEngine[u8PortNum].ePESubState = ePE_SRC_VDM_IDENTITY_REQUEST_IDLE_SS;
                    
                    break;    
                }
            
                case ePE_SRC_VDM_IDENTITY_REQUEST_IDLE_SS:
                {
                    /* Hook to notify PE state machine entry into idle sub-state */
                    MCHP_PSF_HOOK_NOTIFY_IDLE(u8PortNum, eIDLE_PE_NOTIFY);
                    break;
                }
                
                case ePE_SRC_VDM_IDENTITY_REQUEST_GOODCRC_RCVD_SS:
                {
					/* GoodCRC received for VDM Identity request */
                    DEBUG_PRINT_PORT_STR (u8PortNum,"PE_SRC_VDM_IDENTITY_REQUEST-GOODCRC_SS\r\n");
                    
					/* Start the VDMIDentityRequest Sender Response timer, if timed out set the PE
						sub-state to ePE_SRC_VDM_IDENTITY_REQUEST_SENDER_RESPONSE_TIMEOUT */
                    gasPolicyEngine[u8PortNum].u8PETimerID = PDTimer_Start (
                                                            (PE_VDMRESPONSE_TIMEOUT_MS),
                                                            PE_SSChngAndTimeoutValidate_TimerCB,u8PortNum,  
                                                            (UINT8)ePE_SRC_VDM_IDENTITY_REQUEST_NO_RESPONSE_SS);
                    gasPolicyEngine[u8PortNum].ePESubState = ePE_SRC_VDM_IDENTITY_REQUEST_IDLE_SS;
                    break;  
                }
                
                case ePE_SRC_VDM_IDENTITY_REQUEST_NO_RESPONSE_SS:
                {
                    gasPolicyEngine[u8PortNum].ePEState = ePE_SRC_VDM_IDENTITY_NAKED; 
                    break;
                }
                
                default:
                {
                    break;
                }
            }
            break;  
        }
        
		/************ PE_SRC_VDM_IDENTITY_ACKED ****************/
        case ePE_SRC_VDM_IDENTITY_ACKED:
        {
            DEBUG_PRINT_PORT_STR (u8PortNum,"PE_SRC_VDM_IDENTITY_ACKED\r\n");
            
			/* VDM ACK received from cable */
			/* Pass the cable data to the DPM */
            if (PE_VDM_ACK == DPM_StoreCableIdentity(u8PortNum, u8SOPType, (UINT16) u32Header, (UINT32*) pu8DataBuf))
            {
                DPM_UpdatePDSpecRev (u8PortNum, CONFIG_PD_DEFAULT_SPEC_REV);
                gasPolicyEngine[u8PortNum].u8DiscoverIdentityCounter = RESET_TO_ZERO;
                gasPolicyEngine[u8PortNum].ePEState = ePE_SRC_SEND_CAPABILITIES;
                gasPolicyEngine[u8PortNum].ePESubState = ePE_SRC_SEND_CAP_ENTRY_SS;
                
                /* Post eMCHP_PSF_CABLE_IDENTITY_DISCOVERED notification */
                (void)DPM_NotifyClient(u8PortNum, eMCHP_PSF_CABLE_IDENTITY_DISCOVERED);
            }            
            else
            {
                gasPolicyEngine[u8PortNum].ePEState = ePE_SRC_VDM_IDENTITY_NAKED;
            }
            
            break;  
        }
        
        case ePE_SRC_VDM_IDENTITY_NAKED:
        {
            DEBUG_PRINT_PORT_STR (u8PortNum,"PE_SRC_VDM_IDENTITY_NAKED\r\n");
            
            /* VDM NAK received */
			/* If DiscoverIdentityCounter reaches nDiscoverIdentityCount change the PE state to	
				PE_SRC_SEND_CAPABILITIES */
            if(gasPolicyEngine[u8PortNum].u8DiscoverIdentityCounter > PE_N_DISCOVER_IDENTITY_COUNT)
            {
                DPM_UpdatePDSpecRev (u8PortNum, CONFIG_PD_DEFAULT_SPEC_REV);
                gasPolicyEngine[u8PortNum].u8PEPortSts |= PE_CABLE_RESPOND_NAK;
                gasPolicyEngine[u8PortNum].u8DiscoverIdentityCounter = RESET_TO_ZERO;
                gasPolicyEngine[u8PortNum].ePEState = ePE_SRC_SEND_CAPABILITIES;
                gasPolicyEngine[u8PortNum].ePESubState = ePE_SRC_SEND_CAP_ENTRY_SS;               
                
                /* Post eMCHP_PSF_CABLE_IDENTITY_NAKED notification */
                (void)DPM_NotifyClient(u8PortNum, eMCHP_PSF_CABLE_IDENTITY_NAKED);
            }            
            else
            {
                DPM_UpdatePDSpecRev (u8PortNum, PD_SPEC_REVISION_2_0);
                gasPolicyEngine[u8PortNum].ePEState = ePE_SRC_VDM_IDENTITY_REQUEST;
                gasPolicyEngine[u8PortNum].ePESubState = ePE_SRC_VDM_IDENTITY_REQUEST_ENTRY_SS;
            }
                        
            break;  
        }
        
#if (TRUE == INCLUDE_PD_SOURCE_PPS)        
        
            /************* PE_SRC_SINK_ALERT_RECEIVED *******/
        case ePE_SRC_SINK_ALERT_RECEIVED: 
        {
            DEBUG_PRINT_PORT_STR (u8PortNum,"ePE_SRC_SINK_ALERT_RECEIVED\r\n"); 
            /* Store the Alert Information received from Sink Partner */
            (void)MCHP_PSF_HOOK_MEMCPY(&gasCfgStatusData.sPPSPerPortData[u8PortNum].u32PartnerAlert, 
                                            pu8DataBuf, BYTE_LEN_4);
            /* Move to PE_SRC_READY state */                                
            gasPolicyEngine[u8PortNum].ePEState = ePE_SRC_READY; 
            gasPolicyEngine[u8PortNum].ePESubState = ePE_SRC_READY_END_AMS_SS;
            
            /* Send notification */
            (void)DPM_NotifyClient(u8PortNum, eMCHP_PSF_SINK_ALERT_RCVD);
                               
            break; 
        }       
            /************* ePE_SRC_GET_SINK_STATUS **********/
        case ePE_SRC_GET_SINK_STATUS:
        {
            switch(gasPolicyEngine[u8PortNum].ePESubState)
            {
                case ePE_SRC_GET_SINK_STATUS_ENTRY_SS:
                {
                    DEBUG_PRINT_PORT_STR (u8PortNum,"ePE_SRC_GET_SINK_STATUS_ENTRY_SS\r\n"); 
                    
                    u32TransmitHeader = PRL_FormSOPTypeMsgHeader (u8PortNum, PE_CTRL_GET_STATUS, \
                                            PE_OBJECT_COUNT_0, PE_NON_EXTENDED_MSG);

                    u8TransmitSOP = PRL_SOP_TYPE;
                    u32pTransmitDataObj = NULL;
                    pfnTransmitCB = PE_StateChange_TransmitCB;                 
                        
                    u32TransmitTmrIDTxSt = PRL_BUILD_PKD_TXST_U32(ePE_SRC_GET_SINK_STATUS, ePE_SRC_GET_SINK_STATUS_GOODCRC_RCVD_SS, \
                                                    ePE_SEND_SOFT_RESET, ePE_SEND_SOFT_RESET_SOP_SS);
                                                
                    u8IsTransmit = TRUE;                                             
                   
                    gasPolicyEngine[u8PortNum].ePESubState = ePE_SRC_GET_SINK_STATUS_IDLE_SS;
                    
                    break; 
                }
                
                case ePE_SRC_GET_SINK_STATUS_GOODCRC_RCVD_SS:
                {
                    DEBUG_PRINT_PORT_STR (u8PortNum,"ePE_SRC_GET_SINK_STATUS_GOODCRC_RECEIVED_SS\r\n"); 
                    
                    gasPolicyEngine[u8PortNum].ePESubState = ePE_SRC_GET_SINK_STATUS_IDLE_SS; 
                    
                    /* Start Sender Response timer and Set the timer callback to transition to 
					ePE_SRC_GET_SINK_STATUS_SENDER_RESPONSE_TIMEDOUT_SS sub state if timeout happens */
                    gasPolicyEngine[u8PortNum].u8PETimerID = PDTimer_Start (
                                                            PE_SENDERRESPONSE_TIMEOUT_MS,
                                                            PE_SubStateChange_TimerCB, u8PortNum,  
                                                            (UINT8)ePE_SRC_GET_SINK_STATUS_NO_RESPONSE_SS); 
                    /* Hook to notify PE state machine entry into idle sub-state */
                    MCHP_PSF_HOOK_NOTIFY_IDLE(u8PortNum, eIDLE_PE_NOTIFY);
                    
                    break;
                }
                
                case ePE_SRC_GET_SINK_STATUS_NO_RESPONSE_SS:
                {
                   /* Sink Status not received within tSenderResponse. Send 
                      SINK_STATUS_NOT_RECEIVED notification and move to 
                      PE_SRC_READY state */ 
                   DEBUG_PRINT_PORT_STR (u8PortNum,"ePE_SRC_GET_SINK_STATUS_SENDER_RESPONSE_TIMEDOUT_SS\r\n"); 
                                      
                   gasPolicyEngine[u8PortNum].ePEState = ePE_SRC_READY; 
                   gasPolicyEngine[u8PortNum].ePESubState = ePE_SRC_READY_END_AMS_SS;
                   
                   (void)DPM_NotifyClient(u8PortNum, eMCHP_PSF_SINK_STATUS_NOT_RCVD);
                    
                    break;
                }
                
                case ePE_SRC_GET_SINK_STATUS_RESPONSE_RCVD_SS:
                {
                    DEBUG_PRINT_PORT_STR (u8PortNum,"ePE_SRC_GET_SINK_STATUS_RESPONSE_RECEIVED_SS\r\n"); 
                    
                    /* Store the received Sink Status and send notification */                            
                    (void)MCHP_PSF_HOOK_MEMCPY(gasCfgStatusData.sPPSPerPortData[u8PortNum].u8aPartnerStatus, 
                                 pu8DataBuf, PE_STATUS_DATA_BLOCK_SIZE_IN_BYTES);           

                    gasPolicyEngine[u8PortNum].ePEState = ePE_SRC_READY; 
                    gasPolicyEngine[u8PortNum].ePESubState = ePE_SRC_READY_END_AMS_SS;
                    
                    (void)DPM_NotifyClient(u8PortNum, eMCHP_PSF_SINK_STATUS_RCVD);
                    
                    break;
                }
                
                case ePE_SRC_GET_SINK_STATUS_IDLE_SS:
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
        
        /************* ePE_SRC_SEND_SOURCE_ALERT **********/
        case ePE_SRC_SEND_SOURCE_ALERT:
        {
            switch(gasPolicyEngine[u8PortNum].ePESubState)
            {
                case ePE_SRC_SEND_SOURCE_ALERT_ENTRY_SS: 
                {
                    DEBUG_PRINT_PORT_STR (u8PortNum,"ePE_SRC_SEND_SOURCE_ALERT_ENTRY_SS\r\n"); 
                    
                    /* Obtain the Alert Data Object from DPM */
                    u32DataBlock = DPM_ObtainAlertDO(u8PortNum);
                    
                    u32TransmitHeader = PRL_FormSOPTypeMsgHeader(u8PortNum, PE_DATA_ALERT,  \
                                                 PE_ALERT_DATA_OBJECT_SIZE, PE_NON_EXTENDED_MSG);
                    u8TransmitSOP = PRL_SOP_TYPE;
                    u32pTransmitDataObj = &u32DataBlock;
                    pfnTransmitCB = PE_StateChange_TransmitCB;
                    
                    u32TransmitTmrIDTxSt = PRL_BUILD_PKD_TXST_U32( ePE_SRC_READY, \
                                ePE_SRC_READY_END_AMS_SS, ePE_SEND_SOFT_RESET, \
                                ePE_SEND_SOFT_RESET_SOP_SS);
                    
                    u8IsTransmit = TRUE;
                    
                    gasPolicyEngine[u8PortNum].ePESubState = ePE_SRC_SEND_SOURCE_ALERT_IDLE_SS;
                    
                    break; 
                }
                
                case ePE_SRC_SEND_SOURCE_ALERT_IDLE_SS:
                {
                    /* Idle state to wait for message transmit completion */
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
        
        /************* ePE_SRC_GIVE_SOURCE_STATUS **********/
        case ePE_SRC_GIVE_SOURCE_STATUS:
        {
            switch(gasPolicyEngine[u8PortNum].ePESubState)            
            {
                case ePE_SRC_GIVE_SOURCE_STATUS_ENTRY_SS:
                {
                    DEBUG_PRINT_PORT_STR (u8PortNum,"ePE_SRC_GIVE_SOURCE_STATUS_ENTRY_SS\r\n"); 
                            
                    /* Obtain the Status Data Block from DPM */
                    DPM_ObtainStatusDB(u8PortNum, u8aStatusDB);
                    
                    /* Form Combined Message Header*/
                    u32TransmitHeader =  /* Combined Message Header */
                        PRL_FORM_COMBINED_MSG_HEADER(((1u << PRL_EXTMSG_CHUNKED_BIT_POS) | (PRL_EXTMSG_DATA_FIELD_MASK & PE_STATUS_DATA_BLOCK_SIZE_IN_BYTES)), /* Extended Msg Header*/
                                PRL_FormSOPTypeMsgHeader(u8PortNum,PE_EXT_STATUS,PE_STATUS_DATA_OBJ_CNT, /* Standard Msg Header */
                                            PE_EXTENDED_MSG));

                    u8TransmitSOP = PRL_SOP_TYPE;
                    u32pTransmitDataObj = (UINT32 *)u8aStatusDB;
                    pfnTransmitCB = PE_StateChange_TransmitCB;
                    
                    u32TransmitTmrIDTxSt = PRL_BUILD_PKD_TXST_U32( ePE_SRC_READY, \
                                ePE_SRC_READY_END_AMS_SS, ePE_SEND_SOFT_RESET, \
                                ePE_SEND_SOFT_RESET_SOP_SS);
                    
                    u8IsTransmit = TRUE;
                    
                    gasPolicyEngine[u8PortNum].ePESubState = ePE_SRC_GIVE_SOURCE_STATUS_IDLE_SS;
                                        
                    break; 
                }
                
                case ePE_SRC_GIVE_SOURCE_STATUS_IDLE_SS:
                {
                    /* Idle state to wait for message transmit completion */
                    /* Hook to notify PE state machine entry into idle substate */
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
        
        /************* ePE_SRC_GIVE_PPS_STATUS **********/
        case ePE_SRC_GIVE_PPS_STATUS:
        {
            switch(gasPolicyEngine[u8PortNum].ePESubState)            
            {
                case ePE_SRC_GIVE_PPS_STATUS_ENTRY_SS:
                {
                    DEBUG_PRINT_PORT_STR (u8PortNum,"ePE_SRC_GIVE_PPS_STATUS_ENTRY_SS\r\n"); 
                    
                    /* Obtain the PPS Status Data Block from DPM */
                    u32DataBlock = DPM_ObtainPPSStatusDB(u8PortNum);
                    
                    /* Form Combined Message Header*/
                    u32TransmitHeader =  /* Combined Message Header */
                        PRL_FORM_COMBINED_MSG_HEADER(((1u << PRL_EXTMSG_CHUNKED_BIT_POS) | (PRL_EXTMSG_DATA_FIELD_MASK & PE_PPS_STATUS_DATA_BLOCK_SIZE_IN_BYTES)), /* Extended Msg Header */
                                PRL_FormSOPTypeMsgHeader(u8PortNum,PE_EXT_PPS_STATUS,PE_PPS_STATUS_DATA_OBJ_CNT, /* Standard Msg Header */
                                            PE_EXTENDED_MSG));

                    u8TransmitSOP = PRL_SOP_TYPE;
                    u32pTransmitDataObj = &u32DataBlock;
                    pfnTransmitCB = PE_StateChange_TransmitCB;
                    
                    u32TransmitTmrIDTxSt = PRL_BUILD_PKD_TXST_U32( ePE_SRC_READY, \
                                ePE_SRC_READY_END_AMS_SS, ePE_SEND_SOFT_RESET, \
                                ePE_SEND_SOFT_RESET_SOP_SS);
                    
                    u8IsTransmit = TRUE;
                    
                    gasPolicyEngine[u8PortNum].ePESubState = ePE_SRC_GIVE_PPS_STATUS_IDLE_SS;
                                        
                    break; 
                }
                
                case ePE_SRC_GIVE_PPS_STATUS_IDLE_SS:
                {
                    /* Idle state to wait for message transmit completion */
                    /* Hook to notify PE state machine entry into idle substate */
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
#endif
