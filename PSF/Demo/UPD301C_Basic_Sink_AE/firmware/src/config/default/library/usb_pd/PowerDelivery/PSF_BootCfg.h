/***************************************************************************
  Power Delivery Source Pro Configurable globals Header File

  Company:
    Microchip Technology Inc.

  File Name:
    PSF_BootCfg.h

  Description:
    This header file contains user configurable globals and function prototypes.
************************************************************************** */
/*******************************************************************************
Copyright ©  [2022] Microchip Technology Inc. and its subsidiaries.

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

#ifndef _PSF_BOOTCFG_H    /* Guard against multiple inclusion */
#define _PSF_BOOTCFG_H

/*********************************Common Configuration*****************************************/
#define CFG_PRODUCT_ID              0x350U
#define CFG_VENDOR_ID               0x424U
#define CFG_HW_VERSION              0x0U
#define CFG_SILICON_VERSION         0x0U

/*Length of Manufacturer string should be within the range of 0 to 21.
 Default length of Manufacturer string is 20*/
#define CFG_MANUFACTURER_STR_LEN    20U

/*Number of characters in Manufacturer string should be less than or equal to
 the value in CFG_MANUFACTURER_STR_LEN. Default manufacturer string is 
 "Microchip Technology" */
#define CFG_MANUFACTURER_STR        "Microchip Technology"

/***********************************Port Specific configuration***********************/    

/*************************************************************************************/
/**************************************Port 0 Configurations**************************/
/*************************************************************************************/

/*Configuration value for u32CfgData*/
/* Note: The CFG_PORT_0_CFG_DATA is a bit mapped entry consisting of the following
Port Specific Configurations
Port Power Role Values (Bit 1:0) 0 - Sink, 1 - Source, 2 - DRP
Data Role supported values (Bit 2) 0 - Dual Role Data Not supported, 1 - Dual Role Date Supported
Rp Value (Bit 4:3) 0 -Rd(Sink), 1-Default USB Rp, 2- 1.5A Rp, 3-3.0A Rp
Port Enable (Bit 5) 0- Port Disable; 1- Port Enable
VCONN OCS Enable (Bit 9) 0- Disable, 1-Enable 
Power Data State for FRS (Bit 12:11) 0 - FRS Disabled for Port, 1 - FRS only in Sink/Data Host, 2 - FRS only in Source/Data Device */
#define CFG_PORT_0_CFG_DATA                  0x238U

/********************************PIO Specific configuration******************************/
#define CFG_PORT_0_UPD_FAULT_IN_PIO_NO       eUPD_PIO9
#define CFG_PORT_0_UPD_FAULT_IN_MODE         eINPUT_ACTIVE_LOW
#define CFG_PORT_0_UPD_EN_SNK                eUPD_PIO6
#define CFG_PORT_0_UPD_EN_SNK_PIO_MODE       ePUSH_PULL_ACTIVE_HIGH



/**********************Fault Related configuration**************************/
#define CFG_PORT_0_OVER_VOLTAGE_FACTOR              115U
#define CFG_PORT_0_UNDER_VOLTAGE_FACTOR             85U
#define CFG_PORT_0_FAULT_IN_OCS_DEBOUNCE_MS         5U
#define CFG_PORT_0_VCONN_OCS_DEBOUNCE_IN_MS         2U
#define CFG_PORT_0_MAX_VBUS_POWER_FAULT_COUNT       3U
#define CFG_PORT_0_MAX_VCONN_FAULT_COUNT            3U
#define CFG_PORT_0_POWER_GOOD_TIMER_MS              10000U
#define CFG_PORT_0_MAX_PORT_CURRENT_IN_10mA         300U

/*********************************************************************************/
/*********************************PDO Configuration ******************************/
/*********************************************************************************/

/******************************Sink PDO Configuration***************************/
#define CFG_PORT_0_SINK_NUM_OF_PDOS       4U

#define CFG_PORT_0_SINK_PDO_1             0x1801912cU
#define CFG_PORT_0_SINK_PDO_2             0x2d12cU
#define CFG_PORT_0_SINK_PDO_3             0x4b12cU
#define CFG_PORT_0_SINK_PDO_4             0x6412cU

/********************************Sink Specific Configuration********************************/
/*u8SinkConfigSel configuration*/
#define CFG_PORT_0_SINK_CONFIG_SEL                             0x0U

#define CFG_PORT_0_SINK_MAX_OPERATING_CURRENT_InmA             3000U
#define CFG_PORT_0_SINK_MIN_OPERATING_CURRENT_InmA             1000U

#define CFG_PORT_0_SINK_PDO_1_PREFERRED_MIN_CURRENT            2000U
#define CFG_PORT_0_SINK_PDO_2_PREFERRED_MIN_CURRENT            2000U
#define CFG_PORT_0_SINK_PDO_3_PREFERRED_MIN_CURRENT            2000U
#define CFG_PORT_0_SINK_PDO_4_PREFERRED_MIN_CURRENT            2000U

/******************************DAC_I defines*********************************/
#define CFG_PORT_0_SINK_DAC_I_MAX_OP_VOLTAGE                   2500U
#define CFG_PORT_0_SINK_DAC_I_MIN_OP_VOLTAGE                   0U
#define CFG_PORT_0_SINK_DAC_I_CUR_INDICATION_MAX               5000U
/*Configuration value for u8DAC_I_Direction*/
/*0 - High amperage, Max Voltage, 1- High amperage, Min Voltage*/
#define CFG_PORT_0_SINK_DAC_I_DIR                              0U

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
#define CFG_PORT_0_SINK_CAPS_EXTD_VID                          0x424U
#define CFG_PORT_0_SINK_CAPS_EXTD_PID                          0x350U
#define CFG_PORT_0_SINK_CAPS_EXTD_XID                          0x0U
#define CFG_PORT_0_SINK_CAPS_EXTD_FW_VERSION                   0x0U
#define CFG_PORT_0_SINK_CAPS_EXTD_HW_VERSION                   0x0U
#define CFG_PORT_0_SINK_CAPS_EXTD_SKEDB_VERSION                0x1U
#define CFG_PORT_0_SINK_CAPS_EXTD_LOAD_STEP                    0U
#define CFG_PORT_0_SINK_CAPS_EXTD_LOAD_CHARACTERISTICS         0U
#define CFG_PORT_0_SINK_CAPS_EXTD_COMPLIANCE                   0U
#define CFG_PORT_0_SINK_CAPS_EXTD_TOUCH_TEMP                   0U
#define CFG_PORT_0_SINK_CAPS_EXTD_BATTERY_INFO                 0U
#define CFG_PORT_0_SINK_CAPS_EXTD_SINK_MODES                   4U
#define CFG_PORT_0_SINK_CAPS_EXTD_MIN_PDP_In_WATT              5U
#define CFG_PORT_0_SINK_CAPS_EXTD_OPERATIONAL_PDP_In_WATT      60U
#define CFG_PORT_0_SINK_CAPS_EXTD_MAX_PDP_In_WATT              60U 

/**************************Role Swap Policy defines*****************************/
#define CFG_PORT_0_ROLE_SWAP_POLICY       0x0U  

/*********************** Feature Select Defines *************************************/
/* Note: The CFG_PORT_0_FEATURE_SELECT is a bit mapped entry consisting of the following
Port Specific Configurations
Power Balancing Enable (Bit 0) 0 - Disable, 1 - Enable
VDM Support Enable (Bit 1) 0 - Disable, 1 - Enable
Alt Mode Entry Point Enable (Bit 2) 0 - Disable, 1 - Enable */
#define CFG_PORT_0_FEATURE_SELECT         0x0U


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
  B31-30    -11b Augumented Power Data Object
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
    (((3U) << 30U) |\
    ((pwrLtd) << 27U) | \
    (((maxVolt) / 100U) << 17U) | \
    (((minVolt) / 100U) << 8U) | \
    ((maxCurrent) / 50U)) 

/***********************Defines to form role swap policy************************/

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

/***************************** PRODUCT VDO*************************/
/* Product VDO (From Table 6-34 ID Product VDO of PD 3.0 Spec)
   B31-16     - USB Product ID
   B15-0      - bcd Device */

#define CFG_FORM_PRODUCT_VDO(productID,bcdDevice) \
    (((productID) << 16) | \
    (bcdDevice))

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


void PSF_LoadConfig(GLOBAL_CFG_STATUS_DATA *pasCfgStatusData); 

#endif /* _PSF_BOOTCFG_H */

/* *****************************************************************************
 End of File
 */
