/*******************************************************************************
  Debug Header File

  Company:
    Microchip Technology Inc.

  File Name:
    debug.h
 
  Description:
    This file contains functions to print debug messages from PSF stack
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

#ifndef _DEBUG_H_
#define _DEBUG_H_

#include <PSF_APIHook.h>

#define ENABLE_DEBUG_MSG    1
#define DISABLE_DEBUG_MSG   0

#if (TRUE == CONFIG_HOOK_DEBUG_MSG)
/**************************************************************************************/

    /* Define to print a string */
    #define DEBUG_PRINT_PORT_STR(psfDebugMsgLayer,byPortNum,string)     \
        do                                                              \
        {                                                               \
            if (ENABLE_DEBUG_MSG == psfDebugMsgLayer)                   \
            {                                                           \
                MCHP_PSF_HOOK_PRINT_TRACE(string);                      \
            }                                                           \
        } while(0)

    /* Define to print string and a UINT32 buffer*/
    #define DEBUG_PRINT_PORT_UINT32_STR(psfDebugMsgLayer,byPortNum,string1,dwData,byLen,string2)    \
        do                                                                                          \
        {                                                                                           \
            if (ENABLE_DEBUG_MSG == psfDebugMsgLayer)                                               \
            {                                                                                       \
                MCHP_PSF_HOOK_PRINT_INTEGER(byPortNum,1);                                           \
                MCHP_PSF_HOOK_PRINT_TRACE(string1);                                                 \
                MCHP_PSF_HOOK_PRINT_INTEGER(dwData,byLen);                                          \
                MCHP_PSF_HOOK_PRINT_TRACE(string2);                                                 \
            }                                                                                       \
        } while(0)
/**************************************************************************************/ 
    
#else
    #define DEBUG_PRINT_PORT_STR(psfDebugMsgLayer,byPportNum,string) 
    #define DEBUG_PRINT_PORT_UINT32_STR(psfDebugMsgLayer,byPportNum,string1,u32Buf,u32Len,string2)
#endif /*CONFIG_HOOK_DEBUG_MSG*/

#endif /*_DEBUG_H_*/

