/*******************************************************************************
  PSF debug source file

   Company:
    Microchip Technology Inc.

  File Name:
    debug.c
 
  Description:
    This file contains functions to print debug messages from PSF stack. 
 *******************************************************************************/
/*******************************************************************************
Copyright ©  [2019] Microchip Technology Inc. and its subsidiaries.

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

#if CONFIG_HOOK_DEBUG_MSG

void Debug_PrintInt32Buffer (INT32 const *pi32Buffer, UINT32 u32TotalCount)
{
    for (UINT32 u32Count = 0; u32Count < u32TotalCount; u32Count++)
    {
        MCHP_PSF_HOOK_DEBUG_INT32 (pi32Buffer[u32Count]);        
    }
}
/*********************************************************************************/
void Debug_PrintUint32Buffer (UINT32 const *pu32Buffer, UINT32 u32TotalCount)
{
    for (UINT32 u32Count = 0; u32Count < u32TotalCount; u32Count++)
    {
        MCHP_PSF_HOOK_DEBUG_UINT32 (pu32Buffer[u32Count]);
    }
}
/***********************************************************************************/
void Debug_PrintUint16Buffer (UINT16 const *pu16Buffer, UINT32 u32TotalCount)
{
    for (UINT32 u32Count = 0; u32Count < u32TotalCount; u32Count++)
    {
        MCHP_PSF_HOOK_DEBUG_UINT16 (pu16Buffer[u32Count]);
    }
}
/***********************************************************************************/
void Debug_PrintUint8Buffer (UINT8 const *pu8Buffer, UINT32 u32TotalCount)
{
    for (UINT32 u32Count = 0; u32Count < u32TotalCount; u32Count++)
    {
        MCHP_PSF_HOOK_DEBUG_UINT8 (pu8Buffer[u32Count]);
    }
}
/***********************************************************************************/
#endif
