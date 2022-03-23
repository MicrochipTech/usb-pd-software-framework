/*******************************************************************************
  External Interrupt Controller (EIC) PLIB

  Company
    Microchip Technology Inc.

  File Name
    plib_eic.h

  Summary
    EIC PLIB Header File.

  Description
    This file defines the interface to the EIC peripheral library. This
    library provides access to and control of the associated peripheral
    instance.

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

/* Guards against multiple inclusion */
#ifndef PLIB_EIC_H
#define PLIB_EIC_H

// *****************************************************************************
// *****************************************************************************
// Section: Included Files
// *****************************************************************************
// *****************************************************************************

#include "device.h"
#include <stdbool.h>
#include <stddef.h>

// DOM-IGNORE-BEGIN
#ifdef __cplusplus // Provide C++ Compatibility

    extern "C" {

#endif
// DOM-IGNORE-END

// *****************************************************************************
// *****************************************************************************
// Section: Data Types
// *****************************************************************************
// *****************************************************************************

/* EIC Pin Count */
#define EXTINT_COUNT                        (16U)

typedef enum
{
    /* External Interrupt Controller Pin 3 */
    EIC_PIN_3 = 3,

    /* External Interrupt Controller Pin 7 */
    EIC_PIN_7 = 7,

    /* External Interrupt Controller Pin 14 */
    EIC_PIN_14 = 14,

    /* External Interrupt Controller Pin 15 */
    EIC_PIN_15 = 15,

    EIC_PIN_MAX = 16

} EIC_PIN;


typedef void (*EIC_CALLBACK) (uintptr_t context);

typedef struct
{
    /* External Interrupt Pin Callback Handler */
    EIC_CALLBACK    callback;

    /* External Interrupt Pin Client context */
    uintptr_t       context;

    /* External Interrupt Pin number */
    EIC_PIN         eicPinNo;

} EIC_CALLBACK_OBJ;


// *****************************************************************************
// *****************************************************************************
// Section: Interface Routines
// *****************************************************************************
// *****************************************************************************

void EIC_Initialize(void);

// *****************************************************************************
/* Function:
    void EIC_InterruptEnable (EIC_PIN pin)

  Summary:
    Enables interrupts on a pin.

  Description
    This function enables interrupts on an external interrupt pin.
    When enabled, the interrupt pin sense will be configured as per the
    configuration set in MHC.

   Precondition:
    EIC_Initialize() function must have been called for the
    associated instance.

   Parameters:
    pin - EIC Pin number

   Returns:
    None

   Example:
    <code>
    EIC_Initialize();
    EIC_InterruptEnable(EIC_PIN_3);
    </code>

  Remarks:
    None.
*/
void EIC_InterruptEnable (EIC_PIN pin);

// *****************************************************************************
/* Function:
    void EIC_InterruptDisable (EIC_PIN pin)

  Summary:
    Disables interrupts on a pin.

  Description
    This function disables interrupts on an external interrupt pin.

   Precondition:
    EIC_Initialize() function must have been called for the
    associated instance.

   Parameters:
    pin - EIC Pin number.

   Returns:
    None

   Example:
    <code>
    EIC_Initialize();
    EIC_InterruptDisable(EIC_PIN_3);
    </code>

  Remarks:
    None.
*/

void EIC_InterruptDisable (EIC_PIN pin);

// *****************************************************************************
/* Function:
    void EIC_CallbackRegister (EIC_PIN pin, EIC_CALLBACK callback
                                            uintptr_t context);

  Summary:
    Registers the function to be called when an interrupt condition has been
    sensed on the pin.

  Description
    This function registers the callback function to be called when an interrupt
    condition has been sensed on the pin. A unique callback function can be
    registered for each pin.

    When an interrupt condition has been sensed on the pin, the library will
    call the registered callback function and will then clear the interrupt
    condition when the callback function exits.

  Precondition:
    EIC_Initialize() must have been called first for the associated
    instance.

  Parameters:
    pin - EIC Pin number

    callback - callback function pointer. Setting this to NULL will disable the
    callback feature.

    context - An application defined context value that will be passed to the
    callback function.

  Returns:
    None.

  Example:
    <code>
    // Refer to the description of the EIC_CALLBACK data type for details on API
    // usage.
    </code>

  Remarks:
    Context value can be set to NULL, if not required.
*/

void EIC_CallbackRegister(EIC_PIN pin, EIC_CALLBACK callback, uintptr_t context);



#ifdef __cplusplus // Provide C++ Compatibility

    }

#endif

#endif /* PLIB_EIC_H */
