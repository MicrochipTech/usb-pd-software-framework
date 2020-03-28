/*******************************************************************************
  DAC File

  Company:
    Microchip Technology Inc.

  File Name:
    DAC.c

  Description:
    This file contains the function definitions of DAC interface for
    sink port
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

#include "DAC.h"
#include <stdio.h>
#include "../../../Source/include/psf_stdinc.h"
/***********************************************************************************/
UINT8 SAMD20_DACInitialisation(void)
{
    DAC_Initialize();
    return TRUE;
}

UINT8 SAMD20_DACConvert (UINT16 u16NegotiatedCur, UINT16 u16MaxOutVolt, \
        UINT16 u16MinOutVolt, UINT16 u16MaxCurIndication, UINT8 u8Direction)
{
    if(SAMD20DAC_DIR_HIGH_AMP_MAX_VOLT == u8Direction)
    {
        UINT16 u16DacData = (u16NegotiatedCur * (u16MaxOutVolt - u16MinOutVolt)) / u16MaxCurIndication;
        {
            char sBuf[200] = {0};
            sprintf(sBuf, "u16DacData = %d, u16NegotiatedCur = %d\r\n\n",u16DacData,u16NegotiatedCur );
            DEBUG_PRINT_PORT_STR (0,sBuf);

        }
        DAC_DataWrite(u16DacData);
    }
    return TRUE;
}