/*******************************************************************************
  USART Driver Interface Header File

  Company:
    Microchip Technology Inc.

  File Name:
    drv_usart.h

  Summary:
    USART Driver Interface Header File

  Description:
    The USART device driver provides a simple interface to manage the USART or
    UART modules on Microchip PIC32 microcontrollers.  This file provides the
    interface definition for the USART driver.
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

#ifndef DRV_USART_H
#define DRV_USART_H

// *****************************************************************************
// *****************************************************************************
// Section: File includes
// *****************************************************************************
// *****************************************************************************
#include <stddef.h>
#include <stdbool.h>
#include "system/system.h"
#include "driver/driver.h"
#include "drv_usart_definitions.h"

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
/* USART Driver Buffer Handle

  Summary:
    Handle identifying a read or write buffer passed to the driver.

  Description:
    A buffer handle value is returned by a call to the DRV_USART_ReadBufferAdd
    or DRV_USART_WriteBufferAdd functions. This handle is associated with the
    buffer passed into the function and it allows the application to track the
    completion of the data from (or into) that buffer.  The buffer handle value
    returned from the "buffer add" function is returned back to the client
    by the "event handler callback" function registered with the driver.

    The buffer handle assigned to a client request expires when the client has
    been notified of the completion of the buffer transfer (after event handler
    function that notifies the client returns) or after the buffer has been
    retired by the driver if no event handler callback was set.

  Remarks:
    None
*/

typedef uintptr_t DRV_USART_BUFFER_HANDLE;

// *****************************************************************************
/* USART Driver Invalid Buffer Handle

  Summary:
    Definition of an invalid buffer handle.

  Description:
    This is the definition of an invalid buffer handle. An invalid buffer handle
    is returned by DRV_USART_ReadBufferAdd and DRV_USART_WriteBufferAdd
    functions if the buffer add request was not successful.

  Remarks:
    None
*/

#define DRV_USART_BUFFER_HANDLE_INVALID /*DOM-IGNORE-BEGIN*/((DRV_USART_BUFFER_HANDLE)(-1))/*DOM-IGNORE-END*/

// *****************************************************************************
/* USART Driver Buffer Events

   Summary
    Identifies the possible events that can result from a buffer add request.

   Description
    This enumeration identifies the possible events that can result from a
    buffer add request caused by the client calling either the
    DRV_USART_ReadBufferAdd or DRV_USART_WriteBufferAdd functions.

   Remarks:
    One of these values is passed in the "event" parameter of the event
    handling callback function that the client registered with the driver by
    calling the DRV_USART_BufferEventHandlerSet function when a buffer
    transfer request is completed.

*/

typedef enum
{
    /* The buffer is pending to be serviced */
    DRV_USART_BUFFER_EVENT_PENDING = 0,

    /* All data from or to the buffer was transferred successfully. */
    DRV_USART_BUFFER_EVENT_COMPLETE = 1,

    /* Transfer Handle given is expired. It means transfer
    is completed but with or without error is not known. */
    DRV_USART_BUFFER_EVENT_HANDLE_EXPIRED = 2,

    /* There was an error while processing the buffer transfer request. */
    DRV_USART_BUFFER_EVENT_ERROR = -1,

    /* Transfer Handle given is invalid */
    DRV_USART_BUFFER_EVENT_HANDLE_INVALID = -2

} DRV_USART_BUFFER_EVENT;

// *****************************************************************************
/* USART Driver Errors

  Summary:
    Defines the different types of errors for USART driver

  Description:
    This data type defines the different types of errors for USART driver.
    DRV_USART_ERROR_NONE : No errors
    DRV_USART_ERROR_OVERRUN : Receive Overflow error
    DRV_USART_ERROR_PARITY : Parity error
    DRV_USART_ERROR_FRAMING : Framing error

  Remarks:
    This structure is implementation specific. It is fully defined in
    drv_usart_definitions.h.
*/

typedef enum _DRV_USART_ERROR DRV_USART_ERROR;

// *****************************************************************************
/* USART Driver Serial Setup Data

  Summary:
    Defines the data required to dynamically set the serial settings.

  Description:
    This data type defines the data required to dynamically set the serial
    settings for the specific USART driver instance.

  Remarks:
    This structure is implementation specific. It is fully defined in
    drv_usart_definitions.h.
*/

typedef struct _DRV_USART_SERIAL_SETUP DRV_USART_SERIAL_SETUP;

// *****************************************************************************
/* USART Driver Initialization Data

  Summary:
    Defines the data required to initialize the USART driver

  Description:
    This data type defines the data required to initialize the
    USART driver.

  Remarks:
    This structure is implementation specific. It is fully defined in
    drv_usart_definitions.h.
*/

typedef struct _DRV_USART_INIT DRV_USART_INIT;

// *****************************************************************************
/* USART Driver Buffer Event Handler Function Pointer

   Summary
    Pointer to a USART Driver Buffer Event handler function

   Description
    This data type defines the required function signature for the USART driver
    buffer event handling callback function. A client must register a pointer
    using the buffer event handling function whose function signature (parameter
    and return value types) match the types specified by this function pointer
    in order to receive buffer related event calls back from the driver.

    The parameters and return values are described here and
    a partial example implementation is provided.

  Parameters:
    event - Identifies the type of event

    bufferHandle - Handle identifying the buffer to which the vent relates

    context - Value identifying the context of the application that
    registered the event handling function.

  Returns:
    None.

  Example:
    <code>
    void APP_MyBufferEventHandler(
        DRV_USART_BUFFER_EVENT event,
        DRV_USART_BUFFER_HANDLE bufferHandle,
        uintptr_t context
    )
    {
        MY_APP_DATA_STRUCT* pAppData = (MY_APP_DATA_STRUCT* ) context;

        switch(event)
        {
            case DRV_USART_BUFFER_EVENT_COMPLETE:

                // Handle the completed buffer.
                break;

            case DRV_USART_BUFFER_EVENT_ERROR:
            default:

                // Handle error.
                break;
        }
    }
    </code>

  Remarks:
    If the event is DRV_USART_BUFFER_EVENT_COMPLETE, it means that the data was
    transferred successfully.

    If the event is DRV_USART_BUFFER_EVENT_ERROR, it means that the data was not
    transferred successfully. The DRV_USART_BufferCompletedBytesGet function can
    be called to find out how many bytes were processed.

    The bufferHandle parameter contains the buffer handle of the buffer that
    associated with the event. And bufferHandle will be valid while the buffer
    request is in the queue and during callback, unless an error occurred.
    After callback returns, the driver will retire the buffer handle.

    The context parameter contains the a handle to the client context,
    provided at the time the event handling function was registered using the
    DRV_USART_BufferEventHandlerSet function.  This context handle value is
    passed back to the client as the "context" parameter.  It can be any value
    necessary to identify the client context or instance (such as a pointer to
    the client's data) instance of the client that made the buffer add request.

    The event handler function executes in the peripheral's interrupt
    context. It is recommended of the application to not perform process
    intensive or blocking operations within this function.

    The DRV_USART_ReadBufferAdd and DRV_USART_WriteBufferAdd functions can
    be called in the event handler to add a buffer to the driver queue. These
    functions can only be called to add buffers to the driver whose event
    handler is running. For example, USART2 driver buffers cannot be added in
    USART1 driver event handler.
*/

typedef void ( *DRV_USART_BUFFER_EVENT_HANDLER )( DRV_USART_BUFFER_EVENT event, DRV_USART_BUFFER_HANDLE bufferHandle, uintptr_t context );

// *****************************************************************************
// *****************************************************************************
// Section: USART Driver System Interface Routines
// *****************************************************************************
// *****************************************************************************

// *****************************************************************************
/* Function:
    SYS_MODULE_OBJ DRV_USART_Initialize
    (
        const SYS_MODULE_INDEX index,
        const SYS_MODULE_INIT * const init
    )

  Summary:
    Initializes the USART instance for the specified driver index.

  Description:
    This routine initializes the USART driver instance for the specified driver
    index, making it ready for clients to open and use it. The initialization
    data is specified by the init parameter. The initialization may fail if the
    number of driver objects allocated are insufficient or if the specified
    driver instance is already initialized. The driver instance index is
    independent of the USART module ID. For example, driver instance 0 can be
    assigned to USART2.

  Precondition:
    None.

  Parameters:
    index - Identifier for the instance to be initialized

    init - Pointer to the init data structure containing any data necessary to
    initialize the driver.

  Returns:
    If successful, returns a valid handle to a driver instance object.
    Otherwise, returns SYS_MODULE_OBJ_INVALID.

  Example:
    <code>
    // The following code snippet shows an example USART driver initialization.

    SYS_MODULE_OBJ              objectHandle;

    const DRV_USART_PLIB_INTERFACE drvUsart0PlibAPI = {
        .readCallbackRegister = (DRV_USART_PLIB_READ_CALLBACK_REG)USART1_ReadCallbackRegister,
        .read = (DRV_USART_PLIB_READ)USART1_Read,
        .readIsBusy = (DRV_USART_PLIB_READ_IS_BUSY)USART1_ReadIsBusy,
        .readCountGet = (DRV_USART_PLIB_READ_COUNT_GET)USART1_ReadCountGet,
        .writeCallbackRegister = (DRV_USART_PLIB_WRITE_CALLBACK_REG)USART1_WriteCallbackRegister,
        .write = (DRV_USART_PLIB_WRITE)USART1_Write,
        .writeIsBusy = (DRV_USART_PLIB_WRITE_IS_BUSY)USART1_WriteIsBusy,
        .writeCountGet = (DRV_USART_PLIB_WRITE_COUNT_GET)USART1_WriteCountGet,
        .errorGet = (DRV_USART_PLIB_ERROR_GET)USART1_ErrorGet,
        .serialSetup = (DRV_USART_PLIB_SERIAL_SETUP)USART1_SerialSetup
    };

    const DRV_USART_INIT drvUsart0InitData = {
        .usartPlib = &drvUsart0PlibAPI,
        .numClients = DRV_USART_CLIENTS_NUMBER_IDX0,
        .clientObjPool = (uintptr_t)&drvUSART0ClientObjPool[0],
        .dmaChannelTransmit = DRV_USART_XMIT_DMA_CH_IDX0,
        .usartTransmitAddress = (void *)&(USART1_REGS->US_THR),
        .dmaChannelReceive = DRV_USART_RCV_DMA_CH_IDX0,
        .usartReceiveAddress = (void *)&(USART1_REGS->US_RHR),
        .bufferObjPoolSize = DRV_USART_QUEUE_SIZE_IDX0,
        .bufferObjPool = (uintptr_t)&drvUSART0BufferObjPool[0],
        .interruptUSART = USART1_IRQn,
        .interruptDMA = XDMAC_IRQn,
        .remapDataWidth = drvUsart0remapDataWidth,
        .remapParity = drvUsart0remapParity,
        .remapStopBits = drvUsart0remapStopBits,
        .remapError = drvUsart0remapError,
    };

    objectHandle = DRV_USART_Initialize(DRV_USART_INDEX_1, (SYS_MODULE_INIT*)&drvUsart0InitData);

    if (objectHandle == SYS_MODULE_OBJ_INVALID)
    {
        // Handle error
    }
    </code>

  Remarks:
    This routine must be called before any other USART routine is called.

    This routine should only be called once during system initialization.
    This routine will NEVER block for hardware access.
*/

SYS_MODULE_OBJ DRV_USART_Initialize( const SYS_MODULE_INDEX index, const SYS_MODULE_INIT* const init );

// *****************************************************************************
/* Function:
    SYS_STATUS DRV_USART_Status( SYS_MODULE_OBJ object )

  Summary:
    Gets the current status of the USART driver module.

  Description:
    This routine provides the current status of the USART driver module.

  Precondition:
    Function DRV_USART_Initialize should have been called before calling this
    function.

  Parameters:
    object - Driver object handle, returned from the
    DRV_USART_Initialize routine

  Returns:
    SYS_STATUS_READY - Initialization have succeeded and the USART is ready for
    additional operations

    SYS_STATUS_UNINITIALIZED - Indicates that the driver has not been initialized

  Example:
    <code>
    SYS_MODULE_OBJ      object;     // Returned from DRV_USART_Initialize
    SYS_STATUS          usartStatus;

    usartStatus = DRV_USART _Status(object);
    if (usartStatus == SYS_STATUS_READY)
    {
        // This means the driver can be opened using the
        // DRV_USART_Open() function.
    }
    </code>

  Remarks:
    A driver can opened only when its status is SYS_STATUS_READY.
*/

SYS_STATUS DRV_USART_Status( SYS_MODULE_OBJ object);

// *****************************************************************************
// *****************************************************************************
// Section: USART Driver Common Client Interface Routines
// *****************************************************************************
// *****************************************************************************

// *****************************************************************************
/* Function:
    DRV_HANDLE DRV_USART_Open
    (
        const SYS_MODULE_INDEX index,
        const DRV_IO_INTENT ioIntent
    )

  Summary:
    Opens the specified USART driver instance and returns a handle to it.

  Description:
    This routine opens the specified USART driver instance and provides a
    handle that must be provided to all other client-level operations to
    identify the caller and the instance of the driver. The ioIntent
    parameter defines how the client interacts with this driver instance.

    Specifying a DRV_IO_INTENT_EXCLUSIVE will cause the driver to provide
    exclusive access to this client. The driver cannot be opened by any
    other client.

  Precondition:
    Function DRV_USART_Initialize must have been called before calling this
    function.

  Parameters:
    index - Identifier for the object instance to be opened

    intent - Zero or more of the values from the enumeration DRV_IO_INTENT
    "ORed" together to indicate the intended use of the driver.
    See function description for details.

  Returns:
    If successful, the routine returns a valid open-instance handle (a number
    identifying both the caller and the module instance).

    If an error occurs, the return value is DRV_HANDLE_INVALID. Error can occur
    - if the number of client objects allocated via DRV_USART_CLIENTS_NUMBER is
      insufficient.
    - if the client is trying to open the driver but driver has been opened
      exclusively by another client.
    - if the driver peripheral instance being opened is not initialized or is
      invalid.
    - if the client is trying to open the driver exclusively, but has already
      been opened in a non exclusive mode by another client.
    - if the driver is not ready to be opened, typically when the initialize
      routine has not completed execution.

  Example:
    <code>
    DRV_HANDLE handle;

    handle = DRV_USART_Open(DRV_USART_INDEX_0, DRV_IO_INTENT_EXCLUSIVE);
    if (handle == DRV_HANDLE_INVALID)
    {
        // Unable to open the driver
        // May be the driver is not initialized or the initialization
        // is not complete.
    }
    </code>

  Remarks:
    The handle returned is valid until the DRV_USART_Close routine is called.
    This routine will NEVER block waiting for hardware. If the requested intent
    flags are not supported, the routine will return DRV_HANDLE_INVALID. This
    function is thread safe in a RTOS application.
*/

DRV_HANDLE DRV_USART_Open( const SYS_MODULE_INDEX index, const DRV_IO_INTENT ioIntent );

// *****************************************************************************
/* Function:
    void DRV_USART_Close( DRV_Handle handle )

  Summary:
    Closes an opened-instance of the USART driver.

  Description:
    This routine closes an opened-instance of the USART driver, invalidating the
    handle. Any buffers in the driver queue that were submitted by this client
    will be removed. A new handle must be obtained by calling DRV_USART_Open
    before the caller may use the driver again.

  Precondition:
    DRV_USART_Open must have been called to obtain a valid opened device handle.

  Parameters:
    handle - A valid open-instance handle, returned from the driver's
    open routine

  Returns:
    None.

  Example:
    <code>
    // 'handle', returned from the DRV_USART_Open

    DRV_USART_Close(handle);

    </code>

  Remarks:
    None.
*/

void DRV_USART_Close( const DRV_HANDLE handle);

// *****************************************************************************
/* Function:
    DRV_USART_ERROR DRV_USART_ErrorGet( const DRV_USART_BUFFER_HANDLE bufferHandle )

   Summary:
    Gets the USART hardware errors associated with the transfer request.

   Description:
    This function returns the errors associated with the given bufferHandle.
    The call to this function also clears all the associated error flags.
    This function can be used for non-DMA buffer transfers only. It cannot be
    used when the USART driver is configured to use DMA.

   Precondition:
    DRV_USART_Open must have been called to obtain a valid opened device handle.

   Parameters:
    bufferHandle - A valid handle to the transfer request

   Returns:
    Errors occurred as listed by DRV_USART_ERROR.
    This function reports multiple USART errors if occurred.

  Example:
    <code>
    // 'bufferHandle', returned from the DRV_USART_ReadBufferAdd

    if (DRV_USART_ErrorGet(bufferHandle) & DRV_USART_ERROR_OVERRUN)
    {
        //Errors are cleared by the driver, take respective action
        //for the overrun error case.
    }
    </code>

  Remarks:
    USART errors are normally associated with the receiver.
    Once the DRV_USART_ErrorGet API is called by the application, the errors
    are cleared (set to DRV_USART_ERROR_NONE) by the driver.
    The errors remain valid only until the buffer object associated with the
    bufferHandle is not assigned to another transfer request; in which case the
    API returns the error status as DRV_USART_ERROR_NONE. Hence, application may
    want to call this routine from the callback routine itself, in case where an
    error is reported by the driver in the callback.
    This function is expected to work in non-DMA mode only.
*/

DRV_USART_ERROR DRV_USART_ErrorGet( const DRV_USART_BUFFER_HANDLE bufferHandle );

// *****************************************************************************
/* Function:
    bool DRV_USART_SerialSetup
    (
        const DRV_HANDLE handle,
        DRV_USART_SERIAL_SETUP* setup
    )

   Summary:
    Sets the USART serial communication settings dynamically.

   Description:
    This function sets the USART serial communication settings dynamically.

   Precondition:
    DRV_USART_Open must have been called to obtain a valid opened device handle.
    The USART transmit or receive transfer status should not be busy.

   Parameters:
    handle - A valid open-instance handle, returned from the driver's
    open routine

    setup - Pointer to the structure containing the serial setup.

   Returns:
    true - Serial setup was updated successfully.
    false - Failure while updating serial setup.

   Example:
    <code>
    // 'handle', returned from the DRV_USART_Open

    DRV_USART_SERIAL_SETUP setup = {
            115200,
            DRV_USART_PARITY_ODD,
            DRV_USART_DATA_8_BIT,
            DRV_USART_STOP_1_BIT
        };

    DRV_USART_SerialSetup(handle, &setup);
    </code>

   Remarks:
    This routine must not be called from the interrupt context.
*/

bool DRV_USART_SerialSetup(const DRV_HANDLE handle, DRV_USART_SERIAL_SETUP* setup);

// *****************************************************************************
// *****************************************************************************
// Section: USART Driver Buffer Queuing Model Interface Routines
// *****************************************************************************
// *****************************************************************************

// *****************************************************************************
/* Function:
    void DRV_USART_BufferEventHandlerSet
    (
        const DRV_HANDLE handle,
        const DRV_USART_BUFFER_EVENT_HANDLER eventHandler,
        const uintptr_t context
    )

  Summary:
    Allows a client to identify a buffer event handling function for the driver
    to call back when queued buffer transfers have finished.

  Description:
    This function allows a client to register a buffer event handling function
    with the driver to call back when queued buffer transfers have finished.
    When a client calls either the DRV_USART_ReadBufferAdd or
    DRV_USART_WriteBufferAdd function, it is provided with a handle identifying
    the buffer that was added to the driver's buffer queue.  The driver will
    pass this handle back to the client by calling "eventHandler" function when
    the buffer transfer has completed.

    The event handler should be set before the client performs any "buffer add"
    operations that could generate events. The event handler once set, persists
    until the client closes the driver or sets another event handler (which
    could be a "NULL" pointer to indicate no callback).

  Precondition:
    DRV_USART_Open must have been called to obtain a valid opened device handle.

  Parameters:
    handle - A valid open-instance handle, returned from the driver's open
    routine.

    eventHandler - Pointer to the event handler function.

    context - The value of parameter will be passed back to the client
    unchanged, when the eventHandler function is called.  It can be used to
    identify any client specific data object that identifies the instance of the
    client module (for example, it may be a pointer to the client module's state
    structure).

  Returns:
    None.

  Example:
    <code>
    // myAppObj is an application specific state data object.
    MY_APP_OBJ myAppObj;

    uint8_t mybuffer[MY_BUFFER_SIZE];
    DRV_USART_BUFFER_HANDLE bufferHandle;

    // myUSARTHandle is the handle returned
    // by the DRV_USART_Open function.

    // Client registers an event handler with driver. This is done once

    DRV_USART_BufferEventHandlerSet(
        myUSARTHandle,
        APP_USARTBufferEventHandler,
        (uintptr_t)&myAppObj
    );

    DRV_USART_ReadBufferAdd(
        myUSARThandle,
        myBuffer,
        MY_BUFFER_SIZE,
        &bufferHandle
    );

    if(bufferHandle == DRV_USART_BUFFER_HANDLE_INVALID)
    {
        // Error handling here
    }

    // Event is received when the buffer is processed.

    void APP_USARTBufferEventHandler(
        DRV_USART_BUFFER_EVENT event,
        DRV_USART_BUFFER_HANDLE handle,
        uintptr_t context
    )
    {
        // The context handle was set to an application specific
        // object. It is now retrievable easily in the event handler.
        MY_APP_OBJ* myAppObj = (MY_APP_OBJ *) context;

        switch(event)
        {
            case DRV_USART_BUFFER_EVENT_COMPLETE:
                // This means the data was transferred.
                break;

            case DRV_USART_BUFFER_EVENT_ERROR:
                // Error handling here.
                break;

            default:
                break;
        }
    }
    </code>

  Remarks:
    If the client does not want to be notified when the queued buffer transfer
    has completed, it does not need to register a callback. This function is
    thread safe when called in a RTOS application.
*/

void DRV_USART_BufferEventHandlerSet( const DRV_HANDLE handle, const DRV_USART_BUFFER_EVENT_HANDLER eventHandler, const uintptr_t context );

// *****************************************************************************
/* Function:
    void DRV_USART_WriteBufferAdd
    (
        const DRV_HANDLE handle,
        void* buffer,
        size_t size,
        DRV_USART_BUFFER_HANDLE* bufferHandle
    );

  Summary:
    Queues a write operation.

  Description:
    This function schedules a non-blocking write operation. The function returns
    with a valid buffer handle in the bufferHandle argument if the write request
    was scheduled successfully.  The function adds the request to the driver
    instance queue and returns immediately. While the request is in the
    queue, the application buffer is owned by the driver and should not be
    modified.  On returning, the bufferHandle parameter may be
    DRV_USART_BUFFER_HANDLE_INVALID for the following reasons:
    - if a buffer could not be allocated to the request because the queue is full
    - if the input buffer handle is NULL
    - if the input buffer pointer is NULL
    - if the buffer size (number of bytes to write) is 0
    - if the driver handle is invalid

    If the requesting client registered an event callback with the driver, the
    driver will issue a DRV_USART_BUFFER_EVENT_COMPLETE event if the buffer was
    processed successfully or a DRV_USART_BUFFER_EVENT_ERROR event if the buffer
    was not processed successfully.

  Precondition:
    DRV_USART_Open must have been called to obtain a valid opened device handle.

  Parameters:
    handle - Handle of the communication channel as return by the
    DRV_USART_Open function.

    buffer - Data to be transmitted.

    size - Buffer size in bytes.

    bufferHandle - Pointer to an argument that will contain the return
    buffer handle.

  Returns:
    The bufferHandle parameter will contain the return buffer handle. This will
    be DRV_USART_BUFFER_HANDLE_INVALID if the function was not successful.

  Example:
    <code>

    MY_APP_OBJ myAppObj;
    uint8_t mybuffer[MY_BUFFER_SIZE];
    DRV_USART_BUFFER_HANDLE bufferHandle;

    // myUSARTHandle is the handle returned
    // by the DRV_USART_Open function.

    DRV_USART_WriteBufferAdd(
        myUSARThandle,
        myBuffer,
        MY_BUFFER_SIZE,
        &bufferHandle
    );

    if(bufferHandle == DRV_USART_BUFFER_HANDLE_INVALID)
    {
        // Error handling here
    }

    // Event is received when
    // the buffer is processed.
    </code>

  Remarks:
    This function is thread safe in a RTOS application. It can be called from
    within the USART Driver Buffer Event Handler that is registered by this
    client. It should not be called in the event handler associated with another
    USART driver instance. It should not otherwise be called directly in an ISR.

*/

void DRV_USART_WriteBufferAdd( const DRV_HANDLE handle, void* buffer, const size_t size, DRV_USART_BUFFER_HANDLE* bufferHandle);

// *****************************************************************************
/* Function:
    void DRV_USART_ReadBufferAdd
    (
        const DRV_HANDLE handle,
        void* buffer,
        const size_t size,
        DRV_USART_BUFFER_HANDLE* bufferHandle
    )

  Summary:
    Queues a read operation.

  Description:
    This function schedules a non-blocking read operation. The function returns
    with a valid buffer handle in the bufferHandle argument if the read request
    was scheduled successfully. The function adds the request to the hardware
    instance receive queue and returns immediately. While the request is in the
    queue, the application buffer is owned by the driver and should not be
    modified. The function returns DRV_USART_BUFFER_HANDLE_INVALID in the
    bufferHandle argument:
    - if a buffer could not be allocated to the request because the queue is full
    - if the input buffer handle is NULL
    - if the input buffer pointer is NULL
    - if the buffer size (number of bytes to read) is 0
    - if the driver handle is invalid

    If the requesting client registered an event callback with the driver, the
    driver will issue a DRV_USART_BUFFER_EVENT_COMPLETE event if the buffer was
    processed successfully of DRV_USART_BUFFER_EVENT_ERROR event if the buffer
    was not processed successfully.

  Precondition:
    DRV_USART_Open must have been called to obtain a valid opened device handle.

  Parameters:
    handle - Handle of the communication channel as returned by the
    DRV_USART_Open function.

    buffer - Buffer where the received data will be stored.

    size - Buffer size in bytes.

    bufferHandle - Pointer to an argument that will contain the return
    buffer handle.

  Returns:
     The buffer handle is returned in the bufferHandle argument. This is
     DRV_USART_BUFFER_HANDLE_INVALID if the request was not successful.

  Example:
    <code>

    MY_APP_OBJ myAppObj;
    uint8_t mybuffer[MY_BUFFER_SIZE];
    DRV_USART_BUFFER_HANDLE bufferHandle;

    // myUSARTHandle is the handle returned
    // by the DRV_USART_Open function.

    DRV_USART_ReadBufferAdd(
        myUSARThandle,
        myBuffer,
        MY_BUFFER_SIZE,
        &bufferHandle
    );

    if(bufferHandle == DRV_USART_BUFFER_HANDLE_INVALID)
    {
        // Error handling here
    }

    // Event is received when the buffer is processed.
    </code>

  Remarks:
    This function is thread safe in a RTOS application. It can be called from
    within the USART Driver Buffer Event Handler that is registered by the
    client. It should not be called in the event handler associated with another
    USART driver instance. It should not be called directly in an ISR.

*/

void DRV_USART_ReadBufferAdd( const DRV_HANDLE handle, void* buffer, const size_t size, DRV_USART_BUFFER_HANDLE* const bufferHandle);

// *****************************************************************************
/* Function:
    size_t DRV_USART_BufferCompletedBytesGet
    (
        DRV_USART_BUFFER_HANDLE bufferHandle
    );

  Summary:
    Returns the number of bytes that have been processed for the
    specified buffer request.

  Description:
    The client can use this function, in a case where the buffer is terminated
    due to an error, to obtain the number of bytes that have been processed.
    Or in any other use case. This function can be used for non-DMA buffer
    transfers only. It cannot be used when the USART driver is configured to
    use DMA.

  Precondition:
    DRV_USART_Open must have been called to obtain a valid opened device handle.

    Either the DRV_USART_ReadBufferAdd or DRV_USART_WriteBufferAdd function
    must have been called and a valid buffer handle returned.

  Parameters:
    bufferhandle - Handle for the buffer of which the processed number of
    bytes to be obtained.

  Returns:
    Returns the number of bytes that have been processed for this buffer.

    Returns DRV_USART_BUFFER_HANDLE_INVALID for an invalid or an expired
    buffer handle.

  Example:
    <code>
    // myAppObj is an application specific object.
    MY_APP_OBJ myAppObj;

    uint8_t mybuffer[MY_BUFFER_SIZE];
    DRV_USART_BUFFER_HANDLE bufferHandle;

    // myUSARTHandle is the handle returned
    // by the DRV_USART_Open function.

    // Client registers an event handler with driver. This is done once
    DRV_USART_BufferEventHandlerSet(
        myUSARTHandle,
        APP_USARTBufferEventHandle,
        (uintptr_t)&myAppObj
    );

    DRV_USART_ReadBufferAdd(
        myUSARThandle,
        myBuffer,
        MY_BUFFER_SIZE,
        bufferHandle
    );

    if(bufferHandle == DRV_USART_BUFFER_HANDLE_INVALID)
    {
        // Error handling here
    }

    // Event Processing Technique. Event is received when
    // the buffer is processed.

    void APP_USARTBufferEventHandler(
        DRV_USART_BUFFER_EVENT event,
        DRV_USART_BUFFER_HANDLE bufferHandle,
        uintptr_t context
    )
    {
        // The context handle was set to an application specific
        // object. It is now retrievable easily in the event handler.

        MY_APP_OBJ* myAppObj = (MY_APP_OBJ *)context;
        size_t processedBytes;

        switch(event)
        {
            case DRV_USART_BUFFER_EVENT_COMPLETE:
                // This means the data was transferred.
                break;

            case DRV_USART_BUFFER_EVENT_ERROR:
                // Error handling here.
                // We can find out how many bytes have been processed in this
                // buffer request prior to the error.
                processedBytes= DRV_USART_BufferCompletedBytesGet(bufferHandle);
                break;

            default:
                break;
        }
    }
    </code>

  Remarks:
    This function is expected to work in non-DMA mode only.
    This function is thread safe when used in a RTOS application.
*/

size_t DRV_USART_BufferCompletedBytesGet( DRV_USART_BUFFER_HANDLE bufferHandle );

// *****************************************************************************
/* Function:
    DRV_USART_BUFFER_EVENT DRV_USART_BufferStatusGet
    (
        const DRV_USART_BUFFER_HANDLE bufferHandle
    )

  Summary:
    Returns the transmit/receive request status.

  Description:
    This function can be used to poll the status of the queued buffer request
    if the application doesn't prefer to use the event handler (callback)
    function to get notified.

  Precondition:
    DRV_USART_Open must have been called to obtain a valid opened device handle.

    Either the DRV_USART_ReadBufferAdd or DRV_USART_WriteBufferAdd function
    must have been called and a valid buffer handle returned.

  Parameters:
    bufferhandle - Handle for the buffer of which the processed number of
    bytes to be obtained.

  Returns:
    Returns either pending, success or error event for the buffer. Pending means
    the buffer is queued but not serviced yet.

  Example:
  <code>
    // myAppObj is an application specific object.
    MY_APP_OBJ myAppObj;

    uint8_t mybuffer[MY_BUFFER_SIZE];
    DRV_USART_BUFFER_HANDLE bufferHandle;
    DRV_USART_BUFFER_EVENT event;

    // myUSARTHandle is the handle returned
    // by the DRV_USART_Open function.

    // Client registers an event handler with driver. This is done once

    DRV_USART_BufferEventHandlerSet(
        myUSARTHandle,
        APP_USARTBufferEventHandle,
        (uintptr_t)&myAppObj
    );

    DRV_USART_ReadBufferAdd(
        myUSARThandle,
        myBuffer,
        MY_BUFFER_SIZE,
        bufferHandle
    );

    if(bufferHandle == DRV_USART_BUFFER_HANDLE_INVALID)
    {
        // Error handling here
    }

    //Check the status of the buffer
    //This call can be used to wait until the buffer is processed.

    while ((event = DRV_USART_BufferStatusGet(bufferHandle)) == DRV_USART_BUFFER_EVENT_PENDING);

    //Buffer is processed, check the event variable to determine if the buffer request
    //is executed successfully or not.
  </code>

  Remarks:
    This function returns error event if the buffer handle is invalid.

*/

DRV_USART_BUFFER_EVENT DRV_USART_BufferStatusGet( const DRV_USART_BUFFER_HANDLE bufferHandle );

// *****************************************************************************
/* Function:
    bool DRV_USART_WriteQueuePurge( const DRV_HANDLE handle )

  Summary:
    Removes all write requests from the queue for the given client. This API does
	not abort the on-going write transfer.

  Description:
    This function removes all the buffer requests from the queue.
    The client can use this function to purge the queue on timeout or to remove
    unwanted stalled buffer requests or in any other use case.

  Precondition:
    DRV_USART_Open must have been called to obtain a valid opened device handle.

  Parameters:
    handle - Handle of the communication channel as returned by the
    DRV_USART_Open function.

  Returns:
    true - Write queue purge is successful.

    false - Write queue purge has failed.

  Example:
    <code>
    // myUSARTHandle is the handle returned by the DRV_USART_Open function.
    // Use DRV_USART_WriteBufferAdd to queue write requests

    // Application timeout function, where remove queued buffers.
    void APP_TimeOut(void)
    {
        if(DRV_USART_WriteQueuePurge(myUSARThandle) == false)
        {
            //Couldn't purge the write queue, try again.
        }
        else
        {
            //Queue purge successful.
        }
    }
    </code>

  Remarks:
    This function is thread safe when used in an RTOS environment.
    Avoid this function call from within the callback.
	This function does not abort the on-going write transfer.
*/

bool DRV_USART_WriteQueuePurge( const DRV_HANDLE handle );

// *****************************************************************************
/* Function:
    bool DRV_USART_ReadQueuePurge( const DRV_HANDLE handle )

  Summary:
    Removes all buffer requests from the queue for the given client and also aborts
	the on-going read request.

  Description:
    This function removes all the buffer requests from the queue and aborts the 
	on-going read request that is submitted to the PLIB.
    The client can use this function to purge the queue on timeout or to remove
    unwanted stalled buffer requests or in any other use case.

  Precondition:
    DRV_USART_Open must have been called to obtain a valid opened device handle.

  Parameters:
    handle - Handle of the communication channel as returned by the
    DRV_USART_Open function.

  Returns:
    true - Read queue purge is successful.

    false - Read queue purge has failed.

  Example:
    <code>
    // myUSARTHandle is the handle returned by the DRV_USART_Open function.
    // Use DRV_USART_ReadBufferAdd to queue read requests

    // Application timeout function, where remove queued buffers.
    void APP_TimeOut(void)
    {
        if(DRV_USART_ReadQueuePurge(myUSARThandle) == false)
        {
            //Couldn't purge the read queue, try again.
        }
        else
        {
            //Queue purge successful.
        }
    }
    </code>

  Remarks:
    This function is thread safe when used in an RTOS environment.
    Avoid this function call from within the callback.
*/

bool DRV_USART_ReadQueuePurge( const DRV_HANDLE handle );

// *****************************************************************************
/* Function:
    bool DRV_USART_ReadAbort(const DRV_HANDLE handle)

  Summary:
    Aborts an on-going read request

  Description:
    This function aborts an on-going read transfer. No callback is given for
    the on-going request being aborted. When USART is configured for non-dma
    transfers, application may call the DRV_USART_BufferCompletedBytesGet()
    API (before calling the DRV_USART_ReadAbort API)to find out how many bytes
    have been received for the on-going read request.

  Precondition:
    DRV_USART_Open must have been called to obtain a valid opened device handle.

  Parameters:
    handle - Handle of the communication channel as return by the
    DRV_USART_Open function.

  Returns:
    true - operation was successful
    false - error in running the API

  Example:
    <code>

    // myUSARTHandle is the handle returned
    // by the DRV_USART_Open function.

    // For non-DMA based transfers DRV_USART_BufferCompletedBytesGet() can be
    // called to find out the number of bytes received before aborting the
    // request.

    uint32_t processedBytes;

    processedBytes= DRV_USART_BufferCompletedBytesGet(bufferHandle);

    DRV_USART_ReadAbort(myUSARTHandle);
    </code>

  Remarks:
    This function is thread safe in a RTOS application.
    Calling this function does not have any impact on the read/write requests
    that may be pending in the transfer queue. To purge the read/write request
    queues call the DRV_USART_ReadQueuePurge() and DRV_USART_WriteQueuePurge()
    APIs.
*/
bool DRV_USART_ReadAbort(const DRV_HANDLE handle);

// *****************************************************************************
// *****************************************************************************
// Section: USART Driver Synchronous(Blocking Model) Transfer Interface Routines
// *****************************************************************************
// *****************************************************************************

// *****************************************************************************
/* Function:
    bool DRV_USART_WriteBuffer
    (
        const DRV_HANDLE handle,
        void* buffer,
        const size_t size
    );

  Summary:
    This is a blocking function that writes data over USART.

  Description:
    This function does a blocking write operation. The function blocks till
    the data write is complete.
    Function will return false to report failure. The failure will occur for
    the following reasons:
    - if the handle is invalid
    - if the input buffer pointer is NULL
    - if the buffer size is 0

  Precondition:
    DRV_USART_Open must have been called to obtain a valid opened device handle.

  Parameters:
    handle - Handle of the communication channel as return by the
    DRV_USART_Open function.

    buffer - Pointer to the data to be transmitted.

    size - Buffer size in bytes.

  Returns:
    true - write is successful
    false - error has occurred

  Example:
    <code>

    MY_APP_OBJ myAppObj;
    uint8_t myBuffer[MY_BUFFER_SIZE];

    // myUSARTHandle is the handle returned
    // by the DRV_USART_Open function.

    if (DRV_USART_WriteBuffer(myUSARTHandle, myBuffer, MY_BUFFER_SIZE) == false)
    {
        // Error handling here
    }
    </code>

  Remarks:
    This function is thread safe in a RTOS application.
    This function should not be called from an interrupt context.
*/
bool DRV_USART_WriteBuffer( const DRV_HANDLE handle, void* buffer, const size_t size);

// *****************************************************************************
/* Function:
    bool DRV_USART_ReadBuffer
    (
        const DRV_HANDLE handle,
        void* buffer,
        const size_t size
    );

  Summary:
    This is a blocking function that reads data over USART.

  Description:
    This function does a blocking read operation. The function blocks till
    the data read is complete or error has occurred during read.
    Function will return false to report failure. The failure will occur for
    the following reasons:
    - if the handle is invalid
    - if the input buffer pointer is NULL
    - if the buffer size is 0
    - Hardware errors

  Precondition:
    DRV_USART_Open must have been called to obtain a valid opened device handle.

  Parameters:
    handle - Handle of the communication channel as return by the
    DRV_USART_Open function.

    buffer - Pointer to the receive buffer.

    size - Buffer size in bytes.

  Returns:
    true - read is successful
    false - error has occurred

  Example:
    <code>

    MY_APP_OBJ myAppObj;
    uint8_t myBuffer[MY_BUFFER_SIZE];

    // myUSARTHandle is the handle returned
    // by the DRV_USART_Open function.

    if (DRV_USART_ReadBuffer(myUSARTHandle, myBuffer, MY_BUFFER_SIZE) == false)
    {
        // Error handling here
    }
    </code>

  Remarks:
    This function is thread safe in a RTOS application.
    This function should not be called from an interrupt context.
*/
bool DRV_USART_ReadBuffer( const DRV_HANDLE handle, void* buffer, const size_t size);

#include "driver/usart/src/drv_usart_local.h"

//DOM-IGNORE-BEGIN
#ifdef __cplusplus
}
#endif
//DOM-IGNORE-END


#endif // #ifndef DRV_USART_H
