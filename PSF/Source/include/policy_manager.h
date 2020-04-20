/*******************************************************************************
Policy Manager Header

Company:
    Microchip Technology Inc.

File Name:
    policy_manager.h

Description:
    This header file contains the data structures, constant definition,
		function prototypes and definitions for Policy Manager
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

#ifndef _POLICY_MANAGER_H_
#define _POLICY_MANAGER_H_

// *****************************************************************************
// *****************************************************************************
// Section: Define to get DPM Status
// *****************************************************************************
// *****************************************************************************
/***************************Define to get DPM Status*****************************************/
/*Bit definition for u8DPM_ConfigData variable*/
#define DPM_DEFAULT_POWER_ROLE_MASK           BIT(0)
#define DPM_DEFAULT_DATA_ROLE_MASK            BIT(1)
#define DPM_DEFAULT_PD_SPEC_REV_MASK         (BIT(2) | BIT(3))
#define DPM_VCONN_SWAP_SUPPORT_MASK           BIT(4)

/*Bit position for u8DPM_ConfigData variable*/
#define DPM_DEFAULT_POWER_ROLE_POS           0
#define DPM_DEFAULT_DATA_ROLE_POS            1
#define DPM_DEFAULT_PD_SPEC_REV_POS          2

/*Defines for getting data from u8DPM_ConfigData variable*/
#define DPM_GET_DEFAULT_POWER_ROLE(u8PortNum)         ((gasDPM[u8PortNum].u8DPM_ConfigData & DPM_DEFAULT_POWER_ROLE_MASK) >> DPM_DEFAULT_POWER_ROLE_POS)
#define DPM_GET_DEFAULT_DATA_ROLE(u8PortNum)          ((gasDPM[u8PortNum].u8DPM_ConfigData & DPM_DEFAULT_DATA_ROLE_MASK) >> DPM_DEFAULT_DATA_ROLE_POS)
#define DPM_GET_DEFAULT_PD_SPEC_REV(u8PortNum)        ((gasDPM[u8PortNum].u8DPM_ConfigData & DPM_DEFAULT_PD_SPEC_REV_MASK) >> DPM_DEFAULT_PD_SPEC_REV_POS)

/*Bit definition for u8DPM_Status variable*/
#define DPM_CURR_POWER_ROLE_MASK            BIT(0)
#define DPM_CURR_DATA_ROLE_MASK             BIT(1)
#define DPM_CURR_PD_SPEC_REV_MASK          (BIT(2) | BIT(3))
#define DPM_VDM_STATE_ACTIVE_MASK           BIT(4)

/*Bit position for u8DPM_Status variable*/
#define DPM_CURR_POWER_ROLE_POS            0
#define DPM_CURR_DATA_ROLE_POS             1
#define DPM_CURR_PD_SPEC_REV_POS           2
#define DPM_VDM_STATE_ACTIVE_POS           4

/*Defines for getting data from u8DPM_Status variable*/
#define DPM_GET_CURRENT_POWER_ROLE(u8PortNum)         ((gasDPM[u8PortNum].u8DPM_Status & DPM_CURR_POWER_ROLE_MASK) >> DPM_CURR_POWER_ROLE_POS)
#define DPM_GET_CURRENT_DATA_ROLE(u8PortNum)          ((gasDPM[u8PortNum].u8DPM_Status & DPM_CURR_DATA_ROLE_MASK) >> DPM_CURR_DATA_ROLE_POS)
#define DPM_GET_CURRENT_PD_SPEC_REV(u8PortNum)        ((gasDPM[u8PortNum].u8DPM_Status & DPM_CURR_PD_SPEC_REV_MASK) >> DPM_CURR_PD_SPEC_REV_POS)

// *****************************************************************************
// *****************************************************************************
// Section: Defines to get data from given PDO
// *****************************************************************************
// *****************************************************************************   
/*Macros to get PDO type, PDO Current, PDO voltage,USB Comm capable bit from
Source/Sink Power delivery objects*/

/*Defines for getting voltage from PDO*/
#define DPM_PDO_VOLTAGE_MASK                 0x3FF
#define DPM_PDO_VOLTAGE_POS                  10
#define DPM_PDO_VOLTAGE_UNIT                 50
#define DPM_GET_VOLTAGE_FROM_PDO_MILLI_V(u32PDO)    (((u32PDO >> DPM_PDO_VOLTAGE_POS) & DPM_PDO_VOLTAGE_MASK) * DPM_PDO_VOLTAGE_UNIT)

/*Defines for getting current from PDO[9:0]*/
#define DPM_PDO_CURRENT_MASK                 0x1FF
#define DPM_PDO_CURRENT_UNIT                 10
#define DPM_GET_CURRENT_FROM_PDO_MILLI_A(u32PDO)    ((u32PDO & DPM_PDO_CURRENT_MASK) * DPM_PDO_CURRENT_UNIT)

#define DPM_GET_PDO_TYPE(X)   					((X & 0xC0000000) >> 30)
#define DPM_GET_PDO_CURRENT(X)                  ((X & 0x000003FF))
#define DPM_GET_PDO_VOLTAGE(X)                  ((X & 0x000FFC00) >> 10)	/*in 50mv units*/
#define DPM_GET_PDO_USB_COMM_CAP(X)             ((X & 0x04000000) >> 26)
#define DPM_MAX_PDO_CNT                          7

/*******************************************************************************/

// *****************************************************************************
// *****************************************************************************
// Section: Data types and constants
// *****************************************************************************
// *****************************************************************************
/********************************************Define to form Data Request*******/
#define DPM_FORM_DATA_REQUEST(OBJECT_POSITION,CAPABLITY_MISMATCH,GIVEBACK_FLAG, \
        USB_COMMUNICATION_CAPABLE,NO_USB_SUSP,OPERATING_CURRENT,MAXIMUM_OPERATING_CURRENT)\
        ((OBJECT_POSITION << 28) | (GIVEBACK_FLAG << 27) | (CAPABLITY_MISMATCH << 26) | \
        (USB_COMMUNICATION_CAPABLE << 25) | (NO_USB_SUSP << 24) | (OPERATING_CURRENT << 10) \
          | (MAXIMUM_OPERATING_CURRENT))

/********************* Macros for E-Cable ***************************/
#define DPM_VDM_GET_CMD_TYPE(u32VDMHeader)      ((u32VDMHeader & DPM_VDM_CMD_TYPE_MASK) >> \
                                                        DPM_VDM_CMD_TYPE_POS)

#define DPM_GET_CABLE_CUR_VAL(u32ProductTypeVDO)    ((u32ProductTypeVDO & DPM_CABLE_CUR_VAL_BIT_MASK) >> \
                                                        DPM_CABLE_CUR_VAL_BIT_POS)

#define DPM_CABLE_CUR_VAL_BIT_MASK       (BIT(5) | BIT(6))
#define DPM_CABLE_CUR_VAL_BIT_POS        5

#define DPM_VDM_CMD_TYPE_MASK            (BIT(6) | BIT(7))
#define DPM_VDM_CMD_TYPE_POS             6               

#define DPM_CABLE_CURR_3A                1
#define DPM_CABLE_CURR_5A                2

#define DPM_CABLE_CURR_3A_UNIT           300
#define DPM_CABLE_CURR_5A_UNIT           500

#define DPM_VDM_HEADER_POS              0
#define DPM_VMD_PRODUCT_TYPE_VDO_POS    4

#define DPM_DEBUG_PDO_GENERATION( USB_SUSPEND, UNCONS_POWER, USB_COM, MAX_CURRENT, MAX_VOLTAGE)  \
        (((UINT32)USB_SUSPEND << 28) | ((UINT32)UNCONS_POWER << 27) | ((UINT32)USB_COM << 26) | (((UINT32)MAX_VOLTAGE/50) << 10) | (((UINT32)MAX_CURRENT)/10))

#define DPM_DEBUG_PDO_5V_9MA      DPM_DEBUG_PDO_GENERATION(1, 1, 0, 900, 5000)
#define DPM_DEBUG_PDO_5V_1P5A     DPM_DEBUG_PDO_GENERATION(1, 1, 0, 1500, 5000)
#define DPM_DEBUG_PDO_5V_3A       DPM_DEBUG_PDO_GENERATION(1, 1, 0, 3000, 5000)

/****************Defines to get Status from u8DPM_Status****************************/
#define DPM_GET_DPM_STATUS(u8PortNum)				gasDPM[u8PortNum].u8DPM_Status
/*Defines for getting data by passing u8DPM_Status variable*/
#define DPM_GET_CURRENT_POWER_ROLE_FRM_STATUS(u8DPM_Status)   ((u8DPM_Status & DPM_CURR_POWER_ROLE_MASK) >> DPM_CURR_POWER_ROLE_POS)
#define DPM_GET_CURRENT_DATA_ROLE_FRM_STATUS(u8DPM_Status)    ((u8DPM_Status & DPM_CURR_DATA_ROLE_MASK) >> DPM_CURR_DATA_ROLE_POS)
#define DPM_GET_CURRENT_PD_SPEC_REV_FRM_STATUS(u8DPM_Status)  ((u8DPM_Status & DPM_CURR_PD_SPEC_REV_MASK) >> DPM_CURR_PD_SPEC_REV_POS)

/********************** Return Values from PE_ValidateMessage API**************/
#define DPM_VALID_REQUEST            1
#define DPM_INVALID_REQUEST          0   

/***********Macros for u8VConnEnable argument of DPM_VConnOnOff API ************/
#define DPM_VCONN_ON             1
#define DPM_VCONN_OFF            0

/*********Macros for u8VbusOnorOff argument of DPM_TypeCSrcVBus5VOnOff API*********/
#define DPM_VBUS_ON              1
#define DPM_VBUS_OFF             0

/**** Macros for PDOIndex Port Power Handling for Typec Vsafe0V and Vsafe5V****/
#define DPM_VSAFE0V_PDO_INDEX   0
#define DPM_VSAFE5V_PDO_INDEX_1 1

/*defines to set u16SinkOperatingCurrInmA */
#define DPM_0mA     0
#define DPM_500mA   500
#define DPM_900mA   900
#define DPM_1000mA  1000
#define DPM_1500mA  1500
#define DPM_2000mA  2000
#define DPM_3000mA  3000
#define DPM_4000mA  4000
#define DPM_5000mA  5000

/*defines to convert voltage and current in terms of 50mV and 10mA to mV and mA respectively*/
#define DPM_10mA    10
#define DPM_50mV    50

/***************************************u8PowerFaultISR defines*************** */
#define DPM_POWER_FAULT_OVP				BIT(0)
#define DPM_POWER_FAULT_UV				BIT(1)
#define DPM_POWER_FAULT_VBUS_OCS		BIT(2)
#define DPM_POWER_FAULT_VCONN_OCS	    BIT(3)

/* ****************************** Port Connection Status parameters *********** */
#define DPM_PORT_ATTACHED_STATUS                              BIT(0)
#define DPM_PORT_ORIENTATION_FLIPPED_STATUS                   BIT(1)
#define DPM_PORT_DATA_ROLE_STATUS                             BIT(2)
#define DPM_PORT_POWER_ROLE_STATUS                            BIT(3)
#define DPM_PORT_VCONN_STATUS                                 BIT(4)
#define DPM_PORT_CABLE_REDUCED_SRC_CAPABILITIES_STATUS        BIT(5)
#define DPM_PORT_PD_BAL_REDUCED_SRC_CAPABILITIES_STATUS       BIT(6)
#define DPM_PORT_SRC_CAPABILITY_MISMATCH_STATUS               BIT(7)
#define DPM_PORT_AS_SRC_PD_CONTRACT_GOOD_STATUS               BIT(8)
#define DPM_PORT_AS_SRC_RDO_ACCEPTED_STATUS                   BIT(9)
#define DPM_PORT_AS_SRC_RDO_REJECTED_STATUS                   BIT(10)
#define DPM_PORT_AS_SNK_LAST_REQ_ACCEPT_STATUS                BIT(11)
#define DPM_PORT_AS_SNK_LAST_REQ_REJECT_STATUS                BIT(12)
#define DPM_PORT_AS_SNK_LAST_REQ_PS_RDY_STATUS                BIT(13)
#define DPM_PORT_SINK_CAPABILITY_MISMATCH_STATUS              BIT(14)
#define DPM_PORT_RP_VAL_DETECT_DEFAULT_USB_STATUS             BIT(15)
#define DPM_PORT_RP_VAL_DETECT_1_5A_STATUS                    BIT(16)
#define DPM_PORT_RP_VAL_DETECT_3A_STATUS                     (BIT(15)|BIT(16))
#define DPM_PORT_RP_VAL_DETECT_MASK_STATUS                   (BIT(15)|BIT(16))

/* *************************Port IO Status parameters *********************** */
#define DPM_PORT_IO_EN_DC_DC_STATUS                  BIT(0)
#define DPM_PORT_IO_VSEL0_STATUS                     BIT(1)
#define DPM_PORT_IO_VSEL1_STATUS                     BIT(2)
#define DPM_PORT_IO_VSEL2_STATUS                     BIT(3)
#define DPM_PORT_IO_EN_VBUS_STATUS                   BIT(4)
#define DPM_PORT_IO_VBUS_DIS_STATUS                  BIT(5)
#define DPM_PORT_IO_EN_SINK_STATUS                   BIT(6)
#define DPM_PORT_IO_15_IND_STATUS                    BIT(7)
#define DPM_PORT_IO_30_IND_STATUS                    BIT(8)
#define DPM_PORT_IO_CAP_MISMATCH_STATUS              BIT(9)

/*********************u8SinkConfigSel defines******************/
#define DPM_SINK_CONFIG_SINK_MODE_SEL_MASK  (BIT(0) | BIT(1))
#define DPM_SINK_MODE_A     0x00
#define DPM_SINK_MODE_B      BIT(0)

/*********************u8DAC_I_Direction defines****************/
#define DPM_DAC_DIR_HIGH_AMP_MAX_VOLT 0
#define DPM_DAC_DIR_HIGH_AMP_MIN_VOLT 1

#define DPM_SINK_CONFIG_NO_USB_SUSP_POS        2
#define DPM_SINK_CONFIG_NO_USB_SUSP_MASK       BIT(2)

#define DPM_SINK_CONFIG_GIVE_BACK_FLAG_POS  3
#define DPM_SINK_CONFIG_GIVE_BACK_FLAG_MASK BIT(3)

/*********************Defines for evaluating and sorting PDOs******************/
#define DPM_2_DIMENSION_TO_STORE_INDEX_AND_PWR 2
#define DPM_PDO_WITH_MAX_PWR 0
#define DPM_PDO_PWR 0
#define DPM_PDO_INDEX 1
#define DPM_NEXT_PWR_INDEX(x) (x+1)

#define DPM_4BYTES_FOR_EACH_PDO_OF(PDO_Count) (PDO_Count*4)
// *****************************************************************************
// *****************************************************************************
// Section: Data Structures
// *****************************************************************************
// *****************************************************************************
/*Structure of Device Policy Manager*/
typedef struct MCHP_PSF_STRUCT_PACKED_START
{
  UINT8 u8DPM_ConfigData;   //Bit 0 - Default Port Role <p />
                            //Bit 1 - Default Data Role <p />
                            //Bit 3:2 - Default PD Spec Revision <p />
                            //Bit 4 - Default Vconn Swap support
  UINT8 u8DPM_Status;       //Bit 0 - Status of Port Role <p />
                            //Bit 1 - Status of Data Role <p />
                            //Bit 3:2 - Status of PD Spec Revision <p />
                            //Bit 4 - Status of Vconn Swap support
  UINT8 u8VCONNErrCounter;
  UINT8 u8NegotiatedPDOIndex;
  UINT16 u16MaxCurrSupportedin10mA;   //Maximum current supported by E-Cable in 10mA
  UINT16 u16SinkOperatingCurrInmA;   //Operating current
  UINT32  u32NegotiatedPDO;     //NegotiatedPDO
 
  #if (TRUE == INCLUDE_POWER_FAULT_HANDLING)
	  UINT8 u8VBUSPowerGoodTmrID;     //VBUS PowerGood Timer ID
      UINT8 u8VCONNPowerGoodTmrID;    //VConn PowerGood Timer ID
	  UINT8 u8VBUSPowerFaultCount;      //VBUS Power fault count
      UINT8 u8VCONNPowerFaultCount;     //VConn Power fault count     
      UINT8 u8VCONNGoodtoSupply;        //Vconn good to supply
	  UINT8 u8PowerFaultISR;          //Power fault ISR flag
	  UINT8 u8HRCompleteWait;         //Hard Reset complete wait flag
      UINT8 u8TypeCErrRecFlag;        //Type-C Error Recovery Flag
  #endif
  
}MCHP_PSF_STRUCT_PACKED_END DEVICE_POLICY_MANAGER;

/************************Client Request Enum******************************/
typedef enum DPM_ClientRequest
{    
    eMCHP_PSF_DPM_RENEGOTIATE=1,
    eMCHP_PSF_DPM_HANDLE_VBUS_FAULT,
    eMCHP_PSF_DPM_GET_SNK_CAPS
}eMCHP_PSF_DPM_ClientRequest;

/* Enumeration to define the types of PDO */ 
typedef enum PDOtype
{
    ePDO_FIXED = 0x00,
    ePDO_BATTERY = 0x01,
    ePDO_VARIABLE = 0x02,
    ePDO_INVALID = 0xFF
} ePDOtypes;

/**************************************************************************************************
Summary:
	PSF DPM Client Request Type enum
Description:
    eMCHP_PSF_DPM_ClientRequest enum defines all the Client Request type 
    DPM can handle in DPM_HandleClientRequest.
 
    eMCHP_PSF_DPM_RENEGOTIATE: To initiate a renegotiation between the port 
    partner, this client request is posted. On completion of the request,
    eMCHP_PSF_PD_CONTRACT_NEGOTIATED is posted in notification DPM callback.
 
    eMCHP_PSF_DPM_HANDLE_VBUS_FAULT: 
    PSF has inbuilt fault mechanism to handle VCONN OCS, VBUS OCS through 
    FAULT_IN pin, Over Voltage and Under voltage. If external VBUS fault is 
    detected by the system, for PSF to handle this fault, Client request 
    eMCHP_PSF_DPM_HANDLE_VBUS_FAULT shall be posted.
    1)	When fault occurs at implicit contract, Type C error recovery is entered. 
    When eMCHP_PSF_DPM_HANDLE_VBUS_FAULT is posted, PSF enters error recovery 
    notifying eMCHP_PSF_ERROR_RECOVERY followed by Type C attach notifying 
    eMCHP_PSF_TYPEC_CC1_ATTACH/ eMCHP_PSF_TYPEC_CC2_ATTACH indicating the request 
    is processed completely.
    2) When a fault is notified during an explicit contract through the client 
    request, HardReset is sent followed by negotiation. It is indicated by 
    eMCHP_PSF_PD_CONTRACT_NEGOTIATED notification indicating request is 
    processed completely.

    eMCHP_PSF_DPM_GET_SNK_CAPS:
    Application can get the sink capability by sending the Get_Sink_Cap 
    Control message by raising a client request eMCHP_PSF_DPM_GET_SNK_CAPS.
    On the request is complete, either eMCHP_PSF_GET_SINK_CAPS_RCVD or 
    eMCHP_PSF_GET_SINK_CAPS_NOT_RCVD notification is posted based on the response 
    received for Get_Sink_Cap message.
*******************************************************************************/	
// *****************************************************************************
// *****************************************************************************
// Section: Interface Routines
// *****************************************************************************
// *****************************************************************************
/**************************************************************************************************
    Function:
        void DPM_Init(UINT8 u8PortNum);
    Summary:
        This API initializes a given Port to the Power Role and Data role based on the port's global variable
    Devices Supported:
        UPD350 REV A
    Description:
        Port initialization of Device policy manager initialize the member of DPM structure 
        "gasDPM[u8PortNum]"
    Conditions:
        This API is called inside the PD Stack initialisation API call .
    Input:
        u8PortNum - Port Number.
    Return:
        None.
    Remarks:
        None.
**************************************************************************************************/
void DPM_Init(UINT8 u8PortNum);
/**************************************************************************************************
    Function:
        void DPM_RunStateMachine (UINT8 u8PortNum);
    Summary:
        This API is called to run the DPM State machine for a given port.
    Devices Supported:
        UPD350 REV A
    Description:
        This API is called to run the Type C and Policy Engine State machine for a given port.
    Conditions:
        None.
    Input:
        u8PortNum - Port Number.
    Return:
        None.
    Remarks:
        None.
**************************************************************************************************/
void DPM_RunStateMachine (UINT8 u8PortNum);
/****************************** DPM APIs Accessing Type C Port Control Module*********************/

/**************************************************************************************************
    Function:
        void DPM_SetPortPower(UINT8 u8PortNum);
    Summary:
        This API is used to drive the VBUS for requested power.
    Devices Supported:
        UPD350 REV A
    Description:
        This API is used to drive the VBUS for requested power.
    Conditions:
        None.
    Input:
        u8PortNum     - Port Number for power to be driven
    Return:
        None
    Remarks:
        None
**************************************************************************************************/
void DPM_SetPortPower(UINT8 u8PortNum);

/****************************** DPM Source related APIs*****************************************/
/**************************************************************************************************
    Function:
        void DPM_Get_Source_Capabilities(UINT8 u8PortNum, UINT8* NumOfPdo, UINT32* pu32DataObj);
    Summary:
        This API is used to get the port source capabilities from the DPM.
    Devices Supported:
        UPD350 REV A
    Description:
         This API is used to get the port source capabilities from the DPM.
    Conditions:
        None.
    Input:
        u8PortNum - Port Number for which source capabilities to be returned.
        NumOfPdo - Pointer to hold number of PDOs supported by the port
        pu32DataObj - Pointer to hold the source capabilities.
    Return:
        None
    Remarks:
        None
**************************************************************************************************/
void DPM_Get_Source_Capabilities(UINT8 u8PortNum, UINT8* NumOfPdo, UINT32* pu32DataObj);

/**************************************************************************************************
    Function:
        UINT8 DPM_ValidateRequest(UINT8 u8PortNum, UINT16 u16Header, UINT8 *u8DataBuf);
    Summary:
        This API is used to validate the received request messgae from the sink port parnter.
    Devices Supported:
        UPD350 REV A
    Description:
         This API is used to validate the received request messgae from the sink port parnter.
    Conditions:
        None.
    Input:
        u8PortNum     - Port Number for which the received request message to be validated
        u16Header - Request message Header
        u8DataBuf - Pointer which holds the data objects of received request message.
    Return:
        DPM_VALID_REQUEST - Received message is valid request
    Remarks:
        None
**************************************************************************************************/
UINT8 DPM_ValidateRequest(UINT8 u8PortNum, UINT16 u16Header, UINT8 *u8DataBuf);

/**************************************************************************************************
    Function:
        DPM_StoreVDMECableData(UINT8 u8PortNum, UINT8 u8SOPType, UINT16 u16Header, UINT32* u8DataBuf);
    Summary:
        This API is used to store the capabilities received from the E-Marked cable.
    Devices Supported:
        UPD350 REV A
    Description:
         This API is used to store the capabilities received from the E-Marked cable.
    Conditions:
        None.
    Input:
        u8PortNum - Port Number for which E-Cable data to be stored
        u8SOPType - Sop type of received 
        u16Header - Header of received Discover Identity command response
        u8DataBuf - Pointer which holds the data objects of received Discover Identity command response.
    Return:
        None
    Remarks:
        None
**************************************************************************************************/
UINT8 DPM_StoreVDMECableData(UINT8 u8PortNum, UINT8 u8SOPType, UINT16 u16Header, UINT32* u8DataBuf);

/**************************************************************************************************
    Function:
        void DPM_VCONNPowerGood_TimerCB (UINT8 u8PortNum, UINT8 u8DummyVariable);
    Summary:
        This API is given as the timer call back API when starting the VCONNPowerGood Timer from 
        Type-C state machine.
    Devices Supported:
        UPD350 REV A
    Description:
        This API is given as the timer call back API when starting the VCONNPowerGood Timer from 
        Type-C state machine. 
    Conditions:
        None
    Input:
        u8PortNum       - Port Number.
        u8DummyVariable - Dummy Argument
    Return:
        None.
    Remarks:
        None.
**************************************************************************************************/
void DPM_VCONNPowerGood_TimerCB (UINT8 u8PortNum, UINT8 u8DummyVariable);

/**************************************************************************************************
    Function:
        void DPM_VConnOnOff(UINT8 u8PortNum, UINT8 u8VConnEnable);
    Summary:
        This API is used to turn on/off the VCONN supply of a given port
    Devices Supported:
        UPD350 REV A
    Description:
         This API is used to turn on/off the VCONN of a given port
    Conditions:
        None.
    Input:
        u8PortNum     - Port Number for which the sink capability to be returned
        u8VConnEnable - Variable denoting either to turn on or off the VCONN
                      - DPM_VCONN_ON (Turn on VCONN)
                      - DPM_VCONN_OFF (Turn off VCONN)
    Return:
        None
    Remarks:
        None
**************************************************************************************************/
void DPM_VConnOnOff(UINT8 u8PortNum, UINT8 u8VConnEnable);

/**************************************************************************************************
    Function:
        UINT16 DPM_GetVBUSVoltage(UINT8 u8PortNum);
    Summary:
        This API returns the VBUS level of a given port
    Devices Supported:
        UPD350 REV A
    Description:
        This API returns the VBUS level of a given port
    Conditions:
        None.
    Input:
        u8PortNum       - Port Number for which the sink capability to be returned
    Return:
        UINT16
    Remarks:
        None
**************************************************************************************************/
UINT16 DPM_GetVBUSVoltage(UINT8 u8PortNum);
/**************************************************************************************************
    Function:
        void DPM_GetTypeCStates(UINT8 u8PortNum, UINT8 *pu8TypeCState, UINT8 *pu8TypeCSubState);
    Summary:
        This API is used to get the present Type C state and Type C sub state for a given port
    Devices Supported:
        UPD350 REV A
    Description:
         This API is used to get the present Type C state and Type C sub state for a given port
    Conditions:
        None.
    Input:
        u8PortNum       - Port Number for which the sink capability to be returned
        *pu8TypeCState   -  Pointer holding the Type C State
        *pu8TypeCSubState - Pointer holding the Type C Sub State
    Return:
        None
    Remarks:
        None
**************************************************************************************************/
void DPM_GetTypeCStates(UINT8 u8PortNum, UINT8 *pu8TypeCState, UINT8 *pu8TypeCSubState);

/**************************************************************************************************
    Function:
        void DPM_SetTypeCState(UINT8 u8PortNum, UINT8 u8TypeCState, UINT8 u8TypeCSubState);
    Summary:
        This API is used to set the state and sub state of Type C State machine for a given port
    Devices Supported:
        UPD350 REV A
    Description:
         This API is used to set the state and sub state of Type C State machine for a given port
    Conditions:
        None.
    Input:
        u8PortNum       - Port Number for which the sink capability to be returned
        u8TypeCState   -  Type C State to be set
        u8TypeCSubState - Type C SubState to be set
    Return:
        None
    Remarks:
        None
**************************************************************************************************/
void DPM_SetTypeCState(UINT8 u8PortNum, UINT8 u8TypeCState, UINT8 u8TypeCSubState);

/**************************************************************************************************
    Function:
        void DPM_GetPoweredCablePresence(UINT8 u8PortNum, UINT8 *pu8RaPresence);
    Summary:
        This API is used to find out the presence of powered cable in the CC lines for a given port
    Devices Supported:
        UPD350 REV A
    Description:
         This API is used to find out the presence of powered cable in the CC lines for a given port
    Conditions:
        None.
    Input:
        u8PortNum       - Port Number for which the sink capability to be returned
        *pu8RaPresence   - Pointer returning the Powered cable presence
    Return:
        None
    Remarks:
        None
**************************************************************************************************/
void DPM_GetPoweredCablePresence(UINT8 u8PortNum, UINT8 *pu8RaPresence);

/**************************************************************************************************
    Function:
        void DPM_TypeCSrcVBus5VOnOff(UINT8 u8PortNum, UINT8 u8VbusOnorOff);
    Summary:
        This API drives the VBUS line of a given port to either 0V or 5V.
    Devices Supported:
        UPD350 REV A
    Description:
        This API drives the VBUS line of a given port to either 0V or 5V depending on the value of the argument 
    u8VbusOnorOff
    Conditions:
        None.
    Input:
        u8PortNum       - Port Number for which the sink capability to be returned
        u8VbusOnorOff   - Variable denoting either to turn on or off the VBUS
                        - DPM_VBUS_ON (Turn on VBUS)
                        - DPM_VBUS_OFF (Turn off VBUS)
    Return:
        None
    Remarks:
        None
**************************************************************************************************/
void DPM_TypeCSrcVBus5VOnOff(UINT8 u8PortNum, UINT8 u8VbusOnorOff);

/**************************************************************************************************
    Function:
        UINT8 DPM_IsPort_VCONN_Source(UINT8 u8PortNum);
    Summary:
        This API is used find whether the port is currently sourcing VCONN or not
    Devices Supported:
        UPD350 REV A
    Description:
        This API is used find whether the port is currently sourcing VCONN or not. It returns 1 if it is a 
    VCONN Source and 0 if it does not supply VCONN currently
    Conditions:
        None.
    Input:
        u8PortNum  - Port Number for which the sink capability to be returned
    Return:
        UINT8

    Remarks:
        None
**************************************************************************************************/
UINT8 DPM_IsPort_VCONN_Source(UINT8 u8PortNum);

/**************************************************************************************************
    Function:
        void DPM_Evaluate_Received_Src_caps(UINT8 u8PortNum ,UINT16 u16RecvdSrcCapsHeader, UINT32 *u32RecvdSrcCapsPayload);

    Summary:
        Device Policy Manager evaluates the received source capability aganist the sink capability and 
    chooses the nearest match PDO for the data request message

    Devices Supported:
        UPD350 REV A
    Description:
        This Function is called by the policy engine to the Device policy Manager to choose the PDO for the 
    data request message

    Conditions:
        None.
    Input:
        u8PortNum               - Port Number for which the sink capability to be returned
        u16RecvdSrcCapsHeader  -  Header of the received source capability message
        u32RecvdSrcCapsPayload  -  Pointer variable holding the Data object of Source Capability message
    Return:
    None

    Remarks:
    None
**************************************************************************************************/
void DPM_Evaluate_Received_Src_caps(UINT8 u8PortNum ,UINT16 u16RecvdSrcCapsHeader, UINT32 *u32RecvdSrcCapsPayload);

/**************************************************************************************************
    Function:
        UINT8 DPM_Find_Src_Sink_Caps_match(UINT32 u32In_SrcPDO, UINT32 u32In_SinkPDO);
    Summary:
        Device Policy Manager compares a given source PDO and a sink PDO and returns the match
    Devices Supported:
        UPD350 REV A
    Description:
        This Function is used to find the match between the single source and sink PDO
    Conditions:
        None.
    Input:

        u32In_SrcPDO  -  Given Source PDO
        u32In_SinkPDO  - Given Sink PDO
    Return:
        UINT8
    Remarks:
        None
**************************************************************************************************/
UINT8 DPM_Find_Src_Sink_Caps_match(UINT32 u32In_SrcPDO, UINT32 u32In_SinkPDO);

/**************************************************************************************************
    Function:
        void DPM_Get_Sink_Capabilities(UINT8 u8PortNum,UINT8* u8pSinkPDOCnt, UINT32 * pu32DataObj);
    Summary:
        Device Policy Manager updates the policy engine about the sink capability for a given port
    Devices Supported:
        UPD350 REV A
    Description:
        This Function is called by the policy engine to the Device policy Manager to get the sink capability Power data object for a given port
    Conditions:
        None.
    Input:

        u8PortNum      - Port Number for which the sink capability to be returned
        u8pSinkPDOCnt  - Pointer variable holding the PDO count of sink capability
        pu32DataObj   - Pointer variable holding the Data object of Sink Capability message
    Return:
        None
    Remarks:
        None
**************************************************************************************************/
void DPM_Get_Sink_Capabilities(UINT8 u8PortNum, UINT8* u8pSinkPDOCnt, UINT32 * pu32DataObj);

/**************************************************************************************************
    Function:
        void DPM_VBUSOnOffTimerCB (UINT8 u8PortNum, UINT8 u8DummyVariable);
    Summary:
        This API is given as the timer call back API when starting the VBUSOnOff Timer from 
        Type-C and source policy engine state machines.
    Devices Supported:
        UPD350 REV A
    Description:
        This API is given as the timer call back API when starting the VBUSOnOff Timer from 
        Type-C and source policy engine state machines.  
    Conditions:
        None
    Input:
        u8PortNum       - Port Number.
        u8DummyVariable - Dummy Argument
    Return:
        None.
    Remarks:
        None.

**************************************************************************************************/
void DPM_VBUSOnOffTimerCB (UINT8 u8PortNum, UINT8 u8DummyVariable);

/**************************************************************************************************
    Function:
        void DPM_SrcReadyTimerCB (UINT8 u8PortNum, UINT8 u8DummyVariable);
    Summary:
        This API is given as the timer call back API when starting the tSrcReady Timer from 
        Type-C and source policy engine state machines.     
    Devices Supported:
        UPD350 REV A
    Description:
        This API is given as the timer call back API when starting the tSrcReady Timer from 
        Type-C and source policy engine state machines.  
    Conditions:
        None
    Input:
        u8PortNum       - Port Number.
        u8DummyVariable - Dummy Argument
    Return:
        None.
    Remarks:
        None.

**************************************************************************************************/
void DPM_SrcReadyTimerCB (UINT8 u8PortNum, UINT8 u8DummyVariable);

/**************************************************************************************************
    Function:
        void DPM_VCONNONTimerErrorCB (UINT8 u8PortNum , UINT8 u8DummyVariable);
    Summary:
        This API is given as the timer call back API when starting the VCONN ON Timer from 
        Type-C and source policy engine state machines.
    Devices Supported:
        UPD350 REV A
    Description:
        This API is given as the timer call back API when starting the VCONN ON Timer from 
        Type-C and source policy engine state machines.
    Conditions:
        None
    Input:
        u8PortNum       - Port Number.
        u8DummyVariable - Dummy Argument
    Return:
        None.
    Remarks:
        None.
**************************************************************************************************/
void DPM_VCONNONTimerErrorCB (UINT8 u8PortNum , UINT8 u8DummyVariable);

/**************************************************************************************************
    Function:
        void DPM_VCONNOFFErrorTimerCB (UINT8 u8PortNum , UINT8 u8DummyVariable);
    Summary:
        This API is given as the timer call back API when starting the VCONN OFF Timer from 
        Type-C and source policy engine state machines.
    Devices Supported:
        UPD350 REV A
    Description:
        This API is given as the timer call back API when starting the VCONN OFF Timer from 
        Type-C and source policy engine state machines.    
    Conditions:
        None
    Input:
        u8PortNum       - Port Number.
        u8DummyVariable - Dummy Argument
    Return:
        None.

    Remarks:
        None.

**************************************************************************************************/
void DPM_VCONNOFFErrorTimerCB (UINT8 u8PortNum , UINT8 u8DummyVariable);

/**************************************************************************************************
    Function:
        void DPM_ResetVCONNErrorCnt (UINT8 u8PortNum);
    Summary:
        This API is used to reset the VCONN Error Count  
    Devices Supported:
        UPD350 REV A
    Description:
        This API is used to reset the VCONN Error Count to Zero.
    Conditions:
        None
    Input:
        u8PortNum       - Port Number.
    Return:
        None.
    Remarks:
        None.
**************************************************************************************************/
void DPM_ResetVCONNErrorCnt (UINT8 u8PortNum);

/**************************************************************************************************
    Function:
        void DPM_IsHardResetInProgress (UINT8 u8PortNum);
    Summary:
        This API is used to Identify the Status of HardReset Progress.
    Devices Supported:
        UPD350 REV A
    Description:
        This API is used to Identify the current Status of HardReset Progress.       
    Conditions:
        None
    Input:
        u8PortNum       - Port Number.
    Return:
        0 - Hard Reset Not in Progress
        1 - Hard Reset is in Progress
    Remarks:
        None.

**************************************************************************************************/
UINT8 DPM_IsHardResetInProgress(UINT8 u8PortNum);

/**************************************************************************************************
    Function:
        void DPM_StateMachineInit();
    Summary:
        This API refers to initial the Device Policy Manager 
    Devices Supported:
        UPD350 REV A
    Description:
        This API is used to initialize the Type-C, Policy Engine and
        Protocol Layer.  
    Conditions:
        None
    Input:
        None
    Return:
        None
    Remarks:
        None.

**************************************************************************************************/
void DPM_StateMachineInit(void);

/**************************************************************************************************
    Function:
        void DPM_CalculateAndSortPower(UINT8 u8PDOCount, UINT32 *pu32CapsPayload, UINT8 u8Power[][2]);
    Summary:
        This API refers to calculate and sort the power.
    Devices Supported:
        UPD350 REV A
    Description:
        This API is used to calculate and sort the power in higher to lower order
        from Source/Sink Capabilities.
    Conditions:
        None
    Input:
        u8PDOCount - Number of PDOs.
        pu32CapsPayload - Pointer which holds the array of Source/Sink Capabilities.
        u8Power - Array which holds the calculated power in descending order.
    Return:
        None
    Remarks:
        None.

**************************************************************************************************/
void DPM_CalculateAndSortPower(UINT8 u8PDOCount, UINT32 *pu32CapsPayload, UINT8 u8Power[][2], UINT8 u8SinkMode);

/****************************** DPM Fault handling related APIs*****************************************/
/**************************************************************************************************
    Function:
        void DPM_PowerFaultHandler(UINT8 u8PortNum)
    Summary:
        This API handles the VBUS and VCONN fault occurred 
    Devices Supported:
        UPD350 REV A
    Description:
        This API is called periodically to service the VBUS or VCONN power fault occurred.
    Conditions:
        None
    Input:
        u8PortNum - Port number of the device.Value passed will be less than CONFIG_PD_PORT_COUNT.
    Return:
        None
    Remarks:
        None.

**************************************************************************************************/
void DPM_PowerFaultHandler(UINT8 u8PortNum);

/**************************************************************************************************
    Function:
        void DPM_EnablePowerFaultDetection(UINT8 u8PortNum)
    Summary:
        This API enables VBUS Power fault under voltage and over voltage detection. 
    Devices Supported:
        UPD350 REV A
    Description:
        VBUS power fault detection of under voltage and over voltage is not enabled by default.
        It is enabled after negotiation through this API. This API configures the VBUS comparator
        to detect over voltage and under voltage for the voltage negoitated and established with the 
        port partner.
    Conditions:
        None
    Input:
        u8PortNum - Port number of the device.Value passed will be less than CONFIG_PD_PORT_COUNT.
    Return:
        None
    Remarks:
        None.

**************************************************************************************************/
void DPM_EnablePowerFaultDetection(UINT8 u8PortNum);

/**************************************************************************************************
    Function:
        UINT8 DPM_HandleClientRequest(UINT8 u8PortNum, eMCHP_PSF_DPM_ClientRequest ePDMClientRequestType)
    Summary:
        API handles client request from Application layer. 
    Devices Supported:
        UPD350 REV A
    Description:
        Application layer can call this API for DPM to handle any Client request 
        listed in eMCHP_PSF_DPM_ClientRequest enum than includes Renegotiation,
        Power Fault handling.
    Conditions:
        None
    Input:
        u8PortNum - Port number of the device.Value passed will be less than CONFIG_PD_PORT_COUNT.
        eMCHP_PSF_DPM_ClientRequest - Client Request Type requested by 
    Return:
        TRUE - If the DPM is ready to handle the Client Request
        FALSE - If the DPM is busy, Application layer has to retry later. 
    Remarks:
        None.
**************************************************************************************************/
UINT8 DPM_HandleClientRequest(UINT8 u8PortNum, eMCHP_PSF_DPM_ClientRequest ePDMClientRequestType);

/**************************************************************************************************
    Function:
        void DPM_StoreSinkCapabilities(UINT8 u8PortNum, UINT16 u16Header, UINT32* u32DataBuf);
    Summary:
        Stores the Sink capabilities received from Port Partner. 
    Description:
        This API is used to store the Sink capabilities received from Port Partner 
        that is attached as Sink. This will be called from PE Source State machine 
        once the response is received for GET_SINK_CAP PD message. 
    Conditions:
        None
    Input:
        u8PortNum - Port number of the device.Value passed will be less than CONFIG_PD_PORT_COUNT.
        u16Header - Header Data of the Sink Caps message
        u32DataBuf - Data Buffer containing Sink capabilities 
    Return:
        None.  
    Remarks:
        None.
**************************************************************************************************/

void DPM_StoreSinkCapabilities(UINT8 u8PortNum, UINT16 u16Header, UINT32* u32DataBuf);

/**************************************************************************************************
    Function:
        void DPM_ResetNewPDOParameters(UINT8 u8PortNum);
    Summary:
        Resets the New PDO parameters once New PDOs are advertised. 
    Description:
        This API clears the New PDO Select flag, New PDO Count and all the New
        PDO registers. 
    Conditions:
        None
    Input:
        u8PortNum - Port number of the device.Value passed will be less than CONFIG_PD_PORT_COUNT. 
    Return:
        None.  
    Remarks:
        None.
**************************************************************************************************/

void DPM_ResetNewPDOParameters(UINT8 u8PortNum); 

/**************************************************************************************************
    Function:
        void DPM_UpdateAdvertisedPDOParam(UINT8 u8PortNum); 
    Summary:
        Updates the Advertised PDO registers and status bits once PDOs are advertised. 
    Description:
        This API sets the advertised PDO count, updates the registers and sets/clears  
        the PD_BAL_REDUCED_SOURCE_CAPABILITIES bit in Port Connection Status 
        after comparing the advertised PDOs with Fixed PDOs.       . 
    Conditions:
        None
    Input:
        u8PortNum - Port number of the device.Value passed will be less than CONFIG_PD_PORT_COUNT. 
    Return:
        None.  
    Remarks:
        None.
**************************************************************************************************/

void DPM_UpdateAdvertisedPDOParam(UINT8 u8PortNum); 

/**************************************************************************************************
    Function:
        UINT8 DPM_ComparePDOs(UINT8 u8PortNum); 
    Summary:
        Compares advertised PDOs and Fixed PDOs. 
    Description:
        This API compares the default Source capabilities(Fixed PDOs) with the 
        advertised Source capabilities(Advertised PDOs).       . 
    Conditions:
        None
    Input:
        u8PortNum - Port number of the device.Value passed will be less than CONFIG_PD_PORT_COUNT. 
    Return:
        TRUE - If Fixed and advertised PDOs are same.
        FALSE - If Fixed and advertised PDOs are not same.   
    Remarks:
        None.
**************************************************************************************************/

UINT8 DPM_ComparePDOs(UINT8 u8PortNum);

/**************************************************************************************************
    Function:
        UINT8 DPM_NotifyClient(UINT8 u8PortNum, eMCHP_PSF_NOTIFICATION eDPMNotification)
    Summary:
        Notifies client of the PD Events from stack. 
    Description:
        Application layer can call this API for DPM to send notifications of 
        PD events detected by PSF stack which includes Type C Attach/Detach,
        PD Contract negotiated, etc., 
    Conditions:
        None
    Input:
        u8PortNum - Port number of the device.Value passed will be less than CONFIG_PD_PORT_COUNT.
        eDPMNotification - Notification of type eMCHP_PSF_NOTIFICATION
    Return:
        TRUE - PSF has to handle the event in addition to sending the 
               notification. 
        FALSE - PSF does not want to handle the event after sending the 
               notification.
    Remarks:
        None.
**************************************************************************************************/
UINT8 DPM_NotifyClient(UINT8 u8PortNum, eMCHP_PSF_NOTIFICATION eDPMNotification);

#endif /*_POLICY_MANAGER_H_*/

