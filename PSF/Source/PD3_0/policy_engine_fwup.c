/*******************************************************************************
  Policy Engine Firmware Update Source file

  Company:
    Microchip Technology Inc.

  File Name:
    policy_engine_fwup.c

  Description:
    This file contains the function definitions for Policy Engine Firmware Update
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
/** Header File inclusion */
#include <psf_stdinc.h>

#if (FALSE != INCLUDE_PDFU)
/**Global Variables*/
PD_FW_UPDATE_INFO_TYPE  gsPdfuInfo;

/** Static functions */


typedef UINT8 (*PE_FwUpdtStateProcessType)(UINT8 u8PortNum,UINT8 u8PDFUMsgType,UINT8* pu8DataBuffer, UINT16 u16ExtendedMsgHeader);  /* */

const PE_FwUpdtStateProcessType PE_FwUpdtStateEvtHandler[] = {
    PE_FwUpdtProcessEnumerationState,
    PE_FwUpdtProcessReconfigEntryState,
    PE_FwUpdtProcessReconfigureState,
    PE_FwUpdtProcessTransferState,
    PE_FwUpdtProcessValidationState
};

/* Data Resend Counter */
#define gu8PE_FWUP_INITIATE_RESRESEND_CTR   gsPdfuInfo.u8ResendResponseTimes
#define gu8PE_FWUP_PDFUDATA_RESRESEND_CTR   gsPdfuInfo.u8ResendResponseTimes
#define gu8PE_FWUP_VALIDATE_RESRESEND_CTR   gsPdfuInfo.u8ResendResponseTimes

/**************************************************************************************************
 *  @fn:
        UINT8 PE_FwUpdtInitialize(void)
 *  @brief  Initialization of Global data.
    @param
        Void
    @return:
        Void
**************************************************************************************************/
void PE_FwUpdtInitialize(void)
{
    gsPdfuInfo.u8EventType = PE_FWUP_NO_EVT;
    gsPdfuInfo.u8IsPDFUActive = FALSE;
    gsPdfuInfo.u8PDFUPortNum = SET_TO_ZERO;
    gsPdfuInfo.u8ResendResponseTimes = SET_TO_ZERO;
    gsPdfuInfo.u16PDFUWaitTime = SET_TO_ZERO;
    gsPdfuInfo.u16PDFUResponseLength = SET_TO_ZERO;
    gsPdfuInfo.u8TransferPhaseComplete = PE_FWUP_TRANSFER_PHASE_NOTSTARTED;
    gsPdfuInfo.u16ExptDataBlockIdx = SET_TO_ZERO;
    gsPdfuInfo.pu8ResponseBuffer = &gu8PDFUResBuffer[0];
}

/**************************************************************************************************
 *  @fn:
        UINT8 PE_FwUpdtStateMachine( UINT8 u8PortNum,UINT32* pu32Msg_Header, UINT8 *pu8RcvMsgDataBuf)
 *  @brief  CCFWUP State machine Task
 *  @details  This function implements state machine of CC FW Update.
 *  Performs state transitions and event processing of each state.
 *  This function is invoked from the PD Task.
    @param
        u8PortNum - Port in which the PDFU is happening
        UINT32* pu32Msg_Header - Both Input and Output.
                                 Contains Input message header value.
                                 This function modifies it with, Tx message Header value.
        UINT8 *pu8RcvMsgDataBuf- Input - Pointer to Receive message data buffer.
    @return:
		UINT8 u8IsResponseApplicable - If response is applicable or not.
**************************************************************************************************/
UINT8 PE_FwUpdtStateMachine(
    UINT8   u8PortNum,          /* */
    UINT32  *pu32Msg_Header,    /* */
    UINT8   *pu8RcvMsgDataBuf)  /* */
{
    /*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
    UINT8   u8IsResponseApplicable = PE_FWUP_NO_RESPONSE;
    UINT16  u16Message_Header = (UINT16) * pu32Msg_Header;
    UINT16  u16Extended_Header = (UINT16) (*pu32Msg_Header >> 16);
    UINT8   u8MsgType = PE_FWUP_EVT_NO_MSG_RCVD;
    /*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
    
    if(ePE_FWUP_MANIFEST_SS == PE_FWUP_GET_CURRENT_SUBSTATE(u8PortNum))
    {
        /** During the Manifestation state, Boot from the newly Updated Updateable application*/
        MCHP_PSF_HOOK_BOOT_UPDATABLE_APP();
    }
    
    if (PE_FWUP_NEWMSG_EVT == (PE_FWUP_NEWMSG_EVT & gsPdfuInfo.u8EventType))
    {
        /**Get the Req Message Type of the Received message*/
        u8MsgType = pu8RcvMsgDataBuf[PE_FWUP_RESPBUFF_REQUEST_TYPE_INDEX];

        if(PE_FWUP_EVT_PDFUABORT_REQ == u8MsgType)
        {
            /**1)  When Abort message is received, any running timer is already killed by the Receive handler
            2)  Reset the state to Source ready/ Sink ready state
            3)  Initialize the state machine variables
            4)  There is no response for PDFU_ABORT request => Section: 4.3 Termination*/
            PE_FwUpdtResetToEnumState(u8PortNum);
            PE_FwUpdtInitialize();
        }
        else
        {
            /**For all other PDFU Requests invoke the State specific Event handler*/
            /**
            * Based on the current State of processing,
            * the individual state handling function are invoked
            */
            u8IsResponseApplicable = (PE_FwUpdtStateEvtHandler[((UINT8)PE_FWUP_GET_CURRENT_SUBSTATE(u8PortNum) - (UINT8)ePE_FWUP_ENUMERATION_SS)])(u8PortNum,
                                                                      u8MsgType,
                                                                      pu8RcvMsgDataBuf,
                                                                      u16Extended_Header);
        }
        /** Clear the new message received event */
        gsPdfuInfo.u8EventType &= ~PE_FWUP_NEWMSG_EVT;
    }
    else  if (PE_FWUP_TIMEOUT_EVT == (PE_FWUP_TIMEOUT_EVT & gsPdfuInfo.u8EventType))
    {
        /**- Time out event handler -**/
        if (SET_TO_ZERO != gsPdfuInfo.u8ResendResponseTimes)
        {
            if((FALSE == gasChunkSM [u8PortNum].u8EnableChunkSM) && (FALSE == gasChunkSM [u8PortNum].u8AbortFlag ))
            {
                u8IsResponseApplicable = PE_FWUP_SEND_POPULATED_RESPONSE;
                u8MsgType = gsPdfuInfo.pu8ResponseBuffer[PE_FWUP_RESPBUFF_RESPONSE_TYPE_INDEX];
                gsPdfuInfo.u8ResendResponseTimes--;
                if (SET_TO_ZERO == gsPdfuInfo.u8ResendResponseTimes)
                {
                    PE_FwUpdtResetToEnumState(u8PortNum);
                }
                gsPdfuInfo.u8EventType &= (~PE_FWUP_TIMEOUT_EVT);
            }
            else
            {
                //PRL_ResetChunkSM (u8PortNum);
                gasChunkSM [u8PortNum].u8AbortFlag = TRUE;
            }
        }
        else
        {
          /**Incase of PDFU_DATANR request where the response is not applicable;
            and next request is not received within "tNextReqRcvd * DataResendTimes" time ,
            the firmware shall reset to Enumeration state, and there is no means to initimate the Host*/
            PE_FwUpdtResetToEnumState(u8PortNum);
            PE_FwUpdtInitialize();
            gsPdfuInfo.u8EventType &= (~PE_FWUP_TIMEOUT_EVT);
        }

    }

    if(PE_FWUP_SEND_UNEXPECTEDCMD_RESPONSE == u8IsResponseApplicable)
    {
        gsPdfuInfo.pu8ResponseBuffer[PE_FWUP_RESPBUFF_STATUS_INDEX] = (UINT8)ePE_FWUP_errUNEXPECTED_REQUEST;
        gsPdfuInfo.u16PDFUResponseLength = PE_FWUP_ERRUNEXPT_RESPONSE_LEN;
        PE_FwUpdtResetToEnumState(u8PortNum);
    }

    if (PE_FWUP_NO_RESPONSE != u8IsResponseApplicable)
    {
        gsPdfuInfo.pu8ResponseBuffer[PE_FWUP_RESPBUFF_PROTOCOL_VERSION_INDEX] = PE_FWUP_PROTOCOL_VERSION;
        gsPdfuInfo.pu8ResponseBuffer[PE_FWUP_RESPBUFF_RESPONSE_TYPE_INDEX] = ((~0x80) & u8MsgType);

        /**   Update the Message Header */
        u16Message_Header = PRL_FormSOPTypeMsgHeader(u8PortNum,
                                                     PE_EXT_FW_UPDATE_RESPONSE,
                                                     7,
                                                     PE_EXTENDED_MSG);
        u16Extended_Header = (1u << PRL_EXTMSG_CHUNKED_BIT_POS) | (PRL_EXTMSG_DATA_FIELD_MASK & gsPdfuInfo.u16PDFUResponseLength);
        *pu32Msg_Header = PRL_FORM_COMBINED_MSG_HEADER(u16Extended_Header, u16Message_Header);
        //if(FALSE == gasChunkSM [u8PortNum].u8EnableChunkSM)
        //{
            //gasChunkSM [u8PortNum].u8AbortFlag = TRUE;
          //  PRL_ResetChunkSM (u8PortNum);
        //}
    }

    return u8IsResponseApplicable;
}

/**************************************************************************************************

    @fn:
        void PE_FwUpdtProcessTimerEvent(UINT8 u8PortNum, UINT8 u8PdFwUpdtState)
	@brief:
		Callback function from the PD timer module interrupt.
	@details
		This function sets the local timer event for the CCFW State machine
        to process.
    @param
		u8NotUsed - Parameter not used.
		u8ccfwupState - At what CCFWUP State the timer has been started.
	@return:
		void.
*************************************************************************************************/
void PE_FwUpdtProcessTimerEvent(
    UINT8   u8PortNum,          /* */
    UINT8   u8PdFwUpdtState)    /* */
{
    if ((UINT8)ePE_PDFU_MODE == u8PdFwUpdtState)
    {
        gasPolicyEngine[u8PortNum].u32TimeoutMsgHeader = PRL_IsAnyMsgPendinginPRL(u8PortNum);

        /* Check for Msg Header is SET_TO_ZERO */
        if (SET_TO_ZERO == gasPolicyEngine[u8PortNum].u32TimeoutMsgHeader)
        {
            /* Copy Receive Msg Handler Header to Timeout Header */
            gasPolicyEngine[u8PortNum].u32TimeoutMsgHeader = gasPolicyEngine[u8PortNum].u32MsgHeader;
        }

        gsPdfuInfo.u8EventType |= PE_FWUP_TIMEOUT_EVT;
    }
    else
    {
        gsPdfuInfo.u8EventType &= (~PE_FWUP_TIMEOUT_EVT);
    }

    PE_FWUP_SET_TIMER_INDEX(u8PortNum, PE_FWUP_INVALID_TMRIDX);
}

/**************************************************************************************************

    @fn:
        UINT8 PE_FwUpdtProcessEnumerationState(UINT8 u8PortNum,UINT8 u8PDFUMsgType,UINT8* pu8DataBuffer, UINT16 u16ExtendedMsgHeader)
	@brief:
		Processes the Enumeration state events and initiates state transition.
	@details
        *Enumeration state is the Initial state/Idle state of the PD FW update Statemachine.
        *This function Processes,
            #the Get_FW_ID command and responds with appropriate information.
            #the PDFU_Initiate command and posts response.
            #the invalid commands and notifies the PDFU Initiator.
        *If the PDFU_Initiate is replied with Ready for update, then State transition from Enumeration to Initiate_Complete state.
        *If the PDFU_Initiate is replied with wait time, then state transitions to Initiate_Incomplete state.
	@param
          u8PortNum - Port in which the PDFU is happening
          u8PDFUMsgType - Msg Type of the newly received PDFU Message
          pu8DataBuffer - Pointer to the buffer where received message data is stored.
          u16ExtendedMsgHeader - Extended message header information.
	@return:
		UINT8 status of whether Response Applicable for the current request or Not.
**************************************************************************************************/
UINT8 PE_FwUpdtProcessEnumerationState(UINT8 u8PortNum,UINT8 u8PDFUMsgType,UINT8* pu8DataBuffer, UINT16 u16ExtendedMsgHeader)  /* */
{
    /*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
    UINT8   u8IsResponseApplicable = PE_FWUP_NO_RESPONSE;
    UINT8   u8WaitTime;
    /*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

    switch (u8PDFUMsgType)
    {
    case PE_FWUP_EVT_GETFWID_REQ:
        {
            u8IsResponseApplicable = PE_FWUP_SEND_POPULATED_RESPONSE;

            /** To process the request \a PDFW_ProcessGetFWIDRequest \norm is invoked.
            The response message contents are copied into \a gsPdfuInfo.pu8ResponseBuffer*/
            gsPdfuInfo.u16PDFUResponseLength = PE_FwUpdtGetFWIDRequest();

            PE_FwUpdtResetToEnumState(u8PortNum);
            break;
        }

    /**  (PDFU Initiate Req Received)*/
    case PE_FWUP_EVT_PDFUINTIATE_REQ:
        {
            /** Get the Configured Amount of delay required for processing PDFU Initiate request*/
            u8WaitTime = PE_FWUP_RECONFIG_WAITTIME;
            u8IsResponseApplicable =PE_FwUpdtPDFUInitateRequest(u8PortNum, u8WaitTime);
            gsPdfuInfo.u16ExptDataBlockIdx = SET_TO_ZERO;
            break;
        }

    case PE_FWUP_EVT_PDFUDATAPAUSE_REQ:
        /** When Data Pause request is received Ignore the command */
        break;

    case PE_FWUP_EVT_PDFUDATANR_REQ:
        /** When Data NR request is received Ignore the command */
        break;

    case PE_FWUP_EVT_VENDORSPECIFIC_REQ:
        /**When Vendor specific command received; ignore */
        break;

    case PE_FWUP_EVT_PDFUDATA_REQ:
    case PE_FWUP_EVT_PDFUVALIDATE_REQ:
    /** When PDFU_DATA, PDFU_VALIDATE commands are received Notify as unexpected */
    default:
        u8IsResponseApplicable = PE_FWUP_SEND_UNEXPECTEDCMD_RESPONSE;
        /** For any other invalid command Ids - Notify as unexpected */

        break;
    }

    return u8IsResponseApplicable;
}

/**************************************************************************************************

    @fn:
        void PE_FwUpdtTxDoneCallBack(
            UINT8   u8PortNum,          
            UINT8   u8TXDoneState,      
            UINT8   u8TxDoneSubState,   
            UINT8   u8TxFailedState,    
            UINT8   u8TxFailedSubState)

    @brief:
            Transmission completed callback invoked from the protocol layer
            -Invoked after the GoodCRC is received for the posted response 


    @param
        u8PortNum - Port in which the PDFU is happening
        u8TXDoneState -   State change when Transmission completed Successfully    
        u8TxDoneSubState -  Sub-State change when Transmission completed Successfully
        u8TxFailedState -  State change when Transmission completed with Failure
        u8TxFailedSubState -Sub-State change when Transmission completed Failure

    @return
            
**************************************************************************************************/
void PE_FwUpdtTxDoneCallBack(
    UINT8   u8PortNum,          /* */
    UINT8   u8TXDoneState,      /* */
    UINT8   u8TxDoneSubState,   /* */
    UINT8   u8TxFailedState,    /* */
    UINT8   u8TxFailedSubState) /* */
{

    if(gasPRL [u8PortNum].u8TxStateISR != PRL_TX_DONE_ST)
    {   /** The response has not been sent */
        gsPdfuInfo.u8EventType |= PE_FWUP_TIMEOUT_EVT;
        gasPolicyEngine[u8PortNum].u32TimeoutMsgHeader = 0x00;
        gsPdfuInfo.u8ResendResponseTimes++;
    }
    else
    {
        if ((PE_FWUP_NO_WAIT_TIME != gsPdfuInfo.u16PDFUWaitTime)&&(ePE_PDFU_MODE == PE_FWUP_GET_CURRENT_STATE(u8PortNum)))
        {
            PE_FWUP_SET_TIMER_INDEX(u8PortNum,
                                    PE_FwUpdtStartTimer(gsPdfuInfo.u16PDFUWaitTime,
                                                        PE_FwUpdtProcessTimerEvent,
                                                        (UINT8)PE_FWUP_GET_CURRENT_STATE(u8PortNum),
                                                        u8PortNum));
        }
    }
  
    if (PD_ROLE_SOURCE == DPM_GET_CURRENT_POWER_ROLE(u8PortNum))
    {
        if ((UINT8)ePE_SRC_READY_END_AMS_SS == u8TxDoneSubState)
            PE_FwUpdtInitialize();
    }
    else
    {
        if ((UINT8)ePE_SNK_READY_IDLE_SS == u8TxDoneSubState)
            PE_FwUpdtInitialize();
    }

    PE_FWUP_SET_CURRENT_STATE(u8PortNum, (ePolicyState) u8TXDoneState);
    PE_FWUP_CHANGE_SUBSTATE(u8PortNum, (ePolicySubState) u8TxDoneSubState);
    

}

/**************************************************************************************************

    @fn:
    UINT8 PE_FwUpdtProcessReconfigEntryState(UINT8 u8PortNum,UINT8 u8PDFUMsgType,UINT8* pu8DataBuffer, UINT16 u16ExtendedMsgHeader)

    @brief:
            Processes the Initiate Incomplete state events and initiates state transition.

    @details
    *This function Processes,
        #The PDFU_Initiate command and posts response.
        #The invalid commands and notifies the PDFU Initiator.
    *The PDFU_Initiate is replied with Ready for update, then State transition from Initiate_incomplete to Initiate_Complete state.

    @param
    u8PortNum - Port in which the PDFU is happening
    u8PDFUMsgType - Msg Type of the newly received PDFU Message
    pu8DataBuffer - Pointer to the buffer where received message data is stored.
    u16ExtendedMsgHeader - Extended message header information.

    @return:
            UINT8 status of whether Response Applicable for the current request or Not.
**************************************************************************************************/
UINT8 PE_FwUpdtProcessReconfigEntryState(UINT8 u8PortNum,UINT8 u8PDFUMsgType,UINT8* pu8DataBuffer, UINT16 u16ExtendedMsgHeader)  /* */
{
    /*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
    UINT8   u8IsResponseApplicable = PE_FWUP_NO_RESPONSE;
    UINT8   u8WaitTime;
    /*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

    switch (u8PDFUMsgType)
    {
    case PE_FWUP_EVT_PDFUINTIATE_REQ:
        /**  (PDFU Initiate Req Received)*/
        u8WaitTime = PE_FWUP_NO_WAIT_TIME;
        u8IsResponseApplicable =PE_FwUpdtPDFUInitateRequest(u8PortNum, u8WaitTime);
        break;

    case PE_FWUP_EVT_PDFUDATAPAUSE_REQ:
        /** When Data Pause request is received Ignore the command */


    case PE_FWUP_EVT_PDFUDATANR_REQ:
        /** When Data NR request is received Ignore the command */


    case PE_FWUP_EVT_VENDORSPECIFIC_REQ:
        /**When Vendor specific message received, ignore */
        if (PE_FWUP_NO_WAIT_TIME != gsPdfuInfo.u16PDFUWaitTime)
        {
            /*Resend response times to be modified*/
            gu8PE_FWUP_INITIATE_RESRESEND_CTR = PE_FWUP_RECONFIGURERESEND;
            PE_FWUP_SET_TIMER_INDEX(u8PortNum,
                                    PE_FwUpdtStartTimer(gsPdfuInfo.u16PDFUWaitTime,
                                                        PE_FwUpdtProcessTimerEvent,
                                                        (UINT8)PE_FWUP_GET_CURRENT_STATE(u8PortNum),
                                                        u8PortNum));
        }        
        break;

    case PE_FWUP_EVT_PDFUDATA_REQ:
    case PE_FWUP_EVT_GETFWID_REQ:
    case PE_FWUP_EVT_PDFUVALIDATE_REQ:
        /** When PDFU_DATA, PDFU_VALIDATE commands are received Notify as unexpected */
        u8IsResponseApplicable = PE_FWUP_SEND_UNEXPECTEDCMD_RESPONSE;
        break;
    }

    return u8IsResponseApplicable;
}

/**************************************************************************************************

    @fn
    UINT8 PE_FwUpdtProcessReconfigureState(UINT8 u8PortNum,UINT8 u8PDFUMsgType,UINT8* pu8DataBuffer,UINT16  u16ExtendedMsgHeader)

    @brief
            Processes the Initiate Completed state events and initiates state transition.

    @details
    *This function Processes,
        #The PDFU_Initiate command and posts response.
        #process the first PDFU_DATA request and performs necessary state transitions
        #The invalid commands and notifies the PDFU Initiator.
    *On reception of PDFU_DATA, the State transition from Initiate_Complete to Transfer state.

    @param
    u8PortNum - Port in which the PDFU is happening
    u8PDFUMsgType - Msg Type of the newly received PDFU Message
    pu8DataBuffer - Pointer to the buffer where received message data is stored.
    u16ExtendedMsgHeader - Extended message header information.

    @return:
            UINT8 status of whether Response Applicable for the current request or Not.
**************************************************************************************************/
UINT8 PE_FwUpdtProcessReconfigureState(
    UINT8   u8PortNum,      /* */
    UINT8   u8PDFUMsgType,  /* */
    UINT8   *pu8DataBuffer, /* */
    UINT16  u16ExtendedMsgHeader)   /* */
{
    /*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
    UINT8   u8IsResponseApplicable = PE_FWUP_NO_RESPONSE;
    UINT8   u8WaitTime;
    /*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

    switch (u8PDFUMsgType)
    {
    case PE_FWUP_EVT_PDFUINTIATE_REQ:
        /**  (PDFU Initiate Req Received)*/
        u8WaitTime = PE_FWUP_NO_WAIT_TIME;
        u8IsResponseApplicable = PE_FwUpdtPDFUInitateRequest(u8PortNum, u8WaitTime);
        break;

    case PE_FWUP_EVT_PDFUDATA_REQ:
        /**  (PDFU_DATA Req Received)*/
        u8IsResponseApplicable = PE_FwUpdtPDFUDataRequest(u8PortNum, pu8DataBuffer, u16ExtendedMsgHeader);
        break;

    case PE_FWUP_EVT_PDFUDATAPAUSE_REQ:
        /** When Data Pause request is received Ignore the command */
        

    case PE_FWUP_EVT_PDFUDATANR_REQ:
        /** When Data NR request is received Ignore the command */
        

    case PE_FWUP_EVT_VENDORSPECIFIC_REQ:
        /**Vendor specific message handling */
        if (PE_FWUP_NO_WAIT_TIME != gsPdfuInfo.u16PDFUWaitTime)
        {
            /*Resend response times to be modified*/
            gu8PE_FWUP_INITIATE_RESRESEND_CTR = PE_FWUP_RECONFIGURERESEND;
            PE_FWUP_SET_TIMER_INDEX(u8PortNum,
                                    PE_FwUpdtStartTimer(gsPdfuInfo.u16PDFUWaitTime,
                                                        PE_FwUpdtProcessTimerEvent,
                                                        (UINT8)PE_FWUP_GET_CURRENT_STATE(u8PortNum),
                                                        u8PortNum));
        }            
        break;

    case PE_FWUP_EVT_GETFWID_REQ:
    case PE_FWUP_EVT_PDFUVALIDATE_REQ:
    default:
        /** When GET_FW_ID, PDFU_VALIDATE commands are received Notify as unexpected */
        u8IsResponseApplicable = PE_FWUP_SEND_UNEXPECTEDCMD_RESPONSE;
        break;
    }

    return u8IsResponseApplicable;
}

/**************************************************************************************************
    @fn
    UINT8 PE_FwUpdtProcessTransferState(UINT8 u8PortNum,UINT8 u8PDFUMsgType,UINT8* pu8DataBuffer,UINT16  u16ExtendedMsgHeader)

    @brief
            Processes the Transfer state events and initiates state transition.

    @details
    *Transfer state process the PDFU DATA packets and PDFU DATA NR packets
    *This function Processes,
        #The PDFU_DATA/DATA_NR command and posts responses necessary.
        #The invalid commands and notified to the PDFU Initiator.
        #When the PDFU_DATA transfers are completed, then waits for the PDFU_VALIDATION request
    *On reception of PDFU_Validate command after the PDFU transfers had completed,
    then State transition from Transfer state to Validation state happens.

    @param
    u8PortNum - Port in which the PDFU is happening
    u8PDFUMsgType - Message Type of the newly received PDFU Message
    pu8DataBuffer - Pointer to the buffer where received message data is stored.
    u16ExtendedMsgHeader - Extended message header information.

    @return:
            UINT8 status of whether Response Applicable for the current request or Not.
**************************************************************************************************/
UINT8 PE_FwUpdtProcessTransferState(
    UINT8   u8PortNum,      /* */
    UINT8   u8PDFUMsgType,  /* */
    UINT8   *pu8DataBuffer, /* */
    UINT16  u16ExtendedMsgHeader)   /* */
{
    /*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
    UINT8   u8IsResponseApplicable = (UINT8)PE_FWUP_NO_RESPONSE;
    UINT8   u8Status = (UINT8)ePE_FWUP_OK;
    /*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

    switch (u8PDFUMsgType)
    {
    case PE_FWUP_EVT_PDFUDATA_REQ:
        /**  (PDFU Data Req Received)*/

        u8IsResponseApplicable = PE_FwUpdtPDFUDataRequest(u8PortNum, pu8DataBuffer, u16ExtendedMsgHeader);
        break;

    case PE_FWUP_EVT_PDFUDATAPAUSE_REQ:
        u8IsResponseApplicable = PE_FWUP_SEND_POPULATED_RESPONSE;
        gsPdfuInfo.pu8ResponseBuffer[PE_FWUP_RESPBUFF_STATUS_INDEX] = (UINT8)ePE_FWUP_errREJECT_PAUSE;
        gsPdfuInfo.u16PDFUResponseLength = PE_FWUP_ERRUNEXPT_RESPONSE_LEN;
        if (PE_FWUP_TRANSFER_PHASE_COMPLETE != gsPdfuInfo.u8TransferPhaseComplete)
        {
            /**Continue the transfer phase, if the PDFU Initiator is understanding that Responder
            is not supporting the Pause and continuing the Transfer phase*/
            gsPdfuInfo.u16PDFUWaitTime = (PE_FWUP_tPDFUNextRequestRcvd + PE_FWUP_PDFUDATA_WAITTIME);
            gu8PE_FWUP_PDFUDATA_RESRESEND_CTR = PE_FWUP_DATARESEND;
        }
        else
        {
            gsPdfuInfo.u16PDFUWaitTime = (PE_FWUP_tPDFUNextRequestRcvd + PE_FWUP_VALIDATE_WAITTIME);
            gu8PE_FWUP_VALIDATE_RESRESEND_CTR = PE_FWUP_VALIDATERESEND;
        }

        /**Ignore TBD*/
        break;

    case PE_FWUP_EVT_PDFUDATANR_REQ:
        /** Check if Firmware update is Progress*/
        if (PE_FWUP_TRANSFER_PHASE_COMPLETE != gsPdfuInfo.u8TransferPhaseComplete)
        {
            u8Status = MCHP_PSF_HOOK_PROGRAM_FWBLOCK(pu8DataBuffer, \
                        (PRL_GET_DATA_SIZE(u16ExtendedMsgHeader) - 4u));

            if ((UINT8)ePE_FWUP_OK == u8Status)
            {
              /** For the PDFU_DATANR request the maximum wait-time for the next PDFU_DATANR is defined in the below statement;
                On expiry the this timer, the PDFU Responder will exit the PDFU flow.*/
                gsPdfuInfo.u16PDFUWaitTime = (PE_FWUP_tPDFUNextRequestRcvd * PE_FWUP_DATARESEND);
                PE_FWUP_SET_TIMER_INDEX(u8PortNum,
                                        PE_FwUpdtStartTimer(gsPdfuInfo.u16PDFUWaitTime,
                                                            PE_FwUpdtProcessTimerEvent,
                                                            (UINT8)PE_FWUP_GET_CURRENT_SUBSTATE(u8PortNum),
                                                            u8PortNum));
                gu8PE_FWUP_PDFUDATA_RESRESEND_CTR = SET_TO_ZERO; /**NR requests doesnt carry any response*/
            }
            else
            {
                /*
                 * Unexpectedly PDFU_DATA_NR is received more than expected

                 */

                /**
                        Other errors like DataBlock index out of bound
                 */
                PE_FwUpdtResetToEnumState(u8PortNum);
                PE_FwUpdtInitialize();

            }
        }
        else
        {

        }
        break;

    case PE_FWUP_EVT_PDFUVALIDATE_REQ:
        /** (PDFU_VALIDATE request received)*/
        if (PE_FWUP_TRANSFER_PHASE_COMPLETE == gsPdfuInfo.u8TransferPhaseComplete)
        {
            u8IsResponseApplicable = PE_FwUpdtPDFUValidateRequest(u8PortNum);
        }
        else
        {
            /** Firmware image transfer is not complete */

            /** Notify as unexpected */
          u8IsResponseApplicable = PE_FWUP_SEND_UNEXPECTEDCMD_RESPONSE;
        }
        break;

    case PE_FWUP_EVT_VENDORSPECIFIC_REQ:
        /**Vendor specific handling */
        break;

    case PE_FWUP_EVT_PDFUINTIATE_REQ:
    case PE_FWUP_EVT_GETFWID_REQ:
    default:
        /** Notify as unexpected */
        u8IsResponseApplicable = PE_FWUP_SEND_UNEXPECTEDCMD_RESPONSE;
        break;
    }

    return u8IsResponseApplicable;
}

/**************************************************************************************************
    @fn
    UINT8 PE_FwUpdtProcessValidationState(UINT8 u8PortNum,UINT8 u8PDFUMsgType,UINT8* pu8DataBuffer,UINT16  u16ExtendedMsgHeader)

    @brief
            Processes the Validation state events and initiates state transition.

    @details
      Handler for PDFU_VALIDATE request, PDFU ABORT request and other invalid requests are 
      invoked from this Top level function.


    @param
    u8PortNum - Port in which the PDFU is happening
    u8PDFUMsgType - Msg Type of the newly received PDFU Message
    pu8DataBuffer - Pointer to the buffer where received message data is stored.
    u16ExtendedMsgHeader - Extended message header information.

    @return:
            UINT8 status of whether Response Applicable for the current request or Not.
**************************************************************************************************/

UINT8 PE_FwUpdtProcessValidationState(UINT8 u8PortNum,UINT8 u8PDFUMsgType,UINT8* pu8DataBuffer, UINT16 u16ExtendedMsgHeader)  /* */
{
    /*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
    UINT8   u8IsResponseApplicable = PE_FWUP_NO_RESPONSE;
    /*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

    switch (u8PDFUMsgType)
    {
    case PE_FWUP_EVT_PDFUVALIDATE_REQ:
        u8IsResponseApplicable = PE_FwUpdtPDFUValidateRequest(u8PortNum);
        break;

    case PE_FWUP_EVT_VENDORSPECIFIC_REQ:
        /**Vendor specific handling */
        break;

    case PE_FWUP_EVT_PDFUDATANR_REQ:
        break;

    case PE_FWUP_EVT_PDFUINTIATE_REQ:
    case PE_FWUP_EVT_PDFUDATA_REQ:
    case PE_FWUP_EVT_GETFWID_REQ:
    default:
        u8IsResponseApplicable = PE_FWUP_SEND_UNEXPECTEDCMD_RESPONSE;

        /** Notify as unexpected */
        break;
    }

    return u8IsResponseApplicable;
}


/**************************************************************************************************
    @fn
    void PE_FwUpdtResetToEnumState(UINT8 u8PortNum)
    @brief
            Resets the State Machine to SRC Ready or SNK Ready State. (i.e. Enumeration State for PDFU)
    @details
    Kill the Timers and Initialze the State Machine Data.
    @param
    u8PortNum - Port in which the PDFU is active
    @return:
    void
**************************************************************************************************/
void PE_FwUpdtResetToEnumState(
    UINT8   u8PortNum)  /* */
{

    ePolicyState eState = ePE_INVALIDSTATE;
    ePolicySubState eSubState = ePE_INVALIDSUBSTATE;
    UINT8 u8CurrentPowerRole;
    
    gsPdfuInfo.u16PDFUWaitTime = SET_TO_ZERO;

    gsPdfuInfo.u8ResendResponseTimes = SET_TO_ZERO;
    
    u8CurrentPowerRole = DPM_GET_CURRENT_POWER_ROLE(u8PortNum);

    if (PD_ROLE_SOURCE == u8CurrentPowerRole)
    {
        eState  = ePE_SRC_READY;
        eSubState = ePE_SRC_READY_END_AMS_SS;
        //PE_FWUP_SET_CURRENT_STATE(u8PortNum, ePE_SRC_READY);
        //PE_FWUP_CHANGE_SUBSTATE(u8PortNum, ePE_SRC_READY_END_AMS_SS);
        gsPdfuInfo.u32Transmit_TmrID_TxSt = PRL_BUILD_PKD_TXST_U32(ePE_SRC_READY,
                                                                   ePE_SRC_READY_END_AMS_SS,
                                                                   ePE_SRC_READY,
                                                                   ePE_SRC_READY_END_AMS_SS);
    }
    else
    if (PD_ROLE_SINK == u8CurrentPowerRole)
    {
        eState  = ePE_SNK_READY;
        eSubState = ePE_SNK_READY_IDLE_SS;     
        //PE_FWUP_SET_CURRENT_STATE(u8PortNum, ePE_SNK_READY);
        //PE_FWUP_CHANGE_SUBSTATE(u8PortNum, ePE_SNK_READY_IDLE_SS);
        gsPdfuInfo.u32Transmit_TmrID_TxSt = PRL_BUILD_PKD_TXST_U32(ePE_SNK_READY,
                                                                   ePE_SNK_READY_IDLE_SS,
                                                                   ePE_SNK_READY,
                                                                   ePE_SNK_READY_IDLE_SS);
    }
    else
    { }
    
    PE_FWUP_SET_CURRENT_STATE(u8PortNum, eState);
    PE_FWUP_CHANGE_SUBSTATE(u8PortNum, eSubState);
}

/**************************************************************************************************

    @fn:
        void PE_FwUpdtClearTimeoutStatus(void)

    @brief:
        Get the PDFU current Active Port Number from the Data structure present in the PDFU State machine
*************************************************************************************************/
UINT8 PE_FwUpdtGetPDFUActivePortNum(void)
{
    /*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
    UINT8   u8PDFUPortNum = PE_INVALID_PORT_NUM;
    /*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

    if (TRUE == gsPdfuInfo.u8IsPDFUActive)
    {
        u8PDFUPortNum = gsPdfuInfo.u8PDFUPortNum;
    }

    return u8PDFUPortNum;
}

/**************************************************************************************************

    @fn:
        void PE_FwUpdtClearTimeoutStatus(void)

    @brief:
        Get the TimeOut Status from the Event Variable present in PDFU State machine
*************************************************************************************************/
UINT8 PE_FwUpdtGetTimeoutStatus(void)
{
    /** Get the Timeout Status API*/
    return (PE_FWUP_TIMEOUT_EVT & gsPdfuInfo.u8EventType);
}

/**************************************************************************************************

    @fn:
        void PE_FwUpdtClearTimeoutStatus(void)

    @brief:
        Sets the TimeOut Status in the Event Variable present in PDFU State machine
*************************************************************************************************/
void PE_FwUpdtSetTimeoutStatus(void)
{
    /** Set the Timeout event API*/
    gsPdfuInfo.u8EventType |= PE_FWUP_TIMEOUT_EVT;
}

/**************************************************************************************************

    @fn:
        void PE_FwUpdtClearTimeoutStatus(void)

    @brief:
        Clears the TimeOut Status in the Event Variable present in PDFU State machine
*************************************************************************************************/
void PE_FwUpdtClearTimeoutStatus(void)
{
    /** Clear the Timeout event API*/
    gsPdfuInfo.u8EventType &= (~PE_FWUP_TIMEOUT_EVT);
}

/**************************************************************************************************

    @fn:
        void PE_FwUpdtSetMsgRcvStatus(void)

    @brief:
        Sets the Message Received Status in the Event Variable present in PDFU State machine
*************************************************************************************************/
void PE_FwUpdtSetMsgRcvStatus(void)
{
    /** Set the New Msg arrival event*/
    gsPdfuInfo.u8EventType |= PE_FWUP_NEWMSG_EVT;

    /** Firmware Flow is such a way that both Timeout and New Msg event cannot
    be set simultaneously*/
}

/**************************************************************************************************

    @fn:
        UINT16 PE_FwUpdtGetFWIDRequest(void) 
    @brief:
		Handler of the PDFU Get FW ID request in the Enumeration State.
    @return:
		u16Length - Length of the Get FW ID response.
*************************************************************************************************/
UINT16 PE_FwUpdtGetFWIDRequest(void)
{
    /*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
    PD_FW_UPDATE_GET_FW_ID  stGetFWID;
    UINT16                  u16Length = (UINT16)SET_TO_ZERO;
    /*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

    /*Hard code to PD FW update error code OK*/
    stGetFWID.u8Status = (UINT8)ePE_FWUP_OK;
    stGetFWID.u16VID = gasCfgStatusData.u16VendorID;

    /*Marked as TBD in Hermes interface*/
    stGetFWID.u16PID = gasCfgStatusData.u16ProducdID;

    /*Set HW revision to 1 as mentioned UDID section of Hermes Interface*/
    stGetFWID.u8HWVersion =
        (UINT8)(
            CONFIG_HWMINOR_VERSION |
            (CONFIG_HWMAJOR_VERSION << PE_FWUP_HWMAJOR_VER_Pos)
        );

    /*Set Silicon revision */
    stGetFWID.u8SiVersion = gasCfgStatusData.u8SiVersion; 

    /*Firmware Revision*/
    stGetFWID.u16FWVersion1 = (UINT16)SET_TO_ZERO;
    stGetFWID.u16FWVersion2 = (UINT16)SET_TO_ZERO;
    stGetFWID.u16FWVersion3 = (UINT16)SET_TO_ZERO;
    stGetFWID.u16FWVersion4 = (((UINT16)(SYSTEM_FW_REV >> 8)) |((UINT16)SYSTEM_FW_REV & (UINT16)0xFFu));

    /*Current image bank, u8CurrentMemory was stored when booting*/
    stGetFWID.u8ImageBank = MCHP_PSF_HOOK_GETCURRENT_IMAGEBANK();

    /*Flags only applies to PD FW Update Spec, Hence fill it as Zero*/
    stGetFWID.u8Flags1 = (UINT8) (CONFIG_PDFU_VIA_USBPD_SUPPORTED | (CONFIG_PDFU_SUPPORTED << PE_FWUP_FLAGS1_BIT2));
    stGetFWID.u8Flags2 = (UINT8) (PE_FWUP_RESPONDER_FULLFUNC | (PE_FWUP_RESPONSER_UNPLUG << PE_FWUP_FLAGS2_BIT1));
    stGetFWID.u8Flags3 =
        (UINT8)(
            PE_FWUP_HARDRESET_ONPDFU_CMPLT |
            (PE_FWUP_RESPONDER_ALTERNATEMODE << PE_FWUP_FLAGS3_BIT2)
        );
    stGetFWID.u8Flags4 = (UINT8)SET_TO_ZERO;
    u16Length = (UINT16)sizeof(PD_FW_UPDATE_GET_FW_ID);

    /*Copy to PDFW Buffer: Tested with Own MemCpy , the difference was around only 10 bytes */
    MCHP_PSF_HOOK_MEMCPY(&gsPdfuInfo.pu8ResponseBuffer[INDEX_2], (UINT8 *) &stGetFWID, u16Length);


    /*Add Response Header Length*/
    u16Length = u16Length + (UINT16)PE_FWUP_RESPBUFF_STATUS_INDEX;
    return u16Length;
}

/**************************************************************************************************

    @fn:
        UINT8 PE_FwUpdtPDFUInitateRequest(UINT8   u8PortNum, UINT8   u8WaitTime) 
    @brief:
		Handler of the PDFU Initiate request in the Enumeration State/Reconfiguration States.
    @param
		u8PortNum - Port Number in which the PDFU is active.
                u8WaitTime - Wait time required for processing the PDFU Initiate request
    @return:
		u8IsResponseApplicable - Tells information about the response for the PDFU Data Request
*************************************************************************************************/
UINT8 PE_FwUpdtPDFUInitateRequest(UINT8   u8PortNum, UINT8   u8WaitTime) 
{

#if (PE_FWUP_RECONFIG_WAITTIME != PE_FWUP_INFINITE_WAIT_TIME)
    UINT8 u8IsResponseApplicable = (UINT8)PE_FWUP_SEND_POPULATED_RESPONSE;
    UINT16 u16PDFUResponseLength = (UINT16)PE_FWUP_RESPBUFF_STATUS_INDEX;
    ePolicySubState eSubState = ePE_INVALIDSUBSTATE;
    
    if (CONFIG_UPDATABLE_IMAGEBANK_INDEX != MCHP_PSF_HOOK_GETCURRENT_IMAGEBANK())
    {
        /**PDFU Initiate message is considered as First Message of the PDFU Flow*/
        gsPdfuInfo.u8IsPDFUActive = TRUE;

        /** The port in which this message first received is Latched */
        gsPdfuInfo.u8PDFUPortNum = u8PortNum;


        /** To process the request \a PDFW_ProcessPDFUInitateRequest \norm is invoked.
        The response message contents are copied into \a gsPdfuInfo.pu8ResponseBuffer*/
        u16PDFUResponseLength = PE_FWUP_RESPBUFF_STATUS_INDEX;

        /* Hard code to PD FW update error code OK */
        gsPdfuInfo.pu8ResponseBuffer[u16PDFUResponseLength++] = (UINT8)ePE_FWUP_OK;
        gsPdfuInfo.pu8ResponseBuffer[u16PDFUResponseLength++] = u8WaitTime;

        /**
         * Bits 19:0 Maximum firmware image length in bytes that PDFU Responder
         * can receive ;
         * As per Section 5 of PD FW Update spec, Multi-byte fields that contain
         * numerical values are formatted at consecutive offsets, and shall be
         * formatted with the LSB at the lower offset and the MSB at the higher
         * offse
         */
        gsPdfuInfo.pu8ResponseBuffer[u16PDFUResponseLength++] = (CONFIG_MAX_FIRMWARE_IMAGESIZE & 0xff);
        gsPdfuInfo.pu8ResponseBuffer[u16PDFUResponseLength++] = ((CONFIG_MAX_FIRMWARE_IMAGESIZE >> 8)) & 0xff;
        gsPdfuInfo.pu8ResponseBuffer[u16PDFUResponseLength++] = (UINT8)(((((UINT32)CONFIG_MAX_FIRMWARE_IMAGESIZE) >> 16)) & 0x0f);

        /**Next State transition depends on whether the response is Ready Now or Ready after waittime**/
        if ((PE_FWUP_NO_WAIT_TIME < u8WaitTime) && (PE_FWUP_INFINITE_WAIT_TIME > u8WaitTime))
        {
            /**  (Transition to Initiate_incomplete state)*/

            /**A PDFU Responder shall enter the Reconfiguration Phase after receipt
            of a PDFU_INITIATE Request in the Enumeration Phase. */
            //PE_FWUP_CHANGE_SUBSTATE(u8PortNum, ePE_FWUP_RECONFIG_ENTRY_SS);
            eSubState = ePE_FWUP_RECONFIG_ENTRY_SS;

            /**If the Wait time is configured as non-zero,
            State transition happens to Reconfigure Entry Phase to wait for
            next PDFU_Initiate request*/
            gsPdfuInfo.u32Transmit_TmrID_TxSt = PRL_BUILD_PKD_TXST_U32(ePE_PDFU_MODE,
                                                                       ePE_FWUP_RECONFIG_ENTRY_SS,
                                                                       ePE_PDFU_MODE,
                                                                       ePE_FWUP_RECONFIG_ENTRY_SS);

            /** The Initiate response Resend counter is loaded with the \a PE_FWUP_RECONFIGRESEND value
            \a gu8PE_FWUP_INITIATE_RESRESEND_CTR variable is used for implementing this counter.
            */
            gu8PE_FWUP_INITIATE_RESRESEND_CTR = PE_FWUP_RECONFIGURERESEND;

            /** Timeout for next PDFU_Initiate request is "wait-time + next_request_timeout"*/
            gsPdfuInfo.u16PDFUWaitTime = (PE_FWUP_tPDFUNextRequestRcvd + u8WaitTime);
        }
        else    /*if (PE_FWUP_NO_WAIT_TIME == u8WaitTime)*/
        {
            /**  (Transition to Reconfigured state)*/

            /**A PDFU Responder shall enter the Reconfiguration Phase after receipt
            of a PDFU_INITIATE Request in the Enumeration Phase. */
            //PE_FWUP_CHANGE_SUBSTATE(u8PortNum, ePE_FWUP_RECONFIGURED_SS);
            eSubState = ePE_FWUP_RECONFIGURED_SS;

            /**PDFU_Initiate Ready response to be sent.
            So the next state is Reconfigured State.*/
            gsPdfuInfo.u32Transmit_TmrID_TxSt = PRL_BUILD_PKD_TXST_U32(ePE_PDFU_MODE,
                                                                       ePE_FWUP_RECONFIGURED_SS,
                                                                       ePE_PDFU_MODE,
                                                                       ePE_FWUP_RECONFIGURED_SS);

            /**Wait for PDFU_DATA Message*/
            gsPdfuInfo.u16PDFUWaitTime = (PE_FWUP_tPDFUNextRequestRcvd);

            /** The Initiate response Resend counter is loaded with the \a PE_FWUP_DATARESEND value
            \a gu8PE_FWUP_PDFUDATA_RESRESEND_CTR variable is used for implementing this counter.
            */
            gu8PE_FWUP_PDFUDATA_RESRESEND_CTR = PE_FWUP_DATARESEND;
        }
        PE_FWUP_CHANGE_SUBSTATE(u8PortNum,eSubState);
    }
    else
    {
        /** Reset and Boot from Fixed Application for continuing the Firmware Upgrade */
        MCHP_PSF_HOOK_BOOT_FIXED_APP();
    }

#else
    UINT8 u8IsResponseApplicable = PE_FWUP_SEND_UNEXPECTEDCMD_RESPONSE;
    /** Device not Support*/
#endif
    
    gsPdfuInfo.u16PDFUResponseLength = u16PDFUResponseLength;
    return u8IsResponseApplicable;
}

/**************************************************************************************************

    @fn:
        UINT8 PE_FwUpdtPDFUValidateRequest(UINT8   u8PortNum)
    @brief:
		Handler of the PDFU Validate request in the Transfer State/Validation State.
    @param
		u8PortNum - Port Number in which the PDFU is active.
    @return:
		u8IsResponseApplicable - Tells information about the response for the PDFU Data Request
*************************************************************************************************/
UINT8 PE_FwUpdtPDFUValidateRequest(UINT8   u8PortNum)  
{
    /*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
    UINT8   u8ValidationStatus;
    UINT8   u8ResponseApplicable = (UINT8)PE_FWUP_SEND_POPULATED_RESPONSE;
    
    UINT16 u16PDFUResponseLength = (UINT16)PE_FWUP_RESPBUFF_STATUS_INDEX;
    ePolicySubState eSubState = ePE_INVALIDSUBSTATE;
    /*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

    /**Processing the PDFUVALIDATE request*/

#if (PE_FWUP_VALIDATE_WAITTIME != PE_FWUP_INFINITE_WAIT_TIME)
    /** To process the request \a PDFW_ProcessPDFUInitateRequest \norm is invoked.
    The response message contents are copied into \a gsPdfuInfo.pu8ResponseBuffer*/
   // gsPdfuInfo.u16PDFUResponseLength = PE_FWUP_RESPBUFF_STATUS_INDEX;
    u8ValidationStatus = (UINT8)MCHP_PSF_HOOK_VALIDATE_FIRMWARE();
    if (PE_FWUP_VALIDATION_SUCCESSFUL == u8ValidationStatus)
    {
        gsPdfuInfo.pu8ResponseBuffer[u16PDFUResponseLength++] = (UINT8)ePE_FWUP_OK;
        gsPdfuInfo.pu8ResponseBuffer[u16PDFUResponseLength++] = (UINT8)PE_FWUP_NO_WAIT_TIME;        /**Wait time*/
        gsPdfuInfo.pu8ResponseBuffer[u16PDFUResponseLength++] =
            (UINT8)(
                (UINT8)0x01u <<
                PE_FWUP_FLAGS1_BIT0
            );
        
        gsPdfuInfo.u32Transmit_TmrID_TxSt = PRL_BUILD_PKD_TXST_U32(ePE_PDFU_MODE,
                                                                   ePE_FWUP_MANIFEST_SS,
                                                                   ePE_PDFU_MODE,
                                                                   ePE_FWUP_MANIFEST_SS);
        //PE_FWUP_CHANGE_SUBSTATE(u8PortNum, ePE_FWUP_VALIDATION_SS);
        eSubState = ePE_FWUP_VALIDATION_SS;
    }
    else
    if (PE_FWUP_VALIDATION_FAILURE == u8ValidationStatus)
    {
        gsPdfuInfo.pu8ResponseBuffer[u16PDFUResponseLength++] = (UINT8)ePE_FWUP_errVERIFY;
        gsPdfuInfo.pu8ResponseBuffer[u16PDFUResponseLength++] = PE_FWUP_NO_WAIT_TIME;        /**Wait time*/
        gsPdfuInfo.pu8ResponseBuffer[u16PDFUResponseLength++] =
            (
                SET_TO_ZERO <<
                PE_FWUP_FLAGS1_BIT0
            );
        PE_FwUpdtResetToEnumState(u8PortNum);
    }
    else
    {
        /**Firmware Validation is started and is InProgress State*/

        /**Response Status: OK - Request Porcessing delayed*/
        gsPdfuInfo.pu8ResponseBuffer[u16PDFUResponseLength++] = (UINT8)ePE_FWUP_OK;

        /**On Reception of every PDFU_VALIDATE request if the Validation process is still in progress,
        then Wait time is again intimated to the Initiator*/
        gsPdfuInfo.pu8ResponseBuffer[u16PDFUResponseLength++] = PE_FWUP_VALIDATE_WAITTIME;   /**Wait time*/

        /**Note:Flag is ignored when Wait time is Non-Zero*/
        gsPdfuInfo.pu8ResponseBuffer[u16PDFUResponseLength++] =
            (
                SET_TO_ZERO <<
                PE_FWUP_FLAGS1_BIT0
            );

        /** Timeout for next PDFU_Validate request is "waittime + next_request_timeout"*/
        gsPdfuInfo.u16PDFUWaitTime = (PE_FWUP_tPDFUNextRequestRcvd + PE_FWUP_VALIDATE_WAITTIME);
    

        gsPdfuInfo.u32Transmit_TmrID_TxSt = PRL_BUILD_PKD_TXST_U32(ePE_PDFU_MODE,
                                                                   ePE_FWUP_VALIDATION_SS,
                                                                   ePE_PDFU_MODE,
                                                                   ePE_FWUP_VALIDATION_SS);
        //PE_FWUP_CHANGE_SUBSTATE(u8PortNum, ePE_FWUP_VALIDATION_SS);
        eSubState = ePE_FWUP_VALIDATION_SS;
        /**Initialize the response Resend times*/
        gu8PE_FWUP_VALIDATE_RESRESEND_CTR = (UINT8)PE_FWUP_VALIDATERESEND;
    }
    if (ePE_INVALIDSUBSTATE !=eSubState)
    {
        PE_FWUP_CHANGE_SUBSTATE(u8PortNum, eSubState);
    }
#else
    u8ResponseApplicable = (UINT8)PE_FWUP_SEND_UNEXPECTEDCMD_RESPONSE;
    /** Assuming Device not accepting for validation*/
#endif
    
    gsPdfuInfo.u16PDFUResponseLength = u16PDFUResponseLength;
    return u8ResponseApplicable;
}

/**************************************************************************************************

    @fn:
        UINT8 PE_FwUpdtPDFUDataRequest(UINT8   u8PortNum,UINT8 *pu8DataBuffer,UINT16  u16ExtendedMsgHeader)
    @brief:
		Handler of the PDFU Data request in the Transfer State.
    @param
		u8PortNum - Port Number in which the PDFU is active.
                pu8DataBuffer - Pointer to the buffer where the Received data is stored.
                u16ExtendedMsgHeader - Extd Header where the Number of bytes received info us present.

    @return:
		u8IsResponseApplicable - Tells information about the response for the PDFU Data Request
*************************************************************************************************/
UINT8 PE_FwUpdtPDFUDataRequest(
    UINT8   u8PortNum,      /* */
    UINT8   *pu8DataBuffer, /* */
    UINT16  u16ExtendedMsgHeader)   /* */
{
    /*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
    UINT8   u8Status = (UINT8)ePE_FWUP_OK;
    UINT16  u16DataBlockSize = (UINT16)SET_TO_ZERO;
    UINT8   u8WaitTime = (UINT8)PE_FWUP_NO_WAIT_TIME;
    UINT8   u8IsResponseApplicable = (UINT8)PE_FWUP_SEND_POPULATED_RESPONSE;
    /*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
    gsPdfuInfo.u16PDFUWaitTime = SET_TO_ZERO;

    if (PE_FWUP_TRANSFER_PHASE_COMPLETE != gsPdfuInfo.u8TransferPhaseComplete)
    {
        /** In Transfer state,Ready for processing PDFU DATA request*/

        u16DataBlockSize = (UINT16)(PRL_GET_DATA_SIZE(u16ExtendedMsgHeader) - (UINT16)sizeof(UINT32)); /**Excluding the Message Header*/
        
        u8WaitTime = (UINT8)PE_FWUP_PDFUDATA_WAITTIME;
        
        if(gsPdfuInfo.u16ExptDataBlockIdx == ((UINT16)pu8DataBuffer[2u] | (((UINT16)pu8DataBuffer[3u]) << 8)))
        {
            if ((u16DataBlockSize < (UINT16)PE_FWUP_PDFU_DATA_BLOCK_SIZE))
            {
                if((UINT16)(CONFIG_MAX_FIRMWARE_IMAGESIZE/PE_FWUP_PDFU_DATA_BLOCK_SIZE) == gsPdfuInfo.u16ExptDataBlockIdx)
                {
                  /**Last Packet and size less than 256 */
                    gsPdfuInfo.u8TransferPhaseComplete = (UINT8)PE_FWUP_TRANSFER_PHASE_COMPLETE;
                    u8WaitTime = (UINT8)PE_FWUP_INFINITE_WAIT_TIME;
                    gsPdfuInfo.u16ExptDataBlockIdx = (UINT16)SET_TO_ZERO;
                }
                else
                {
                    /** In between packet received is lesser in size */
                    u8Status = (UINT8)ePE_FWUP_errNOTDONE;
                }
            }
        }
        else
        {
            /**Invalid data block index / DataBlock Index is received out of sequence**/
            u8Status = (UINT8)ePE_FWUP_errADDRESS;
        }

        if ((u16DataBlockSize != (UINT16)SET_TO_ZERO)&& (((UINT8)ePE_FWUP_OK) == u8Status))
        {
            /** To process the request \a MCHP_PSF_HOOK_PROGRAM_FWBLOCK \norm is invoked.
            The response message contents are copied into \a gsPdfuInfo.pu8ResponseBuffer*/
            u8Status = MCHP_PSF_HOOK_PROGRAM_FWBLOCK(pu8DataBuffer, u16DataBlockSize);
        }
        UINT16 u16PDFUResponseLength = (UINT16)PE_FWUP_RESPBUFF_STATUS_INDEX;
        gsPdfuInfo.u16ExptDataBlockIdx++;
        gsPdfuInfo.pu8ResponseBuffer[u16PDFUResponseLength++] = u8Status;
        gsPdfuInfo.pu8ResponseBuffer[u16PDFUResponseLength++] = u8WaitTime;
        gsPdfuInfo.pu8ResponseBuffer[u16PDFUResponseLength++] = (UINT8)PE_FWUP_NUM_DATA_NR;
        gsPdfuInfo.pu8ResponseBuffer[u16PDFUResponseLength++] = (UINT8)(gsPdfuInfo.u16ExptDataBlockIdx & (UINT16)0x00FFu);
        gsPdfuInfo.pu8ResponseBuffer[u16PDFUResponseLength++] = (UINT8)(gsPdfuInfo.u16ExptDataBlockIdx >> 8u);
        gsPdfuInfo.u16PDFUResponseLength = u16PDFUResponseLength;
        /** Validate if Status of the PDFU Data Command is positive */
        if (((UINT8)ePE_FWUP_OK) != u8Status)
        {
            PE_FwUpdtResetToEnumState(u8PortNum);
        }
        else
        {
            /*
                If the received PDFU DATA doesn't cause any error conditions and
                processing is successful,then the next state is Transfer state,
                where the further PDFU_DATA and PDFU_DATA_NR requests are
                processed.
            */
            PE_FWUP_CHANGE_SUBSTATE(u8PortNum, ePE_FWUP_TRANSFER_SS);
            gsPdfuInfo.u32Transmit_TmrID_TxSt = PRL_BUILD_PKD_TXST_U32(ePE_PDFU_MODE,
                                                                       ePE_FWUP_TRANSFER_SS,
                                                                       ePE_PDFU_MODE,
                                                                       ePE_FWUP_TRANSFER_SS);

            /** Assuming the use-case: Entire FW Update is only one PDFU_DATA
            Packet(size less than 255 bytes),it is necessary to verify that
            PDFU Transfer phase is completed or not before going into further
            processing  */
            if (PE_FWUP_TRANSFER_PHASE_COMPLETE != gsPdfuInfo.u8TransferPhaseComplete)
            {
                /** : Transfer phase is not complete*/

                /**Start the PDFU_DATA wait timeout for next packet*/
                gsPdfuInfo.u16PDFUWaitTime = (UINT16)(PE_FWUP_tPDFUNextRequestRcvd + PE_FWUP_PDFUDATA_WAITTIME);

                /**Initialize the Response resend counter to PE_FWUP_DATARESEND value.*/
                gu8PE_FWUP_PDFUDATA_RESRESEND_CTR = (UINT8)PE_FWUP_DATARESEND;

                
            }
            else
            {
                /**if entire firmware update is a single PDFU DATA Transfer*/

                /**Start the PDFU_VALIDATE wait timeout for next packet*/
                gsPdfuInfo.u16PDFUWaitTime = (UINT16)PE_FWUP_tPDFUNextRequestRcvd;

                /**Initialize the Response resend counter to PE_FWUP_VALIDATERESEND value.*/
                gu8PE_FWUP_VALIDATE_RESRESEND_CTR = (UINT8)PE_FWUP_VALIDATERESEND;
            }
        }
    }
    else
    {
        u8IsResponseApplicable = (UINT8)PE_FWUP_SEND_UNEXPECTEDCMD_RESPONSE;
    }

    return u8IsResponseApplicable;
}


/**************************************************************************************************

    @fn:
        void PE_FwUpdtKillTimer(UINT8   u8PortNum)
    @brief:
		Kills the Running PDFU Timer for the Specified Port Number.
    @param
		u8PortNum - Port Number in which the PDFU is active.
    @return:
		void.
*************************************************************************************************/
void PE_FwUpdtKillTimer(UINT8   u8PortNum)
{
    if(PE_FWUP_INVALID_TMRIDX != PE_FWUP_GET_TIMER_INDEX(u8PortNum))
    {
        PE_FwUpdtStopTimer(PE_FWUP_GET_TIMER_INDEX(u8PortNum));
        PE_FWUP_SET_TIMER_INDEX(u8PortNum, PE_FWUP_INVALID_TMRIDX);
    }
    /**clear Interrupt event */
    gsPdfuInfo.u8EventType &= (~PE_FWUP_TIMEOUT_EVT);
    gsPdfuInfo.u16PDFUWaitTime = (UINT16)PE_FWUP_NO_WAIT_TIME;
}

/**@}*/

#endif
