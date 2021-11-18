/*******************************************************************************
  I2C DC DC Driver Header File

  Company:
    Microchip Technology Inc.

  File Name:
    i2c_dc_dc_driver.h

  Description:
    This file contains function prototypes for i2c driver 
 *******************************************************************************/

// DOM-IGNORE-BEGIN
/*******************************************************************************
* Copyright (C) [2019-2020] Microchip Technology Inc. and its subsidiaries.
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

#ifndef _I2C_DC_DC_DRIVER_H    /* Guard against multiple inclusion */
#define _I2C_DC_DC_DRIVER_H

/* ************************************************************************** */
/* ************************************************************************** */
/* Section: Included Files                                                    */
/* ************************************************************************** */
/* ************************************************************************** */
#include "i2c_dc_dc_ung8198.h"

/* SERCOM instance for I2C driver */
#define PSF_APP_I2C_INSTANCE     3

/****************************************************************************
    Function:
        PSF_APP_I2CDCDCReadDriver (UINT16 u16Address,UINT8 *pu8ReadBuf,UINT8 u8ReadLen)
    Summary:
        Function for I2C DC DC Controller read drivers.  
    Description:
        This function reads the given length of data from the given address 
    Conditions:
        None
    Input:
        u16Address - I2C address of the DC DC controller
        pu8ReadBuf - Pointer to Read buffer
        u8ReadLen - Length of bytes to read
    Return:
        UINT8 - Returns TRUE - If the I2C read is successful.
                Returns FALSE - If the I2C read is not successful
    Remarks:
        None
**************************************************************************************************/
UINT8 PSF_APP_I2CDCDCReadDriver (UINT16 u16Address,UINT8 *pu8ReadBuf,UINT8 u8ReadLen);

/****************************************************************************
    Function:
        PSF_APP_I2CDCDCWriteDriver(UINT16 u16Address,UINT8 *pu8WriteBuf,UINT8 u8WriteLen)
    Summary:
        Function for I2C DC DC Controller Write drivers.  
    Description:
        This function writes the given length of data to the given address 
    Conditions:
        None
    Input:
        u16Address - I2C address of the DC DC controller
        pu8WriteBuf - Pointer to write buffer
        u8WriteLen - Length of bytes to read
    Return:
        UINT8 - Returns TRUE - If the I2C write is successful.
                Returns FALSE - If the I2C write is not successful
    Remarks:
        None
**************************************************************************************************/
UINT8 PSF_APP_I2CDCDCWriteDriver(UINT16 u16Address,UINT8 *pu8WriteBuf,UINT8 u8WriteLen);

/****************************************************************************
    Function:
        PSF_APP_I2CDCDCWriteReadDriver(UINT16 u16Address,UINT8 *pu8WriteBuf,UINT8 u8WriteLen,\
                                              UINT8 *pu8ReadBuf,UINT8 u8ReadLen)
    Summary:
        Function for I2C DC DC Controller Write and read drivers.  
    Description:
        This function writes the given length of data to the given address and 
        then reads the given length of data
    Conditions:
        None
    Input:
        u16Address - I2C address of the DC DC controller
        pu8WriteBuf - Pointer to write buffer
        u8WriteLen - Length of bytes to read
        pu8ReadBuf - Pointer to Read buffer
        u8ReadLen - Length of bytes to read
    Return:
        UINT8 - Returns TRUE - If the I2C write-read is successful.
                Returns FALSE - If the I2C write-read is not successful
    Remarks:
        None
**************************************************************************************************/
UINT8 PSF_APP_I2CDCDCWriteReadDriver(UINT16 u16Address,UINT8 *pu8WriteBuf,UINT8 u8WriteLen,\
                                              UINT8 *pu8ReadBuf,UINT8 u8ReadLen);
/****************************************************************************
    Function:
        PSF_APP_I2CDCDCIsBusyDriver(void)
    Summary:
        Function to check if the I2C drivers is busy  
    Description:
        This function checks for the busy state of I2C driver
    Conditions:
        None
    Input:
        None
    Return:
        UINT8 - Returns TRUE - If the I2C driver is busy
                Returns FALSE - If the I2C driver is not busy
    Remarks:
        None
**************************************************************************************************/
UINT8 PSF_APP_I2CDCDCIsBusyDriver(void);
/****************************************************************************
    Function:
        void PSF_APP_I2CDCDCAlertCallback(uintptr_t u8PortNum)
    Summary:
        I2C DC DC Alert callback wrapper function.  
    Description:
        This API serves as a wrapper for DC DC alert interrupt handler to 
        register as callback for device's (SAMD20) function EIC_CallbackRegister.
    Conditions:
        None
    Input:
        u8PortNum - Port number. Value passed will be less than CONFIG_PD_PORT_COUNT
    Return:
        None.
    Remarks:
        None
**************************************************************************************************/
void PSF_APP_I2CDCDCAlertCallback(uintptr_t u8PortNum);
/****************************************************************************
    Function:
        UINT8 PSFDCDC_Initialize(uintptr_t u8PortNum) 
    Summary:
        Initializes I2C DC DC Control for the port
    Description:
        This API is called for initializing the I2C DC DC Module for the given port 
        during MchpPSF_Init to initialize Port power control. 
    Conditions:
        None.
    Input:
        u8PortNum - Corresponding Port Number. 
                    Value passed will be less than CONFIG_PD_PORT_COUNT.
    Return:
      None
    Remarks:
        None
**************************************************************************************************/
UINT8 PSFDCDC_Initialize(uintptr_t u8PortNum);
/****************************************************************************
    Function:
        void PSFDCDC_SetPortPower(UINT8 u8PortNum, UINT16 u16VBUSVoltage, UINT16 u16Current)
    Summary:
        API to drive Power on VBUS
    Description:
        This API drives Power on VBUS line based on the input parameters.
    Conditions:
        None.
    Input:
        u8PortNum - Corresponding Port Number. Value passed will be less than CONFIG_PD_PORT_COUNT.
        u16VBUSVoltage - Provides negotiated voltage to drive power
        u16Current - Provides negotiated current to drive power
    Return:
      None
    Remarks:
        None
**************************************************************************************************/
void PSFDCDC_SetPortPower(UINT8 u8PortNum, UINT16 u16Voltage, UINT16 u16Current);
              
#endif /*_I2C_DC_DC_DRIVER_H */

/* *****************************************************************************
 End of File
 */
