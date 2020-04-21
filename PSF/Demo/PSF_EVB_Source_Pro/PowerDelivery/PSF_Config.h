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
#define INCLUDE_PD_SINK    		0

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
    INCLUDE_PD_SOURCE and INCLUDE_PD_3_0 shall be set to 1. 
Example:
    <code>
    #define INCLUDE_PD_SOURCE_PPS	1(Include PPS functionality in PSF)
    #define INCLUDE_PD_SOURCE_PPS	0(Exclude PPS functionality from PSF)
    </code>
**************************************************************************************************/
#define INCLUDE_PD_SOURCE_PPS           1

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
    Macro to indicate GPIO based DC-DC Controller. 
Description:
	PWRCTRL_DEFAULT_PSF_GPIO_CONFIG defines the default GPIO based DC-DC Controller used by PSF.  
Remarks:
	None.                                 
  **************************************************************************/
#define PWRCTRL_DEFAULT_PSF_GPIO_CONFIG     1

/**************************************************************************
Summary:
    Macro to indicate I2C based DC-DC Controller. 
Description:
	I2C_DC_DC_CONTROL_CONFIG defines the default GPIO based DC-DC Controller used by PSF.  
Remarks:
	None.                                 
  **************************************************************************/
#define I2C_DC_DC_CONTROL_CONFIG            2


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
#define CONFIG_HOOK_DEBUG_MSG                       0

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

// *****************************************************************************
// *****************************************************************************
// Section: PIO configurations
// *****************************************************************************
// *****************************************************************************
/**************************************************************************************************
Summary:
    UPD350 GPIO Output mode enum.
Description:
	eUPD_OUTPUT_PIN_MODES_TYPE enum defines the various combination modes applicable for UPD350 GPIO
    in output mode.
Remarks:
        None
**************************************************************************************************/
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
    UPD350 Fault_IN GPIO mode enum.
Description:
	eFAULT_IN_MODE_TYPE enum defines the various combination modes applicable for UPD350 GPIO
    in input mode.
Remarks:
    None
**************************************************************************************************/
typedef enum
{
    eFAULT_IN_ACTIVE_LOW         = 0x20U,   //Active low input signal
    eFAULT_IN_ACTIVE_HIGH        = 0x10U,   //Active high input signal
    eFAULT_IN_ACTIVE_LOW_PU      = 0xA0U,   //Active low signal with internal pull up
    eFAULT_IN_ACTIVE_HIGH_PD     = 0x50U    //Active high signal with internal pull down
}eFAULT_IN_MODE_TYPE;

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
	Datatypes, Bit-Mapped bytes or another structure.
    
	<b>1. Members that are Integer data types:</b>
	
    <table>
    Name                            Size in   R/W Config   R/W Run   \Description
                                     Bytes     time         time      
    ------------------------------  --------  -----------  --------  -------------------------------------------------------------------
    u32aSourcePDO[7]                28        R/W          R         * Upto 7 fixed Source PDOs 
																		where Voltage is specified 
																		in mV and Current is 
																		specified in mA.
                                                                      * This array should be used 
																	    for Source Operation. 
    u32aSinkPDO[7]                  28        R/W          R         * Upto 7 fixed Sink PDOs where 
																		Voltage is specified in mV
                                                                        and Current is specified in 
																		mA.
                                                                      * This array should be used 
																	    for Sink operation.
    u32aNewPDO[7]                   28        R/W          R/W       * Upto 7 fixed New PDOs where 
																		Voltage is specified in mV 
																		and Current in mA.
                                                                      * This array is common for 
																	    Source and Sink. It is 
																		valid only when Bit 0 of  
																		u8ClientRequest is set to 1.
    u32aAdvertisedPDO[7]            28        R            R         * Upto 7 PDOs that are 
																		advertised to Port Partner. 
                                                                      * During run time, this array 
																	    holds the value of current
                                                                        u32aNewPDO[7] if Bit 0 of 
																		u8ClientRequest is enabled 
																		else holds the value of 
																		current u32aSourcePDO[7]
    u32aPartnerPDO[7]               28        R            R         * Upto 7 fixed Source PDOs 
																		where Voltage is specified 
																		in mV and Current is 
																		specified in mA
                                                                      * This array is common for 
																	    Source and Sink.
    u32RDO                          4         R            R         * Complete raw RDO Data as 
																		Sent/Requested by connected 
																		port partner, Will be blank 
																		of no RDO has been received.
                                                                      * This variable is common for 
																	    Source and Sink.
    u16AllocatedPowerIn250mW        2         R            R         * Allocated Power for the Port 
																		PD contract in 0.25W steps
    u16NegoVoltageIn50mV            2         R            R         * Negotiated Voltage from the 
																		Port Bits 19:10 from the RDO.
                                                                        Voltage is in 50mV steps. 
																		Possible values are,
                                                                        1. 0x00 = No Contract
                                                                        2. 0x064 = 5V
                                                                        3. 0x0B4 = 9 V
                                                                        4. 0x12C = 15V
                                                                        5. 0x190= 20V
                                                                        6. 0x3FF = 51.15V
    u16NegoCurrentIn10mA            2         R            R         * Negotiated Current from the 
																		Port Bits 9: 0 from the RDO.
                                                                        Ampere is in 10mA steps. 
																		Sample values are,
                                                                        1. 0x0000 = No Contract
                                                                        2. 0x012C = 3A
                                                                        3. 0x01F4 = 5A
                                                                        4. 0x03FF = 10.24A
    u16MaximumOperatingCurInmA      2         R/W          R         * Maximum allowable current or 
																		system's maximum operating
                                                                        current in terms of mA
    u16aMinPDOPreferredCurInmA[7]   14        R/W          R         * Preferred minimum current 
																		range for the PDO by which 
																		the Sink may select without 
																		setting Capability Mismatch 
																		Bit with highest current 
																		preferred.
                                                                      * This array is applicable 
																	    only for Sink Operation. 
                                                                        
    u16MinimumOperatingCurInmA      2         R/W          R         * Minimum operating current by 
																	    the System.
                                                                      * This variable is applicable 
																	    only for Sink Operation. 
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
    u8NewPDOCnt                     1         R/W          R/W       * Number of New PDOs Supported.
                                                                      * This variable is common for 
																	    both Source and Sink. It is
																		valid only when Bit 0 of 
																		u8ClientRequest is set to 1.
    u8AdvertisedPDOCnt              1         R            R         * Number of PDOs advertised to 
																		port partner.
    u8PartnerPDOCnt                 1         R            R         * Number of PDOs received from 
																		port partner.
                                                                      * This variable is common for 
																	    Source and Sink.
    u8SinkConfigSel                 1         R/W                    * Sink Selection mode for 
																		operation.
                                                                        1. '0x00' Mode A: Prefer 
																			  Higher Voltage and 
																			  Wattage
                                                                        2. '0x01' Mode B: Prefer 
																		      Lower Voltage and 
																			  Wattage
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
                                                                      * EN_VBUS is to enable VBUS 
																	    drive out of DC/DC
                                                                        controller. EN_VBUS pin 
																		connects to a load switch 
																		device such as a power FET 
																		or load switch IC. It is 
																		driven as per u8Mode_EN_VBUS configuration mode whenever 
																		stack requires VBUS to 
																		driven high as well as low.
                                                                      * The range of valid values is
 																	    0 to 15 which correspond to
                                                                        UPD350 PIO0 to PIO15.
                                                                      * To disable the pin 
																	    functionality from the 
																		stack, the user can define a
																		value of 0xFF. It is 
																		applicable only when
                                                                        CONFIG_DCDC_CTRL is defined 
																		as
                                                                        PWRCTRL_DEFAULT_PSF_GPIO_CONFIG 
																		and for Source operation
                                                                        only.
                                                                      * By defining     
																	    INCLUDE_UPD_PIO_OVERRIDE_SUPPORT 
																		as '1', The PIO Override 
																		feature of the UPD350 shall 
																		be utilized in this
                                                                        pin to ensure that fast and 
																		autonomous action is taken 
																		by the UPD350 in a fault 
																		condition.
    u8Mode_EN_VBUS                  1         R/W          R         *  Defines the PIO mode of the 
																		 UPD350 PIO EN_VBUS defined 
																		 in u8Pio_EN_VBUS. 
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
																		the pin funtionality from 
																		stack, user can define it 
																		as 0xFF. 
																	  * It is applicable only when 
																		CONFIG_DCDC_CTRL is 
																		defined as 
																		PWRCTRL_DEFAULT_PSF_GPIO_CONFIG
																		and 
																		INCLUDE_POWER_FAULT_HANDLING
																		defined as '1'. 
	u8Mode_FAULT_IN                 1         R/W          R         * Defines the PIO mode of the 
																	    UPD350 PIO FAULT_IN defined 
																	    in u8Pio_FAULT_IN. 
																	  * It takes only values from 
																		enum eFAULT_IN_MODE_TYPE.
	u8Pio_VBUS_DIS                  1         R/W          R         * Defines the UPD350 PIO for 
																		VBUS Discharge functionality. 
																	  * It is a control for 
																	    discharging VBUS (connecting
																		VBUS to GND). It asserts as 
																		per u8mode_VBUS_DIS whenever
																		VBUS voltage must transition
																		from a high voltage to a 
																		lower voltage state and 
																		when VBUS is disabled. 
																	  * The range of valid values 
																	    are 0 to 15 which correspond
																		to UPD350 PIO0 to PIO15. 
																	  * To disable the pin 
																		functionality from the stack,
																		user can define a value of 
																		0cFF. 
																	  * It is applicable only when 
																		CONFIG_DCDC_CTRL is 
																		defined as 
																		PWRCTRL_DEFAULT_PSF_GPIO_CONFIG
	u8mode_VBUS_DIS                 1         R/W          R         * Defines the PIO mode of the 
																		UPD350 PIO VBUS_DIS defined 
																		in u8Pio_VBUS_DIS. 
																	  * It takes values only from
																		enum 
																		eUPD_OUTPUT_PIN_MODES_TYPE
	u8Pio_DC_DC_EN                  1         R/W          R         * Defines the UPD350 PIO to 
															            enable DC-DC controller. 
																	  * It is asserted as per 
																	    u8Mode_DC_DC_EN during 
																		initialization and 
																		deasserted during error
																		condition to reset the 
																		DC-DC controller. 
																	  * It can take values from 0
																	    to 15 and to disable the 
																		funtionality from stack, 
																		user can define it as 0xFF. 
																	  * It is applicable only when 
																	    CONFIG_DCDC_CTRL is defined 
																		as 
																		PWRCTRL_DEFAULT_PSF_GPIO_CONFIG
	u8Mode_DC_DC_EN                 1         R/W          R         * Defines the PIO mode of the 
																	    UPD350 PIO DC_DC_EN defined 
																	    in u8Pio_DC_DC_EN. 
																	  * It takes values only 
																	    from enum 
																		eUPD_OUTPUT_PIN_MODES_TYPE
	u8aPio_VSEL[3]                  3         R/W          R         * Defines the UPD350 PIO as 
																	    voltage selector pins
																	    (VSEL[2:0]). 
																	  * PSF Stack provides provision
																	    for three Voltage selector 
																		pins VSEL[2:0]. 
																	  * It is used to control the 
																	    output voltage of the DC/DC
																		controller. In a typical 
																		application, these pins are
																		used to switch in different 
																		resistors into the 
																		feedback loop to vary the 
																		output voltage. 
																	  * The range of valid values is
																		0 to 15 which correspond to
																		UPD350 PIO0 to PIO15. 
																	  * To disable the pin 
																		funtionality from the stack,
																		user can define a value of 
																		0xFF. Index 0 to 2 of this 
																		array correponds to VSEL0 to
																		VSEL2. 
																	  * It is applicable only when
																	    CONFIG_DCDC_CTRL is defined 
																		as 
																		PWRCTRL_DEFAULT_PSF_GPIO_CONFIG
																		
	u8aMode_VSEL[3]                 3         R/W          R         * Defines the PIO mode of the 
																		UPD350 PIO VSEL pins VSEL0
																		to VSEL2. 
																	  * It takes values only from 
																		enum 
																		eUPD_OUTPUT_PIN_MODES_TYPE.
	u8aVSELTruthTable[8]            8         R/W          R         * Index 0 defines the assertion 
																		and deassertion to be driven
                                                                        on VSEL[2:0] pins(defined in 
																		u8aPio_VSEL[3]) by the PSF 
																		as per u8aMode_VSEL[3] to 
																		have an output voltage of 
																		VSafe0V out of DC/DC 
																		controller.
                                                                      * Index 1 to 7 defines the 
																	    assertion and deassertion to
																		be driven on VSEL[2:0] pins
																		(defined in u8aPio_VSEL[3]) 
																		by the PSF stack as per 
																		u8aMode_VSEL[3] to have an 
																		output voltage of PDO 
																		voltage defined in 
																		u32aSourcePDO[7] out of 
																		DC/DC controller.
                                                                      * It is applicable only for 
																	    Source.
                                                                      * For a 1 pin per voltage 
																	    implementation,
																		correponding VSEL
                                                                        mapping would be,
                                                                        1. '000' 5V (No pins 
																			 asserted)
                                                                        2. '001' 9V (VSEL0 
																		     asserted)
                                                                        3. '010' 15V (VSEL1 
																		     asserted)
                                                                        4. '100' 20V (VSEL2 
																		     asserted)
	u8Pio_EN_SINK                   1         R/W          R         * Defines the UPD350 PIO 
																		number used for EN_SINK pin
																	  * This is applicable only for
																		Sink operation. 
	u8Mode_EN_SINK                  1         R/W          R         * Defines the PIO mode for 
																		EN_SINK pin
																	  * This is applicable only for 
																		Sink operation. 
    u8SnkPio_1_5A_IND               1         R/W          R         * Defines the UPD350 PIO number 
																		used for 1.5A_IND pin 
																	  * This is applicable only for 
																		Sink operation. 
	u8Mode_1_5A_IND                 1         R/W          R         * Defines the PIO mode for 
																		1.5A_IND pin 
																	  * This is applicable only for
																		Sink operation. 
	u8SnkPio_3A_IND                 1         R/W          R         * Defines the UPD350 PIO 
																		number used for 3A_IND pin
																	  * This is applicable only for 
																	    Sink operation. 
	u8Mode_3A_IND                   1         R/W          R         * Defines the PIO mode for 
																		3A_IND pin 
																	  * This is applicable only for 
																		Sink operation. 
	u8PIO_SNK_PD_NEG_CMPLT          1         R/W          R         * Defines the UPD350 PIO number
																		for SNK_PD_NEG_CMPLT pin 
																	  * This is applicable only for 
																	    Sink operation. 
	u8Mode_SNK_PD_NEG_CMPLT         1         R/W          R         * Defines the PIO mode for 
																		SNK_PD_NEG_CMPLT pin 
																	  * This is applicable only for
																		Sink operation. 
	u8PIO_SNK_CAP_MISMATCH          1         R/W          R         * Defines the UPD350 PIO number
																	    for SNK_CAP_MISMATCH
																	  * This is applicable only for
																		Sink operation. 
	u8Mode_SNK_CAP_MISMATCH         1         R/W          R         * Defines the PIO mode for 
																		SNK_CAP_MISMATCH pin
																	  * This is applicable only for 
																		Sink operation. 
	u16DAC_I_MaxOutVoltInmV        1                                * Defines the maximum voltage 
																		on DAC_I with a maximum of 
																		2.5V in terms of mV 
																	  * This is applicable only for
																		Sink operation. 
	u16DAC_I_MinOutVoltInmV        1								 * Defines the minimum voltage 
																		on DAC_I with a minimum of 
																		0V in terms of mV 
																	  * This is applicable only for
																		Sink operation. 
	u16DAC_I_CurrentInd_MaxInA       1								 * Defines which current in
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
	u8DAC_I_Direction               1         R/W     	   R         * Specifies the direction of 
																	     DAC_I to allow user invert 
																		 direction of DAC_I if 
																		 required 
																		 1. 0 - High Amperage -
   																		      Max Voltage 
																		 2. 1- High Amperage - 
																			  Min Voltage 
																	  * This is applicable only 
																		 for Sink operation. 
	u8aReserved1[2]					2								 Reserved					 
	u8aReserved2[2]					2								 Reserved					 
	u8aReserved3[2]					2								 Reserved					 		
    </table>
    
    
    
    <b>2. Members that are Bit-Mapped bytes:</b>
    
    <b>a. u32CfgData</b>:
    
    u32CfgData variable holds the Port Configuration Data. It's size is 4 bytes.
    <table>
    Bit     R/W Config   R/W Run   \Description
             time         time      
    ------  -----------  --------  --------------------
    0       RW           R         Port Enable/Disable
                                    * '0' Disabled
                                    * '1' Enabled
    3:1     RW           R         Port Power Role
                                    * '000' Sink
                                    * '001' Source
    5:4     RW           R         Rp Selection
                                    * '00' Disabled
                                    * '01' USB Power
                                    * '10' 1.5A
                                    * '11' 3.0A
    8:6     RW           R         USB Data
                                    * '000' No Data
                                    * '001' USB2
                                    * '010' USB3.1 Gen1
                                    * '011' USB3.1 Gen2
    9       RW           R         VCONN OCS Enable
                                    * '0' Disable
                                    * '1' Enable
    32:10   RW           R         Reserved
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
									* '0' Unflipped 
									* '1' Flipped 
	2       R            R         Data Role 
									* '0' UFP 
									* '1' DFP 
	3       R            R         Power Role 
									* '0' Sink 
									* '1' Source 
	4       R            R         VCONN Status  
									* '0' Disabled 
									* '1' Enabled 
	5       R            R         Cable Reduced Source Capabilities 
									* '0' Attached USB-C cable supports the locally-defined Source 
									    PDOs
									* '1' Attached USB-C cable does not support the locally defined 
									    Source PDOs	
	6       R            R         PD Bal Reduced Source Capabilities
									* '0' The advertised PDOs are equivalent to the default 
									     configured values 
									* '1' The advertised PDOs have been reduced from default 
									     configured values
	7       R            R         Source Capability Mismatch 
									* '0' De-asserted by  Source port when there is capability 
									     mismatch with sink partner 
									* '1' Asserted by Source port when sink port indicates 
									     capability mismatch in RDO
	8       R            R         As Source PD Contract Good 
									* '0' As Source: USB-C Connection Only (No Request Made Yet)
									* '1' As Source; USB PD connection established, Power request 
									      has been made, accepted and PS_RDY message sent. 
								    * This bit will always remain 0 when acting as sink.	
	9       R            R         As Source RDO Accepted
									* '0' As Source: No RDO Accept message has been sent to last 
									      Request made by attached Sink or no Request has yet been 
										  made during connection. 
									* '1' As Source: RDO Accept message has been sent to last 
									      Request made by attached Sink
								    * This bit will always remain 0 when acting as sink		  
	10      R            R         As Source RDO Rejected  
									* '0' As source; No RDO reject message has been sent to last 
											request made by attached Sink or no Request has yet been 
											made during connection 
									* '1' As Source: RDO Reject message has been sent to last 
									        Request made by attached Sink
									* This bit will always remain 0 when acting as Sink 		
	11      R            R         As Sink Last Request Accept 
									* '0' As Sink: Last RDO Request was not Accepted or no request 
									      has yet been made. 
									* '1' As Sink: Last RDO Request was Accepted
									* This bit will always remain 0 when acting as a source.
	12      R            R         As Sink Last Request Reject 
									* '0' As Sink: Last RDO Request was not Rejected or no request 
									   has yet been made. 
									* '1' As Sink: Last RDO Request was Rejected
									* This bit will always remain 0 when acting as a source.
	13      R            R         As Sink Last Request PS_RDY 
									* '0' As Sink: PS_RDY not yet received for last RDO request  
									* '1' As Sink: PS_RDY received for last RDO request
									* This bit will always remain 0 when acting as a source.
	14      R            R         Sink Capability Mismatch  
									* '0' De-asserted by the Sink Port when there is no capability 
										mismatch 
									* '1' Asserted by Sink Port when no Source capability was found
	16:15   R            R         Rp Value detected by Sink 
									* '00' Disabled 
									* '01' USB Power 
								    * '10' 1.5A 
									* '11' 3.0A 
	31:17	R			 R         Reserved 				
	</table>

	<b>c. u16PortIOStatus</b>: 
	u16PortIOStatus variable holds the IO status of the port. It's size is 2 bytes. 
	<table> 
    Bit     R/W Config   R/W Run   \Description
             time         time      
    ------  -----------  --------  --------------------
    0       R            R         EN_DC_DC Status  
                                    * '0' Asserted 
                                    * '1' De-asserted 
    1       R            R         VSEL0 Status  
                                    * '0' Asserted 
                                    * '1' De-asserted
    2       R            R         VSEL1 Status  
                                    * '0' Asserted 
                                    * '1' De-asserted
    3       R            R         VSEL2 Status  
                                    * '0' Asserted 
                                    * '1' De-asserted
    4       R            R         EN_VBUS Status  
                                    * '0' Asserted 
                                    * '1' De-asserted
    5       R            R         VBUS_DIS Status  
                                    * '0' Asserted 
                                    * '1' De-asserted
    6       R            R         EN_SINK Status  
                                    * '0' Asserted 
                                    * '1' De-asserted
    7       R            R         1.5_IND Status  
                                    * '0' Asserted 
                                    * '1' De-asserted
    8       R            R         3.0_IND Status  
                                    * '0' Asserted 
                                    * '1' De-asserted
    9       R            R         PS_RDY Received 
                                    * '0' Asserted 
                                    * '1' De-asserted
    10      R            R         Capability Mismatch  
                                    * '0' Asserted 
                                    * '1' De-asserted
    15:11                          Reserved 
	</table>
	
	<b>d. u16PortStatusChange</b>: 
	u16PortStatusChange variable defines the port connection status change bits. It's size is 2 
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
	15:12                  		   Reserved 
	</table> 	
	
	<b>e. u16PortIntrMask</b>: 
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
	
	<b>f. u8ClientRequest</b>: 
	u8ClientRequest variable defines the client request mask bits. It's size is 1 byte. Application 
	can make use of this variable to request PSF to handle the mentioned client requests. Only one 
	client request could be handled by PSF at a given time. So, it is recommended that the 
	application could raise a single request at a time i.e set only one of the bits in this variable.
	
	In case PSF is busy, it cannot handle any of the client requests. In this case, the 
	u8ClientRequest variable would be cleared and eMCHP_PSF_BUSY notification would be posted by 
	PSF, so that the application initiate the request again by setting the respective bit in this 
	variable. If the request is accepted and processed, a response notification would be posted by 
	PSF as mentioned in the below table.
	<table> 
    Bit     R/W Config   R/W Run   \Description
             time         time      
    ------  -----------  --------  --------------------
    0       R/W          R/W       Renegotiation Request 
                                    * '0' PSF has not received any renegotiation request.
                                    * '1' PSF has received a renegotiation request. 
									Before initiating the request, user has to fill the Source 
									capabilities in u32aNewPDO array and the PDO count in 
									u8NewPDOCnt. 
									Once the request is processed by PSF, u32aNewPDO array and 
									u8NewPDOCnt would be cleared and 
									eMCHP_PSF_PD_CONTRACT_NEGOTIATED notification would be posted. 
    1       R/W          R/W       Get Sink capabilities Request 
                                    * '0' PSF has not received any request for getting the sink 
									      capabilities.
                                    * '1' PSF has received a request for getting the sink 
									      capabilities. 
									Once the request is processed by PSF, 
									eMCHP_PSF_GET_SINK_CAPS_RCVD or eMCHP_PSF_GET_SINK_CAPS_NOT_RCVD
									notification would be posted depending on the Sink partner's 
									response to Get_Sink_Caps message. User can read the received 
									sink capabilities from u32aPartnerPDO array. 
    2       R/W          R/W       Send Alert Request 
                                    * '0' PSF has not received any request for sending the alert
									      message to the port partner.
                                    * '1' PSF has received a request to send the alert
									      message to the port partner.										  
    3       R/W          R/W       Get Status Request 
                                    * '0' PSF has not received any request for getting the partner 
									      status.
                                    * '1' PSF has received a request for getting the sink 
									      capabilities.
    4       R/W          R/W       Get Sink capabilities Extended Request 
                                    * '0' PSF has not received any request for getting the extended 
										  sink capabilities.
                                    * '1' PSF has received a request for getting the extended sink 
									      capabilities.
	7:5  						   Reserved 									
	</table> 								
 
  Remarks:
    None                                                                                                                                
  ***************************************************************************************************************************************/

typedef struct _PortCfgStatus
{
    UINT32 u32CfgData;				
    UINT32 u32aSourcePDO[7];		
    UINT32 u32aSinkPDO[7];          
    UINT32 u32aNewPDO[7];		    
    UINT32 u32aAdvertisedPDO[7];	
    UINT32 u32aPartnerPDO[7];       
    UINT32 u32RDO;                  
	UINT32 u32PortConnectStatus;	
    UINT16 u16AllocatedPowerIn250mW;   
    UINT16 u16NegoVoltageIn50mV;      
    UINT16 u16NegoCurrentIn10mA;      
    UINT16 u16MaximumOperatingCurInmA; 
    #if (TRUE == INCLUDE_PD_SINK)
    UINT16 u16aMinPDOPreferredCurInmA[7]; 
    UINT16 u16MinimumOperatingCurInmA;
    UINT16 u16DAC_I_MaxOutVoltInmV; 
    UINT16 u16DAC_I_MinOutVoltInmV; 
    UINT16 u16DAC_I_CurrentInd_MaxInA;  
    #endif
    UINT16 u16PortIOStatus;
    UINT16 u16PortStatusChange;
    UINT16 u16PortIntrMask;
    UINT16 u16PowerGoodTimerInms;   
    UINT8 u8SourcePDOCnt;			
    UINT8 u8SinkPDOCnt;             
    UINT8 u8NewPDOCnt;              
    UINT8 u8AdvertisedPDOCnt; 		
    UINT8 u8PartnerPDOCnt;          
    UINT8 u8ClientRequest;           
    UINT8 u8SinkConfigSel;         
    UINT8 u8FaultInDebounceInms;    
    UINT8 u8OCSThresholdPercentage; 
    UINT8 u8OVThresholdPercentage;  
    UINT8 u8UVThresholdPercentage;	
    UINT8 u8VCONNOCSDebounceInms;
    UINT8 u8VBUSMaxFaultCnt;
    UINT8 u8VCONNMaxFaultCnt;
    UINT8 u8Pio_EN_VBUS;
    UINT8 u8Mode_EN_VBUS;
    UINT8 u8Pio_FAULT_IN;
    UINT8 u8Mode_FAULT_IN;
    UINT8 u8Pio_VBUS_DIS;
    UINT8 u8mode_VBUS_DIS;
    UINT8 u8Pio_DC_DC_EN;
    UINT8 u8Mode_DC_DC_EN;
    UINT8 u8aReserved1[2];
    #if (CONFIG_DCDC_CTRL == PWRCTRL_DEFAULT_PSF_GPIO_CONFIG) 
    UINT8 u8aPio_VSEL[3];
    UINT8 u8aMode_VSEL[3];
	UINT8 u8aVSELTruthTable[8];
    UINT8 u8aReserved2[2];
	#endif
    #if (TRUE == INCLUDE_PD_SINK)
    UINT8 u8Pio_EN_SINK; 
    UINT8 u8Mode_EN_SINK; 
    UINT8 u8SnkPio_1_5A_IND; 
    UINT8 u8Mode_1_5A_IND;  
    UINT8 u8SnkPio_3A_IND;  
    UINT8 u8Mode_3A_IND;    
    UINT8 u8PIO_SNK_PD_NEG_CMPLT; 
    UINT8 u8Mode_SNK_PD_NEG_CMPLT; 
    UINT8 u8PIO_SNK_CAP_MISMATCH; 
    UINT8 u8Mode_SNK_CAP_MISMATCH; 
    UINT8 u8DAC_I_Direction; 
    UINT8 u8aReserved3[3];
    #endif
	 
   } PORT_CFG_STATUS, *PPORT_CFG_STATUS;

 /**********************************************************************
   Summary:
     This structure contains port specific Power Balancing Configuration parameters.
	 sPBPerPortData is referred from _GlobalCfgStatusData.
   Description:
	 This structure contains global configuration and status parameters that are either Integer 
	 Datatypes, Bit-Mapped bytes or another structure. This structure is used only when either of 
	 the macros INCLUDE_POWER_BALANCING or INCLUDE_POWER_THROTTLING is set to '1'.
	
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
    u16MaxPrtCurrentIn10mA          2         R/W          R         * Maximum allowable current for 
													                    ports in 10mA steps 
																	  * Sample values this variable
																	    can take are, 
																		1. 0x0032 = 0.5A
																		2. 0x012C = 3A 
																		3. 0x01F4 = 5A
																	  * Note : Values above 5A 
																	    (0x01F5 - 0x0FFF) are not 
																		supported	 
	u8aReserved4[3]					3						          Reserved 											
	</table>	

    <b>2. Members that are Bit-Mapped bytes:</b>
    
    <b>u8PBEnablePriority</b>:
    
    u8PBEnablePriority defines PB Enable/Disable per port and also the priority for the port. 
	It's size is 1 byte. 
    <table>
    Bit     R/W Config   R/W Run   \Description
             time         time      
    ------  -----------  --------  --------------------
    0       R/W          R/W       PB Enable/Disable
                                    * '0' Disable
                                    * '1' Enable
	3:1     R/W          R/W       Selects the port Priority 								
									* 000b is the highest priority
    7:4     			           Reserved 									
   Remarks:
     None                                                               
   **********************************************************************/

#if (TRUE == INCLUDE_POWER_BALANCING) 

typedef struct _PBPortCfgStatus
{
    UINT16 u16MaxPrtPwrBankAIn250mW; 
    UINT16 u16MaxPrtPwrBankBIn250mW; 
    UINT16 u16MaxPrtPwrBankCIn250mW; 
    UINT16 u16MaxPrtCurrentIn10mA; 
    UINT8 u8PBEnablePriority; 
    UINT8 u8aReserved4[3];
} PB_PORT_CFG_STATUS, *PPB_PORT_CFG_STATUS;

#endif 
 /**********************************************************************
   Summary:
     This structure contains port specific PPS configuration parameters.
	 sPPSPerPortData is referred from _GlobalCfgStatusData.
   Description:
     This structure contains the following PPS configuration parameters. 
	 This structure is used only when INCLUDE_PD_SOURCE_PPS is set to '1'.
	 
	<table> 	
    Name                            Size in   R/W Config   R/W Run   \Description
                                     Bytes     time         time      
    ------------------------------  --------  -----------  --------  -------------------------------------------------------------------
    u8PPSEnable                     1         R/W          R         PPS Enable/Disable.
																	  * '0' = Disable 
																	  * '1' = Enable 
    u32aPPSApdo[3]                  12        R/W          R         Defines the PPS APDOs. 
	u8aReserved5[3]				    3                                Reserved 
	</table> 																 												  																 																  
   Remarks:
     None                                                               
   **********************************************************************/
#if (TRUE == INCLUDE_PD_SOURCE_PPS)   

typedef struct _PPSPortCfgStatus
{
    UINT8 u8PPSEnable;
    UINT8 u8aReserved5[3];
    UINT32 u32aPPSApdo[3];  
} PPS_PORT_CFG_STATUS, *PPPS_PORT_CFG_STATUS;

#endif 
  /**********************************************************************
   Summary:
	This structure contains the system level, Port specific configurations and Status
	parameters of PSF for Type C, PD, PB, PT and PPS parameters.
	gasCfgStatusData is the defined variable of this structure.
   Description:
     This structure contains global configuration and status parameters that are either Integer 
	 Datatypes, Bit-Mapped bytes or another structure.
	
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

	u16Reserved               	    2         R/W          R         Reserved 

	u16ProducdID               	    2         R/W          R         * Defines the Product Identifier 
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
	u16ProductTypeVDO               2         R/W          R         Defines Product Type VDO  

	u16ProductVDO                   2         R/W          R         Defines Product VDO

	u16CertStatVDO                  2         R/W          R         Defines Cert Stat VDO
	
	u16IDHeaderVDO                  2         R/W          R         Defines ID Header VDO 
	     
	u8PwrThrottleCfg 	            1         R/W          R/W       * Defines the currently 
																		selected Power Bank. 
																	  * Possible values are, 
																		1. 0x00 = Bank A 
																		2. 0x01 = Bank B
																	    3. 0x02 = Bank C 
																		4. 0x03 = Shutdown mode
																	  *	This variable is used only 
																	    when either of
																        INCLUDE_POWER_BALANCING or
																	    INCLUDE_POWER_THROTTLING is 
																		set to '1'. 			
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
																		
	u16SharedPwrCapacityIn250mW     2         R/W          R        * Defines the currently 
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
    u8aReserved6[2]				    2 								Reserved 	
    u8aReserved7[3]				    3								Reserved 
    u8aReserved8[3]				    3 								Reserved 
    u16aReserved1				    2 								Reserved 	
																	
																		
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
	4       R/W          R         PD Power balancing Enable/Disable for the system								
									* 0 = PD Balancing is disabled 
									* 1 = PD Balancing is enabled
    7:5                            Reserved  									
	</table>								
	
	<b>3. Members that are another structures:</b>
	<table>
    Structure        \Description     
    ------           --------------------
	sPerPortData     Includes Type C and PD parameters of a port, say default Source PDOs, default 
					  Sink PDOs, currently negotiated voltage and current values, under voltage and 
					  over voltage threshold values, etc., Tag for this structure is _PortCfgStatus.
	sPPSPerPortData  Includes PPS parameters of a port, say PPS Enable/Disable option and list of 
						Augmented PDOs supported. Tag for this structure is _PPSPortCfgStatus. 
	sPBPerPortData   Includes Power Balancing parameters of a port, say maximum power and maximum 
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
    UINT8 u8aManfString[8]; 
    UINT8 u8PSFMajorVersion; 
    UINT8 u8PSFMinorVersion; 
    UINT8 u8aReserved6[2];     
    UINT16 u16ProducdID;	
    UINT16 u16VendorID;		
    UINT16 u16ProductTypeVDO; 
    UINT16 u16ProductVDO; 
    UINT16 u16CertStatVDO; 
    UINT16 u16IDHeaderVDO; 
    
    PORT_CFG_STATUS sPerPortData[CONFIG_PD_PORT_COUNT]; 
#if (TRUE == INCLUDE_POWER_BALANCING)
    UINT8 u8PBEnableSelect;	
    UINT8 u8aReserved7[3];	
#endif 
#if ((TRUE == INCLUDE_POWER_BALANCING) || (TRUE == INCLUDE_POWER_THROTTLING))    
    UINT8 u8PwrThrottleCfg;	
    UINT8 u8aReserved8[3];
    UINT16 u16SystemPowerBankAIn250mW; 
    UINT16 u16MinPowerBankAIn250mW;   
    UINT16 u16SystemPowerBankBIn250mW; 
    UINT16 u16MinPowerBankBIn250mW;   
    UINT16 u16SystemPowerBankCIn250mW; 
    UINT16 u16MinPowerBankCIn250mW;    
#endif
#if (TRUE == INCLUDE_POWER_BALANCING)    
    UINT16 u16SharedPwrCapacityIn250mW; 
    UINT16 u16Reserved1;
    PB_PORT_CFG_STATUS sPBPerPortData[CONFIG_PD_PORT_COUNT];	
#endif 
    
#if (TRUE == INCLUDE_PD_SOURCE_PPS)
    PPS_PORT_CFG_STATUS sPPSPerPortData[CONFIG_PD_PORT_COUNT]; 
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
	 stack to funtion properly. This can be done through MCHP_PSF_HOOK_BOOT_TIME_CONFIG which 
	 initializes the parameters defined in gasCfgStatusData during compile time. For accessing 
	 the configuration registers and reading the status registers at run time, an I2C slave
	 interface shall be used by the user application. 																 												  																 																  
   Remarks:
     None                                                               
   **********************************************************************/
   
extern GLOBAL_CFG_STATUS_DATA gasCfgStatusData;   

#include "PSFSourcePro_App.h"
#include "PSFSourcePro_BootCfg.h"

#endif
