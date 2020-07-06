/*************************************************************************** 
  User Configuration File

  Company:
    Microchip Technology Inc.

  File Name:
    PSFDRP_BootCfg.c

  Summary:
    User Configuration Source file

  Description:
    This source file contains user configurable globals and functions.

************************************************************************** */

/* ************************************************************************** */
/* ************************************************************************** */
/* Section: Included Files                                                    */
/* ************************************************************************** */
/* ************************************************************************** */
#include <psf_stdinc.h>
#include <string.h>

/*******************************************************************/
/******************* Global variables********************************/
/*******************************************************************/

/* ************************************************************************** */
/* ************************************************************************** */
// Section: Local Functions                                                   */
/* ************************************************************************** */
/* ************************************************************************** */
static void CFG_PerPortParams (UINT8 u8PortNum, GLOBAL_CFG_STATUS_DATA *pasCfgStatusData);
static void CFG_PBPerPortParams (UINT8 u8PortNum, GLOBAL_CFG_STATUS_DATA *pasCfgStatusData);

static void CFG_PerPortParams (UINT8 u8PortNum, GLOBAL_CFG_STATUS_DATA *pasCfgStatusData)
{    
    pasCfgStatusData->sPerPortData[u8PortNum].u32CfgData =       
                    ((CFG_VCONN_OCS_ENABLE) | (CFG_PORT_ENABLE) | (CFG_PORT_RP_CURRENT_VALUE)| \
                    (CFG_PORT_POWER_ROLE));
    
    /* PDO 1: Fixed PDO with 5V,3A capability */
    pasCfgStatusData->sPerPortData[u8PortNum].u32aSourcePDO[0] = CFG_FORM_FIXED_PDO1(CFG_PORT_SOURCE_PDO_1_VOLTAGE, \
            CFG_PORT_SOURCE_PDO_1_CURRENT, CFG_PORT_SOURCE_USB_COMM, CFG_PORT_SOURCE_USB_SUSP,  \
            CFG_PORT_SOURCE_UNCONSTARINED_PWR);    

    /* PDO 2: Fixed PDO with 9V,3A capability */
    pasCfgStatusData->sPerPortData[u8PortNum].u32aSourcePDO[1] = CFG_FORM_FIXED_PDOx(CFG_PORT_SOURCE_PDO_2_VOLTAGE, \
                                                    CFG_PORT_SOURCE_PDO_2_CURRENT);        

    /* PDO 3: Fixed PDO with 15V,3A capability */
    pasCfgStatusData->sPerPortData[u8PortNum].u32aSourcePDO[2] = CFG_FORM_FIXED_PDOx(CFG_PORT_SOURCE_PDO_3_VOLTAGE, \
                                                    CFG_PORT_SOURCE_PDO_3_CURRENT);     

    /* PDO 4: Fixed PDO with 20V,3A capability */
    pasCfgStatusData->sPerPortData[u8PortNum].u32aSourcePDO[3] = CFG_FORM_FIXED_PDOx(CFG_PORT_SOURCE_PDO_4_VOLTAGE, \
                                                    CFG_PORT_SOURCE_PDO_4_CURRENT);         

    /* PDO 5: PPS APDO with 3.3V-21V, 3A capability */
    pasCfgStatusData->sPerPortData[u8PortNum].u32aSourcePDO[4] = CFG_FORM_PPS_APDO(CFG_POWER_SUPPLY_TYPE_PROGRAMMABLE, \
                        CFG_PORT_SOURCE_APDO_5_MIN_VOLTAGE, CFG_PORT_SOURCE_APDO_5_MAX_VOLTAGE, \
                        CFG_PORT_SOURCE_APDO_5_MAX_CURRENT, CFG_PORT_SOURCE_APDO_5_PPS_PWR_LTD);            
    
    pasCfgStatusData->sPerPortData[u8PortNum].u8SourcePDOCnt = CFG_PORT_SOURCE_NUM_OF_PDOS;

 pasCfgStatusData->sPerPortData[u8PortNum].u32aSinkPDO[0] = CFG_FORM_SINK_FIXED_PDO1(CFG_PORT_SINK_PDO_1_CURRENT, \
            CFG_PORT_SINK_PDO_1_VOLTAGE, CFG_PORT_SINK_USB_COMM, \
            CFG_PORT_SINK_UNCONSTRAINED_PWR,CFG_PORT_SINK_HIGHER_CAPABILITY);    

    pasCfgStatusData->sPerPortData[u8PortNum].u32aSinkPDO[1] = CFG_FORM_FIXED_PDOx(CFG_PORT_SINK_PDO_2_VOLTAGE, \
                                                    CFG_PORT_SINK_PDO_2_CURRENT);        

    pasCfgStatusData->sPerPortData[u8PortNum].u32aSinkPDO[2] = CFG_FORM_FIXED_PDOx(CFG_PORT_SINK_PDO_3_VOLTAGE, 
                                                    CFG_PORT_SINK_PDO_3_CURRENT);     

    pasCfgStatusData->sPerPortData[u8PortNum].u32aSinkPDO[3] = CFG_FORM_FIXED_PDOx(CFG_PORT_SINK_PDO_4_VOLTAGE, 
                                                    CFG_PORT_SINK_PDO_4_CURRENT);         \

    pasCfgStatusData->sPerPortData[u8PortNum].u32aSinkPDO[4] = CFG_FORM_FIXED_PDOx(CFG_PORT_SINK_PDO_5_VOLTAGE, 
                                                    CFG_PORT_SINK_PDO_5_CURRENT);            \

    pasCfgStatusData->sPerPortData[u8PortNum].u32aSinkPDO[5] = CFG_FORM_FIXED_PDOx(CFG_PORT_SINK_PDO_6_VOLTAGE, 
                                                    CFG_PORT_SINK_PDO_6_CURRENT);            \
    
    pasCfgStatusData->sPerPortData[u8PortNum].u32aSinkPDO[6] = CFG_FORM_FIXED_PDOx(CFG_PORT_SINK_PDO_7_VOLTAGE, 
                                                    CFG_PORT_SINK_PDO_7_CURRENT); 
    
    pasCfgStatusData->sPerPortData[u8PortNum].u8SinkPDOCnt = CFG_PORT_SINK_NUM_OF_PDOS;

#if (TRUE == INCLUDE_PD_SINK)
    /*Assigning PDO preferred minimum current*/
    pasCfgStatusData->sPerPortData[u8PortNum].u16aMinPDOPreferredCurInmA[0] = CFG_PORT_SINK_PDO_1_PREFERRED_MIN_CURRENT;
    pasCfgStatusData->sPerPortData[u8PortNum].u16aMinPDOPreferredCurInmA[1] = CFG_PORT_SINK_PDO_2_PREFERRED_MIN_CURRENT;
    pasCfgStatusData->sPerPortData[u8PortNum].u16aMinPDOPreferredCurInmA[2] = CFG_PORT_SINK_PDO_3_PREFERRED_MIN_CURRENT;
    pasCfgStatusData->sPerPortData[u8PortNum].u16aMinPDOPreferredCurInmA[3] = CFG_PORT_SINK_PDO_4_PREFERRED_MIN_CURRENT;
    pasCfgStatusData->sPerPortData[u8PortNum].u16aMinPDOPreferredCurInmA[4] = CFG_PORT_SINK_PDO_5_PREFERRED_MIN_CURRENT;
    pasCfgStatusData->sPerPortData[u8PortNum].u16aMinPDOPreferredCurInmA[5] = CFG_PORT_SINK_PDO_6_PREFERRED_MIN_CURRENT;
    pasCfgStatusData->sPerPortData[u8PortNum].u16aMinPDOPreferredCurInmA[6] = CFG_PORT_SINK_PDO_7_PREFERRED_MIN_CURRENT;

    /*Assigning maximum operating current and minimum operating current to 
     * 3000mA and 1000mA respectively*/
    pasCfgStatusData->sPerPortData[u8PortNum].u16SnkMaxOperatingCurInmA = CFG_PORT_SINK_MAX_OPERATING_CURRENT_InmA;
    pasCfgStatusData->sPerPortData[u8PortNum].u16SnkMinOperatingCurInmA = CFG_PORT_SINK_MIN_OPERATING_CURRENT_InmA;
#endif

    pasCfgStatusData->sPerPortData[u8PortNum].u8OCSThresholdPercentage = 0x00;
    pasCfgStatusData->sPerPortData[u8PortNum].u8FaultInDebounceInms = CFG_FAULT_IN_OCS_DEBOUNCE_MS;
    pasCfgStatusData->sPerPortData[u8PortNum].u8OVThresholdPercentage = CFG_OVER_VOLTAGE_FACTOR;
    pasCfgStatusData->sPerPortData[u8PortNum].u8UVThresholdPercentage = CFG_UNDER_VOLTAGE_FACTOR;
    pasCfgStatusData->sPerPortData[u8PortNum].u8VCONNOCSDebounceInms = CFG_VCONN_OCS_DEBOUNCE_IN_MS;
    pasCfgStatusData->sPerPortData[u8PortNum].u8VBUSMaxFaultCnt = CFG_MAX_VBUS_POWER_FAULT_COUNT;
    pasCfgStatusData->sPerPortData[u8PortNum].u8VCONNMaxFaultCnt = CFG_MAX_VCONN_FAULT_COUNT;
    pasCfgStatusData->sPerPortData[u8PortNum].u16PowerGoodTimerInms = MILLISECONDS_TO_TICKS(CFG_POWER_GOOD_TIMER_MS);
    pasCfgStatusData->sPerPortData[u8PortNum].u16MaxSrcPrtCurrentIn10mA = CFG_MAX_PORT_CURRENT_IN_10mA; 
    pasCfgStatusData->sPerPortData[u8PortNum].u16FeatureSelect = CFG_PB_PORT_ENABLE; 
    
#if (TRUE == INCLUDE_PD_SOURCE)
    pasCfgStatusData->sPerPortData[u8PortNum].u8Pio_EN_VBUS = (UINT8)CFG_PORT_UPD_EN_VBUS;
    pasCfgStatusData->sPerPortData[u8PortNum].u8Mode_EN_VBUS = (UINT8)CFG_PORT_UPD_EN_VBUS_PIO_MODE;
#endif
    pasCfgStatusData->sPerPortData[u8PortNum].u8Pio_FAULT_IN = (UINT8)CFG_PORT_UPD_FAULT_IN_PIO_NO;
    pasCfgStatusData->sPerPortData[u8PortNum].u8Mode_FAULT_IN = (UINT8)CFG_PORT_UPD_FAULT_IN_MODE;
#if (TRUE == INCLUDE_PD_SINK)    
    pasCfgStatusData->sPerPortData[u8PortNum].u8Pio_EN_SINK  = (UINT8)CFG_PORT_UPD_EN_SNK;
    pasCfgStatusData->sPerPortData[u8PortNum].u8Mode_EN_SINK  = (UINT8)CFG_PORT_UPD_EN_SNK_PIO_MODE;
    
	pasCfgStatusData->sPerPortData[u8PortNum].u8SinkConfigSel = ((CFG_PORT_SINK_MODE_A)| \
                                                                 (CFG_PORT_SINK_USB_SUSP << DPM_SINK_CONFIG_NO_USB_SUSP_POS) |\
                                                                 (CFG_PORT_SINK_GIVE_BACK_FLAG << DPM_SINK_CONFIG_GIVE_BACK_FLAG_POS));
    
    pasCfgStatusData->sPerPortData[u8PortNum].u16DAC_I_CurrentInd_MaxInA = CFG_PORT_SINK_DAC_I_CUR_INDICATION_MAX;
    pasCfgStatusData->sPerPortData[u8PortNum].u16DAC_I_MaxOutVoltInmV = CFG_PORT_SINK_DAC_I_MAX_OP_VOLTAGE;
    pasCfgStatusData->sPerPortData[u8PortNum].u16DAC_I_MinOutVoltInmV = CFG_PORT_SINK_DAC_I_MIN_OP_VOLTAGE;
    pasCfgStatusData->sPerPortData[u8PortNum].u8DAC_I_Direction = CFG_PORT_SINK_DAC_I_DIR_HIGH_AMP_MAX_VOLT;
#endif



}

void CFG_PBPerPortParams (UINT8 u8PortNum, GLOBAL_CFG_STATUS_DATA *pasCfgStatusData)
{   
    #if ((TRUE == INCLUDE_POWER_BALANCING) || (TRUE == INCLUDE_POWER_THROTTLING))
    pasCfgStatusData->sPBPerPortData[u8PortNum].u16MaxPrtPwrBankAIn250mW = CFG_PB_MAX_PORT_POWER_BANKA;
    pasCfgStatusData->sPBPerPortData[u8PortNum].u16MaxPrtPwrBankBIn250mW = CFG_PB_MAX_PORT_POWER_BANKB;
    pasCfgStatusData->sPBPerPortData[u8PortNum].u16MaxPrtPwrBankCIn250mW = CFG_PB_MAX_PORT_POWER_BANKC;
    pasCfgStatusData->sPBPerPortData[u8PortNum].u8PortPriority = u8PortNum;
    #endif
}

/* ************************************************************************** */
/* ************************************************************************** */
// Section: Interface Functions                                               */
/* ************************************************************************** */
/* ************************************************************************** */
void PSF_LoadConfig(GLOBAL_CFG_STATUS_DATA *pasCfgStatusData)
{
    UINT8 u8PortNum = 0;
    
    pasCfgStatusData->u16ProducdID = PRODUCT_ID;
    pasCfgStatusData->u16VendorID = VENDOR_ID;
    pasCfgStatusData->u16ProductTypeVDO = PRODUCT_TYPE_VDO;
    pasCfgStatusData->u16ProductVDO = PRODUCT_VDO;
    pasCfgStatusData->u16CertStatVDO = CERT_STAT_VDO;
    pasCfgStatusData->u16IDHeaderVDO = ID_HEADER_VDO;
    pasCfgStatusData->u8HWVersion = HW_VERSION;
    pasCfgStatusData->u8SiVersion = SILICON_VERSION; 
    pasCfgStatusData->u8PwrThrottleCfg = ((CFG_PD_THROTTLE_BANK_A << 1) | CFG_PT_ENABLE);
    
#if (TRUE == INCLUDE_POWER_BALANCING)    
    pasCfgStatusData->u8PBEnableSelect = CFG_PB_ENABLE;  
#endif 
    
#if ((TRUE == INCLUDE_POWER_BALANCING) || (TRUE == INCLUDE_POWER_THROTTLING))    
    pasCfgStatusData->u16SystemPowerBankAIn250mW = CFG_PB_TOT_SYS_POWER_BANKA;
    pasCfgStatusData->u16MinPowerBankAIn250mW = CFG_PB_MIN_POWER_BANKA;
    pasCfgStatusData->u16SystemPowerBankBIn250mW = CFG_PB_TOT_SYS_POWER_BANKB;
    pasCfgStatusData->u16MinPowerBankBIn250mW = CFG_PB_MIN_POWER_BANKB;
    pasCfgStatusData->u16SystemPowerBankCIn250mW =CFG_PB_TOT_SYS_POWER_BANKC;
    pasCfgStatusData->u16MinPowerBankCIn250mW = CFG_PB_MIN_POWER_BANKC;
#endif     
    
    for (u8PortNum = 0; u8PortNum < CONFIG_PD_PORT_COUNT; u8PortNum++)
    {
        CFG_PerPortParams (u8PortNum, pasCfgStatusData);  
        CFG_PBPerPortParams (u8PortNum, pasCfgStatusData);
    }

}



/* *****************************************************************************
 End of File
 */
