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

#include "PSF_Config.h"
#include "Drivers.h"
#include "PSFSourcePro_App.h"
#include "Mpq_dc_dc_control.h"
#include "PSFSourcePro_BootCfg.h"

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
#define MCHP_PSF_HOOK_UPDHW_INTF_INIT()      SAMD20_SPIInitialisation();              

/*************************************************************************************************
Function:
    MCHP_PSF_HOOK_UPD_COMM_ENABLE(u8PortNum,u8Enable)
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
    u8Enable - Parameter indicating whether to enable or disable the communication for the port. 
               When u8Enable is 1 - Enable interface, 0 - Disable interface.
Return:
    None.
Example:
    <code>
        #define MCHP_PSF_HOOK_UPD_COMM_ENABLE (u8PortNum,u8Enable)\
						hw_spi_drive_cs (u8PortNum, u8Enable)
        void hw_spi_drive_cs (UINT8 u8PortNum,UINT8 u8Enable);
        void hw_spi_drive_cs (UINT8 u8PortNum,UINT8 u8Enable)
        {
            if (u8Enable == TRUE)
            {
                //Set pin level low for SOC's GPIO that is connected to the u8PortNum port's 
                //UPD350 SPI CS pin
            }
            else if(u8Enable == FALSE)
            {
                //Set pin level high for SOC GPIO that is connected to the u8PortNum port's
                //UPD350 SPI CS pin
            }
        }
    </code>
    <code>
        #define MCHP_PSF_HOOK_UPD_COMM_ENABLE(u8PortNum,u8Enable)\
							hw_port_i2cmux_routing(u8PortNum,u8Enable)	
        void hw_port_i2cmux_routing(u8PortNum,u8Enable);
        void hw_port_i2cmux_routing(u8PortNum,u8Enable)
        {
            if (u8Enable == TRUE)
            {
                //Route the I2C mux to the u8PortNum UPD350 Port 
            }
            else if(u8Enable == FALSE)
            { 
                //disable the I2C mux routing to the u8PortNum UPD350 Port
            }
        }
    </code>
Remarks:
    User definition of this Hook function is mandatory for SPI Hardware interface                                           
*************************************************************************************************/
#define MCHP_PSF_HOOK_UPD_COMM_ENABLE(u8PortNum,u8Enable) SAMD20_DriveChipSelect(u8PortNum,u8Enable)

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
#define MCHP_PSF_HOOK_UPD_WRITE(u8PortNum,pu8WriteBuf,u8WriteLen)\
            SAMD20_SPIWritedriver(u8PortNum,pu8WriteBuf,u8WriteLen)

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
#define MCHP_PSF_HOOK_UPD_READ(u8PortNum,pu8WriteBuf,u8WriteLen,pu8ReadBuf, u8ReadLen)\
            SAMD20_SPIReaddriver(u8PortNum,pu8WriteBuf,u8WriteLen,pu8ReadBuf, u8ReadLen)

// *****************************************************************************
#if (CONFIG_DCDC_CTRL == I2C_DC_DC_CONTROL_CONFIG)

/*********************************************************************************************
Function:
    MCHP_PSF_HOOK_UPDI2C_DCDC_INTF_INIT()
Summary:
    Initialize the I2C hardware interface of SOC used for communicating with I2C based DCDC Converter.
Description:
    If I2C based DCDC converter is used to deliver power, PSF will require an I2C hardware interface from SOC to communicate with the DCDC converter.  
 
    This Hook is used to initialize the SOC's Hardware interface for I2C communication with I2C based DCDC converter. It is called 
    during initialization of PSF. Define relevant function that has no arguments but a return type 
    UINT8 that indicates whether initialization is successful. 
Conditions:
	This hook API should be called if I2C based DCDC converter is used to deliver power.
Return:
    UINT8 - Return TRUE if initialization was successful or else return FALSE. 
Example:

    <code>
        #define MCHP_PSF_HOOK_UPDI2C_DCDC_INTF_INIT()      hw_i2c_init()
        UINT8 hw_i2c_init(void);
        UINT8 hw_i2c_init(void)
        {
            //Initialise SOC's I2C master
            //Return TRUE if initialsation is successful else FALSE
        }
    </code>
Remarks:
    User definition of this Hook function is mandatory if I2C based DCDC converter is used to deliver power.   
**************************************************************************/

#define MCHP_PSF_HOOK_UPDI2C_DCDC_INTF_INIT() SAMD20_I2CDCDCInitialisation()

/*********************************************************************************************
Function:
    MCHP_PSF_HOOK_UPDI2C_DCDC_READ(u16Address,pu8ReadBuf,u8ReadLen)
Summary:
    Initiates a read transfer from I2C based DCDC converter via I2C.
Description:
    This hook is called to read registers of I2C based DCDC converter. Define 
    relevant function that has UINT8, UINT8*, UINT8 arguments with a return type UINT8.
Conditions:
    None.
Input:
    u16Address -  Address of I2C DCDC converter.
    pu8ReadBuf - PSF shall pass the pointer to the buffer in which data read via I2C Hardware bus should be stored. 
                 Data type of the pointer buffer must be UINT8*.
    u8ReadLen - PSF shall pass the Number of bytes to be read on the I2C Hardware bus. 
                Data type of this parameter must be UINT8.
Return:
    UINT8 - Return TRUE if read was successful or else return FALSE.
Example:
    <code>
        #define MCHP_PSF_HOOK_UPDI2C_DCDC_READ(u16Address,pu8ReadBuf,u8ReadLen)\   
					I2C_Read (u16Address,pu8ReadBuf,u8ReadLen)
        UINT8 I2C_Read(UINT16 u16Address,UINT8* pu8ReadBuf,UINT8 u8ReadLen);
        UINT8 I2C_Read(UINT16 u16Address,UINT8* pu8ReadBuf,UINT8 u8ReadLen)
        {
			// Select I2C address for the UPD350 I2C slave using u16Address 
            for(UINT8 u8Rxcount = 0; u8Rxcount < u8ReadLen; u8Rxcount++)
            {
                //Read data bytes from I2C bus
            }
            // Return TRUE if the read is successful; else FALSE
        }
    </code>

Remarks:
    User definition of this Hook function is mandatory if I2C based DCDC converter is used to deliver power.                                       
*********************************************************************************************/
#define MCHP_PSF_HOOK_UPDI2C_DCDC_READ(u16Address,pu8ReadBuf,u8ReadLen)       \
            SAMD20_I2CDCDCReadDriver(u16Address,pu8ReadBuf,u8ReadLen)

/*********************************************************************************************
Function:
    MCHP_PSF_HOOK_UPDI2C_DCDC_WRITE(u16Address,pu8WriteBuf,u8WriteLen)
Summary:
    Initiates a write transfer to I2C based DCDC converter via I2C.
Description:
    This hook is called to write to registers of I2C based DCDC converter. Define 
    relevant function that has UINT8, UINT8*, UINT8 arguments with a return type UINT8.
Conditions:
    None.
Input:
    u16Address -  Address of I2C DCDC converter. Data type of this parameter must be UINT16.
    pu8WriteBuf - PSF shall pass the pointer to the buffer which has the data to be written on the
                  I2C Hardware bus. Data type of the pointer buffer must be UINT8*.
    u8WriteLen - PSF shall pass the Number of bytes to be written on the I2C Hardware bus. Data
                 type of this parameter must be UINT8.
Return:
    UINT8 - Return TRUE if write was successful or else return FALSE.
Example:
    <code>
        #define MCHP_PSF_HOOK_UPDI2C_DCDC_WRITE(u16Address,pu8WriteBuf,u8WriteLen)\   
					I2C_Write (u16Address,pu8WriteBuf,u8WriteLen)
        UINT8 I2C_Write(UINT16 u16Address,UINT8* pu8WriteBuf,UINT8 u8WriteLen);
        UINT8 I2C_Write(UINT16 u16Address,UINT8* pu8WriteBuf,UINT8 u8WriteLen)
        {
			// Select I2C address for the UPD350 I2C slave using u16Address 
            for(UINT8 u8Txcount = 0; u8Txcount < u8WriteLen; u8Txcount++)
            {
                //Write data bytes to I2C bus
            }
            // Return TRUE if the write is successful; else FALSE
        }
    </code>

Remarks:
    User definition of this Hook function is mandatory if I2C based DCDC converter is used to deliver power.                                       
*********************************************************************************************/
#define MCHP_PSF_HOOK_UPDI2C_DCDC_WRITE(u16Address,pu8WriteBuf,u8WriteLen)     \
            SAMD20_I2CDCDCWriteDriver(u16Address,pu8WriteBuf,u8WriteLen)

/*********************************************************************************************
Function:
    MCHP_PSF_HOOK_UPDI2C_DCDC_WRITE_READ(u16Address,pu8WriteBuf,u8WriteLen,pu8ReadBuf,u8ReadLen)
Summary:
    Initiates a write and read transfer from I2C based DCDC converter via I2C.
Description:
    This hook is called to write and read registers of I2C based DCDC converter. Define 
    relevant function that has UINT8, UINT8*, UINT8 arguments with a return type UINT8.
Conditions:
    None.
Input:
    u16Address -  Address of I2C DCDC converter. Data type of this parameter must be UINT8.
	pu8WriteBuf - PSF shall pass the pointer to the buffer which has the data to be written on the
                  I2C Hardware bus. Data type of the pointer buffer must be UINT8*.
    u8WriteLen - PSF shall pass the Number of bytes to be written on the I2C Hardware bus. Data
                 type of this parameter must be UINT8.
    pu8ReadBuf - PSF shall pass the pointer to the buffer in which data read via I2C Hardware bus should be stored. Data type of the pointer buffer must be UINT8*.
    u8ReadLen - PSF shall pass the Number of bytes to be read on the I2C Hardware bus. Data
                 type of this parameter must be UINT8.
Return:
    UINT8 - Return TRUE if write and read was successful or else return FALSE.
Example:
    <code>
        #define MCHP_PSF_HOOK_UPDI2C_DCDC_WRITE_READ(u16Address,pu8WriteBuf,u8WriteLen,pu8ReadBuf,u8ReadLen)\   
					I2C_WriteRead (u16Address,pu8WriteBuf,u8WriteLen,pu8ReadBuf,u8ReadLen)
        UINT8 I2C_WriteRead (UINT16 u16Address,UINT8* pu8WriteBuf,UINT8 u8WriteLen,UINT8* pu8ReadBuf,UINT8 u8ReadLen);
        UINT8 I2C_WriteRead (UINT16 u16Address,UINT8* pu8WriteBuf,UINT8 u8WriteLen,UINT8* pu8ReadBuf,UINT8 u8ReadLen)
        {
			// Select I2C address for the UPD350 I2C slave using u16Address 
			
			for(UINT8 u8Txcount = 0; u8Txcount < u8WriteLen; u8Txcount++)
            {
                //Write data bytes to I2C bus
            }
			
            for(UINT8 u8Rxcount = 0; u8Rxcount < u8ReadLen; u8Rxcount++)
            {
                //Read data bytes from I2C bus
            }
            // Return TRUE if the write is successful; else FALSE
        }
    </code>

Remarks:
    This Hook function may be used if I2C based DCDC converter is used to deliver power.                                       
*********************************************************************************************/
#define MCHP_PSF_HOOK_UPDI2C_DCDC_WRITE_READ(u16Address,pu8WriteBuf,u8WriteLen,pu8ReadBuf,u8ReadLen) \
            SAMD20_I2CDCDCWriteReadDriver(u16Address,pu8WriteBuf,u8WriteLen,pu8ReadBuf,u8ReadLen)

/***********************************************************************
Function:
    MCHP_PSF_HOOK_UPDI2C_DCDC_IsBusy()
Summary:
    Returns the busy status of I2C hardware interface of SOC used for communicating with I2C based DCDC Converter.
Description:
    This Hook is to check the busy status of the SOC's Hardware interface for I2C communication with I2C based DCDC converter. Define relevant function that has no arguments but a return type 
    UINT8 that indicates whether the I2C hardware interface is busy.
Conditions:
	This hook API may be called if I2C based DCDC converter is used to deliver power.
Return:
    UINT8 - Return TRUE if I2C hardware interface is busy or else return FALSE. 
Example:

    <code>
        #define MCHP_PSF_HOOK_UPDI2C_DCDC_IsBusy()      hw_i2c_isBusy()
        UINT8 hw_i2c_isBusy(void);
        UINT8 hw_i2c_isBusy(void)
        {
			//Get I2C busy status
            //Return TRUE if I2C interface is busy else FALSE
        }
    </code>
Remarks:
	None
**************************************************************************/
#define MCHP_PSF_HOOK_UPDI2C_DCDC_ISBUSY() SAMD20_I2CDCDCIsBusyDriver()

/* *****************************************************************************
Function:
	MCHP_PSF_HOOK_I2CDCDCAlertInit(byPortNum)
Summary:
    Configures GPIO of SOC as Input enables external interrupt for that GPIO.
Description:
    This Hook is used to configure a GPIO in input mode and enables external interrupt for that pin. 
	For each port, this hook can be used to assign alert pin of I2C based DCDC converter to a GPIO. 
	Whenever, an interrupt is fired from alert pin of I2C based DCDC converter, the SOC can call a callback function to execute the necessary fault handling operations.
Conditions:
	This hook API may be called if I2C based DCDC converter is used to deliver power.
Return:
	None.
Example:

    <code>
        #define MCHP_PSF_HOOK_I2CDCDCAlertInit()      hw_alertinit()
        void hw_alertinit(void);
        void hw_alertinit(void)
        {
			//Enable PIOx of SOC as input
			//Assign a callback api for interrupt that handles the fault condition
			//Enable external interrupt for PIOx        
        }
    </code>
Remarks:
	None
**************************************************************************/
#define MCHP_PSF_HOOK_I2CDCDCALERTINIT(byPortNum) SAMD20_I2CDCDCAlertInit(byPortNum)

// *****************************************************************************
#if (CONFIG_I2C_DCDC_TYPE == MPQ)

    #define MCHP_PSF_HOOK_I2CDCDC_CONTROLLER_INIT(byPortNum) MPQDCDC_Initialize(byPortNum)

    #define MCHP_PSF_HOOK_I2CDCDC_CONTROLLER_SET_POWER(u8PortNum, u8PDOIndex, u16VBUSVoltage, u16Current) \
        MPQDCDC_SetPortPower(u8PortNum, u8PDOIndex, u16VBUSVoltage, u16Current)

#endif //#if (CONFIG_I2C_DCDC_TYPE == MPQ)
#endif //#if (CONFIG_DCDC_CTRL == I2C_DC_DC_CONTROL_CONFIG)

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
#define MCHP_PSF_HOOK_HW_PDTIMER_INIT()   SAMD20_HWTimerInit() 

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
#define MCHP_PSF_CONFIG_16BIT_PDTIMER_COUNTER     1

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
    PSF requires a GPIO specific to each port for UPD350 interrupt detection via UPD350's IRQ_N lines.
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
#define MCHP_PSF_HOOK_UPD_IRQ_GPIO_INIT(u8PortNum)	SAMD20_UPD350AlertInit(u8PortNum)

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
    recommended to connect a single GPIO to the reset line of all UPD350s. User can also define a  
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
#define MCHP_PSF_HOOK_UPD_RESET_GPIO_INIT(u8PortNum) SAMD20_UPD350ResetGPIOInit(u8PortNum)              

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
    '0' via this hook. Define relevant function that has port number as argument without return type.
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
#define MCHP_PSF_HOOK_UPD_RESET_THRU_GPIO(u8PortNum)    SAMD20_ResetUPD350(u8PortNum) 

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
    This hook is called when PSF enters into a critical section. It must provide an implementation
    to disable the interrupts globally. This hook implementation must be very short, otherwise 
    resposne time may be delayed and cause timing issues/conflicts. Define relevant function that 
	has no arguments without return type.
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
    This hook is called when PSF exits from critical section. It must provide an 
    implementation to enable the interrupts globally. This function must be very short, otherwise 
    response time to the interrupt may be delayed and cause timing issues/conflicts. Define 
	relevant function that has no arguments without return type.
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
#define MCHP_PSF_HOOK_MEMCMP(pObj1, pObj2, iLength) SAMD20_MemCmp(pObj1, pObj2, iLength)                          

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
#define MCHP_PSF_HOOK_MEMCPY(pDest, pSrc, iLen) SAMD20_MemCpy(pDest, pSrc, iLen)

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
#define MCHP_PSF_STRUCT_PACKED_START   __attribute__((packed, aligned(4)))

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
    short, otherwise response time to the interrupt may be delayed and cause timing issues/conflicts.
	Passing of the Compliance test "TD.4.2.1" (Source Connect Sink) in "USB_Type_C_Functional_Test_Specification" 
	depends on the VBUS Discharge circuitry used. Typical VBUS Discharge time from any higher voltage 
	to 0V should be around 10ms.
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

/******************************************************************************************************
  Section:
           Boot Time Configuration
    
    
    
    
    * *************************************************************************** *
    * *************************************************************************** *
    * ********************************Globals******************************************* *
    * *********************************************************************************************** *
  Summary:
    User configurable boot time structure that contains port specific
    config and status parameters.
  Description:
    This structure contains the following configuration and status
    \parameters.
      * Type C Configuration
      * Default Source/Sink PDOs and PDO Count
      * New PDOs and PDO Count
      * Advertised PDOs and PDO Count
      * Partner PDO and PDO Count
      * Over-Voltage and Under-Voltage Threshold values
      * Maximum number of VBUS and VCONN Faults
      * Power Good Timer value
      * Mode and PIO No of Port Control pins
      * Port Connection Status parameters
      * Port Status Change parameters
      * Current negotiated Voltage and Current values
  Remarks:
    None                                                                                               
  ******************************************************************************************************/
typedef struct MCHP_PSF_STRUCT_PACKED_START _PortCfgStatus
{
    UINT32 u32CfgData;				/*<p>Bit  0   - Port Enable/Disable </p>
									  <p>     0   - Disabled </p>
									  <p>     1   - Enabled </p>
									 <p>Bits 3:1 - Port Role </p>
									 <p>     000 - Sink </p>
									 <p>     001 - Source </p>
									 <p>Bits 5:4 - Rp Selection </p>
									 <p>     00  - Disabled </p>
									 <p>	  01  - USB Power </p>
									 <p>	  10  - 1.5A </p>
									 <p>	  11  - 3.0A </p>
									 <p>Bits 8:6 - USB Data </p>
									 <p>     000 - No Data </p>
									 <p>     001 - USB2 </p>
									 <p>     010 - USB3.1 Gen1 </p>
									 <p>     011 - USB3.1 Gen2	</p>
									 <p>Bits 32:9 - Reserved</p>	*/								
    UINT32 u32FixedPDO[7];		    //Upto 7 fixed Source PDOs where Voltage is specified in mV and Current in mA.	 
    UINT32 u32NewPDO[7];		    //Upto 7 fixed New PDOs where Voltage is specified in mV and Current in mA.	 
    UINT32 u32AdvertisedPDO[7];	    //Upto 7 PDOs that are advertised to Port Partner.  
    UINT8 u8FixedPDOCnt;			//Number of Default Source PDOs supported 
    UINT8 u8AdvertisedPDOCnt; 		//Number of Advertised Source PDOs 
#if (CONFIG_DCDC_CTRL == PWRCTRL_DEFAULT_PSF_GPIO_CONFIG)    
    UINT8 u8VSELTruthTable[8];		/*This truth table corresponds to the assertion and de-assertion of the  VSEL[2:0] pins to drive the Voltage in GPIO PMPD module.  */
#endif 
    UINT8 u8OCSThresholdPercentage; //OCS Threshold 
    UINT8 u8FaultInDebounce;		/* Debounce timer value in terms of milliseconds for VBUS overcurrent fault
	conditions before reacting and entering fault recovery routine.It is applicable only for OCS detection via   FAULT_IN configured UPD350 pin. */
    UINT8 u8OVThresholdPercentage;  //Over Voltage threshold for VBUS_DET 
    UINT8 u8UVThresholdPercentage;	//Under Voltage threshold for VBUS_DET
    UINT8 u8VCONNOCS;				//<p>VCONN OCS Enable/Disable</p>
									//<p>0x00 - Disable</p>
									//<p>0x01 � Enable</p>
    UINT8 u8VCONNOCSDebounce;/*Debounce timer value in terms of milliseconds for VCONN overcurrent fault
	conditions before reacting and entering fault recovery routine. */
    UINT8 u8MaxFaultCntVBUS;//Maximum number of back-to-back faults allowed before permanent shut down of the port
    UINT8 u8MaxFaultCntVCONN;//Maximum number of back-to-back faults allowed before permanent shut down of the port
    UINT8 u8NewPDOSlct;/*<p>0 = Clear</p>
					   <p>1 = Set, when Set the below steps are done</p>
					   <p>1.Load New Source PDO</p>
					   <p>2.Advertise New PDOs to Port partner</p>
					   <p>3.Clear all New PDO registers</p>
					   <p>4.Clear this bit </p> */
    UINT8 u8NewPDOCnt; //The number of New PDOs Supported
    UINT16 u16PowerGoodTimer;/*Power good timer to determine if the power remains in good state without fault for 	the duration in ms. */
    UINT16 u16PortIntMask;/*<p>Bit 0: ATTACH_EVENT_C_MASK</p>
						 <p>Bit 1: DETACH_EVENT_C_MASK</p>
						 <p>Bit 2: AS_SOURCE_NEW_REQUEST_C_MASK</p>
						 <p>Bit 3: AS_SINK_NEW_PDOS_RECVD_C_MASK</p>
						 <p>Bit 4: AS_SINK_NEW_REQ_SENT_C_MASK</p>
						 <p>Bit 5: AS_SINK_LAST_REQ_ACCEPT_C_MASK</p>
						 <p>Bit 6: AS_SINK_LAST_REQ_REJECT_C_MASK</p>
						 <p>Bit 7: AS_SINK_LAST_SINK_PS_RDY_C_MASK</p>
						 <p>Bit 8: HARD_RESET_EVENT_C_MASK</p>
						 <p>Bit 9: VBUS_VCONN_FAULT_C_MASK</p>
						 <p>When,</p>
						 <p>0= Do not mask interrupt pin toggle on changes to this event.</p>
					     <p>1=Mask this event from generating interrupt pin toggle.</p> */

    UINT8 u8Pio_VBUS_EN;//Defines the UPD350 PIO number used for EN_VBUS pin
    UINT8 u8Mode_VBUS_EN;//Defines the PIO mode for EN_VBUS pin
    UINT8 u8Pio_FAULT_IN;//Defines the UPD350 PIO number used for FAULT_IN pin
    UINT8 u8Mode_FAULT_IN;//Defines the PIO mode for FAULT_IN pin
    UINT8 u8Pio_VBUS_DIS;//Defines the UPD350 PIO number used for VBUS_DIS pin
    UINT8 u8mode_VBUS_DIS;//Defines the PIO mode for VBUS_DIS pin
    UINT8 u8Pio_DC_DC_EN;//Defines the UPD350 PIO number used for DC_DC_EN pin
    UINT8 u8Mode_DC_DC_EN;//Defines the PIO mode for DC_DC_EN pin
#if (CONFIG_DCDC_CTRL == PWRCTRL_DEFAULT_PSF_GPIO_CONFIG)   
    UINT8 u8Pio_VSEL[3];//Defines the UPD350 PIO number used for VSEL0, VSEL1 and VSEL2 pins
    UINT8 u8Mode_VSEL[3];//Defines the PIO mode for VSEL0,VSEL1 and VSEL2 pins
#endif     
    UINT16 u16PortConnectStatus;/*<p> Define the connection status of the port </p>
							     <p>Bit 0: Attached </p> 
								 <p> 	 0 - Detached </p>
								 <p> 	 1 - Attached </p> 
								 <p>Bit 1: Orientation </p> 
								 <p>    0 - Unflipped </p> 
								 <p>    1 - Flipped </p> 
								 <p>Bit 2: Data Role </p>
								 <p>    0 - UFP </p>
								 <p>    1 - DFP </p>
							     <p>Bit 3: Power Role</p>
								 <p>    0 - Sink </p>
								 <p>    1 - Source</p> 
								 <p>Bit 4: As Source PD Contract Good </p>  
								 <p>    0 - As Source: USB-C Connection Only (No Request Made Yet) </p> 
								 <p>    1 - As Source; USB PD connection established, Power request has been made, 	accepted and PS_RDY message sent </p> 
								 <p>Bit 5: As Source RDO Accepted </p>  
								 <p>    0 - As Source: No RDO Accept message has been sent to last Request made by attached Sink or no Request has yet been made during connection  </p> 
								 <p>    1 - As Source: RDO Accept message has been sent to last Request made by attached Sink </p> 
								 <p>Bit 6: As Source RDO Rejected </p>  
								 <p>    0 - As source; No RDO reject message has been sent to last request made by attached Sink or no Request has yet been made during connection  </p> 
								 <p>    1 - As Source: RDO Reject message has been sent to last Request made by attached Sink </p> 
								 <p>Bit 7: As Sink Last Request Accept </p>  
								 <p>    0 - As Sink: Last RDO Request was not Accepted or no request has yet been made  </p> 
								 <p>    1 - As Sink: Last RDO Request was Accepted </p> 
								 <p>Bit 8: As Sink Last Request Reject </p>  
								 <p>    0 - As Sink: Last RDO Request was not Rejected or no request has yet been made  </p> 
								 <p>    1 - As Sink: Last RDO Request was Rejected </p>
								 <p>Bit 9: As Sink Last Request PS_RDY </p>  
								 <p>    As Sink: PS_RDY not yet received for last RDO request   </p> 
								 <p>    As Sink: PS_RDY received for last RDO request </p>
								 <p>Bit 10: VCONN Status </p>  
								 <p>    0 - Disabled   </p> 
								 <p>    1 - Enabled </p>
								 <p>Bit 11: Cable Reduced Source Capabilities </p>  
								 <p>    0 - The attached USB-C cable supports the locally-defined Source PDOs </p>  
								 <p>    1 - The attached USB-C cable does not support the locally defined Source PDOs (ex: 3A cable used w/ >3A Source Caps) </p>
								 <p>Bit 12: PD Bal Reduced Source Capabilities </p>  
								 <p>    0 - The advertised PDOs are equivalent to the default configured values</p>
								 <p>    1 - The advertised PDOs have been reduced from default configured values due to PB algorithm </p>
								<p>Bits 15:13: Reserved </p>  */
    UINT16 u16PortStatusChange;/*<p> Defines the port connection status change bits </p>
							     <p>Bit 0: ATTACH_EVENT_C </p> 
								 <p> 	 0 - Since the last read of this register, PSF has not experienced a USB-C attach </p>
								 <p> 	 1 - Since the last read of this register, PSF has experienced a USB-C attach </p> 
								 <p>Bit 1: DETACH_EVENT_C </p> 
								 <p>    0 - Since the last read of this register, PSF has not experienced a USB-C detach </p> 
								 <p>    1 - Since the last read of this register, PSF has experienced a USB-C detach </p> 
								 <p>Bit 2: AS_SOURCE_NEW_REQUEST_C </p>  
								 <p>    0 - As Source, since the last read of this register, PSF has not received any new PDO request from attached port partner </p>
								 <p>    1 - As Source: Received a new PDO request the attached Sink port partner, - As Sink: Received an updated set of Source capabilities form the attached Source port partner </p>
							     <p>Bit 3: AS_SINK_NEW_PDOS_RECVD_C </p>
								 <p>    0 - As Sink: Since the last read of this register, PSF has not received any CHANGED source capabilities </p>
								 <p>    1 - As Sink: Received an updated set of Source capabilities form the attached Source port partner </p> 
								 <p>Bit 4: AS_SINK_NEW_REQ_SENT_C </p>  
								 <p>    0 - As Sink: Since the last read of this register, PSF has not sent any additional Sink RDOs </p> 
								 <p>    1 - As Sink: Since the last read of this register, PSF has issued a new Sink RDO to the attached Source, This bit always to remain 0 when acting as a source </p> 
								 <p>Bit 5: AS_SINK_LAST_REQ_ACCEPT_C </p>  
								 <p>    0 - As Sink: Since the last read of this register, PSF has not received any new Request Accept messages from the attached Source  </p> 
								 <p>    1 - As Sink: Since the last read of this register, PSF has received a new Request Accept from the attached Source </p> 
								 <p>Bit 6: AS_SINK_LAST_REQ_REJECT_C </p>  
								 <p>    0 - As Sink: Since the last read of this register, PSF has not received any new Request Reject messages from the attached Source </p> 
								 <p>    1 - As Sink: Since the last read of this register, PSF has received a new Request Reject message from the attached Source </p> 
								 <p>Bit 7: AS_SINK_LAST_SINK_PS_RDY_C </p>  
								 <p>    0 - As Sink: Since the last read of this register, PSF has not received any PS_RDY messages from the attached Source  </p> 
								 <p>    1 - As Sink: Since the last read of this register, PSF has received a PS_RDY message from the attached Source </p> 
								 <p>Bit 8: HARD_RESET_EVENT_C </p>  
								 <p>    0 - Since the last read of this register, PSF has not experienced a USB PD Hard_Reset  </p> 
								 <p>    1 - Since the last read of this register, PSF has experienced a USB PD Hard_Reset </p>
								 <p>Bit 9: PIN_RESET_EVENT_C </p>  
								 <p>    Since the last read of this register, PSF has not been reset via POR or pin  </p> 
								 <p>    Since the last read of this register, PSF has been reset via POR or pin </p>
								 <p>Bit 10: VBUS_VCONN_FAULT_C </p>  
								 <p>    0 - Since the last read of this register, no faults have been detected   </p> 
								 <p>    1 - Since the last read of this register, 1 or more faults have been detected </p>
								 <p>Bits 15:11: Reserved </p> */
    UINT16 u16AllocatedPower;//Allocated Power for the Port PD contract in 0.25W steps
    UINT16 u16NegoVoltage;/*<p> Negotiated Voltage from the Port Bits 19:10 from the RDO
								Voltage is in 50mV steps</p> 
						  <p>	0x00 = No Contract </p>
						  <p>  0x064 = 5V</p>
						  <p>  0x0B4 = 9 V</p>
						  <p>  0x12C = 15V</p>
						  <p>  0x190 = 20V</p>
						  <p>  0x3FF = 51.15V</p> */
    UINT16 u16NegoCurrent;/*<p> Negotiated Current from the Port Bits 9: 0 from the RDO, Ampere is in 10mA steps</p>
						 <p> 0x0000 = No Contract </p>
						 <p> 0x012C = 3A </p>
						 <p> 0x01F4 = 5A </p>
						 <p> 0x03FF = 10.24A </p> */
    UINT16 u16PortIOStatus;/*<p> Defines the Port IO status </p> 
									<p>Bit  0   - EN_DC_DC_STATUS </p>
									<p>Bit  1   - VSEL0_STATUS </p>
									<p>Bit  2   - VSEL1_STATUS </p>
									<p>Bit  3   - VSEL2_STATUS </p>
									<p>Bit  4   - EN_VBUS_STATUS </p>
									<p>Bit  5   - VBUS_DIS_STATUS </p>
									<p>Bit  6   - EN_SINK_STATUS </p>
									<p>Bit  7   - 1.5_IND </p>
									<p>Bit  8   - 3.0_IND </p>
									<p>Bit  9   - PS_RDY_RECVD </p>
									<p>Bit  10   - CAP_MISMATCH </p>
									<p>Bits 15:11  - Reserved </p>
									<p>	Where the logic state, </p>
									<p> 0 = Asserted </p>
									<p> 1 = De-asserted>/p> */
    UINT16 u16PortFaultChangeBits;/*<p> Defines the fault change bits </p> 
								    <p>Bit  0   - FLT_VBUS_C </p>
									<p>Bit  1   - FLT_VCONN_C </p> */
    UINT32 u32PartnerPDO[7];/*Partner PDOs Configuration: Upto 7 fixed Source PDOs where Voltage is specified in mV and Current is specified in mA */
    UINT8 u8PartnerPDOCnt; //Number of Partner PDOs 
    UINT32 u32RDO;/*Complete raw RDO Data as Sent/Requested by connected port partner, Will be blank of no RDO has  been received */
} MCHP_PSF_STRUCT_PACKED_END PORT_CFG_STATUS, *PPORT_CFG_STATUS;

 /**********************************************************************
   Summary:
     User configurable boot time structure that contains port specific power 
	 balancing Config parameters.
   Description:
     This structure contains the following Power Balancing configuration
     parameters.
       * Maximum Port Power Bank A
       * Maximum Port Power Bank B
       * Maximum Port Power Bank C
       * Maximum Port Current
       * PB Enable
       * PB Port Priority
   Remarks:
     None                                                               
   **********************************************************************/
#if (TRUE == INCLUDE_POWER_BALANCING) 

typedef struct MCHP_PSF_STRUCT_PACKED_START _PBPortCfgStatus
{
    UINT16 u16MaxPrtPwrBankA; /*<p>Maximum Port Power Bank A in 0.25W steps</p>
								<p>Unit/LSB = 0.25W</p>
								<p>0x0000 = 0W (invalid)</p>
								<p>0x0001 = 0.25W</p>
								<p>0x00F0 = 60W</p>
								<p>0x0190 = 100W</p>
								<p>0xFFFF = 16381.25W</p>
								<p>Note: a setting of 0x191-0xFFF is invalid </p>*/
    UINT16 u16MaxPrtPwrBankB; /*<p>Maximum Port Power Bank B in 0.25W steps</p>
								<p>Unit/LSB = 0.25W</p>
								<p>0x0000 = 0W (invalid)</p>
								<p>0x0001 = 0.25W</p>
								<p>0x00F0 = 60W</p>
								<p>0x0190 = 100W</p>
								<p>0xFFFF = 16381.25W</p>
								<p>Note: a setting of 0x191-0xFFF is invalid</p> */
    UINT16 u16MaxPrtPwrBankC; /* <p>Maximum Port Power Bank C in 0.25W steps</p>
								<p>Unit/LSB = 0.25W</p>
								<p>0x0000 = 0W (invalid)</p>
								<p>0x0001 = 0.25W</p>
								<p>0x00F0 = 60W</p>
								<p>0x0190 = 100W</p>
								<p>0xFFFF = 16381.25W</p>
								<p>Note: a setting of 0x191-0xFFF is invalid</p> */
    UINT16 u16MaxPrtCurrent; /* <p>Maximum allowable current for ports in 10mA steps</p>
								<p>0x0032 = 0.5A</p>
								<p>0x012C = 3A</p>
								<p>0x01F4 = 5A</p>
								<p>Values above 5A (0x01F5 - 0x0FFF) are not supported</p> */
    UINT8 u8PBEnablePriority; /* <p>Defines PB Enable/Disable per port and also the priority for the port</p>
								<p>Bit 0 : PB Enable/Disable </p>
								<p>0 = Disable</p>
								<p>1 = Enable</p>
								<p>Bit 3-1: Select priority</p>
								<p>000b is the highest priority </p>*/
} MCHP_PSF_STRUCT_PACKED_END PB_PORT_CFG_STATUS, *PPB_PORT_CFG_STATUS;

#endif 
 /**********************************************************************
   Summary:
     User configurable boot time structure that contains port specific 
     PPS config parameters.
   Description:
     This structure contains the following PPS configuration
     parameters.
       * PPS Enable 
       * PPS APDOs 
   Remarks:
     None                                                               
   **********************************************************************/
#if (TRUE == INCLUDE_PD_SOURCE_PPS)   

typedef struct MCHP_PSF_STRUCT_PACKED_START _PPSPortCfgStatus
{
    UINT8 u8PPSEnable; /* <p> PPS Enable/Disable </p>
						  <p> 0x00 = Disable </p>
						  <p> 0x01 Enable </p> */
    UINT32 u32PPSApdo[3];  /* Defines the PPS APDOs */
} MCHP_PSF_STRUCT_PACKED_END PPS_PORT_CFG_STATUS, *PPPS_PORT_CFG_STATUS;

#endif 
 /**********************************************************************
   Summary:
     User configurable boot time structure that contains the global 
	 Config and Status parameters. 
   Description:
     This structure contains the following global configuration and 
	 status parameters. 
     parameters.
       * Power Delivery IDs 
       * PORT_CFG_STATUS structure 
       * PB Enable for the system 
	   * PB Algorithm 
	   * Power Throttle Configuration 
	   * Total System Power of Banks A,B & C 
	   * Guaranteed Minimum Power of Banks A,B & C 
       * PB_PORT_CFG_STATUS structure
       * PPS_PORT_CFG_STATUS structure
   Remarks:
     None                                                               
   **********************************************************************/
typedef struct MCHP_PSF_STRUCT_PACKED_START _GlobalCfgStatusData 
{
    UINT8 u8MinorVersion;  /* Structure Minor version */ 
    UINT8 u8MajorVersion;  /* Structure Major version */
    UINT8 u8HWVersion;	   /* HW Version */ 
    UINT8 u8SiVersion;     /* Silicon Version */ 
    UINT8 u8ManfString[8]; /* Manufacturer String */
    UINT8 u8PSFMajorVersion; /* PSF Stack Major Version */ 
    UINT8 u8PSFMinorVersion; /* PSF Stack Minor Version */ 
    UINT16 u16Reserved;     /* Reserved variable to avoid unaligned addr */
    UINT16 u16ProducdID;	/* Product ID */
    UINT16 u16VendorID;		/* Vendor ID */ 
    UINT16 u16ProductTypeVDO; /* Product Type VDO */ 
    UINT16 u16ProductVDO; /* Product VDO*/ 
    UINT16 u16CertStatVDO; /* Cert Stat VDO */  
    UINT16 u16IDHeaderVDO; /* ID Header VDO */ 
    
    PORT_CFG_STATUS sPerPortData[CONFIG_PD_PORT_COUNT]; /* Per Port Config and Status */ 
#if (TRUE == INCLUDE_POWER_BALANCING)
    /* Power Balancing Configurations */
    UINT8 u8PBEnableSelect;		/* <p> Bits 3-0: Selection of Power Balancing Algorithm </p>
									<p> 0000 = First Come First Serve </p>
									<p> 0001 = Last Plugged Gets Priority </p>
									<p> 0010 = Port 1 Fixed Priority </p>
									<p> 0011-1111 = Reserved </p>

									<p> Bit 4:PD Power balancing Enable/Disable </p>
									<p> 0 = PD Balancing is Enabled</p> 
									<p> 1= PD Balancing is Disabled </p>*/ 
#endif 
#if ((TRUE == INCLUDE_POWER_BALANCING) || (TRUE == INCLUDE_POWER_THROTTLING))    
    UINT8 u8PwrThrottleCfg;	 /*	<p>Define the currently selected bank </p>
								<p>	0x00 = Bank A</p>
								<p>	0x00 = Bank B</p>
								<p>	0x00 = Bank C</p>
								<p>	0x00 = Shutdown</p> */
    UINT16 u16SystemPowerBankA; /* <p>Total System power Bank A Each Unit is 0.25W</p>
								<p>0x0000 = 0W (invalid)</p>
								<p>0x0001 = 0.25W</p>
								<p>0x01E0 = 120W</p>
								<p>0x0320 = 200W</p>
								<p>0xFFFF = 16381.25W</p>
								<p>Note: a setting of 0x0321-0xFFFF is invalid</p> */ 
    UINT16 u16MinPowerBankA;   /* <p>Guaranteed minimum power Bank A Each Unit is 0.25W</p>
								<p>0x0000 = 0W (invalid)</p>
								<p>0x0001 = 0.25W</p>
								<p>0x003C = 15W</p>
								<p>0x0190 = 100W</p>
								<p>0xFFFF = 16381.25W</p>
								<p>Note: a setting of 0x191-0xFFF is invalid</p> */
    UINT16 u16SystemPowerBankB; /* <p>Total System power Bank B Each Unit is 0.25W</p>
								<p>0x0000 = 0W (invalid) </p>
								<p>0x0001 = 0.25W</p>
								<p>0x01E0 = 120W</p>
								<p>0x0320 = 200W</p>
								<p>0xFFFF = 16381.25W</p>
								<p>Note: a setting of 0x0321-0xFFFF is invalid</p> */
    UINT16 u16MinPowerBankB;   /* <p>Guaranteed minimum power Bank B Each Unit is 0.25W</p>
								<p>0x0000 = 0W (invalid)</p>
								<p>0x0001 = 0.25W</p>
								<p>0x003C = 15W</p>
								<p>0x0190 = 100W</p>
								<p>0xFFFF = 16381.25W</p>
								<p>Note: a setting of 0x191-0xFFF is invalid</p> */
    UINT16 u16SystemPowerBankC; /* <p>Total System power Bank C Each Unit is 0.25W</p>
								<p>0x0000 = 0W (invalid)</p>
								<p>0x0001 = 0.25W</p>
								<p>0x01E0 = 120W</p>
								<p>0x0320 = 200W</p>
								<p>0xFFFF = 16381.25W</p>
								<p>Note: a setting of 0x0321-0xFFFF is invalid</p> */
    UINT16 u16MinPowerBankC;    /* <p>Guaranteed minimum power Bank C Each Unit is 0.25W</p>
								<p>0x0000 = 0W (invalid)</p>
								<p>0x0001 = 0.25W</p>
								<p>0x003C = 15W</p>
								<p>0x0190 = 100W</p>
								<p>0xFFFF = 16381.25W</p>
								<p>Note: a setting of 0x191-0xFFF is invalid</p> */
#endif
#if (TRUE == INCLUDE_POWER_BALANCING)    
    UINT16 u16SharedPowerCapacity; /* The currently available shared power
                                    capacity from which power is allocated to ports 
                                    that are not yet in a valid negotiated contract, 
                                    The shared power capacity is dynamically adjusted 
                                    as ports are connected and disconnected
								<p>Each Unit is 0.25W</p>
								<p>0x0000 = 0W (invalid)</p>
								<p>0x0001 = 0.25W</p>
								<p>0xFFFF = 16381.25W </p>*/
    PB_PORT_CFG_STATUS sPBPerPortData[CONFIG_PD_PORT_COUNT];	/* Per Port PB Config and Status */
#endif 
    
    /* PPS Configurations */
#if (TRUE == INCLUDE_PD_SOURCE_PPS)
    PPS_PORT_CFG_STATUS sPPSPerPortData[CONFIG_PD_PORT_COUNT];  /* Per Port PPS Config and Status */
#endif
}MCHP_PSF_STRUCT_PACKED_END GLOBAL_CFG_STATUS_DATA, * PGLOBAL_CFG_STATUS_DATA;

extern GLOBAL_CFG_STATUS_DATA gasCfgStatusData;   
   
/*********************************************************************************************
  Function:
        MCHP_PSF_HOOK_BOOT_TIME_CONFIG(gasCfgStatusData)
  Summary:
    Updates the global and per port Configuration parameters.
  Description:
    This function is called to update the configuration parameters of
    Type-C, PD, Power Balancing, Power throttling and PPS. This API must 
	have a input parameter of GLOBAL_CFG_STATUS_DATA prototype (Structure
	Pointer to GLOBAL_CFG_STATUS_DATA).
  Conditions:
    None.
  Input:
    gasCfgStatusData -  Holds the structure pointer of the structure
						GLOBAL_CFG_STATUS_DATA
  Return:
    None.
  Example:
    <code>
    \#define  MCHP_PSF_HOOK_BOOT_TIME_CONFIG(_GlobalCfgStatusData)  STRAPS_PowerRole_Set(_GlobalCfgStatusData)
    void STRAPS_PowerRole_Set(GLOBAL_CFG_STATUS_DATA *PGLOBAL_CFG_STATUS_DATA);
    void STRAPS_PowerRole_Set(GLOBAL_CFG_STATUS_DATA *PGLOBAL_CFG_STATUS_DATA)
    {
        // Configure Cfg variables for Source or Sink
    }
    </code>
  Remarks:
    User definition of this Hook function is optional                                         
  *********************************************************************************************/
#define  MCHP_PSF_HOOK_BOOT_TIME_CONFIG(gasCfgStatusData)  	PSF_LoadConfig()

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

#ifdef CONFIG_HOOK_DEBUG_MSG

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
#define MCHP_PSF_HOOK_DEBUG_INIT()   SAMD20_UART_Initialisation()

/***********************************************************************
Function:
    MCHP_PSF_HOOK_PRINT_CHAR(byData)            
Summary:
    Outputs a character via UART interface
Description:
    This hook is can be called to output a character via UART interface to help the user in debugging. byData is of type char.
    Define relevant function to print a character via UART terminal with argument of type char and no return type. 
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

#define MCHP_PSF_HOOK_PRINT_CHAR(byData)    SAMD20_UART_Write_Char(byData);


/***********************************************************************
Function:
    MCHP_PSF_HOOK_PRINT_INTEGER(dwWriteInt, byWidth)            
Summary:
    Outputs an integer via UART interface
Description:
    This hook is can be called to output an integer via UART interface to help the user in debugging. 
    The size of integer is specified in byWidth.
    dwWriteInt is of type unsigned long. byWidth is of type unsigned char.
    Define relevant function to print an integer via UART terminal with arguments of type unsigned long and unsigned char and no return type. 

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
#define MCHP_PSF_HOOK_PRINT_INTEGER(dwWriteInt, byWidth)    SAMD20_UART_Write_Int(dwWriteInt, byWidth);


/***********************************************************************
Function:
    MCHP_PSF_HOOK_PRINT_TRACE(pbyMessage)        
Summary:
    Outputs a string via UART interface
Description:
    This hook is can be called to output a string via UART interface to help the user in debugging. pbyMessage is of type char *.
    Define relevant function to print a string via UART terminal with argument of type char* and no return type. 

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
#define MCHP_PSF_HOOK_PRINT_TRACE(pbyMessage)  SAMD20_UART_Write_String(pbyMessage);

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
    This API is to run the PSF state machine.For single threaded environment, it should be called 
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
    configured for 48MHz environment, MchpPSF_RUN has to be called for
    every 2ms for Successful 2-Port Source only operation.                
  *************************************************************************/
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
 
    <b> eMCHP_PSF_PD_CONTRACT_NEGOTIATED</b>: PSF notifies when PD contract is
    established with the Port partner.
   
    <b> eMCHP_PSF_GET_SINK_CAPS_RCVD</b>: This event is used by PSF to notify DPM when 
    Sink capabilities has been received from Port Partner in response to the Get_Sink_Caps
    message initiated by PSF on request from DPM. 
    
    <b> eMCHP_PSF_GET_SINK_CAPS_NOT_RCVD</b>: This event is used by PSF to notify DPM when
    Sink capabilities has not been received from Port Partner within tSenderResponseTimer
    as a response to the Get_Sink_Caps message initiated by PSF on request from DPM.   
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
eMCHP_PSF_VBUS_PWR_FAULT,            // VBUS Power Fault has occurred
eMCHP_PSF_PD_CONTRACT_NEGOTIATED,   // PD Contract established with port partner
eMCHP_PSF_GET_SINK_CAPS_RCVD,        // Sink Caps received from Port Partner
eMCHP_PSF_GET_SINK_CAPS_NOT_RCVD     // Sink Caps not received from Port Partner
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
#define MCHP_PSF_NOTIFY_CALL_BACK(u8PortNum, ePSFNotification)   PDStack_Events(u8PortNum, ePSFNotification)
 
/*******************************************************************************
Function:
    MCHP_PSF_HOOK_SET_MCU_IDLE()
Summary:
    Configures the SoC to enter Low power mode.
Description:
    This hook is called when eMCHP_PSF_UPDS_IN_IDLE event notification is posted 
    by PSF. A suitable function that puts SoC to low power mode need to be 
    implemented in this hook.
Conditions:
    None.
Return:
    None.
Example:
    <code>
        #define MCHP_PSF_HOOK_SET_MCU_IDLE()   SAMD20_SetMCUIdle()
    </code>
Remarks:
User definition of this Hook function is optional.                          
*******************************************************************************/  
#define MCHP_PSF_HOOK_SET_MCU_IDLE          SAMD20_SetMCUIdle()

#endif /*_PSF_API_HOOK_H_*/

