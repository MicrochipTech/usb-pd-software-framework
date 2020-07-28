/***************************************************************************
  Power Delivery Source Pro Configurable globals Header File

  Company:
    Microchip Technology Inc.

  File Name:
    PSFDrp_BootCfg.h

  Description:
    This header file contains user configurable globals and function prototypes.
************************************************************************** */
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

#ifndef _PSFDRP_BOOTCFG_H    /* Guard against multiple inclusion */
#define _PSFDRP_BOOTCFG_H

#define STRUCT_MAJOR_VERSION    0x01U
#define STRUCT_MINOR_VERSION    0x00U
#define PRODUCT_ID              0x301CU
#define VENDOR_ID               0x0424U
#define PRODUCT_TYPE_VDO        0x0000U
#define PRODUCT_VDO             0x0000U  
#define CERT_STAT_VDO           0x0000U
#define ID_HEADER_VDO           0x0000U
#define HW_VERSION              0x00U
#define SILICON_VERSION         0x00U

/**************************Defines for power roles*****************************/
#define CFG_PORT_POWER_ROLE_SINK        0U
#define CFG_PORT_POWER_ROLE_SOURCE      1U
#define CFG_PORT_POWER_ROLE_DRP         2U

#define CFG_PORT_POWER_ROLE_0                CFG_PORT_POWER_ROLE_DRP
#define CFG_PORT_POWER_ROLE_1                CFG_PORT_POWER_ROLE_DRP
#define CFG_PORT_DUAL_ROLE_DATA_POS          2U
#define CFG_PORT_DUAL_ROLE_DATA_0            (1 << CFG_PORT_DUAL_ROLE_DATA_POS)
#define CFG_PORT_DUAL_ROLE_DATA_1            (1 << CFG_PORT_DUAL_ROLE_DATA_POS)
#define CFG_PORT_RPVAL_POS                   3U
#define CFG_PORT_ENDIS_POS                   5U
#define CFG_PORT_RP_CURRENT_VALUE_0          (3U << CFG_PORT_RPVAL_POS)
#define CFG_PORT_RP_CURRENT_VALUE_1          (3U << CFG_PORT_RPVAL_POS)
#define CFG_PORT_ENABLE                      (1U << CFG_PORT_ENDIS_POS)

/* DRP configuration default number of Source PDOs for Notebook port */
#define CFG_PORT_NOTE_SOURCE_NUM_OF_PDOS      1U
/* DRP configuration default number of Source PDOs for Dock port */
#if (TRUE == INCLUDE_PD_SOURCE_PPS)
#define CFG_PORT_DOCK_SOURCE_NUM_OF_PDOS      5U
#else 
#define CFG_PORT_DOCK_SOURCE_NUM_OF_PDOS      4U
#endif 

#define CFG_PORT_SOURCE_USB_SUSP           0U
#define CFG_PORT_SOURCE_UNCONSTARINED_PWR  1U
#define CFG_PORT_SOURCE_USB_COMM           0U
#define CFG_PORT_DUAL_ROLE_POWER           1U 
#define CFG_PORT_DUAL_ROLE_DATA            0U

/***************Source PDO defines for Dock type port (port 1)*****************/
#define CFG_PORT_DOCK_SOURCE_PDO_1_CURRENT         3000U 
#define CFG_PORT_DOCK_SOURCE_PDO_2_CURRENT         3000U
#define CFG_PORT_DOCK_SOURCE_PDO_3_CURRENT         3000U
#define CFG_PORT_DOCK_SOURCE_PDO_4_CURRENT         3000U

#define CFG_PORT_DOCK_SOURCE_PDO_1_VOLTAGE         5000U
#define CFG_PORT_DOCK_SOURCE_PDO_2_VOLTAGE         9000U
#define CFG_PORT_DOCK_SOURCE_PDO_3_VOLTAGE         15000U
#define CFG_PORT_DOCK_SOURCE_PDO_4_VOLTAGE         20000U
/******************************************************************************/

/***************Sink PDO defines for Dock type port (port 1)*******************/
#define CFG_PORT_DOCK_SINK_NUM_OF_PDOS          1U

#define CFG_PORT_DOCK_SINK_PDO_1_CURRENT        0U 
#define CFG_PORT_DOCK_SINK_PDO_2_CURRENT        0U
#define CFG_PORT_DOCK_SINK_PDO_3_CURRENT        0U
#define CFG_PORT_DOCK_SINK_PDO_4_CURRENT        0U

#define CFG_PORT_DOCK_SINK_PDO_1_VOLTAGE        5000U
#define CFG_PORT_DOCK_SINK_PDO_2_VOLTAGE        0U
#define CFG_PORT_DOCK_SINK_PDO_3_VOLTAGE        0U
#define CFG_PORT_DOCK_SINK_PDO_4_VOLTAGE        0U
/******************************************************************************/

/************Source PDO defines for Note book type port (port 0)***************/
#define CFG_PORT_NOTE_SOURCE_PDO_1_CURRENT         3000U 
#define CFG_PORT_NOTE_SOURCE_PDO_2_CURRENT         0U
#define CFG_PORT_NOTE_SOURCE_PDO_3_CURRENT         0U
#define CFG_PORT_NOTE_SOURCE_PDO_4_CURRENT         0U

#define CFG_PORT_NOTE_SOURCE_PDO_1_VOLTAGE         5000U
#define CFG_PORT_NOTE_SOURCE_PDO_2_VOLTAGE         0U
#define CFG_PORT_NOTE_SOURCE_PDO_3_VOLTAGE         0U
#define CFG_PORT_NOTE_SOURCE_PDO_4_VOLTAGE         0U
/******************************************************************************/

/************Sink PDO defines for Note book type port (port 0)*****************/
#define CFG_PORT_NOTE_SINK_NUM_OF_PDOS          4U

#define CFG_PORT_NOTE_SINK_PDO_1_CURRENT        3000U 
#define CFG_PORT_NOTE_SINK_PDO_2_CURRENT        3000U
#define CFG_PORT_NOTE_SINK_PDO_3_CURRENT        3000U
#define CFG_PORT_NOTE_SINK_PDO_4_CURRENT        3000U

#define CFG_PORT_NOTE_SINK_PDO_1_VOLTAGE        5000U
#define CFG_PORT_NOTE_SINK_PDO_2_VOLTAGE        9000U
#define CFG_PORT_NOTE_SINK_PDO_3_VOLTAGE        15000U
#define CFG_PORT_NOTE_SINK_PDO_4_VOLTAGE        20000U
/******************************************************************************/

#define CFG_PORT_SINK_HIGHER_CAPABILITY    1U
#define CFG_PORT_SINK_UNCONSTRAINED_PWR    1U 
#define CFG_PORT_SINK_USB_COMM             0U

#define CFG_PORT_SINK_PDO_1_PREFERRED_MIN_CURRENT 2000U
#define CFG_PORT_SINK_PDO_2_PREFERRED_MIN_CURRENT 2000U
#define CFG_PORT_SINK_PDO_3_PREFERRED_MIN_CURRENT 2000U
#define CFG_PORT_SINK_PDO_4_PREFERRED_MIN_CURRENT 2000U
#define CFG_PORT_SINK_PDO_5_PREFERRED_MIN_CURRENT 0U
#define CFG_PORT_SINK_PDO_6_PREFERRED_MIN_CURRENT 0U
#define CFG_PORT_SINK_PDO_7_PREFERRED_MIN_CURRENT 0U

/* Source APDO defines */
#define CFG_PORT_SOURCE_APDO_5_MIN_VOLTAGE    3300U
#define CFG_PORT_SOURCE_APDO_5_MAX_VOLTAGE    21000U
#define CFG_PORT_SOURCE_APDO_5_MAX_CURRENT    3000U
#define CFG_PORT_SOURCE_APDO_5_PPS_PWR_LTD    0U

#define CFG_MAX_PDO_COUNT                 7U
#define CFG_OVER_VOLTAGE_FACTOR			  115U
#define CFG_UNDER_VOLTAGE_FACTOR		  85U
#define CFG_MAX_VBUS_POWER_FAULT_COUNT	  3U
#define CFG_MAX_VCONN_FAULT_COUNT		  3U
#define CFG_POWER_GOOD_TIMER_MS			  10000U
#define CFG_PORT_SINK_MODE_A              0x00
#define CFG_PORT_SINK_MODE_B              0x01
#define CFG_PORT_SINK_GIVE_BACK_FLAG      0U
#define CFG_PORT_SINK_USB_SUSP            0U


/*********************DAC_I defines******************/
#define CFG_PORT_SINK_DAC_I_MAX_OP_VOLTAGE             2500U
#define CFG_PORT_SINK_DAC_I_MIN_OP_VOLTAGE             0U
#define CFG_PORT_SINK_DAC_I_CUR_INDICATION_MAX         5000U
#define CFG_PORT_SINK_DAC_I_DIR_HIGH_AMP_MAX_VOLT      0U
#define CFG_PORT_SINK_DAC_I_DIR_HIGH_AMP_MIN_VOLT      1U

/*****************Sink Hardware defines**************/
#define CFG_PORT_NOTE_SINK_MIN_OPERATING_CURRENT_InmA             1000U
#define CFG_PORT_NOTE_SINK_MAX_OPERATING_CURRENT_InmA             3000U
#define CFG_PORT_DOCK_SINK_MIN_OPERATING_CURRENT_InmA             0U
#define CFG_PORT_DOCK_SINK_MAX_OPERATING_CURRENT_InmA             0U

/**************************Role Swap Policy defines*****************************/
#define CFG_PORT_AS_DFP_REQUEST_DR_SWAP_POS         0
#define CFG_PORT_AS_UFP_REQUEST_DR_SWAP_POS         1
#define CFG_PORT_AS_DFP_ACCEPT_DR_SWAP_POS          2
#define CFG_PORT_AS_UFP_ACCEPT_DR_SWAP_POS          3
#define CFG_PORT_AS_SRC_REQUEST_PR_SWAP_POS         4
#define CFG_PORT_AS_SNK_REQUEST_PR_SWAP_POS         5
#define CFG_PORT_AS_SRC_ACCEPT_PR_SWAP_POS          6
#define CFG_PORT_AS_SNK_ACCEPT_PR_SWAP_POS          7

#define CFG_PORT_NOTE_AS_DFP_REQUEST_DR_SWAP  (0 << CFG_PORT_AS_DFP_REQUEST_DR_SWAP_POS)
#define CFG_PORT_NOTE_AS_UFP_REQUEST_DR_SWAP  (1 << CFG_PORT_AS_UFP_REQUEST_DR_SWAP_POS)
#define CFG_PORT_NOTE_AS_DFP_ACCEPT_DR_SWAP   (0 << CFG_PORT_AS_DFP_ACCEPT_DR_SWAP_POS)
#define CFG_PORT_NOTE_AS_UFP_ACCEPT_DR_SWAP   (1 << CFG_PORT_AS_UFP_ACCEPT_DR_SWAP_POS)
#define CFG_PORT_NOTE_AS_SRC_REQUEST_PR_SWAP  (1 << CFG_PORT_AS_SRC_REQUEST_PR_SWAP_POS)
#define CFG_PORT_NOTE_AS_SNK_REQUEST_PR_SWAP  (0 << CFG_PORT_AS_SNK_REQUEST_PR_SWAP_POS)
#define CFG_PORT_NOTE_AS_SRC_ACCEPT_PR_SWAP   (1 << CFG_PORT_AS_SRC_ACCEPT_PR_SWAP_POS)
#define CFG_PORT_NOTE_AS_SNK_ACCEPT_PR_SWAP   (0 << CFG_PORT_AS_SNK_ACCEPT_PR_SWAP_POS)

#define CFG_PORT_DOCK_AS_DFP_REQUEST_DR_SWAP  (1 << CFG_PORT_AS_DFP_REQUEST_DR_SWAP_POS)
#define CFG_PORT_DOCK_AS_UFP_REQUEST_DR_SWAP  (0 << CFG_PORT_AS_UFP_REQUEST_DR_SWAP_POS)
#define CFG_PORT_DOCK_AS_DFP_ACCEPT_DR_SWAP   (1 << CFG_PORT_AS_DFP_ACCEPT_DR_SWAP_POS)
#define CFG_PORT_DOCK_AS_UFP_ACCEPT_DR_SWAP   (0 << CFG_PORT_AS_UFP_ACCEPT_DR_SWAP_POS)
#define CFG_PORT_DOCK_AS_SRC_REQUEST_PR_SWAP  (0 << CFG_PORT_AS_SRC_REQUEST_PR_SWAP_POS)
#define CFG_PORT_DOCK_AS_SNK_REQUEST_PR_SWAP  (1 << CFG_PORT_AS_SNK_REQUEST_PR_SWAP_POS)
#define CFG_PORT_DOCK_AS_SRC_ACCEPT_PR_SWAP   (0 << CFG_PORT_AS_SRC_ACCEPT_PR_SWAP_POS)
#define CFG_PORT_DOCK_AS_SNK_ACCEPT_PR_SWAP   (1 << CFG_PORT_AS_SNK_ACCEPT_PR_SWAP_POS)
/*******************************************************************************/

#define CFG_PORT_UPD_EN_VBUS               eUPD_PIO3
#define CFG_PORT_UPD_EN_VBUS_PIO_MODE      ePUSH_PULL_ACTIVE_HIGH
#define CFG_PORT_UPD_FAULT_IN_PIO_NO       eUPD_PIO5
#define CFG_PORT_UPD_FAULT_IN_MODE         eFAULT_IN_ACTIVE_LOW
#define CFG_PORT_UPD_EN_SNK                eUPD_PIO6
#define CFG_PORT_UPD_EN_SNK_PIO_MODE       ePUSH_PULL_ACTIVE_HIGH

#define CFG_VCONN_OCS_EN_POS               9U
#define CFG_VCONN_OCS_ENABLE               (1 << CFG_VCONN_OCS_EN_POS)
#define CFG_VCONN_OCS_DEBOUNCE_IN_MS       2U
#define CFG_FAULT_IN_OCS_DEBOUNCE_MS       5U

#define CFG_MAX_PORT_CURRENT_IN_10mA       300U 

/* Power Throttling Bank values */
#define CFG_PD_THROTTLE_BANK_A             0U 
#define CFG_PD_THROTTLE_BANK_B             1U
#define CFG_PD_THROTTLE_BANK_C             2U
#define CFG_PD_THROTTLE_SHUTDOWN_MODE      3U

/* Default Total System Power Bank A is 120W */
#define CFG_PB_TOT_SYS_POWER_BANKA            0x01E0U
/* Guaranteed Minimum Power for Bank A is 15W */
#define CFG_PB_MIN_POWER_BANKA                0x003CU
/* Default Total System Power Bank B is 120W */
#define CFG_PB_TOT_SYS_POWER_BANKB            0x01E0U
/* Guaranteed Minimum Power for Bank A is 15W */
#define CFG_PB_MIN_POWER_BANKB                0x003CU
/* Default Total System Power Bank C is 120W */
#define CFG_PB_TOT_SYS_POWER_BANKC            0x01E0U
/* Guaranteed Minimum Power for Bank A is 15W */
#define CFG_PB_MIN_POWER_BANKC                0x003CU

/* Default Max Port Power for Bank A is 60W */
#define CFG_PB_MAX_PORT_POWER_BANKA           0x00F0U
/* Default Max Port Power for Bank B is 60W */
#define CFG_PB_MAX_PORT_POWER_BANKB           0x00F0U
/* Default Max Port Power for Bank C is 60W */
#define CFG_PB_MAX_PORT_POWER_BANKC           0x00F0U
/* Default Max Port current is 3A */
#define CFG_PB_MAX_PORT_CURRENT               0x012CU

/* Note: By default, PB is disabled for the system since support for 
   PPS + PB coexistence is planned for future phases. If PB needs to be 
   enabled, change the value of CFG_PB_ENABLE macro to 0x10U and 
   CFG_PB_PORT_ENABLE to 0x01U. This will enable PB for the system 
   with FCFS as Selection Algorithm and for the ports. */
#define CFG_PB_ENABLE                         0x00U 
#define CFG_PB_PORT_ENABLE                    0x00U

/* Note: By default, PT is disabled for the system since support for 
   PPS + PT coexistence is planned for future phases. If PT needs to 
   be enabled, change the value of CFG_PT_ENABLE macro to 1U. */
#define CFG_PT_ENABLE                         0U 

/*Source PDO 
  B31-B30   - Supply Type
  B29       - Dual Power role
  B28       - USB Suspend supported
  B27       - UnConstrained Power
  B26       - USB Communication capable
  B25       - Dual-Role Data
  B24       - Unchunked extended message
  B23-B22   - Reserved
  B21-B20   - Peak Current
  B19-B10   - Voltage in 50mV units
  B9-B0     - Maximum current in 10mA units */
/*Sink PDO 
  B31-B30   - Supply Type
  B29       - Dual Power role
  B28       - Higher Capability
  B27       - UnConstrained Power
  B26       - USB Communication capable
  B25       - Dual-Role Data
  B24-B23   - Fast Role Swap required USB Type-C current
  B22-B20   - Reserved
  B19-B10   - Voltage in 50mV units
  B9-B0     - Maximum current in 10mA units */
#define CFG_PDO_VOLTAGE_POS                   10U 
#define CFG_PDO_VOLTAGE_UNIT                  50U
#define CFG_PDO_CURRENT_UNIT                  10U
#define CFG_PDO_DUAL_ROLE_DATA_POS            25U
#define CFG_PDO_USB_COMMN_POS                 26U
#define CFG_PDO_UNCONSTRAINED_PWR             27U
#define CFG_PDO_USB_SUSPEND_POS               28U
#define CFG_PDO_DUAL_ROLE_PWR_POS             29U

#define CFG_PDO_HIGHER_CAPABILITY_POS         28U

/* Power Supply type - Bits 31:10 of Power Data Object */
#define CFG_POWER_SUPPLY_TYPE_FIXED           0x00U
#define CFG_POWER_SUPPLY_TYPE_VARIABLE        0x01U
#define CFG_POWER_SUPPLY_TYPE_BATTERY         0x02U
#define CFG_POWER_SUPPLY_TYPE_PROGRAMMABLE    0x03U

/* Position of various APDO Fields */
#define CFG_APDO_MIN_VOLTAGE_POS               8U
#define CFG_APDO_MAX_VOLTAGE_POS               17U
#define CFG_APDO_MAX_CURRENT_POS               0U
#define CFG_APDO_PPS_PWR_LIMITED_POS           27U
#define CFG_APDO_PWR_SUPPLY_TYPE_POS           30U

/* Units of various APDO Fields */
#define CFG_APDO_MIN_VOLTAGE_UNIT              100U
#define CFG_APDO_MAX_VOLTAGE_UNIT              100U
#define CFG_APDO_MAX_CURRENT_UNIT              50U

/* Macro used to form PPS APDO */
#define CFG_FORM_PPS_APDO(pdoType,minVolt,maxVolt,maxCurrent,pwrLtd) (((pdoType) \
    << CFG_APDO_PWR_SUPPLY_TYPE_POS) | ((pwrLtd) << CFG_APDO_PPS_PWR_LIMITED_POS) | \
    (((maxVolt) / CFG_APDO_MAX_VOLTAGE_UNIT) << CFG_APDO_MAX_VOLTAGE_POS) | \
    (((minVolt) / CFG_APDO_MIN_VOLTAGE_UNIT) << CFG_APDO_MIN_VOLTAGE_POS) | \
    (((maxCurrent) / CFG_APDO_MAX_CURRENT_UNIT) << CFG_APDO_MAX_CURRENT_POS)) 

/* Macro used to form Fixed PDO 1 */

#define CFG_FORM_SOURCE_FIXED_PDO1(voltage,current,DualRoleData,usbCommn,usbSusp,unconstrainedPwr,isDrp) \
    (((usbSusp) << CFG_PDO_USB_SUSPEND_POS) | \
    ((isDrp) << CFG_PDO_DUAL_ROLE_PWR_POS) | \
    ((unconstrainedPwr) << CFG_PDO_UNCONSTRAINED_PWR) | \
    ((usbCommn) << CFG_PDO_USB_COMMN_POS) | \
    ((DualRoleData) << CFG_PDO_DUAL_ROLE_DATA_POS) |\
    (((voltage)/CFG_PDO_VOLTAGE_UNIT) << CFG_PDO_VOLTAGE_POS) | \
    ((current)/CFG_PDO_CURRENT_UNIT))            

/* Macro used to form Sink Fixed PDO 1 */
#define CFG_FORM_SINK_FIXED_PDO1(current,voltage,DualRoleData,usbCommn,unconstrainedPwr,HigherCapability,isDrp)  \
    (((isDrp) << CFG_PDO_DUAL_ROLE_PWR_POS) | \
    (((HigherCapability) << CFG_PDO_HIGHER_CAPABILITY_POS)) | \
    ((unconstrainedPwr) << CFG_PDO_UNCONSTRAINED_PWR) | \
    ((usbCommn) << CFG_PDO_USB_COMMN_POS) | \
    ((DualRoleData) << CFG_PDO_DUAL_ROLE_DATA_POS) |\
    (((voltage)/CFG_PDO_VOLTAGE_UNIT) << CFG_PDO_VOLTAGE_POS) | \
    ((current)/CFG_PDO_CURRENT_UNIT))  

/* Macro used to form Fixed PDOs 2 to 7 */
#define CFG_FORM_FIXED_PDOx(voltage,current)   \
    ((((voltage)/CFG_PDO_VOLTAGE_UNIT) << CFG_PDO_VOLTAGE_POS) | \
    ((current)/CFG_PDO_CURRENT_UNIT))

void PSF_LoadConfig(); 

#endif /* _PSFDRP_BOOTCFG_H */

/* *****************************************************************************
 End of File
 */
