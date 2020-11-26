/*******************************************************************************
 UPD interrupt source file

  Company:
    Microchip Technology Inc.

  File Name:
    upd_interrupts.c

  Description:
    This file contains functions to handle UPD interrupts.
 *******************************************************************************/
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
*******************************************************************************/

#include <psf_stdinc.h>

void UPDIntr_AlertHandler (UINT8 u8PortNum)
{
    do
    {
        if (UPD_PORT_DISABLED == DPM_GET_CONFIGURED_PORT_EN(u8PortNum))
        {
            break;
        }
              
        MCHP_PSF_HOOK_DISABLE_GLOBAL_INTERRUPT();
        
        UINT16 u16InterruptStatus = SET_TO_ZERO;
        UINT16 u16Data = SET_TO_ZERO;

#if (TRUE == INCLUDE_POWER_MANAGEMENT_CTRL)
        UINT8 u8ReadData = SET_TO_ZERO;
	
        if (UPD_STATE_IDLE == gau8ISRPortState[u8PortNum])
        {             
            /*UPD350 was in idle , make sure SPI_TEST register is read as 0x02*/
            while (TRUE)
            {
                /*Read SPI_TEST register*/
                UPD_RegisterReadISR(u8PortNum, (UINT16)UPD_SPI_TEST, &u8ReadData, BYTE_LEN_1);
                    
                /*Check the SPI_TEST register value is 0x02*/
                if (UPD_SPI_TEST_VAL == u8ReadData)
                {
                    break;
                }
            }
        }
#endif

        UPD_RegisterReadISR (u8PortNum,UPDINTR_INT_STS,(UINT8 *)&u16InterruptStatus,BYTE_LEN_2);
        
        /*Checking for Device ready Interrupt*/
        if(u16InterruptStatus & UPDINTR_RDY_INT)
        {                
            UPD_RegisterReadISR (u8PortNum, UPDINTR_INT_EN, (UINT8 *)&u16Data, BYTE_LEN_2);
            
            u16Data &= ~UPDINTR_RDY_INT; 
            
            UPD_RegisterWriteISR (u8PortNum, UPDINTR_INT_EN, (UINT8 *)&u16Data, BYTE_LEN_2);            
        }

        #if(TRUE == INCLUDE_UPD_HPD)
        if(u16InterruptStatus & UPDINTR_HPD_INT)
        {
            UPD_HPDHandleISR(u8PortNum);
        }
        #endif

        /*CC,PWR,VBUS interrupts are handled by the function "TypeC_InterruptHandler"*/
        if((u16InterruptStatus & UPDINTR_CC_INT) || (u16InterruptStatus & UPDINTR_PWR_INT) || \
                (u16InterruptStatus & UPDINTR_VBUS_INT) || (u16InterruptStatus & UPDINTR_EXT_INT))
        {
            TypeC_HandleISR (u8PortNum, u16InterruptStatus);
        }
        
        /*Checking for PD MAC Interrupt*/
        if (u16InterruptStatus & UPDINTR_MAC_INT)
        {
            PRL_HandleISR (u8PortNum);
        }
		
		/* Checking for UPD GPIO interrupt */
        if (u16InterruptStatus & UPDINTR_PIO_INT)
        {
            UPD_PIOHandleISR (u8PortNum);
        }

#if (TRUE == INCLUDE_POWER_MANAGEMENT_CTRL)        
        /*Set UPD_STATE_ACTIVE at End of ISR*/
        gau8ISRPortState[u8PortNum] = UPD_STATE_ACTIVE;
#endif
        
        MCHP_PSF_HOOK_ENABLE_GLOBAL_INTERRUPT();	
    }
    while (FALSE);
}
