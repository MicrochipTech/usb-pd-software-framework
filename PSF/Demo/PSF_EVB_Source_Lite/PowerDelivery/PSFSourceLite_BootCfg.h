/***************************************************************************
  Power Delivery Source Lite Configurable globals Header File

  Company:
    Microchip Technology Inc.

  File Name:
    PSFSourceLite_BootCfg.h

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

#ifndef _PSFSOURCELITE_BOOTCFG_H    /* Guard against multiple inclusion */
#define _PSFSOURCELITE_BOOTCFG_H

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
#define CFG_PORT_0_POWER_ROLE                1
#define CFG_PORT_1_POWER_ROLE                1

/*Data Role supported values 0-Dual Role Data Not supported, 1- Dual Role Date Supported */
#define CFG_PORT_0_DUAL_ROLE_DATA_SUPPORT    (0U << 2)
#define CFG_PORT_1_DUAL_ROLE_DATA_SUPPORT    (0U << 2)

/*Rp Value - 0 -Rd(Sink), 1-Default USB Rp, 2- 1.5A Rp, 3-3.0A Rp */
#define CFG_PORT_0_RP_CURRENT_VALUE          (3U << 3)
#define CFG_PORT_1_RP_CURRENT_VALUE          (3U << 3)

/*Port Enable - 0- Port Disable; 1- Port Enable*/
#define CFG_PORT_0_ENABLE       (1U << 5)
#define CFG_PORT_1_ENABLE       (1U << 5)

/*VCONN OCS Enable - 0- Disable, 1-Enable*/
#define CFG_PORT_0_VCONN_OCS_ENABLE     (1U << 9)
#define CFG_PORT_1_VCONN_OCS_ENABLE     (1U << 9)  

/********************************PIO Specific configuration******************************/
#define CFG_PORT_0_UPD_FAULT_IN_PIO_NO       eUPD_PIO5
#define CFG_PORT_1_UPD_FAULT_IN_PIO_NO       eUPD_PIO5

#define CFG_PORT_0_UPD_FAULT_IN_MODE         eINPUT_ACTIVE_LOW
#define CFG_PORT_1_UPD_FAULT_IN_MODE         eINPUT_ACTIVE_LOW

#define CFG_PORT_0_UPD_EN_VBUS               eUPD_PIO3
#define CFG_PORT_1_UPD_EN_VBUS               eUPD_PIO3
#define CFG_PORT_0_UPD_EN_VBUS_PIO_MODE      ePUSH_PULL_ACTIVE_HIGH
#define CFG_PORT_1_UPD_EN_VBUS_PIO_MODE      ePUSH_PULL_ACTIVE_HIGH

/* Note: UPD PIO9 is shared between HPD and VSEL2 pin functionalities. 
   Hence, INCLUDE_UPD_HPD is disabled by default. If user application
   wishes to use HPD feature, then VSEL2 functionality has to be mapped
   with any MCU pin since HPD pin can only be a UPD PIO */
#define CFG_PORT_0_HPD_IO                    eUPD_PIO9
#define CFG_PORT_1_HPD_IO                    eUPD_PIO9
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
/*Port 0 PDO configuration*/
#define CFG_PORT_0_DUAL_ROLE_POWER           0U 
#define CFG_PORT_0_DUAL_ROLE_DATA            0U
#define CFG_PORT_0_SOURCE_NUM_OF_PDOS        4U
#define CFG_PORT_0_SOURCE_USB_SUSP           0U
#define CFG_PORT_0_SOURCE_UNCONSTARINED_PWR  1U
#define CFG_PORT_0_SOURCE_USB_COMM           0U
#define CFG_PORT_0_SOURCE_PDO_1     CFG_FORM_SOURCE_FIXED_PDO1(5000U, 3000U,\
                                    CFG_PORT_0_DUAL_ROLE_DATA, \
                                    CFG_PORT_0_SOURCE_USB_COMM, \
                                    CFG_PORT_0_SOURCE_USB_SUSP,  \
                                    CFG_PORT_0_SOURCE_UNCONSTARINED_PWR, \
                                    CFG_PORT_0_DUAL_ROLE_POWER);
#define CFG_PORT_0_SOURCE_PDO_2     CFG_FORM_FIXED_PDOx(9000U, 3000U)    
#define CFG_PORT_0_SOURCE_PDO_3     CFG_FORM_FIXED_PDOx(15000U, 3000U)   
#define CFG_PORT_0_SOURCE_PDO_4     CFG_FORM_FIXED_PDOx(20000U, 3000U)
#define CFG_PORT_0_SOURCE_PDO_5     CFG_FORM_FIXED_PDOx(0,0)   
#define CFG_PORT_0_SOURCE_PDO_6     CFG_FORM_FIXED_PDOx(0,0)
#define CFG_PORT_0_SOURCE_PDO_7     CFG_FORM_FIXED_PDOx(0,0)   
/********************************************************************************/
/**********************************Port 1****************************************/
/*********************************************************************************/
#define CFG_PORT_1_DUAL_ROLE_POWER           0U 
#define CFG_PORT_1_DUAL_ROLE_DATA            0U
#define CFG_PORT_1_SOURCE_NUM_OF_PDOS        4U
#define CFG_PORT_1_SOURCE_USB_SUSP           0U
#define CFG_PORT_1_SOURCE_UNCONSTARINED_PWR  1U
#define CFG_PORT_1_SOURCE_USB_COMM           0U
#define CFG_PORT_1_SOURCE_PDO_1     CFG_FORM_SOURCE_FIXED_PDO1(5000U, 3000U, \
                                    CFG_PORT_1_DUAL_ROLE_DATA, \
                                    CFG_PORT_1_SOURCE_USB_COMM, \
                                    CFG_PORT_1_SOURCE_USB_SUSP,  \
                                    CFG_PORT_1_SOURCE_UNCONSTARINED_PWR, \
                                    CFG_PORT_1_DUAL_ROLE_POWER);
#define CFG_PORT_1_SOURCE_PDO_2     CFG_FORM_FIXED_PDOx(9000U,3000U)    
#define CFG_PORT_1_SOURCE_PDO_3     CFG_FORM_FIXED_PDOx(15000U,3000U)   
#define CFG_PORT_1_SOURCE_PDO_4     CFG_FORM_FIXED_PDOx(20000U,3000U)
#define CFG_PORT_1_SOURCE_PDO_5     CFG_FORM_FIXED_PDOx(0,0)   
#define CFG_PORT_1_SOURCE_PDO_6     CFG_FORM_FIXED_PDOx(0,0)  
#define CFG_PORT_1_SOURCE_PDO_7     CFG_FORM_FIXED_PDOx(0,0)  

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


#define CFG_PORT_1_AS_DFP_REQUEST_DR_SWAP             0
#define CFG_PORT_1_AS_UFP_REQUEST_DR_SWAP             0
#define CFG_PORT_1_AS_DFP_ACCEPT_DR_SWAP              0
#define CFG_PORT_1_AS_UFP_ACCEPT_DR_SWAP              0
#define CFG_PORT_1_AS_SRC_REQUEST_PR_SWAP             0
#define CFG_PORT_1_AS_SNK_REQUEST_PR_SWAP             0
#define CFG_PORT_1_AS_SRC_ACCEPT_PR_SWAP              0
#define CFG_PORT_1_AS_SNK_ACCEPT_PR_SWAP              0
#define CFG_PORT_1_AS_VCONN_SRC_REQ_VCONN_SWAP        0
#define CFG_PORT_1_AS_NOT_VCONN_SRC_REQ_VCONN_SWAP    0
#define CFG_PORT_1_AS_VCONN_SRC_ACCEPT_VCONN_SWAP     1
#define CFG_PORT_1_AS_NOT_VCONN_SRC_ACCEPT_VCONN_SWAP 1


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

/* Macro used to form Fixed PDOs 2 to 7 */
#define CFG_FORM_FIXED_PDOx(voltage,current)   ((((voltage)/50) << 10) | ((current)/10))

/***********************Defines to form role swap policy************************/
#define CFG_PORT_0_ROLE_SWAP_POLICY  CFG_FORM_ROLE_SWAP_POLICY(CFG_PORT_0_AS_DFP_REQUEST_DR_SWAP, \
 CFG_PORT_0_AS_UFP_REQUEST_DR_SWAP, CFG_PORT_0_AS_DFP_ACCEPT_DR_SWAP, CFG_PORT_0_AS_UFP_ACCEPT_DR_SWAP,\
 CFG_PORT_0_AS_SRC_REQUEST_PR_SWAP, CFG_PORT_0_AS_SNK_REQUEST_PR_SWAP, CFG_PORT_0_AS_SRC_ACCEPT_PR_SWAP,\
 CFG_PORT_0_AS_SNK_ACCEPT_PR_SWAP, CFG_PORT_0_AS_VCONN_SRC_REQ_VCONN_SWAP, CFG_PORT_0_AS_NOT_VCONN_SRC_REQ_VCONN_SWAP,\
 CFG_PORT_0_AS_VCONN_SRC_ACCEPT_VCONN_SWAP, CFG_PORT_0_AS_NOT_VCONN_SRC_ACCEPT_VCONN_SWAP);

#define CFG_PORT_1_ROLE_SWAP_POLICY CFG_FORM_ROLE_SWAP_POLICY(CFG_PORT_1_AS_DFP_REQUEST_DR_SWAP, \
 CFG_PORT_1_AS_UFP_REQUEST_DR_SWAP, CFG_PORT_1_AS_DFP_ACCEPT_DR_SWAP, CFG_PORT_1_AS_UFP_ACCEPT_DR_SWAP,\
 CFG_PORT_1_AS_SRC_REQUEST_PR_SWAP, CFG_PORT_1_AS_SNK_REQUEST_PR_SWAP, CFG_PORT_1_AS_SRC_ACCEPT_PR_SWAP,\
 CFG_PORT_1_AS_SNK_ACCEPT_PR_SWAP, CFG_PORT_1_AS_VCONN_SRC_REQ_VCONN_SWAP, CFG_PORT_1_AS_NOT_VCONN_SRC_REQ_VCONN_SWAP,\
 CFG_PORT_1_AS_VCONN_SRC_ACCEPT_VCONN_SWAP, CFG_PORT_1_AS_NOT_VCONN_SRC_ACCEPT_VCONN_SWAP);

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

void PSF_LoadConfig(); 

#endif /* _PSFSOURCELITE_BOOTCFG_H */

/* *****************************************************************************
 End of File
 */
