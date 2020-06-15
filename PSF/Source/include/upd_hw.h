/*******************************************************************************
 UPD Hardware Abstraction layer file

  Company:
    Microchip Technology Inc.

  File Name:
    upd_hw.h

  Description:
    This header file contains the functions for UPD350 hardware interface access
    for GPIO, Register access, PIO override
 *******************************************************************************/
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
*******************************************************************************/

#ifndef _UPD_HW_H_
#define _UPD_HW_H_

// *****************************************************************************
// *****************************************************************************
// Section: Constants
// *****************************************************************************
// *****************************************************************************

/***********************************************************************************************/
typedef enum
{
    eUPD_PIO0 = 0x00U,
    eUPD_PIO1,
    eUPD_PIO2,
    eUPD_PIO3,
    eUPD_PIO4,
    eUPD_PIO5,
    eUPD_PIO6,
    eUPD_PIO7,
    eUPD_PIO8,
    eUPD_PIO9,
    eUPD_PIO10,
    eUPD_PIO11,
    eUPD_PIO12,
    eUPD_PIO13,
    eUPD_PIO14,
    eUPD_PIO15,
    eUPD_PIO_UN_DEF = 0xFFU
}eUPD_PIO_NUM_TYPE;

/*Defines used for 'u8PIONum' argument in UPD_GPIOEnableDisable, UPD_GPIOSetDirection, 
    UPD_GPIOSetBufferType functions*/

/* Defines used for 'u8EnableDisable' argument in UPD_GPIOEnableDisable function */
#define UPD_ENABLE_GPIO    0
#define UPD_DISABLE_GPIO   1

/* Defines used for 'u8Direction' argument in UPD_GPIOSetDirection function */
#define UPD_GPIO_SETDIR_OUTPUT  0
#define UPD_GPIO_SETDIR_INPUT   1

/* Defines used for 'u8BufferType' argument in UPD_GPIOSetBufferType function */ 
#define UPD_GPIO_SETBUF_PUSHPULL    0
#define UPD_GPIO_SETBUF_OPENDRAIN   1

/* Defines used for 'u8SetClear' argument in UPD_GPIOSetClearOutput function */
#define UPD_GPIO_SET    0
#define UPD_GPIO_CLEAR   1

/* Defines used for 'u8IntrType' */
#define UPD_GPIO_CLEAR_INTR			0
/* use UPD_CFG_PIO_RISING_ALERT, UPD_CFG_PIO_FALLING_ALERT defines for rising & falling pio intr*/

/*****************************************************************************************************/

/* UPD SPI Opcodes definition */


#define CONFIG_UPD350_SPI   1
#define CONFIG_UPD350_I2C   0

#if (CONFIG_UPD350_SPI == CONFIG_DEFINE_UPD350_HW_INTF_SEL)

#define UPD_SPI_WRITE_OPCODE                        0x02
#define UPD_SPI_READ_OPCODE                         0x0B
#define UPD_SPI_DUMMY_BYTE                          0x00
#define UPD_SPI_WRITE_CMD_LEN                       3
#define UPD_SPI_READ_CMD_LEN                        4

#else

#define UPD_I2C_REG_CMD_LEN                       2
#define UPD_I2C_MAX_BYTE_WRITE                    38

#endif

/*****************************************************************************************************/
/* UPD Address Base*/
#define UPD_SYSTEM_CSR_BASE_ADDR	                0x0000
#define UPD_CLK_PWR_MANAGE_CSR_BASE_ADDR            0x1000

/**************************************************************************************************/

/* System Control and Status Register */
#define UPD_VID          	        UPD_SYSTEM_CSR_BASE_ADDR + 0x04
#define UPD_SPI_TEST      	        UPD_SYSTEM_CSR_BASE_ADDR + 0x0E
#define UPD_HW_CTL          	    UPD_SYSTEM_CSR_BASE_ADDR + 0x18
#define UPD_PIO_STS				    UPD_SYSTEM_CSR_BASE_ADDR + 0x20
#define UPD_PIO_STS_LOW		        UPD_SYSTEM_CSR_BASE_ADDR + 0x20
#define UPD_PIO_STS_HIGH	        UPD_SYSTEM_CSR_BASE_ADDR + 0x21
#define UPD_PIO_INT_STS				UPD_SYSTEM_CSR_BASE_ADDR + 0x22
#define UPD_PIO_INT_EN			    UPD_SYSTEM_CSR_BASE_ADDR + 0x24

/* Configure PIOx Registers*/
#define UPD_CFG_PIO_BASE			UPD_SYSTEM_CSR_BASE_ADDR + 0x30                                                  
#define UPD_CFG_PIO0			    UPD_SYSTEM_CSR_BASE_ADDR + 0x30
#define UPD_CFG_PIO1			    UPD_SYSTEM_CSR_BASE_ADDR + 0x31
#define UPD_CFG_PIO2			    UPD_SYSTEM_CSR_BASE_ADDR + 0x32
#define UPD_CFG_PIO3			    UPD_SYSTEM_CSR_BASE_ADDR + 0x33
#define UPD_CFG_PIO4			    UPD_SYSTEM_CSR_BASE_ADDR + 0x34
#define UPD_CFG_PIO5			    UPD_SYSTEM_CSR_BASE_ADDR + 0x35
#define UPD_CFG_PIO6			    UPD_SYSTEM_CSR_BASE_ADDR + 0x36
#define UPD_CFG_PIO7			    UPD_SYSTEM_CSR_BASE_ADDR + 0x37
#define UPD_CFG_PIO8			    UPD_SYSTEM_CSR_BASE_ADDR + 0x38
#define UPD_CFG_PIO9			    UPD_SYSTEM_CSR_BASE_ADDR + 0x39
#define UPD_CFG_PIO10			    UPD_SYSTEM_CSR_BASE_ADDR + 0x3A
#define UPD_CFG_PIO11			    UPD_SYSTEM_CSR_BASE_ADDR + 0x3B
#define UPD_CFG_PIO12			    UPD_SYSTEM_CSR_BASE_ADDR + 0x3C
#define UPD_CFG_PIO13			    UPD_SYSTEM_CSR_BASE_ADDR + 0x3D
#define UPD_CFG_PIO14			    UPD_SYSTEM_CSR_BASE_ADDR + 0x3E
#define UPD_CFG_PIO15			    UPD_SYSTEM_CSR_BASE_ADDR + 0x3F

#define UPD_CFG_PIO_REGADDR(PIONum) (UPD_CFG_PIO_BASE + PIONum)

/* PIO override register*/
#define UPD_PIO_MON_VAL				UPD_SYSTEM_CSR_BASE_ADDR + 0x54
#define UPD_PIO_OVR_EN				UPD_SYSTEM_CSR_BASE_ADDR + 0x55
#define UPD_PIO_OVR_STS				UPD_SYSTEM_CSR_BASE_ADDR + 0x56
#define UPD_PIO_OVR_INT_STS			UPD_SYSTEM_CSR_BASE_ADDR + 0x57
#define UPD_PIO_OVR_INT_EN			UPD_SYSTEM_CSR_BASE_ADDR + 0x58
												  											  
#define UPD_CFG_PIO_OUT_LOW		    UPD_SYSTEM_CSR_BASE_ADDR + 0x59
#define UPD_CFG_PIO_OUT_HIGH	    UPD_SYSTEM_CSR_BASE_ADDR + 0x5A
#define UPD_CFG_PIO_OUT_UPD_LOW	    UPD_SYSTEM_CSR_BASE_ADDR + 0x5B
#define UPD_CFG_PIO_OUT_UPD_HIGH	UPD_SYSTEM_CSR_BASE_ADDR + 0x5C
												  
#define UPD_PIO_OVR_OUT				UPD_SYSTEM_CSR_BASE_ADDR + 0x68
#define UPD_PIO_OVR_DIR				UPD_SYSTEM_CSR_BASE_ADDR + 0x6A
                                                  
#define UPD_PIO_DEBOUNCE_10MS_COUNT UPD_SYSTEM_CSR_BASE_ADDR + 0x6C
#define UPD_PIO_DEBOUNCE_1MS_COUNT  UPD_SYSTEM_CSR_BASE_ADDR + 0x6D
#define UPD_PIO_DEBOUNCE_1US_COUNT  UPD_SYSTEM_CSR_BASE_ADDR + 0x6E
#define UPD_PIO_DEBOUNCE_EN         UPD_SYSTEM_CSR_BASE_ADDR + 0x70                                                  
                                                  
                                                 										  
#define UPD_TRIM_ZTC			    UPD_SYSTEM_CSR_BASE_ADDR + 0x98
#define UPD_TRIM_ZTC_BYTE_3		    UPD_TRIM_ZTC + 0x02
												  
#define UPD_PIO_OVR0_SRC_SEL		UPD_SYSTEM_CSR_BASE_ADDR + 0xC0							
#define UPD_PIO_OVR1_SRC_SEL		UPD_SYSTEM_CSR_BASE_ADDR + 0xC1
#define UPD_PIO_OVR2_SRC_SEL		UPD_SYSTEM_CSR_BASE_ADDR + 0xC2													  
#define UPD_PIO_OVR3_SRC_SEL		UPD_SYSTEM_CSR_BASE_ADDR + 0xC3													  
#define UPD_PIO_OVR4_SRC_SEL		UPD_SYSTEM_CSR_BASE_ADDR + 0xC4													  
#define UPD_PIO_OVR5_SRC_SEL		UPD_SYSTEM_CSR_BASE_ADDR + 0xC5													  
												  
#define UPD_PIO_OVR0_OUT_EN			UPD_SYSTEM_CSR_BASE_ADDR + 0xD0								
#define UPD_PIO_OVR1_OUT_EN			UPD_SYSTEM_CSR_BASE_ADDR + 0xD2
#define UPD_PIO_OVR2_OUT_EN			UPD_SYSTEM_CSR_BASE_ADDR + 0xD4													  
#define UPD_PIO_OVR3_OUT_EN			UPD_SYSTEM_CSR_BASE_ADDR + 0xD6													  
#define UPD_PIO_OVR4_OUT_EN			UPD_SYSTEM_CSR_BASE_ADDR + 0xD8													  
#define UPD_PIO_OVR5_OUT_EN			UPD_SYSTEM_CSR_BASE_ADDR + 0xDA		
/**************************************************************************************************/
												  
/*Defines used for Configuring PIO register values */
#define UPD_CFG_PIO_GPIO_ENABLE  		0x1
#define UPD_CFG_PIO_DIRECTION    	    0x2
#define UPD_CFG_PIO_BUFFER_TYPE         0x4
#define UPD_CFG_PIO_DATAOUTPUT		    0x8
#define UPD_CFG_PIO_RISING_ALERT		0x10
#define UPD_CFG_PIO_FALLING_ALERT		0x20
#define UPD_CFG_PIO_PULL_DOWN_ENABLE	BIT(6)
#define UPD_CFG_PIO_PULL_UP_ENABLE		BIT(7)

typedef enum
{
    UPD_GPIO_ASSERT = 0x00U,
    UPD_GPIO_DE_ASSERT = UPD_CFG_PIO_DATAOUTPUT
}eUPD_GPIO_DRIVE_TYPE;

/*******************************************************************************/
/* Defines for PIO Overridex Source Select Register (PIO_OVRx_SRC_SEL)*/
/* defines for Override Select[4:0]*/
#define UPD_PIO_OVR_SRC_SEL_VBUS_THR	0x11
/* define for VBUS Threshold Select [7:5] field*/											  
#define UPD_PIO_OVR_VBUS_THR_SEL_POS		5

#define UPD_PIO_OVR_VSAFE0V_THR_MATCH		(0 << UPD_PIO_OVR_VBUS_THR_SEL_POS)
#define UPD_PIO_OVR_VBUS0_THR_MATCH			(1 << UPD_PIO_OVR_VBUS_THR_SEL_POS)
#define UPD_PIO_OVR_VBUS1_THR_MATCH			(2 << UPD_PIO_OVR_VBUS_THR_SEL_POS)	
#define UPD_PIO_OVR_VBUS2_THR_MATCH			(3 << UPD_PIO_OVR_VBUS_THR_SEL_POS)													  
#define UPD_PIO_OVR_VBUS3_THR_MATCH			(4 << UPD_PIO_OVR_VBUS_THR_SEL_POS)													  
#define UPD_PIO_OVR_VBUS4_THR_MATCH			(5 << UPD_PIO_OVR_VBUS_THR_SEL_POS)
												  
/* Override Selection bit define*/
#define UPD_PIO_OVR_0						BIT(0)										  
#define UPD_PIO_OVR_1						BIT(1)
#define UPD_PIO_OVR_2						BIT(2)	
#define UPD_PIO_OVR_3						BIT(3)	
#define UPD_PIO_OVR_4						BIT(4)
#define UPD_PIO_OVR_5						BIT(5)		
                                                  
#define UPD_MCU_ACTIVE                      0
#define UPD_MCU_IDLE                        1 
                                                  
#define UPD_PIO_DEBOUNCE_FIELD_WIDTH        2

/*Debounce Enable type*/
#define UPD_PIO_DEBOUNCE_DISABLE            0                                                  
#define UPD_PIO_DEBOUNCE_CNT_TYP_1_US       1
#define UPD_PIO_DEBOUNCE_CNT_TYP_1_MS       2
#define UPD_PIO_DEBOUNCE_CNT_TYP_10_MS      3
#define UPD_PIO_DEBOUNCE_DISABLE_MASK       3                                                  
/*****************************************************************************************************/

/* Mask Macros of System Control and Status Register */

/* TRIM_ZTC */
#define UPD_PM_V2I_ENABLE		    	BIT(0)

/* TYPEC_HW_CTL Register Bits */
#define UPD_SRESET						BIT(0)
#define UPD_DEV_READY					BIT(2)
#define UPD_HW_CTL_REG_DEV_RDY_MSK  	BIT(2)

/*****************************************************************************************************/

/* Clock and Power Management control and status register */ 
#define UPD_CLK_CTL				    	UPD_CLK_PWR_MANAGE_CSR_BASE_ADDR + 0x00
#define UPD_CC_SAMP_CLK             	UPD_CLK_PWR_MANAGE_CSR_BASE_ADDR + 0x06
#define UPD_VBUS_SAMP_CLK		    	UPD_CLK_PWR_MANAGE_CSR_BASE_ADDR + 0x07

/*Defines for UPD_CLK_CTL register*/
#define UPD_CLK_CTL_ZTC_CUR_REF_EN      BIT(3)
#define UPD_CLK_CTL_20_KHZ_OSC_EN       BIT(0)

/*Defines for UPD_CC_SAMP_CLK register*/
#define UPD_CC_CLK_20_KHZ               0
#define UPD_CC_CLK_48_KHZ               BIT(3)
#define UPD_CC_SAMP_GEN_10_KS           0

/*Defines for UPD_CC_SAMP_CLK register*/
#define UPD_VBUS_CLK_20_KHZ             0
#define UPD_VBUS_CLK_48_KHZ             BIT(3)
#define UPD_VBUS_SAMP_GEN_10_KS         0
                                                  
/*Vendor ID & Product ID information*/
#define UPD_VID_LSB                     0x24
#define UPD_VID_MSB                     0x04  
                                                  
#define UPD_PID_LSB                     0x50   
#define UPD_PID_MSB                     0x03
                                                  
#define UPD_SPI_TEST_VAL                0x02   
												  
/*****************************************************************************************************/

/* UPD idle state*/
#define UPD_STATE_ACTIVE					0x01
#define UPD_STATE_IDLE						0x02
#define UPD_STATE_STARTED_IDLE_TIMER		0x03
                                                  
#define UPD_48MHZ_OSC_ENABLE	BIT(1)
#define UPD_RING_OSC_ENABLE		BIT(2)
#define UPD_SYS_CLK_ENABLE		BIT(6)

/*Status of ports whether disabled or enabled*/
#define UPD_PORT_DISABLED        0x00
#define UPD_PORT_ENABLED         0x01

// *****************************************************************************
// *****************************************************************************
// Section: Interface Routines
// *****************************************************************************
// *****************************************************************************
/****************************************************************************************************
	Function:
		void UPD_RegWriteByte (UINT8 u8PortNum, UINT16 u16RegOffset, UINT8 u8WriteData)

	Summary:
		Wrapper to do BYTE write to UPD register
	
	Devices Supported:
		UPD350 REV A

	Description:
		This API is to write a BYTE to any given UPD register address
		
	Precondition:
		None.

	Parameters:
		u8PortNum		- Corresponding port number
		u16RegOffset	- UPD Register Address where given data to be written
		u8WriteData		- Byte data to be written to the location

	Return:
		None.

	Remarks:
		None.
 **************************************************************************************************/
void UPD_RegWriteByte (UINT8 u8PortNum, UINT16 u16RegOffset, UINT8 u8WriteData);

/*****************************************************************************************************
	Function:
		void UPD_RegWriteWord (UINT8 u8PortNum, UINT16 u16RegOffset, UINT16 u16WriteData)

	Summary:
		Wrapper to do WORD write to UPD register
	
	Devices Supported:
		UPD350 REV A

	Description:
		This API is to write a WORD to any given UPD register address
		
	Precondition:
		None.

	Parameters:
		u8PortNum		-  Corresponding port number
		u16RegOffset	-  UPD Register Address where given data to be written
		u16WriteData	-  16-bit WORD data to be written to the location

	Return:
		None.

	Remarks:
		None.
**************************************************************************************************/
void UPD_RegWriteWord (UINT8 u8PortNum, UINT16 u16RegOffset, UINT16 u16WriteData);

/*****************************************************************************************************
	Function:
		UINT8 UPD_RegReadByte (UINT8 u8PortNum, UINT16 u16RegOffset)

	Summary:
		Wrapper to read a BYTE from a UPD register.
	
	Devices Supported:
		UPD350 REV A

	Description:
		This API is to read a BYTE from any given UPD register address. 
		It returns the read value.
		
	Precondition:
		None.

	Parameters:
		u8PortNum		-  Corresponding port number
		u16RegOffset	-  UPD Register Address from where data to be read

	Return:
		UINT8 			- Read data is written.

	Remarks:
		None.
**************************************************************************************************/
UINT8 UPD_RegReadByte (UINT8 u8PortNum, UINT16 u16RegOffset);

/*****************************************************************************************************
	Function:
		UINT16 UPD_RegReadWord (UINT8 u8PortNum, UINT16 u16RegOffset)

	Summary:
		Wrapper to read a WORD from a UPD register through SPI
	
	Devices Supported:
		UPD350 REV A

	Description:
		This API is to read a WORD from any given UPD register address. 
		It returns the read value.
		
	Precondition:
		None.

	Parameters:
		u8PortNum		-  Corresponding port number
		u16RegOffset	-  UPD Register Address from where data to be read

	Return:
		UINT16 			- Read data is written.

	Remarks:
		None.
**************************************************************************************************/
UINT16 UPD_RegReadWord (UINT8 u8PortNum, UINT16 u16RegOffset);

/*****************************************************************************************************
	Function:
		void UPD_RegisterWrite(UINT8 u8PortNum, UINT16 u16RegOffset, UINT8 *pu8WriteData, UINT8 u8WriteDataLen)

	Summary:
		To write a buffer to a UPD register address
	
	Devices Supported:
		UPD350 REV A

	Description:
		This API is to write a buffer to UPD Register Address.
		
	Precondition:
		None.

	Parameters:
		u8PortNum		- Corresponding port number
		u16RegOffset	- UPD Register Address where data buffer to be written
		pu8WriteData	- Pointer to buffer to be written
		u8WriteDataLen	- size of buffer to be written

	Return:
		None.

	Remarks:
		None.
**************************************************************************************************/
void UPD_RegisterWrite(UINT8 u8PortNum, UINT16 u16RegOffset, UINT8 *pu8WriteData, UINT8 u8WriteDataLen);

/*****************************************************************************************************
	Function:
		void UPD_RegisterRead(UINT8 u8PortNum, UINT16 u16RegOffset, UINT8 *pu8ReadData, UINT16 u8Readlen)

	Summary:
		To read a buffer from a UPD register address
	
	Devices Supported:
		UPD350 REV A

	Description:
		This API is to read buffer of passed data size from UPD Register Address.
		
	Precondition:
		None.

	Parameters:
		u8PortNum		-  Corresponding port number
		u16RegOffset	-  UPD Register Address from where buffer to be read
		*pu8ReadData	-  Pointer to data buffer where read data is to be updated
		u8Readlen       -  size of data to read from the register

	Return:
		None.

	Remarks:
		None.
**************************************************************************************************/
void UPD_RegisterRead(UINT8 u8PortNum, UINT16 u16RegOffset, UINT8 *pu8ReadData, UINT8 u8Readlen);

/*****************************************************************************************************
	Function:
		void UPD_RegByteSetBit (UINT8 u8PortNum, UINT16 u16RegOffset, UINT8 u8BitMsk);

	Summary:
		To SET a bit in a Byte accessed UPD Register.
	
	Devices Supported:
		UPD350 REV A

	Description:
		This API is to SET a specified bit in a UPD Register through Byte access.
		
	Precondition:
		It is confined to byte access of the UPD Register.

	Parameters:
		u8PortNum		-  Corresponding port number
		u16RegOffset	-  UPD Register Address
		u8BitMsk		-  Mask for the bit to be SET

	Return:
		None.

	Remarks:
		None.
**************************************************************************************************/
void UPD_RegByteSetBit (UINT8 u8PortNum, UINT16 u16RegOffset, UINT8 u8BitMsk);

/*****************************************************************************************************
	Function:
		void UPD_RegByteClearBit (UINT8 u8PortNum, UINT16 u16RegOffset, UINT8 u8BitMsk)

	Summary:
		To CLEAR a bit in a Byte accessed UPD Register.
	
	Devices Supported:
		UPD350 REV A

	Description:
		This API is to CLEAR a specified bit in a UPD Register through Byte access.
		
	Precondition:
		It is confined to byte access of the UPD Register.

	Parameters:
		u8PortNum		-  Corresponding port number
		u16RegOffset	-  UPD Register Address
		u8BitMsk		-  Mask for the bit to be CLEARed

	Return:
		None.

	Remarks:
		None.
**************************************************************************************************/
void UPD_RegByteClearBit (UINT8 u8PortNum, UINT16 u16RegOffset, UINT8 u8BitMsk);

/*****************************************************************************************************
	Function:
		void UPD_RegisterWriteISR(UINT8 u8PortNum, UINT16 u16RegOffset, UINT8 *pu8WriteData, UINT8 u8WriteDataLen);

	Summary:
		To write a buffer in ISR context to a UPD register address.
	
	Devices Supported:
		UPD350 REV A

	Description:
		This API is used in ISR to write a buffer to UPD Register Address without enabling or 
        disabling the interrupts.
		
	Precondition:
		It should be used only ISR.

	Parameters:
		u8PortNum		- Corresponding port number
		u16RegOffset	- UPD Register Address where data buffer to be written
		pu8WriteData	- Pointer to buffer to be written
		u8WriteDataLen  - size of buffer to be written

	Return:
		None.

	Remarks:
		None.
**************************************************************************************************/
void UPD_RegisterWriteISR(UINT8 u8PortNum, UINT16 u16RegOffset, UINT8 *pu8WriteData, UINT8 u8WriteDataLen);

/*****************************************************************************************************
	Function:
		void UPD_RegisterReadISR(UINT8 u8PortNum, UINT16 u16RegOffset, UINT8 *pu8ReadData, UINT8 u8Readlen)

	Summary:
		To read a buffer in ISR context from a UPD register address.
	
	Devices Supported:
		UPD350 REV A

	Description:
		This API is used in ISR to read a buffer from UPD register address without enabling or 
        disabling the interrupts.
		
	Precondition:
		It should be used only ISR.

	Parameters:
		u8PortNum		-  Corresponding port number
		u16RegOffset	-  UPD Register Address from where buffer to be read
		*pu8ReadData	-  Pointer to data buffer where read data is to be updated
		u8Readlen       -  size of data to read from the register

	Return:
		None.

	Remarks:
		None.
**************************************************************************************************/
void UPD_RegisterReadISR(UINT8 u8PortNum, UINT16 u16RegOffset, UINT8 *pu8ReadData, UINT8 u8Readlen);

/*****************************************************************************************************
	Function:
		void UPD_GPIOEnableDisable(UINT8 u8PortNum,UINT8 u8PIONum, UINT8 u8EnableDisable)

	Summary:
		To enable/disable the UPD's PIO's GPIO functionality.
	
	Devices Supported:
		UPD350 REV A

	Description:
		This API enables/disables the GPIO functionality of UPD350's PIO whose 
        PIO number is passed.
		
	Precondition:
		None.

	Parameters:
		u8PortNum		-  Corresponding port number
        u8PIONum        -  PIO number for which GPIO functionality has to be enabled or disabled.
		u8EnableDisable	-  This parameter can take following values:
                            UPD_ENABLE_GPIO - enables the GPIO functionality for the u8PIONum
                            UPD_DISABLE_GPIO - disables the GPIO functionality for the u8PIONum

	Return:
		None.

	Remarks:
		None.
**************************************************************************************************/
void UPD_GPIOEnableDisable(UINT8 u8PortNum,UINT8 u8PIONum, UINT8 u8EnableDisable);

/*****************************************************************************************************
	Function:
		void UPD_GPIOSetDirection(UINT8 u8PortNum, UINT8 u8PIONum, UINT8 u8Direction)

	Summary:
		API sets the PIO's direction.
	
	Devices Supported:
		UPD350 REV A

	Description:
		This API is to set the UPD350's PIO to either input or output direction.
		
	Precondition:
		None.

	Parameters:
		u8PortNum		-  Corresponding port number
        u8PIONum        -  PIO number for which GPIO direction is to be set.
		u8Direction	    -  This parameter can take following values:
                            UPD_GPIO_SETDIR_OUTPUT - sets the output direction for u8PIONum.
                            UPD_GPIO_SETDIR_INPUT - sets the input direction for the u8PIONum.

	Return:
		None.

	Remarks:
		None.
**************************************************************************************************/
void UPD_GPIOSetDirection(UINT8 u8PortNum,UINT8 u8PIONum, UINT8 u8Direction);

/*****************************************************************************************************
	Function:
		void UPD_GPIOSetBufferType(UINT8 u8PortNum, UINT8 u8PIONum, UINT8 u8BufferType)

	Summary:
		API sets the output buffer type.
	
	Devices Supported:
		UPD350 REV A

	Description:
		This API is to set the UPD350's PIO to Push/pull driver or open drain driver buffer type.
		
	Precondition:
		None.

	Parameters:
		u8PortNum		-  Corresponding port number
        u8PIONum        -  PIO number for which GPIO output buffer type has to be set.
		u8BufferType	-  This buffer type parameter can take following values:
                            UPD_GPIO_SETBUF_PUSHPULL - the output buffer for the corresponding GPIO 
                                                       signal is configured as a push/pull driver.
                            UPD_GPIO_SETBUF_OPENDRAIN - the output buffer for the corresponding GPIO 
                                                       signal is configured as an open drain driver.

	Return:
		None.

	Remarks:
		None.
**************************************************************************************************/
void UPD_GPIOSetBufferType(UINT8 u8PortNum, UINT8 u8PIONum, UINT8 u8BufferType);

/*****************************************************************************************************
	Function:
		void UPD_GPIOUpdateOutput(UINT8 u8PortNum, UINT8 u8PIONum, UINT8 u8PioMode, UINT8 u8DriveType)

	Summary:
		API updates the output data of the GPIO.
	
	Devices Supported:
		UPD350 REV A

	Description:
		This API is to sets or clears the UPD350's PIO data output to active or inactive.
		
	Precondition:
		None.

	Parameters:
		u8PortNum		-  Corresponding port number
        u8PIONum        -  PIO number for which GPIO data output is set or cleared.
		u8PioMode	    -  This parameter can take following values:
                            0x08 - sets the Data output to Active (high for push-pull, high-z for open-drain).
                            0x00 - sets the Data output to Inactive (low for push-pull or open-drain).
        u8DriveType     -  eUPD_GPIO_DRIVE_TYPE whether to assert or de-assert the GPIO
	Return:
		None.

	Remarks:
		None.
**************************************************************************************************/

void UPD_GPIOUpdateOutput(UINT8 u8PortNum, UINT8 u8PIONum, UINT8 u8PioMode, UINT8 u8DriveType);

/*****************************************************************************************************
	Function:
		void UPD_GPIOSetClearOutput(UINT8 u8PortNum, UINT8 u8PIONum, UINT8 u8SetClear)

	Summary:
		API sets or clears the output data of the GPIO.
	
	Devices Supported:
		UPD350 REV A

	Description:
		This API is to sets or clears the UPD350's PIO data output to active or inactive.
		
	Precondition:
		None.

	Parameters:
		u8PortNum		-  Corresponding port number
        u8PIONum        -  PIO number for which GPIO data output is set or cleared.
		u8SetClear	    -  This parameter can take following values:
                            UPD_GPIO_SET - sets the Data output to Active (high for push-pull, high-z for open-drain).
                            UPD_GPIO_CLEAR - sets the Data output to Inactive (low for push-pull or open-drain).
	Return:
		None.

	Remarks:
		None.
**************************************************************************************************/
void UPD_GPIOSetClearOutput(UINT8 u8PortNum, UINT8 u8PIONum, UINT8 u8SetClear);

/*****************************************************************************************************
	Function:
		void UPD_GPIOSetIntrAlert (UINT8 u8PortNum, UINT8 u8PIONum, UINT8 u8IntrType)

	Summary:
		API sets the alert interrupt for the GPIO.
	
	Devices Supported:
		UPD350 REV A

	Description:
		This API is to sets or clears the alert interrupt type.
		
	Precondition:
		None.

	Parameters:
		u8PortNum		-  Corresponding port number
        u8PIONum        -  PIO number for which interrupt type has to be configured.
		u8SetClear	    -  This parameter can take following values:
                            UPD_GPIO_CLEAR_INTR- No Alert interrupt is configured
                            UPD_CFG_PIO_RISING_ALERT - Configure PIO interrupt to rising alert
                            UPD_CFG_PIO_FALLING_ALERT - Configure PIO interrupt to falling alert
	Return:
		None.

	Remarks:
		None.
**************************************************************************************************/
void UPD_GPIOSetIntrAlert (UINT8 u8PortNum, UINT8 u8PIONum, UINT8 u8IntrType);

/*****************************************************************************************************
	Function:
		void UPD_ConfigurePIODebounceCount(UINT8 u8PortNum, UINT8 u8CountType, UINT8 u8CountValue)

	Summary:
		API configures the debounce count for corresponding debounce type.
	
	Devices Supported:
		UPD350 REV A

	Description:
		This API is to configure the debounce count for corresponding debounce type.
		
	Precondition:
		None.

	Parameters:
		u8PortNum		-  Corresponding port number
        u8CountValue    -  Sets the count value to corresponding debounce count type
        u8CountType     -  This parameter can take following values:
                           UPD_PIO_DEBOUNCE_CNT_TYP_1_US - Sets the count value to 1us PIO debounce count
                           UPD_PIO_DEBOUNCE_CNT_TYP_1_MS - Sets the count value to 1ms PIO debounce count
                           UPD_PIO_DEBOUNCE_CNT_TYP_10_MS - Sets the count value to 10ms PIO debounce count
	Return:
		None.

	Remarks:
		None.
**************************************************************************************************/
void UPD_ConfigurePIODebounceCount(UINT8 u8PortNum, UINT8 u8CountType, UINT8 u8CountValue);

/*****************************************************************************************************
	Function:
		void UPD_GPIOSetDebounce (UINT8 u8PortNum, UINT8 u8PIONum, UINT8 u8DebounceEnType)

	Summary:
		API enables the debounce for the UPD350 PIO.
	
	Devices Supported:
		UPD350 REV A

	Description:
		This API is to set or reset the debounce corresponidng debounce type for the PIO number passed.
		
	Precondition:
		None.

	Parameters:
		u8PortNum		-  Corresponding port number
        u8PIONum        -  UPD350 PIO number for which the debounce has to be enabled or disabled
        u8DebounceEnType-  This parameter can take following values:
                           UPD_PIO_DEBOUNCE_CNT_TYP_1_US - Enables 1us PIO debounce count for the u8PIONum
                           UPD_PIO_DEBOUNCE_CNT_TYP_1_MS - Enables 1ms PIO debounce count for the u8PIONum
                           UPD_PIO_DEBOUNCE_CNT_TYP_10_MS - Enables 10ms PIO debounce count for the u8PIONum
                           UPD_PIO_DEBOUNCE_DISABLE - Disables the debounce
	Return:
		None.

	Remarks:
		None.
**************************************************************************************************/
void UPD_GPIOSetDebounce (UINT8 u8PortNum, UINT8 u8PIONum, UINT8 u8DebounceEnType);

/**************************************************************************************************
    Function:
        void UPD_GPIOInit(UINT8 u8PortNum)

    Summary:
        This API initialize the GPIO module based on port number passed.

    Devices Supported:
        UPD350 REV A

    Description:
        This API initialize the GPIO module(interrupts, PIO configuration) based on port numnber passed.

    Conditions:
        None.

    Input:
        u8PortNum - Port number of the device.
					Value passed will be less than CONFIG_PD_PORT_COUNT.

    Return:
        None.

    Remarks:
        This API has to be called for all the ports (0 to (CONFIG_PD_PORT_COUNT -1))
		to initialize the GPIO module for the port.

**************************************************************************************************/
void UPD_GPIOInit(UINT8 u8PortNum);

/*****************************************************************************************************
	Function:
		void UPD_PIOHandleISR (UINT8 u8PortNum)

	Summary:
        UPD's GPIO interrupt Handler

    Devices Supported:
        UPD350 REV A

    Description:
		This API is called to process UPD's GPIO interrupt.
		
	Precondition:
		None.

	Parameters:
		u8PortNum		-  Corresponding port number

	Return:
		None.

	Remarks:
		None.
**************************************************************************************************/
void UPD_PIOHandleISR (UINT8 u8PortNum);

/*****************************************************************************************************
	Function:
		void UPD_FaultInInit (UINT8 u8PortNum)

	Summary:
		API to configure the fault in pin of the UPD350
	
	Devices Supported:
		UPD350 REV A

	Description:
		This API enables the fault in pin configured for the port passed.
		
	Precondition:
		None.

	Parameters:
		u8PortNum		-  Corresponding port number
	Return:
		None.

	Remarks:
		None.
**************************************************************************************************/
void UPD_FaultInInit (UINT8 u8PortNum);

/**************************************************************************************************
    Function:
        void UPD_ConfigPIOOvrforPwrFault (UINT8 u8PortNum)

    Summary:
        This API initialize PIO override on power fault for the port.

    Devices Supported:
        UPD350 REV A

    Description:
        This API initialize PIO override on power fault for the port.

    Conditions:
        confined to INCLUDE_UPD_PIO_OVERRIDE_SUPPORT define.
        PIO Override concept is applicable only if the EN_VBUS and FAULT_IN pin are UPD's PIO
        in case of Source. EN_SINK and FAULT_IN in case of Sink.

    Input:
        u8PortNum - Port number of the device.
					Value passed will be less than CONFIG_PD_PORT_COUNT.

    Return:
        None.

    Remarks:
        Fault_IN for the port is obtained from gasCfgStatusData.sPerPortData[u8PortNum].u8Pio_FAULT_IN 
        EN_VBUS for the port is obtained from gasCfgStatusData.sPerPortData[u8PortNum].u8Pio_EN_VBUS
        EN_SINK for the port is obtained from gasCfgStatusData.sPerPortData[u8PortNum].u8Pio_EN_SINK

**************************************************************************************************/
void UPD_ConfigPIOOvrforPwrFault (UINT8 u8PortNum);

/**************************************************************************************************
    Function:
        void UPD_SetIdleCB (UINT8 u8PortNum, UINT8 u8DummyVariable)

    Summary:
        This API is given as the timer call back API when starting the Idle Timer 

    Devices Supported:
        UPD350 REV A

    Description:
        This API is given as the timer call back API when starting the Idle Timer
        started to indicate the UPD350s Idle state. 

    Conditions:
        confined to INCLUDE_POWER_MANAGEMENT_CTRL define.

    Input:
        u8PortNum - Port number of the device.
					Value passed will be less than CONFIG_PD_PORT_COUNT.
                    
        u8DummyVariable - Dummy state variable
        
    Return:
        None.

    Remarks:
        None.
**************************************************************************************************/
void UPD_SetIdleCB (UINT8 u8PortNum, UINT8 u8DummyVariable);

/**************************************************************************************************
    Function:
        void PD_StartIdleTimer(UINT8 u8PortNum)

    Summary:
        This API starts the port Idle timer 

    Devices Supported:
        UPD350 REV A

    Description:
        This API starts the  port Idle timer with the value specified in
        CONFIG_PORT_UPD_IDLE_TIMEOUT_MS

    Conditions:
        confined to INCLUDE_POWER_MANAGEMENT_CTRL define.

    Input:
        u8PortNum - Port number of the device.
					Value passed will be less than CONFIG_PD_PORT_COUNT.
        
    Return:
        None.

    Remarks:
        None.
**************************************************************************************************/
void PD_StartIdleTimer(UINT8 u8PortNum);

/**************************************************************************************************
    Function:
        UINT8 UPD_CheckUPDsActive()

    Summary:
        This API is to check the active or idle status of UPD350s 

    Devices Supported:
        UPD350 REV A

    Description:
        This API is to check the Active or Idle status of UPD350s
    Conditions:
        confined to INCLUDE_POWER_MANAGEMENT_CTRL define.

    Input:
        None
        
    Return:
        None.

    Remarks:
        TRUE - UPD350s are in Active State
        FALSE - All the UPD350s are in Idle State
**************************************************************************************************/
UINT8 UPD_CheckUPDsActive();

/**************************************************************************************************
    Function:
        UPD_PwrManagementCtrl(UINT8 u8PortNum)

    Summary:
        This API is to initiate the Power Management control.

    Devices Supported:
        UPD350 REV A

    Description:
        This API is to initiate the Power Management control and indicate the
        Application layer to put MCU in Idle state.

    Conditions:
        confined to INCLUDE_POWER_MANAGEMENT_CTRL define.

    Input:
        u8PortNum - Port number of the device.
					Value passed will be less than CONFIG_PD_PORT_COUNT.
        
    Return:
        None.

    Remarks:
        None.
**************************************************************************************************/
void UPD_PwrManagementCtrl(UINT8 u8PortNum);

/**************************************************************************************************
    Function:
        UPD_PwrManagementInit(UINT8 u8PortNum)

    Summary:
        This API is to initialize the Power Management control.

    Devices Supported:
        UPD350 REV A

    Description:
        This API is to initialize the Power Management control variables and starts
        the Idle timer during initialization.

    Conditions:
        confined to INCLUDE_POWER_MANAGEMENT_CTRL define.

    Input:
        u8PortNum - Port number of the device.
					Value passed will be less than CONFIG_PD_PORT_COUNT.
        
    Return:
        None.

    Remarks:
        None.
**************************************************************************************************/
void UPD_PwrManagementInit(UINT8 u8PortNum);

/**************************************************************************************************
    Function:
        UPD_CheckAndDisablePorts()

    Summary:
        This API is to check the whether the port is working or not.

    Devices Supported:
        UPD350 REV A

    Description:
        This API is to check the whether the port is working or not by reading
        the UPD350s SPI_TEST register. 

    Conditions:
        None.

    Input:
        None.
        
    Return:
        None.

    Remarks:
        None.
**************************************************************************************************/
void UPD_CheckAndDisablePorts (void);

/**************************************************************************************************
    Function:
        UPD_FindVBusCorrectionFactor()

    Summary:
        This API is to find the VBUS correction factor.

    Devices Supported:
        UPD350 REV A

    Description:
        This API is to find the VBUS correction factor.

    Conditions:
        None.

    Input:
        None.
        
    Return:
        None.

    Remarks:
        None.
**************************************************************************************************/
void UPD_FindVBusCorrectionFactor(void);

/**************************************************************************************************
    Function:
        void UPD_EnableFaultIn(UINT8 u8PortNum)

    Summary:
        This API enables Fault_In. 

    Devices Supported:
        UPD350 REV A

    Description:
        This API is to enable the Fault_In to detect OCS.

    Conditions:
        None.

    Input:
        u8PortNum - Port number of the device.
					Value passed will be less than CONFIG_PD_PORT_COUNT.
        
    Return:
        None.

    Remarks:
        None.
**************************************************************************************************/
void UPD_EnableFaultIn(UINT8 u8PortNum);

/**************************************************************************************************
    Function:
        void UPD_ConfigPwrFaultPIOOvverride (UINT8 u8PortNum)

    Summary:
        This API configures PIO override functionality.

    Devices Supported:
        UPD350 REV A

    Description:
        This API is to configures UPD350's PIO override feature for undervoltage, overvoltage and 
        Fault_In OCS.

    Conditions:
        None.

    Input:
        u8PortNum - Port number of the device.
					Value passed will be less than CONFIG_PD_PORT_COUNT.
        
    Return:
        None.

    Remarks:
        None.
**************************************************************************************************/
void UPD_ConfigPwrFaultPIOOvverride (UINT8 u8PortNum);

#endif /*_UPD_HW_H_*/
