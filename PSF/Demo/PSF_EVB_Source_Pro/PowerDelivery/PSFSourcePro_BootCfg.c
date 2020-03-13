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
    gasCfgStatusData.sPerPortData[u8PortNum].u32CfgData =       \
                    ((CONFIG_VCONN_OCS_ENABLE << TYPEC_VCONN_OCS_EN_POS ) |\
                    (CONFIG_PORT_ENABLE << TYPEC_PORT_ENDIS_POS)| \
                    ((CONFIG_PORT_RP_CURRENT_VALUE) << TYPEC_PORT_RPVAL_POS)|\
                    (CONFIG_PORT_POWER_ROLE));
    gasCfgStatusData.sPerPortData[u8PortNum].u32aSourcePDO[0] =           \
                  (((CONFIG_PORT_SOURCE_USB_SUSP) << 28) | \
                  ((CONFIG_PORT_SOURCE_UNCONSTARINED_PWR) << 27) | \
                   ((CONFIG_PORT_SOURCE_USB_COMM) << 26) | \
                (((CONFIG_PORT_SOURCE_PDO_1_VOLTAGE)/50) << 10) | \
                    ((CONFIG_PORT_SOURCE_PDO_1_CURRENT)/10));
    gasCfgStatusData.sPerPortData[u8PortNum].u32aSourcePDO[1] =           \
                          ((((CONFIG_PORT_SOURCE_PDO_2_VOLTAGE)/50) << 10) | \
                           ((CONFIG_PORT_SOURCE_PDO_2_CURRENT)/10));
    gasCfgStatusData.sPerPortData[u8PortNum].u32aSourcePDO[2] =           \
                          ((((CONFIG_PORT_SOURCE_PDO_3_VOLTAGE)/50) << 10) | \
                           ((CONFIG_PORT_SOURCE_PDO_3_CURRENT)/10));
    gasCfgStatusData.sPerPortData[u8PortNum].u32aSourcePDO[3] =           \
                          ((((CONFIG_PORT_SOURCE_PDO_4_VOLTAGE)/50) << 10) | \
                           ((CONFIG_PORT_SOURCE_PDO_4_CURRENT)/10));
    gasCfgStatusData.sPerPortData[u8PortNum].u32aSourcePDO[4] =           \
                          ((((CONFIG_PORT_SOURCE_PDO_5_VOLTAGE)/50) << 10) | \
                           ((CONFIG_PORT_SOURCE_PDO_5_CURRENT)/10));
    gasCfgStatusData.sPerPortData[u8PortNum].u32aSourcePDO[5] =           \
                          ((((CONFIG_PORT_SOURCE_PDO_6_VOLTAGE)/50) << 10) | \
                           ((CONFIG_PORT_SOURCE_PDO_6_CURRENT)/10));
    gasCfgStatusData.sPerPortData[u8PortNum].u32aSourcePDO[6] =           \
                          ((((CONFIG_PORT_SOURCE_PDO_7_VOLTAGE)/50) << 10) | \
                           ((CONFIG_PORT_SOURCE_PDO_7_CURRENT)/10));
    gasCfgStatusData.sPerPortData[u8PortNum].u8SourcePDOCnt = CONFIG_PORT_SOURCE_NUM_OF_PDOS;

    (void)MCHP_PSF_HOOK_MEMCPY(gasCfgStatusData.sPerPortData[u8PortNum].u32aAdvertisedPDO, 
            gasCfgStatusData.sPerPortData[u8PortNum].u32aSourcePDO, (gasCfgStatusData.sPerPortData[u8PortNum].u8SourcePDOCnt * 4));
    gasCfgStatusData.sPerPortData[u8PortNum].u8AdvertisedPDOCnt = \
                        gasCfgStatusData.sPerPortData[u8PortNum].u8SourcePDOCnt;  
    
    gasCfgStatusData.sPerPortData[u8PortNum].u8OCSThresholdPercentage = 0x00;
    gasCfgStatusData.sPerPortData[u8PortNum].u8FaultInDebounceInms = CONFIG_FAULT_IN_OCS_DEBOUNCE_MS;
    gasCfgStatusData.sPerPortData[u8PortNum].u8OVThresholdPercentage = CONFIG_OVER_VOLTAGE_FACTOR;
    gasCfgStatusData.sPerPortData[u8PortNum].u8UVThresholdPercentage = CONFIG_UNDER_VOLTAGE_FACTOR;
    gasCfgStatusData.sPerPortData[u8PortNum].u8VCONNOCSDebounceInms = CONFIG_VCONN_OCS_DEBOUNCE_IN_MS;
    gasCfgStatusData.sPerPortData[u8PortNum].u8MaxFaultCntVBUS = CONFIG_MAX_VBUS_POWER_FAULT_COUNT;
    gasCfgStatusData.sPerPortData[u8PortNum].u8MaxFaultCntVCONN = CONFIG_MAX_VCONN_FAULT_COUNT;
    gasCfgStatusData.sPerPortData[u8PortNum].u16PowerGoodTimerInms = MILLISECONDS_TO_TICKS(CONFIG_POWER_GOOD_TIMER_MS);

    gasCfgStatusData.sPerPortData[u8PortNum].u8Pio_VBUS_EN = (UINT8)CONFIG_PORT_UPD_EN_VBUS;
    gasCfgStatusData.sPerPortData[u8PortNum].u8Mode_VBUS_EN = (UINT8)CONFIG_PORT_UPD_EN_VBUS_PIO_MODE;
    gasCfgStatusData.sPerPortData[u8PortNum].u8Pio_FAULT_IN = (UINT8)CONFIG_PORT_UPD_FAULT_IN_PIO_NO;
    gasCfgStatusData.sPerPortData[u8PortNum].u8Mode_FAULT_IN = (UINT8)CONFIG_PORT_UPD_FAULT_IN_MODE;
    gasCfgStatusData.sPerPortData[u8PortNum].u8Pio_VBUS_DIS = (UINT8)CONFIG_PORT_UPD_VBUS_DIS_PIO_NO;
    gasCfgStatusData.sPerPortData[u8PortNum].u8mode_VBUS_DIS = (UINT8)CONFIG_PORT_UPD_VBUS_DIS_PIO_MODE;
    gasCfgStatusData.sPerPortData[u8PortNum].u8Pio_DC_DC_EN = (UINT8)CONFIG_PORT_UPD_DC_DC_EN_PIO_NO;
    gasCfgStatusData.sPerPortData[u8PortNum].u8Mode_DC_DC_EN = (UINT8)CONFIG_PORT_UPD_DC_DC_EN_PIO_MODE;

#if (CONFIG_DCDC_CTRL == PWRCTRL_DEFAULT_PSF_GPIO_CONFIG)  
    gasCfgStatusData.sPerPortData[u8PortNum].u8aPio_VSEL[0] =         \
                                                (UINT8)CONFIG_PORT_UPD_VSEL0_PIO_NO;
    gasCfgStatusData.sPerPortData[u8PortNum].u8aPio_VSEL[1] =         \
                                                (UINT8)CONFIG_PORT_UPD_VSEL1_PIO_NO;
    gasCfgStatusData.sPerPortData[u8PortNum].u8aPio_VSEL[2] =         \
                                                (UINT8)CONFIG_PORT_UPD_VSEL2_PIO_NO;

    gasCfgStatusData.sPerPortData[u8PortNum].u8aMode_VSEL[0] =       \
                                               (UINT8)CONFIG_PORT_UPD_VSEL0_PIO_MODE;
    gasCfgStatusData.sPerPortData[u8PortNum].u8aMode_VSEL[1] =       \
                                               (UINT8)CONFIG_PORT_UPD_VSEL1_PIO_MODE;
    gasCfgStatusData.sPerPortData[u8PortNum].u8aMode_VSEL[2] =       \
                                               (UINT8)CONFIG_PORT_UPD_VSEL2_PIO_MODE;

    gasCfgStatusData.sPerPortData[u8PortNum].u8aVSELTruthTable[0] =  \
                                             CONFIG_PORT_VSAFE0V_VSEL_MAPPING;
    gasCfgStatusData.sPerPortData[u8PortNum].u8aVSELTruthTable[1] =  \
                                             CONFIG_PORT_PDO_1_VSEL_MAPPING;
    gasCfgStatusData.sPerPortData[u8PortNum].u8aVSELTruthTable[2] =  \
                                             CONFIG_PORT_PDO_2_VSEL_MAPPING;
    gasCfgStatusData.sPerPortData[u8PortNum].u8aVSELTruthTable[3] =  \
                                             CONFIG_PORT_PDO_3_VSEL_MAPPING;
    gasCfgStatusData.sPerPortData[u8PortNum].u8aVSELTruthTable[4] =  \
                                             CONFIG_PORT_PDO_4_VSEL_MAPPING;
    gasCfgStatusData.sPerPortData[u8PortNum].u8aVSELTruthTable[5] =  \
                                             CONFIG_PORT_PDO_5_VSEL_MAPPING;
    gasCfgStatusData.sPerPortData[u8PortNum].u8aVSELTruthTable[6] =  \
                                             CONFIG_PORT_PDO_6_VSEL_MAPPING;
    gasCfgStatusData.sPerPortData[u8PortNum].u8aVSELTruthTable[7] =  \
                                             CONFIG_PORT_PDO_7_VSEL_MAPPING;    
#endif    
}

void CFG_PBPerPortParams (UINT8 u8PortNum)
{
#if (TRUE == INCLUDE_POWER_BALANCING)     
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
void PSF_LoadConfig()
{
    UINT8 u8PortNum = 0;
    
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
    gasCfgStatusData.u8PSFMajorVersion = HIBYTE(SYSTEM_FW_REV); 
    gasCfgStatusData.u8PSFMinorVersion = LOBYTE(SYSTEM_FW_REV);
    
#if ((TRUE == INCLUDE_POWER_BALANCING) || (TRUE == INCLUDE_POWER_THROTTLING))    
    gasCfgStatusData.u8PwrThrottleCfg = PD_THROTTLE_BANK_A;
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
