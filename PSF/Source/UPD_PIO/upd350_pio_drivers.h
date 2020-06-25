/*******************************************************************************
  UPD350 PIO Configuration Header File 

  Company:
    Microchip Technology Inc.

  File Name:
    upd350_pio_drivers.h

  Description:
    This header file contains the data structures, constant definitions and 
	function prototypes for UPD350 PIO Drivers. 
*******************************************************************************/
/*******************************************************************************
Copyright ©  [2020] Microchip Technology Inc. and its subsidiaries.

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

#ifndef _UPD350_PIO_DRIVERS_H    /* Guard against multiple inclusion */
#define _UPD350_PIO_DRIVERS_H

/* ************************************************************************** */
/* ************************************************************************** */
/* Section: Constants                                                         */
/* ************************************************************************** */
/* ************************************************************************** */ 
/* UPD350 PIO numbers */

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

/***********************************************************************************************/
/* Defines used for 'u8BufType' argument in UPDPIO_SetBufferType function */ 
#define UPD_PIO_SETBUF_PUSHPULL    0
#define UPD_PIO_SETBUF_OPENDRAIN   1

/* Defines used for 'u8PUEnable' argument in UPDPIO_EnablePullUp function */
#define UPD_PIO_ENABLE_PULLUP       0
#define UPD_PIO_DISABLE_PULLUP      1

/* Defines used for 'u8PDEnable' argument in UPDPIO_EnablePullDown function */
#define UPD_PIO_ENABLE_PULLDOWN     0
#define UPD_PIO_DISABLE_PULLDOWN    1

/*****************************************************************************************************/

/*Defines used for Configuring PIO register values */
#define UPD_CFG_PIO_GPIO_ENABLE  		0x01
#define UPD_CFG_PIO_DIRECTION    	    0x02
#define UPD_CFG_PIO_BUFFER_TYPE         0x04
#define UPD_CFG_PIO_DATAOUTPUT		    0x08
#define UPD_CFG_PIO_RISING_ALERT		0x10
#define UPD_CFG_PIO_FALLING_ALERT		0x20
#define UPD_CFG_PIO_PULL_DOWN_ENABLE	0x40
#define UPD_CFG_PIO_PULL_UP_ENABLE		0x80

typedef enum
{
    UPD_GPIO_ASSERT = 0x00U,
    UPD_GPIO_DE_ASSERT = UPD_CFG_PIO_DATAOUTPUT
}eUPD_GPIO_DRIVE_TYPE;


/***************************************************************************************/
// *****************************************************************************
// *****************************************************************************
// Section: Data Structures
// *****************************************************************************
// *****************************************************************************




// *****************************************************************************
// *****************************************************************************
// Section: Interface Functions
// *****************************************************************************
// *****************************************************************************
/*****************************************************************************************************
	Function:
		void UPDPIO_EnableOutput(UINT8 u8PortNum, eUPD_PIO_NUM_TYPE ePIONum); 

	Summary:
		Enables the UPD350 PIO's GPIO functionality and set direction as output.
	
	Devices Supported:
		UPD350 REV A

	Description:
		This API enables the GPIO functionality of UPD350's PIO whose 
        PIO number is passed and configures it's direction as output.
		
	Precondition:
		None.

	Parameters:
        u8PortNum - PD Port to which the UPD350 belongs to. 
                    It can take values from 0 to (CONFIG_PD_PORT_COUNT - 1). 
        ePIONum - UPD350 PIO number. It can take values from the enum 
                  eUPD_PIO_NUM_TYPE other than eUPD_PIO_UN_DEF.

	Return:
		None.

	Remarks:
		None.
**************************************************************************************************/
void UPDPIO_EnableOutput(UINT8 u8PortNum, eUPD_PIO_NUM_TYPE ePIONum); 

/*****************************************************************************************************
	Function:
		void UPDPIO_EnableInput(UINT8 u8PortNum, eUPD_PIO_NUM_TYPE ePIONum);

	Summary:
		Enables the UPD350 PIO's GPIO functionality and set direction as input.
	
	Devices Supported:
		UPD350 REV A

	Description:
		This API enables the GPIO functionality of UPD350's PIO whose 
        PIO number is passed and configures it's direction as input.
		
	Precondition:
		None.

	Parameters:
        u8PortNum - PD Port to which the UPD350 belongs to. 
                    It can take values from 0 to (CONFIG_PD_PORT_COUNT - 1). 
        ePIONum - UPD350 PIO number. It can take values from the enum 
                  eUPD_PIO_NUM_TYPE other than eUPD_PIO_UN_DEF.

	Return:
		None.

	Remarks:
		None.
**************************************************************************************************/
void UPDPIO_EnableInput(UINT8 u8PortNum, eUPD_PIO_NUM_TYPE ePIONum);

/*****************************************************************************************************
	Function:
		void UPDPIO_Disable(UINT8 u8PortNum, eUPD_PIO_NUM_TYPE ePIONum);

	Summary:
		Disables the UPD350 PIO's GPIO functionality.
	
	Devices Supported:
		UPD350 REV A

	Description:
		This API disables the GPIO functionality of UPD350's PIO whose 
        PIO number is passed.
		
	Precondition:
		None.

	Parameters:
        u8PortNum - PD Port to which the UPD350 belongs to. 
                    It can take values from 0 to (CONFIG_PD_PORT_COUNT - 1). 
        ePIONum - UPD350 PIO number. It can take values from the enum 
                  eUPD_PIO_NUM_TYPE other than eUPD_PIO_UN_DEF.

	Return:
		None.

	Remarks:
		None.
**************************************************************************************************/
void UPDPIO_Disable(UINT8 u8PortNum, eUPD_PIO_NUM_TYPE ePIONum);

/*****************************************************************************************************
	Function:
		void UPDPIO_SetBufType(UINT8 u8PortNum, eUPD_PIO_NUM_TYPE ePIONum, UINT8 u8BufType); 

	Summary:
		Sets the UPD350 PIO's output buffer type. 
	
	Devices Supported:
		UPD350 REV A

	Description:
		This API sets the output buffer type of UPD350's PIO as either 
        Push/pull or open drain.
		
	Precondition:
		UPDPIO_EnableOutput(UINT8 u8PortNum, eUPD_PIO_NUM_TYPE ePIONum) 
        shall be called before calling this API. 

	Parameters:
        u8PortNum - PD Port to which the UPD350 belongs to. 
                    It can take values from 0 to (CONFIG_PD_PORT_COUNT - 1). 
        ePIONum - UPD350 PIO number. It can take values from the enum 
                  eUPD_PIO_NUM_TYPE other than eUPD_PIO_UN_DEF.
        u8BufType - Output Buffer type to be set for the PIO.
                    It can take either 0 or 1. 
                    '0' - Push Pull driver 
                    '1' - Open Drain driver 
                    
	Return:
		None.

	Remarks:
		None.
**************************************************************************************************/
void UPDPIO_SetBufferType(UINT8 u8PortNum, eUPD_PIO_NUM_TYPE ePIONum, UINT8 u8BufType); 

/*****************************************************************************************************
	Function:
		void UPDPIO_DriveHigh(UINT8 u8PortNum, eUPD_PIO_NUM_TYPE ePIONum); 

	Summary:
		Drives the UPD350's PIO output to high.
	
	Devices Supported:
		UPD350 REV A

	Description:
		This API drives the UPD350's PIO output to high.
		
	Precondition:
		UPDPIO_EnableOutput(UINT8 u8PortNum, eUPD_PIO_NUM_TYPE ePIONum) shall be
        called before calling this API. 

	Parameters:
        u8PortNum - PD Port to which the UPD350 belongs to. 
                    It can take values from 0 to (CONFIG_PD_PORT_COUNT - 1). 
        ePIONum - UPD350 PIO number. It can take values from the enum 
                  eUPD_PIO_NUM_TYPE other than eUPD_PIO_UN_DEF.

	Return:
		None.

	Remarks:
		None.
**************************************************************************************************/
void UPDPIO_DriveHigh(UINT8 u8PortNum, eUPD_PIO_NUM_TYPE ePIONum); 

/*****************************************************************************************************
	Function:
		void UPDPIO_DriveLow(UINT8 u8PortNum, eUPD_PIO_NUM_TYPE ePIONum); 

	Summary:
		Drives the UPD350's PIO output to low.
	
	Devices Supported:
		UPD350 REV A

	Description:
		This API drives the UPD350's PIO output to low.
		
	Precondition:
		UPDPIO_EnableOutput(UINT8 u8PortNum, eUPD_PIO_NUM_TYPE ePIONum) shall be
        called before calling this API. 

	Parameters:
        u8PortNum - PD Port to which the UPD350 belongs to. 
                    It can take values from 0 to (CONFIG_PD_PORT_COUNT - 1). 
        ePIONum - UPD350 PIO number. It can take values from the enum 
                  eUPD_PIO_NUM_TYPE other than eUPD_PIO_UN_DEF.

	Return:
		None.

	Remarks:
		None.
**************************************************************************************************/
void UPDPIO_DriveLow(UINT8 u8PortNum, eUPD_PIO_NUM_TYPE ePIONum); 

/*****************************************************************************************************
	Function:
		void UPDPIO_EnablePullUp(UINT8 u8PortNum, eUPD_PIO_NUM_TYPE ePIONum, UINT8 u8PUEnable); 

	Summary:
		Enables/disables internal pull-up on UPD350's PIO pads.
	
	Devices Supported:
		UPD350 REV A

	Description:
		This API enables/disables internal pull-up on UPD350's PIO pads.
		
	Precondition:
		The following APIs shall be called before calling this API. 
        UPDPIO_EnableOutput(UINT8 u8PortNum, eUPD_PIO_NUM_TYPE ePIONum); 
        UPDPIO_SetBufType(UINT8 u8PortNum, eUPD_PIO_NUM_TYPE ePIONum, UINT8 u8BufType) 
        with u8BufType as value '1'. 

	Parameters:
        u8PortNum - PD Port to which the UPD350 belongs to. 
                    It can take values from 0 to (CONFIG_PD_PORT_COUNT - 1). 
        ePIONum - UPD350 PIO number. It can take values from the enum 
                  eUPD_PIO_NUM_TYPE other than eUPD_PIO_UN_DEF.
     	u8PUEnable - Enable/Disable Pull-up. 
                    '0' - Enable pull-up
                    '1' - Disable pull-up 

	Return:
		None.

	Remarks:
		None.
**************************************************************************************************/
void UPDPIO_EnablePullUp(UINT8 u8PortNum, eUPD_PIO_NUM_TYPE ePIONum, UINT8 u8PUEnable); 

/*****************************************************************************************************
	Function:
		void UPDPIO_EnablePullDown(UINT8 u8PortNum, eUPD_PIO_NUM_TYPE ePIONum, UINT8 u8PDEnable); 

	Summary:
		Enables/disables internal pull-down on UPD350's PIO pads.
	
	Devices Supported:
		UPD350 REV A

	Description:
		This API enables/disables internal pull-up on UPD350's PIO pads.
		
	Precondition:
        None.

	Parameters:
        u8PortNum - PD Port to which the UPD350 belongs to. 
                    It can take values from 0 to (CONFIG_PD_PORT_COUNT - 1). 
        ePIONum - UPD350 PIO number. It can take values from the enum 
                  eUPD_PIO_NUM_TYPE other than eUPD_PIO_UN_DEF.
     	u8PDEnable - Enable/Disable Pull-down. 
                    '0' - Enable pull-down
                    '1' - Disable pull-down

	Return:
		None.

	Remarks:
		None.
**************************************************************************************************/
void UPDPIO_EnablePullDown(UINT8 u8PortNum, eUPD_PIO_NUM_TYPE ePIONum, UINT8 u8PDEnable); 

#endif /* _UPD350_PIO_DRIVERS_H */
/* *****************************************************************************
 End of File
 */
