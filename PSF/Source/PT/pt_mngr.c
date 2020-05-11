/*******************************************************************************
 Power Throttling Source File

 Company:
   Microchip Technology Inc.

 File Name:
   pt_mngr.c

 Description:
   This file contains the function for Power Throttling State machine. 
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

#if (TRUE == INCLUDE_POWER_THROTTLING)

void PT_Init(UINT8 u8PortNum)
{
    gu8PrevPTBank[u8PortNum] = PD_THROTTLE_BANK_A; 
}

void PT_RunStateMachine(UINT8 u8PortNum)
{
    UINT8 u8CurrPTBank = gasCfgStatusData.u8PwrThrottleCfg; 
    UINT8 u8PrevPTBank = gu8PrevPTBank[u8PortNum]; 
    UINT8 u8InitiateRenegotiation = FALSE; 
    
    if (u8CurrPTBank != u8PrevPTBank)
    {
        gu8PrevPTBank[u8PortNum] = u8CurrPTBank; 
        
        switch(u8CurrPTBank)
        {
            case PD_THROTTLE_BANK_A: 
            {
                /* To-do: Set Temperature status as Normal in Real Time Flags 
                   of PPS_Status message */
                /* To-do: Set Temperature status as Normal in Status message */
                /* To-do: Clear Bit 5 in Power Status Field of Status message */
                /* To-do: Trigger an alert message with type of alert as 
                 Operating condition change */
                
                if (gasPolicy_Engine[u8PortNum].u8PEPortSts & PE_EXPLICIT_CONTRACT)
                {
                    u8InitiateRenegotiation = TRUE; 
                }
                
                break; 
            }    
            
            case PD_THROTTLE_BANK_B:
            case PD_THROTTLE_BANK_C:
            {
                if (gasPolicy_Engine[u8PortNum].u8PEPortSts & PE_EXPLICIT_CONTRACT)
                {
                    u8InitiateRenegotiation = TRUE; 
                }

                break; 
            }
            
            case PD_THROTTLE_SHUTDOWN_MODE:
            {
                /* To-do: To be implemented once requirement is clearly 
                   defined by Systems team */
                break; 
            }
            
            default:
            {
                break; 
            }
        }
        
        if (TRUE == u8InitiateRenegotiation)
        {
#if (TRUE == INCLUDE_POWER_BALANCING)
            if (TRUE == DPM_IS_PB_ENABLED(u8PortNum))
            {
                /* Send a notification to PB to trigger renegotiation based on 
                   updated bank value. */                    
            }
            else
            {
                DPM_SET_RENEGOTIATE_REQ(u8PortNum);
            }
#else 
            /* Raise a DPM client request to trigger renegotiation */
            DPM_SET_RENEGOTIATE_REQ(u8PortNum); 
#endif 
        }
    }
}


#endif 