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

Copyright �  [2019] Microchip Technology Inc. and its subsidiaries.

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
    PSF needs a Hardware interface from SOC either SPI or I2C to communicate with UPD350. UPD350 
    supports either I2C or SPI interface depending on UPD350 part used.  UPD350 A and C supports I2C
    interface and UPD350 B and D part supports SPI interface. 
    This Hook is to initialize the SOC's Hardware interface for communication. It is called 
    during initialization of PSF. Define relevant function that has no arguments but a return type 
    UINT8 that indicates whether initialization is successful. 
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
#define MCHP_PSF_HOOK_UPDHW_INTF_INIT()               0     

/*************************************************************************************************
Function:
    MCHP_PSF_HOOK_UPD_COMM_ENDIS(u8PortNum,u8EnDis)
Summary:
    Enable or disables hardware interface(SPI/I2C) communication to the port's UPD350
Description:
    This hook is called before and after the read/write operations of UPD350 registers with port
    specifically enable/disable hardware interface(I2C/SPI) communication to the port's UPD350.
    For SPI interface, SPI chip select level shall be driven low/high with respect to the port number 
    passed in the hook to enable/disable SPI communication respectively to the port's UPD350.
    For I2C interface, I2C mux shall be configured to route/disable the SOC's I2C master to the port
    number passed in the hook to enable/disable I2C communication respectively to the port's UPD350.
    Define relevant function that has two UINT8 argument with out return type.    
Conditions:
    None.
Input:
    u8PortNum - Port number of the device. It takes value between 0 to (CONFIG_PD_PORT_COUNT-1).
    u8EnDis - Parameter indicating whether to enable or disable the communication for the port. 
Return:
    None.
Example:
    <code>
        #define MCHP_PSF_HOOK_UPD_COMM_ENDIS (u8PortNum,u8EnDis)\
						hw_spi_drive_cs (u8PortNum, u8EnDis)
        void hw_spi_drive_cs (UINT8 u8PortNum,UINT8 u8EnDis);
        void hw_spi_drive_cs (UINT8 u8PortNum,UINT8 u8EnDis)
        {
            if (u8EnDis == TRUE)
            {
                //Set pin level low for SOC's GPIO that is connected to the u8PortNum port's 
                //UPD350 SPI CS pin
            }
            else if(u8EnDis == FALSE)
            {
                //Set pin level high for SOC GPIO that is connected to the u8PortNum port's
                //UPD350 SPI CS pin
            }
        }
    </code>
    <code>
        #define MCHP_PSF_HOOK_UPD_COMM_ENDIS(u8PortNum,u8EnDis)\
							hw_port_i2cmux_routing(u8PortNum,u8EnDis)	
        void hw_port_i2cmux_routing(u8PortNum,u8EnDis);
        void hw_port_i2cmux_routing(u8PortNum,u8EnDis)
        {
            if (u8EnDis == TRUE)
            {
                //Route the I2C mux to the u8PortNum UPD350 Port 
            }
            else if(u8EnDis == FALSE)
            { 
                //disable the I2C mux routing to the u8PortNum UPD350 Port
            }
        }
    </code>
Remarks:
    User definition of this Hook function is mandatory for SPI Hardware interface                                           
*************************************************************************************************/
#define MCHP_PSF_HOOK_UPD_COMM_ENDIS(u8PortNum,u8EnDis)

/*********************************************************************************************
Function:
    MCHP_PSF_HOOK_UPD_WRITE(u8PortNum,pu8WriteBuf,u8WriteLen)
Summary:
    Initiates a write transfer to UPD350 via I2C/SPI
Description:
    This hook is called to write to UPD350 registers specific to the port. Its definition is 
    confined to CONFIG_DEFINE_UPD350_HW_INTF_SEL definition for SPI or I2C selection. Define 
    relevant function that has UINT8, UINT8*, UINT8 arguments with a return type UINT8.
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
#define MCHP_PSF_HOOK_UPD_WRITE(u8PortNum,pu8WriteBuf,u8WriteLen)	0

/***************************************************************************************
Function:
    MCHP_PSF_HOOK_UPD_READ(u8PortNum,pu8WriteBuf,u8WriteLen,pu8ReadBuf,u8ReadLen)   
Summary:
    Initiates a read transfer to UPD350 via I2C/SPI
Description:
    This hook is called to read to UPD350 registers with respect to the port. Its definition is 
    confined CONFIG_DEFINE_UPD350_HW_INTF_SEL definition for SPI/I2C selection. Define relevant 
    function that has UINT8, UINT8*, UINT8, UINT8*, UINT8 arguments with UINT8 return type.
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
#define MCHP_PSF_HOOK_UPD_READ(u8PortNum,pu8WriteBuf,u8WriteLen,pu8ReadBuf, u8ReadLen)	0

// *****************************************************************************
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
    PSF requires a single dedicated hardware timer module for its functionality. This Hook
    initializes and starts the hardware timer module for MCHP_PSF_PDTIMER_INTERRUPT_RATE interrupt
    frequency. To inform PSF about the occurrence of hardware timer interrupt  API 
    MchpPSF_PDTimerHandler should be called by the SOC layer on every timer interrupt. Define 
    relevant function that has no argument with UINT8 return type. 
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
#define MCHP_PSF_HOOK_HW_PDTIMER_INIT()		0

/**************************************************************************************************
Summary:
	PD Timer Interrupt Rate
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
    set will be 65535 ticks.(Ticks = Resolution of the Hardware timer used). When set as 0 , maximum 
    timeout that can be set will be 4294967296 ticks. Default value of MCHP_PSF_CONFIG_16BIT_PDTIMER_COUNTER is 
    set as 1. With Hardware timer resolution set as 1ms , PSF will be capable of handling timeouts 
    upto 65.535 Seconds.
Remarks :
    None
Example :
    <code>
      #define MCHP_PSF_CONFIG_16BIT_PDTIMER_COUNTER	1 (Sets timeout variable inside the PSF as unsigned 16bit)
      #define MCHP_PSF_CONFIG_16BIT_PDTIMER_COUNTER	0 (Sets timeout variable inside the PSF as unsigned 32bit)
    </code>
**************************************************************************************************/
#define MCHP_PSF_CONFIG_16BIT_PDTIMER_COUNTER	0

// *****************************************************************************
// *****************************************************************************
// Section: UPD350 IRQ Control
// *****************************************************************************
// *****************************************************************************
/**************************************************************************
Function:
    MCHP_PSF_HOOK_UPD_IRQ_GPIO_INIT(u8PortNum)
Summary:
    Initializes the SOC GPIOs connected to the IRQ_N lines of ports' UPD350.
Description:
    PSF requires GPIO specific to each port for UPD350 interrupt detection via UPD350's IRQ_N lines.
    IRQ_N is an active low signal. This Hook shall initialize the SOC GPIOs connected to the IRQ_N
    lines of UPD350s in the system for interrupt notification. It is recommended to configure SOC
    GPIOs interrupt in edge level detection with internal pull up since the UPD350 keeps the IRQ_N
    line in low state until the interrupt is cleared. To inform PSF the occurrence of UPD350 
    interrupt, API MchpPSF_UPDIrqHandler shall be called by SOC on interrupt detection port
    specifically. Define relevant function that has port number as argument without return type.
Conditions:
    SOC GPIO connected to IRQ_N should be wakeup capable if INCLUDE_POWER_MANAGEMENT_CTRL defined as 1.
Input:
    u8PortNum - Port number of the device. It takes value between 0 to (CONFIG_PD_PORT_COUNT-1).
Return:
    None.
Example:
    <code>
		#define MCHP_PSF_HOOK_UPD_IRQ_GPIO_INIT(u8PortNum)      INT_ExtIrqInit(u8PortNum)
        void INT_ExtIrqInit(UINT8 u8PortNum);
        void INT_ExtIrqInit(UINT8 u8PortNum)
        {
            if(0 == u8PortNum)
            {
                //Initializes the SOC GPIO that is connected to Port 0's UPD350
                //Configure GPIO for internal pull up and low level edge detection 
                //Register MchpPSF_UPDIrqHandler(0) as callback for interrupt occurence
            }
            if(1 == u8PortNum)
            {
                //Initializes the SOC GPIO that is connected to Port 1's UPD350
                //Configure GPIO for internal pull and low level edge detection
                //Register MchpPSF_UPDIrqHandler(1) as callback for interrupt occurrence
            }
        }
    </code>
Remarks:
    User definition of this Hook function is mandatory                     
**************************************************************************/
#define MCHP_PSF_HOOK_UPD_IRQ_GPIO_INIT(u8PortNum)

// *****************************************************************************
// *****************************************************************************
// Section: UPD350 Reset Control
// *****************************************************************************
// *****************************************************************************
/**************************************************************************
Function:
    MCHP_PSF_HOOK_UPD_RESET_GPIO_INIT(u8PortNum)
Summary:
    Initializes the SOC GPIOs connected to the RESET_N lines of UPD350s
Description:
    This hook initializes the SOC GPIOs connected to the RESET_N lines of Port's UPD350. It is 
    recommended to connect a single GPIO to the reset line of all UPD350s. User can also have 
    separate GPIO for each port. As the UPD350 RESET_N is active low signal, SOC should initialize 
    the GPIO to be high by default. Define relevant function that has port number as argument 
    without return type.
Conditions:
    None.
Input:
    u8PortNum -  Port number of the device. It takes value between 0 to (CONFIG_PD_PORT_COUNT-1).
Return:
    None.
Example:
    <code>
        #define MCHP_PSF_HOOK_UPD_RESET_GPIO_INIT(u8PortNum)      updreset_init(u8PortNum)
        void updreset_init(UINT8 u8PortNum);
        void updreset_init(UINT8 u8PortNum)
        {
            // If single SOC GPIO is connected to all the UPD350's, do initialisation only once 
            // when PortNum is '0'. If separate GPIOs are used for each port UPD350, do 
			//initialisation port specifically
            if (0 == u8PortNum)
            {
                //Initialization of SOC GPIO connected to UPD350 reset lines
                //Make the gpio line high by default
            }
        }
    </code>
Remarks:
    User definition of this Hook function is mandatory                      
*************************************************************************/
#define MCHP_PSF_HOOK_UPD_RESET_GPIO_INIT(u8PortNum)             

/**********************************************************************
Function:
    MCHP_PSF_HOOK_UPD_RESET_THRU_GPIO(u8PortNum)
Summary:
    Resets the UPD350 connected specific to the port. 
Description:
    This hook is to reset the UPD350 connected to the port by driving the SOC GPIO connected to the
    RESET_N pin of that UPD350. Since, RESET_N is active low signal, SOC GPIO should be driven low 
    for a while and then back to default high state. It is recommended to have common reset pin for 
    all ports. In such case user must drive the GPIO for UPD350 reset only when u8PortNum passed is 
    '0' via this hook.Define relevant function that has port number as argument without return type.
Conditions:
    None.
Input:
    u8PortNum -  Port number of the device. It takes value between 0 to (CONFIG_PD_PORT_COUNT-1).
Return:
    None.
Example:
    <code>
        #define MCHP_PSF_HOOK_UPD_RESET_THRU_GPIO(u8PortNum)      updreset_thru_gpio(u8PortNum)
        void updreset_thru_gpio(UINT8 u8PortNum);
        void updreset_thru_gpio(UINT8 u8PortNum)
        {
            if (0 == u8PortNum)
            {
               //Enable pull down
               // Wait for xxx uS
               // Enable pull up
            }
        }
    </code>
Remarks:
    User definition of this Hook function is mandatory                  
**********************************************************************/
#define MCHP_PSF_HOOK_UPD_RESET_THRU_GPIO(u8PortNum)

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
    This hook is called when PSF enter into a critical section. It must provide an implementation
    to disable the interrupts globally. This hook implementation must be very short, otherwise 
    response time to the interrupt will take longer time. Define relevant function that has no 
    arguments without return type.
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
#define MCHP_PSF_HOOK_DISABLE_GLOBAL_INTERRUPT()           

/*******************************************************************************
Function:
    MCHP_PSF_HOOK_ENABLE_GLOBAL_INTERRUPT()
Summary:
    Enables the global interrupt.
Description:
    This hook is called by the PSF when exits from critical section. It must provide an 
    implementation to enable the interrupts globally. This function must be very short, otherwise 
    response time to the interrupt will take longer time. Define relevant function that has no 
    arguments without return type.
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
#define MCHP_PSF_HOOK_ENABLE_GLOBAL_INTERRUPT()

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
    This function is called to compare two memory regions pau8Obj1, pau8Obj2 with specified length 
    u8Length. User must define this hook based on compiler of SOC.
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
#define MCHP_PSF_HOOK_MEMCMP(pObj1, pObj2, iLength)       0                   

/**************************************************************************
Function:
    MCHP_PSF_HOOK_MEMCPY(pDest, pSrc, iLen)
Summary:
    Copies one memory area to another memory area
Description:
    This function is called to copy iLen bytes from pSrc memory area to pDest memory area. User must 
    define this function based on compiler of SOC. The memory areas must not overlap.
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
#define MCHP_PSF_HOOK_MEMCPY(pDest, pSrc, iLen) 	0

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
    Generally packed structures will be used to save space & align the bytes in data memory based 
    on the compiler. If this pre-processor is defined, then all the PSF's "C" structures will be 
    replaced with this keyword for compilation. If this pre-processor is not defined, then it will  
    be default compilation rules based on the compiler.
Remarks:
    Need to be packed always based on type of SOC.	
Example:
    <code>
        #define MCHP_PSF_STRUCT_PACKED_START   __attribute__((__packed__)) 
    </code>
**************************************************************************************************/
#define MCHP_PSF_STRUCT_PACKED_START

/**************************************************************************************************
Summary:
    Structure packing to align the bytes in data memory based on the compiler.
Description:
    Generally packed structures will be used to save space & align the bytes in data memory based 
    on the compiler. If this pre-processor is defined, then all the PSF's "C" structures will be 
    replaced with this keyword for compilation. If this pre-processor is not defined, then it will  
    be default compilation rules based on the compiler.
Remarks:
    Need to be packed always based on type of SOC.
Example:
    <code>
        #define CONFIG_STRUCT_PACKED_END    _Pragma("pack()")
    </code>
**************************************************************************************************/
#define MCHP_PSF_STRUCT_PACKED_END

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
    boost controller and load switch.
Description:
    PSF provides default DC-DC Buck booster control configuration via CONFIG_DCDC_CTRL define.
    User can chose to implement their own DC-DC buck booster control or modify the default using 
    this hook. This hook is to initialize the hardware modules related to port power functionality. 
    Implementation of this function depends on the type of DC-DC buck boost controller and load
    switch used. Define relevant function that has no argument without return type.
Conditions:
    API implementation must make sure the Port Power(VBUS) of all ports must be set to 0V.
Input:
    u8PortNum -  Port number of the device. It takes value between 0 to (CONFIG_PD_PORT_COUNT-1).
Return:
    None.
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
    User definition of this Hook function is mandatory if CONFIG_DCDC_CTRL is undefined                        
*****************************************************************************/
#define MCHP_PSF_HOOK_HW_PORTPWR_INIT(u8PortNum)                       

/****************************************************************************
Function:
    MCHP_PSF_HOOK_PORTPWR_DRIVE_VBUS(u8PortNum, u16VBUSVolatge, u16Current)
Summary:
    Drives the VBUS for given voltage, current
Description:
    PSF provides default DC-DC Buck booster control configuration via CONFIG_DCDC_CTRL define. If 
    user chose to implement their own DC-DC buck booster control, this hook must be implemented to
    drive VBUS as per the parameter passed based on voltage and current. It can also be used to 
    modify the default option. Implementation of this function depends on the type of DC-DC buck 
    boost controller and load switch used. Define relevant function that has UINT8,UINT16, UINT16
    arguments without return type.
Conditions:
    It is applicable only for Source operation.
Input:
    u8PortNum -  Port number of the device. It takes value between 0 to (CONFIG_PD_PORT_COUNT-1).         
    u16VBUSVolatge - VBUS Voltage level to be driven in VBUS expressed in terms of milliVolts.
    u16Current     - VBUS current level in terms of mA.
Return:
    None.
Example:
    <code>
        #define MCHP_PSF_HOOK_PORTPWR_DRIVE_VBUS(u8PortNum, u16VBUSVolatge, u16Current)\
                hw_portpower_driveVBUS(u8PortNum, u16VBUSVolatge, u16Current)
      void hw_portpower_driveVBUS(UINT8 u8PortNum, UINT16 u16VBUSVolatge, UINT16 u16Current);
      void hw_portpower_driveVBUS(UINT8 u8PortNum, UINT16 u16VBUSVolatge, UINT16 u16Current)
      {
            // Configure DC-DC buck boost control to drive u16VBUSVolatge & u16Current in VBUS
      }
    </code>
Remarks:
    User definition of this Hook function is mandatory if CONFIG_DCDC_CTRL is undefined.                      
****************************************************************************/
#define MCHP_PSF_HOOK_PORTPWR_DRIVE_VBUS(u8PortNum,u16VBUSVolatge,u16Current)

/*******************************************************************************************
Function:
    MCHP_PSF_HOOK_PORTPWR_ENDIS_VBUSDISCH(u8PortNum,u8EnableDisable)   
Summary:
    Enables or disables VBUS Discharge functionality for a given port.
Description:
    VBUS Discharge mechanism is required to enable quick of discharge VBUS when VBUS transition from 
    higher to lower voltage. PSF provides default DC-DC Buck booster control configuration via 
    CONFIG_DCDC_CTRL define. If user chose to implement their own DC-DC buck booster control, this 
    hook must be implemented to enable or disable the VBUS Discharge functionality for a given Port. 
    Implementation of this function depends on the type of DC-DC buck boost controller and load 
    switch used. Define relevant function that has UINT8,UINT8 arguments without return type.
Conditions:
    MCHP_PSF_HOOK_PORTPWR_ENDIS_VBUSDISCH is called in ISR handler. Its implementation shall be very 
    short, otherwise response time to the interrupt will take longer time. Passing of the Compliance
    test "TD.4.2.1" (Source Connect Sink) in "USB_Type_C_Functional_Test_Specification" depends on 
    the VBUS Discharge circuitry used. Typical VBUS Discharge time from any higher voltage to 0V 
    should be around 10ms.
Input:
    u8PortNum -  Port number of the device. It takes value between 0 to (CONFIG_PD_PORT_COUNT-1).
    u8EnableDisable -  Flag indicating whether to enable/disable VBUS Discharge mechanism.
Return:
    None.
Example:
    <code>
        #define MCHP_PSF_HOOK_PORTPWR_ENDIS_VBUSDISCH(u8PortNum, u8EnableDisable) \
                      hw_portpower_enab_dis_VBUSDischarge(u8PortNum, u8EnableDisable)
        void hw_portpower_enab_dis_VBUSDischarge(UINT8 u8PortNum,UINT8 u8EnableDisable);
        void hw_portpower_enab_dis_VBUSDischarge(UINT8 u8PortNum,UINT8 u8EnableDisable)
        {
            if (TRUE == u8EnableDisable)
            {
                //Enable the VBUS Discharge for "u8PortNum" Port
            }
            else
            {
                //Disable the VBUS Discharge for "u8PortNum" Port
            }
        }
    </code>
Remarks:
    User definition of this Hook function is mandatory if CONFIG_DCDC_CTRL is undefined.                                    
*******************************************************************************************/
#define MCHP_PSF_HOOK_PORTPWR_ENDIS_VBUSDISCH(u8PortNum, u8EnableDisable)	

/*******************************************************************************************
Function:
    MCHP_PSF_HOOK_PORTPWR_CONFIG_SINK_HW(u8PortNum,u16Voltage,u16Current)
Summary:
    Enables or disables sink hardware circuitry and configures it to sinks the VBUS voltage for 
    a given port based on the sink requested voltage and current.
Description:
    This hook is to enable or disable sink hardware circuitry and configure it for Sink requested 
    requested current and voltage.Implementation of this function depends on the type of Sink 
    circuitry used. Define relevant function that has UINT8,UINT16,UINT16 arguments without return type.
Conditions:
    It is applicable only for Sink operation.
Input:
    u8PortNum -  Port number of the device. It takes value between 0 to (CONFIG_PD_PORT_COUNT-1).
    u16voltage -  Enable Sink HW Circuitry if the u16voltage is not Vsafe0V to drain power.
                    Disable sink HW circuitry if the u16voltage is VSafe0V.
                    Configure the HW to requested u16voltage in mV.
    u16Current -  Configure the HW for the requested current passed in terms of mA.
Return:
    None.
Example:
    <code>
        #define MCHP_PSF_HOOK_PORTPWR_CONFIG_SINK_HW(u8PortNum, u16Voltage, u16Current)\
            hw_SinkCircuitary_enab_dis_(u8PortNum, u16Voltage, u16Current)
        void hw_SinkCircuitary_enab_dis_(UINT8 u8PortNum,UINT16 u16Votlage,UINT16 u16Current);
        void hw_SinkCircuitary_enab_dis_(UINT8 u8PortNum,UINT16 u16Votlage,UINT16 u16Current)
        {
            if(u16Voltage == Vsafe0V)
            {
                //Disable the Sink circuitary for "u8PortNum" Port
            }
            else
            {
                //Enable the Sink circuitary for "u8PortNum" Port and configure it to drain u16Voltage 
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
// Section: Boot Time Configuration
// *****************************************************************************
// *****************************************************************************   
/**********************************Globals*********************************************/
/**************************************************************************************************
Summary:
	User configurable boot time structure.
Description:
	User Configurable Structure at boot time: This structure contains the Type-C configure parameters 
	PDO count and PDOs parameters
Remarks:
    None
**************************************************************************************************/
typedef struct MCHP_PSF_STRUCT_PACKED_START _PortData_cfg 
{
	UINT32 u32CfgData;			//Bits 2:0 - Port Role
								//Bits 4:3 - Type-C Current
								//Bits 5 - Port Enable/Disable
	UINT32 u32PDO[7];		    //Source/Sink Capabilities PDOs
	UINT8  u8PDOCnt;			//PDO count of Source/Sink Capabilities
}MCHP_PSF_STRUCT_PACKED_END PORT_CONFIG_DATA;

extern PORT_CONFIG_DATA gasPortConfigurationData[CONFIG_PD_PORT_COUNT];
/**************************************************************************************************
Function:
    MCHP_PSF_HOOK_BOOT_TIME_CONFIG(CfgGlobals)
Summary :
    Updates the Type-C Configurable parameters(Power Role and Rp Current), number of PDO and PDOs 
    parameter at boot time.
Description :
    This function is called to update the configuration parameters of Type-C (Power Role and Rp Current),
    number of PDO and PDOs parameter at boot time(Stack initialization). 
    This API must have a input parameter of PORT_CONFIG_DATA prototype (Structure Pointer to PORT_CONFIG_DATA).
Precondition :
    None.
Parameters :
    CfgGlobals - Holds the structure pointer of the structure PORT_CONFIG_DATA
Return :
    None.
Example :
    <code>
    #define  MCHP_PSF_HOOK_BOOT_TIME_CONFIG(_CfgGlobals_)  STRAPS_PowerRole_Set(_CfgGlobals_)
    void STRAPS_PowerRole_Set(PORT_CONFIG_DATA *PortConfigData);
    void STRAPS_PowerRole_Set(PORT_CONFIG_DATA *PortConfigData)
    {
        // Configure Cfg variables for Source or Sink
    }
    </code>
Remarks:
    User definition of this Hook function is optional
**************************************************************************************************/
#define  MCHP_PSF_HOOK_BOOT_TIME_CONFIG(CfgGlobals)  	

// *****************************************************************************
// *****************************************************************************
// Section: HOOK CONFIGURATION
// *****************************************************************************
// *****************************************************************************
/**************************************************************************
Function:
    MCHP_PSF_HOOK_DPM_PRE_PROCESS(u8PortNum)
Summary:
    This hook is called before entering into the DPM state machine.
Description:
    This hook is called at the entry of DPM_RunStateMachine() API before executing the Type C state 
    machine and policy engine state machine. USER_APPLICATION can define this function if a change 
    is required in default device policy manager functionality or add a user defined state machine. 
    Define relevant function that has one UINT8 argument without return type.
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
    This hook is called during initialization of PSF if CONFIG_HOOK_DEBUG_MSG is set to 1. Define
    relevant function to initialize the Debug interface used with no arguments without return type.
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
#define MCHP_PSF_HOOK_DEBUG_INIT()   

/*************************************************************************
Function:
    MCHP_PSF_HOOK_DEBUG_STRING(pcharBuf)
Summary:
    To pass a string to Debug interface
Description:
    This hook is called by PSF to send a character string to DEBUG_MODULE. It will be called if
    CONFIG_HOOK_DEBUG_MSG is set to 1. Define relevant function that has CHAR pointer argument 
    without return type.
Conditions:
    None.
Input:
    pcharBuf -  Pointer to the character buffer
Return:
    None.
Example:
    <code>
        #define MCHP_PSF_HOOK_DEBUG_STRING(pcharBuf)       uart_write(pcharBuf)
        void uart_write(char *chBuffer);
        void uart_write(char *chBuffer)
        {
            //Write character string to UART
        }
    </code>
Remarks:
    User definition of this Hook function is mandatory when CONFIG_HOOK_DEBUG_MSG is declared as '1'.             
*************************************************************************/ 
#define MCHP_PSF_HOOK_DEBUG_STRING(pcharBuf)		                                  

/**************************************************************************
Function:
    MCHP_PSF_HOOK_DEBUG_UINT8(u8Val)
Summary:
    Send a UINT8 to Debug interface
Description:
    This hook is called by stack to send a UINT8 data to debug interface. This API will be called 
    if CONFIG_HOOK_DEBUG_MSG is set to 1. Define relevant function that has UINT8 argument without 
    return type.
Conditions:
    None.
Input:
    u8Val -  UINT8 data to be sent to Debug interface
Return:
    None.
Example:
    <code>
        #define MCHP_PSF_HOOK_DEBUG_UINT8(u8Val)     uart_write(u8Val)
        void uart_write(UINT8 u8Val);
        void uart_write(UINT8 u8Val)
        {
            //Convert UINT8 to character string and write to UART
        }
    </code>
Remarks:
    User definition of this Hook function is mandatory when CONFIG_HOOK_DEBUG_MSG is declared as '1'. 
**************************************************************************/ 
#define MCHP_PSF_HOOK_DEBUG_UINT8(u8Val)              

/*************************************************************************
Function:
    MCHP_PSF_HOOK_DEBUG_UINT16(u16Val)
Summary:
    Send UINT16 to Debug interface
Description:
    This hook is called by stack to send a UINT16 data to DEBUG_MODULE This API will be called if
    CONFIG_HOOK_DEBUG_MSG is set to 1. Define relevant function that has UINT16 argument without 
    return type.
Conditions:
    None.
Input:
    u16Val -  UINT16 data to be sent to Debug interface
Return:
    None.
Example:
    <code>
        #define MCHP_PSF_HOOK_DEBUG_UINT16(u16Val)     uart_write(u16Val)
        void uart_write(UINT16 u16Val);
        void uart_write(UINT16 u16Val)
        {
            //Convert UINT16 to character string and write to UART
        }

    </code>
Remarks:
   User definition of this Hook function is mandatory when CONFIG_HOOK_DEBUG_MSG is declared as '1'. 
*************************************************************************/ 
#define MCHP_PSF_HOOK_DEBUG_UINT16(u16Val)      

/*************************************************************************
Function:
    MCHP_PSF_HOOK_DEBUG_UINT32(u32Val)
Summary:
    Send UINT32 to Debug interface
Description:
    This hook is called by stack to send a UINT32 data to DEBUG_MODULE. This API will be called if
    CONFIG_HOOK_DEBUG_MSG is set to 1. Define relevant function that has a UINT32 argument without 
    return type.
Conditions:
    None.
Input:
    u32Val -  UINT32 data to be sent to Debug interface
Return:
    None.
Example:
    <code>
        #define MCHP_PSF_HOOK_DEBUG_UINT32(u32Val)    uart_write(u32Val)
        void uart_write(UINT32 u32Val);
        void uart_write(UINT32 u32Val)
        {
            //Convert UINT32 to character string and write to UART
        }
    </code>
Remarks:
    User definition of this Hook function is mandatory when CONFIG_HOOK_DEBUG_MSG is declared as '1'. 
*************************************************************************/ 
#define MCHP_PSF_HOOK_DEBUG_UINT32(u32Val)		                        

/**************************************************************************
Function:
    MCHP_PSF_HOOK_DEBUG_INT32(i32Val)
Summary:
    Send INT32 to Debug interface
Description:
    This hook is called by stack to send a INT32 data to Debug interface. This API will be called if
    CONFIG_HOOK_DEBUG_MSG is set to 1. Define relevant function that has INT32 argument without 
    return type.
Conditions:
    None.
Input:
    i32Val -  INT32 data to be sent to DEBUG_MODULE
Return:
    None.
Example:
    <code>
        #define MCHP_PSF_HOOK_DEBUG_INT32(_i32Val_)        uart_write(i32Val)
        void uart_write(INT32 i32Val);
        void uart_write(INT32 i32Val)
        {
            //Convert INT32 to character string and write to UART
        }
    </code>
Remarks:
    User definition of this Hook function is mandatory when CONFIG_HOOK_DEBUG_MSG is declared as '1'. 
**************************************************************************/ 
#define MCHP_PSF_HOOK_DEBUG_INT32(i32Val)		                  

// *****************************************************************************
// *****************************************************************************
// Section: Power Delivery Firmware Update
// *****************************************************************************
// *****************************************************************************
/**************************************************************************
Function:
    MCHP_PSF_HOOK_GETCURRENT_IMAGEBANK()
Summary:
    To Return the Index of the Image Bank which is currently executing.
    Example:
        1. 0x01 - Corresponds to Bootloader Application
        2. 0x02 - Corresponds to Fixed Application
        3. 0x03 - Corresponds to Updatable Application
Description:
    This hook is called by PSF to get the Index of the image bank which is currently executing in 
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
    To validate the Flashed Binary using a user defined validation method and return the status of
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
    Application image bank, requires switch to Fixed application for performing the upgrade.
    The application switching may include invalidating the Updatable_Application signatures (and/or)
    jump/reset for fresh boot from Fixed application.
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
Description:
    MCHP_PSF_HOOK_IS_PDFU_ALLOWED_NOW specifies if PD Firmware Update can be currently allowed, 
    based on the priority of the application tasks currently executing.
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
    For the current PSF implementation, return value is not used. API called with void. With SAMD20 
	environment configured for CPU frequency 48MHZ, this API took maximum of 3.488ms and 6.182ms 
	execution time for 1 and 2 port solution respectively.
**************************************************************************************************/
UINT8 MchpPSF_Init(void);

/**************************************************************************************************
Function:
    void MchpPSF_RUN(void)
Summary:
    PSF state machine run API
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
	In SAMD20 environment where CPU frequency is configured for 48MHZ and single threaded environment 
	where MchpPSF_RUN is called in a while(1) loop, it took maximum of 74.57us and 0.1439 ms execution
	time for 1 port and 2 port solution respectively. Multi threaded Free RTOS environment is 
	untested and latency of API call is to identified yet.
**************************************************************************************************/
void MchpPSF_RUN(void);

/**************************************************************************************************
Function:
    void MchpPSF_UPDIrqHandler(UINT8 u8PortNum)
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
void MchpPSF_UPDIrqHandler(UINT8 u8PortNum);

/**************************************************************************************************
Function:
    void MchpPSF_PDTimerHandler(void)
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
void MchpPSF_PDTimerHandler(void);

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
    
    <b>eMCHP_PSF_UPDS_IN_IDLE: </b>This event is posted by Power management control. PSF runs an
	algorithm backend for Power management control. If there is no activity in UPD350 for 
	CONFIG_PORT_UPD_IDLE_TIMEOUT_MS corresponding UPD350 is put to low power mode. When all the
	UPD350 present in the system enters low mode, eMCHP_PSF_UPDS_IN_IDLE is posted. User can put 
	SOC in low power mode as required on this notification. This notification occurs only when
    INCLUDE_POWER_MANAGEMENT_CTRL defined as 1.
    
    <b> eMCHP_PSF_VCONN_PWR_FAULT:</b> UPD350 has VCONN comparators to detect VCONN OCS faults. 
	This event is notified when VCONN OCS fault is detected by UPD350. For this notification, PSF
	expects a return value to decide whether to handle the fault occurred. When user returns TRUE
	for VCONN power fault, Incase of explicit contract, if VCONN power fault count is less than
	CONFIG_MAX_VCONN_POWER_FAULT_COUNT, PSF DPM power fault manager handles it by sending Hard Reset.
	If the count exceeds max fault count,VCONN is powered off until physical detach of port partner.
	Incase of implicit contract, PSF handles by entering TypeC Error Recovery. This notification
	occurs only when INCLUDE_POWER_FAULT_HANDLING is defined as 1.
    
    <b> eMCHP_PSF_VBUS_PWR_FAULT</b>: PSF notifies all VBUS power fault VBUS Over voltage, VBUS
	under voltage, VBUS OCS via this notification. For this notification, PSF expects a return
	value to decide whether to handle the fault occurred.When user returns TRUE for power fault,
    Incase of explicit contract, if power fault count is less than CONFIG_MAX_VBUS_POWER_FAULT_COUNT,
	PSF DPM power fault manager handles it by sending Hard Reset. When the power fault count 
	exceeds the max fault count,CC termination on the port is removed until the physical detach of
	the port partner. Incase of implicit contract, PSF handles by entering TypeC Error Recovery.
	This notification occurs only when INCLUDE_POWER_FAULT_HANDLING is defined as 1.
Remarks:
    None                                                                                               
  ******************************************************************************************************/
typedef enum MCHP_PSF_NOTIFICATION
{    
eMCHP_PSF_TYPEC_DETACH_EVENT = 1,   // Detach event has occurred
eMCHP_PSF_TYPEC_CC1_ATTACH,         // Port partner attached at CC1 orientation
eMCHP_PSF_TYPEC_CC2_ATTACH,         // Port partner attached at CC2 orientation
eMCHP_PSF_UPDS_IN_IDLE,             // All the UPD350s are in Idle
eMCHP_PSF_VCONN_PWR_FAULT,          // VCONN Power Fault has occurred
eMCHP_PSF_VBUS_PWR_FAULT            // VBUS Power Fault has occurred
} eMCHP_PSF_NOTIFICATION;

/****************************************************************************************************
Function:
    MCHP_PSF_NOTIFY_CALL_BACK(u8PortNum, ePSFNotification)   
Summary:
    Notifies the USER_APPLICATION about various PD events from PSF.
Description:
    This hook is called by the various modules of PSF to notify the USER_APPLICATION about different 
    PD events such as Type-C Attach and Detach , Type-C Orientation. USER_APPLICATION can define 
    this hook function if it wants external handling of the PD events. Define relevant function that 
    has  UINT8, eMCHP_PSF_NOTIFICATION argument without return type.
Conditions:
    None.
Input:
    u8PortNum -  Port number of the device. It takes value between 0 to (CONFIG_PD_PORT_COUNT-1).
    ePSFNotification -   Type of Notification occurred inside the stack. This argument can take 
                        one of the values from enum eMCHP_PSF_NOTIFICATION.                   
Return:
    UINT8 - Except for eMCHP_PSF_VCONN_PWR_FAULT and eMCHP_PSF_VBUS_PWR_FAULT the return value is 
            ignored by PSF.
            For eMCHP_PSF_VCONN_PWR_FAULT and eMCHP_PSF_VBUS_PWR_FAULT event, user can return 
                TRUE - if the Power fault shall be handled by PSF
                FALSE - if the Power fault occurrence is ignored.
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
#define MCHP_PSF_NOTIFY_CALL_BACK(u8PortNum, ePSFNotification)
                                           
#endif /*_PSF_API_HOOK_H_*/

