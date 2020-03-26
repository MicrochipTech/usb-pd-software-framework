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
GLOBAL_CFG_STATUS_DATA gasCfgStatusData;

/* ************************************************************************** */
/* ************************************************************************** */
// Section: Local Functions                                                   */
/* ************************************************************************** */
/* ************************************************************************** */
static void CFG_PerPortParams (UINT8 u8PortNum);
static void CFG_PBPerPortParams (UINT8 u8PortNum);

static void CFG_PerPortParams (UINT8 u8PortNum)
{    
    gasCfgStatusData.sPerPortData[u8PortNum].u32CfgData =       
                    ((CFG_VCONN_OCS_ENABLE) | (CFG_PORT_ENABLE) | (CFG_PORT_RP_CURRENT_VALUE)| \
                    (CFG_PORT_POWER_ROLE));
    
    gasCfgStatusData.sPerPortData[u8PortNum].u32aSourcePDO[0] = CFG_FORM_FIXED_PDO1(CFG_PORT_SOURCE_PDO_1_VOLTAGE, \
            CFG_PORT_SOURCE_PDO_1_CURRENT, CFG_PORT_SOURCE_USB_COMM, CFG_PORT_SOURCE_USB_SUSP,  \
            CFG_PORT_SOURCE_UNCONSTARINED_PWR);    

    gasCfgStatusData.sPerPortData[u8PortNum].u32aSourcePDO[1] = CFG_FORM_FIXED_PDOx(CFG_PORT_SOURCE_PDO_2_VOLTAGE, \
                                                    CFG_PORT_SOURCE_PDO_2_CURRENT);        

    gasCfgStatusData.sPerPortData[u8PortNum].u32aSourcePDO[2] = CFG_FORM_FIXED_PDOx(CFG_PORT_SOURCE_PDO_3_VOLTAGE, 
                                                    CFG_PORT_SOURCE_PDO_3_CURRENT);     

    gasCfgStatusData.sPerPortData[u8PortNum].u32aSourcePDO[3] = CFG_FORM_FIXED_PDOx(CFG_PORT_SOURCE_PDO_4_VOLTAGE, 
                                                    CFG_PORT_SOURCE_PDO_4_CURRENT);         \

    gasCfgStatusData.sPerPortData[u8PortNum].u32aSourcePDO[4] = CFG_FORM_FIXED_PDOx(CFG_PORT_SOURCE_PDO_5_VOLTAGE, 
                                                    CFG_PORT_SOURCE_PDO_5_CURRENT);            \

    gasCfgStatusData.sPerPortData[u8PortNum].u32aSourcePDO[5] = CFG_FORM_FIXED_PDOx(CFG_PORT_SOURCE_PDO_6_VOLTAGE, 
                                                    CFG_PORT_SOURCE_PDO_6_CURRENT);            \
    
    gasCfgStatusData.sPerPortData[u8PortNum].u32aSourcePDO[6] = CFG_FORM_FIXED_PDOx(CFG_PORT_SOURCE_PDO_7_VOLTAGE, 
                                                    CFG_PORT_SOURCE_PDO_7_CURRENT); 
    
    gasCfgStatusData.sPerPortData[u8PortNum].u8SourcePDOCnt = CFG_PORT_SOURCE_NUM_OF_PDOS;

    (void)MCHP_PSF_HOOK_MEMCPY(gasCfgStatusData.sPerPortData[u8PortNum].u32aAdvertisedPDO, 
            gasCfgStatusData.sPerPortData[u8PortNum].u32aSourcePDO, (gasCfgStatusData.sPerPortData[u8PortNum].u8SourcePDOCnt * 4));
    gasCfgStatusData.sPerPortData[u8PortNum].u8AdvertisedPDOCnt = \
                        gasCfgStatusData.sPerPortData[u8PortNum].u8SourcePDOCnt;  
    
    gasCfgStatusData.sPerPortData[u8PortNum].u8OCSThresholdPercentage = 0x00;
    gasCfgStatusData.sPerPortData[u8PortNum].u8FaultInDebounceInms = CFG_FAULT_IN_OCS_DEBOUNCE_MS;
    gasCfgStatusData.sPerPortData[u8PortNum].u8OVThresholdPercentage = CFG_OVER_VOLTAGE_FACTOR;
    gasCfgStatusData.sPerPortData[u8PortNum].u8UVThresholdPercentage = CFG_UNDER_VOLTAGE_FACTOR;
    gasCfgStatusData.sPerPortData[u8PortNum].u8VCONNOCSDebounceInms = CFG_VCONN_OCS_DEBOUNCE_IN_MS;
    gasCfgStatusData.sPerPortData[u8PortNum].u8VBUSMaxFaultCnt = CFG_MAX_VBUS_POWER_FAULT_COUNT;
    gasCfgStatusData.sPerPortData[u8PortNum].u8VCONNMaxFaultCnt = CFG_MAX_VCONN_FAULT_COUNT;
    gasCfgStatusData.sPerPortData[u8PortNum].u16PowerGoodTimerInms = MILLISECONDS_TO_TICKS(CFG_POWER_GOOD_TIMER_MS);

    gasCfgStatusData.sPerPortData[u8PortNum].u8Pio_VBUS_EN = (UINT8)CFG_PORT_UPD_EN_VBUS;
    gasCfgStatusData.sPerPortData[u8PortNum].u8Mode_VBUS_EN = (UINT8)CFG_PORT_UPD_EN_VBUS_PIO_MODE;
    gasCfgStatusData.sPerPortData[u8PortNum].u8Pio_FAULT_IN = (UINT8)CFG_PORT_UPD_FAULT_IN_PIO_NO;
    gasCfgStatusData.sPerPortData[u8PortNum].u8Mode_FAULT_IN = (UINT8)CFG_PORT_UPD_FAULT_IN_MODE;
    gasCfgStatusData.sPerPortData[u8PortNum].u8Pio_VBUS_DIS = (UINT8)CFG_PORT_UPD_VBUS_DIS_PIO_NO;
    gasCfgStatusData.sPerPortData[u8PortNum].u8mode_VBUS_DIS = (UINT8)CFG_PORT_UPD_VBUS_DIS_PIO_MODE;
    gasCfgStatusData.sPerPortData[u8PortNum].u8Pio_DC_DC_EN = (UINT8)CFG_PORT_UPD_DC_DC_EN_PIO_NO;
    gasCfgStatusData.sPerPortData[u8PortNum].u8Mode_DC_DC_EN = (UINT8)CFG_PORT_UPD_DC_DC_EN_PIO_MODE;

#if (CONFIG_DCDC_CTRL == PWRCTRL_DEFAULT_PSF_GPIO_CONFIG)  
    gasCfgStatusData.sPerPortData[u8PortNum].u8aPio_VSEL[0] =         \
                                                (UINT8)CFG_PORT_UPD_VSEL0_PIO_NO;
    gasCfgStatusData.sPerPortData[u8PortNum].u8aPio_VSEL[1] =         \
                                                (UINT8)CFG_PORT_UPD_VSEL1_PIO_NO;
    gasCfgStatusData.sPerPortData[u8PortNum].u8aPio_VSEL[2] =         \
                                                (UINT8)CFG_PORT_UPD_VSEL2_PIO_NO;

    gasCfgStatusData.sPerPortData[u8PortNum].u8aMode_VSEL[0] =       \
                                               (UINT8)CFG_PORT_UPD_VSEL0_PIO_MODE;
    gasCfgStatusData.sPerPortData[u8PortNum].u8aMode_VSEL[1] =       \
                                               (UINT8)CFG_PORT_UPD_VSEL1_PIO_MODE;
    gasCfgStatusData.sPerPortData[u8PortNum].u8aMode_VSEL[2] =       \
                                               (UINT8)CFG_PORT_UPD_VSEL2_PIO_MODE;

    gasCfgStatusData.sPerPortData[u8PortNum].u8aVSELTruthTable[0] =  \
                                             CFG_PORT_VSAFE0V_VSEL_MAPPING;
    gasCfgStatusData.sPerPortData[u8PortNum].u8aVSELTruthTable[1] =  \
                                             CFG_PORT_PDO_1_VSEL_MAPPING;
    gasCfgStatusData.sPerPortData[u8PortNum].u8aVSELTruthTable[2] =  \
                                             CFG_PORT_PDO_2_VSEL_MAPPING;
    gasCfgStatusData.sPerPortData[u8PortNum].u8aVSELTruthTable[3] =  \
                                             CFG_PORT_PDO_3_VSEL_MAPPING;
    gasCfgStatusData.sPerPortData[u8PortNum].u8aVSELTruthTable[4] =  \
                                             CFG_PORT_PDO_4_VSEL_MAPPING;
    gasCfgStatusData.sPerPortData[u8PortNum].u8aVSELTruthTable[5] =  \
                                             CFG_PORT_PDO_5_VSEL_MAPPING;
    gasCfgStatusData.sPerPortData[u8PortNum].u8aVSELTruthTable[6] =  \
                                             CFG_PORT_PDO_6_VSEL_MAPPING;
    gasCfgStatusData.sPerPortData[u8PortNum].u8aVSELTruthTable[7] =  \
                                             CFG_PORT_PDO_7_VSEL_MAPPING;    
#endif    
}

void CFG_PBPerPortParams (UINT8 u8PortNum)
{   
    #if ((TRUE == INCLUDE_POWER_BALANCING) || (TRUE == INCLUDE_POWER_THROTTLING))
    gasCfgStatusData.sPBPerPortData[u8PortNum].u16MaxPrtPwrBankA = CFG_PB_MAX_PORT_POWER_BANKA;
    gasCfgStatusData.sPBPerPortData[u8PortNum].u16MaxPrtPwrBankB = CFG_PB_MAX_PORT_POWER_BANKB;
    gasCfgStatusData.sPBPerPortData[u8PortNum].u16MaxPrtPwrBankC = CFG_PB_MAX_PORT_POWER_BANKC;
    gasCfgStatusData.sPBPerPortData[u8PortNum].u16MaxPrtCurrent = CFG_PB_MAX_PORT_CURRENT;
    gasCfgStatusData.sPBPerPortData[u8PortNum].u8PBEnablePriority = ((u8PortNum << 1) | CFG_PB_PORT_ENABLE);
    #endif
}

/* ************************************************************************** */
/* ************************************************************************** */
// Section: Interface Functions                                               */
/* ************************************************************************** */
/* ************************************************************************** */
void PSF_LoadConfig(GLOBAL_CFG_STATUS_DATA gasCfgStatusData)
{
    UINT8 u8PortNum = 0;
    UINT16 u16StackVersion = SYSTEM_FW_REV;
    
    memset (&gasCfgStatusData, 0, sizeof(gasCfgStatusData));
    
    gasCfgStatusData.u8MinorVersion = STRUCT_MINOR_VERSION;
    gasCfgStatusData.u8MajorVersion = STRUCT_MAJOR_VERSION;
    gasCfgStatusData.u16ProducdID = PRODUCT_ID;
    gasCfgStatusData.u16VendorID = VENDOR_ID;
    gasCfgStatusData.u16ProductTypeVDO = PRODUCT_TYPE_VDO;
    gasCfgStatusData.u16ProductVDO = PRODUCT_VDO;
    gasCfgStatusData.u16CertStatVDO = CERT_STAT_VDO;
    gasCfgStatusData.u16IDHeaderVDO = ID_HEADER_VDO;
    gasCfgStatusData.u8HWVersion = HW_VERSION;
    gasCfgStatusData.u8SiVersion = SILICON_VERSION; 
    gasCfgStatusData.u8PSFMajorVersion = HIBYTE(u16StackVersion); 
    gasCfgStatusData.u8PSFMinorVersion = LOBYTE(u16StackVersion);
    
#if ((TRUE == INCLUDE_POWER_BALANCING) || (TRUE == INCLUDE_POWER_THROTTLING))    
    gasCfgStatusData.u8PwrThrottleCfg = CFG_PD_THROTTLE_BANK_A;
    gasCfgStatusData.u8PBEnableSelect = CFG_PB_ENABLE;  
    gasCfgStatusData.u16SystemPowerBankA = CFG_PB_TOT_SYS_POWER_BANKA;
    gasCfgStatusData.u16MinPowerBankA = CFG_PB_MIN_POWER_BANKA;
    gasCfgStatusData.u16SystemPowerBankB = CFG_PB_TOT_SYS_POWER_BANKB;
    gasCfgStatusData.u16MinPowerBankB = CFG_PB_MIN_POWER_BANKB;
    gasCfgStatusData.u16SystemPowerBankC =CFG_PB_TOT_SYS_POWER_BANKC;
    gasCfgStatusData.u16MinPowerBankC = CFG_PB_MIN_POWER_BANKC;
#endif     
    
    for (u8PortNum = 0; u8PortNum < CONFIG_PD_PORT_COUNT; u8PortNum++)
    {
        CFG_PerPortParams (u8PortNum);  
        CFG_PBPerPortParams (u8PortNum);   
    }

}



/* *****************************************************************************
 End of File
 */
