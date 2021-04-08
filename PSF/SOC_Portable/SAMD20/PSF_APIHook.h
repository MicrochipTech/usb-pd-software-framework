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

Copyright �  [2019-2020] Microchip Technology Inc. and its subsidiaries.

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

#include "Drivers.h"


// *****************************************************************************
// *****************************************************************************
// Section: UPD350 Hardware Interface Configuration
// *****************************************************************************
// *****************************************************************************
/**************************************************************************
Function:
    MCHP_PSF_HOOK_UPDHW_INTF_INIT()
Summary:
    Initialize the hardware interface(SPI/I2C) used for communicating with UPD350 part.
Description:
    PSF requires a Hardware interface from SOC(either SPI or I2C) to communicate with UPD350. UPD350 
    supports either I2C or SPI interface depending on UPD350 SKU used.  UPD350 A and C SKUs support
    I2C interface and UPD350 B and D SKUs support SPI interface. 
    This Hook is used to initialize the SOC's Hardware interface for communication. It is called 
    during initialization of PSF. This hook is assigned to a function that takes no arguments and
    has a return type of UINT8 which indicates if the initialization was successful or not. 
Conditions:
    Use SPI interface for part UPD350 B and D.
    Use I2C interface for part UPD350 A and C.
Return:
    UINT8 - Return TRUE if initialization was successful or else return FALSE. If hardware 
            interface initialization(SPI/I2C) fails and the API returns FALSE, all the PD ports
            are disabled by PSF by default.
Example:
    <code>
        #define MCHP_PSF_HOOK_UPDHW_INTF_INIT()      hw_spi_init()
        UINT8 hw_spi_init(void);
        UINT8 hw_spi_init(void)
        {
            //Intialise SOC's SPI master
            //Return TRUE if initialsation is successful else FALSE
        }
    </code>
    <code>
        #define MCHP_PSF_HOOK_UPDHW_INTF_INIT()      hw_i2c_init()
        UINT8 hw_i2c_init(void);
        UINT8 hw_i2c_init(void)
        {
            //Initialise SOC's I2C master
            //Return TRUE if initialsation is successful else FALSE
        }
    </code>
Remarks:
    User definition of this Hook function is mandatory.                    
**************************************************************************/
#define MCHP_PSF_HOOK_UPDHW_INTF_INIT()      SAMD20_SPIInitialisation();              

/*********************************************************************************************
Function:
    MCHP_PSF_HOOK_UPD_WRITE(u8PortNum,pu8WriteBuf,u8WriteLen)
Summary:
    Initiates a write transfer to UPD350 via I2C/SPI
Description:
    This hook is used to write to the registers of the UPD350 of a given port over the chosen
    communication interface defined by CONFIG_DEFINE_UPD350_HW_INTF_SEL. This hook is assigned to
    a function that takes arguments of type UINT8, UINT8 *, UINT8, and has a return type of UINT8.
Conditions:
    None.
Input:
    u8PortNum -  Port number of the device. It takes value between 0 to (CONFIG_PD_PORT_COUNT-1).
    pu8WriteBuf - PSF shall pass the pointer to the buffer which has the data to be written on the
                   SPI/I2C Hardware bus. Data type of the pointer buffer must be UINT8*.
    u8WriteLen - PSF shall pass the Number of bytes to be written on the SPI/I2C Hardware bus. Data
                    type of this parameter must be UINT8.
Return:
    UINT8 - Return TRUE if write was successful or else return FALSE.
Example:
    <code>
        #define MCHP_PSF_HOOK_UPD_WRITE(u8PortNum,pu8WriteBuf,u8WriteLen)\   
					SPI_Write (u8PortNum,pu8WriteBuf,u8WriteLen)
        UINT8 SPI_Write(UINT8 u8PortNum, UINT8 *pu8WriteBuffer, UINT8 u8Writelength);
        UINT8 SPI_Write(UINT8 u8PortNum, UINT8 *pu8WriteBuffer, UINT8 u8Writelength)
        {
            for(UINT8 u8Txcount = 0; u8Txcount < u16Writelength; u8Txcount++)
            {
                //Write data bytes to SPI bus
            }
            // Return TRUE if the write is successful; else FALSE
        }
    </code>

    <code>
        #define MCHP_PSF_HOOK_UPD_WRITE(u8PortNum,pu8WriteBuf,u8WriteLen)\
					I2C_Write (u8PortNum,pu8WriteBuf,u8WriteLen)
        UINT8 I2C_Write(UINT8 u8PortNum, UINT8 *pu8WriteBuffer, UINT8 u8Writelength);
        UINT8 I2C_Write(UINT8 u8PortNum, UINT8 *pu8WriteBuffer, UINT8 u8Writelength)
        {
            // Select I2C address for the UPD350 I2C slave using u8PortNum 
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
#define MCHP_PSF_HOOK_UPD_WRITE(u8PortNum,pu8WriteBuf,u8WriteLen)\
            SAMD20_SPIWritedriver (u8PortNum,pu8WriteBuf,u8WriteLen)

/***************************************************************************************
Function:
    MCHP_PSF_HOOK_UPD_READ(u8PortNum,pu8WriteBuf,u8WriteLen,pu8ReadBuf,u8ReadLen)   
Summary:
    Initiates a read transfer to UPD350 via I2C/SPI
Description:
    This hook is used to read the registers of the UPD350 of a given port over the chosen 
    communication interface defined by CONFIG_DEFINE_UPD350_HW_INTF_SEL. This hook is assigned to
    a function that takes arguments of type UNIT8, UINT8 *, UINT8, UINT8 *, UNIT8 and has a return
    type of UINT8.
Conditions:
    None.
Input:
    u8PortNum -  Port number of the device. It takes value between 0 to (CONFIG_PD_PORT_COUNT-1).
    pu8WriteBuf - PSF shall pass the pointer to the buffer which has the data to be written on
                    the SPI/I2C Hardware bus before read. It contains the Register address to be 
                    read. Data type of the pointer buffer must be UINT8*.
    u8WriteLen - PSF shall pass the Number of bytes to be written on the SPI/I2C Hardware bus. 
                    Data type of this parameter must be UINT8.
    pu8ReadBuf - PSF will pass the pointer to the buffer where data read from the SPI/I2C bus to
                    be stored. Data type of the pointer buffer must be UINT8*.
    u8ReadLen - PSF will pass the number of bytes to be read on the SPI/I2C bus. Data type of this
                    parameter must be UINT8.
Return:
    UINT8 - Return TRUE if read was successful or else return FALSE.
Example:
    <code>
        #define MCHP_PSF_HOOK_UPD_READ(u8PortNum,pu8WriteBuf,u8WriteLen,pu8ReadBuf,u8ReadLen)  \
                SPI_Read (u8PortNum,pu8WriteBuf,u8WriteLen,pu8ReadBuf,u8ReadLen)
        void SPI_Read (UINT8 u8PortNum, UINT8 *pu8WriteBuffer, UINT8 u8Writelength, \
								UINT8 *pu8ReadBuffer, UINT8 u8Readlength);
        void SPI_Read (UINT8 u8PortNum, UINT8 *pu8WriteBuffer, UINT8 u8Writelength, \
								UINT8 *pu8ReadBuffer, UINT16 u8Readlength)
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
        #define MCHP_PSF_HOOK_UPD_READ(u8PortNum,pu8WriteBuf,u8WriteLen,pu8ReadBuf,u8ReadLen)\
                        I2C_Read(u8PortNum,pu8WriteBuf,u8WriteLen,pu8ReadBuf,u8ReadLen)

        void I2C_Read (UINT8 u8PortNum, UINT8 *pu8WriteBuffer, UINT8 u8Writelength, \
								UINT8 *pu8ReadBuffer, UINT16 u8Readlength);
        void I2C_Read (UINT8 u8PortNum, UINT8 *pu8WriteBuffer, UINT8 u8Writelength, \
								UINT8 *pu8ReadBuffer, UINT16 u8Readlength)
        {
            //Select I2C address for the UPD350 I2C slave using u8PortNum 
            for(UINT8 u8Txcount = 0; u8Txcount < u16Writelength; u8Txcount++)
            {
                //Write data bytes to I2C bus
            }
            for(UINT8 u8Rxcount = 0; u8Rxcount< u8Readlength; u8Rxcount++)
            {
                //Read data from I2C bus
            }
            // Return TRUE if the read is successful else  return FALSE
        }
    </code>
Remarks:
    User definition of this Hook function is mandatory.                                 
***************************************************************************************/
#define MCHP_PSF_HOOK_UPD_READ(u8PortNum,pu8WriteBuf,u8WriteLen,pu8ReadBuf, u8ReadLen)\
            SAMD20_SPIReaddriver (u8PortNum,pu8WriteBuf,u8WriteLen,pu8ReadBuf, u8ReadLen)


// *****************************************************************************
// Section: PDTimer configuration
// *****************************************************************************
// *****************************************************************************
/***********************************************************************
Function:
    MCHP_PSF_HOOK_HW_PDTIMER_INIT() 
Summary:
    Hook to Initialize and start the hardware timer module.
Description:
    PSF requires a single dedicated hardware timer module for synchronizing the various state
    machines in the stack. This Hook initializes and starts the hardware timer module for 
    MCHP_PSF_PDTIMER_INTERRUPT_RATE interrupt frequency. To inform PSF about the occurrence 
    of hardware timer interrupt, MchpPSF_PDTimerHandler should be called by the SOC layer on every
    occurrence of the timer interrupt. This hook is assigned to a function that takes no arguments
    and has a return type of UINT8. 
Conditions:
    None.
Return:
    UINT8 - Returns TRUE if initialization is successful else FALSE. If Timer initialization fails 
            and the API returns FALSE, all the PD ports are disabled by PSF by default.
Example:
    <code>
        #define MCHP_PSF_HOOK_HW_PDTIMER_INIT()		Timer_Init()
        UINT8 Timer_Init(void);
        UINT8 Timer_Init(void)
        {
			//Initialize and start the SOC timer module for MCHP_PSF_PDTIMER_INTERRUPT_RATE 
            //interrupt frequency & register MchpPSF_PDTimerHandler as callback
            //Return TRUE if Timer initialisation is successful else return FALSE
        }
    </code>
Remarks:
    User definition of this Hook function is mandatory                  
***********************************************************************/			      
#define MCHP_PSF_HOOK_HW_PDTIMER_INIT()   SAMD20_HWTimerInit() 

/**************************************************************************************************
Summary:
	Rate at which the Hardware PD timer generates an interrupt 
Description:
    MCHP_PSF_PDTIMER_INTERRUPT_RATE defines the frequency of interrupt set in the hardware timer 
    dedicated for PSF. In other words, it is the resolution of the hardware timer. It can be 
    configured depending on the resolution of the hardware timer available.
Remarks:
    Resolution of the hardware timer has to be at least 1ms. Tested resolution values of hardware 
    timer is 1ms.(Corresponding MCHP_PSF_PDTIMER_INTERRUPT_RATE value is 1000).
Example:
    <code>
        #define MCHP_PSF_PDTIMER_INTERRUPT_RATE		1000 
		(1000 interrupts per second, with interrupt interval or resolution of 1ms)
    </code>
**************************************************************************************************/
#define MCHP_PSF_PDTIMER_INTERRUPT_RATE	1000

/**************************************************************************************************
Summary:
	Config 16-bit PD Timer Counter
Description :
    MCHP_PSF_CONFIG_16BIT_PDTIMER_COUNTER can be defined as either 1 or 0 to set the timeout counter in PSF 
    to unsigned 16bit or unsigned 32bit correspondingly. When set as 1, maximum timeout that can be 
    set will be 65535 ticks.(Ticks = Resolution of the Hardware timer used). When set as 0, maximum 
    timeout that can be set will be 4294967296 ticks. Default value of MCHP_PSF_CONFIG_16BIT_PDTIMER_COUNTER is 
    set as 1. With Hardware timer resolution set as 1ms, PSF will be capable of handling timeouts 
    upto 65.535 Seconds.
Remarks :
    None
Example :
    <code>
      #define MCHP_PSF_CONFIG_16BIT_PDTIMER_COUNTER	1 (Sets timeout variable inside the PSF as unsigned 16bit)
      #define MCHP_PSF_CONFIG_16BIT_PDTIMER_COUNTER	0 (Sets timeout variable inside the PSF as unsigned 32bit)
    </code>
**************************************************************************************************/
#define MCHP_PSF_CONFIG_16BIT_PDTIMER_COUNTER     1

// *****************************************************************************
// *****************************************************************************
// Section: SOC INTERRUPT CONFIGURATION
// *****************************************************************************
// *****************************************************************************
/*********************************************************************************
 Function:
    MCHP_PSF_HOOK_DISABLE_GLOBAL_INTERRUPT()
Summary:
    Disables the global interrupt.
Description:
    This hook is used to disable the global interrupts of the chosen SOC. It is usually invoked
    when PSF enters a critical section in the code. This hook is assigned to a function that takes
    no arguments and has a return type of void. The assigned function should be very short to
    ensure interrupt response times are shorter and potential timing issues or race conditions
    can be prevented.
Conditions:
    None.
Return:
    None.
Example:
    <code>
        #define MCHP_PSF_HOOK_DISABLE_GLOBAL_INTERRUPT()   CRITICAL_SECTION_ENTER()
        void CRITICAL_SECTION_ENTER(void);
        void CRITICAL_SECTION_ENTER()
        {
            //Disable SOC's global interrupts
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
    This hook is used to enable the global interrupts of the chosen SOC. It is usually invoked when
    PSF exits from a critical section in the code. This hook is assigned to a function that takes no
    arguments and has a return type of void. The assigned function should be very short to ensure 
    interrupt response times are shorter and potential timing issues or race conditions can be 
    prevented.
Conditions:
    None.
Return:
    None.
Example:
    <code>
        #define MCHP_PSF_HOOK_ENABLE_GLOBAL_INTERRUPT()   CRITICAL_SECTION_EXIT()
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

// *****************************************************************************
// *****************************************************************************
// Section: MEMORY COMPARE AND COPY
// *****************************************************************************
// *****************************************************************************
/************************************************************************
Function:
    MCHP_PSF_HOOK_MEMCMP(pObj1, pObj2, iLength)
Summary:
    Compare two memory regions
Description:
    This hook is used to compare two memory regions pau8Obj1, pau8Obj2 with specified length 
    u8Length. User must define this hook based on the compiler of SOC.
Conditions:
    None.
Input:
    pObj1 -  This is the pointer to block of Memory region 1
    pObj2 -  This is the pointer to block of Memory region 2
    iLength -   This is the number of bytes to be compared.
Return:
    Return 0 if two memory regions are same else return number of bytes did not match.
Example:
    <code>
        #define MCHP_PSF_HOOK_MEMCMP(pObj1, pObj2, iLength) memcmp(pObj1, pObj2, iLength)
        //This hook definition can be compiler defined or user defined.
    </code>
Remarks:
    User definition of this Hook function is mandatory                   
************************************************************************/
#define MCHP_PSF_HOOK_MEMCMP(pObj1, pObj2, iLength) SAMD20_MemCmp (pObj1, pObj2, iLength)                          

/**************************************************************************
Function:
    MCHP_PSF_HOOK_MEMCPY(pDest, pSrc, iLen)
Summary:
    Copies one memory area to another memory area
Description:
    This hook is used to copy iLen bytes from pSrc memory area to pDest memory area. User must 
    define this hook based on the compiler of SOC. The memory areas cannot overlap.
Conditions:
    None.
Input:
    pDest -  This is the pointer to block of destination memory region
    pSrc -   This is the pointer to block of source memory region
    iLen -   This is the number of bytes to be copied.
Return:
    Returns a pointer to pDest.
Example:
    <code>
        #define MCHP_PSF_HOOK_MEMCPY(pDest, pSrc, iLen) memcpy(pDest, pSrc, iLen)
        //This hook definition can be compiler defined or user defined.
    </code>
Remarks:
    User definition of this Hook function is mandatory                     
**************************************************************************/
#define MCHP_PSF_HOOK_MEMCPY(pDest, pSrc, iLen) SAMD20_MemCpy (pDest, pSrc, iLen)

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
    Packed structures are used to save space & align the bytes in data memory based 
    on the compiler. If this pre-processor is defined, then all the PSF's "C" structures will be 
    replaced with this keyword prior to compilation. If this pre-processor is not defined, then 
    structures will be compiled using the default rules of the compiler.
Remarks:
    Need to be packed always based on the type of SOC.	
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
    Packed structures are used to save space & align the bytes in data memory based 
    on the compiler. If this pre-processor is defined, then all the PSF's "C" structures will be 
    replaced with this keyword prior to compilation. If this pre-processor is not defined, then 
    structures will be compiled using the default rules of the compiler.
Remarks:
    Need to be packed always based on the type of SOC.
Example:
    <code>
        #define CONFIG_STRUCT_PACKED_END    _Pragma("pack()")
    </code>
**************************************************************************************************/
#define MCHP_PSF_STRUCT_PACKED_END

/*********************************************************************************************
  Function:
        MCHP_PSF_HOOK_BOOT_TIME_CONFIG(pasCfgStatusData)
  Summary:
    Updates the global and per port Configuration parameters.
  Description:
    This hook is used to update the configuration parameters of Type-C, PD, Power Balancing, 
	Power throttling, PPS, VDM and AltMode contained in gasCfgStatusData structure. 
    This hook must have an argument gasCfgStatusData.
  Conditions:
    None.
  Input:
    pasCfgStatusData -  Holds the structure pointer of the structure
						_GlobalCfgStatusData
  Return:
    None.
  Example:
    <code>
		#define MCHP_PSF_HOOK_BOOT_TIME_CONFIG(pasCfgStatusData) PSF_LoadConfig(pasCfgStatusData)
		void PSF_LoadConfig(pasCfgStatusData)
		{
		// Configure the global parameters
		// Enable Power Throttling and Select Bank B
		pasCfgStatusData->u8PwrThrottleCfg = 0x03;
		// Set 120W as Total system Power of Bank A
		pasCfgStatusData->u16SystemPowerBankAIn250mW = 0x01E0U;
		// Configure per port parameters
		// Set Port 1's VBUS Maximum Fault Count as 3
		pasCfgStatusData->sPerPortData[0].u8VBUSMaxFaultCnt = 0x03; // 0 is the port number
		// Configure per port PB parameters
		// Set Port 2's maximum port power for Bank C as 60W
		pasCfgStatusData.sPBPerPortData[1]->u16MaxPrtPwrBankCIn250mW = 0x00F0U; // 1 is the port number
		}
    </code>
  Remarks:
    User definition of this Hook function is mandatory                                          
  *********************************************************************************************/
#define  MCHP_PSF_HOOK_BOOT_TIME_CONFIG(pasCfgStatusData)       PSF_LoadConfig (pasCfgStatusData)

// *****************************************************************************
// *****************************************************************************
// Section: HOOK CONFIGURATION
// *****************************************************************************
// *****************************************************************************
/**************************************************************************
Function:
    MCHP_PSF_HOOK_DPM_PRE_PROCESS(u8PortNum)
Summary:
    This hook is called inside the DPM_RunStateMachine() API.
Description:
    This hook is called before executing the Type C state machine and policy engine state machine.  
    USER_APPLICATION can define this function if a change is required in default device policy 
    manager or add a user defined state machine. This hook is assigned to a function that takes an
    argument of type UNIT8 and has no return type.
Conditions:
    None.
Input:
    u8PortNum -  Port number of the device. It takes value between 0 to (CONFIG_PD_PORT_COUNT-1).
Return:
    None.
Example:
    <code>
        #define MCHP_PSF_HOOK_DPM_PRE_PROCESS(u8PortNum) HookDevicePolicyManagerPreProcess(u8PortNum)
        void HookDevicePolicyManagerPreProcess(UINT8 u8PortNum);
        void HookDevicePolicyManagerPreProcess(UINT8 u8PortNum)
        {
            //any application related change or enhancement or user defined state machine
        }
    </code>
Remarks:
    User definition of this Hook function is optional                      
**************************************************************************/
#define MCHP_PSF_HOOK_DPM_PRE_PROCESS(u8PortNum)     

#if (TRUE == CONFIG_HOOK_DEBUG_MSG)

// *****************************************************************************
// *****************************************************************************
// Section: DEBUG MESSAGES CONFIGURATION
// *****************************************************************************
// *****************************************************************************
/***********************************************************************
Function:
    MCHP_PSF_HOOK_DEBUG_INIT()            
Summary:
    Initialization of debug module interface
Description:
    This hook is used to initialize the debug interface used by the SOC. It is called during
    initialization of PSF if CONFIG_HOOK_DEBUG_MSG is set to 1. This hook is assigned 
    to a function that takes no arguments and has no return type 
Conditions:
    None.
Return:
    None.
Example:
    <code>
        #define MCHP_PSF_HOOK_DEBUG_INIT()          uart_init()
        void uart_init();
        void uart_init()
        {
            //Initialzes the uart module to send and receive a character
        }
    </code>
Remarks:
    User definition of this Hook function is mandatory when CONFIG_HOOK_DEBUG_MSG is declared as '1'.                  
***********************************************************************/  
#define MCHP_PSF_HOOK_DEBUG_INIT()   SAMD20_UART_Initialisation()

/***********************************************************************
Function:
    MCHP_PSF_HOOK_PRINT_CHAR(byData)            
Summary:
    Outputs a character via UART interface
Description:
    This hook is used to output a character via UART interface to help the user in debugging. 
    This hook is assigned to a function that takes an argument of type char and has no return type.
Conditions:
    MCHP_PSF_HOOK_DEBUG_INIT() should have been called before using this API.
Return:
    None.
Example:
    <code>
        #define MCHP_PSF_HOOK_PRINT_CHAR(byData)          uart_print_char(byData)
        void uart_print_char(char);
        void uart_print_char(char byData)
        {
            //Print a character through UART terminal
        }
    </code>
Remarks:
    This hook API can be used only if CONFIG_HOOK_DEBUG_MSG is 1.                 
***********************************************************************/  

#define MCHP_PSF_HOOK_PRINT_CHAR(byData)    SAMD20_UART_Write_Char (byData);


/***********************************************************************
Function:
    MCHP_PSF_HOOK_PRINT_INTEGER(dwWriteInt, byWidth)            
Summary:
    Outputs an integer via UART interface
Description:
    This hook is used to output an integer via UART interface to help the user in debugging. 
    The size of integer is specified in byWidth. dwWriteInt is of type unsigned long. 
    byWidth is of type unsigned char. This hook is assigned to a function that takes 
    arguments of type unsigned long and unsigned char and has no return type.
Conditions:
    MCHP_PSF_HOOK_DEBUG_INIT() should have been called before using this API.
Return:
    None.
Example:
    <code>
        #define MCHP_PSF_HOOK_PRINT_INTEGER(dwWriteInt, byWidth)          uart_print_char(dwWriteInt, byWidth)
        void uart_print_int(UINT32 dwWriteInt, UINT8 byWidth);
        void uart_print_int(UINT32 dwWriteInt, UINT8 byWidth)
        {
            //Print byWidth no of bytes from dwWriteInt through UART terminal.
        }
    </code>
Remarks:
    This hook API can be used only if CONFIG_HOOK_DEBUG_MSG is 1.                 
***********************************************************************/  
#define MCHP_PSF_HOOK_PRINT_INTEGER(dwWriteInt, byWidth)    SAMD20_UART_Write_Int (dwWriteInt, byWidth);


/***********************************************************************
Function:
    MCHP_PSF_HOOK_PRINT_TRACE(pbyMessage)        
Summary:
    Outputs a string via UART interface
Description:
    This hook is used to output a string via UART interface to help the user in debugging. 
    pbyMessage is of type char *. This hook is assigned to a function that takes 
    an argument of type char * and has no return type.
Conditions:
    MCHP_PSF_HOOK_DEBUG_INIT() should have been called before using this API.
Return:
    None.
Example:
    <code>
        #define MCHP_PSF_HOOK_PRINT_TRACE(pbyMessage)           uart_print_string(pbyMessage)
        void uart_print_string(char *);
        void uart_print_string(char * pbyMessage)
        {
            //Print a string in pbyMessage through UART terminal
        }
    </code>
Remarks:
    This hook API can be used only if CONFIG_HOOK_DEBUG_MSG is 1.                 
***********************************************************************/ 
#define MCHP_PSF_HOOK_PRINT_TRACE(pbyMessage)  SAMD20_UART_Write_String (pbyMessage);

#else
    #define MCHP_PSF_HOOK_DEBUG_INIT() 
    #define MCHP_PSF_HOOK_PRINT_CHAR(byData)
    #define MCHP_PSF_HOOK_PRINT_INTEGER(dwData, byWidth)
    #define MCHP_PSF_HOOK_PRINT_TRACE(pbyMessage)

#endif //CONFIG_HOOK_DEBUG_MSG

// *****************************************************************************
// *****************************************************************************
// Section: Power Delivery Firmware Update
// *****************************************************************************
// *****************************************************************************
/**************************************************************************
Function:
    MCHP_PSF_HOOK_GETCURRENT_IMAGEBANK()
Summary:
    Returns the Index of the Image Bank which is currently executing.
    Example:
        1. 0x01 - Corresponds to Bootloader Application
        2. 0x02 - Corresponds to Fixed Application
        3. 0x03 - Corresponds to Updatable Application
Description:
    This hook is used to get the Index of the image bank which is currently executing in 
    the application. PSF follows "Architecture 2 - Fixed base application with updatable application 
    image". In which the Fixed Application is Image Bank 1 and updatable Application is Image Bank 2.
Conditions:
    This function is invoked by the PD Firmware Update State-machine during the Enumeration Phase 
    (On reception PDFU_GET_FW_ID Command).
Return:
    Returns UINT8 - the index of the Image Bank. It can take following values:
    0x01 - IMAGE_BANK_BOOTLOADER
    0x02 - IMAGE_BANK_FIXED_APP
    0x03 - IMAGE_BANK_UPDATABLE_APP
Example:
    <code>
        #define MCHP_PSF_HOOK_GETCURRENT_IMAGEBANK()  getCurrentImageBank()
        UINT8 getCurrentImageBank(void)
        {
            return u8ImageBankIndex;
        }
    </code>
Remarks:
    The User definition of the function is mandatory in both Fixed and Updatable application when 
    INCLUDE_PDFU is TRUE.                 
**************************************************************************/
#define MCHP_PSF_HOOK_GETCURRENT_IMAGEBANK()    0x0

/****************************************************************************************
Function:
    MCHP_PSF_HOOK_PROGRAM_FWBLOCK(u8pObj1,u16Len)
Summary:
    Validate the Data block and program the data to the Memory, and return the status of the 
    Programming Operation.
Description:
    This hook is invoked during the Transfer Phase on the successful reception event of every 
    PDFU_DATA packet. It is responsible for updating the Firmware data to the memory and identifying
    any errors during the Firmware flash.
Conditions:
    Only during the Policy Engine State-Reconfigure Phase or Transfer phase this function hook will 
    be invoked.
Input:
    u8pObj -  UINT8 Pointer to PDFU_DATA packet payload Buffer. 
                u8pObj[0] - Reserved field Contains PD FW Version. 
                u8pObj[1] - Reserved field Contains Msg Type which is PDFU_DATA 0x83. 
                u8pObj[2] - LSB of Data Block Index. 
                u8pObj[3] - MSB of Data Block
                Index u8pObj[4..260]- Firmware Image data upto 256 bytes where the Data block index 
                                        is used to calculate the Physical memory address to which 
                                        the current data block corresponds to 16 bit parameter.
    u16Len -   Indicates the length of the Firmware data contained in the packet.
Return:
    Returns ePE_PDFU_RESPONSE_CODE Type Return Value - The Status of the Program Operation.
            1. ePE_FWUP_OK -  Upon successful flash operation.
            2. ePE_FWUP_errVERIFY - When verification of the flash operation failed.
            3. ePE_FWUP_errADDRESS - When data block index is out of range.
Example:
    <code>
        #define MCHP_PSF_HOOK_PROGRAM_FWBLOCK(u8pObj, u16Len)\
						PDFW_ProcessPDFUDataRequest(u8pObj, u16Len)
        ePE_PDFU_RESPONSE_CODE PDFW_ProcessPDFUDataRequest( UINT8 u8RequestBuffer, \
												UINT16 u16RequestLength)
        {
            UINT16 u16DataBlockIndex = *((UINT16*)&amp;u8RequestBuffer[2]);
            u32ProgAddr = CalculateAddress(u16DataBlockIndex);
            if( u32ProgAddr < 0xFFFFu )
            {
                ProgramMemoryCB(u32ProgAddr, &u8RequestBuffer[4u],u16RequestLength);
                ReadMemoryCB(u32ProgAddr, &u8ResponseBuffer[0u],u16RequestLength);
                //Compare data written and read
                if (0 == MCHP_PSF_HOOK_MEMCMP(&u8ResponseBuffer[0], \
										&u8RequestBuffer[4], u16RequestLength))
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
    User definition of this Hook function is mandatory when INCLUDE_PDFU is TRUE.                                                          
****************************************************************************************/
#define MCHP_PSF_HOOK_PROGRAM_FWBLOCK(u8pObj,u16Len) 0

/**********************************************************************************************************
  Function:
        MCHP_PSF_HOOK_VALIDATE_FIRMWARE()
  Summary:
    Validates the Flashed Binary using a user defined validation method and returns the status of
	the Firmware Validation Operation.
  Description:
    This hook is invoked during the validation phase on reception of every PDFU Validation Request.
	It is responsible for validating the Firmware data in the memory. It shall return the progress
	status of the Validation on every invocation. If the Status indicates "On going" then the
	Validation command will be responded with the configured Wait time 
	CONFIG_VALIDATION_PHASE_WAITTIME. Validation Method can be any custom method as required by the User.
  Conditions:
    Multiple invocations of the function hook is possible from PDFU Validation phase. Until the
	Validation process is complete, for every request of PDFU_VALIDATION command this function will
	be invoked.The definition of this function shall include 
	1) Starting the Validation process on the First call, 
	2) Returning the Status of the Validation process during subsequent invocation of the function.
  Return:
    Returns the UINT8 Status of the Validation Operation. It take
    following values
 
    0x00u - PE_FWUP_VALIDATION_SUCCESSFUL
    
    0x01u - PE_FWUP_VALIDATION_INPROGRESS
    
    0x02u - PE_FWUP_VALIDATION_FAILURE
  Example:
    <code>
        \#define MCHP_PSF_HOOK_VALIDATE_FIRMWARE()   PDFW_ProcessPDFUDataRequest()
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
  **********************************************************************************************************/
#define MCHP_PSF_HOOK_VALIDATE_FIRMWARE()   0

/**************************************************************************
Function:
    MCHP_PSF_HOOK_BOOT_FIXED_APP()
Summary:
    MCHP_PSF_HOOK_BOOT_FIXED_APP shall perform necessary operation to switch from Updatable
    application to Fixed application.
Description:
    Re-flash of the Updatable_Application image bank while currently executing in the Updatable
    Application image bank, requires switching to the fixed application for performing the upgrade.
    The application switching shall include invalidating the Updatable_Application signatures and/or
    jump/reset for fresh boot from the fixed application.
Conditions:
    This hook is invoked by the PD Firmware Update State-machine during the Reconfiguration phase(On
    reception PDFU_INITIATE Command), when the Updatable application is currently running.
Return:
    No Return Value. During execution of this function the control shall be switched to the Fixed 
    application.
Example:
    <code>
        #define MCHP_PSF_HOOK_BOOT_FIXED_APP()   Boot_Fixed_Application()
        void Boot_Fixed_Application(void)
        {
            EraseUpdatableAppSignature(); //Invalidate the Updatable app sign
            Reset(); //Reset to boot from Fixed app
        }
    </code>
Remarks:
    User definition of this Hook function is mandatory in the Updatable application when INCLUDE_PDFU 
    is TRUE                            
**************************************************************************/
#define MCHP_PSF_HOOK_BOOT_FIXED_APP()

/***********************************************************************************
Function:
    MCHP_PSF_HOOK_BOOT_UPDATABLE_APP()
Summary:
    MCHP_PSF_HOOK_BOOT_UPDATABLE_APP shall perform necessary operation to boot from the updatable 
    application after a PDFU is successfully completed.
Description:
    As the flashing operation is executed from the Fixed application, once the PDFU process is 
    complete it is necessary to switch to the newly upgraded updatable application. This hook 
    definition shall implement necessary operations to safely exit the fixed application and boot 
    from the updatable application. The application switching may include setting the valid
    Updatable_Application signatures (and) jump/reset for fresh boot from Updatable application.
Conditions:
    This function is invoked by the PD Firmware Update State-machine during the Manifestation phase
    (On reception PDFU_INITIATE Command), when the Fixed application is currently running.
Return:
    No Return Value. During execution of this function the control shall be switched to the 
    Updatable application.
Example:
    <code>
        #define MCHP_PSF_HOOK_BOOT_UPDATABLE_APP()   Boot_Updatable_Application()
        void Boot_Updatable_Application(void)
        {
            Reset(); //Reset to boot from Updatable app
        }
    </code>
Remarks:
    User definition of this Hook function is mandatory in the Fixed application when INCLUDE_PDFU is 
    TRUE.                                     
***********************************************************************************/ 
#define MCHP_PSF_HOOK_BOOT_UPDATABLE_APP()         

/**************************************************************************
Function:
    MCHP_PSF_HOOK_IS_PDFU_ALLOWED_NOW()
Summary:
    MCHP_PSF_HOOK_BOOT_UPDATABLE_APP specifies if PD Firmware Update can be currently allowed, 
    based on the priority of the application tasks currently executing. 
Description:
    1. When the PD Firmware Update is allowed, the PDFU Initiator can perform firmware upgrade by 
       the PDFU Sequence
    2. When the PD Firmware Update is not allowed, the PDFU Initiator is responded with the error 
       code during the Reconfiguration phase.
    Example scenario of When PDFU cannot be allowed:
        Assuming a product with firmware update capability through CC and I2C as well. In an example,
        if the firmware upgrade through I2C is already in progress, then PDFU through CC interface 
        shall not be allowed. To handle such situations, this macro shall return the current status 
        of allow-ability of firmware upgrade.
Conditions:
    This function is invoked by the PD Firmware Update State-machine during the Reconfiguration 
    Phase (On reception PDFU_INITIATE Command).   
Return:
    UINT8 value - Shall return the run time status whether PDFU via CC is allowed now or not.
    0x00 - PDFU Not Allowed.
    0x01 - PDFU Allowed.
Example:
    <code>
        #define MCHP_PSF_HOOK_IS_PDFU_ALLOWED_NOW   isPdfuAllowedNow()
        UINT8 isPdfuAllowedNow(void)
        {
            return u8PdfuAllow;
        }
    </code>
 Remarks:
    User definition of this Hook function is mandatory in fixed as well as updatable when 
    INCLUDE_PDFU is TRUE.
**************************************************************************/
#define MCHP_PSF_HOOK_IS_PDFU_ALLOWED_NOW()     0

// *****************************************************************************
// *****************************************************************************
// Section: APIs to be called by the user application
// *****************************************************************************
// *****************************************************************************
/****************************************************************************
Function:
    UINT8 MchpPSF_Init(void)
Summary:
    PSF initialization API 
Description:
    This API should be called by the SOC layer to initialize the PSF stack and UPD350 Hardware.
Conditions:
    API should be called before MchpPSF_RUN().
Input:
    None
Return:
    TRUE - Stack and UPD350 HW successfully initialized.
    FALSE - Stack and UPD350 HW initialization failed.
Remarks:
    For the current PSF implementation, return value is not used. 
    Note: <b> With SAMD20 SOC environment configured for CPU frequency of 48MHZ, this API
    took maximum of 3.488ms and 6.182ms execution time for 1 and 2 port Source only solution 
    respectively. </b>
**************************************************************************************************/
UINT8 MchpPSF_Init (void);

/**************************************************************************************************
Function:
	void MchpPSF_RUN (void)
Summary:
    PSF state machine run API
Description:
    This API is used to run the PSF state machine.For single threaded environment, it should be called 
	repeatedly within a while(1).
Conditions:
    API should be called only after MchpPSF_Init().
Input:
    None
Return:
    None
Remarks:
    In SAMD20 environment where CPU frequency is configured for 48MHZ and
    single threaded environment where MchpPSF_RUN is called in a while(1)
    loop, it took maximum of 74.57us and 0.1439 ms execution time for 1
    port and 2 source port solution respectively. In Multi threaded SAMD20
    configured for 48MHz environment, MchpPSF_RUN can be called for
    every 1ms to 5ms for Successful 2-Port Source only operation.                
  *************************************************************************/
void MchpPSF_RUN (void);

/**************************************************************************************************
Function:
    void MchpPSF_UPDIrqHandler (UINT8 u8PortNum)
Summary:
    UPD350 IRQ Interrupt Handler 
Description:
    This API handles the UPD350 IRQ_N Interrupt, User should call this API when the IRQ line 
    interrupt triggered to the SOC. This API will services and then clear the Alert interrupt for 
    corresponding port. 
Conditions:
    This API should be called inside the GPIO ISR for IRQ interrupt 
Input:
    u8PortNum -  Port number of the device. It takes value between 0 to (CONFIG_PD_PORT_COUNT-1).
Return:
    None
Remarks:
    With SAMD20 environment configured for CPU frequency 48MHZ, this API took maximum of 3.98us and
	5.8us execution time for 1 and 2 port solution respectively.
**************************************************************************************************/
void MchpPSF_UPDIrqHandler (UINT8 u8PortNum);

/**************************************************************************************************
Function:
    void MchpPSF_PDTimerHandler (void)
Summary:
    PD Timer Interrupt Handler 
Description:
    This API is used to handle the PD Timer (Software timer) Interrupt, User should call this 
    API whenever the hardware timer interrupt triggered. 
Conditions:
    This API should be called inside the Hardware timer ISR. 
Input:
    None
Return:
    None
Remarks:
    With SAMD20 environment configured for CPU frequency 48MHZ, this API took maximum of 262us  
	execution time for both 1 and 2 port solution.
**************************************************************************************************/
void MchpPSF_PDTimerHandler (void);

// *****************************************************************************
// *****************************************************************************
// Section:  Notification callback from PSF
// *****************************************************************************
// *****************************************************************************
/**************************************************************************************************
Summary:
	PSF notification enum
Description:
    eMCHP_PSF_NOTIFICATION enum defines the all the notifications PSF can notify via 
	MCHP_PSF_NOTIFY_CALL_BACK.
    
    <b>eMCHP_PSF_TYPEC_DETACH_EVENT:</b> This event is posted by PSF Type C state machine when 
	port partner Detach event is detected.
    
    <b> eMCHP_PSF_TYPEC_CC1_ATTACH: </b>This event is posted by PSF Type C state machine when port
	partner Type C attach is detected in CC1 pin.
    
    <b>eMCHP_PSF_TYPEC_CC2_ATTACH:</b> This event is posted by PSF Type C state machine when port
	partner Type C attach is detected in CC2 pin.
    
    <b>eMCHP_PSF_TYPEC_ERROR_RECOVERY:</b> PSF notifies Type-C Error Recovery condition 
	via this notification. For this notification, PSF expects a return value to decide whether
    to handle Error Recovery. When user application returns TRUE, PSF enters Type-C Error Recovery
    state and resolves the error condition. The user application may also return FALSE, in which case,
    the user application will itself handle Error Recovery condition by raising a Port disable client
    request (BIT(0) of gasCfgStatusData.sPerPortData[u8PortNum].u32ClientRequest variable).

    <b>eMCHP_PSF_UPDS_IN_IDLE: </b>This event is posted by Power management control. PSF runs an
	algorithm in the background for Power management control. The Idle timeout value maintained 
    by PSF is 15 seconds. If there is no activity in UPD350 for 15 seconds, the corresponding 
    UPD350 is put in low power mode. When all the UPD350s present in the system enter low power 
    mode, eMCHP_PSF_UPDS_IN_IDLE is notified. User can then choose to put the SOC into a low power
    state based on this notification. This notification occurs only when 
    INCLUDE_POWER_MANAGEMENT_CTRL is defined as 1.
    
    <b> eMCHP_PSF_VCONN_PWR_FAULT:</b> UPD350 has VCONN comparators to detect VCONN OCS faults. 
	This event is notified when VCONN OCS fault is detected by UPD350. For this notification, PSF
	expects a return value to decide whether to handle the fault occurred. When user returns TRUE
	for VCONN power fault, Incase of explicit contract, if VCONN power fault count is less than
	u8VCONNMaxFaultCnt, PSF DPM power fault manager handles it by sending Hard Reset. If the count 
    exceeds max fault count,VCONN is powered off until physical detach of port partner. Incase of 
    implicit contract, PSF handles by entering TypeC Error Recovery. This notification occurs only 
    when INCLUDE_POWER_FAULT_HANDLING is defined as 1.
    
    <b> eMCHP_PSF_VBUS_PWR_FAULT</b>: PSF notifies all VBUS power fault VBUS Over voltage, VBUS
	under voltage, VBUS OCS via this notification. For this notification, PSF expects a return
	value to decide whether to handle the fault occurred.When user returns TRUE for power fault,
    Incase of explicit contract, if power fault count is less than u8VBUSMaxFaultCnt, 
    PSF DPM power fault manager handles it by sending Hard Reset. When the power fault count 
    exceeds the max fault count, CC termination on the port is removed until the physical detach of
  	the port partner. Incase of implicit contract, PSF handles by entering TypeC Error Recovery. 
    This notification occurs only when INCLUDE_POWER_FAULT_HANDLING is defined as 1.
 
    <b> eMCHP_PSF_RECOVERED_FRM_VCONN_PWR_FAULT </b>: This event is notified when a port is recovered
    from  VCONN power fault if the VCONN is stable for u16PowerGoodTimerInms without 
    any further fault occurrence. PSF clears VCONN fault recorded with this notification.
    
    <b> eMCHP_PSF_RECOVERED_FRM_VBUS_PWR_FAULT </b>: This event is notified when a port
    recovers from VBUS power fault when the VBUS is stable for u16PowerGoodTimerInms without
    any further VBUS fault occurrence. PSF clears the VBUS fault count it recorded with
    this notification.
 
    <b> eMCHP_PSF_PORT_POWERED_OFF</b>: This event is used by PSF to notify application when 
    the port has been powered off as a result of VBUS or VCONN fault count exceeding the 
    values present in u8VBUSMaxFaultCnt or u8VCONNMaxFaultCnt variables respectively within 
    u16PowerGoodTimerInms time period. When a port is powered off, it stops sourcing/sinking 
    power and waits for a detach. The port can be revived only when the partner is detached 
    and attached again. 

    <b> eMCHP_PSF_PD_CONTRACT_NEGOTIATED</b>: PSF notifies when PD contract is
    established with the Port partner.
    
    <b> eMCHP_PSF_HARD_RESET_COMPLETE</b>: This event is posted when a Hard Reset is 
    is sent or received by the port and as a result of which the port has entered into an 
    implicit contract and is going to reestablish the explicit contract with the port partner. 
  
    <b> eMCHP_PSF_PE_SRC_DISABLED</b>: This event is posted when Source policy
    engine enters PE_SRC_DISABLED indicating Power Delivery capable port partner 
    is not present and only Implicit contract is established with Sink port partner.
   
    <b> eMCHP_PSF_SINK_CAPS_RCVD</b>: This event is used by PSF to notify application when 
    Sink capabilities has been received from Port Partner in response to the Get_Sink_Cap
    message initiated by PSF. Application can read the Sink Capabilities by accessing 
    gasCfgStatusData.sPerPortData[u8PortNum].u32aPartnerSinkPDO array. This event is applicable 
    only when the port is configured for Source only or DRP configuration.  
    
    <b> eMCHP_PSF_SINK_CAPS_NOT_RCVD</b>: This event is used by PSF to notify application when
    Sink capabilities has not been received from Port Partner within tSenderResponseTimer
    as a response to the Get_Sink_Cap message initiated by PSF. This event is applicable 
    only when when the port is configured for Source only or DRP configuration. 
    
    <b> eMCHP_PSF_CAPS_MISMATCH</b>: It is notified by PSF when there is a capability
    mismatch with Source partner PDOs in a PD negotiation. This event is applicable 
    only when PSF is operating as Sink or the power role of the port is resolved as 
    Sink during DRP operation. 
    
    <b> eMCHP_PSF_NEW_SRC_CAPS_RCVD</b>: It is notified by PSF when new source capability
    message is received from the Source Partner. This event is applicable 
    only when PSF is operating as Sink or the power role of the port is resolved as 
    Sink during DRP operation. 
  
    <b> eMCHP_PSF_SINK_ALERT_RCVD</b>: This event is used by PSF to notify application when PD
	Alert message has been received from Sink Partner. Application can read the alert
    data by accessing gasCfgStatusData.sPPSPerPortData[u8PortNum].u32PartnerAlert.
    This event is applicable only when PSF is operating as Source or the power role of the port 
    is resolved as Source during DRP operation.   
 
    <b> eMCHP_PSF_SINK_STATUS_RCVD</b>: This event is used by PSF to notify application when 
    Sink Status has been received from Port Partner in response to the Get_Status
    message initiated by PSF. Application can read the Sink Status by accessing 
    gasCfgStatusData.sPPSPerPortData[u8PortNum].u8aPartnerStatus array.
    This event is applicable only when PSF is operating as Source or the power role of the port is
    resolved as Source during DRP operation. 
 
    <b> eMCHP_PSF_SINK_STATUS_NOT_RCVD</b>: This event is used by PSF to notify application when
    Sink Status has not been received from Port Partner within tSenderResponseTimer
    as a response to the Get_Status message initiated by PSF.
    gasCfgStatusData.sPPSPerPortData[u8PortNum].u8aPartnerStatus[6] would have 0 
    when this notification is posted. This event is applicable only when PSF is operating as Source
    or the power role of the port is resolved as Source during DRP operation. 
 
    <b> eMCHP_PSF_VCONN_SWAP_COMPLETE</b>: This notification will be posted when the VCONN Swap
    is completed and the VCONN roles of the both the partners are changed successfully. This 
    notification will also be posted when the VCONN Swap initiated by either of the partners
    is rejected by the other. Users can know the current status of VCONN
    through u32PortConnectStatus.
    
    <b> eMCHP_PSF_VCONN_SWAP_RCVD</b>: This notification will be posted by PSF when a VCONN 
    Swap message is received from port partner. Application can make use of this event to 
    dynamically update the VCONN Swap Policy Configuration through u16SwapPolicy based on 
    which the VCONNN Swap request would be accepted or rejected by PSF. 

    <b> eMCHP_PSF_VCONN_SWAP_NO_RESPONSE_RCVD </b>: This notification will be posted by PSF 
    when a VCONN Swap message has been initiated by PSF and no response has been 
    received from the port partner. 

    <b> eMCHP_PSF_FR_SWAP_COMPLETE</b>: This notification will be posted when the FR Swap
    is completed and the power roles of the both the partners are changed successfully.
    Users can know the current power role status through u32PortConnectStatus. This event 
    is applicable only when PSF is operating as DRP and FR_Swap is supported.
   
    <b> eMCHP_PSF_PR_SWAP_COMPLETE</b>: This notification will be posted when the PR Swap
    is completed and the power roles of the both the partners are changed successfully.
    This notification will also be posted when the PR_Swap initiated by either of the 
    partners is rejected by the other. Users can know the current power role status 
    through u32PortConnectStatus. This event is applicable only when PSF is operating as DRP.
    
    <b> eMCHP_PSF_PR_SWAP_RCVD</b>: This notification will be posted by PSF when a Power 
    Role Swap message is received from port partner. Application can make use of this event to 
    dynamically update the Power Role Swap Policy Configuration through u16SwapPolicy based on 
    which the Power Role Swap request would be accepted or rejected by PSF. 
    This event is applicable only when PSF is operating as DRP.
  
    <b> eMCHP_PSF_PR_SWAP_NO_RESPONSE_RCVD </b>: This notification will be posted by PSF 
    when a Power Role Swap message has been initiated by PSF and no response has been 
    received from the port partner. 
    This event is applicable only when PSF is operating as DRP.
   
    <b>eMCHP_PSF_DR_SWAP_COMPLETE</b>: This notification will be posted when the DR Swap
    is completed and the data roles of the both the partners are changed successfully.
    This notification will also be posted when the DR_Swap initiated by either of the 
    partners is rejected by the other. Users can know the current data role status 
    through u32PortConnectStatus.
        
    <b>eMCHP_PSF_DR_SWAP_RCVD</b>: This occurs whenever there is a DR_SWAP message
    from port partner. User can modify u16SwapPolicy dynamically to Accept or 
    Reject DR_SWAP. Note for dynamic DR_SWAP initiation by PSF, client request 
    has to be raised apart from dynamic change of u16SwapPolicy configuration.
    
    <b> eMCHP_PSF_DR_SWAP_NO_RESPONSE_RCVD </b>: This is posted by PSF when
    Response Timer times out and there is no response from the port partner for 
    the DR_SWAP initiated. 
    
    <b> eMCHP_PSF_CABLE_IDENTITY_DISCOVERED </b>: This event is used by PSF to notify the 
    application when the Cable Identity has been received from Cable in response to the 
    VDM:Discover Identity request initiated by PSF. Application can read the cable identity by 
    accessing the gasCfgStatusData.sPerPortData[u8PortNum].u32aCableIdentity array.
    This event is applicable only when the port is operating as both VBUS Source and
    VCONN Source.
  
    <b> eMCHP_PSF_CABLE_IDENTITY_NAKED </b>: This event is used by PSF to notify the 
    application of the scenarios Good_CRC not received, no response, NAK response and 
    BUSY response from cable for SOP' VDM:Discover Identity request initiated by PSF. 
    This event is applicable only when the port is operating as both VBUS Source and
    VCONN Source. 
   
    <b> eMCHP_PSF_VDM_RESPONSE_RCVD </b>: This notification will be posted by PSF when the partner 
    has sent an ACK/NAK response for the VDM request initiated by PSF within VDM Response time. 
    This notification will not be posted for a BUSY response. Instead, PSF will re-initiate the 
    request after PE_VDM_BUSY_TIMEOUT_MS. This will be done for a maximum Busy count of 5 times.
  
    <b> eMCHP_PSF_VDM_RESPONSE_NOT_RCVD </b>: This notification will be posted by PSF when
	the VDM response timer has timed out due to no response from the partner for the VDM request 
    sent or when the partner has responded BUSY for a maximum of 5 times and the VDM request will 
    not be initiated by PSF anymore or when the partner has sent Not Supported for the VDM request.

    <b> eMCHP_PSF_VDM_REQUEST_RCVD </b>: This notification will be posted by PSF when a VDM request
    received from the partner is evaluated and determined to be ACKed or when the request 
    is a SVID specific command i.e it needs evaluation by the user application. In case of 
    Structured VDM commands like Discover Identity, Discover SVIDs, Discover Modes, Enter Mode, 
    and Exit Mode, PSF expects a return value from the application to send ACK/NAK response.
    If the application returns TRUE, PSF will send ACK response. The application may also return 
    FALSE in which case PSF will send NAK response. If after the evaluation by PSF, it is 
    determined that these commands are to be NAKed, then PSF will send NAK response without 
    notifying the user application. For SVID specific commands, PSF will not 
    send any response since it is expected that the user application will send the response by 
    raising a VDM client request within the VDM response time specified by USB PD Specification. 
 
    <b> eMCHP_PSF_VDM_AMS_COMPLETE </b>: This notification will be posted by PSF when Good CRC 
    is received for the VDM request initiated by PSF that consists of no command response or when 
    Good CRC is received for the VDM response sent in response to the VDM request received
    from partner.  

    <b> eMCHP_PSF_HPD_ENABLED </b>: This notification will be posted by PSF when
	an Hot Plug Detect (HPD) support is enabled.

    <b> eMCHP_PSF_HPD_EVENT_HIGH </b>: This notification will be posted by PSF when
	an HPD_HIGH event has occurred. 

    <b> eMCHP_PSF_HPD_EVENT_LOW </b>: This notification will be posted by PSF when
	an HPD_LOW event has occurred. 

    <b> eMCHP_PSF_HPD_EVENT_IRQ_HPD </b>: This notification will be posted by PSF when
	an IRQ_HPD event has occurred. 

    <b> eMCHP_PSF_HPD_DISABLED </b>: This notification will be posted by PSF when
	an Hot Plug Detect (HPD) support is disabled.
 
    <b> eMCHP_PSF_ALT_MODE_ENTRY_FAILED </b>: This notification will be posted by PSF when
    AltModeEntry timer has timed out because of No 'Enter Mode' command from the port partner
   
    <b> eMCHP_PSF_PORT_DISABLED</b>: This event is used by PSF to notify the application 
    that a port has been disabled as a result of port disable client request (BIT[0] in 
    gasCfgStatusData.sPerPortData[u8PortNum].u32ClientRequest variable). When a port is disabled,
    any connection to the port is prevented by removing all terminations from the CC pins. 
    A disabled port will continue to be in disabled state even after the port partner is
    physically detached and attached. The port can only be revived by a client request to 
    enable the port (BIT[1] in gasCfgStatusData.sPerPortData[u8PortNum].u32ClientRequest variable). 

    <b> eMCHP_PSF_PORT_ENABLED</b>: This event is used by PSF to notify the application 
    that a port has been enabled as a result of port enable client request (BIT[1] in 
    gasCfgStatusData.sPerPortData[u8PortNum].u32ClientRequest variable). When a port is enabled,
    Type-C attach and PD negotiation sequence happens again.
 
Remarks:
    None                                                                                               
  ******************************************************************************************************/
typedef enum MCHP_PSF_NOTIFICATION
{    
eMCHP_PSF_TYPEC_DETACH_EVENT = 1,      // Detach event has occurred
eMCHP_PSF_TYPEC_CC1_ATTACH,            // Port partner attached at CC1 orientation
eMCHP_PSF_TYPEC_CC2_ATTACH,            // Port partner attached at CC2 orientation
eMCHP_PSF_TYPEC_ERROR_RECOVERY,        // Error recovery condition has occurred and it will be handled by PSF if user application returns TRUE
eMCHP_PSF_UPDS_IN_IDLE,                // All the UPD350s are Idle and in low power mode
eMCHP_PSF_VCONN_PWR_FAULT,             // VCONN Power Fault has occurred
eMCHP_PSF_VBUS_PWR_FAULT,              // VBUS Power Fault has occurred
eMCHP_PSF_PORT_POWERED_OFF,            // Port powered off since fault count exceeded maximum fault count
eMCHP_PSF_RECOVERED_FRM_VCONN_PWR_FAULT,       // Port Recovered from VCONN power fault
eMCHP_PSF_RECOVERED_FRM_VBUS_PWR_FAULT,        // Port Recovered from VBUS Power fault
eMCHP_PSF_PE_SRC_DISABLED,              // Only Type C device is present, Partner does not support PD
eMCHP_PSF_PD_CONTRACT_NEGOTIATED,      // PD Contract established with port partner
eMCHP_PSF_HARD_RESET_COMPLETE,         // Hard Reset is sent or received by the port         
eMCHP_PSF_SINK_CAPS_RCVD,              // Sink Caps received from Port Partner
eMCHP_PSF_SINK_CAPS_NOT_RCVD,          // Sink Caps not received from Port Partner
eMCHP_PSF_CAPS_MISMATCH,               // Capability mismatch with Source Port Partner
eMCHP_PSF_NEW_SRC_CAPS_RCVD,           // New source capability message is received from Source Partner
eMCHP_PSF_SINK_ALERT_RCVD,             // Alert message received from Sink Partner         
eMCHP_PSF_SINK_STATUS_RCVD,            // Sink Status received from Sink Partner
eMCHP_PSF_SINK_STATUS_NOT_RCVD,        // Sink Status not received from Sink Partner
eMCHP_PSF_VCONN_SWAP_COMPLETE,         // VCONN Swap completed
eMCHP_PSF_VCONN_SWAP_RCVD,             // VCONN Swap Received from port partner
eMCHP_PSF_VCONN_SWAP_NO_RESPONSE_RCVD, // No response from port partner for VCONN Swap sent
eMCHP_PSF_FR_SWAP_COMPLETE,            // Fast Role Swap completed        
eMCHP_PSF_PR_SWAP_COMPLETE,            // Power Role Swap completed
eMCHP_PSF_PR_SWAP_RCVD,                // Power Role Swap Received from port partner
eMCHP_PSF_PR_SWAP_NO_RESPONSE_RCVD,    // No response from port partner for Power Role Swap sent 
eMCHP_PSF_DR_SWAP_COMPLETE,            // Data Role Swap completed
eMCHP_PSF_DR_SWAP_RCVD,                // Data Role swap received from port partner
eMCHP_PSF_DR_SWAP_NO_RESPONSE_RCVD,    // No response from port partner for the DR_SWAP initiated        
eMCHP_PSF_CABLE_IDENTITY_DISCOVERED,   // ACK received from cable for Discover Identity sent to SOP'        
eMCHP_PSF_CABLE_IDENTITY_NAKED,        // NAK received from cable for Discover Identity sent to SOP'
eMCHP_PSF_VDM_RESPONSE_RCVD,           // Response received from partner for VDM request sent to SOP
eMCHP_PSF_VDM_RESPONSE_NOT_RCVD,       // No response from partner for VDM request sent to SOP  
eMCHP_PSF_VDM_REQUEST_RCVD,            // VDM Request received from partner        
eMCHP_PSF_VDM_AMS_COMPLETE,            // VDM AMS Completed 
eMCHP_PSF_HPD_ENABLED,                 // Indicates that HPD module is enabled
eMCHP_PSF_HPD_EVENT_HIGH,              //Indicates that HPD_HIGH event has occurred
eMCHP_PSF_HPD_EVENT_LOW,               //Indicates that HPD_LOW event has occurred
eMCHP_PSF_HPD_EVENT_IRQ_HPD,           //Indicates that IRQ_HPD event has occurred
eMCHP_PSF_HPD_DISABLED,                // Indicates that HPD module is disabled        
eMCHP_PSF_ALT_MODE_ENTRY_FAILED,       // Alt Mode Entry Failed        
eMCHP_PSF_PORT_DISABLED,               // Indicates that port is disabled successfully        
eMCHP_PSF_PORT_ENABLED                // Indicates that the port is enabled successfully
} eMCHP_PSF_NOTIFICATION;

/****************************************************************************************************
Function:
    MCHP_PSF_NOTIFY_CALL_BACK(u8PortNum, ePSFNotification)   
Summary:
    Notifies the USER_APPLICATION about various PD events from PSF.
Description:
    This hook is used by the various modules of PSF to notify the USER_APPLICATION about different 
    PD events such as Type-C Attach and Detach, Type-C Orientation. USER_APPLICATION can define 
    this hook function if it wants external handling of the PD events. This hook is assigned to a 
    function that takes an argument of type UINT8 and has a return type of UINT8.
Conditions:
    None.
Input:
    u8PortNum -  Port number of the device. It takes value between 0 to (CONFIG_PD_PORT_COUNT-1).
    ePSFNotification - Type of Notification occurred inside the stack. This argument can take 
                       one of the values from enum eMCHP_PSF_NOTIFICATION.                   
Return:
    UINT8 - Except for eMCHP_PSF_VCONN_PWR_FAULT, eMCHP_PSF_VBUS_PWR_FAULT, 
            eMCHP_PSF_TYPEC_ERROR_RECOVERY and eMCHP_PSF_VDM_REQUEST_RCVD the return value is 
            ignored by PSF. For these events, user can return TRUE or FALSE depending on whether
            the relevant action is required to be taken by PSF or not.
Example:
    <code>
        #define MCHP_PSF_NOTIFY_CALL_BACK(u8PortNum, ePSFNotification)\
                      HookNotifyPDEvents(u8PortNum, ePSFNotification)
        void HookNotifyPDEvents(UINT8 u8PortNum, eMCHP_PSF_NOTIFICATION ePSFNotification);
        void HookNotifyPDEvents(UINT8 u8PortNum, eMCHP_PSF_NOTIFICATION ePSFNotification)
        {
            // Return for Power fault notification
            // Implement user specific application as required
        }

    </code>
Remarks:
    User definition of this Hook function is mandatory                                                
****************************************************************************************************/
#define MCHP_PSF_NOTIFY_CALL_BACK(u8PortNum, ePSFNotification)   App_HandlePSFEvents (u8PortNum, ePSFNotification)
 
/**************************************************************************************************************
  Section:
         GPIO Control
    
    
    
    * *************************************************************************** *
    * *************************************************************************** *
    * *********************************************************************************************** *
  Summary:
    GPIO Functionality enum.
  Description:
    eMCHP_PSF_GPIO_FUNCTIONALITY enum contains all the GPIO pin roles or functions that are used by
    PSF on a per port basis. Depending on the PD mode supported, each of these pin roles are
    assigned a unique GPIO from the MCU or the UPD350.
    <table>
    Functionality             \Input/Output   \Description
    ------------------------  --------------  -----------------------------------------------------------------
    eUPD350_ALERT_FUNC        \Input          * PSF needs an SOC GPIO to be assigned to this role
                                                 for detecting the status of the IRQ_N line of
                                                 UPD350 associated with each port
                                               * IRQ_N is an active low signal. This GPIO functionality
                                                 shall initialize the SOC GPIOs connected to the IRQ_N
                                                 lines of UPD350s in the system for interrupt notification.
                                                 It is recommended to configure SOC GPIOs interrupt in edge
                                                 level detection with internal pull up since the UPD350
                                                 keeps the IRQ_N line in low state until the interrupt is
                                                 cleared.
                                               * To notify PSF about the occurrence of UPD350 
                                                 interrupt, the API MchpPSF_UPDIrqHandler shall be 
                                                 called by SOC on interrupt detection of the 
                                                 specific port. 
                                               * GPIO connected to IRQ_N should be wakeup capable if
                                                 INCLUDE_POWER_MANAGEMENT_CTRL defined as 1.This is a
                                                 mandatory functionality and configured only during
                                                 initialization.
    eI2C_DC_DC_ALERT_FUNC     \Input          * Configures the GPIO of the SOC for DC DC Alert
                                                 functionality. This is not a mandatory functionality and
                                                 can be configured based on the DC DC controller used.
    eUPD350_RESET_FUNC        \Input          * This GPIO functionality is to control SOC GPIOs
                                                 connected to the RESET_N lines of Port's UPD350. It is
                                                 mandatory to connect a single GPIO to the reset 
                                                 line of all UPD350s. The UPD350 RESET_N is an 
                                                 active low signal. 
                                               * The GPIO control for reset shall be handled as below
											      * Initialization - Drive the GPIO high
												  * eGPIO_ASSERT - Drive the GPIO low and provide a sufficient 
                                                     delay for the UPD350 to reset
                                                  * eGPIO_DEASSERT - Drive the GPIO high												  
                                                * As single line is connected to all the UPD350 reset pin, the
                                                   port number passed as argument to the hooks 
												   MCHP_PSF_HOOK_GPIO_FUNC_INIT	and MCHP_PSF_HOOK_GPIO_FUNC_DRIVE
                                                   shall be 0.
                                               * This is a mandatory functionality to reset UPD350 and
                                                 done only during initialization.
    eSPI_CHIP_SELECT_FUNC     \Output         * The SOC GPIO assigned this pin role is used to
                                                 enable/disable the SPI communication with the 
                                                 UPD350 of a given port. It is applicable only when
                                                 CONFIG_DEFINE_UPD350_HW_INTF_SEL is defined as
                                                 CONFIG_UPD350_SPI. The eSPI_CHIP_SELECT_FUNC is 
                                                 active low signal.
                                               * The GPIO control for CS shall be handled as below
											      * Initialization - Drive the GPIO high
												  * eGPIO_ASSERT - Drive the GPIO low for the specific port to enable 
												     communication
                                                  * eGPIO_DEASSERT - Drive the GPIO high for the specific port to disable 
												     communication											  
                                               * It is mandatory to have a unique MCU pin for every
                                                 port in the design, which is assigned this role.
    eVBUS_DIS_FUNC            \Output         * VBUS Discharge mechanism is required to enable quick
                                                 discharge of VBUS when VBUS transitions from higher to
                                                 lower voltage.PSF requests the application layer to assert
                                                 this pin whenever it requires a quick discharge of VBUS.
                                                 PSF requests for de-assertion once the quick discharge
                                                 is complete. The state of GPIO during initialization, 
                                                 assertion and de-assertion
												 of this functionality is specific discharge circuitry used.
                                               * It is mandatory to have a unique pin for every
                                                 port in the design, which is assigned this role.  
    eDC_DC_EN_FUNC            \Output         * DC_DC_EN functionality is required to enable DC DC
                                                 Controller. PSF requests the application layer to assert
                                                 when PSF is initialized, ready to operate and CC pins are
                                                 functional. It will be toggled during error condition say,
                                                 on occurrence of fault to reset the DC DC controller. 
                                                 The state of GPIO during initialization, assertion 
												 and de-assertion of this functionality 
												 is specific to the DC DC Controller used.
                                               * This is applicable only for Source functionality
    eORIENTATION_FUNC         \Output         * Orientation functionality is used to indicate the
                                                 orientation of the type-C cable connector. It can be
                                                 used to control an external USB data multiplexer.
                                                 PSF requests the application layer to initialize
                                                 this functionality during device attach, assert
												 assert during CC1 attach and de-assert during
												 CC2 attach. The state of GPIO during 
                                                 initialization, assertion and de-assertion
												 of this functionality is user specific. 
                                               * This is not mandatory, depends on user application.
    eSNK_CAPS_MISMATCH_FUNC   \Output         * Sink Caps mismatch functionality is used to indicate any
                                                 mismatch of capability during a PD negotiation. It is
                                                 applicable only in Sink mode operation. PSF requests 
                                                 the application to assert when PD Sink negotiation is
                                                 complete and if there was a capability mismatch with the
                                                 selection. PSF requests the application to de-assert
                                                 during port partner detach or during a new 
												 negotiation. The state of GPIO during initialization,
												 assertion and de-assertion of this functionality
                                                 is user specific.
                                               * This is not mandatory, depends on the user application.
    eSNK_1_5A_IND_FUNC        \Output         * This functionality indicates if the current
                                                 capability of the sink is more than 1.5A. PSF
                                                 requests the application to assert when the current
                                                 capability or negotiated current is 1.5A or more and 
												 less than 3A. PSF requests the application to 
                                                 de-assert on detach event or during renegotiation. 
												 The state of GPIO during initialization,
												 assertion and de-assertion of this functionality
                                                 is user specific.
												* This is applicable only in sink mode of operation
                                                   and it is not mandatory, depends on user application.
    eSNK_3A_IND_FUNC          \Output         * 3A indicator functionality indicates if the current
                                                 capability of the sink is more than 3A. PSF 
                                                 requests the application to assert when current
												 capability or negotiated current is 3A or more. 
                                                 PSF requests the application to de-assert
												 on detach event or during renegotiation. The 
												 state of GPIO during initialization, assertion
                                                 and de-assertion of this functionality 
												 is user specific.
                                               * This is applicable only for sink mode operation
                                                 and it is not mandatory, depends on the 
                                                 user application.
    ePOWER_ROLE_FUNC          \Output         * Power role indicator functionality is used to 
                                                 indicate the current power role of a port.
                                                 PSF requests the application to assert this pin
                                                 when the current power role is source. PSF 
                                                 requests the application to de-assert this pin
                                                 when the current power role is sink or when the 
                                                 port partner is detached. The state of GPIO during
                                                 initialization, assertion and de-assertion of this
                                                 functionality is user specific.
                                               * This is applicable only for DRP mode operation and 
                                                 it is not mandatory, depends on user application.
    eDATA_ROLE_FUNC           \Output         * Data role indicator functionality is to indicate
                                                 that the current data role is Host/Hub DFP.
                                                 PSF requests the application to assert the pin
												 when current data role is DFP. PSF requests the 
                                                 application to de-assert the pin when the current
												 data role is UFP or when the port partner is 
                                                 detached. The state of GPIO during initialization,
                                                 assertion and de-assertion of this functionality
                                                 is user specific.
                                               * This is applicable only for DRP mode operation and
                                                 it is not mandatory, depends on user application.
    </table>
  Remarks:
    None                                                                                                       
  **************************************************************************************************************/
typedef enum eMCHP_PSF_GPIO_Functionality
{
    eUPD350_ALERT_FUNC,            
    eI2C_DC_DC_ALERT_FUNC,         
    eUPD350_RESET_FUNC,             
    eSPI_CHIP_SELECT_FUNC,         
    eVBUS_DIS_FUNC,                
    eDC_DC_EN_FUNC,                
    eORIENTATION_FUNC,             
    eSNK_CAPS_MISMATCH_FUNC,       
    eSNK_1_5A_IND_FUNC,            
    eSNK_3A_IND_FUNC,
    ePOWER_ROLE_FUNC,
    eDATA_ROLE_FUNC
} eMCHP_PSF_GPIO_FUNCTIONALITY;

/**************************************************************************************************
Summary:
    GPIO  Drive enum.
Description:
	eMCHP_PSF_GPIO_DRIVE_VAL enum defines Assert and Deassert drives of the  
    various GPIO functionality Pins that are used in PSF.
Remarks:
        None
**************************************************************************************************/
typedef enum eMCHP_PSF_GPIO_DriveValue 
{
    eGPIO_DEASSERT,               //Drive to De-Assert the GPIO
    eGPIO_ASSERT                  //Drive to Assert the GPIO
} eMCHP_PSF_GPIO_DRIVE_VAL;


/**************************************************************************
Function:
    MCHP_PSF_HOOK_GPIO_FUNC_INIT(u8PortNum, eGPIOFunc)
Summary:
    Hook to initialize all the GPIO functionality pins in application layer.
Description:
    PSF calls this API to initialize the eMCHP_PSF_GPIO_FUNCTIONALITY pins in the
    application layer. Users has to define an appropriate function with UINT8 and 
    eMCHP_PSF_GPIO_FUNCTIONALITY as argument. Users can assign any PIO either from 
    UDP350 or MCU for any GPIO functionality defined. Drive of this API will be 
    controlled by the hook MCHP_PSF_HOOK_GPIO_FUNC_DRIVE.
Conditions:
    None.
Input:
    u8PortNum - Port number of the device. It takes value between 0 to 
                (CONFIG_PD_PORT_COUNT-1).
    eGPIOFunc - Passes the GPIO functionality type that has to be initialized by the application. 
Return:
    None.
Example:
    <code>
        #define MCHP_PSF_HOOK_GPIO_FUNC_INIT(u8PortNum, eGPIO_Func) 
        App_GPIOContol_Initialisation(u8PortNum, eGPIO_Func)
        void App_GPIOControl_Initialisation(UINT8 u8PortNum, eMCHP_PSF_GPIO_FUNCTIONALITY eGPIOFunc)
        {
            switch(eGPIOFunc)
            {
                case eDC_DC_EN_FUNC:
                {
                    //Initialise the GPIO assigned for DC_DC
                    // Drive the GPIO in default state
                    break;
                }
                case eVBUS_DIS_FUNC:
                {
                    //Initialise the GPIO assigned for VBUS_Discharge functionality
                    //Drive the GPIO in default state
                }
            }

        }

    </code>
Remarks:
    User definition of this Hook function is mandatory as well as it is mandatory to define functionality for ePSF_GPIO_Functionality.
 **************************************************************************/
#define MCHP_PSF_HOOK_GPIO_FUNC_INIT(u8PortNum, eGPIOFunc) App_GPIOControl_Init (u8PortNum, eGPIOFunc)

/**************************************************************************
Function:
    MCHP_PSF_HOOK_GPIO_FUNC_DRIVE(u8PortNum, eGPIOFunc, eDriveVal)
Summary:
    Hook to drive GPIOs assigned to GPIO functionality pins in application layer.
Description:
    PSF calls this API to drive the ePSF_GPIO_Functionality pins in application 
    layer as per  drive value ePSF_GPIO_DriveVal. Users has to define an 
    appropriate function with UINT8, eMCHP_PSF_GPIO_FUNCTIONALITY, eMCHP_PSF_GPIO_DRIVE_VAL 
    as argument. Users can assign any PIO either from UDP350 or MCU for any GPIO 
    functionality defined.
Conditions:
    None.
Input:
    u8PortNum - Port number of the device. It takes value between 0 to (CONFIG_PD_PORT_COUNT-1).
    eGPIOFunc - Passes the GPIO functionality type that has to be initialized by 
                the application. 
    eDriveVal - Drive value for the pin
Return:
    None.
Example:
    <code>
        #define MCHP_PSF_HOOK_GPIO_FUNC_DRIVE (u8PortNum, eGPIO_Func, eDriveVal) 
        App_GPIOContol_Drive(u8PortNum, eGPIO_Func, eDriveVal)
        void App_GPIOContol_Drive(UINT8 u8PortNum, 
                       eMCHP_PSF_GPIO_FUNCTIONALITY eGPIOFunc, eMCHP_PSF_GPIO_DRIVE_VAL eDriveVal )
        {
            switch(eGPIOFunc)
            {
                case eDC_DC_EN_FUNC:
                {
                    if (eGPIO_Assert == eDriveVal)
                    {
                        // Assert the DC_DC pin	
                    }
                    else
                    {
                        // De-assert the DC_DC pin
                    }
                    break;
                }
                case eVBUS_DIS_FUNC:
                {
                    if (eGPIO_Assert == eDriveVal)
                    {
                        // Assert the VBUS Discharge pin	
                    }
                    else
                    {
                        // De-assert the VBUS Discharge pin
                    }
                    break;
                }
            }
        }
    </code>
Remarks:
    User definition of this Hook function is mandatory as well as it is mandatory 
    to define functionality for eMCHP_PSF_GPIO_FUNCTIONALITY.
 *************************************************************************/
#define MCHP_PSF_HOOK_GPIO_FUNC_DRIVE(u8PortNum, eGPIOFunc, eDriveVal) \
App_GPIOControl_Drive (u8PortNum, eGPIOFunc, eDriveVal)

// *****************************************************************************
// *****************************************************************************
// Section: Port power control
// *****************************************************************************
// *****************************************************************************   
/*****************************************************************************
Function:
    MCHP_PSF_HOOK_HW_PORTPWR_INIT(u8PortNum)
Summary:
    Initializes all the hardware modules related to port power functionality especially DC-DC buck 
    boost controller and load switch. Additionally, in case of sink functionality, this hook may be 
    defined with APIs to initialize a DAC.
Description:
    This hook is used to initialize the hardware modules related to port power functionality. 
    Implementation of this function depends on the type of DC-DC buck boost controller, load
    switch or DAC used. This hook is assigned to a function that takes an argument of type UINT8
    and has a return type of UINT8.
Conditions:
    API implementation must make sure that the Port Power(VBUS) of all ports is set to 0V.
Input:
    u8PortNum -  Port number of the device. It takes value between 0 to (CONFIG_PD_PORT_COUNT-1).
Return:    
    UINT8 - Return TRUE if initialization was successful or else return FALSE. 
Example:
    <code>
        #define MCHP_PSF_HOOK_HW_PORTPWR_INIT(u8PortNum)       hw_portpower_init(u8PortNum)
        void hw_portpower_init(void);
        void hw_portpower_init(void)
        {
            //Initializes the hardware modules related to port power functionality
        }
    </code>
Remarks:
    User definition of this Hook function is mandatory.
    A DAC may initialized under this hook if PSF is configured as SINK.                        
*****************************************************************************/

#define MCHP_PSF_HOOK_HW_PORTPWR_INIT(u8PortNum)  App_PortPowerInit (u8PortNum)
                                                   
/****************************************************************************
Function:
    MCHP_PSF_HOOK_PORTPWR_DRIVE_VBUS(u8PortNum, u16VBUSVoltage, u16Current)
Summary:
    Drives the VBUS line with the negotiated voltage and current
Description:
    If the user chose to implement their own DC-DC buck boost control, this hook must be implemented 
    to drive VBUS as per the arguments passed with it. Implementation of this hook depends on the 
    type of DC-DC buck boost controller and load switch used. This hook is assigned to a function
    that takes arguments of type UINT8, UINT16, UINT16 and no return type.
Conditions:
    It is applicable only for Source operation.
Input:
    u8PortNum -  Port number of the device. It takes value between 0 to (CONFIG_PD_PORT_COUNT-1).         
    u16VBUSVoltage - VBUS Voltage level to be driven in VBUS expressed in terms of milliVolts.
    u16Current     - VBUS current level in terms of mA.
Return:
    None.
Example:
    <code>
        #define MCHP_PSF_HOOK_PORTPWR_DRIVE_VBUS(u8PortNum, u16VBUSVoltage, u16Current)\
                hw_portpower_driveVBUS(u8PortNum, u16VBUSVoltage, u16Current)
      void hw_portpower_driveVBUS(UINT8 u8PortNum, UINT16 u16VBUSVoltage, UINT16 u16Current);
      void hw_portpower_driveVBUS(UINT8 u8PortNum, UINT16 u16VBUSVoltage, UINT16 u16Current)
      {
            // Configure DC-DC buck boost control to drive u16VBUSVoltage & u16Current in VBUS
      }
    </code>
Remarks:
    User definition of this Hook function is mandatory.                      
****************************************************************************/

#define MCHP_PSF_HOOK_PORTPWR_DRIVE_VBUS(u8PortNum,u16VBUSVoltage,u16Current)   \
        App_PortPowerSetPower (u8PortNum, u16VBUSVoltage, u16Current)

/*******************************************************************************************
Function:
    MCHP_PSF_HOOK_PORTPWR_CONFIG_SINK_HW(u8PortNum,u16Voltage,u16Current)
Summary:
    Enables or disables sink hardware circuitry and configures it to sink the VBUS voltage for 
    a given port based on the sink requested voltage and current.
Description:
    This hook is used to enable or disable sink hardware circuitry and configure it for Sink  
    requested current and voltage. Implementation of this function depends on the type of Sink 
    circuitry used. This hook is assigned to a function that takes arguments of type 
    UINT8,UINT16,UINT16 without return type.
Conditions:
    It is applicable only for Sink operation.
Input:
    u8PortNum -  Port number of the device. It takes value between 0 to (CONFIG_PD_PORT_COUNT-1).
    u16voltage -  Enable Sink HW Circuitry if the u16voltage is not vSafe0V to drain power.
                    Disable sink HW circuitry if the u16voltage is vSafe0V.
                    Configure the HW to requested u16voltage in mV.
    u16Current -  Configure the HW for the requested current passed in terms of mA.
Return:
    None.
Example:
    <code>
        #define MCHP_PSF_HOOK_PORTPWR_CONFIG_SINK_HW(u8PortNum, u16Voltage, u16Current)\
            hw_Configure_SinkCircuitary(u8PortNum, u16Voltage, u16Current)
        void hw_Configure_SinkCircuitary(UINT8 u8PortNum,UINT16 u16Votlage,UINT16 u16Current);
        void hw_Configure_SinkCircuitary(UINT8 u8PortNum,UINT16 u16Votlage,UINT16 u16Current)
        {
            if(u16Voltage == Vsafe0V)
            {
                //Disable the Sink circuitary for "u8PortNum" Port
            }
            else
            {
                //Enable the Sink circuitary for "u8PortNum" Port and 
                    configure it to drain u16Voltage 
            }
            //Conifgure Sink circuitary for u16Current current rating
        }
    </code>

Remarks:
    User definition of this Hook function is mandatory if PSF is configured for Sink functionality.
*******************************************************************************************/
#define MCHP_PSF_HOOK_PORTPWR_CONFIG_SINK_HW(u8PortNum,u16Voltage,u16Current)

// *****************************************************************************
// *****************************************************************************
// Section:  DAC 
// *****************************************************************************
// *****************************************************************************
/*******************************************************************************
Function:
    MCHP_PSF_HOOK_DRIVE_DAC_I(UINT8 u8PortNum, UINT16 u16DACData)
Summary:
    Indicates the implicit/explicit current capability of attached source partner.
Description:
    This hook is used by PSF to inform the sink about the implicit/explicit current
    capability of the attached source partner. The current capability is 
    indicated through a voltage level on Digital to Analog Converter(DAC)'s 
    output pin. The voltage level on DAC's output pin is calculated based on 
    per port Configuration parameters, which were configured using 
    MCHP_PSF_HOOK_BOOT_TIME_CONFIG(pasCfgStatusData) hook.
  
    In gasCfgStatusData structure, if u16DAC_I_CurrentInd_MaxInA is 5000mA, 
    u16DAC_I_MaxOutVoltInmV is 2500mV, u16DAC_I_MinOutVoltInmV is 0V and direction 
    mentioned in u8DAC_I_Direction is High Amperage - Max Voltage, then 
        1. 0.5A > DAC_I = 0.25V 
        2. 1.5A > DAC_I = 0.75V
        3. 2.0A > DAC_I = 1V
        4. 3.0A > DAC_I = 1.5V 
        5. 4.0A > DAC_I = 2.0V
        6. 5.0A > DAC_I = 2.5V
    In gasCfgStatusData structure, if u16DAC_I_CurrentInd_MaxInA is 3000mA, 
    u16DAC_I_MaxOutVoltInmV is 2500mV, u16DAC_I_MinOutVoltInmV is 0V and direction 
    mentioned in u8DAC_I_Direction is High Amperage - Max Voltage, then 																	  * If it is 3A and maximum 
        1. 0.5A > DAC_I = 0.42V 
        2. 1.5A > DAC_I = 1.25V
        3. 2.0A > DAC_I = 1.67V
        4. 3.0A > DAC_I = 2.5V
        5. 4.0A > DAC_I = 2.5V
        6. 5.0A > DAC_I = 2.5V    

    A suitable function that initializes DAC from SoC may be 
    implemented in this hook. 
Conditions:
    SoC should support a DAC and the DAC should be initialized under 
    MCHP_PSF_HOOK_HW_PORTPWR_INIT() hook.
Input:
    u8PortNum   - Port number for which DAC_I needs to be driven
    u16DACData  - Analog voltage to be driven on DAC_I pin
Return:
    None.
Example:
    <code>
        #define MCHP_PSF_HOOK_DRIVE_DAC_I(u8PortNum, u16DACData)   HW_Drive_DAC_I(u8PortNum, u16DACData)
        void HW_Drive_DAC_I(UINT8 u8PortNum, UINT16 u16DACData);
        void HW_Drive_DAC_I(UINT8 u8PortNum, UINT16 u16DACData)
        {
            //Implement user specific application to output volatge provided under 
            //u16DACData argument in DAC's output pin
        }
    </code>
Remarks:
    This hook is applicable only if INCLUDE_PD_SINK macro is 1. Definition of this
    hook is not mandatory. This is applicable only for Sink operation.
*******************************************************************************/ 
#define MCHP_PSF_HOOK_DRIVE_DAC_I(u8PortNum, u16DACData)  App_DriveDAC_I (u8PortNum, u16DACData)

/*******************************************************************************
Function:
    MCHP_PSF_HOOK_GET_OUTPUT_VOLTAGE_IN_mV(u8PortNum)
Summary:
    Gets the current output voltage driven in the VBUS.
Description:
    This hook is used when PSF needs to know about the present voltage driven
    by external DC_DC controller. The function should be defined with return type
    UINT32 and UINT8 type as input parameter. If the DC_DC controller does not 
    have the capability to measure output voltage, return 0xFFFFFFFF to denote the feature
    is not supported. 
Conditions:
    Output voltage shall be returned in terms of milliVolts.
Return:
    UINT32 - Output voltage in terms of mV if the DC_DC controller have the capability to 
    measure output voltage. Else, return 0xFFFFFFFF.
Example:
    <code>
        #define MCHP_PSF_HOOK_GET_OUTPUT_VOLTAGE_IN_mV(u8PortNum)   DCDC_GetOutVoltage(u8PortNum)
        UINT32 DCDC_GetOutVoltage(UINT8 u8PortNum)
        {
            // return Output voltage driven by the external DC_DC controller
            // in terms of milliVolts
        }
    </code>
Remarks:
    User definition of this Hook function is mandatory when INCLUDE_PD_SOURCE_PPS is
    defined as '1'.                          
*******************************************************************************/  
#define MCHP_PSF_HOOK_GET_OUTPUT_VOLTAGE_IN_mV        0xFFFFFFFF

/*************************************************************************************************
  Function:
          MCHP_PSF_HOOK_GET_OUTPUT_CURRENT_IN_mA(u8PortNum)
  Summary:
    Gets the output current.
  Description:
    This hook is used when PSF needs to know about the current drawn from
    external DC_DC controller. The function should be defined with return
    type UINT32 and UINT8 type as input parameter. If the DC_DC controller
    does not have the capability to measure output current, return 0xFFFFFFFF to denote
    the feature is not supported.
  Conditions:
    Output Current shall be returned in terms of mA.
  Return:
    UINT32 - Output current in terms of mA if the DC_DC controller have the capability to 
    measure output current. Else, return 0xFFFFFFFF.
  Example:
    <code>
        \#define MCHP_PSF_HOOK_GET_OUTPUT_CURRENT_IN_mV(u8PortNum)   DCDC_GetOutCurrent(u8PortNum)
        UINT32 DCDC_GetOutCurrent(UINT8 u8PortNum)
        {
            // return Output current driven by the external DC_DC controller
            // in terms of mA.
        }
    </code>
  Remarks:
    User definition of this Hook function is mandatory when
    INCLUDE_PD_SOURCE_PPS is defined as '1'.                                                      
  *************************************************************************************************/  
#define MCHP_PSF_HOOK_GET_OUTPUT_CURRENT_IN_mA        0xFFFFFFFF

/**************************************************************************************************
Summary:
    PSF Notify Idle enum.
Description:
	eMCHP_PSF_NOTIFY_IDLE enum notifies the Idle state in PSF  
Remarks:
        None
**************************************************************************************************/
typedef enum ePSF_NOTIFY_IDLE 
{
    eIDLE_PE_NOTIFY,               //Notify Policy Engine Idle State
    eIDLE_TYPEC_NOTIFY             //Notify Type C Idle State
} eMCHP_PSF_NOTIFY_IDLE;

/**************************************************************************
Function:
    MCHP_PSF_HOOK_NOTIFY_IDLE(u8PortNum, eIDLESubState)
Summary:
    Hook to notify entry of Policy Engine and Type C State Machine into idle state
Description:
    PSF calls this API to notify the entry of Policy Engine and Type C State Machine
    into idle state. The entry into idle state refers to when the state machine
    waits for an event from the partner or for the activated timer to get expired.
    The entry into Idle state of Policy Engine or Type C state machine is 
    differentiated based on the enum argument passed
Conditions:
    None.
Input:
    u8PortNum - Port number of the device. It takes value between 0 to (CONFIG_PD_PORT_COUNT-1).
    eIDLESubState - Defines the idle notification of Policy Engine(eIDLE_PE_NOTIFY) or   
                   Type C State machine(eIDLE_TYPEC_NOTIFY) 
Return:
    None.
Example:
    <code>
        #define MCHP_PSF_HOOK_NOTIFY_IDLE(u8PortNum, eIDLESubState) 
        PSF_IDLENotification(u8PortNum, eIDLESubState)
        void  PSF_IDLENotification(u8PortNum, eIDLESubState)
        {
            if(eIDLESubState == eIDLE_PE_NOTIFY)
        {
            gu8PEIDLEFlag[u8PortNum] = TRUE;
        }
        else if (eIDLESubState == eIDLE_TYPEC_NOTIFY)
        {
            gu8TypeCIDLEFlag[u8PortNum] = TRUE;
        }
        else
        {
            //Do Nothing 
        }
    </code>
Remarks:
    User definition of this Hook function is not mandatory and would be useful
    in an RTOS environment                          
 *************************************************************************/
#define MCHP_PSF_HOOK_NOTIFY_IDLE(u8PortNum, eIDLESubState)

/*******************************************************************************
Function:
    MCHP_PSF_HOOK_PDTIMER_EVENT()
Summary:
    Hook for PD timer timeout event
Description:
    This hook is used when PD timer expires for the given event to call 
    the callback function. 
Conditions:
    None
Input:
    None 
Return:
    None.
Example:
    <code>
        #define MCHP_PSF_HOOK_PDTIMER_EVENT ()  PSF_IdleExit()
    void PSF_IdleExit()
    {
        gbyIdleFlag = FALSE;
    }
    </code>
Remarks:
    User definition of this Hook function is not mandatory and would be useful
    in an RTOS environment                          
*******************************************************************************/  

#define MCHP_PSF_HOOK_PDTIMER_EVENT()

#endif /*_PSF_API_HOOK_H_*/

