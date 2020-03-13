/*******************************************************************************
  Serial Communication Interface Inter-Integrated Circuit (SERCOM I2C) Library
  Source File

  Company:
    Microchip Technology Inc.

  File Name:
    plib_sercom3_i2c.c

  Summary:
    SERCOM I2C PLIB Implementation file

  Description:
    This file defines the interface to the SERCOM I2C peripheral library.
    This library provides access to and control of the associated peripheral
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

#include "plib_sercom3_i2c.h"

// *****************************************************************************
// *****************************************************************************
// Section: Global Data
// *****************************************************************************
// *****************************************************************************


/* SERCOM3 I2C baud value for 400 Khz baud rate */
#define SERCOM3_I2CM_BAUD_VALUE			(13620U)

static SERCOM_I2C_OBJ sercom3I2CObj;

// *****************************************************************************
// *****************************************************************************
// Section: SERCOM3 I2C Implementation
// *****************************************************************************
// *****************************************************************************
// *****************************************************************************

// *****************************************************************************
/* Function:
    void SERCOM3_I2C_Initialize(void)

  Summary:
    Initializes the instance of the SERCOM peripheral operating in I2C mode.

  Description:
    This function initializes the given instance of the SERCOM I2C peripheral as
    configured by the user from the MHC.

  Remarks:
    Refer plib_sercom3_i2c.h for more information.
*/

void SERCOM3_I2C_Initialize(void)
{
    /* Enable smart mode enable */
    SERCOM3_REGS->I2CM.SERCOM_CTRLB = SERCOM_I2CM_CTRLB_SMEN_Msk;

    /* Wait for synchronization */
    while((SERCOM3_REGS->I2CM.SERCOM_STATUS & SERCOM_I2CM_STATUS_SYNCBUSY_Msk) & SERCOM_I2CM_STATUS_SYNCBUSY_Msk);

    /* Baud rate - Master Baud Rate*/
    SERCOM3_REGS->I2CM.SERCOM_BAUD = SERCOM_I2CM_BAUD_BAUD(SERCOM3_I2CM_BAUD_VALUE);

    /* Set Operation Mode (Master), SDA Hold time, run in stand by and i2c master enable */
    SERCOM3_REGS->I2CM.SERCOM_CTRLA = SERCOM_I2CM_CTRLA_MODE_I2C_MASTER | SERCOM_I2CM_CTRLA_SDAHOLD_75NS | SERCOM_I2CM_CTRLA_ENABLE_Msk ;

    /* Wait for synchronization */
    while((SERCOM3_REGS->I2CM.SERCOM_STATUS & SERCOM_I2CM_STATUS_SYNCBUSY_Msk) & SERCOM_I2CM_STATUS_SYNCBUSY_Msk);

    /* Initial Bus State: IDLE */
    SERCOM3_REGS->I2CM.SERCOM_STATUS = SERCOM_I2CM_STATUS_BUSSTATE(0x01);

    /* Wait for synchronization */
    while((SERCOM3_REGS->I2CM.SERCOM_STATUS & SERCOM_I2CM_STATUS_SYNCBUSY_Msk) & SERCOM_I2CM_STATUS_SYNCBUSY_Msk);

    /* Initialize the SERCOM3 PLib Object */
    sercom3I2CObj.error = SERCOM_I2C_ERROR_NONE;
    sercom3I2CObj.state = SERCOM_I2C_STATE_IDLE;

    /* Enable all Interrupts */
    SERCOM3_REGS->I2CM.SERCOM_INTENSET = SERCOM_I2CM_INTENSET_Msk;
}

bool SERCOM3_I2C_TransferSetup(SERCOM_I2C_TRANSFER_SETUP* setup, uint32_t srcClkFreq )
{       
    uint32_t baudValue;    
    uint32_t i2cClkSpeed;
    
    if (setup == NULL)
    {
        return false;
    }        
        
    i2cClkSpeed = setup->clkSpeed;
    
    if( srcClkFreq == 0)
    {
        srcClkFreq = 48000000UL;
    }    
    
    /* Reference clock frequency must be atleast two times the baud rate */
    if (srcClkFreq < (2*i2cClkSpeed))
    {
        return false;
    }
    
    baudValue = ((((float)srcClkFreq)/i2cClkSpeed) - ((((float)srcClkFreq) * (100/1000000000.0)) + 10))/2.0;
    
    /* BAUD.BAUD must be non-zero */
    if (baudValue == 0)
    {
        return false;
    }
    
    /* Disable the I2C before changing the I2C clock speed */
    SERCOM3_REGS->I2CM.SERCOM_CTRLA &= ~SERCOM_I2CM_CTRLA_ENABLE_Msk;
    
    /* Wait for synchronization */
    while((SERCOM3_REGS->I2CM.SERCOM_STATUS & SERCOM_I2CM_STATUS_SYNCBUSY_Msk) & SERCOM_I2CM_STATUS_SYNCBUSY_Msk);
    
    /* Baud rate - Master Baud Rate*/
    SERCOM3_REGS->I2CM.SERCOM_BAUD = SERCOM_I2CM_BAUD_BAUD(baudValue);   

    /* Re-enable the I2C module */
    SERCOM3_REGS->I2CM.SERCOM_CTRLA |= SERCOM_I2CM_CTRLA_ENABLE_Msk; 
    
    /* Wait for synchronization */
    while((SERCOM3_REGS->I2CM.SERCOM_STATUS & SERCOM_I2CM_STATUS_SYNCBUSY_Msk) & SERCOM_I2CM_STATUS_SYNCBUSY_Msk);

    /* Since the I2C module was disabled, re-initialize the bus state to IDLE */
    SERCOM3_REGS->I2CM.SERCOM_STATUS = SERCOM_I2CM_STATUS_BUSSTATE(0x01);    
    
    /* Wait for synchronization */
    while((SERCOM3_REGS->I2CM.SERCOM_STATUS & SERCOM_I2CM_STATUS_SYNCBUSY_Msk) & SERCOM_I2CM_STATUS_SYNCBUSY_Msk);
    
    return true;
}

// *****************************************************************************
/* Function:
    void SERCOM3_I2C_InitiateRead(uint16_t address)

  Summary:
    Intiates I2C Read

  Description:

  Remarks:
    Refer plib_sercom3_i2c.h for more information.
*/

static void SERCOM3_I2C_InitiateRead(uint16_t address)
{
    sercom3I2CObj.state = SERCOM_I2C_STATE_TRANSFER_READ;

    SERCOM3_REGS->I2CM.SERCOM_ADDR = (address << 1) | I2C_TRANSFER_READ;

    /* Wait for synchronization */
    while((SERCOM3_REGS->I2CM.SERCOM_STATUS & SERCOM_I2CM_STATUS_SYNCBUSY_Msk) & SERCOM_I2CM_STATUS_SYNCBUSY_Msk);
}

// *****************************************************************************
/* Function:
    void SERCOM3_I2C_CallbackRegister(SERCOM_I2C_CALLBACK callback,
                                                              uintptr_t context)

   Summary:
    Sets the pointer to the function (and it's context) to be called when the
    given SERCOM I2C's transfer events occur.

   Precondition:
    SERCOMx_I2C_Initialize must have been called for the associated SERCOM instance.

   Parameters:
    callback - A pointer to a function with a calling signature defined
    by the SERCOM_I2C_CALLBACK data type.

    context - A value (usually a pointer) passed (unused) into the function
    identified by the callback parameter.

   Returns:
    None.
*/

void SERCOM3_I2C_CallbackRegister(SERCOM_I2C_CALLBACK callback, uintptr_t contextHandle)
{
    //sercom3I2CObj.callback = callback;
    sercom3I2CObj.callback = NULL;

    sercom3I2CObj.context  = contextHandle;
}

/// *****************************************************************************
/* Function:
    void SERCOM3_2C_InitiateTransfer(uint16_t address, bool type)

  Summary:
    Send the 7-bit or 10-bit slave address.

  Precondition:
    SERCOMx_I2C_Initialize must have been called for the associated SERCOM instance.

   Parameters:
     address - 7-bit / 10-bit slave address.
     type - Read / Write

  Remarks:
    None.
*/

static void SERCOM3_I2C_InitiateTransfer(uint16_t address, bool type)
{
    sercom3I2CObj.writeCount = 0;
    sercom3I2CObj.readCount = 0;

    /* Clear all flags */
    SERCOM3_REGS->I2CM.SERCOM_INTFLAG = SERCOM_I2CM_INTFLAG_Msk;

    /* Smart mode enabled - ACK is set to send while receiving the data */
    SERCOM3_REGS->I2CM.SERCOM_CTRLB &= ~SERCOM_I2CM_CTRLB_ACKACT_Msk;

    /* Wait for synchronization */
    while((SERCOM3_REGS->I2CM.SERCOM_STATUS & SERCOM_I2CM_STATUS_SYNCBUSY_Msk) & SERCOM_I2CM_STATUS_SYNCBUSY_Msk);

    /* Reset Error Information */
    sercom3I2CObj.error = SERCOM_I2C_ERROR_NONE;

    if(type)
    {
        sercom3I2CObj.state = SERCOM_I2C_STATE_TRANSFER_READ;

        /* Write 7bit address with direction (ADDR.ADDR[0]) equal to 1*/
        SERCOM3_REGS->I2CM.SERCOM_ADDR = (address << 1) | I2C_TRANSFER_READ;
    }
    else
    {
        sercom3I2CObj.state = SERCOM_I2C_STATE_TRANSFER_WRITE;

        /* Write 7bit address with direction (ADDR.ADDR[0]) equal to 0*/
        SERCOM3_REGS->I2CM.SERCOM_ADDR = (address << 1) | I2C_TRANSFER_WRITE;
    }

    /* Wait for synchronization */
    while((SERCOM3_REGS->I2CM.SERCOM_STATUS & SERCOM_I2CM_STATUS_SYNCBUSY_Msk) & SERCOM_I2CM_STATUS_SYNCBUSY_Msk);
}

// *****************************************************************************
/* Function:
    bool SERCOM3_I2C_Read(uint16_t address, uint8_t *pdata,
                                                                 uint32_t length)

   Summary:
    Reads data from the slave.

   Precondition:
    SERCOMx_I2C_Initialize must have been called for the associated SERCOM instance.

   Parameters:
    address - 7-bit / 10-bit slave address.
    pdata   - pointer to destination data buffer
    length  - length of data buffer in number of bytes.

   Returns:
    Request status.
    True - Request was successful.
    False - Request has failed.
*/

bool SERCOM3_I2C_Read(uint16_t address, uint8_t *pdata, uint32_t length)
{
    /* Check for ongoing transfer */
    if(sercom3I2CObj.state != SERCOM_I2C_STATE_IDLE)
    {
        return false;
    }

    sercom3I2CObj.address = address;
    sercom3I2CObj.readBuffer = pdata;
    sercom3I2CObj.readSize = length;
    sercom3I2CObj.writeBuffer = NULL;
    sercom3I2CObj.writeSize = 0;
    sercom3I2CObj.error = SERCOM_I2C_ERROR_NONE;

    SERCOM3_I2C_InitiateTransfer(address, true);

    return true;
}

// *****************************************************************************
/* Function:
    bool SERCOM3_I2C_Write(uint16_t address, uint8_t *pdata,
                                                                 uint32_t length)

   Summary:
    Writes data onto the slave.

   Precondition:
    SERCOMx_I2C_Initialize must have been called for the associated SERCOM instance.

   Parameters:
    address - 7-bit / 10-bit slave address.
    pdata   - pointer to source data buffer
    length  - length of data buffer in number of bytes.

   Returns:
    Request status.
    True - Request was successful.
    False - Request has failed.
*/

bool SERCOM3_I2C_Write(uint16_t address, uint8_t *pdata, uint32_t length)
{
    /* Check for ongoing transfer */
    if(sercom3I2CObj.state != SERCOM_I2C_STATE_IDLE)
    {
        return false;
    }

    sercom3I2CObj.address = address;
    sercom3I2CObj.readBuffer = NULL;
    sercom3I2CObj.readSize = 0;
    sercom3I2CObj.writeBuffer = pdata;
    sercom3I2CObj.writeSize = length;
    sercom3I2CObj.error = SERCOM_I2C_ERROR_NONE;

    SERCOM3_I2C_InitiateTransfer(address, false);

    return true;
}

// *****************************************************************************
/* Function:
    bool SERCOM3_I2C_WriteRead(uint16_t address, uint8_t *wdata,
                               uint32_t wlength, uint8_t *rdata, uint32_t rlength)

   Summary:
    Write and Read data from Slave.

   Precondition:
    SERCOMx_I2C_Initialize must have been called for the associated SERCOM instance.

   Parameters:
    address - 7-bit / 10-bit slave address.
    wdata   - pointer to write data buffer
    wlength - write data length in bytes.
    rdata   - pointer to read data buffer.
    rlength - read data length in bytes.

   Returns:
    Request status.
    True - Request was successful.
    False - Request has failed.
*/

bool SERCOM3_I2C_WriteRead(uint16_t address, uint8_t *wdata, uint32_t wlength, uint8_t *rdata, uint32_t rlength)
{
    /* Check for ongoing transfer */
    if(sercom3I2CObj.state != SERCOM_I2C_STATE_IDLE)
    {
        return false;
    }

    sercom3I2CObj.address = address;
    sercom3I2CObj.readBuffer = rdata;
    sercom3I2CObj.readSize = rlength;
    sercom3I2CObj.writeBuffer = wdata;
    sercom3I2CObj.writeSize = wlength;
    sercom3I2CObj.error = SERCOM_I2C_ERROR_NONE;

    SERCOM3_I2C_InitiateTransfer(address, false);

    return true;
}

// *****************************************************************************
/* Function:
    bool SERCOM3_I2C_IsBusy(void)

   Summary:
    Returns the Peripheral busy status.

   Precondition:
    SERCOMx_I2C_Initialize must have been called for the associated SERCOM instance.

   Parameters:
    None.

   Returns:
    true - Busy.
    false - Not busy.
*/

bool SERCOM3_I2C_IsBusy(void)
{
    if((sercom3I2CObj.state == SERCOM_I2C_STATE_IDLE) && ((SERCOM3_REGS->I2CM.SERCOM_STATUS & SERCOM_I2CM_STATUS_BUSSTATE_Msk) == SERCOM_I2CM_STATUS_BUSSTATE(0x01)))
    {
        return false;
    }
    else
    {
        return true;
    }
}

// *****************************************************************************
/* Function:
    SERCOM_I2C_ERROR SERCOM3_I2C_ErrorGet(void)

   Summary:
    Returns the error during transfer.

   Precondition:
    SERCOMx_I2C_Initialize must have been called for the associated SERCOM instance.

   Parameters:
    None.

   Returns:
    Error during transfer.
*/

SERCOM_I2C_ERROR SERCOM3_I2C_ErrorGet(void)
{
    return sercom3I2CObj.error;
}

// *****************************************************************************
/* Function:
    void SERCOM3_I2C_InterruptHandler(void)

  Summary:
    SERCOM3 I2C Peripheral Interrupt Handler.

  Description:
    This function is SERCOM3 I2C Peripheral Interrupt Handler and will
    called on every SERCOM3 I2C interrupt.

   Precondition:
    None.

   Parameters:
    None.

   Returns:
    None.

   Remarks:
    The function is called as peripheral instance's interrupt handler if the
    instance interrupt is enabled. If peripheral instance's interrupt is not
    enabled user need to call it from the main while loop of the application.
*/

void SERCOM3_I2C_InterruptHandler(void)
{
    if(SERCOM3_REGS->I2CM.SERCOM_INTENSET != 0)
    {
        /* Checks if the arbitration lost in multi-master scenario */
        if((SERCOM3_REGS->I2CM.SERCOM_STATUS & SERCOM_I2CM_STATUS_ARBLOST_Msk) == SERCOM_I2CM_STATUS_ARBLOST_Msk)
        {
            /*
             * Re-initiate the transfer if arbitration is lost
             * in between of the transfer
             */
            sercom3I2CObj.state = SERCOM_I2C_REINITIATE_TRANSFER;

        }
        /* Check for Bus Error during transmission */
        else if((SERCOM3_REGS->I2CM.SERCOM_STATUS & SERCOM_I2CM_STATUS_BUSERR_Msk) == SERCOM_I2CM_STATUS_BUSERR_Msk)
        {
            /* Set Error status */
            sercom3I2CObj.state = SERCOM_I2C_STATE_ERROR;
            sercom3I2CObj.error = SERCOM_I2C_ERROR_BUS;
        }
        /* Checks slave acknowledge for address or data*/
        else if((SERCOM3_REGS->I2CM.SERCOM_STATUS & SERCOM_I2CM_STATUS_RXNACK_Msk) == SERCOM_I2CM_STATUS_RXNACK_Msk)
        {
            sercom3I2CObj.state = SERCOM_I2C_STATE_ERROR;
            sercom3I2CObj.error = SERCOM_I2C_ERROR_NAK;
        }
        else
        {
            switch(sercom3I2CObj.state)
            {
                case SERCOM_I2C_REINITIATE_TRANSFER:
                {
                    if (sercom3I2CObj.writeSize != 0)
                    {
                        /* Initiate Write transfer */
                        SERCOM3_I2C_InitiateTransfer(sercom3I2CObj.address, false);
                    }
                    else
                    {
                        /* Initiate Read transfer */
                        SERCOM3_I2C_InitiateTransfer(sercom3I2CObj.address, true);
                    }

                    break;
                }

                case SERCOM_I2C_STATE_IDLE:
                {
                    break;
                }
                case SERCOM_I2C_STATE_TRANSFER_WRITE:
                {
                    if (sercom3I2CObj.writeCount == (sercom3I2CObj.writeSize))
                    {
                        if(sercom3I2CObj.readSize != 0)
                        {
                            SERCOM3_I2C_InitiateRead(sercom3I2CObj.address);
                        }
                        else
                        {
                            SERCOM3_REGS->I2CM.SERCOM_CTRLB |= SERCOM_I2CM_CTRLB_CMD(3);

                            /* Wait for synchronization */
                            while((SERCOM3_REGS->I2CM.SERCOM_STATUS & SERCOM_I2CM_STATUS_SYNCBUSY_Msk) & SERCOM_I2CM_STATUS_SYNCBUSY_Msk);

                            sercom3I2CObj.state = SERCOM_I2C_STATE_TRANSFER_DONE;
                        }
                    }
                    /* Write next byte */
                    else
                    {
                        SERCOM3_REGS->I2CM.SERCOM_DATA = sercom3I2CObj.writeBuffer[sercom3I2CObj.writeCount++];
                    }

                    break;
                }
                case SERCOM_I2C_STATE_TRANSFER_READ:
                {
                    if(sercom3I2CObj.readCount == (sercom3I2CObj.readSize - 1))
                    {
                        /* Set NACK and send stop condition to the slave from master */
                        SERCOM3_REGS->I2CM.SERCOM_CTRLB |= SERCOM_I2CM_CTRLB_ACKACT_Msk | SERCOM_I2CM_CTRLB_CMD(3);

                        /* Wait for synchronization */
                        while((SERCOM3_REGS->I2CM.SERCOM_STATUS & SERCOM_I2CM_STATUS_SYNCBUSY_Msk) & SERCOM_I2CM_STATUS_SYNCBUSY_Msk);

                        sercom3I2CObj.state = SERCOM_I2C_STATE_TRANSFER_DONE;
                    }

                    /* Read the received data */
                    sercom3I2CObj.readBuffer[sercom3I2CObj.readCount++] = SERCOM3_REGS->I2CM.SERCOM_DATA;

                    break;
                }
                default:
                {
                    break;
                }
            }
        }

        /* Error Status */
        if(sercom3I2CObj.state == SERCOM_I2C_STATE_ERROR)
        {
            /* Reset the PLib objects and Interrupts */
            sercom3I2CObj.state = SERCOM_I2C_STATE_IDLE;
            
            /* Generate STOP condition */
            SERCOM3_REGS->I2CM.SERCOM_CTRLB |= SERCOM_I2CM_CTRLB_CMD(3);

            /* Wait for synchronization */
            while((SERCOM3_REGS->I2CM.SERCOM_STATUS & SERCOM_I2CM_STATUS_SYNCBUSY_Msk) & SERCOM_I2CM_STATUS_SYNCBUSY_Msk);

            SERCOM3_REGS->I2CM.SERCOM_INTFLAG = SERCOM_I2CM_INTFLAG_Msk;

            if (sercom3I2CObj.callback != NULL)
            {
                sercom3I2CObj.callback(sercom3I2CObj.context);
            }
        }
        /* Transfer Complete */
        else if(sercom3I2CObj.state == SERCOM_I2C_STATE_TRANSFER_DONE)
        {
            /* Reset the PLib objects and interrupts */
            sercom3I2CObj.state = SERCOM_I2C_STATE_IDLE;
            sercom3I2CObj.error = SERCOM_I2C_ERROR_NONE;

            SERCOM3_REGS->I2CM.SERCOM_INTFLAG = SERCOM_I2CM_INTFLAG_Msk;
            
            /* Wait for the NAK and STOP bit to be transmitted out and I2C state machine to rest in IDLE state */
            while((SERCOM3_REGS->I2CM.SERCOM_STATUS & SERCOM_I2CM_STATUS_BUSSTATE_Msk) != SERCOM_I2CM_STATUS_BUSSTATE(0x01));

            if(sercom3I2CObj.callback != NULL)
            {
                sercom3I2CObj.callback(sercom3I2CObj.context);
            }

        }
    }

    return;
}