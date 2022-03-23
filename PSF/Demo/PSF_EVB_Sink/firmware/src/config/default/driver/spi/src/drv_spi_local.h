/*******************************************************************************
  SPI Driver Local Data Structures

  Company:
    Microchip Technology Inc.

  File Name:
    drv_spi_local.h

  Summary:
    SPI Driver Local Data Structures

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

#ifndef _DRV_SPI_LOCAL_H
#define _DRV_SPI_LOCAL_H

// *****************************************************************************
// *****************************************************************************
// Section: Included Files
// *****************************************************************************
// *****************************************************************************

#include "osal/osal.h"

// *****************************************************************************
// *****************************************************************************
// Section: Data Type Definitions
// *****************************************************************************
// *****************************************************************************

// *****************************************************************************
/* SPI Driver client Handle Macros

  Summary:
    SPI driver client Handle Macros

  Description:
    client handle related utility macros. SPI client client handle is a combination
    of client index (8-bit), instance index (8-bit) and token (16-bit). The token
    is incremented for every new driver open request.

  Remarks:
    None
*/

#define DRV_SPI_CLIENT_INDEX_MASK               (0x000000FF)
#define DRV_SPI_INSTANCE_INDEX_MASK             (0x0000FF00)
#define DRV_SPI_TOKEN_MASK                      (0xFFFF0000)
#define DRV_SPI_TOKEN_MAX                       (0x0000FFFF)

#define _USE_FREQ_CONFIGURED_IN_CLOCK_MANAGER   0

typedef enum
{
    /* All data was transferred successfully. */
    DRV_SPI_TRANSFER_STATUS_COMPLETE,

    /* There was an error while processing transfer request. */
    DRV_SPI_TRANSFER_STATUS_ERROR,

} DRV_SPI_TRANSFER_STATUS;

// *****************************************************************************
/* SPI Driver Instance Object

  Summary:
    Object used to keep any data required for an instance of the SPI driver.

  Description:
    None.

  Remarks:
    None.
*/

typedef struct
{
    /* Flag to indicate this object is in use  */
    bool                                inUse;

    /* Flag to indicate that driver has been opened Exclusively*/
    bool                                isExclusive;

    /* Keep track of the number of clients that have opened this driver */
    size_t                              nClients;

    /* Maximum number of clients */
    size_t                              nClientsMax;

    /* Memory pool for Client Objects */
    uintptr_t                           clientObjPool;

    /* The status of the driver */
    SYS_STATUS                          status;

    /* PLIB API list that will be used by the driver to access the hardware */
    const DRV_SPI_PLIB_INTERFACE*       spiPlib;


    /* The active client for this driver instance */
    uintptr_t                           activeClient;

    /* This is an instance specific token counter used to generate unique handles */
    uint16_t                            spiTokenCount;

    /* Status of the last data transfer on this driver instance */
    volatile DRV_SPI_TRANSFER_STATUS    transferStatus;

    const uint32_t*                     remapDataBits;

    const uint32_t*                     remapClockPolarity;

    const uint32_t*                     remapClockPhase;

    /* Handle to the client that owns the exclusive use mutex */
    DRV_HANDLE                          exclusiveUseClientHandle;

    bool                                drvInExclusiveMode;

    uint32_t                            exclusiveUseCntr;

    /* Mutex to protect access to PLIB */
    OSAL_MUTEX_DECLARE(transferMutex);

    /* Mutex to protect access to the client object pool */
    OSAL_MUTEX_DECLARE(clientMutex);

    /* Semaphore to wait for data exchange to complete. This is released from ISR */
    OSAL_SEM_DECLARE(transferDone);

    /* Mutex to lock SPI driver instance for exclusive use by a client */
    OSAL_MUTEX_DECLARE(mutexExclusiveUse);

} DRV_SPI_OBJ;

// *****************************************************************************
/* SPI Driver Client Object

  Summary:
    Object used to track a single client.

  Description:
    This object is used to keep the data necesssary to keep track of a single
    client.

  Remarks:
    None.
*/

typedef struct _DRV_SPI_CLIENT_OBJ
{
    /* The hardware instance object associated with the client */
    DRV_SPI_OBJ*                   dObj;

    /* The IO intent with which the client was opened */
    DRV_IO_INTENT                  ioIntent;

    /* This flags indicates if the object is in use or is
     * available
     */
    bool                           inUse;

    /* Client specific setup */
    DRV_SPI_TRANSFER_SETUP         setup;

    /* Indicates whether the client has changed the SPI setup */
    bool                           setupChanged;

    /* Client handle assigned to this client object when it was opened */
    DRV_HANDLE                     clientHandle;

} DRV_SPI_CLIENT_OBJ;

#endif //#ifndef _DRV_SPI_LOCAL_H
