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

	/*Initialize PSF Stack and Structure version*/
    IntGlobals_StackStructVersion();
    
    /* Load configurations */
    MCHP_PSF_HOOK_BOOT_TIME_CONFIG(&gasCfgStatusData);
       
    /*Timer module Initialization*/
    u8InitStatus &= PDTimer_Init();
    
    /*Initialize HW SPI module defined by the user*/
    u8InitStatus &= MCHP_PSF_HOOK_UPDHW_INTF_INIT();
	
    for (UINT8 u8PortNum = SET_TO_ZERO; u8PortNum < CONFIG_PD_PORT_COUNT; u8PortNum++)
    {
        /*If Timer and HW module of SOC are not initialized properly disable all the ports*/
        if (TRUE != u8InitStatus)
        {
            gasCfgStatusData.sPerPortData[u8PortNum].u32CfgData &= \
                                                       ~(TYPEC_PORT_ENDIS_MASK);
        }
        #if (CONFIG_DEFINE_UPD350_HW_INTF_SEL == CONFIG_UPD350_SPI)
        /*Initialize chip select in case of SPI configuration*/
        MCHP_PSF_HOOK_GPIO_FUNC_INIT(u8PortNum, eSPI_CHIP_SELECT_FUNC);
        #endif
    }
    
    /*Since, Reset is common for all the ports. It is called only once with PORT0 as dummy value*/
    MCHP_PSF_HOOK_GPIO_FUNC_INIT(PORT0, eUPD350_RESET_FUNC);
        
	/*Initialize Internal global variables*/
    IntGlobals_PDInitialization();
    
    UPD_CheckAndDisablePorts();	

    /* VBUS threshold correction factor */
    UPD_FindVBusCorrectionFactor();
    
    #if (TRUE == CONFIG_HOOK_DEBUG_MSG)
        /*Initialize debug hardware*/
        MCHP_PSF_HOOK_DEBUG_INIT();
    #endif
    
    for (UINT8 u8PortNum = SET_TO_ZERO; u8PortNum < CONFIG_PD_PORT_COUNT; u8PortNum++)
    {
        if (UPD_PORT_ENABLED == ((gasCfgStatusData.sPerPortData[u8PortNum].u32CfgData \
                                    & TYPEC_PORT_ENDIS_MASK) >> TYPEC_PORT_ENDIS_POS))
        {
            /*Port Power Initialization*/
            PWRCTRL_initialization(u8PortNum);
        }
    }
    
    for (UINT8 u8PortNum = SET_TO_ZERO; u8PortNum < CONFIG_PD_PORT_COUNT; u8PortNum++)
    {
        if (UPD_PORT_ENABLED == ((gasCfgStatusData.sPerPortData[u8PortNum].u32CfgData \
                                    & TYPEC_PORT_ENDIS_MASK) >> TYPEC_PORT_ENDIS_POS))
        {
            /* Initialize the Port's IRQ*/
            MCHP_PSF_HOOK_GPIO_FUNC_INIT(u8PortNum, eUPD350_ALERT_FUNC);
            
            /*Initialize the Port's DC_DC Alert */
            MCHP_PSF_HOOK_GPIO_FUNC_INIT(u8PortNum, eI2C_DC_DC_ALERT_FUNC);        
        }
    }    
    
    DPM_StateMachineInit();  

    MCHP_PSF_HOOK_ENABLE_GLOBAL_INTERRUPT();
    
    return u8InitStatus;

}
/********************************************************************************************/
void MchpPSF_RUN()
{
	for (UINT8 u8PortNum = SET_TO_ZERO; u8PortNum < CONFIG_PD_PORT_COUNT; u8PortNum++)
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

