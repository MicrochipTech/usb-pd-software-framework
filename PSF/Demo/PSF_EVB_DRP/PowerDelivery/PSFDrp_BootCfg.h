/***************************************************************************
  Power Delivery DRP Configurable globals Header File

  Company:
    Microchip Technology Inc.

  File Name:
    PSFDrp_BootCfg.h

  Description:
    This header file contains user configurable globals and function prototypes.
************************************************************************** */
/*******************************************************************************
Copyright ©  [2020] Microchip Technology Inc. and its subsidiaries.

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

/*********************************Common Configuration*****************************************/
#define CFG_PRODUCT_ID              0x0350U
#define CFG_VENDOR_ID               0x0424U
#define CFG_PRODUCT_TYPE_VDO        0x0000U
#define CFG_PRODUCT_VDO             0x0000U  
#define CFG_CERT_STAT_VDO           0x0000U
#define CFG_ID_HEADER_VDO           0x0000U
#define CFG_HW_VERSION              0x00U
#define CFG_SILICON_VERSION         0x00U

/*Length of Manufacturer string should be within the range of 0 to 21.
 Default length of Manufacturer string is 20*/
#define CFG_MANUFACTURER_STR_LEN    20U

/*Number of characters in Manufacturer string should be less than or equal to
 the value in CFG_MANUFACTURER_STR_LEN. Default manufacturer string is 
 "Microchip Technology" */
#define CFG_MANUFACTURER_STR        "Microchip Technology"


/*******************Power Throttling configuration**********************/
/* Note: By default, PT is disabled for the system since support for 
   PPS + PT coexistence is planned for future phases. If PT needs to 
   be enabled, change the value of CFG_PT_ENABLE macro to 1U. */
#define CFG_PT_ENABLE                         0U 
/*Bank A - 0, Bank B - 1, Bank C - 2, Shutdown mode - 3*/
#define CFG_PT_POWER_THROTTLE_BANK_SEL  0U /*Bank A selected*/

/**************************Power Balancing configuration******************* */
/* Note: By default, PB is disabled for the system since support for 
   PPS + PB coexistence is planned for future phases. If PB needs to be 
   enabled, change the value of CFG_PB_ENABLE macro to 0x10U and 
   CFG_PB_PORT_ENABLE to 0x01U. This will enable PB for the system 
   with FCFS as Selection Algorithm and for the ports. */
#define CFG_PB_ENABLE                         0x00U 
#define CFG_PB_PORT_ENABLE                    0x00U

/* Default Total System Power Bank A is 120W */
#define CFG_PB_TOT_SYS_POWER_BANKA_IN_250mW            0x01E0U 
/* Guaranteed Minimum Power for Bank A is 15W */
#define CFG_PB_MIN_POWER_BANKA_IN_250mW                0x003CU
/* Default Total System Power Bank B is 120W */
#define CFG_PB_TOT_SYS_POWER_BANKB_IN_250mW            0x01E0U
/* Guaranteed Minimum Power for Bank A is 15W */
#define CFG_PB_MIN_POWER_BANKB_IN_250mW                0x003CU
/* Default Total System Power Bank C is 120W */
#define CFG_PB_TOT_SYS_POWER_BANKC_IN_250mW            0x01E0U
/* Guaranteed Minimum Power for Bank A is 15W */
#define CFG_PB_MIN_POWER_BANKC_IN_250mW                0x003CU

/* Default Max Port Power for Bank A is 60W */
#define CFG_PB_MAX_PORT_0_POWER_BANKA_IN_250mW           0x00F0U
/* Default Max Port Power for Bank B is 60W */
#define CFG_PB_MAX_PORT_0_POWER_BANKB_IN_250mW           0x00F0U
/* Default Max Port Power for Bank C is 60W */
#define CFG_PB_MAX_PORT_0_POWER_BANKC_IN_250mW           0x00F0U

#define CFG_PB_MAX_PORT_1_POWER_BANKA_IN_250mW           0x00F0U
/* Default Max Port Power for Bank B is 60W */
#define CFG_PB_MAX_PORT_1_POWER_BANKB_IN_250mW           0x00F0U
/* Default Max Port Power for Bank C is 60W */
#define CFG_PB_MAX_PORT_1_POWER_BANKC_IN_250mW           0x00F0U

/***********************************Port Specific configuration***********************/    
/*Configuration value for u32CfgData*/
/*Power Role values 0 - Sink, 1 - Source, 2 - DRP*/
#define CFG_PORT_0_POWER_ROLE                2
#define CFG_PORT_1_POWER_ROLE                2

/*Data Role supported values 0-Dual Role Data Not supported, 1- Dual Role Date Supported */
#define CFG_PORT_0_DUAL_ROLE_DATA_SUPPORT    (1U << 2)
#define CFG_PORT_1_DUAL_ROLE_DATA_SUPPORT    (1U << 2)

/*Rp Value - 0 -Rd(Sink), 1-Default USB Rp, 2- 1.5A Rp, 3-3.0A Rp */
#define CFG_PORT_0_RP_CURRENT_VALUE          (3U << 3)
#define CFG_PORT_1_RP_CURRENT_VALUE          (3U << 3)

/*Port Enable - 0- Port Disable; 1- Port Enable*/
#define CFG_PORT_0_ENABLE       (1U << 5)
#define CFG_PORT_1_ENABLE       (1U << 5)

/*VCONN OCS Enable - 0-Disable, 1-Enable*/
#define CFG_PORT_0_VCONN_OCS_ENABLE     (1U << 9)
#define CFG_PORT_1_VCONN_OCS_ENABLE     (1U << 9)  

/********************************PIO Specific configuration******************************/
#define CFG_PORT_0_UPD_FAULT_IN_PIO_NO       eUPD_PIO5
#define CFG_PORT_1_UPD_FAULT_IN_PIO_NO       eUPD_PIO5

#define CFG_PORT_0_UPD_FAULT_IN_MODE         eFAULT_IN_ACTIVE_LOW
#define CFG_PORT_1_UPD_FAULT_IN_MODE         eFAULT_IN_ACTIVE_LOW

#define CFG_PORT_0_UPD_EN_VBUS               eUPD_PIO3
#define CFG_PORT_1_UPD_EN_VBUS               eUPD_PIO3
#define CFG_PORT_0_UPD_EN_VBUS_PIO_MODE      ePUSH_PULL_ACTIVE_HIGH
#define CFG_PORT_1_UPD_EN_VBUS_PIO_MODE      ePUSH_PULL_ACTIVE_HIGH

#define CFG_PORT_0_UPD_EN_SNK                eUPD_PIO6
#define CFG_PORT_1_UPD_EN_SNK                eUPD_PIO6
#define CFG_PORT_0_UPD_EN_SNK_PIO_MODE       ePUSH_PULL_ACTIVE_HIGH
#define CFG_PORT_1_UPD_EN_SNK_PIO_MODE       ePUSH_PULL_ACTIVE_HIGH

/**********************Fault Related configuration**************************/
#define CFG_OVER_VOLTAGE_FACTOR			  115U
#define CFG_UNDER_VOLTAGE_FACTOR		  85U
#define CFG_FAULT_IN_OCS_DEBOUNCE_MS      5U
#define CFG_VCONN_OCS_DEBOUNCE_IN_MS      2U
#define CFG_MAX_VBUS_POWER_FAULT_COUNT	  3U
#define CFG_MAX_VCONN_FAULT_COUNT		  3U
#define CFG_POWER_GOOD_TIMER_MS			  10000U
#define CFG_MAX_PORT_CURRENT_IN_10mA      300U 

/*********************************************************************************/
/*********************************PDO Configuration ******************************/
/**********************************************************************************/

/********************************************************************************/
/**********************************Port 0****************************************/
/*********************************************************************************/
/*Port 0 Notebook common PDO configuration for Source and Sink*/
#define CFG_PORT_0_DUAL_ROLE_POWER           1U 
#define CFG_PORT_0_DUAL_ROLE_DATA            1U

/*Port - 0 NoteBook Source PDO Configuration***/
#define CFG_PORT_0_SOURCE_NUM_OF_PDOS      1U
#define CFG_PORT_0_SOURCE_USB_SUSP           0U
#define CFG_PORT_0_SOURCE_UNCONSTARINED_PWR  1U
#define CFG_PORT_0_SOURCE_USB_COMM           0U
#define CFG_PORT_0_SOURCE_PDO_1     CFG_FORM_SOURCE_FIXED_PDO1(5000U, 3000U,\
                                    CFG_PORT_0_DUAL_ROLE_DATA, \
                                    CFG_PORT_0_SOURCE_USB_COMM, \
                                    CFG_PORT_0_SOURCE_USB_SUSP,  \
                                    CFG_PORT_0_SOURCE_UNCONSTARINED_PWR, \
                                    CFG_PORT_0_DUAL_ROLE_POWER);
#define CFG_PORT_0_SOURCE_PDO_2     CFG_FORM_FIXED_PDOx(0,0)    
#define CFG_PORT_0_SOURCE_PDO_3     CFG_FORM_FIXED_PDOx(0,0)   
#define CFG_PORT_0_SOURCE_PDO_4     CFG_FORM_FIXED_PDOx(0,0)
#define CFG_PORT_0_SOURCE_PDO_5     CFG_FORM_FIXED_PDOx(0,0)   
#define CFG_PORT_0_SOURCE_PDO_6     CFG_FORM_FIXED_PDOx(0,0)
#define CFG_PORT_0_SOURCE_PDO_7     CFG_FORM_FIXED_PDOx(0,0)   

/*Port - 0 Note Book Sink PDO configuration*/
#define CFG_PORT_0_SINK_NUM_OF_PDOS          4U
#define CFG_PORT_0_SINK_HIGHER_CAPABILITY    1U
#define CFG_PORT_0_SINK_UNCONSTRAINED_PWR    1U 
#define CFG_PORT_0_SINK_USB_COMM             0U
#define CFG_PORT_0_SINK_PDO_1_VOLTAGE        5000U
#define CFG_PORT_0_SINK_PDO_1_CURRENT        3000U 
#define CFG_PORT_0_SINK_PDO_1     CFG_FORM_SINK_FIXED_PDO1(5000U, 3000U, \
                                    CFG_PORT_0_DUAL_ROLE_DATA, \
                                    CFG_PORT_0_SINK_USB_COMM, \
                                    CFG_PORT_0_SINK_UNCONSTRAINED_PWR, \
                                    CFG_PORT_0_SINK_HIGHER_CAPABILITY, \
                                    CFG_PORT_0_DUAL_ROLE_POWER);
#define CFG_PORT_0_SINK_PDO_2     CFG_FORM_FIXED_PDOx(9000U,3000U)    
#define CFG_PORT_0_SINK_PDO_3     CFG_FORM_FIXED_PDOx(15000u,3000U)   
#define CFG_PORT_0_SINK_PDO_4     CFG_FORM_FIXED_PDOx(20000,3000U)
#define CFG_PORT_0_SINK_PDO_5     CFG_FORM_FIXED_PDOx(0,0)   
#define CFG_PORT_0_SINK_PDO_6     CFG_FORM_FIXED_PDOx(0,0)
#define CFG_PORT_0_SINK_PDO_7     CFG_FORM_FIXED_PDOx(0,0) 


/********************************************************************************/
/**********************************Port 1****************************************/
/*********************************************************************************/
/*Port 1 Dock common PDO configuration for Source and Sink*/
#define CFG_PORT_1_DUAL_ROLE_POWER           1U 
#define CFG_PORT_1_DUAL_ROLE_DATA            1U

/*Port - 1 Dock Source PDO Configuration***/
#if (TRUE == INCLUDE_PD_SOURCE_PPS)
#define CFG_PORT_1_SOURCE_NUM_OF_PDOS      5U
#else 
#define CFG_PORT_1_SOURCE_NUM_OF_PDOS      4U
#endif 
#define CFG_PORT_1_SOURCE_USB_SUSP           0U
#define CFG_PORT_1_SOURCE_UNCONSTARINED_PWR  1U
#define CFG_PORT_1_SOURCE_USB_COMM           0U
/* Source APDO defines */
#define CFG_PORT_1_SOURCE_APDO_5_MIN_VOLTAGE    3300U
#define CFG_PORT_1_SOURCE_APDO_5_MAX_VOLTAGE    21000U
#define CFG_PORT_1_SOURCE_APDO_5_MAX_CURRENT    3000U
#define CFG_PORT_1_SOURCE_APDO_5_PPS_PWR_LTD    0U

#define CFG_PORT_1_SOURCE_PDO_1     CFG_FORM_SOURCE_FIXED_PDO1(5000U, 3000U, \
                                    CFG_PORT_1_DUAL_ROLE_DATA, \
                                    CFG_PORT_1_SOURCE_USB_COMM, \
                                    CFG_PORT_1_SOURCE_USB_SUSP,  \
                                    CFG_PORT_1_SOURCE_UNCONSTARINED_PWR, \
                                    CFG_PORT_1_DUAL_ROLE_POWER);
#define CFG_PORT_1_SOURCE_PDO_2     CFG_FORM_FIXED_PDOx(9000U,3000U)    
#define CFG_PORT_1_SOURCE_PDO_3     CFG_FORM_FIXED_PDOx(15000U,3000U)   
#define CFG_PORT_1_SOURCE_PDO_4     CFG_FORM_FIXED_PDOx(20000U,3000U)
#if (TRUE == INCLUDE_PD_SOURCE_PPS)
#define CFG_PORT_1_SOURCE_PDO_5     CFG_FORM_PPS_APDO(CFG_PORT_1_SOURCE_APDO_5_MIN_VOLTAGE, \
                                                    CFG_PORT_1_SOURCE_APDO_5_MAX_VOLTAGE, \
                                                    CFG_PORT_1_SOURCE_APDO_5_MAX_CURRENT, \
                                                    CFG_PORT_1_SOURCE_APDO_5_PPS_PWR_LTD);            
#else 
#define CFG_PORT_1_SOURCE_PDO_5     CFG_FORM_FIXED_PDOx(0,0)   
#endif 
#define CFG_PORT_1_SOURCE_PDO_6     CFG_FORM_FIXED_PDOx(0,0)
#define CFG_PORT_1_SOURCE_PDO_7     CFG_FORM_FIXED_PDOx(0,0)

/*Port - 1 Dock Sink PDO configuration*/
#define CFG_PORT_1_SINK_NUM_OF_PDOS          1U
#define CFG_PORT_1_SINK_HIGHER_CAPABILITY    1U
#define CFG_PORT_1_SINK_UNCONSTRAINED_PWR    1U 
#define CFG_PORT_1_SINK_USB_COMM             0U

#define CFG_PORT_1_SINK_PDO_1     CFG_FORM_SINK_FIXED_PDO1(5000, 0,\
                                    CFG_PORT_1_DUAL_ROLE_DATA, \
                                    CFG_PORT_1_SINK_USB_COMM, \
                                    CFG_PORT_1_SINK_UNCONSTRAINED_PWR, \
                                    CFG_PORT_1_SINK_HIGHER_CAPABILITY, \
                                    CFG_PORT_1_DUAL_ROLE_POWER); 
#define CFG_PORT_1_SINK_PDO_2     CFG_FORM_FIXED_PDOx(0,0)    
#define CFG_PORT_1_SINK_PDO_3     CFG_FORM_FIXED_PDOx(0,0)   
#define CFG_PORT_1_SINK_PDO_4     CFG_FORM_FIXED_PDOx(0,0)
#define CFG_PORT_1_SINK_PDO_5     CFG_FORM_FIXED_PDOx(0,0)   
#define CFG_PORT_1_SINK_PDO_6     CFG_FORM_FIXED_PDOx(0,0)
#define CFG_PORT_1_SINK_PDO_7     CFG_FORM_FIXED_PDOx(0,0) 


/********************************Sink Specific Configuration********************************/
/*u8SinkConfigSel configuration*/
/*Sink Mode A - 0x00, Mode B - 0x01*/
#define CFG_PORT_0_SINK_MODE            0x00
#define CFG_PORT_1_SINK_MODE            0x00
/*Sink Give Back Flag - 0 - Disabled, 1 - Enabled*/
#define CFG_PORT_0_SINK_GIVE_BACK_FLAG      (0U << 2)
#define CFG_PORT_1_SINK_GIVE_BACK_FLAG      (0U << 2)
/*Sink USB Suspend support - 0- Disabled, 1-Enabled*/
#define CFG_PORT_0_SINK_USB_SUSP            (0U << 3)
#define CFG_PORT_1_SINK_USB_SUSP            (0U << 3)

#define CFG_PORT_0_SINK_MIN_OPERATING_CURRENT_InmA             1000U
#define CFG_PORT_0_SINK_MAX_OPERATING_CURRENT_InmA             3000U
#define CFG_PORT_1_SINK_MIN_OPERATING_CURRENT_InmA             0U
#define CFG_PORT_1_SINK_MAX_OPERATING_CURRENT_InmA             0U

#define CFG_PORT_0_SINK_PDO_1_PREFERRED_MIN_CURRENT 2000U
#define CFG_PORT_0_SINK_PDO_2_PREFERRED_MIN_CURRENT 2000U
#define CFG_PORT_0_SINK_PDO_3_PREFERRED_MIN_CURRENT 2000U
#define CFG_PORT_0_SINK_PDO_4_PREFERRED_MIN_CURRENT 2000U
#define CFG_PORT_0_SINK_PDO_5_PREFERRED_MIN_CURRENT 0U
#define CFG_PORT_0_SINK_PDO_6_PREFERRED_MIN_CURRENT 0U
#define CFG_PORT_0_SINK_PDO_7_PREFERRED_MIN_CURRENT 0U

#define CFG_PORT_1_SINK_PDO_1_PREFERRED_MIN_CURRENT 0U
#define CFG_PORT_1_SINK_PDO_2_PREFERRED_MIN_CURRENT 0U
#define CFG_PORT_1_SINK_PDO_3_PREFERRED_MIN_CURRENT 0U
#define CFG_PORT_1_SINK_PDO_4_PREFERRED_MIN_CURRENT 0U
#define CFG_PORT_1_SINK_PDO_5_PREFERRED_MIN_CURRENT 0U
#define CFG_PORT_1_SINK_PDO_6_PREFERRED_MIN_CURRENT 0U
#define CFG_PORT_1_SINK_PDO_7_PREFERRED_MIN_CURRENT 0U

/*********************DAC_I defines******************/
#define CFG_PORT_0_SINK_DAC_I_MAX_OP_VOLTAGE             2500U
#define CFG_PORT_0_SINK_DAC_I_MIN_OP_VOLTAGE             0U
#define CFG_PORT_0_SINK_DAC_I_CUR_INDICATION_MAX         5000U
/*Configuration value for u8DAC_I_Direction*/
/*0 - High amperage = Max Voltage, 1- High amperage = Min Voltage*/
#define CFG_PORT_0_SINK_DAC_I_DIR      0U

/**************************Role Swap Policy defines*****************************/
#define CFG_PORT_0_AS_DFP_REQUEST_DR_SWAP             0 
#define CFG_PORT_0_AS_UFP_REQUEST_DR_SWAP             1
#define CFG_PORT_0_AS_DFP_ACCEPT_DR_SWAP              0 
#define CFG_PORT_0_AS_UFP_ACCEPT_DR_SWAP              1 
#define CFG_PORT_0_AS_SRC_REQUEST_PR_SWAP             1
#define CFG_PORT_0_AS_SNK_REQUEST_PR_SWAP             0
#define CFG_PORT_0_AS_SRC_ACCEPT_PR_SWAP              1
#define CFG_PORT_0_AS_SNK_ACCEPT_PR_SWAP              0
#define CFG_PORT_0_VCONN_SWAP_REQ_AS_VCONN_SRC        0
#define CFG_PORT_0_VCONN_SWAP_REQ_NOT_AS_VCONN_SRC    1
#define CFG_PORT_0_VCONN_SWAP_ACCEPT_AS_VCONN_SRC     1
#define CFG_PORT_0_VCONN_SWAP_ACCEPT_NOT_AS_VCONN_SRC 1


#define CFG_PORT_1_AS_DFP_REQUEST_DR_SWAP             1
#define CFG_PORT_1_AS_UFP_REQUEST_DR_SWAP             0
#define CFG_PORT_1_AS_DFP_ACCEPT_DR_SWAP              1
#define CFG_PORT_1_AS_UFP_ACCEPT_DR_SWAP              0
#define CFG_PORT_1_AS_SRC_REQUEST_PR_SWAP             0
#define CFG_PORT_1_AS_SNK_REQUEST_PR_SWAP             1
#define CFG_PORT_1_AS_SRC_ACCEPT_PR_SWAP              0
#define CFG_PORT_1_AS_SNK_ACCEPT_PR_SWAP              1
#define CFG_PORT_1_VCONN_SWAP_REQ_AS_VCONN_SRC        0
#define CFG_PORT_1_VCONN_SWAP_REQ_NOT_AS_VCONN_SRC    0
#define CFG_PORT_1_VCONN_SWAP_ACCEPT_AS_VCONN_SRC     1
#define CFG_PORT_1_VCONN_SWAP_ACCEPT_NOT_AS_VCONN_SRC 1

/*****************Defines to form PDOs ****************************************/
/* Macro used to form Fixed PDO 1 */
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
#define CFG_FORM_SOURCE_FIXED_PDO1(voltage,current,DualRoleData,usbCommn,usbSusp,unconstrainedPwr,isDrp) \
    (((isDrp) << 29) | \
    ((usbSusp) << 28) | \
    ((unconstrainedPwr) << 27) | \
    ((usbCommn) << 26) | \
    ((DualRoleData) << 25) |\
    (((voltage)/50) << 10) | \
    ((current)/10))            

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
/* Macro used to form Sink Fixed PDO 1 */
#define CFG_FORM_SINK_FIXED_PDO1(voltage,current,DualRoleData,usbCommn,unconstrainedPwr,HigherCapability,isDrp)  \
    (((isDrp) << 29) | \
    (((HigherCapability) << 28)) | \
    ((unconstrainedPwr) << 27) | \
    ((usbCommn) << 26) | \
    ((DualRoleData) << 25) |\
    (((voltage)/50) << 10) | \
    ((current)/10))  

/* Macro used to form Fixed PDOs 2 to 7 */
#define CFG_FORM_FIXED_PDOx(voltage,current)   ((((voltage)/50) << 10) | ((current)/10))

/*APDO Source
  B31-30    -11b Augmented Power Data Object
  B29-28    - 00b Programmable Power Supply
  B27       - PPS Power Limited
  B26-25    - Reserved
  B24-17    - Maximum voltage in 100mV increment 
  B16       - Reserved
  B15-8     - Minimum voltage in 100mV increment 
  B7        - Reserved
  B6-0      - Maximum current in 50mA increment */
/* Macro used to form PPS APDO */
#define CFG_FORM_PPS_APDO(minVolt,maxVolt,maxCurrent,pwrLtd)\
    (((0x3U) << 30U) |\
    ((pwrLtd) << 27U) | \
    (((maxVolt) / 100U) << 17U) | \
    (((minVolt) / 100U) << 8U) | \
    ((maxCurrent) / 50U)) 

/***********************Defines to form role swap policy************************/
#define CFG_PORT_0_ROLE_SWAP_POLICY  CFG_FORM_ROLE_SWAP_POLICY(CFG_PORT_0_AS_DFP_REQUEST_DR_SWAP, \
 CFG_PORT_0_AS_UFP_REQUEST_DR_SWAP, CFG_PORT_0_AS_DFP_ACCEPT_DR_SWAP, CFG_PORT_0_AS_UFP_ACCEPT_DR_SWAP,\
 CFG_PORT_0_AS_SRC_REQUEST_PR_SWAP, CFG_PORT_0_AS_SNK_REQUEST_PR_SWAP, CFG_PORT_0_AS_SRC_ACCEPT_PR_SWAP,\
 CFG_PORT_0_AS_SNK_ACCEPT_PR_SWAP, CFG_PORT_0_VCONN_SWAP_REQ_AS_VCONN_SRC, CFG_PORT_0_VCONN_SWAP_REQ_NOT_AS_VCONN_SRC,\
 CFG_PORT_0_VCONN_SWAP_ACCEPT_AS_VCONN_SRC, CFG_PORT_0_VCONN_SWAP_ACCEPT_NOT_AS_VCONN_SRC);

#define CFG_PORT_1_ROLE_SWAP_POLICY CFG_FORM_ROLE_SWAP_POLICY(CFG_PORT_1_AS_DFP_REQUEST_DR_SWAP, \
 CFG_PORT_1_AS_UFP_REQUEST_DR_SWAP, CFG_PORT_1_AS_DFP_ACCEPT_DR_SWAP, CFG_PORT_1_AS_UFP_ACCEPT_DR_SWAP,\
 CFG_PORT_1_AS_SRC_REQUEST_PR_SWAP, CFG_PORT_1_AS_SNK_REQUEST_PR_SWAP, CFG_PORT_1_AS_SRC_ACCEPT_PR_SWAP,\
 CFG_PORT_1_AS_SNK_ACCEPT_PR_SWAP, CFG_PORT_1_VCONN_SWAP_REQ_AS_VCONN_SRC, CFG_PORT_1_VCONN_SWAP_REQ_NOT_AS_VCONN_SRC,\
 CFG_PORT_1_VCONN_SWAP_ACCEPT_AS_VCONN_SRC, CFG_PORT_1_VCONN_SWAP_ACCEPT_NOT_AS_VCONN_SRC);

#define CFG_FORM_ROLE_SWAP_POLICY(AsDfp_Request_DRSwap, AsUfp_Request_DRSwap, \
 AsDfp_Accept_DRSwap, AsUfp_Accept_DRSwap, AsSrc_Request_PRSwap, AsSnk_Request_PRSwap,\
 AsSrc_Accept_PRSwap, AsSnk_Accept_PRSwap, AsVCONNSrc_Req_VCONNSwap, AsNotVCONNSrc_Req_VCONNSwap,\
 AsVCONNSrc_Accept_VCONNSwap, AsNotVCONNSrc_Accept_VCONNSwap) ( (AsDfp_Request_DRSwap << 0) | \
 (AsUfp_Request_DRSwap << 1) | (AsDfp_Accept_DRSwap << 2) | (AsUfp_Accept_DRSwap << 3) \
 | (AsSrc_Request_PRSwap << 4) | (AsSnk_Request_PRSwap << 5) | (AsSrc_Accept_PRSwap << 6) \
 | (AsSnk_Accept_PRSwap << 7) | (AsVCONNSrc_Req_VCONNSwap << 8) | (AsNotVCONNSrc_Req_VCONNSwap << 9)\
 | (AsVCONNSrc_Accept_VCONNSwap << 10) | (AsNotVCONNSrc_Accept_VCONNSwap << 11))


/*Defines for getting default power role configured to a port from 
 * gasCfgStatusData.sPerPortData[u8PortNum].u32CfgData variable*/
/*CFG_GET_CONFIGURED_POWER_ROLE(u8PortNum) will return one of the following values
	- 0 (Sink)
	- 1 (Source)
	- 2 (DRP) */
#define CFG_GET_CONFIGURED_POWER_ROLE(u8PortNum)  \
    (gasCfgStatusData.sPerPortData[u8PortNum].u32CfgData & (BIT(0)|BIT(1)))

void PSF_LoadConfig(GLOBAL_CFG_STATUS_DATA *pasCfgStatusData); 

#endif /* _PSFDRP_BOOTCFG_H */

/* *****************************************************************************
 End of File
 */
