/*******************************************************************************
 PSF stack main source file

  Company:
    Microchip Technology Inc.

  File Name:
    pd_main.c

  Description:
    This file contains the "main" and initialization function for PSF.  
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

/*******************************************************************/
/******************* Functions**************************************/
/*******************************************************************/

UINT8 MchpPSF_Init(void)
{
    UINT8 u8InitStatus = TRUE;
    
    /* Load default configurations */
    CFG_LoadDefaults();
       
    /*Timer module Initialization*/
    u8InitStatus &= PDTimer_Init();
    
    /*Initialize HW SPI module defined by the user*/
    u8InitStatus &= MCHP_PSF_HOOK_UPDHW_INTF_INIT();
	
#if (CONFIG_DCDC_CTRL == I2C_DC_DC_CONTROL_CONFIG)
    /*Initialize HW I2C module interface defined by the user*/
    (void)MCHP_PSF_HOOK_UPDI2C_DCDC_INTF_INIT();
#endif    
    
    for (UINT8 u8PortNum = 0; u8PortNum < CONFIG_PD_PORT_COUNT; u8PortNum++)
    {
        /*If Timer and HW module of SOC are not initialized properly disable all the ports*/
        if (TRUE != u8InitStatus)
        {
            gasCfgStatusData.sPerPortData[u8PortNum].u32CfgData &= \
                                                       ~(TYPEC_PORT_ENDIS_MASK);
        }
        /*UPD350 Reset GPIO Init*/
        MCHP_PSF_HOOK_UPD_RESET_GPIO_INIT(u8PortNum);
    }

    MCHP_PSF_HOOK_BOOT_TIME_CONFIG(&gasCfgStatusData);  
        
	/*Initialize Internal global variables*/
    IntGlobals_PDInitialization();
    
    UPD_CheckAndDisablePorts();	

    /* VBUS threshold correction factor */
    UPD_FindVBusCorrectionFactor();
    
    #if CONFIG_HOOK_DEBUG_MSG
    /*Initialize debug hardware*/
    MCHP_PSF_HOOK_DEBUG_INIT();
    #endif
    
    MCHP_PSF_HOOK_DISABLE_GLOBAL_INTERRUPT();
    
    
    for (UINT8 u8PortNum = 0; u8PortNum < CONFIG_PD_PORT_COUNT; u8PortNum++)
    {
        if (UPD_PORT_ENABLED == ((gasCfgStatusData.sPerPortData[u8PortNum].u32CfgData \
                                    & TYPEC_PORT_ENDIS_MASK) >> TYPEC_PORT_ENDIS_POS))
        {
            /*User defined UPD Interrupt Initialization for MCU*/
            MCHP_PSF_HOOK_UPD_IRQ_GPIO_INIT(u8PortNum);
            
            /*Port Power Initialization*/
            PWRCTRL_initialization(u8PortNum);
        }
    }
    
#if (CONFIG_DCDC_CTRL == I2C_DC_DC_CONTROL_CONFIG)
    for (UINT8 u8PortNum = 0; u8PortNum < CONFIG_PD_PORT_COUNT; u8PortNum++)
    {
        if (UPD_PORT_ENABLED == ((gasCfgStatusData.sPerPortData[u8PortNum].u32CfgData \
                                    & TYPEC_PORT_ENDIS_MASK) >> TYPEC_PORT_ENDIS_POS))
        {
            (void)MCHP_PSF_HOOK_I2CDCDCAlertInit(u8PortNum);
            (void)MCHP_PSF_HOOK_I2CDCDC_CONTROLLER_INIT(u8PortNum);

        }
    }
#endif
	    
    DPM_StateMachineInit();  

    MCHP_PSF_HOOK_ENABLE_GLOBAL_INTERRUPT();
    
    return u8InitStatus;

}
/********************************************************************************************/
void MchpPSF_RUN()
{
	for (UINT8 u8PortNum = 0; u8PortNum < CONFIG_PD_PORT_COUNT; u8PortNum++)
  	{
        if (UPD_PORT_ENABLED == ((gasCfgStatusData.sPerPortData[u8PortNum].u32CfgData \
                                  & TYPEC_PORT_ENDIS_MASK) >> TYPEC_PORT_ENDIS_POS))
        {
           DPM_RunStateMachine (u8PortNum);
        }
	}
}
/*********************************************************************************************/

/*********************************************************************************************/
void MchpPSF_UPDIrqHandler(UINT8 u8PortNum)
{
    UPDIntr_AlertHandler(u8PortNum);
}
/*********************************************************************************************/

/*********************************************************************************************/
void MchpPSF_PDTimerHandler(void)
{
    PDTimer_InterruptHandler();
}
/*********************************************************************************************/

