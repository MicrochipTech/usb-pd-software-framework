/* ************************************************************************** */
/** PSF Control Terminal header file

  @Company
    Company Name

  @File Name
    psf_control_terminal.h

  @Description
    This file contains function prototypes for PSF Control Terminal
 */
/* ************************************************************************** */
// DOM-IGNORE-BEGIN
/*******************************************************************************
* Copyright (C) 2019-2020 Microchip Technology Inc. and its subsidiaries.
*
* Subject to your compliance with these terms, you may use Microchip software
* and any derivatives exclusively with Microchip products. It is your
* responsibility to comply with third party license terms applicable to your
* use of third party software (including open source software) that may
* accompany Microchip software.
*
* THIS SOFTWARE IS SUPPLIED BY MICROCHIP "AS IS". NO WARRANTIES, WHETHER
* EXPRESS, IMPLIED OR STATUTORY, APPLY TO THIS SOFTWARE, INCLUDING ANY IMPLIED
* WARRANTIES OF NON-INFRINGEMENT, MERCHANTABILITY, AND FITNESS FOR A
* PARTICULAR PURPOSE.
*
* IN NO EVENT WILL MICROCHIP BE LIABLE FOR ANY INDIRECT, SPECIAL, PUNITIVE,
* INCIDENTAL OR CONSEQUENTIAL LOSS, DAMAGE, COST OR EXPENSE OF ANY KIND
* WHATSOEVER RELATED TO THE SOFTWARE, HOWEVER CAUSED, EVEN IF MICROCHIP HAS
* BEEN ADVISED OF THE POSSIBILITY OR THE DAMAGES ARE FORESEEABLE. TO THE
* FULLEST EXTENT ALLOWED BY LAW, MICROCHIP'S TOTAL LIABILITY ON ALL CLAIMS IN
* ANY WAY RELATED TO THIS SOFTWARE WILL NOT EXCEED THE AMOUNT OF FEES, IF ANY,
* THAT YOU HAVE PAID DIRECTLY TO MICROCHIP FOR THIS SOFTWARE.
*******************************************************************************/
// DOM-IGNORE-END

#ifndef PSF_CONTROL_TERMINAL_H
#define	PSF_CONTROL_TERMINAL_H

// DOM-IGNORE-BEGIN
#ifdef __cplusplus // Provide C++ Compatibility

	extern "C" {

#endif
#define PCT_PDO_CURRENT_UNIT            0x0A
#define PCT_GET_CURRENT_FROM_PDO_MILLI_A(u32PDO)   \
    ((u32PDO & DPM_PDO_CURRENT_MASK) * PCT_PDO_CURRENT_UNIT)
typedef enum
{
    ePCT_UNLOCK, //PCT waits for the command from the user and PCT runs until it gets unlocked by space bar
    ePCT_LOCK
            
}PCT_LOCK_STATE;

void MchpPSF_PCTInit(void);
void PSF_monitorandwait(void);
void MchpPSF_PCTRUN(PCT_LOCK_STATE);
UINT8* HextoAscii(UINT32 u32HexVal,UINT8 Length);
UINT8 *HextoAscii_Int (UINT32 u32HexVal, UINT8 u8Length);
void PCTWrite(const UINT8 *u8aPrintString,const UINT8 *u8aValue,UINT8 u8Size,UINT8 u8StrSize);
void PCTPrintPDO();
typedef enum
{
    eINIT,
    eREAD_DATA_WAIT,
    eREAD_DATA,
    eREAD_CONTINUE,
    eREAD_PROCESS,
    eHALT,   
    eTOGGLE_BLOCK
            
}STATE;

#ifdef	__cplusplus

    }
    
#endif

#endif	/* PSF_CONTROL_TERMINAL_H */

