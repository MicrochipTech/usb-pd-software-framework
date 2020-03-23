/*******************************************************************************
  SERCOM Universal Synchronous/Asynchrnous Receiver/Transmitter PLIB

  Company
    Microchip Technology Inc.

  File Name
    plib_sercom3_usart.h

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

#ifdef CONFIG_HOOK_DEBUG_MSG

#ifndef PLIB_SERCOM3_USART_H // Guards against multiple inclusion
#define PLIB_SERCOM3_USART_H

// *****************************************************************************
// *****************************************************************************
// Section: Included Files
// *****************************************************************************
// *****************************************************************************

#include <generic_defs.h>

#include "plib_sercom_usart_common.h"

// DOM-IGNORE-BEGIN
#ifdef __cplusplus // Provide C++ Compatibility

	extern "C" {

#endif
// DOM-IGNORE-END

// *****************************************************************************
// *****************************************************************************
// Section: Interface Routines
// *****************************************************************************
// *****************************************************************************

/* The following functions make up the methods (set of possible operations) of
this interface.
*/

// *****************************************************************************
/* Function:
    void SERCOM3_USART_Initialise (void);

  Summary:
    Initializes instance SERCOM3 operating in UART mode.

  Description:
    This function initializes instance SERCOM3 operating in UART mode.
    This function should be called before any other UART library function. The SERCOM
    module will be configured as per the MHC settings.

  Precondition:
    MCC GUI should be configured with the right values. The Generic Clock
    configuration and the SERCOM Peripheral Clock channel should have been
    configured in the clock manager GUI.The function will itself enable the
    required peripheral clock channel and main clock.

  Parameters:
    None.

  Returns:
    None.

  Example:
    <code>
        SERCOM3_USART_Initialise();
    </code>

  Remarks:
    This function must be called once before any other UART function is called.
*/
        
void SERCOM3_USART_Initialise( void );

// *****************************************************************************
/* Function:
bool SERCOM3_USART_SerialSetup( USART_SERIAL_SETUP * psSerialSetup, 
                                                      * UINT32 dwClkFrequency );


 Summary:
    Configure SERCOM UART operational parameters at run time.

  Description:
    This function allows the application to change the SERCOM UART operational
    parameter at run time. The application can thus override the MHC defined
    configuration for these parameters. The parameter are specified via the
    USART_SERIAL_SETUP type setup parameter. Each member of this parameter
    should be initialized to the desired value.

    Calling this function will affect any ongoing communication. The application
    must thus ensure that there is no on-going communication on UART before
    calling this function.

  Precondition:
    SERCOM SERCOM3 UART must first be initialized using SERCOM3_USART_Initialise().

  Parameters :
    setup - pointer to the data structure of type USART_SERIAL_SETUP containing
    the operation parameters. Each operation parameter must be specified even if
    the parameter does not need to change.

    dwClkFrequency - Current value of GCLK frequency feeding the SERCOM3 core.

  Returns:
    true - setup was successful.

    false - if baud rate is specified as 0 or if USART_SERIAL_SETUP does not contain the operation parameters. 

  Example:
    <code>
        USART_SERIAL_SETUP setup;
        setup.dwBaudRate = 9600;
        setup.parity = eUSART_PARITY_NONE;
        setup.dataWidth = 0; //8-bit data width
        setup.stopBits = 0; //1 stop bit

        // Assuming 20 MHz as peripheral Master clock frequency
        if (SERCOM3_USART_SerialSetup (&setup, 20000000) == false)
        {
            // this means setup could not be done, debug the reason.
        }

    </code>

  Remarks:
    The application would need to call this function only if the operational
    parameter need to be different than the ones configured in MHC.
*/

bool SERCOM3_USART_SerialSetup( USART_SERIAL_SETUP * psSerialSetup, UINT32 dwClkFrequency );

bool SERCOM3_USART_Write( void *pWriteData, const size_t size );

bool SERCOM3_USART_TransmitterIsReady( void );

bool SERCOM3_USART_TransmitComplete( void );

void SERCOM3_USART_WriteByte(UINT16 wWritedata );


USART_ERROR SERCOM3_USART_ErrorGet( void );

UINT32 SERCOM3_USART_FrequencyGet( void );

void SERCOM3_USART_Write_Int(UINT32 dwWriteInt, UINT8 byWidth);

void SERCOM3_USART_Write_String(char * pbyMessage);

void SERCOM3_USART_Write_Char(char byData);

// DOM-IGNORE-BEGIN
#ifdef __cplusplus  // Provide C++ Compatibility

    }

#endif
// DOM-IGNORE-END

#endif //PLIB_SERCOM3_USART_H

#endif //#ifdef CONFIG_HOOK_DEBUG_MSG