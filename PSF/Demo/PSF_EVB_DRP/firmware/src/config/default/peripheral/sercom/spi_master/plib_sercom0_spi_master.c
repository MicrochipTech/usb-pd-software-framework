/*******************************************************************************
  SERIAL COMMUNICATION SERIAL PERIPHERAL INTERFACE(SERCOM0_SPI) PLIB

  Company
    Microchip Technology Inc.

  File Name
    plib_sercom0_spi_master.c

  Summary
    SERCOM0_SPI Master PLIB Implementation File.

  Description
    This file defines the interface to the SERCOM SPI peripheral library.
    This library provides access to and control of the associated
    peripheral instance.

  Remarks:
    None.

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

#include "interrupts.h"
#include "plib_sercom0_spi_master.h"

// *****************************************************************************
// *****************************************************************************
// Section: MACROS Definitions
// *****************************************************************************
// *****************************************************************************


/* SERCOM0 clk freq value for the baud calculation */
#define SERCOM0_Frequency      (48000000UL)

/* SERCOM0 SPI baud value for 8000000 Hz baud rate */
#define SERCOM0_SPIM_BAUD_VALUE         (2UL)


// *****************************************************************************
// *****************************************************************************
// Section: SERCOM0_SPI Implementation
// *****************************************************************************
// *****************************************************************************
// *****************************************************************************

// *****************************************************************************
/* Function:
    void SERCOM0_SPI_Initialize(void);

  Summary:
    Initializes instance SERCOM0 of the SERCOM module operating in SPI mode.

  Description:
    This function initializes instance SERCOM0 of SERCOM module operating in SPI mode.
    This function should be called before any other library function. The SERCOM
    module will be configured as per the MHC settings.

  Remarks:
    Refer plib_sercom0_spi.h file for more information.
*/

void SERCOM0_SPI_Initialize(void)
{

    /* Selection of the Character Size and Receiver Enable */
    SERCOM0_REGS->SPIM.SERCOM_CTRLB = SERCOM_SPIM_CTRLB_CHSIZE_8_BIT | SERCOM_SPIM_CTRLB_RXEN_Msk ;

    /* Wait for synchronization */
    while((SERCOM0_REGS->SPIM.SERCOM_STATUS & (uint16_t)SERCOM_SPIM_STATUS_SYNCBUSY_Msk) == (uint16_t)SERCOM_SPIM_STATUS_SYNCBUSY_Msk)
    {
        /* Do nothing */
    }

    /* Selection of the Baud Value */
    SERCOM0_REGS->SPIM.SERCOM_BAUD = (uint8_t)SERCOM_SPIM_BAUD_BAUD(SERCOM0_SPIM_BAUD_VALUE);

    /* Configure Data Out Pin Out , Master Mode,
     * Data In and Pin Out,Data Order and Standby mode if configured
     * and Selection of the Clock Phase and Polarity and Enable the SPI Module
     */
    SERCOM0_REGS->SPIM.SERCOM_CTRLA = SERCOM_SPIM_CTRLA_MODE_SPI_MASTER | SERCOM_SPIM_CTRLA_DOPO_PAD2 | SERCOM_SPIM_CTRLA_DIPO_PAD0 | SERCOM_SPIM_CTRLA_CPOL_IDLE_LOW | SERCOM_SPIM_CTRLA_CPHA_LEADING_EDGE | SERCOM_SPIM_CTRLA_DORD_MSB | SERCOM_SPIM_CTRLA_ENABLE_Msk ;


    /* Wait for synchronization */
    while((SERCOM0_REGS->SPIM.SERCOM_STATUS & (uint16_t)SERCOM_SPIM_STATUS_SYNCBUSY_Msk) == (uint16_t)SERCOM_SPIM_STATUS_SYNCBUSY_Msk)
    {
        /* Do nothing */
    }
}

// *****************************************************************************
/* Function:
    bool SERCOM0_SPI_TransferSetup(SPI_TRANSFER_SETUP *setup,
                                                uint32_t spiSourceClock);

 Summary:
    Configure SERCOM SPI operational parameters at run time.

  Description:
    This function allows the application to change the SERCOM SPI operational
    parameter at run time. The application can thus override the MHC defined
    configuration for these parameters. The parameter are specified via the
    SPI_TRANSFER_SETUP type setup parameter. Each member of this parameter
    should be initialized to the desired value.

    The application may feel need to call this function in situation where
    multiple SPI slaves, each with different operation parameters, are connected
    to one SPI master. This function can thus be used to setup the SPI Master to
    meet the communication needs of the slave.

    Calling this function will affect any ongoing communication. The application
    must thus ensure that there is no on-going communication on the SPI before
    calling this function.

  Remarks:
    Refer plib_sercom0_spi.h file for more information.
*/

bool SERCOM0_SPI_TransferSetup(SPI_TRANSFER_SETUP *setup, uint32_t spiSourceClock)
{
    uint32_t baudValue = 0U;

    bool statusValue = false;

    if(spiSourceClock == 0U)
    {
        /* Fetch Master Clock Frequency directly */
        spiSourceClock = SERCOM0_Frequency;
    }

    /* Disable the SPI Module */
    SERCOM0_REGS->SPIM.SERCOM_CTRLA &= ~(SERCOM_SPIM_CTRLA_ENABLE_Msk);

    /* Wait for synchronization */
    while((SERCOM0_REGS->SPIM.SERCOM_STATUS & (uint16_t)SERCOM_SPIM_STATUS_SYNCBUSY_Msk) == (uint16_t)SERCOM_SPIM_STATUS_SYNCBUSY_Msk)
    {
        /* Do nothing */
    }

    if(setup != NULL)
    {
        baudValue = (spiSourceClock/(2U*(setup->clockFrequency))) - 1U;

        if((baudValue > 0U) && (baudValue <= 255U))
        {
            /* Selection of the Clock Polarity and Clock Phase */
            SERCOM0_REGS->SPIM.SERCOM_CTRLA &= ~(SERCOM_SPIM_CTRLA_CPOL_Msk | SERCOM_SPIM_CTRLA_CPHA_Msk);
            SERCOM0_REGS->SPIM.SERCOM_CTRLA |= (uint32_t)setup->clockPolarity | (uint32_t)setup->clockPhase;

            /* Selection of the Baud Value */
            SERCOM0_REGS->SPIM.SERCOM_BAUD = (uint8_t)baudValue;

            /* Selection of the Character Size */
            SERCOM0_REGS->SPIM.SERCOM_CTRLB &= ~SERCOM_SPIM_CTRLB_CHSIZE_Msk;
            SERCOM0_REGS->SPIM.SERCOM_CTRLB |= (uint32_t)setup->dataBits;

            /* Wait for synchronization */
            while((SERCOM0_REGS->SPIM.SERCOM_STATUS & (uint16_t)SERCOM_SPIM_STATUS_SYNCBUSY_Msk) == (uint16_t)SERCOM_SPIM_STATUS_SYNCBUSY_Msk)
            {
                /* Do nothing */
            }

            statusValue = true;
        }
    }

    /* Enabling the SPI Module */
    SERCOM0_REGS->SPIM.SERCOM_CTRLA |= SERCOM_SPIM_CTRLA_ENABLE_Msk;

    /* Wait for synchronization */
    while((SERCOM0_REGS->SPIM.SERCOM_STATUS & (uint16_t)SERCOM_SPIM_STATUS_SYNCBUSY_Msk) == (uint16_t)SERCOM_SPIM_STATUS_SYNCBUSY_Msk)
    {
        /* Do nothing */
    }

    return statusValue;
}



bool SERCOM0_SPI_IsTransmitterBusy(void)
{
    return ((SERCOM0_REGS->SPIM.SERCOM_INTFLAG & SERCOM_SPIM_INTFLAG_TXC_Msk) == 0)? true : false;
}

// *****************************************************************************
/* Function:
    bool SERCOM0_SPI_WriteRead (void* pTransmitData, size_t txSize
                                        void* pReceiveData, size_t rxSize);

  Summary:
    Write and Read data on SERCOM SERCOM0 SPI peripheral.

  Description:
    This function transmits "txSize" number of bytes and receives "rxSize"
    number of bytes on SERCOM SERCOM0 SPI module. Data pointed by pTransmitData is
    transmitted and received data is saved in the location pointed by
    pReceiveData. The function will transfer the maximum of "txSize" or "rxSize"
    data units towards completion.

    When "Interrupt Mode" option is unchecked in MHC, this function will be
    blocking in nature.  In this mode, the function will not return until all
    the requested data is transferred.  The function returns true after
    transferring all the data.  This indicates that the operation has been
    completed.

    When "Interrupt Mode" option is selected in MHC, the function will be
    non-blocking in nature.  The function returns immediately. The data transfer
    process continues in the peripheral interrupt.  The application specified
    transmit and receive buffer  are ownerd by the library until the data
    transfer is complete and should not be modified by the application till the
    transfer is complete.  Only one transfer is allowed at any time. The
    Application can use a callback function or a polling function to check for
    completion of the transfer. If a callback is required, this should be
    registered prior to calling the SERCOM0_SPI_WriteRead() function. The
    application can use the SERCOM0_SPI_IsBusy() to poll for completion.

  Remarks:
    Refer plib_sercom0_spi.h file for more information.
*/

bool SERCOM0_SPI_WriteRead (void* pTransmitData, size_t txSize, void* pReceiveData, size_t rxSize)
{
    size_t txCount = 0U;
    size_t rxCount = 0U;
    size_t dummySize = 0U;
    size_t receivedData;
    uint32_t dataBits;
    bool isSuccess = false;

    /* Verify the request */
    if(((txSize > 0U) && (pTransmitData != NULL)) || ((rxSize > 0U) && (pReceiveData != NULL)))
    {
        dataBits = SERCOM0_REGS->SPIM.SERCOM_CTRLB & SERCOM_SPIM_CTRLB_CHSIZE_Msk;

        if(dataBits != (uint32_t)SPI_DATA_BITS_8)
        {
            /* For 9-bit transmission, the txSize and rxSize must be an even number. */
            if(((txSize > 0U) && ((txSize & 0x01U) != 0U)) || ((rxSize > 0U) && ((rxSize & 0x01U) != 0U)))
            {
                return isSuccess;
            }
        }

        if(pTransmitData == NULL)
        {
            txSize = 0U;
        }

        if(pReceiveData == NULL)
        {
            rxSize = 0U;
        }

        /* Flush out any unread data in SPI DATA Register from the previous transfer */
        while((SERCOM0_REGS->SPIM.SERCOM_INTFLAG & SERCOM_SPIM_INTFLAG_RXC_Msk) == SERCOM_SPIM_INTFLAG_RXC_Msk)
        {
            receivedData = SERCOM0_REGS->SPIM.SERCOM_DATA;
        }

        SERCOM0_REGS->SPIM.SERCOM_STATUS |= (uint16_t)SERCOM_SPIM_STATUS_BUFOVF_Msk;

        if(rxSize > txSize)
        {
            dummySize = rxSize - txSize;
        }

        if(dataBits != (uint32_t)SPI_DATA_BITS_8)
        {
            rxSize >>= 1U;
            txSize >>= 1U;
            dummySize >>= 1U;
        }

        /* Make sure DRE is empty */
        while((SERCOM0_REGS->SPIM.SERCOM_INTFLAG & SERCOM_SPIM_INTFLAG_DRE_Msk) != SERCOM_SPIM_INTFLAG_DRE_Msk)
        {
            /* Do nothing */
        }

        while((txCount != txSize) || (dummySize != 0U))
        {
            if(txCount != txSize)
            {
                if(dataBits == (uint32_t)SPI_DATA_BITS_8)
                {
                    SERCOM0_REGS->SPIM.SERCOM_DATA = ((uint8_t*)pTransmitData)[txCount++];
                }
                else
                {
                    SERCOM0_REGS->SPIM.SERCOM_DATA = ((uint16_t*)pTransmitData)[txCount++] & SERCOM_SPIM_DATA_Msk;
                }
            }
            else if(dummySize > 0U)
            {
                if(dataBits == (uint32_t)SPI_DATA_BITS_8)
                {
                    SERCOM0_REGS->SPIM.SERCOM_DATA = 0xFFU;
                }
                else
                {
                    SERCOM0_REGS->SPIM.SERCOM_DATA = 0xFFFFU & SERCOM_SPIM_DATA_Msk;
                }

                dummySize--;
            }
            else
            {
                /* Do nothing */
            }

            if(rxSize == 0U)
            {
                /* For transmit only request, wait for DRE to become empty */
                while((SERCOM0_REGS->SPIM.SERCOM_INTFLAG & SERCOM_SPIM_INTFLAG_DRE_Msk) != SERCOM_SPIM_INTFLAG_DRE_Msk)
                {
                    /* Do nothing */
                }
            }
            else
            {
                /* If data is read, wait for the Receiver Data Register to become full */
                while((SERCOM0_REGS->SPIM.SERCOM_INTFLAG & SERCOM_SPIM_INTFLAG_RXC_Msk) != SERCOM_SPIM_INTFLAG_RXC_Msk)
                {
                    /* Do nothing */
                }

                receivedData = SERCOM0_REGS->SPIM.SERCOM_DATA;

                if(rxCount < rxSize)
                {
                    if(dataBits == (uint32_t)SPI_DATA_BITS_8)
                    {
                        ((uint8_t*)pReceiveData)[rxCount++] = (uint8_t)receivedData;
                    }
                    else
                    {
                        ((uint16_t*)pReceiveData)[rxCount++] = (uint16_t)(receivedData & SERCOM_SPIM_DATA_Msk);
                    }
                }
            }
        }

        /* Make sure no data is pending in the shift register */
        while((SERCOM0_REGS->SPIM.SERCOM_INTFLAG & SERCOM_SPIM_INTFLAG_TXC_Msk) != SERCOM_SPIM_INTFLAG_TXC_Msk)
        {
            /* Do nothing */
        }

        isSuccess = true;
    }

    return isSuccess;
}

bool SERCOM0_SPI_Write(void* pTransmitData, size_t txSize)
{
    return SERCOM0_SPI_WriteRead(pTransmitData, txSize, NULL, 0U);
}

bool SERCOM0_SPI_Read(void* pReceiveData, size_t rxSize)
{
    return SERCOM0_SPI_WriteRead(NULL, 0U, pReceiveData, rxSize);
}

