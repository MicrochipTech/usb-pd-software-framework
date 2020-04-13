/*******************************************************************************
 Power Balancing Source File

 Company:
   Microchip Technology Inc.

 File Name:
   pb_mngr.c

 Description:
   This file contains the function for Power Balancing State machine. 
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

#if (TRUE == INCLUDE_POWER_BALANCING)


UINT8 PB_HandleDPMEvents (UINT8 u8PortNum, UINT8 eDPM_EVENT)
{
    UINT32 u32SourcePDO            = SET_TO_ZERO; 
    UINT32 u32SinkRDO              = SET_TO_ZERO;
    UINT16 u16GivebackPwrIn250mW   = SET_TO_ZERO;
    UINT16 u16PrevNegPwrIn250mW    = SET_TO_ZERO;
    UINT8 u8IsNegotiationInProgress = FALSE;   
    UINT8 u8IsAvailablePwrSufficient = FALSE;
        
    switch(eDPM_EVENT)
    {
        case eMCHP_PSF_TYPEC_CC1_ATTACH: 
            /* fallthrough */
        case eMCHP_PSF_TYPEC_CC2_ATTACH: 
            
            PB_InitiateNegotiationWrapper(u8PortNum, gasPBIntPortParam[u8PortNum].u16MinGuaranteedPwrIn250mW); 
            gasPBIntPortParam[u8PortNum].u8PBPortStatusMask |= PB_PORT_STATUS_ATTACH;
            gasPBIntPortParam[u8PortNum].u8AttachSeqNo = gu8AttachSeq++;
            
            break; 
            
        case eMCHP_PSF_TYPEC_DETACH_EVENT: 
            
            /* Give power back to pool.On the following condition, the giveback 
            power is calculated based on the Required Power. This is because, 
            the Port Required Power will have the power subtracted from pool power*/   
            if ((ePB_RENEGOTIATION_IN_PROGRESS_STATE == gasPBIntPortParam[u8PortNum].ePBPortState) || \
               (ePB_RENEGOTIATION_COMPLETED_STATE == gasPBIntPortParam[u8PortNum].ePBPortState) || \
               (ePB_RENEGOTIATION_PENDING_STATE == gasPBIntPortParam[u8PortNum].ePBPortState))
            {
                u16GivebackPwrIn250mW = gasPBIntPortParam[u8PortNum].u16RequiredPrtPwrIn250mW - \
                             gasPBIntPortParam[u8PortNum].u16MinGuaranteedPwrIn250mW;
            }
             /*The give back power is calculated based on Neg Power since, the required power will have 
             the max power required for the port but Neg power will have the actual power that is
             withdrawn from the pool power. This can happen only when a detach occurs for the 
             recovering port*/
            else if (ePB_PWR_RECOVERING_STATE == gasPBIntPortParam[u8PortNum].ePBPortState)
            {
                u16GivebackPwrIn250mW = gasPBIntPortParam[u8PortNum].u16NegotiatedPwrIn250mW - \
                            gasPBIntPortParam[u8PortNum].u16MinGuaranteedPwrIn250mW;
            }
            else
            {
                u16GivebackPwrIn250mW = 0;
            }
            
            /*If the Timer for the port is started, kill the timer*/
            if (ePB_WAIT_FOR_ASYNC_REQ_SS == gasPBIntPortParam[u8PortNum].eRenegSubState)
            {
                PDTimer_Kill (gu8PBTimerID);              
            }
            
            /*Set Renegotiation pending for lower priority ports only if the 
             port is attached and a PD contract is negotiated*/
            
            if (ePB_IDLE_STATE != gasPBIntPortParam[u8PortNum].ePBPortState)
            {
                /*Set negotiation pending for all the lower priority ports*/
                PB_SetRenegotiationPendingForLowPriorityPorts (u8PortNum);
            }
            
            gasCfgStatusData.u16SharedPwrCapacityIn250mW += u16GivebackPwrIn250mW;
            
             /*Reset all the state variables and the flags associated with the port*/
            gasPBIntPortParam[u8PortNum].u8PBPortStatusMask = FALSE; 
            
            PB_UpdateAttachSeq (u8PortNum);
            
            gasPBIntPortParam[u8PortNum].u8AttachSeqNo = 0x00;            
            gasPBIntPortParam[u8PortNum].u16NegotiatedPwrIn250mW = 0;
            gasPBIntPortParam[u8PortNum].u16RequiredPrtPwrIn250mW = 0;
            gasPBIntPortParam[u8PortNum].eRenegSubState = ePB_IDLE_SS;
            gasPBIntPortParam[u8PortNum].ePBPortState = ePB_IDLE_STATE;
            gasPBIntPortParam[u8PortNum].eGetSinkCapSS = ePB_SINK_CAPS_NOT_INITIATED;      
            gasCfgStatusData.sPerPortData[u8PortNum].u16NegoCurrentIn10mA = 0;
            gasCfgStatusData.sPerPortData[u8PortNum].u16NegoVoltageIn50mV = 0; 
                                   
            if (gsPBIntSysParam.u8RecoveringMode)
            {
                if (u8PortNum == gsPBIntSysParam.u8ReclaimPortNum)
                {
                    /*If the recovering mode is on and the detach occurred for the reclaiming
                     port, then see if there is enough power for Recover Port. If not
                     again Reclaim*/
                    u8IsAvailablePwrSufficient = PB_NegotiateIfRequiredPwrAvailableInPool (gsPBIntSysParam.u8RecoverPortNum);
                    
                    if (u8IsAvailablePwrSufficient)
                    {
                        gsPBIntSysParam.u8RecoveringMode = FALSE;
                    }
                    else
                    {
                        if (PB_RECLAIM_FAILED == PB_ReclaimPower (gsPBIntSysParam.u8RecoverPortNum))
                        {
                         /*There is no power in the lower priority port. Just advertise whatever power
                             available in the pool*/             
                            /*Initiate renegotiation for the port with new wattage*/
                            PB_InitiateNegotiationWrapper (u8PortNum, gasCfgStatusData.u16SharedPwrCapacityIn250mW + gasPBIntPortParam[u8PortNum].u16NegotiatedPwrIn250mW);
                            
                            PB_ChangePortStates(u8PortNum, ePB_RENEGOTIATION_IN_PROGRESS_STATE, ePB_FIRST_RENEGOTIATION_IN_PROGRESS_SS); 
                            
                            gasCfgStatusData.u16SharedPwrCapacityIn250mW = 0;
                            gsPBIntSysParam.u8RecoveringMode = FALSE;
                        }                       
                    }
                }
                
                if (u8PortNum == gsPBIntSysParam.u8RecoverPortNum)
                {
                    /*If the detach is for a Recover Port, then already the power
                     is refilled.*/
                    gsPBIntSysParam.u8RecoveringMode = FALSE;
                }                   
            } 
            
            /*If there is any port which is already negotiating then don't do 
            anything. Else renegotiate the next higher priority port in 
            Pending state*/
            u8IsNegotiationInProgress = PB_IsNegotiationInProgressForOtherPort(u8PortNum);

            if (!u8IsNegotiationInProgress)
            {
                PB_InitiateNextPortNegotiation ();
            }
            
            break; 
        
        case eMCHP_PSF_PD_CONTRACT_NEGOTIATED:
             
            u32SourcePDO = gasDPM[u8PortNum].u32NegotiatedPDO; 
            u32SinkRDO = gasCfgStatusData.sPerPortData[u8PortNum].u32RDO;
            
            /* If initial negotiation complete, go ahead and publish max caps */
            if (gasPBIntPortParam[u8PortNum].u8PBPortStatusMask & PB_PORT_STATUS_INITIAL_NEG_DONE)
            {   
                if (ePB_FIRST_RENEGOTIATION_IN_PROGRESS_SS == gasPBIntPortParam[u8PortNum].eRenegSubState)
                {
                    /* Calculate the power that was negotiated in the last contract */
                    PB_CalculateNegotiatedPower(u8PortNum, u32SourcePDO, u32SinkRDO);                    
                    
                    /*Renegotiate that port with new PDO that's assigned*/
                    /*If Reneg Flag is set, then some detach for higher priority port
                     *  has happened when the Negotiation was happening for this port.
                     * Hence we need to again Renegotiate this port as well other ports
                     * based on the priority
                     */    
                    if (gasPBIntPortParam[u8PortNum].u8PBPortStatusMask & PB_PORT_STATUS_RENEG_AGAIN)
                    {
                        /*Refill pool with excess power */
                        (void)PB_ReleaseExcessPwr (u8PortNum);
                        
                        gasPBIntPortParam[u8PortNum].u8PBPortStatusMask &= ~(PB_PORT_STATUS_RENEG_AGAIN);
                        
                        /*Change the port state to Negotiation Pending*/
                        PB_ChangePortStates (u8PortNum, ePB_RENEGOTIATION_PENDING_STATE, ePB_IDLE_SS);
                        
                        /*Find out the highest port in pending state*/
                        PB_InitiateNextPortNegotiation ();
                    }      
                    else
                    {                      
                        /* Start a timer for 200 ms and change state as WAIT_FOR_ASYNC_REQ*/
                        gu8PBTimerID = PDTimer_Start (MILLISECONDS_TO_TICKS(gsPBIntSysParam.u32AsyncReqWaitTimerInms), PB_TimerEnd, u8PortNum, PB_TIMER_EXPIRED_EVENT);

                        PB_ChangePortStates (u8PortNum, ePB_RENEGOTIATION_IN_PROGRESS_STATE, ePB_WAIT_FOR_ASYNC_REQ_SS);

                    }
                }
                else if (ePB_SECOND_RENEGOTIATION_IN_PROGRESS_SS == gasPBIntPortParam[u8PortNum].eRenegSubState)
                {                    
                    /* Calculate the power that was negotiated in the last contract */
                   PB_CalculateNegotiatedPower(u8PortNum, u32SourcePDO, u32SinkRDO);                    
                    
                   /*Calculate if there is excess power and refill the common pool*/
                   (void)PB_ReleaseExcessPwr (u8PortNum);
                   
                    /*If Reneg Flag is set, then some detach for higher priority port
                     *  has happened when the Negotiation was happening for this port.
                     * Hence we need to again Renegotiate this port as well other ports
                     * based on the priority
                     */  
                    if (gasPBIntPortParam[u8PortNum].u8PBPortStatusMask & PB_PORT_STATUS_RENEG_AGAIN)
                    {
                        gasPBIntPortParam[u8PortNum].u8PBPortStatusMask &= ~(PB_PORT_STATUS_RENEG_AGAIN);
                        
                        /*Change the port state to Negotiation Pending*/
                        PB_ChangePortStates (u8PortNum, ePB_RENEGOTIATION_PENDING_STATE, ePB_IDLE_SS);
                        
                        /*Find out the highest port in pending state*/
                        PB_InitiateNextPortNegotiation ();
                    }
                    else
                    {
                        PB_ChangePortStates (u8PortNum, ePB_RENEGOTIATION_COMPLETED_STATE, ePB_IDLE_SS);
                        
                        if (gsPBIntSysParam.u8RecoveringMode)
                        {                           
                            if (u8PortNum == gsPBIntSysParam.u8ReclaimPortNum)
                            {
                                /*If the Reclaim port's Neg is complete, see if the 
                                 * Recover port has enough port to Reneg. Else Reclaim
                                 */
                                u8IsAvailablePwrSufficient = PB_NegotiateIfRequiredPwrAvailableInPool (gsPBIntSysParam.u8RecoverPortNum);

                                if (u8IsAvailablePwrSufficient)
                                {
                                    gsPBIntSysParam.u8RecoveringMode = FALSE;
                                }
                                else
                                {
                                    if (PB_RECLAIM_FAILED == PB_ReclaimPower (gsPBIntSysParam.u8RecoverPortNum))
                                    {
                                       /*There is no power in the lower priority port. Just advertise whatever power
                                         available in the pool*/             
                                        /*Initiate renegotiation for the port with new wattage */
                                        PB_InitiateNegotiationWrapper (gsPBIntSysParam.u8RecoverPortNum, \
                                                gasCfgStatusData.u16SharedPwrCapacityIn250mW + gasPBIntPortParam[u8PortNum].u16NegotiatedPwrIn250mW);
                                        
                                        PB_ChangePortStates (gsPBIntSysParam.u8RecoverPortNum, \
                                                ePB_RENEGOTIATION_IN_PROGRESS_STATE, ePB_FIRST_RENEGOTIATION_IN_PROGRESS_SS);;
                                                
                                        gasCfgStatusData.u16SharedPwrCapacityIn250mW = 0;
                                        gsPBIntSysParam.u8RecoveringMode = FALSE;
                                    }
                                }       
                            }


                        }        
                        else
                        {
                            /*Set Renegotiation Pending for all the lower priority ports*/
                            PB_SetRenegotiationPendingForLowPriorityPorts (u8PortNum);

                            PB_InitiateNextPortNegotiation ();
                        } 

                    }
                }
                
                else if (ePB_WAIT_FOR_ASYNC_REQ_SS == gasPBIntPortParam[u8PortNum].eRenegSubState)
                {
                    /* Calculate the power that was negotiated in the last contract */
                    PB_CalculateNegotiatedPower(u8PortNum, u32SourcePDO, u32SinkRDO);                    
                    
                    PDTimer_Kill (gu8PBTimerID);
                    
                    /*Refill pool with excess power */
                    (void)PB_ReleaseExcessPwr (u8PortNum);
                    
                    if (gasPBIntPortParam[u8PortNum].u8PBPortStatusMask & PB_PORT_STATUS_RENEG_AGAIN)
                    {   
                        gasPBIntPortParam[u8PortNum].u8PBPortStatusMask &= ~(PB_PORT_STATUS_RENEG_AGAIN);        
                        
                        /*Change the port state to Negotiation Pending*/
                        PB_ChangePortStates (u8PortNum, ePB_RENEGOTIATION_PENDING_STATE, ePB_IDLE_SS);
                        
                        /*Find out the highest port in pending state*/
                        PB_InitiateNextPortNegotiation ();
                    }
                    else
                    {
                         PB_InitiateNegotiationWrapper (u8PortNum, gasPBIntPortParam[u8PortNum].u16NegotiatedPwrIn250mW); 
                         
                         PB_ChangePortStates (u8PortNum, ePB_RENEGOTIATION_IN_PROGRESS_STATE,  ePB_SECOND_RENEGOTIATION_IN_PROGRESS_SS);          
                    }   
                }
                
                else
                {
                        /*This is an asynchronous notification. This can happen
                        when some other client had initiated the negotiation or
                        an async request from the sink */ 

                    u16PrevNegPwrIn250mW = gasPBIntPortParam[u8PortNum].u16NegotiatedPwrIn250mW;    
                    
                    PB_CalculateNegotiatedPower(u8PortNum, u32SourcePDO, u32SinkRDO);
                    
                    if (gsPBIntSysParam.u8RecoveringMode)
                    {
                        if (u8PortNum == gsPBIntSysParam.u8ReclaimPortNum)
                        {

                        }

                        if (u8PortNum == gsPBIntSysParam.u8RecoverPortNum)
                        {
                            /*Refill the pool with the power more than the */
                            gasCfgStatusData.u16SharedPwrCapacityIn250mW +=  (u16PrevNegPwrIn250mW - gasPBIntPortParam[u8PortNum].u16NegotiatedPwrIn250mW);                               
                        }
                    }
                    else
                    {
                        if (gasPBIntPortParam[u8PortNum].u16NegotiatedPwrIn250mW < u16PrevNegPwrIn250mW)
                        {    
                            gasCfgStatusData.u16SharedPwrCapacityIn250mW += (u16PrevNegPwrIn250mW - gasPBIntPortParam[u8PortNum].u16NegotiatedPwrIn250mW);
                            
                            /*Renegotiate the same port to the newly negotiated power*/ 
                            PB_InitiateNegotiationWrapper (u8PortNum, gasPBIntPortParam[u8PortNum].u16NegotiatedPwrIn250mW); 
                            
                            PB_ChangePortStates(u8PortNum, ePB_RENEGOTIATION_IN_PROGRESS_STATE, \
                                                ePB_SECOND_RENEGOTIATION_IN_PROGRESS_SS); 
                        }
                    }      
                }
            }
            else
            {
                /* Calculate the power that was negotiated in the last contract */
                PB_CalculateNegotiatedPower(u8PortNum, u32SourcePDO, u32SinkRDO);
                
                /*This notification is to indicate the completion of Initial 15W negotiation */
                gasPBIntPortParam[u8PortNum].u8PBPortStatusMask |= PB_PORT_STATUS_INITIAL_NEG_DONE;              
                
                /* If negotiation is on going for any other port, make this port 
                 as pending. We should complete the negotiation for a port and then
                 only we have to initiate negotiation for other ports. */
                u8IsNegotiationInProgress = PB_IsNegotiationInProgressForOtherPort (u8PortNum);
                
                if (u8IsNegotiationInProgress)
                {
                    PB_ChangePortStates(u8PortNum, ePB_RENEGOTIATION_PENDING_STATE, ePB_IDLE_SS);
                }
                /* Else Initiate Get Sink Caps to determine capability of the partner */
                else
                {
                    if (ePB_SINK_CAPS_NOT_INITIATED == gasPBIntPortParam[u8PortNum].eGetSinkCapSS)
                    {
                        
                        PB_InitiateGetSinkCapsWrapper (u8PortNum);
                    }
                    else
                    {
                        PB_SinkCapsReceivedHandler (u8PortNum);
                    }                                       
                }
            }
            
            break;  
            
        case eMCHP_PSF_GET_SINK_CAPS_RCVD: 
            
            if (ePB_SINK_CAPS_INITIATED == gasPBIntPortParam[u8PortNum].eGetSinkCapSS)
            {
                gasPBIntPortParam[u8PortNum].eGetSinkCapSS = ePB_SINK_CAPS_COMPLETED;

                PB_SinkCapsReceivedHandler (u8PortNum);
            }            
            break; 
            
        case PB_TIMER_EXPIRED_EVENT:   
            
            /* We did not receive any asynchronous request within the time period. So,
              go ahead and refill the pool with excess power */
            (void)PB_ReleaseExcessPwr (u8PortNum);
            
            /*Initiate Negotiation for the port with the negotiated Power*/
            PB_InitiateNegotiationWrapper (u8PortNum, gasPBIntPortParam[u8PortNum].u16NegotiatedPwrIn250mW);
            
            PB_ChangePortStates(u8PortNum, ePB_RENEGOTIATION_IN_PROGRESS_STATE, ePB_SECOND_RENEGOTIATION_IN_PROGRESS_SS);
            
            break; 
            
        case eMCHP_PSF_GET_SINK_CAPS_NOT_RCVD:
            
            if (TRUE == (gasPBIntPortParam[u8PortNum].u8PBPortStatusMask & PB_PORT_STATUS_ATTACH))
            {
                if (ePB_SINK_CAPS_INITIATED == gasPBIntPortParam[u8PortNum].eGetSinkCapSS)
                {
                    /*Re initiate Sink Caps*/
                    PB_InitiateGetSinkCapsWrapper (u8PortNum);
                }
            }
            break; 
#if 0  /* To-do : Decide if PPM_Busy is needed */           
        case eNOTIFY_PPM_BUSY:
            
            /*If the PPM Busy notification is received that means the request was 
             rejected. There are only 2 client request as of now.
             1. Get Sink Caps
             2. Renegotiation*/
            
            if (TRUE == (gasPBIntPortParam[u8PortNum].u8PBPortStatusMask & PB_PORT_STATUS_ATTACH))
            {
                if (ePB_GET_SINKCAPS_SENT_SS == gasPBIntPortParam[u8PortNum].eRenegSubState)
                {
                    /*Re initiate Sink Caps*/
                    PB_InitiateGetSinkCapsWrapper (u8PortNum);
                }
                else if ((ePB_FIRST_RENEGOTIATION_IN_PROGRESS_SS == gasPBIntPortParam[u8PortNum].eRenegSubState) ||\
                        (ePB_SECOND_RENEGOTIATION_IN_PROGRESS_SS == gasPBIntPortParam[u8PortNum].eRenegSubState))
                {            
                    /*Initiate renegotiation with the required power again*/ 
                    PB_InitiateNegotiationWrapper (u8PortNum, gasPBIntPortParam[u8PortNum].u16RequiredPrtPwrIn250mW); 
                    
                    PB_ChangePortStates(u8PortNum, ePB_RENEGOTIATION_IN_PROGRESS_STATE, \
                            gasPBIntPortParam[u8PortNum].eRenegSubState);                    
                }
            }
            break; 
#endif 
        
        default : 
            break; 
    }
    
    return TRUE; 
} 


#endif 