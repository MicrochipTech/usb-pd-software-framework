/*******************************************************************************
 Policy Engine Header

 Company:
     Microchip Technology Inc.

 File Name:
     policy_engine.h

 Description:
        This header file contains the data structures, constant definition,
		function prototypes and definitions for Policy Engine.
*******************************************************************************/
/*******************************************************************************
Copyright ©  [2019-2020] Microchip Technology Inc. and its subsidiaries.

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

#ifndef _POLICY_ENGINE_H_
#define _POLICY_ENGINE_H_
// *****************************************************************************
// *****************************************************************************
// Section: Constants
// *****************************************************************************
// *****************************************************************************
/***************************************************
            Port Power Role and Data Role
***************************************************/
/* Power Role */
#define PD_ROLE_SINK            0
#define PD_ROLE_SOURCE          1

/* Data role */
#define PD_ROLE_UFP    0
#define PD_ROLE_DFP    1

/***************************************************
          Port supported Spec Revision
***************************************************/
/* PD Spec Revision 2.0 */
#define PD_SPEC_REVISION_2_0  0x01
/* PD Spec Revision 3.0 */
#define PD_SPEC_REVISION_3_0  0x02

/***************************************************
                   PD Counters
***************************************************/
/**************************************************************************************************
	Description:
	PE_N_CAPSCOUNT refers the nCapsCount, Refer Table 6-55 Counter parameters in PD Specification 3.0

	Remarks:
	PE_N_CAPSCOUNT is defined as per PD Specification 3.0 which is not configurable.

	Example:
	<code>
	#define PE_N_CAPSCOUNT		50
	</code>
**************************************************************************************************/
#define PE_N_CAPSCOUNT                  50

/**************************************************************************************************
	Description:
	PE_N_HARD_RESET_COUNT refers the nHardResetCount, Refer Table 6-55 Counter parameters in PD Specification 3.0

	Remarks:
	PE_N_HARD_RESET_COUNT is defined as per PD Specification 3.0 which is not configurable.

	Example:
	<code>
	#define PE_N_HARD_RESET_COUNT		2
	</code>
**************************************************************************************************/
#define PE_N_HARD_RESET_COUNT           2

/**************************************************************************************************
	Description:
	PE_N_DISCOVER_IDENTITY_COUNT refers the nDiscoverIdentityCount, Refer Table 6-55 Counter parameters in PD Specification 3.0

	Remarks:
	PE_N_DISCOVER_IDENTITY_COUNT is defined as per PD Specification 3.0 which is not configurable.

	Example:
	<code>
	#define PE_N_DISCOVER_IDENTITY_COUNT		20
	</code>
**************************************************************************************************/
#define PE_N_DISCOVER_IDENTITY_COUNT    20

/**************************************************************************************************
	Description:
	PE_N_BUSY_COUNT refers the nBusyCount, Refer Table 6-55 Counter parameters in PD Specification 3.0

	Remarks:
	PE_N_BUSY_COUNT is defined as per PD Specification 3.0 which is not configurable.

	Example:
	<code>
	#define PE_N_BUSY_COUNT		7
	</code>
**************************************************************************************************/
#define PE_N_BUSY_COUNT                 7

#if (TRUE == INCLUDE_PD_3_0)
	/*Current Spec Role*/
	#define CONFIG_PD_DEFAULT_SPEC_REV  PD_SPEC_REVISION_3_0
#else
	#define CONFIG_PD_DEFAULT_SPEC_REV  PD_SPEC_REVISION_2_0
#endif

/******************************************************************************
                Return Values from PE_ValidateMessage API
******************************************************************************/
#define PE_PROCESS_MSG          0x1u
#define PE_MSG_HANDLED          0x2u
#define PE_PROCESS_EXTDMSG      0x3u


#define PE_UNSUPPORTED_MSG          1
#define PE_SUPPORTED_MSG            0
#define PE_SUPPORTED_EXTDMSG         0x02u

#define PE_REQUEST_OBJ_MASK         0x70
#define PE_REQUEST_OBJ_POS          4
#define PE_REQUEST_OPR_CUR_MASK     0xFFC00
#define PE_REQUEST_OPR_CUR_START_POS      10
#define PE_REQUEST_MAX_CUR_MASK     0x3FF
#define PE_REQUEST_MAX_CUR_POS      9


/*-------------- Port Status -----------------*/
#define PE_PDCONTRACT_MASK             BIT(0)
#define PE_PDCONNECTED_STS_MASK        BIT(1)
#define PE_VALID_PDCONTRACT            BIT(2)
#define PE_AMS_TYPE                    BIT(3)
#define PE_NO_RESPONSE_TIMEDOUT        BIT(4)
#define PE_CABLE_RESPOND_NAK           BIT(5)

#define PE_CABLE_RESPOND_NAK_POS       5

#define PE_HARDRESET_PROGRESS_MASK     BIT(6)
#define PE_HARDRESET_PROGRESS_POS      6

#define PE_HARDRESET_INPROGRESS    1
#define PE_HARDRESET_NOTPROGRESS   0

/*Mask used for getting the present contract type from u8PEPortSts variable*/ 
#define PE_EXPLICIT_CONTRACT        1
#define PE_IMPLICIT_CONTRACT        0

/* Define to get PD contract */
#define PE_GET_PD_CONTRACT(u8PortNum)	(gasPolicy_Engine[u8PortNum].u8PEPortSts & PE_PDCONTRACT_MASK)		

/*--------------VDM related macros--------------------*/
#define PE_VDM_ACK                  1
#define PE_VDM_NAK                  2
#define PE_VDM_BUSY                 3

#define PE_VDM_COMMAND_MASK             0x0000001F

#define PE_VDM_COMMAND_TYPE_MASK        0x000000C0
#define PE_VDM_COMMAND_TYPE_POS         6u

#define PE_VDM_OBJ_POS_MASK             0x700
#define PE_VDM_SVID_MASK                0xFFFF0000

#define PE_SRC_VDM_HEADER_HIGH_VER      0xFF00A001
#define PE_SRC_VDM_HEADER_LOW_VER       0xFF008001

#define PE_GET_VDM_CMD(VDM_HEADER)              (VDM_HEADER & PE_VDM_COMMAND_MASK)
#define PE_GET_VDM_CMD_TYPE(VDM_HEADER)         ((VDM_HEADER & PE_VDM_COMMAND_TYPE_MASK) >> PE_VDM_COMMAND_TYPE_POS)
#define PE_VDM_NAK_COMMAND_TYPE                 (2 << PE_VDM_COMMAND_TYPE_POS )

/*VDM Command Types found in VDM Header message */
#define PE_GET_VDM_CMD_TYPE_REQ				0
#define PE_GET_VDM_CMD_TYPE_ACK				1
#define PE_GET_VDM_CMD_TYPE_NAK				2
#define PE_GET_VDM_CMD_TYPE_BUSY			3

/* Capability max current check */
#define PE_MAX_CURR_MASK        0x1FF


typedef enum {
    /* Source Policy Engine Main State */ 
	ePE_SRC_STARTUP,
	ePE_SRC_DISCOVERY,
	ePE_SRC_SEND_CAPABILITIES,
	ePE_SRC_NEGOTIATE_CAPABILITY,
	ePE_SRC_TRANSITION_SUPPLY,
	ePE_SRC_READY,
	ePE_SRC_DISABLED,
	ePE_SRC_CAPABILITY_RESPONSE,
    ePE_SRC_SEND_SOFT_RESET,
	ePE_SRC_HARD_RESET,
	ePE_SRC_HARD_RESET_RECEIVED,
	ePE_SRC_TRANSITION_TO_DEFAULT,
	ePE_SRC_WAIT_NEW_CAPABILITIES,
    ePE_SRC_GET_SINK_CAP,
	ePE_SRC_SOFT_RESET,
    ePE_SRC_VDM_IDENTITY_REQUEST,
    ePE_SRC_VDM_IDENTITY_ACKED,
    ePE_SRC_VDM_IDENTITY_NAKED,
	//----------------Sink Specific Policy Engine States-------------------------//
	ePE_SNK_STARTUP,
	ePE_SNK_DISCOVERY,
	ePE_SNK_WAIT_FOR_CAPABILITIES,
	ePE_SNK_EVALUATE_CAPABILITY,
	ePE_SNK_SELECT_CAPABILITY,
	ePE_SNK_TRANSITION_SINK,
	ePE_SNK_READY,
	ePE_SNK_HARD_RESET,
	ePE_SNK_TRANSITION_TO_DEFAULT,
	ePE_SNK_GIVE_SINK_CAP,
	ePE_SNK_SOFT_RESET,
	ePE_SNK_SEND_SOFT_RESET,
	ePE_SNK_GIVE_SOURCE_CAP,
	/*VCONN Swap related States*/
    ePE_VCS_EVALUATE_SWAP,
    ePE_VCS_ACCEPT_SWAP,
    ePE_VCS_WAIT_FOR_VCONN,
    ePE_VCS_TURN_OFF_VCONN,
    ePE_VCS_TURN_ON_VCONN,
    ePE_VCS_SEND_PS_RDY,
    //---------------------------VDM states---------------------------------------//
    ePE_VDM_GET_IDENTITY,
    //-------------------------Common States------------------------------//
    ePE_SEND_NOT_SUPPORTED,
    ePE_SEND_REJECT,
	ePE_BIST_MODE,
    ePE_PDFU_MODE,
	ePE_INVALIDSTATE   /* Number of states. Not an actual state. */
}ePolicyState;

/*Policy Engine Sub-state */
typedef enum {
	/* ePE_SRC_STARTUP */
    ePE_SRC_STARTUP_ENTRY_SS,
    ePE_SRC_STARTUP_IDLE_SS,
	/* ePE_SRC_SEND_CAPABILITIES */
    ePE_SRC_SEND_CAP_ENTRY_SS,
    ePE_SRC_SEND_CAP_IDLE_SS,
    ePE_SRC_SEND_CAP_GOODCRC_RECEIVED_SS,
	/* ePE_SRC_DISCOVERY */
    ePE_SRC_DISCOVERY_ENTRY_SS,
    ePE_SRC_DISCOVERY_IDLE_SS,
    ePE_SRC_DISCOVERY_EXIT_SS,
	/* ePE_SRC_CAPABILITY_RESPONSE */
    ePE_SRC_CAPABILITY_RESPONSE_ENTRY_SS,
    ePE_SRC_CAPABILITY_RESPONSE_IDLE_SS,
	/* ePE_SRC_TRANSITION_SUPPLY */
    ePE_SRC_TRANSITION_SUPPLY_ENTRY_SS,
    ePE_SRC_TRANSITION_SUPPLY_EXIT_SS,
    ePE_SRC_TRANSITION_SUPPLY_IDLE_SS,
    ePE_SRC_TRANSITION_SUPPLY_GOODCRC_RECEIVED,
    ePE_SRC_TRANSITION_SUPPLY_POWER_ON,
	/* ePE_SRC_READY */
    ePE_SRC_READY_ENTRY_SS,
    ePE_SRC_READY_END_AMS_SS,
	/* ePE_SRC_DISABLED */
    ePE_SRC_DISABLED_ENTRY_SS,
    ePE_SRC_DISABLED_IDLE_SS,
	/* ePE_SRC_HARD_RESET */
    ePE_SRC_HARD_RESET_ENTRY_SS,
    ePE_SRC_HARD_RESET_IDLE_SS,
    ePE_SRC_HARD_RESET_EXIT_SS,
	/* ePE_SRC_HARD_RESET_RECEIVED */
    ePE_SRC_HARD_RESET_RECEIVED_ENTRY_SS,
    ePE_SRC_HARD_RESET_RECEIVED_IDLE_SS,
    ePE_SRC_HARD_RESET_RECEIVED_EXIT_SS,
	/* ePE_SRC_TRANSITION_TO_DEFAULT */
    ePE_SRC_TRANSITION_TO_DEFAULT_ENTRY_SS,
    ePE_SRC_TRANSITION_TO_DEFAULT_VSAFE0V_SS,
    ePE_SRC_TRANSITION_TO_DEFAULT_CHECK_VCONNOFF_SS,
    ePE_SRC_TRANSITION_TO_DEFAULT_IDLE_SS,
    ePE_SRC_TRANSITION_TO_DEFAULT_SRCRECOVER_TIMEOUT_SS,
    ePE_SRC_TRANSITION_TO_DEFAULT_POWER_ON_SS,
    ePE_SRC_TRANSITION_TO_DEFAULT_VBUS_CHECK_SS,
    ePE_SRC_TRANSITION_TO_DEFAULT_VCONNON_CHECK_SS,
    ePE_SRC_TRANSITION_TO_DEFAULT_EXIT_SS,
	/* ePE_SRC_GET_SINK_CAP */
    ePE_SRC_GET_SINK_CAP_ENTRY_SS,
    ePE_SRC_GET_SINK_CAP_IDLE_SS,
    ePE_SRC_GET_SINK_CAP_GOODCRC_RECEIVED_SS,
    ePE_SRC_GET_SINK_CAP_TIMER_TIMEDOUT_SS,
    ePE_SRC_GET_SINK_CAP_RESPONSE_RECEIVED_SS, 
	/* ePE_SRC_SOFT_RESET */
    ePE_SRC_SOFT_RESET_ENTRY_SS,
    ePE_SRC_SOFT_RESET_IDLE_SS,
	/* ePE_SRC_SEND_SOFT_RESET */
    ePE_SRC_SEND_SOFT_RESET_SOP_SS,
    ePE_SRC_SEND_SOFT_RESET_SOP_P_SS,
    ePE_SRC_SEND_SOFT_RESET_SOP_DP_SS,
    ePE_SRC_SEND_SOFT_RESET_IDLE_SS,
    ePE_SRC_SEND_SOFT_RESET_SENDER_RESPONSE_TIMEDOUT,
    ePE_SRC_SEND_SOFT_RESET_GOODCRC_RECEIVED_SS,
	/* ePE_SRC_VDM_IDENTITY_REQUEST */
    ePE_SRC_VDM_IDENTITY_REQUEST_ENTRY_SS,
    ePE_SRC_VDM_IDENTITY_REQUEST_IDLE_SS,
    ePE_SRC_VDM_IDENTITY_REQUEST_GOODCRC_SS,
    ePE_SRC_VDM_IDENTITY_REQUEST_SENDER_RESPONSE_TIMEOUT,
    //----------------Sink Specific Policy Engine SubStates-------------------------//
    /*ePE_SNK_WAIT_FOR_CAPABILITIES*/
    ePE_SNK_WAIT_FOR_CAPABILITIES_ENTRY_SS,
    ePE_SNK_WAIT_FOR_CAPABILITIES_WAIT_SS,
    /*ePE_SNK_SELECT_CAPABILITY*/
    ePE_SNK_SELECT_CAPABILITY_SEND_REQ_SS,
    ePE_SNK_SELECT_CAPABILITY_SEND_REQ_IDLE_SS,
    ePE_SNK_SELECT_CAPABILITY_REQ_SENT_SS,
    ePE_SNK_SELECT_CAPABILITY_WAIT_FOR_ACCEPT_SS,
    /*ePE_SNK_TRANSITION_SINK*/
    ePE_SNK_TRANSITION_SINK_ENTRY_SS,
    ePE_SNK_TRANSITION_SINK_WAIT_FOR_PSRDY_SS,
    /*ePE_SNK_READY*/
    ePE_SNK_READY_ENTRY_SS,
    ePE_SNK_READY_IDLE_SS,
    /*ePE_SNK_SOFT_RESET*/
    ePE_SNK_SOFT_RESET_SEND_ACCEPT_SS,
    ePE_SNK_SOFT_RESET_WAIT_SS,
    /*ePE_SNK_SEND_SOFT_RESET*/
    ePE_SNK_SEND_SOFT_RESET_ENTRY_SS,
    ePE_SNK_SEND_SOFT_RESET_SEND_IDLE_SS,
    ePE_SNK_SEND_SOFT_RESET_SENT_SS,
    ePE_SNK_SEND_SOFT_RESET_WAIT_FOR_ACCEPT_SS,
    /*ePE_SNK_HARD_RESET*/
    ePE_SNK_HARD_RESET_SEND_SS,
    /*ePE_SNK_TRANSITION_TO_DEFAULT*/
    ePE_SNK_TRANSITION_TO_DEFAULT_ENTRY_SS,
    ePE_SNK_TRANSITION_TO_DEFAULT_VCONNOFF_CHECK_SS,
    ePE_SNK_TRANSITION_TO_DEFAULT_RESETHW_SS,
    ePE_SNK_TRANSITION_TO_DEFAULT_WAIT_SS,
    /*ePE_SNK_GIVE_SINK_CAP*/
    ePE_SNK_GIVE_SINK_CAP_ENTRY_SS,
    ePE_SNK_GIVE_SINK_CAP_IDLE_SS,
    //----------------------------VCONN SWAP Sub states-------------------------------//
    /*PE_VCS_WAIT_FOR_VCONN*/
    ePE_VCS_WAIT_FOR_VCONN_START_TIMER_SS,
    ePE_VCS_WAIT_FOR_VCONN_WAIT_FOR_PS_RDY_SS,
    /*ePE_VCS_ACCEPT_SWAP*/
    ePE_VCS_ACCEPT_SWAP_SEND_ACCEPT_SS,
    ePE_VCS_ACCEPT_SWAP_IDLE_SS,
    ePE_VCS_ACCEPT_SWAP_ACCEPT_SENT_SS,
    /*ePE_VCS_TURN_OFF_VCONN*/
    ePE_VCS_TURN_OFF_VCONN_ENTRY_SS,
    ePE_VCS_TURN_OFF_VCONN_CHECK_SS,
    /*ePE_VCS_TURN_ON_VCONN*/
    ePE_VCS_TURN_ON_VCONN_ENTRY_SS,
    ePE_VCS_TURN_ON_VCONN_CHECK_SS,
    /*ePE_VCS_SEND_PS_RDY*/
    ePE_VCS_SEND_PS_RDY_ENTRY_SS,
    ePE_VCS_SEND_PS_RDY_IDLE_SS,
    //--------------------Common States-------------------------------------------------//
    /*ePE_SEND_NOT_SUPPORTED*/
    ePE_SEND_NOT_SUPPORTED_ENTRY_SS,
    ePE_SEND_NOT_SUPPORTED_IDLE_SS,
    /*ePE_SEND_REJECT*/
    ePE_SEND_REJECT_ENTRY_SS,
    ePE_SEND_REJECT_IDLE_SS,
	/*ePE_BIST_MODE*/
	ePE_BIST_MODE_ENTRY_SS,
	ePE_BIST_MODE_IDLE_SS,
	ePE_BIST_MODE_EXIT_SS,
    
    /*ePE_PDFU_MODE*/
    ePE_FWUP_ENUMERATION_SS,
    ePE_FWUP_RECONFIG_ENTRY_SS,
    ePE_FWUP_RECONFIGURED_SS,
    ePE_FWUP_TRANSFER_SS,
    ePE_FWUP_VALIDATION_SS,
    ePE_FWUP_MANIFEST_SS,
	ePE_INVALIDSUBSTATE

}ePolicySubState;

/*Macros for Control Message type */
#define PE_CTRL_GOOD_CRC            1
#define	PE_CTRL_GOTO_MIN            2
#define	PE_CTRL_ACCEPT              3
#define	PE_CTRL_REJECT              4
#define	PE_CTRL_PING                5
#define	PE_CTRL_PS_RDY              6
#define	PE_CTRL_GET_SOURCE_CAP      7
#define	PE_CTRL_GET_SINK_CAP        8
#define	PE_CTRL_DR_SWAP             9
#define	PE_CTRL_PR_SWAP             10
#define	PE_CTRL_VCONN_SWAP          11
#define	PE_CTRL_WAIT                12
#define	PE_CTRL_SOFT_RESET          13
#define	PE_CTRL_NOT_SUPPORTED            16
#define	PE_CTRL_GET_SOURCE_CAP_EXTENDED  17
#define	PE_CTRL_GET_STATUS               18
#define	PE_CTRL_FR_SWAP                  19
#define	PE_CTRL_GET_PPS_STATUS           20
#define	PE_CTRL_GET_COUNTRY_CODES        21

/*Macros for Data message type */
#define PE_DATA_SOURCE_CAP          1
#define	PE_DATA_REQUEST             2
#define	PE_DATA_BIST                3
#define	PE_DATA_SINK_CAP            4
#define PE_DATA_BATTERY_STS         5
#define PE_DATA_ALERT               6
#define PE_DATA_GET_COUNTRY_INFO    7
#define PE_DATA_VENDOR_DEFINED      15

/*Macros for Extended message type */
#define PE_EXT_FW_UPDATE_REQUEST    10
#define PE_EXT_FW_UPDATE_RESPONSE   11

/*Defines to be used in PRL_FormSOPTypeMsgHeader*/
#define PE_OBJECT_COUNT_1            1
#define PE_OBJECT_COUNT_0            0

#define PE_NON_EXTENDED_MSG          0
#define PE_EXTENDED_MSG              0x01u


/***************************************************************************************/
// *****************************************************************************
// *****************************************************************************
// Section: Data Structures
// *****************************************************************************
// *****************************************************************************
/* Port Structure of Policy Engine */
typedef struct MCHP_PSF_STRUCT_PACKED_START
{
    ePolicyState ePEState;                  // Policy Engine Main State
	ePolicySubState ePETimeoutSubState;     // Policy Engine Timeout State
	ePolicySubState ePESubState;            // Policy Engine sub-state
    UINT32 u32TimeoutMsgHeader;             // Timeout Global Header
    UINT32 u32MsgHeader;                    // Receive Handler Global Header
	UINT8 u8HardResetCounter;               // HardResetCounter Value
	UINT8 u8CapsCounter;                    // CapsCounter Value
	UINT8 u8DiscoverIdentityCounter;        // DiscoverIdentityCounter
	UINT8 u8PEPortSts;                      // Bit 0 - PD Contract <p />
                                            // Bit 1 - PD connected status <p />
                                            // Bit 2 - PD Valid contract <p />
                                            // Bit 3 - AMS Type <p />
                                            // Bit 4 - NoResponse timer timeout status <p />
                                            // Bit 5 - NAK received status from E-Cable
                                            // Bit 6 - HardReset Progress flag
	UINT8 u8PETimerID;                      // Policy Engine Timer ID
	UINT8 u8PENoResponseTimerID;            // NoResponse Timer ID
    UINT8  u8HardResetRecvdISR;             // Hard Reset received status
}MCHP_PSF_STRUCT_PACKED_END PolicyEngine_Status;

// *****************************************************************************
// *****************************************************************************
// Section: Interface Routines
// *****************************************************************************
// *****************************************************************************
/**************************************************************************************************
    Function:
        void PE_InitPort (UINT8 u8PortNum);

    Summary:
        This API initializes the policy engine variables for a given port based on port's type

    Devices Supported:
        UPD350 REV A

    Description:
         This API initializes the policy engine variables for a given port based on port's type

    Conditions:
        This API is called inside the PD Stack initialisation API call .

    Input:
        u8PortNum - Port Number.

    Return:
        None.

    Remarks:
        None.
**************************************************************************************************/
void PE_InitPort (UINT8 u8PortNum);
/**************************************************************************************************
    Function:
        void PE_RunStateMachine (UINT8 u8PortNum);

    Summary:
        This API is called to run the policy engine state machine for a given port

    Devices Supported:
        UPD350 REV A

    Description:
         This API is called to run the policy engine state machine for a given port

    Conditions:
        None

    Input:
        u8PortNum - Port Number.

    Return:
        None.

    Remarks:
        None.
**************************************************************************************************/
void PE_RunStateMachine(UINT8 u8PortNum);
/**************************************************************************************************
    Function:
        void PE_RunCommonStateMachine(UINT8 u8PortNum , UINT8 *u8DataBuf , UINT8 u8SOPType ,UINT32 u32Header);

    Summary:
        This API is called to run the Common state machine of policy engine which handles the
        VCONN Swap, VDM messages regardless of the port's type

    Devices Supported:
        UPD350 REV A

    Description:
         This API is called to run the Common state machine of policy engine which handles the
        VCONN Swap, VDM messages regardless of the port's type

    Conditions:
        None

    Input:
        u8PortNum - Port Number.
        pu8DataBuf - Pointer to the 8 bit buffer of received PD message object
        u8SOPType  - SOP type of received PD message
        u32Header  - Header of the received PD message

    Return:
        None.

    Remarks:
        None.
**************************************************************************************************/
void PE_RunCommonStateMachine(UINT8 u8PortNum , UINT8 *pu8DataBuf , UINT8 u8SOPType ,UINT32 u32Header);
/**************************************************************************************************
    Function:
        void PE_ReceiveMsgHandler(UINT8 u8PortNum, UINT32 u32Header);

    Summary:
        This API is called to process the received PD message and do the state transition in 
        policy engine state machine dependning on the current policy engine state and substate

    Devices Supported:
        UPD350 REV A

    Description:
        This API is called to process the received PD message and do the state transition in 
        policy engine state machine dependning on the current policy engine state and substate

    Conditions:
        None

    Input:
        u8PortNum - Port Number.
        u32Header  - Header of the received PD message

    Return:
        None.

    Remarks:
        None.
**************************************************************************************************/
void PE_ReceiveMsgHandler(UINT8 u8PortNum, UINT32 u32Header);
/**************************************************************************************************
    Function:
        UINT8 PE_ValidateMessage(UINT8 u8PortNum, UINT32 u32Header);

    Summary:
        This API is called to validate the received PD message. It can set the policy engine states
        itself if the received message is a not supported one.

    Devices Supported:
        UPD350 REV A

    Description:
        This API is called to validate the received PD message. It can set the policy engine states
        itself if the received message is a not supported one.

    Conditions:
        None

    Input:
        u8PortNum - Port Number.
        u32Header  - Header of the received PD message

    Return:
        PE_MSG_HANDLED - Returns this value if the received message is already handled inside the API
        PE_PROCESS_MSG - Returns this value if the received message is to be handled by the 
        PE_ReceiveMsgHandler API
        PE_PROCESS_EXTDMSG - Retunr this value if the received message is extended message and to be 
        handled by the PE_ReceiveHandler API

    Remarks:
        None.
**************************************************************************************************/
UINT8 PE_ValidateMessage(UINT8 u8PortNum, UINT32 u32Header);
/**************************************************************************************************
    Function:
        PE_HandleRcvdMsgAndTimeoutEvents (UINT8 u8PortNum, ePolicyState eNextState , ePolicySubState eNextSubState);

    Summary:
        This API is called to see whether timeout set for the received message has occured before 
        or after the reception of message in protocol layer.

    Devices Supported:
        UPD350 REV A

    Description:
        This API is called to see whether timeout set for the received message has occured before 
        or after the reception of message in protocol layer

    Conditions:
        None

    Input:
        u8PortNum - Port Number.
        u32Header  - Header of the received PD message

    Return:
        None.

    Remarks:
        None.

**************************************************************************************************/
void PE_HandleRcvdMsgAndTimeoutEvents (UINT8 u8PortNum, ePolicyState eNextState , ePolicySubState eNextSubState);
/**************************************************************************************************
    Function:
        void PE_HandleUnExpectedMsg(UINT8 u8PortNum);

    Summary:
        This API is called to handle the unexpected message received by the port
       
    Devices Supported:
        UPD350 REV A

    Description:
        This API is called to handle the unexpected message received by the port. Policy engine states
        are changed to send either the soft reset or hard reset signal depending on its current state.
        
    Conditions:
        None

    Input:
        u8PortNum - Port Number.

    Return:
        None.

    Remarks:
        None.

**************************************************************************************************/
void PE_HandleUnExpectedMsg(UINT8 u8PortNum);
/**************************************************************************************************
    Function:
        UINT8 PE_IsMsgUnsupported(UINT8 u8PortNum, UINT16 u16Header);

    Summary:
        This API is called inside the PE_ValidateMessage API to find out whether the received 
        message is supported by the port or not.
       
    Devices Supported:
        UPD350 REV A

    Description:
        

    Conditions:
        None

    Input:
        u8PortNum - Port Number.
        u32Header  - Header of the received PD message

    Return:
        UINT8 - PE_UNSUPPORTED_MSG (Message not supported by the port)
              - PE_SUPPORTED_MSG (Message supported by the port)

    Remarks:
        None.

**************************************************************************************************/
UINT8 PE_IsMsgUnsupported(UINT8 u8PortNum, UINT16 u16Header);
/**************************************************************************************************
    Function:
       This API is called inside the timer module once the software timer times out to change the
       policy engine state to a given value

    Summary:
       
    Devices Supported:
        UPD350 REV A

    Description:
         This API is called inside the timer module once the software timer times out to change the
       policy engine substate to a given value        

    Conditions:
        None

    Input:
        u8PortNum - Port Number.
        u8PEState - Policy engine state to change after timeout

    Return:
        None.

    Remarks:
        None.

**************************************************************************************************/
void PE_StateChange_TimerCB(UINT8 u8PortNum, UINT8 u8PEState);
/**************************************************************************************************
    Function:
        void PE_SubStateChange_TimerCB (UINT8 u8PortNum, UINT8 u8PESubState);

    Summary:
       This API is called inside the timer module once the software timer times out to change the
       policy engine substate to a given value

    Devices Supported:
        UPD350 REV A

    Description:
        This API is called inside the timer module once the software timer times out to change the
       policy engine substate to a given value
        
    Conditions:
        None

    Input:
        u8PortNum     - Port Number.
        u8PESubState  - Policy engine Substate to change after timeout

    Return:
        None.

    Remarks:
        None.

**************************************************************************************************/
void PE_SubStateChange_TimerCB (UINT8 u8PortNum, UINT8 u8PESubState);
/**************************************************************************************************
    Function:
        void PE_SubStateChangeAndTimeoutValidateCB(UINT8 u8PortNum, UINT8 u8PESubState);

    Summary:
        This API is set as a timer callback by the policy engine when the PE expects a 
        message from port partner within the particular timer value
       
    Devices Supported:
        UPD350 REV A

    Description:
        This API is set as a timer callback by the policy engine when the PE expects a 
        message from port partner within the particular timer value
        
    Conditions:
        None

    Input:
        u8PortNum     - Port Number.
        u8PESubState  -Policy engine Substate to change after timeout

    Return:
        None.

    Remarks:
        None.

**************************************************************************************************/
void PE_SubStateChangeAndTimeoutValidateCB(UINT8 u8PortNum, UINT8 u8PESubState);
/**************************************************************************************************
    Function:
        void PE_SendNotSupportedOrRejectMsg(UINT8 u8PortNum);

    Summary:
        This API is called to send the Not Supported or Reject message from the policy engine 
        based on the current PD Specification of the port.
       
    Devices Supported:
        UPD350 REV A

    Description:
        This API is called to send the Not Supported or Reject message from the policy engine 
        based on the current PD Specification of the port.
       
    Conditions:
        None

    Input:
        u8PortNum - Port Number.

    Return:
        None.

    Remarks:
        None.

**************************************************************************************************/
void PE_SendNotSupportedOrRejectMsg(UINT8 u8PortNum);
/**************************************************************************************************
    Function:
        void PE_StateChange_TransmitCB (UINT8 u8PortNum, UINT8 u8TXDoneState, UINT8 u8TxDoneSubState, UINT8 u8TxFailedState, UINT8 u8TxFailedSubState);

    Summary:
        This API is called by the protocol layer upon transmission of the PD message initiated from
        policy engine.
       
    Devices Supported:
        UPD350 REV A

    Description:
         This API is called by the protocol layer upon transmission of the PD message initiated from
        policy engine. Policy engine States are changed based on the transmission result of PD message
        
    Conditions:
        None

    Input:
        u8PortNum           - Port Number.
        u8TXDoneState       - PE State to be set if the PD message was transmitted successfully
        u8TxDoneSubState    - PE SubState to be set if the PD message was transmitted successfully
        u8TxFailedState     - PE State to be set if the PD message transmission was failed 
        u8TxFailedSubState  - PE SubState to be set if the PD message transmission was failed 

    Return:
        None.

    Remarks:
        None.

**************************************************************************************************/
void PE_StateChange_TransmitCB (UINT8 u8PortNum, UINT8 u8TXDoneState, UINT8 u8TxDoneSubState, UINT8 u8TxFailedState, UINT8 u8TxFailedSubState);
/**************************************************************************************************
    Function:
        void PE_FindVDMStateActiveFlag(UINT8 u8PortNum);

    Summary:
        This API is called to find out whether the current active policy engine state is a VDM state
        for a given port
       
    Devices Supported:
        UPD350 REV A

    Description:
        This API is called to find out whether the current active policy engine state is a VDM state
        for a given port

    Conditions:
        None

    Input:
        u8PortNum - Port Number.

    Return:
        None.

    Remarks:
        None.

**************************************************************************************************/
void PE_FindVDMStateActiveFlag(UINT8 u8PortNum);
/**************************************************************************************************
    Function:
        void PE_KillPolicyEngineTimer (UINT8 u8PortNum);

    Summary:
        This API is called to kill the software timer with Policy engine timer ID
       
    Devices Supported:
        UPD350 REV A

    Description:
        This API is called to kill the software timer with Policy engine timer ID
        
    Conditions:
        None

    Input:
        u8PortNum - Port Number.

    Return:
        None.

    Remarks:
        None.

**************************************************************************************************/
void PE_KillPolicyEngineTimer (UINT8 u8PortNum);
/**************************************************************************************************
    Function:
        void PE_SendSoftResetMsg (UINT8 u8PortNum);

    Summary:
        This API is called to send the Soft reset message from the policy engine state machine for a port
       
    Devices Supported:
        UPD350 REV A

    Description:
        This API is called to send the Soft reset message from the policy engine state machine for a port
        
    Conditions:
        None

    Input:
        u8PortNum - Port Number.

    Return:
        None.

    Remarks:
        None.

**************************************************************************************************/
void PE_SendSoftResetMsg (UINT8 u8PortNum);
/**************************************************************************************************
    Function:
        void PE_SendHardResetMsg (UINT8 u8PortNum);

    Summary:
        This API is called to send the Hard reset signal from the policy engine state machine for a port
       
    Devices Supported:
        UPD350 REV A

    Description:
        This API is called to send the Hard reset signal from the policy engine state machine for a port
        
    Conditions:
        None

    Input:
        u8PortNum - Port Number.

    Return:
        None.

    Remarks:
        None.

**************************************************************************************************/
void PE_SendHardResetMsg (UINT8 u8PortNum);
/**************************************************************************************************
    Function:
        void PE_SetHardResetReceiveFlag(UINT8 u8PortNum);

    Summary:
        This API is called by the protocol layer upon reception of Hard reset signal for a port
       
    Devices Supported:
        UPD350 REV A

    Description:
         This API is called by the protocol layer upon reception of Hard reset signal for a port
        
    Conditions:
        None

    Input:
        u8PortNum - Port Number.

    Return:
        None.

    Remarks:
        None.

**************************************************************************************************/
void PE_SetHardResetReceiveFlag(UINT8 u8PortNum);
/**************************************************************************************************
    Function:
        void PE_SrcRunStateMachine(UINT8 u8PortNum , UINT8 *pu8DataBuf , UINT8 u8SOPType ,UINT32 u32Header);

    Summary:
        This API is called to run the policy engine source state machine. 
       
    Devices Supported:
        UPD350 REV A

    Description:
        This API is called to run the policy engine source state machine. 
        

    Conditions:
        None

    Input:
        u8PortNum  - Port Number.
        pu8DataBuf - Pointer to the UINT8 buffer containing the received PD message object
        u8SOPType  - SOP type of the received PD message
        u32Header  - Header of the received PD message

    Return:
        None.

    Remarks:
        None.

**************************************************************************************************/
void PE_SrcRunStateMachine(UINT8 u8PortNum , UINT8 *pu8DataBuf , UINT8 u8SOPType ,UINT32 u32Header);
/**************************************************************************************************
    Function:
        void PE_NoResponseTimerCB(UINT8 u8PortNum, UINT8 u8DummyPE_State);

    Summary:
        This API is given as the timer call back API when starting the NoResponse timer from 
        source policy engine state machine
       
    Devices Supported:
        UPD350 REV A

    Description:
        This API is given as the timer call back API when starting the NoResponse timer from 
        source policy engine state machine
        
    Conditions:
        None

    Input:
        u8PortNum       - Port Number.
        u8DummyPE_State - Dummy Argument

    Return:
        None.

    Remarks:
        None.

**************************************************************************************************/
void PE_NoResponseTimerCB(UINT8 u8PortNum, UINT8 u8DummyPE_State);
/**************************************************************************************************
    Function:
        void PE_SnkRunStateMachine(UINT8 u8PortNum ,UINT8 *pu8DataBuf ,UINT8 u8SOPType ,UINT32 u32Header);

    Summary:
        This API is called to run the policy engine sink state machine.
       
    Devices Supported:
        UPD350 REV A

    Description:
        This API is called to run the policy engine sink state machine.        

    Conditions:
        None

    Input:
        u8PortNum - Port Number.
        pu8DataBuf - Pointer to the UINT8 buffer containing the received PD message object
        u8SOPType  - SOP type of the received PD message
        u32Header  - Header of the received PD message

    Return:
        None.

    Remarks:
        None.
**************************************************************************************************/
void PE_SnkRunStateMachine(UINT8 u8PortNum ,UINT8 *pu8DataBuf ,UINT8 u8SOPType ,UINT32 u32Header);
/**************************************************************************************************
    Function:
        UINT8 PE_IsPolicyEngineIdle(UINT8 u8PortNum)
    Summary:
        Checks whether PE is idle.
    Devices Supported:
        UPD350 REV A
    Description:
        This API is called to check whether Policy Engine is Idle state.        
    Conditions:
        None
    Input:
        u8PortNum - Port Number.Value passed will be less than CONFIG_PD_PORT_COUNT.
    Return:
        TRUE - If Policy Engine is IDLE
        FALSE- If Policy Engine is not IDLE.
    Remarks:
        None.
**************************************************************************************************/
UINT8 PE_IsPolicyEngineIdle(UINT8 u8PortNum); 
#endif /*_POLICY_ENGINE_H_*/
