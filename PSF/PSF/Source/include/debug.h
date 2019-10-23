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

#if CONFIG_HOOK_DEBUG_MSG
/**************************************************************************************/
    
    /*API to print INT32 (pi32Buffer) buffer of u32TotalCount length*/
    void Debug_PrintInt32Buffer (INT32 const *pi32Buffer, UINT32 u32TotalCount);
    /*API to print UINT32 (pu32Buffer) buffer of u32TotalCount length*/
	void Debug_PrintUint32Buffer (UINT32 const *pu32Buffer, UINT32 u32TotalCount);
    /*API to print UINT16 (pu16Buffer) buffer of u32TotalCount length*/
	void Debug_PrintUint16Buffer (UINT16 const *pu16Buffer, UINT32 u32TotalCount);
    /*API to print UINT8 (pu8Buffer) buffer of u32TotalCount length*/
	void Debug_PrintUint8Buffer (UINT8 const *pu8Buffer, UINT32 u32TotalCount);
/**************************************************************************************/ 
   
    /* Define to print a string */
    #define DEBUG_PRINT_PORT_STR(u8portNum,string)    \
                {MCHP_PSF_HOOK_DEBUG_INT32(u8portNum);\
                 MCHP_PSF_HOOK_DEBUG_STRING(string);}
    
    /* Define to print string and a UINT8 buffer*/
    #define DEBUG_PRINT_UINT8_BUF_STR(u8portNum,string1,u8Buf,u32Len,string2) \
                {MCHP_PSF_HOOK_DEBUG_INT32(u8portNum);\
                 MCHP_PSF_HOOK_DEBUG_STRING(string1);\
                 Debug_PrintUint8Buffer(u8Buf,u32Len);\
                 MCHP_PSF_HOOK_DEBUG_STRING(string2);}
     
    /* Define to print string and a UINT16 buffer*/
    #define DEBUG_PRINT_UINT16_BUF_STR(u8portNum,string1,u16Buf,u32Len,string2)  \
                {MCHP_PSF_HOOK_DEBUG_INT32(u8portNum);\
                MCHP_PSF_HOOK_DEBUG_STRING(string1);\
                Debug_PrintUint16Buffer(u16Buf,u32Len);\
                MCHP_PSF_HOOK_DEBUG_STRING(string2);}
    
    /* Define to print string and a UINT32 buffer*/
    #define DEBUG_PRINT_UINT32_BUF_STR(u8portNum,string1,u32Buf,u32Len,string2) \
                {MCHP_PSF_HOOK_DEBUG_INT32(u8portNum);\
                MCHP_PSF_HOOK_DEBUG_STRING(string1);\
                Debug_PrintUint32Buffer(u32Buf,u32Len);\
                MCHP_PSF_HOOK_DEBUG_STRING(string2);}
    
    /* Define to print string and INT32 buffer*/
    #define DEBUG_PRINT_INT32_BUF_STR(u8portNum,string1,i32Buf,u32Len,string2)  \
                {MCHP_PSF_HOOK_DEBUG_INT32(u8portNum);\
                MCHP_PSF_HOOK_DEBUG_STRING(string1);\
                Debug_PrintInt32Buffer(i32Buf,u32Len);\
                MCHP_PSF_HOOK_DEBUG_STRING(string2);}

/**************************************************************************************/ 
    
#else

    #define DEBUG_PRINT_PORT_STR(u8portNum,string) 

    #define DEBUG_PRINT_UINT8_BUF_STR(u8portNum,string1,u8Buf,u32Len,string2)  

    #define DEBUG_PRINT_UINT16_BUF_STR(u8portNum,string1,u16Buf,u32Len,string2) 

    #define DEBUG_PRINT_UINT32_BUF_STR(u8portNum,string1,u32Buf,u32Len,string2)

    #define HOOK_DEBUG_BUF_INT8(u8portNum,string1,i32Buf,u32Len,string2) 

#endif /*CONFIG_HOOK_DEBUG_MSG*/

#endif /*_DEBUG_H_*/

