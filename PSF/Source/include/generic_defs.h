/*******************************************************************************
  Generic Defines file
 
  Company:
    Microchip Technology Inc.

  File Name:
    generic_defs.h

  Description:
    This file contains generic defines used in the stack.
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

#ifndef GENERIC_DEFS_H
#define	GENERIC_DEFS_H

/* TRUE/FALSE defines */
#define FALSE                                     0
#define TRUE                                      1

/* Unsigned integers */
#define UINT8                                     unsigned char
#define UINT16                                    unsigned short
#define UINT32                                    unsigned long

/* Signed Integer */
#define INT8                                      signed char
#define INT16                                     short
#define INT32                                     long

/* Signed character */
#define CHAR                                      char

/* Unsigned character */
#define UCHAR                                     unsigned char

/* Size of operator */
#define SIZEOF(x)                                 sizeof(x)
/********************************************************************************************/
/* To obtain equivalent value from BIT position*/
#define BIT(x)                                   (1UL << x)

/* Data Size */ 
#define BYTE_LEN_1			        			1
#define BYTE_LEN_2			        			2
#define BYTE_LEN_3			        			3
#define BYTE_LEN_4			        			4
#define BYTE_LEN_10                 			10
#define BYTE_LEN_16			        			16

/*Index position*/
#define INDEX_0                                 0
#define INDEX_1                                 1
#define INDEX_2                                 2
#define INDEX_3                                 3
#define INDEX_4                                 4
#define INDEX_5                                 5

/********************************************************************************************/
/*Low Byte and High byte from a UINT16*/
#define LOBYTE(u16Var)							((UINT8)(u16Var))
#define HIBYTE(u16Var)							((UINT8)((u16Var) >> 8) & 0xFF)

/*Low word and High word from a UINT32*/
#define HIWORD(u32Var)							((UINT16)(((u32Var) >> 16) & 0xFFFF))
#define LOWORD(u32Var)							((UINT16)((u32Var) & 0xFFFF))

/********************************************************************************************/
/*Make word from Bytes*/
#define MAKE_UINT16(u8HiByte, u8LoByte)			\
        (((UINT16)u8LoByte) | (((UINT16)u8HiByte) << 8))

/*Make UINT32 from word*/
#define MAKE_UINT32(u16HiWord, u16LoWord)		\
            (((UINT32)u16LoWord) | (((UINT32)u16HiWord) << 16))

/*Make UINT32 from Bytes*/
#define MAKE_UINT32_FROM_BYTES(u8LoWordLoByte, u8LoWordHiByte, u8HiWordLoByte, u8HiWordHiByte)\
                                            (((UINT32)u8LoWordLoByte)				|\
                                            (((UINT32)u8LoWordHiByte) << 8)			|\
                                            (((UINT32)u8HiWordLoByte) << 16) 		|\
                                            (((UINT32)u8HiWordHiByte) << 24u))

#define MIN(a,b) (((a) < (b)) ? (a) : (b))	/* Returns the minimum of two */
#define MAX(a,b) ((a) > (b) ? (a) : (b))	/* Returns the maximum of two */

/* Extract first decimal value in a floating point number */
#define NO_OF_DECIMAL_POINTS_PRECISION          10 
#define EXTRACT_FIRST_DECIMAL_FROM_FLOAT(fNum)  ((UINT16)(fNum * NO_OF_DECIMAL_POINTS_PRECISION) \
                                                    % NO_OF_DECIMAL_POINTS_PRECISION)
/* Round off float value to nearest integer */
#define ROUND_OFF_FLOAT_TO_INT(fNum)            (EXTRACT_FIRST_DECIMAL_FROM_FLOAT(fNum) >= 5) \
                                                    ? ((UINT16)fNum + 1) : ((UINT16)fNum)          
/******************************CLEAR & RESET VALUE *************************************/

#define CLR_VAL                                         0x00
#define RESET_TO_ZERO									0
#define SET_TO_ZERO									    0  
#define SET_TO_ONE                                      1
/********************************************************************************************/
/* Defines for Port reference*/
#define PORT0       0
#define PORT1       1
#define PORT2       2
#define PORT3       3

/**********************************************************************************************/

#endif /*GENERIC_DEFS_H*/
