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
#define PD_ROLE_DRP             2

/* Data role */
#define PD_ROLE_UFP             0
#define PD_ROLE_DFP             1
#define PD_ROLE_TOGGLING        2

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
	#define PE_N_BUSY_COUNT		5
	</code>
**************************************************************************************************/
#define PE_N_BUSY_COUNT                 5

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

/*-------------- Policy Engine Port Status -----------------*/
#define PE_PDCONTRACT_MASK             BIT(0)
#define PE_PDCONNECTED_STS_MASK        BIT(1)
#define PE_NO_RESPONSE_TIMEDOUT        BIT(2)

#define PE_CABLE_RESPOND_NAK           BIT(3)
#define PE_CABLE_RESPOND_NAK_POS       3

#define PE_HARDRESET_PROGRESS_MASK     BIT(4)
#define PE_HARDRESET_PROGRESS_POS      4

#define PE_PR_OR_FR_SWAP_IN_PROGRESS_MASK    BIT(5)
#define PE_PR_OR_FR_SWAP_IN_PROGRESS_POS     5

/*Mask used for getting the present contract type from u8PEPortSts variable*/ 
#define PE_EXPLICIT_CONTRACT        1
#define PE_IMPLICIT_CONTRACT        0

/* Define to get PD contract */
#define PE_GET_PD_CONTRACT(u8PortNum)	(gasPolicyEngine[u8PortNum].u8PEPortSts & PE_PDCONTRACT_MASK)		

/* Alert message defines */
#define PE_ALERT_DATA_OBJECT_SIZE              1 

/* Status message defines */
#define PE_STATUS_DATA_BLOCK_SIZE_IN_BYTES     6 
#define PE_STATUS_DATA_OBJ_CNT                 2

/* PPS Status message defines */
#define PE_PPS_STATUS_DATA_BLOCK_SIZE_IN_BYTES 4
#define PE_PPS_STATUS_DATA_OBJ_CNT             2 

/* Sink Caps Extd message defines */
#define PE_SINK_CAP_EXTD_DATA_BLOCK_SIZE_IN_BYTES     21 
#define PE_SINK_CAP_EXTD_DATA_DATA_OBJ_CNT            2

typedef enum {
    //--------------------------Source States-----------------------------------//
	ePE_SRC_STARTUP,
	ePE_SRC_DISCOVERY,
	ePE_SRC_SEND_CAPABILITIES,
	ePE_SRC_NEGOTIATE_CAPABILITY,
	ePE_SRC_TRANSITION_SUPPLY,
	ePE_SRC_READY,
	ePE_SRC_DISABLED,
	ePE_SRC_CAPABILITY_RESPONSE,
	ePE_SRC_HARD_RESET,
	ePE_SRC_HARD_RESET_RECEIVED,
	ePE_SRC_TRANSITION_TO_DEFAULT,
	ePE_SRC_WAIT_NEW_CAPABILITIES,
    ePE_SRC_VDM_IDENTITY_REQUEST,
    ePE_SRC_VDM_IDENTITY_ACKED,
    ePE_SRC_VDM_IDENTITY_NAKED,
    ePE_SRC_SINK_ALERT_RECEIVED, 
    ePE_SRC_GET_SINK_STATUS,
    ePE_SRC_SEND_SOURCE_ALERT,
    ePE_SRC_GIVE_SOURCE_STATUS,
    ePE_SRC_GIVE_PPS_STATUS,
	//--------------------------Sink States-------------------------------------//
	ePE_SNK_STARTUP,
	ePE_SNK_DISCOVERY,
	ePE_SNK_WAIT_FOR_CAPABILITIES,
	ePE_SNK_EVALUATE_CAPABILITY,
	ePE_SNK_SELECT_CAPABILITY,
	ePE_SNK_TRANSITION_SINK,
	ePE_SNK_READY,
	ePE_SNK_HARD_RESET,
	ePE_SNK_TRANSITION_TO_DEFAULT,
	//--------------------------VCONN SWAP States-----------------------------------//
    ePE_VCS_SEND_SWAP,
    ePE_VCS_EVALUATE_SWAP,
    ePE_VCS_ACCEPT_SWAP,
    ePE_VCS_WAIT_FOR_VCONN,
    ePE_VCS_TURN_OFF_VCONN,
    ePE_VCS_TURN_ON_VCONN,
    ePE_VCS_SEND_PS_RDY,
    //--------------------------DR_SWAP States-----------------------------------//
    ePE_DRS_EVALUATE_SWAP,
    ePE_DRS_ACCEPT_SWAP,
    ePE_DRS_DFP_UFP_ROLE_CHANGE,
    ePE_DRS_SEND_SWAP,
    //--------------------------PR_Swap States-----------------------------------// 
    ePE_PRS_SEND_SWAP, 
    ePE_PRS_EVALUATE_SWAP,
    ePE_PRS_ACCEPT_SWAP,
    ePE_PRS_SRC_SNK_TRANSITION_TO_OFF, 
    ePE_PRS_SRC_SNK_ASSERT_RD, 
    ePE_PRS_SRC_SNK_WAIT_SOURCE_ON, 
    ePE_PRS_SNK_SRC_TRANSITION_TO_OFF,
    ePE_PRS_SNK_SRC_ASSERT_RP,
    ePE_PRS_SNK_SRC_SOURCE_ON,
            
    //--------------------------FR_Swap States-----------------------------------// 
    ePE_FRS_SRC_SNK_EVALUATE_SWAP,
    ePE_FRS_SRC_SNK_ACCEPT_SWAP,
    ePE_FRS_SRC_SNK_TRANSITION_TO_OFF,
    ePE_FRS_SRC_SNK_ASSERT_RD,
    ePE_FRS_SRC_SNK_WAIT_SOURCE_ON,
    ePE_FRS_SNK_SRC_SEND_SWAP,
    ePE_FRS_SNK_SRC_TRANSITION_TO_OFF,
    ePE_FRS_SNK_SRC_VBUS_APPLIED,
    ePE_FRS_SNK_SRC_ASSERT_RP,
    ePE_FRS_SNK_SRC_SOURCE_ON,
    ePE_FRS_HANDLE_ERROR_RECOVERY,
	
    //---------------------------VDM states---------------------------------------//
    ePE_VDM_INITIATE_VDM, 
    ePE_VDM_EVALUATE_VDM,            
    ePE_VDM_RESPOND_VDM,            
    //-------------------------Common States------------------------------//
    ePE_GET_SINK_CAP,
    ePE_SOFT_RESET,
    ePE_SEND_SOFT_RESET,
    ePE_SEND_NOT_SUPPORTED,
    ePE_SEND_REJECT,
	ePE_BIST_MODE,
    ePE_PDFU_MODE,
    ePE_GIVE_CAP,
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
    ePE_SRC_SEND_CAP_GOODCRC_RCVD_SS,
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
    ePE_SRC_TRANSITION_SUPPLY_GOODCRC_RCVD_SS,
    ePE_SRC_TRANSITION_SUPPLY_DRIVE_POWER_SS,
	/* ePE_SRC_READY */
    ePE_SRC_READY_ENTRY_SS,
    ePE_SRC_READY_END_AMS_SS,
    ePE_SRC_READY_IDLE_SS,
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
    ePE_SRC_TRANSITION_TO_DEFAULT_WAIT_FOR_VBUS_OFF_SS,
    ePE_SRC_TRANSITION_TO_DEFAULT_WAIT_FOR_VCONN_OFF_SS,
    ePE_SRC_TRANSITION_TO_DEFAULT_IDLE_SS,
    ePE_SRC_TRANSITION_TO_DEFAULT_SRCRECOVER_TIMEOUT_SS,
    ePE_SRC_TRANSITION_TO_DEFAULT_POWER_ON_SS,
    ePE_SRC_TRANSITION_TO_DEFAULT_WAIT_FOR_VBUS_ON_SS,
    ePE_SRC_TRANSITION_TO_DEFAULT_WAIT_FOR_VCONN_ON_SS,
    ePE_SRC_TRANSITION_TO_DEFAULT_EXIT_SS,
	/* ePE_SRC_VDM_IDENTITY_REQUEST */
    ePE_SRC_VDM_IDENTITY_REQUEST_ENTRY_SS,
    ePE_SRC_VDM_IDENTITY_REQUEST_IDLE_SS,
    ePE_SRC_VDM_IDENTITY_REQUEST_GOODCRC_RCVD_SS,
    ePE_SRC_VDM_IDENTITY_REQUEST_NO_RESPONSE_SS,
    /* ePE_SRC_GET_SINK_STATUS */
    ePE_SRC_GET_SINK_STATUS_ENTRY_SS, 
    ePE_SRC_GET_SINK_STATUS_MSG_DONE_SS,
    ePE_SRC_GET_SINK_STATUS_NO_RESPONSE_SS, 
    ePE_SRC_GET_SINK_STATUS_RESPONSE_RCVD_SS,
    ePE_SRC_GET_SINK_STATUS_IDLE_SS,
    /* ePE_SRC_SEND_SOURCE_ALERT */
    ePE_SRC_SEND_SOURCE_ALERT_ENTRY_SS,
    ePE_SRC_SEND_SOURCE_ALERT_IDLE_SS,
    /* ePE_SRC_GIVE_SOURCE_STATUS */
    ePE_SRC_GIVE_SOURCE_STATUS_ENTRY_SS,
    ePE_SRC_GIVE_SOURCE_STATUS_IDLE_SS,
    /* ePE_SRC_GIVE_PPS_STATUS */
    ePE_SRC_GIVE_PPS_STATUS_ENTRY_SS, 
    ePE_SRC_GIVE_PPS_STATUS_IDLE_SS, 
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
    ePE_SNK_READY_END_AMS_SS,
    ePE_SNK_READY_IDLE_SS,
    /*ePE_SNK_HARD_RESET*/
    ePE_SNK_HARD_RESET_SEND_SS,
    /*ePE_SNK_TRANSITION_TO_DEFAULT*/
    ePE_SNK_TRANSITION_TO_DEFAULT_ENTRY_SS,
    ePE_SNK_TRANSITION_TO_DEFAULT_WAIT_FOR_VCONN_OFF_SS,
    ePE_SNK_TRANSITION_TO_DEFAULT_RESETHW_SS,
    ePE_SNK_TRANSITION_TO_DEFAULT_WAIT_SS,
    //----------------------------VCONN SWAP Sub states-------------------------------//
    /*ePE_VCS_SEND_SWAP*/
    ePE_VCS_SEND_SWAP_ENTRY_SS,
    ePE_VCS_SEND_SWAP_MSG_DONE_SS,
    ePE_VCS_SEND_SWAP_NO_RESPONSE_SS,
    ePE_VCS_SEND_SWAP_IDLE_SS,
    ePE_VCS_SEND_SWAP_REJECT_RCVD_SS,
    ePE_VCS_SEND_SWAP_WAIT_RCVD_SS,
    /*ePE_VCS_WAIT_FOR_VCONN*/
    ePE_VCS_WAIT_FOR_VCONN_START_TIMER_SS,
    ePE_VCS_WAIT_FOR_VCONN_WAIT_FOR_PS_RDY_SS,
    /*ePE_VCS_ACCEPT_SWAP*/
    ePE_VCS_ACCEPT_SWAP_SEND_ACCEPT_SS,
    ePE_VCS_ACCEPT_SWAP_IDLE_SS,
    /*ePE_VCS_SEND_PS_RDY*/
    ePE_VCS_SEND_PS_RDY_ENTRY_SS,
    ePE_VCS_SEND_PS_RDY_IDLE_SS,
    ePE_VCS_SEND_PS_RDY_MSG_DONE_SS,
    
    //---------------------DR_SWAP sub states---------------------------------//
    /*ePE_DRS_ACCEPT_SWAP*/        
    ePE_DRS_ACCEPT_SWAP_SEND_ACCEPT_SS,
    ePE_DRS_ACCEPT_SWAP_IDLE_SS,
    /*ePE_DRS_SEND_SWAP*/
    ePE_DRS_SEND_SWAP_ENTRY_SS,
    ePE_DRS_SEND_SWAP_IDLE_SS,
    ePE_DRS_SEND_SWAP_MSG_DONE_SS,
    ePE_DRS_SEND_SWAP_NO_RESPONSE_SS,
    ePE_DRS_SEND_SWAP_WAIT_RCVD_SS,
    ePE_DRS_SEND_SWAP_REJECT_RCVD_SS,
            
    //--------------------PR_Swap specific Sub-States---------------------------//
    ePE_PRS_SEND_SWAP_ENTRY_SS,
    ePE_PRS_SEND_SWAP_MSG_DONE_SS,
    ePE_PRS_SEND_SWAP_NO_RESPONSE_SS,
    ePE_PRS_SEND_SWAP_IDLE_SS,
    ePE_PRS_SEND_SWAP_REJECT_RCVD_SS,
    ePE_PRS_SEND_SWAP_WAIT_RCVD_SS, 
    ePE_PRS_ACCEPT_SWAP_ENTRY_SS,
    ePE_PRS_ACCEPT_SWAP_IDLE_SS, 
    ePE_PRS_SRC_SNK_TRANSITION_TO_OFF_ENTRY_SS,
    ePE_PRS_SRC_SNK_TRANSITION_TO_OFF_PWROFF_SS,
    ePE_PRS_SRC_SNK_TRANSITION_TO_OFF_EXIT_SS,
    ePE_PRS_SRC_SNK_TRANSITION_TO_OFF_IDLE_SS,
    ePE_PRS_SRC_SNK_WAIT_SOURCE_ON_SEND_PSRDY_SS,
    ePE_PRS_SRC_SNK_WAIT_SOURCE_ON_MSG_DONE_SS,
    ePE_PRS_SRC_SNK_WAIT_SOURCE_ON_ERROR_SS,
    ePE_PRS_SRC_SNK_WAIT_SOURCE_ON_IDLE_SS,
    ePE_PRS_SRC_SNK_WAIT_SOURCE_ON_WAIT_FOR_PSRDY_SS,
    ePE_PRS_SRC_SNK_WAIT_SOURCE_ON_PSRDY_RCVD_SS,
    ePE_PRS_SRC_SNK_WAIT_SOURCE_ON_EXIT_SS,
    ePE_PRS_SNK_SRC_TRANSITION_TO_OFF_ENTRY_SS,
    ePE_PRS_SNK_SRC_TRANSITION_TO_OFF_WAIT_FOR_PSRDY_SS,
    ePE_PRS_SNK_SRC_SOURCE_ON_SEND_PSRDY_SS,
    ePE_PRS_SNK_SRC_SOURCE_ON_MSG_DONE_SS,
    ePE_PRS_SNK_SRC_SOURCE_ON_MSG_ERROR_SS,
    ePE_PRS_SNK_SRC_SOURCE_ON_IDLE_SS,
	ePE_PRS_SNK_SRC_SOURCE_ON_EXIT_SS,
    //--------------------FR_Swap specific Sub-States---------------------------//
    /* ePE_FRS_SRC_SNK_ACCEPT_SWAP */
    ePE_FRS_SRC_SNK_ACCEPT_SWAP_ENTRY_SS,
    ePE_FRS_SRC_SNK_ACCEPT_SWAP_IDLE_SS,
    /* ePE_FRS_SRC_SNK_WAIT_SOURCE_ON */
    ePE_FRS_SRC_SNK_WAIT_SOURCE_ON_SEND_PSRDY_SS,
    ePE_FRS_SRC_SNK_WAIT_SOURCE_ON_MSG_DONE_SS,
    ePE_FRS_SRC_SNK_WAIT_SOURCE_ON_ERROR_SS,
    ePE_FRS_SRC_SNK_WAIT_SOURCE_ON_IDLE_SS,
    ePE_FRS_SRC_SNK_WAIT_SOURCE_ON_WAIT_FOR_PSRDY_SS,
    ePE_FRS_SRC_SNK_WAIT_SOURCE_ON_PSRDY_RCVD_SS,
    ePE_FRS_SRC_SNK_WAIT_SOURCE_ON_EXIT_SS, 
    /* ePE_FRS_SNK_SRC_SEND_SWAP */
    ePE_FRS_SNK_SRC_SEND_SWAP_ENTRY_SS,
    ePE_FRS_SNK_SRC_SEND_SWAP_MSG_DONE_SS,
    ePE_FRS_SNK_SRC_SEND_SWAP_NO_RESPONSE_SS,
    ePE_FRS_SNK_SRC_SEND_SWAP_IDLE_SS,
    /* ePE_FRS_SNK_SRC_TRANSITION_TO_OFF */
    ePE_FRS_SNK_SRC_TRANSITION_TO_OFF_ENTRY_SS,
    ePE_FRS_SNK_SRC_TRANSITION_TO_OFF_WAIT_FOR_PSRDY_SS,
    /* ePE_FRS_SNK_SRC_SOURCE_ON */
    ePE_FRS_SNK_SRC_SOURCE_ON_SEND_PSRDY_SS,
    ePE_FRS_SNK_SRC_SOURCE_ON_MSG_DONE_SS,
    ePE_FRS_SNK_SRC_SOURCE_ON_IDLE_SS,
    ePE_FRS_SNK_SRC_SOURCE_ON_EXIT_SS,
    //--------------------VDM Sub States-------------------------------------------------//            
    /* ePE_VDM_INITIATE_VDM */ 
    ePE_VDM_INITIATE_VDM_ENTRY_SS, 
    ePE_VDM_INITIATE_VDM_MSG_DONE_SS,
    ePE_VDM_INITIATE_VDM_RESPONSE_RCVD_SS,
    ePE_VDM_INITIATE_VDM_BUSY_RCVD_SS,
    ePE_VDM_INITIATE_VDM_NO_RESPONSE_SS, 
    ePE_VDM_INITIATE_VDM_END_AMS_SS, 
    ePE_VDM_INITIATE_VDM_IDLE_SS,
    /* ePE_VDM_RESPOND_VDM */
    ePE_VDM_RESPOND_VDM_ENTRY_SS,
    ePE_VDM_RESPOND_VDM_SVID_SPECIFIC_SS,    
    ePE_VDM_RESPOND_VDM_END_AMS_SS,
    ePE_VDM_RESPOND_VDM_IDLE_SS,
    //--------------------Common States-------------------------------------------------//
    /* ePE_GET_SINK_CAP */
    ePE_GET_SINK_CAP_ENTRY_SS,
    ePE_GET_SINK_CAP_IDLE_SS,
    ePE_GET_SINK_CAP_MSG_DONE_SS,
    ePE_GET_SINK_CAP_NO_RESPONSE_SS,
    ePE_GET_SINK_CAP_RESPONSE_RCVD_SS, 
    /* ePE_SOFT_RESET */        
    ePE_SOFT_RESET_ENTRY_SS, 
    ePE_SOFT_RESET_IDLE_SS,
    /* ePE_SEND_SOFT_RESET */
    ePE_SEND_SOFT_RESET_SOP_SS,
    ePE_SEND_SOFT_RESET_SOP_P_SS,
    ePE_SEND_SOFT_RESET_IDLE_SS,
    ePE_SEND_SOFT_RESET_MSG_DONE_SS,      
    ePE_SEND_SOFT_RESET_P_MSG_DONE_SS,
    ePE_SEND_SOFT_RESET_P_IDLE_SS,
    ePE_DFP_SEND_CBL_RESET_SS,
    /*ePE_SEND_NOT_SUPPORTED*/
    ePE_SEND_NOT_SUPPORTED_ENTRY_SS,
    ePE_SEND_NOT_SUPPORTED_IDLE_SS,
    /*ePE_SEND_REJECT*/
    ePE_SEND_REJECT_ENTRY_SS,
    ePE_SEND_REJECT_MSG_DONE_SS,
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
    /* ePE_GIVE_CAP */ 
    ePE_GIVE_CAP_ENTRY_SS,
    ePE_GIVE_CAP_IDLE_SS,
	ePE_INVALIDSUBSTATE

}ePolicySubState;

/* Macro for Reserved Message Type */
#define PE_RESERVED_MSG_TYPE        0 

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
#define PE_CTRL_GET_SINK_CAP_EXTENDED    22 

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
#define PE_EXT_STATUS               2 
#define PE_EXT_FW_UPDATE_REQUEST    10
#define PE_EXT_FW_UPDATE_RESPONSE   11
#define PE_EXT_PPS_STATUS           12 
#define PE_EXT_SINK_CAPS_EXTD       15 

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
                                            // Bit 2 - NoResponse timer timeout status <p />
                                            // Bit 3 - NAK received status from E-Cable <p />
                                            // Bit 4 - HardReset In Progress Flag <p />
                                            // Bit 5 - PR/FR Swap In Progress Flag <p />
	UINT8 u8PETimerID;                      // Policy Engine Timer ID
	UINT8 u8PENoResponseTimerID;            // NoResponse Timer ID
    UINT8 u8HardResetRcvdISR;               // Hard Reset received status  
#if (TRUE == INCLUDE_PD_VDM)
    UINT8 u8VDMBusyCounter;                 // VDM Busy Counter 
#endif 
}MCHP_PSF_STRUCT_PACKED_END POLICY_ENGINE_STATUS;

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
        This API is called inside the PD Stack initialization API call .

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
        void PE_ResetParams (UINT8 u8PortNum);

    Summary:
        This API resets policy engine parameters for a given port.

    Devices Supported:
        UPD350 REV A

    Description:
        This API resets policy engine parameters for a given port.
 
    Conditions:
        None.

    Input:
        u8PortNum - Port Number.

    Return:
        None.

    Remarks:
        None.
**************************************************************************************************/
void PE_ResetParams (UINT8 u8PortNum);

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
void PE_RunStateMachine (UINT8 u8PortNum);

/**************************************************************************************************
    Function:
        void PE_RunCommonStateMachine (UINT8 u8PortNum , UINT8 *u8DataBuf , UINT8 u8SOPType ,UINT32 u32Header);

    Summary:
        This API is called to run the Common state machine of policy engine that handles the
        VDM messages regardless of the port's type

    Devices Supported:
        UPD350 REV A

    Description:
        This API is called to run the Common state machine of policy engine that handles the
        VDM messages regardless of the port's type

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
void PE_RunCommonStateMachine (UINT8 u8PortNum , UINT8 *pu8DataBuf , UINT8 u8SOPType ,UINT32 u32Header);

/**************************************************************************************************
    Function:
        void PE_ReceiveMsgHandler (UINT8 u8PortNum, UINT32 u32Header, UINT8 *pu8DataBuf);

    Summary:
        This API is called to process the received PD message and do the state transition in 
        policy engine state machine depending on the current policy engine state and sub-state

    Devices Supported:
        UPD350 REV A

    Description:
        This API is called to process the received PD message and do the state transition in 
        policy engine state machine depending on the current policy engine state and sub-state

    Conditions:
        None

    Input:
        u8PortNum - Port Number.
        u32Header  - Header of the received PD message
        pu8DataBuf - Pointer to the 8 bit buffer of received PD message object

    Return:
        None.

    Remarks:
        None.
**************************************************************************************************/
void PE_ReceiveMsgHandler (UINT8 u8PortNum, UINT32 u32Header, UINT8 *pu8DataBuf);

/**************************************************************************************************
    Function:
        UINT8 PE_ValidateMessage (UINT8 u8PortNum, UINT32 u32Header);

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
        PE_PROCESS_EXTDMSG - Return this value if the received message is extended message and to be 
        handled by the PE_ReceiveHandler API

    Remarks:
        None.
**************************************************************************************************/
UINT8 PE_ValidateMessage (UINT8 u8PortNum, UINT32 u32Header);

/**************************************************************************************************
    Function:
        PE_HandleRcvdMsgAndTimeoutEvents (UINT8 u8PortNum, ePolicyState eNextState , ePolicySubState eNextSubState);

    Summary:
        This API is called to see whether timeout set for the received message has occurred before 
        or after the reception of message in protocol layer.

    Devices Supported:
        UPD350 REV A

    Description:
        This API is called to see whether timeout set for the received message has occurred before 
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
void PE_HandleUnExpectedMsg (UINT8 u8PortNum);

/**************************************************************************************************
    Function:
        UINT8 PE_IsMsgUnsupported (UINT8 u8PortNum, UINT16 u16Header);

    Summary:
        This API is called inside the PE_ValidateMessage API to find out whether the received 
        message is supported by the port or not.
       
    Devices Supported:
        UPD350 REV A

    Description:
        This API is called inside the PE_ValidateMessage API to find out whether the received 
        message is supported by the port or not.

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
UINT8 PE_IsMsgUnsupported (UINT8 u8PortNum, UINT16 u16Header);

/**************************************************************************************************
    Function:
       void PE_StateChange_TimerCB (UINT8 u8PortNum, UINT8 u8PEState);

    Summary:
        This API is called inside the timer module once the software timer times out to change the
        policy engine state to a given value
       
    Devices Supported:
        UPD350 REV A

    Description:
         This API is called inside the timer module once the software timer times out to change the
         policy engine sub-state to a given value        

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
void PE_StateChange_TimerCB (UINT8 u8PortNum, UINT8 u8PEState);

/**************************************************************************************************
    Function:
        void PE_SubStateChange_TimerCB (UINT8 u8PortNum, UINT8 u8PESubState);

    Summary:
       This API is called inside the timer module once the software timer times out to change the
       policy engine sub-state to a given value

    Devices Supported:
        UPD350 REV A

    Description:
        This API is called inside the timer module once the software timer times out to change the
       policy engine sub-state to a given value
        
    Conditions:
        None

    Input:
        u8PortNum     - Port Number.
        u8PESubState  - Policy engine Sub-state to change after timeout

    Return:
        None.

    Remarks:
        None.

**************************************************************************************************/
void PE_SubStateChange_TimerCB (UINT8 u8PortNum, UINT8 u8PESubState);

/**************************************************************************************************
    Function:
        void PE_SSChngAndTimeoutValidate_TimerCB (UINT8 u8PortNum, UINT8 u8PESubState);

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
        u8PESubState  -Policy engine Sub-state to change after timeout

    Return:
        None.

    Remarks:
        None.

**************************************************************************************************/
void PE_SSChngAndTimeoutValidate_TimerCB (UINT8 u8PortNum, UINT8 u8PESubState);

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
        void PE_SendHardReset (UINT8 u8PortNum);

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
void PE_SendHardReset (UINT8 u8PortNum);

/**************************************************************************************************
    Function:
        void PE_RunSrcStateMachine (UINT8 u8PortNum, UINT8 *pu8DataBuf, UINT32 u32Header);

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
        u32Header  - Header of the received PD message

    Return:
        None.

    Remarks:
        None.

**************************************************************************************************/
void PE_RunSrcStateMachine (UINT8 u8PortNum, UINT8 *pu8DataBuf, UINT32 u32Header);

/**************************************************************************************************
    Function:
        void PE_NoResponse_TimerCB (UINT8 u8PortNum, UINT8 u8DummyPE_State);

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
void PE_NoResponse_TimerCB (UINT8 u8PortNum, UINT8 u8DummyPE_State);

/**************************************************************************************************
    Function:
        void PE_RunSnkStateMachine (UINT8 u8PortNum, UINT8 *pu8DataBuf, UINT32 u32Header);

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
        u32Header  - Header of the received PD message

    Return:
        None.

    Remarks:
        None.
**************************************************************************************************/
void PE_RunSnkStateMachine (UINT8 u8PortNum, UINT8 *pu8DataBuf, UINT32 u32Header);

/**************************************************************************************************
    Function:
        UINT8 PE_IsPolicyEngineIdle (UINT8 u8PortNum)
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
UINT8 PE_IsPolicyEngineIdle (UINT8 u8PortNum); 

/**************************************************************************************************
    Function:
        void PE_RunPRSwapStateMachine (UINT8 u8PortNum);

    Summary:
        PR_Swap Policy Engine State Machine. 
       
    Devices Supported:
        UPD350 REV A

    Description:
        This API is called to run the Power Role Swap Policy Engine State Machine.
        
    Conditions:
        None

    Input:
        u8PortNum - Port Number.

    Return:
        None.

    Remarks:
        None.

**************************************************************************************************/
void PE_RunPRSwapStateMachine (UINT8 u8PortNum);

/**************************************************************************************************
    Function:
        void PE_RunFRSwapStateMachine (UINT8 u8PortNum);

    Summary:
        FR_Swap Policy Engine State Machine. 
       
    Devices Supported:
        UPD350 REV A

    Description:
        This API is called to run the Fast Role Swap Policy Engine State Machine.
        
    Conditions:
        None

    Input:
        u8PortNum - Port Number.

    Return:
        None.

    Remarks:
        None.

**************************************************************************************************/
void PE_RunFRSwapStateMachine (UINT8 u8PortNum);

/**************************************************************************************************
    Function:
        void PE_RunDRSwapStateMachine (UINT8 u8PortNum);

    Summary:
        DR_SWAP Policy Engine State Machine. 
       
    Devices Supported:
        UPD350 REV A

    Description:
        This API is called to run the DR Swap Policy Engine State Machine.
        
    Conditions:
        None

    Input:
        u8PortNum - Port Number.

    Return:
        None.

    Remarks:
        None.

**************************************************************************************************/
void PE_RunDRSwapStateMachine (UINT8 u8PortNum);

/**************************************************************************************************
    Function:
        void PE_RunVCONNSwapStateMachine (UINT8 u8PortNum);

    Summary:
        VCONN_SWAP Policy Engine State Machine. 
       
    Devices Supported:
        UPD350 REV A

    Description:
        This API is called to run the VCONN Swap Policy Engine State Machine.
        
    Conditions:
        None

    Input:
        u8PortNum - Port Number.

    Return:
        None.

    Remarks:
        None.

**************************************************************************************************/
void PE_RunVCONNSwapStateMachine (UINT8 u8PortNum);

/**************************************************************************************************
    Function:
        void PE_RunVDMStateMachine (UINT8 u8PortNum, UINT8 *pu8DataBuf, UINT32 u32Header);

    Summary:
        Vendor Defined Message Policy Engine State Machine. 
       
    Devices Supported:
        UPD350 REV A

    Description:
        This API is called to run the VDM Policy Engine State Machine.
        
    Conditions:
        None

    Input:
        u8PortNum - Port Number.
        pu8DataBuf - Pointer to the 8 bit buffer of received PD message object
        u32Header - Header of the received PD message

    Return:
        None.

    Remarks:
        None.

**************************************************************************************************/
void PE_RunVDMStateMachine (UINT8 u8PortNum, UINT8 *pu8DataBuf, UINT32 u32Header); 

#endif /*_POLICY_ENGINE_H_*/
