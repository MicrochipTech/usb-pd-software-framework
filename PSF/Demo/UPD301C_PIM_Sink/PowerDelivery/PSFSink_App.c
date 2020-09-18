/***************************************************************************
 User Application File

  Company:
    Microchip Technology Inc.

  File Name:
    PSFSink_App.c

  Summary:
    User Application Source file

  Description:
    This source file contains user application specific functions and interfaces
************************************************************************** */
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

/* ************************************************************************** */
/* ************************************************************************** */
/* Section: Included Files                                                    */
/* ************************************************************************** */
/* ************************************************************************** */

#include "psf_stdinc.h"

/* ************************************************************************** */
/* ************************************************************************** */
/* Section: File Scope or Global Data                                         */
/* ************************************************************************** */
/* ************************************************************************** */




/* ************************************************************************** */
/* ************************************************************************** */
// Section: Local Functions                                                   */
/* ************************************************************************** */
/* ************************************************************************** */
void App_SetMCUIdle()
{
    MCHP_PSF_HOOK_DISABLE_GLOBAL_INTERRUPT();
    
    /*Disable Timer to avoid interrupt from Timer*/
    TC0_TimerStop(); 
    
    DEBUG_PRINT_PORT_STR (3, "Set SAMD20 to IDLE\r\n");
    
	/*If there is any pending interrupt it will not go to sleep*/
    SCB->SCR |=  (SCB_SCR_SLEEPDEEP_Msk )| (SCB_SCR_SEVONPEND_Msk);
    
    __DSB();
    __WFI();
    
    MCHP_PSF_HOOK_ENABLE_GLOBAL_INTERRUPT();
    
    /* Resume from Idle*/
    /* Enable the disabled interrupt*/
    TC0_TimerStart();
}

/* ************************************************************************** */
/* ************************************************************************** */
// Section: Interface Functions                                               */
/* ************************************************************************** */
/* ************************************************************************** */

UINT8 App_HandlePSFEvents(UINT8 u8PortNum, eMCHP_PSF_NOTIFICATION ePDEvent)
{
    UINT8 u8RetVal = FALSE;
    
    switch(ePDEvent)
    {
        case eMCHP_PSF_UPDS_IN_IDLE:
		{
#if (TRUE == INCLUDE_POWER_MANAGEMENT_CTRL)
            /* Configure the SoC to enter Low power mode. */
			App_SetMCUIdle();
#endif
			break;
		}
        
        case eMCHP_PSF_VCONN_PWR_FAULT:
        {
			/*Returning TRUE to enable PSF to handle the power fault*/
            u8RetVal = TRUE;
            break;
        }
        
        case eMCHP_PSF_VBUS_PWR_FAULT:
        {
			/*Returning TRUE to enable PSF to handle the power fault*/
            u8RetVal = TRUE;
            break;
        }
           
        case eMCHP_PSF_PORT_POWERED_OFF:
        {
            break;
        }
        
        case eMCHP_PSF_TYPEC_DETACH_EVENT:
        {
            break;
        }
        case eMCHP_PSF_TYPEC_CC1_ATTACH:
        {
            break;
        }
        case eMCHP_PSF_TYPEC_CC2_ATTACH:
        {
            break;
        }
        case eMCHP_PSF_CAPS_MISMATCH:
        {
            break;
        }
        case eMCHP_PSF_NEW_SRC_CAPS_RCVD:
        {
            break;
        }
        
        case eMCHP_PSF_PD_CONTRACT_NEGOTIATED: 
        {
            break; 
        }
       
        case eMCHP_PSF_TYPEC_ERROR_RECOVERY: 
        {
            break; 
        }
        
        case eMCHP_PSF_BUSY:
        {
            break; 
        }
        
        default:
            break;
    }

    return u8RetVal;
}
/**************************************************************************************/
void App_GPIOControl_Init(UINT8 u8PortNum, eMCHP_PSF_GPIO_FUNCTIONALITY eGPIOFunc)
{
    switch(eGPIOFunc)
    {
        case eUPD350_ALERT_FUNC:
        {           
            PORT_PinWrite(PORT_PIN_PA14, TRUE);
            PORT_PinInputEnable(PORT_PIN_PA14);
            EIC_CallbackRegister((EIC_PIN)PORT_PIN_PA14, SAMD20_UPD350AlertCallback, PORT0);
            EIC_InterruptEnable((EIC_PIN)PORT_PIN_PA14);
            break;
        }
        case eUPD350_RESET_FUNC:
        {
            /* UPD350 Reset pin initialization is done as part of  
               PORT_Initialize() by harmony. User can also choose to do the 
               initialization here. 
               It is configured in input mode, since configuring it in output 
               mode and driving it high always would increase current consumption. */      
            break;
        }
        case eSPI_CHIP_SELECT_FUNC:
        {
            SPI_SS_0_Set();
            SPI_SS_0_OutputEnable();            
            break; 
        }
        case eVBUS_DIS_FUNC:
        {      
              //SB Edit
//            UPDPIO_SetBufferType(u8PortNum, eUPD_PIO4, UPD_PIO_SETBUF_PUSHPULL);
//            UPDPIO_DriveLow(u8PortNum, eUPD_PIO4);
//            UPDPIO_EnableOutput(u8PortNum, eUPD_PIO4);
            PORT_PinWrite(PORT_PIN_PA28, FALSE);     //SB Edit
            PORT_PinOutputEnable(PORT_PIN_PA28);    //SB Edit
            
            break; 
        }
        case eORIENTATION_FUNC:
        {
            /*Init is called when detach happens*/
            UPDPIO_Disable(u8PortNum, eUPD_PIO2);
            break;
        }
        case eSNK_CAPS_MISMATCH_FUNC:
        {
            SNK_CAP_MISMATCH_Clear();
            SNK_CAP_MISMATCH_OutputEnable();            
            break;
        }
        case eSNK_1_5A_IND_FUNC:
        {
            SNK_1_5A_IND_Clear();
            SNK_1_5A_IND_OutputEnable();            
            break;
        }
        case eSNK_3A_IND_FUNC:
        {
            SNK_3A_IND_Clear();
            SNK_3A_IND_OutputEnable();            
            break;
        }    
        default:
        {
            break; 
        }
    }
}


void App_GPIOControl_Drive(UINT8 u8PortNum, eMCHP_PSF_GPIO_FUNCTIONALITY eGPIOFunc,
                                    eMCHP_PSF_GPIO_DRIVE_VAL eGPIODrive)
{
    switch(eGPIOFunc)
    {
        case eUPD350_ALERT_FUNC:
        {
            /*Alert is an input pin. Drive not applicable*/
            break;
        }
        case eI2C_DC_DC_ALERT_FUNC:
        {
            /*DC-DC Alert not applicable for Sink application */
            break;
        }
        case eUPD350_RESET_FUNC:
        {
            /* UPD350 Reset pin is configured in input mode, since configuring
               it in output mode and driving it high always would increase 
               current consumption */
            if (eGPIO_ASSERT == eGPIODrive)
            {
                /*UPD350 Reset is active low signal*/
                /* Pull down is driven to reset the UPD350*/
                PORT_PinWrite(UPD350_RESET_PIN, FALSE);
                
                /* Reset pin shall be held low for a minimum reset assertion
                   time of UPD350. We wait for ~800us */
                for(UINT16 u16delayloop = 0u; u16delayloop <(6000);u16delayloop++)
                {
                    __asm volatile("nop");
                    __asm volatile("nop");

                }                                
            }
            else
            {
                PORT_PinWrite(UPD350_RESET_PIN, TRUE);
            }
            break;
        }
        case eSPI_CHIP_SELECT_FUNC:
        {
            if (eGPIO_ASSERT == eGPIODrive)
            {
                /* Drive low the CS to enable the communication*/
                /*PORT_PIN_PA10*/
                SPI_SS_0_Clear();
            }
            else
            {
                /* Drive high the CS to disable the communication for the port*/
                SPI_SS_0_Set();
            }
            break; 
        }
        case eVBUS_DIS_FUNC:
        {
            if (eGPIO_ASSERT == eGPIODrive)
            {
//                UPDPIO_DriveHigh(u8PortNum, eUPD_PIO4);
                PORT_PinWrite(PORT_PIN_PA28, TRUE);     //SB Edit
            }
            else
            {
//                UPDPIO_DriveLow(u8PortNum, eUPD_PIO4);
                PORT_PinWrite(PORT_PIN_PA28, FALSE);     //SB Edit
            }
            break;
        }            
        case eDC_DC_EN_FUNC:
        {
            /*Not Applicable for Sink*/
            break;
        }
        case eORIENTATION_FUNC:
        {
            if (eGPIO_ASSERT == eGPIODrive)
            {
                /*Asserts when attach happens at CC1*/                
                UPDPIO_SetBufferType(u8PortNum, eUPD_PIO2,UPD_PIO_SETBUF_PUSHPULL);
                UPDPIO_DriveHigh(u8PortNum, eUPD_PIO2);
                UPDPIO_EnableOutput(u8PortNum, eUPD_PIO2);
            }
            else
            {
                /*De-assert when attach happens at CC2*/                
                UPDPIO_SetBufferType(u8PortNum,eUPD_PIO2,UPD_PIO_SETBUF_PUSHPULL);
                UPDPIO_DriveLow(u8PortNum, eUPD_PIO2); 
                UPDPIO_EnableOutput(u8PortNum, eUPD_PIO2);
            }
            break;
        }
        case eSNK_CAPS_MISMATCH_FUNC:
        {
            if (eGPIO_ASSERT == eGPIODrive)
            {
                SNK_CAP_MISMATCH_Set();
            }
            else
            {
                SNK_CAP_MISMATCH_Clear();
            }
            break;
        }
        case eSNK_1_5A_IND_FUNC:
        {
            if (eGPIO_ASSERT == eGPIODrive)
            {
                SNK_1_5A_IND_Set();
            }
            else
            {
                SNK_1_5A_IND_Clear();
            }
            break;
        }
        case eSNK_3A_IND_FUNC:
        {
            if (eGPIO_ASSERT == eGPIODrive)
            {
                SNK_3A_IND_Set();
            }
            else
            {
                SNK_3A_IND_Clear();
            }
            break;
        }
        default:
        {
            break; 
        }
    }    
}

UINT8 App_PortPowerInit(UINT8 u8PortNum)
{
    DAC_Initialize();
    
    return TRUE; 
}

#if (TRUE == INCLUDE_PD_SINK)
void App_DriveDAC_I(UINT8 u8PortNum, UINT16 u16DACData)
{
    if(PORT0 == u8PortNum)
    {
        /*SAMD20 internally divides u16DACData by 0x3FF. Hence multiplying with 0x3FF*/
        /*SAMD20 internally multiplies u16DACData by 3.3V. Hence, dividing by 3.3V*/
        /*Dividing by 1000 to convert voltage u16DACData in mV to Volt.*/

        UINT32 u32DACCalculate = u16DACData * APP_DAC_MAX_STEP_COUNT;

        u16DACData = (UINT16)(u32DACCalculate / APP_DAC_VREF);
        DAC_DataWrite(u16DACData);
    }
}
#endif
/* *****************************************************************************
 End of File
 */
