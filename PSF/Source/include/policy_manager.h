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

#ifndef _POLICY_MANAGER_H_
#define _POLICY_MANAGER_H_

// *****************************************************************************
// *****************************************************************************
// Section: Define to get DPM Status, default configuration and configured roles
// *****************************************************************************
// *****************************************************************************
/***************************Define to get DPM configuration data*****************************************/
/*Bit definition for u8DPMConfigData variable*/
#define DPM_DEFAULT_POWER_ROLE_MASK          (BIT(0)|BIT(1))
#define DPM_DEFAULT_DATA_ROLE_MASK           (BIT(2)|BIT(3))
#define DPM_DEFAULT_PD_SPEC_REV_MASK         (BIT(4)|BIT(5))
#define DPM_NEW_PDO_ENABLE_MASK               BIT(6)

/*Bit position for u8DPMConfigData variable*/
#define DPM_DEFAULT_POWER_ROLE_POS           0
#define DPM_DEFAULT_DATA_ROLE_POS            2
#define DPM_DEFAULT_PD_SPEC_REV_POS          4
#define DPM_NEW_PDO_ENABLE_POS               6 

/*Defines for getting default values configured to a port from gasDPM[u8PortNum].u8DPMConfigData*/
/*DPM_GET_DEFAULT_POWER_ROLE(u8PortNum) will return one of the following values
	- PD_ROLE_SINK
	- PD_ROLE_SOURCE
	- PD_ROLE_DRP*/
#define DPM_GET_DEFAULT_POWER_ROLE(u8PortNum)\
((gasDPM[u8PortNum].u8DPMConfigData & DPM_DEFAULT_POWER_ROLE_MASK) >> DPM_DEFAULT_POWER_ROLE_POS)

/*DPM_GET_CURRENT_DATA_ROLE(u8PortNum) will return one of the following values
	- PD_ROLE_UFP
	- PD_ROLE_DFP
	- PD_ROLE_TOGGLING */
#define DPM_GET_DEFAULT_DATA_ROLE(u8PortNum)          \
    ((gasDPM[u8PortNum].u8DPMConfigData & DPM_DEFAULT_DATA_ROLE_MASK) >> DPM_DEFAULT_DATA_ROLE_POS)
#define DPM_GET_DEFAULT_PD_SPEC_REV(u8PortNum)\
    ((gasDPM[u8PortNum].u8DPMConfigData & DPM_DEFAULT_PD_SPEC_REV_MASK) >> DPM_DEFAULT_PD_SPEC_REV_POS)
#define DPM_GET_NEW_PDO_STATUS(u8PortNum)\
    ((gasDPM[u8PortNum].u8DPMConfigData & DPM_NEW_PDO_ENABLE_MASK) >> DPM_NEW_PDO_ENABLE_POS)
/*************************************************************************************************/

/*************************************************************************************************/
/***************************Define to get configured data by user application*****************************************/
/*Bit definition for gasCfgStatusData.sPerPortData[u8PortNum].u32CfgData */
#define DPM_CFG_POWER_ROLE_MASK             (BIT(1) | BIT(0))
#define DPM_CFG_DATA_ROLE_MASK              (BIT(2))
#define DPM_CFG_RPVAL_MASK                  (BIT(4) | BIT(3))
#define DPM_CFG_PORT_ENDIS_MASK             (BIT(5))

/*Bit Pos for gasCfgStatusData.sPerPortData[u8PortNum].u32CfgData */
#define DPM_CFG_POWER_ROLE_POS              0
#define DPM_CFG_DUAL_ROLE_DATA_POS          2
#define DPM_CFG_RPVAL_POS                   3
#define DPM_CFG_PORT_ENDIS_POS              5
#define DPM_CFG_VCONN_OCS_EN_POS            9
/*Enable defines for gasCfgStatusData.sPerPortData[u8PortNum].u32CfgData */
#define DPM_CFG_PORT_ENABLE                 (1 << DPM_CFG_PORT_ENDIS_POS)
#define DPM_CFG_VCONN_OCS_ENABLE               (1 << DPM_CFG_VCONN_OCS_EN_POS)

/*Defines for getting default values configured to a port from gasDPM[u8PortNum].u8DPMConfigData*/
/*DPM_GET_CONFIGURED_POWER_ROLE(u8PortNum) will return one of the following values
	- PD_ROLE_SINK
	- PD_ROLE_SOURCE
	- PD_ROLE_DRP*/
#define DPM_GET_CONFIGURED_POWER_ROLE(u8PortNum)  \
    ((gasCfgStatusData.sPerPortData[u8PortNum].u32CfgData & DPM_CFG_POWER_ROLE_MASK) \
    >> DPM_CFG_POWER_ROLE_POS)

/*Possible value of below define
    TYPEC_UFP                               PD_ROLE_UFP
    TYPEC_DFP_DEFAULT_CURRENT				(0x01)
    TYPEC_DFP_1A5_CURRENT				    (0x02)
    TYPEC_DFP_3A0_CURRENT				    (0x03) */
#define DPM_GET_CONFIGURED_SOURCE_RP_VAL(u8PortNum)\
    ((gasCfgStatusData.sPerPortData[u8PortNum].u32CfgData & DPM_CFG_RPVAL_MASK) \
                            >> DPM_CFG_RPVAL_POS)
/*Possible value for below define
    UPD_PORT_ENABLED - 0x1
    UPD_PORT_DISABLED - 0x0*/
/*Define to get Port Enable*/
#define DPM_GET_CONFIGURED_PORT_EN(u8PortNum)\
((gasCfgStatusData.sPerPortData[u8PortNum].u32CfgData & DPM_CFG_PORT_ENDIS_MASK) \
            >> DPM_CFG_PORT_ENDIS_POS)
#define DPM_DISABLE_CONFIGURED_PORT_EN(u8PortNum)\
(gasCfgStatusData.sPerPortData[u8PortNum].u32CfgData &= ~(DPM_CFG_PORT_ENABLE))
#define DPM_ENABLE_CONFIGURED_PORT_EN(u8PortNum)\
(gasCfgStatusData.sPerPortData[u8PortNum].u32CfgData |= (DPM_CFG_PORT_ENABLE))

/*Define to Get VCONN OCS - Possible value
    DPM_CFG_VCONN_OCS_ENABLE - BIT(9) in case of enable or 0x00 in case of disable*/
#define DPM_GET_CONFIGURED_VCONN_OCS_EN(u8PortNum)\
(gasCfgStatusData.sPerPortData[u8PortNum].u32CfgData & (DPM_CFG_VCONN_OCS_ENABLE))
/*************************************************************************************************/

/**************************************************************************************************/
/***************************Define to get DPM current status data*****************************************/
/************************************************************************************************************/
/*Bit definition for u16DPMStatus variable*/
#define DPM_CURR_POWER_ROLE_MASK            (BIT(0)|BIT(1))
#define DPM_CURR_DATA_ROLE_MASK             (BIT(2)|BIT(3))
#define DPM_CURR_PD_SPEC_REV_MASK           (BIT(4)|BIT(5))
#define DPM_VDM_STATE_ACTIVE_MASK           BIT(6)
#define DPM_CURR_EXPLICIT_CONTRACT_TYPE_MASK (BIT(8) | BIT(7))
/*Bit position for u16DPMStatus variable*/
#define DPM_CURR_POWER_ROLE_POS            0
#define DPM_CURR_DATA_ROLE_POS             2
#define DPM_CURR_PD_SPEC_REV_POS           4
#define DPM_VDM_STATE_ACTIVE_POS           6
#define DPM_CURR_EXPLICIT_CONTRACT_TYPE_POS  7 
/*Defines for getting current status of a port from gasDPM[u8PortNum].u16DPMStatus using u8PortNum variable*/
/*DPM_GET_CURRENT_POWER_ROLE(u8PortNum) will return one of the following values
	- PD_ROLE_SINK
	- PD_ROLE_SOURCE
	- PD_ROLE_DRP*/
#define DPM_GET_CURRENT_POWER_ROLE(u8PortNum)         \
    ((gasDPM[u8PortNum].u16DPMStatus & DPM_CURR_POWER_ROLE_MASK) >> DPM_CURR_POWER_ROLE_POS)

/*DPM_GET_CURRENT_DATA_ROLE(u8PortNum) will return one of the following values
	- PD_ROLE_UFP
	- PD_ROLE_DFP
	- PD_ROLE_TOGGLING */
#define DPM_GET_CURRENT_DATA_ROLE(u8PortNum)         \
   ((gasDPM[u8PortNum].u16DPMStatus & DPM_CURR_DATA_ROLE_MASK) >> DPM_CURR_DATA_ROLE_POS)

#define DPM_GET_CURRENT_PD_SPEC_REV(u8PortNum)       \
    ((gasDPM[u8PortNum].u16DPMStatus & DPM_CURR_PD_SPEC_REV_MASK) >> DPM_CURR_PD_SPEC_REV_POS)
#define DPM_GET_CURRENT_EXPLICIT_CONTRACT(u8PortNum) \
    ((gasDPM[u8PortNum].u16DPMStatus & DPM_CURR_EXPLICIT_CONTRACT_TYPE_MASK) >> \
    DPM_CURR_EXPLICIT_CONTRACT_TYPE_POS)
/**************************************************************************************************/

/*******************************************************************************/
/*********************** Define to get status from u8PEPortSts ********* */
/*Defines to get hardreset status*/
#define DPM_IS_HARDRESET_IN_PROGRESS(u8PortNum)    \
((gasPolicyEngine[u8PortNum].u8PEPortSts & PE_HARDRESET_PROGRESS_MASK) >> \
    PE_HARDRESET_PROGRESS_POS)

/*Define to check if PR_SWAP is in progress ****/
#define DPM_PR_SWAP_IN_PROGRESS(u8PortNum)  ((gasPolicyEngine[u8PortNum].u8PEPortSts & PE_PR_SWAP_IN_PROGRESS_MASK) >> \
                                                                       PE_PR_SWAP_IN_PROGRESS_POS)


// *****************************************************************************
// *****************************************************************************
// Section: Defines to get data from global structure pasCfgStatusData
// *****************************************************************************
// *****************************************************************************
/* ****************************** Port Connection Status parameters - **********
 *  gasCfgStatusData.sPerPortData[u8PortNum].u32PortConnectStatus *********** */
#define DPM_PORT_ATTACHED_STATUS                              BIT(0)
#define DPM_PORT_ORIENTATION_FLIPPED_STATUS                   BIT(1)
#define DPM_PORT_DATA_ROLE_STATUS_POS                         2
#define DPM_PORT_DATA_ROLE_STATUS_MASK                        ((BIT(0)|BIT(1)) << DPM_PORT_DATA_ROLE_STATUS_POS)
#define DPM_PORT_DATA_ROLE_STATUS_UFP                         (0 << DPM_PORT_DATA_ROLE_STATUS_POS)
#define DPM_PORT_DATA_ROLE_STATUS_DFP                         (1 << DPM_PORT_DATA_ROLE_STATUS_POS)
#define DPM_PORT_DATA_ROLE_STATUS_TOGGLING                    (2 << DPM_PORT_DATA_ROLE_STATUS_POS)
#define DPM_PORT_POWER_ROLE_STATUS_POS                        4
#define DPM_PORT_POWER_ROLE_STATUS_MASK                       ((BIT(0)|BIT(1)) << DPM_PORT_POWER_ROLE_STATUS_POS)
#define DPM_PORT_POWER_ROLE_STATUS_SINK                       (0 << DPM_PORT_POWER_ROLE_STATUS_POS)
#define DPM_PORT_POWER_ROLE_STATUS_SOURCE                     (1 << DPM_PORT_POWER_ROLE_STATUS_POS)
#define DPM_PORT_POWER_ROLE_STATUS_DRP                        (2 << DPM_PORT_POWER_ROLE_STATUS_POS)
#define DPM_PORT_VCONN_STATUS                                 BIT(6)
#define DPM_PORT_CABLE_REDUCED_SRC_CAPABILITIES_STATUS        BIT(7)
#define DPM_PORT_SRC_CAPABILITIES_REDUCED_STATUS              BIT(8)
#define DPM_PORT_SRC_CAPABILITY_MISMATCH_STATUS               BIT(9)
#define DPM_PORT_AS_SRC_PD_CONTRACT_GOOD_STATUS               BIT(10)
#define DPM_PORT_AS_SRC_RDO_ACCEPTED_STATUS                   BIT(11)
#define DPM_PORT_AS_SRC_RDO_REJECTED_STATUS                   BIT(12)
#define DPM_PORT_AS_SNK_LAST_REQ_ACCEPT_STATUS                BIT(13)
#define DPM_PORT_AS_SNK_LAST_REQ_REJECT_STATUS                BIT(14)
#define DPM_PORT_AS_SNK_LAST_REQ_PS_RDY_STATUS                BIT(15)
#define DPM_PORT_SINK_CAPABILITY_MISMATCH_STATUS              BIT(16)
#define DPM_PORT_RP_VAL_DETECT_DEFAULT_USB_STATUS             BIT(17)
#define DPM_PORT_RP_VAL_DETECT_1_5A_STATUS                    BIT(18)
#define DPM_PORT_RP_VAL_DETECT_3A_STATUS                     (BIT(17)|BIT(18))
#define DPM_PORT_RP_VAL_DETECT_MASK_STATUS                   (BIT(17)|BIT(18))

/* *************************Port IO Status parameters *****************************
 * gasCfgStatusData.sPerPortData[u8PortNum].u32PortIOStatus *********************** */
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
#define DPM_PORT_IO_POWER_ROLE_STATUS                BIT(10)
#define DPM_PORT_IO_DATA_ROLE_STATUS                 BIT(11)

/***************************u16SwapPolicy values*****************************/
#define DPM_AUTO_DR_SWAP_REQ_AS_DFP          BIT(0)
#define DPM_AUTO_DR_SWAP_REQ_AS_UFP          BIT(1)
#define DPM_AUTO_DR_SWAP_ACCEPT_AS_DFP       BIT(2)
#define DPM_AUTO_DR_SWAP_ACCEPT_AS_UFP       BIT(3)
#define DPM_AUTO_PR_SWAP_REQ_AS_SRC          BIT(4)
#define DPM_AUTO_PR_SWAP_REQ_AS_SNK          BIT(5)
#define DPM_AUTO_PR_SWAP_ACCEPT_AS_SRC       BIT(6)
#define DPM_AUTO_PR_SWAP_ACCEPT_AS_SNK       BIT(7)
#define DPM_AUTO_VCONN_SWAP_REQ_AS_SRC       BIT(8)
#define DPM_AUTO_VCONN_SWAP_REQ_AS_SNK       BIT(9)
#define DPM_AUTO_VCONN_SWAP_ACCEPT_AS_SRC    BIT(10)
#define DPM_AUTO_VCONN_SWAP_ACCEPT_AS_SNK    BIT(11)

/*********************u8SinkConfigSel defines******************/
#define DPM_SINK_CONFIG_SINK_MODE_SEL_MASK  (BIT(0) | BIT(1))
#define DPM_SINK_MODE_A      0x00
#define DPM_SINK_MODE_B      BIT(0)

/*********************u8DAC_I_Direction defines****************/
#define DPM_DAC_DIR_HIGH_AMP_MAX_VOLT 0
#define DPM_DAC_DIR_HIGH_AMP_MIN_VOLT 1

#define DPM_SINK_CONFIG_NO_USB_SUSP_POS        2
#define DPM_SINK_CONFIG_NO_USB_SUSP_MASK       BIT(2)

#define DPM_SINK_CONFIG_GIVE_BACK_FLAG_POS  3
#define DPM_SINK_CONFIG_GIVE_BACK_FLAG_MASK BIT(3)

/****************** Power Balancing Defines ***********/
/**************************Feature Select parameters - **********
 ****gasCfgStatusData.sPerPortData[u8PortNum].u16FeatureSelect *********************** */
#define DPM_PORT_PB_ENABLE                           BIT(0)
/* Macro to know if PB is enabled for the system and for the port */
#define DPM_IS_PB_ENABLED(u8PortNum)   \
    ((gasCfgStatusData.sPerPortData[u8PortNum].u16FeatureSelect & DPM_PORT_PB_ENABLE) \
    ? TRUE : FALSE)   
/**********gasCfgStatusData.u8PBEnableSelect defines******/
/* PB Enable for System */
#define DPM_PB_ENABLE                   0x10

/****************** Power throttling Defines ***********/
/* PT Enable for the system */
#define DPM_PT_ENABLE                   0x01 
/* Macro to know if PT is enabled for the system */
#define DPM_IS_PT_ENABLED             \
    ((gasCfgStatusData.u8PwrThrottleCfg & DPM_PT_ENABLE) ? TRUE : FALSE)  
/* Macro to get current PT Bank */
#define DPM_PT_BANK_MASK                    0x06 
#define DPM_PT_BANK_POS                     1
#define DPM_GET_CURRENT_PT_BANK             ((gasCfgStatusData.u8PwrThrottleCfg & DPM_PT_BANK_MASK) >> DPM_PT_BANK_POS)

/* Power Throttling Bank values */
#define DPM_PD_THROTTLE_BANK_A                      0U 
#define DPM_PD_THROTTLE_BANK_B                      1U
#define DPM_PD_THROTTLE_BANK_C                      2U
#define DPM_PD_THROTTLE_SHUTDOWN_MODE               3U


// *****************************************************************************
// *****************************************************************************
// Section: Defines to get data from given PDO
// *****************************************************************************
// *****************************************************************************   
/*Macros to get PDO type, PDO Current, PDO voltage,USB Comm capable bit from
Source/Sink Power delivery objects*/

/*Defines for getting voltage from PDO*/
#define DPM_PDO_VOLTAGE_MASK                 0x000FFC00
#define DPM_PDO_VOLTAGE_POS                  10
#define DPM_PDO_VOLTAGE_UNIT                 50
#define DPM_GET_VOLTAGE_FROM_PDO_MILLI_V(u32PDO)   \
    (((u32PDO & DPM_PDO_VOLTAGE_MASK) >> DPM_PDO_VOLTAGE_POS) * DPM_PDO_VOLTAGE_UNIT)

/*Defines for getting current from PDO[9:0]*/
#define DPM_PDO_CURRENT_MASK              0x000003FF 
#define DPM_PDO_CURRENT_UNIT                 10

#define DPM_GET_PDO_TYPE(X)   					((X & 0xC0000000) >> 30)
#define DPM_GET_PDO_CURRENT(X)                  ((X & 0x000003FF))
#define DPM_GET_PDO_VOLTAGE(X)                  ((X & 0x000FFC00) >> 10)	/*in 50mv units*/
#define DPM_GET_PDO_USB_COMM_CAP(X)             ((X & 0x04000000) >> 26)
#define DPM_GET_PDO_DUAL_DATA(X)                ((X & 0x02000000) >> 25)
#define DPM_GET_PDO_DUAL_POWER(X)               ((X & 0x20000000) >> 29)
#define DPM_MAX_PDO_CNT                          7

#define DPM_DEBUG_PDO_GENERATION( USB_SUSPEND, UNCONS_POWER, USB_COM, MAX_CURRENT, MAX_VOLTAGE)  \
        (((UINT32)USB_SUSPEND << 28) | ((UINT32)UNCONS_POWER << 27) | ((UINT32)USB_COM << 26) | (((UINT32)MAX_VOLTAGE/50) << 10) | (((UINT32)MAX_CURRENT)/10))

#define DPM_DEBUG_PDO_5V_9MA      DPM_DEBUG_PDO_GENERATION(1, 1, 0, 900, 5000)
#define DPM_DEBUG_PDO_5V_1P5A     DPM_DEBUG_PDO_GENERATION(1, 1, 0, 1500, 5000)
#define DPM_DEBUG_PDO_5V_3A       DPM_DEBUG_PDO_GENERATION(1, 1, 0, 3000, 5000)
// *****************************************************************************
// *****************************************************************************
// Section: Defines to get data from given APDO
// *****************************************************************************
// *****************************************************************************   
/*********************PPS APDO Defines ******************/
#define DPM_APDO_MAX_CURRENT_UNIT                50 
#define DPM_APDO_MIN_VOLTAGE_UNIT                100
#define DPM_APDO_MAX_VOLTAGE_UNIT                100

#define DPM_APDO_MIN_VOLTAGE_POS                 8
#define DPM_APDO_MAX_VOLTAGE_POS                 17 
#define DPM_APDO_PWR_LIMITED_POS                 27 

#define DPM_APDO_MAX_CURRENT_MASK                0x0000007F 
#define DPM_GET_APDO_MAX_CURRENT(u32Apdo)        ((u32Apdo) & DPM_APDO_MAX_CURRENT_MASK)      
#define DPM_GET_APDO_MAX_CURRENT_IN_mA(u32Apdo)  ((DPM_GET_APDO_MAX_CURRENT(u32Apdo)) * DPM_APDO_MAX_CURRENT_UNIT)

#define DPM_APDO_MIN_VOLTAGE_MASK                0x0000FF00 
#define DPM_GET_APDO_MIN_VOLTAGE(u32Apdo)        (((u32Apdo) & DPM_APDO_MIN_VOLTAGE_MASK) >> DPM_APDO_MIN_VOLTAGE_POS)
#define DPM_GET_APDO_MIN_VOLTAGE_IN_mV(u32Apdo)  ((DPM_GET_APDO_MIN_VOLTAGE(u32Apdo)) * DPM_APDO_MIN_VOLTAGE_UNIT) 

#define DPM_APDO_MAX_VOLTAGE_MASK                0x01FE0000 
#define DPM_GET_APDO_MAX_VOLTAGE(u32Apdo)        (((u32Apdo) & DPM_APDO_MAX_VOLTAGE_MASK) >> DPM_APDO_MAX_VOLTAGE_POS)
#define DPM_GET_APDO_MAX_VOLTAGE_IN_mV(u32Apdo)  ((DPM_GET_APDO_MAX_VOLTAGE(u32Apdo)) * DPM_APDO_MAX_VOLTAGE_UNIT) 

#define DPM_3A_IN_50mA                           (DPM_3000mA / 50) 

/*********************PPS RDO Defines ******************/ 
/* Defines for return values of DPM_GET_CURRENT_EXPLICIT_CONTRACT macro */
#define DPM_PD_FIXED_SUPPLY_CONTRACT             0U
#define DPM_PD_BATTERY_SUPPLY_CONTRACT           1U
#define DPM_PD_VARIABLE_SUPPLY_CONTRACT          2U
#define DPM_PD_PPS_CONTRACT                      3U

#define DPM_PROG_RDO_OP_VOLTAGE_MASK             0x000FFE00
#define DPM_PROG_RDO_OPR_CURRENT_MASK            0x0000007F

#define DPM_PROG_RDO_OP_VOLTAGE_POS              9 

#define DPM_PROG_RDO_OP_VOLTAGE_UNIT             20
#define DPM_PROG_RDO_OPR_CURRENT_UNIT            50

#define DPM_GET_OP_VOLTAGE_FROM_PROG_RDO(u32Rdo)       (((u32Rdo) & DPM_PROG_RDO_OP_VOLTAGE_MASK) >> DPM_PROG_RDO_OP_VOLTAGE_POS)  
#define DPM_GET_OP_VOLTAGE_FROM_PROG_RDO_IN_mV(u32Rdo) ((DPM_GET_OP_VOLTAGE_FROM_PROG_RDO(u32Rdo)) * DPM_PROG_RDO_OP_VOLTAGE_UNIT)   

#define DPM_GET_PROG_RDO_OPR_CURRENT(u32Rdo)         ((u32Rdo) & DPM_PROG_RDO_OPR_CURRENT_MASK)  
#define DPM_GET_PROG_RDO_OPR_CURRENT_IN_mA(u32Rdo)   ((DPM_GET_PROG_RDO_OPR_CURRENT(u32Rdo)) * DPM_PROG_RDO_OPR_CURRENT_UNIT)   

/* vPpsSmallStep of USB PD Spec 3.0 */
#define DPM_PPS_VOLTAGE_SMALL_STEP               500

/***************************PPS status Data block *********************/
#define DPM_PPSSDB_OUTPUT_VOLTAGE_FIELD_POS       0
#define DPM_PPSSDB_OUTPUT_CURRENT_FIELD_POS       16
#define DPM_PPSSDB_REAL_TIME_FLAG_FIELD_POS       24

#define DPM_PPSSDB_OUTPUT_VOLT_UNSUPPORTED_VAL            0xFFFF
#define DPM_PPSSDB_OUTPUT_CURRENT_UNSUPPORTED_VAL         0xFF
#define DPM_PPSSDB_OUTPUT_USER_CONFIGURED_UNSUPPORTED_VAL 0xFFFFFFFF

/*******************************************************************************/

// *****************************************************************************
// *****************************************************************************
// Section: Defines to decode VDM packet
// *****************************************************************************
// ***************************************************************************** 
/** Macros for E-Cable *****/
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

// *****************************************************************************
// *****************************************************************************
// Section: Defines to form Data Request packet
// *****************************************************************************
// *****************************************************************************   
#define DPM_FORM_DATA_REQUEST(OBJECT_POSITION,CAPABLITY_MISMATCH,GIVEBACK_FLAG, \
        USB_COMMUNICATION_CAPABLE,NO_USB_SUSP,OPERATING_CURRENT,MAXIMUM_OPERATING_CURRENT)\
        ((OBJECT_POSITION << 28) | (GIVEBACK_FLAG << 27) | (CAPABLITY_MISMATCH << 26) | \
        (USB_COMMUNICATION_CAPABLE << 25) | (NO_USB_SUSP << 24) | (OPERATING_CURRENT << 10) \
          | (MAXIMUM_OPERATING_CURRENT))

// *****************************************************************************
// *****************************************************************************
// Section: constants for function return values
// *****************************************************************************
// *****************************************************************************
/********************** Return Values from PE_ValidateMessage API**************/
#define DPM_VALID_REQUEST            1
#define DPM_INVALID_REQUEST          0   

/***********Macros for u8VConnEnable argument of DPM_VCONNOnOff API ************/
#define DPM_VCONN_ON             1
#define DPM_VCONN_OFF            0

/*********Macros for u8VbusOnorOff argument of DPM_TypeCSrcVBus5VOnOff API*********/
#define DPM_VBUS_ON              1
#define DPM_VBUS_OFF             0

/**** Macros for PDOIndex Port Power Handling for Typec Vsafe0V and Vsafe5V****/
#define DPM_VSAFE0V_PDO_INDEX   0
#define DPM_VSAFE5V_PDO_INDEX_1 1

/*******************DPM_ReturnPowerStatus possible return values macros*********/
/*Source Power limited due to cable supported current*/
#define DPM_PWRSTS_SRCPWR_LTD_CABLE_CURR           BIT(1)
/*Source Power limited due to insufficient power available while sourcing other ports */
#define DPM_PWRSTS_SRCPWR_LTD_INSUFF_PWR_AVAIL     BIT(2)
/*Source Power limited due to insufficient external power */
#define DPM_PWRSTS_SRCPWR_LTD_INSUFF_EXT_PWR       BIT(3)
/*Source power limited due to Event Flag in place (Event flag must also be set)*/
#define DPM_PWRSTS_SRCPWR_LTD_EVNT_FLAG            BIT(4)
/*Source power limited due to temperature*/
#define DPM_PWRSTS_SRCPWR_LTD_TEMP                 BIT(5)
/********************** Return Values from DPM_EvaluateRoleSwap API**************/
#define DPM_IGNORE_INITIATE_SWAP            3
#define DPM_REQUEST_SWAP                    2
#define DPM_ACCEPT_SWAP                     1
#define DPM_REJECT_SWAP                     0 
/******************************************************************************/

// *****************************************************************************
// *****************************************************************************
// Section: constants
// *****************************************************************************
// *****************************************************************************
/*defines to set u16SinkOperatingCurrInmA */
#define DPM_0mA     0
#define DPM_50mA    50
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
#define DPM_20mV    20
#define DPM_50mV    50

/* define to set allocated power in terms of 250mW */
#define DPM_250mW   250000

/************************Temperature Status possible values macros*********/
#define DPM_TEMP_STATUS_NOT_SUPPORTED       0
#define DPM_TEMP_STATUS_NORMAL              BIT(1)
#define DPM_TEMP_STATUS_WARNING             BIT(2)
#define DPM_TEMP_STATUS_OVER_TEMP           (BIT(1) | BIT(2))
#define DPM_TEMP_STATUS_MASK                (BIT(1) | BIT(2))

/*********************Defines for evaluating and sorting PDOs******************/
#define DPM_2_DIMENSION_TO_STORE_INDEX_AND_PWR 2
#define DPM_PDO_WITH_MAX_PWR 0
#define DPM_PDO_PWR 0
#define DPM_PDO_INDEX 1
#define DPM_NEXT_PWR_INDEX(x) (x+1)
#define DPM_4BYTES_FOR_EACH_PDO_OF(PDO_Count) (PDO_Count*4)

// *****************************************************************************
// *****************************************************************************
// Section: Data Structure and structure variable defines
// *****************************************************************************
// *****************************************************************************
/***************************************u8PowerFaultISR defines*************** */
#define DPM_POWER_FAULT_OVP				BIT(0)
#define DPM_POWER_FAULT_UV				BIT(1)
#define DPM_POWER_FAULT_VBUS_OCS		BIT(2)
#define DPM_POWER_FAULT_VCONN_OCS	    BIT(3)

/************************u8AlertType variable possible values macros*********/
/*Battery Status Change Event (Attach/Detach/Charging/discharging/idle)*/
#define DPM_ALERT_TYPE_BATTERY_STATUS_CHANGE    BIT(1)
/*OCP Applicable only for Source*/
#define DPM_ALERT_TYPE_OCP                      BIT(2)
#define DPM_ALERT_TYPE_OTP                      BIT(3)
/*Operating Condition Change*/
#define DPM_ALERT_TYPE_OPR_COND_CHANGE          BIT(4)
/*Source Input Change Event */
#define DPM_ALERT_TYPE_SRC_INPUT_CHANGE         BIT(5)       
#define DPM_ALERT_TYPE_OVP                      BIT(6)
#define DPM_ALERT_TYPE_FIELD_POS                24

/********************u8StatusEventFlags variable possible values macros*********/
#define DPM_EVENT_TYPE_OCP                      BIT(1)
#define DPM_EVENT_TYPE_OTP                      BIT(2)       
#define DPM_EVENT_TYPE_OVP                      BIT(3)
#define DPM_EVENT_TYPE_CL_MODE                  BIT(4)
#define DPM_EVENT_TYPE_CL_CV_MODE_MASK          BIT(4)

/*OMF - Operating Mode Flag indicating Source operating mode.
 When set it is Current Limit Mode, When cleared it is Constant Voltage mode*/
#define DPM_REAL_TIME_FLAG_OMF_IN_CL_MODE          BIT(3)
#define DPM_REAL_TIME_FLAG_OMF_FIELD_MASK          BIT(3)

/**************************Defines for u8PowerFaultFlags***********************/
#define DPM_HR_COMPLETE_WAIT_POS              0
#define DPM_TYPEC_ERR_RECOVERY_FLAG_POS       1
#define DPM_HR_COMPLETE_WAIT_MASK             (1 << DPM_HR_COMPLETE_WAIT_POS)
#define DPM_TYPEC_ERR_RECOVERY_FLAG_MASK      (1 << DPM_TYPEC_ERR_RECOVERY_FLAG_POS)
 
/****************** New PDO Enable/Disable Defines ************/
#define DPM_ENABLE_NEW_PDO(u8PortNum)     (gasDPM[u8PortNum].u8DPMConfigData |= DPM_NEW_PDO_ENABLE_MASK)
#define DPM_DISABLE_NEW_PDO(u8PortNum)    (gasDPM[u8PortNum].u8DPMConfigData &= ~(DPM_NEW_PDO_ENABLE_MASK))
/************************ Client Request Defines ******************************/
#define DPM_NO_CLIENT_REQ_PENDING                0x00 
#define DPM_CLEAR_ALL_CLIENT_REQ                 0x00 

#define DPM_CLIENT_REQ_RENEGOTIATE               BIT(0)
#define DPM_CLIENT_REQ_HANDLE_FAULT_VBUS_OV      BIT(3)
#define DPM_CLIENT_REQ_HANDLE_FAULT_VBUS_OCS     BIT(4)
#define DPM_CLIENT_REQ_HANDLE_VBUS_OCS_EXIT      BIT(5)

/* Macros to raise a client request to DPM */
#define DPM_SET_RENEGOTIATE_REQ(u8PortNum)     (gasCfgStatusData.sPerPortData[u8PortNum].u32ClientRequest \
                                                            |= DPM_CLIENT_REQ_RENEGOTIATE)
#define DPM_SET_VBUS_FAULT_OV_REQ(u8PortNum)    (gasCfgStatusData.sPerPortData[u8PortNum].u32ClientRequest \
                                                            |= DPM_CLIENT_REQ_HANDLE_FAULT_VBUS_OV)

#define DPM_SET_VBUS_FAULT_OCS_REQ(u8PortNum)   (gasCfgStatusData.sPerPortData[u8PortNum].u32ClientRequest \
                                                            |= DPM_CLIENT_REQ_HANDLE_FAULT_VBUS_OCS)

/***************************Internal Events Defines**********************************/
#define DPM_INT_EVT_INITIATE_GET_SINK_CAPS  BIT(0)
#define DPM_INT_EVT_INITIATE_RENEGOTIATION   BIT(1)
#define DPM_INT_EVT_INITIATE_VCONN_SWAP     BIT(2)
#define DPM_INT_EVT_INITIATE_PR_SWAP        BIT(3)
#define DPM_INT_EVT_INITIATE_DR_SWAP        BIT(4)
#define DPM_INT_EVT_INITIATE_ALERT          BIT(5)
#define DPM_INT_EVT_INITIATE_GET_STATUS     BIT(6)
/**********************************************************************************/                                   
// *****************************************************************************
// *****************************************************************************
// Section: Data Structures
// *****************************************************************************
// *****************************************************************************
/*Structure of Device Policy Manager*/
typedef struct MCHP_PSF_STRUCT_PACKED_START
{
  UINT32  u32NegotiatedPDO;           //NegotiatedPDO
  UINT16 u16MaxCurrSupportedin10mA;   //Maximum current supported by E-Cable in 10mA
  UINT16 u16SinkOperatingCurrInmA;    //Operating current
  UINT16 u16PrevVBUSVoltageInmV;      // Previous VBUS Voltage in terms of mV
  UINT16 u16ExpectedVBUSVoltageInmV;  // Expected VBUS Voltage in terms of mV
  UINT16 u16DPMStatus;                 //Bit 1:0 - Status of Port Role <p />
                                        //Bit 3:2 - Status of Data Role <p />
                                        //Bit 5:4 - Status of PD Spec Revision <p />
                                        //Bit 6 - Status of Vconn Swap support
                                        //Bits 8:7 - Type of current Explicit Contract 
                                        //      00 - Fixed  
                                        //      01 - Variable
                                        //      10 - Battery
                                        //      11 - Programmable
  UINT8 u8DPMInternalEvents;        //DPM_INT_EVT_INITIATE_GET_SINK_CAPS  BIT(0)
                                    //DPM_INT_EVT_INITIATE_RENEGOTIATION    BIT(1)
                                    //DPM_INT_EVT_INITIATE_VCONN_SWAP     BIT(2)
                                    //DPM_INT_EVT_INITIATE_PR_SWAP        BIT(3)
                                    //DPM_INT_EVT_INITIATE_DR_SWAP        BIT(4)
                                    //DPM_INT_EVT_INITIATE_ALERT          BIT(5)
                                    //DPM_INT_EVT_INITIATE_GET_STATUS     BIT(6)
  UINT8 u8DPMConfigData;   //Bit 0 - Default Port Role <p />
                            //Bit 1 - Default Data Role <p />
                            //Bit 3:2 - Default PD Spec Revision <p />
                            //Bit 4 - New PDO Enable <p /> 

  UINT8 u8VCONNErrCounter;
  UINT8 u8NegotiatedPDOIndex;
#if (TRUE == INCLUDE_POWER_FAULT_HANDLING)
      UINT8 u8PowerFaultISR;          //Power fault ISR flag
	  UINT8 u8VBUSPowerGoodTmrID;     //VBUS PowerGood Timer ID
      UINT8 u8VCONNPowerGoodTmrID;    //VConn PowerGood Timer ID
	  UINT8 u8VBUSPowerFaultCount;      //VBUS Power fault count
      UINT8 u8VCONNPowerFaultCount;     //VCONN Power fault count     
      UINT8 u8VCONNGoodtoSupply;        //VCONN good to supply
	  UINT8 u8PowerFaultFlags;        //Flags required for power fault handling
                                      //BIT 0 - Hard Reset complete wait flag
                                      //BIT 1 - Type-C Error Recovery Flag
#endif
#if (TRUE == INCLUDE_PD_SOURCE_PPS)
  UINT8 u8AlertType;
  UINT8 u8StatusEventFlags;
  UINT8 u8RealTimeFlags;
  UINT8 u8StsClearTmrID;
#endif
#if (TRUE == INCLUDE_PD_PR_SWAP)
  UINT8 u8PRSwapWaitTmrID;         // PR_Swap Wait Timer ID
#endif 
#if (TRUE == INCLUDE_PD_DR_SWAP)
  UINT8 u8DRSwapWaitTmrID;
#endif
}MCHP_PSF_STRUCT_PACKED_END DEVICE_POLICY_MANAGER;

// *****************************************************************************
// *****************************************************************************
// Section: Enum
// *****************************************************************************
// *****************************************************************************
/***********************************************************************************/
/* Enumeration to define the types of PDO */ 
typedef enum PDOtype
{
    ePDO_FIXED = 0x00,
    ePDO_BATTERY = 0x01,
    ePDO_VARIABLE = 0x02,
    ePDO_PROGRAMMABLE = 0x03,
    ePDO_INVALID = 0xFF
} ePDOtypes;

/* Enum for Swap messages */
typedef enum {
    eVCONN_SWAP_RCVD = 0x0,
    eDR_SWAP_RCVD = BIT(0),
    ePR_SWAP_RCVD = BIT(1),
    eVCONN_SWAP_INITIATE = BIT(2), /*same as DPM_INT_EVT_INITIATE_VCONN_SWAP value*/
    ePR_SWAP_INITIATE = BIT(3),  /*same as DPM_INT_EVT_INITIATE_PR_SWAP value*/
    eDR_SWAP_INITIATE = BIT(4)  /*same as DPM_INT_EVT_INITIATE_DR_SWAP value*/
}eRoleSwapMsgtype;

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
        This API is called inside the PD Stack initialization API call .
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
        void DPM_GetSourceCapabilities(UINT8 u8PortNum, UINT8* NumOfPdo, UINT32* pu32DataObj);
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
void DPM_GetSourceCapabilities(UINT8 u8PortNum, UINT8* NumOfPdo, UINT32* pu32DataObj);

/**************************************************************************************************
    Function:
        UINT8 DPM_ValidateRequest(UINT8 u8PortNum, UINT16 u16Header, UINT8 *u8DataBuf);
    Summary:
        This API is used to validate the received request message from the sink port partner.
    Devices Supported:
        UPD350 REV A
    Description:
         This API is used to validate the received request message from the sink port partner.
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
        void DPM_UpdatePowerRole(UINT8 u8PortNum, UINT8 u8NewPowerRole)
    Summary:
        This API is used to set power role in gasDPM[u8PortNum].u16DPMStatus
        variable, Port Connect Status register and Port IO Status register.
    Devices Supported:
        UPD350 REV A
    Description:
        This API is used to assign power role in various status fields
        namely gasDPM[u8PortNum].u16DPMStatus variable, Port Connect Status register
        and Port IO Status register.
    Conditions:
        None.
    Input:
        u8PortNum      - Port Number for power and data roles need to be assigned
        u8NewPowerRole - New Power role to be assigned to the port.
                      This can take the following values:
                      1. PD_ROLE_SOURCE
                      2. PD_ROLE_SINK
                      3. PD_ROLE_DRP
    Return:
        None
    Remarks:
        None
**************************************************************************************************/
void DPM_UpdatePowerRole(UINT8 u8PortNum, UINT8 u8NewPowerRole);

/**************************************************************************************************
    Function:
        void DPM_UpdateDataRole(UINT8 u8PortNum, UINT8 u8NewDataRole)
    Summary:
        This API is used to set data role in gasDPM[u8PortNum].u16DPMStatus
        variable, Port Connect Status register and Port IO Status register.
    Devices Supported:
        UPD350 REV A
    Description:
        This API is used to assign data role in various status fields
        namely gasDPM[u8PortNum].u16DPMStatus variable, Port Connect Status register
        and Port IO Status register.
    Conditions:
        None.
    Input:
        u8PortNum     - Port Number for power and data roles need to be assigned
        u8NewDataRole - Data role to be assigned to the port.
                      This can take the following values:
                      1. PD_ROLE_DFP
                      2. PD_ROLE_UFP
                      3. PD_ROLE_TOGGLING
    Return:
        None
    Remarks:
        None
**************************************************************************************************/
void DPM_UpdateDataRole(UINT8 u8PortNum, UINT8 u8NewDataRole);

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
        void DPM_VCONNOnOff(UINT8 u8PortNum, UINT8 u8VConnEnable);
    Summary:
        This API is used to turn on/off the VCONN supply of a given port
    Devices Supported:
        UPD350 REV A
    Description:
         This API is used to turn on/off the VCONN of a given port
    Conditions:
        None.
    Input:
        u8PortNum     - Port Number
        u8VConnEnable - Variable denoting either to turn on or off the VCONN
                      - DPM_VCONN_ON (Turn on VCONN)
                      - DPM_VCONN_OFF (Turn off VCONN)
    Return:
        None
    Remarks:
        None
**************************************************************************************************/
void DPM_VCONNOnOff(UINT8 u8PortNum, UINT8 u8VConnEnable);

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
        u8PortNum - Port Number 
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
        u8PortNum       - Port Number 
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
        u8PortNum       - Port Number 
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
        u8PortNum       - Port Number
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
        UINT8 DPM_IsPortVCONNSource(UINT8 u8PortNum);
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
        u8PortNum  - Port Number 
    Return:
        UINT8

    Remarks:
        None
**************************************************************************************************/
UINT8 DPM_IsPortVCONNSource(UINT8 u8PortNum);

/**************************************************************************************************
    Function:
        void DPM_EvaluateReceivedSrcCaps(UINT8 u8PortNum ,UINT16 u16RecvdSrcCapsHeader, UINT32 *u32RecvdSrcCapsPayload);

    Summary:
        Device Policy Manager evaluates the received source capability against the sink capability and 
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
void DPM_EvaluateReceivedSrcCaps(UINT8 u8PortNum ,UINT16 u16RecvdSrcCapsHeader, UINT32 *u32RecvdSrcCapsPayload);

/**************************************************************************************************
    Function:
        UINT8 DPM_FindSrcSinkCapsMatch(UINT32 u32In_SrcPDO, UINT32 u32In_SinkPDO);
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
UINT8 DPM_FindSrcSinkCapsMatch(UINT32 u32In_SrcPDO, UINT32 u32In_SinkPDO);

/**************************************************************************************************
    Function:
        void DPM_GetSinkCapabilities(UINT8 u8PortNum,UINT8* u8pSinkPDOCnt, UINT32 * pu32DataObj);
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
void DPM_GetSinkCapabilities(UINT8 u8PortNum, UINT8* u8pSinkPDOCnt, UINT32 * pu32DataObj);

/**************************************************************************************************
    Function:
        void DPM_VBUSorVCONNOnOff_TimerCB (UINT8 u8PortNum, UINT8 u8DummyVariable);
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
void DPM_VBUSorVCONNOnOff_TimerCB (UINT8 u8PortNum, UINT8 u8DummyVariable);

/**************************************************************************************************
    Function:
        void DPM_SrcReady_TimerCB (UINT8 u8PortNum, UINT8 u8DummyVariable);
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
void DPM_SrcReady_TimerCB (UINT8 u8PortNum, UINT8 u8DummyVariable);

/**************************************************************************************************
    Function:
        void DPM_VCONNONError_TimerCB (UINT8 u8PortNum , UINT8 u8DummyVariable);
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
void DPM_VCONNONError_TimerCB (UINT8 u8PortNum , UINT8 u8DummyVariable);

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
        to detect over voltage and under voltage for the voltage negotiated and established with the 
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
        void DPM_ClientRequestHandler(UINT8 u8PortNum);
    Summary:
        API handles client request from Application layer. 
    Devices Supported:
        UPD350 REV A
    Description:
        Application layer sets the corresponding Client Request bit in
        Configuration for the DPM to handle any Client request 
        that includes Renegotiation, Get Sink caps, etc., 
    Conditions:
        None
    Input:
        u8PortNum - Port number of the device.Value passed will be less than CONFIG_PD_PORT_COUNT.
    Return:
        None. 
    Remarks:
        None.
**************************************************************************************************/
void DPM_ClientRequestHandler(UINT8 u8PortNum); 

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
        the REDUCED_SOURCE_CAPABILITIES bit in Port Connection Status 
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

/**************************************************************************************************
    Function:
        UINT32 DPM_ReturnPPSSrcTransTmrVal(UINT8 u8PortNum);
    Summary:
        Determines if PS_RDY needs to be sent within tPpsSrcTransLarge 
        or tPpsSrcTransSmall in case of PPS contract.  
    Description:
        This API is used to determine if PS_RDY needs to be sent within 
        tPpsSrcTransLarge or tPpsSrcTransSmall in case the current explicit
        contract is for a Programmable APDO.
    Conditions:
        None
    Input:
        u8PortNum - Port number of the device.Value passed will be less than CONFIG_PD_PORT_COUNT.
    Return:
        PE_PPS_SRCTRANSLARGE_TIMEOUT_MS - If difference between previous and current 
        RDO output voltages is greater than vPpsSmallStep
        PE_PPS_SRCTRANSSMALL_TIMEOUT_MS - If difference between previous and current 
        RDO output voltages is less than or equal to vPpsSmallStep
    Remarks:
        None.
**************************************************************************************************/

UINT32 DPM_ReturnPPSSrcTransTmrVal(UINT8 u8PortNum);

/**************************************************************************************************
    Function:
        void DPM_HandleExternalVBUSFault(UINT8 u8PortNum, UINT8 u8FaultType); 
    Summary:
        Enables DPM to handle the VBUS Fault.   
    Description:
        PSF has inbuilt fault mechanism to handle VCONN OCS, VBUS OCS 
        through FAULT_IN pin, Over Voltage and Under voltage. If external 
        VBUS fault is detected by the system, for PSF to handle this fault,
        this API would be called if a client request has been raised to 
        handle the fault condition. 
    Conditions:
        None
    Input:
        u8PortNum - Port number of the device.Value passed will be less than CONFIG_PD_PORT_COUNT.
        u8FaultType - Type of Fault
    Return:
        None. 
    Remarks:
        None.
**************************************************************************************************/

void DPM_HandleExternalVBUSFault(UINT8 u8PortNum, UINT8 u8FaultType); 

/**************************************************************************************************
    Function:
        void DPM_UpdatePDO (UINT8 u8PortNum, UINT16 u16PowerIn250mW);

    Summary:
        This API is used to form the PDOs as per power wattage value given.   

    Description:
        This API updates the u32aNewPDO[7] array by calculating the current 
        value using power value that is given as input to this API and voltage
        value derived from u32aSourcePDO[7] array

    Conditions:
        None.

    Input:
        u8PortNum - Port number.
        u16PowerIn250mW - Power value in terms of 250mW

    Return:
        None. 

    Remarks:
        None. 

**************************************************************************************************/
void DPM_UpdatePDO(UINT8 u8PortNum, UINT16 u16PowerIn250mW); 

/**************************************************************************************************
    Function:
        void DPM_InternalEventHandler(UINT8 u8PortNum); 
    Summary:
        This API handles the internal events posted to DPM
    Description:
        This API to handle the internal events posted to DPM by various layer.
    Conditions:
        None.
    Input:
        u8PortNum - Port number.
    Return:
        None. 
    Remarks:
        None. 
**************************************************************************************************/
void DPM_InternalEventHandler(UINT8 u8PortNum);

/**************************************************************************************************
    Function:
        void DPM_RegisterInternalEvent(UINT8 u8PortNum, UINT8 u8EventType)
    Summary:
        This API to register an internal events to DPM layer
    Description:
        Internal Events to be handled by DPM_InternalEventHandler shall be registered by
        this API. 
    Conditions:
        None.
    Input:
        u8PortNum - Port number.
        u8EventType - Internal Event Type to be registered.
    Return:
        None. 
    Remarks:
        None. 
**************************************************************************************************/
void DPM_RegisterInternalEvent(UINT8 u8PortNum, UINT8 u8EventType);

/**************************************************************************************************
    Function:
        UINT32 DPM_ObtainAlertDO(UINT8 u8PortNum)
    Summary:
        Returns Alert Data Object
    Description:
        API to return Alert Data Object for Alert message
    Conditions:
        None.
    Input:
        u8PortNum - Port number.
    Return:
        UINT32 - Alert Data Type. 
    Remarks:
        None. 
**************************************************************************************************/
UINT32 DPM_ObtainAlertDO(UINT8 u8PortNum);

/**************************************************************************************************
    Function:
        DPM_ObtainStatusDB(UINT8 u8PortNum, UINT8 *pau8StatusDO);
    Summary:
        Updates Status Data Block in the input array passed
    Description:
        API to get Status message Data Block
    Conditions:
        None.
    Input:
        u8PortNum - Port number.
        pau8StatusDO - Pointer to array of length 6 to be passed.
    Return:
        None. 
    Remarks:
        None. 
**************************************************************************************************/
void DPM_ObtainStatusDB(UINT8 u8PortNum, UINT8 *pu8StatusDO);

/**************************************************************************************************
    Function:
        UINT32 DPM_ObtainPPSStatusDB (UINT8 u8PortNum)
    Summary:
        Returns PPS Status Data Block
    Description:
        API to return PPS Status message Data Block.
    Conditions:
        None.
    Input:
        u8PortNum - Port number.
    Return:
        UINT32 - PPS status Data Block. 
    Remarks:
        None. 
**************************************************************************************************/
UINT32 DPM_ObtainPPSStatusDB (UINT8 u8PortNum);

/**************************************************************************************************
    Function:
        void DPM_StatusFaultPersist_TimerCB (UINT8 u8PortNum, UINT8 u8DummyVariable)
    Summary:
        Timer callback for DPM_STATUS_FAULT_PERSIST_TIMEOUT_MS timeout
    Description:
        API to clear the status flags stored on DPM_STATUS_FAULT_PERSIST_TIMEOUT_MS timeout
    Conditions:
        None.
    Input:
        u8PortNum - Port number.
        u8DummyVariable - Dummy variable
    Return:
        None.
    Remarks:
        None. 
**************************************************************************************************/
void DPM_StatusFaultPersist_TimerCB (UINT8 u8PortNum, UINT8 u8DummyVariable);

/**************************************************************************************************
    Function:
        UINT8 DPM_ReturnPowerStatus (UINT8 u8PortNum); 
    Summary:
        Returns Current Power Status of the system 
    Description:
        API to return current power status of the system. The value 
        returned by this API would be 5th field of Status Data Block. 
    Conditions:
        None.
    Input:
        u8PortNum - Port number.
    Return:
        UINT8 - Power Status 
    Remarks:
        None. 
**************************************************************************************************/
UINT8 DPM_ReturnPowerStatus (UINT8 u8PortNum); 

/**************************************************************************************************
    Function:
        UINT8 DPM_ReturnTemperatureStatus(); 
    Summary:
        Returns Current Temperature Status of the system based on currently
        active throttling bank. 
    Description:
        API to return current temperature status of the system. The value 
        returned by this API would be 4th field of Status Data Block. 
    Conditions:
        None.
    Input:
        None.
    Return:
        UINT8 - Temperature Status 
    Remarks:
        None. 
**************************************************************************************************/
UINT8 DPM_ReturnTemperatureStatus (void); 

/**************************************************************************************************
    Function:
        void DPM_EnablePort(UINT8 u8PortNum, UINT8 u8Enable); 
    Summary:
        API to enable/disable a port 
    Description:
        This API can be called to disable or enable a particular port.
    Conditions:
        None.
    Input:
        u8PortNum - Port number.
        u8Enable - TRUE - Enable the port 
                   FALSE - Disable the port 
    Return:
        None.
    Remarks:
        None. 
**************************************************************************************************/
void DPM_EnablePort(UINT8 u8PortNum, UINT8 u8Enable); 

/**************************************************************************************************
    Function:
        UINT8 DPM_IsAPDOAdvertised(UINT8 u8PortNum);  
    Summary:
        API to know if at least one APDO is advertised.
    Description:
        This API can be called to know if the advertised Source Capabilities 
        contain a PPS APDO.
    Conditions:
        None.
    Input:
        u8PortNum - Port number.
    Return:
        UINT8  - TRUE - APDO is advertised
                 FALSE - No APDO is advertised
    Remarks:
        None. 
**************************************************************************************************/
UINT8 DPM_IsAPDOAdvertised(UINT8 u8PortNum); 

/**************************************************************************************************
    Function:
        void DPM_OnTypeCDetach(UINT8 u8PortNum); 
    Summary:
        API to clear the variables that are applicable during a type c detach event. 
    Description:
        This API clears all the applicable variables whose data is no more valid
        once a Type C Detach event has occurred. 
    Conditions:
        None.
    Input:
        u8PortNum - Port number.
    Return:
        None.
    Remarks:
        None. 
**************************************************************************************************/
void DPM_OnTypeCDetach(UINT8 u8PortNum);

/**************************************************************************************************
    Function:
        void DPM_OnPDNegotiationCmplt(UINT8 u8PortNum); 
    Summary:
        API to do necessary operation required by DPM during a PD negotiation complete. 
    Description:
        This API does the internal event registering on PD negotiation complete.
    Conditions:
        None.
    Input:
        u8PortNum - Port number.
    Return:
        None.
    Remarks:
        None. 
**************************************************************************************************/
void DPM_OnPDNegotiationCmplt(UINT8 u8PortNum);

/**************************************************************************************************
    Function:
        void DPM_SwapWait_TimerCB (UINT8 u8PortNum, UINT8 u8SwapInitiateType); 
    Summary:
        Timer callback for PE_PR_SWAP_WAIT_TIMEOUT_MS timeout
    Description:
        API to re-initiate PR_Swap on PE_PR_SWAP_WAIT_TIMEOUT_MS timeout
        in case the port partner has not sent a request within PE_PR_SWAP_WAIT_TIMEOUT_MS
        of reception of Wait message from the port partner.
    Conditions:
        None.
    Input:
        u8PortNum - Port number.
        u8SwapInitiateType - Type of Swap to be initiated on wait timeout
    Return:
        None.
    Remarks:
        None. 
**************************************************************************************************/
void DPM_SwapWait_TimerCB (UINT8 u8PortNum, UINT8 u8SwapInitiateType);

/**************************************************************************************************
    Function:
        UINT8 DPM_EvaluateRoleSwap (UINT8 u8PortNum, eRoleSwapMsgtype eRoleSwapMsg)
    Summary:
        API to evaluate VCONN_Swap, DR_Swap and PR_Swap received from partner or initiation. 
    Description:
        This API evaluates the Role swap message received from port partner 
        based on the policy bits defined by user configuration and returns 
        accept/reject status. It also evaluates whether any swap message can be
        initiated by PSF based on the user configuration.
    Conditions:
        None.
    Input:
        u8PortNum - Port number.
        eRoleSwapMsgtype - Role Swap message type 
    Return:
        UINT8 - DPM_ACCEPT_SWAP in case of accept
                DPM_REJECT_SWAP in case of reject.
                DPM_REQUEST_SWAP in case PSF has to initiate swap
                DPM_IGNORE_INITIATE_SWAP in case PSF should not initiate 
    Remarks:
        None. 
**************************************************************************************************/
UINT8 DPM_EvaluateRoleSwap (UINT8 u8PortNum, eRoleSwapMsgtype eRoleSwapMsg); 

#endif /*_POLICY_MANAGER_H_*/

