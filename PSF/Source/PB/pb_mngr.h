/*******************************************************************************
  Power Balancing Header File 

  Company:
    Microchip Technology Inc.

  File Name:
    pb_mngr.h

  Description:
    This header file contains the data structures, constant definitions and 
	function prototypes for Power Balancing. 
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

#ifndef _PB_MNGR_H    /* Guard against multiple inclusion */
#define _PB_MNGR_H

/* ************************************************************************** */
/* ************************************************************************** */
/* Section: Constants                                                         */
/* ************************************************************************** */
/* ************************************************************************** */ 

/* Asynchronous Request Wait time value - 200ms */
#define PB_ASYN_REQ_WAIT_TIMER_IN_MS            0xC8  

/* Macros to indicate Reclaim status of ports */
#define PB_RECLAIM_FAILED                       0x00
#define PB_RECLAIM_SUCCESS                      0x01

/* Bits 3-0: Selection of Power Balancing Algorithm
        0000 = First Come First Serve
        0001 = Last Plugged Gets Priority
   Bit 4: PD Power balancing Enable/Disable */
#define FIRST_COME_FIRST_SERVE_ALGO             0x10
#define LAST_COME_FIRST_SERVE_ALGO              0x11

/* Macros to denote PDO Masks and Bit Positions*/
#define PB_PDO_TYPE_MSK                  0xC0000000
#define PB_FIXED_PDO_VOLTAGE_MASK        0x000FFC00 
#define PB_FIXED_PDO_CURRENT_MASK        0x000003FF 
#define PB_VARIABLE_PDO_MAX_VOLT_MASK    0x3FF00000
#define PB_VARIABLE_PDO_MIN_VOLT_MASK    0x000FFC00 
#define PB_PDO_VOLTGE_BIT_POS                   10
#define PB_PDO_TYPE_BIT_POS                     30
#define PB_VARIABLE_PDO_MAX_VOLT_BIT_POS        20
#define PB_VARIABLE_PDO_MIN_VOLT_BIT_POS        10

/* Macros to define RDO bit positions */
#define PB_RDO_OP_CURRENT_BIT_POS               10
#define PB_RDO_OBJECT_BIT_POS                   28
#define PB_RDO_GIVEBACK_BIT_POS                 27
#define PB_RDO_CABABILITY_MISMATCH_BIT_POS      26
#define PB_RDO_USB_COM_CAPABLE_BIT_POS          25
#define PB_RDO_NO_USB_SUSPEND_BIT_POS           24

/* Macros to define RDO bit masks */
#define PB_RDO_OP_CURRENT_MSK                   (UINT32)(0x000FFC00)
#define PB_RDO_MAX_MIN_CURRENT_MSK              (UINT32)(0x000003FF)
#define PB_RDO_OBJECT_POS_MSK                   (UINT32)(0x70000000)
#define PB_RDO_GIVEBACK_MSK                     (UINT32)(1 << PB_RDO_GIVEBACK_BIT_POS)
#define PB_RDO_CAPABILITIY_MISMATCH_MSK         (UINT32)(1 << PB_RDO_CABABILITY_MISMATCH_BIT_POS)
#define PB_RDO_USB_COM_CAPABLE_MSK              (UINT32)(1 << PB_RDO_USB_COM_CAPABLE_BIT_POS)
#define PB_RDO_NO_USB_SUSPEND_MSK               (UINT32)(1 << PB_RDO_NO_USB_SUSPEND_BIT_POS)

/* Port Status Mask */
#define PB_PORT_STATUS_ATTACH               0x01U
#define PB_PORT_STATUS_INITIAL_NEG_DONE     0x02U
#define PB_PORT_STATUS_PORT_IN_MIN_PWR      0x04U
#define PB_PORT_STATUS_CAPABILITY_MISMATCH  0x08U
#define PB_PORT_STATUS_RENEG_AGAIN          0x10U

/* Macros to define Excess Power */
#define PB_ZERO_EXCESS_POWER                    0x0000

/* Macro to denote invalid Port */
#define PB_INVALID_PORT                         0xFF 

/* Macro to get the voltage value from Fixed PDO */
#define PB_GET_VOLTAGE_FROM_FIXED_PDO(u32PDO)   (UINT16)(((UINT32)u32PDO & \
                                    PB_FIXED_PDO_VOLTAGE_MASK) >> PB_PDO_VOLTGE_BIT_POS)

/* Macro to get the current value from Fixed PDO */
#define PB_GET_CURRENT_FROM_PDO(u32PDO)   (UINT16)((UINT32)u32PDO & PB_FIXED_PDO_CURRENT_MASK)

/* Macro to get the PDO Type from Fixed PDO */
#define PB_GET_PDO_TYPE(u32PDO)             (((UINT32)u32PDO & PB_PDO_TYPE_MSK) \
                                                        >> PB_PDO_TYPE_BIT_POS) 

/* Macro to get the maximum voltage from Variable PDO */
#define PB_GET_VARIABLE_PDO_MAX_VOLTAGE(u32PDO)    (UINT16)(((UINT32)u32PDO & PB_VARIABLE_PDO_MAX_VOLT_MASK) \
                                            >> PB_VARIABLE_PDO_MAX_VOLT_BIT_POS)  

/* Macro to get the minimum voltage from Variable PDO */
#define PB_GET_VARIABLE_PDO_MIN_VOLTAGE(u32PDO)    (UINT16)(((UINT32)u32PDO & PB_VARIABLE_PDO_MIN_VOLT_MASK) \
                                            >> PB_VARIABLE_PDO_MIN_VOLT_BIT_POS)

/* Macro to convert voltage from 50mV units to V units */
#define PB_CONVERT_PDO_VOLTAGE_FROM_50mV_TO_V(voltIn50mV)   ((float)voltIn50mV/20) 

/* Macro to convert power from 10mW units to 250mW units */
#define PB_CONVERT_POWER_FROM_10mW_TO_250mW(powIn10mW)      (powIn10mW/25) 

/* Macro to get Operating current from RDO */
#define PB_GET_OPERATING_CURRENT_FROM_RDO(u32RDO) (((UINT32)u32RDO & PB_RDO_OP_CURRENT_MSK) >> PB_RDO_OP_CURRENT_BIT_POS)

/* Macro to get Maximum Current from RDO */
#define PB_GET_MAX_CURRENT_FROM_RDO(u32RDO)  ((UINT32)u32RDO & PB_RDO_MAX_MIN_CURRENT_MSK) 

/* Macro to get Object Position from RDO */
#define PB_GET_OBJ_POS_FROM_RDO(u32RDO)      (((UINT32)u32RDO & PB_RDO_OBJECT_POS_MSK) >> PB_RDO_OBJECT_BIT_POS)

/* Macro to check if Sink supports GiveBack */
#define PB_IS_GIVEBACK_SUPPORTED(u32RDO)  (((UINT32)u32RDO & PB_RDO_GIVEBACK_MSK)!= 0) 

/* Macro to check if Capability Mismatch bit is set in RDO */
#define PB_IS_CAPABILITY_MISMATCH(u32RDO) (((UINT32)u32RDO & PB_RDO_CAPABILITIY_MISMATCH_MSK) != 0)  

/* Macro to get priority of the port from Config variable */
#define PB_GET_PORT_PRIORITY(u8PortNum) ((gasCfgStatusData.sPBPerPortData[u8PortNum].u8PBEnablePriority & 0x06) >> 1) 

/* Enumeration to define Power Balancing enabled PD Port States */
typedef enum PBPortStates
{
    ePB_IDLE_STATE = 1,
    ePB_RENEGOTIATION_IN_PROGRESS_STATE,
    ePB_RENEGOTIATION_PENDING_STATE,
    ePB_RENEGOTIATION_COMPLETED_STATE,
    ePB_PWR_RECOVERING_STATE,
    ePB_INVALID_STATE = 0xFF
}PB_PORT_STATES;

/* Enumeration to define Power Balancing enabled PD Port Sub-States */
typedef enum PortSubState
{
    ePB_IDLE_SS = 1,
    ePB_GET_SINKCAPS_SENT_SS,
    ePB_FIRST_RENEGOTIATION_IN_PROGRESS_SS,
    ePB_SECOND_RENEGOTIATION_IN_PROGRESS_SS,
    ePB_WAIT_FOR_ASYNC_REQ_SS,
    ePB_INVALID_SS = 0xFF
}PB_RENEG_SUBSTATE;

/* Enumeration to define the state of Get_Sink_caps */
typedef enum GetSinkCapsState
{
    ePB_SINK_CAPS_NOT_INITIATED =  1,
    ePB_SINK_CAPS_INITIATED,
    ePB_SINK_CAPS_COMPLETED
}GET_SINK_CAP_SS;

/***************************************************************************************/
// *****************************************************************************
// *****************************************************************************
// Section: Data Structures
// *****************************************************************************
// *****************************************************************************

/*****************************************************************************
  Summary:
    Power Balancing Internal System Parameters   

  Description:
    This structure contains the system parameters that are internal to PB layer. 

  Remarks:
    Need to be packed always based on type of microcontroller.
**********************************************************************************/
typedef struct MCHP_PSF_STRUCT_PACKED_START _PBIntSysParam 
{
    UINT32 u32AsyncReqWaitTimerInms; /* Asynchronous Request Wait timer value*/
    UINT16 u16TotalSysPwrIn250mW;  /* Max Shared capacity of the system */
    UINT16 u16Reserved1;           /* Reserved for future use */ 
    UINT8 u8ReclaimPortNum;        /* Port from which power is reclaimed */
    UINT8 u8RecoverPortNum;        /* Port which is in recovering mode */  
    UINT8 u8RecoveringMode;        /* Recover status of a port */
    UINT8 u8Reserved2;             /* Reserved for future use */ 
} MCHP_PSF_STRUCT_PACKED_END  PB_INT_SYS_PARAM;

/*****************************************************************************
  Summary:
    Power Balancing Internal Port Parameters   

  Description:
    This structure contains the port parameters that are internal to PB layer. 

  Remarks:
    Need to be packed always based on type of microcontroller.
**********************************************************************************/
typedef struct MCHP_PSF_STRUCT_PACKED_START _PBIntPortParam 
{
    UINT16 u16RequiredPrtPwrIn250mW; /* Power required by a port in units of 250mW */
    UINT16 u16NegotiatedPwrIn250mW; /* Negotiated power in units of 250mW */
    UINT16 u16MinGuaranteedPwrIn250mW; /* Minimum guaranteed power for a port */
    UINT16 u16MaxPortPwrIn250mW;      /* Maximum Port Power in units of 250mW */ 
    PB_PORT_STATES ePBPortState;      /* States of PB State Machine */
    PB_RENEG_SUBSTATE eRenegSubState; /* Sub-states of PB State Machine */
    GET_SINK_CAP_SS eGetSinkCapSS;    /* Get Sink caps sub-states */
    UINT8 u8AttachSeqNo;              /* Sequence in which ports are attached */
    UINT8 u8PBPortStatusMask;     /* Bit - Position 
        0     Attach 
        1     InitialNegotiationDone 
        2     IsPortInMinimalPower 
        3     CapabilityMismatch 
        4     RenegAgain */         
} MCHP_PSF_STRUCT_PACKED_END  PB_INT_PORT_PARAM; 

// *****************************************************************************
// *****************************************************************************
// Section: Interface Functions
// *****************************************************************************
// *****************************************************************************

/**************************************************************************************************
    Function:
        UINT8 PB_HandleDPMEvents (UINT8 u8PortNum, UINT8 eDPM_EVENT);

    Summary:
        This API handles all the events detected by Device Policy Manager. 

    Description:
        This API is the heart of PB manager. Whenever there is an event 
        detected by PPM layer, this function would be triggered to process 
        and act upon the event.

    Conditions:
        None.

    Input:
        u8PortNum - Port Number.
        ePPM_EVENT - Event detected by PPM 

    Return:
        TRUE.

    Remarks:
        None.

**************************************************************************************************/
UINT8 PB_HandleDPMEvents (UINT8 u8PortNum, UINT8 eDPM_EVENT);
/**************************************************************************************************
    Function:
        void PB_Init (void);

    Summary:
        This API initializes all the PB system and port parameters.  

    Description:
        This API initializes the System level and port level parameters of Power
        Balancing based on the Power Throttling bank that is currently selected.  

    Conditions:
        None.

    Input:
        None.

    Return:
        None.

    Remarks:
        This API shall be called once the configuration parameters are 
        updated during boot time.

**************************************************************************************************/
void PB_Init(void); 
/**************************************************************************************************
    Function:
        void PB_InitializePortParam (UINT8 u8PortNum);

    Summary:
        This API initializes all the PB port specific parameters.  

    Description:
        This API initializes the port level parameters of Power Balancing based 
        on the Power Throttling bank that is currently selected. 

    Conditions:
        None.

    Input:
        u8PortNum - Port Number.

    Return:
        None.

    Remarks:
        This API shall be called only when Power Balancing is enabled for the 
        particular port. 

**************************************************************************************************/
void PB_InitializePortParam(UINT8 u8PortNum); 
/**************************************************************************************************
    Function:
        void PB_ChangePortStates (UINT8 u8PortNum, PB_PORT_STATES ePortState, PB_RENEG_SUBSTATE ePortSubState);

    Summary:
        This API changes the state machine states of a port.  

    Description:
        This API updates the ePBPortState and eRenegSubState parameters of 
        gasPBIntPortParam[u8PortNum] with the new state values. PB State machine
        is driven based on these values. 

    Conditions:
        None.

    Input:
        u8PortNum - Port Number.
        ePortState - Port state.
        ePortSubState - Port sub state. 

    Return:
        None.

    Remarks:
        None. 

**************************************************************************************************/
void PB_ChangePortStates(UINT8 u8PortNum, PB_PORT_STATES ePortState, PB_RENEG_SUBSTATE ePortSubState);
/**************************************************************************************************
    Function:
        void PB_CalculateNegotiatedPower (UINT8 u8PortNum, UINT32 u32PDO, UINT32 u32RDO);

    Summary:
        This API calculates the currently negotiated power of a port.  

    Description:
        This API computes the power that is established by the current explicit
        contract by taking the Source PDO voltage and RDO current. The computed
        power will be represented in terms of 250mW units. 

    Conditions:
        None.

    Input:
        u8PortNum - Port Number.
        u32PDO - Source PDO value.
        u32RDO - RDO requested by Sink.

    Return:
        None.

    Remarks:
        None. 

**************************************************************************************************/
void PB_CalculateNegotiatedPower(UINT8 u8PortNum, UINT32 u32PDO, UINT32 u32RDO);
/**************************************************************************************************
    Function:
        void PB_InitiateNegotiationWrapper (UINT8 u8PortNum, UINT16 u16NewWattage);

    Summary:
        This is a wrapper API for PPM API that initiates negotiation.  

    Description:
        This API calls the PPM API that forms the PDOs and starts the negotiation
        based on the power wattage value given as input. 

    Conditions:
        None.

    Input:
        u8PortNum - Port Number.
        u16NewWattage - Power value to be advertised. 

    Return:
        None.

    Remarks:
        None. 

**************************************************************************************************/
void PB_InitiateNegotiationWrapper(UINT8 u8PortNum, UINT16 u16NewWattage);
/**************************************************************************************************
    Function:
        void PB_InitiateGetSinkCapsWrapper (UINT8 u8PortNum);

    Summary:
        This is a wrapper API for PPM API that initiates Get_Sink_Caps.  

    Description:
        This API calls the PPM API that initiates Get_Sink_Caps PD message to 
        get the Sink capabilities of the port partner. 

    Conditions:
        None.

    Input:
        u8PortNum - Port Number. 

    Return:
        None.

    Remarks:
        None. 

**************************************************************************************************/
void PB_InitiateGetSinkCapsWrapper(UINT8 u8PortNum);
/**************************************************************************************************
    Function:
        void PB_CalculateRequiredPortPower (UINT8 u8PortNum, const PDO_INFO *psPDOInfo);

    Summary:
        This API calculates the power required for a port.  

    Description:
        This API calculates the power required for a port based on the 
        sink capabilities received from the port partner. 

    Conditions:
        None.

    Input:
        u8PortNum - Port Number. 
        psPDOInfo - Sink PDOs. 

    Return:
        None.

    Remarks:
        None. 

**************************************************************************************************/
void PB_CalculateRequiredPortPower(UINT8 u8PortNum, UINT8 u8SinkPDOCnt, const UINT32 *pu32SinkCap);
/**************************************************************************************************
    Function:
        UINT8 PB_NegotiateIfRequiredPwrAvailableInPool (UINT8 u8PortNum);

    Summary:
        This API initiates PD negotiation is required port power is available. 

    Description:
        This API checks if the required port power is available in the pool. If 
        yes, initiates the PD negotiation. 

    Conditions:
        None.

    Input:
        u8PortNum - Port Number.

    Return:
        None.

    Remarks:
        None. 

**************************************************************************************************/
UINT8 PB_NegotiateIfRequiredPwrAvailableInPool(UINT8 u8PortNum);
/**************************************************************************************************
    Function:
        UINT8 PB_ReturnHigherPriorityPortWithoutCapMismatch (UINT8 u8PortNum1, UINT8 u8PortNum2);

    Summary:
        This API returns the higher priority port without capability mismatch          

    Description:
        This API returns the higher priority port among the two ports 
        that does not have capability mismatch bit set. 

    Conditions:
        None.

    Input:
        u8PortNum1 - Port Number 1.
        u8PortNum2 - Port Number 2. 

    Return:
        None.

    Remarks:
        None. 

**************************************************************************************************/
UINT8 PB_ReturnHigherPriorityPortWithoutCapMismatch (UINT8 u8PortNum1, UINT8 u8PortNum2);
/**************************************************************************************************
    Function:
        UINT8 PB_ReturnHigherPriorityPort (UINT8 u8PortNum1, UINT8 u8PortNum2);

    Summary:
        This API returns the higher priority port with capability mismatch.          

    Description:
        This API returns the higher priority port among the two ports 
        by taking capability mismatch into account.  

    Conditions:
        None.

    Input:
        u8PortNum1 - Port Number 1.
        u8PortNum2 - Port Number 2.

    Return:
        None.

    Remarks:
        None. 

**************************************************************************************************/
UINT8 PB_ReturnHigherPriorityPort (UINT8 u8PortNum1, UINT8 u8PortNum2);
/**************************************************************************************************
    Function:
        UINT8 PB_IdentifyLowestPriorityPort (UINT8 u8PortNum);

    Summary:
        This API returns the port that has the least priority in the entire system.          

    Description:
        This API returns the lowest priority port from which power can be 
        reclaimed.   

    Conditions:
        None.

    Input:
        u8PortNum - Port Number.

    Return:
        None.

    Remarks:
        None. 

**************************************************************************************************/
UINT8 PB_IdentifyLowestPriorityPort(UINT8 u8PortNum);
/**************************************************************************************************
    Function:
        UINT8 PB_ReclaimPower (UINT8 u8PortNum);

    Summary:
        This API reclaims power from the identified lower priority ports.          

    Description:
        This API reclaims power from the lower priority ports when the 
        port with highest priority is in short of required power.   

    Conditions:
        None.

    Input:
        u8PortNum - Port Number.

    Return:
        None.

    Remarks:
        None. 

**************************************************************************************************/
UINT8 PB_ReclaimPower(UINT8 u8PortNum);
/**************************************************************************************************
    Function:
        void PB_SinkCapsReceivedHandler (UINT8 u8PortNum);

    Summary:
        This API calculates the required power and starts negotiation once 
        Sink caps received notification is given by PPM.          

    Description:
        This API
        i. Gets the received sink capabilities from PPM. 
        ii. Calculates the port power required by the Sink based on the sink caps. 
        iii. Checks if required power is available in the pool. 
        iv. Reclaims power from lower priority ports if sufficient power is not available. 

    Conditions:
        None.

    Input:
        u8PortNum - Port Number.

    Return:
        None.

    Remarks:
        None. 

**************************************************************************************************/
void PB_SinkCapsReceivedHandler(UINT8 u8PortNum);
/**************************************************************************************************
    Function:
        void PB_GiveBackExcessPwr (UINT8 u8PortNum);

    Summary:
        This API releases the excess port power back to the pool.          

    Description:
        This API calculates the excess power other than the negotiated power 
        and releases it back to the pool. 
        
    Conditions:
        None.

    Input:
        u8PortNum - Port Number.

    Return:
        None.

    Remarks:
        None. 

**************************************************************************************************/
UINT8 PB_ReleaseExcessPwr(UINT8 u8PortNum);
/**************************************************************************************************
    Function:
        void PB_SetRenegotiationPendingForLowPriorityPorts (UINT8 u8PortNum);

    Summary:
        This API sets the port state as Renegotiation pending for all the low 
        priority ports.

    Description:
        This API sets the port state as Renegotiation pending for all the lower 
        priority ports so that they can be renegotiated once a detach event is 
        occurred. 
        
    Conditions:
        None.

    Input:
        u8PortNum - Port Number.

    Return:
        None.

    Remarks:
        None. 

**************************************************************************************************/
void PB_SetRenegotiationPendingForLowPriorityPorts(UINT8 u8PortNum);
/**************************************************************************************************
    Function:
        void PB_UpdateAttachSeq (UINT8 u8PortNum);

    Summary:
        This API updates the sequence in which ports were attached. 

    Description:
        This API is used to update the order in which ports were attached.  
        
    Conditions:
        None.

    Input:
        u8PortNum - Port Number.

    Return:
        None.

    Remarks:
        None. 

**************************************************************************************************/
void PB_UpdateAttachSeq(UINT8 u8PortNum);
/**************************************************************************************************
    Function:
        UINT8 PB_IsNegotiationInProgressForOtherPort (UINT8 u8PortNum);

    Summary:
        This API checks if PD negotiation is in progress for other ports.  

    Description:
        This API returns true if PD negotiation is pending for any of the ports
        other than the current port, else returns false. Based on this, current 
        port will be continued with renegotiation. 
        
    Conditions:
        None.

    Input:
        u8PortNum - Port Number.

    Return:
        True - if negotiation is pending for any other port.
        False - if negotiation is pending for none of the ports. 

    Remarks:
        None. 

**************************************************************************************************/
UINT8 PB_IsNegotiationInProgressForOtherPort (UINT8 u8PortNum);
/**************************************************************************************************
    Function:
        UINT8 PB_IdentifyHighestPriorityPortInPendingState (void);

    Summary:
        This API returns the highest priority port that is in pending state.  

    Description:
        This API returns the port with highest priority for which renegotiation
        is pending. This will be used in cases where the ports need to be scheduled
        for renegotiation when a detach or power fault has occurred in some port. 
        
    Conditions:
        None.

    Input:
        u8PortNum - Port Number.

    Return:
        Highest priority Port number 

    Remarks:
        None. 

**************************************************************************************************/
UINT8 PB_IdentifyHighestPriorityPortInPendingState(void);
/**************************************************************************************************
    Function:
        void PB_InitiateNextPortNegotiation (void);

    Summary:
        This API starts the negotiation for the ports other than the port for 
        which a detach or power fault has occurred.   

    Description:
        This API identifies the next highest priority port, get the sink caps 
        from its port partner and starts the negotiation for it when a  
        detach or power fault has occurred. 
        
    Conditions:
        None.

    Input:
        None.

    Return:
        Highest priority Port number 

    Remarks:
        None. 

**************************************************************************************************/
void PB_InitiateNextPortNegotiation(void);
/**************************************************************************************************
    Function:
        UINT8 PB_PortInWaitForAsyncTimerState (void);

    Summary:
        This API returns the port that is in eWAIT_FOR_ASYNC_REQ state.   

    Description:
        This API returns the port that is in eWAIT_FOR_ASYNC_REQ state currently. 
        
    Conditions:
        None.

    Input:
        None.

    Return:
        Port in eWAIT_FOR_ASYNC_REQ state.

    Remarks:
        None. 

**************************************************************************************************/
UINT8 PB_PortInWaitForAsyncTimerState(void);
/**************************************************************************************************
    Function:
        void PB_AsyncTimerCB (UINT8 u8PortNum, UINT8 u8Dummy);

    Summary:
        This is the API registered as callback for Timer expiry.   

    Description:
        This API will be called when the Timer expires and it will in turn call 
        the API PB_HandleDPMEvents() to handle the timer expired event. 
        
    Conditions:
        None.

    Input:
        u8PortNum - Port Number.
        u8Dummy   - Dummy argument to match the function prototype.

    Return:
        None.

    Remarks:
        None. 

**************************************************************************************************/
void PB_AsyncTimerCB(UINT8, UINT8);

/**************************************************************************************************
    Function:
        void PB_HandleReclaimPortDetachOrRenegCmplt(void);

    Summary:
        This API is used to handle the detach event or Renegotiation complete
        event of Reclaiming Port. 

    Description:
        This API can be called to handle the detach event or Renegotiation complete
        event that occurred for a Reclaiming port. This API checks if sufficient power
        is available for the recovering port. If yes, it clears the recovering mode 
        status. Else, it initiates reclaiming power from the next low priority port. 
 
    Conditions:
        None.

    Input:
        None.

    Return:
        None. 

    Remarks:
        None. 

**************************************************************************************************/
void PB_HandleReclaimPortDetachOrRenegCmplt(void); 

/**************************************************************************************************
    Function:
        void PB_HandleHighPriorityPortDetach(UINT8 u8PortNum); 

    Summary:
        This API is used to handle the detach event of higher priority port.  

    Description:
        This API can be called to handle the detach event of higher priority
        port. It clears the Reneg again status bit and initiates Renegotiation 
        for the next high priority port in the system. 
 
    Conditions:
        None.

    Input:
        u8PortNum - Port Number 

    Return:
        None. 

    Remarks:
        None. 

**************************************************************************************************/
void PB_HandleHighPriorityPortDetach(UINT8 u8PortNum); 

/**************************************************************************************************
    Function:
        void PB_OnPTBankSwitch(UINT8 u8PortNum); 

    Summary:
        This API is used to handle the Throttling Bank Switch Event.

    Description:
        This API can be called to update the global and per port PB parameters 
        and initiate renegotiation for minimum guaranteed power based 
        on the new PT bank.  
    Conditions:
        None.
    Input:
        u8PortNum - Port Number 
    Return:
        None. 

    Remarks:
        None. 

**************************************************************************************************/
void PB_OnPTBankSwitch(UINT8 u8PortNum); 

#endif /* _PB_MNGR_H */

/* *****************************************************************************
 End of File
 */
