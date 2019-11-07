/***************************************************************************
 User Application File

  Company:
    Microchip Technology Inc.

  File Name:
    User_App.c

  Summary:
    User Application Source file

  Description:
    This source file contains user application specific functions and interfaces
************************************************************************** */
/* ************************************************************************** */
/* ************************************************************************** */
/* Section: Included Files                                                    */
/* ************************************************************************** */
/* ************************************************************************** */

#include "psf_stdinc.h"
#include "Drivers.h"
#include "PDSource_App.h"

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
void SetMCUIdle()
{
#if INCLUDE_POWER_MANAGEMENT_CTRL
    MCHP_PSF_HOOK_DISABLE_GLOBAL_INTERRUPT();
    
    /*Disable Timer to avoid interrupt from Timer*/
    TC0_TimerStop(); 
    
    DEBUG_PRINT_PORT_STR (3, "UPD301: Set UPD301 IDLE");
    
	/*If there is any pending interrupt it will not go to sleep*/
    SCB->SCR |=  (SCB_SCR_SLEEPDEEP_Msk )| (SCB_SCR_SEVONPEND_Msk);
    
    __DSB();
    __WFI();
    
    MCHP_PSF_HOOK_ENABLE_GLOBAL_INTERRUPT();
    
    /* Resume from Idle*/
    /* Enable the disabled interrupt*/
    TC0_TimerStart();
#endif
}

/* ************************************************************************** */
/* ************************************************************************** */
// Section: Interface Functions                                               */
/* ************************************************************************** */
/* ************************************************************************** */

UINT8 PDStack_Events(UINT8 u8PortNum, UINT8 u8PDEvent)
{
    UINT8 u8RetVal = FALSE;
    
    switch(u8PDEvent)
    {
        case eMCHP_PSF_TYPEC_DETACH_EVENT:
        {
            UPD_GPIOEnableDisable(u8PortNum,(UINT8)eUPD_PIO2, UPD_DISABLE_GPIO);
            break;
        }
        case eMCHP_PSF_TYPEC_CC1_ATTACH:
        {
            UPD_GPIOEnableDisable(u8PortNum,eUPD_PIO2,UPD_ENABLE_GPIO);
            UPD_GPIOSetDirection(u8PortNum,eUPD_PIO2,UPD_GPIO_SETDIR_OUTPUT);
            UPD_GPIOSetBufferType(u8PortNum,eUPD_PIO2,UPD_GPIO_SETBUF_PUSHPULL);
            UPD_GPIOSetClearOutput(u8PortNum,eUPD_PIO2,UPD_GPIO_CLEAR);
            break;
        }
        case eMCHP_PSF_TYPEC_CC2_ATTACH:
        {
            UPD_GPIOEnableDisable(u8PortNum,eUPD_PIO2,UPD_ENABLE_GPIO);
            UPD_GPIOSetDirection(u8PortNum,eUPD_PIO2,UPD_GPIO_SETDIR_OUTPUT);
            UPD_GPIOSetBufferType(u8PortNum,eUPD_PIO2,UPD_GPIO_SETBUF_PUSHPULL);
            UPD_GPIOSetClearOutput(u8PortNum,eUPD_PIO2,UPD_GPIO_SET);
            break;
        }
		
		case eMCHP_PSF_UPDS_IN_IDLE:
		{
			SetMCUIdle();
			break;
		}
        
        case eMCHP_PSF_VCONN_PWR_FAULT:
        {
            u8RetVal = MCHP_PSF_HANDLE_PWR_FAULT;
            break;
        }
        
        case eMCHP_PSF_VBUS_PWR_FAULT:
        {
            u8RetVal = MCHP_PSF_HANDLE_PWR_FAULT;
            break;
        }
        
        default:
            break;
    }

    return u8RetVal;
}
/**************************************************************************************/


/* *****************************************************************************
 End of File
 */
