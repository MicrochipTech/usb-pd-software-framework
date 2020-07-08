/*******************************************************************************
Policy Engine Firmware update Header

Company:
    Microchip Technology Inc.

File Name:
    policy_enigne_fwup.h

Description:
    This header file contains the data structures, constant definition,
		function prototypes and definitions for Policy Engine firmware update.
*******************************************************************************/
/*******************************************************************************
Copyright ©  [2019] Microchip Technology Inc. and its subsidiaries.

Subject to your compliance with these terms, you may use Microchip software and
any derivatives exclusively with Microchip products. It is your responsibility
to comply with third party license terms applicable to your use of third party
software (including open source software) that may accompany Microchip software.

THIS SOFTWARE IS SUPPLIED BY MICROCHIP "AS IS".  NO WARRANTIES, WHETHER EXPRESS,
IMPLIED OR STATUTORY, APPLY TO THIS SOFTWARE, INCLUDING ANY IMPLIED WARRANTIES
OF NON-INFRINGEMENT, MERCHANTABILITY, AND FITNESS FOR A PARTICULAR PURPOSE. IN
NO EVENT WILL MICROCHIP BE LIABLE FOR ANY INDIRECT, SPECIAL, PUNITIVE,
INCIDENTAL OR CONSEQUENTIAL LOSS, DAMAGE, COST OR EXPENSE OF ANY KIND
WHATSOEVER RELATED TO THE SOFTWARE, HOWEVER CAUSED, EVEN IF MICROCHIP HAS BEEN
ADVISED OF THE POSSIBILITY OR THE DAMAGES ARE FORESEEABLE.  TO THE FULLEST
EXTENT ALLOWED BY LAW, MICROCHIP'S TOTAL LIABILITY ON ALL CLAIMS IN ANY WAY
RELATED TO THIS SOFTWARE WILL NOT EXCEED THE AMOUNT OF FEES, IF ANY, THAT YOU
HAVE PAID DIRECTLY TO MICROCHIP FOR THIS SOFTWARE.
*******************************************************************************/

#ifndef _PE_FWUP_H_
#define _PE_FWUP_H_

// *****************************************************************************
// *****************************************************************************
// Section: Constants
// *****************************************************************************
// *****************************************************************************
/** Enum type of Status Information during Firmware Update */
typedef enum
{
    ePE_FWUP_OK=0u,
    ePE_FWUP_errTarget=1u,
    ePE_FWUP_errWrite=3u,
    ePE_FWUP_errERASE=4u,
    ePE_FWUP_errCHECK_ERASED=5u,
    ePE_FWUP_errPROG=6u,
    ePE_FWUP_errVERIFY=7u,
    ePE_FWUP_errADDRESS=8u,
    ePE_FWUP_errNOTDONE=9u,
    ePE_FWUP_errFIRMWARE=0x0Au,
    ePE_FWUP_errUNKNOWN = 0x0Eu,
    ePE_FWUP_errUNEXPECTED_REQUEST=0x82u,
    ePE_FWUP_errREJECT_PAUSE=0x83u

}ePE_PDFU_RESPONSE_CODE;

/*************Typedef for the Transmit done callback******************/
typedef PRLTxCallback   PE_FwUpdtTxDoneCallBack_Type;

/*******************Defines to get the policy engine status ****************************/
#define PE_FWUP_GET_CURRENT_STATE(PE_PORT_NUM)                 (gasPolicy_Engine[PE_PORT_NUM].ePEState)
#define PE_FWUP_SET_CURRENT_STATE(PE_PORT_NUM,PE_STATE)        (gasPolicy_Engine[PE_PORT_NUM].ePEState = PE_STATE)

#define PE_FWUP_GET_CURRENT_SUBSTATE(PE_PORT_NUM)              (gasPolicy_Engine[PE_PORT_NUM].ePESubState)
#define PE_FWUP_CHANGE_SUBSTATE(PE_PORT_NUM,PE_FWUP_NEXT_STATE)(gasPolicy_Engine[PE_PORT_NUM].ePESubState = PE_FWUP_NEXT_STATE)
#define PE_FWUP_GET_CURRENT_MSGTYPE(PE_PORT_NUM)                NULL

#define PE_FWUP_GET_TIMER_INDEX(PE_PORT_NUM)                   (gasPolicy_Engine[PE_PORT_NUM].u8PETimerID)
#define PE_FWUP_SET_TIMER_INDEX(PE_PORT_NUM,PE_FWUP_TIMERIDX)  (gasPolicy_Engine[PE_PORT_NUM].u8PETimerID = PE_FWUP_TIMERIDX)


/********************* PDFU State machine generic events macro *************************/
#define PE_FWUP_NO_EVT          0x00u
#define PE_FWUP_NEWMSG_EVT      BIT(0)
#define PE_FWUP_TIMEOUT_EVT     BIT(1)
#define PE_FWUP_INTERRUPTION_EVT        BIT(2)


/******************** Protocol version of the PD FW Update Specification****************/
#define PE_FWUP_PROTOCOL_VERSION     0x01u

/******************** Defines for Response buffer Index of specific parameters***********/
/** In which index of the Response buffer the ProtocolVersion is to be filled*/
#define PE_FWUP_RESPBUFF_PROTOCOL_VERSION_INDEX    0x00u
/** In which index of the Response buffer the Response Message Type is to be filled*/
#define PE_FWUP_RESPBUFF_RESPONSE_TYPE_INDEX    0x01u
#define PE_FWUP_RESPBUFF_REQUEST_TYPE_INDEX    0x01u
/** In Which Index of the Response buffer the response status field is filled */
#define PE_FWUP_RESPBUFF_STATUS_INDEX    0x02u


/******************** Defines for Initial/Invalid values specific parameters***********/
/**Defines the invalid timer index of the Policy engine timer*/
#define PE_FWUP_INVALID_TMRIDX  MAX_CONCURRENT_TIMERS
/**Defines the invalid port number*/
#define PE_INVALID_PORT_NUM     0xFFu


/********************** Defines the PDFU Response group ***************************/
/**Deines the No response event group*/
#define PE_FWUP_NO_RESPONSE                 0x00u
/**Deines the response group, where the Response can be filled only by the corresponding State handler*/
#define PE_FWUP_SEND_POPULATED_RESPONSE     0x01u
/**Defines the Response group, where the response is generic for all States. i.e. Unexpected Response*/
#define PE_FWUP_SEND_UNEXPECTEDCMD_RESPONSE    0x02u


/*********************************************************************************/
/*!
*   \def PE_FWUP_xxxx_REQ
*   \brief  The following defines defines the CC FW Update Request message indexes
*/
#define PE_FWUP_EVT_NO_MSG_RCVD                   0x00u
#define PE_FWUP_EVT_GETFWID_REQ                 0x81u
#define PE_FWUP_EVT_PDFUINTIATE_REQ              0x82u
#define PE_FWUP_EVT_PDFUDATA_REQ                 0x83u
#define PE_FWUP_EVT_PDFUDATANR_REQ              0x84u
#define PE_FWUP_EVT_PDFUVALIDATE_REQ             0x85u
#define PE_FWUP_EVT_PDFUABORT_REQ                0x86u
#define PE_FWUP_EVT_PDFUDATAPAUSE_REQ           0x87u
#define PE_FWUP_EVT_VENDORSPECIFIC_REQ           0x8Fu

/*********************************************************************************/
/*!
*   \def PE_FWUP_xxxx_RES
*   \brief  The following defines defines the CC FW Update Response message indexes
*/
#define PE_FWUP_GET_FW_ID_RES                 0x01u
#define PE_FWUP_PDFU_INTIATE_RES              0x02u
#define PE_FWUP_PDFU_DATA_RES                 0x03u
#define PE_FWUP_PDFU_VALIDATE_RES             0x05u
#define PE_FWUP_PDFU_ABORT_REQ                0x06u
#define PE_FWUP_PDFU_DATA_PAUSE_REQ           0x07u
#define PE_FWUP_VENDOR_SPECIFIC_RES           0x7Fu

/*******************************************************************/


/** CCFWUP Response Length defines */
/** Defines the response length of Notify Unexpected request */
#define PE_FWUP_ERRUNEXPT_RESPONSE_LEN         0x03u

/*******************************************************************/

/*! Wait Time defines*/
/*!
 Wait time macro for Reconfigure state ,
 where the current PDFU Initiate request processing takes "Wait time" ms,
 and next request to be issued by the initiator after the specified wait time.
*/
#define PE_FWUP_RECONFIG_WAITTIME    CONFIG_RECONFIG_PHASE_WAITTIME /**in ms*/
/*!
 Wait time macro for Transfer state ,
 where the current PDFU Data request processing takes "Wait time" ms,
 and next request to be issued by the initiator after the specified wait time.
*/
#define PE_FWUP_PDFUDATA_WAITTIME    CONFIG_TRANSFER_PHASE_WAITTIME /**in ms*/
/*!
 Wait time macro for Validation state ,
 where the current PDFU Validate request processing takes "Wait time" ms,
 and next request to be issued by the initiator after the specified wait time.
*/
#define PE_FWUP_VALIDATE_WAITTIME    CONFIG_VALIDATION_PHASE_WAITTIME /**in ms*/

/**Set to 1 if firmware is not updatable.
Otherwise shall be set to 0.*/
#define PE_FWUP_PDFU_SUPPORT          CONFIG_PDFU_SUPPORTED
/**Set to 1 to indicate support for PDFU via USB PD Firmware Update flow.
Otherwise shall be set to 0.*/
#define PE_FWUP_PDFU_VIA_USBPD        CONFIG_PDFU_VIA_USBPD_SUPPORTED
/**Hardware Minor Version*/
#define PE_FWUP_HWMINOR_VERSION       CONFIG_HWMINOR_VERSION
/** Hardware Major Version*/
#define PE_FWUP_HWMAJOR_VERSION       CONFIG_HWMAJOR_VERSION
/**Silicon Base Version*/
#define PE_FWUP_SILICON_VERSION       CONFIG_SILICON_VERSION
/**Image bank for which firmware is requested*/
#define PE_FWUP_UPDATABLE_IMAGEBANK   CONFIG_UPDATABLE_IMAGEBANK_INDEX
/**Set to 1 if PDFU Responder is fully functional during firmware update.
 Otherwise shall be set to 0.**/
#define PE_FWUP_RESPONDER_FULLFUNC    FALSE
/**Set to 1 if unplug during firmware update is safe.
 Otherwise, shall be set to 0.*/
#define PE_FWUP_RESPONSER_UNPLUG      FALSE
/**Set to 1 if Hard Reset required to complete firmware update.
Otherwise shall be set to 0.*/
#define PE_FWUP_HARDRESET_ONPDFU_CMPLT  FALSE

/**Set to 1 if Alternate Modes are available during firmware update.
Otherwise shall be set to 0 if Alternate Modes are not available during firmware update
or are not supported.*/
#define PE_FWUP_RESPONDER_ALTERNATEMODE FALSE

#define PE_FWUP_PDFU_DATA_BLOCK_SIZE    256u

#define PE_FWUP_HWMAJOR_VER_Pos       0x04u

#define PE_FWUP_SILICON_VER_Pos       0x04u

#define PE_FWUP_FLAGS1_BIT0                   0
#define PE_FWUP_FLAGS1_BIT1                   1
#define PE_FWUP_FLAGS1_BIT2                   2
#define PE_FWUP_FLAGS1_BIT3                   3

#define PE_FWUP_FLAGS2_BIT0                   0
#define PE_FWUP_FLAGS2_BIT1                   1

#define PE_FWUP_FLAGS3_BIT0                   0
#define PE_FWUP_FLAGS3_BIT1                   1
#define PE_FWUP_FLAGS3_BIT2                   2
#define PE_FWUP_FLAGS3_BIT3                   3
#define PE_FWUP_FLAGS3_BIT4                   4

#define PE_FWUP_FLAGS4_BIT0                   0
#define PE_FWUP_FLAGS4_BIT1                   1
#define PE_FWUP_FLAGS4_BIT2                   2
#define PE_FWUP_FLAGS4_BIT3                   3

/****************************************************************************/


/*! CCFWUP Resend response defines */
/*!
 Resend response times macro for Reconfigure state ,
 where the response is resent while waiting for PDFU Initiate request
*/
#define PE_FWUP_RECONFIGURERESEND             0x03u
/*!
 Resend response times macro for Transfer state ,
 where the response is resent while waiting for PDFU DATA request
*/
#define PE_FWUP_DATARESEND                    0x03u
/*!
 Resend response times macro for Validation state ,
 where the response is resent while waiting for PDFU Validation request
*/
#define PE_FWUP_VALIDATERESEND                0x03u
/*!
Currently reserved for future usage
*/
#define PE_FWUP_PAUSERESEND                   0x03u

/*****************************************************************************/

/*! Next Request wait timeout */
/*!
    When the timeout for the next expected FW Update request, failing which
    the state machine runs error recovery mechanisms
    -30ms timeout for each request chunk and Response chunk
    -Maximum of 10 Chunks can constitute a Extended message.
    -10 Request chunks & 10 Response chunks with each having timeout of 30ms.
*/
#define PE_FWUP_tPDFUNextRequestRcvd     (30u * 19u)/***/

/*****************************************************************************/
/*! Number of PDFU_DATA_NR messages that can be received by the Responder
following the current PDFU_DATA request*/

#define PE_FWUP_NUM_DATA_NR                   0x00u

/** Initialization macros for Timer/counter*/
#define PE_FWUP_NO_WAIT_TIME                 0x00u
#define PE_FWUP_INFINITE_WAIT_TIME           0xFFu

/*****************************************************************************/
/** CC FW UP State machine specific macros*/
#define PE_FWUP_TRANSFER_PHASE_COMPLETE       0x02u
#define PE_FWUP_TRANSFER_PHASE_PROGRESS       0x01u
#define PE_FWUP_TRANSFER_PHASE_NOTSTARTED     0x00u

#define PE_FWUP_VALIDATION_SUCCESSFUL       0x00u
#define PE_FWUP_VALIDATION_INPROGRESS       0x01u
#define PE_FWUP_VALIDATION_FAILURE          0x02u

/****************************************************************************/


/** Generic Timer Module Defines */
#define PE_FwUpdtStartTimer(u32Timeout_ticks,pfnTimerCallback,pdfwupdt_state,u8PortNum)    PDTimer_Start(u32Timeout_ticks, pfnTimerCallback, u8PortNum, pdfwupdt_state)
#define PE_FwUpdtStopTimer(u8TimerID)                             PDTimer_Kill(u8TimerID)

// *****************************************************************************
// *****************************************************************************
// Section: Data Structures
// *****************************************************************************
// *****************************************************************************
typedef struct MCHP_PSF_STRUCT_PACKED_START
{
    UINT8   u8IsPDFUActive; /** Says if the PDFU session is active or Not*/
    UINT8   u8PDFUPortNum;  /** Says in which port PDFU Session is active*/
    UINT8   u8EventType;    /** This variable stores the timeout event & Msg Reception from the timer module.
                                This variable is shared across the Interrupt context and normal context.
                                Always Indicates the latest event to the PDFU State Machine*/
    UINT8   u8ResendResponseTimes; /** This variable counts the resend number of times
                                    of the Previous response. When the next request is not received within the expected time,
                                    PDFU Responder assumes that the PDFU Initiator is struck with the previous request handling.
                                    Hence PDFU Responder resends the previous response again a Resend number of time.*/

    UINT16  u16PDFUWaitTime; /**This variable stores the timeout value for the next expected request.
                            In each state, the next expected message is different. The wait time calculation depends on,
                            1)	Processing time (Wait Time) for processing the request.
                            2)	Next request reception Timeout. ()
                            Initial value = NULL**/


    UINT16  u16PDFUResponseLength;/** This variable stores the length of the response message for the recently received request message.
                                    Initial value = NULL */
    UINT8*  pu8ResponseBuffer;    /**Pointer to the Buffer where the Response data is stored*/
    UINT32  u32Transmit_TmrID_TxSt; /**After Response Transmission is complete, this variable has the appropriate state Transitions */
    UINT16  u16ExptDataBlockIdx;    /**Stores the Next Expected Data block Index*/
    UINT8   u8TransferPhaseComplete; /** Transfer Phase is completed or not is Stored in this variable*/

}PD_FW_UPDATE_INFO_TYPE;

/*****Structural storage type for the Get_FW_ID message information***************/
typedef struct PE_GET_FW_ID_RESPONSE
{
    UINT8 u8Status;
    UINT16 u16VID;
    UINT16 u16PID;
    UINT8 u8HWVersion;
    UINT8 u8SiVersion;
    UINT16 u16FWVersion1;
    UINT16 u16FWVersion2;
    UINT16 u16FWVersion3;
    UINT16 u16FWVersion4;
    UINT8 u8ImageBank;
    UINT8 u8Flags1;
    UINT8 u8Flags2;
    UINT8 u8Flags3;
    UINT8 u8Flags4;
}PD_FW_UPDATE_GET_FW_ID;

MCHP_PSF_STRUCT_PACKED_END

extern PD_FW_UPDATE_INFO_TYPE gsPdfuInfo;

// *****************************************************************************
// *****************************************************************************
// Section: Interface Routines
// *****************************************************************************
// *****************************************************************************
/**************************************************************************************************
    Function:
        void  PE_FwUpdtInitialize(void)

    Summary:
        This API Initializes the Global Data used in the FW Update state-machine.

    Devices Supported:
        UPD350 REV A

    Description:
        This API Initializes all the Global variables used in the FW Update state-machine.

    Conditions:
		None.

    Input:
        None.

    Return:
        None.

    Remarks:
        None.
**************************************************************************************************/
void  PE_FwUpdtInitialize(void);
/**************************************************************************************************

    Function:
        UINT8 PE_FwUpdtStateMachine(UINT8 u8PortNum,UINT32* pu32Msg_Header, UINT8 *pu8RcvMsgDataBuf,UINT8 *u8pTransmit_DataObj\
        PRLTxCallback Transmit_cb,UINT32* pu32Transmit_TmrID_TxSt)

    Summary:
        PE FW Update State machine Task

    Devices Supported:
        UPD350 REV A

    Description:  This function implements state machine of CC FW Update, handles the necessary
                FW update commands, responses, timeout events.
                (i.e.Performs state transitions and event processing of each state.)
                This function is invoked from Common State machine.
    Input:
        UINT8 u8PortNum - Port in which the PDFU is happening
        UINT32* pu32Msg_Header - Both Input and Output.
                         Contains Input message header value.
                         This function modifies it with, Tx message Header value.
    Output:
        UINT32* pu32Msg_Header - Both Input and Output.
                         Contains Input message header value.
                         This function modifies it with, Tx message Header value.
        UINT8 *pu8RcvMsgDataBuf- Input - Pointer to Receive message data buffer.
        UINT8 *u8pTransmit_DataObj - Output - Pointer to Transmit message data buffer.
        PRLTxCallback Transmit_cb -Output - Call back function
        UINT32* pu32Transmit_TmrID_TxSt -Output - Transmit message State changes.

    Return:
        UINT8 u8IsResponseApplicable - If response is applicable or not.

    Remarks:
        None.
**************************************************************************************************/
UINT8 PE_FwUpdtStateMachine(UINT8 u8PortNum,UINT32* pu32Msg_Header, UINT8 *pu8RcvMsgDataBuf);
/**************************************************************************************************
    Function:
        void  PE_FwUpdtProcessTimerEvent(UINT8 u8PortNum, UINT8 u8PdFwUpdtState);
    Summary:
        PE FW Update Timer callback function

    Devices Supported:
        UPD350 REV A

    Description:  This function is invoked on the Timeout event.

    Input:
        UINT8 u8PortNum - Port in which the PDFU is happening
        UINT8 u8PdFwUpdtState - At what PE State the timer has been started.

    Return:
        None.

    Remarks:
        None.
**************************************************************************************************/
void  PE_FwUpdtProcessTimerEvent(UINT8 u8PortNum, UINT8 u8PdFwUpdtState);
/**************************************************************************************************

    Function:
        void PE_FwUpdtTxDoneCallBack (UINT8 u8PortNum, UINT8 u8TXDoneState,
            UINT8 u8TxDoneSubState, UINT8 u8TxFailedState, UINT8 u8TxFailedSubState);

    Summary:
        PE FW Update Response Completion callback function

    Devices Supported:
        UPD350 REV A

    Description:  This function is invoked on the Response Message Transmission complete Event.

    Input:
        UINT8 u8PortNum - Port in which the PDFU is happening
        UINT8 u8TXDoneState - State change On Successful transmission
        UINT8 u8TxDoneSubState - Sub State change On Successful transmission
        UINT8 u8TxFailedState - State change On Failure transmission
        UINT8 u8TxFailedSubState - Sub State change On Failure transmission
    Return:
        None.

    Remarks:
        None.
**************************************************************************************************/
void PE_FwUpdtTxDoneCallBack (UINT8 u8PortNum, UINT8 u8TXDoneState, UINT8 u8TxDoneSubState, UINT8 u8TxFailedState, UINT8 u8TxFailedSubState);
/**************************************************************************************************

    Function:
        UINT8 PE_FwUpdtProcessEnumerationState(UINT8 u8PortNum,UINT8 u8PDFUMsgType)
	Summary:
		Processes the Enumeration state events like Requests & Timeouts
        and initiates appropriate state transitions.
    Devices Supported:
        UPD350 REV A

    Description:
        *Enumeration state is the Initial state/Idle state of the PD FW update State machine.
        *This function Processes,
            #the Get_FW_ID command and responds with appropriate information.
            #the PDFU_Initiate command and posts response.
            #the invalid commands and notifies the PDFU Initiator.
        *If the PDFU_Initiate is replied with Ready for update, then State transition from Enumeration to Initiate_Complete state.
        *If the PDFU_Initiate is replied with wait time, then state transitions to Initiate_Incomplete state.
    Input:
        u8PortNum - Port in which the PDFU is happening
        u8PDFUMsgType - Msg Type of the newly received PDFU Message
    Return
		UINT8 status of whether Response Applicable for the current request or Not.
**************************************************************************************************/
UINT8 PE_FwUpdtProcessEnumerationState(UINT8 u8PortNum,UINT8 u8PDFUMsgType,UINT8* pu8DataBuffer, UINT16 u16ExtendedMsgHeader);
/**************************************************************************************************

    Function:
        UINT8 PE_FwUpdtProcessReconfigEntryState(UINT8 u8PortNum,UINT8 u8PDFUMsgType)
	Summary:
		This is the intermediate state of Reconfiguration phase, when the Reconfiguration process
        is under process.
    Devices Supported:
        UPD350 REV A

    Description:
        *This function Processes,
            #The PDFU_Initiate command and posts response.
            #The invalid commands and notifies the PDFU Initiator.
        *The PDFU_Initiate is replied with Ready for FW update, then State transition from Reconfigure_entry to Reconfigured state.
    Input:
        u8PortNum - Port in which the PDFU is happening
        u8PDFUMsgType - Msg Type of the newly received PDFU Message
    Return:
		UINT8 status of whether Response Applicable for the current request or Not.
**************************************************************************************************/
UINT8 PE_FwUpdtProcessReconfigEntryState(UINT8 u8PortNum,UINT8 u8PDFUMsgType,UINT8* pu8DataBuffer, UINT16 u16ExtendedMsgHeader);
/**************************************************************************************************
    Function:
        void PE_FwUpdtProcessReconfigureState(UINT8 u8PortNum,UINT8 u8PDFUMsgType,UINT8* pu8DataBuffer)
	Summary:
		Processes the Initiate Completed state events and initiates state transition.
    Devices Supported:
        UPD350 REV A

    Description:
        *This function Processes,
            #The PDFU_Initiate command and posts response.
            #process the first PDFU_DATA request and performs necessary state transitions
            #The invalid commands and notifies the PDFU Initiator.
        *On reception of PDFU_DATA, the State transition from Initiate_Complete to Transfer state.
    Input:
        u8PortNum - Port in which the PDFU is happening
        u8PDFUMsgType - Msg Type of the newly received PDFU Message
        UINT8* pu8DataBuffer - Pointer to Data Buffer.
    Return:
		UINT8 status of whether Response Applicable for the current request or Not.
**************************************************************************************************/
UINT8 PE_FwUpdtProcessReconfigureState(UINT8 u8PortNum,UINT8 u8PDFUMsgType,UINT8* pu8DataBuffer , UINT16 u16ExtendedMsgHeader	);
/**************************************************************************************************
    Function:
        void PE_FwUpdtProcessTransferState(UINT8 u8PortNum,UINT8 u8PDFUMsgType,UINT8* pu8DataBuffer)
	Summary:
        During the Transfer State Image data is transfered through the PDFU_DATA packets.
        Flash is update for every PDFUDATA packet (256 bytes) reception.
        To Immediately Read back from Flash and compare against received.
        and to Post the response.
		(i.e.Processes the Transfer state events and initiates state transition.)
    Devices Supported:
        UPD350 REV A

    Description:
        *Transfer state process the PDFU DATA packets and PDFU DATA NR packets
        *This function Processes,
            #The PDFU_DATA/DATA_NR command and posts responses necessary.
            #The invalid commands and notified to the PDFU Initiator.
            #When the PDFU_DATA transfers are completed, then waits for the PDFU_VALIDATION request
        *On reception of PDFU_Validate command after the PDFU transfers had completed,
        then State transition from Transfer state to Validation state happens.
    Input:
        u8PortNum - Port in which the PDFU is happening
        u8PDFUMsgType - Msg Type of the newly received PDFU Message
        UINT8* pu8DataBuffer - Pointer to Data Buffer.

	Return:
		UINT8 status of whether Response Applicable for the current request or Not.
**************************************************************************************************/
UINT8 PE_FwUpdtProcessTransferState(UINT8 u8PortNum,UINT8 u8PDFUMsgType,UINT8* pu8DataBuffer, UINT16 u16ExtendedMsgHeader);
/**************************************************************************************************
    Function:
        void PE_FwUpdtResetToEnumState(UINT8 u8PortNum)
	Summary:
		Resets the State Machine to SRC Ready or SNK Ready State. (i.e. Enumeration State for PDFU)
    Devices Supported:
        UPD350 REV A

    Description:
        Kill the Timers and Initialize the State Machine Data.
    Input:
        u8PortNum - Port in which the PDFU is active
	Return:
        void
**************************************************************************************************/
void PE_FwUpdtResetToEnumState(UINT8 u8PortNum);
/**************************************************************************************************
    Function:
        UINT8 PE_FwUpdtProcessValidationState(UINT8 u8PortNum,UINT8 u8PDFUMsgType);
	Summary:
        The Validation phase is when the Flashed image is validated and response is sent to Initiator.
    Devices Supported:
        UPD350 REV A

    Description:

    Input:
        u8PortNum - Port in which the PDFU is happening
        u8PDFUMsgType - Msg Type of the newly received PDFU Message

	Return:
		UINT8 status of whether Response Applicable for the current request or Not.
**************************************************************************************************/

UINT8 PE_FwUpdtProcessValidationState(UINT8 u8PortNum,UINT8 u8PDFUMsgType,UINT8* pu8DataBuffer, UINT16 u16ExtendedMsgHeader);

void PE_FwUpdtSetMsgRcvStatus(void);
void PE_FwUpdtClearTimeoutStatus(void);
void PE_FwUpdtSetTimeoutStatus(void);
UINT8 PE_FwUpdtGetTimeoutStatus(void);
UINT8 PE_FwUpdtGetPDFUActivePortNum(void);
UINT16  PE_FwUpdtGetFWIDRequest(void);
extern UINT8 PE_FwUpdtPDFUInitateRequest(UINT8 u8PortNum, UINT8 u8WaitTime);
UINT8 PE_FwUpdtPDFUValidateRequest(UINT8 u8PortNum);
UINT8 PE_FwUpdtPDFUDataRequest(UINT8 u8PortNum,UINT8 *pu8DataBuffer,UINT16 u16ExtendedMsgHeader);
void PE_FwUpdtKillTimer(UINT8   u8PortNum);

#endif  /*_PE_FWUP_H_*/
