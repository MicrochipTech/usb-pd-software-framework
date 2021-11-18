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
/*TCn_TimerInitialize to initialize the timer is called from initialization.c - MS Edit,*/
/*Timer drivers -   calls TCn_TimerStart to start and run the timer and
                    registers PSF stack's timer callback through function PSF_APP_HWTimerCallback
                    in TCn_TimerCallbackRegiseter. n is the TC instance*/
#define PSF_APP_Timer_Initialise(n)    TCn_TimerInitialize(n)
#define TCn_TimerInitialize(n)   {TC##n##_TimerStart(); \
                                  TC##n##_TimerCallbackRegister(PSF_APP_HWTimerCallback, (uintptr_t)NULL);}


/*SERCOM0_SPI_Initialize() to initialize the SPI is called from initialization.c - MS Edit,*/
/*SPI drivers - SERCOMn_SPI_Initialize is called to initialize the SERCOM SPI module. 
                    n is the SERCOM instance*/
#define PSF_APP_SPI_Initialise(n)     SERCOMn_SPI_Initialize(n)
#define SERCOMn_SPI_Initialize(n)   SERCOM##n##_SPI_Initialize()

/*SPI drivers - SERCOMn_SPI_WriteRead is called for SPI read write operation. n is the SERCOM instance*/
#define PSF_APP_SPI_WriteRead(n,pTransmitData,txSize,pReceiveData,rxSize)\
                    SERCOMn_SPI_WriteRead(n,pTransmitData,txSize,pReceiveData,rxSize)
#define SERCOMn_SPI_WriteRead(n,pTransmitData,txSize,pReceiveData,rxSize)\
 SERCOM##n##_SPI_WriteRead(pTransmitData,txSize,pReceiveData,rxSize)

#if (TRUE == CONFIG_HOOK_DEBUG_MSG)
    /*Debug UART drivers*/

    /*SERCOM1_USART_Initialise is initialized as part of initialization.c - MS Edit*/
    #define PSF_APP_UART_Init(n) SERCOMn_UART_Initialise(n)
    #define SERCOMn_UART_Initialise(n) SERCOM##n##_USART_Initialize()

    #define PSF_APP_UART_Write(n, buffer, u32size) SERCOMn_UART_WRITE(n, buffer, u32size)
    #define SERCOMn_UART_WRITE(n, buffer, u32size) SERCOM##n##_USART_Write(buffer, u32size)

    #define PSF_APP_UART_IsTransmitComplete(n) SERCOMn_UART_IsTransmitComplete(n)
    #define SERCOMn_UART_IsTransmitComplete(n) SERCOM##n##_USART_TransmitComplete()

#endif /* CONFIG_HOOK_DEBUG_MSG */

/* ************************************************************************** */
/* ************************************************************************** */
// Section: Internal Functions                                               */
/* ************************************************************************** */
static void PSF_APP_HWTimerCallback(TC_TIMER_STATUS status, uintptr_t context)
{
    if (TC_TIMER_STATUS_OVERFLOW == (TC_TIMER_STATUS)((UINT8)status & (UINT8)TC_TIMER_STATUS_OVERFLOW))
    {
        /*PSF Timer Callback is called for Timer ISR*/
        MchpPSF_PDTimerHandler();
    }
}

/* ************************************************************************** */
/* ************************************************************************** */
// Section: Interface Functions                                               */
/* ************************************************************************** */
/*******************************Timer driver APIs************************************/
UINT8 PSF_APP_HWTimerInit(void)
{
    /*Timer is initialized and started to run for 1ms continously*/
    PSF_APP_Timer_Initialise(PSF_APP_TIMER_INSTANCE);
    return TRUE;
}
/*****************************************************************************/
/*********************************Alert APIs*****************************/
/*****************************************************************************/
void PSF_APP_UPD350AlertCallback(uintptr_t u8PortNum)
{
    /*PSF Alert Handler is called for specific port to service UPD350 Alert interrupt*/
    MchpPSF_UPDIrqHandler(u8PortNum);
}
/*****************************************************************************/
/*****************************************************************************/
/*********************************SPI Driver APIs*****************************/
/*****************************************************************************/
UINT8 PSF_APP_SPIInitialisation(void)
{
    //PSF_APP_SPI_Initialise(PSF_APP_SPI_INSTANCE); //MS Edit
    return TRUE;
}
UINT8 PSF_APP_SPIReaddriver (UINT8 u8PortNum, UINT8 *pu8WriteBuffer, UINT8 u8Writelength, UINT8 *pu8ReadBuffer, UINT8 u8Readlength)
{
    UINT8 u8Return = PSF_APP_SPI_WriteRead(PSF_APP_SPI_INSTANCE, pu8WriteBuffer, u8Writelength, NULL, SET_TO_ZERO);
    u8Return &= PSF_APP_SPI_WriteRead(PSF_APP_SPI_INSTANCE, NULL, SET_TO_ZERO, pu8ReadBuffer, u8Readlength);
    return u8Return;
}
UINT8 PSF_APP_SPIWritedriver (UINT8 u8PortNum, UINT8 *pu8WriteBuffer, UINT8 u8Writelength)
{
    return PSF_APP_SPI_WriteRead(PSF_APP_SPI_INSTANCE, pu8WriteBuffer, u8Writelength, NULL, SET_TO_ZERO);
}

/*****************************************************************************/
/*****************************************************************************/
/*********************************MCU APIs*****************************/
/*****************************************************************************/
void PSF_APP_EnterCriticalSection()
{
    if (gu32CriticalSectionCnt++ == 0)  
    {
        /*Interrupt is disabled if Critical section is not entered already*/
        __disable_irq();
		
    }
}
void PSF_APP_ExitCriticalSection()
{
    
    if (--gu32CriticalSectionCnt == 0)
    {
        /*Interrupt is Enabled when there is no pending critical section nesting*/
		__enable_irq();
    }
}
void* PSF_APP_MemCpy(void *pdest, const void *psrc, int ilen)
{
   // Typecast src and dest addresses to (char *)
   char *csrc = (char *)psrc;
   char *cdest = (char *)pdest;
 
   // Copy contents of src[] to dest[]
   for (int i=SET_TO_ZERO; i<ilen; i++)
       cdest[i] = csrc[i];
   
   return pdest;
}

int PSF_APP_MemCmp(const void *pau8Data1, const void *pau8Data2, int ilen)
{
	int index;
    UINT8 *pu8Obj1 = (UINT8 *)pau8Data1;
    UINT8 *pu8Obj2 = (UINT8 *)pau8Data2;
	
	for (index = SET_TO_ZERO; index < ilen; index++)
    {
    	if (pu8Obj1[index] != pu8Obj2[index])
            return (pu8Obj1[index] - pu8Obj2[index]);            	
	}
    
	return 0;
}

/*****************************************************************************/
/*****************************************************************************/
/*********************************UART APIs*****************************/
/*****************************************************************************/
#if (TRUE == CONFIG_HOOK_DEBUG_MSG)

/*Removed PSF_APP_UART_Initialisation- MS Edit*/
void PSF_APP_UART_Write_Char(char byData)
{
    (void)PSF_APP_UART_Write(PSF_APP_UART_INSTANCE, (void*)&byData,1); 
    while(!PSF_APP_UART_IsTransmitComplete(PSF_APP_UART_INSTANCE)) 
    { 
    }
}

void PSF_APP_UART_Write_Int(UINT32 dwWriteInt, UINT8 byWidth)
{
    (void)PSF_APP_UART_Write(PSF_APP_UART_INSTANCE, (void*)&dwWriteInt, byWidth); 
    while(!PSF_APP_UART_IsTransmitComplete(PSF_APP_UART_INSTANCE)) 
    { 
    } 
}

void PSF_APP_UART_Write_String(char* pbyMessage)
{
    (void)PSF_APP_UART_Write(PSF_APP_UART_INSTANCE, (void*)pbyMessage,strlen(pbyMessage)); 
    while(!PSF_APP_UART_IsTransmitComplete(PSF_APP_UART_INSTANCE)) 
    { 
    }
}


#endif /* CONFIG_HOOK_DEBUG_MSG */

/* *****************************************************************************
 End of File
 */
