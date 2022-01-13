/*******************************************************************************
  USART Driver Implementation.

  Company:
    Microchip Technology Inc.

  File Name:
    drv_usart.c

  Summary:
    Source code for the USART driver dynamic implementation.

  Description:
    This file contains the source code for the dynamic implementation of the
    USART driver.
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

// *****************************************************************************
// *****************************************************************************
// Section: Included Files
// *****************************************************************************
// *****************************************************************************

#include "configuration.h"
#include "driver/usart/drv_usart.h"
#include "drv_usart_local.h"

// *****************************************************************************
// *****************************************************************************
// Section: Global Data
// *****************************************************************************
// *****************************************************************************

/* This is the driver instance object array. */
static DRV_USART_OBJ gDrvUSARTObj[DRV_USART_INSTANCES_NUMBER];

// *****************************************************************************
// *****************************************************************************
// Section: File scope functions
// *****************************************************************************
// *****************************************************************************

static inline uint32_t  _DRV_USART_MAKE_HANDLE(uint16_t token, uint8_t drvIndex, uint8_t index)
{
    return ((token << 16) | (drvIndex << 8) | index);
}

static inline uint16_t _DRV_USART_UPDATE_TOKEN(uint16_t token)
{
    token++;

    if (token >= DRV_USART_TOKEN_MAX)
    {
        token = 1;
    }

    return token;
}

static DRV_USART_CLIENT_OBJ* _DRV_USART_DriverHandleValidate(DRV_HANDLE handle)
{
    /* This function returns the pointer to the client object that is
       associated with this handle if the handle is valid. Returns NULL
       otherwise. */

    uint32_t drvInstance = 0;
    DRV_USART_CLIENT_OBJ* clientObj = NULL;

    if((handle != DRV_HANDLE_INVALID) && (handle != 0))
    {
        /* Extract the drvInstance value from the handle */
        drvInstance = ((handle & DRV_USART_INSTANCE_MASK) >> 8);

        if (drvInstance >= DRV_USART_INSTANCES_NUMBER)
        {
            return (NULL);
        }

        if ((handle & DRV_USART_INDEX_MASK) >= gDrvUSARTObj[drvInstance].nClientsMax)
        {
            return (NULL);
        }

        /* Extract the client index and obtain the client object */
        clientObj = &((DRV_USART_CLIENT_OBJ *)gDrvUSARTObj[drvInstance].clientObjPool)[handle & DRV_USART_INDEX_MASK];

        if ((clientObj->clientHandle != handle) || (clientObj->inUse == false))
        {
            return (NULL);
        }
    }

    return(clientObj);
}

static void _DRV_USART_DisableInterrupts(DRV_USART_OBJ* dObj)
{
    bool interruptStatus;
    const DRV_USART_INTERRUPT_SOURCES* intInfo = dObj->interruptSources;
    const DRV_USART_MULTI_INT_SRC* multiVector = &dObj->interruptSources->intSources.multi;

    interruptStatus = SYS_INT_Disable();

    if (intInfo->isSingleIntSrc == true)
    {
        /* Disable USART interrupt */
        dObj->usartInterruptStatus = SYS_INT_SourceDisable((INT_SOURCE)intInfo->intSources.usartInterrupt);

    }
    else
    {
        /* Disable USART interrupt sources */
        if(multiVector->usartTxReadyInt != -1)
        {
            dObj->usartTxReadyIntStatus = SYS_INT_SourceDisable((INT_SOURCE)multiVector->usartTxReadyInt);
        }

        if(multiVector->usartTxCompleteInt != -1)
        {
            dObj->usartTxCompleteIntStatus = SYS_INT_SourceDisable((INT_SOURCE)multiVector->usartTxCompleteInt);
        }

        if(multiVector->usartRxCompleteInt != -1)
        {
            dObj->usartRxCompleteIntStatus = SYS_INT_SourceDisable((INT_SOURCE)multiVector->usartRxCompleteInt);
        }

        if(multiVector->usartErrorInt != -1)
        {
            dObj->usartErrorIntStatus = SYS_INT_SourceDisable((INT_SOURCE)multiVector->usartErrorInt);
        }

    }

    SYS_INT_Restore(interruptStatus);
}

static void _DRV_USART_EnableInterrupts(DRV_USART_OBJ* dObj)
{
    bool interruptStatus;
    const DRV_USART_INTERRUPT_SOURCES* intInfo = dObj->interruptSources;
    const DRV_USART_MULTI_INT_SRC* multiVector = &dObj->interruptSources->intSources.multi;

    interruptStatus = SYS_INT_Disable();

    if (intInfo->isSingleIntSrc == true)
    {
        /* Enable USART interrupt */
        SYS_INT_SourceRestore((INT_SOURCE)intInfo->intSources.usartInterrupt, dObj->usartInterruptStatus);

    }
    else
    {
        /* Enable USART interrupt sources */
        if(multiVector->usartTxReadyInt != -1)
        {
            SYS_INT_SourceRestore((INT_SOURCE)multiVector->usartTxReadyInt, dObj->usartTxReadyIntStatus);
        }

        if(multiVector->usartTxCompleteInt != -1)
        {
            SYS_INT_SourceRestore((INT_SOURCE)multiVector->usartTxCompleteInt, dObj->usartTxCompleteIntStatus);
        }

        if(multiVector->usartRxCompleteInt != -1)
        {
            SYS_INT_SourceRestore((INT_SOURCE)multiVector->usartRxCompleteInt, dObj->usartRxCompleteIntStatus);
        }

        if(multiVector->usartErrorInt != -1)
        {
            SYS_INT_SourceRestore((INT_SOURCE)multiVector->usartErrorInt, dObj->usartErrorIntStatus);
        }

    }

    SYS_INT_Restore(interruptStatus);
}

static bool _DRV_USART_ResourceLock( DRV_USART_OBJ* dObj )
{
    /* We will allow buffers to be added in the interrupt context of the USART
       driver. But we must make sure that if we are inside interrupt, then we
       should not modify mutex. */
    if(dObj->interruptNestingCount == 0)
    {
        /* Grab a mutex. This is okay because we are not in an interrupt context */
        if(OSAL_MUTEX_Lock(&(dObj->mutexTransferObjects), OSAL_WAIT_FOREVER) == OSAL_RESULT_FALSE)
        {
            return false;
        }
    }

    _DRV_USART_DisableInterrupts(dObj);

    return true;
}

static void _DRV_USART_ResourceUnlock( DRV_USART_OBJ* dObj )
{
    _DRV_USART_EnableInterrupts(dObj);

    if(dObj->interruptNestingCount == 0)
    {
        /* Mutex is never acquired from the interrupt context and hence should
         * never be released if in interrupt context.
         */
        OSAL_MUTEX_Unlock(&(dObj->mutexTransferObjects));
    }
}

static DRV_USART_ERROR _DRV_USART_GetErrorType(
    const uint32_t* remapError,
    uint32_t errorMask
)
{
    uint32_t i;
    DRV_USART_ERROR error = DRV_USART_ERROR_NONE;

    for (i = 0; i < 3; i++)
    {
        if (remapError[i] == errorMask)
        {
            error = (DRV_USART_ERROR)(i+1);
            break;
        }
    }

    return error;
}

static DRV_USART_OBJ* _DRV_USART_GetDriverObj(
    DRV_USART_BUFFER_HANDLE bufferHandle
)
{
    uint32_t drvInstance = 0;

    if (bufferHandle == DRV_USART_BUFFER_HANDLE_INVALID)
    {
        return NULL;
    }

    /* Extract driver instance value from the transfer handle */
    drvInstance = ((bufferHandle & DRV_USART_INSTANCE_MASK) >> 8);

    if(drvInstance >= DRV_USART_INSTANCES_NUMBER)
    {
        return NULL;
    }
    else
    {
        return (DRV_USART_OBJ*)&gDrvUSARTObj[drvInstance];
    }
}

static DRV_USART_BUFFER_OBJ* _DRV_USART_GetTransferObj(
    DRV_USART_BUFFER_HANDLE bufferHandle
)
{
    DRV_USART_OBJ* dObj = NULL;
    uint8_t bufferIndex;

    dObj = _DRV_USART_GetDriverObj(bufferHandle);

    if (dObj == NULL)
    {
        return NULL;
    }

    /* Extract buffer index value from the buffer handle */
    bufferIndex = bufferHandle & DRV_USART_INDEX_MASK;

    if(bufferIndex >= dObj->bufferObjPoolSize)
    {
        return NULL;
    }

    /* Check if the buffer object is still valid */
    if (dObj->bufferObjPool[bufferIndex].bufferHandle == bufferHandle)
    {
        return &dObj->bufferObjPool[bufferIndex];
    }
    else
    {
        /* Buffer handle is stale, buffer object is re-assigned */
        return NULL;
    }
}

static DRV_USART_BUFFER_OBJ* _DRV_USART_FreeTransferObjGet(DRV_USART_CLIENT_OBJ* clientObj)
{
    uint32_t index;
    DRV_USART_OBJ* dObj = (DRV_USART_OBJ* )&gDrvUSARTObj[clientObj->drvIndex];
    DRV_USART_BUFFER_OBJ* pBufferObj = dObj->bufferObjPool;

    for (index = 0; index < dObj->bufferObjPoolSize; index++)
    {
        if (pBufferObj[index].inUse == false)
        {
            pBufferObj[index].inUse = true;
            pBufferObj[index].next = NULL;

            /* Generate a unique buffer handle consisting of an incrementing
             * token counter, driver index and the buffer index.
             */
            pBufferObj[index].bufferHandle = (DRV_USART_BUFFER_HANDLE)_DRV_USART_MAKE_HANDLE(
                dObj->usartTokenCount, (uint8_t)clientObj->drvIndex, index);

            /* Update the token for next time */
            dObj->usartTokenCount = _DRV_USART_UPDATE_TOKEN(dObj->usartTokenCount);

            return &pBufferObj[index];
        }
    }
    return NULL;
}

static bool _DRV_USART_TransferObjAddToList(
    DRV_USART_OBJ* dObj,
    DRV_USART_BUFFER_OBJ* bufferObj,
    DRV_USART_DIRECTION dir
)
{
    DRV_USART_BUFFER_OBJ** pBufferObjList;
    bool isFirstBufferInList = false;

    if (dir == DRV_USART_DIRECTION_TX)
    {
        pBufferObjList = (DRV_USART_BUFFER_OBJ**)&(dObj->transmitObjList);
    }
    else
    {
        pBufferObjList = (DRV_USART_BUFFER_OBJ**)&(dObj->receiveObjList);
    }

    // Is the buffer object list empty?
    if (*pBufferObjList == NULL)
    {
        *pBufferObjList = bufferObj;
        isFirstBufferInList = true;
    }
    else
    {
        // List is not empty. Iterate to the end of the buffer object list
        while (*pBufferObjList != NULL)
        {
            if ((*pBufferObjList)->next == NULL)
            {
                (*pBufferObjList)->next = bufferObj;
                break;
            }
            else
            {
                pBufferObjList = (DRV_USART_BUFFER_OBJ**)&((*pBufferObjList)->next);
            }
        }
    }

    return isFirstBufferInList;
}

static DRV_USART_BUFFER_OBJ* _DRV_USART_TransferObjListGet(
    DRV_USART_OBJ* dObj,
    DRV_USART_DIRECTION dir
)
{
    DRV_USART_BUFFER_OBJ* pBufferObj = NULL;

    // Return the element at the head of the linked list
    if (dir == DRV_USART_DIRECTION_TX)
    {
        pBufferObj = (DRV_USART_BUFFER_OBJ*)dObj->transmitObjList;
    }
    else
    {
        pBufferObj = (DRV_USART_BUFFER_OBJ*)dObj->receiveObjList;
    }
    return pBufferObj;
}

static void _DRV_USART_RemoveTransferObjFromList(
    DRV_USART_OBJ* dObj,
    DRV_USART_DIRECTION dir
)
{
    DRV_USART_BUFFER_OBJ** pBufferObjList;

    if (dir == DRV_USART_DIRECTION_TX)
    {
        pBufferObjList = (DRV_USART_BUFFER_OBJ**)&(dObj->transmitObjList);
    }
    else
    {
        pBufferObjList = (DRV_USART_BUFFER_OBJ**)&(dObj->receiveObjList);
    }

    // Remove the element at the head of the linked list
    if (*pBufferObjList != NULL)
    {
        /* Save the buffer object to be removed. Set the next buffer object as
         * the new head of the linked list. Reset the removed buffer object. */

        DRV_USART_BUFFER_OBJ* temp = *pBufferObjList;
        *pBufferObjList = (*pBufferObjList)->next;
        temp->currentState = DRV_USART_BUFFER_IS_FREE;
        temp->next = NULL;
        temp->inUse = false;
    }
}

static void _DRV_USART_RemoveClientTransfersFromList(
    DRV_USART_OBJ* dObj,
    DRV_USART_CLIENT_OBJ* clientObj,
    DRV_USART_DIRECTION dir
)
{
    DRV_USART_BUFFER_OBJ** pBufferObjList;
    DRV_USART_BUFFER_OBJ* delBufferObj = NULL;

    if (dir == DRV_USART_DIRECTION_TX)
    {
        pBufferObjList = (DRV_USART_BUFFER_OBJ**)&(dObj->transmitObjList);
    }
    else
    {
        pBufferObjList = (DRV_USART_BUFFER_OBJ**)&(dObj->receiveObjList);
    }

    while (*pBufferObjList != NULL)
    {
        // Do not remove the buffer object that is already in process
        if (((*pBufferObjList)->clientHandle == clientObj->clientHandle) &&
            ((*pBufferObjList)->currentState == DRV_USART_BUFFER_IS_IN_QUEUE))
        {
            // Save the node to be deleted off the list
            delBufferObj = *pBufferObjList;
            // Update the current node to point to the deleted node's next node
            *pBufferObjList = (DRV_USART_BUFFER_OBJ*)(*pBufferObjList)->next;
            // Reset the deleted node
            delBufferObj->next = NULL;
            delBufferObj->currentState = DRV_USART_BUFFER_IS_FREE;
            delBufferObj->status = DRV_USART_BUFFER_EVENT_COMPLETE;
            delBufferObj->inUse = false;
        }
        else
        {
            // Move to the next node
            pBufferObjList = (DRV_USART_BUFFER_OBJ**)&((*pBufferObjList)->next);
        }
    }
}

static void _DRV_USART_ReadAbort(DRV_USART_OBJ* dObj, DRV_USART_CLIENT_OBJ* clientObj)
{
    DRV_USART_BUFFER_OBJ* bufferObj = NULL;

    // Get the buffer object at the head of the list
    bufferObj = _DRV_USART_TransferObjListGet(dObj, DRV_USART_DIRECTION_RX);

    if (bufferObj == NULL)
    {
        // List is empty.
        return;
    }

    /* Make sure the ongoing request belongs to the client that called this API and is currently with the PLIB */
    if ((bufferObj->clientHandle == clientObj->clientHandle) && (bufferObj->currentState == DRV_USART_BUFFER_IS_PROCESSING))
    {

        dObj->usartPlib->readAbort();

        /* Free the buffer at the top of the list */
        _DRV_USART_RemoveTransferObjFromList(dObj, DRV_USART_DIRECTION_RX);
    }
}

static bool _DRV_USART_QueuePurge(const DRV_HANDLE handle, DRV_USART_DIRECTION dir)
{
    DRV_USART_OBJ* dObj = NULL;
    DRV_USART_CLIENT_OBJ* clientObj = NULL;

    /* Validate the driver handle */
    clientObj = _DRV_USART_DriverHandleValidate(handle);

    if (clientObj == NULL)
    {
        return false;
    }

    dObj = (DRV_USART_OBJ* )&gDrvUSARTObj[clientObj->drvIndex];

    if (_DRV_USART_ResourceLock(dObj) == false)
    {
        return false;
    }

    if (dir == DRV_USART_DIRECTION_RX)
    {
        /* For read, abort the ongoing read request and then remove the queued requests */
        _DRV_USART_ReadAbort(dObj, clientObj);
    }

    /* Remove any pending read requests in the queue */
    _DRV_USART_RemoveClientTransfersFromList(dObj, clientObj, dir);

    _DRV_USART_ResourceUnlock(dObj);

    return true;
}

static void _DRV_USART_WriteSubmit( DRV_USART_OBJ* dObj )
{
    // Get the buffer object at the top of the list
    DRV_USART_BUFFER_OBJ* bufferObj = _DRV_USART_TransferObjListGet(dObj, DRV_USART_DIRECTION_TX);

    if (bufferObj == NULL)
    {
        // List is empty
        return;
    }

    if (bufferObj->currentState != DRV_USART_BUFFER_IS_IN_QUEUE)
    {
        return;
    }

    bufferObj->currentState = DRV_USART_BUFFER_IS_PROCESSING;

    dObj->usartPlib->write(bufferObj->buffer, bufferObj->size);
}

static void _DRV_USART_ReadSubmit( DRV_USART_OBJ* dObj )
{
    // Get the buffer object at the top of the list
    DRV_USART_BUFFER_OBJ* bufferObj = _DRV_USART_TransferObjListGet(dObj, DRV_USART_DIRECTION_RX);

    if (bufferObj == NULL)
    {
        // List is empty.
        return;
    }

    if (bufferObj->currentState != DRV_USART_BUFFER_IS_IN_QUEUE)
    {
        return;
    }

    bufferObj->currentState = DRV_USART_BUFFER_IS_PROCESSING;

    dObj->usartPlib->read(bufferObj->buffer, bufferObj->size);
}

static void _DRV_USART_BufferQueueTask(
    DRV_USART_OBJ* dObj,
    DRV_USART_DIRECTION direction,
    DRV_USART_BUFFER_EVENT event,
    uint32_t plibErrorMask
)
{
    DRV_USART_CLIENT_OBJ* clientObj = NULL;
    DRV_USART_BUFFER_OBJ* bufferObj = NULL;
    DRV_USART_BUFFER_HANDLE bufferHandle;

    if((dObj->inUse == false) || (dObj->status != SYS_STATUS_READY))
    {
        return;
    }

    // Get the buffer object at the head of the list
    bufferObj = _DRV_USART_TransferObjListGet(dObj, direction);

    // bufferObj can never be NULL. Very likely a false PLIB callback to the driver.
    if (bufferObj == NULL)
    {
        return;
    }

    // Get the client object that owns this buffer
    clientObj = &((DRV_USART_CLIENT_OBJ *)gDrvUSARTObj[((bufferObj->clientHandle & DRV_USART_INSTANCE_MASK) >> 8)].clientObjPool)
                [bufferObj->clientHandle & DRV_USART_INDEX_MASK];

    /* Check if the client that submitted the request is active? */
    if (clientObj->clientHandle == bufferObj->clientHandle)
    {
        bufferObj->status = event;

        if(bufferObj->status == DRV_USART_BUFFER_EVENT_ERROR)
        {
            // Save the error in buffer object. This will be valid until it is
            // read by the application or the buffer object is assigned to a new request,
            // whichever happens first.
            bufferObj->errors = _DRV_USART_GetErrorType(dObj->remapError, plibErrorMask);
            bufferObj->nCount = dObj->usartPlib->readCountGet();
        }
        else
        {
            /* Buffer transfer was successful, hence set completed bytes to
             * requested buffer size */
            bufferObj->nCount = bufferObj->size;
        }

        /* Save the bufferHandle locally before freeing the buffer object */
        bufferHandle = bufferObj->bufferHandle;

        /* Free the completed buffer.
         * This is done before giving callback to allow application to use the freed
         * buffer and queue in a new request from within the callback */

        _DRV_USART_RemoveTransferObjFromList(dObj, direction);

        if((clientObj->eventHandler != NULL))
        {
            dObj->interruptNestingCount++;

            clientObj->eventHandler(event, bufferHandle, clientObj->context);

            dObj->interruptNestingCount--;
        }
    }
    else
    {
        /* Free the completed buffer */
        _DRV_USART_RemoveTransferObjFromList(dObj, direction);
    }

    // Submit the next request (if any) from the queue to the USART PLIB
    if (direction == DRV_USART_DIRECTION_RX)
    {
        _DRV_USART_ReadSubmit(dObj);
    }
    else
    {
        _DRV_USART_WriteSubmit(dObj);
    }
}

static void _DRV_USART_TX_PLIB_CallbackHandler( uintptr_t context )
{
    DRV_USART_OBJ* dObj = (DRV_USART_OBJ* )context;
    uint32_t errorMask = (uint32_t) DRV_USART_ERROR_NONE;

    _DRV_USART_BufferQueueTask(dObj, DRV_USART_DIRECTION_TX, DRV_USART_BUFFER_EVENT_COMPLETE, errorMask);

    return;
}

static void _DRV_USART_RX_PLIB_CallbackHandler( uintptr_t context )
{
    DRV_USART_OBJ* dObj = (DRV_USART_OBJ* )context;
    uint32_t errorMask;

    errorMask = dObj->usartPlib->errorGet();

    if(errorMask == (uint32_t) DRV_USART_ERROR_NONE)
    {
        _DRV_USART_BufferQueueTask(dObj, DRV_USART_DIRECTION_RX, DRV_USART_BUFFER_EVENT_COMPLETE, errorMask);
    }
    else
    {
        _DRV_USART_BufferQueueTask(dObj, DRV_USART_DIRECTION_RX, DRV_USART_BUFFER_EVENT_ERROR, errorMask);
    }

    return;
}


// *****************************************************************************
// *****************************************************************************
// Section: USART Driver Common Interface Implementation
// *****************************************************************************
// *****************************************************************************

SYS_MODULE_OBJ DRV_USART_Initialize(
    const SYS_MODULE_INDEX drvIndex,
    const SYS_MODULE_INIT* const init
)
{
    DRV_USART_OBJ* dObj = NULL;
    DRV_USART_INIT* usartInit = (DRV_USART_INIT *)init ;

    /* Validate the request */
    if(drvIndex >= DRV_USART_INSTANCES_NUMBER)
    {
        return SYS_MODULE_OBJ_INVALID;
    }

    /* Is the driver instance already initialized? */
    if(gDrvUSARTObj[drvIndex].inUse == true)
    {
        return SYS_MODULE_OBJ_INVALID;
    }

    /* Allocate the driver object */
    dObj = &gDrvUSARTObj[drvIndex];

    /* Create the Mutexes needed for RTOS mode. These calls always passes in the
     * non-RTOS mode */
    if(OSAL_MUTEX_Create(&dObj->mutexTransferObjects) != OSAL_RESULT_TRUE)
    {
        return SYS_MODULE_OBJ_INVALID;
    }

    if(OSAL_MUTEX_Create(&dObj->mutexClientObjects) != OSAL_RESULT_TRUE)
    {
        return SYS_MODULE_OBJ_INVALID;
    }

    dObj->inUse                 = true;
    dObj->usartPlib             = usartInit->usartPlib;
    dObj->nClients              = 0;
    dObj->nClientsMax           = usartInit->numClients;
    dObj->clientObjPool         = usartInit->clientObjPool;
    dObj->usartTokenCount       = 1;
    dObj->bufferObjPoolSize     = usartInit->bufferObjPoolSize;
    dObj->bufferObjPool         = (DRV_USART_BUFFER_OBJ*)usartInit->bufferObjPool;
    dObj->transmitObjList       = (DRV_USART_BUFFER_OBJ*)NULL;
    dObj->receiveObjList        = (DRV_USART_BUFFER_OBJ*)NULL;
    dObj->interruptNestingCount = 0;
    dObj->interruptSources      = usartInit->interruptSources;
    dObj->remapDataWidth        = usartInit->remapDataWidth;
    dObj->remapParity           = usartInit->remapParity;
    dObj->remapStopBits         = usartInit->remapStopBits;
    dObj->remapError            = usartInit->remapError;
    dObj->dataWidth             = usartInit->dataWidth;

    /* Register a callback with either DMA or USART PLIB based on configuration.
     * dObj is used as a context parameter, that will be used to distinguish the
     * events for different driver instances. */
    dObj->usartPlib->writeCallbackRegister(_DRV_USART_TX_PLIB_CallbackHandler, (uintptr_t)dObj);

    dObj->usartPlib->readCallbackRegister(_DRV_USART_RX_PLIB_CallbackHandler, (uintptr_t)dObj);

    /* Update the status */
    dObj->status = SYS_STATUS_READY;

    /* Return the object structure */
    return ( (SYS_MODULE_OBJ)drvIndex );
}

SYS_STATUS DRV_USART_Status( SYS_MODULE_OBJ object)
{
    /* Validate the request */
    if( (object == SYS_MODULE_OBJ_INVALID) || (object >= DRV_USART_INSTANCES_NUMBER) )
    {
        return SYS_STATUS_UNINITIALIZED;
    }

    return (gDrvUSARTObj[object].status);
}

DRV_HANDLE DRV_USART_Open(
    const SYS_MODULE_INDEX drvIndex,
    const DRV_IO_INTENT ioIntent
)
{
    DRV_USART_OBJ* dObj = NULL;
    DRV_USART_CLIENT_OBJ* clientObj = NULL;
    uint32_t iClient;

    /* Validate the request */
    if (drvIndex >= DRV_USART_INSTANCES_NUMBER)
    {
        return DRV_HANDLE_INVALID;
    }

    dObj = &gDrvUSARTObj[drvIndex];

    if((dObj->status != SYS_STATUS_READY) || (dObj->inUse == false))
    {
        return DRV_HANDLE_INVALID;
    }

    /* Guard against multiple threads trying to open the driver */
    if (OSAL_MUTEX_Lock(&dObj->mutexClientObjects , OSAL_WAIT_FOREVER ) == OSAL_RESULT_FALSE)
    {
        return DRV_HANDLE_INVALID;
    }

    /* Take care of Exclusive mode intent of driver */
    if(dObj->isExclusive)
    {
        /* This means the another client has opened the driver in exclusive
           mode. So the driver cannot be opened by any other client. */
        OSAL_MUTEX_Unlock( &dObj->mutexClientObjects);
        return DRV_HANDLE_INVALID;
    }

    if((dObj->nClients > 0) && (ioIntent & DRV_IO_INTENT_EXCLUSIVE))
    {
        /* This means the driver was already opened and another driver was
           trying to open it exclusively.  We cannot give exclusive access in
           this case */
        OSAL_MUTEX_Unlock(&dObj->mutexClientObjects);
        return DRV_HANDLE_INVALID;
    }

    for(iClient = 0; iClient != dObj->nClientsMax; iClient++)
    {
        clientObj = &((DRV_USART_CLIENT_OBJ *)dObj->clientObjPool)[iClient];

        if(clientObj->inUse == false)
        {
            /* This means we have a free client object to use */
            clientObj->inUse = true;

            if(ioIntent & DRV_IO_INTENT_EXCLUSIVE)
            {
                /* Set the driver exclusive flag */
                dObj->isExclusive = true;
            }

            dObj->nClients++;

            /* Generate the client handle */
            clientObj->clientHandle = (DRV_HANDLE)_DRV_USART_MAKE_HANDLE(dObj->usartTokenCount, (uint8_t)drvIndex, iClient);

            /* Increment the instance specific token counter */
            dObj->usartTokenCount = _DRV_USART_UPDATE_TOKEN(dObj->usartTokenCount);

            /* We have found a client object and also updated corresponding driver object members, now release the mutex */
            OSAL_MUTEX_Unlock(&(dObj->mutexClientObjects));

            /* This driver will always work in Non-Blocking mode */
            clientObj->ioIntent = (DRV_IO_INTENT)(ioIntent | DRV_IO_INTENT_NONBLOCKING);

            /* Initialize other elements in Client Object */
            clientObj->eventHandler  = NULL;
            clientObj->context       = (uintptr_t)NULL;
            clientObj->drvIndex      = drvIndex;

            return clientObj->clientHandle;
        }
    }

    OSAL_MUTEX_Unlock(&(dObj->mutexClientObjects));

    return DRV_HANDLE_INVALID;
}

void DRV_USART_Close( DRV_HANDLE handle )
{
    DRV_USART_OBJ* dObj = NULL;
    DRV_USART_CLIENT_OBJ* clientObj = NULL;

    /* Validate the driver handle */
    clientObj = _DRV_USART_DriverHandleValidate(handle);

    if (clientObj == NULL)
    {
        return;
    }

    dObj = (DRV_USART_OBJ* )&gDrvUSARTObj[clientObj->drvIndex];

    /* Guard against multiple threads trying to open/close the driver */
    if (OSAL_MUTEX_Lock(&dObj->mutexClientObjects , OSAL_WAIT_FOREVER ) == OSAL_RESULT_FALSE)
    {
        return;
    }

    /* We will be removing the transfers queued by the client. Guard the linked list
     * against interrupts and/or other threads trying to modify the linked list.
     */
    if (_DRV_USART_ResourceLock(dObj) == false)
    {
        return;
    }

    _DRV_USART_RemoveClientTransfersFromList(dObj, clientObj, DRV_USART_DIRECTION_TX);
    _DRV_USART_RemoveClientTransfersFromList(dObj, clientObj, DRV_USART_DIRECTION_RX);

    _DRV_USART_ResourceUnlock(dObj);

    /* Reduce the number of clients */
    dObj->nClients--;

    /* Reset the exclusive flag */
    dObj->isExclusive = false;

    /* Invalidate the client handle */
    clientObj->clientHandle = DRV_HANDLE_INVALID;

    /* De-allocate the client object */
    clientObj->inUse = false;

    OSAL_MUTEX_Unlock(&(dObj->mutexClientObjects));
}

DRV_USART_ERROR DRV_USART_ErrorGet( const DRV_USART_BUFFER_HANDLE bufferHandle )
{
    DRV_USART_OBJ* dObj = NULL;
    DRV_USART_BUFFER_OBJ* bufferObj = NULL;
    DRV_USART_ERROR errors = DRV_USART_ERROR_NONE;

    /* Get USART driver object from bufferHandle */
    dObj = _DRV_USART_GetDriverObj(bufferHandle);

    if (dObj == NULL)
    {
        return errors;
    }

    if (_DRV_USART_ResourceLock(dObj) == false)
    {
        return errors;
    }

    /* Get buffer object from bufferHandle */
    bufferObj = _DRV_USART_GetTransferObj(bufferHandle);

    if (bufferObj != NULL)
    {
        errors = bufferObj->errors;
        /* Clear the errors */
        bufferObj->errors = DRV_USART_ERROR_NONE;
    }

    _DRV_USART_ResourceUnlock(dObj);

    return errors;
}

bool DRV_USART_SerialSetup(
    const DRV_HANDLE handle,
    DRV_USART_SERIAL_SETUP* setup
)
{
    DRV_USART_OBJ* dObj = NULL;
    DRV_USART_CLIENT_OBJ* clientObj = NULL;
    DRV_USART_SERIAL_SETUP setupRemap;
    bool isSuccess = false;

    /* Validate the driver handle */
    clientObj = _DRV_USART_DriverHandleValidate(handle);

    if (clientObj == NULL)
    {
        return isSuccess;
    }

    if (setup == NULL)
    {
        return isSuccess;
    }

    dObj = (DRV_USART_OBJ* )&gDrvUSARTObj[clientObj->drvIndex];

    /* Grab a mutex */
    if(OSAL_MUTEX_Lock(&(dObj->mutexTransferObjects), OSAL_WAIT_FOREVER) == OSAL_RESULT_FALSE)
    {
        return isSuccess;
    }

    setupRemap.dataWidth = (DRV_USART_DATA_BIT)dObj->remapDataWidth[setup->dataWidth];
    setupRemap.parity = (DRV_USART_PARITY)dObj->remapParity[setup->parity];
    setupRemap.stopBits = (DRV_USART_STOP_BIT)dObj->remapStopBits[setup->stopBits];
    setupRemap.baudRate = setup->baudRate;

    if((setupRemap.dataWidth != DRV_USART_DATA_BIT_INVALID) &&
        (setupRemap.parity != DRV_USART_PARITY_INVALID) &&
        (setupRemap.stopBits != DRV_USART_STOP_BIT_INVALID)
    )
    {
        /* Clock source cannot be modified dynamically, so passing the '0' to pick
         * the configured clock source value */
         isSuccess = dObj->usartPlib->serialSetup(&setupRemap, 0);

         if (isSuccess == true)
         {
            dObj->dataWidth = setup->dataWidth;
         }
    }

    OSAL_MUTEX_Unlock(&(dObj->mutexTransferObjects));

    return isSuccess;
}

void DRV_USART_BufferEventHandlerSet(
    const DRV_HANDLE handle,
    const DRV_USART_BUFFER_EVENT_HANDLER eventHandler,
    const uintptr_t context
)
{
    DRV_USART_OBJ* dObj = NULL;
    DRV_USART_CLIENT_OBJ* clientObj = NULL;

    /* Validate the driver handle */
    clientObj = _DRV_USART_DriverHandleValidate(handle);

    if (clientObj == NULL)
    {
        return;
    }

    dObj = (DRV_USART_OBJ* )&gDrvUSARTObj[clientObj->drvIndex];

    if (_DRV_USART_ResourceLock(dObj) == false)
    {
        return;
    }

    clientObj->eventHandler = eventHandler;
    clientObj->context = context;

    _DRV_USART_ResourceUnlock(dObj);
}

static void _DRV_USART_BufferAdd(
    DRV_HANDLE handle,
    void* buffer,
    const size_t size,
    DRV_USART_BUFFER_HANDLE* bufferHandle,
    DRV_USART_DIRECTION dir
)
{
    DRV_USART_OBJ* dObj = NULL;
    DRV_USART_CLIENT_OBJ* clientObj = NULL;
    DRV_USART_BUFFER_OBJ* bufferObj = NULL;

    /* Validate the Request */
    if (bufferHandle == NULL)
    {
        return;
    }

    *bufferHandle = DRV_USART_BUFFER_HANDLE_INVALID;

    if((size == 0) || (buffer == NULL))
    {
        return;
    }

    /* Validate the driver handle */
    clientObj = _DRV_USART_DriverHandleValidate(handle);

    if (clientObj == NULL)
    {
        return;
    }

    dObj = (DRV_USART_OBJ* )&gDrvUSARTObj[clientObj->drvIndex];

    if (_DRV_USART_ResourceLock(dObj) == false)
    {
        return;
    }

    // Get a free buffer object
    bufferObj = _DRV_USART_FreeTransferObjGet(clientObj);

    if(bufferObj == NULL)
    {
        _DRV_USART_ResourceUnlock(dObj);
        return;
    }

    /* Configure the buffer object */
    bufferObj->buffer       = buffer;
    bufferObj->size         = size;
    bufferObj->nCount       = 0;
    bufferObj->clientHandle = handle;
    bufferObj->errors       = DRV_USART_ERROR_NONE;
    bufferObj->currentState = DRV_USART_BUFFER_IS_IN_QUEUE;
    bufferObj->status       = DRV_USART_BUFFER_EVENT_PENDING;

    *bufferHandle = bufferObj->bufferHandle;

    // Add the buffer object to the transfer buffer list
    if (_DRV_USART_TransferObjAddToList(dObj, bufferObj, dir) == true)
    {
        /* This is the first request in the queue, hence initiate a PLIB transfer */
        if (dir == DRV_USART_DIRECTION_TX)
        {
            _DRV_USART_WriteSubmit(dObj);
        }
        else
        {
            _DRV_USART_ReadSubmit(dObj);
        }
    }

    _DRV_USART_ResourceUnlock(dObj);
}

void DRV_USART_WriteBufferAdd(
    DRV_HANDLE handle,
    void* buffer,
    const size_t size,
    DRV_USART_BUFFER_HANDLE* bufferHandle
)
{
    _DRV_USART_BufferAdd(handle, buffer, size, bufferHandle, DRV_USART_DIRECTION_TX);
}

void DRV_USART_ReadBufferAdd(
    DRV_HANDLE handle,
    void* buffer,
    const size_t size,
    DRV_USART_BUFFER_HANDLE* bufferHandle
)
{
    _DRV_USART_BufferAdd(handle, buffer, size, bufferHandle, DRV_USART_DIRECTION_RX);
}

size_t DRV_USART_BufferCompletedBytesGet( DRV_USART_BUFFER_HANDLE bufferHandle )
{
    DRV_USART_OBJ* dObj = NULL;
    DRV_USART_BUFFER_OBJ* bufferObj = NULL;
    size_t processedBytes = DRV_USART_BUFFER_HANDLE_INVALID;

    /* Get USART driver object from bufferHandle */
    dObj = _DRV_USART_GetDriverObj(bufferHandle);

    if (dObj == NULL)
    {
        return processedBytes;
    }

    if (_DRV_USART_ResourceLock(dObj) == false)
    {
        return processedBytes;
    }

    /* Get buffer object from bufferHandle */
    bufferObj = _DRV_USART_GetTransferObj(bufferHandle);

    if (bufferObj != NULL)
    {
        /* Check if the buffer is currently submitted to PLIB/DMA */
        if(bufferObj->currentState == DRV_USART_BUFFER_IS_PROCESSING)
        {
            /* Check if buffer object belongs to transmit or receive list */
            if(dObj->transmitObjList == bufferObj)
            {
                /* Get the number of bytes processed by PLIB. */
                processedBytes = dObj->usartPlib->writeCountGet();
            }
            else if(dObj->receiveObjList == bufferObj)
            {
                /* Get the number of bytes processed by PLIB. */
                processedBytes = dObj->usartPlib->readCountGet();
            }
        }
        else
        {
            /* Buffer is not with PLIB/DMA, so get the nCount of buffer object */
            processedBytes = bufferObj->nCount;
        }
    }

    _DRV_USART_ResourceUnlock(dObj);

    return processedBytes;
}

DRV_USART_BUFFER_EVENT DRV_USART_BufferStatusGet(
    const DRV_USART_BUFFER_HANDLE bufferHandle
)
{
    DRV_USART_OBJ* dObj = NULL;
    uint8_t bufferIndex;
    DRV_USART_BUFFER_EVENT event = DRV_USART_BUFFER_EVENT_HANDLE_INVALID;

    /* Get USART driver object from bufferHandle */
    dObj = _DRV_USART_GetDriverObj(bufferHandle);

    if (dObj == NULL)
    {
        return event;
    }

    if (_DRV_USART_ResourceLock(dObj) == false)
    {
        return event;
    }

    /* Extract transfer buffer index value from the buffer handle */
    bufferIndex = bufferHandle & DRV_USART_INDEX_MASK;

    /* Validate the transferIndex and corresponding request */
    if(bufferIndex < dObj->bufferObjPoolSize)
    {
        if(bufferHandle == dObj->bufferObjPool[bufferIndex].bufferHandle)
        {
            event = dObj->bufferObjPool[bufferIndex].status;
        }
        else
        {
            event = DRV_USART_BUFFER_EVENT_HANDLE_EXPIRED;
        }
    }

    _DRV_USART_ResourceUnlock(dObj);

    return event;
}

bool DRV_USART_WriteQueuePurge( const DRV_HANDLE handle )
{
    return _DRV_USART_QueuePurge(handle, DRV_USART_DIRECTION_TX);
}

bool DRV_USART_ReadQueuePurge( const DRV_HANDLE handle )
{
    return _DRV_USART_QueuePurge(handle, DRV_USART_DIRECTION_RX);
}

bool DRV_USART_ReadAbort(const DRV_HANDLE handle)
{
    DRV_USART_OBJ* dObj = NULL;
    DRV_USART_CLIENT_OBJ* clientObj = NULL;

    /* Validate the driver handle */
    clientObj = _DRV_USART_DriverHandleValidate(handle);

    if (clientObj == NULL)
    {
        return false;
    }

    dObj = (DRV_USART_OBJ* )&gDrvUSARTObj[clientObj->drvIndex];

    if (_DRV_USART_ResourceLock(dObj) == false)
    {
        return false;
    }

    _DRV_USART_ReadAbort(dObj, clientObj);

    // Submit the next request (if any) from the queue to the USART PLIB
    _DRV_USART_ReadSubmit(dObj);

    _DRV_USART_ResourceUnlock(dObj);

    return true;
}