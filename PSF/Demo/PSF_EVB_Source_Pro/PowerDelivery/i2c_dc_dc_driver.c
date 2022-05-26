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
// DOM-IGNORE-END

/* ************************************************************************** */
/* ************************************************************************** */
/* Section: Included Files                                                    */
/* ************************************************************************** */
/* ************************************************************************** */
#include "i2c_dc_dc_driver.h"


/* ************************************************************************** */
/* ************************************************************************** */
/* Section: File Scope or Global Data                                         */
/* ************************************************************************** */
/* ************************************************************************** */

DRV_I2C_DATA I2CData;

/* ************************************************************************** */
/* ************************************************************************** */
/* Section: defines                                                */
/* ************************************************************************** */
/* ************************************************************************** */

#define PSF_APP_I2CDCDC_Read(n,u16Address,pu8ReadBuf,u8ReadLen) \
                    DRV_I2C_ReadTransferAdd(I2CData.i2cHandle,u16Address,pu8ReadBuf,u8ReadLen,&I2CData.transferHandle)

#define PSF_APP_I2CDCDC_Write(n,u16Address,pu8WritedBuf,u8WriteLen) \
                    DRV_I2C_WriteTransferAdd(I2CData.i2cHandle,u16Address,pu8WriteBuf,u8WriteLen,&I2CData.transferHandle)


#define PSF_APP_I2CDCDC_WriteRead(n,u16Address,pu8WritedBuf,u8WriteLen,pu8ReadBuf,u8ReadLen) \
        DRV_I2C_WriteReadTransferAdd(I2CData.i2cHandle,u16Address,pu8WritedBuf,u8WriteLen,pu8ReadBuf,u8ReadLen,&I2CData.transferHandle)


/*****************************************************************************/
/*****************************************************************************/
/*********************************I2C Driver APIs*****************************/
/*****************************************************************************/
void PSF_APP_I2CDCDCAlertCallback(uintptr_t u8PortNum)
{
    /*PSF Alert Handler is called for specific port to service UPD350 Alert interrupt*/
    MPQDCDC_HandleAlert(u8PortNum);
}

UINT8 PSF_APP_I2CDCDCReadDriver (UINT16 u16Address,UINT8 *pu8ReadBuf,UINT8 u8ReadLen)
{
    while(I2CData.isTransferDone != true)
    {
    }
    I2CData.isTransferDone = false;
    PSF_APP_I2CDCDC_Read (PSF_APP_I2C_INSTANCE,u16Address,pu8ReadBuf,u8ReadLen);
    while(I2CData.isTransferDone != true)
    {
        /* wait for the current transfer to complete */
    }
    return true;
}

UINT8 PSF_APP_I2CDCDCWriteDriver(UINT16 u16Address,UINT8 *pu8WriteBuf,UINT8 u8WriteLen)
{
    while(I2CData.isTransferDone != true)
    {
    }
    I2CData.isTransferDone = false;
    PSF_APP_I2CDCDC_Write (PSF_APP_I2C_INSTANCE,u16Address,pu8WriteBuf,u8WriteLen);
    while(I2CData.isTransferDone != true)
    {
        /* wait for the current transfer to complete */
    }
    return true;
   
}

UINT8 PSF_APP_I2CDCDCWriteReadDriver(UINT16 u16Address,UINT8 *pu8WriteBuf,UINT8 u8WriteLen,\
                                              UINT8 *pu8ReadBuf,UINT8 u8ReadLen)
{
    while(I2CData.isTransferDone != true)
    {
    }
    I2CData.isTransferDone = false;
    PSF_APP_I2CDCDC_WriteRead (PSF_APP_I2C_INSTANCE,u16Address,pu8WriteBuf,u8WriteLen,\
                                                            pu8ReadBuf,u8ReadLen);
    while(I2CData.isTransferDone != true)
    {
        /* wait for the current transfer to complete */
    }
    return true;
}

UINT8 PSFDCDC_Initialize(uintptr_t u8PortNum)    
{
    UINT8 u8Return = FALSE;
    
    u8Return = MPQDCDC_Initialize(u8PortNum);
            
    return u8Return;
}

void PSFDCDC_SetPortPower(UINT8 u8PortNum, UINT16 u16Voltage, UINT16 u16Current)
{
    
    MPQDCDC_SetPortPower(u8PortNum, u16Voltage, u16Current);
    
}

static void Drv_I2CEventHandler(
    DRV_I2C_TRANSFER_EVENT event,
    DRV_I2C_TRANSFER_HANDLE transferHandle,
    uintptr_t context
)
{
    if(event == DRV_I2C_TRANSFER_EVENT_COMPLETE)
    {
        I2CData.isTransferDone = true;
    }
    
}

void PSF_APP_I2C_Drv_Initialize ( void )
{
    I2CData.i2cHandle      = DRV_HANDLE_INVALID;
    I2CData.isTransferDone = true;
    I2CData.transferHandle = DRV_I2C_TRANSFER_HANDLE_INVALID;
    I2CData.i2cHandle = DRV_I2C_Open(DRV_I2C_INDEX_0,DRV_IO_INTENT_READWRITE);
    if(I2CData.i2cHandle != DRV_HANDLE_INVALID)
    {
        /* Register the I2C Driver client event callback */
        DRV_I2C_TransferEventHandlerSet(I2CData.i2cHandle, Drv_I2CEventHandler, 0);     
    }
}
/* *****************************************************************************
 End of File
 */
