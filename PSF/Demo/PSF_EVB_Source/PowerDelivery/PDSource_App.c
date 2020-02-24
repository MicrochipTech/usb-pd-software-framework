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
void SetMCUIdle()
{
#if (TRUE == INCLUDE_POWER_MANAGEMENT_CTRL)
    MCHP_PSF_HOOK_DISABLE_GLOBAL_INTERRUPT();
    
    /*Disable Timer to avoid interrupt from Timer*/
    TC0_TimerStop(); 
    
    DEBUG_PRINT_PORT_STR (3, "Set SAMD20 to IDLE");
    
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
            #if (TRUE == INCLUDE_POWER_BALANCING)
            PB_HandlePPMEvents (u8PortNum, (UINT8)eMCHP_PSF_TYPEC_DETACH_EVENT); 
            #endif 
            break;
        }
        case eMCHP_PSF_TYPEC_CC1_ATTACH:
        {
            UPD_GPIOEnableDisable(u8PortNum,(UINT8)eUPD_PIO2,UPD_ENABLE_GPIO);
            UPD_GPIOSetDirection(u8PortNum,(UINT8)eUPD_PIO2,UPD_GPIO_SETDIR_OUTPUT);
            UPD_GPIOSetBufferType(u8PortNum,(UINT8)eUPD_PIO2,UPD_GPIO_SETBUF_PUSHPULL);
            UPD_GPIOSetClearOutput(u8PortNum,(UINT8)eUPD_PIO2,UPD_GPIO_CLEAR);
            #if (TRUE == INCLUDE_POWER_BALANCING)
            PB_HandlePPMEvents (u8PortNum, (UINT8)eMCHP_PSF_TYPEC_CC1_ATTACH); 
            #endif 
            break;
        }
        case eMCHP_PSF_TYPEC_CC2_ATTACH:
        {
            UPD_GPIOEnableDisable(u8PortNum,(UINT8)eUPD_PIO2,UPD_ENABLE_GPIO);
            UPD_GPIOSetDirection(u8PortNum,(UINT8)eUPD_PIO2,UPD_GPIO_SETDIR_OUTPUT);
            UPD_GPIOSetBufferType(u8PortNum,(UINT8)eUPD_PIO2,UPD_GPIO_SETBUF_PUSHPULL);
            UPD_GPIOSetClearOutput(u8PortNum,(UINT8)eUPD_PIO2,UPD_GPIO_SET);
            #if (TRUE == INCLUDE_POWER_BALANCING)
            PB_HandlePPMEvents (u8PortNum, (UINT8)eMCHP_PSF_TYPEC_CC2_ATTACH);
            #endif 
            break;
        }
		
		case eMCHP_PSF_UPDS_IN_IDLE:
		{
			SetMCUIdle();
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
        
        case eMCHP_PSF_PD_CONTRACT_NEGOTIATED: 
        {
            #if (TRUE == INCLUDE_POWER_BALANCING)
            PB_HandlePPMEvents (u8PortNum, (UINT8)eMCHP_PSF_PD_CONTRACT_NEGOTIATED);
            #endif 
            break; 
        }
        
        case eMCHP_PSF_GET_SINK_CAPS_NOT_RCVD: 
        {
            #if (TRUE == INCLUDE_POWER_BALANCING)
            PB_HandlePPMEvents (u8PortNum, (UINT8)eMCHP_PSF_GET_SINK_CAPS_NOT_RCVD);
            #endif  
            break; 
        }  
        
        case eMCHP_PSF_GET_SNK_CAPS_RCVD:
        {
            #if (TRUE == INCLUDE_POWER_BALANCING)
            PB_HandlePPMEvents (u8PortNum, (UINT8)eMCHP_PSF_GET_SNK_CAPS_RCVD);
            #endif  
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
