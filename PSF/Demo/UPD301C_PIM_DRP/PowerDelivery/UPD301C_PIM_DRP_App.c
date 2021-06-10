/***************************************************************************
 User Application File

  Company:
    Microchip Technology Inc.

  File Name:
    UPD301C_PIM_DRP_App.c

  Summary:
    User Application Source file

  Description:
    This source file contains user application specific functions and interfaces
************************************************************************** */
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

/* ************************************************************************** */
/* ************************************************************************** */
/* Section: Included Files                                                    */
/* ************************************************************************** */
/* ************************************************************************** */

#include "psf_stdinc.h"
#include "i2c_dc_dc_driver.h"

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
        
        case eMCHP_PSF_TYPEC_ERROR_RECOVERY: 
        {
			/*Returning TRUE to enable PSF to handle Error Recovery*/
            u8RetVal = TRUE;
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
        
        case eMCHP_PSF_PE_SRC_DISABLED:
        {
            break; 
        }
        
        case eMCHP_PSF_PD_CONTRACT_NEGOTIATED: 
        {
            break; 
        }
        
        case eMCHP_PSF_HARD_RESET_COMPLETE:
        {
            break; 
        }
        
        case eMCHP_PSF_SINK_CAPS_NOT_RCVD: 
        {
            break; 
        }  
        
        case eMCHP_PSF_SINK_CAPS_RCVD:
        {
            /* Sink Capabilities received from partner is available in 
               gasCfgStatusData.sPerPortData[u8PortNum].u32aPartnerSinkPDO array */            
            break;            
        }
        
        case eMCHP_PSF_SINK_ALERT_RCVD: 
        {
            /* Alert information received from Sink is available in 
             gasCfgStatusData.sPPSPerPortData[u8PortNum].u32PartnerAlert */
            break; 
        }
        
        case eMCHP_PSF_SINK_STATUS_NOT_RCVD:
        {
            break; 
        }

        case eMCHP_PSF_SINK_STATUS_RCVD:
        {
            /* Status information received from Sink is available in 
            gasCfgStatusData.sPPSPerPortData[u8PortNum].u8aPartnerStatus array */
            break; 
        }
        
        case eMCHP_PSF_VCONN_SWAP_COMPLETE:
        {
            break; 
        }
        
        case eMCHP_PSF_VCONN_SWAP_RCVD:
        {
            break; 
        }
        
        case eMCHP_PSF_VCONN_SWAP_NO_RESPONSE_RCVD:
        {
            break; 
        }        
                
        case eMCHP_PSF_PR_SWAP_COMPLETE:
        {
            break; 
        }
        
        case eMCHP_PSF_PR_SWAP_RCVD:
        {
            break; 
        }
        
        case eMCHP_PSF_PR_SWAP_NO_RESPONSE_RCVD:
        {
            break; 
        }
        
        case eMCHP_PSF_DR_SWAP_COMPLETE:
        {
            break; 
        }
        
        case eMCHP_PSF_DR_SWAP_RCVD:
        {
            break; 
        }
        
        case eMCHP_PSF_DR_SWAP_NO_RESPONSE_RCVD:
        {
            break; 
        }
        
        case eMCHP_PSF_CABLE_IDENTITY_DISCOVERED:
        {
            /* Cable identity received from cable is available in 
            gasCfgStatusData.sPerPortData[u8PortNum].u32aCableIdentity array */                                   
            break; 
        }
        
        case eMCHP_PSF_CABLE_IDENTITY_NAKED:
        {
            break;
        }
        
        case eMCHP_PSF_VDM_RESPONSE_RCVD:
        {
            break;
        }
        
        case eMCHP_PSF_VDM_RESPONSE_NOT_RCVD:
        {
            break; 
        }
        
        case eMCHP_PSF_VDM_REQUEST_RCVD:
        {
            u8RetVal = TRUE;
            break; 
        }
        
        case eMCHP_PSF_VDM_AMS_COMPLETE:
        {
            break; 
        }

        case eMCHP_PSF_HPD_ENABLED:
        {
            break;
        }
        
        case eMCHP_PSF_HPD_EVENT_HIGH:
        {
            break;
        }

        case eMCHP_PSF_HPD_EVENT_LOW:
        {
            break;
        }

        case eMCHP_PSF_HPD_EVENT_IRQ_HPD:
        {
            break;
        }
        
        case eMCHP_PSF_HPD_DISABLED:
        {
            break;
        }
        
        case eMCHP_PSF_ALT_MODE_ENTRY_FAILED:
        {                         
            break; 
        }
        
        case eMCHP_PSF_PORT_DISABLED:
        {
            break;
        }
        
        case eMCHP_PSF_PORT_ENABLED:
        {
            break; 
        }
        
        default:
        {
            break;
        }
    }

    return u8RetVal;
}
/**************************************************************************************/

void App_GPIOControl_Init(UINT8 u8PortNum, eMCHP_PSF_GPIO_FUNCTIONALITY eGPIOFunc)
{
    UINT16 u16Delay;
    switch(eGPIOFunc)
    {
        case eUPD350_ALERT_FUNC:
        {
            if (PORT0 == u8PortNum)
            {
                PORT_PinWrite(PORT_PIN_PA14, TRUE);
                PORT_PinInputEnable(PORT_PIN_PA14);
                EIC_CallbackRegister((EIC_PIN)PORT_PIN_PA14, SAMD20_UPD350AlertCallback, PORT0);
                EIC_InterruptEnable((EIC_PIN)PORT_PIN_PA14);
            }
            #if (CONFIG_PD_PORT_COUNT > PORT_COUNT_1)         
            else if (PORT1 == u8PortNum)
            {
                PORT_PinWrite(PORT_PIN_PA15, TRUE);
                PORT_PinInputEnable(PORT_PIN_PA15);
                EIC_CallbackRegister((EIC_PIN)PORT_PIN_PA15, SAMD20_UPD350AlertCallback, PORT1);
                EIC_InterruptEnable((EIC_PIN)PORT_PIN_PA15);
            }
            #endif
            else
            {
                /* Do Nothing */
            }
            break;
        }
        case eI2C_DC_DC_ALERT_FUNC:
        {
#if (CONFIG_DCDC_CTRL == PWRCTRL_I2C_DC_DC)
            if (PORT0 == u8PortNum)
            {
                PORT_PinWrite(PORT_PIN_PA04, TRUE);
                PORT_PinInputEnable(PORT_PIN_PA04);
                EIC_CallbackRegister((EIC_PIN)PORT_PIN_PA04, SAMD20_I2CDCDCAlertCallback, PORT0);
                EIC_InterruptEnable((EIC_PIN)PORT_PIN_PA04);
            }
            #if (CONFIG_PD_PORT_COUNT > PORT_COUNT_1)  
            else if (PORT1 == u8PortNum)
            {
                PORT_PinWrite(PORT_PIN_PA03, TRUE);
                PORT_PinInputEnable(PORT_PIN_PA03);
                EIC_CallbackRegister((EIC_PIN)PORT_PIN_PA03, SAMD20_I2CDCDCAlertCallback, PORT1);
                EIC_InterruptEnable((EIC_PIN)PORT_PIN_PA03);
            }
            #endif
            else
            {
                /* Do Nothing */
            }
#endif 
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
            if (PORT0 == u8PortNum)
            {
                SPI_SS_0_Set();
                SPI_SS_0_OutputEnable();
            }
            #if (CONFIG_PD_PORT_COUNT > PORT_COUNT_1)  
            else if(PORT1 == u8PortNum)
            {
                SPI_SS_1_Set();
                SPI_SS_1_OutputEnable();
            }
            #endif
            else
            {
                /* Do Nothing */
            }
            break; 
        }
        case eVBUS_DIS_FUNC:
        {
            if (PORT0 == u8PortNum)
            {
                PORT_PinWrite(PORT_PIN_PA28, FALSE);
                PORT_PinOutputEnable(PORT_PIN_PA28);        
            }
            else if(PORT1 == u8PortNum)
            {
                UPDPIO_SetBufferType(u8PortNum, eUPD_PIO4, UPD_PIO_SETBUF_PUSHPULL);
                UPDPIO_DriveLow(u8PortNum, eUPD_PIO4);
                UPDPIO_EnableOutput(u8PortNum, eUPD_PIO4);
            }
            else
            {
                /* Do Nothing */
            }
            break; 
        }
        case eDC_DC_EN_FUNC:
        {
            if (PORT0 == u8PortNum)
            {
                /*DC_DC_EN will be cleared initially. 
                 It will be set when DRP acts as source*/
                DC_DC_EN_0_Clear();
                DC_DC_EN_0_OutputEnable();
            }
            #if (CONFIG_PD_PORT_COUNT > PORT_COUNT_1) 
            else if(PORT1 == u8PortNum)
            {
                /*DC_DC_EN will be cleared initially. 
                It will be set when DRP acts as source*/
                DC_DC_EN_1_Clear();
                DC_DC_EN_1_OutputEnable();
            }
            #endif
            else
            {
                /* Do Nothing */
            }
             
             /* Delay for the DC/DC module to stabilize after Initialization */
            for(u16Delay = 0; u16Delay < 20000; u16Delay++)
            {
                __NOP();
            }   

            break; 
        }
        case eORIENTATION_FUNC:
        {
            /*Init is called when detach happens*/
            UPDPIO_Disable (u8PortNum, eUPD_PIO2);       
            break;
        }
        case eSNK_CAPS_MISMATCH_FUNC: 
        {
            if (PORT0 == u8PortNum)
            {
                SNK_CAP_MISMATCH_Clear();
                SNK_CAP_MISMATCH_OutputEnable();  
            }
            else
            {
                /*Do nothing since sink specific IOs are applicable only for port 0*/
            }
            break;
        }
//        case eSNK_1_5A_IND_FUNC:
//        {
//            if (PORT0 == u8PortNum)
//            {
//                SNK_1_5A_IND_Clear();
//                SNK_1_5A_IND_OutputEnable();            
//            }
//            else
//            {
//                /*Do nothing since sink specific IOs are applicable only for port 0*/
//            }
//            break;
//        }
//        case eSNK_3A_IND_FUNC:
//        {
//            if (PORT0 == u8PortNum)
//            {
//                SNK_3A_IND_Clear();
//                SNK_3A_IND_OutputEnable();  
//            }
//            else
//            {
//                /*Do nothing since sink specific IOs are applicable only for port 0*/
//            }
//            break;
//        }    
        case ePOWER_ROLE_FUNC:
        {
#if (FALSE == CONFIG_HOOK_DEBUG_MSG)
            if (PORT0 == u8PortNum)
            {
                POWER_ROLE_0_Clear();
                POWER_ROLE_0_OutputEnable();  
            }
#endif
#if (CONFIG_DCDC_CTRL == PWRCTRL_GPIO_DC_DC)
            #if (CONFIG_PD_PORT_COUNT > PORT_COUNT_1)  
            else if (PORT1 == u8PortNum)
            {
                POWER_ROLE_1_Clear();
                POWER_ROLE_1_OutputEnable(); 
            }
            #endif  
#endif
            else
            {
                /*Do nothing*/
            }            
            break;
        }     
        case eDATA_ROLE_FUNC:
        {
#if (FALSE == CONFIG_HOOK_DEBUG_MSG)
            if (PORT0 == u8PortNum)
            {
                DATA_ROLE_0_Clear();
                DATA_ROLE_0_OutputEnable();  
            }
#endif
#if (CONFIG_DCDC_CTRL == PWRCTRL_GPIO_DC_DC)
            #if (CONFIG_PD_PORT_COUNT > PORT_COUNT_1)  
            else if (PORT1 == u8PortNum)
            {
                DATA_ROLE_1_Clear();
                DATA_ROLE_1_OutputEnable(); 
            }
            #endif
#endif
            else
            {
                /*Do nothing*/
            }            
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
            /*DC-DC Alert is an input pin. Drive not applicable*/
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
                if (PORT0 == u8PortNum)
                {
                    /*PORT_PIN_PA10*/
                    SPI_SS_0_Clear();
                }
                #if (CONFIG_PD_PORT_COUNT > PORT_COUNT_1) 
                else if(PORT1 == u8PortNum)
                {
                    /*PORT_PIN_PA01*/
                    SPI_SS_1_Clear();
                } 
                #endif
                else
                {
                    /* Do Nothing */
                }
            }
            else
            {
                /* Drive high the CS to disable the communication for the port*/
                if (PORT0 == u8PortNum)
                {
                    SPI_SS_0_Set();
                }
                #if (CONFIG_PD_PORT_COUNT > PORT_COUNT_1)  
                else if(PORT1 == u8PortNum)
                {
                    SPI_SS_1_Set();
                }
                #endif
                else
                {
                    /* Do Nothing */
                }
            }
            break; 
        }
        case eVBUS_DIS_FUNC:
        {
            if (eGPIO_ASSERT == eGPIODrive)
            {
                if (PORT0 == u8PortNum)
                {
                    PORT_PinWrite(PORT_PIN_PA28, TRUE);
                }
                else if (PORT1 == u8PortNum)
                {
                    UPDPIO_DriveHigh(u8PortNum, eUPD_PIO4);
                }
                else
                {
                    /* Do Nothing */
                }
            }
            else
            {
                if (PORT0 == u8PortNum)
                {
                    PORT_PinWrite(PORT_PIN_PA28, FALSE);
                }
                else if (PORT1 == u8PortNum)
                {
                    UPDPIO_DriveLow(u8PortNum, eUPD_PIO4);
                }
                else
                {
                    /* Do Nothing */
                }                
            }
            break; 
        }
        case eDC_DC_EN_FUNC:
        {
            if (eGPIO_ASSERT == eGPIODrive)
            {
                if (PORT0 == u8PortNum)
                {
                    DC_DC_EN_0_Set();
                }
                #if (CONFIG_PD_PORT_COUNT > PORT_COUNT_1)  
                else if(PORT1 == u8PortNum)
                {
                    DC_DC_EN_1_Set();
                }
                #endif
                else
                {
                    /* Do Nothing */
                }
            }
            else
            {
                if (PORT0 == u8PortNum)
                {
                    DC_DC_EN_0_Clear();
                }
                #if (CONFIG_PD_PORT_COUNT > PORT_COUNT_1)  
                else if(PORT1 == u8PortNum)
                {
                    DC_DC_EN_1_Clear();
                }
                #endif
                else
                {
                    /* Do Nothing */
                }
            }
            break;
        }
        case eORIENTATION_FUNC:  
        {
            if (eGPIO_ASSERT == eGPIODrive)
            {
                UPDPIO_SetBufferType (u8PortNum, eUPD_PIO2, UPD_PIO_SETBUF_PUSHPULL);
                UPDPIO_DriveHigh (u8PortNum, eUPD_PIO2);
                UPDPIO_EnableOutput (u8PortNum, eUPD_PIO2);
            }
            else
            {
                UPDPIO_SetBufferType (u8PortNum, eUPD_PIO2, UPD_PIO_SETBUF_PUSHPULL);
                UPDPIO_DriveLow (u8PortNum, eUPD_PIO2); 
                UPDPIO_EnableOutput (u8PortNum, eUPD_PIO2);
            }
            break;  
        }
        case eSNK_CAPS_MISMATCH_FUNC: 
        {
            if (PORT0 == u8PortNum)
            {
                if (eGPIO_ASSERT == eGPIODrive)
                {
                    SNK_CAP_MISMATCH_Set();
                }
                else
                {
                    SNK_CAP_MISMATCH_Clear();
                }
            }
            else
            {
                /*Do nothing since sink specific IOs are applicable only for port 0*/
            }
            break;
        }
//        case eSNK_1_5A_IND_FUNC:
//        {
//            if (PORT0 == u8PortNum)
//            {
//                if (eGPIO_ASSERT == eGPIODrive)
//                {
//                    SNK_1_5A_IND_Set();
//                }
//                else
//                {
//                    SNK_1_5A_IND_Clear();
//                }
//            }
//            else
//            {
//                /*Do nothing since sink specific IOs are applicable only for port 0*/
//            }
//            break;
//        }
//        case eSNK_3A_IND_FUNC:
//        {
//            if (PORT0 == u8PortNum)
//            {
//                if (eGPIO_ASSERT == eGPIODrive)
//                {
//                    SNK_3A_IND_Set();
//                }
//                else
//                {
//                    SNK_3A_IND_Clear();
//                }
//            }
//            else
//            {
//                /*Do nothing since sink specific IOs are applicable only for port 0*/
//            }
//            break;
//        }
        case ePOWER_ROLE_FUNC:
        {
#if (FALSE == CONFIG_HOOK_DEBUG_MSG)
            if (PORT0 == u8PortNum)
            {
                if (eGPIO_ASSERT == eGPIODrive)
                {
                    POWER_ROLE_0_Set();
                }
                else
                {
                    POWER_ROLE_0_Clear();
                }
            }
#endif
#if (CONFIG_DCDC_CTRL == PWRCTRL_GPIO_DC_DC)
            #if (CONFIG_PD_PORT_COUNT > PORT_COUNT_1)  
            else if (PORT1 == u8PortNum)
            {
                if (eGPIO_ASSERT == eGPIODrive)
                {
                    POWER_ROLE_1_Set();
                }
                else
                {
                    POWER_ROLE_1_Clear();
                }            
            }
            #endif
#endif
            else
            {
                /*Do nothing*/
            }            
            break;
        }
        case eDATA_ROLE_FUNC:
        {
#if (FALSE == CONFIG_HOOK_DEBUG_MSG)
            if (PORT0 == u8PortNum)
            {
                if (eGPIO_ASSERT == eGPIODrive)
                {
                    DATA_ROLE_0_Set();
                }
                else
                {
                    DATA_ROLE_0_Clear();
                }
            }
#endif
#if(CONFIG_DCDC_CTRL == PWRCTRL_GPIO_DC_DC)
            #if (CONFIG_PD_PORT_COUNT > PORT_COUNT_1)
            else if (PORT1 == u8PortNum)
            {
                if (eGPIO_ASSERT == eGPIODrive)
                {
                    DATA_ROLE_1_Set();
                }
                else
                {
                    DATA_ROLE_1_Clear();
                }            
            }
            #endif
#endif
            else
            {
                /*Do nothing*/
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
    UINT8 u8Return; 
   
#if (TRUE == INCLUDE_PD_SINK) 
    DAC_Initialize();
#endif
	
#if (CONFIG_DCDC_CTRL == PWRCTRL_GPIO_DC_DC)
    
    /*VSEL0 Init */
    UPDPIO_SetBufferType(u8PortNum, eUPD_PIO7, UPD_PIO_SETBUF_PUSHPULL);
    UPDPIO_DriveLow(u8PortNum, eUPD_PIO7);
    UPDPIO_EnableOutput(u8PortNum, eUPD_PIO7);
    
    /*VSEL 1 Init */
    if (PORT0 == u8PortNum)
    {
        PORT_PinWrite(PORT_PIN_PA05, FALSE);
        PORT_PinOutputEnable(PORT_PIN_PA05);   
    }
    else if(PORT1 == u8PortNum)
    {                       
        UPDPIO_SetBufferType(u8PortNum, eUPD_PIO8, UPD_PIO_SETBUF_PUSHPULL);
        UPDPIO_DriveLow(u8PortNum, eUPD_PIO8);
        UPDPIO_EnableOutput(u8PortNum, eUPD_PIO8);
    }
    else
    {
        /* Do Nothing */
    }                

    /*VSEL 2 Init */
    if (PORT0 == u8PortNum)
    {
        PORT_PinWrite(PORT_PIN_PA02, FALSE);
        PORT_PinOutputEnable(PORT_PIN_PA02);   
    }
    else if(PORT1 == u8PortNum)
    {                       
        UPDPIO_SetBufferType(u8PortNum, eUPD_PIO9, UPD_PIO_SETBUF_PUSHPULL);
        UPDPIO_DriveLow(u8PortNum, eUPD_PIO9);
        UPDPIO_EnableOutput(u8PortNum, eUPD_PIO9);
    }
    else
    {
        /* Do Nothing */
    }                
    u8Return = TRUE; 
    
#elif (CONFIG_DCDC_CTRL == PWRCTRL_I2C_DC_DC)
    u8Return = MPQDCDC_Initialize(u8PortNum); /* MPQ4230 - I2C based DC/DC */ 
#endif 
    
    return u8Return; 
}

void App_PortPowerSetPower(UINT8 u8PortNum, UINT16 u16Voltage, UINT16 u16Current)
{
#if (CONFIG_DCDC_CTRL == PWRCTRL_GPIO_DC_DC)
    /*
     Voltage    VSEL0   VSEL1   VSEL2
     *0V        0       0       0
     *5V        0       0       0
     *9V        1       0       0
     *15V       0       1       0
     *20V       0       0       1
     */  
    /* Update Port IO Status */
    gasCfgStatusData.sPerPortData[u8PortNum].u32PortIOStatus &= 
              ~(DPM_PORT_IO_VSEL0_STATUS | DPM_PORT_IO_VSEL1_STATUS | DPM_PORT_IO_VSEL2_STATUS);             
    
    switch(u16Voltage)
    {
        case APP_VOLTAGE_5000mV:
        {
            UPDPIO_DriveLow(u8PortNum, eUPD_PIO7);
            (PORT0 == u8PortNum) ? (PORT_PinWrite(PORT_PIN_PA05, FALSE)): (UPDPIO_DriveLow(u8PortNum, eUPD_PIO8)); 
            (PORT0 == u8PortNum) ? (PORT_PinWrite(PORT_PIN_PA02, FALSE)): (UPDPIO_DriveLow(u8PortNum, eUPD_PIO9)); 
            break;
        }
        case APP_VOLTAGE_9000mV:
        {
            UPDPIO_DriveHigh(u8PortNum, eUPD_PIO7);
            (PORT0 == u8PortNum) ? (PORT_PinWrite(PORT_PIN_PA05, FALSE)): (UPDPIO_DriveLow(u8PortNum, eUPD_PIO8)); 
            (PORT0 == u8PortNum) ? (PORT_PinWrite(PORT_PIN_PA02, FALSE)): (UPDPIO_DriveLow(u8PortNum, eUPD_PIO9)); 
            /* Update Port IO Status */
            gasCfgStatusData.sPerPortData[u8PortNum].u32PortIOStatus |= DPM_PORT_IO_VSEL0_STATUS;
            break;
        }
        case APP_VOLTAGE_15000mV:
        {
            UPDPIO_DriveLow(u8PortNum, eUPD_PIO7);
            (PORT0 == u8PortNum) ? (PORT_PinWrite(PORT_PIN_PA05, TRUE)): (UPDPIO_DriveHigh(u8PortNum, eUPD_PIO8)); 
            (PORT0 == u8PortNum) ? (PORT_PinWrite(PORT_PIN_PA02, FALSE)): (UPDPIO_DriveLow(u8PortNum, eUPD_PIO9)); 
            /* Update Port IO Status */
            gasCfgStatusData.sPerPortData[u8PortNum].u32PortIOStatus |= DPM_PORT_IO_VSEL1_STATUS;
            break;
        }
        case APP_VOLTAGE_20000mV:
        {
            UPDPIO_DriveLow(u8PortNum, eUPD_PIO7);
            (PORT0 == u8PortNum) ? (PORT_PinWrite(PORT_PIN_PA05, FALSE)): (UPDPIO_DriveLow(u8PortNum, eUPD_PIO8)); 
            (PORT0 == u8PortNum) ? (PORT_PinWrite(PORT_PIN_PA02, TRUE)): (UPDPIO_DriveHigh(u8PortNum, eUPD_PIO9)); 
            /* Update Port IO Status */
            gasCfgStatusData.sPerPortData[u8PortNum].u32PortIOStatus |= DPM_PORT_IO_VSEL2_STATUS;
            break;
        }
        default:
        /*Intentionally break is left*/
        case APP_VOLTAGE_0mV:
        {
            UPDPIO_DriveLow(u8PortNum, eUPD_PIO7);
            (PORT0 == u8PortNum) ? (PORT_PinWrite(PORT_PIN_PA05, FALSE)): (UPDPIO_DriveLow(u8PortNum, eUPD_PIO8)); 
            (PORT0 == u8PortNum) ? (PORT_PinWrite(PORT_PIN_PA02, FALSE)): (UPDPIO_DriveLow(u8PortNum, eUPD_PIO9)); 
            break;
        }
    }
    
#elif (CONFIG_DCDC_CTRL == PWRCTRL_I2C_DC_DC)/* MPQ4230 - I2C based DC/DC */ 
    MPQDCDC_SetPortPower(u8PortNum, u16Voltage, u16Current);
#endif     
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
