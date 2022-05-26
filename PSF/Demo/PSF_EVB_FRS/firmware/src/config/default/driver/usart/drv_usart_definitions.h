/*******************************************************************************
  USART Driver Definitions Header File

  Company:
    Microchip Technology Inc.

  File Name:
    drv_usart_definitions.h

  Summary:
    USART Driver Definitions Header File

  Description:
    This file provides implementation-specific definitions for the USART
    driver's system interface.
*******************************************************************************/

//DOM-IGNORE-BEGIN
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
//DOM-IGNORE-END

#ifndef DRV_USART_DEFINITIONS_H
#define DRV_USART_DEFINITIONS_H

// *****************************************************************************
// *****************************************************************************
// Section: File includes
// *****************************************************************************
// *****************************************************************************

#include "system/int/sys_int.h"

// DOM-IGNORE-BEGIN
#ifdef __cplusplus  // Provide C++ Compatibility

    extern "C" {

#endif
// DOM-IGNORE-END

// *****************************************************************************
// *****************************************************************************
// Section: Data Types
// *****************************************************************************
// *****************************************************************************

// *****************************************************************************
/* USART Driver Errors Declaration */

typedef enum _DRV_USART_ERROR
{
    DRV_USART_ERROR_NONE = 0,

    DRV_USART_ERROR_OVERRUN = 1,

    DRV_USART_ERROR_PARITY = 2,

    DRV_USART_ERROR_FRAMING = 3

} _DRV_USART_ERROR;

// *****************************************************************************
/* USART Serial Setup */

typedef enum
{
    DRV_USART_DATA_5_BIT = 0,
    DRV_USART_DATA_6_BIT = 1,
    DRV_USART_DATA_7_BIT = 2,
    DRV_USART_DATA_8_BIT = 3,
    DRV_USART_DATA_9_BIT = 4,

    /* Force the compiler to reserve 32-bit memory space for each enum */
    DRV_USART_DATA_BIT_INVALID = 0xFFFFFFFF

} DRV_USART_DATA_BIT;

typedef enum
{
    DRV_USART_PARITY_NONE = 0,
    DRV_USART_PARITY_EVEN = 1,
    DRV_USART_PARITY_ODD = 2,
    DRV_USART_PARITY_MARK = 3,
    DRV_USART_PARITY_SPACE = 4,
    DRV_USART_PARITY_MULTIDROP = 5,

    /* Force the compiler to reserve 32-bit memory space for each enum */
    DRV_USART_PARITY_INVALID = 0xFFFFFFFF

} DRV_USART_PARITY;

typedef enum
{
    DRV_USART_STOP_1_BIT = 0,
    DRV_USART_STOP_1_5_BIT = 1,
    DRV_USART_STOP_2_BIT = 2,

    /* Force the compiler to reserve 32-bit memory space for each enum */
    DRV_USART_STOP_BIT_INVALID = 0xFFFFFFFF

} DRV_USART_STOP_BIT;

typedef struct _DRV_USART_SERIAL_SETUP
{
    uint32_t baudRate;

    DRV_USART_PARITY parity;

    DRV_USART_DATA_BIT dataWidth;

    DRV_USART_STOP_BIT stopBits;

} _DRV_USART_SERIAL_SETUP;

// *****************************************************************************
/* USART PLIB API Set needed by the driver */

typedef void (* DRV_USART_PLIB_CALLBACK)( uintptr_t context);

typedef void(*DRV_USART_PLIB_READ_CALLBACK_REG)(DRV_USART_PLIB_CALLBACK callback, uintptr_t context);
typedef bool(*DRV_USART_PLIB_READ)(void *buffer, const size_t size);
typedef bool(*DRV_USART_PLIB_READ_IS_BUSY)(void);
typedef size_t(*DRV_USART_PLIB_READ_COUNT_GET)(void);
typedef bool (*DRV_USART_PLIB_READ_ABORT)(void);

typedef void(*DRV_USART_PLIB_WRITE_CALLBACK_REG)(DRV_USART_PLIB_CALLBACK callback, uintptr_t context);
typedef bool(*DRV_USART_PLIB_WRITE)(void *buffer, const size_t size);
typedef bool(*DRV_USART_PLIB_WRITE_IS_BUSY)(void);
typedef size_t(*DRV_USART_PLIB_WRITE_COUNT_GET)(void);

typedef uint32_t (*DRV_USART_PLIB_ERROR_GET)(void);
typedef bool(*DRV_USART_PLIB_SERIAL_SETUP)(_DRV_USART_SERIAL_SETUP* setup, uint32_t clkSrc);

typedef struct
{
    DRV_USART_PLIB_READ_CALLBACK_REG readCallbackRegister;
    DRV_USART_PLIB_READ read;
    DRV_USART_PLIB_READ_IS_BUSY readIsBusy;
    DRV_USART_PLIB_READ_COUNT_GET readCountGet;
    DRV_USART_PLIB_READ_ABORT readAbort;

    DRV_USART_PLIB_WRITE_CALLBACK_REG writeCallbackRegister;
    DRV_USART_PLIB_WRITE write;
    DRV_USART_PLIB_WRITE_IS_BUSY writeIsBusy;
    DRV_USART_PLIB_WRITE_COUNT_GET writeCountGet;

    DRV_USART_PLIB_ERROR_GET errorGet;
    DRV_USART_PLIB_SERIAL_SETUP serialSetup;

} DRV_USART_PLIB_INTERFACE;

typedef struct
{
    int32_t         usartTxReadyInt;
    int32_t         usartTxCompleteInt;
    int32_t         usartRxCompleteInt;
    int32_t         usartErrorInt;
} DRV_USART_MULTI_INT_SRC;

typedef union
{
    DRV_USART_MULTI_INT_SRC             multi;
    int32_t                             usartInterrupt;
} DRV_USART_INT_SRC;

typedef struct
{
    bool                        isSingleIntSrc;
    DRV_USART_INT_SRC           intSources;
} DRV_USART_INTERRUPT_SOURCES;

// *****************************************************************************
/* USART Driver Initialization Data Declaration */

struct _DRV_USART_INIT
{
    /* Identifies the PLIB API set to be used by the driver to access the
     * peripheral. */
    const DRV_USART_PLIB_INTERFACE*         usartPlib;

    /* Number of clients */
    uint32_t                                numClients;

    /* Memory Pool for Client Objects */
    uintptr_t                               clientObjPool;


    const uint32_t*                         remapDataWidth;

    const uint32_t*                         remapParity;

    const uint32_t*                         remapStopBits;

    const uint32_t*                         remapError;

    /* Size of transmit and receive buffer pool */
    uint32_t                                bufferObjPoolSize;

    /* Pointer to the transmit and receive buffer pool */
    uintptr_t                               bufferObjPool;

    const DRV_USART_INTERRUPT_SOURCES*      interruptSources;

    DRV_USART_DATA_BIT                      dataWidth;
};

//DOM-IGNORE-BEGIN
#ifdef __cplusplus

    }

#endif
//DOM-IGNORE-END

#endif // #ifndef DRV_USART_DEFINITIONS_H
