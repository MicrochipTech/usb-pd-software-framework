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

#include "interrupts.h"
#include "plib_sercom3_i2c_master.h"


// *****************************************************************************
// *****************************************************************************
// Section: Global Data
// *****************************************************************************
// *****************************************************************************


#define SERCOM3_I2CM_SPEED_HZ           400000

/* SERCOM3 I2C baud value */
#define SERCOM3_I2CM_BAUD_VALUE         (0x34U)


static SERCOM_I2C_OBJ sercom3I2CObj;

// *****************************************************************************
// *****************************************************************************
// Section: SERCOM3 I2C Implementation
// *****************************************************************************
// *****************************************************************************
// *****************************************************************************

void SERCOM3_I2C_Initialize(void)
{
    /* Reset the module */
    SERCOM3_REGS->I2CM.SERCOM_CTRLA = SERCOM_I2CM_CTRLA_SWRST_Msk ;

    /* Wait for synchronization */
    while((SERCOM3_REGS->I2CM.SERCOM_STATUS & (uint16_t)SERCOM_I2CM_STATUS_SYNCBUSY_Msk) == (uint16_t)SERCOM_I2CM_STATUS_SYNCBUSY_Msk)
    {
        /* Do nothing */
    }

    /* Enable smart mode */
    SERCOM3_REGS->I2CM.SERCOM_CTRLB = SERCOM_I2CM_CTRLB_SMEN_Msk;

    /* Wait for synchronization */
    while((SERCOM3_REGS->I2CM.SERCOM_STATUS & (uint16_t)SERCOM_I2CM_STATUS_SYNCBUSY_Msk) == (uint16_t)SERCOM_I2CM_STATUS_SYNCBUSY_Msk)
    {
        /* Do nothing */
    }

    /* Baud rate - Master Baud Rate*/
    SERCOM3_REGS->I2CM.SERCOM_BAUD = SERCOM3_I2CM_BAUD_VALUE;

    /* Set Operation Mode (Master), SDA Hold time, run in stand by and i2c master enable */
    SERCOM3_REGS->I2CM.SERCOM_CTRLA = SERCOM_I2CM_CTRLA_MODE_I2C_MASTER | SERCOM_I2CM_CTRLA_SDAHOLD_75NS | SERCOM_I2CM_CTRLA_ENABLE_Msk ;

    /* Wait for synchronization */
    while((SERCOM3_REGS->I2CM.SERCOM_STATUS & (uint16_t)SERCOM_I2CM_STATUS_SYNCBUSY_Msk) == (uint16_t)SERCOM_I2CM_STATUS_SYNCBUSY_Msk)
    {
        /* Do nothing */
    }

    /* Initial Bus State: IDLE */
    SERCOM3_REGS->I2CM.SERCOM_STATUS = (uint16_t)SERCOM_I2CM_STATUS_BUSSTATE(0x01UL);

    /* Wait for synchronization */
    while((SERCOM3_REGS->I2CM.SERCOM_STATUS & (uint16_t)SERCOM_I2CM_STATUS_SYNCBUSY_Msk) == (uint16_t)SERCOM_I2CM_STATUS_SYNCBUSY_Msk)
    {
        /* Do nothing */
    }

    /* Initialize the SERCOM3 PLib Object */
    sercom3I2CObj.error = SERCOM_I2C_ERROR_NONE;
    sercom3I2CObj.state = SERCOM_I2C_STATE_IDLE;

    /* Enable all Interrupts */
    SERCOM3_REGS->I2CM.SERCOM_INTENSET = (uint8_t)SERCOM_I2CM_INTENSET_Msk;
}

static bool SERCOM3_I2C_CalculateBaudValue(uint32_t srcClkFreq, uint32_t i2cClkSpeed, uint32_t* baudVal)
{
    uint32_t baudValue = 0U;
    float fSrcClkFreq = (float)srcClkFreq;
    float fI2cClkSpeed = (float)i2cClkSpeed;
    float fBaudValue = 0.0f;

    /* Reference clock frequency must be atleast two times the baud rate */
    if (srcClkFreq < (2U * i2cClkSpeed))
    {
        return false;
    }

    if (i2cClkSpeed <= 400000U)
    {
        /* Standard, FM and FM+ baud calculation */
        fBaudValue = (fSrcClkFreq / fI2cClkSpeed) - ((fSrcClkFreq * (100.0f / 1000000000.0f)) + 10.0f);
        baudValue = (uint32_t)fBaudValue;

        /* For I2C clock speed upto 400 KHz, the value of BAUD<7:0> determines both SCL_L and SCL_H with SCL_L = SCL_H */
        if (baudValue > (0xFFU * 2U))
        {
            /* Set baud rate to the minimum possible value */
            baudValue = 0xFFU;
        }
        else if (baudValue <= 1U)
        {
            /* Baud value cannot be 0. Set baud rate to maximum possible value */
            baudValue = 1U;
        }
        else
        {
            baudValue /= 2U;
        }
    }
    else
    {
        return false;
    }
    *baudVal = baudValue;
    return true;
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

    if( srcClkFreq == 0U)
    {
        srcClkFreq = 48000000UL;
    }

    if (SERCOM3_I2C_CalculateBaudValue(srcClkFreq, i2cClkSpeed, &baudValue) == false)
    {
        return false;
    }


    /* Disable the I2C before changing the I2C clock speed */
    SERCOM3_REGS->I2CM.SERCOM_CTRLA &= ~SERCOM_I2CM_CTRLA_ENABLE_Msk;

    /* Wait for synchronization */
    while((SERCOM3_REGS->I2CM.SERCOM_STATUS & (uint16_t)SERCOM_I2CM_STATUS_SYNCBUSY_Msk) == (uint16_t)SERCOM_I2CM_STATUS_SYNCBUSY_Msk)
    {
        /* Do nothing */
    }


    /* Baud rate - Master Baud Rate*/
    SERCOM3_REGS->I2CM.SERCOM_BAUD = baudValue;


    /* Re-enable the I2C module */
    SERCOM3_REGS->I2CM.SERCOM_CTRLA |= SERCOM_I2CM_CTRLA_ENABLE_Msk;

    /* Wait for synchronization */
    while((SERCOM3_REGS->I2CM.SERCOM_STATUS & (uint16_t)SERCOM_I2CM_STATUS_SYNCBUSY_Msk) == (uint16_t)SERCOM_I2CM_STATUS_SYNCBUSY_Msk)
    {
        /* Do nothing */
    }


    /* Since the I2C module was disabled, re-initialize the bus state to IDLE */
    SERCOM3_REGS->I2CM.SERCOM_STATUS = (uint16_t)SERCOM_I2CM_STATUS_BUSSTATE(0x01UL);

    /* Wait for synchronization */
    while((SERCOM3_REGS->I2CM.SERCOM_STATUS & (uint16_t)SERCOM_I2CM_STATUS_SYNCBUSY_Msk) == (uint16_t)SERCOM_I2CM_STATUS_SYNCBUSY_Msk)
    {
        /* Do nothing */
    }

    return true;
}

void SERCOM3_I2C_CallbackRegister(SERCOM_I2C_CALLBACK callback, uintptr_t contextHandle)
{
    sercom3I2CObj.callback = callback;

    sercom3I2CObj.context  = contextHandle;
}


static void SERCOM3_I2C_SendAddress(uint16_t address, bool dir)
{
    /* If operation is I2C read */
    if(dir)
    {
        /* <xxxx-xxxR> <read-data> <P> */

        /* Next state will be to read data */
        sercom3I2CObj.state = SERCOM_I2C_STATE_TRANSFER_READ;
    }
    else
    {
        /* <xxxx-xxxW> <write-data> <P> */

        /* Next state will be to write data */
        sercom3I2CObj.state = SERCOM_I2C_STATE_TRANSFER_WRITE;
    }


    SERCOM3_REGS->I2CM.SERCOM_ADDR = ((uint32_t)address << 1U) | (dir ? 1UL :0UL);

    /* Wait for synchronization */
    while((SERCOM3_REGS->I2CM.SERCOM_STATUS & (uint16_t)SERCOM_I2CM_STATUS_SYNCBUSY_Msk) == (uint16_t)SERCOM_I2CM_STATUS_SYNCBUSY_Msk)
    {
        /* Do nothing */
    }

}

static void SERCOM3_I2C_InitiateTransfer(uint16_t address, bool dir)
{
    sercom3I2CObj.writeCount = 0U;
    sercom3I2CObj.readCount = 0U;

    /* Clear all flags */
    SERCOM3_REGS->I2CM.SERCOM_INTFLAG = (uint8_t)SERCOM_I2CM_INTFLAG_Msk;

    /* Smart mode enabled with SCLSM = 0, - ACK is set to send while receiving the data */
    SERCOM3_REGS->I2CM.SERCOM_CTRLB &= ~SERCOM_I2CM_CTRLB_ACKACT_Msk;

    /* Wait for synchronization */
    while((SERCOM3_REGS->I2CM.SERCOM_STATUS & (uint16_t)SERCOM_I2CM_STATUS_SYNCBUSY_Msk) == (uint16_t)SERCOM_I2CM_STATUS_SYNCBUSY_Msk)
    {
        /* Do nothing */
    }


    SERCOM3_I2C_SendAddress(address, dir);
}

static bool SERCOM3_I2C_XferSetup(
    uint16_t address,
    uint8_t* wrData,
    uint32_t wrLength,
    uint8_t* rdData,
    uint32_t rdLength,
    bool dir,
    bool isHighSpeed
)
{
    /* Check for ongoing transfer */
    if(sercom3I2CObj.state != SERCOM_I2C_STATE_IDLE)
    {
        return false;
    }

    sercom3I2CObj.address        = address;
    sercom3I2CObj.readBuffer     = rdData;
    sercom3I2CObj.readSize       = rdLength;
    sercom3I2CObj.writeBuffer    = wrData;
    sercom3I2CObj.writeSize      = wrLength;
    sercom3I2CObj.transferDir    = dir;
    sercom3I2CObj.isHighSpeed    = isHighSpeed;
    sercom3I2CObj.error          = SERCOM_I2C_ERROR_NONE;


    SERCOM3_I2C_InitiateTransfer(address, dir);

    return true;
}

bool SERCOM3_I2C_Read(uint16_t address, uint8_t* rdData, uint32_t rdLength)
{
    return SERCOM3_I2C_XferSetup(address, NULL, 0, rdData, rdLength, true, false);
}

bool SERCOM3_I2C_Write(uint16_t address, uint8_t* wrData, uint32_t wrLength)
{
    return SERCOM3_I2C_XferSetup(address, wrData, wrLength, NULL, 0, false, false);
}

bool SERCOM3_I2C_WriteRead(uint16_t address, uint8_t* wrData, uint32_t wrLength, uint8_t* rdData, uint32_t rdLength)
{
    return SERCOM3_I2C_XferSetup(address, wrData, wrLength, rdData, rdLength, false, false);
}


bool SERCOM3_I2C_IsBusy(void)
{
    bool isBusy = true;
    if((sercom3I2CObj.state == SERCOM_I2C_STATE_IDLE))
    {
        if(((SERCOM3_REGS->I2CM.SERCOM_STATUS & SERCOM_I2CM_STATUS_BUSSTATE_Msk) == SERCOM_I2CM_STATUS_BUSSTATE(0x01UL)))
        {
           isBusy = false;
        }
    }
    return isBusy;
}

SERCOM_I2C_ERROR SERCOM3_I2C_ErrorGet(void)
{
    return sercom3I2CObj.error;
}

void SERCOM3_I2C_InterruptHandler(void)
{
    if(SERCOM3_REGS->I2CM.SERCOM_INTENSET != 0U)
    {
        /* Checks if the arbitration lost in multi-master scenario */
        if((SERCOM3_REGS->I2CM.SERCOM_STATUS & SERCOM_I2CM_STATUS_ARBLOST_Msk) == SERCOM_I2CM_STATUS_ARBLOST_Msk)
        {
            /* Set Error status */
            sercom3I2CObj.state = SERCOM_I2C_STATE_ERROR;
            sercom3I2CObj.error = SERCOM_I2C_ERROR_BUS;

        }
        /* Check for Bus Error during transmission */
        else if((SERCOM3_REGS->I2CM.SERCOM_STATUS & SERCOM_I2CM_STATUS_BUSERR_Msk) == SERCOM_I2CM_STATUS_BUSERR_Msk)
        {
            /* Set Error status */
            sercom3I2CObj.state = SERCOM_I2C_STATE_ERROR;
            sercom3I2CObj.error = SERCOM_I2C_ERROR_BUS;
        }
        /* Checks slave acknowledge for address or data */
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

                    if (sercom3I2CObj.writeSize != 0U)
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


                case SERCOM_I2C_STATE_IDLE:

                    break;



                case SERCOM_I2C_STATE_TRANSFER_WRITE:

                    if (sercom3I2CObj.writeCount == (sercom3I2CObj.writeSize))
                    {
                        if(sercom3I2CObj.readSize != 0U)
                        {

                            /* Write 7bit address with direction (ADDR.ADDR[0]) equal to 1*/
                            SERCOM3_REGS->I2CM.SERCOM_ADDR =  ((uint32_t)(sercom3I2CObj.address) << 1U) | (uint32_t)I2C_TRANSFER_READ;

                            /* Wait for synchronization */
                            while((SERCOM3_REGS->I2CM.SERCOM_STATUS & (uint16_t)SERCOM_I2CM_STATUS_SYNCBUSY_Msk) == (uint16_t)SERCOM_I2CM_STATUS_SYNCBUSY_Msk)
                            {
                                /* Do nothing */
                            }

                            sercom3I2CObj.state = SERCOM_I2C_STATE_TRANSFER_READ;

                        }
                        else
                        {
                            SERCOM3_REGS->I2CM.SERCOM_CTRLB |= SERCOM_I2CM_CTRLB_CMD(3UL);

                            /* Wait for synchronization */
                            while((SERCOM3_REGS->I2CM.SERCOM_STATUS & (uint16_t)SERCOM_I2CM_STATUS_SYNCBUSY_Msk) == (uint16_t)SERCOM_I2CM_STATUS_SYNCBUSY_Msk)
                            {
                                /* Do nothing */
                            }


                            sercom3I2CObj.state = SERCOM_I2C_STATE_TRANSFER_DONE;
                        }
                    }
                    /* Write next byte */
                    else
                    {
                        SERCOM3_REGS->I2CM.SERCOM_DATA = sercom3I2CObj.writeBuffer[sercom3I2CObj.writeCount++];

                        /* Wait for synchronization */
                            while((SERCOM3_REGS->I2CM.SERCOM_STATUS & (uint16_t)SERCOM_I2CM_STATUS_SYNCBUSY_Msk) == (uint16_t)SERCOM_I2CM_STATUS_SYNCBUSY_Msk)
                            {
                                /* Do nothing */
                            }
                    }

                    break;

                case SERCOM_I2C_STATE_TRANSFER_READ:

                    if(sercom3I2CObj.readCount == (sercom3I2CObj.readSize - 1U))
                    {
                        /* Set NACK and send stop condition to the slave from master */
                        SERCOM3_REGS->I2CM.SERCOM_CTRLB |= SERCOM_I2CM_CTRLB_ACKACT_Msk | SERCOM_I2CM_CTRLB_CMD(3UL);

                        /* Wait for synchronization */
                        while((SERCOM3_REGS->I2CM.SERCOM_STATUS & (uint16_t)SERCOM_I2CM_STATUS_SYNCBUSY_Msk) == (uint16_t)SERCOM_I2CM_STATUS_SYNCBUSY_Msk)
                        {
                            /* Do nothing */
                        }

                        sercom3I2CObj.state = SERCOM_I2C_STATE_TRANSFER_DONE;
                    }

                    /* Wait for synchronization */
                        while((SERCOM3_REGS->I2CM.SERCOM_STATUS & (uint16_t)SERCOM_I2CM_STATUS_SYNCBUSY_Msk) == (uint16_t)SERCOM_I2CM_STATUS_SYNCBUSY_Msk)
                        {
                            /* Do nothing */
                        }

                    /* Read the received data */
                    sercom3I2CObj.readBuffer[sercom3I2CObj.readCount++] = SERCOM3_REGS->I2CM.SERCOM_DATA;


                    break;

                default:

                    /* Do nothing */
                    break;
            }
        }

        /* Error Status */
        if(sercom3I2CObj.state == SERCOM_I2C_STATE_ERROR)
        {
            /* Reset the PLib objects and Interrupts */
            sercom3I2CObj.state = SERCOM_I2C_STATE_IDLE;

            /* Generate STOP condition */
            SERCOM3_REGS->I2CM.SERCOM_CTRLB |= SERCOM_I2CM_CTRLB_CMD(3UL);

            /* Wait for synchronization */
            while((SERCOM3_REGS->I2CM.SERCOM_STATUS & (uint16_t)SERCOM_I2CM_STATUS_SYNCBUSY_Msk) == (uint16_t)SERCOM_I2CM_STATUS_SYNCBUSY_Msk)
            {
                /* Do nothing */
            }


            SERCOM3_REGS->I2CM.SERCOM_INTFLAG = (uint8_t)SERCOM_I2CM_INTFLAG_Msk;

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

            SERCOM3_REGS->I2CM.SERCOM_INTFLAG = (uint8_t)SERCOM_I2CM_INTFLAG_Msk;

            /* Wait for the NAK and STOP bit to be transmitted out and I2C state machine to rest in IDLE state */
            while((SERCOM3_REGS->I2CM.SERCOM_STATUS & SERCOM_I2CM_STATUS_BUSSTATE_Msk) != SERCOM_I2CM_STATUS_BUSSTATE(0x01UL))
            {
                /* Do nothing */
            }

            if(sercom3I2CObj.callback != NULL)
            {
                sercom3I2CObj.callback(sercom3I2CObj.context);
            }

        }
        else
        {
            /* Do nothing */
        }
    }

    return;
}