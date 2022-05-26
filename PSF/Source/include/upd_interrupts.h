/*******************************************************************************
 UPD Interrupt file

  Company:
    Microchip Technology Inc.

  File Name:
    upd_interrupt.h

  Summary:
    UPD350 Interrupt Handling header file

  Description:
    This header file contains register defines and function for UPD350 interrupt
    handling.
 *******************************************************************************/
/*******************************************************************************
Copyright © [2022] Microchip Technology Inc. and its subsidiaries.

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


#ifndef _UPD_INTERRUPTS_H_
#define _UPD_INTERRUPTS_H_

// *****************************************************************************
// *****************************************************************************
// Section: Constants
// *****************************************************************************
// *****************************************************************************

#define UPDINTR_INT_STS         	UPD_SYSTEM_CSR_BASE_ADDR + 0x10
#define UPDINTR_INT_EN				UPD_SYSTEM_CSR_BASE_ADDR + 0x14

/* Bit definition of UPDINTR_INT_STS register */

#define UPDINTR_GP_TIMER_INT            BIT (14)
#define UPDINTR_RDY_INT                 BIT (12)
#define UPDINTR_EXT_INT                 BIT (11)
#define UPDINTR_PWR_INT                 BIT (10)
#define UPDINTR_VBUS_INT                BIT (9)
#define UPDINTR_HPD_INT                 BIT (8)
#define UPDINTR_PSW_INT                 BIT (7)
#define UPDINTR_WDT_INT                 BIT (6)
#define UPDINTR_PPC_INT                 BIT (5)
#define UPDINTR_MAC_INT                 BIT (4)
#define UPDINTR_OCS_CMP_INT             BIT (3)
#define UPDINTR_PIO_OVERRIDE_INT        BIT (2)
#define UPDINTR_PIO_INT                 BIT (1)
#define UPDINTR_CC_INT                  BIT (0)

/*Bit definitions for HPD_INT_STS register*/
#define UPDINTR_IRQ_HPD                       BIT(0)
#define UPDINTR_HPD_LOW                       BIT(1)
#define UPDINTR_HPD_HIGH                      BIT(2)
#define UPDINTR_HPD_QUEUE_NOT_EMPTY           BIT(3)
// *****************************************************************************
// *****************************************************************************
// Section: Interface Routines
// *****************************************************************************
// *****************************************************************************
/****************************************************************************************************
	Function:
		void UPDIntr_AlertHandler (UINT8 u8PortNum)
 
	Summary:
        UPD Interrupt Handler API
	
	Devices Supported:
		UPD350 REV A

	Description:
		This API handles the any UPD350 that has occurred through UPD350 IRQ Pin.
		
	Precondition:
		None.

	Parameters:
		u8PortNum - Port number of the device. Value passed will be less than CONFIG_PD_PORT_COUNT.

	Return:
		None.

	Remarks:
		None.
 **************************************************************************************************/
void UPDIntr_AlertHandler (UINT8 u8PortNum);


#endif /*_UPD_INTERRUPTS_H_*/
