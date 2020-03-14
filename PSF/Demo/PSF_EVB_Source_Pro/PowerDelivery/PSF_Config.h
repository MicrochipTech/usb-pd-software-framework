/*******************************************************************************
  PSF stack configuration

  Company:
    Microchip Technology Inc.

  File Name:
    PSF_Config.h

  Summary:
    PSF configuration header file

  Description:
    This header file contains the configuration parameters of PSF stack to configure the 
    Power delivery modules.
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
	at the compile. User can set this define to 0 to reduce code size, if none of the PD enabled 
	ports require PD 3.0 specific features.
Remarks:
    Recommended default value is '1'.
Example:
    <code>
    #define INCLUDE_PD_3_0	1(Include USB PD 3.0 specific features to PSF)
    #define INCLUDE_PD_3_0	0(Exclude USB PD 3.0 specific features from PSF)
    </code>
**************************************************************************************************/
#define INCLUDE_PD_3_0                    1

/**************************************************************************************************
Summary:
    Source support code inclusion.
Description:
    Setting the INCLUDE_PD_SOURCE as 1 enables PSF to include the USB PD Source functionality
    at compile time. User can set this define to 0 to reduce code size if none of the PD 
    enabled ports in the system are configured for Source operation.
Remarks: 
    Recommended default value is '1' for Source Application.
Example:
    <code>
    #define INCLUDE_PD_SOURCE	1(Include USB PD Source functionality in PSF)
    #define INCLUDE_PD_SOURCE	0(Exclude USB PD Source functionality from PSF)
    </code>
**************************************************************************************************/
#define INCLUDE_PD_SOURCE  		1

/**************************************************************************************************
Summary:
    Sink support code inclusion.
Description:
    Setting the INCLUDE_PD_SINK as 1 enables PSF to include USB PD Sink functionality at the 
	compile time. User can set this define to 0 to reduce code size if none of the PD enabled ports 
	are configured for Sink operation.
Remarks:
    Recommended default value is '1' for Sink Application.
Example:
    <code>
    #define INCLUDE_PD_SINK	1(Include USB PD Sink functionality in PSF)
    #define INCLUDE_PD_SINK	0(Exclude USB PD Sink functionality from PSF)
    </code>
**************************************************************************************************/
#define INCLUDE_PD_SINK    		1

/**************************************************************************************************
Summary:
    VCONN Support code inclusion.
Description:
    Setting the INCLUDE_VCONN_SWAP_SUPPORT as 1 enables PSF to include the VCONN Swap
	functionality at the compile time. User can set this define to 0 to reduce code size if none of
	the PD enabled ports requires VCONN Swap functionality.
Remarks:
    Recommended default value is 1. For Source Operation, it is mandatory to define this macro as '1'.
    When INCLUDE_PD_SOURCE is defined as '1', define this macro as '1'.
Example:
    <code>
    #define INCLUDE_VCONN_SWAP_SUPPORT	1(Include VCONN Swap functionality in PSF)
    #define INCLUDE_VCONN_SWAP_SUPPORT	0(Exclude VCONN Swap functionality from PSF)
    </code>
**************************************************************************************************/
#define INCLUDE_VCONN_SWAP_SUPPORT  	1

/**************************************************************************************************
Summary:
    Power Fault Handling code inclusion.
Description:
    Setting the INCLUDE_POWER_FAULT_HANDLING as 1 enables PSF to handle power faults (Source and 
	Sink over voltage, Source OCS, Sink under voltage) as per Power Delivery specification Rev3.0 as
	applicable. User can set this define to 0 to reduce code size if PSF based power fault handling
	is not required.
Remarks:
    Recommended default value is 1.
Example:
    <code>
    #define INCLUDE_POWER_FAULT_HANDLING	1(Include Power Fault handling to PSF)
    #define INCLUDE_POWER_FAULT_HANDLING	0(Exclude Power Fault handling from PSF )
    </code>
**************************************************************************************************/
#define INCLUDE_POWER_FAULT_HANDLING          1

/**************************************************************************************************
Summary:
    PIO Override Feature code inclusion.
Description:
    PIO override is UPD350 specific feature which changes the state of a PIO without software
    intervention. PSF use this feature to disable EN_VBUS instantly on detection of a 
    Power Fault Condition. Setting the INCLUDE_UPD_PIO_OVERRIDE_SUPPORT as 1 enables this feature.
    User can set this define to 0 to reduce code size of PSF if PIO override based 
    power faulting is not required.
Remarks:
    To use this feature, EN_VBUS and FAULT_IN Pin of the system should be UPD350 PIOs.
    It is also confined to INCLUDE_POWER_FAULT_HANDLING define, thus INCLUDE_POWER_FAULT_HANDLING
    should be declared as 1 for INCLUDE_UPD_PIO_OVERRIDE_SUPPORT define to be effective. 
    Recommended default value is 1 if UPD350 PIOs are used for EN_VBUS and FAULT_IN.
Example:
    <code>
    #define INCLUDE_UPD_PIO_OVERRIDE_SUPPORT	1(Include UPD350 PIO Override support for Power 
                                                        fault to PSF)
    #define INCLUDE_UPD_PIO_OVERRIDE_SUPPORT	0(Exclude UPD350 PIO Override support for Power 
                                                        fault from PSF)
    </code>
**************************************************************************************************/
#define INCLUDE_UPD_PIO_OVERRIDE_SUPPORT      1

/**************************************************************************************************
Summary:
    Power Management Control Support code inclusion.
Description:
    Setting the INCLUDE_POWER_MANAGEMENT_CTRL as 1 enables PSF to include the 
    functionality that puts the UPD350 into low power mode if UPD350 is inactive for 
    CONFIG_PORT_UPD_IDLE_TIMEOUT_MS time and PSF notifies the same via the call back
    MCHP_PSF_NOTIFY_CALL_BACK. User can set this define to 0 to reduce code size of the PSF 
    if low power mode operation of UPD350 is not required for the application.
Remarks:
    Recommended default value is 1.
Example:
    <code>
    #define INCLUDE_POWER_MANAGEMENT_CTRL	1(Include power management feature)
    #define INCLUDE_POWER_MANAGEMENT_CTRL	0(Exclude power management feature)
    </code>
**************************************************************************************************/
#define INCLUDE_POWER_MANAGEMENT_CTRL         0

/**************************************************************************
Summary:
    PD Firmware update code inclusion.
Description:
    Setting the INCLUDE_PDFU as 1 includes the state machine code for PD Firmware Update 
    feature as per USB Power Delivery FW Update Specification v1.0. User can set this define 
    to 0 to reduce code size if the PSF application does not use Firmware update feature. 
Remarks:
    Recommended default value is 0 unless Firmware update feature is used. It is mandatory to have 
    INCLUDE_PD_3_0 is defined as '1' when INCLUDE_PDFU is '1'.
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
    Power Balancing functionality at compile time. User can set this define to 0
    to reduce code size if none of the PD enabled Source ports in the system 
    require Power Balancing functionality.
Remarks: 
    Recommended default value is 1. For INCLUDE_POWER_BALANCING to be 1, 
    INCLUDE_PD_SOURCE shall be set to 1. 
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
    Power Throttling(PT) feature at compile time. User can set this define to 0
    to reduce code size if none of the Source ports in the system 
    require PT functionality.
Remarks: 
    Recommended default value is 1. For INCLUDE_POWER_THROTTLING to be 1, 
    INCLUDE_PD_SOURCE shall be set to 1. 
Example:
    <code>
    #define INCLUDE_POWER_THROTTLING	1(Include PT functionality in PSF)
    #define INCLUDE_POWER_THROTTLING	0(Exclude PT functionality from PSF)
    </code>
**************************************************************************************************/
#define INCLUDE_POWER_THROTTLING        0 

/**************************************************************************************************
Summary:
    PPS support code inclusion.
Description:
    Setting the INCLUDE_PD_SOURCE_PPS as 1 enables PSF to include the Programmable 
    Power Supply(PPS) feature at compile time. User can set this define to 0
    to reduce code size if none of the Source ports in the system 
    require PPS functionality.
Remarks: 
    Recommended default value is 1. For INCLUDE_PD_SOURCE_PPS to be 1, 
    INCLUDE_PD_SOURCE shall be set to 1. 
Example:
    <code>
    #define INCLUDE_PD_SOURCE_PPS	1(Include PPS functionality in PSF)
    #define INCLUDE_PD_SOURCE_PPS	0(Exclude PPS functionality from PSF)
    </code>
**************************************************************************************************/
#define INCLUDE_PD_SOURCE_PPS           0 

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
 #define CONFIG_DEFINE_UPD350_HW_INTF_SEL         CONFIG_UPD350_SPI

/**************************************************************************************************
Summary:
    UPD350 Idle Timeout value in milliseconds.
Description :
    CONFIG_PORT_UPD_IDLE_TIMEOUT_MS is the idle time after which UPD350 is put to low power mode by   
    the power management control if there is no activity or interrupt in UPD350.
Remarks :
    It shall be expressed in MILLISECONDS_TO_TICKS define.
    CONFIG_PORT_UPD_IDLE_TIMEOUT_MS is valid only if INCLUDE_POWER_MANAGEMENT_CTRL set as 1.
    By default, this define is set to to 15seconds.
Example :
    <code>
     #define CONFIG_PORT_UPD_IDLE_TIMEOUT_MS	  MILLISECONDS_TO_TICKS(15000) (Timeout is 15 seconds)
    </code>
**************************************************************************************************/
#define CONFIG_PORT_UPD_IDLE_TIMEOUT_MS 	MILLISECONDS_TO_TICKS(15000)

// *****************************************************************************
// *****************************************************************************
// Section: DC-DC Buck boost controller configurations
// *****************************************************************************
// *****************************************************************************
/**************************************************************************
Summary:
    DC DC Buck Boost Controller default configuration option.
Description:
	CONFIG_DCDC_CTRL is to define the default DC-DC control provided by the PSF stack. If 
	CONFIG_DCDC_CTRL defined as PWRCTRL_DEFAULT_PSF_GPIO_CONFIG, default GPIO based DC-DC controller
	is used. If CONFIG_DCDC_CTRL is defined as I2C_DC_DC_CONTROL_CONFIG, default I2C based 
    DC-DC Controller is used. If left undefined, default stack's DC-DC control option is not used 
    and the user must control power via power control APIs provided by the stack.  
Remarks:
	None.
Example:
	<code>
	#define CONFIG_DCDC_CTRL    PWRCTRL_DEFAULT_PSF_GPIO_CONFIG (Uses default GPIO based DC-DC contol)
	#define CONFIG_DCDC_CTRL    I2C_DC_DC_CONTROL_CONFIG (Uses default I2C based DC-DC contol)
	#define CONFIG_DCDC_CTRL    (If undefined, Default DC DC control provided by stack is not used)
	</code>                                  
  **************************************************************************/
#define CONFIG_DCDC_CTRL        I2C_DC_DC_CONTROL_CONFIG
					
/**************************************************************************
Summary:
    Default I2C DC DC Controller Type.
Description:
	CONFIG_I2C_DCDC_TYPE is to define the default I2C DC-DC control provided by the PSF stack.
    This macro is valid only when CONFIG_DCDC_CTRL is set to I2C_DC_DC_CONTROL_CONFIG. If 
	CONFIG_I2C_DCDC_TYPE defined as MPQ, Monolithic MPQ4230 I2C DC-DC Controller 
    is used. If CONFIG_I2C_DCDC_TYPE defined as ONSEMI, On Semi I2C DC-DC 
    Controller is used. 
Remarks:
	None.
Example:
	<code>
	#define CONFIG_I2C_DCDC_TYPE    MPQ (Uses Monolithic I2C DC-DC contol)
    #define CONFIG_I2C_DCDC_TYPE    ONSEMI (Uses OnSemi I2C DC-DC contol)	
	</code>                                  
  **************************************************************************/
#define CONFIG_I2C_DCDC_TYPE        MPQ

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

#include "PSFSourcePro_App.h"
#include "PSFSourcePro_BootCfg.h"

#endif
