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
void PE_SrcRunStateMachine(UINT8 u8PortNum , UINT8 *pu8DataBuf , UINT8 u8SOPType ,UINT32 u32Header)
{
    /* Receive VDM Header */
    UINT32 u32VDMHeader = SET_TO_ZERO;

	/* Source Capabilities PDO Count */
    UINT8 u8SrcPDOCnt = SET_TO_ZERO;

	/* Source Cap Objects */
    UINT32 u32DataObj[7] = {SET_TO_ZERO};

	/* Type-C state and sub-state */
    UINT8 u8TypeCState = TYPEC_INVALID_STATE, u8TypeCSubState = TYPEC_INVALID_STATE;

	/* Transmit Msg Type - SOP SOP' SOP" */
    UINT8 u8TransmitSOP = PRL_SOP_TYPE;

	/* Transmit Msg Header */
	UINT16 u16Transmit_Header = SET_TO_ZERO;

	/* Transmit Data Object */
	UINT32 *u32pTransmit_DataObj = SET_TO_ZERO; 

	/* Transmit Call back */
	PRLTxCallback Transmit_cb = NULL;

	/* Transmit Call back variables */
	UINT32 u32Transmit_TmrID_TxSt = SET_TO_ZERO;

	/* Transmit Flag */
	UINT8 u8IsTransmit = FALSE;

	/* Ra Presence Check */
    UINT8 u8RaPresence = FALSE;

#if (TRUE == CONFIG_HOOK_DEBUG_MSG)    
    /* Added for negotiated PDO debug message */
    UINT32 u32PDODebug = SET_TO_ZERO;
#endif
    
    /* Get the Type-C state from DPM */
    DPM_GetTypeCStates(u8PortNum, &u8TypeCState, &u8TypeCSubState);
    DPM_GetPoweredCablePresence(u8PortNum, &u8RaPresence);
    
	/* Setting Timeout sub-state to invalid state */
    gasPolicy_Engine[u8PortNum].ePETimeoutSubState = ePE_INVALIDSUBSTATE;
    
    /* If port partner detached set the Policy Engine State to PE_SRC_STARTUP */
    if((TYPEC_UNATTACHED_SRC == u8TypeCState) && (ePE_SRC_STARTUP != gasPolicy_Engine[u8PortNum].ePEState))
    {
		/* Kill all the port timers */
        PDTimer_KillPortTimers(u8PortNum);
        gasPolicy_Engine[u8PortNum].u8PETimerID = MAX_CONCURRENT_TIMERS;
        gasPolicy_Engine[u8PortNum].ePEState = ePE_SRC_STARTUP;
        gasPolicy_Engine[u8PortNum].ePESubState = ePE_SRC_STARTUP_ENTRY_SS;
        
        gasPolicy_Engine[u8PortNum].u8HardResetCounter = RESET_TO_ZERO;

    }
     
    /* Source Policy Engine State Machine */
    switch(gasPolicy_Engine[u8PortNum].ePEState)
    {
        /********************PE_SRC_STARTUP*********************/
        case ePE_SRC_STARTUP:
        {
            switch(gasPolicy_Engine[u8PortNum].ePESubState)
            {
                case ePE_SRC_STARTUP_ENTRY_SS:
                {
                    DEBUG_PRINT_PORT_STR (u8PortNum,"PE_SRC_STARTUP-ENTRY_SS\r\n");
					
					/* Reset the Caps Counter value */
                    gasPolicy_Engine[u8PortNum].u8CapsCounter = FALSE;
					
                    gasPolicy_Engine[u8PortNum].u8PEPortSts &= (~PE_PDCONNECTED_STS_MASK);
                    
					/* Set PD Contract as Implicit Contract */
                    gasPolicy_Engine[u8PortNum].u8PEPortSts &= (~PE_EXPLICIT_CONTRACT);
					
					/* Reset the Protocol Layer */
                    PRL_ProtocolReset(u8PortNum);
                    
                    /* Set spec revision to default spec revision in every detach */
                    gasDPM[u8PortNum].u8DPM_Status &= ~DPM_CURR_PD_SPEC_REV_MASK;
                    gasDPM[u8PortNum].u8DPM_Status |= (CONFIG_PD_DEFAULT_SPEC_REV << DPM_CURR_PD_SPEC_REV_POS); 
        
                    /* Spec Rev is updated by PRL*/
                    PRL_UpdateSpecAndDeviceRoles (u8PortNum);
					
                    gasPolicy_Engine[u8PortNum].ePESubState = ePE_SRC_STARTUP_IDLE_SS;
                    
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
                    if(( TYPEC_ATTACHED_SRC == u8TypeCState) && (TYPEC_ATTACHED_SRC_RUN_SM_SS == u8TypeCSubState))
                    {
						/* If E-Cable attached with port partner, before sending the source capabilities message
							Cable discovery identity message will be send to the cable to find the cable capabilities */
                        if(FALSE == u8RaPresence)
                        { 
							/* E-Cable not present, Port partner alone attached */
                            DEBUG_PRINT_PORT_STR (u8PortNum,"PE_SRC_STARTUP-IDLE_SS: Device Attached\r\n");
                            gasPolicy_Engine[u8PortNum].ePEState = ePE_SRC_SEND_CAPABILITIES;
                            gasPolicy_Engine[u8PortNum].ePESubState = ePE_SRC_SEND_CAP_ENTRY_SS;                      
                        }
                        else
                        {
							/* Port partner attached with E-Cable */
                            DEBUG_PRINT_PORT_STR (u8PortNum,"PE_SRC_STARTUP-IDLE_SS: E-Cable and Device Attached\r\n");
                            gasPolicy_Engine[u8PortNum].ePEState = ePE_SRC_VDM_IDENTITY_REQUEST;
                            gasPolicy_Engine[u8PortNum].ePESubState = ePE_SRC_VDM_IDENTITY_REQUEST_ENTRY_SS;
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

            switch(gasPolicy_Engine[u8PortNum].ePESubState)
            {
                case ePE_SRC_SEND_CAP_ENTRY_SS:
                {

                    /* If HardReset counter reaches nHardResetCount and NoResponseTimer timed out */
                    if((gasPolicy_Engine[u8PortNum].u8HardResetCounter > PE_N_HARD_RESET_COUNT) &&
                       (gasPolicy_Engine[u8PortNum].u8PEPortSts & PE_NO_RESPONSE_TIMEDOUT))
                    {
						/* If PD Connected change the Type-C state to TYPEC_ERROR_RECOVERY state */
                        if(gasPolicy_Engine[u8PortNum].u8PEPortSts & PE_PDCONNECTED_STS_MASK)
                        {
                            DPM_SetTypeCState(u8PortNum, TYPEC_ERROR_RECOVERY, TYPEC_ERROR_RECOVERY_ENTRY_SS);
                        }
                        
						/* If PD not connected change the PE state to ePE_SRC_DISABLED */
                        else
                        {
                            gasPolicy_Engine[u8PortNum].ePEState = ePE_SRC_DISABLED; 
                        }
                    }
                 
                    else
                    {
						/* Send Source Capabilities message to Port partner */
                        DEBUG_PRINT_PORT_STR (u8PortNum,"PE_SRC_SEND_CAP-ENTRY_SS: Send Source Capabilities\r\n");
                        DPM_Get_Source_Capabilities(u8PortNum, &u8SrcPDOCnt, u32DataObj);
                        
                        u16Transmit_Header = PRL_FormSOPTypeMsgHeader(u8PortNum, PE_DATA_SOURCE_CAP,  \
                                                                            u8SrcPDOCnt, PE_NON_EXTENDED_MSG);
                        
                        u8TransmitSOP = PRL_SOP_TYPE;
                        u32pTransmit_DataObj = u32DataObj;
                        Transmit_cb = PE_StateChange_TransmitCB;
                        /* Commented for compilation */
                        if(gasPolicy_Engine[u8PortNum].u8PEPortSts & PE_PDCONNECTED_STS_MASK)
                        {
                            /*The Protocol Layer indicates that the Message has not been sent , so
                             Send Soft reset since pd connected presently*/
                            u32Transmit_TmrID_TxSt = PRL_BUILD_PKD_TXST_U32( ePE_SRC_SEND_CAPABILITIES, \
                                                        ePE_SRC_SEND_CAP_GOODCRC_RECEIVED_SS, \
                                                          ePE_SRC_SEND_SOFT_RESET, ePE_SRC_SEND_SOFT_RESET_SOP_SS);                                                  
                          
                        }
                        else
                        {
                          
                            /*The Policy Engine Shall transition to the PE_SRC_Discovery state when:
                              The Protocol Layer indicates that the Message has not been sent and we are presently not Connected
                             */
                            u32Transmit_TmrID_TxSt = PRL_BUILD_PKD_TXST_U32( ePE_SRC_SEND_CAPABILITIES, \
                                                        ePE_SRC_SEND_CAP_GOODCRC_RECEIVED_SS, \
                                                        ePE_SRC_DISCOVERY, ePE_SRC_DISCOVERY_ENTRY_SS);
                        }

                        u8IsTransmit = TRUE;
                        gasPolicy_Engine[u8PortNum].u8CapsCounter ++;
                                            
                        gasPolicy_Engine[u8PortNum].ePESubState = ePE_SRC_SEND_CAP_IDLE_SS;
                    }
                    break;  
                }
                
                case ePE_SRC_SEND_CAP_IDLE_SS:
                {
                    break;  
                }
                
                case ePE_SRC_SEND_CAP_GOODCRC_RECEIVED_SS:
                {
                    DEBUG_PRINT_PORT_STR (u8PortNum,"PE_SRC_SEND_CAP-GOODCRC_RECEIVED_SS\r\n");
                    
                    /* Update Advertised PDO registers */
                    DPM_UpdateAdvertisedPDOParam(u8PortNum); 
                                           
                    /* Reset CapsCounter and HardReset Counter to 0 */
                    gasPolicy_Engine[u8PortNum].u8CapsCounter = RESET_TO_ZERO;
                    gasPolicy_Engine[u8PortNum].u8HardResetCounter = RESET_TO_ZERO;
                    gasPolicy_Engine[u8PortNum].ePESubState = ePE_SRC_SEND_CAP_IDLE_SS;
					
					/* Set PD Status as Connected */
                    gasPolicy_Engine[u8PortNum].u8PEPortSts |= PE_PDCONNECTED_STS_MASK;
                    
					/* Start Sender Response timer and Set the timer callback to transition to 
					ePE_SRC_HARD_RESET sate and ePE_SRC_HARD_RESET_ENTRY_SS sub state if timeout happens */
                    gasPolicy_Engine[u8PortNum].u8PETimerID = PDTimer_Start (
                                                           CONFIG_PE_SENDERRESPONSE_TIMEOUT_MS,
                                                            PE_SubStateChangeAndTimeoutValidateCB, u8PortNum,  
                                                            (UINT8)ePE_SRC_HARD_RESET_ENTRY_SS);
                    
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
            /* Set PD Status as Connected as ePE_SRC_SEND_CAP_GOODCRC_RECEIVED_SS state 
             may be skipped sometime when Request from partner arises quickly after Good_CRC*/
            gasPolicy_Engine[u8PortNum].u8PEPortSts |= PE_PDCONNECTED_STS_MASK;

			/* Validate the received Request message by passing the received message to DPM */
            if(DPM_VALID_REQUEST == DPM_ValidateRequest(u8PortNum, (UINT16)u32Header, pu8DataBuf))
            {
				/* Received Request is a valid request */
                DEBUG_PRINT_PORT_STR (u8PortNum,"PE_SRC_NEGOTIATE_CAPABILITY: Request is Valid\r\n");
                
                /* No RDO Reject message has been sent to last Request made by attached Sink */
                gasCfgStatusData.sPerPortData[u8PortNum].u32PortConnectStatus &= 
                                        ~(DPM_PORT_AS_SRC_RDO_REJECTED_STATUS); 
                
                gasPolicy_Engine[u8PortNum].ePEState = ePE_SRC_TRANSITION_SUPPLY;
                gasPolicy_Engine[u8PortNum].ePESubState = ePE_SRC_TRANSITION_SUPPLY_ENTRY_SS;
            }
            
            else
            {
				/* Received Request is invalid */
                DEBUG_PRINT_PORT_STR (u8PortNum,"PE_SRC_NEGOTIATE_CAPABILITY: Request is Invalid\r\n"); 
                
                /* No RDO Accept message has been sent to last Request made by attached Sink */
                gasCfgStatusData.sPerPortData[u8PortNum].u32PortConnectStatus &= 
                                        ~(DPM_PORT_AS_SRC_RDO_ACCEPTED_STATUS); 
                
                gasPolicy_Engine[u8PortNum].ePEState = ePE_SRC_CAPABILITY_RESPONSE;
                gasPolicy_Engine[u8PortNum].ePESubState = ePE_SRC_CAPABILITY_RESPONSE_ENTRY_SS;
            }
            
            break;
        }
        
		/************** PE_SRC_TRANSITION_SUPPLY ****************/
        case ePE_SRC_TRANSITION_SUPPLY:
        {

            switch(gasPolicy_Engine[u8PortNum].ePESubState)
            {
                case ePE_SRC_TRANSITION_SUPPLY_ENTRY_SS:
                {
                    DEBUG_PRINT_PORT_STR (u8PortNum,"PE_SRC_TRANSITION_SUPPLY-ENTRY_SS\r\n");
                    
					/* Send the Accept message */
                    u16Transmit_Header = PRL_FormSOPTypeMsgHeader (u8PortNum, PE_CTRL_ACCEPT,
                                            PE_OBJECT_COUNT_0, PE_NON_EXTENDED_MSG);

                    u8TransmitSOP = PRL_SOP_TYPE;
                    u32pTransmit_DataObj = NULL;
                    Transmit_cb = PE_StateChange_TransmitCB;
      
                    u32Transmit_TmrID_TxSt = PRL_BUILD_PKD_TXST_U32( ePE_SRC_TRANSITION_SUPPLY, \
                                                ePE_SRC_TRANSITION_SUPPLY_GOODCRC_RECEIVED, \
                                                ePE_SRC_HARD_RESET, ePE_SRC_HARD_RESET_ENTRY_SS);

                    u8IsTransmit = TRUE;
                                        
                    
                             
                    gasPolicy_Engine[u8PortNum].ePESubState = ePE_SRC_TRANSITION_SUPPLY_IDLE_SS;
                    break;  
                }
                
                case ePE_SRC_TRANSITION_SUPPLY_GOODCRC_RECEIVED:
                {
                    /* Set AS_SOURCE_RDO_ACCEPTED bit in Port Connection Status */
                    gasCfgStatusData.sPerPortData[u8PortNum].u32PortConnectStatus |= DPM_PORT_AS_SRC_RDO_ACCEPTED_STATUS;
                    
                    gasPolicy_Engine[u8PortNum].u8PETimerID = PDTimer_Start (
                                                            (CONFIG_PE_SRCTRANSISTION_TIMEOUT_MS),
                                                            PE_SubStateChange_TimerCB,u8PortNum,  
                                                            (UINT8)ePE_SRC_TRANSITION_SUPPLY_POWER_ON);
                    gasPolicy_Engine[u8PortNum].ePESubState = ePE_SRC_TRANSITION_SUPPLY_IDLE_SS;
                    break;
                }
                
                case ePE_SRC_TRANSITION_SUPPLY_POWER_ON:
                {
                    /* Drive the Power to as requested by port partner*/
                    DPM_SetPortPower(u8PortNum);
                    /* Start tSrcReady timer */
                    gasPolicy_Engine[u8PortNum].u8PETimerID = PDTimer_Start (
                                                              (CONFIG_PE_SRC_READY_TIMEOUT_MS),
                                                              DPM_VBUSOnOffTimerCB, u8PortNum,  
                                                              (UINT8)SET_TO_ZERO);
                    gasPolicy_Engine[u8PortNum].ePESubState = ePE_SRC_TRANSITION_SUPPLY_EXIT_SS;
                    break;
                }
                
                case ePE_SRC_TRANSITION_SUPPLY_EXIT_SS:
                {
					/* Get the driven voltage status */
                    UINT16 u16DrivenVoltage = DPM_GET_VOLTAGE_FROM_PDO_MILLI_V(gasDPM[u8PortNum].u32NegotiatedPDO);
   		
					/* If the voltage reached the driven voltage level send the PS_RDY message */
                    if(u16DrivenVoltage == DPM_GetVBUSVoltage(u8PortNum))
                    {
                        /* Kill tSrcReady Timer */
                        PE_KillPolicyEngineTimer (u8PortNum);
                        DEBUG_PRINT_PORT_STR (u8PortNum,"PE_SRC_TRANSITION_SUPPLY-EXIT_SS\r\n");
                        u16Transmit_Header = PRL_FormSOPTypeMsgHeader (u8PortNum, PE_CTRL_PS_RDY, \
                                            PE_OBJECT_COUNT_0, PE_NON_EXTENDED_MSG);

                        u8TransmitSOP = PRL_SOP_TYPE;
                        u32pTransmit_DataObj = NULL;
                        Transmit_cb = PE_StateChange_TransmitCB;
                        
                        
                        u32Transmit_TmrID_TxSt = PRL_BUILD_PKD_TXST_U32( ePE_SRC_READY, ePE_SRC_READY_ENTRY_SS, \
                                                    ePE_SRC_HARD_RESET, ePE_SRC_HARD_RESET_ENTRY_SS);
                        
                        
                        u8IsTransmit = TRUE;
                        gasPolicy_Engine[u8PortNum].ePESubState = ePE_SRC_TRANSITION_SUPPLY_IDLE_SS;
                    }
                    break;  
                }
                
                case ePE_SRC_TRANSITION_SUPPLY_IDLE_SS:
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
        
		/***************** PE_SRC_CAPABILITY_RESPONSE ******************/
        case ePE_SRC_CAPABILITY_RESPONSE:
        {
            switch(gasPolicy_Engine[u8PortNum].ePESubState)
            {
                case ePE_SRC_CAPABILITY_RESPONSE_ENTRY_SS:
                {
					/* If Explicit Contract already initiated, Send the reject message and set the
						PE state to PE_SRC_READY*/
                    if((gasPolicy_Engine[u8PortNum].u8PEPortSts & PE_EXPLICIT_CONTRACT))
                    {
                        u32Transmit_TmrID_TxSt = PRL_BUILD_PKD_TXST_U32( ePE_SRC_READY, \
                                                    ePE_SRC_READY_ENTRY_SS, ePE_SRC_SEND_SOFT_RESET, \
                                                    ePE_SRC_SEND_SOFT_RESET_SOP_SS);
                    }
                    
					/* If Explicit Contract is not initiated, Send the reject message and set the
						PE state to PE_SRC_WAIT_NEW_CAPABILITIES*/
                    else
                    {
						/* Change the state to PE_SRC_WAIT_NEW_CAPABILITIES to wait for capabilities change */
                        u32Transmit_TmrID_TxSt = PRL_BUILD_PKD_TXST_U32( ePE_SRC_WAIT_NEW_CAPABILITIES, \
                                                    NULL, ePE_SRC_SEND_SOFT_RESET, \
                                                    ePE_SRC_SEND_SOFT_RESET_SOP_SS);
                    }
                    
					/* Send Reject message for Requested Invalid Capability */
                    u16Transmit_Header = PRL_FormSOPTypeMsgHeader (u8PortNum, PE_CTRL_REJECT, \
                                        PE_OBJECT_COUNT_0, PE_NON_EXTENDED_MSG);

                    u8TransmitSOP = PRL_SOP_TYPE;
                    u32pTransmit_DataObj = NULL;
                    Transmit_cb = PE_StateChange_TransmitCB;
                    
                    u8IsTransmit = TRUE;
                    
                    /* RDO Reject message has been sent to last Request made by attached Sink */
                    gasCfgStatusData.sPerPortData[u8PortNum].u32PortConnectStatus |= 
                                             DPM_PORT_AS_SRC_RDO_REJECTED_STATUS;
                    
                    gasPolicy_Engine[u8PortNum].ePESubState = ePE_SRC_CAPABILITY_RESPONSE_IDLE_SS;
                    
                    break;  
                }
                
                case ePE_SRC_CAPABILITY_RESPONSE_IDLE_SS:
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
        
		/************** PE_SRC_READY *****************/
        case ePE_SRC_READY:
        {
            switch(gasPolicy_Engine[u8PortNum].ePESubState)
            {
                case ePE_SRC_READY_ENTRY_SS:
                {
                    DEBUG_PRINT_PORT_STR (u8PortNum,"PE_SRC_READY-ENTRY_SS\r\n");
                    
#if (TRUE == CONFIG_HOOK_DEBUG_MSG)
                    u32PDODebug = gasDPM[u8PortNum].u32NegotiatedPDO;
                    DEBUG_PRINT_PORT_UINT32_STR( u8PortNum, "PDPWR", u32PDODebug, 1, "\r\n");
#endif
					
					/* Set the PD contract as Explicit Contract */
                    gasPolicy_Engine[u8PortNum].u8PEPortSts |= PE_EXPLICIT_CONTRACT;
                    gasPolicy_Engine[u8PortNum].ePESubState = ePE_SRC_READY_END_AMS_SS;
					
                    /* Update the Port Connection Status register : As Source, 
                       USB PD connection established, Power request has been made, 
                       accepted and PS_RDY message sent */
                    gasCfgStatusData.sPerPortData[u8PortNum].u32PortConnectStatus |= 
                                        DPM_PORT_AS_SRC_PD_CONTRACT_GOOD_STATUS; 
  
                    /* Reset New PDO Parameters if renegotiation was requested */
                    if (DPM_ENABLE_NEW_PDO == DPM_GET_NEW_PDO_STATUS(u8PortNum)) 
                    {                                                
                        DPM_ResetNewPDOParameters(u8PortNum);
                    }                    
                    
                    DPM_EnablePowerFaultDetection(u8PortNum);
                    
                    #if (TRUE == INCLUDE_PD_3_0)
                    /* Collision avoidance - Rp value set to TYPEC_SINK_TXOK */
                    (void)PRL_SetCollisionAvoidance (u8PortNum, TYPEC_SINK_TXOK);
                    #endif
                    /* Notify that PD contract is established*/    
                    (void)DPM_NotifyClient(u8PortNum, eMCHP_PSF_PD_CONTRACT_NEGOTIATED);
                    
                    break;
                }
                
                case ePE_SRC_READY_END_AMS_SS:
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
        
		/************* PE_SRC_WAIT_NEW_CAPABILITIES **************/
        case ePE_SRC_WAIT_NEW_CAPABILITIES:
        {
			/* Since our source capabilities are fixed this state will be in idle*/
            break;  
        }
        
		/*************** PE_SRC_DISCOVERY **********************/
        case ePE_SRC_DISCOVERY:
        {
            switch(gasPolicy_Engine[u8PortNum].ePESubState)
            {
                case ePE_SRC_DISCOVERY_ENTRY_SS:
                {
                    DEBUG_PRINT_PORT_STR (u8PortNum,"PE_SRC_DISCOVERY-ENTRY_SS\r\n");
					
					/* Start Source Capability  Timer, if timed out set the PE sub-state to 
						ePE_SRC_DISCOVERY_EXIT_SS */
                    gasPolicy_Engine[u8PortNum].u8PETimerID = PDTimer_Start (
                                                              (CONFIG_PE_SOURCECAPABILITY_TIMEOUT_MS),
                                                              PE_SubStateChange_TimerCB,u8PortNum,  
                                                              (UINT8) ePE_SRC_DISCOVERY_EXIT_SS);
                    
                    gasPolicy_Engine[u8PortNum].ePESubState = ePE_SRC_DISCOVERY_IDLE_SS;
                    break;  
                }
                
                case ePE_SRC_DISCOVERY_IDLE_SS:
                { 
                    /* Wait for Timeout */
                    break;  
                }
                
                case ePE_SRC_DISCOVERY_EXIT_SS:
                {
                    DEBUG_PRINT_PORT_STR (u8PortNum,"PE_SRC_DISCOVERY-EXIT_SS\r\n");
					
					/* If Caps Counter value reaches nCapsCount or Hard Reset Counter reaches
						nHardResetCount, NoResponse timer timed out and PD is not connected change the 
						PE state to ePE_SRC_DISABLED*/
                    if((gasPolicy_Engine[u8PortNum].u8CapsCounter > PE_N_CAPSCOUNT) ||
                       ((gasPolicy_Engine[u8PortNum].u8HardResetCounter > PE_N_HARD_RESET_COUNT) &&
                       (!(gasPolicy_Engine[u8PortNum].u8PEPortSts & PE_PDCONNECTED_STS_MASK)) &&
                       (gasPolicy_Engine[u8PortNum].u8PEPortSts & PE_NO_RESPONSE_TIMEDOUT)))
                    {
                        gasPolicy_Engine[u8PortNum].ePEState = ePE_SRC_DISABLED;        
                    }
                    
                    else
                    {
                        gasPolicy_Engine[u8PortNum].ePEState = ePE_SRC_SEND_CAPABILITIES;
                        gasPolicy_Engine[u8PortNum].ePESubState = ePE_SRC_SEND_CAP_ENTRY_SS;
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
            switch(gasPolicy_Engine[u8PortNum].ePESubState)
            {
                case ePE_SRC_HARD_RESET_ENTRY_SS:
                {
                    DEBUG_PRINT_PORT_STR (u8PortNum,"PE_SRC_HARD_RESET-ENTRY_SS\r\n");
                    
                    /* No request has been made by Sink within tSenderResponse */
                    if (gasPolicy_Engine[u8PortNum].u8PEPortSts & PE_PDCONNECTED_STS_MASK)
                    {
                        gasCfgStatusData.sPerPortData[u8PortNum].u32PortConnectStatus &= 
                                        ~(DPM_PORT_AS_SRC_RDO_ACCEPTED_STATUS | DPM_PORT_AS_SRC_RDO_REJECTED_STATUS);
                    }
                    
					/* Kill the policy engine timer */
                    PE_KillPolicyEngineTimer (u8PortNum);  
                 
                    /* Send Hard Reset to Port Partner*/
                    PRL_SendCableorHardReset(u8PortNum, PRL_SEND_HARD_RESET, NULL, 0);	

                    /* Increment the HardReset Counter */
                    gasPolicy_Engine[u8PortNum].u8HardResetCounter++;
                    
                    /* Start the PSHardResetTimer, if timed out set the PE sub-state to
						ePE_SRC_HARD_RESET_EXIT_SS */
                    gasPolicy_Engine[u8PortNum].u8PETimerID = PDTimer_Start (
                                                              (CONFIG_PE_PSHARDRESET_TIMEOUT_MS),
                                                              PE_SubStateChange_TimerCB, u8PortNum,  
                                                              (UINT8) ePE_SRC_HARD_RESET_EXIT_SS);
                    
                    gasPolicy_Engine[u8PortNum].ePESubState = ePE_SRC_HARD_RESET_IDLE_SS;
                    
                    break;  
                }
                
                case ePE_SRC_HARD_RESET_IDLE_SS:
                {
                    /* Wait for PSHardResetTimer to times out */
                    break;  
                }
                
                case ePE_SRC_HARD_RESET_EXIT_SS:
                {
                    DEBUG_PRINT_PORT_STR (u8PortNum,"PE_SRC_HARD_RESET-EXIT_SS\r\n");
                    gasPolicy_Engine[u8PortNum].ePEState = ePE_SRC_TRANSITION_TO_DEFAULT;
                    gasPolicy_Engine[u8PortNum].ePESubState = ePE_SRC_TRANSITION_TO_DEFAULT_ENTRY_SS;
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
            switch(gasPolicy_Engine[u8PortNum].ePESubState)
            {
                case ePE_SRC_HARD_RESET_RECEIVED_ENTRY_SS:
                {
                    DEBUG_PRINT_PORT_STR (u8PortNum,"PE_SRC_HARD_RESET_RECEIVED-ENTRY_SS\r\n");
                    
                    /* Kill All the Active Policy Engine timers for the port */                   
                    PE_KillPolicyEngineTimer (u8PortNum);  
                                        
                    /* Start PSHardResetTimer, if timed out set PE sub-state to
						ePE_SRC_HARD_RESET_RECEIVED_EXIT_SS */
                    gasPolicy_Engine[u8PortNum].u8PETimerID = PDTimer_Start (
                                                              (CONFIG_PE_PSHARDRESET_TIMEOUT_MS),
                                                              PE_SubStateChange_TimerCB, u8PortNum,  
                                                              (UINT8) ePE_SRC_HARD_RESET_RECEIVED_EXIT_SS);
                    
                    gasPolicy_Engine[u8PortNum].ePESubState = ePE_SRC_HARD_RESET_RECEIVED_IDLE_SS;
                    
                    break;  
                }
                
                case ePE_SRC_HARD_RESET_RECEIVED_IDLE_SS:
                {
                    /* Wait for PSHardResetTimer */
                    break;
                }
                
                case ePE_SRC_HARD_RESET_RECEIVED_EXIT_SS:
                {
                    DEBUG_PRINT_PORT_STR (u8PortNum,"PE_SRC_HARD_RESET_RECEIVED-EXIT_SS\r\n");
                    gasPolicy_Engine[u8PortNum].ePEState = ePE_SRC_TRANSITION_TO_DEFAULT;
                    gasPolicy_Engine[u8PortNum].ePESubState = ePE_SRC_TRANSITION_TO_DEFAULT_ENTRY_SS;  
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
            switch(gasPolicy_Engine[u8PortNum].ePESubState)
            {
                case ePE_SRC_TRANSITION_TO_DEFAULT_ENTRY_SS:
                {
                    DEBUG_PRINT_PORT_STR (u8PortNum,"PE_SRC_TRANSITION_TO_DEFAULT-ENTRY_SS\r\n");
                    
					/* Turn Off VCONN if it is a */
                    if(DPM_IsPort_VCONN_Source(u8PortNum))
                    {
                        DPM_VConnOnOff(u8PortNum, DPM_VCONN_OFF);
                    }
					
					/* Turn Off VBus */
                    DPM_TypeCSrcVBus5VOnOff(u8PortNum, DPM_VBUS_OFF);
                    
                     /*Start the VBUS OFF timer for monitoring the time taken for 
                    power module to reach Vsafe0V*/
                    gasPolicy_Engine[u8PortNum].u8PETimerID = PDTimer_Start (
                                                              (CONFIG_TYPEC_VBUS_OFF_TIMER_MS),
                                                              DPM_VBUSOnOffTimerCB, u8PortNum,  
                                                              (UINT8)SET_TO_ZERO);
                   
                    gasPolicy_Engine[u8PortNum].ePESubState = ePE_SRC_TRANSITION_TO_DEFAULT_VSAFE0V_SS;
                
                    break;  
                }
                
                case ePE_SRC_TRANSITION_TO_DEFAULT_VSAFE0V_SS:
                {
                                  
                    if(TYPEC_VBUS_0V == DPM_GetVBUSVoltage(u8PortNum))
                    {
                        /*Kill the VBUS ON timer since Vsafe0V is reached*/
                        PE_KillPolicyEngineTimer (u8PortNum);
                        
                        /*Start the VCONN_OFF timer*/
                        /*This Timeout is implemented outside of the PD Specification to track 
                        VCONN Turn OFF error*/
                        gasPolicy_Engine[u8PortNum].u8PETimerID = PDTimer_Start (\
                                                                  CONFIG_PE_VCONNOFF_TIMEOUT_MS,\
                                                                  DPM_VCONNOFFErrorTimerCB,\
                                                                  u8PortNum,\
                                                                  (UINT8)SET_TO_ZERO);
                        
#if (TRUE == CONFIG_HOOK_DEBUG_MSG)
                        u32PDODebug = SET_TO_ZERO;
                        DEBUG_PRINT_PORT_UINT32_STR( u8PortNum, "PDPWR", u32PDODebug, 1, "\r\n");
#endif
                        gasPolicy_Engine[u8PortNum].ePESubState = ePE_SRC_TRANSITION_TO_DEFAULT_CHECK_VCONNOFF_SS;
                     
                    }
                    break;  
                }
                
                case ePE_SRC_TRANSITION_TO_DEFAULT_CHECK_VCONNOFF_SS:
                {
                    
                     if(!DPM_IsPort_VCONN_Source(u8PortNum))
                     {
                       
                        /*Stop the VCONN_OFF timer*/
                        PE_KillPolicyEngineTimer (u8PortNum);
                        
                        /* Start tSRCRecovery timer, if timedout set the PE sub-state to
                        ePE_SRC_TRANSITION_TO_DEFAULT_POWER_ON_SS*/
                        gasPolicy_Engine[u8PortNum].u8PETimerID = PDTimer_Start (
                                                            (CONFIG_PE_SRCRECOVER_TIMEOUT_MS),
                                                            PE_SubStateChange_TimerCB,u8PortNum,  
                                                            (UINT8) ePE_SRC_TRANSITION_TO_DEFAULT_SRCRECOVER_TIMEOUT_SS);

                        gasPolicy_Engine[u8PortNum].ePESubState = ePE_SRC_TRANSITION_TO_DEFAULT_IDLE_SS;                    
                     }
                    
                    break;            
                }
                
                case ePE_SRC_TRANSITION_TO_DEFAULT_IDLE_SS:
                {
                    /* Wait for SrcRecoverTimer to times out */
                    break;  
                }
                
                case ePE_SRC_TRANSITION_TO_DEFAULT_SRCRECOVER_TIMEOUT_SS:
                {
                    /* SrcRecover timer timed out - Added for Power fault handling*/
                    gasPolicy_Engine[u8PortNum].ePESubState = ePE_SRC_TRANSITION_TO_DEFAULT_POWER_ON_SS;
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
                        DPM_VConnOnOff(u8PortNum, DPM_VCONN_ON);
                    }
					
					/* Turn On VBus */
                    DPM_TypeCSrcVBus5VOnOff(u8PortNum, DPM_VBUS_ON);
                    
                     /*Start the VBUS ON timer for monitoring the time taken for 
                    power module to reach Vsafe5V*/
                    gasPolicy_Engine[u8PortNum].u8PETimerID = PDTimer_Start (
                                                              (CONFIG_TYPEC_VBUS_ON_TIMER_MS),
                                                              DPM_VBUSOnOffTimerCB, u8PortNum,  
                                                              (UINT8)SET_TO_ZERO);
                    
                    gasPolicy_Engine[u8PortNum].ePESubState = ePE_SRC_TRANSITION_TO_DEFAULT_VBUS_CHECK_SS;
                    
                    break;  
                }
                
                case ePE_SRC_TRANSITION_TO_DEFAULT_VBUS_CHECK_SS:
                {
                    if(TYPEC_VBUS_5V == DPM_GetVBUSVoltage(u8PortNum))
                    {
                        DEBUG_PRINT_PORT_STR (u8PortNum,"PE_SRC_TRANSITION_TO_DEFAULT-EXIT_SS\r\n");
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
                        
                        /*Kill the VBUS ON timer since Vsafe5V is reached*/
                        PE_KillPolicyEngineTimer (u8PortNum);

#if (TRUE == CONFIG_HOOK_DEBUG_MSG)
                        DEBUG_PRINT_PORT_UINT32_STR( u8PortNum, "PDPWR", u32PDODebug, 1, "\r\n");
#endif
                        /* Inform Protocol Layer about Hard Reset Complete */
                        PRL_HRorCRCompltIndicationFromPE(u8PortNum);
						
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
                            gasPolicy_Engine[u8PortNum].u8PETimerID  =PDTimer_Start (
                                                             CONFIG_TYPEC_VCONNON_TIMEOUT_MS,
                                                              DPM_VCONNONTimerErrorCB, u8PortNum,  
                                                              (UINT8)SET_TO_ZERO);
                        
                            gasPolicy_Engine[u8PortNum].ePESubState = ePE_SRC_TRANSITION_TO_DEFAULT_VCONNON_CHECK_SS;
                        }
                        else
                        {
                             gasPolicy_Engine[u8PortNum].ePESubState = ePE_SRC_TRANSITION_TO_DEFAULT_EXIT_SS; 
                        
                        }
                        
                    }
                    break;
                }
                
                case ePE_SRC_TRANSITION_TO_DEFAULT_VCONNON_CHECK_SS:
                {
                
                    if(DPM_IsPort_VCONN_Source(u8PortNum))
                    {                          
                        PE_KillPolicyEngineTimer (u8PortNum);                        
                        gasPolicy_Engine[u8PortNum].ePESubState = ePE_SRC_TRANSITION_TO_DEFAULT_EXIT_SS;                                                          
                    }                    
                    break;
                  
                }
                case ePE_SRC_TRANSITION_TO_DEFAULT_EXIT_SS:
                {
                  
                    /*Start NoResponse timer */
                    gasPolicy_Engine[u8PortNum].u8PENoResponseTimerID = PDTimer_Start (
                                                        (CONFIG_PE_NORESPONSE_TIMEOUT_MS),
                                                        PE_NoResponseTimerCB, u8PortNum,  
                                                        (UINT8)SET_TO_ZERO);
                    
                    gasPolicy_Engine[u8PortNum].ePEState = ePE_SRC_STARTUP;
                    gasPolicy_Engine[u8PortNum].ePESubState = ePE_SRC_STARTUP_ENTRY_SS;
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
           switch(gasPolicy_Engine[u8PortNum].ePESubState) 
           {
                case ePE_SRC_GET_SINK_CAP_ENTRY_SS:
                {
                    DEBUG_PRINT_PORT_STR (u8PortNum,"ePE_SRC_GET_SINK_CAP_ENTRY_SS\r\n"); 
                    
                    u16Transmit_Header = PRL_FormSOPTypeMsgHeader (u8PortNum, PE_CTRL_GET_SINK_CAP, \
                                            PE_OBJECT_COUNT_0, PE_NON_EXTENDED_MSG);

                    u8TransmitSOP = PRL_SOP_TYPE;
                    u32pTransmit_DataObj = NULL;
                    Transmit_cb = PE_StateChange_TransmitCB;                 
                        
                    u32Transmit_TmrID_TxSt = PRL_BUILD_PKD_TXST_U32(ePE_SRC_GET_SINK_CAP, ePE_SRC_GET_SINK_CAP_GOODCRC_RECEIVED_SS, \
                                                    ePE_SRC_READY, ePE_SRC_READY_END_AMS_SS);
                                                
                    u8IsTransmit = TRUE;                                             
                   
                    gasPolicy_Engine[u8PortNum].ePESubState = ePE_SRC_GET_SINK_CAP_IDLE_SS;
                    
                    break; 
                }                   
                
                case ePE_SRC_GET_SINK_CAP_GOODCRC_RECEIVED_SS: 
                {
                    DEBUG_PRINT_PORT_STR (u8PortNum,"ePE_SRC_GET_SINK_CAP_GOODCRC_RECEIVED\r\n"); 
                    gasPolicy_Engine[u8PortNum].ePESubState = ePE_SRC_GET_SINK_CAP_IDLE_SS; 
                    
                    /* Start Sender Response timer and Set the timer callback to transition to 
					ePE_SRC_GET_SINK_CAP_TIMER_TIMEDOUT sub state if timeout happens */
                    gasPolicy_Engine[u8PortNum].u8PETimerID = PDTimer_Start (
                                                           CONFIG_PE_SENDERRESPONSE_TIMEOUT_MS,
                                                            PE_SubStateChange_TimerCB, u8PortNum,  
                                                            (UINT8)ePE_SRC_GET_SINK_CAP_TIMER_TIMEDOUT_SS); 
                    
                    break; 
                }
                  
                case ePE_SRC_GET_SINK_CAP_TIMER_TIMEDOUT_SS:
                {
                   /* Sink caps not received within tSenderResponse. Send 
                      SINK_CAPS_NOT_RECEIVED notification and move to 
                      PE_SRC_READY state */ 
                   DEBUG_PRINT_PORT_STR (u8PortNum,"ePE_SRC_GET_SINK_CAP_TIMER_TIMEDOUT\r\n"); 
                   gasPolicy_Engine[u8PortNum].ePEState = ePE_SRC_READY; 
                   gasPolicy_Engine[u8PortNum].ePESubState = ePE_SRC_READY_END_AMS_SS;
                   
                   (void)DPM_NotifyClient(u8PortNum, eMCHP_PSF_GET_SINK_CAPS_NOT_RCVD);
                   
                   break;  
                }   
                
                case ePE_SRC_GET_SINK_CAP_RESPONSE_RECEIVED_SS:
                {
                    DEBUG_PRINT_PORT_STR (u8PortNum,"ePE_SRC_GET_SINK_CAP_RESPONSE_RECEIVED\r\n"); 
                    /* Pass the received sink caps to DPM and send notification */         
                    DPM_StoreSinkCapabilities (u8PortNum, (UINT16) u32Header, (UINT32*) pu8DataBuf); 
                    
                    gasPolicy_Engine[u8PortNum].ePEState = ePE_SRC_READY; 
                    gasPolicy_Engine[u8PortNum].ePESubState = ePE_SRC_READY_END_AMS_SS;
                    
                    (void)DPM_NotifyClient(u8PortNum, eMCHP_PSF_GET_SINK_CAPS_RCVD);
                    
                    break; 
                }
                
                case ePE_SRC_GET_SINK_CAP_IDLE_SS: 
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
            
        /************* PE_SRC_SEND_SOFT_RESET ******************/
        case ePE_SRC_SEND_SOFT_RESET:
        {
            switch(gasPolicy_Engine[u8PortNum].ePESubState)
            {
                case ePE_SRC_SEND_SOFT_RESET_SOP_SS:
                {
                    DEBUG_PRINT_PORT_STR (u8PortNum,"ePE_SRC_SEND_SOFT_RESET-SOP_SS\r\n");
                    
                    /* If RDO reject message is not sent, PE moves to this state. 
                       Hence, clearing the status variable in this state */
                    gasCfgStatusData.sPerPortData[u8PortNum].u32PortConnectStatus &= 
                                        ~(DPM_PORT_AS_SRC_RDO_REJECTED_STATUS);
                    
                    /* Kill Policy Engine Timer */
                    PE_KillPolicyEngineTimer (u8PortNum);
                    
					/* Reset the Protocol Layer for SOP type message */
                    PRL_ProtocolspecificSOPReset(u8PortNum, (UINT8) PRL_SOP_TYPE);
                    
					/* Send SoftReset message */
                    u8TransmitSOP = PRL_SOP_TYPE;
                    u16Transmit_Header = PRL_FormSOPTypeMsgHeader(u8PortNum, PE_CTRL_SOFT_RESET, \
																	PE_OBJECT_COUNT_0, PE_NON_EXTENDED_MSG);
                    u32pTransmit_DataObj = NULL;
                    Transmit_cb = PE_StateChange_TransmitCB;
                    u32Transmit_TmrID_TxSt = PRL_BUILD_PKD_TXST_U32( ePE_SRC_SEND_SOFT_RESET, \
                                                    ePE_SRC_SEND_SOFT_RESET_GOODCRC_RECEIVED_SS, \
                                                    ePE_SRC_HARD_RESET, ePE_SRC_HARD_RESET_ENTRY_SS);
                    u8IsTransmit = TRUE;
                    
                    gasPolicy_Engine[u8PortNum].ePESubState = ePE_SRC_SEND_SOFT_RESET_IDLE_SS;
                    
                    break;  
                }
                
                case ePE_SRC_SEND_SOFT_RESET_SOP_P_SS:
                {
                    DEBUG_PRINT_PORT_STR (u8PortNum,"PE_SRC_SEND_SOFT_RESET-ENTRY_SS\r\n");
					
                    /* Kill Policy Engine Timer */
                    PE_KillPolicyEngineTimer (u8PortNum);
                    
					/* Reset the Protocol Layer for SOP_P type message */
                    PRL_ProtocolspecificSOPReset(u8PortNum, PRL_SOP_P_TYPE);

                    /* Send Soft Reset Message */
                    u8TransmitSOP = PRL_SOP_P_TYPE;
                    u16Transmit_Header = PRL_FormNonSOPTypeMsgHeader(u8PortNum, PE_CTRL_SOFT_RESET, \
																	PE_OBJECT_COUNT_0, PE_NON_EXTENDED_MSG);
                    u32pTransmit_DataObj = NULL;
                    Transmit_cb = PE_StateChange_TransmitCB;
                    u32Transmit_TmrID_TxSt = PRL_BUILD_PKD_TXST_U32( ePE_SRC_SEND_SOFT_RESET, \
                                                    ePE_SRC_SEND_SOFT_RESET_GOODCRC_RECEIVED_SS, \
                                                    ePE_SRC_HARD_RESET, ePE_SRC_HARD_RESET_ENTRY_SS);
                    u8IsTransmit = TRUE;
                    
                    gasPolicy_Engine[u8PortNum].ePESubState = ePE_SRC_SEND_SOFT_RESET_IDLE_SS;
                    
                    break;  
                }
                
                case ePE_SRC_SEND_SOFT_RESET_IDLE_SS:
                {
                    break;  
                }
                
                case ePE_SRC_SEND_SOFT_RESET_GOODCRC_RECEIVED_SS:
                {
                    DEBUG_PRINT_PORT_STR (u8PortNum,"PE_SRC_SEND_SOFT_RESET-GOODCRC_RECEIVED_SS\r\n");
                    gasPolicy_Engine[u8PortNum].u8PETimerID = PDTimer_Start (
                                                            CONFIG_PE_SENDERRESPONSE_TIMEOUT_MS,
                                                            PE_SubStateChange_TimerCB,u8PortNum,  
                                                            (UINT8)ePE_SRC_SEND_SOFT_RESET_SENDER_RESPONSE_TIMEDOUT);
                    gasPolicy_Engine[u8PortNum].ePESubState = ePE_SRC_SEND_SOFT_RESET_IDLE_SS;
                    break;  
                }
                
                case ePE_SRC_SEND_SOFT_RESET_SENDER_RESPONSE_TIMEDOUT:
                {
					/* Sender Response timer timed out */
                    DEBUG_PRINT_PORT_STR (u8PortNum,"PE_SRC_SEND_SOFT_RESET-SENDER_RESPONSE_TIMEDOUT\r\n");
                    gasPolicy_Engine[u8PortNum].ePEState = ePE_SRC_HARD_RESET;
                    gasPolicy_Engine[u8PortNum].ePESubState = ePE_SRC_HARD_RESET_ENTRY_SS; 
                    break;
                }
                
                default:
                {
                    break;
                }
              
            }
            break;  
        }
        
		/************** PE_SRC_SOFT_RESET ***************/
        case ePE_SRC_SOFT_RESET:
        {
            switch(gasPolicy_Engine[u8PortNum].ePESubState)
            {
                case ePE_SRC_SOFT_RESET_ENTRY_SS:
                {
                    DEBUG_PRINT_PORT_STR (u8PortNum,"ePE_SRC_SOFT_RESET-ENTRY_SS\r\n");
                    
                    /* Kill Policy Engine Timer */
                    PE_KillPolicyEngineTimer (u8PortNum);
                    
					/* Reset Protocol Layer */
                    PRL_ProtocolResetAllSOPs(u8PortNum);
                    
                    /* Send Accept message */
                    u16Transmit_Header = PRL_FormSOPTypeMsgHeader (u8PortNum, PE_CTRL_ACCEPT,
                                            PE_OBJECT_COUNT_0, PE_NON_EXTENDED_MSG);

                    u8TransmitSOP = PRL_SOP_TYPE;
                    u32pTransmit_DataObj = NULL;
                    Transmit_cb = PE_StateChange_TransmitCB;

                    u32Transmit_TmrID_TxSt = PRL_BUILD_PKD_TXST_U32( ePE_SRC_SEND_CAPABILITIES, \
                                                ePE_SRC_SEND_CAP_ENTRY_SS, \
                                                ePE_SRC_HARD_RESET, ePE_SRC_HARD_RESET_ENTRY_SS);

                    u8IsTransmit = TRUE;
                    
                    gasPolicy_Engine[u8PortNum].ePESubState = ePE_SRC_SOFT_RESET_IDLE_SS;
                    
                    break;  
                }
                
                case ePE_SRC_SOFT_RESET_IDLE_SS:
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
        
		/************* PE_SRC_DISABLED ***************/
        case ePE_SRC_DISABLED:
        {
            /* Port Disabled */
            break;  
        }
        
		/*************** PE_SRC_VDM_IDENTITY_REQUEST *****************/
        case ePE_SRC_VDM_IDENTITY_REQUEST:
        {
            
            switch(gasPolicy_Engine[u8PortNum].ePESubState)
            {
                case ePE_SRC_VDM_IDENTITY_REQUEST_ENTRY_SS:
                {
                    DEBUG_PRINT_PORT_STR (u8PortNum,"PE_SRC_VDM_IDENTITY_REQUEST-ENTRY_SS\r\n");
                    
                    /* Choosing VDM version as per Current spec revision */
                    if(PD_SPEC_REVISION_2_0 == DPM_GET_CURRENT_PD_SPEC_REV(u8PortNum))
                    {
                        u32VDMHeader = PE_SRC_VDM_HEADER_LOW_VER;
                    }
                    
                    else
                    {
                        u32VDMHeader = PE_SRC_VDM_HEADER_HIGH_VER;
                    }
                    
					/* Send VDM Discover Identity message to E-Cable */
                    u16Transmit_Header = PRL_FormNonSOPTypeMsgHeader(u8PortNum, (UINT8)PE_DATA_VENDOR_DEFINED,  \
                                                                        1, PE_NON_EXTENDED_MSG);
                    u8TransmitSOP = PRL_SOP_P_TYPE;
                    u32pTransmit_DataObj = &u32VDMHeader;
                    Transmit_cb = PE_StateChange_TransmitCB;

                    u32Transmit_TmrID_TxSt = PRL_BUILD_PKD_TXST_U32( ePE_SRC_VDM_IDENTITY_REQUEST, \
                                                ePE_SRC_VDM_IDENTITY_REQUEST_GOODCRC_SS, ePE_SRC_VDM_IDENTITY_NAKED, NULL);
              
                    gasPolicy_Engine[u8PortNum].u8DiscoverIdentityCounter++;
                    u8IsTransmit = TRUE;
                    
                    gasPolicy_Engine[u8PortNum].ePESubState = ePE_SRC_VDM_IDENTITY_REQUEST_IDLE_SS;
                    
                    break;    
                }
            
                case ePE_SRC_VDM_IDENTITY_REQUEST_IDLE_SS:
                {
                    break;
                }
                
                case ePE_SRC_VDM_IDENTITY_REQUEST_GOODCRC_SS:
                {
					/* GoodCRC received for VDM Identity request */
                    DEBUG_PRINT_PORT_STR (u8PortNum,"PE_SRC_VDM_IDENTITY_REQUEST-GOODCRC_SS\r\n");
                    
					/* Start the VDMIDentityRequest Sender Response timer, if timed out set the PE
						sub-state to ePE_SRC_VDM_IDENTITY_REQUEST_SENDER_RESPONSE_TIMEOUT */
                    gasPolicy_Engine[u8PortNum].u8PETimerID = PDTimer_Start (
                                                            (CONFIG_PE_VDMRESPONSE_TIMEOUT_MS),
                                                            PE_SubStateChangeAndTimeoutValidateCB,u8PortNum,  
                                                            (UINT8)ePE_SRC_VDM_IDENTITY_REQUEST_SENDER_RESPONSE_TIMEOUT);
                    gasPolicy_Engine[u8PortNum].ePESubState = ePE_SRC_VDM_IDENTITY_REQUEST_IDLE_SS;
                    break;  
                }
                
                case ePE_SRC_VDM_IDENTITY_REQUEST_SENDER_RESPONSE_TIMEOUT:
                {
                    gasPolicy_Engine[u8PortNum].ePEState = ePE_SRC_VDM_IDENTITY_NAKED; 
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
            if( PE_VDM_ACK == DPM_StoreVDMECableData(u8PortNum, u8SOPType, (UINT16) u32Header, (UINT32*) pu8DataBuf))
            {
                gasDPM[u8PortNum].u8DPM_Status &= ~(DPM_CURR_PD_SPEC_REV_MASK);
                gasDPM[u8PortNum].u8DPM_Status |= (CONFIG_PD_DEFAULT_SPEC_REV << DPM_CURR_PD_SPEC_REV_POS);
                gasPolicy_Engine[u8PortNum].u8DiscoverIdentityCounter = RESET_TO_ZERO;
                gasPolicy_Engine[u8PortNum].ePEState = ePE_SRC_SEND_CAPABILITIES;
                gasPolicy_Engine[u8PortNum].ePESubState = ePE_SRC_SEND_CAP_ENTRY_SS;
            }
            
            else
            {
                gasPolicy_Engine[u8PortNum].ePEState = ePE_SRC_VDM_IDENTITY_NAKED;
            }
            
            break;  
        }
        
        case ePE_SRC_VDM_IDENTITY_NAKED:
        {
            DEBUG_PRINT_PORT_STR (u8PortNum,"PE_SRC_VDM_IDENTITY_NAKED\r\n");
            
            /* VDM NAK received */
			/* If DiscoverIdentityCounter reaches nDiscoverIdentityCount change the PE state to	
				PE_SRC_SEND_CAPABILITIES */
            if(gasPolicy_Engine[u8PortNum].u8DiscoverIdentityCounter > PE_N_DISCOVER_IDENTITY_COUNT)
            {
                gasDPM[u8PortNum].u8DPM_Status &= ~(DPM_CURR_PD_SPEC_REV_MASK);
                gasDPM[u8PortNum].u8DPM_Status |= (CONFIG_PD_DEFAULT_SPEC_REV << DPM_CURR_PD_SPEC_REV_POS);
                gasPolicy_Engine[u8PortNum].u8PEPortSts |= PE_CABLE_RESPOND_NAK;
                gasPolicy_Engine[u8PortNum].u8DiscoverIdentityCounter = RESET_TO_ZERO;
                gasPolicy_Engine[u8PortNum].ePEState = ePE_SRC_SEND_CAPABILITIES;
                gasPolicy_Engine[u8PortNum].ePESubState = ePE_SRC_SEND_CAP_ENTRY_SS;               
            }
            
            else
            {
                gasDPM[u8PortNum].u8DPM_Status &= ~(DPM_CURR_PD_SPEC_REV_MASK);
                gasDPM[u8PortNum].u8DPM_Status |= (PD_SPEC_REVISION_2_0 << DPM_CURR_PD_SPEC_REV_POS);
                gasPolicy_Engine[u8PortNum].ePEState = ePE_SRC_VDM_IDENTITY_REQUEST;
                gasPolicy_Engine[u8PortNum].ePESubState = ePE_SRC_VDM_IDENTITY_REQUEST_ENTRY_SS;
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
		(void) PRL_TransmitMsg (u8PortNum, (UINT8) u8TransmitSOP, (UINT32)u16Transmit_Header, \
                    (UINT8 *)u32pTransmit_DataObj, Transmit_cb, u32Transmit_TmrID_TxSt); 
    }
}
#endif
