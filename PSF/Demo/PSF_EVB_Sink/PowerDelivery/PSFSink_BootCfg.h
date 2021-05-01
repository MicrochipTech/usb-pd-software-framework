/***************************************************************************
  Power Delivery Sink Configurable globals Header File

  Company:
    Microchip Technology Inc.

  File Name:
    PSFSink_BootCfg.h

  Description:
    This header file contains user configurable globals and function prototypes.
************************************************************************** */
/*******************************************************************************
Copyright �  [2020] Microchip Technology Inc. and its subsidiaries.

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

#ifndef _PSFSINK_BOOTCFG_H    /* Guard against multiple inclusion */
#define _PSFSINK_BOOTCFG_H

/*********************************Common Configuration*****************************************/
#define CFG_PRODUCT_ID              0x0350U
#define CFG_VENDOR_ID               0x0424U
#define CFG_HW_VERSION              0x00U
#define CFG_SILICON_VERSION         0x00U

/*Length of Manufacturer string should be within the range of 0 to 21.
 Default length of Manufacturer string is 20*/
#define CFG_MANUFACTURER_STR_LEN    20U

/*Number of characters in Manufacturer string should be less than or equal to
 the value in CFG_MANUFACTURER_STR_LEN. Default manufacturer string is 
 "Microchip Technology" */
#define CFG_MANUFACTURER_STR        "Microchip Technology"


/***********************************Port Specific configuration***********************/    
/*Configuration value for u32CfgData*/
/*Power Role values 0 - Sink, 1- Source, 2- DRP*/
#define CFG_PORT_0_POWER_ROLE                0

/*Data Role supported values 0-Dual Role Data Not supported, 1- Dual Role Date Supported */
#define CFG_PORT_0_DUAL_ROLE_DATA_SUPPORT    (0U << 2)

/*Rp Value - 0 -Rd(Sink), 1-Default USB Rp, 2- 1.5A Rp, 3-3.0A Rp */
#define CFG_PORT_0_RP_CURRENT_VALUE          (3U << 3)

/*Port Enable - 0- Port Disable; 1- Port Enable*/
#define CFG_PORT_0_ENABLE       (1U << 5)

/*VCONN OCS Enable - 0- Disable, 1-Enable*/
#define CFG_PORT_0_VCONN_OCS_ENABLE     (1U << 9)

/********************************PIO Specific configuration******************************/
#define CFG_PORT_0_UPD_FAULT_IN_PIO_NO       eUPD_PIO5

#define CFG_PORT_0_UPD_FAULT_IN_MODE         eINPUT_ACTIVE_LOW

#define CFG_PORT_0_UPD_EN_SNK                eUPD_PIO6
#define CFG_PORT_0_UPD_EN_SNK_PIO_MODE       ePUSH_PULL_ACTIVE_HIGH

/* Note: UPD PIO9 is shared between HPD and VSEL2 pin functionalities. 
   Hence, INCLUDE_UPD_HPD is disabled by default. If user application
   wishes to use HPD feature, then VSEL2 functionality has to be mapped
   with any MCU pin since HPD pin can only be a UPD PIO */
#define CFG_PORT_0_HPD_IO                    eUPD_PIO9

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
#define CFG_PORT_0_DUAL_ROLE_POWER           0U 
#define CFG_PORT_0_DUAL_ROLE_DATA            0U

/*Port 0 PDO configuration for Sink*/
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

/********************************Sink Specific Configuration********************************/
/*u8SinkConfigSel configuration*/
/*Sink Mode A - 0x00, Mode B - 0x01*/
#define CFG_PORT_0_SINK_MODE                0x00
/*Sink Give Back Flag - 0 - Disabled, 1 - Enabled*/
#define CFG_PORT_0_SINK_GIVE_BACK_FLAG      (0U << 2)
/*Sink USB Suspend support - 0- Disabled, 1-Enabled*/
#define CFG_PORT_0_SINK_USB_SUSP            (0U << 3)

#define CFG_PORT_0_SINK_MIN_OPERATING_CURRENT_InmA             1000U
#define CFG_PORT_0_SINK_MAX_OPERATING_CURRENT_InmA             3000U

#define CFG_PORT_0_SINK_PDO_1_PREFERRED_MIN_CURRENT 2000U
#define CFG_PORT_0_SINK_PDO_2_PREFERRED_MIN_CURRENT 2000U
#define CFG_PORT_0_SINK_PDO_3_PREFERRED_MIN_CURRENT 2000U
#define CFG_PORT_0_SINK_PDO_4_PREFERRED_MIN_CURRENT 2000U
#define CFG_PORT_0_SINK_PDO_5_PREFERRED_MIN_CURRENT 0U
#define CFG_PORT_0_SINK_PDO_6_PREFERRED_MIN_CURRENT 0U
#define CFG_PORT_0_SINK_PDO_7_PREFERRED_MIN_CURRENT 0U


/*********************DAC_I defines******************/
#define CFG_PORT_0_SINK_DAC_I_MAX_OP_VOLTAGE             2500U
#define CFG_PORT_0_SINK_DAC_I_MIN_OP_VOLTAGE             0U
#define CFG_PORT_0_SINK_DAC_I_CUR_INDICATION_MAX         5000U
/*Configuration value for u8DAC_I_Direction*/
/*0 - High amperage = Max Voltage, 1- High amperage = Min Voltage*/
#define CFG_PORT_0_SINK_DAC_I_DIR      0U

/**************************Sink Caps Extd defines*****************************/
/*Sink Capabilities Extended Data Block (From Table 6-60 of USB PD Spec 3.0) 
  Offset  Field               Size 
    0     VID                 2
    2     PID                 2
    4     XID                 4
    8     FW Version          1 
    9     HW Version          1 
    10    SKEDB Version       1 
    11    Load Step           1 
    12    Sink Load           2
          Characteristics
    14    Compliance          1 
    15    Touch Temp          1
    16    Battery Info        1 
    17    Sink Modes          1 
    18    Sink Minimum PDP    1
    19    Sink Operational    1 
          PDP
    20    Sink Maximum PDP    1
  */
#define CFG_PORT_0_SINK_CAPS_EXTD_VID                       CFG_VENDOR_ID
#define CFG_PORT_0_SINK_CAPS_EXTD_PID                       CFG_PRODUCT_ID
#define CFG_PORT_0_SINK_CAPS_EXTD_XID                       0U
#define CFG_PORT_0_SINK_CAPS_EXTD_FW_VERSION                0U
#define CFG_PORT_0_SINK_CAPS_EXTD_HW_VERSION                CFG_HW_VERSION
#define CFG_PORT_0_SINK_CAPS_EXTD_SKEDB_VERSION             1U
#define CFG_PORT_0_SINK_CAPS_EXTD_LOAD_STEP                 0U
#define CFG_PORT_0_SINK_CAPS_EXTD_LOAD_CHARACTERISTICS      0U
#define CFG_PORT_0_SINK_CAPS_EXTD_COMPLIANCE                0U
#define CFG_PORT_0_SINK_CAPS_EXTD_TOUCH_TEMP                0U
#define CFG_PORT_0_SINK_CAPS_EXTD_BATTERY_INFO              0U
#define CFG_PORT_0_SINK_CAPS_EXTD_SINK_MODES                4U
#define CFG_PORT_0_SINK_CAPS_EXTD_MIN_PDP_In_WATT           5U
#define CFG_PORT_0_SINK_CAPS_EXTD_OPERATIONAL_PDP_In_WATT   60U
#define CFG_PORT_0_SINK_CAPS_EXTD_MAX_PDP_In_WATT           60U 

/**************************Role Swap Policy defines*****************************/
#define CFG_PORT_0_AS_DFP_REQUEST_DR_SWAP             0 
#define CFG_PORT_0_AS_UFP_REQUEST_DR_SWAP             0
#define CFG_PORT_0_AS_DFP_ACCEPT_DR_SWAP              0 
#define CFG_PORT_0_AS_UFP_ACCEPT_DR_SWAP              0 
#define CFG_PORT_0_AS_SRC_REQUEST_PR_SWAP             0
#define CFG_PORT_0_AS_SNK_REQUEST_PR_SWAP             0
#define CFG_PORT_0_AS_SRC_ACCEPT_PR_SWAP              0
#define CFG_PORT_0_AS_SNK_ACCEPT_PR_SWAP              0
#define CFG_PORT_0_AS_VCONN_SRC_REQ_VCONN_SWAP        0
#define CFG_PORT_0_AS_NOT_VCONN_SRC_REQ_VCONN_SWAP    0
#define CFG_PORT_0_AS_VCONN_SRC_ACCEPT_VCONN_SWAP     1
#define CFG_PORT_0_AS_NOT_VCONN_SRC_ACCEPT_VCONN_SWAP 1

/*********************** Feature Select Defines *************************************/
#define CFG_PORT_0_ENABLE_VDM_SUPPORT                 (0U << 1)

#define CFG_PORT_0_ENABLE_AME_MONITORING              (0U << 2)

/* Note: VDM defines are set to 0 by default. Users shall input necessary
   data relevant to their application. Refer section 6.4.4.3.1 of USB
   Power Delivery Specification v3.0 for detailed information on
   Discover Identity */
/******************* PD Identity Count Defines **************************/
#define CFG_PORT_0_PD_IDENTITY_COUNT                   0U

/**************** ID Header VDO Defines *********************************/
#define CFG_PORT_0_ID_HEADER_VDO_USB_HOST_CAPABLE         0U
#define CFG_PORT_0_ID_HEADER_VDO_USB_DEVICE_CAPABLE       0U
#define CFG_PORT_0_ID_HEADER_VDO_PROD_TYPE_UFP            0U
#define CFG_PORT_0_ID_HEADER_VDO_MODAL_OPR_SUPPORTED      0U
#define CFG_PORT_0_ID_HEADER_VDO_PROD_TYPE_DFP            0U
#define CFG_PORT_0_ID_HEADER_VDO_CONNECTOR_TYPE           0U
#define CFG_PORT_0_ID_HEADER_VDO_USB_VENDOR_ID            0U 

/**************** Cert Stat VDO Defines *********************************/
#define CFG_PORT_0_CERT_STAT_VDO_XID                  0U

/******************* Product VDO Defines *********************************/
#define CFG_PORT_0_PROD_VDO_USB_PRODUCT_ID            0U
#define CFG_PORT_0_PROD_VDO_BCD_DEVICE                0U 

/******************* Product Type VDO Defines ****************************/
                /*************** UFP VDO 1 *************/
#define CFG_PORT_0_UFP_VDO_1_UFP_VDO_VERSION          0U
#define CFG_PORT_0_UFP_VDO_1_DEVICE_CAPABILITY        0U
#define CFG_PORT_0_UFP_VDO_1_CONNECTOR_TYPE           0U
#define CFG_PORT_0_UFP_VDO_1_ALTERNATE_MODES          0U
#define CFG_PORT_0_UFP_VDO_1_USB_HIGHEST_SPEED        0U 

                /*************** UFP VDO 2 *************/
#define CFG_PORT_0_UFP_VDO_2_USB4_MIN_PWR             0U
#define CFG_PORT_0_UFP_VDO_2_USB4_MAX_PWR             0U
#define CFG_PORT_0_UFP_VDO_2_USB3_MIN_PWR             0U
#define CFG_PORT_0_UFP_VDO_2_USB3_MAX_PWR             0U

                /*************** DFP VDO ***************/
#define CFG_PORT_0_DFP_VDO_DFP_VDO_VERSION            0U
#define CFG_PORT_0_DFP_VDO_DFP_HOST_CAPABILITY        0U
#define CFG_PORT_0_DFP_VDO_DFP_CONNECTOR_TYPE         0U
#define CFG_PORT_0_DFP_VDO_DFP_PORT_NUMBER            0U

/*************************** SVID Defines ****************************/
                /*************** SVIDs ***************/
#define CFG_PORT_0_SVID_0                             0U
#define CFG_PORT_0_SVID_1                             0U

                /************* SVIDs Count ***********/
#define CFG_PORT_0_SVID_COUNT                         0U

/*************************** Mode Defines ****************************/
                /*************** Modes ***************/
#define CFG_PORT_0_SVID_0_MODE_0                      0U
#define CFG_PORT_0_SVID_0_MODE_1                      0U
#define CFG_PORT_0_SVID_1_MODE_0                      0U
#define CFG_PORT_0_SVID_1_MODE_1                      0U

                /*************** Modes Count ***************/
#define CFG_PORT_0_SVID_0_MODES_COUNT                 0U
#define CFG_PORT_0_SVID_1_MODES_COUNT                 0U

                /************ Start Mode Index **************/
#define CFG_PORT_0_SVID_0_START_MODE_INDEX            (0U << 3)
#define CFG_PORT_0_SVID_1_START_MODE_INDEX            (0U << 3)

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

/***********************Defines to form role swap policy************************/
#define CFG_PORT_0_ROLE_SWAP_POLICY  CFG_FORM_ROLE_SWAP_POLICY(CFG_PORT_0_AS_DFP_REQUEST_DR_SWAP, \
 CFG_PORT_0_AS_UFP_REQUEST_DR_SWAP, CFG_PORT_0_AS_DFP_ACCEPT_DR_SWAP, CFG_PORT_0_AS_UFP_ACCEPT_DR_SWAP,\
 CFG_PORT_0_AS_SRC_REQUEST_PR_SWAP, CFG_PORT_0_AS_SNK_REQUEST_PR_SWAP, CFG_PORT_0_AS_SRC_ACCEPT_PR_SWAP,\
 CFG_PORT_0_AS_SNK_ACCEPT_PR_SWAP, CFG_PORT_0_AS_VCONN_SRC_REQ_VCONN_SWAP, CFG_PORT_0_AS_NOT_VCONN_SRC_REQ_VCONN_SWAP,\
 CFG_PORT_0_AS_VCONN_SRC_ACCEPT_VCONN_SWAP, CFG_PORT_0_AS_NOT_VCONN_SRC_ACCEPT_VCONN_SWAP);

#define CFG_FORM_ROLE_SWAP_POLICY(AsDfp_Request_DRSwap, AsUfp_Request_DRSwap, \
 AsDfp_Accept_DRSwap, AsUfp_Accept_DRSwap, AsSrc_Request_PRSwap, AsSnk_Request_PRSwap,\
 AsSrc_Accept_PRSwap, AsSnk_Accept_PRSwap, AsVCONNSrc_Req_VCONNSwap, AsNotVCONNSrc_Req_VCONNSwap,\
 AsVCONNSrc_Accept_VCONNSwap, AsNotVCONNSrc_Accept_VCONNSwap) ( (AsDfp_Request_DRSwap << 0) | \
 (AsUfp_Request_DRSwap << 1) | (AsDfp_Accept_DRSwap << 2) | (AsUfp_Accept_DRSwap << 3) \
 | (AsSrc_Request_PRSwap << 4) | (AsSnk_Request_PRSwap << 5) | (AsSrc_Accept_PRSwap << 6) \
 | (AsSnk_Accept_PRSwap << 7) | (AsVCONNSrc_Req_VCONNSwap << 8) | (AsNotVCONNSrc_Req_VCONNSwap << 9)\
 | (AsVCONNSrc_Accept_VCONNSwap << 10) | (AsNotVCONNSrc_Accept_VCONNSwap << 11))

/***********************Defines to form VDM Header************************/
/* VDM Header (From Table 6-25 Structured VDM Header of PD 3.0 Spec)
   B31-16  - Standard or Vendor ID (SVID) 
   B15     - VDM Type 
   B14-13  - Structured VDM Version
   B12-11  - Reserved 
   B10-8   - Object Position 
   B7-6    - Command Type 
   B5      - Reserved 
   B4-0    - Command */

#define CFG_FORM_VDM_HEADER(svid,vdmType,svdmVersion,objPos,cmdType,cmd) \
    (((svid) << 16) | \
    ((vdmType) << 15) | \
    ((svdmVersion) << 13) | \
    ((objPos) << 8) | \
    ((cmdType) << 6) | \
    (cmd))

/****************** Defines to configure Discover Identity ****************/
/***************************** ID HEADER VDO*************************/
/* ID Header VDO (From Table 6-29 ID Header VDO of PD 3.0 Spec)
   B31     - USB Commn Capable as USB Host
   B30     - USB Commn Capable as USB Device 
   B29-27  - Product Type (UFP/Cable Plug)
   B26     - Modal Operation Supported
   B25-23  - Product Type (DFP)
   B22-21  - Connector Type 
   B20-16  - Reserved 
   B15-0   - USB Vendor ID */

#define CFG_FORM_ID_HEADER_VDO(host,device,prodTypeUFP,modalOpr,prodTypeDFP,connectorType,vendorID) \
    (((host) << 31) | \
    ((device) << 30) | \
    ((prodTypeUFP) << 27) | \
    ((modalOpr) << 26) | \
    ((prodTypeDFP) << 23) | \
    ((connectorType) << 21) | \
    (vendorID))

#define CFG_PORT_0_ID_HEADER_VDO   CFG_FORM_ID_HEADER_VDO(CFG_PORT_0_ID_HEADER_VDO_USB_HOST_CAPABLE, \
        CFG_PORT_0_ID_HEADER_VDO_USB_DEVICE_CAPABLE, CFG_PORT_0_ID_HEADER_VDO_PROD_TYPE_UFP, \
        CFG_PORT_0_ID_HEADER_VDO_MODAL_OPR_SUPPORTED, CFG_PORT_0_ID_HEADER_VDO_PROD_TYPE_DFP, \
        CFG_PORT_0_ID_HEADER_VDO_CONNECTOR_TYPE, CFG_PORT_0_ID_HEADER_VDO_USB_VENDOR_ID)

/***************************** PRODUCT VDO*************************/
/* Product VDO (From Table 6-34 ID Product VDO of PD 3.0 Spec)
   B31-16     - USB Product ID
   B15-0      - bcd Device */

#define CFG_FORM_PRODUCT_VDO(productID,bcdDevice) \
    (((productID) << 16) | \
    (bcdDevice))

#define CFG_PORT_0_PRODUCT_VDO   CFG_FORM_PRODUCT_VDO(CFG_PORT_0_PROD_VDO_USB_PRODUCT_ID, \
                                 CFG_PORT_0_PROD_VDO_BCD_DEVICE)

/******************* PRODUCT TYPE VDO - UFP VDO 1 *********************/
/* UFP VDO 1 (From Table 6-35 UFP VDO 1 of PD 3.0 Spec)
   B31-29  - UFP VDO Version 
   B28     - Reserved 
   B27-24  - Device Capability 
   B23-22  - Connector Type 
   B21-6   - Reserved  
   B5-3    - Alternate Modes
   B2-0    - USB Highest Speed */

#define CFG_FORM_UFP_VDO_1(vdoVer,devCapability,connectorType,altModes,usbSpeed) \
    (((vdoVer) << 29) | \
    ((devCapability) << 24) | \
    ((connectorType) << 22) | \
    ((altModes) << 3) | \
    (usbSpeed))

#define CFG_PORT_0_UFP_VDO_1    CFG_FORM_UFP_VDO_1(CFG_PORT_0_UFP_VDO_1_UFP_VDO_VERSION, \
        CFG_PORT_0_UFP_VDO_1_DEVICE_CAPABILITY, CFG_PORT_0_UFP_VDO_1_CONNECTOR_TYPE, \
        CFG_PORT_0_UFP_VDO_1_ALTERNATE_MODES, CFG_PORT_0_UFP_VDO_1_USB_HIGHEST_SPEED)

/******************* PRODUCT TYPE VDO - UFP VDO 2 *********************/
/* UFP VDO 2 (From Table 6-36 UFP VDO 2 of PD 3.0 Spec)
   B31-30  - Reserved
   B29-23  - USB4 Min Power
   B22-16  - USB4 Max Power 
   B15-14  - Reserved
   B13-7   - USB3 Min Power
   B6-0    - USB3 Max Power */

#define CFG_FORM_UFP_VDO_2(usb4MinPwr,usb4MaxPwr,usb3MinPwr,usb3MaxPwr) \
    (((usb4MinPwr) << 23) | \
    ((usb4MaxPwr) << 16) | \
    ((usb3MinPwr) << 7) | \
    (usb3MaxPwr))

#define CFG_PORT_0_UFP_VDO_2    CFG_FORM_UFP_VDO_2(CFG_PORT_0_UFP_VDO_2_USB4_MIN_PWR, \
        CFG_PORT_0_UFP_VDO_2_USB4_MAX_PWR, CFG_PORT_0_UFP_VDO_2_USB3_MIN_PWR, \
        CFG_PORT_0_UFP_VDO_2_USB3_MAX_PWR)

/******************* PRODUCT TYPE VDO - DFP VDO *********************/
/* DFP VDO (From Table 6-37 DFP VDO of PD 3.0 Spec)
   B31-29  - DFP VDO Version
   B28-27  - Reserved
   B26-24  - Host Capability
   B23-22  - Connector Type
   B21-5   - Reserved
   B4-0    - Port Number */

#define CFG_FORM_DFP_VDO(vdoVersion,hostCapability,connectorType,portNum) \
    (((vdoVersion) << 29) | \
    ((hostCapability) << 24) | \
    ((connectorType) << 22) | \
    (portNum))

#define CFG_PORT_0_DFP_VDO    CFG_FORM_DFP_VDO(CFG_PORT_0_DFP_VDO_DFP_VDO_VERSION, \
        CFG_PORT_0_DFP_VDO_DFP_HOST_CAPABILITY, CFG_PORT_0_DFP_VDO_DFP_CONNECTOR_TYPE, \
        CFG_PORT_0_DFP_VDO_DFP_PORT_NUMBER)

void PSF_LoadConfig(GLOBAL_CFG_STATUS_DATA *pasCfgStatusData);  

#endif /* _PSFSINK_BOOTCFG_H */

/* *****************************************************************************
 End of File
 */
