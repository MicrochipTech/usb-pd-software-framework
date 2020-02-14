/*******************************************************************************
 Power Balancing Source File

 Company:
   Microchip Technology Inc.

 File Name:
   PB_Mngr.c

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

#if INCLUDE_POWER_BALANCING


void PB_HandlePPMEvents (UINT8 u8PortNum, UINT8 ePPM_EVENT)
{
    UINT32 u32SourcePDO            = SET_TO_ZERO; 
    UINT32 u32SinkRDO              = SET_TO_ZERO;
    UINT16 u16GivebackPwr          = SET_TO_ZERO;
    UINT16 u16PrevNegPwr           = SET_TO_ZERO;
    UINT16 u16NewWattage           = SET_TO_ZERO;
    UINT8 u8NegotiationInProgress  = FALSE;   
    UINT8 u8AvailablePwrSufficient = FALSE;
        
    switch(ePPM_EVENT)
    {
        case eNOTIFY_PPM_ATTACHED: 
            
            /* To-do : Trigger 15W negotiation if it is not done in init */
            PB_InitiateNegotiationWrapper(u8PortNum, gasPBIntPortParam[u8PortNum].u16MinGuaranteedPwrIn250mW); 
            gasPBIntPortParam[u8PortNum].u8Attach = TRUE;
            gasPBIntPortParam[u8PortNum].u8AttachSeqNo = gu8AttachSeq++;
            
            break; 
            
        case eNOTIFY_PPM_DETACHED: 
            
            /* Give power back to pool.On the following condition, the giveback 
            power is calculated based on the Required Power. This is because, 
            the Port Required Power will have the power subtracted from pool power*/   
            if ((eRENEGOTIATION_IN_PROGRESS == gasPBIntPortParam[u8PortNum].ePBPortState) || \
               (eRENEGOTIATION_COMPLETED == gasPBIntPortParam[u8PortNum].ePBPortState) || \
               (eRENEGOTIATION_PENDING == gasPBIntPortParam[u8PortNum].ePBPortState))
            {
                u16GivebackPwr = gasPBIntPortParam[u8PortNum].u16RequiredPrtPwrIn250mW - \
                             gasPBIntPortParam[u8PortNum].u16MinGuaranteedPwrIn250mW;
            }
             /*The give back power is calculated based on Neg Power since, the required power will have 
             the max power required for the port but Neg power will have the actual power that is
             withdrawn from the pool power. This can happen only when a detach occurs for the 
             recovering port*/
            else if (ePWR_RECOVERING_STATE == gasPBIntPortParam[u8PortNum].ePBPortState)
            {
                u16GivebackPwr = gasPBIntPortParam[u8PortNum].u16NegotiatedPwrIn250mW - \
                            gasPBIntPortParam[u8PortNum].u16MinGuaranteedPwrIn250mW;
            }
            else
            {
                u16GivebackPwr = 0;
            }
            
            /*If the Timer for the port is started, kill the timer*/
            if (eWAIT_FOR_ASYNC_REQ == gasPBIntPortParam[u8PortNum].eRenegSubState)
            {
                //PDTimer_Kill (gu8PBTimerID);
                /* To-do : Kill the Timer */
            }
            
            /*Set Renegotiation pending for lower priority ports only if the 
             port is attached and a PD contract is negotiated*/
            
            if (ePB_IDLE_STATE != gasPBIntPortParam[u8PortNum].ePBPortState)
            {
                /*Set negotiation pending for all the lower priority ports*/
                PB_SetRenegotiationPendingForLowPriorityPorts (u8PortNum);
            }
            
            gsPBIntSysParam.u16PoolPowerIn250mW += u16GivebackPwr;
            
             /*Reset all the state variables and the flags associated with the port*/
            gasPBIntPortParam[u8PortNum].u8Attach = FALSE;             
            gasPBIntPortParam[u8PortNum].u8InitialNegotationDone = FALSE;
            gasPBIntPortParam[u8PortNum].u8IsPortInMinimalPower = FALSE;           
            gasPBIntPortParam[u8PortNum].u8CapabilityMismatch = FALSE;           
              
            PB_UpdateAttachSeq (u8PortNum);
            
            gasPBIntPortParam[u8PortNum].u8AttachSeqNo = 0x00;            
            gasPBIntPortParam[u8PortNum].u16NegotiatedPwrIn250mW = 0;
            gasPBIntPortParam[u8PortNum].u16RequiredPrtPwrIn250mW = 0;
            gasPBIntPortParam[u8PortNum].eRenegSubState = eIDLE_STATE;
            gasPBIntPortParam[u8PortNum].ePBPortState = ePB_IDLE_STATE;
            gasPBIntPortParam[u8PortNum].eGetSinkCapSS = eSINK_CAPS_NOT_INITIATED;
            gasPBIntPortParam[u8PortNum].u8RenegAgain = FALSE;
            gsPortStatusData.sPBPortStatusdata[u8PortNum].u16NegoCurrent = 0;
            gsPortStatusData.sPBPortStatusdata[u8PortNum].u16NegoVoltage = 0; 
                        
            /* To-do : Call PB_InitiateNegotiationWrapper else 
               Initialize PDOs for 15W power. 
               gasPBPortParam[u8PortID].u16PortRequiredPwrIn250mW = u16NewWattage; */
            if (gsPBIntSysParam.u8RecoveringMode)
            {
                if (u8PortNum == gsPBIntSysParam.u8ReclaimPortNum)
                {
                    /*If the recovering mode is on and the detach occurred for the reclaiming
                     port, then see if there is enough power for Recover Port. If not
                     again Reclaim*/
                    u8AvailablePwrSufficient = PB_NegotiateIfRequiredPwrAvailableInPool (gsPBIntSysParam.u8RecoverPortNum);
                    
                    if (u8AvailablePwrSufficient)
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
                            u16NewWattage = gsPBIntSysParam.u16PoolPowerIn250mW + gasPBIntPortParam[u8PortNum].u16NegotiatedPwrIn250mW;
                            PB_InitiateNegotiationWrapper (u8PortNum, u16NewWattage);
                            
                            PB_ChangePortStates(u8PortNum, eRENEGOTIATION_IN_PROGRESS, eFIRST_RENEGOTIATION_IN_PROGRESS); 
                            
                            gsPBIntSysParam.u16PoolPowerIn250mW = 0;
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
            u8NegotiationInProgress = PB_IsNegotiationInProgressForOtherPort(u8PortNum);

            if (!u8NegotiationInProgress)
            {
                PB_InitiateNextPortNegotiation ();
            }
            
            break; 
        
        case eNOTIFY_PPM_CONTRACT_NEGOTIATED:
            
            /* To-do : Call the PPM function which gives the current PDO 
               selected by Sink and RDO raised by Sink. Update them in 
               u32SourcePDO and u32RDO variables */ 
            u32SourcePDO = gasDPM[u8PortNum].u32NegotiatedPDO; 
            u32SinkRDO = gasDPM[u8PortNum].u32SinkReqRDO;
            /* Calculate the power that was negotiated in the last contract */
            PB_CalculateNegotiatedPower(u8PortNum, u32SourcePDO, u32SinkRDO); 
            
            /* If initial negotiation complete, go ahead and publish max caps */
            if (gasPBIntPortParam[u8PortNum].u8InitialNegotationDone)
            {   
                if (eFIRST_RENEGOTIATION_IN_PROGRESS == gasPBIntPortParam[u8PortNum].eRenegSubState)
                {
                    /*Renegotiate that port with new PDO that's assigned*/
                    /*If Reneg Flag is set, then some detach for higher priority port
                     *  has happened when the Negotiation was happening for this port.
                     * Hence we need to again Renegotiate this port as well other ports
                     * based on the priority
                     */    
                    if (gasPBIntPortParam[u8PortNum].u8RenegAgain)
                    {
                        /*Refill pool with excess power */
                        (void)PB_ReleaseExcessPwr (u8PortNum);
                        
                        gasPBIntPortParam[u8PortNum].u8RenegAgain = FALSE;
                        
                        /*Change the port state to Negotiation Pending*/
                        PB_ChangePortStates (u8PortNum, eRENEGOTIATION_PENDING, eIDLE_STATE);
                        
                        /*Find out the highest port in pending state*/
                        PB_InitiateNextPortNegotiation ();
                    }      
                    else
                    {                      
                        /* To-do : Start a timer for 200 ms and change state as WAIT_FOR_ASYNC_REQ*/
                        gu8PBTimerID = PDTimer_Start (gsPBIntSysParam.u32AsyncReqWaitTimer, PB_TimerEnd, u8PortNum, 0);

                        PB_ChangePortStates (u8PortNum, eRENEGOTIATION_IN_PROGRESS, eWAIT_FOR_ASYNC_REQ);

                    }
                }
                else if (eSECOND_RENEGOTIATION_IN_PROGRESS == gasPBIntPortParam[u8PortNum].eRenegSubState)
                {
                   /*Calculate if there is excess power and refill the common pool*/
                   (void)PB_ReleaseExcessPwr (u8PortNum);
                   
                    /*If Reneg Flag is set, then some detach for higher priority port
                     *  has happened when the Negotiation was happening for this port.
                     * Hence we need to again Renegotiate this port as well other ports
                     * based on the priority
                     */  
                    if (gasPBIntPortParam[u8PortNum].u8RenegAgain)
                    {
                        gasPBIntPortParam[u8PortNum].u8RenegAgain = FALSE;
                        
                        /*Change the port state to Negotiation Pending*/
                        PB_ChangePortStates (u8PortNum, eRENEGOTIATION_PENDING, eIDLE_STATE);
                        
                        /*Find out the highest port in pending state*/
                        PB_InitiateNextPortNegotiation ();
                    }
                    else
                    {
                        PB_ChangePortStates (u8PortNum, eRENEGOTIATION_COMPLETED, eIDLE_STATE);
                        
                        if (gsPBIntSysParam.u8RecoveringMode)
                        {                           
                            if (u8PortNum == gsPBIntSysParam.u8ReclaimPortNum)
                            {
                                /*If the Reclaim port's Neg is complete, see if the 
                                 * Recover port has enough port to Reneg. Else Reclaim
                                 */
                                u8AvailablePwrSufficient = PB_NegotiateIfRequiredPwrAvailableInPool (gsPBIntSysParam.u8RecoverPortNum);

                                if (u8AvailablePwrSufficient)
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
                                        u16NewWattage = gsPBIntSysParam.u16PoolPowerIn250mW + gasPBIntPortParam[u8PortNum].u16NegotiatedPwrIn250mW;
                                        PB_InitiateNegotiationWrapper (gsPBIntSysParam.u8RecoverPortNum, u16NewWattage);
                                        
                                        PB_ChangePortStates (gsPBIntSysParam.u8RecoverPortNum, \
                                                eRENEGOTIATION_IN_PROGRESS, eFIRST_RENEGOTIATION_IN_PROGRESS);;
                                                
                                        gsPBIntSysParam.u16PoolPowerIn250mW = 0;
                                        gsPBIntSysParam.u8RecoveringMode = FALSE;
                                    }
                                }       
                            }


                        }                    

                    }
                }
                
                else if (eWAIT_FOR_ASYNC_REQ == gasPBIntPortParam[u8PortNum].eRenegSubState)
                {
                    PDTimer_Kill (gu8PBTimerID);
                    
                    /*Refill pool with excess power */
                    (void)PB_ReleaseExcessPwr (u8PortNum);
                    
                    if (gasPBIntPortParam[u8PortNum].u8RenegAgain)
                    {   
                        gasPBIntPortParam[u8PortNum].u8RenegAgain = FALSE;        
                        
                        /*Change the port state to Negotiation Pending*/
                        PB_ChangePortStates (u8PortNum, eRENEGOTIATION_PENDING, eIDLE_STATE);
                        
                        /*Find out the highest port in pending state*/
                        PB_InitiateNextPortNegotiation ();
                    }
                    else
                    {
                         PB_InitiateNegotiationWrapper (u8PortNum, gasPBIntPortParam[u8PortNum].u16NegotiatedPwrIn250mW); 
                         
                         PB_ChangePortStates (u8PortNum, eRENEGOTIATION_IN_PROGRESS,  eSECOND_RENEGOTIATION_IN_PROGRESS);          
                    }   
                }
                
                else
                {
                        /*This is an asynchronous notification. This can happen
                        when some other client had initiated the negotiation or
                        an async request from the sink */ 

                    u16PrevNegPwr = gasPBIntPortParam[u8PortNum].u16NegotiatedPwrIn250mW;    

                    if (gsPBIntSysParam.u8RecoveringMode)
                    {
                        if (u8PortNum == gsPBIntSysParam.u8ReclaimPortNum)
                        {

                        }

                        if (u8PortNum == gsPBIntSysParam.u8RecoverPortNum)
                        {
                            /*Refill the pool with the power more than the */
                            gsPBIntSysParam.u16PoolPowerIn250mW +=  (u16PrevNegPwr - gasPBIntPortParam[u8PortNum].u16NegotiatedPwrIn250mW);                               
                        }
                    }
                    else
                    {
                        if (gasPBIntPortParam[u8PortNum].u16NegotiatedPwrIn250mW < u16PrevNegPwr)
                        {    
                            gsPBIntSysParam.u16PoolPowerIn250mW += (u16PrevNegPwr - gasPBIntPortParam[u8PortNum].u16NegotiatedPwrIn250mW);
                            
                            /*Renegotiate the same port to the newly negotiated power*/
                            u16NewWattage = gasPBIntPortParam[u8PortNum].u16NegotiatedPwrIn250mW; 
                            PB_InitiateNegotiationWrapper (u8PortNum,u16NewWattage); 
                            
                            PB_ChangePortStates(u8PortNum, eRENEGOTIATION_IN_PROGRESS, \
                                                eSECOND_RENEGOTIATION_IN_PROGRESS); 
                        }
                    }      
                }
            }
            else
            {
                /*This notification is to indicate the completion of Initial 15W negotiation */
                gasPBIntPortParam[u8PortNum].u8InitialNegotationDone = TRUE;              
                
                /* If negotiation is on going for any other port, make this port 
                 as pending. We should complete the negotiation for a port and then
                 only we have to initiate negotiation for other ports. */
                u8NegotiationInProgress = PB_IsNegotiationInProgressForOtherPort (u8PortNum);
                
                if (u8NegotiationInProgress)
                {
                    PB_ChangePortStates(u8PortNum, eRENEGOTIATION_PENDING, eIDLE_STATE);
                }
                /* Else Initiate Get Sink Caps to determine capability of the partner */
                else
                {
                    if (eSINK_CAPS_NOT_INITIATED == gasPBIntPortParam[u8PortNum].eGetSinkCapSS)
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
            
        case eNOTIFY_PPM_SINK_CAPS_RCVD: 
            
            gasPBIntPortParam[u8PortNum].eGetSinkCapSS = eSINK_CAPS_COMPLETED;
            
            PB_SinkCapsReceivedHandler (u8PortNum);
            
            break; 
            
        case PB_TIMER_EXPIRED_EVENT:   
            
            /* We did not receive any asynchronous request within the time period. So,
              go ahead and refill the pool with excess power */
            (void)PB_ReleaseExcessPwr (u8PortNum);
            
            /*Initiate Negotiation for the port with the negotiated Power*/
            u16NewWattage = gasPBIntPortParam[u8PortNum].u16NegotiatedPwrIn250mW; 
            PB_InitiateNegotiationWrapper (u8PortNum, u16NewWattage);
            
            PB_ChangePortStates(u8PortNum, eRENEGOTIATION_IN_PROGRESS, eSECOND_RENEGOTIATION_IN_PROGRESS);
            
            break; 
            
        case eNOTIFY_PPM_SINK_CAPS_NOT_RCVD:
            
            if (TRUE == gasPBIntPortParam[u8PortNum].u8Attach)
            {
                if (eSINK_CAPS_INITIATED == gasPBIntPortParam[u8PortNum].eGetSinkCapSS)
                {
                    /*Re initiate Sink Caps*/
                    PB_InitiateGetSinkCapsWrapper (u8PortNum);
                }
            }
            break; 
            
        case eNOTIFY_PPM_BUSY:
            
            /*If the PPM Busy notification is received that means the request was 
             rejected. There are only 2 client request as of now.
             1. Get Sink Caps
             2. Renegotiation*/
            
            if (TRUE == gasPBIntPortParam[u8PortNum].u8Attach)
            {
                if (eGET_SINKCAPS_SENT == gasPBIntPortParam[u8PortNum].eRenegSubState)
                {
                    /*Re initiate Sink Caps*/
                    PB_InitiateGetSinkCapsWrapper (u8PortNum);
                }
                else if ((eFIRST_RENEGOTIATION_IN_PROGRESS == gasPBIntPortParam[u8PortNum].eRenegSubState) ||\
                        (eSECOND_RENEGOTIATION_IN_PROGRESS == gasPBIntPortParam[u8PortNum].eRenegSubState))
                {            
                    /*Initiate renegotiation with the required power again*/
                    u16NewWattage = gasPBIntPortParam[u8PortNum].u16RequiredPrtPwrIn250mW; 
                    PB_InitiateNegotiationWrapper (u8PortNum, u16NewWattage); 
                    
                    PB_ChangePortStates(u8PortNum, eRENEGOTIATION_IN_PROGRESS, \
                            gasPBIntPortParam[u8PortNum].eRenegSubState);                    
                }
            }
            break; 
        
        default : 
            break; 
    }
} 


#endif 