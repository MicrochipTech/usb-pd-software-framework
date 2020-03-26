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

void PB_Init(void)
{
    UINT8 u8PortNum; 
    UINT16 u16TotSysPwr = 0; 
        
    /* Initialize System parameters only if PB is enabled for the system */
    if (gasCfgStatusData.u8PBEnableSelect & CFG_PB_ENABLE)
    {
        /* Get the Total System Power based on currently selected Throttling bank */
        if (CFG_PD_THROTTLE_BANK_A == gasCfgStatusData.u8PwrThrottleCfg) 
        {
            u16TotSysPwr = gasCfgStatusData.u16SystemPowerBankA;
        }
        else if (CFG_PD_THROTTLE_BANK_B == gasCfgStatusData.u8PwrThrottleCfg)
        {
            u16TotSysPwr = gasCfgStatusData.u16SystemPowerBankB;
        }
        else if (CFG_PD_THROTTLE_BANK_C == gasCfgStatusData.u8PwrThrottleCfg)
        {
            u16TotSysPwr = gasCfgStatusData.u16SystemPowerBankC; 
        }
        else
        {
            /* Power Throttling Shutdown mode */
        }
    
        gsPBIntSysParam.u16TotalSysPwrIn250mW = u16TotSysPwr;
        gsPBIntSysParam.u32AsyncReqWaitTimer = PB_ASYN_REQ_WAIT_TIMER; 
        gsPBIntSysParam.u8ReclaimPortNum = SET_TO_ZERO; 
        gsPBIntSysParam.u8RecoverPortNum = SET_TO_ZERO; 
        gsPBIntSysParam.u8RecoveringMode = FALSE;         
        gasCfgStatusData.u16SharedPowerCapacity = gsPBIntSysParam.u16TotalSysPwrIn250mW; 
        
        /* Initialize Port Parameters only if the Port role is Source and 
           PB is enabled for the port */
        for (u8PortNum = 0; u8PortNum < CONFIG_PD_PORT_COUNT; u8PortNum++)
        {
            if (PD_ROLE_SOURCE == DPM_GET_CURRENT_POWER_ROLE(u8PortNum))
            {    
                if (gasCfgStatusData.sPBPerPortData[u8PortNum].u8PBEnablePriority & CFG_PB_PORT_ENABLE)
                {
                    PB_InitializePortParam(u8PortNum); 
                    /* Deduct the Minimum Guaranteed Power from available Shared Capacity */
                    gasCfgStatusData.u16SharedPowerCapacity -= gasPBIntPortParam[u8PortNum].u16MinGuaranteedPwrIn250mW; 
                }
            }            
        }
    }
}

void PB_InitializePortParam(UINT8 u8PortNum)
{
    UINT16 u16MinPwr = 0, u16MaxPwr = 0;
    
    /* Initialize all the Port specific parameters */ 
    gasPBIntPortParam[u8PortNum].u8AttachSeqNo             = SET_TO_ZERO; 
    gasPBIntPortParam[u8PortNum].eRenegSubState            = ePB_IDLE_SS; 
    gasPBIntPortParam[u8PortNum].u16NegotiatedPwrIn250mW   = SET_TO_ZERO; 
    gasPBIntPortParam[u8PortNum].eGetSinkCapSS             = ePB_SINK_CAPS_NOT_INITIATED; 
    gasPBIntPortParam[u8PortNum].u8PortStatusMask          = FALSE; 
    gasPBIntPortParam[u8PortNum].ePBPortState              = ePB_IDLE_STATE;
    gasPBIntPortParam[u8PortNum].u16RequiredPrtPwrIn250mW  = SET_TO_ZERO; 
    
    /* Get the Guaranteed Minimum Power and Maximum Power based on 
       currently selected throttling bank */
    if (CFG_PD_THROTTLE_BANK_A == gasCfgStatusData.u8PwrThrottleCfg) 
    {
        u16MinPwr = gasCfgStatusData.u16MinPowerBankA;
        u16MaxPwr = gasCfgStatusData.sPBPerPortData[u8PortNum].u16MaxPrtPwrBankA; 
    }
    else if (CFG_PD_THROTTLE_BANK_B == gasCfgStatusData.u8PwrThrottleCfg)
    {
        u16MinPwr = gasCfgStatusData.u16MinPowerBankB;
        u16MaxPwr = gasCfgStatusData.sPBPerPortData[u8PortNum].u16MaxPrtPwrBankB; 
    }
    else if (CFG_PD_THROTTLE_BANK_C == gasCfgStatusData.u8PwrThrottleCfg)
    {
        u16MinPwr = gasCfgStatusData.u16MinPowerBankC;
        u16MaxPwr = gasCfgStatusData.sPBPerPortData[u8PortNum].u16MaxPrtPwrBankC; 
    }
    else
    {
        /* Power Throttling Shutdown mode */
    }
    
    gasPBIntPortParam[u8PortNum].u16MinGuaranteedPwrIn250mW = u16MinPwr;
    gasPBIntPortParam[u8PortNum].u16MaxPortPwrIn250mW       = u16MaxPwr; 
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
    UINT16 u16CurrentIn10mA = 0;
    UINT16 u16TempPwr;
    float fNegotiatedVoltage; 
    
    /*we need to calculate the power the sink is drawing and 
     give back the rest to the pool*/
    u16NegVoltIn50mV = PB_GET_VOLTAGE_FROM_FIXED_PDO(u32PDO); 
    
    /* Update the global status register */
    gasCfgStatusData.sPerPortData[u8PortNum].u16NegoVoltageIn50mV = u16NegVoltIn50mV; 
    
    /* Convert the voltage from 50mV units to V */
    fNegotiatedVoltage = PB_CONVERT_PDO_VOLTAGE_FROM_50mV_TO_V(u16NegVoltIn50mV); 
    
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
            gasPBIntPortParam[u8PortNum].u8PortStatusMask |= PB_PORT_STATUS_CAPABILITY_MISMATCH;
            u16CurrentIn10mA = PB_GET_OPERATING_CURRENT_FROM_RDO(u32RDO);
        }
        else
        {
            gasPBIntPortParam[u8PortNum].u8PortStatusMask &= ~(PB_PORT_STATUS_CAPABILITY_MISMATCH);
            u16CurrentIn10mA = PB_GET_MAX_CURRENT_FROM_RDO(u32RDO);
        }        
    }
    
    /* Update the global status register */
    gasCfgStatusData.sPerPortData[u8PortNum].u16NegoCurrentIn10mA = u16CurrentIn10mA; 
    
    /* Calculate power in terms of 10mW */
    u16TempPwr = (UINT16) ((UINT16)fNegotiatedVoltage * u16CurrentIn10mA);
    
    /* Convert the power in terms of 250 mW */
    gasPBIntPortParam[u8PortNum].u16NegotiatedPwrIn250mW = PB_CONVERT_POWER_FROM_10mW_TO_250mW(u16TempPwr); 
    
    if (gasPBIntPortParam[u8PortNum].u16NegotiatedPwrIn250mW <=     \
                    gasPBIntPortParam[u8PortNum].u16MinGuaranteedPwrIn250mW)
    {
       gasPBIntPortParam[u8PortNum].u16NegotiatedPwrIn250mW = gasPBIntPortParam[u8PortNum].u16MinGuaranteedPwrIn250mW;  
       gasPBIntPortParam[u8PortNum].u8PortStatusMask |= PB_PORT_STATUS_PORT_IN_MIN_PWR; 
    }
    else
    {
       gasPBIntPortParam[u8PortNum].u8PortStatusMask &= ~(PB_PORT_STATUS_PORT_IN_MIN_PWR);
    }
    
    gasCfgStatusData.sPerPortData[u8PortNum].u16AllocatedPowerIn250mW = gasPBIntPortParam[u8PortNum].u16NegotiatedPwrIn250mW; 
}

void PB_InitiateNegotiationWrapper(UINT8 u8PortNum, UINT16 u16NewWattage)
{    
    PB_UpdatePDO(u8PortNum, u16NewWattage); 
    
    /* To-do: Do we need to re initiate the request in case of failure */
    (void) DPM_HandleClientRequest(u8PortNum, eMCHP_PSF_DPM_RENEGOTIATE);  
            
    gasPBIntPortParam[u8PortNum].u16RequiredPrtPwrIn250mW = u16NewWattage;   
}

void PB_InitiateGetSinkCapsWrapper(UINT8 u8PortNum)
{
    /* To-do: Do we need to re initiate the request in case of failure */
    (void) DPM_HandleClientRequest (u8PortNum, eMCHP_PSF_DPM_GET_SNK_CAPS);
    
    PB_ChangePortStates (u8PortNum, ePB_RENEGOTIATION_IN_PROGRESS_STATE, ePB_GET_SINKCAPS_SENT_SS);
    
    gasPBIntPortParam[u8PortNum].eGetSinkCapSS = ePB_SINK_CAPS_INITIATED; 
}

void PB_CalculateRequiredPortPower(UINT8 u8PortNum, UINT8 u8SinkPDOCnt, const UINT32 *pu32SinkCap)
{
    UINT16 u16PortMaxRequiredPwr = 0;  
    UINT16 u16TempPwr = 0, u16TempCurr = 0, u16TempVolt = 0; 
    UINT8  u8PDOCnt;
    float fPDOVoltage = 0; 
    
    for (u8PDOCnt = 0; u8PDOCnt < u8SinkPDOCnt; u8PDOCnt++)
    {
        if ((ePDO_FIXED == (ePDOtypes)PB_GET_PDO_TYPE (pu32SinkCap[u8PDOCnt])) || \
                (ePDO_VARIABLE == (ePDOtypes)PB_GET_PDO_TYPE (pu32SinkCap[u8PDOCnt])))
        {
            if (ePDO_FIXED == (ePDOtypes)PB_GET_PDO_TYPE (pu32SinkCap[u8PDOCnt]))
            {      
                u16TempVolt = PB_GET_VOLTAGE_FROM_FIXED_PDO (pu32SinkCap[u8PDOCnt]);          
            }
            else
            {
                u16TempVolt = PB_GET_VARIABLE_PDO_MAX_VOLTAGE (pu32SinkCap[u8PDOCnt]);     
            }
            
            fPDOVoltage = PB_CONVERT_PDO_VOLTAGE_FROM_50mV_TO_V (u16TempVolt);
            u16TempCurr = PB_GET_CURRENT_FROM_PDO (pu32SinkCap[u8PDOCnt]);
            u16TempPwr =  (UINT16)(u16TempCurr * (UINT16) fPDOVoltage); 
                
            if (u16TempPwr > u16PortMaxRequiredPwr)
            {
                /* Now power will be in units of 10mW */
                u16PortMaxRequiredPwr = u16TempPwr;
            }
        }
    }    
    
    /* Convert the Power in terms of 250mW */
    u16PortMaxRequiredPwr = PB_CONVERT_POWER_FROM_10mW_TO_250mW(u16PortMaxRequiredPwr);
    
    /* Required Power should be greater than Minimum Guaranteed power and should 
       be within the limit of Maximum Port Power */
    if (u16PortMaxRequiredPwr > gasPBIntPortParam[u8PortNum].u16MinGuaranteedPwrIn250mW)
    {
        gasPBIntPortParam[u8PortNum].u16RequiredPrtPwrIn250mW = MIN (u16PortMaxRequiredPwr, gasPBIntPortParam[u8PortNum].u16MaxPortPwrIn250mW);        
    }
    else
    {
        gasPBIntPortParam[u8PortNum].u16RequiredPrtPwrIn250mW = gasPBIntPortParam[u8PortNum].u16MinGuaranteedPwrIn250mW;
    } 
}

UINT8 PB_NegotiateIfRequiredPwrAvailableInPool(UINT8 u8PortNum)
{
    UINT16 u16AvailablePwr; 
    UINT16 u16NewWattage = 0; 
    UINT8 u8RequiredPwrAvailable = FALSE; 
    
    /* Include the current negotiated power of the port along with pool power. */
    u16AvailablePwr = gasCfgStatusData.u16SharedPowerCapacity + gasPBIntPortParam[u8PortNum].u16NegotiatedPwrIn250mW;
    
    /* Check if required power is available in pool */
    if (u16AvailablePwr >= gasPBIntPortParam[u8PortNum].u16RequiredPrtPwrIn250mW)
    {
        /* Exclude Negotiated power while reducing the pool power since it would have 
           been already deducted */
        gasCfgStatusData.u16SharedPowerCapacity -= (gasPBIntPortParam[u8PortNum].u16RequiredPrtPwrIn250mW \
                                            - gasPBIntPortParam[u8PortNum].u16NegotiatedPwrIn250mW);                       
        
        /* Initiate Negotiation with new wattage value */
        u16NewWattage = gasPBIntPortParam[u8PortNum].u16RequiredPrtPwrIn250mW; 
        PB_InitiateNegotiationWrapper(u8PortNum, u16NewWattage); 
        
        PB_ChangePortStates(u8PortNum, ePB_RENEGOTIATION_IN_PROGRESS_STATE, ePB_FIRST_RENEGOTIATION_IN_PROGRESS_SS); 
        
        u8RequiredPwrAvailable = TRUE; 
    }
    
    return u8RequiredPwrAvailable;
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
            u8HigherPriorityPort = 0xFF;
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
        if ((PB_PORT_STATUS_CAPABILITY_MISMATCH == (gasPBIntPortParam[u8PortNum1].u8PortStatusMask & \
                                        PB_PORT_STATUS_CAPABILITY_MISMATCH)) && \
            (FALSE == (gasPBIntPortParam[u8PortNum2].u8PortStatusMask & PB_PORT_STATUS_CAPABILITY_MISMATCH)))
        {
            u8HigherPriorityPort = u8PortNum1;        
        }
        else if ((FALSE == (gasPBIntPortParam[u8PortNum1].u8PortStatusMask & PB_PORT_STATUS_CAPABILITY_MISMATCH)) && \
                 (PB_PORT_STATUS_CAPABILITY_MISMATCH == (gasPBIntPortParam[u8PortNum2].u8PortStatusMask & \
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
                u8HigherPriorityPort = 0xFF;
            }
        }
    }
    else
    {
        u8HigherPriorityPort = 0xFF;
    }
    
    return u8HigherPriorityPort;
}

UINT8 PB_IdentifyLowestPriorityPort(UINT8 u8PortNum)
{
    UINT8 u8LoopPortNum;
    UINT8 u8LowPriorityPort = u8PortNum;  
    
    for (u8LoopPortNum = 0; u8LoopPortNum < CONFIG_PD_PORT_COUNT ; u8LoopPortNum++)
    {    
        if (u8LoopPortNum != u8PortNum)
        {
            /*Continue to consider the port only if it is in attached state and also
            the renegotiation should have been complete and also the port 
            should have some power in the reserve to give back */
            
            if ((gasPBIntPortParam[u8LoopPortNum].u8PortStatusMask & PB_PORT_STATUS_ATTACH) && \
                (ePB_RENEGOTIATION_COMPLETED_STATE == gasPBIntPortParam[u8LoopPortNum].ePBPortState)&& \
                (FALSE == (gasPBIntPortParam[u8LoopPortNum].u8PortStatusMask & PB_PORT_STATUS_PORT_IN_MIN_PWR)))
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
    UINT16 u16RecoverPower = 0; 
    UINT16 u16RenegotiatePwr = 0;
    UINT16 u16AvailablePwr = 0;    
    UINT8 u8RetVal = PB_RECLAIM_FAILED;
    UINT8 u8LowPriorityPort;  
    
    u8LowPriorityPort = PB_IdentifyLowestPriorityPort(u8PortNum);
    
    if (u8LowPriorityPort != u8PortNum)   
    {
        /*Calculate how much power can be recovered from lower priority port*/      
        gsPBIntSysParam.u8RecoveringMode = TRUE;
        gsPBIntSysParam.u8RecoverPortNum = u8PortNum;
        gsPBIntSysParam.u8ReclaimPortNum = u8LowPriorityPort;
        
        u16AvailablePwr = gasCfgStatusData.u16SharedPowerCapacity + gasPBIntPortParam[u8PortNum].u16NegotiatedPwrIn250mW;
        
        /*This is the power that must be recovered from the lower priority port*/
        u16RecoverPower = gasPBIntPortParam[u8PortNum].u16RequiredPrtPwrIn250mW - u16AvailablePwr;

        u16RenegotiatePwr = MAX ((gasPBIntPortParam[gsPBIntSysParam.u8ReclaimPortNum].u16NegotiatedPwrIn250mW - \
                            u16RecoverPower), gasPBIntPortParam[gsPBIntSysParam.u8ReclaimPortNum].u16MinGuaranteedPwrIn250mW);
        
        /*Update the pool with the power we recovered*/
        gasCfgStatusData.u16SharedPowerCapacity += (gasPBIntPortParam[gsPBIntSysParam.u8ReclaimPortNum].u16NegotiatedPwrIn250mW \
                                                    - u16RenegotiatePwr);
        
        /* Renegotiate the lower priority port with new power */
        PB_InitiateNegotiationWrapper (gsPBIntSysParam.u8ReclaimPortNum, u16RenegotiatePwr);
        
        /* Change the state of reclaiming port to Renegotiation_in_progress */
        PB_ChangePortStates (gsPBIntSysParam.u8ReclaimPortNum, ePB_RENEGOTIATION_IN_PROGRESS_STATE, ePB_FIRST_RENEGOTIATION_IN_PROGRESS_SS);
        
        /*Change the state of recovering Port to IDLE*/
        PB_ChangePortStates (gsPBIntSysParam.u8RecoverPortNum, ePB_PWR_RECOVERING_STATE, ePB_IDLE_SS);
       
        u8RetVal = PB_RECLAIM_SUCCESS;  
    }
    else
    {
        /* Lower Priority Port Not Identified */
    }
    return u8RetVal;
    
}

void PB_SinkCapsReceivedHandler(UINT8 u8PortNum)
{
    UINT8 u8SinkPDOCnt; 
    UINT32 *pu32SinkCap; 
    UINT16 u16AvailablePwrIn250mW = 0; 
    UINT8 u8AvailablePwrSufficient; 
   
    /* Get the Sink capabilities from DPM */
    u8SinkPDOCnt = gasCfgStatusData.sPerPortData[u8PortNum].u8PartnerPDOCnt;  
    pu32SinkCap = (UINT32 *)&gasCfgStatusData.sPerPortData[u8PortNum].u32aPartnerPDO[INDEX_0]; 
    
    /* Calculate the power required for the port based on Sink caps values */
    PB_CalculateRequiredPortPower(u8PortNum, u8SinkPDOCnt, pu32SinkCap); 

    u8AvailablePwrSufficient = PB_NegotiateIfRequiredPwrAvailableInPool(u8PortNum); 
    
    if (!u8AvailablePwrSufficient)
    {
        if (PB_RECLAIM_FAILED == PB_ReclaimPower (u8PortNum))
        {
            /*There is no power in the lower priority port. Just advertise 
             whatever power is available in the pool */  
            u16AvailablePwrIn250mW = gasCfgStatusData.u16SharedPowerCapacity + gasPBIntPortParam[u8PortNum].u16NegotiatedPwrIn250mW;
            
            /*Initiate renegotiation for the port with u16AvailablePwr */
            PB_InitiateNegotiationWrapper (u8PortNum, u16AvailablePwrIn250mW);
            
            /* Change the states of the port */
            PB_ChangePortStates (u8PortNum, ePB_RENEGOTIATION_IN_PROGRESS_STATE, ePB_FIRST_RENEGOTIATION_IN_PROGRESS_SS);
            
            gasCfgStatusData.u16SharedPowerCapacity = 0;
            gsPBIntSysParam.u8RecoveringMode = FALSE;
        }        
    }
}

UINT8 PB_ReleaseExcessPwr(UINT8 u8PortNum)
{
    UINT16 u16ExcessPwr;
    UINT8 u8ExcessPwrReturned = FALSE;    
    
     /*The Allocated Power for the Port would not have been negotiated. 
     The sink could have negotiated for a lesser power. We need to giveback
     the excess power to the pool*/
    
    u16ExcessPwr = gasPBIntPortParam[u8PortNum].u16RequiredPrtPwrIn250mW - \
                            gasPBIntPortParam[u8PortNum].u16NegotiatedPwrIn250mW; 
    
    gasCfgStatusData.u16SharedPowerCapacity += u16ExcessPwr; 
    
    /*After giving back the excess power make the required power same as negotiated power*/
    
    gasPBIntPortParam[u8PortNum].u16RequiredPrtPwrIn250mW = gasPBIntPortParam[u8PortNum].u16NegotiatedPwrIn250mW; 
    
    if (u16ExcessPwr != 0x00)
    {
        u8ExcessPwrReturned = TRUE;
    }
    
    return u8ExcessPwrReturned; 
}

void PB_SetRenegotiationPendingForLowPriorityPorts(UINT8 u8PortNum)
{
    UINT8 u8LowPriorityPort;
    
    for (u8LowPriorityPort = 0; u8LowPriorityPort < CONFIG_PD_PORT_COUNT ; u8LowPriorityPort++)
    {
        if (u8LowPriorityPort != u8PortNum)
        {
            if ((gasPBIntPortParam[u8LowPriorityPort].u8PortStatusMask & PB_PORT_STATUS_ATTACH) && \
                (gasPBIntPortParam[u8LowPriorityPort].u8PortStatusMask & PB_PORT_STATUS_INITIAL_NEG_DONE))
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
                        gasPBIntPortParam[u8LowPriorityPort].u8PortStatusMask |= PB_PORT_STATUS_RENEG_AGAIN;
                    }
                }

            }
        }
        
    }
    
}

void PB_UpdateAttachSeq(UINT8 u8PortNum)
{
    UINT8 u8OtherPorts;
    
    for (u8OtherPorts = 0; u8OtherPorts < CONFIG_PD_PORT_COUNT ; u8OtherPorts++)
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
   
    for (u8OtherPorts = 0; u8OtherPorts < CONFIG_PD_PORT_COUNT ; u8OtherPorts++)
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
    UINT8 u8NextPort = 0xFF;
   
    for (u8LoopPortNum = 0; u8LoopPortNum < CONFIG_PD_PORT_COUNT; u8LoopPortNum++)
    {
        if (ePB_RENEGOTIATION_PENDING_STATE == gasPBIntPortParam[u8LoopPortNum].ePBPortState)
        {
            if (0xFF == u8NextPort)
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
                
    if (u8HighestPortInPendingstate != 0xFF)
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

UINT8 PB_PortInWaitForAsyncTimerState(void)
{
    UINT8 u8PortNum = 0xFF;
    UINT8 u8Loop;
    for (u8Loop = 0; u8Loop < CONFIG_PD_PORT_COUNT ; u8Loop++)
    {
        if (ePB_WAIT_FOR_ASYNC_REQ_SS == gasPBIntPortParam[u8Loop].eRenegSubState)
        {
            u8PortNum = u8Loop;
        }
    }
    
    return u8PortNum;
}

void PB_TimerEnd(UINT8 u8PortNum, UINT8 u8PBEvent)
{
    /* Handle Timer expired event. */
    (void)PB_HandleDPMEvents (u8PortNum, u8PBEvent); 
}

void PB_UpdatePDO(UINT8 u8PortNum, UINT16 u16PowerIn250mW)
{
    float fVoltageInmV = 0; 
    UINT16 u16CurrentIn10mA = 0; 
    
    /* Enable New PDO Select. Load the New PDO Count which is same as Fixed PDO 
       count */
    gasCfgStatusData.sPerPortData[u8PortNum].u8NewPDOSelect = TRUE; 
    gasCfgStatusData.sPerPortData[u8PortNum].u8NewPDOCnt = gasCfgStatusData.sPerPortData[u8PortNum].u8SourcePDOCnt; 
    
    /* In Power Balancing, voltages remain fixed irrespective of the power 
       value advertised. Only current varies with power. */
    for (UINT8 u8Index = INDEX_0; u8Index < gasCfgStatusData.sPerPortData[u8PortNum].u8NewPDOCnt; u8Index++)
    {
        /* Get the voltage value from PDO */
        fVoltageInmV = DPM_GET_VOLTAGE_FROM_PDO_MILLI_V(gasCfgStatusData.sPerPortData[u8PortNum].u32aSourcePDO[u8Index]); 
        
        /* Calculate new current value based on new power */
        u16CurrentIn10mA = (UINT16)(((float)u16PowerIn250mW / fVoltageInmV) * (PB_POWER_UINTS_MILLI_W / DPM_PDO_CURRENT_UNIT)); 
        
        /* In PB, current value of a port should not exceed PORT_MAX_I */ 
        u16CurrentIn10mA = MIN(u16CurrentIn10mA, gasCfgStatusData.sPBPerPortData[u8PortNum].u16MaxPrtCurrent); 
        
        /* Load the New PDO registers with the new PDO values */
        gasCfgStatusData.sPerPortData[u8PortNum].u32aNewPDO[u8Index] = \
                    (gasCfgStatusData.sPerPortData[u8PortNum].u32aSourcePDO[u8Index] & ~(PB_FIXED_PDO_CURRENT_MASK)) | u16CurrentIn10mA;  

    }

}
#endif
