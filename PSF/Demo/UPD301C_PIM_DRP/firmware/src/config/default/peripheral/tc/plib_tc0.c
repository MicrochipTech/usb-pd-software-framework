/*******************************************************************************
  Timer/Counter(TC0) PLIB

  Company
    Microchip Technology Inc.

  File Name
    plib_tc0.c

  Summary
    TC0 PLIB Implementation File.

  Description
    This file defines the interface to the TC peripheral library. This
    library provides access to and control of the associated peripheral
    instance.

  Remarks:
    None.

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

// *****************************************************************************
// *****************************************************************************
// Section: Included Files
// *****************************************************************************
// *****************************************************************************
/* This section lists the other files that are included in this file.
*/

#include "plib_tc0.h"


// *****************************************************************************
// *****************************************************************************
// Section: Global Data
// *****************************************************************************
// *****************************************************************************

TC_TIMER_CALLBACK_OBJ TC0_CallbackObject;

// *****************************************************************************
// *****************************************************************************
// Section: TC0 Implementation
// *****************************************************************************
// *****************************************************************************

// *****************************************************************************
/* Initialize the TC module in Timer mode */
void TC0_TimerInitialize( void )
{
    /* Reset TC */
    TC0_REGS->COUNT8.TC_CTRLA = TC_CTRLA_SWRST_Msk;

    while((TC0_REGS->COUNT8.TC_STATUS & TC_STATUS_SYNCBUSY_Msk))
    {
        /* Wait for Write Synchronization */
    }

    /* Configure counter mode & prescaler */
    TC0_REGS->COUNT8.TC_CTRLA = TC_CTRLA_MODE_COUNT8 | TC_CTRLA_PRESCALER_DIV256 | TC_CTRLA_WAVEGEN_MPWM ;

    /* Configure timer period */
    TC0_REGS->COUNT8.TC_CC[0U] = 187U;

    /* Clear all interrupt flags */
    TC0_REGS->COUNT8.TC_INTFLAG = TC_INTFLAG_Msk;

    TC0_CallbackObject.callback = NULL;
    /* Enable interrupt*/
    TC0_REGS->COUNT8.TC_INTENSET = TC_INTENSET_OVF_Msk;

    TC0_REGS->COUNT8.TC_EVCTRL = TC_EVCTRL_OVFEO_Msk;

    while((TC0_REGS->COUNT8.TC_STATUS & TC_STATUS_SYNCBUSY_Msk))
    {
        /* Wait for Write Synchronization */
    }
}

/* Enable the TC counter */
void TC0_TimerStart( void )
{
    TC0_REGS->COUNT8.TC_CTRLA |= TC_CTRLA_ENABLE_Msk;
    while((TC0_REGS->COUNT8.TC_STATUS & TC_STATUS_SYNCBUSY_Msk))
    {
        /* Wait for Write Synchronization */
    }
}

/* Disable the TC counter */
void TC0_TimerStop( void )
{
    TC0_REGS->COUNT8.TC_CTRLA &= ~TC_CTRLA_ENABLE_Msk;
    while((TC0_REGS->COUNT8.TC_STATUS & TC_STATUS_SYNCBUSY_Msk))
    {
        /* Wait for Write Synchronization */
    }
}

uint32_t TC0_TimerFrequencyGet( void )
{
    return (uint32_t)(187500UL);
}

/* Get the current timer counter value */
uint8_t TC0_Timer8bitCounterGet( void )
{
    /* Write command to force COUNT register read synchronization */
    TC0_REGS->COUNT8.TC_READREQ = TC_READREQ_RREQ_Msk | TC_COUNT8_COUNT_REG_OFST;

    while((TC0_REGS->COUNT8.TC_STATUS & TC_STATUS_SYNCBUSY_Msk))
    {
        /* Wait for Write Synchronization */
    }

    /* Read current count value */
    return (uint8_t)TC0_REGS->COUNT8.TC_COUNT;
}

/* Configure timer counter value */
void TC0_Timer8bitCounterSet( uint8_t count )
{
    TC0_REGS->COUNT8.TC_COUNT = count;

    while((TC0_REGS->COUNT8.TC_STATUS & TC_STATUS_SYNCBUSY_Msk))
    {
        /* Wait for Write Synchronization */
    }
}

/* Configure timer period */
void TC0_Timer8bitPeriodSet( uint8_t period )
{
    TC0_REGS->COUNT8.TC_CC[0] = period;
    while((TC0_REGS->COUNT8.TC_STATUS & TC_STATUS_SYNCBUSY_Msk))
    {
        /* Wait for Write Synchronization */
    }
}

/* Read the timer period value */
uint8_t TC0_Timer8bitPeriodGet( void )
{
    /* Write command to force CC register read synchronization */
    TC0_REGS->COUNT8.TC_READREQ = TC_READREQ_RREQ_Msk | TC_COUNT8_CC_REG_OFST;

    while((TC0_REGS->COUNT8.TC_STATUS & TC_STATUS_SYNCBUSY_Msk))
    {
        /* Wait for Write Synchronization */
    }
    return (uint8_t)TC0_REGS->COUNT8.TC_CC[0];
}



/* Register callback function */
void TC0_TimerCallbackRegister( TC_TIMER_CALLBACK callback, uintptr_t context )
{
    TC0_CallbackObject.callback = callback;

    TC0_CallbackObject.context = context;
}

/* Timer Interrupt handler */
void TC0_TimerInterruptHandler( void )
{
    TC_TIMER_STATUS status;
    status = TC0_REGS->COUNT8.TC_INTFLAG;
    /* Clear interrupt flags */
    TC0_REGS->COUNT8.TC_INTFLAG = TC_INTFLAG_Msk;
    if(TC0_CallbackObject.callback != NULL)
    {
        TC0_CallbackObject.callback(status, TC0_CallbackObject.context);
    }
}

