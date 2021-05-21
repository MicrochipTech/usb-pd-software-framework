/*******************************************************************************
  I2C DC DC driver Source File

  Company:
    Microchip Technology Inc.

  File Name:
    i2c_dc_dc_driver.c

  Description:
    This file contains the function definitions of I2C driver 
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

/* ************************************************************************** */
/* ************************************************************************** */
/* Section: Included Files                                                    */
/* ************************************************************************** */
/* ************************************************************************** */
#include "i2c_dc_dc_driver.h"
#include "../../firmware/src/config/default/peripheral/sercom/i2cm/plib_sercom3_i2c.h"


/* ************************************************************************** */
/* ************************************************************************** */
/* Section: defines                                                */
/* ************************************************************************** */
/* ************************************************************************** */

#define SAMD20I2CDCDC_Read(n,u16Address,pu8ReadBuf,u8ReadLen) \
                    SERCOMn_I2C_Read(n,u16Address,pu8ReadBuf,u8ReadLen)
#define SERCOMn_I2C_Read(n,u16Address,pu8ReadBuf,u8ReadLen) \
        SERCOM##n##_I2C_Read(u16Address,pu8ReadBuf,u8ReadLen)

#define SAMD20I2CDCDC_Write(n,u16Address,pu8WritedBuf,u8WriteLen) \
                    SERCOMn_I2C_Write(n,u16Address,pu8WritedBuf,u8WriteLen)
#define SERCOMn_I2C_Write(n,u16Address,pu8WritedBuf,u8WriteLen) \
        SERCOM##n##_I2C_Write(u16Address,pu8WritedBuf,u8WriteLen)

#define SAMD20I2CDCDC_WriteRead(n,u16Address,pu8WritedBuf,u8WriteLen,pu8ReadBuf,u8ReadLen) \
        SERCOMn_I2C_WriteRead(n,u16Address,pu8WritedBuf,u8WriteLen,pu8ReadBuf,u8ReadLen)
#define SERCOMn_I2C_WriteRead(n,u16Address,pu8WritedBuf,u8WriteLen,pu8ReadBuf,u8ReadLen) \
        SERCOM##n##_I2C_WriteRead(u16Address,pu8WritedBuf,u8WriteLen,pu8ReadBuf,u8ReadLen)

#define SAMD20_I2cDCDCIsBusy(n) SERCOMn_I2C_IsBusy(n)
#define SERCOMn_I2C_IsBusy(n) SERCOM##n##_I2C_IsBusy()

/*****************************************************************************/
/*****************************************************************************/
/*********************************I2C Driver APIs*****************************/
/*****************************************************************************/
void SAMD20_I2CDCDCAlertCallback(uintptr_t u8PortNum)
{
    /*PSF Alert Handler is called for specific port to service UPD350 Alert interrupt*/
    MPQDCDC_HandleAlert(u8PortNum);
}

UINT8 SAMD20_I2CDCDCReadDriver (UINT16 u16Address,UINT8 *pu8ReadBuf,UINT8 u8ReadLen)
{
    UINT8 u8Return = FALSE;
    
    u8Return = SAMD20I2CDCDC_Read (SAMD20_I2C_INSTANCE,u16Address,pu8ReadBuf,u8ReadLen);
    return u8Return;
}

UINT8 SAMD20_I2CDCDCWriteDriver(UINT16 u16Address,UINT8 *pu8WriteBuf,UINT8 u8WriteLen)
{
    UINT8 u8Return = FALSE;
    
    u8Return = SAMD20I2CDCDC_Write (SAMD20_I2C_INSTANCE,u16Address,pu8WriteBuf,u8WriteLen);
    return u8Return;
   
}

UINT8 SAMD20_I2CDCDCWriteReadDriver(UINT16 u16Address,UINT8 *pu8WriteBuf,UINT8 u8WriteLen,\
                                              UINT8 *pu8ReadBuf,UINT8 u8ReadLen)
{
    UINT8 u8Return = FALSE;
    
    u8Return = SAMD20I2CDCDC_WriteRead (SAMD20_I2C_INSTANCE,u16Address,pu8WriteBuf,u8WriteLen,\
                                                            pu8ReadBuf,u8ReadLen);
    return u8Return;
}

UINT8 SAMD20_I2CDCDCIsBusyDriver(void)
{
    return SAMD20_I2cDCDCIsBusy(SAMD20_I2C_INSTANCE);
}

/* *****************************************************************************
 End of File
 */
