/*******************************************************************************
  PSF stack porting and integration API Hook header file.

  Company:
    Microchip Technology Inc.

  File Name:
    PSF_APIHook.h

  Summary:
    Porting header file 

  Description:
    This header file contains the porting hook functions to be integrated with any hardware platform
*******************************************************************************/
//DOM-IGNORE-BEGIN
/*******************************************************************************

Copyright ©  [2019] Microchip Technology Inc. and its subsidiaries.

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
#ifndef _PSF_API_HOOK_H_
#define _PSF_API_HOOK_H_

//DOM-IGNORE-END
// *****************************************************************************
// *****************************************************************************
// Section: Header files of User Application required for PSF stack operation.
// *****************************************************************************
// *****************************************************************************

#include "PSF_Config.h"
#include "Drivers.h"
#include "PDSource_App.h"
// *****************************************************************************
// *****************************************************************************
// Section: HOOK CONFIGURATION
// *****************************************************************************
 /**************************************************************************
  Function:
    MCHP_PSF_HOOK_DPM_PRE_PROCESS(_u8port_num_)
  Summary:
    This function is called before entering into the DPM state machine.
  Description:
    This function is called at the entry of DPM_RunStateMachine() API before
    executing the Type C state machine and policy engine state machine.
    USER_APPLICATION can define this function if a change is required in
    default device policy manager functionality. Define relevant function
    that has one UINT8 argument without return type.
  Conditions:
    None.
  Input:
    _u8port_num_ -  Port number of the device. Value passed will be less
                    than CONFIG_PD_PORT_COUNT
  Return:
    None.
  Example:
    <code>
    \#define MCHP_PSF_HOOK_DPM_PRE_PROCESS(_u8port_num_) \\
                    HookDevicePolicyManagerPreProcess(_u8port_num_)
    
    void HookDevicePolicyManagerPreProcess(UINT8 u8PortNum);
    
    void HookDevicePolicyManagerPreProcess(UINT8 u8PortNum)
    {
    
        //any application related change or enhancement for the
        //DPM_RunStateMachine() API
    
    }
    </code>
  Remarks:
    User definition of this Hook function is optional                      
  **************************************************************************/
#define MCHP_PSF_HOOK_DPM_PRE_PROCESS(_u8port_num_)     

/*************************************************************************************
  Function:
    MCHP_PSF_HOOK_DPM_POST_PROCESS(_u8port_num_)
  Summary:
    This function is called before exiting from the DPM state machine
  Description:
    This function is called at the exit of DPM_RunStateMachine() API after
    executing the Type C state machine and policy engine state machine.
    USER_APPLICATION can define this function if a change is required in
    default device policy manager functionality. Define relevant function
    that has one UINT8 argument without return type.
  Conditions:
    None.
  Input:
    _u8port_num_ -  Port number of the device. Value passed will be less
                    than CONFIG_PD_PORT_COUNT
  Return:
    None.
  Example:
    <code>
    \#define MCHP_PSF_HOOK_DPM_POST_PROCESS(_u8port_num_) \
                          HookDevicePolicyManagerPostProcess(_u8port_num_)
    
    void HookDevicePolicyManagerPostProcess(UINT8 u8PortNum);
    
    void HookDevicePolicyManagerPostProcess(UINT8 u8PortNum)
    {
    
        //any application related change or enhancement for the DPM_RunStateMachine() API
    
    }
    </code>

  Remarks:
    User definition of this Hook function is optional                                 
  *************************************************************************************/
#define MCHP_PSF_HOOK_DPM_POST_PROCESS(_u8port_num_) 	

/****************************************************************************************************
  Function:
    MCHP_PSF_NOTIFY_CALL_BACK(_u8port_num_, _u8pd_event_)   
  Summary:
    Notifies the USER_APPLICATION about various PD events such as Type C
    Attach and Detach , Type C orientation .
  Description:
    This function is called by the various modules of Zeus Stack to notify
    the USER_APPLICATION about different PD events such as Type-C Attach
    and Detach , Type-C Orientation. USER_APPLICATION can define this hook
    \function if it wants external handling of the PD events. Define
    relevant function that has one UINT8 argument without return type.
  Conditions:
    None.
  Input:
    _u8port_num_ -  Port number of the device. Value passed will be less
                    than CONFIG_PD_PORT_COUNT
    _u8pd_event_ -   Type of PD event occurred inside the stack. This
                    argument can take one of the following, MCHP_PSF_SNK_TYPEC_ATTACH
                    \eMCHP_PSF_TYPEC_DETACH_EVENT - Detach event is detected in Type C State machine
                    \eMCHP_PSF_TYPEC_CC1_ATTACH - Attach is detected in CC1 pin
                    \eMCHP_PSF_TYPEC_CC2_ATTACH - Attach is detected in CC2 pin 
					\eMCHP_PSF_UPDS_IN_IDLE - UPD350s is in Idle 
                    \eMCHP_PSF_VCONN_PWR_FAULT - VCONN Power fault 
                    \eMCHP_PSF_VBUS_PWR_FAULT - VBUS Power fault 
  Return:
    None.
  Example:
    <code>
    \#define MCHP_PSF_NOTIFY_CALL_BACK(_u8port_num_, _u8pd_event_) \\
    HookNotifyPDEvents(_u8port_num_, _u8pd_event_)
    
    void HookNotifyPDEvents(UINT8 u8Portnum, UINT8 u8PDEvents);
    
    void HookNotifyPDEvents(UINT8 u8Portnum, UINT8 u8PDEvents)
    {
    
        Switch(u8PDEvents)
        {
            case eMCHP_PSF_TYPEC_DETACH_EVENT:
            {
                //USER_APPLICATION code for Type C detach event in "u8PortNum" Port
                break;
    
            }
            case eMCHP_PSF_TYPEC_CC1_ATTACH:
            {
                //USER_APPLICATION code for Type C attach event in CC1 of "u8PortNum" Port
                break;
    
            }
            case eMCHP_PSF_TYPEC_CC2_ATTACH:
            {
                //USER_APPLICATION code for Type C attach event in CC2 of "u8PortNum" Port
                break;
    
            }
            default:
            {
                break;
            }
    
        }
    
    }
    
    </code>
  Remarks:
    User definition of this Hook function is optional                                                
  ****************************************************************************************************/
#define MCHP_PSF_NOTIFY_CALL_BACK(_u8port_num_, _u8pd_event_)   PDStack_Events(_u8port_num_, _u8pd_event_)

/* Enum for Notification Call back */
enum MCHP_PSF_NOTIFICATION
{    
   eMCHP_PSF_TYPEC_DETACH_EVENT = 1,
   eMCHP_PSF_TYPEC_CC1_ATTACH,
   eMCHP_PSF_TYPEC_CC2_ATTACH,
   eMCHP_PSF_UPDS_IN_IDLE,
   eMCHP_PSF_VCONN_PWR_FAULT,
   eMCHP_PSF_VBUS_PWR_FAULT
};


/**************************************************************************************************
	Function:
		MCHP_PSF_HOOK_BOOT_TIME_CONFIG(_CfgGlobals_)

	Summary :
        Updates the Type-C Configurable parameters(Power Role and Rp Current), number of
        PDO and PDOs parameter at boot time.

	Description :
		This function is called to update the configuration parameters of Type-C (Power Role and Rp Current),
        number of PDO and PDOs parameter at boot time(Stack initialization). 
        This API must have a input parameter of PORT_CONFIG_DATA prototype (Structure Pointer to PORT_CONFIG_DATA).
		
	Precondition :
		None.

	Parameters :
		_CfgGlobals_ - Holds the structure pointer of the structure PORT_CONFIG_DATA

	Return :
		None.

	Example :

		<code>
		#define  MCHP_PSF_HOOK_BOOT_TIME_CONFIG(_CfgGlobals_)            STRAPS_PowerRole_Set(_CfgGlobals_)

		void STRAPS_PowerRole_Set(PORT_CONFIG_DATA *PortConfigData);

		void STRAPS_PowerRole_Set(PORT_CONFIG_DATA *PortConfigData)
		{
			// Read Straps for enabled ports 1 to n
			
			if(gpio_get_pin_level(PA28) == PULL_UP)
			{
				// Configure Cfg variables for Source
				// Set Source Capabilities
			}
			
			else
			{
				// Configure Cfg variables for sink
				// Set Sink Capabilities
			}
			
			// Repeat the above steps upto port n

		}
		</code>

	Remarks:
		User definition of this Hook function is optional
**************************************************************************************************/
#define  MCHP_PSF_HOOK_BOOT_TIME_CONFIG(CfgGlobals)  	

// *****************************************************************************
// *****************************************************************************
// Section: DEBUG MESSAGES CONFIGURATION
// *****************************************************************************
// *****************************************************************************

#if CONFIG_HOOK_DEBUG_MSG
/***********************************************************************
    Function:
        MCHP_PSF_HOOK_DEBUG_INIT()            
    Summary:
        Initialization of debug module
    Description:
        This function is called during
        initialization of stack if CONFIG_HOOK_DEBUG_MSG is set to 1. Define
        relevant function that has no arguments without return type.
    Conditions:
        None.
    Return:
        None.
    Example:
        <code>
          \#define MCHP_PSF_HOOK_DEBUG_INIT()          uart_init()

          void uart_init();

          void uart_init()
          {
              //Initialzes the uart module to send and receive a character
          }

        </code>
    Remarks:
        User definition of this Hook function is optional                   
  ***********************************************************************/  
#define MCHP_PSF_HOOK_DEBUG_INIT()                       
/*************************************************************************
    Function:
        MCHP_PSF_HOOK_DEBUG_STRING(_str_)
    Summary:
        Initialization of debug module
    Description:
        This function is called by stack to send
        a character string to DEBUG_MODULE. This API will be called if
        CONFIG_HOOK_DEBUG_MSG is set to 1. Define relevant function that has
        CHAR pointer argument without return type.
    Conditions:
        None.
    Input:
        _str_ -  Pointer to the character buffer
    Return:
        None.
    Example:
        <code>
          \#define MCHP_PSF_HOOK_DEBUG_STRING(_str_)       uart_write(_str_)

          void uart_write(char *chBuffer);

          void uart_write(char *chBuffer)
          {
            //Write character string to UART
          }

        </code>
    Remarks:
        User definition of this Hook function is optional                     
  *************************************************************************/ 
#define MCHP_PSF_HOOK_DEBUG_STRING(_str_)		                                  

/************************************************************************
    Function:
        MCHP_PSF_HOOK_DEBUG_CHAR(_char_)
    Summary:
        Send CHAR to DEBUG_MODULE
    Description:
        This function is called by stack to send a character to DEBUG_MODULE.
        This API will be called if CONFIG_HOOK_DEBUG_MSG is set to 1. Define
        relevant function that has CHAR argument without return type.
    Conditions:
        None.
    Input:
        chName -  Character to be sent to DEBUG_MODULE
    Return:
        None.
    Example:
        <code>
          \#define MCHP_PSF_HOOK_DEBUG_CHAR(_char_)   uart_write(_char_)

          void uart_write(char chName);

          void uart_write(char chName)
          {
              //Write character to UART
          }

        </code>
    Remarks:
        User definition of this Hook function is optional                    
************************************************************************/ 
	#define MCHP_PSF_HOOK_DEBUG_CHAR(_char_)               

/**************************************************************************
    Function:
        MCHP_PSF_HOOK_DEBUG_UINT8(_u8Val_)
    Summary:
        Send UINT8 to DEBUG_MODULE
    Description:
        This function is called by stack to send a
        UINT8 data to DEBUG_MODULE This API will be called if
        CONFIG_HOOK_DEBUG_MSG is set to 1. Define relevant function that has
        UINT8 argument without return type.
    Conditions:
        None.
    Input:
        u8Val -  UINT8 data to be sent to DEBUG_MODULE
    Return:
        None.
    Example:
        <code>
          \#define MCHP_PSF_HOOK_DEBUG_UINT8(_u8Val_)     uart_write(_u8Val_)

          void uart_write(UINT8 u8Val);

          void uart_write(UINT8 u8Val)
          {
            //Convert UINT8 to character string and write to UART
          }

        </code>
    Remarks:
        User definition of this Hook function is optional                      
**************************************************************************/ 
	#define MCHP_PSF_HOOK_DEBUG_UINT8(_u8Val_)              

/*************************************************************************
    Function:
        MCHP_PSF_HOOK_DEBUG_UINT16(_u16Val_)
    Summary:
        Send UINT16 to DEBUG_MODULE
    Description:
        This function is called by stack to send
        a UINT16 data to DEBUG_MODULE This API will be called if
        CONFIG_HOOK_DEBUG_MSG is set to 1. Define relevant function that has
        UINT16 argument without return type.
    Conditions:
        None.
    Input:
        u16Val -  UINT16 data to be sent to DEBUG_MODULE
    Return:
        None.
    Example:
        <code>
          \#define MCHP_PSF_HOOK_DEBUG_UINT16(_u16Val_)     uart_write(_u16Val_)

          void uart_write(UINT16 u16Val);

          void uart_write(UINT16 u16Val)
          {
             //Convert UINT16 to character string and write to UART
          }

        </code>
    Remarks:
        User definition of this Hook function is optional                     
*************************************************************************/ 
#define MCHP_PSF_HOOK_DEBUG_UINT16(_u16Val_)                
/*************************************************************************
    Function:
        MCHP_PSF_HOOK_DEBUG_UINT32(_u32Val_)
    Summary:
        Send UINT32 to DEBUG_MODULE
    Description:
        This function is called by stack to send
        a UINT32 data to DEBUG_MODULE. This API will be called if
        CONFIG_HOOK_DEBUG_MSG is set to 1. Define relevant function that has a
        UINT32 argument without return type.
    Conditions:
        None.
    Input:
        u32Val -  UINT32 data to be sent to DEBUG_MODULE
    Return:
        None.
    Example:
        <code>
          \#define MCHP_PSF_HOOK_DEBUG_UINT32(_u32Val_)    uart_write(_u32Val_)

          void uart_write(UINT32 u32Val);

          void uart_write(UINT32 u32Val)
          {
            //Convert UINT32 to character string and write to UART
          }
        </code>
    Remarks:
        User definition of this Hook function is optional                     
*************************************************************************/ 
	#define MCHP_PSF_HOOK_DEBUG_UINT32(_u32Val_)		                        

/**************************************************************************
    Function:
        MCHP_PSF_HOOK_DEBUG_INT32(_i32Val_)
    Summary:
        Send INT32 to DEBUG_MODULE
    Description:
        This function is called by stack to send
        a INT32 data to DEBUG_MODULE. This API will be called if
        CONFIG_HOOK_DEBUG_MSG is set to 1. Define relevant function that has
        INT32 argument without return type.
    Conditions:
        None.
    Input:
        i32Val -  INT32 data to be sent to DEBUG_MODULE
    Return:
        None.
    Example:
        <code>
          \#define MCHP_PSF_HOOK_DEBUG_INT32(_i32Val_)        uart_write(_i32Val_)

          void uart_write(INT32 i32Val);

          void uart_write(INT32 i32Val)
          {
            //Convert INT32 to character string and write to UART
          }

        </code>
    Remarks:
        User definition of this Hook function is optional                      
  **************************************************************************/ 
    #define MCHP_PSF_HOOK_DEBUG_INT32(_i32Val_)		                  
		                               	
#endif

// *****************************************************************************
// *****************************************************************************
// Section: PDTimer configuration
// *****************************************************************************
// *****************************************************************************
/***********************************************************************
    Function:
        MCHP_PSF_HOOK_HW_PDTIMER_INIT() 
    Summary:
        Initializes and starts the hardware timer module.
    Description:
        This function initializes and starts the hardware timer module. PSF
        Stack requires a single dedicated hardware timer module for its
        functionality. Define relevant function that has no argument without
        \return type.
    Conditions:
        None.
    Return:
        UINT8 - Returns TRUE if initialization is successful else FALSE. If Timer initialization
                fails and this API returns FALSE, all the PD ports are disabled by default.
    Example:
        <code>
          #define MCHP_PSF_HOOK_HW_PDTIMER_INIT()       Timer_Init()
    
          UINT8 Timer_Init(void);
    
          UINT8 Timer_Init(void)
          {
            //Initializes and starts the microcontroller timer module
            // Return TRUE if Timer initialisation is successful
            //else return FALSE
          }
        </code>
    Remarks:
        User definition of this Hook function is mandatory                  
  ***********************************************************************/			      
#define MCHP_PSF_HOOK_HW_PDTIMER_INIT()   SAMD20_HWTimerInit() 

/**************************************************************************************************
    Description :
        MCHP_PSF_PDTIMER_INTERRUPT_RATE refers to the frequency of interrupt set in the hardware timer dedicated for PSF stack.In other words it is the resolution of the hardware timer.
        MCHP_PSF_PDTIMER_INTERRUPT_RATE can be configured depending on the resolution of the hardware timer available

	Remarks :
        Resolution of the hardware timer has to be at least 1ms. Tested resolution values of hardware timer are 500us, 1ms(Corresponding MCHP_PSF_PDTIMER_INTERRUPT_RATE values are 2000,1000)
	 
	Example :
        <code>
          # define MCHP_PSF_PDTIMER_INTERRUPT_RATE 1000  (1000 interrupts per seconnd, with interrupt interval or resolution of 1ms)
        </code>
**************************************************************************************************/
#define MCHP_PSF_PDTIMER_INTERRUPT_RATE	1000

/**************************************************************************************************
	Description :
        CONFIG_16_BIT_PDTIMEOUT can be defined as either 1 or 0 to set the timeout counter in PSF Stack to unsigned 16bit or unsigned 32bit correspondingly.
        When set as 1,maximum timeout that can be set will be 65535 ticks.(Ticks = Resolution of the Hardware timer used). When set as 0 , maximum timeout that can be set will be 4294967296 ticks.
        Default value of CONFIG_16_BIT_PDTIMEOUT is set as 1. With Hardware timer resolution set as 1ms , PSF stack will be capable of handling timeouts upto 65.535 Seconds.

	Remarks :
        None

	Example :
        <code>
          #define CONFIG_16_BIT_PDTIMEOUT	1 (Sets timeout variable inside the PDStack unsigned 16bit)
          #define CONFIG_16_BIT_PDTIMEOUT	0 (Sets timeout variable inside the PDStack unsigned 32bit)
        </code>
**************************************************************************************************/
#define CONFIG_16_BIT_PDTIMEOUT     1

// *****************************************************************************
// *****************************************************************************
// Section: Port power control configuration
// *****************************************************************************
// *****************************************************************************   
/*****************************************************************************
    Function:
        MCHP_PSF_HOOK_HW_PORTPWR_INIT(_u8Port_Num_)
    Summary:
        Initializes all the hardware modules related to port power
        functionality
    Description:
        This function initializes the hardware modules related to port power
        functionality. Implementation of this function depends on the type of
        DC-DC Converter used. Port power(VBUS) of all ports must be set to 0V
        in this API. Define relevant function that has no argument without
        \return type.
    Conditions:
        None.
    Return:
        None.
    Example:
        <code>
          \#define MCHP_PSF_HOOK_HW_PORTPWR_INIT(_u8Port_Num_)       hw_portpower_init(_u8Port_Num_)
    
          void hw_portpower_init(void);
    
          void hw_portpower_init(void)
          {
            //Initializes the hardware modules related to port power functionality
          }
        </code>
    Remarks:
        User definition of this Hook function is mandatory                        
  *****************************************************************************/
#define MCHP_PSF_HOOK_HW_PORTPWR_INIT(_u8Port_Num_)                       

/****************************************************************************
    Function:
        MCHP_PSF_HOOK_PORTPWR_DRIVE_VBUS(_u8Port_Num_, _u16VBUSVolatge_,_u16Current_)
    Summary:
        Drives the VBUS line of a given Type C port to a particular voltage
    Description:
        This function is used to drive the VBUS line of a given Type C port to
        a particular voltage.Implementation of this function depends on the
        type of DC-DC Converter used. Define relevant function that has
        UINT8,UINT8 arguments without return type.
    Conditions:
        None.
    Input:
        _u8Port_Num_ -     Port number of the device. Value passed will be less
                           than CONFIG_PD_PORT_COUNT
        _u16VBUSVolatge_ - VBUS Voltage level to be driven. Data type of this
                           parameter must be UINT16. 
        * _u16Current_     - VBUS current level in mA.
    Return:
        None.
    Example:
        <code>
          \#define MCHP_PSF_HOOK_PORTPWR_DRIVE_VBUS(_u8Port_Num_, _u8VBUSVolatge_) \\
                    hw_portpower_driveVBUS(_u8Port_Num_, _u8VBUSVolatge_)
    
          void hw_portpower_driveVBUS(UINT8 u8PortNum,UINT8 u8VBUS_Volatge);
    
          void hw_portpower_driveVBUS(UINT8 u8PortNum,UINT8 u8VBUS_Volatge)
          {
    
            Switch(u8VBUS_Volatge)
            {
                case PWRCTRL_VBUS_0V:
                {
                    //Drive 0V in VBUS line of "u8PortNum" Port
                    break;
                }
                case PWRCTRL_VBUS_5V:
                {
                    //Drive 5V in VBUS line of "u8PortNum" Port
                    break;
                }
                case PWRCTRL_VBUS_9V:
                {
                    //Drive 9V in VBUS line of "u8PortNum" Port
                    break; 
                }
                case PWRCTRL_VBUS_15V:
                {
                    //Drive 15V in VBUS line of "u8PortNum" Port
                    break;
                }
                case PWRCTRL_VBUS_20V:
                {
                    //Drive 20V in VBUS line of "u8PortNum" Port
                    break;
                }
                default:
                {
                    break;
                }
            }
          }
        </code>
    Remarks:
        User definition of this Hook function is mandatory                       
  ****************************************************************************/
#define MCHP_PSF_HOOK_PORTPWR_DRIVE_VBUS(_u8Port_Num_,_u8PDOIndex_,_u16VBUSVolatge_,_u16Current_)

/*******************************************************************************************
    Function:
        MCHP_PSF_HOOK_PORTPWR_ENDIS_VBUSDISCH( _u8Port_Num_,_u8EnableDisable_)   
    Summary:
        Enables or disables VBUS Discharge functionality for a given Port
    Description:
        This function is used to enable or disable the VBUS Discharge
        functionality for a given Port .Implementation of this function depends
        on the type of DC-DC Converter used. Define relevant function that has
        UINT8,UINT8 arguments without return type.
    Conditions:
        MCHP_PSF_HOOK_PORTPWR_ENDIS_VBUSDISCH is called in ISR handler.
        This function must be very short, otherwise response time to the interrupt
        will take longer time.
    Input:
        _u8Port_Num_ -      Port number of the device. Value passed will be
                            less than CONFIG_PD_PORT_COUNT
    _   u8EnableDisable_ -  Flag indicating whether to enable/disable VBUS
                            Discharge. Data type of this parameter must be
                            UINT8.<p />Value of this argument must be one of
                            the following,<p />PWRCTRL_ENABLE_VBUSDIS \- Enable
                            the VBUS Discharge for a given port<p />PWRCTRL_DISABLE_VBUSDIS
                            \- Disable the VBUS Discharge for a given port
    Return:
        None.
    Example:
        <code>
          \#define MCHP_PSF_HOOK_PORTPWR_ENDIS_VBUSDISCH(_u8Port_Num, _u8EnableDisable)   \\
                          hw_portpower_enab_dis_VBUSDischarge(_u8Port_Num, _u8EnableDisable)
    
          void hw_portpower_enab_dis_VBUSDischarge(UINT8 u8PortNum,UINT8 u8EnableDisable);
    
    void hw_portpower_enab_dis_VBUSDischarge(UINT8 u8PortNum,UINT8 u8EnableDisable)
    {
         Switch(u8EnableDisable)
        {
            case TRUE:
            {
                //Enable the VBUS Discharge for "u8PortNum" Port
                break;
    
            }
            case FALSE:
            {
                //Disable the VBUS Discharge for "u8PortNum" Port
                break;
    
            }
            default:
            {
                break;
            }
          }
        </code>

  Remarks:
        User definition of this Hook function is mandatory.Passing of the Compliance test "TD.4.2.1" (Source Connect Sink) in "USB_Type_C_Functional_Test_Specification" depends on the VBUS Discharge circuitry used.
	Typical VBUS Discharge time from any higher voltage to 0V is around 10ms                                  
  *******************************************************************************************/
#define MCHP_PSF_HOOK_PORTPWR_ENDIS_VBUSDISCH(_u8Port_Num, _u8EnableDisable_)	

/*******************************************************************************************
    Function:
        MCHP_PSF_HOOK_PORTPWR_ENDIS_SINK_HW(_u8Port_Num, _u16Voltage_, _u16Current_)
    Summary:
        Enables or disables sink hardware circuitry to sinks the VBUS voltage for a given Port 
        based on the sink requested voltage and current.
    Description:
        This function is used to enable or disable sink hardware circuitry which sinks the 
        VBUS voltage for a given Port and also configure the Sink HW or circuitry 
        to Sink requested current and voltage.Implementation of this function depends
        on the type of Sink circuitry used. 
    Conditions:
        None.
    Input:
        _u8Port_Num_ -  Port number of the device. Value passed will be
					    less than CONFIG_PD_PORT_COUNT
        _u16voltage_ -  Enable Sink HW Circuitry if the u16voltage is non Vsafe0V to drain power.
                        Disable sink HW circuitry if the u16voltage is VSafe0V.
                        Configure the HW to requested voltage passed in mV.
        _u16Current_ -  Configure the HW  for the requested current passed in mA
    Return:
        None.
    Example:
        <code>
		  \#define MCHP_PSF_HOOK_PORTPWR_ENDIS_SINK_HW(_u8Port_Num, _u16voltage_, _u16Current_)  \\
		  hw_SinkCircuitary_enab_dis_(_u8Port_Num, _u16voltage_, _u16Current_)

            void hw_SinkCircuitary_enab_dis_(UINT8 u8PortNum,UINT16 u16Votlage, UINT16 u16Current);

            void hw_SinkCircuitary_enab_dis_(UINT8 u8PortNum, UINT16 u16Votlage, UINT16 u16Current)
            {
   
                if (u16Current >= 3000)
                {
                    //Configure the sink HW to 3A
                }
                else if (u16Current >= 1500)
                {
                    //Configure the sink HW to 1.5A
                }
                else if (u16Current == 0)
                {
                    //Configure the Sink HW to OFF state
                }
                else
                {
                    // handle less than 1.5A configuration
                }
            
                if(u16Voltage == Vsafe0V)
                {
                    //Disable the Sink circuitary for "u8PortNum" Port
                }
                else
                {
                    //Enable the Sink circuitary for "u8PortNum" Port
                }
            }
        </code>

    Remarks:
        User definition of this Hook function is mandatory.
*******************************************************************************************/
#define MCHP_PSF_HOOK_PORTPWR_ENDIS_SINK_HW(_u8Port_Num,_u16Voltage_,_u16Current_)
                                                                                           
// *****************************************************************************
// *****************************************************************************
// Section: MCU INTERRUPT CONFIGURATION
// *****************************************************************************
// *****************************************************************************
/*********************************************************************************
     Function:
        MCHP_PSF_HOOK_DISABLE_GLOBAL_INTERRUPT()
    Summary:
        Disables the global interrupt.
    Description:
        This function is called when entering into a critical section. It must
        provide an implementation to disable the interrupts globally. This
        \function must be very short, otherwise response time to the interrupt
        will take longer time. Define relevant function that has no arguments
        without return type.
    Conditions:
        None.
    Return:
        None.
    Example:
        <code>
          \#define MCHP_PSF_HOOK_DISABLE_GLOBAL_INTERRUPT()   CRITICAL_SECTION_ENTER()
    
          void CRITICAL_SECTION_ENTER(void);
    
          void CRITICAL_SECTION_ENTER()
          {
            //Disable global interrupts
          }
        </code>
    Remarks:
        User definition of this Hook function is mandatory                            
  *********************************************************************************/
#define MCHP_PSF_HOOK_DISABLE_GLOBAL_INTERRUPT()  SAMD20_EnterCriticalSection()             

/*******************************************************************************
    Function:
        MCHP_PSF_HOOK_ENABLE_GLOBAL_INTERRUPT()
    Summary:
        Enables the global interrupt.
    Description:
        This function is called when exiting from critical section. It must
        provide an implementation to enable the interrupts globally. This
        \function must be very short, otherwise response time to the interrupt
        will take longer time. Define relevant function that has no arguments
        without return type.
    Conditions:
        None.
    Return:
        None.
    Example:
        <code>
          \#define MCHP_PSF_HOOK_ENABLE_GLOBAL_INTERRUPT()   CRITICAL_SECTION_EXIT()
    
          void CRITICAL_SECTION_EXIT(void);
    
          void CRITICAL_SECTION_EXIT()
          {
            //Enable global interrupts
          }
        </code>
  Remarks:
    User definition of this Hook function is mandatory                          
  *******************************************************************************/                           
#define MCHP_PSF_HOOK_ENABLE_GLOBAL_INTERRUPT()     SAMD20_ExitCriticalSection()           

/**************************************************************************
    Function:
        MCHP_PSF_HOOK_UPD_ALERT_GPIO_INIT(_u8Port_num_)
    Summary:
        Initializes the MCU GPIO that is connected to the alert line of a port's
        UPD350 for interrupt notification
    Description:
        This function initializes the MCU GPIO that is connected to the alert
        line of a port's UPD350 for interrupt notification. It is recommended to
        configure MCU GPIOs interrupt in edge level detection since the UPD350
        keeps the alert line in low state until the interrupt is cleared.
        Define relevant function that has port number as argument without return type.
    Conditions:
        None.
    Input:
        _u8Port_Num_ -  Port number of the device. Value passed will be less
                    than CONFIG_PD_PORT_COUNT
    Return:
        None.
    Example:
        <code>
          \#define MCHP_PSF_HOOK_UPD_ALERT_GPIO_INIT(_u8Port_num_)      INT_ExtIrqInit(_u8Port_num_)
    
          void INT_ExtIrqInit(UINT8 u8Portnum);
    
          void INT_ExtIrqInit(UINT8 u8Portnum)
          {
            
            if(0 == u8Portnum)
			{
				//Initializes the MCU GPIO that is connected to Port 0's UPD350
			}
			if(1 == u8Portnum)
			{
				//Initializes the MCU GPIO that is connected to Port 1's UPD350
			}
          }
        </code>
    Remarks:
        User definition of this Hook function is mandatory                     
  **************************************************************************/
#define MCHP_PSF_HOOK_UPD_ALERT_GPIO_INIT(_u8Port_num_)	SAMD20_UPD350AlertInit(_u8Port_num_)

// *****************************************************************************
// *****************************************************************************
// Section: Reset UPD350 through MCU GPIO initialization
// *****************************************************************************
// *****************************************************************************
/**************************************************************************
    Function:
        MCHP_PSF_HOOK_UPD_RESET_GPIO_INIT(_u8Port_num_)
    Summary:
        Initializes the MCU GPIOs that are connected to the reset line of port's
        UPD350
    Description:
        This function initializes the MCU GPIOs that are connected to the reset
        line of the Port's UPD350. It is recommended to connect a single GPIO to the
        reset line of all UPD350s. Default line state of configured MCU GPIO
        must be high. Define relevant function that has port number as argument without
        \return type.
    Conditions:
        None.
    Input:
        _u8Port_Num_ -  Port number of the device. Value passed will be less
                    than CONFIG_PD_PORT_COUNT
    Return:
        None.
    Example:
        <code>
          \#define MCHP_PSF_HOOK_UPD_RESET_GPIO_INIT(_u8Port_num_)      updreset_init(_u8Port_num_)
    
          void updreset_init(UINT8 u8Portnum);
    
          void updreset_init(UINT8 u8Portnum)
          {
            if (0 == u8Portnum)
            {
                //Initialization of MCU GPIOs connected to UPD350 reset lines
                //Make the gpio line high as default
            }
          }
        </code>
    Remarks:
        User definition of this Hook function is optional                      
*************************************************************************/
#define MCHP_PSF_HOOK_UPD_RESET_GPIO_INIT(_u8Port_num_) SAMD20_UPD350ResetGPIOInit(_u8Port_num_)              

/**********************************************************************
    Function:
        MCHP_PSF_HOOK_UPD_RESET_THRU_GPIO(_u8Port_num_)
    Summary:
        Resets the UPD350 connected specific to the port. 
        User can implement common reset pin for all ports. in this case user must reset UPD350 only for port 0.

    Description:
        This function resets UPD350 connected to the port. Define relevant
        \function that has port number as argument without return type.
    Conditions:
        None.
    Input:
        _u8Port_Num_ -  Port number of the device. Value passed will be less
                      than CONFIG_PD_PORT_COUNT
    Return:
        None.
  Example:
        <code>
            \#define MCHP_PSF_HOOK_UPD_RESET_THRU_GPIO(_u8Port_num_)      updreset_thru_gpio(_u8Port_num_)
    
            void updreset_thru_gpio(UINT8 u8Portnum);

            void updreset_thru_gpio(UINT8 u8Portnum)
            {
                if (0 == u8Portnum)
                {
                    //Enable pull down

                   // Wait for xxx uS

                   // Enable pull up
                }
            }
        </code>
  Remarks:
        User definition of this Hook function is optional                  
  **********************************************************************/
#define MCHP_PSF_HOOK_UPD_RESET_THRU_GPIO(_u8Port_num_)    SAMD20_ResetUPD350(_u8Port_num_)         

// *****************************************************************************
// *****************************************************************************
// Section: Structure packing
// *****************************************************************************
// *****************************************************************************
// *****************************************************************************

/**************************************************************************************************
    Summary:
        Structure packing to align the bytes in data memory based on the compiler.
    Description:
        Generally packed structures will be used to save space & align the bytes in data memory based on the compiler.
        If this pre-processor is defined then all used PD stack “C” structures will be replaced with keyword for compilation.
        If this pre-processor is not defined then it will be default compilation rules based on the compiler.
    Remarks:
        Need to be packed always based on type of microcontroller.	
    Example:
        <code>
            #define MCHP_PSF_STRUCT_PACKED_START   __attribute__((__packed__)) 
        </code>
**************************************************************************************************/
#define MCHP_PSF_STRUCT_PACKED_START   __attribute__((packed, aligned(4)))

/**************************************************************************************************
    Summary:
        Structure packing to align the bytes in data memory based on the compiler.
    Description:
        Generally packed structures will be used to save space & align the bytes in data memory based on the compiler.
        If this pre-processor is defined then all used PD stack “C” structures will be replaced with keyword for compilation.
        If this pre-processor is not defined then it will be default compilation rules based on the compiler.
    Remarks:
        Need to be packed always based on type of microcontroller.
    Example:
        <code>

        </code>
**************************************************************************************************/
#define MCHP_PSF_STRUCT_PACKED_END    
// *****************************************************************************
// *****************************************************************************
// Section: UPD350 Hardware Interface Configuration
// *****************************************************************************
// *****************************************************************************

/**************************************************************************
    Function:
        MCHP_PSF_HOOK_UPD_HW_INIT()
    Summary:
        Initialize the hardware interface(SPI/I2C) used for communicating with UPD350 part.
    Description:
        UPD350 supports both I2C and SPI interface. Depending on the UPD350 part used,
        the hardware communication interface between the SOC and UPD350 
        differs. UPD350 A and C supports I2C interface and UPD350 B and D part 
        supports SPI interface. This function is to initialize the interface as 
        required.It is called during initialization of stack. Define relevant 
        function that has no arguments without return type.
    Conditions:
        Use SPI interface for part UPD350 B and D.
        Use I2C interface for part UPD350 A and C.
    Return:
        UINT8 - Return TRUE if initialization was successful or else return FALSE. If hardware 
                interface initialization(SPI/I2C) fails and this API returns FALSE, all the PD ports
                are disabled by default.
    Example:
        <code>
            #define MCHP_PSF_HOOK_UPD_HW_INIT()      hw_spi_init()

            UINT8 hw_spi_init( void );

            UINT8 hw_spi_init( void )
            {
                //Intialise SOC's SPI master
                // Return TRUE if initialsation is successful else FALSE
            }

        </code>   
        <code>
            #define MCHP_PSF_HOOK_UPD_HW_INIT()      hw_i2c_init()

            UINT8 hw_i2c_init( void );

            UINT8 hw_i2c_init( void )
            {
                //Initialise SOC's I2C master
                // Return TRUE if initialsation is successful else FALSE
            }

        </code>
    Remarks:
        User definition of this Hook function is mandatory                     
**************************************************************************/
#define MCHP_PSF_HOOK_UPD_HW_INIT()      SAMD20_SPIInitialisation();              

/*************************************************************************************************
    Function:
        MCHP_PSF_HOOK_UPD_COMM_EN(_u8Port_Num_)
    Summary:
        Enable hardware interface(SPI/I2C) to the port's UPD350 for communication
    Description:
        This function is called before starting read/write operations of UPD350
        registers with respect to the port.This is to enable hardware 
        interface(I2C/SPI) communication to the port's UPD350 if required.

        For SPI interface, SPI chip select level shall be driven 
        low with respect to the port number given in the argument of this function 
        to enable SPI communication with port's UPD350.

        For I2C interface, I2C mux shall be configured to route the SOC's I2C master
        to the port number given in the argument of this function 
        to enable I2C communication with port's UPD350.

        Define relevant function that has one UINT8 argument with out return type.    
    Conditions:
        None.
    Input:
        _u8Port_Num_ -  Port number of the device. Value passed will be less
                        than CONFIG_PD_PORT_COUNT
    Return:
        None.
    Example:
        <code>

          \#define MCHP_PSF_HOOK_UPD_COMM_EN(_u8Port_Num_)      hw_spi_cs_low (_u8Port_Num_)

          void hw_spi_cs_low(UINT8 u8Portnum);

          void hw_spi_cs_low (UINT8 u8Portnum)
          {
              if (u8Portnum == 0)
              {
                  //Set pin level low for respective GPIO 
                      that is connected to the UPD350 SPI CS pin
              }
              if (u8Portnum == 1)
              {
                  //Set pin level low for respective GPIO 
                      that is connected to the UPD350 SPI CS pin
              }
          }
        </code>

        <code>

          \#define MCHP_PSF_HOOK_UPD_COMM_EN(_u8Port_Num_)      hw_enable_port_i2cmux_routing (_u8Port_Num_)

          void hw_enable_port_i2cmux_routing(UINT8 u8Portnum);

          void hw_enable_port_i2cmux_routing (UINT8 u8Portnum)
          {
              if (u8Portnum == 0)
              {
                  //Route the I2C mux to the Port 0
              }
              if (u8Portnum == 1)
              {
                  //Route the I2C mux to the Port 1
              }
          }
        </code>
    Remarks:
        User definition of this Hook function is mandatory for SPI Hardware interface                                           
*************************************************************************************************/
#define MCHP_PSF_HOOK_UPD_COMM_ENDIS(_u8Port_num_,u8EnDis) SAMD20_DriveChipSelect(_u8Port_num_,u8EnDis)   

/**********************************************************************************************
    Function:
        MCHP_PSF_HOOK_UPD_COMM_DIS(_u8Port_num_)
    Summary:
        Disable hardware interface(SPI/I2C) to the port's UPD350 for communication
    Description:
      This function is called after finishing read/write operations of UPD350
      registers with respect to the port.This is to disable hardware 
      interface(I2C/SPI) communication to the port's UPD350 if required.

      For SPI interface, SPI chip select level shall be driven 
      high with respect to the port number given in the argument of this function 
      to disable SPI communication with port's UPD350.

      For I2C interface, I2C mux routing the SOC's I2C master to the port number 
      given in the argument of this function shall be disabled.

      Define relevant function that has one UINT8 argument with out return type.
    Conditions:
        None.
    Input:
        _u8Port_num_ -  Port number of the device. Value passed will be less
                        than CONFIG_PD_PORT_COUNT
    Return:
        None.
    Example:
        <code>
          \#define MCHP_PSF_HOOK_UPD_COMM_DIS(_u8Port_num_)      hw_spi_cs_high (_u8Port_num)

          void hw_spi_cs_high(UINT8 u8Portnum);

          void hw_spi_cs_high (UINT8 u8Portnum)
          {
              if (u8Portnum == 0)
              {
                  //Set pin level high for respective GPIO that is connected to 
                      the UPD350 CS pin
              }
              if (u8Portnum == 1)
              {
                  //Set pin level high for respective GPIO that is connected to 
                      the UPD350 CS pin
              }
          }
        </code>

        <code>

          \#define MCHP_PSF_HOOK_UPD_COMM_EN(_u8Port_Num_)      hw_disable_port_i2cmux_routing (_u8Port_Num_)

          void hw_disable_port_i2cmux_routing(UINT8 u8Portnum);

          void hw_disable_port_i2cmux_routing (UINT8 u8Portnum)
          {
              if (u8Portnum == 0)
              {
                  //disbale the I2C mux routing to the Port 0
              }
              if (u8Portnum == 1)
              {
                  //disbale the I2C mux routing to the Port 1
              }
          }
        </code>
    Remarks:
        User definition of this Hook function is mandatory for SPI Hardware interface                                         
**********************************************************************************************/
#define MCHP_PSF_HOOK_UPD_COMM_DIS(_u8Port_num_) 

/*********************************************************************************************
    Function:
        MCHP_PSF_HOOK_UPD_WRITE(_u8Port_num_,_pu8write_buf_,_u8write_len_)
    Summary:
        Function to initiate a write transfer to UPD350 via I2C/SPI
    Description:
        This function is called to write to UPD350 registers specific to the port.
        This function definition must be confined to the definition 
        CONFIG_DEFINE_UPD350_HW_INTF_SEL.
        Define relevant function that has UINT8, UINT8*, UINT8 arguments
        without return type.
    Conditions:
        None.
    Input:
        _u8Port_num_ -  Port number of the device. Value passed will be less
                        than CONFIG_PD_PORT_COUNT.
        _pu8write_buf_ -  Stack shall pass the pointer to the buffer which has the data
                          to be written on the SPI/I2C Hardware bus. Data type of 
                          the pointer buffer must be UINT8*.
        _u8write_len_ -  Stack shall pass the Number of bytes to be written on
                          the SPI/I2C Hardware bus. Data type of this parameter must be
                          UINT8.
    Return:
        UINT8 - Return TRUE if write was successful or else return FALSE.
    Example:
        <code>
          #define MCHP_PSF_HOOK_UPD_WRITE(_u8Port_num_,_pu8write_buf_,_u8write_len_)   
                                                      SPI_Write (_u8Port_num_,_pu8write_buf_, _u8write_len_)

          UINT8 SPI_Write(UINT8 _u8Port_num_, UINT8 *pu8WriteBuffer, UINT8 u8Writelength);

          UINT8 SPI_Write(UINT8 _u8Port_num_, UINT8 *pu8WriteBuffer, UINT8 u8Writelength)
          {
              for(UINT8 u8Txcount = 0; u8Txcount < u16Writelength; u8Txcount++)
              {
                  //Write data bytes to SPI bus
              }
              // Return TRUE if the write is successful; else FALSE
          }
        </code>
        <code>
          #define MCHP_PSF_HOOK_UPD_WRITE(_u8Port_num_,_pu8write_buf_,_u8write_len_)   
                                                      I2C_Write (_u8Port_num_,_pu8write_buf_, _u8write_len_)

          UINT8 I2C_Write(UINT8 _u8Port_num_, UINT8 *pu8WriteBuffer, UINT8 u8Writelength);

          UINT8 I2C_Write(UINT8 _u8Port_num_, UINT8 *pu8WriteBuffer, UINT8 u8Writelength)
          {
              // Select I2C address for the UPD350 I2C slave using _u8Port_num_ 

              for(UINT8 u8Txcount = 0; u8Txcount < u16Writelength; u8Txcount++)
              {
                  //Write data bytes to I2C bus
              }
               // Return TRUE if the write is successful; else FALSE
          }
        </code>
    Remarks:
        User definition of this Hook function is mandatory                                        
*********************************************************************************************/
#define MCHP_PSF_HOOK_UPD_WRITE(_u8Port_num_,_pu8write_buf_,_u8write_len_) SAMD20_SPIWritedriver(_u8Port_num_,_pu8write_buf_,_u8write_len_)

/***************************************************************************************
    Function:
        MCHP_PSF_HOOK_UPD_READ(_u8Port_num_,_pu8write_buf_,_u8write_len_,_pu8read_buf_,_u8read_len_)   
    Summary:
        Function to initiate a read transfer to UPD350 via I2C/SPI
    Description:
        This function is called to read to UPD350 registers with respect to the
        port.
        This function definition must be confined to the definition 
        CONFIG_DEFINE_UPD350_HW_INTF_SEL.
        Define relevant function that has UINT8, UINT8*, UINT8, UINT8*, UINT8 arguments
        without return type.
    Conditions:
        None.
    Input:
        _u8Port_num_ -  Port number of the device. Value passed will be less
                        than CONFIG_PD_PORT_COUNT.
        _pu8write_buf_ -  Stack shall pass the pointer to the buffer which has the data
                          to be written on the SPI/I2C Hardware bus. It contains the Register
                          address to be read. Data type of the pointer buffer must be UINT8*.
        _u8write_len_ -  Stack shall pass the Number of bytes to be written on
                          the SPI/I2C Hardware bus. Data type of this parameter must be
                          UINT8.
        _pu8read_buf_ -  Stack will pass the pointer to the buffer where data to
                         be read on the SPI/I2C bus. Data type of the pointer buffer
                         must be UINT8*
        _u8read_len_ -  Stack will pass the number of bytes to be read on the
                         SPI/I2C bus. Data type of this parameter must be UINT8
    Return:
        UINT8 - Return TRUE if read was successful or else return FALSE.
    Example:
        <code>
          #define MCHP_PSF_HOOK_UPD_READ(_u8Port_num_,_pu8write_buf_,_u8write_len_,_pu8read_buf_,_u8read_len_)  \\
                    SPI_Read (_u8Port_num_,_pu8write_buf_,_u8write_len_,_pu8read_buf_,_u8read_len_)

          void SPI_Read (UINT8 u8Portnum, UINT8 *pu8WriteBuffer, UINT8 u8Writelength, UINT8 *pu8ReadBuffer, UINT8 u8Readlength)

          void SPI_Read (UINT8 u8Portnum, UINT8 *pu8WriteBuffer, UINT8 u8Writelength, UINT8 *pu8ReadBuffer, UINT16 u8Readlength)
          {
            for(UINT8 u8Txcount = 0; u8Txcount < u16Writelength; u8Txcount++)
            {
                //Write data bytes to SPI bus
            }
            for(UINT8 u8Rxcount = 0; u8Rxcount< u8Readlength; u8Rxcount++)
            {
                  //Read data from SPI bus
            }
            // Return TRUE if the read is successful; else FALSE
          }
        </code>
        <code>
          #define MCHP_PSF_HOOK_UPD_READ(_u8Port_num_,_pu8write_buf_,_u8write_len_,_pu8read_buf_,_u8read_len_)  \\
                            I2C_Read(_u8Port_num_,_pu8write_buf_,_u8write_len_,_pu8read_buf_,_u8read_len_)

          void I2C_Read (UINT8 u8Portnum, UINT8 *pu8WriteBuffer, UINT8 u8Writelength, UINT8 *pu8ReadBuffer, UINT16 u8Readlength)

          void I2C_Read (UINT8 u8Portnum, UINT8 *pu8WriteBuffer, UINT8 u8Writelength, UINT8 *pu8ReadBuffer, UINT16 u8Readlength)
          {
            //Select I2C address for the UPD350 I2C slave using u8Portnum 
            for(UINT8 u8Txcount = 0; u8Txcount < u16Writelength; u8Txcount++)
            {
                //Write data bytes to I2C bus
            }
            for(UINT8 u8Rxcount = 0; u8Rxcount< u8Readlength; u8Rxcount++)
            {
                //Read data from I2C bus
            }
            // Return TRUE if the read is successful; else  return FALSE
          }
        </code>
    Remarks:
        User definition of this Hook function is mandatory                                  
***************************************************************************************/
#define MCHP_PSF_HOOK_UPD_READ(_u8Port_num_,_pu8write_buf_,_u8write_len_,_pu8read_buf_, _u8read_len_)\
    SAMD20_SPIReaddriver(_u8Port_num_,_pu8write_buf_,_u8write_len_,_pu8read_buf_, _u8read_len_)
                                           
/************************************************************************
    Function:
        MCHP_PSF_HOOK_MEMCMP(_OBJ1_, _OBJ2_, _LEN_)
    Summary:
        Compare two memory regions
    Description:
        This function is called to compare two memory regions _OBJ1_ &amp;
        _OBJ2_ with specified length _LEN_. USER_APPLICATION must define this
        \function based on compiler of MCU.
    Conditions:
        None.
    Input:
        _OBJ1_ -  This is the pointer to block of Memory region 1
        _OBJ2_ -  This is the pointer to block of Memory region 2
        _LEN_ -   This is the number of bytes to be compared.
    Return:
        \Returns 0 if two memory regions are same.
    Example:
        <code>
            \#define MCHP_PSF_HOOK_MEMCMP(_OBJ1_, _OBJ2_, _LEN_)  \\
                                             memcmp(_OBJ1_, _OBJ2_, _LEN_)
        </code>
    Remarks:
        User definition of this Hook function is mandatory                   
  ************************************************************************/
#define MCHP_PSF_HOOK_MEMCMP(_OBJ1_, _OBJ2_, _LEN_) SAMD20_MemCmp(_OBJ1_, _OBJ2_, _LEN_)                           

/**************************************************************************
    Function:
        MCHP_PSF_HOOK_MEMCPY(_DEST_, _SRC_, _LEN_)
    Summary:
        Copies one memory area to another memory area
    Description:
        This function is called to copy _LEN_ bytes from _SRC_ memory area to
        _DEST_ memory area. USER_APPLICATION must define this function based on
        compiler of MCU. The memory areas must not overlap.
    Conditions:
        None.
    Input:
        _DEST_ -  This is the pointer to block of destination memory region
        _SRC_ -   This is the pointer to block of source memory region
        _LEN_ -   This is the number of bytes to be copied.
    Return:
        \Returns a pointer to _DEST_.
    Example:
        <code>
          \#define MCHP_PSF_HOOK_MEMCPY(_DEST_, _SRC_, _LEN_)  \\
                                        memcpy(_DEST_, _SRC_, _LEN_)

          //This function definition can be compiler defined or user defined
        </code>
    Remarks:
        User definition of this Hook function is mandatory                     
**************************************************************************/
#define MCHP_PSF_HOOK_MEMCPY(_DEST_, _SRC_, _LEN_) SAMD20_MemCpy(_DEST_, _SRC_, _LEN_)
                                                                           
/**************************************************************************
    Function:
        MSCH_PSF_HOOK_GETCURRENT_IMAGEBANK()
    Summary:
        To Return the Index of the Image Bank which is currently executing.
        \Example:
        \1. 0x00 - Corresponds to Bootloader Application
        \2. 0x01 - Corresponds to Fixed Application
        \3. 0x02 - Corresponds to Updatable Application
    Description:
        This function is called to get the Index of the Image Bank which is
        currently executing. Zeus Stack follows "Architecture 2 - Fixed base
        application with updatable application image" In which the Fixed
        Application is Image Bank 1 and Updatable Application is Image Bank 2.
    Conditions:
        This function is invoked by the PD Firmware Update State-machine during
        the Enumeration Phase (On reception PDFU_GET_FW_ID Command).
    Return:
        \Returns Byte Value -That is the index of the Image Bank.
        IMAGE_BANK_BOOTLOADER 0x01
        IMAGE_BANK_FIXED_APP 0x02
        IMAGE_BANK_UPDATABLE_APP 0x03
    Example:
        <code>
            \#define MSCH_PSF_HOOK_GETCURRENT_IMAGEBANK()  getCurrentImageBank()

            UINT8 getCurrentImageBank(void)
            {
               \return u8ImageBankIndex;
            }
        </code>
    Remarks:
        The User definition of the function is mandatory in both Fixed and
        Updatable application when INCLUDE_PDFU is TRUE.                 
**************************************************************************/
#define MSCH_PSF_HOOK_GETCURRENT_IMAGEBANK()

/****************************************************************************************
    Function:
        MSCH_PSF_HOOK_PROGRAM_FWBLOCK( _OBJ1_,_LEN_)
    Summary:
        Validate the Data block and program the data to the Memory, and return
        the Status of the Programming Operation.
    Description:
        This Function is invoked during the Transfer Phase on the Successful
        reception event of every PDFU DATA packet. This function is responsible
        for Updating the Firmware data to the memory, and identifying any
        errors during the Firmware flash.
    Conditions:
        Only during the Policy Engine State -Reconfigure Phase or Transfer
        Phase this function will be invoked.
    Input:
        _OBJ1_ -  UINT8 Pointer to PDFU_DATA packet payload Buffer. _OBJ1_[0]
                  Reserved field Contains PD FW Version. OBJ1_[1]
                  Reserved field Contains Msg Type which is PDFU_DATA 0x83. 
                  _OBJ1_[2] LSB of Data Block Index. _OBJ1_[3] MSB of Data Block
                  Index _OBJ1_[4..260] Firmware Image data upto 256
                  bytes. Where, the Data block index is used to
                  calculate the Physical memory address to which the current
                  data block corresponds to 16 bit parameter.
        _LEN_ -   Indicates the length of the Firmware data
                  contained in the packet.
    Return:
        \Returns ePE_PDFU_RESPONSE_CODE Type Return Value - The Status of the
        Program Operation.
          * ePE_FWUP_OK &#45; Upon successful flash operation.
          * ePE_FWUP_errVERIFY &#45; When verification of the flash operation
            failed.
          * ePE_FWUP_errADDRESS &#45; when data block index is out of range.
    Example:
        <code>
            \#define MSCH_PSF_HOOK_PROGRAM_FWBLOCK(_OBJ1_, _LEN_)     (PDFW_ProcessPDFUDataRequest)
        </code>
            ePE_PDFU_RESPONSE_CODE PDFW_ProcessPDFUDataRequest( UINT8 u8RequestBuffer, \
                                                           UINT16 u16RequestLength)
            {
                UINT16 u16DataBlockIndex = *((UINT16*)&amp;u8RequestBuffer[2]);
                u32ProgAddr = CalculateAddress(u16DataBlockIndex);
                if( u32ProgAddr \< 0xFFFFu )
                {
                    ProgramMemoryCB(u32ProgAddr, &amp;u8RequestBuffer[4u],
                                    u16RequestLength);
                    ReadMemoryCB(u32ProgAddr, &amp;u8ResponseBuffer[0u], u16RequestLength);
                    
                    //Compare data written and read
                    if (MCHP_PSF_HOOK_MEMCMP(&amp;u8ResponseBuffer[0], &amp;u8RequestBuffer[4],
                     u16RequestLength) == 0)
                    {
                        //Set the status as OK
                        u8Status = ePE_FWUP_OK;
                    }
                    else
                    {
                        //Verification Stage failure
                        u8Status = ePE_FWUP_errVERIFY;
                    }
                }

                else
                {
                    u8Status = ePE_FWUP_errADDRESS;
                }

                return u8Status;
            }
        </code>
    Remarks:
        User definition of this Hook function is mandatory when
        INCLUDE_PDFU is TRUE.                                                          
 ****************************************************************************************/
#define MSCH_PSF_HOOK_PROGRAM_FWBLOCK(_OBJ1_,_LEN_)

/**************************************************************************************************
    Function:
        MSCH_PSF_HOOK_VALIDATE_FIRMWARE(_OBJ1_,_LEN_)
    Summary:
        To validate the Flashed Binary using a User defined validation method
        and return the Status of the Firmware Validation Operation.
    Description:
        This Function is invoked during the validation phase on reception of
        every PDFU Validation Request.
        This function is responsible for Validating the Firmware data in the
        memory. The Function shall return the progress status of the Validation
        on every invocation. If the Status indicates "On going" then the
        Validation command will be responded with the configured Wait time.
        Validation Method can be any Custom method as required by the User.
    Conditions:
        Multiple invocations of the function is possible from PDFU
        Validation phase.
        Until the Validation process is complete, for every request of
        PDFU_VALIDATION command this function will be invoked.
        
        The definition of this function shall include 1) Starting the
        Validation process on the First call, 2) Returning the Status of the
        Validation process during subsequent invocation of the function.
    Return:
        \Returns the UINT8 Status of the Validation Operation.
        * PE_FWUP_VALIDATION_SUCCESSFUL 0x00u
        * PE_FWUP_VALIDATION_INPROGRESS 0x01u
        * PE_FWUP_VALIDATION_FAILURE 0x02u
    Example:
        <code>
            \#define MSCH_PSF_HOOK_VALIDATE_FIRMWARE() (PDFW_ProcessPDFUDataRequest)

            UINT8 PDFW_ProcessPDFUValidateRequest(void)
            {    
                The definition of this function shall include
                1) Starting the Validation process on the First call,
                2) Returning the Status of the Validation process during subsequent invocation of the function.
            }
        </code>
    Remarks:
        User definition of this Hook function is mandatory when INCLUDE_PDFU is
        TRUE                                                                                           
**************************************************************************************************/
#define MSCH_PSF_HOOK_VALIDATE_FIRMWARE()

/**************************************************************************
    Function:
        MSCH_PSF_HOOK_BOOT_FIXED_APP()
    Summary:
        MSCH_PSF_HOOK_BOOT_FIXED_APP shall perform necessary operation to
        switch from Updatable application to Fixed application.
    Description:
        Re-flash of the Updatable_Application image bank while currently
        executing in the Updatable_Application image bank, requires switch to
        Fixed application for performing the upgrade.
        The application switching may include invalidating the
        Updatable_Application signatures (and/or) jump/reset for fresh boot
        from Fixed application.
    Conditions:
        This function is invoked by the PD Firmware Update State-machine during
        the Reconfiguration phase(On reception PDFU_INITIATE Command), when the
        Updatable application is currently running.
    Return:
        No Return Value. During execution of this function the control shall be
        switched to the Fixed application.
    Example:
        <code>
            \#define MSCH_PSF_HOOK_BOOT_FIXED_APP()   Boot_Fixed_Application()
            void Boot_Fixed_Application(void)
            {
                UPD301_EraseUpdatableAppSignature(); //Invalidate the Updatable app sign
                UPD301_Reset() //Reset to boot from Fixed app
            }
        </code>
    Remarks:
        User definition of this Hook function is mandatory in the Updatable
        application when INCLUDE_PDFU is TRUE                            
  **************************************************************************/

#define MSCH_PSF_HOOK_BOOT_FIXED_APP()          
 /***********************************************************************************
    Function:
        MSCH_PSF_HOOK_BOOT_UPDATABLE_APP()
    Summary:
        MSCH_PSF_HOOK_BOOT_UPDATABLE_APP shall perform necessary
        operation to boot from the updatable application after a PDFU is
        successfully completed.
    Description:
        As the flashing operation is executed from the Fixed application, once
        the PDFU process is complete it is necessary to switch to the newly
        upgraded updatable application.
        This function definition shall implement necessary operations to safely
        exit the fixed application and boot from the updatable application.
        The application switching may include setting the valid
        Updatable_Application signatures (and) jump/reset for fresh boot from
        Updatable application.
    Conditions:
        This function is invoked by the PD Firmware Update State-machine during
        the Manifestation phase(On reception PDFU_INITIATE Command), when the
        Fixed application is currently running.
    Return:
        No Return Value. During execution of this function the control shall be
        switched to the Updatable application.
   Example:
        <code>
            \#define MSCH_PSF_HOOK_BOOT_UPDATABLE_APP()   Boot_Updatable_Application()
     
            void Boot_Updatable_Application(void)
            {
                UPD301_Reset() //Reset to boot from Fixed app
            }
        </code>
   Remarks:
        User definition of this Hook function is mandatory in the Fixed
        application when INCLUDE_PDFU is TRUE                                     
***********************************************************************************/ 

#define MSCH_PSF_HOOK_BOOT_UPDATABLE_APP()         
                                                  
/**************************************************************************
    Function:
        MSCH_PSF_HOOK_IS_PDFU_ALLOWED_NOW()
    Description:
        MSCH_PSF_HOOK_IS_PDFU_ALLOWED_NOW specifies if PD Firmware Update can be
        currently allowed, based on the priority of the application tasks
        currently executing.
        1. When the PD Firmware Update is allowed, the PDFU Initiator can
        perform firmware upgrade by the PDFU Sequence
        2. When the PD Firmware Update is not allowed, the PDFU Initiator is
        responded with the error code during the Reconfiguration phase.
  
        Example scenario of When PDFU cannot be allowed:
        Assuming a product with firmware update capability through CC and I2C
        as well. In an example, if the firmware upgrade through I2C is already
        in progress, then PDFU through CC interface shall not be allowed. To
        handle such situations, this macro shall return the current status of
        allowability of firmware upgrade.
    Conditions:
        This function is invoked by the PD Firmware Update State-machine during
        the Reconfiguration Phase (On reception PDFU_INITIATE Command).
    Remarks:
        Shall return the run time status whether PDFU is allowed now or not.
    Return:
        UINT8 value
        0x00 - PDFU Not Allowed.
        0x01 - PDFU Allowed.
    Example:
        <code>
            \#define MSCH_PSF_HOOK_IS_PDFU_ALLOWED_NOW   isPdfuAllowedNow()
            UINT8 isPdfuAllowedNow(void)
            {
                return u8PdfuAllow;
            }
        </code>
**************************************************************************/
#define MSCH_PSF_HOOK_IS_PDFU_ALLOWED_NOW()

// *****************************************************************************
// *****************************************************************************
// Section: Power Fault APIs
// *****************************************************************************
// *****************************************************************************

/*********************************************************************************
    Function:
        CONFIG_HOOK_NOTIFY_POWER_FAULT(_u8PortNum_, _pu8PwrFaultsts_)
    Summary:
        Power Fault Notification from stack.
    Description:
        This hook notifies the type of Power fault that has occurred from DPM Power Fault 
        Manager. 
        This Hook gives user have options to
                i)  Ignore power fault handling for that instance of DPM Power fault manager:
                    User should return FALSE to ignore the instance.
                ii) Ignore the power fault occurrence:
                    User should clear the fault occurrence indication in  power fault status
                    pointer  _pu8PwrFaultsts_ as well as return FALSE to ignore the instance.
                iii) Allow DPM power fault handler to handle it:
                    User should return TRUE for DPM power fault manager to handle it.
                    Incase of explicit contract and power fault count is less than 
                    CONFIG_MAX_VBUS_POWER_FAULT_COUNT, DPM power fault manager handles
                    by sending Hard Reset. Incase of implicit contract, it handles by
                    entering TypeC Error Recovery. 
    Conditions:
        None. 
    Input:
        _u8Port_Num_    -  Port number of the device. Value passed will be less
                            than CONFIG_PD_PORT_COUNT
        _pu8PwrFaultsts_ - Pointer to Power fault status for the port. The value *_pu8PwrFaultsts_
                            can be interpreted from following.
                            BIT(0) - DPM_POWER_FAULT_OVP (Over voltage fault)          
                            BIT(1) - DPM_POWER_FAULT_UV (Under voltage)
                            BIT(2) - DPM_POWER_FAULT_VBUS_OCS (VBUS Over-current fault)
                            BIT(3) - DPM_POWER_FAULT_VCONN_OCS (VCONN over-current fault)
                            This pointer can be modified to disable the occurred power fault.
    Return:
        UINT8 is the return value. Depending on the return value occurred Power fault is handled
        by DPM Power Fault Manager.
        Return value can be
        TRUE - for stack to handle occurred power fault 
        FALSE - for stack to neglect the Power fault occurred for that instance
        Note that returning FALSE will not reset power fault that has occurred. 
        It is ignored Power fault handling for that instance alone.
    Example:
        <code>
            \#define CONFIG_HOOK_NOTIFY_POWER_FAULT(_u8PortNum_, _pu8PwrFaultsts_)  

            UINT8 UPD301_HandlePowerFault(UINT8 u8PortNum, UINT8 *pu8PwrFaultSts);

            UINT8 UPD301_HandlePowerFault(UINT8 u8PortNum, UINT8 *pu8PwrFaultSts)
            {
                Return MCHP_PSF_HANDLE_PWR_FAULT(TRUE) to handle occured power fault.
                Return MCHP_PSF_NEGLECT_PWR_FAULT(FALSE) to neglect the Power fault occurred 
                                                    for that instance.
                Return MCHP_PSF_NEGLECT_PWR_FAULT(FALSE) and reset the power fault status to
                                            ignore the power fault occurred completely.
            }
        </code>
    Remarks:
        User definition of this Hook function is mandatory.
*********************************************************************************/                                             
#define CONFIG_HOOK_NOTIFY_POWER_FAULT(_u8PortNum_, _pu8PwrFaultsts_)

#define MCHP_PSF_HANDLE_PWR_FAULT	TRUE
#define MCHP_PSF_NEGLECT_PWR_FAULT	FALSE

/**********************************Globals*********************************************/
/* User Configurable Structure at boot time: This structure contains the Type-C configure 
   parameters, PDO count and PDOs parameters*/

typedef struct MCHP_PSF_STRUCT_PACKED_START _PortData_cfg 
{
    UINT32 u32CfgData;			//Bits 2:0 - Port Role <p />
								//Bits 4:3 - Type-C Current (Applicable only for Source)
                                //Bits 5 - Port Enable / Disable
    UINT32 u32PDO[7];		    //Source/Sink Capabilities PDOs
    UINT8  u8PDOCnt;			//PDO count of Source/Sink Capabilities
}MCHP_PSF_STRUCT_PACKED_END PORT_CONFIG_DATA;

extern PORT_CONFIG_DATA gasPortConfigurationData[CONFIG_PD_PORT_COUNT];



/****************************************************************************
    Function:
        UINT8 MchpPSF_Init(void)

    Summary:
        PSF Stack initialisation API 

    Description:
        This API should be called by the SOC layer to initialize the 
        PSF stack and UPD350 Hardware.

    Conditions:
        API should be called before MchpPSF_RUN().

    Input:
        None

    Return:
        TRUE - Stack and UPD350 HW successfully initialized.
        FALSE - Stack and UPD350 HW initialization failed.
 
    Remarks:
        For the current PSF implementation, return value is not used. API called with void.

**************************************************************************************************/
UINT8 MchpPSF_Init(void);

/**************************************************************************************************
    Function:
        void MchpPSF_RUN(void)

    Summary:
        PSF Stack state machine run API

    Description:
        This API is to run the PSF state machine. 
        For single threaded environment, it should be called repeatedly within a while(1).
         
    Conditions:
        API should be called only after MchpPSF_Init().

    Input:
        None

    Return:
        None

    Remarks:
        Multi threaded Free RTOS environment is untested and latency of API call is to
        identified yet.

**************************************************************************************************/
void MchpPSF_RUN(void);

/**************************************************************************************************
    Function:
        void MchpPSF_UPDAlertISR(UINT8 u8PortNum)

    Summary:
        UPD350 Alert Interrupt Handler 

    Description:
        This API is used to handle the UPD350 Alert Interrupt, User should call this 
        API when the Alert line interrupt triggered to the MCU. This API will
        services and then clear the Alert interrupt for corresponding port. 
         
    Conditions:
        This API should be called inside the GPIO ISR for Alert interrupt 

    Input:
        u8PortNum - Port Number

    Return:
        None

    Remarks:
        None

**************************************************************************************************/
void MchpPSF_UPDAlertISR(UINT8 u8PortNum);

/**************************************************************************************************
    Function:
        void MchpPSF_PDTimerISR(void)

    Summary:
        PD Timer Interrupt Handler 

    Description:
        This API is used to handle the PD Timer (Software timer) Interrupt, User should call this 
        API when the hardware timer interrupt triggered. 
         
    Conditions:
        This API should be called inside the Hardware timer ISR. 

    Input:
        None

    Return:
        None

    Remarks:
        None

**************************************************************************************************/
void MchpPSF_PDTimerISR(void);

#endif /*_PSF_API_HOOK_H_*/
