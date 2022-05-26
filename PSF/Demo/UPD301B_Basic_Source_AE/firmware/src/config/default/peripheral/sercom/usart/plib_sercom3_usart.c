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
* Copyright (C) [2022] Microchip Technology Inc. and its subsidiaries.
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

#include "interrupts.h"
#include "plib_sercom3_usart.h"

// *****************************************************************************
// *****************************************************************************
// Section: Global Data
// *****************************************************************************
// *****************************************************************************


/* SERCOM3 USART baud value for 115200 Hz baud rate */
#define SERCOM3_USART_INT_BAUD_VALUE            (63019UL)

static SERCOM_USART_OBJECT sercom3USARTObj;

// *****************************************************************************
// *****************************************************************************
// Section: SERCOM3 USART Interface Routines
// *****************************************************************************
// *****************************************************************************

void static SERCOM3_USART_ErrorClear( void )
{
    uint8_t  u8dummyData = 0U;
    USART_ERROR errorStatus = (USART_ERROR) (SERCOM3_REGS->USART_INT.SERCOM_STATUS & (uint16_t)(SERCOM_USART_INT_STATUS_PERR_Msk | SERCOM_USART_INT_STATUS_FERR_Msk | SERCOM_USART_INT_STATUS_BUFOVF_Msk ));

    if(errorStatus != USART_ERROR_NONE)
    {
        /* Clear all errors */
        SERCOM3_REGS->USART_INT.SERCOM_STATUS = (uint16_t)(SERCOM_USART_INT_STATUS_PERR_Msk | SERCOM_USART_INT_STATUS_FERR_Msk | SERCOM_USART_INT_STATUS_BUFOVF_Msk);

        /* Flush existing error bytes from the RX FIFO */
        while((SERCOM3_REGS->USART_INT.SERCOM_INTFLAG & (uint8_t)SERCOM_USART_INT_INTFLAG_RXC_Msk) == (uint8_t)SERCOM_USART_INT_INTFLAG_RXC_Msk)
        {
            u8dummyData = (uint8_t)SERCOM3_REGS->USART_INT.SERCOM_DATA;
        }
    }

    /* Ignore the warning */
    (void)u8dummyData;
}

void SERCOM3_USART_Initialize( void )
{
    /*
     * Configures USART Clock Mode
     * Configures TXPO and RXPO
     * Configures Data Order
     * Configures Standby Mode
     * Configures Sampling rate
     * Configures IBON
     */
    SERCOM3_REGS->USART_INT.SERCOM_CTRLA = SERCOM_USART_INT_CTRLA_MODE_USART_INT_CLK | SERCOM_USART_INT_CTRLA_RXPO(0x0UL) | SERCOM_USART_INT_CTRLA_TXPO(0x1UL) | SERCOM_USART_INT_CTRLA_DORD_Msk | SERCOM_USART_INT_CTRLA_IBON_Msk | SERCOM_USART_INT_CTRLA_FORM(0x0UL) ;

    /* Configure Baud Rate */
    SERCOM3_REGS->USART_INT.SERCOM_BAUD = (uint16_t)SERCOM_USART_INT_BAUD_BAUD(SERCOM3_USART_INT_BAUD_VALUE);

    /*
     * Configures RXEN
     * Configures TXEN
     * Configures CHSIZE
     * Configures Parity
     * Configures Stop bits
     */
    SERCOM3_REGS->USART_INT.SERCOM_CTRLB = SERCOM_USART_INT_CTRLB_CHSIZE_8_BIT | SERCOM_USART_INT_CTRLB_SBMODE_1_BIT | SERCOM_USART_INT_CTRLB_RXEN_Msk | SERCOM_USART_INT_CTRLB_TXEN_Msk;

    /* Wait for sync */
    while((SERCOM3_REGS->USART_INT.SERCOM_STATUS & (uint16_t)SERCOM_USART_INT_STATUS_SYNCBUSY_Msk) == (uint16_t)SERCOM_USART_INT_STATUS_SYNCBUSY_Msk)
    {
        /* Do nothing */
    }


    /* Enable the UART after the configurations */
    SERCOM3_REGS->USART_INT.SERCOM_CTRLA |= SERCOM_USART_INT_CTRLA_ENABLE_Msk;

    /* Wait for sync */
    while((SERCOM3_REGS->USART_INT.SERCOM_STATUS & (uint16_t)SERCOM_USART_INT_STATUS_SYNCBUSY_Msk) == (uint16_t)SERCOM_USART_INT_STATUS_SYNCBUSY_Msk)
    {
        /* Do nothing */
    }

    /* Initialize instance object */
    sercom3USARTObj.rxBuffer = NULL;
    sercom3USARTObj.rxSize = 0;
    sercom3USARTObj.rxProcessedSize = 0;
    sercom3USARTObj.rxBusyStatus = false;
    sercom3USARTObj.rxCallback = NULL;
    sercom3USARTObj.txBuffer = NULL;
    sercom3USARTObj.txSize = 0;
    sercom3USARTObj.txProcessedSize = 0;
    sercom3USARTObj.txBusyStatus = false;
    sercom3USARTObj.txCallback = NULL;
    sercom3USARTObj.errorStatus = USART_ERROR_NONE;
}

uint32_t SERCOM3_USART_FrequencyGet( void )
{
    return 48000000UL;
}

bool SERCOM3_USART_SerialSetup( USART_SERIAL_SETUP * serialSetup, uint32_t clkFrequency )
{
    bool setupStatus       = false;
    uint32_t baudValue     = 0U;

    bool transferProgress = sercom3USARTObj.txBusyStatus;
    transferProgress = sercom3USARTObj.rxBusyStatus || transferProgress; 
    if(transferProgress)
    {
        /* Transaction is in progress, so return without updating settings */
        return setupStatus;
    }

    if((serialSetup != NULL) && (serialSetup->baudRate != 0U))
    {
        if(clkFrequency == 0U)
        {
            clkFrequency = SERCOM3_USART_FrequencyGet();
        }

        if(clkFrequency >= (16U * serialSetup->baudRate))
        {
            baudValue = 65536U - (uint32_t)(((uint64_t)65536U * 16U * serialSetup->baudRate) / clkFrequency);
        }

        if(baudValue != 0U)
        {
            /* Disable the USART before configurations */
            SERCOM3_REGS->USART_INT.SERCOM_CTRLA &= ~SERCOM_USART_INT_CTRLA_ENABLE_Msk;

            /* Wait for sync */
            while((SERCOM3_REGS->USART_INT.SERCOM_STATUS & (uint16_t)SERCOM_USART_INT_STATUS_SYNCBUSY_Msk) == SERCOM_USART_INT_STATUS_SYNCBUSY_Msk)
            {
                /* Do nothing */
            }

            /* Configure Baud Rate */
            SERCOM3_REGS->USART_INT.SERCOM_BAUD = (uint16_t)SERCOM_USART_INT_BAUD_BAUD(baudValue);

            /* Configure Parity Options */
            if(serialSetup->parity == USART_PARITY_NONE)
            {
                SERCOM3_REGS->USART_INT.SERCOM_CTRLA = 
                (SERCOM3_REGS->USART_INT.SERCOM_CTRLA & ~SERCOM_USART_INT_CTRLA_FORM_Msk) | SERCOM_USART_INT_CTRLA_FORM(0x0);

                SERCOM3_REGS->USART_INT.SERCOM_CTRLB = (SERCOM3_REGS->USART_INT.SERCOM_CTRLB & ~(SERCOM_USART_INT_CTRLB_CHSIZE_Msk | SERCOM_USART_INT_CTRLB_SBMODE_Msk)) | ((uint32_t) serialSetup->dataWidth | (uint32_t) serialSetup->stopBits);
            }
            else
            {
                SERCOM3_REGS->USART_INT.SERCOM_CTRLA = 
                (SERCOM3_REGS->USART_INT.SERCOM_CTRLA & ~SERCOM_USART_INT_CTRLA_FORM_Msk) | SERCOM_USART_INT_CTRLA_FORM(0x1UL);

                SERCOM3_REGS->USART_INT.SERCOM_CTRLB = (SERCOM3_REGS->USART_INT.SERCOM_CTRLB & ~(SERCOM_USART_INT_CTRLB_CHSIZE_Msk | SERCOM_USART_INT_CTRLB_SBMODE_Msk | SERCOM_USART_INT_CTRLB_PMODE_Msk)) | (uint32_t) serialSetup->dataWidth | (uint32_t) serialSetup->stopBits | (uint32_t) serialSetup->parity ;
            }

            /* Wait for sync */
            while((SERCOM3_REGS->USART_INT.SERCOM_STATUS & SERCOM_USART_INT_STATUS_SYNCBUSY_Msk) == SERCOM_USART_INT_STATUS_SYNCBUSY_Msk)
            {
                /* Do nothing */
            }

            /* Enable the USART after the configurations */
            SERCOM3_REGS->USART_INT.SERCOM_CTRLA |= SERCOM_USART_INT_CTRLA_ENABLE_Msk;

            /* Wait for sync */
            while((SERCOM3_REGS->USART_INT.SERCOM_STATUS & SERCOM_USART_INT_STATUS_SYNCBUSY_Msk) == SERCOM_USART_INT_STATUS_SYNCBUSY_Msk)
            {
                /* Do nothing */
            }

            setupStatus = true;
        }
    }

    return setupStatus;
}

USART_ERROR SERCOM3_USART_ErrorGet( void )
{
    USART_ERROR errorStatus = sercom3USARTObj.errorStatus;

    sercom3USARTObj.errorStatus = USART_ERROR_NONE;

    return errorStatus;
}


void SERCOM3_USART_TransmitterEnable( void )
{
    SERCOM3_REGS->USART_INT.SERCOM_CTRLB |= SERCOM_USART_INT_CTRLB_TXEN_Msk;

    /* Wait for sync */
    while((SERCOM3_REGS->USART_INT.SERCOM_STATUS & (uint16_t)SERCOM_USART_INT_STATUS_SYNCBUSY_Msk) == (uint16_t)SERCOM_USART_INT_STATUS_SYNCBUSY_Msk)
    {
        /* Do nothing */
    }
}

void SERCOM3_USART_TransmitterDisable( void )
{
    SERCOM3_REGS->USART_INT.SERCOM_CTRLB &= ~SERCOM_USART_INT_CTRLB_TXEN_Msk;

    /* Wait for sync */
    while((SERCOM3_REGS->USART_INT.SERCOM_STATUS & SERCOM_USART_INT_STATUS_SYNCBUSY_Msk) == SERCOM_USART_INT_STATUS_SYNCBUSY_Msk)
    {
        /* Do nothing */
    }
}

bool SERCOM3_USART_Write( void *buffer, const size_t size )
{
    bool writeStatus      = false;
    uint8_t *pu8Data      = (uint8_t*)buffer;
    uint32_t processedSize = 0U;

    if(pu8Data != NULL)
    {
        if(sercom3USARTObj.txBusyStatus == false)
        {
            sercom3USARTObj.txBuffer = pu8Data;
            sercom3USARTObj.txSize = size;
            sercom3USARTObj.txBusyStatus = true;

            /* Initiate the transfer by sending first byte */
            while (((SERCOM3_REGS->USART_INT.SERCOM_INTFLAG & SERCOM_USART_INT_INTFLAG_DRE_Msk) == SERCOM_USART_INT_INTFLAG_DRE_Msk) &&
                    (processedSize < sercom3USARTObj.txSize))
            {
                if (((SERCOM3_REGS->USART_INT.SERCOM_CTRLB & SERCOM_USART_INT_CTRLB_CHSIZE_Msk) >> SERCOM_USART_INT_CTRLB_CHSIZE_Pos) != 0x01U)
                {
                    /* 8-bit mode */
                    SERCOM3_REGS->USART_INT.SERCOM_DATA = sercom3USARTObj.txBuffer[processedSize++];
                }
                else
                {
                    /* 9-bit mode */
                    SERCOM3_REGS->USART_INT.SERCOM_DATA = ((uint16_t*)sercom3USARTObj.txBuffer)[processedSize++];
                }
            }
            sercom3USARTObj.txProcessedSize = processedSize;
            SERCOM3_REGS->USART_INT.SERCOM_INTENSET = (uint8_t)SERCOM_USART_INT_INTFLAG_DRE_Msk;

            writeStatus = true;
        }
    }

    return writeStatus;
}


bool SERCOM3_USART_WriteIsBusy( void )
{
    return sercom3USARTObj.txBusyStatus;
}

size_t SERCOM3_USART_WriteCountGet( void )
{
    return sercom3USARTObj.txProcessedSize;
}

void SERCOM3_USART_WriteCallbackRegister( SERCOM_USART_CALLBACK callback, uintptr_t context )
{
    sercom3USARTObj.txCallback = callback;

    sercom3USARTObj.txContext = context;
}


void SERCOM3_USART_ReceiverEnable( void )
{
    SERCOM3_REGS->USART_INT.SERCOM_CTRLB |= SERCOM_USART_INT_CTRLB_RXEN_Msk;

    /* Wait for sync */
    while((SERCOM3_REGS->USART_INT.SERCOM_STATUS & (uint16_t)SERCOM_USART_INT_STATUS_SYNCBUSY_Msk) == (uint16_t)SERCOM_USART_INT_STATUS_SYNCBUSY_Msk)
    {
        /* Do nothing */
    }
}

void SERCOM3_USART_ReceiverDisable( void )
{
    SERCOM3_REGS->USART_INT.SERCOM_CTRLB &= ~SERCOM_USART_INT_CTRLB_RXEN_Msk;

    /* Wait for sync */
    while((SERCOM3_REGS->USART_INT.SERCOM_STATUS & (uint16_t)SERCOM_USART_INT_STATUS_SYNCBUSY_Msk) == (uint16_t)SERCOM_USART_INT_STATUS_SYNCBUSY_Msk)
    {
        /* Do nothing */
    }
}

bool SERCOM3_USART_Read( void *buffer, const size_t size )
{
    bool readStatus        = false;
    uint8_t* pu8Data       = (uint8_t*)buffer;

    if(pu8Data != NULL)
    {
        if(sercom3USARTObj.rxBusyStatus == false)
        {
            /* Clear error flags and flush out error data that may have been received when no active request was pending */
            SERCOM3_USART_ErrorClear();

            sercom3USARTObj.rxBuffer = pu8Data;
            sercom3USARTObj.rxSize = size;
            sercom3USARTObj.rxProcessedSize = 0U;
            sercom3USARTObj.rxBusyStatus = true;
            sercom3USARTObj.errorStatus = USART_ERROR_NONE;

            readStatus = true;

            /* Enable Receive Complete interrupt */
            SERCOM3_REGS->USART_INT.SERCOM_INTENSET =  (uint8_t)SERCOM_USART_INT_INTENSET_RXC_Msk;
        }
    }

    return readStatus;
}

bool SERCOM3_USART_ReadIsBusy( void )
{
    return sercom3USARTObj.rxBusyStatus;
}

size_t SERCOM3_USART_ReadCountGet( void )
{
    return sercom3USARTObj.rxProcessedSize;
}

bool SERCOM3_USART_ReadAbort(void)
{
    if (sercom3USARTObj.rxBusyStatus == true)
    {
         /* Disable the receive interrupt */
        SERCOM3_REGS->USART_INT.SERCOM_INTENCLR = (uint8_t)(SERCOM_USART_INT_INTENCLR_RXC_Msk);

        sercom3USARTObj.rxBusyStatus = false;

        /* If required application should read the num bytes processed prior to calling the read abort API */
        sercom3USARTObj.rxSize = sercom3USARTObj.rxProcessedSize = 0U;
    }

    return true;
}

void SERCOM3_USART_ReadCallbackRegister( SERCOM_USART_CALLBACK callback, uintptr_t context )
{
    sercom3USARTObj.rxCallback = callback;

    sercom3USARTObj.rxContext = context;
}



void static SERCOM3_USART_ISR_RX_Handler( void )
{
    uint16_t temp;

    USART_ERROR errorStatus = (USART_ERROR) (SERCOM3_REGS->USART_INT.SERCOM_STATUS & (uint16_t)(SERCOM_USART_INT_STATUS_PERR_Msk | SERCOM_USART_INT_STATUS_FERR_Msk | SERCOM_USART_INT_STATUS_BUFOVF_Msk));

    if(sercom3USARTObj.rxBusyStatus == true)
    {
        if(sercom3USARTObj.rxProcessedSize < sercom3USARTObj.rxSize)
        {
            if (errorStatus != USART_ERROR_NONE)
            {
                /* Save the error to be reported later */
                sercom3USARTObj.errorStatus = errorStatus;

                /* Clear the error flags and flush out the error bytes */
                SERCOM3_USART_ErrorClear();

                sercom3USARTObj.rxBusyStatus = false;
                sercom3USARTObj.rxSize = 0U;

                SERCOM3_REGS->USART_INT.SERCOM_INTENCLR = (uint8_t)SERCOM_USART_INT_INTENCLR_RXC_Msk;

                if(sercom3USARTObj.rxCallback != NULL)
                {
                    sercom3USARTObj.rxCallback(sercom3USARTObj.rxContext);
                }
            }
            else
            {
                temp = SERCOM3_REGS->USART_INT.SERCOM_DATA;

                if (((SERCOM3_REGS->USART_INT.SERCOM_CTRLB & SERCOM_USART_INT_CTRLB_CHSIZE_Msk) >> SERCOM_USART_INT_CTRLB_CHSIZE_Pos) != 0x01U)
                {
                    /* 8-bit mode */
                    sercom3USARTObj.rxBuffer[sercom3USARTObj.rxProcessedSize++] = (uint8_t) (temp);
                }
                else
                {
                    /* 9-bit mode */
                    ((uint16_t*)sercom3USARTObj.rxBuffer)[sercom3USARTObj.rxProcessedSize++] = (uint16_t) (temp);
                }

                if(sercom3USARTObj.rxProcessedSize == sercom3USARTObj.rxSize)
                {
                    sercom3USARTObj.rxBusyStatus = false;
                    sercom3USARTObj.rxSize = 0U;
                    SERCOM3_REGS->USART_INT.SERCOM_INTENCLR = (uint8_t)SERCOM_USART_INT_INTENCLR_RXC_Msk;

                    if(sercom3USARTObj.rxCallback != NULL)
                    {
                        sercom3USARTObj.rxCallback(sercom3USARTObj.rxContext);
                    }
                }
            }

        }
    }
}

void static SERCOM3_USART_ISR_TX_Handler( void )
{
    bool  dataRegisterEmpty= false;
    bool  dataAvailable = false;
    if(sercom3USARTObj.txBusyStatus == true)
    {
        dataAvailable = (sercom3USARTObj.txProcessedSize < sercom3USARTObj.txSize);
        dataRegisterEmpty = ((SERCOM3_REGS->USART_INT.SERCOM_INTFLAG & SERCOM_USART_INT_INTFLAG_DRE_Msk) == SERCOM_USART_INT_INTFLAG_DRE_Msk);
        
        while(dataRegisterEmpty && dataAvailable)
        {
            if (((SERCOM3_REGS->USART_INT.SERCOM_CTRLB & SERCOM_USART_INT_CTRLB_CHSIZE_Msk) >> SERCOM_USART_INT_CTRLB_CHSIZE_Pos) != 0x01U)
            {
                /* 8-bit mode */
                SERCOM3_REGS->USART_INT.SERCOM_DATA = sercom3USARTObj.txBuffer[sercom3USARTObj.txProcessedSize++];
            }
            else
            {
                /* 9-bit mode */
                SERCOM3_REGS->USART_INT.SERCOM_DATA = ((uint16_t*)sercom3USARTObj.txBuffer)[sercom3USARTObj.txProcessedSize++];
            }
            dataAvailable = (sercom3USARTObj.txProcessedSize < sercom3USARTObj.txSize);
            dataRegisterEmpty = ((SERCOM3_REGS->USART_INT.SERCOM_INTFLAG & SERCOM_USART_INT_INTFLAG_DRE_Msk) == SERCOM_USART_INT_INTFLAG_DRE_Msk);
        }

        if(sercom3USARTObj.txProcessedSize >= sercom3USARTObj.txSize)
        {
            sercom3USARTObj.txBusyStatus = false;
            sercom3USARTObj.txSize = 0U;
            SERCOM3_REGS->USART_INT.SERCOM_INTENCLR = (uint8_t)SERCOM_USART_INT_INTENCLR_DRE_Msk;

            if(sercom3USARTObj.txCallback != NULL)
            {
                sercom3USARTObj.txCallback(sercom3USARTObj.txContext);
            }
        }
    }
}

void SERCOM3_USART_InterruptHandler( void )
{
    bool testCondition = false;
    if(SERCOM3_REGS->USART_INT.SERCOM_INTENSET != 0U)
    {

        testCondition = ((SERCOM3_REGS->USART_INT.SERCOM_INTFLAG & SERCOM_USART_INT_INTFLAG_DRE_Msk) == SERCOM_USART_INT_INTFLAG_DRE_Msk);
        testCondition = ((SERCOM3_REGS->USART_INT.SERCOM_INTENSET & SERCOM_USART_INT_INTENSET_DRE_Msk) == SERCOM_USART_INT_INTENSET_DRE_Msk) && testCondition;
        /* Checks for data register empty flag */
        if(testCondition)
        {
            SERCOM3_USART_ISR_TX_Handler();
        }

        testCondition = ((SERCOM3_REGS->USART_INT.SERCOM_INTFLAG & SERCOM_USART_INT_INTFLAG_RXC_Msk) == SERCOM_USART_INT_INTFLAG_RXC_Msk);
        testCondition = ((SERCOM3_REGS->USART_INT.SERCOM_INTENSET & SERCOM_USART_INT_INTENSET_RXC_Msk) == SERCOM_USART_INT_INTENSET_RXC_Msk) && testCondition;
        /* Checks for receive complete empty flag */
        if(testCondition)
        {
            SERCOM3_USART_ISR_RX_Handler();
        }
    }
}
