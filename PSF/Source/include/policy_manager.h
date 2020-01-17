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
// Section: Data types and constants
// *****************************************************************************
// *****************************************************************************

/******************************************************************************
                Return Values from PE_ValidateMessage API
******************************************************************************/
#define DPM_VALID_REQUEST            1
#define DPM_INVALID_REQUEST          0   
   
#define DPM_OBJECT_POSITION_1                    1
#define PDO_MAX_OBJECTS 				         7

/*Macros to get PDO type, PDO Current, PDO voltage,USB Comm capable bit from
Source/Sink Power delivery objects*/
#define DPM_GET_PDO_TYPE(X)   					((X & 0xC0000000) >> 30)
#define DPM_GET_PDO_CURRENT(X)                  ((X & 0x000003FF))
#define DPM_GET_PDO_VOLTAGE(X)                  ((X & 0x000FFC00) >> 10)	/*in 50mv units*/
#define DPM_GET_PDO_USB_COMM_CAP(X)             ((X & 0x04000000) >> 26)

#define DPM_PD0_VOLTAGE_5                        100

#define DPM_FORM_DATA_REQUEST(OBJECT_POSITION,CAPABLITY_MISMATCH,USB_COMMUNICATION_CAPABLE, \
        OPERATING_CURRENT,MAXIMUM_OPERATING_CURRENT) ((OBJECT_POSITION << 28) | \
        (CAPABLITY_MISMATCH << 26) | (USB_COMMUNICATION_CAPABLE << 25) | (OPERATING_CURRENT << 10) \
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
    
/*Defines for getting data from u8DPM_Status variable*/
#define DPM_GET_CURRENT_POWER_ROLE(u8PortNum)         ((gasDPM[u8PortNum].u8DPM_Status & DPM_CURR_POWER_ROLE_MASK) >> DPM_CURR_POWER_ROLE_POS)
#define DPM_GET_CURRENT_DATA_ROLE(u8PortNum)          ((gasDPM[u8PortNum].u8DPM_Status & DPM_CURR_DATA_ROLE_MASK) >> DPM_CURR_DATA_ROLE_POS)
#define DPM_GET_CURRENT_PD_SPEC_REV(u8PortNum)        ((gasDPM[u8PortNum].u8DPM_Status & DPM_CURR_PD_SPEC_REV_MASK) >> DPM_CURR_PD_SPEC_REV_POS)

/* Define to get negotiated current */
#define DPM_GET_SINK_CURRRENT(u8PortNum)         (gasDPM[u8PortNum].u16MaxCurrSupportedin10mA * DPM_10mA)

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


#define DPM_GET_DPM_STATUS(u8PortNum)				gasDPM[u8PortNum].u8DPM_Status

/*Defines for getting data by passing u8DPM_Status variable*/
#define DPM_GET_CURRENT_POWER_ROLE_FRM_STATUS(u8DPM_Status)   ((u8DPM_Status & DPM_CURR_POWER_ROLE_MASK) >> DPM_CURR_POWER_ROLE_POS)
#define DPM_GET_CURRENT_DATA_ROLE_FRM_STATUS(u8DPM_Status)    ((u8DPM_Status & DPM_CURR_DATA_ROLE_MASK) >> DPM_CURR_DATA_ROLE_POS)
#define DPM_GET_CURRENT_PD_SPEC_REV_FRM_STATUS(u8DPM_Status)  ((u8DPM_Status & DPM_CURR_PD_SPEC_REV_MASK) >> DPM_CURR_PD_SPEC_REV_POS)

/*Defines for getting voltage from PDO*/
#define DPM_PDO_VOLTAGE_MASK                 0x3FF
#define DPM_PDO_VOLTAGE_POS                  10
#define DPM_PDO_VOLTAGE_UNIT                 50
#define DPM_GET_VOLTAGE_FROM_PDO_MILLI_V(u32PDO)    (((u32PDO >> DPM_PDO_VOLTAGE_POS) & DPM_PDO_VOLTAGE_MASK) * DPM_PDO_VOLTAGE_UNIT)

/*Defines for getting current from PDO[9:0]*/
#define DPM_PDO_CURRENT_MASK                 0x1FF
#define DPM_PDO_CURRENT_UNIT                 10
#define DPM_GET_CURRENT_FROM_PDO_MILLI_A(u32PDO)    ((u32PDO & DPM_PDO_CURRENT_MASK) * DPM_PDO_CURRENT_UNIT)

/*u8PowerFaultISR defines */
#define DPM_POWER_FAULT_OVP				BIT(0)
#define DPM_POWER_FAULT_UV				BIT(1)
#define DPM_POWER_FAULT_VBUS_OCS		BIT(2)
#define DPM_POWER_FAULT_VCONN_OCS	    BIT(3)

/*----------Macros for u8VConnEnable argument of DPM_VConnOnOff API ------------*/
#define DPM_VCONN_ON             1
#define DPM_VCONN_OFF            0

/*----------Macros for u8VbusOnorOff argument of DPM_TypeCVBus5VOnOff API ------------*/
#define DPM_VBUS_ON              1
#define DPM_VBUS_OFF             0

/* Macros for CONFIG_HOOK_NOTIFY_POWER_FAULT return value */


/* Macros for PDOIndex Port Power Handling for Typec Vsafe0V and Vsafe5V*/
#define DPM_VSAFE0V_PDO_INDEX   0
#define DPM_VSAFE5V_PDO_INDEX_1 1

/*define to convert u16MaxCurrSupportedin10mA expressed interms of 10mA to mA*/
#define DPM_10mA    10

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
  UINT32  u32NegotiatedPDO;     //NegotiatedPDO
  UINT32 u32SinkReqRDO;         //Sink Requested RDO
 
  #if INCLUDE_POWER_FAULT_HANDLING
	  UINT8 u8VBUSPowerGoodTmrID;     //VBUS PowerGood Timer ID
      UINT8 u8VCONNPowerGoodTmrID;    //VConn PowerGood Timer ID
	  UINT8 u8VBUSPowerFaultCount;      //VBUS Power fault count
      UINT8 u8VCONNPowerFaultCount;     //VConn Power fault count     
      UINT8 u8VCONNGoodtoSupply;        //Vconn good to supply
	  UINT8 u8PowerFaultISR;          //Power fault ISR flag
	  UINT8 u8HRCompleteWait;         //Hard Reset complete wait flag
  #endif
  
}MCHP_PSF_STRUCT_PACKED_END DEVICE_POLICY_MANAGER;

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
        u8PortNum - Port Number for which source cabilities to be returned.
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
        void DPM_TypeCVBus5VOnOff(UINT8 u8PortNum, UINT8 u8VbusOnorOff);
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
void DPM_TypeCVBus5VOnOff(UINT8 u8PortNum, UINT8 u8VbusOnorOff);

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
void DPM_CalculateAndSortPower(UINT8 u8PDOCount, UINT32 *pu32CapsPayload, UINT8 u8Power[][2]);

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

#endif /*_POLICY_MANAGER_H_*/

