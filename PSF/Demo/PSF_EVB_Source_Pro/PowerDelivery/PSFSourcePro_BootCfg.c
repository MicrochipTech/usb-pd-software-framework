/*************************************************************************** 
  User Configuration File

  Company:
    Microchip Technology Inc.

  File Name:
    PSFSourcePro_BootCfg.c

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
static void CFG_PPSPerPortParams (UINT8 u8PortNum, GLOBAL_CFG_STATUS_DATA *pasCfgStatusData);

static void CFG_PerPortParams (UINT8 u8PortNum, GLOBAL_CFG_STATUS_DATA *pasCfgStatusData)
{    
    pasCfgStatusData->sPerPortData[u8PortNum].u32CfgData =       
                    ((CFG_VCONN_OCS_ENABLE) | (CFG_PORT_ENABLE) | (CFG_PORT_RP_CURRENT_VALUE)| \
                    (CFG_PORT_POWER_ROLE));
    
    pasCfgStatusData->sPerPortData[u8PortNum].u32aSourcePDO[0] = CFG_FORM_FIXED_PDO1(CFG_PORT_SOURCE_PDO_1_VOLTAGE, \
            CFG_PORT_SOURCE_PDO_1_CURRENT, CFG_PORT_SOURCE_USB_COMM, CFG_PORT_SOURCE_USB_SUSP,  \
            CFG_PORT_SOURCE_UNCONSTARINED_PWR);    

    pasCfgStatusData->sPerPortData[u8PortNum].u32aSourcePDO[1] = CFG_FORM_FIXED_PDOx(CFG_PORT_SOURCE_PDO_2_VOLTAGE, \
                                                    CFG_PORT_SOURCE_PDO_2_CURRENT);        

    pasCfgStatusData->sPerPortData[u8PortNum].u32aSourcePDO[2] = CFG_FORM_FIXED_PDOx(CFG_PORT_SOURCE_PDO_3_VOLTAGE, 
                                                    CFG_PORT_SOURCE_PDO_3_CURRENT);     

    pasCfgStatusData->sPerPortData[u8PortNum].u32aSourcePDO[3] = CFG_FORM_FIXED_PDOx(CFG_PORT_SOURCE_PDO_4_VOLTAGE, 
                                                    CFG_PORT_SOURCE_PDO_4_CURRENT);         \

    pasCfgStatusData->sPerPortData[u8PortNum].u32aSourcePDO[4] = CFG_FORM_FIXED_PDOx(CFG_PORT_SOURCE_PDO_5_VOLTAGE, 
                                                    CFG_PORT_SOURCE_PDO_5_CURRENT);            \

    pasCfgStatusData->sPerPortData[u8PortNum].u32aSourcePDO[5] = CFG_FORM_FIXED_PDOx(CFG_PORT_SOURCE_PDO_6_VOLTAGE, 
                                                    CFG_PORT_SOURCE_PDO_6_CURRENT);            \
    
    pasCfgStatusData->sPerPortData[u8PortNum].u32aSourcePDO[6] = CFG_FORM_FIXED_PDOx(CFG_PORT_SOURCE_PDO_7_VOLTAGE, 
                                                    CFG_PORT_SOURCE_PDO_7_CURRENT); 
    
    pasCfgStatusData->sPerPortData[u8PortNum].u8SourcePDOCnt = CFG_PORT_SOURCE_NUM_OF_PDOS;

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
    
    pasCfgStatusData->sPerPortData[u8PortNum].u8Pio_EN_VBUS = (UINT8)CFG_PORT_UPD_EN_VBUS;
    pasCfgStatusData->sPerPortData[u8PortNum].u8Mode_EN_VBUS = (UINT8)CFG_PORT_UPD_EN_VBUS_PIO_MODE;
    pasCfgStatusData->sPerPortData[u8PortNum].u8Pio_FAULT_IN = (UINT8)CFG_PORT_UPD_FAULT_IN_PIO_NO;
    pasCfgStatusData->sPerPortData[u8PortNum].u8Mode_FAULT_IN = (UINT8)CFG_PORT_UPD_FAULT_IN_MODE;
    pasCfgStatusData->sPerPortData[u8PortNum].u8Pio_VBUS_DIS = (UINT8)CFG_PORT_UPD_VBUS_DIS_PIO_NO;
    pasCfgStatusData->sPerPortData[u8PortNum].u8mode_VBUS_DIS = (UINT8)CFG_PORT_UPD_VBUS_DIS_PIO_MODE;
    pasCfgStatusData->sPerPortData[u8PortNum].u8Pio_DC_DC_EN = (UINT8)CFG_PORT_UPD_DC_DC_EN_PIO_NO;
    pasCfgStatusData->sPerPortData[u8PortNum].u8Mode_DC_DC_EN = (UINT8)CFG_PORT_UPD_DC_DC_EN_PIO_MODE;

#if (CONFIG_DCDC_CTRL == PWRCTRL_GPIO_DC_DC)  
    pasCfgStatusData->sPerPortData[u8PortNum].u8aPio_VSEL[0] =         \
                                                (UINT8)CFG_PORT_UPD_VSEL0_PIO_NO;
    pasCfgStatusData->sPerPortData[u8PortNum].u8aPio_VSEL[1] =         \
                                                (UINT8)CFG_PORT_UPD_VSEL1_PIO_NO;
    pasCfgStatusData->sPerPortData[u8PortNum].u8aPio_VSEL[2] =         \
                                                (UINT8)CFG_PORT_UPD_VSEL2_PIO_NO;

    pasCfgStatusData->sPerPortData[u8PortNum].u8aMode_VSEL[0] =       \
                                               (UINT8)CFG_PORT_UPD_VSEL0_PIO_MODE;
    pasCfgStatusData->sPerPortData[u8PortNum].u8aMode_VSEL[1] =       \
                                               (UINT8)CFG_PORT_UPD_VSEL1_PIO_MODE;
    pasCfgStatusData->sPerPortData[u8PortNum].u8aMode_VSEL[2] =       \
                                               (UINT8)CFG_PORT_UPD_VSEL2_PIO_MODE;

    pasCfgStatusData->sPerPortData[u8PortNum].u8aVSELTruthTable[0] =  \
                                             CFG_PORT_VSAFE0V_VSEL_MAPPING;
    pasCfgStatusData->sPerPortData[u8PortNum].u8aVSELTruthTable[1] =  \
                                             CFG_PORT_PDO_1_VSEL_MAPPING;
    pasCfgStatusData->sPerPortData[u8PortNum].u8aVSELTruthTable[2] =  \
                                             CFG_PORT_PDO_2_VSEL_MAPPING;
    pasCfgStatusData->sPerPortData[u8PortNum].u8aVSELTruthTable[3] =  \
                                             CFG_PORT_PDO_3_VSEL_MAPPING;
    pasCfgStatusData->sPerPortData[u8PortNum].u8aVSELTruthTable[4] =  \
                                             CFG_PORT_PDO_4_VSEL_MAPPING;
    pasCfgStatusData->sPerPortData[u8PortNum].u8aVSELTruthTable[5] =  \
                                             CFG_PORT_PDO_5_VSEL_MAPPING;
    pasCfgStatusData->sPerPortData[u8PortNum].u8aVSELTruthTable[6] =  \
                                             CFG_PORT_PDO_6_VSEL_MAPPING;
    pasCfgStatusData->sPerPortData[u8PortNum].u8aVSELTruthTable[7] =  \
                                             CFG_PORT_PDO_7_VSEL_MAPPING;    
#endif       
}

void CFG_PBPerPortParams (UINT8 u8PortNum, GLOBAL_CFG_STATUS_DATA *pasCfgStatusData)
{   
    #if ((TRUE == INCLUDE_POWER_BALANCING) || (TRUE == INCLUDE_POWER_THROTTLING))
    pasCfgStatusData->sPBPerPortData[u8PortNum].u16MaxPrtPwrBankAIn250mW = CFG_PB_MAX_PORT_POWER_BANKA;
    pasCfgStatusData->sPBPerPortData[u8PortNum].u16MaxPrtPwrBankBIn250mW = CFG_PB_MAX_PORT_POWER_BANKB;
    pasCfgStatusData->sPBPerPortData[u8PortNum].u16MaxPrtPwrBankCIn250mW = CFG_PB_MAX_PORT_POWER_BANKC;
    pasCfgStatusData->sPBPerPortData[u8PortNum].u8PBPortPriority = u8PortNum;
    #endif
}

void CFG_PPSPerPortParams (UINT8 u8PortNum, GLOBAL_CFG_STATUS_DATA *pasCfgStatusData)
{
    #if (TRUE == INCLUDE_PD_SOURCE_PPS) 
    /* Enable PPS Port Configuration parameters */
    pasCfgStatusData->sPPSPerPortData[u8PortNum].u8PPSCfgData = ((CFG_PPS_APDO_3_ENABLE << CFG_PPS_APDO_3_ENABLE_POS) | \
                        (CFG_PPS_APDO_2_ENABLE << CFG_PPS_APDO_2_ENABLE_POS) | \
                        (CFG_PPS_APDO_1_ENABLE << CFG_PPS_APDO_1_ENABLE_POS) |
                        (CFG_PPS_ENABLE)); 
    
    /* Update the value of APDO1 */
    pasCfgStatusData->sPPSPerPortData[u8PortNum].u32aPPSApdo[0] = CFG_FORM_PPS_APDO(CFG_POWER_SUPPLY_TYPE_PROGRAMMABLE, 
                        CFG_PORT_SOURCE_APDO_1_MIN_VOLTAGE, CFG_PORT_SOURCE_APDO_1_MAX_VOLTAGE, 
                        CFG_PORT_SOURCE_APDO_1_MAX_CURRENT, CFG_PORT_SOURCE_APDO_1_PPS_PWR_LIMITED);  

    /* Update the value of APDO2 */
    pasCfgStatusData->sPPSPerPortData[u8PortNum].u32aPPSApdo[1] = CFG_FORM_PPS_APDO(CFG_POWER_SUPPLY_TYPE_PROGRAMMABLE, 
                        CFG_PORT_SOURCE_APDO_2_MIN_VOLTAGE, CFG_PORT_SOURCE_APDO_2_MAX_VOLTAGE, 
                        CFG_PORT_SOURCE_APDO_2_MAX_CURRENT, CFG_PORT_SOURCE_APDO_2_PPS_PWR_LIMITED);  

    /* Update the value of APDO3 */
    pasCfgStatusData->sPPSPerPortData[u8PortNum].u32aPPSApdo[2] = CFG_FORM_PPS_APDO(CFG_POWER_SUPPLY_TYPE_PROGRAMMABLE, 
                        CFG_PORT_SOURCE_APDO_3_MIN_VOLTAGE, CFG_PORT_SOURCE_APDO_3_MAX_VOLTAGE, 
                        CFG_PORT_SOURCE_APDO_3_MAX_CURRENT, CFG_PORT_SOURCE_APDO_3_PPS_PWR_LIMITED);  
    
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

#if (TRUE == INCLUDE_POWER_BALANCING)    
    pasCfgStatusData->u8PBEnableSelect = CFG_PB_ENABLE;  
#endif 
    
#if ((TRUE == INCLUDE_POWER_BALANCING) || (TRUE == INCLUDE_POWER_THROTTLING))    
    pasCfgStatusData->u8PwrThrottleCfg = CFG_PD_THROTTLE_BANK_A;
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
        CFG_PPSPerPortParams (u8PortNum, pasCfgStatusData);
    }

}



/* *****************************************************************************
 End of File
 */
