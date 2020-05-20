/*******************************************************************************
  SAMD20 HAL Drivers porting Source File

  Company:
    Microchip Technology Inc.

  File Name:
    Drivers.c

  Description:
    This file contains provides the HAL drivers and interfaces
     required from SAMD20E16 for PSF Porting and integration. 
 *******************************************************************************/

// DOM-IGNORE-BEGIN
/*******************************************************************************
* Copyright (C) 2019-2020 Microchip Technology Inc. and its subsidiaries.
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
#include "Drivers.h"

/* ************************************************************************** */
/* ************************************************************************** */
/* Section: File Scope or Global Data                                         */
/* ************************************************************************** */
/* ************************************************************************** */

static UINT32 gu32CriticalSectionCnt = SET_TO_ZERO;

/* ************************************************************************** */
/* ************************************************************************** */
/* Section: defines                                                */
/* ************************************************************************** */
/* ************************************************************************** */

/*Timer drivers - Calls TCn_TimerInitialize to initialize the timer,
                   calls TCn_TimerStart to start and run the timer and
                    registers PSF stack's timer callback through function SAMD20_HWTimerCallback
                    in TCn_TimerCallbackRegiseter. n is the TC instance*/
#define SAMD20Timer_Initialise(n)    TCn_TimerInitialize(n)
#define TCn_TimerInitialize(n)   {TC##n##_TimerInitialize();\
                                    TC##n##_TimerStart(); \
                                    TC##n##_TimerCallbackRegister(SAMD20_HWTimerCallback, (uintptr_t)NULL);}


/*SPI drivers - SERCOMn_SPI_Initialize is called to initialize the SERCOM SPI module. 
                    n is the SERCOM instance*/
#define SAMD20SPI_Initialise(n)     SERCOMn_SPI_Initialize(n)
#define SERCOMn_SPI_Initialize(n)   SERCOM##n##_SPI_Initialize()

/*SPI drivers - SERCOMn_SPI_WriteRead is called for SPI read write operation. n is the SERCOM instance*/
#define SAMD20SPI_WriteRead(n,pTransmitData,txSize,pReceiveData,rxSize)\
                    SERCOMn_SPI_WriteRead(n,pTransmitData,txSize,pReceiveData,rxSize)
#define SERCOMn_SPI_WriteRead(n,pTransmitData,txSize,pReceiveData,rxSize)\
 SERCOM##n##_SPI_WriteRead(pTransmitData,txSize,pReceiveData,rxSize)

#if (TRUE == CONFIG_HOOK_DEBUG_MSG)
    /*Debug UART drivers*/
    #define SAMD20UART_Init(n) SERCOMn_UART_Initialise(n)
    #define SERCOMn_UART_Initialise(n) SERCOM##n##_USART_Initialize()

    #define SAMD20UART_Write_Char(n, byData) SERCOMn_UART_Write_Char(n, byData)
    #define SERCOMn_UART_Write_Char(n, byData) SERCOM##n##_USART_Write_Char(byData)

    #define SAMD20UART_Write_Int(n, dwWriteInt, byWidth) SERCOMn_UART_Write_Int(n, dwWriteInt, byWidth)
    #define SERCOMn_UART_Write_Int(n, dwWriteInt, byWidth) SERCOM##n##_USART_Write_Int(dwWriteInt, byWidth)

    #define SAMD20UART_Write_String(n, pbyMessage) SERCOMn_UART_Write_String(n, pbyMessage)
    #define SERCOMn_UART_Write_String(n, pbyMessage) SERCOM##n##_USART_Write_String(pbyMessage)

#endif //CONFIG_HOOK_DEBUG_MSG

#if (CONFIG_DCDC_CTRL == PWRCTRL_I2C_DC_DC)
#define SAMD20I2CDCDC_Initialise(n) SERCOMn_I2CDCDC_Initialize(n)
#define SERCOMn_I2CDCDC_Initialize(n) SERCOM##n##_I2C_Initialize()

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

#endif //#if (CONFIG_DCDC_CTRL == PWRCTRL_I2C_DC_DC)

/* ************************************************************************** */
/* ************************************************************************** */
// Section: Internal Functions                                               */
/* ************************************************************************** */
static void SAMD20_HWTimerCallback(TC_TIMER_STATUS status, uintptr_t context)
{
    if (TC_TIMER_STATUS_OVERFLOW == (TC_TIMER_STATUS)((UINT8)status & (UINT8)TC_TIMER_STATUS_OVERFLOW))
    {
        /*PSF Timer Callback is called for Timer ISR*/
        MchpPSF_PDTimerHandler();
    }
}
static void SAMD20_UPD350AlertCallback(uintptr_t u8PortNum)
{
    /*PSF Alert Handler is called for specific port to service UPD350 Alert interrupt*/
    MchpPSF_UPDIrqHandler(u8PortNum);
}

/* ************************************************************************** */
/* ************************************************************************** */
// Section: Interface Functions                                               */
/* ************************************************************************** */
/*******************************Timer driver APIs************************************/
UINT8 SAMD20_HWTimerInit(void)
{
    /*Timer is initialized and started to run for 1ms continously*/
    SAMD20Timer_Initialise(SAMD20_TIMER_INSTANCE);
    return TRUE;
}
/*****************************************************************************/
/*****************************************************************************/
/*********************************SPI Driver APIs*****************************/
/*****************************************************************************/
UINT8 SAMD20_SPIInitialisation(void)
{
    SAMD20SPI_Initialise(SAMD20_SPI_INSTANCE);
    return TRUE;
}
UINT8 SAMD20_SPIReaddriver (UINT8 u8PortNum, UINT8 *pu8WriteBuffer, UINT8 u8Writelength, UINT8 *pu8ReadBuffer, UINT8 u8Readlength)
{
    UINT8 u8Return = SAMD20SPI_WriteRead(SAMD20_SPI_INSTANCE, pu8WriteBuffer, u8Writelength, NULL, SET_TO_ZERO);
    u8Return &= SAMD20SPI_WriteRead(SAMD20_SPI_INSTANCE, NULL, SET_TO_ZERO, pu8ReadBuffer, u8Readlength);
    return u8Return;
}
UINT8 SAMD20_SPIWritedriver (UINT8 u8PortNum, UINT8 *pu8WriteBuffer, UINT8 u8Writelength)
{
    return SAMD20SPI_WriteRead(SAMD20_SPI_INSTANCE, pu8WriteBuffer, u8Writelength, NULL, SET_TO_ZERO);
}
void SAMD20_DriveChipSelect(UINT8 u8PortNum, UINT8 u8EnableComm)
{
    if (TRUE == u8EnableComm)
    {
        /* Drive low the CS to enable the communication*/
        if (PORT0 == u8PortNum)
        {
            /*PORT_PIN_PA10*/
            SPI_SS_0_Clear();
        }
        #if (TRUE == INCLUDE_PD_SOURCE) 
        else if(PORT1 == u8PortNum)
        {
            /*PORT_PIN_PA01*/
            SPI_SS_1_Clear();
        } 
        #endif
    }
    else
    {
        /* Drive high the CS to disable the communication for the port*/
        if (PORT0 == u8PortNum)
        {
            SPI_SS_0_Set();
        }
        #if (TRUE == INCLUDE_PD_SOURCE) 
        else if(PORT1 == u8PortNum)
        {
            SPI_SS_1_Set();
        }
        #endif
    }
}
#if (CONFIG_DCDC_CTRL == PWRCTRL_I2C_DC_DC)
/*****************************************************************************/
/*****************************************************************************/
/*********************************I2C Driver APIs*****************************/
/*****************************************************************************/
void SAMD20_I2CDCDCAlertCallback(uintptr_t u8PortNum)
{
    /*PSF Alert Handler is called for specific port to service UPD350 Alert interrupt*/
    MPQDCDC_HandleAlertISR(u8PortNum);
}

void SAMD20_I2CDCDCAlertInit(UINT8 u8PortNum)
{
    /* MCU PIO routed to I2C_DCDC_alert line configured for low level and internal pullup*/
    if (PORT0 == u8PortNum)
    {
        PORT_PinInputEnable(SAMD20_DCDC_ALERT0);
        PORT_PinWrite(SAMD20_DCDC_ALERT0, TRUE);
        EIC_CallbackRegister((EIC_PIN)SAMD20_DCDC_ALERT0, SAMD20_I2CDCDCAlertCallback, PORT0);
        EIC_InterruptEnable((EIC_PIN)SAMD20_DCDC_ALERT0);
    }
    else if (PORT1 == u8PortNum)
    {
        PORT_PinInputEnable(SAMD20_DCDC_ALERT1);
        PORT_PinWrite(SAMD20_DCDC_ALERT1, TRUE);
        EIC_CallbackRegister((EIC_PIN)SAMD20_DCDC_ALERT1, SAMD20_I2CDCDCAlertCallback, PORT1);
        EIC_InterruptEnable((EIC_PIN)SAMD20_DCDC_ALERT1);
    }
}

UINT8 SAMD20_I2CDCDCInitialisation (void)
{
    SAMD20I2CDCDC_Initialise(SAMD20_I2C_INSTANCE);
    return TRUE;

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

bool SAMD20_I2CDCDCIsBusyDriver(void)
{
    return SAMD20_I2cDCDCIsBusy(SAMD20_I2C_INSTANCE);
}
#endif
/*****************************************************************************/
/*****************************************************************************/
/*******************************UPD350 Alert APIs******************************************/
/*****************************************************************************/
void SAMD20_UPD350AlertInit(UINT8 u8PortNum)
{
    /* MCU PIO routed to UPD350 line configured for low level and internal pullup*/
    if (PORT0 == u8PortNum)
    {
        PORT_PinInputEnable((PORT_PIN)SAMD20_PORT0_EIC_PIN);
        PORT_PinWrite((PORT_PIN)SAMD20_PORT0_EIC_PIN, TRUE);
        EIC_CallbackRegister((EIC_PIN)SAMD20_PORT0_EIC_PIN, SAMD20_UPD350AlertCallback, PORT0);
        EIC_InterruptEnable((EIC_PIN)SAMD20_PORT0_EIC_PIN);
    }
    else if (PORT1 == u8PortNum)
    {
        PORT_PinInputEnable((PORT_PIN)SAMD20_PORT1_EIC_PIN);
        PORT_PinWrite((PORT_PIN)SAMD20_PORT1_EIC_PIN, TRUE);
        EIC_CallbackRegister((EIC_PIN)SAMD20_PORT1_EIC_PIN, SAMD20_UPD350AlertCallback, PORT1);
        EIC_InterruptEnable((EIC_PIN)SAMD20_PORT1_EIC_PIN);
    }
}

/*****************************************************************************/

/*****************************************************************************/
/*********************************UPD350 Reset APIs*****************************/
/*****************************************************************************/
void SAMD20_UPD350ResetGPIOInit(UINT8 u8PortNum)
{
    /*Current implementation supports common reset pin for all ports
      Hence, parameter u8PortNum is not considered*/
    
    /*PORT_PIN_PA00*/
    /* UPD350 RESET_N pin active low; set to internal pull up by default*/
     UPD350_RESET_InputEnable();
     PORT_PinWrite(UPD350_RESET_PIN, TRUE);
}

void SAMD20_ResetUPD350(UINT8 u8PortNum)
{
    /*UPD350 Reset function is called for each port respectively
     Since, all UPD350 PIOs are tied to single PIO, Reset is done for PORT0
     alone to avoid multiple reset */
    if (PORT0 == u8PortNum)
    {
        UPD350_RESET_InputEnable();

        /* Pull down is driven to reset the UPD350*/
        PORT_PinWrite(UPD350_RESET_PIN, FALSE);

        /*Delay */
        for(UINT16 u16delayloop = 0u; u16delayloop <(6000);u16delayloop++)
        {
            __asm volatile("nop");
            __asm volatile("nop");

        }
        /* Set to default state*/
        PORT_PinWrite(UPD350_RESET_PIN, TRUE);
    }
}
/*****************************************************************************/
/*****************************************************************************/
/*********************************MCU APIs*****************************/
/*****************************************************************************/
void SAMD20_EnterCriticalSection()
{
    if (gu32CriticalSectionCnt++ == 0)  
    {
        /*Interrupt is disabled if Critical section is not entered already*/
        __disable_irq();
		
    }
}
void SAMD20_ExitCriticalSection()
{
    
    if (--gu32CriticalSectionCnt == 0)
    {
        /*Interrupt is Enabled when there is no pending critical section nesting*/
		__enable_irq();
    }
}
void* SAMD20_MemCpy(void *pdest, const void *psrc, int ilen)
{
   // Typecast src and dest addresses to (char *)
   char *csrc = (char *)psrc;
   char *cdest = (char *)pdest;
 
   // Copy contents of src[] to dest[]
   for (int i=SET_TO_ZERO; i<ilen; i++)
       cdest[i] = csrc[i];
   
   return pdest;
}

int SAMD20_MemCmp(const void *pau8Data1, const void *pau8Data2, int ilen)
{
	int i;
    UINT8 *pu8Obj1 = (UINT8 *)pau8Data2;
    UINT8 *pu8Obj2 = (UINT8 *)pau8Data2;
	
	for (i = SET_TO_ZERO; i < ilen; i++)
    {
    	if (pu8Obj1[i] != pu8Obj2[i])
            return (pu8Obj1[i] - pu8Obj2[i]);            	
	}
    
	return 0;
}

/*****************************************************************************/
/*****************************************************************************/
/*********************************Sink APIs*****************************/
/*****************************************************************************/
#if (TRUE == INCLUDE_PD_SINK)

void SAMD20_ConfigureSinkHardware(UINT8 u8PortNum,UINT16 u16VBUSVoltage,UINT16 u16Current)
{
    /* Clear the 1.5A_IND and 3A_IND */
    SNK_1_5A_IND_Clear();
    SNK_3A_IND_Clear();
    
    if (u16Current >= DPM_3000mA)
    {
        SNK_3A_IND_Set();
    }
    else if (u16Current >= DPM_1500mA)
    {
        SNK_1_5A_IND_Set();
    }
    else
    {
        //Do nothing
    }
}


void SAMD20_Drive_DAC_I(UINT16 u16DACData)
{
    /*SAMD20 intenally divides u16DACData by 0x3FF. Hence multiplying with 0x3FF*/
    /*SAMD20 internally multiplies u16DACData by 3.3V. Hence, dividing by 3.3V*/
    /*Dividing by 1000 to convert voltage u16DACData in mV to Volt.*/

    UINT32 u32DACCalculate = u16DACData * 0x3FF;

    u16DACData = (UINT16)(u32DACCalculate / 3300);
    DAC_DataWrite(u16DACData);
    
}
#endif

/*****************************************************************************/
/*****************************************************************************/
/*********************************UART APIs*****************************/
/*****************************************************************************/
#if (TRUE == CONFIG_HOOK_DEBUG_MSG)

void SAMD20_UART_Initialisation(void)
{
    SAMD20UART_Init(SAMD20_UART_INSTANCE);
}

void SAMD20_UART_Write_Char(char byData)
{
    SAMD20UART_Write_Char(SAMD20_UART_INSTANCE, byData);
}

void SAMD20_UART_Write_Int(UINT32 dwWriteInt, UINT8 byWidth)
{
    SAMD20UART_Write_Int(SAMD20_UART_INSTANCE, dwWriteInt, byWidth);
}

void SAMD20_UART_Write_String(char* pbyMessage)
{
    SAMD20UART_Write_String(SAMD20_UART_INSTANCE, pbyMessage);
}


#endif //CONFIG_HOOK_DEBUG_MSG

/* *****************************************************************************
 End of File
 */
