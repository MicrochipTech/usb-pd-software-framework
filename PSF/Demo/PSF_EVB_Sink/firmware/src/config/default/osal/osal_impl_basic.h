/*******************************************************************************
  Operating System Abstraction Layer Basic Implementation

  Company:
    Microchip Technology Inc.

  File Name:
    osal_impl_basic.h

  Summary:
    Header file for the OSAL Basic implementation.

  Description:
    This file defines the additions or variations to the OSAL base implementation.
 Where it is logical or possible to implement an OSAL function in a simple form
 without an RTOS being present then the function has been defined here and
 implemented either here as an inline or #define. Longer functions that are part
 of the basic implementation may also be found in the file osal.c
 The best way to consider this file is detailing any deviations from the osal.h
 definitions OR as the complete implementation of those functions when pretending
 to support BASIC operations.
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

#ifndef OSAL_IMPL_BASIC_H
#define OSAL_IMPL_BASIC_H

#ifdef __cplusplus
extern "C" {
#endif

// *****************************************************************************
// *****************************************************************************
// Section: Included Files
// *****************************************************************************
// *****************************************************************************
#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include "system/int/sys_int.h"
#include "device.h"


typedef uint8_t                     OSAL_SEM_HANDLE_TYPE;
typedef uint8_t                     OSAL_MUTEX_HANDLE_TYPE;
typedef uint32_t                    OSAL_CRITSECT_DATA_TYPE;
#define OSAL_WAIT_FOREVER           (uint16_t) 0xFFFF

#define OSAL_SEM_DECLARE(semID)         uint8_t    semID
#define OSAL_MUTEX_DECLARE(mutexID)     uint8_t    mutexID

// *****************************************************************************
/* Macro: OSAL_ASSERT
 */

#define OSAL_ASSERT(test, message)      test

// *****************************************************************************
/* OSAL Result type

  Summary:
    Enumerated type representing the general return value from OSAL functions.

  Description:
    This enum represents possible return types from OSAL functions.

  Remarks:
    These enum values are the possible return values from OSAL functions
    where a standard success/fail type response is required. The majority
    of OSAL functions will return this type with a few exceptions.
*/

typedef enum OSAL_SEM_TYPE
{
  OSAL_SEM_TYPE_BINARY,
  OSAL_SEM_TYPE_COUNTING
} OSAL_SEM_TYPE;

typedef enum OSAL_CRIT_TYPE
{
  OSAL_CRIT_TYPE_LOW,
  OSAL_CRIT_TYPE_HIGH
} OSAL_CRIT_TYPE;

typedef enum OSAL_RESULT
{
  OSAL_RESULT_NOT_IMPLEMENTED = -1,
  OSAL_RESULT_FALSE = 0,
  OSAL_RESULT_TRUE = 1
} OSAL_RESULT;

// *****************************************************************************
// *****************************************************************************
// Section: Section: Interface Routines Group Declarations
// *****************************************************************************
// *****************************************************************************
__STATIC_INLINE OSAL_RESULT OSAL_SEM_Create(OSAL_SEM_HANDLE_TYPE* semID, OSAL_SEM_TYPE type, uint8_t maxCount, uint8_t initialCount);
__STATIC_INLINE OSAL_RESULT OSAL_SEM_Delete(OSAL_SEM_HANDLE_TYPE* semID);
__STATIC_INLINE OSAL_RESULT OSAL_SEM_Pend(OSAL_SEM_HANDLE_TYPE* semID, uint16_t waitMS);
__STATIC_INLINE OSAL_RESULT OSAL_SEM_Post(OSAL_SEM_HANDLE_TYPE* semID);
__STATIC_INLINE OSAL_RESULT OSAL_SEM_PostISR(OSAL_SEM_HANDLE_TYPE* semID);
__STATIC_INLINE uint8_t OSAL_SEM_GetCount(OSAL_SEM_HANDLE_TYPE* semID);

__STATIC_INLINE OSAL_CRITSECT_DATA_TYPE OSAL_CRIT_Enter(OSAL_CRIT_TYPE severity);
__STATIC_INLINE void OSAL_CRIT_Leave(OSAL_CRIT_TYPE severity, OSAL_CRITSECT_DATA_TYPE status);

__STATIC_INLINE OSAL_RESULT OSAL_MUTEX_Create(OSAL_MUTEX_HANDLE_TYPE* mutexID);
__STATIC_INLINE OSAL_RESULT OSAL_MUTEX_Delete(OSAL_MUTEX_HANDLE_TYPE* mutexID);
__STATIC_INLINE OSAL_RESULT OSAL_MUTEX_Lock(OSAL_MUTEX_HANDLE_TYPE* mutexID, uint16_t waitMS);
__STATIC_INLINE OSAL_RESULT OSAL_MUTEX_Unlock(OSAL_MUTEX_HANDLE_TYPE* mutexID);

__STATIC_INLINE void* OSAL_Malloc(size_t size);
__STATIC_INLINE void OSAL_Free(void* pData);

OSAL_RESULT OSAL_Initialize(void);

__STATIC_INLINE const char* OSAL_Name(void);

// *****************************************************************************
// *****************************************************************************
// Section: Interface Routines Group Defintions
// *****************************************************************************
// *****************************************************************************

/* Critical Section group */
// *****************************************************************************
/* Function: OSAL_CRITSECT_DATA_TYPE OSAL_CRIT_Enter(OSAL_CRIT_TYPE severity)
 */
static OSAL_CRITSECT_DATA_TYPE OSAL_CRIT_Enter(OSAL_CRIT_TYPE severity)
{
  if(severity == OSAL_CRIT_TYPE_LOW)
    return (0);
  /*if priority is set to HIGH the user wants interrupts disabled*/
  return (SYS_INT_Disable());
}

// *****************************************************************************
/* Function: void OSAL_CRIT_Leave(OSAL_CRIT_TYPE severity, OSAL_CRITSECT_DATA_TYPE status)
 */
static void OSAL_CRIT_Leave(OSAL_CRIT_TYPE severity, OSAL_CRITSECT_DATA_TYPE status)
{
  if(severity == OSAL_CRIT_TYPE_LOW)
    return;
  /*if priority is set to HIGH the user wants interrupts re-enabled to the state
  they were before disabling.*/
  SYS_INT_Restore(status);
}

// *****************************************************************************
/* Function: OSAL_RESULT OSAL_SEM_Create(OSAL_SEM_HANDLE_TYPE semID, OSAL_SEM_TYPE type,
                                uint8_t maxCount, uint8_t initialCount)
 */
static OSAL_RESULT __attribute__((always_inline)) OSAL_SEM_Create(OSAL_SEM_HANDLE_TYPE* semID, OSAL_SEM_TYPE type,
                                uint8_t maxCount, uint8_t initialCount)
{
  OSAL_CRITSECT_DATA_TYPE IntState;

  IntState = OSAL_CRIT_Enter(OSAL_CRIT_TYPE_HIGH);

  if (type == OSAL_SEM_TYPE_COUNTING)
    *semID = initialCount;
  else
    *semID = 1;

  OSAL_CRIT_Leave(OSAL_CRIT_TYPE_HIGH,IntState);

  return OSAL_RESULT_TRUE;
}

// *****************************************************************************
/* Function: OSAL_RESULT OSAL_SEM_Delete(OSAL_SEM_HANDLE_TYPE semID)
 */
static OSAL_RESULT __attribute__((always_inline)) OSAL_SEM_Delete(OSAL_SEM_HANDLE_TYPE* mutexID)
{
   return (OSAL_RESULT_TRUE);
}

// *****************************************************************************
/* Function: OSAL_RESULT OSAL_SEM_Pend(OSAL_SEM_HANDLE_TYPE semID, uint16_t waitMS)
 */
static  OSAL_RESULT __attribute__((always_inline)) OSAL_SEM_Pend(OSAL_SEM_HANDLE_TYPE* semID, uint16_t waitMS)
{
  OSAL_CRITSECT_DATA_TYPE IntState;

  IntState = OSAL_CRIT_Enter(OSAL_CRIT_TYPE_HIGH);

  if (*semID > 0)
  {
    (*semID)--;
    OSAL_CRIT_Leave(OSAL_CRIT_TYPE_HIGH,IntState);

    return OSAL_RESULT_TRUE;
  }

  OSAL_CRIT_Leave(OSAL_CRIT_TYPE_HIGH,IntState);

  return OSAL_RESULT_FALSE;
}

// *****************************************************************************
/* Function: OSAL_RESULT OSAL_SEM_Post(OSAL_SEM_HANDLE_TYPE semID)
 */
static OSAL_RESULT __attribute__((always_inline)) OSAL_SEM_Post(OSAL_SEM_HANDLE_TYPE* semID)
{
  OSAL_CRITSECT_DATA_TYPE IntState;

  IntState = OSAL_CRIT_Enter(OSAL_CRIT_TYPE_HIGH);
  (*semID)++;
  OSAL_CRIT_Leave(OSAL_CRIT_TYPE_HIGH,IntState);

  return OSAL_RESULT_TRUE;
}

// *****************************************************************************
/* Function: OSAL_RESULT OSAL_SEM_PostISR(OSAL_SEM_HANDLE_TYPE semID)
 */
static OSAL_RESULT __attribute__((always_inline)) OSAL_SEM_PostISR(OSAL_SEM_HANDLE_TYPE* semID)
{
  (*semID)++;
  return OSAL_RESULT_TRUE;
}

// *****************************************************************************
/* Function: uint8_t OSAL_SEM_GetCount(OSAL_SEM_HANDLE_TYPE semID)
 */
static uint8_t __attribute__((always_inline)) OSAL_SEM_GetCount(OSAL_SEM_HANDLE_TYPE* semID)
{
  return *semID;
}

// *****************************************************************************
/* Function: OSAL_RESULT OSAL_MUTEX_Create(OSAL_MUTEX_HANDLE_TYPE mutexID)
 */
static OSAL_RESULT __attribute__((always_inline)) OSAL_MUTEX_Create(OSAL_MUTEX_HANDLE_TYPE* mutexID)
{
  *mutexID = 1;
  return OSAL_RESULT_TRUE;
}

// *****************************************************************************
/* Function: OSAL_RESULT OSAL_MUTEX_Delete(OSAL_MUTEX_HANDLE_TYPE mutexID)
 */
static OSAL_RESULT __attribute__((always_inline)) OSAL_MUTEX_Delete(OSAL_MUTEX_HANDLE_TYPE* mutexID)
{
  return (OSAL_RESULT_TRUE);
}
// *****************************************************************************
/* Function: OSAL_RESULT OSAL_MUTEX_Lock(OSAL_MUTEX_HANDLE_TYPE mutexID, uint16_t waitMS)
 */
static OSAL_RESULT __attribute__((always_inline)) OSAL_MUTEX_Lock(OSAL_MUTEX_HANDLE_TYPE* mutexID, uint16_t waitMS)
{
  if (*mutexID == 1)
  {
    *mutexID = 0;
    return OSAL_RESULT_TRUE;
  }
  return OSAL_RESULT_FALSE;
}

// *****************************************************************************
/* Function: OSAL_RESULT OSAL_MUTEX_Unlock(OSAL_MUTEX_HANDLE_TYPE mutexID)
 */
static OSAL_RESULT __attribute__((always_inline)) OSAL_MUTEX_Unlock(OSAL_MUTEX_HANDLE_TYPE* mutexID)
{
  *mutexID = 1;
  return OSAL_RESULT_TRUE;
}

// *****************************************************************************
/* Function: void* OSAL_Malloc(size_t size)
 */
static void* __attribute__((always_inline)) OSAL_Malloc(size_t size)
{
    return malloc(size);
}

// *****************************************************************************
/* Function: void OSAL_Free(void* pData)
 */
static void __attribute__((always_inline)) OSAL_Free(void* pData)
{
    free(pData);
}

// Initialization and Diagnostics
// *****************************************************************************
/* Function: OSAL_RESULT OSAL_Initialize()
 */
#define OSAL_Initialize()


// *****************************************************************************
/* Function: const char* OSAL_Name()
 */
static const char* __attribute__((always_inline)) OSAL_Name(void)
{
  return((const char*) "BASIC");
}


#ifdef __cplusplus
}
#endif

#endif // _OSAL_IMPL_BASIC_H

/*******************************************************************************
 End of File
 */




