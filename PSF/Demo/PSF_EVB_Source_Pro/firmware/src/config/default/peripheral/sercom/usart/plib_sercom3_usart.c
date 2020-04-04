/*******************************************************************************
  SERCOM Universal Synchronous/Asynchrnous Receiver/Transmitter PLIB

  Company
    Microchip Technology Inc.

  File Name
    plib_sercom3_usart.c

  Summary
    USART peripheral library interface.

  Description
    This file defines the interface to the USART peripheral library. This
    library provides access to and control of the associated peripheral
    instance.

  Remarks:
    None.
*******************************************************************************/

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

// *****************************************************************************
// *****************************************************************************
// Section: Included Files
// *****************************************************************************
// *****************************************************************************
#include "plib_sercom3_usart.h"
#include <string.h>

// *****************************************************************************
// *****************************************************************************
// Section: Global Data
// *****************************************************************************
// *****************************************************************************


/*SERCOM3 USART baud rate*/
#define SERCOM3_USART_BAUDRATE (3000000U)

// *****************************************************************************
// *****************************************************************************
// Section: SERCOM3 USART Interface Routines
// *****************************************************************************
// *****************************************************************************

void static SERCOM3_USART_ErrorClear( void )
{
    UINT8  byDummyData = 0;

    /* Clear all errors */
    SERCOM3_REGS->USART_INT.SERCOM_STATUS = SERCOM_USART_INT_STATUS_PERR_Msk | SERCOM_USART_INT_STATUS_FERR_Msk | SERCOM_USART_INT_STATUS_BUFOVF_Msk;

    /* Flush existing error bytes from the RX FIFO */
    while((SERCOM3_REGS->USART_INT.SERCOM_INTFLAG & SERCOM_USART_INT_INTFLAG_RXC_Msk) == SERCOM_USART_INT_INTFLAG_RXC_Msk)
    {
        byDummyData = SERCOM3_REGS->USART_INT.SERCOM_DATA;
    }

    /* Ignore the warning */
    (void)byDummyData;
}

void SERCOM3_USART_Initialise( void )
{
    /*
     * Configures USART Clock Mode
     * Configures TXPO and RXPO
     * Configures Data Order
     * Configures Standby Mode
     * Configures Sampling rate
     * Configures IBON
     */
    UINT16 wBaudValue =0;
    
    SERCOM3_REGS->USART_INT.SERCOM_CTRLA = SERCOM_USART_INT_CTRLA_MODE_USART_INT_CLK | SERCOM_USART_INT_CTRLA_RXPO_PAD0 | SERCOM_USART_INT_CTRLA_TXPO_PAD2 | SERCOM_USART_INT_CTRLA_DORD_Msk | SERCOM_USART_INT_CTRLA_IBON_Msk | SERCOM_USART_INT_CTRLA_FORM(0x0) ;

    /* Configure Baud Rate */
    wBaudValue = 65536 - (((uint64_t)65536 * 16 * SERCOM3_USART_BAUDRATE) / SERCOM3_USART_FrequencyGet());
    
    SERCOM3_REGS->USART_INT.SERCOM_BAUD = SERCOM_USART_INT_BAUD_BAUD(wBaudValue);

    /*
     * Configures RXEN
     * Configures TXEN
     * Configures CHSIZE
     * Configures Parity
     * Configures Stop bits
     */
    SERCOM3_REGS->USART_INT.SERCOM_CTRLB = SERCOM_USART_INT_CTRLB_CHSIZE_8_BIT | SERCOM_USART_INT_CTRLB_SBMODE_1_BIT | SERCOM_USART_INT_CTRLB_RXEN_Msk | SERCOM_USART_INT_CTRLB_TXEN_Msk;

    /* Wait for sync */
    while((SERCOM3_REGS->USART_INT.SERCOM_STATUS & SERCOM_USART_INT_STATUS_SYNCBUSY_Msk) & SERCOM_USART_INT_STATUS_SYNCBUSY_Msk);

    /* Enable the UART after the configurations */
    SERCOM3_REGS->USART_INT.SERCOM_CTRLA |= SERCOM_USART_INT_CTRLA_ENABLE_Msk;

    /* Wait for sync */
    while((SERCOM3_REGS->USART_INT.SERCOM_STATUS & SERCOM_USART_INT_STATUS_SYNCBUSY_Msk) & SERCOM_USART_INT_STATUS_SYNCBUSY_Msk);
}

uint32_t SERCOM3_USART_FrequencyGet( void )
{
    return (uint32_t) (48000000UL);
}

bool SERCOM3_USART_SerialSetup( USART_SERIAL_SETUP * psSerialSetup, uint32_t dwClkFrequency )
{
    bool bSetupStatus       = false;
    UINT32 dwBaudValue     = 0;

    if((psSerialSetup != NULL) & (psSerialSetup->dwBaudRate != 0))
    {
        if(dwClkFrequency == 0)
        {
            dwClkFrequency = SERCOM3_USART_FrequencyGet();
        }

        if(dwClkFrequency >= (16 * psSerialSetup->dwBaudRate))
        {
            dwBaudValue = 65536 - ((uint64_t)65536 * 16 * psSerialSetup->dwBaudRate) / dwClkFrequency;
        }

        if(dwBaudValue != 0)
        {
            /* Disable the USART before configurations */
            SERCOM3_REGS->USART_INT.SERCOM_CTRLA &= ~SERCOM_USART_INT_CTRLA_ENABLE_Msk;

            /* Wait for sync */
            while((SERCOM3_REGS->USART_INT.SERCOM_STATUS & SERCOM_USART_INT_STATUS_SYNCBUSY_Msk) & SERCOM_USART_INT_STATUS_SYNCBUSY_Msk);

            /* Configure Baud Rate */
            SERCOM3_REGS->USART_INT.SERCOM_BAUD = SERCOM_USART_INT_BAUD_BAUD(dwBaudValue);

            /* Configure Parity Options */
            if(psSerialSetup->parity == eUSART_PARITY_NONE)
            {
                SERCOM3_REGS->USART_INT.SERCOM_CTRLA |= SERCOM_USART_INT_CTRLA_FORM(0x0) ;

                SERCOM3_REGS->USART_INT.SERCOM_CTRLB |= psSerialSetup->dataWidth | psSerialSetup->stopBits;
            }
            else
            {
                SERCOM3_REGS->USART_INT.SERCOM_CTRLA |= SERCOM_USART_INT_CTRLA_FORM(0x1) ;

                SERCOM3_REGS->USART_INT.SERCOM_CTRLB |= psSerialSetup->dataWidth | psSerialSetup->parity | psSerialSetup->stopBits;
            }

            /* Wait for sync */
            while((SERCOM3_REGS->USART_INT.SERCOM_STATUS & SERCOM_USART_INT_STATUS_SYNCBUSY_Msk) & SERCOM_USART_INT_STATUS_SYNCBUSY_Msk);

            /* Enable the USART after the configurations */
            SERCOM3_REGS->USART_INT.SERCOM_CTRLA |= SERCOM_USART_INT_CTRLA_ENABLE_Msk;

            /* Wait for sync */
            while((SERCOM3_REGS->USART_INT.SERCOM_STATUS & SERCOM_USART_INT_STATUS_SYNCBUSY_Msk) & SERCOM_USART_INT_STATUS_SYNCBUSY_Msk);

            bSetupStatus = true;
        }
    }

    return bSetupStatus;
}

USART_ERROR SERCOM3_USART_ErrorGet( void )
{
    USART_ERROR errorStatus = eUSART_ERROR_NONE;

    errorStatus = SERCOM3_REGS->USART_INT.SERCOM_STATUS & (SERCOM_USART_INT_STATUS_PERR_Msk | SERCOM_USART_INT_STATUS_FERR_Msk | SERCOM_USART_INT_STATUS_BUFOVF_Msk);

    if(errorStatus != eUSART_ERROR_NONE)
    {
        SERCOM3_USART_ErrorClear();
    }

    return errorStatus;
}

bool SERCOM3_USART_Write( void *pWriteData, const size_t dwSize )
{
    bool bWriteStatus      = false;
    UINT8 *pbyData      = (UINT8*)pWriteData;
    UINT32 dwLength    = dwSize;

    if(pbyData != NULL)
    {
        /* Blocks while buffer is being transferred */
        while(dwLength--)
        {
            /* Check if USART is ready for new data */
            while((SERCOM3_REGS->USART_INT.SERCOM_INTFLAG & SERCOM_USART_INT_INTFLAG_DRE_Msk) != SERCOM_USART_INT_INTFLAG_DRE_Msk);

            /* Write data to USART module */
            SERCOM3_REGS->USART_INT.SERCOM_DATA = *pbyData++;
        }

        bWriteStatus = true;
    }

    return bWriteStatus;
}

bool SERCOM3_USART_TransmitterIsReady( void )
{
    bool bTransmitterStatus = false;

    if((SERCOM3_REGS->USART_INT.SERCOM_INTFLAG & SERCOM_USART_INT_INTFLAG_DRE_Msk) == SERCOM_USART_INT_INTFLAG_DRE_Msk)
    {
        bTransmitterStatus = true;
    }

    return bTransmitterStatus;
}

bool SERCOM3_USART_TransmitComplete( void )
{
    bool bTransmitComplete = false;

    if((SERCOM3_REGS->USART_INT.SERCOM_INTFLAG & SERCOM_USART_INT_INTFLAG_TXC_Msk) == SERCOM_USART_INT_INTFLAG_TXC_Msk)
    {
        bTransmitComplete = true;
    }

    return bTransmitComplete;
}

void SERCOM3_USART_WriteByte( UINT16 wWriteData )
{
    /* Check if USART is ready for new data */
    while((SERCOM3_REGS->USART_INT.SERCOM_INTFLAG & SERCOM_USART_INT_INTFLAG_DRE_Msk) != SERCOM_USART_INT_INTFLAG_DRE_Msk);

    SERCOM3_REGS->USART_INT.SERCOM_DATA = wWriteData;
}

void SERCOM3_USART_Write_Int(UINT32 dwWriteInt, UINT8 byWidth)
{
    SERCOM3_USART_Write((void*)&dwWriteInt, byWidth); 
    while(!SERCOM3_USART_TransmitComplete()) 
    { 
    } 
}

void SERCOM3_USART_Write_String(char * pbyMessage)
{
    SERCOM3_USART_Write((void*)pbyMessage,strlen(pbyMessage)); 
    while(!SERCOM3_USART_TransmitComplete()) 
    { 
    }
}

void SERCOM3_USART_Write_Char(char byWriteChar)
{
    SERCOM3_USART_Write((void*)&byWriteChar,1); 
    while(!SERCOM3_USART_TransmitComplete()) 
    { 
    }
}
