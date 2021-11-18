/*******************************************************************************
  Digital-to-Analog Converter (DAC) PLIB

  Company:
    Microchip Technology Inc.

  File Name:
    plib_dac.c

  Summary:
    DAC PLIB Implementation file

  Description:
    This file defines the interface to the DAC peripheral library. This
    library provides access to and control of the associated peripheral
    instance.

*******************************************************************************/
// DOM-IGNORE-BEGIN
/*******************************************************************************
* Copyright (C) 2018 Microchip Technology Inc. and its subsidiaries.
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

// *****************************************************************************
// *****************************************************************************
// Section: Included Files
// *****************************************************************************
// *****************************************************************************
/* This section lists the other files that are included in this file.
*/
#include "device.h"
#include "interrupts.h"
#include "plib_dac.h"


/* (DAC DATA) Mask DATA[15:10] Bit */
#define DAC_DATA_MSB_MASK                    (0x03FFU)


void DAC_Initialize(void)
{
    /* Set Reference Voltage */
    DAC_REGS->DAC_CTRLB = (uint8_t)(DAC_CTRLB_REFSEL(1UL) | DAC_CTRLB_EOEN_Msk );

    DAC_REGS->DAC_EVCTRL = 0U;
    
    /* Enable DAC */
    DAC_REGS->DAC_CTRLA =(uint8_t)(DAC_CTRLA_ENABLE_Msk);	
    while(DAC_REGS->DAC_STATUS != 0U)
    {
        /* Wait for Synchronization after Enabling DAC */
    }
    
}

void DAC_DataWrite(uint16_t data)
{
    /* Write Data to DATA Register for conversion(DATA[9:0]) */
    DAC_REGS->DAC_DATA = (uint16_t)(DAC_DATA_MSB_MASK & DAC_DATA_DATA((uint32_t)data));
    while(DAC_REGS->DAC_STATUS != 0U)
    {
        /* Wait for Synchronization after writing Data to DATA Register */
    }
}
