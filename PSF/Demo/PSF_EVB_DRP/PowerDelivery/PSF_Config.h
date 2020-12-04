/*******************************************************************************
  PSF stack configuration

  Company:
    Microchip Technology Inc.

  File Name:
    PSF_Config.h

  Summary:
    PSF configuration header file

  Description:
    This header file contains the configuration parameters of PSF stack that are 
    required to configure the Power delivery modules.
*******************************************************************************/
//DOM-IGNORE-BEGIN
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

********************************************************************************

*******************************************************************************/
#ifndef _PSF_CONFIG_H_
#define _PSF_CONFIG_H_

#include <generic_defs.h>

//DOM-IGNORE-END

// *****************************************************************************
// *****************************************************************************
// Section: File includes - USER_APPLICATION FILES CAN GO HERE
// *****************************************************************************
// *****************************************************************************

// *****************************************************************************
// *****************************************************************************
// Section: FEATURE INCLUDE/EXCLUDE AT COMPILE TIME
// *****************************************************************************
// *****************************************************************************
/**************************************************************************************************
Summary:
    USB-PD V3.0 support code inclusion.
Description:
    Setting the INCLUDE_PD_3_0 as 1, enables PSF to include USB Power delivery 3.0 specification
    features(collision avoidance and extended message support via chunking) along with PD 2.0 features
	at compile time. Users can set this define to 0 to reduce code size if none of the PD 
    enabled ports require PD 3.0 specific features.
Remarks:
    Default value is '1'.
Example:
    <code>
    #define INCLUDE_PD_3_0	1(Include USB PD 3.0 specific features to PSF)
    #define INCLUDE_PD_3_0	0(Exclude USB PD 3.0 specific features from PSF)
    </code>
**************************************************************************************************/
#define INCLUDE_PD_3_0                     1

/**************************************************************************************************
Summary:
    Source support code inclusion.
Description:
    Setting the INCLUDE_PD_SOURCE as 1 enables PSF to include the USB PD Source functionality
    at the compile time. Users can set this define to 0 to reduce code size if none of the PD 
    enabled ports in the system are configured for Source operation.
Remarks: 
    Default value is '1' for Source and DRP Applications.
Example:
    <code>
    #define INCLUDE_PD_SOURCE	1(Include USB PD Source functionality in PSF)
    #define INCLUDE_PD_SOURCE	0(Exclude USB PD Source functionality from PSF)
    </code>
**************************************************************************************************/
#define INCLUDE_PD_SOURCE           1

/**************************************************************************************************
Summary:
    Sink support code inclusion.
Description:
    Setting the INCLUDE_PD_SINK as 1 enables PSF to include USB PD Sink functionality at the 
	compile time. Users can set this define to 0 to reduce code size if none of the PD 
    enabled ports are configured for Sink operation.
Remarks:
    Default value is '1' for Sink and DRP Applications.
Example:
    <code>
    #define INCLUDE_PD_SINK	1(Include USB PD Sink functionality in PSF)
    #define INCLUDE_PD_SINK	0(Exclude USB PD Sink functionality from PSF)
    </code>
**************************************************************************************************/
#define INCLUDE_PD_SINK    		1

/**************************************************************************************************
Summary:
    DRP support code inclusion.
Description:
    Setting the INCLUDE_PD_DRP as 1 enables PSF to include USB PD DRP functionality at the 
	compile time. Users can set this define to 0 to reduce code size if none of the PD 
    enabled ports are configured for DRP operation.
Remarks:
    Default value is '1' for DRP Application. INCLUDE_PD_SOURCE and INCLUDE_PD_SINK should be
    set to 1 as a prerequisite when INCLUDE_PD_DRP is set to 1.
Example:
    <code>
    #define INCLUDE_PD_DRP	1(Include USB PD DRP functionality in PSF)
    #define INCLUDE_PD_DRP	0(Exclude USB PD DRP functionality from PSF)
    </code>
**************************************************************************************************/
#define INCLUDE_PD_DRP    		1

/**************************************************************************************************
Summary:
    VCONN_Swap Support code inclusion.
Description:
    Setting the INCLUDE_PD_VCONN_SWAP as 1 enables PSF to include the VCONN Swap
	functionality at compile time. Users can set this define to 0 to reduce code size if none of
	the PD enabled ports require VCONN Swap functionality.
Remarks:
    Default value is 1. For Source and DRP Operation, it is mandatory to define this macro as '1'.
    When INCLUDE_PD_SOURCE is defined as '1', define this macro as '1'.
Example:
    <code>
    #define INCLUDE_PD_VCONN_SWAP	1(Include VCONN Swap functionality in PSF)
    #define INCLUDE_PD_VCONN_SWAP	0(Exclude VCONN Swap functionality from PSF)
    </code>
**************************************************************************************************/
#define INCLUDE_PD_VCONN_SWAP  	1

/**************************************************************************************************
Summary:
    Power Fault Handling code inclusion.
Description:
    Setting the INCLUDE_POWER_FAULT_HANDLING as 1 enables PSF to handle power faults (Source and 
	Sink over voltage, Source OCS, Sink under voltage) as per Power Delivery specification Rev3.0 as
	applicable. Users can set this define to 0 to reduce code size if PSF based power fault handling
	is not required.
Remarks:
    Default value is 1.
Example:
    <code>
    #define INCLUDE_POWER_FAULT_HANDLING	1(Include Power Fault handling to PSF)
    #define INCLUDE_POWER_FAULT_HANDLING	0(Exclude Power Fault handling from PSF )
    </code>
**************************************************************************************************/
#define INCLUDE_POWER_FAULT_HANDLING     1     

/**************************************************************************************************
Summary:
    PIO Override Feature code inclusion.
Description:
    PIO override is UPD350 specific feature which changes the state of a PIO without software
    intervention. PSF uses this feature to disable EN_VBUS(in case of source operation) or  
	EN_SINK(in case of sink operation) instantly on detection of a Power Fault or Fast Role Swap
    Condition. Setting the INCLUDE_UPD_PIO_OVERRIDE_SUPPORT as 1 enables this feature. Users can 
    set this define to 0 to reduce code size of PSF if PIO override based power fault handling 
    is not required. It is recommended that this define shall be set when Fast Role Swap feature
    is enabled since Fast Role Swap is tightly coupled with PIO Override feature. 
Remarks:
    To use this feature, EN_VBUS or EN_SINK, FAULT_IN and FRS_Request pins of the system should be
    UPD350 PIOs. It is also confined to INCLUDE_POWER_FAULT_HANDLING define, thus 
    INCLUDE_POWER_FAULT_HANDLING should be declared as 1 for INCLUDE_UPD_PIO_OVERRIDE_SUPPORT
    define to be effective. Recommended default value is 1 if UPD350 PIOs are used for EN_VBUS, 
    EN_SINK and FAULT_IN.
Example:
    <code>
    #define INCLUDE_UPD_PIO_OVERRIDE_SUPPORT	1(Include UPD350 PIO Override support for Power 
                                                        fault to PSF)
    #define INCLUDE_UPD_PIO_OVERRIDE_SUPPORT	0(Exclude UPD350 PIO Override support for Power 
                                                        fault from PSF)
    </code>
**************************************************************************************************/
#define INCLUDE_UPD_PIO_OVERRIDE_SUPPORT     1 

/**************************************************************************************************
Summary:
    Power Management Control Support code inclusion.
Description:
    Setting the INCLUDE_POWER_MANAGEMENT_CTRL as 1 enables PSF to include the 
    functionality that puts the UPD350 into low power mode if UPD350 is inactive for 
    15 seconds time and PSF notifies the same via the call back
    MCHP_PSF_NOTIFY_CALL_BACK. Users can set this define to 0 to reduce code size of the PSF 
    if low power mode operation of UPD350 is not required for the application.
Remarks:
    Default value is 1.
Example:
    <code>
    #define INCLUDE_POWER_MANAGEMENT_CTRL	1(Include power management feature)
    #define INCLUDE_POWER_MANAGEMENT_CTRL	0(Exclude power management feature)
    </code>
**************************************************************************************************/
#define INCLUDE_POWER_MANAGEMENT_CTRL       0

/**************************************************************************
Summary:
    PD Firmware update code inclusion.
Description:
    Setting the INCLUDE_PDFU as 1 includes the state machine code for PD Firmware Update 
    feature as per USB Power Delivery FW Update Specification v1.0. Users can set this define 
    to 0 to reduce code size if the PSF application does not use Firmware update feature. 
Remarks:
    Set to 0 by default. INCLUDE_PD_3_0 should be set to 1 as a prerequisite when INCLUDE_PDFU
    is set to 1.
Example:
    <code>
    #define INCLUDE_PDFU    1(Include PDFU feature)
    #define INCLUDE_PDFU    0(Exclude PDFU feature)
    </code>                                                                        
 ***************************************************************************/
#define INCLUDE_PDFU                  0

/**************************************************************************************************
Summary:
    Power Balancing support code inclusion.
Description:
    Setting the INCLUDE_POWER_BALANCING as 1 enables PSF to include the PD 
    Power Balancing functionality at the compile time. Users can set this define to 0
    to reduce code size if none of the PD enabled Source ports in the system 
    require Power Balancing functionality.
Remarks: 
    Default value is 1 for Source application. INCLUDE_PD_SOURCE should be set to 1 as a
    prerequisite when INCLUDE_POWER_BALANCING is set to 1.
Example:
    <code>
    #define INCLUDE_POWER_BALANCING	1(Include Power Balancing functionality in PSF)
    #define INCLUDE_POWER_BALANCING	0(Exclude Power Balancing functionality from PSF)
    </code>
**************************************************************************************************/
#define INCLUDE_POWER_BALANCING  		1

/**************************************************************************************************
Summary:
    Power Throttling support code inclusion.
Description:
    Setting the INCLUDE_POWER_THROTTLING as 1 enables PSF to include the 
    Power Throttling(PT) feature at the compile time. Users can set this define to 0
    to reduce code size if none of the Source ports in the system 
    require PT functionality.
Remarks: 
    Default value is 1 for Source application. INCLUDE_PD_SOURCE should be set to 1 as a prerequisite
    when INCLUDE_POWER_THROTTLING is set to 1. 
Example:
    <code>
    #define INCLUDE_POWER_THROTTLING	1(Include PT functionality in PSF)
    #define INCLUDE_POWER_THROTTLING	0(Exclude PT functionality from PSF)
    </code>
**************************************************************************************************/
#define INCLUDE_POWER_THROTTLING        1

/**************************************************************************************************
Summary:
    Source PPS support code inclusion.
Description:
    Setting the INCLUDE_PD_SOURCE_PPS as 1 enables PSF to include the Source Programmable 
    Power Supply(PPS) feature at the compile time. Users can set this define to 0
    to reduce code size if none of the Source ports in the system 
    require PPS functionality.
Remarks: 
    Default value is 1 for Source application. INCLUDE_PD_SOURCE and INCLUDE_PD_3_0 should be set
    to 1 as a prerequisite when INCLUDE_PD_SOURCE_PPS is set to 1. 
Example:
    <code>
    #define INCLUDE_PD_SOURCE_PPS	1(Include Source PPS functionality in PSF)
    #define INCLUDE_PD_SOURCE_PPS	0(Exclude Source PPS functionality from PSF)
    </code>
**************************************************************************************************/
#define INCLUDE_PD_SOURCE_PPS           1

/**************************************************************************************************
Summary:
    Data Role Swap support code inclusion.
Description:
    Setting the INCLUDE_PD_DR_SWAP as 1 enables PSF to include the Data Role 
    Swap (DR_SWAP) feature at the compile time. Users can set this define to 0
    to reduce code size if none of the ports in the system require DR_SWAP
    functionality.
Remarks: 
    Default value is 1. Users can configure it based on the
    application.
Example:
    <code>
    #define INCLUDE_PD_DR_SWAP	1(Include DR_SWAP functionality in PSF)
    #define INCLUDE_PD_DR_SWAP	0(Exclude DR_SWAP functionality from PSF)
    </code>
**************************************************************************************************/
#define INCLUDE_PD_DR_SWAP           1

/**************************************************************************************************
Summary:
    Power Role Swap support code inclusion.
Description:
    Setting the INCLUDE_PD_PR_SWAP as 1 enables PSF to include the Power Role 
    Swap (PR_SWAP) feature at the compile time. PR_Swap is applicable only for the ports that
    are configured for DRP operation. Users can set this define to 0 to reduce the code size
    if none of the DRP ports in the system require Power Role Swap functionality.
Remarks: 
    Default value is 1 for DRP application. INCLUDE_PD_DRP should be set to 1 as a prerequisite
    when INCLUDE_PD_PR_SWAP is set to 1. 
Example:
    <code>
    #define INCLUDE_PD_PR_SWAP	1(Include PR_SWAP functionality in PSF)
    #define INCLUDE_PD_PR_SWAP	0(Exclude PR_SWAP functionality from PSF)
    </code>
**************************************************************************************************/
#define INCLUDE_PD_PR_SWAP      1

/**************************************************************************************************
Summary:
    Vendor Defined Message support code inclusion.
Description:
    Setting the INCLUDE_PD_VDM as 1 enables PSF to include the Structured Vendor Defined 
    Message(VDM) feature at the compile time. Users can set this define to 0 to reduce the code size
    if none of the ports in the system require Structured VDM support.
Remarks: 
    Default value is 1. 
Example:
    <code>
    #define INCLUDE_PD_VDM	1(Include Structured VDM support in PSF)
    #define INCLUDE_PD_VDM	0(Exclude Structured VDM support from PSF)
    </code>
**************************************************************************************************/
#define INCLUDE_PD_VDM             1

/**************************************************************************************************
Summary:
    Alternate Mode support code inclusion.
Description:
    Setting the INCLUDE_PD_ALT_MODE as 1 enables PSF to include the Alternate Mode 
    feature at the compile time. Users can set this define to 0 to reduce the code size
    if none of the ports in the system require Alternate Mode support.
Remarks: 
    Default value is 1. INCLUDE_PD_VDM should be set to 1 as a prerequisite when
    INCLUDE_PD_ALT_MODE is set to 1. 
Example:
    <code>
    #define INCLUDE_PD_ALT_MODE	1(Include Alternate Mode support in PSF)
    #define INCLUDE_PD_ALT_MODE	0(Exclude Alternate Mode support from PSF)
    </code>
**************************************************************************************************/
#define INCLUDE_PD_ALT_MODE             1

/**************************************************************************************************
Summary:
    Hot Plug Detect support code inclusion.
Description:
    Setting the INCLUDE_UPD_HPD as 1 enables PSF to include the Hot Plug Detect(HPD) feature 
    at the compile time. Users can set this define to 0 to reduce the code size
    if none of the ports in the system require HPD support.
Remarks: 
    Default value is 1. INCLUDE_PD_ALT_MODE should be set to 1 as a prerequisite when
    INCLUDE_UPD_HPD is set to 1.    
Example:
    <code>
    #define INCLUDE_UPD_HPD	1(Include Hot Plug Detect support in PSF)
    #define INCLUDE_UPD_HPD	0(Exclude Hot Plug Detect support from PSF)
    </code>
**************************************************************************************************/
#define INCLUDE_UPD_HPD             0

/**************************************************************************************************
Summary:
    Fast Role Swap support code inclusion.
Description:
    Setting the INCLUDE_PD_FR_SWAP as 1 enables PSF to include the Fast Role 
    Swap (FR_SWAP) feature at the compile time. FR_Swap is applicable only for the ports that
    are configured for DRP operation. Users can set this define to 0 to reduce the code size
    if none of the DRP ports in the system require Fast Role Swap functionality.
Remarks: 
    Default value is 1 for DRP application. INCLUDE_PD_DRP, INCLUDE_PD_3_0 and 
    INCLUDE_UPD_PIO_OVERRIDE_SUPPORT should be set to 1 as a prerequisite
    when INCLUDE_PD_FR_SWAP is set to 1.
Example:
    <code>
    #define INCLUDE_PD_FR_SWAP	1(Include FR_SWAP functionality in PSF)
    #define INCLUDE_PD_FR_SWAP	0(Exclude FR_SWAP functionality from PSF)
    </code>
**************************************************************************************************/
#define INCLUDE_PD_FR_SWAP      0

// *****************************************************************************
// *****************************************************************************
// Section: Power Delivery IDs
// *****************************************************************************
// *****************************************************************************

/************************************************************************
Summary:
    Hardware Minor Version.
Description:
    CONFIG_HWMAJOR_VERSION defines Hardware Minor Version details of the product. It is used by 
    the PD Firmware Update state-machine during Enumeration phase. This information is shared 
    with the PDFU Initiator as part of GET_FW_ID command's response.
Remarks:
    This is a 4-bit entity. (Valid values are 0x0 to 0xF).
    The user definition of this macro is mandatory when INCLUDE_PDFU is defined as '1'.
Example:
    <code>
    #define CONFIG_HWMINOR_VERSION    0x0
    </code>                                                                    
  ************************************************************************/                                                                                           
#define CONFIG_HWMINOR_VERSION       0x0u

/************************************************************************
Summary:
    Hardware Major Version.
Description:
    CONFIG_HWMAJOR_VERSION defines Hardware Major Version details of the product. It is used by 
    the PD Firmware Update state-machine during Enumeration phase. This information is shared 
    with the PDFU Initiator as part of GET_FW_ID command's response.
Remarks:
    This is a 4-bit entity. (Valid values are 0x0 to 0xF).
    The user definition of this macro is mandatory when INCLUDE_PDFU is defined as '1'.
Example:
    <code>
    #define CONFIG_HWMAJOR_VERSION    0x1
    </code>                                                                 
  ************************************************************************/                                                                                            
#define CONFIG_HWMAJOR_VERSION       0x1u

// *****************************************************************************
// *****************************************************************************
// Section: System Level Configuration
// *****************************************************************************
// *****************************************************************************
/**********************************************************************
Summary:
    Power Delivery Enabled ports count.
Description:
    CONFIG_PD_PORT_COUNT defines the number of Power delivery enabled ports. The maximum number
    of ports PSF can support is '4'. 
Remarks:
    The max and min value for CONFIG_PD_PORT_COUNT is '4' and '1' respectively. PSF refers the 
    Port number in the call backs as 0 to (CONFIG_PD_PORT_COUNT - 1). 
    The default value is 2 and it can be defined based on the user application. For each port 
    defined by this macro approximately 500Bytes of Data RAM is consumed.
Example:
    <code>
    #define CONFIG_PD_PORT_COUNT        2 (Number of PD ports enabled in PSF Stack is 2)
    </code>
**********************************************************************/
#define CONFIG_PD_PORT_COUNT            2

/**************************************************************************
Summary:
    HW Communication interface between SOC and UPD350.
Description:
    CONFIG_DEFINE_UPD350_HW_INTF_SEL defines the Hardware interface for communication between
    the SOC and UPD350. It can take either CONFIG_UPD350_SPI or CONFIG_UPD350_I2C as input value.
	
	<b>CONFIG_UPD350_SPI</b> - SPI is the communication interface between SOC and UPD350.
									SPI interface is supported by UPD350 B and D parts alone.
	
	<b>CONFIG_UPD350_I2C</b> - I2C is the communication interface between SOC and UPD350.
									I2C interface is supported by UPD350 A and C parts alone.
Remarks:
    CONFIG_DEFINE_UPD350_HW_INTF_SEL should be defined based on UPD350 silicon part used for the
    application. All the ports in a system should use either I2C supported or SPI supported 
    UPD350 part. Using mixed interfaces for individual ports is not supported (i.e.: SPI for 
	Port 1 and I2C for Port 2).

Example:
    <code>
	#define CONFIG_DEFINE_UPD350_HW_INTF_SEL    CONFIG_UPD350_SPI
	#define CONFIG_DEFINE_UPD350_HW_INTF_SEL    CONFIG_UPD350_I2C
    </code>

Note:
	If the target for PSF is a UPD301C device, SPI must always be selected. I2C is not an option 
	for UPD301C due to the physical bonding of the UPD301C.


  **************************************************************************/
 #define CONFIG_DEFINE_UPD350_HW_INTF_SEL    CONFIG_UPD350_SPI     

/**************************************************************************
Summary:
    Print status messages from PSF stack through UART interface
Description:
    Setting CONFIG_HOOK_DEBUG_MSG to 1, prints status messages from PSF stack through
    UART interface. 
	
Remarks:
    The following hook APIs should be defined with appropriate UART functions to view 
    status messages from PSF stack.
    1. MCHP_PSF_HOOK_DEBUG_INIT()
    2. MCHP_PSF_HOOK_PRINT_CHAR(byData)
    3. MCHP_PSF_HOOK_PRINT_INTEGER(dwWriteInt, byWidth) 
    4. MCHP_PSF_HOOK_PRINT_TRACE(pbyMessage) 

Example:
    <code>
	#define CONFIG_HOOK_DEBUG_MSG    0
	#define CONFIG_HOOK_DEBUG_MSG    1
    </code>

Note:
    None.
**************************************************************************/
#define CONFIG_HOOK_DEBUG_MSG      0                 


// *****************************************************************************
// *****************************************************************************
// Section: PDFU Configuration
// *****************************************************************************
// *****************************************************************************
/**************************************************************************************************
Summary:
    Power Delivery Firmware Update Supported field.
Description:
    CONFIG_PDFU_SUPPORTED is set to '0' if firmware is not updatable during Run time. Otherwise  
    shall be set to 1. It is used by the PD Firmware Update state-machine during Enumeration  
    phase. This information is shared with the PDFU Initiator as part of GET_FW_ID command's response.
Remarks:
    The user definition of this macro is mandatory when INCLUDE_PDFU is '1'. By default, it is
    defined as '1'.
Example:
    <code>
     #define CONFIG_PDFU_SUPPORTED     1
    </code>                                                                 
**************************************************************************************************/                                                                                         
#define CONFIG_PDFU_SUPPORTED               1

/**************************************************************************************************
Summary:
    Power Delivery Firmware Update Supported via USB config.
Description:
    CONFIG_PDFU_VIA_USBPD_SUPPORTED Set to '1' to indicate support for PDFU via USB PD Firmware 
    Update flow.Otherwise shall be set to '0'. It is used by the PD Firmware Update state-machine 
    during Enumeration phase. This information is shared with the PDFU Initiator as part of
    GET_FW_ID command's response.
Remarks:
    The user definition of this macro is mandatory when INCLUDE_PDFU is '1'. The default value 
    is '1'.
Example:
    <code>
     #define CONFIG_PDFU_VIA_USBPD_SUPPORTED  1
    </code>                                                                   
**************************************************************************************************/                                                                                          
#define CONFIG_PDFU_VIA_USBPD_SUPPORTED     1

/**************************************************************************************************
Summary:
    Maximum Firmware image size.
Description:
    CONFIG_MAX_FIRMWARE_IMAGESIZE defines the ROM size allocated for the Updatable application. 
    PDFU Initiator shall flash entire size during every re-flash operation. Flashing lesser or 
    more than this Size results in error response.
Remarks:
    Choose Firmware Image size in such a way that integral multiple of 256. The definition of 
    this function is mandatory when INCLUDE_PDFU is '1' and shall expressed in terms of bytes. 
    By default, the value is 0x8800UL(34KB).
Example:
    <code>
     #define CONFIG_MAX_FIRMWARE_IMAGESIZE 0x9800UL (38*1024 bytes for 38KB Updatable application).
    </code>                                                                                 
**************************************************************************************************/  
#define CONFIG_MAX_FIRMWARE_IMAGESIZE   0x8800UL

/**************************************************************************************************
Summary:
	Index of Updatable image.
Description:
    CONFIG_UPDATABLE_IMAGEBANK_INDEX specifies the Image bank index for which firmware upgrade  
    is requested (or) in other words it corresponds to the image bank index of the Updatable 
    application as mentioned by Architecture 2 of PD FW Update Specification.

    This information is used during the Reconfiguration phase to determine what application is 
    currently executing and whether application switching to Fixed Application is required or not.
Remarks:
    The user definition of this macro is mandatory when INCLUDE_PDFU is '1'. By default, this
    macro is defined as 0x03.
Example:
    <code>
     #define CONFIG_UPDATABLE_IMAGEBANK_INDEX   0x03u (3 image banks are available)
    </code>                                                             
**************************************************************************************************/                                                                                         
#define CONFIG_UPDATABLE_IMAGEBANK_INDEX   0x03u

/**************************************************************************************************
Summary:
	Reconfig phase wait time value.
Description:
    CONFIG_RECONFIG_PHASE_WAITTIME specifies the Wait time required for the Reconfigure state,
    i.e. the PDFU_Initiate request processing takes "Wait time" millisecond, and next request  
    can be issued by the PDFU_Initiator after the specified wait time. This information is  
    shared with the PDFU Initiator as part of PDFU_INITIATE command's response.
Remarks:
    The user definition of this macro is mandatory when INCLUDE_PDFU is '1'. It can have values 
    from 0x00 to 0xFF. By default, it is defined as '0x00'.
Example:
    <code>
     #define CONFIG_RECONFIG_PHASE_WAITTIME   0x03u //3ms wait time required
    </code>                                                               
**************************************************************************************************/                                                                                            
#define CONFIG_RECONFIG_PHASE_WAITTIME    0x00u 

/**************************************************************************************************
Summary:
	Transfer phase wait time value.
Description:
    CONFIG_TRANSFER_PHASE_WAITTIME Species the Wait time required during the Transfer state, 
    i.e. the PDFU Data request processing takes "Wait time" millisecond, and next PDFU_DATA 
    request to be issued by the initiator after the specified wait time. This information is  
    shared with the PDFU Initiator as part of PDFU_DATA command's response.
Remarks:
    The user definition of this macro is mandatory when INCLUDE_PDFU is '1'. It can have values from
    0x00 to 0xFF. By default, it is defined as '0x03'.
Example:
    <code>
     #define CONFIG_TRANSFER_PHASE_WAITTIME   0x03u //3ms required for processing PDFU_DATA request
    </code>                                                                                     
**************************************************************************************************/                                                                                            
#define CONFIG_TRANSFER_PHASE_WAITTIME    0x64u 

/**************************************************************************************************
Summary:
	Validation phase wait time value.
Description:
    CONFIG_VALIDATION_PHASE_WAITTIME specifies the wait time macro for the validation state,i.e. 
    the PDFU_Validate command's processing takes "Wait time" millisecond, and next request can 
    be issued by the Initiator after the specified wait time.
Remarks:
    The user definition of this macro is mandatory when INCLUDE_PDFU is '1'. It can have values
    from 0x00 to 0xFF. By default, it is defined as '0x03'.
Example:
    <code>
     #define CONFIG_VALIDATION_PHASE_WAITTIME   0x03u
    </code>                                                            
**************************************************************************************************/                                                                                            
#define CONFIG_VALIDATION_PHASE_WAITTIME    0x03u 

/**********************************************************************
Summary:
    Includes reserved bytes in Configuration and Status structure.
Description:
    INCLUDE_CFG_STRUCT_MEMORY_PAD_REGION will define the reserved bytes in the config and status
    register structure, so that expansion of structure members in future can be handled without change
    in the address of the existing member elements. 
Remarks:
    The default value is 0 and it can be defined to 1 based on the user application needs. 
Example:
    <code>
    #define INCLUDE_CFG_STRUCT_MEMORY_PAD_REGION        0 
    </code>
**********************************************************************/
#define INCLUDE_CFG_STRUCT_MEMORY_PAD_REGION            0


/******************************************************************************************************
  Section:
     PIO configurations
    
    * *************************************************************************** *
    * *************************************************************************** *
    * *********************************************************************************************** *
  Summary:
    UPD350 GPIO Output mode enum.
  Description:
    eUPD_OUTPUT_PIN_MODES_TYPE enum defines the various combination modes
    applicable for UPD350 GPIO in output mode. This is applicable only for
    EN_SINK and EN_VBUS functions.
  Remarks:
    None                                                                                               
  ******************************************************************************************************/
typedef enum
{
    ePUSH_PULL_ACTIVE_HIGH       = 0x0CU,   // Active High output signal
    ePUSH_PULL_ACTIVE_LOW        = 0x04U,   // Active low output signal
    eOPEN_DRAIN_ACTIVE_HIGH      = 0x08U,   // Active High Open Drain output signal
    eOPEN_DRAIN_ACTIVE_LOW       = 0x00U,   // Active Low Open Drain output signal
    eOPEN_DRAIN_ACTIVE_HIGH_PU   = 0x88U,   // Active High Open Drain output signal with internal pull up
    eOPEN_DRAIN_ACTIVE_LOW_PU    = 0x80U    // Active Low Open Drain output signal with internal pull up
}eUPD_OUTPUT_PIN_MODES_TYPE;
/**************************************************************************************************
Summary:
    UPD350 GPIO Input mode enum.
Description:
	eUPD_INPUT_PIN_MODES_TYPE enum defines the various combination modes applicable for UPD350 GPIO
    in input mode. This is applicable only for FAULT_IN and FRS_REQUEST pin functionalities.
Remarks:
    None
**************************************************************************************************/
typedef enum
{
    eINPUT_ACTIVE_LOW         = 0x20U,   //Active low input signal
    eINPUT_ACTIVE_HIGH        = 0x10U,   //Active high input signal
    eINPUT_ACTIVE_LOW_PU      = 0xA0U,   //Active low signal with internal pull up
    eINPUT_ACTIVE_HIGH_PD     = 0x50U    //Active high signal with internal pull down
}eUPD_INPUT_PIN_MODES_TYPE;

/***************************************************************************************************************************************
  Section:
                                                     Boot Time Configuration
    
    
    
    
    
    
    
    
    
    * *************************************************************************** *
    * *************************************************************************** *
    * ********************************Globals******************************************* *
    * *********************************************************************************************** *
  Summary:
    This structure contains port specific Type C and PD configuration and status parameters.
	sPerPortData is referred from _GlobalCfgStatusData.	
  Description:
	This structure contains global configuration and status parameters that are either Integer 
	data types, Bit-Mapped bytes or other data structure.
    
	<b>1. Members that are Integer data types:</b>
	
    <table>
    Name                            Size in   R/W Config   R/W Run   \Description
                                     Bytes     time         time      
    ------------------------------  --------  -----------  --------  -------------------------------------------------------------------
    u32aSourcePDO[7]                28        R/W          R         * Source Capabilities array 
                                                                        holding maximum of 7 Data 
                                                                        Objects including Fixed 
                                                                        PDOs and PPS APDOs.																		
                                                                      * This array should be used 
																	    only for Source Operation. 
    u32aSinkPDO[7]                  28        R/W          R         * Upto 7 fixed Sink PDOs where 
																		Voltage is specified in mV
                                                                        and Current is specified in 
																		mA.
                                                                      * This array should be used 
																	    only when the port is 
																		configured as Sink.
    u32aNewSourcePDO[7]             28        R/W          R/W       * New Source Capabilities array 
                                                                        holding maximum of 7 Data 
                                                                        Objects including Fixed 
                                                                        PDOs and PPS APDOs.
                                                                      * This array is applicable 
																	    only when the port acts 
																		as Source.
    u32aNewSinkPDO[7]               28        R/W          R/W       * New Sink Capabilities array 
                                                                        holding maximum of 7 fixed 
																		Sink PDOs where voltage is 
																		specified in mV and Current
																		is specified in mA.
                                                                      * This array is applicable 
																	    only when the port 
																		acts as Sink.															
    u32aAdvertisedPDO[7]            28        R            R         * Upto 7 PDOs that are 
																		advertised to Port Partner. 
                                                                      * During run time, when the port
                                                                        acts as source, this array 
																	    holds the value of current
                                                                        u32aNewSourcePDO[7] if Bit 10 of 
																		u32CfgData is enabled 
																		else holds the value of 
																		current u32aSourcePDO[7].
                                                                      * During run time, when the port
                                                                        acts as sink, this array 
																	    holds the value of current
                                                                        u32aNewSinkPDO[7] if Bit 10 of 
																		u32CfgData is enabled 
																		else holds the value of 
																		current u32aSinkPDO[7].
    u32aPartnerSourcePDO[7]         28        R            R         * Upto 7 fixed Partner's Source PDOs 
																		where Voltage is specified 
																		in mV and Current is 
																		specified in mA
                                                                      * This array is specific for 
																	    Sink functionality.
    u32aPartnerSinkPDO[7]           28        R            R         * Upto 7 fixed Partner's Sink PDOs 
																		where Voltage is specified 
																		in mV and Current is 
																		specified in mA
                                                                      * This array is common for 
																	    Source and Sink functionality.
    u32aCableIdentity[6]            24        R            R         * Cable Identity array 
                                                                        holding the VDM Data 
                                                                        Objects received from cable
                                                                        where Index 0 corresponds
                                                                        to ID Header VDO, Index 1
                                                                        being Cert Stat VDO, 
                                                                        Index 2 being Product VDO
                                                                        and indices 3-5 correspond 
                                                                        to Product Type VDO(s)
    u32RDO                          4         R            R         * Complete raw RDO Data as
																		sent to the port partner 
																		when acting as Sink and 
																		Complete raw RDO Data as 
																		requested by connected port 
																		partner when acting as 
																		Source. 
																	  * Will be blank if no RDO has 
																		been issued/received. 
                                                                      * This variable is common for 
																	    Source and Sink.
    u16AllocatedPowerIn250mW        2         R            R         * Allocated Power for the Port 
																		PD contract in 0.25W steps
    u16NegoVoltageInmV              2         R            R         * Negotiated Voltage in mV 
																	    steps. 
																	  * When acting as Source and in
																		Fixed power supply 
																	    contract, it holds the value
																		of bits 19:10 of PDO in mV. 
																	  * When acting as Source and in
																		Programmable Power Supply 
																	    contract, it holds the value
																		of bits 19:9 of RDO in mV. 
																	  * When acting as Sink, it 
																	    holds the value of bits 
																		19:10 of PDO in mV. 
																	  * This variable is common for 
																		both Source and Sink. 
																		
    u16NegoCurrentInmA              2         R            R         * Negotiated Current in mA 
																		steps. 
																	  * When acting as Source and in
																		Fixed power supply 
																	    contract, it holds the value
																		of bits 9:0 of PDO in mV. 
																	  * When acting as Source and in
																		Programmable Power Supply 
																	    contract, it holds the value
																		of bits 6:0 of RDO in mV. 
																	  * When acting as Sink, it 
																	    holds the value of bits 
																		9:0 of PDO in mV. 
																	  * This variable is common for 
																		both Source and Sink. 	
    u16MaxSrcPrtCurrentIn10mA       2         R/W          R         * Maximum allowable current for 
													                    ports in 10mA steps 
																	  * Sample values this variable
																	    can take are, 
																		1. 0x0032 = 0.5A
																		2. 0x012C = 3A 
																		3. 0x01F4 = 5A
																	  * Note : Values above 5A 
																	    (0x01F5 - 0x0FFF) are not 
																		supported	 
    u16SnkMaxOperatingCurInmA       2         R/W          R         * Maximum allowable current or 
																		system's maximum operating
                                                                        current in terms of mA. 
                                                                      * This variable is 
                                                                         applicable only when 
                                                                         acting as Sink. 
    u16aMinPDOPreferredCurInmA[7]   14         R/W         R         * Preferred minimum current 
																		range for the PDO by which 
																		the Sink may select without 
																		setting Capability Mismatch 
																		Bit with highest current 
																		preferred.
                                                                       * This array is applicable 
																	     only for Sink Operation.                                                                     
    u16SnkMinOperatingCurInmA       2          R/W         R          * Minimum current required by 
																	     the sink hardware to be 
                                                                         operational.
                                                                        * This variable is applicable 
																	      only for Sink Operation.
                                                                        * When a Gotomin message is 
																		  issued by source, sink reduces
																		  its operating current to the 
																		  value provided in this variable.
  	u16DAC_I_MaxOutVoltInmV         2          R/W         R          * Defines the maximum voltage 
																		  on DAC_I with a maximum of 
																		  2.5V in terms of mV 
																		* This is applicable only for
																		  Sink operation. 
	u16DAC_I_MinOutVoltInmV         2		   R/W		   R   		  * Defines the minimum voltage 
																		 on DAC_I with a minimum of 
																		 0V in terms of mV 
																	    * This is applicable only for
																		  Sink operation. 
	u16DAC_I_CurrentInd_MaxInA      2		   R/W		   R    	   * Defines which current in
																		  terms of mA corresponding 
																		  to maximum output voltage 
																		* It can take either 3A or 5A 
																	      value. 
																		* If it is 5A and maximum 
																		  output voltage is 2.5V and if
                                                                          direction mentioned in 
                                                                          u8DAC_I_Direction is High 
                                                                          Amperage - Max Voltage, then 
																		  1. 0.5A > DAC_I = 0.25V 
																		  2. 1.5A > DAC_I = 0.75V
																		  3. 2.0A > DAC_I = 1V
																		  3. 2.0A > DAC_I = 1V
																		  4. 3.0A > DAC_I = 1.5V 
																		  5. 4.0A > DAC_I = 2.0V
																		  6. 5.0A > DAC_I = 2.5V
																	    * If it is 3A and maximum 
																		  output voltage is 2.5V, then
																		  1. 0.5A > DAC_I = 0.42V 
																		  2. 1.5A > DAC_I = 1.25V
																		  3. 2.0A > DAC_I = 1.67V
																		  4. 3.0A > DAC_I = 2.5V
																		  5. 4.0A > DAC_I = 2.5V
																		  6. 5.0A > DAC_I = 2.5V
																	    * This is applicable only for 
																		  Sink operation. 
    u16PowerGoodTimerInms           2         R/W          R         * After an automatic fault 
																		recovery, 
																		u16PowerGoodTimerInms
                                                                        is run to determine whether 
																		power remains in a good 
																		state for the duration of 
																		the timer, then the Fault 
																		Counter is reset. If another
																		fault occurs before the 
																		Power Good Timer expires, 
																		then the Fault Counter is 
																		incremented.
                                                                      * For power Source, it is the 
																	    time a power source must
                                                                        consistently provide power 
																		without a fault to determine 
																		the power is good and a 
																		fault condition does not 
																		exist. For power Sink, it is
																		the time after the sink 
																		established a contract and 
																		its consistently drawing 
																		power from VBUS without a 
																		power fault to determine 
																		that power is good and a
                                                                        fault condition does not 
																		exist.
    u8SourcePDOCnt                  1         R/W          R         * Number of Default Source PDOs
																	    supported.
                                                                      * This variable is applicable 
																	    only when the port is
                                                                        configured as Source.
    u8SinkPDOCnt                    1         R/W          R         * Number of Default Sink PDOs 
																		supported.
                                                                      * This variable is applicable 
																	    only when the port is
                                                                        configured as Sink.
    u8NewSourcePDOCnt               1         R/W          R/W       * Number of New Source PDOs Supported.
                                                                      * This variable is applicable  
																	    only when the port acts as Source.
    u8NewSinkPDOCnt                 1         R/W          R/W       * Number of New Sink PDOs Supported.
                                                                      * This variable is applicable  
																	    only when the port acts as Sink.																		
    u8AdvertisedPDOCnt              1         R            R         * Number of PDOs advertised to 
																		port partner.
    u8PartnerSourcePDOCnt           1         R            R         * Number of Source PDOs received from 
																		port partner.
                                                                      * This variable is specific for 
																	    Sink functionality.
    u8PartnerSinkPDOCnt             1         R            R         * Number of Sink PDOs received from 
																		port partner.
                                                                      * This variable is common for 
																	    Source and Sink functionalities.
    u8CableIdentityCnt              1         R            R         * Number of VDM Data Objects 
                                                                        received from cable.
    u8SinkConfigSel                 1         R/W          R         * BIT[1:0] - Sink Selection 
																	    mode for operation.
                                                                        1. '0x00' Mode A: Prefer 
																		Higher Voltage and Wattage
                                                                        2. '0x01' Mode B: Prefer 
																		Lower Voltage and Wattage
                                                                      * BIT2 - No USB Suspend Flag
																		1. '1':Set the flag to '1' 
																		in RDO request
																		2. '0':Set the flag to '0' 
																		in RDO request
                                                                      * BIT3 - GiveBackFlag
                                                                        1. '1':Set the flag to '1' 
																		in RDO request 
																	    enabling GotoMin feature 
																		2. Set the flag to '0' in 
																		RDO request
																	    disabling GotoMin Feature
    u8FaultInDebounceInms           1         R/W          R         * Debounce timer value in terms 
																        of milliseconds for VBUS
                                                                        overcurrent fault conditions
																		before reacting and entering
                                                                        fault recovery routine.
                                                                      * It is applicable only for 
																	    OCS detection via FAULT_IN
                                                                        configured UPD350 pin.
    u8OCSThresholdPercentage        1         R/W          R         * OCS Threshold. Reserved for 
																		future use. This variable is
                                                                        not currently used by PSF.
    u8OVThresholdPercentage         1         R/W          R         * Percentage of PDO voltage to 
																        be considered as Over 
																		Voltage for that PDO. As per
																		PD specification, desired 
																		range for fixed PDO voltage 
																		is (0.95 &#42; PDO Voltage) 
																		to (1.05 &#42; PDO Voltage), 
																		So u8OVThresholdPercentage 
																		should be greater
                                                                        than the desired range.
                                                                      * If 115% of the PDO voltage 
																	    has to be considered as
                                                                        overvoltage for that PDO 
																		voltage, then define
                                                                        u8OVThresholdPercentage as 
																		115.
                                                                      * It is mandatory to define    	
																		u8OVThresholdPercentage when
                                                                        INCLUDE_POWER_FAULT_HANDLING
																		is defined as '1'.
    u8UVThresholdPercentage         1         R/W          R         * Percentage of PDO voltage to 
																		be considered as under
                                                                        Voltage for that PDO. As per
																		PD specification, desired 
																		range for fixed PDO voltage 
																		is (0.95 &#42; PDO Voltage) 
																		to (1.05 &#42; PDO Volatge),
																		So u8UVThresholdPercentage 
																		should be less than the 
																		desired range.
                                                                      * If 85% of the PDO voltage 
																	    has to be considered as 
																		under voltage for that PDO 
																		voltage, then define
                                                                        u8UVThresholdPercentage as 
																		85.
                                                                      * u8UVThresholdPercentage must
																		be defined when
                                                                        INCLUDE_POWER_FAULT_HANDLING
																		is defined as '1'. As an
                                                                        exceptional case this factor
																		is not considered for 
																		VSafe5V.
    u8VCONNOCSDebounceInms          1         R/W          R         * Debounce timer value in terms
																		of milliseconds for VCONN
                                                                        overcurrent fault conditions
																		before reacting and entering
                                                                        fault recovery routine.
    u8VBUSMaxFaultCnt               1         R/W          R         * The maximum number 
																		of back&#45;to&#45;back VBUS 
																		faults allowed before 
																		permanent shut down of the 
																		port. A back&#45;to&#45;back
																		fault is a second fault 
																		which occurs
                                                                        within the 
																		u16PowerGoodTimerInms after 
																		a port is automatically 
																		reenabled from a previous 
																		fault condition.
                                                                      * During port shutdown due to 
																	    over current fault, the 
																		device removes its CC 
																		termination and wait for 
																		port partner to get detached
																		physically from the port to 
																		resume its normal operation.
    u8VCONNMaxFaultCnt              1         R/W          R         * The maximum number of 
																		back&#45;to&#45;back VCONN 
																		faults allowed before it 
																		permanently disables the 
																		VCONN. A 
																		back&#45;to&#45;back fault 
																		is a second fault which 
																		occurs within the 
																		u16PowerGoodTimerInms after 
																		a port is automatically 
																		reenabled from a previous 
																		fault condition.
                                                                      * If VCONN is disabled due to 
																	    over current VCONN power
                                                                        fault, VCONN will be enabled
																		only after a physical detach
                                                                        and reattach.
    u8Pio_EN_VBUS                   1         R/W          R         * Defines the UPD350 PIO number
																		used for EN_VBUS pin 
																		functionality for the Port.
                                                                      * This variable is applicable 
                                                                        only for source operation.
                                                                      * EN_VBUS is to enable VBUS 
																	    drive out of DC/DC
                                                                        controller. EN_VBUS pin 
																		connects to a load switch 
																		device such as a power FET 
																		or load switch IC. It is 
																		driven as per u8Mode_EN_VBUS
																		configuration mode whenever 
																		stack requires VBUS to 
																		driven high as well as low.
                                                                      * The range of valid values is
 																	    0 to 15 which correspond to
                                                                        UPD350 PIO0 to PIO15.
                                                                      * To disable the pin 
																	    functionality from the 
																		stack, the user can define a
																		value of 0xFF.
                                                                      * By defining     
																	    INCLUDE_UPD_PIO_OVERRIDE_SUPPORT 
																		as '1', the PIO Override 
																		feature of the UPD350 shall 
																		be utilized in this
                                                                        pin to ensure that fast and 
																		autonomous action is taken 
																		by the UPD350 in a fault 
																		condition.
    u8Mode_EN_VBUS                  1         R/W          R         *  Defines the PIO mode of the 
																		 UPD350 PIO EN_VBUS defined 
																		 in u8Pio_EN_VBUS. 
                                                                      * This variable is applicable 
                                                                        only for source operation.
																	  * It takes values only from 
																	    enum 
																		eUPD_OUTPUT_PIN_MODES_TYPE.
																		
	u8Pio_FAULT_IN                  1         R/W          R         *  Defines the UPD PIO used as 
																		 FAULT_IN pin. 
																	  * FAULT_IN pin detects over 
																		current fault or under/over
																		voltage fault from external
																		sensing device based 
																		on its configuration 
																		u8Mode_FAULT_IN. 
																	  * It can take values only 
																		from 0 to 15 and to disable 
																		the pin functionality from 
																		stack, user can define it 
																		as 0xFF. 
																	  * It is applicable only when 
																		INCLUDE_POWER_FAULT_HANDLING
																		defined as '1'. 
	u8Mode_FAULT_IN                 1         R/W          R         * Defines the PIO mode of the 
																	    UPD350 PIO FAULT_IN defined 
																	    in u8Pio_FAULT_IN. 
																	  * It takes values only from 
																		enum 
                                                                        eUPD_INPUT_PIN_MODES_TYPE
	u8Pio_EN_SINK                   1         R/W          R         * Defines the UPD350 PIO 
																		number used for EN_SINK pin.
																	  * This is applicable only for
																		Sink operation.
																	  * This pin is asserted in the 
                                                                        following conditions:
																		1. If the source supports Power
                                                                           delivery, the PD negotiated 
                                                                           current should be greater than
                                                                           or equal to the current 
                                                                           mentioned under 
																		   u16SnkMinOperatingCurInmA 
																		   variable.
                                                                        2. If the source does not support
                                                                           Power delivery and is Type-C only,
                                                                           the Rp value in source partner
                                                                           should be greater than or equal
                                                                           to the current mentioned under 
																		   u16SnkMinOperatingCurInmA 
																		   variable.
																	  * This pin is de-asserted during a
																	    hard reset, a power fault recovery
																		or a detach.
                                                                      * The range of valid values is
 																	    0 to 15 which correspond to
                                                                        UPD350 PIO0 to PIO15.
                                                                      * By defining     
																	    INCLUDE_UPD_PIO_OVERRIDE_SUPPORT 
																		as '1', the PIO Override 
																		feature of the UPD350 shall 
																		be utilized in this
                                                                        pin to ensure that fast and 
																		autonomous action is taken 
																		by the UPD350 in a fault 
																		condition. 
	u8Mode_EN_SINK                  1         R/W          R         * Defines the PIO mode for 
																		EN_SINK pin
																	  * This is applicable only for 
																		Sink operation. 
                                                                      * It takes values only 
																	    from enum 
																		eUPD_OUTPUT_PIN_MODES_TYPE.
	u8DAC_I_Direction               1         R/W          R       	 * Specifies the direction of 
																	    DAC_I to allow user invert 
																		direction of DAC_I if 
																		required 
																		 1. 0 - High Amperage -
   																		      Max Voltage 
																		 2. 1- High Amperage - 
																			  Min Voltage 
																	  * This is applicable only 
																		  for Sink operation.
    u8aSinkCapsExtd[21]             21        R/W          R         * 21-byte Sink Capabilities 
                                                                        Extended Data Block that
                                                                        needs to be sent in 
                                                                        response to a 
                                                                        Get_Sink_Cap_Extended 
                                                                        Message
                                                                      * The contents of the array
                                                                        shall comply with Table 6-60 
                                                                        Sink Capabilities Extended
                                                                        Data Block (SKEDB) of USB 
                                                                        PD 3.0 Specification
                                                                      * This array shall be used 
																	    only when the port is 
																		configured as Sink or DRP
    u8PIO_HPD                       1         R/W          R       	 * Defines the UPD350 PIO 
																		number used for HPD IO pin.
                                                                      * The state of this pin is tracked
                                                                        in u16HPDStatus variable.
																	  * This is applicable only when
																		INCLUDE_UPD_HPD is enabled.
    u8Pio_FRSRequest                1         R/W          R       	 * Defines the UPD350 PIO 
																		number used for FRS Request
                                                                        pin functionality for the 
                                                                        port
                                                                      * This PIO is used to trigger 
                                                                        FRS request signaling upon
                                                                        detection of loss of power 
                                                                        when the port is operating
                                                                        as a Dual Role Source  
                                                                      * This variable is applicable
                                                                        only when 
                                                                        INCLUDE_PD_FR_SWAP is enabled                                                                       
                                                                      * The range of valid values is
 																	    0 to 15 which correspond to
                                                                        UPD350 PIO0 to PIO15 and to
																		disable the pin 
																		functionality, user can
																		define it as 0xFF
                                                                      * By defining     
																	    INCLUDE_UPD_PIO_OVERRIDE_SUPPORT 
																		as '1', the PIO Override 
																		feature of the UPD350 shall 
																		be utilized in this
                                                                        pin to ensure that fast and 
																		autonomous action is taken 
																		by the UPD350 in a Fast Role 
																		Swap condition.   
    u8Mode_FRSRequest               1         R/W          R         * Defines the PIO mode of the 
																		UPD350 PIO FRS_Request
																		defined in u8Pio_FRSRequest 
                                                                      * This variable is applicable 
                                                                        only when INCLUDE_PD_FR_SWAP
                                                                        is enabled
																	  * It takes values only from 
																	    enum 
																		eUPD_INPUT_PIN_MODES_TYPE  
	u8aReserved2  					3								 Reserved	
	u8aReserved3  					3								 Reserved	  
	u8aReserved4  					2								 Reserved	  
    u8aReserved5  					3								 Reserved	    
    u8aReserved8  					2								 Reserved	      
 	u8aReservedPortPadBytes[32]	    32	                             * Reserved bytes included
                                                                        based on configuration macro 
                                                                        INCLUDE_CFG_STRUCT_MEMORY_PAD_REGION 	 		
    </table>
    
    
    
    <b>2. Members that are Bit-Mapped bytes:</b>
    
    <b>a. u32CfgData</b>:
    
    u32CfgData variable holds the Port Configuration Data. It's size is 4 bytes.
    <table>
    Bit     R/W Config   R/W Run   \Description
             time         time      
    ------  -----------  --------  --------------------
    1:0     RW           R         Port Power Role
                                    * '00' Sink
                                    * '01' Source 
                                    * '10' DRP
    2       RW           R         Dual Role Data Capability
                                    * '0' No Dual Role Data
                                    * '1' Dual Role Data supported
    4:3     RW           R         Rp Selection
                                    * '00' Disabled
                                    * '01' USB Power
                                    * '10' 1.5A
                                    * '11' 3.0A 
    5       RW           R         Port Enable/Disable
                                    * '0' Disabled
                                    * '1' Enabled
    8:6     RW           R         USB Data
                                    * '000' No Data
                                    * '001' USB2
                                    * '010' USB3.1 Gen1
                                    * '011' USB3.1 Gen2
    9       RW           R         VCONN OCS Enable
                                    * '0' Disable
                                    * '1' Enable
    10      RW           R         Use New PDOs for negotiation
                                    * '0' Default PDOs provided in
									  gasCfgStatusData.sPerPortData[u8PortNum].u32aSourcePDO 
									  or gasCfgStatusData.sPerPortData[u8PortNum].u32aSinkPDO
                                      will be used depending on the current power role for 
                                      power negotiation.                                 
                                    * '1' New PDOs provided in 
                                      gasCfgStatusData.sPerPortData[u8PortNum].u32aNewSourcePDO 
                                      or gasCfgStatusData.sPerPortData[u8PortNum].u32aNewSinkPDO
                                      will be used depending on the current power role for 
                                      power negotiation.                                  
                                    The first PD negotiation will take place with default PDOs. 
									After the first power negotiation, if user wants PD negotiation
									to happen with new PDOs, the user must ensure that new PDOs
									(gasCfgStatusData.sPerPortData[u8PortNum].u32aNewSourcePDO 
									or gasCfgStatusData.sPerPortData[u8PortNum].u32aNewSinkPDO arrays)
									are configured depending on the current power role and then set this bit.
									Then, further power negotiations will happen based on new PDOs. 
									After power negotiation with new PDOs, if user wants further PD negotiations
									to happen with default PDOs (gasCfgStatusData.sPerPortData[u8PortNum].u32aSourcePDO 
                                    or gasCfgStatusData.sPerPortData[u8PortNum].u32aSinkPDO), this bit can be cleared.
    32:11                          Reserved
    </table>
	
	<b>b. u32PortConnectStatus</b>: 
	u32PortConnectStatus variable holds the connection status of the port. It's size is 4 bytes. 
	<table> 
    Bit     R/W Config   R/W Run   \Description
             time         time      
    ------  -----------  --------  --------------------
    0       R            R         Attached 
                                    * '0' Detached
                                    * '1' Attached
	1       R            R         Orientation 
									* '0' Unflipped - Port Partner attached in CC1 pin 
									* '1' Flipped - Port Partner attached in CC2 pin 
	3:2     R            R         Data Role 
									* '00' UFP 
									* '01' DFP 
                                    * '10' Toggling 
                                    * '11' Reserved
	5:4     R            R         Power Role 
									* '00' Sink 
									* '01' Source 
                                    * '10' DRP 
                                    * '11' Reserved
	6       R            R         VCONN Status  
									* '0' Disabled 
									* '1' Enabled 
	7       R            R         Cable Reduced Source Capabilities 
									* '0' Attached USB-C cable supports the locally-defined Source 
									    PDOs
									* '1' Attached USB-C cable does not support the locally defined 
									    Source PDOs	
	8       R            R         Reduced Source Capabilities
									* '0' The advertised PDOs are equivalent to the default 
									     configured values 
									* '1' The advertised PDOs have been reduced from default 
									     configured values
	9       R            R         Source Capability Mismatch 
									* '0' De-asserted by  Source port when there is capability 
									     mismatch with sink partner 
									* '1' Asserted by Source port when sink port indicates 
									     capability mismatch in RDO
	10      R            R         As Source PD Contract Good 
									* '0' As Source: USB-C Connection Only (No Request Made Yet)
									* '1' As Source; USB PD connection established, Power request 
									      has been made, accepted and PS_RDY message sent. 
								    * This bit will always remain 0 when acting as sink.	
	11      R            R         As Source RDO Accepted
									* '0' As Source: No RDO Accept message has been sent to last 
									      Request made by attached Sink or no Request has yet been 
										  made during connection. 
									* '1' As Source: RDO Accept message has been sent to last 
									      Request made by attached Sink
								    * This bit will always remain 0 when acting as sink		  
	12      R            R         As Source RDO Rejected  
									* '0' As source; No RDO reject message has been sent to last 
											request made by attached Sink or no Request has yet been 
											made during connection 
									* '1' As Source: RDO Reject message has been sent to last 
									        Request made by attached Sink
									* This bit will always remain 0 when acting as Sink 		
	13      R            R         As Sink Last Request Accept 
									* '0' As Sink: Last RDO Request was not Accepted or no request 
									      has yet been made. 
									* '1' As Sink: Last RDO Request was Accepted
									* This bit will always remain 0 when acting as a source.
	14      R            R         As Sink Last Request Reject 
									* '0' As Sink: Last RDO Request was not Rejected or no request 
									   has yet been made. 
									* '1' As Sink: Last RDO Request was Rejected
									* This bit will always remain 0 when acting as a source.
	15      R            R         As Sink Last Request PS_RDY 
									* '0' As Sink: PS_RDY not yet received for last RDO request  
									* '1' As Sink: PS_RDY received for last RDO request
									* This bit will always remain 0 when acting as a source.
	16      R            R         Sink Capability Mismatch  
									* '0' De-asserted by the Sink Port when there is no capability 
										mismatch 
									* '1' Asserted by Sink Port when no Source capability was found
	18:17   R            R         Rp Value detected by Sink 
									* '00' Disabled 
									* '01' USB Power 
								    * '10' 1.5A 
									* '11' 3.0A 
    20:19   R            R         Current Negotiated PD Specification Revision									
									* '01' PD2.0 
                                    * '10' PD3.0   
                                    * '00' & '11' - Reserved 
	31:21	 			           Reserved 				
	</table>

	<b>c. u32PortIOStatus</b>: 
	u32PortIOStatus variable holds the IO status of the port. It's size is 4 bytes. 
	<table> 
    Bit     R/W Config   R/W Run   \Description
             time         time      
    ------  -----------  --------  --------------------
    0       R            R         EN_DC_DC Status  
                                    * '1' Asserted 
                                    * '0' De-asserted 
    1       R            R         VSEL0 Status  
                                    * '1' Asserted 
                                    * '0' De-asserted
    2       R            R         VSEL1 Status  
                                    * '1' Asserted 
                                    * '0' De-asserted
    3       R            R         VSEL2 Status  
                                    * '1' Asserted 
                                    * '0' De-asserted
    4       R            R         EN_VBUS Status  
                                    * '1' Asserted 
                                    * '0' De-asserted
    5       R            R         VBUS_DIS Status  
                                    * '1' Asserted 
                                    * '0' De-asserted
    6       R            R         EN_SINK Status  
                                    * '1' Asserted 
                                    * '0' De-asserted
    7       R            R         1.5_IND Status  
                                    * '1' Asserted 
                                    * '0' De-asserted
    8       R            R         3.0_IND Status  
                                    * '1' Asserted 
                                    * '0' De-asserted
    9       R            R         Capability Mismatch Status
                                    * '1' Asserted 
                                    * '0' De-asserted
    10      R            R         Power role Status
                                    * '1' Asserted if Source
                                    * '0' De-asserted if Sink
									* Applicable only for DRP configuration									
    11      R            R         Data role Status
                                    * '1' Asserted if DFP
                                    * '0' De-asserted if UFP
									* Applicable only for DRP configuration	
    31:12                          Reserved 
	</table>
	
	<b>d. u32PortStatusChange</b>: 
	u32PortStatusChange variable defines the port connection status change bits. It's size is 4 
	bytes. 
	<table> 
    Bit     R/W Config   R/W Run   \Description
             time         time      
    ------  -----------  --------  --------------------
    0       R            R         Attach Event 
                                    * '0' Since the last read of this register, PSF has not 
									    experienced a USB-C attach
                                    * '1' Since the last read of this register, PSF has experienced 
									    a USB-C attach	
    1       R            R         Detach Event 
                                    * '0' Since the last read of this register, PSF has not 
									    experienced a USB-C detach 
                                    * '1' Since the last read of this register, PSF has experienced 
									    a USB-C detach 
    2       R            R         As Source New Request  
                                    * '0' As Source, since the last read of this register, PSF has 
									     not received any new PDO request from attached port partner
                                    * '1' As Source: Received a new PDO request the attached Sink 
									     port partner, As Sink: Received an updated set of Source 
										 capabilities form the attached Source port partner
    3       R            R         As Sink New PDOs Received  
                                    * '0' As Sink: Since the last read of this register, PSF has not
										received any changed source capabilities
                                    * '1' As Sink: Received an updated set of Source capabilities 
									    form the attached Source port partner
    4       R            R         As Sink New Request Sent  
                                    * '0' As Sink: Since the last read of this register, PSF has not
										 sent any additional Sink RDOs
                                    * '1' As Sink: Since the last read of this register, PSF has 
									     issued a new Sink RDO to the attached Source, 
								    * This bit will always remain 0 when acting as a source
    5       R            R         As Sink Last Request Accept 
                                    * '0' As Sink: Since the last read of this register, PSF has not
									    received any new Request Accept messages from the attached 
										Source
                                    * '1' As Sink: Since the last read of this register, PSF has 
									     received a new Request Accept from the attached Source
    6       R            R         As Sink Last Request Reject 
                                    * '0' As Sink: Since the last read of this register, PSF has not
										received any new Request Reject messages from the attached 
										Source
                                    * '1' As Sink: Since the last read of this register, PSF has 
									     received a new Request Reject message from the attached 
										 Source
    7       R            R         As Sink Last Sink PS Rdy 
                                    * '0' As Sink: Since the last read of this register, PSF has not
											received any PS_RDY messages from the attached Source
                                    * '1' As Sink: Since the last read of this register, PSF has 
									        received a PS_RDY message from the attached Source
    8       R            R         Hard Reset Event 
                                    * '0' Since the last read of this register, PSF has not 
									      experienced a USB PD Hard Reset
                                    * '1' Since the last read of this register, PSF has experienced 
									      a USB PD Hard Reset			
    9       R            R         Pin Reset Event 
                                    * '0' Since the last read of this register, PSF has not been 
									      reset via POR or pin
                                    * '1' Since the last read of this register, PSF has been reset 
									      via POR or pin
    10      R            R         VBUS Fault 
                                    * '0' Since the last read of this register, no VBUS faults have 
									      been detected
                                    * '1' Since the last read of this register, 1 or more VBUS 
									      faults have been detected
    11      R            R         VCONN Fault  
                                    * '0' Since the last read of this register, no VCONN faults have
      									been detected
                                    * '1' Since the last read of this register, 1 or more VCONN 
										faults have been detected										  
	31:12                  		   Reserved 
	</table> 	
		
	<b>e. u32ClientRequest</b>: 
	u32ClientRequest variable defines the client request mask bits. It's size is 4 bytes. 
    USER_APPLICATION can set the corresponding bit in this variable to request PSF to handle
    the client requests mentioned in the table below. Only one client request can be handled 
    by PSF at a given time. If more than one client request bits are set at the same time, 
    the requests will be queued internally and processed based on the priority of events where
    bit 0 takes the highest priority and bit 31 takes the least priority. If the request is 
    accepted and processed, a response notification will be posted by PSF as mentioned in the 
    below table.
	<table> 
    Bit     R/W Config   R/W Run   \Description
             time         time      
    ------  -----------  --------  --------------------
    0       R/W          R/W       Port Disable Request 
                                    * Set this bit to request PSF to disable a port.
                                    * This client request will be processed by PSF 
                                       irrespective of whether it is idle.
                                    * Once a port is disabled successfully, 
                                       eMCHP_PSF_PORT_DISABLED notification will be posted
                                       by PSF to user application.
    1       R/W          R/W       Port Enable Request 
                                    * Set this bit to request PSF to enable a port.
                                    * This client request will be processed by PSF 
                                       irrespective of whether it is idle.
                                     * Once a port is disabled successfully, 
                                       eMCHP_PSF_PORT_ENABLED notification will be posted
                                       by PSF to user application.
    2       R/W          R/W       Handle VBUS Power Fault Over voltage Request 
                                    * Set this bit to request PSF to process externally detected
                                        over voltage VBUS fault.
    3       R/W          R/W       Handle VBUS Power Fault Over Current Request   
                                    * Set this bit to request PSF to process externally detected
                                        over current VBUS power fault or to inform PSF that Current
                                        Limit mode is entered by external DC-DC controller.  
    4       R/W          R/W       Handle VBUS Power Fault Over Current Exit Request 
                                    * Set this bit to inform PSF that externally detected 
                                        over current VBUS power fault condition is exited or 
                                        Constant Voltage mode is entered by external DC-DC controller.
    5        R/W          R/W      Respond to a Vendor Defined Message 
                                    * Set this bit to respond to a VDM request received from 
                                       port partner
                                    * Reception of a VDM request from partner will be notified 
                                       through eMCHP_PSF_VDM_REQUEST_RCVD notification. VDM Header,
                                       received from partner will be stored in u32PartnerVDMHeader 
                                       variable of sVDMPerPortData structure, VDOs and number of 
                                       VDOs will be stored in u32aPartnerVDO array and                                        
                                       u8PartnerVDOCnt variable of sAltModePerPortData structure 
                                       respectively. 
                                    * User Application shall ensure that the VDM Header is 
                                       configured in u32VDMHeader variable of sVDMPerPortData 
                                       structure and VDOs in the u32aVDO array and VDOs 
                                       count in u8VDOCnt of sAltModePerPortData structure
                                       while raising this client request
                                    * eMCHP_PSF_VDM_AMS_COMPLETE notification will be posted
                                        when Good CRC is received for the response sent       
                                    * This client request is supported only when 
                                       INCLUDE_PD_ALT_MODE is defined as '1'.    
    6       R/W          R/W       Renegotiation Request 
                                    * Set this bit to request PSF to trigger renegotiation  
                                    * User application may request PSF to renegotiate 
                                       based on default PDOs
                                       (gasCfgStatusData.sPerPortData[u8PortNum].u32aSourcePDO 
                                       or gasCfgStatusData.sPerPortData[u8PortNum].u32aSinkPDO)
                                       or New PDOs 
                                       (gasCfgStatusData.sPerPortData[u8PortNum].u32aNewSourcePDO 
                                       or gasCfgStatusData.sPerPortData[u8PortNum].u32aNewSinkPDO).
                                       To renegotiate with default PDOs, user application must ensure 
                                       that BIT(10) in 
                                       gasCfgStatusData.sPerPortData[u8PortNum].u32CfgData 
                                       variable is cleared and then BIT(6) in 
                                       gasCfgStatusData.sPerPortData[u8PortNum].u32ClientRequest
                                       is set. To renegotiate with New PDOs, user application must 
                                       ensure that the New PDOs                                        
                                       (gasCfgStatusData.sPerPortData[u8PortNum].u32aNewSourcePDO 
                                       or gasCfgStatusData.sPerPortData[u8PortNum].u32aNewSinkPDO) 
                                       are configured, BIT(10) in 
                                       gasCfgStatusData.sPerPortData[u8PortNum].u32CfgData variable
                                       is set and BIT(6) in 
                                       gasCfgStatusData.sPerPortData[u8PortNum].u32ClientRequest 
                                       is set
									* Once the request is processed by PSF, 
                                       eMCHP_PSF_PD_CONTRACT_NEGOTIATED notification will be posted.
									* Note: This client request is not applicable when Power 
                                       Balancing or Power Throttling is enabled. Therefore, the
									   user application should not trigger this client request 
                                       when Power Balancing or Power Throttling is enabled.
    7       R/W          R/W       Initiate a VCONN Swap 
                                    * Set this bit to request PSF to initiate a VCONN Swap
                                    * User Application shall ensure that either of the VCONN Swap 
                                       Policy bits (Bit 8 or Bit 9) is set in  
                                       gasCfgStatusData.sPerPortData[u8PortNum].u16SwapPolicy
                                       variable while raising this client request
                                    * eMCHP_PSF_VCONN_SWAP_COMPLETE notification will be posted 
                                       for an Accept/Reject response, and 
                                       eMCHP_PSF_VCONN_SWAP_NO_RESPONSE_RCVD will be posted
                                       when there is no response for VCONN Swap from the partner
                                    * This client request is supported only when  
                                       INCLUDE_PD_VCONN_SWAP is defined as '1'. 
    8       R/W          R/W       Initiate a Power Role Swap 
                                    * Set this bit to request PSF to initiate a PR_Swap
                                    * User Application shall ensure that either of the PR_Swap
                                       Policy bits (Bit 4 or Bit 5) is set in 
                                       gasCfgStatusData.sPerPortData[u8PortNum].u16SwapPolicy
                                       variable while raising this client request
                                    * eMCHP_PSF_PR_SWAP_COMPLETE notification will be posted 
                                       for an Accept/Reject response, and 
                                       eMCHP_PSF_PR_SWAP_NO_RESPONSE_RCVD will be posted
                                       when there is no response for the PR_Swap from the partner
                                    * This client request is supported only when  
                                       INCLUDE_PD_PR_SWAP is defined as '1'. 
    9       R/W          R/W       Initiate a Data Role Swap 
                                    * Set this bit to request PSF to initiate a DR_Swap
                                    * User Application shall ensure that either of the DR_Swap
                                       policy bits (Bit 0 or Bit 1) is set in 
                                       gasCfgStatusData.sPerPortData[u8PortNum].u16SwapPolicy
                                       variable while raising this client request
                                    * eMCHP_PSF_DR_SWAP_COMPLETE notification will be posted 
                                       for an Accept/Reject response, and 
                                       eMCHP_PSF_DR_SWAP_NO_RESPONSE_RCVD will be posted
                                       when there is no response for the DR_Swap from the partner
                                    * This client request is supported only when  
                                       INCLUDE_PD_DR_SWAP is defined as '1'. 
    10       R/W          R/W      Initiate a Structured Vendor Defined Message        
                                    * Set this bit to request PSF to initiate a VDM request 
                                       to port partner
                                    * User Application shall ensure that the VDM Header is 
                                       configured in u32VDMHeader variable of sVDMPerPortData 
                                       structure and VDOs in the u32aVDO array and VDOs 
                                       count in u8VDOCnt of sAltModePerPortData structure
                                       while raising this client request
                                    * eMCHP_PSF_VDM_RESPONSE_RCVD notification will be posted for
                                       an ACK/NAK response, and eMCHP_PSF_VDM_RESPONSE_NOT_RCVD
                                       will be posted when no response is received.
                                    * eMCHP_PSF_VDM_AMS_COMPLETE notification will be posted 
                                       when Good CRC is received for the VDM requests that does
                                       not consist of a command response, say Attention command
                                    * This client request is supported only when INCLUDE_PD_VDM 
                                       is defined as '1'. 
    11       R/W          R/W      Hot Plug Detect (HPD) disable request        
                                    * Set this bit to request PSF to disable HPD support. 
                                    * This client request is supported only when INCLUDE_UPD_HPD 
                                       is defined as '1'. 
                                    * eMCHP_PSF_HPD_DISABLED notification will be posted once
                                       HPD peripheral is disabled.
    12       R/W          R/W      Hot Plug Detect (HPD) enable request        
                                    * Set this bit to request PSF to enable HPD support. 
                                    * This client request is supported only when INCLUDE_UPD_HPD 
                                       is defined as '1'. 
                                    * eMCHP_PSF_HPD_ENABLED notification will be posted once
                                       HPD peripheral is enabled.

	31:13  						   Reserved 									
	</table> 								
 
	<b>f. u16PortIntrMask</b>: 
	u16PortIntrMask variable defines the port interrupt mask bits. It's size is 2 bytes. 
	<table> 
    Bit     R/W Config   R/W Run   \Description
             time         time      
    ------  -----------  --------  --------------------
    0       RC           RC        Attach Event Mask 
                                    * '0' Do not mask interrupt pin toggle on changes to this event.
                                    * '1' Mask this event from generating interrupt pin toggle.	
    1       RC           RC        Detach Event Mask 
                                    * '0' Do not mask interrupt pin toggle on changes to this event.
                                    * '1' Mask this event from generating interrupt pin toggle.	
    2       RC           RC        As Source New Request Mask 
                                    * '0' Do not mask interrupt pin toggle on changes to this event.
                                    * '1' Mask this event from generating interrupt pin toggle.	
    3       RC           RC        As Sink New PDOs Received Mask 
                                    * '0' Do not mask interrupt pin toggle on changes to this event.
                                    * '1' Mask this event from generating interrupt pin toggle.	
    4       RC           RC        As Sink New Request Sent Mask 
                                    * '0' Do not mask interrupt pin toggle on changes to this event.
                                    * '1' Mask this event from generating interrupt pin toggle.	
    5       RC           RC        As Sink Last Request Accept Mask 
                                    * '0' Do not mask interrupt pin toggle on changes to this event.
                                    * '1' Mask this event from generating interrupt pin toggle.	
    6       RC           RC        As Sink Last Request Reject Mask 
                                    * '0' Do not mask interrupt pin toggle on changes to this event.
                                    * '1' Mask this event from generating interrupt pin toggle.	
    7       RC           RC        As Sink Last Sink PS RDY Mask 
                                    * '0' Do not mask interrupt pin toggle on changes to this event.
                                    * '1' Mask this event from generating interrupt pin toggle.	
    8       RC           RC        Hard Reset Event Mask 
                                    * '0' Do not mask interrupt pin toggle on changes to this event.
                                    * '1' Mask this event from generating interrupt pin toggle.	
    9       RC           RC        Pin Reset Mask 
                                    * '0' Do not mask interrupt pin toggle on changes to this event.
                                    * '1' Mask this event from generating interrupt pin toggle.	
    10      RC           RC        VBUS Fault Mask 
                                    * '0' Do not mask interrupt pin toggle on changes to this event.
                                    * '1' Mask this event from generating interrupt pin toggle.	
    11      RC           RC        VCONN Fault Mask 
                                    * '0' Do not mask interrupt pin toggle on changes to this event.
                                    * '1' Mask this event from generating interrupt pin toggle.
    15:12  						   Reserved 									
	</table> 								
 
	<b>g. u16FeatureSelect</b>: 
	u16FeatureSelect variable defines the enable/disable of various PSF features. It's size is 2 
	bytes. 
	<table> 
    Bit     R/W Config   R/W Run   \Description
             time         time      
    ------  -----------  --------  --------------------
    0       R/W          R         Power Balancing Enable/Disable 
                                    * '0' Disable.
                                    * '1' Enable. 
								    This bit is applicable only for source operation. 			
    1       R/W          R         Indicates if Vendor Defined Message is supported
                                    by the port.
                                    * '0' - Vendor Defined Messages are not supported.
                                       The port will respond with 'Not Supported' on
                                       reception of a Vendor Defined Message.  
                                    * '1' - Vendor Defined Messages are supported.
                                       The port will respond with relevant VDM data on
                                       reception of a Vendor Defined Message.    
    2       R/W          R         AltMode Entry Monitoring Enable/Disable 
                                    * '0' - The port will not monitor AltMode Entry
                                    * '1' - The port will monitor AltMode Entry and indicate
                                       the failure when AMETimer times out         
    15:3	                       Reserved 
    </table>
    
    <b>h. u16SwapPolicy</b>: 
	u16SwapPolicy defines the policy of a port whether to accept, request or reject Power Role Swap,
    Data Role Swap and VCONN Swap based on its power and data roles.
	<table> 
    Bit     R/W Config   R/W Run   \Description
             time         time      
    ------  -----------  --------  --------------------
    0       R/W          R/W       EN_AUTO_DR_SWAP_REQUEST_AS_DFP
                                    * '0' Disable Auto Data Role Request When Data Role is DFP
                                    * '1' Enable Auto Data Role Request when Data Role is DFP 
    1       R/W          R/W       EN_AUTO_DR_SWAP_REQUEST_AS_UFP
                                    * '0' Disable Auto Data Role Request When Data Role is UFP
                                    * '1' Enable Auto Data Role Request when Data Role is UFP
    2       R/W          R/W       EN_AUTO_DR_SWAP_ACCEPT_AS_DFP
                                    * '0' Disable Auto Data Role Accept When Data Role is DFP
                                    * '1' Enable Auto Data Role Accept when Data Role is DFP 
    3       R/W          R/W       EN_AUTO_DR_SWAP_ACCEPT_AS_UFP
                                    * '0' Disable Auto Data Role Accept When Data Role is UFP
                                    * '1' Enable Auto Data Role Accept when Data Role is UFP
    4       R/W          R/W       EN_AUTO_PR_SWAP_REQUEST_AS_SOURCE
                                    * '0' Disable Auto Power Role Request When Power Role is Source
                                    * '1' Enable Auto Power Role Request when Power Role is Source
    5       R/W          R/W       EN_AUTO_PR_SWAP_REQUEST_AS_SINK
                                    * '0' Disable Auto Power Role Request When Power Role is Sink
                                    * '1' Enable Auto Power Role Request when Power Role is Sink
    6       R/W          R/W       EN_AUTO_PR_SWAP_ACCEPT_AS_SOURCE
                                    * '0' Disable Auto Power Role Accept When Power Role is Source
                                    * '1' Enable Auto Power Role Accept when Power Role is Source
    7       R/W          R/W       EN_AUTO_PR_SWAP_ACCEPT_AS_SINK
                                    * '0' Disable Auto Power Role Accept When Power Role is Sink
                                    * '1' Enable Auto Power Role Accept when Power Role is Sink 
    8       R/W          R/W       EN_AUTO_VCONN_SWAP_REQ_AS_VCONN_SRC
                                    * '0' Disable Auto VCONN Swap Request When working as VCONN Source
                                    * '1' Enable Auto VCONN Swap Request When working as VCONN Source
    9       R/W          R/W       EN_AUTO_VCONN_SWAP_REQ_AS_NOT_VCONN_SRC
                                    * '0' Disable Auto VCONN Swap Request When not working as VCONN Source
                                    * '1' Enable Auto VCONN Swap Request When not working as VCONN Source
    10      R/W          R/W       EN_AUTO_VCONN_SWAP_ACCEPT_AS_VCONN_SRC
                                    * '0' Disable Auto VCONN Swap Accept When working as VCONN Source
                                    * '1' Enable Auto VCONN Swap Accept When working as VCONN Source
                                    * Note: This bit shall be set to 1 always to comply with the PD spec for 
                                      VCONN Swap request									
    11      R/W          R/W       EN_AUTO_VCONN_SWAP_ACCEPT_AS_NOT_VCONN_SRC
                                    * '0' Disable Auto VCONN Swap Accept When not working as VCONN Source
                                    * '1' Enable Auto VCONN Swap Accept When not working as VCONN Source 
    15:12  						   Reserved 									
	</table> 

   Remarks:
     None                                                                                                                             
  ***************************************************************************************************************************************/

typedef struct _PortCfgStatus
{
    UINT32 u32CfgData;				
    UINT32 u32aAdvertisedPDO[7];	
    UINT32 u32aPartnerSourcePDO[7];
    UINT32 u32aPartnerSinkPDO[7];
    UINT32 u32RDO;                  
	UINT32 u32PortConnectStatus;	
    UINT32 u32PortStatusChange;
    UINT32 u32PortIOStatus;
    UINT32 u32ClientRequest; 
    UINT16 u16AllocatedPowerIn250mW;   
    UINT16 u16NegoVoltageInmV;      
    UINT16 u16NegoCurrentInmA;      
    UINT16 u16MaxSrcPrtCurrentIn10mA;     
    UINT16 u16PortIntrMask;
    UINT16 u16PowerGoodTimerInms;
    UINT16 u16FeatureSelect; 
    UINT16 u16SwapPolicy; 
	#if (TRUE == INCLUDE_PD_SINK)
    UINT16 u16aMinPDOPreferredCurInmA[7]; 
    UINT16 u16SnkMaxOperatingCurInmA; 
    UINT16 u16SnkMinOperatingCurInmA;
    UINT16 u16DAC_I_MaxOutVoltInmV; 
    UINT16 u16DAC_I_MinOutVoltInmV;
	UINT16 u16DAC_I_CurrentInd_MaxInA; 
    #endif
	UINT8 u8AdvertisedPDOCnt; 		
    UINT8 u8PartnerSinkPDOCnt;
    UINT8 u8PartnerSourcePDOCnt;  
    UINT8 u8SinkConfigSel;         
    UINT8 u8FaultInDebounceInms;    
    UINT8 u8OCSThresholdPercentage; 
    UINT8 u8OVThresholdPercentage;  
    UINT8 u8UVThresholdPercentage;	
    UINT8 u8VCONNOCSDebounceInms;
    UINT8 u8VBUSMaxFaultCnt;
    UINT8 u8VCONNMaxFaultCnt;
    UINT8 u8Pio_FAULT_IN;
    UINT8 u8Mode_FAULT_IN;  
    UINT8 u8aReserved2[3]; 
#if (TRUE == INCLUDE_PD_SOURCE)
	UINT32 u32aSourcePDO[7];
	UINT32 u32aNewSourcePDO[7];	
    UINT32 u32aCableIdentity[6];
	UINT8 u8SourcePDOCnt;
	UINT8 u8NewSourcePDOCnt;
    UINT8 u8CableIdentityCnt; 
    UINT8 u8Pio_EN_VBUS;
    UINT8 u8Mode_EN_VBUS;
    UINT8 u8aReserved3[3];
#endif
#if (TRUE == INCLUDE_PD_SINK)
	UINT32 u32aSinkPDO[7];
	UINT32 u32aNewSinkPDO[7];	
	UINT8 u8SinkPDOCnt;
	UINT8 u8NewSinkPDOCnt;
    UINT8 u8Pio_EN_SINK; 
    UINT8 u8Mode_EN_SINK; 
    UINT8 u8DAC_I_Direction;
    UINT8 u8aSinkCapsExtd[21];
	UINT8 u8aReserved4[2];    
#endif
#if (TRUE == INCLUDE_UPD_HPD)    
    UINT8 u8PIO_HPD;
    UINT8 u8aReserved5[3];
#endif
#if (TRUE == INCLUDE_PD_FR_SWAP)
    UINT8 u8Pio_FRSRequest; 
    UINT8 u8Mode_FRSRequest; 
    UINT8 u8aReserved8[2]; 
#endif 
#if (TRUE == INCLUDE_CFG_STRUCT_MEMORY_PAD_REGION)
    UINT8 u8aReservedPortPadBytes[32];
#endif
   } PORT_CFG_STATUS, *PPORT_CFG_STATUS;

 /**********************************************************************
   Summary:
     This structure contains port specific Power Balancing Configuration parameters.
	 sPBPerPortData is referred from _GlobalCfgStatusData.
   Description:
	 This structure contains global configuration and status parameters that are either Integer 
	 data types, Bit-Mapped bytes or other data structure. This structure is used only when either
	 of the macros INCLUDE_POWER_BALANCING or INCLUDE_POWER_THROTTLING is set to '1'.
	
    <b>1. Members that are Integer data types:</b> 
	
	<table> 	
    Name                            Size in   R/W Config   R/W Run   \Description
                                     Bytes     time         time      
    ------------------------------  --------  -----------  --------  -------------------------------------------------------------------
    u16MaxPrtPwrBankAIn250mW        2         R/W          R         * Maximum Port Power Bank A in 
																		0.25W steps. 
																	  * Unit/LSB = 0.25W
																	  * Sample values this variable
																	    can take are, 
																		1. 0x0001 = 0.25W 
																		2. 0x00F0 = 60W 
																		3. 0x0190 = 100W 
																	  * Note : A setting of 0x0000 
																		and 0x191-0xFFF is invalid.
    u16MaxPrtPwrBankBIn250mW        2         R/W          R         * Maximum Port Power Bank B in 
																		0.25W steps. 
																	  * Unit/LSB = 0.25W
																	  * Sample values this variable
																	    can take are, 
																		1. 0x0001 = 0.25W 
																		2. 0x00F0 = 60W 
																		3. 0x0190 = 100W 
																	  * Note : A setting of 0x0000 
																		and 0x191-0xFFF is invalid.	
    u16MaxPrtPwrBankCIn250mW        2         R/W          R         * Maximum Port Power Bank A in 
																		0.25W steps. 
																	  * Unit/LSB = 0.25W
																	  * Sample values this variable
																	    can take are, 
																		1. 0x0001 = 0.25W 
																		2. 0x00F0 = 60W 
																		3. 0x0190 = 100W 
																	  * Note : A setting of 0x0000 
																		and 0x191-0xFFF is invalid.	
    u8PortPriority                  1         R/W          R         * Selects the port priority 
                                                                      * 000b is the highest priority
	u8Reserved2 					1						          Reserved 											
	</table>	
									
   Remarks:
     None                                                               
   **********************************************************************/

#if ((TRUE == INCLUDE_POWER_BALANCING) || (TRUE == INCLUDE_POWER_THROTTLING)) 

typedef struct _PBPortCfgStatus
{
    UINT16 u16MaxPrtPwrBankAIn250mW; 
    UINT16 u16MaxPrtPwrBankBIn250mW; 
    UINT16 u16MaxPrtPwrBankCIn250mW; 
    UINT8 u8PortPriority; 
    UINT8 u8Reserved2;
} PB_PORT_CFG_STATUS, *PPB_PORT_CFG_STATUS;

#endif 
 /**********************************************************************
   Summary:
     This structure contains port specific VDM Configuration and Status parameters. 
	 sVDMPerPortData is referred from _GlobalCfgStatusData.
   Description:
     This structure contains the following parameters that 
     are either Integer Datatypes or Bit-Mapped bytes.  
	 This structure is used only when INCLUDE_PD_VDM is set to '1'.
	 
	<b>1. Members that are Integer Datatypes:</b> 
	
	<table> 	
    Name                            Size in   R/W Config   R/W Run   \Description
                                     Bytes     time         time      
    ------------------------------  --------  -----------  --------  -------------------------------------------------------------------	
    u32VDMHeader                    4         R/W          R/W       * VDM Header used while 
                                                                        sending a VDM to port 
                                                                        partner
                                                                      * The fields of this variable
                                                                         shall comply with 
                                                                         Table 6-25: Structured VDM 
                                                                         Header of PD Specification
    u32PartnerVDMHeader             4         R            R         * VDM Header sent by partner                                                                         
    u32aPartnerPDIdentity[6]        24        R            R         * Partner Identities received
                                                                        in response to a Discover
                                                                        Identity request. This
                                                                        array can hold upto 6 VDM 
                                                                        Data Objects where 
                                                                        Index 0 corresponds to ID                                                                        
                                                                        Header VDO, Index 1 being
                                                                        Cert Stat VDO, Index 2 
                                                                        being Product VDO and 
                                                                        indices 3-5 correspond to 
                                                                        0-3 Product Type VDO(s)
    u32aPDIdentity[6]               24        R/W           R        * Port PD Identities to be 
                                                                        sent in Discover
                                                                        Identity response. This
                                                                        array can hold upto 6 VDM 
                                                                        Data Objects where 
                                                                        Index 0 corresponds to ID                                                                        
                                                                        Header VDO, Index 1 being
                                                                        Cert Stat VDO, Index 2 
                                                                        being Product VDO and 
                                                                        indices 3-5 correspond to 
                                                                        0-3 Product Type VDO(s)   
    u8PartnerPDIdentityCnt          1         R            R         * Number of Identities received
                                                                        from partner in response to 
                                                                        a Discover Identity request
    u8PDIdentityCnt                 1         R/W          R         * Number of PD Identities of
                                                                        the port that needs to be 
                                                                        sent in response to a
                                                                        Discover Identity request   
    u8aReserved7                    2                                Reserved 
	</table> 

   Remarks:
     None                                                               
   **********************************************************************/
#if (TRUE == INCLUDE_PD_VDM)   

typedef struct _VDMPortCfgStatus
{
    UINT32 u32VDMHeader;  
    UINT32 u32PartnerVDMHeader; 
    UINT32 u32aPartnerPDIdentity[6];
    UINT32 u32aPDIdentity[6];  
    UINT8 u8PDIdentityCnt; 
    UINT8 u8PartnerPDIdentityCnt; 
    UINT8 u8aReserved7[2];     
} VDM_PORT_CFG_STATUS, *PVDM_PORT_CFG_STATUS;

#endif 
/**********************************************************************
   Summary:
     This structure contains port specific Alternate Mode Configuration and 
     Status parameters. sAltModePerPortData is referred from _GlobalCfgStatusData.
   Description:
     This structure contains the following parameters that 
     are either Integer Datatypes or Bit-Mapped bytes.  
	 This structure is used only when INCLUDE_PD_ALT_MODE is set to '1'.
	 
	<b>1. Members that are Integer Datatypes:</b> 
	
	<table> 	
    Name                            Size in   R/W Config   R/W Run   \Description
                                     Bytes     time         time      
    ------------------------------  --------  -----------  --------  -------------------------------------------------------------------	
    u32aModesTable                  64        R/W          R         * List of Modes corresponding
                                                                        to each supported SVID 
    u32aVDO                         24        R/W          R/W       * This array contains VDOs 
                                                                        received from partner
                                                                        during an Enter Mode
                                                                        request and other SVID 
                                                                        specific commands.
                                                                     * Application can make use of
                                                                        this array to send the VDOs
                                                                        to partner while initiating
                                                                        or responding to Enter Mode
                                                                        and other SVID specific 
                                                                        commands
    u32aPartnerVDO                  24        R            R         * This array contains VDOs 
                                                                        received from partner
                                                                        during an Enter Mode
                                                                        request and other SVID 
                                                                        specific commands.  
    u16aSVIDsTable                  32        R/W          R         * List of SVIDs supported by
                                                                        the port
    u8aSVIDEntryTable               16        R/W          R         * SVID Entry table where 
                                                                        in every index, 
                                                                        Bits 2:0 ? No of Modes for
                                                                        an SVID
                                                                        Bits 6:3 ? Start Mode Index
                                                                        The index into the mode 
                                                                        table for the first mode
                                                                        for this SVID. The 
                                                                        allocation of modes in the
                                                                        table starts from this 
                                                                        field up to the value in No
                                                                        of Modes.
                                                                        Bit 7 ? Reserved 
    u8SVIDsCnt                      1         R/W          R         * Number of entries stored in
                                                                        u16aSVIDsTable                  
    u8VDOCnt                        1         R/W          R/W       * Number of VDOs to be sent  
                                                                        from u32aVDO   
    u8PartnerVDOCnt                 1         R            R         * Number of VDOs stored in 
                                                                        u32aPartnerVDO which are
                                                                        received from partner
    u8Reserved3                     1                                Reserved 
	</table> 

   Remarks:
     None                                                               
   **********************************************************************/
#if (TRUE == INCLUDE_PD_ALT_MODE)

typedef struct _AltModePortCfgStatus
{
    UINT32 u32aModesTable[16];
    UINT32 u32aVDO[6]; 
    UINT32 u32aPartnerVDO[6];
    UINT16 u16aSVIDsTable[16];
    UINT8 u8aSVIDEntryTable[16];
	UINT8 u8SVIDsCnt; 	        
    UINT8 u8VDOCnt; 
    UINT8 u8PartnerVDOCnt;     
    UINT8 u8Reserved3; 
} ALT_MODE_PORT_CFG_STATUS, *PALT_MODE_PORT_CFG_STATUS;

#endif 
 /**********************************************************************
   Summary:
     This structure contains port specific status parameters.
	 sPPSPerPortData is referred from _GlobalCfgStatusData.
   Description:
     This structure contains the following status parameters that 
     are either Integer Datatypes or Bit-Mapped bytes.  
	 This structure is used only when INCLUDE_PD_SOURCE_PPS is set to '1'.
	 
	<b>1. Members that are Integer Datatypes:</b> 
	
	<table> 	
    Name                            Size in   R/W Config   R/W Run   \Description
                                     Bytes     time         time      
    ------------------------------  --------  -----------  --------  -------------------------------------------------------------------	
    u32PartnerAlert                 4         R            R         * Complete Alert information 
																		received from Partner, Will 
																		be blank of no Alert has 
																		been received.
                                                                      * This variable is common for 
																	    Source and Sink.	
	u8aPartnerStatus[6]             6         R            R         * Store the Status information 
																		received from Port Partner. 
																	  * This array would hold a valid 
																		value if 
																		eMCHP_PSF_SINK_STATUS_RCVD
																		notification is posted. It 
																		would be 0 when 
																		eMCHP_PSF_SINK_STATUS_NOT_RCVD
																		notification is posted. 
    u8aReserved6[2]				    2                                Reserved 
	</table> 

   Remarks:
     None                                                               
   **********************************************************************/
#if (TRUE == INCLUDE_PD_SOURCE_PPS)   

typedef struct _PPSPortCfgStatus
{
    UINT32 u32PartnerAlert; 
    UINT8 u8aPartnerStatus[6];
    UINT8 u8aReserved6[2];
} PPS_PORT_CFG_STATUS, *PPPS_PORT_CFG_STATUS;

#endif 
  /**********************************************************************
   Summary:
	This structure contains the system level, Port specific configuration and status
	parameters of PSF. gasCfgStatusData is the defined variable of this structure.
   Description:
     This structure contains global configuration and status parameters that are either Integer 
	 data types, Bit-Mapped bytes or other data structure.
	
    <b>1. Members that are Integer data types:</b> 
	
	<table> 	
    Name                            Size in   R/W Config   R/W Run   \Description
                                     Bytes     time         time      
    ------------------------------  --------  -----------  --------  -------------------------------------------------------------------
    u8MinorVersion               	1         R/W          R         Defines Structure Minor Version 
	
	u8MajorVersion               	1         R/W          R         Defines Structure Major Version 
     
	u8HWVersion                  	1         R/W          R         Defines Hardware Version  

	u8SiVersion                  	1         R/W          R         Defines Silicon Version 

	u8aManfString[8]               	1         R/W          R         Defines Manufacturer String 

	u8PSFMajorVersion               1         R/W          R         Defines PSF Stack Major Version 

	u8PSFMinorVersion               1         R/W          R         Defines PSF Stack Minor Version 

	u16ProductID               	    2         R/W          R         * Defines the Product Identifier 
																		Value. 
																	  * It is used by the PD 
																		Firmware Update state 
																		machine during Enumeration 
																		phase. This information is 
																		shared with the PDFU 
																		initiator as part of 
																		GET_FW_ID command's response. 
																	  * User definition of this 
																	    macro is mandatory when 
																		INCLUDE_PDFU is defined as 1. 
																		It should always be two byte
																		wide. 
	u16VendorID               	    2         R/W          R         * Defines the Vendor Identifier 
																		Value. 
																	  * It is used by the PD 
																		Firmware Update state 
																		machine during Enumeration 
																		phase. This information is 
																		shared with the PDFU 
																		initiator as part of 
																		GET_FW_ID command's response. 
																	  * User definition of this 
																	    macro is mandatory when 
																		INCLUDE_PDFU is defined as 1. 
																		It should always be two byte wide. 															   
	u16SystemPowerBankAIn250mW 	    2         R/W          R         * Defines the Total System 
																		Power of Bank A. Each unit 
																		is 0.25W 
																	  * Sample values are, 
																		1. 0x0001 = 0.25W 
																		2. 0x01E0 = 120W
																	    3. 0x0320 = 200W 
																	  * A setting of 0x0000 and 
																	    0x0321-0xFFFF is invalid. 													 
																	  *	This variable is used only 
																		when either of 
																		INCLUDE_POWER_BALANCING or 
																		INCLUDE_POWER_THROTTLING is 
																		set to '1'. 
	u16MinPowerBankAIn250mW    	    2         R/W          R         * Defines the Guaranteed  
																		minimum Power of Bank A. 
																		Each unit is 0.25W 
																	  * Sample values are, 
																		1. 0x0001 = 0.25W 
																		2. 0x003C = 15W
																	    3. 0x0190 = 100W 
																	  * A setting of 0x0000 and 
																	    0x0191-0xFFFF is invalid.
																	  *	This variable is used only 
																		when either of 
																		INCLUDE_POWER_BALANCING or 
																		INCLUDE_POWER_THROTTLING is 
																		set to '1'.	
																		
	u16SystemPowerBankBIn250mW 	    2         R/W          R         * Defines the Total System 
																		Power of Bank B. Each unit 
																		is 0.25W 
																	  * Sample values are, 
																		1. 0x0001 = 0.25W 
																		2. 0x01E0 = 120W
																	    3. 0x0320 = 200W 
																	  * A setting of 0x0000 and 
																	    0x0321-0xFFFF is invalid. 
																	  *	This variable is used only 
																		when either of 
																		INCLUDE_POWER_BALANCING or 
																		INCLUDE_POWER_THROTTLING is 
																		set to '1'.																
																		
	u16MinPowerBankBIn250mW    	    2         R/W          R         * Defines the Guaranteed  
																		minimum Power of Bank B. 
																		Each unit is 0.25W 
																	  * Sample values are, 
																		1. 0x0001 = 0.25W 
																		2. 0x003C = 15W
																	    3. 0x0190 = 100W 
																	  * A setting of 0x0000 and 
																	    0x0191-0xFFFF is invalid.
																	  *	This variable is used only 
																		when either of 
																		INCLUDE_POWER_BALANCING or 
																		INCLUDE_POWER_THROTTLING is 
																		set to '1'.
																		
	u16SystemPowerBankCIn250mW 	    2         R/W          R         * Defines the Total System 
																		Power of Bank C. Each unit 
																		is 0.25W 
																	  * Sample values are, 
																		1. 0x0001 = 0.25W 
																		2. 0x01E0 = 120W
																	    3. 0x0320 = 200W 
																	  * A setting of 0x0000 and 
																	    0x0321-0xFFFF is invalid.
																	  *	This variable is used only 
																		when either of 
																		INCLUDE_POWER_BALANCING or 
																		INCLUDE_POWER_THROTTLING is 
																		set to '1'.
																		
	u16MinPowerBankCIn250mW    	    2         R/W          R         * Defines the Guaranteed  
																		minimum Power of Bank C. 
																		Each unit is 0.25W 
																	  * Sample values are, 
																		1. 0x0001 = 0.25W 
																		2. 0x003C = 15W
																	    3. 0x0190 = 100W 
																	  * A setting of 0x0000 and 
																	    0x0191-0xFFFF is invalid.
																	  *	This variable is used only 
																		when either of 
																		INCLUDE_POWER_BALANCING or 
																		INCLUDE_POWER_THROTTLING is 
																		set to '1'.
																		
	u16SharedPwrCapacityIn250mW     2         R            R         * Defines the currently 
																		available shared power 
																		capacity from which power 
																		is allocated to ports that 
																		are not yet in a valid 
																		negotiated contract. 
																	  *	The shared power capacity is 
																		dynamically adjusted as 
																		ports are connected and 
																		disconnected. 
																		Each unit is 0.25W 
																	  * Sample values are, 
																		1. 0x0001 = 0.25W 
																		2. 0x003C = 15W
																	    3. 0x0190 = 100W 
																	  *	This variable is used only 
																		when either of 
																		INCLUDE_POWER_BALANCING or 
																		INCLUDE_POWER_THROTTLING is 
																		set to '1'.
    u8aReserved1				     3 								 Reserved 	
    u8Reserved1				         1 								 Reserved 	
 	u8aReservedPadBytes[16]	         16	                             * Reserved bytes included
                                                                        based on configuration macro 
                                                                        INCLUDE_CFG_STRUCT_MEMORY_PAD_REGION 	 		
																		
	</table> 															  										

    <b>2. Members that are Bit-Mapped bytes:</b>

    <b>a. u8PBEnableSelect</b>:
    
    u8PBEnableSelect defines PB Enable/Disable for the system and also the Power Balancing algorithm. 
	It's size is 1 byte. 
    <table>
    Bit     R/W Config   R/W Run   \Description
             time         time      
    ------  -----------  --------  --------------------
    3:0     R/W          R         Selection of Power Balancing Algorithm
                                    * 0000 = First Come First Serve
                                    * 0001 = Last Plugged Gets Priority
									* 0010-1111 = Reserved
	4       R/W          R         Power balancing Enable/Disable for the system								
									* 0 = PD Balancing is disabled 
									* 1 = PD Balancing is enabled
    7:5                            Reserved  									
	</table>								
	
    <b>b. u8PwrThrottleCfg</b>:
    
    u8PwrThrottleCfg defines the Power Throttle Enable/Disable configuration and 
    currently selected Power Bank. It's size is 1 byte. 
    <table>
    Bit     R/W Config   R/W Run   \Description
             time         time      
    ------  -----------  --------  --------------------
	0       R/W          R         Power Throttle Enable/Disable for the system								
									* 0 = Disable Power Throttling
									* 1 = Enable Power Throttling
    2:1     R/W          R/W       Selection of Power Throttling Bank
                                    * 00 = Bank A 
									* 01 = Bank B
									* 10 = Bank C 
									* 11 = Shutdown mode
    7:3                            Reserved  									
	</table>								
 
	<b>3. Members that are another structures:</b>
	<table>
    Structure        \Description     
    ------           --------------------
	sPerPortData     Includes Type C and PD parameters of a port, like default Source PDOs, default 
					  Sink PDOs, currently negotiated voltage and current values, under voltage and 
					  over voltage threshold values, etc., Tag for this structure is _PortCfgStatus.
	sPPSPerPortData  Includes PPS parameters of a port, like partner alert and status.
						Tag for this structure is _PPSPortCfgStatus. 
	sPBPerPortData   Includes Power Balancing parameters of a port, like maximum power and maximum 
						current. Tag for this structure is _PBPortCfgStatus.						
 									
	</table>
	
   Remarks:
     None                                                               
   **********************************************************************/

typedef struct _GlobalCfgStatusData 
{
    UINT8 u8MinorVersion;  
    UINT8 u8MajorVersion;  
    UINT8 u8HWVersion;	   
    UINT8 u8SiVersion;     
    UINT8 u8aManfString[21];
    UINT8 u8ManfStringLength;
    UINT8 u8PSFMajorVersion; 
    UINT8 u8PSFMinorVersion; 
    UINT8 u8PwrThrottleCfg;
    UINT8 u8aReserved1[3];    
    UINT16 u16ProductID;	
    UINT16 u16VendorID;		
    
    PORT_CFG_STATUS sPerPortData[CONFIG_PD_PORT_COUNT]; 
    
#if (TRUE == INCLUDE_POWER_BALANCING || (TRUE == INCLUDE_POWER_THROTTLING))
    UINT16 u16SharedPwrCapacityIn250mW;
    UINT8 u8PBEnableSelect;	    
    UINT8 u8Reserved1; 
    UINT16 u16SystemPowerBankAIn250mW; 
    UINT16 u16MinPowerBankAIn250mW;   
    UINT16 u16SystemPowerBankBIn250mW; 
    UINT16 u16MinPowerBankBIn250mW;   
    UINT16 u16SystemPowerBankCIn250mW; 
    UINT16 u16MinPowerBankCIn250mW;       
    PB_PORT_CFG_STATUS sPBPerPortData[CONFIG_PD_PORT_COUNT];	
#endif 
    
#if (TRUE == INCLUDE_PD_SOURCE_PPS)
    PPS_PORT_CFG_STATUS sPPSPerPortData[CONFIG_PD_PORT_COUNT]; 
#endif
    
#if (TRUE == INCLUDE_PD_VDM)
    VDM_PORT_CFG_STATUS sVDMPerPortData[CONFIG_PD_PORT_COUNT]; 
#endif 

#if (TRUE == INCLUDE_PD_ALT_MODE)    
    ALT_MODE_PORT_CFG_STATUS sAltModePerPortData[CONFIG_PD_PORT_COUNT]; 
#endif 
    
#if (TRUE == INCLUDE_CFG_STRUCT_MEMORY_PAD_REGION)
    UINT8 u8aReservedPadBytes[16];
#endif
} GLOBAL_CFG_STATUS_DATA, * PGLOBAL_CFG_STATUS_DATA;

 /**********************************************************************
   Summary:
     This is an instance of _GlobalCfgStatusData.
   Description:
     gasCfgStatusData is the global structure which defines the overall system level configuration 
	 and status parameters of PSF. This structure contains the system level and port specific 
	 Configuration and Status parameters of PSF including Type C, PD, PB, PT and PPS parameters.
	 
	 It is mandatory that the user has to initialize the configuration parameters for the PSF 
	 stack to function properly. This can be done through MCHP_PSF_HOOK_BOOT_TIME_CONFIG which 
	 initializes the parameters defined in gasCfgStatusData during compile time. For accessing 
	 the configuration registers and reading the status registers at run time, an I2C slave
	 interface shall be used by the user application. 																 												  																 																  
   Remarks:
     None                                                               
   **********************************************************************/
   
extern GLOBAL_CFG_STATUS_DATA gasCfgStatusData;   

#include "PSFDrp_App.h"
#include "PSFDrp_BootCfg.h"

#endif
