/*******************************************************************************
 Power Throttling Source File

 Company:
   Microchip Technology Inc.

 File Name:
   pt_mngr.c

 Description:
   This file contains the functions for Power Throttling functionality. 
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

#if (TRUE == INCLUDE_POWER_THROTTLING)

void PT_Init(UINT8 u8PortNum)
{
    if (TRUE == DPM_IS_PT_ENABLED)
    {
        gau8PTPrevBank[u8PortNum] = DPM_PD_THROTTLE_BANK_A; 
    }
}

void PT_HandleBankSwitch(UINT8 u8PortNum)
{
    UINT8 u8CurrPTBank = DPM_GET_CURRENT_PT_BANK; 
    UINT8 u8PrevPTBank = gau8PTPrevBank[u8PortNum];
    
    if (FALSE == DPM_IS_PT_ENABLED)
    {
        return; 
    }
    
    if (u8CurrPTBank != u8PrevPTBank)
    {
        gau8PTPrevBank[u8PortNum] = u8CurrPTBank; 
        
        switch(u8CurrPTBank)
        {
            case DPM_PD_THROTTLE_BANK_A:              
            case DPM_PD_THROTTLE_BANK_B:
            case DPM_PD_THROTTLE_BANK_C:
            {
                /* Trigger Alert message on bank change with Type of Alert as 
                   Operating Condition Change*/
                #if (TRUE == INCLUDE_PD_SOURCE_PPS)
                gasDPM[u8PortNum].u8AlertType  |= DPM_ALERT_TYPE_OPR_COND_CHANGE;
                DPM_RegisterInternalEvent(u8PortNum, DPM_INT_EVT_INITIATE_ALERT); 
                #endif 

                if (DPM_PD_THROTTLE_SHUTDOWN_MODE == u8PrevPTBank)
                {
                    /* Enable the Port which would have been previously disabled */
                    DPM_EnablePort(u8PortNum, TRUE); 

                    /* No need to raise renegotiation request since an 
                       attach interrupt is expected. */
                }              
                else
                {
                    if (TRUE == DPM_IS_PB_ENABLED(u8PortNum))
                    {
                        /* Inform PB to handle the PT Bank Switch event */
                        #if (TRUE == INCLUDE_POWER_BALANCING)
                        PB_OnPTBankSwitch (u8PortNum); 
                        #endif 
                    }
                    else 
                    {
                        if (gasPolicyEngine[u8PortNum].u8PEPortSts & PE_EXPLICIT_CONTRACT)
                        {
                            /* Raise to DPM for renegotiation */
                            DPM_RegisterInternalEvent(u8PortNum, DPM_INT_EVT_INITIATE_RENEGOTIATION);          
                        }
                        else
                        {
                            /* Do nothing if the port is in unattached state. DPM 
                            would advertise Source caps with the current PT bank. */
                        }
                    }
                }               
                break; 
            }
            
            case DPM_PD_THROTTLE_SHUTDOWN_MODE:
            {
                /* Disable the port so that no further Type-C attach can occur */
                DPM_EnablePort(u8PortNum, FALSE); 
                break; 
            }
            
            default:
            {
                break; 
            }
        } 
        
    }
}

void PT_CalculateSrcPDOs(UINT8 u8PortNum)
{
    /* Get current PT Bank */
    UINT8 u8CurrPTBank = DPM_GET_CURRENT_PT_BANK;
    UINT16 u16PowerIn250mW = SET_TO_ZERO; 
    
    if (DPM_PD_THROTTLE_BANK_A == u8CurrPTBank)
    {
        u16PowerIn250mW = gasCfgStatusData.sPBPerPortData[u8PortNum].u16MaxPrtPwrBankAIn250mW;
    }
    else if (DPM_PD_THROTTLE_BANK_B == u8CurrPTBank)
    { 
        u16PowerIn250mW = gasCfgStatusData.sPBPerPortData[u8PortNum].u16MaxPrtPwrBankBIn250mW;
    }
    else if (DPM_PD_THROTTLE_BANK_C == u8CurrPTBank)
    {
        u16PowerIn250mW = gasCfgStatusData.sPBPerPortData[u8PortNum].u16MaxPrtPwrBankCIn250mW;
    }
    else
    {
        /* Do Nothing for Shutdown mode */
    }
    
    /* Enable New PDO select so that New Source capabilities would be 
       sent from u32aNewPDO[7] array */
    DPM_ENABLE_NEW_PDO(u8PortNum);
    
    DPM_UpdateNewPDOFrmSrcPwr(u8PortNum, u16PowerIn250mW);
}

#endif 
