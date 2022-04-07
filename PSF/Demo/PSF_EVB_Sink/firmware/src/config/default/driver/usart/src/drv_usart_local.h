/*******************************************************************************
  USART Driver Local Data Structures

  Company:
    Microchip Technology Inc.

  File Name:
    drv_usart_local.h

  Summary:
    USART Driver Local Data Structures

  Description:
    Driver Local Data Structures
*******************************************************************************/

//DOM-IGNORE-BEGIN
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
//DOM-IGNORE-END

#ifndef DRV_USART_LOCAL_H
#define DRV_USART_LOCAL_H

// *****************************************************************************
// *****************************************************************************
// Section: Included Files
// *****************************************************************************
// *****************************************************************************

#include "driver/usart/drv_usart.h"
#include "osal/osal.h"

// *****************************************************************************
// *****************************************************************************
// Section: Data Type Definitions
// *****************************************************************************
// *****************************************************************************

// *****************************************************************************
/* USART Driver Buffer Handle Macros

  Summary:
    USART driver Buffer Handle Macros

  Description:
    Buffer handle related utility macros. USART driver buffer handle is a
    combination of buffer token and the buffer object index. The token
    is a 16 bit number that is incremented for every new read or write request
    and is used along with the buffer object index to generate a new buffer
    handle for every request.

  Remarks:
    None
*/

/* USART Driver Handle Macros*/
#define DRV_USART_INDEX_MASK                      (0x000000FF)

#define DRV_USART_INSTANCE_MASK                   (0x0000FF00)

#define DRV_USART_TOKEN_MAX                       (0xFFFF)

// *****************************************************************************
/* USART Driver Buffer States

  Summary
    Identifies the possible state of the buffer that can result from a
    buffer request add or queue purge request.

  Description
    This enumeration identifies the possible state of the buffer that can
    result from a buffer request add or queue purge request by the client.

  Remarks:
    DRV_USART_BUFFER_IS_FREE is the state of the buffer which is in the
    free buffer pool.
*/

typedef enum
{
    /* Buffer is not added to either write or read queue. In other words,
     * the buffer is in the free pool.
     */
    DRV_USART_BUFFER_IS_FREE,

    /* Buffer is in the queue. */
    DRV_USART_BUFFER_IS_IN_QUEUE,

    /* USART is processing the buffer. */
    DRV_USART_BUFFER_IS_PROCESSING

} DRV_USART_BUFFER_STATE;


// *****************************************************************************
/* USART Driver Transfer Direction

  Summary
    Identifies the direction of transfer.

  Description
    This enumeration identifies the direction of transfer.

  Remarks:
    None.
*/

typedef enum
{
    /* Receive Operation */
    DRV_USART_DIRECTION_RX,

    /* Transmit Operation */
    DRV_USART_DIRECTION_TX

} DRV_USART_DIRECTION;

// *****************************************************************************
/* USART Driver Buffer Object

  Summary:
    Object used to keep track of a client's buffer.

  Description:
    None.

  Remarks:
    None.
*/

typedef struct _DRV_USART_BUFFER_OBJ
{
    /* Handle to the client that owns this buffer object when it was queued */
    DRV_HANDLE                      clientHandle;

    /* This flag tracks whether this object is in use */
    volatile bool                   inUse;

    /* Pointer to the application read or write buffer */
    void*                           buffer;

    /* Number of bytes to be transferred */
    size_t                          size;

    /* Number of bytes completed */
    volatile size_t                 nCount;

    /* Next buffer pointer */
    struct _DRV_USART_BUFFER_OBJ*   next;

    /* Current state of the buffer */
    DRV_USART_BUFFER_STATE          currentState;

    /* Current status of the buffer */
    volatile DRV_USART_BUFFER_EVENT status;

    /* Errors associated with the USART hardware instance */
    volatile DRV_USART_ERROR        errors;

    DRV_USART_BUFFER_HANDLE         bufferHandle;

} DRV_USART_BUFFER_OBJ;

// *****************************************************************************
/* USART Driver Instance Object

  Summary:
    Object used to keep any data required for an instance of the USART driver.

  Description:
    None.

  Remarks:
    None.
*/

typedef struct
{
    /* Flag to indicate this object is in use  */
    bool                                    inUse;

    /* Flag to indicate that driver has been opened Exclusively*/
    bool                                    isExclusive;

    /* The status of the driver */
    SYS_STATUS                              status;

    /* PLIB API list that will be used by the driver to access the hardware */
    const DRV_USART_PLIB_INTERFACE*         usartPlib;

    /* Number of active clients */
    size_t                                  nClients;

    /* Maximum number of clients */
    size_t                                  nClientsMax;

    /* Memory Pool for Client Objects */
    uintptr_t                               clientObjPool;

    /* Instance specific token counter used to generate unique client/transfer handles */
    uint16_t                                usartTokenCount;

    /* Size of transmit and receive buffer queue */
    uint32_t                                bufferObjPoolSize;

    /* Pointer to the transmit and receive buffer pool */
    DRV_USART_BUFFER_OBJ*                   bufferObjPool;

    /* Linked list of transmit buffer objects */
    DRV_USART_BUFFER_OBJ*                   transmitObjList;

    /* Linked list of receive buffer objects */
    DRV_USART_BUFFER_OBJ*                   receiveObjList;

    /* To identify if we are running from interrupt context or not */
    uint8_t                                 interruptNestingCount;


    /* Mutex to protect access to the client objects */
    OSAL_MUTEX_DECLARE(mutexClientObjects);

    /* Mutex to protect access to the transfer objects */
    OSAL_MUTEX_DECLARE(mutexTransferObjects);

    const uint32_t*                         remapDataWidth;

    const uint32_t*                         remapParity;

    const uint32_t*                         remapStopBits;

    const uint32_t*                         remapError;

    const DRV_USART_INTERRUPT_SOURCES*      interruptSources;

    bool                                    usartTxReadyIntStatus;

    bool                                    usartTxCompleteIntStatus;

    bool                                    usartRxCompleteIntStatus;

    bool                                    usartErrorIntStatus;

    bool                                    dmaTxChannelIntStatus;

    bool                                    dmaRxChannelIntStatus;

    bool                                    usartInterruptStatus;

    bool                                    dmaInterruptStatus;

    DRV_USART_DATA_BIT                      dataWidth;

} DRV_USART_OBJ;

typedef struct _DRV_USART_CLIENT_OBJ
{
    /* The hardware instance index associated with the client */
    SYS_MODULE_INDEX                    drvIndex;

    /* The IO intent with which the client was opened */
    DRV_IO_INTENT                       ioIntent;

    /* This flags indicates if the object is in use or is available */
    bool                                inUse;

    /* Event handler for this function */
    DRV_USART_BUFFER_EVENT_HANDLER      eventHandler;

    /* Application Context associated with this client */
    uintptr_t                           context;

    /* Client handle assigned to this client object when it was opened */
    DRV_HANDLE                          clientHandle;

} DRV_USART_CLIENT_OBJ;

#endif //#ifndef DRV_USART_LOCAL_H
