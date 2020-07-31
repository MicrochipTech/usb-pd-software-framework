/*******************************************************************************
 Power Balancing Helper Source File

 Company:
   Microchip Technology Inc.

 File Name:
   pb_helper.c

 Description:
   This file contains the helper functions that are needed by Power Balancing
   State Machine. 
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

#if (TRUE == INCLUDE_POWER_BALANCING)

void PB_Init(void)
{
    UINT8 u8PortNum; 
        
    /* Initialize System parameters only if PB is enabled for the system */
    if (gasCfgStatusData.u8PBEnableSelect & DPM_PB_ENABLE)
    {
        /* Bank A shall be the default bank selected at power on. */
        gsPBIntSysParam.u16TotalSysPwrIn250mW = gasCfgStatusData.u16SystemPowerBankAIn250mW;
        gsPBIntSysParam.u32AsyncReqWaitTimerInms = PB_ASYN_REQ_WAIT_TIMER_IN_MS; 
        gsPBIntSysParam.u8ReclaimPortNum = SET_TO_ZERO; 
        gsPBIntSysParam.u8RecoverPortNum = SET_TO_ZERO; 
        gsPBIntSysParam.u8RecoveringMode = FALSE;  
        gasCfgStatusData.u16SharedPwrCapacityIn250mW = gsPBIntSysParam.u16TotalSysPwrIn250mW; 
        
        /* Initialize Port Parameters only if the Port role is Source and 
           PB is enabled for the port */
        for (u8PortNum = INDEX_0; u8PortNum < CONFIG_PD_PORT_COUNT; u8PortNum++)
        {
            if (PD_ROLE_SOURCE == DPM_GET_CURRENT_POWER_ROLE(u8PortNum))
            {    
                if (gasCfgStatusData.sPerPortData[u8PortNum].u16FeatureSelect & DPM_PORT_PB_ENABLE)
                {
                    PB_InitializePortParam(u8PortNum); 
                    /* Deduct the Minimum Guaranteed Power from available Shared Capacity */
                    gasCfgStatusData.u16SharedPwrCapacityIn250mW -= gasPBIntPortParam[u8PortNum].u16MinGuaranteedPwrIn250mW; 
                }
            }            
        }
    }
    else
    {
        /* There could be a scenario where PB is not enabled for the 
           system, but enabled for any of the ports. */
        for (u8PortNum = INDEX_0; u8PortNum < CONFIG_PD_PORT_COUNT; u8PortNum++)
        {
            gasCfgStatusData.sPerPortData[u8PortNum].u16FeatureSelect &= ~(DPM_PORT_PB_ENABLE);            
        }
    }
}

void PB_InitializePortParam(UINT8 u8PortNum)
{
    /* Initialize all the Port specific parameters */ 
    gasPBIntPortParam[u8PortNum].u8AttachSeqNo             = SET_TO_ZERO; 
    gasPBIntPortParam[u8PortNum].eRenegSubState            = ePB_IDLE_SS; 
    gasPBIntPortParam[u8PortNum].u16NegotiatedPwrIn250mW   = SET_TO_ZERO; 
    gasPBIntPortParam[u8PortNum].eGetSinkCapSS             = ePB_SINK_CAPS_NOT_INITIATED; 
    gasPBIntPortParam[u8PortNum].u8PBPortStatusMask        = FALSE; 
    gasPBIntPortParam[u8PortNum].ePBPortState              = ePB_IDLE_STATE;
    gasPBIntPortParam[u8PortNum].u16RequiredPrtPwrIn250mW  = SET_TO_ZERO; 
    /* Bank A shall be the default bank selected at power on. */
    gasPBIntPortParam[u8PortNum].u16MinGuaranteedPwrIn250mW = gasCfgStatusData.u16MinPowerBankAIn250mW;
    gasPBIntPortParam[u8PortNum].u16MaxPortPwrIn250mW       = gasCfgStatusData.sPBPerPortData[u8PortNum].u16MaxPrtPwrBankAIn250mW; 
}

void PB_ChangePortStates(UINT8 u8PortNum, PB_PORT_STATES ePortState, PB_RENEG_SUBSTATE ePortSubState)
{
    if (ePB_INVALID_STATE != ePortState)
    {
        gasPBIntPortParam[u8PortNum].ePBPortState = ePortState;
    }
    
    if (ePB_INVALID_SS != ePortSubState)
    {
        gasPBIntPortParam[u8PortNum].eRenegSubState = ePortSubState;
    }    
}

void PB_CalculateNegotiatedPower(UINT8 u8PortNum, UINT32 u32PDO, UINT32 u32RDO)
{
    UINT16 u16NegVoltIn50mV;
    UINT16 u16CurrentIn10mA = SET_TO_ZERO;
    UINT16 u16TempPwrIn10mW;
    float fNegotiatedVoltageInV; 
    
    /*we need to calculate the power the sink is drawing and 
     give back the rest to the pool*/
    u16NegVoltIn50mV = PB_GET_VOLTAGE_FROM_FIXED_PDO(u32PDO); 
       
    /* Convert the voltage from 50mV units to V */
    fNegotiatedVoltageInV = PB_CONVERT_PDO_VOLTAGE_FROM_50mV_TO_V(u16NegVoltIn50mV); 
    
     /*If (sink indicates Giveback support) then 
           Port Negotiated Power = Requested Operating Current * Selected PDO Voltage
       Else 
            If(Capability mismatch is set)
               Port Negotiated Power = Requested Operating Current * Selected PDO Voltage
            Else
              Port Negotiated Power = Requested Max Current * Selected PDO Voltage 
     */     
    if (PB_IS_GIVEBACK_SUPPORTED(u32RDO))
    {
       u16CurrentIn10mA = PB_GET_OPERATING_CURRENT_FROM_RDO(u32RDO);  
    }
    else
    {
        if (PB_IS_CAPABILITY_MISMATCH(u32RDO))
        {
            gasPBIntPortParam[u8PortNum].u8PBPortStatusMask |= PB_PORT_STATUS_CAPABILITY_MISMATCH;
            u16CurrentIn10mA = PB_GET_OPERATING_CURRENT_FROM_RDO(u32RDO);
        }
        else
        {
            gasPBIntPortParam[u8PortNum].u8PBPortStatusMask &= ~(PB_PORT_STATUS_CAPABILITY_MISMATCH);
            u16CurrentIn10mA = PB_GET_MAX_CURRENT_FROM_RDO(u32RDO);
        }        
    }
    
    /* Update the global status register */
    gasCfgStatusData.sPerPortData[u8PortNum].u16NegoCurrentInmA = (u16CurrentIn10mA * DPM_PDO_CURRENT_UNIT); 
    
    /* Calculate power in terms of 10mW */
    u16TempPwrIn10mW = (UINT16) (fNegotiatedVoltageInV * u16CurrentIn10mA);
    
    /* Convert the power in terms of 250 mW */
    gasPBIntPortParam[u8PortNum].u16NegotiatedPwrIn250mW = PB_CONVERT_POWER_FROM_10mW_TO_250mW(u16TempPwrIn10mW); 
    
    if (gasPBIntPortParam[u8PortNum].u16NegotiatedPwrIn250mW <=     \
                    gasPBIntPortParam[u8PortNum].u16MinGuaranteedPwrIn250mW)
    {
       gasPBIntPortParam[u8PortNum].u16NegotiatedPwrIn250mW = gasPBIntPortParam[u8PortNum].u16MinGuaranteedPwrIn250mW;  
       gasPBIntPortParam[u8PortNum].u8PBPortStatusMask |= PB_PORT_STATUS_PORT_IN_MIN_PWR; 
    }
    else
    {
       gasPBIntPortParam[u8PortNum].u8PBPortStatusMask &= ~(PB_PORT_STATUS_PORT_IN_MIN_PWR);
    }
    
    gasCfgStatusData.sPerPortData[u8PortNum].u16AllocatedPowerIn250mW = gasPBIntPortParam[u8PortNum].u16NegotiatedPwrIn250mW; 
}

void PB_InitiateNegotiationWrapper(UINT8 u8PortNum, UINT16 u16NewWattageIn250mW)
{
    /* Update the PDOs in New PDO registers */
    DPM_UpdateSrcPDOfromPwr(u8PortNum, u16NewWattageIn250mW); 
    
    /* Raise Renegotiation request to DPM */
    DPM_RegisterInternalEvent(u8PortNum, DPM_INT_EVT_INITIATE_RENOGIATION);
            
    gasPBIntPortParam[u8PortNum].u16RequiredPrtPwrIn250mW = u16NewWattageIn250mW;   
}

void PB_InitiateGetSinkCapsWrapper(UINT8 u8PortNum)
{
    /* Raise Get Sink caps request to DPM */
    DPM_RegisterInternalEvent(u8PortNum, DPM_INT_EVT_INITIATE_GET_SINK_CAPS);
    
    PB_ChangePortStates (u8PortNum, ePB_RENEGOTIATION_IN_PROGRESS_STATE, ePB_GET_SINKCAPS_SENT_SS);
    
    gasPBIntPortParam[u8PortNum].eGetSinkCapSS = ePB_SINK_CAPS_INITIATED; 
}

void PB_CalculateRequiredPortPower(UINT8 u8PortNum, UINT8 u8SinkPDOCnt, const UINT32 *pu32SinkCap)
{
    UINT16 u16PortMaxRequiredPwrIn250mW = SET_TO_ZERO;  
    UINT16 u16TempPwrIn10mW = SET_TO_ZERO, u16TempCurrIn10mA = SET_TO_ZERO, u16TempVoltIn50mV = SET_TO_ZERO; 
    UINT8  u8PDOCnt;
    float fPDOVoltageInV = SET_TO_ZERO; 
    
    for (u8PDOCnt = INDEX_0; u8PDOCnt < u8SinkPDOCnt; u8PDOCnt++)
    {
        if ((ePDO_FIXED == (ePDOtypes)PB_GET_PDO_TYPE (pu32SinkCap[u8PDOCnt])) || \
                (ePDO_VARIABLE == (ePDOtypes)PB_GET_PDO_TYPE (pu32SinkCap[u8PDOCnt])))
        {
            if (ePDO_FIXED == (ePDOtypes)PB_GET_PDO_TYPE (pu32SinkCap[u8PDOCnt]))
            {      
                u16TempVoltIn50mV = PB_GET_VOLTAGE_FROM_FIXED_PDO (pu32SinkCap[u8PDOCnt]);          
            }
            else
            {
                u16TempVoltIn50mV = PB_GET_VARIABLE_PDO_MAX_VOLTAGE (pu32SinkCap[u8PDOCnt]);     
            }
            
            fPDOVoltageInV = PB_CONVERT_PDO_VOLTAGE_FROM_50mV_TO_V (u16TempVoltIn50mV);
            
            u16TempCurrIn10mA = PB_GET_CURRENT_FROM_PDO (pu32SinkCap[u8PDOCnt]);
            
            u16TempPwrIn10mW =  (UINT16)(u16TempCurrIn10mA * fPDOVoltageInV); 
            
            if (u16TempPwrIn10mW > u16PortMaxRequiredPwrIn250mW)
            {
                /* Now power will be in units of 10mW */
                u16PortMaxRequiredPwrIn250mW = u16TempPwrIn10mW;
            }
        }
    }    
    
    /* Convert the Power in terms of 250mW */
    u16PortMaxRequiredPwrIn250mW = PB_CONVERT_POWER_FROM_10mW_TO_250mW(u16PortMaxRequiredPwrIn250mW);
    
    /* Required Power should be greater than Minimum Guaranteed power and should 
       be within the limit of Maximum Port Power */
    if (u16PortMaxRequiredPwrIn250mW > gasPBIntPortParam[u8PortNum].u16MinGuaranteedPwrIn250mW)
    {
        gasPBIntPortParam[u8PortNum].u16RequiredPrtPwrIn250mW = MIN (u16PortMaxRequiredPwrIn250mW, gasPBIntPortParam[u8PortNum].u16MaxPortPwrIn250mW);        
    }
    else
    {
        gasPBIntPortParam[u8PortNum].u16RequiredPrtPwrIn250mW = gasPBIntPortParam[u8PortNum].u16MinGuaranteedPwrIn250mW;
    } 
}

UINT8 PB_NegotiateIfRequiredPwrAvailableInPool(UINT8 u8PortNum)
{
    UINT16 u16AvailablePwrIn250mW; 
    UINT16 u16NewWattageIn250mW = SET_TO_ZERO; 
    UINT8 u8IsRequiredPwrAvailable = FALSE;
    
    /* Include the current negotiated power of the port along with pool power. */
    u16AvailablePwrIn250mW = gasCfgStatusData.u16SharedPwrCapacityIn250mW + gasPBIntPortParam[u8PortNum].u16NegotiatedPwrIn250mW;
    
    /* Check if required power is available in pool */
    if (u16AvailablePwrIn250mW >= gasPBIntPortParam[u8PortNum].u16RequiredPrtPwrIn250mW)
    {
        /* Exclude Negotiated power while reducing the pool power since it would have 
           been already deducted */
        gasCfgStatusData.u16SharedPwrCapacityIn250mW -= (gasPBIntPortParam[u8PortNum].u16RequiredPrtPwrIn250mW \
                                            - gasPBIntPortParam[u8PortNum].u16NegotiatedPwrIn250mW);                       
        
        /* Initiate Negotiation with new wattage value */
        u16NewWattageIn250mW = gasPBIntPortParam[u8PortNum].u16RequiredPrtPwrIn250mW; 
        PB_InitiateNegotiationWrapper(u8PortNum, u16NewWattageIn250mW); 
        
        PB_ChangePortStates(u8PortNum, ePB_RENEGOTIATION_IN_PROGRESS_STATE, ePB_FIRST_RENEGOTIATION_IN_PROGRESS_SS); 
        
        u8IsRequiredPwrAvailable = TRUE; 
    }
    
    return u8IsRequiredPwrAvailable;
}

UINT8 PB_ReturnHigherPriorityPortWithoutCapMismatch (UINT8 u8PortNum1, UINT8 u8PortNum2)
{
    UINT8 u8HigherPriorityPort = u8PortNum2;
    
    if (PB_GET_PORT_PRIORITY(u8PortNum1) < PB_GET_PORT_PRIORITY(u8PortNum2))
    {
        u8HigherPriorityPort = u8PortNum1;      
    }
    else if (PB_GET_PORT_PRIORITY(u8PortNum1) == PB_GET_PORT_PRIORITY(u8PortNum2))
    {
        if (FIRST_COME_FIRST_SERVE_ALGO == gasCfgStatusData.u8PBEnableSelect)
        {
            if ((gasPBIntPortParam[u8PortNum1].u8AttachSeqNo) < \
                (gasPBIntPortParam[u8PortNum2].u8AttachSeqNo)) 
            {
                u8HigherPriorityPort = u8PortNum1;
            }
        }
        else if (LAST_COME_FIRST_SERVE_ALGO == gasCfgStatusData.u8PBEnableSelect)
        {
            if ((gasPBIntPortParam[u8PortNum1].u8AttachSeqNo) > \
                (gasPBIntPortParam[u8PortNum2].u8AttachSeqNo)) 
            {
                u8HigherPriorityPort = u8PortNum1;
            }
        }
        else
        {
            u8HigherPriorityPort = PB_INVALID_PORT;
        }
        
    }
    else
    {
        u8HigherPriorityPort = u8PortNum2;
    }
      
    return u8HigherPriorityPort;
}

UINT8 PB_ReturnHigherPriorityPort (UINT8 u8PortNum1, UINT8 u8PortNum2)
{
    UINT8 u8HigherPriorityPort = u8PortNum2;
    
    if (PB_GET_PORT_PRIORITY(u8PortNum1) < PB_GET_PORT_PRIORITY(u8PortNum2))
    {
        u8HigherPriorityPort = u8PortNum1;       
    }
    else if (PB_GET_PORT_PRIORITY(u8PortNum1) == PB_GET_PORT_PRIORITY(u8PortNum2))
    {
        if ((PB_PORT_STATUS_CAPABILITY_MISMATCH == (gasPBIntPortParam[u8PortNum1].u8PBPortStatusMask & \
                                        PB_PORT_STATUS_CAPABILITY_MISMATCH)) && \
            (FALSE == (gasPBIntPortParam[u8PortNum2].u8PBPortStatusMask & PB_PORT_STATUS_CAPABILITY_MISMATCH)))
        {
            u8HigherPriorityPort = u8PortNum1;        
        }
        else if ((FALSE == (gasPBIntPortParam[u8PortNum1].u8PBPortStatusMask & PB_PORT_STATUS_CAPABILITY_MISMATCH)) && \
                 (PB_PORT_STATUS_CAPABILITY_MISMATCH == (gasPBIntPortParam[u8PortNum2].u8PBPortStatusMask & \
                                                PB_PORT_STATUS_CAPABILITY_MISMATCH)))
        {
            /* */
        }
        else
        {
            if (FIRST_COME_FIRST_SERVE_ALGO == gasCfgStatusData.u8PBEnableSelect)
            {
                if ((gasPBIntPortParam[u8PortNum1].u8AttachSeqNo) < \
                    (gasPBIntPortParam[u8PortNum2].u8AttachSeqNo)) 
                {
                    u8HigherPriorityPort = u8PortNum1;   
                }
            }
            else if (LAST_COME_FIRST_SERVE_ALGO == gasCfgStatusData.u8PBEnableSelect)
            {
                if ((gasPBIntPortParam[u8PortNum1].u8AttachSeqNo) > \
                    (gasPBIntPortParam[u8PortNum2].u8AttachSeqNo)) 
                {
                    u8HigherPriorityPort = u8PortNum1;
                }
            }
            else
            {
                u8HigherPriorityPort = PB_INVALID_PORT;
            }
        }
    }
    else
    {
        u8HigherPriorityPort = PB_INVALID_PORT;
    }
    
    return u8HigherPriorityPort;
}

UINT8 PB_IdentifyLowestPriorityPort(UINT8 u8PortNum)
{
    UINT8 u8LoopPortNum;
    UINT8 u8LowPriorityPort = u8PortNum;  
    
    for (u8LoopPortNum = INDEX_0; u8LoopPortNum < CONFIG_PD_PORT_COUNT ; u8LoopPortNum++)
    {    
        if (u8LoopPortNum != u8PortNum)
        {
            /*Continue to consider the port only if it is in attached state and also
            the renegotiation should have been complete and also the port 
            should have some power in the reserve to give back */
            
            if ((gasPBIntPortParam[u8LoopPortNum].u8PBPortStatusMask & PB_PORT_STATUS_ATTACH) && \
                (ePB_RENEGOTIATION_COMPLETED_STATE == gasPBIntPortParam[u8LoopPortNum].ePBPortState)&& \
                (FALSE == (gasPBIntPortParam[u8LoopPortNum].u8PBPortStatusMask & PB_PORT_STATUS_PORT_IN_MIN_PWR)))
            {
                /*This check is done here so that Cap Mismatch is not considered 
                 when we are comparing the port for which we need to reclaim. If we
                 do so we form a loop which makes the Higher Priority port get less
                 Power*/
                if (u8LowPriorityPort == u8PortNum)
                {
                    if (u8LowPriorityPort == PB_ReturnHigherPriorityPortWithoutCapMismatch(u8LoopPortNum, u8LowPriorityPort))
                    {
                        u8LowPriorityPort = u8LoopPortNum;             
                    }
                }
                else
                {
                	if (u8LowPriorityPort == PB_ReturnHigherPriorityPort(u8LoopPortNum, u8LowPriorityPort))
                	{
                   		u8LowPriorityPort = u8LoopPortNum;              
                    }
                }
            }

        }
    }
    return u8LowPriorityPort;
}

UINT8 PB_ReclaimPower(UINT8 u8PortNum)
{
    UINT16 u16RecoverPwrIn250mW = SET_TO_ZERO; 
    UINT16 u16RenegotiatePwrIn250mW = SET_TO_ZERO;
    UINT16 u16AvailablePwrIn250mW = SET_TO_ZERO;    
    UINT8 u8ReclaimStatus = PB_RECLAIM_FAILED;
    UINT8 u8LowPriorityPort;  
    
    u8LowPriorityPort = PB_IdentifyLowestPriorityPort(u8PortNum);
    
    if (u8LowPriorityPort != u8PortNum)   
    { 
        /*Calculate how much power can be recovered from lower priority port*/      
        gsPBIntSysParam.u8RecoveringMode = TRUE;
        gsPBIntSysParam.u8RecoverPortNum = u8PortNum;
        gsPBIntSysParam.u8ReclaimPortNum = u8LowPriorityPort;
        
        u16AvailablePwrIn250mW = gasCfgStatusData.u16SharedPwrCapacityIn250mW + gasPBIntPortParam[u8PortNum].u16NegotiatedPwrIn250mW;
        
        /*This is the power that must be recovered from the lower priority port*/
        u16RecoverPwrIn250mW = gasPBIntPortParam[u8PortNum].u16RequiredPrtPwrIn250mW - u16AvailablePwrIn250mW;

        u16RenegotiatePwrIn250mW = MAX ((gasPBIntPortParam[gsPBIntSysParam.u8ReclaimPortNum].u16NegotiatedPwrIn250mW - \
                            u16RecoverPwrIn250mW), gasPBIntPortParam[gsPBIntSysParam.u8ReclaimPortNum].u16MinGuaranteedPwrIn250mW);
        
        /*Update the pool with the power we recovered*/
        gasCfgStatusData.u16SharedPwrCapacityIn250mW += (gasPBIntPortParam[gsPBIntSysParam.u8ReclaimPortNum].u16NegotiatedPwrIn250mW \
                                                    - u16RenegotiatePwrIn250mW);
        
        /* Renegotiate the lower priority port with new power */
        PB_InitiateNegotiationWrapper (gsPBIntSysParam.u8ReclaimPortNum, u16RenegotiatePwrIn250mW);
        
        /* Change the state of reclaiming port to Renegotiation_in_progress */
        PB_ChangePortStates (gsPBIntSysParam.u8ReclaimPortNum, ePB_RENEGOTIATION_IN_PROGRESS_STATE, ePB_FIRST_RENEGOTIATION_IN_PROGRESS_SS);
        
        /*Change the state of recovering Port to IDLE*/
        PB_ChangePortStates (gsPBIntSysParam.u8RecoverPortNum, ePB_PWR_RECOVERING_STATE, ePB_IDLE_SS);
       
        u8ReclaimStatus = PB_RECLAIM_SUCCESS;  
    }
    else
    {
        /* Lower Priority Port Not Identified */
    }
    return u8ReclaimStatus;
    
}

void PB_SinkCapsReceivedHandler(UINT8 u8PortNum)
{
    UINT8 u8SinkPDOCnt; 
    UINT32 *pu32SinkCap; 
    UINT16 u16AvailablePwrIn250mW = SET_TO_ZERO; 
    UINT8 u8IsAvailablePwrSufficient; 
   
    /* Get the Sink capabilities from DPM */
    u8SinkPDOCnt = gasCfgStatusData.sPerPortData[u8PortNum].u8PartnerPDOCnt;  
    pu32SinkCap = (UINT32 *)&gasCfgStatusData.sPerPortData[u8PortNum].u32aPartnerPDO[INDEX_0]; 
    
    /* Calculate the power required for the port based on Sink caps values */
    PB_CalculateRequiredPortPower(u8PortNum, u8SinkPDOCnt, pu32SinkCap); 

    u8IsAvailablePwrSufficient = PB_NegotiateIfRequiredPwrAvailableInPool(u8PortNum); 
    
    if (!u8IsAvailablePwrSufficient)
    {
        if (PB_RECLAIM_FAILED == PB_ReclaimPower (u8PortNum))
        {
            /*There is no power in the lower priority port. Just advertise 
             whatever power is available in the pool */  
            u16AvailablePwrIn250mW = gasCfgStatusData.u16SharedPwrCapacityIn250mW + gasPBIntPortParam[u8PortNum].u16NegotiatedPwrIn250mW;
            
            /*Initiate renegotiation for the port with u16AvailablePwr */
            PB_InitiateNegotiationWrapper (u8PortNum, u16AvailablePwrIn250mW);
            
            /* Change the states of the port */
            PB_ChangePortStates (u8PortNum, ePB_RENEGOTIATION_IN_PROGRESS_STATE, ePB_FIRST_RENEGOTIATION_IN_PROGRESS_SS);
            
            gasCfgStatusData.u16SharedPwrCapacityIn250mW = SET_TO_ZERO;
            gsPBIntSysParam.u8RecoveringMode = FALSE;
        }        
    }
}

UINT8 PB_ReleaseExcessPwr(UINT8 u8PortNum)
{
    UINT16 u16ExcessPwr;
    UINT8 u8IsExcessPwrReturned = FALSE;    
    
     /*The Allocated Power for the Port would not have been negotiated. 
     The sink could have negotiated for a lesser power. We need to giveback
     the excess power to the pool*/
    
    u16ExcessPwr = gasPBIntPortParam[u8PortNum].u16RequiredPrtPwrIn250mW - \
                            gasPBIntPortParam[u8PortNum].u16NegotiatedPwrIn250mW; 
    
    gasCfgStatusData.u16SharedPwrCapacityIn250mW += u16ExcessPwr; 
    
    /*After giving back the excess power make the required power same as negotiated power*/
    
    gasPBIntPortParam[u8PortNum].u16RequiredPrtPwrIn250mW = gasPBIntPortParam[u8PortNum].u16NegotiatedPwrIn250mW; 
    
    if (u16ExcessPwr != PB_ZERO_EXCESS_POWER) 
    {
        u8IsExcessPwrReturned = TRUE;
    }
    
    return u8IsExcessPwrReturned; 
}

void PB_SetRenegotiationPendingForLowPriorityPorts(UINT8 u8PortNum)
{
    UINT8 u8LowPriorityPort;
    
    for (u8LowPriorityPort = INDEX_0; u8LowPriorityPort < CONFIG_PD_PORT_COUNT ; u8LowPriorityPort++)
    {
        if (u8LowPriorityPort != u8PortNum)
        {
            if ((gasPBIntPortParam[u8LowPriorityPort].u8PBPortStatusMask & PB_PORT_STATUS_ATTACH) && \
                (gasPBIntPortParam[u8LowPriorityPort].u8PBPortStatusMask & PB_PORT_STATUS_INITIAL_NEG_DONE))
            {               
                /*Change the state to Negotiation pending if the port is in lower
                 priority. If the negotiation is in progress for the port, set the Reneg 
                 flag so that the port will be renegotiated once it is negotiated*/
                if (u8PortNum == PB_ReturnHigherPriorityPortWithoutCapMismatch (u8PortNum, u8LowPriorityPort))
                {
                    if (gasPBIntPortParam[u8LowPriorityPort].ePBPortState != ePB_RENEGOTIATION_IN_PROGRESS_STATE)
                    {
                        PB_ChangePortStates (u8LowPriorityPort, ePB_RENEGOTIATION_PENDING_STATE, ePB_IDLE_SS);
                    }
                    else
                    {
                        gasPBIntPortParam[u8LowPriorityPort].u8PBPortStatusMask |= PB_PORT_STATUS_RENEG_AGAIN;
                    }
                }

            }
        }
        
    }
    
}

void PB_UpdateAttachSeq(UINT8 u8PortNum)
{
    UINT8 u8OtherPorts;
    
    for (u8OtherPorts = INDEX_0; u8OtherPorts < CONFIG_PD_PORT_COUNT ; u8OtherPorts++)
    {
        if (u8OtherPorts != u8PortNum)
        {
            if (gasPBIntPortParam[u8OtherPorts].u8AttachSeqNo > gasPBIntPortParam[u8PortNum].u8AttachSeqNo)
            {
                gasPBIntPortParam[u8OtherPorts].u8AttachSeqNo--;
            }
        }
    }
    
    gu8AttachSeq--;   
}

UINT8 PB_IsNegotiationInProgressForOtherPort (UINT8 u8PortNum)
{
    UINT8 u8OtherPorts;
    UINT8 u8RetVal = FALSE;
   
    for (u8OtherPorts = INDEX_0; u8OtherPorts < CONFIG_PD_PORT_COUNT ; u8OtherPorts++)
    {
        if (u8OtherPorts != u8PortNum)
        {
            if (ePB_RENEGOTIATION_IN_PROGRESS_STATE == gasPBIntPortParam[u8OtherPorts].ePBPortState)    
            {                                  
                u8RetVal = TRUE;
                break;
            }
        }
    }
    
    return u8RetVal;
}

UINT8 PB_IdentifyHighestPriorityPortInPendingState(void)
{
    UINT8 u8LoopPortNum;
    UINT8 u8NextPort = PB_INVALID_PORT;
   
    for (u8LoopPortNum = INDEX_0; u8LoopPortNum < CONFIG_PD_PORT_COUNT; u8LoopPortNum++)
    {
        if (ePB_RENEGOTIATION_PENDING_STATE == gasPBIntPortParam[u8LoopPortNum].ePBPortState)
        {
            if (PB_INVALID_PORT == u8NextPort)
            {
                u8NextPort = u8LoopPortNum;
            
            }
            else
            {
                u8NextPort = PB_ReturnHigherPriorityPort(u8LoopPortNum, u8NextPort);

            }
        
        }

    }
   
    return u8NextPort;
}

void PB_InitiateNextPortNegotiation(void)
{
    UINT8 u8HighestPortInPendingstate;
   
    u8HighestPortInPendingstate = PB_IdentifyHighestPriorityPortInPendingState();
                
    if (u8HighestPortInPendingstate != PB_INVALID_PORT) 
    {
        if (ePB_SINK_CAPS_NOT_INITIATED == gasPBIntPortParam[u8HighestPortInPendingstate].eGetSinkCapSS)
        {
            PB_InitiateGetSinkCapsWrapper (u8HighestPortInPendingstate);
        }
        else
        {
            PB_SinkCapsReceivedHandler (u8HighestPortInPendingstate);
        }        
    }
}

void PB_AsyncTimerCB(UINT8 u8PortNum, UINT8 u8Dummy)
{
    /* We did not receive any asynchronous request within the time period. So,
    go ahead and refill the pool with excess power */
    (void)PB_ReleaseExcessPwr (u8PortNum);
            
    /*Initiate Negotiation for the port with the negotiated Power*/
    PB_InitiateNegotiationWrapper (u8PortNum, gasPBIntPortParam[u8PortNum].u16NegotiatedPwrIn250mW);
            
    PB_ChangePortStates(u8PortNum, ePB_RENEGOTIATION_IN_PROGRESS_STATE, ePB_SECOND_RENEGOTIATION_IN_PROGRESS_SS);
}

void PB_HandleReclaimPortDetachOrRenegCmplt(void)
{
    UINT8 u8IsAvailablePwrSufficient = PB_NegotiateIfRequiredPwrAvailableInPool (gsPBIntSysParam.u8RecoverPortNum);
                    
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
            /*Initiate renegotiation for the recovering port with new wattage*/
            PB_InitiateNegotiationWrapper (gsPBIntSysParam.u8RecoverPortNum, \
                    (UINT16)(gasCfgStatusData.u16SharedPwrCapacityIn250mW + gasPBIntPortParam[gsPBIntSysParam.u8RecoverPortNum].u16NegotiatedPwrIn250mW));
                            
            PB_ChangePortStates(gsPBIntSysParam.u8RecoverPortNum, ePB_RENEGOTIATION_IN_PROGRESS_STATE, ePB_FIRST_RENEGOTIATION_IN_PROGRESS_SS); 
                            
            gasCfgStatusData.u16SharedPwrCapacityIn250mW = SET_TO_ZERO;
            gsPBIntSysParam.u8RecoveringMode = FALSE;
        }                       
    }
    
}

void PB_HandleHighPriorityPortDetach(UINT8 u8PortNum)
{
    gasPBIntPortParam[u8PortNum].u8PBPortStatusMask &= ~(PB_PORT_STATUS_RENEG_AGAIN);
                        
    /*Change the port state to Negotiation Pending*/
    PB_ChangePortStates (u8PortNum, ePB_RENEGOTIATION_PENDING_STATE, ePB_IDLE_SS);
                        
    /*Find out the highest port in pending state*/
    PB_InitiateNextPortNegotiation ();    
}

void PB_OnPTBankSwitch(UINT8 u8PortNum)
{
    /* Update global and per port parameters of PB */
    if (DPM_PD_THROTTLE_BANK_A == DPM_GET_CURRENT_PT_BANK)
    {
        gsPBIntSysParam.u16TotalSysPwrIn250mW = gasCfgStatusData.u16SystemPowerBankAIn250mW;
        gasPBIntPortParam[u8PortNum].u16MinGuaranteedPwrIn250mW = gasCfgStatusData.u16MinPowerBankAIn250mW;
        gasPBIntPortParam[u8PortNum].u16MaxPortPwrIn250mW       = gasCfgStatusData.sPBPerPortData[u8PortNum].u16MaxPrtPwrBankAIn250mW;         
    }
    else if (DPM_PD_THROTTLE_BANK_B == DPM_GET_CURRENT_PT_BANK)
    {
        gsPBIntSysParam.u16TotalSysPwrIn250mW = gasCfgStatusData.u16SystemPowerBankBIn250mW;
        gasPBIntPortParam[u8PortNum].u16MinGuaranteedPwrIn250mW = gasCfgStatusData.u16MinPowerBankBIn250mW;
        gasPBIntPortParam[u8PortNum].u16MaxPortPwrIn250mW       = gasCfgStatusData.sPBPerPortData[u8PortNum].u16MaxPrtPwrBankBIn250mW; 
    }
    else if (DPM_PD_THROTTLE_BANK_C == DPM_GET_CURRENT_PT_BANK)
    {
        gsPBIntSysParam.u16TotalSysPwrIn250mW = gasCfgStatusData.u16SystemPowerBankCIn250mW;                
        gasPBIntPortParam[u8PortNum].u16MinGuaranteedPwrIn250mW = gasCfgStatusData.u16MinPowerBankCIn250mW;
        gasPBIntPortParam[u8PortNum].u16MaxPortPwrIn250mW       = gasCfgStatusData.sPBPerPortData[u8PortNum].u16MaxPrtPwrBankCIn250mW;                 
    }
    else
    {
       // Do Nothing 
    } 
    
    gasCfgStatusData.u16SharedPwrCapacityIn250mW = gsPBIntSysParam.u16TotalSysPwrIn250mW; 
    
    for (UINT8 u8Index = INDEX_0; u8Index < CONFIG_PD_PORT_COUNT; u8Index++)
    {
        if (PD_ROLE_SOURCE == DPM_GET_CURRENT_POWER_ROLE(u8PortNum))
        {    
            gasCfgStatusData.u16SharedPwrCapacityIn250mW -= gasPBIntPortParam[u8Index].u16MinGuaranteedPwrIn250mW; 
        }
    }   
    
    if (TRUE == (gasPBIntPortParam[u8PortNum].u8PBPortStatusMask & PB_PORT_STATUS_ATTACH))
    {
        /* Clear the initial Negotiation Done status since renegotiation for 
           port is going to start again with minimum guaranteed power value */
        gasPBIntPortParam[u8PortNum].u8PBPortStatusMask &= ~(PB_PORT_STATUS_INITIAL_NEG_DONE);
        
        PB_InitiateNegotiationWrapper(u8PortNum, gasPBIntPortParam[u8PortNum].u16MinGuaranteedPwrIn250mW);
    }
    else
    {
        /* No need to do anything since we expect an attach notification from DPM */
    }
}

#endif
