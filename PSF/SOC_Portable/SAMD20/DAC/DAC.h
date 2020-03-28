/*******************************************************************************
  DAC Control File
 
  Company:
    Microchip Technology Inc.

  File Name:
    DAC.h

  Description:
    This header file contains the function prototypes of DAC interface for
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

#ifndef _DAC_
#define _DAC_


#include <generic_defs.h>
#include "../../config/default/peripheral/dac/plib_dac.h"
// *****************************************************************************
/* ************************************************************************** */
/* ************************************************************************** */
/* Section: File Scope or Global Data, constants                              */
/* ************************************************************************** */
/* ************************************************************************** */

#define SAMD20DAC_DIR_HIGH_AMP_MAX_VOLT 0
#define SAMD20DAC_DIR_HIGH_AMP_MIN_VOLT 1

/* ************************************************************************** */
/* ************************************************************************** */
/* Section: Data Structure                                                    */
/* ************************************************************************** */
/* ************************************************************************** */


// *****************************************************************************
// *****************************************************************************
// Section: Interface Functions
// *****************************************************************************
// *****************************************************************************
/****************************************************************************/

UINT8 SAMD20_DACInitialisation(void);

UINT8 SAMD20_DACConvert (UINT16 u16NegotiatedCur, UINT16 u16MaxOutVolt, \
        UINT16 u16MinOutVolt, UINT16 u16MaxCurIndication, UINT8 u8Direction);

#endif /*_DAC_*/