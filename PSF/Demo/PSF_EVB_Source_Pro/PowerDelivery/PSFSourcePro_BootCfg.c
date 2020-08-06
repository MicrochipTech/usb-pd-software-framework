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




/* ************************************************************************** */
/* ************************************************************************** */
// Section: Interface Functions                                               */
/* ************************************************************************** */
/* ************************************************************************** */
void PSF_LoadConfig(GLOBAL_CFG_STATUS_DATA *pasCfgStatusData)
{
    pasCfgStatusData->u16ProducdID = CFG_PRODUCT_ID;
    pasCfgStatusData->u16VendorID = CFG_VENDOR_ID;
    pasCfgStatusData->u16ProductTypeVDO = CFG_PRODUCT_TYPE_VDO;
    pasCfgStatusData->u16ProductVDO = CFG_PRODUCT_VDO;
    pasCfgStatusData->u16CertStatVDO = CFG_CERT_STAT_VDO;
    pasCfgStatusData->u16IDHeaderVDO = CFG_ID_HEADER_VDO;
    pasCfgStatusData->u8HWVersion = CFG_HW_VERSION;
    pasCfgStatusData->u8SiVersion = CFG_SILICON_VERSION; 
    pasCfgStatusData->u8PwrThrottleCfg = ((CFG_PT_POWER_THROTTLE_BANK_SEL << 1) | CFG_PT_ENABLE);
    
#if (TRUE == INCLUDE_POWER_BALANCING)    
    pasCfgStatusData->u8PBEnableSelect = CFG_PB_ENABLE;  
#endif 
    
#if ((TRUE == INCLUDE_POWER_BALANCING) || (TRUE == INCLUDE_POWER_THROTTLING))    
    pasCfgStatusData->u16SystemPowerBankAIn250mW = CFG_PB_TOT_SYS_POWER_BANKA_IN_250mW;
    pasCfgStatusData->u16MinPowerBankAIn250mW = CFG_PB_MIN_POWER_BANKA_IN_250mW;
    pasCfgStatusData->u16SystemPowerBankBIn250mW = CFG_PB_TOT_SYS_POWER_BANKB_IN_250mW;
    pasCfgStatusData->u16MinPowerBankBIn250mW = CFG_PB_MIN_POWER_BANKB_IN_250mW;
    pasCfgStatusData->u16SystemPowerBankCIn250mW =CFG_PB_TOT_SYS_POWER_BANKC_IN_250mW;
    pasCfgStatusData->u16MinPowerBankCIn250mW = CFG_PB_MIN_POWER_BANKC_IN_250mW;
#endif     
    
    pasCfgStatusData->sPerPortData[PORT0].u32CfgData =       
                ((CFG_PORT_0_VCONN_OCS_ENABLE) | (CFG_PORT_0_ENABLE) | (CFG_PORT_0_RP_CURRENT_VALUE)| \
                (CFG_PORT_0_DUAL_ROLE_DATA_SUPPORT) | (CFG_PORT_0_POWER_ROLE));
    pasCfgStatusData->sPerPortData[PORT1].u32CfgData =       
                ((CFG_PORT_1_VCONN_OCS_ENABLE) | (CFG_PORT_1_ENABLE) | (CFG_PORT_1_RP_CURRENT_VALUE)| \
                (CFG_PORT_1_DUAL_ROLE_DATA_SUPPORT) | (CFG_PORT_1_POWER_ROLE));
    
    
    pasCfgStatusData->sPerPortData[PORT0].u8Pio_FAULT_IN = (UINT8)CFG_PORT_0_UPD_FAULT_IN_PIO_NO;
    pasCfgStatusData->sPerPortData[PORT0].u8Mode_FAULT_IN = (UINT8)CFG_PORT_0_UPD_FAULT_IN_MODE;
    pasCfgStatusData->sPerPortData[PORT0].u8Pio_EN_VBUS = (UINT8)CFG_PORT_0_UPD_EN_VBUS;
    pasCfgStatusData->sPerPortData[PORT0].u8Mode_EN_VBUS = (UINT8)CFG_PORT_0_UPD_EN_VBUS_PIO_MODE;
    
    
    pasCfgStatusData->sPerPortData[PORT1].u8Pio_FAULT_IN = (UINT8)CFG_PORT_1_UPD_FAULT_IN_PIO_NO;
    pasCfgStatusData->sPerPortData[PORT1].u8Mode_FAULT_IN = (UINT8)CFG_PORT_1_UPD_FAULT_IN_MODE;
    pasCfgStatusData->sPerPortData[PORT1].u8Pio_EN_VBUS = (UINT8)CFG_PORT_1_UPD_EN_VBUS;
    pasCfgStatusData->sPerPortData[PORT1].u8Mode_EN_VBUS = (UINT8)CFG_PORT_1_UPD_EN_VBUS_PIO_MODE;
    pasCfgStatusData->sPerPortData[PORT0].u8OCSThresholdPercentage = SET_TO_ZERO;
    pasCfgStatusData->sPerPortData[PORT0].u8OVThresholdPercentage = CFG_OVER_VOLTAGE_FACTOR;
    pasCfgStatusData->sPerPortData[PORT0].u8UVThresholdPercentage = CFG_UNDER_VOLTAGE_FACTOR;
    pasCfgStatusData->sPerPortData[PORT0].u8FaultInDebounceInms = CFG_FAULT_IN_OCS_DEBOUNCE_MS;
    pasCfgStatusData->sPerPortData[PORT0].u8VCONNOCSDebounceInms = CFG_VCONN_OCS_DEBOUNCE_IN_MS;
    pasCfgStatusData->sPerPortData[PORT0].u8VBUSMaxFaultCnt = CFG_MAX_VBUS_POWER_FAULT_COUNT;
    pasCfgStatusData->sPerPortData[PORT0].u8VCONNMaxFaultCnt = CFG_MAX_VCONN_FAULT_COUNT;
    pasCfgStatusData->sPerPortData[PORT0].u16PowerGoodTimerInms = MILLISECONDS_TO_TICKS(CFG_POWER_GOOD_TIMER_MS);
    pasCfgStatusData->sPerPortData[PORT0].u16MaxSrcPrtCurrentIn10mA = CFG_MAX_PORT_CURRENT_IN_10mA;
    
    pasCfgStatusData->sPerPortData[PORT1].u8OCSThresholdPercentage = SET_TO_ZERO;
    pasCfgStatusData->sPerPortData[PORT1].u8FaultInDebounceInms = CFG_FAULT_IN_OCS_DEBOUNCE_MS;
    pasCfgStatusData->sPerPortData[PORT1].u8OVThresholdPercentage = CFG_OVER_VOLTAGE_FACTOR;
    pasCfgStatusData->sPerPortData[PORT1].u8UVThresholdPercentage = CFG_UNDER_VOLTAGE_FACTOR;
    pasCfgStatusData->sPerPortData[PORT1].u8VCONNOCSDebounceInms = CFG_VCONN_OCS_DEBOUNCE_IN_MS;
    pasCfgStatusData->sPerPortData[PORT1].u8VBUSMaxFaultCnt = CFG_MAX_VBUS_POWER_FAULT_COUNT;
    pasCfgStatusData->sPerPortData[PORT1].u8VCONNMaxFaultCnt = CFG_MAX_VCONN_FAULT_COUNT;
    pasCfgStatusData->sPerPortData[PORT1].u16PowerGoodTimerInms = MILLISECONDS_TO_TICKS(CFG_POWER_GOOD_TIMER_MS);
    pasCfgStatusData->sPerPortData[PORT1].u16MaxSrcPrtCurrentIn10mA = CFG_MAX_PORT_CURRENT_IN_10mA; 
   
    pasCfgStatusData->sPerPortData[PORT0].u16FeatureSelect = CFG_PB_PORT_ENABLE;
    pasCfgStatusData->sPerPortData[PORT1].u16FeatureSelect = CFG_PB_PORT_ENABLE;
	
	pasCfgStatusData->sPerPortData[PORT0].u8SourcePDOCnt = CFG_PORT_0_SOURCE_NUM_OF_PDOS;  
    
    /*PDOs update*/
    pasCfgStatusData->sPerPortData[PORT0].u32aSourcePDO[INDEX_0] = CFG_PORT_0_SOURCE_PDO_1;   
    pasCfgStatusData->sPerPortData[PORT0].u32aSourcePDO[INDEX_1] = CFG_PORT_0_SOURCE_PDO_2;
    pasCfgStatusData->sPerPortData[PORT0].u32aSourcePDO[INDEX_2] = CFG_PORT_0_SOURCE_PDO_3;
    pasCfgStatusData->sPerPortData[PORT0].u32aSourcePDO[INDEX_3] = CFG_PORT_0_SOURCE_PDO_4;
    pasCfgStatusData->sPerPortData[PORT0].u32aSourcePDO[INDEX_4] = CFG_PORT_0_SOURCE_PDO_5;
    pasCfgStatusData->sPerPortData[PORT0].u32aSourcePDO[INDEX_5] = CFG_PORT_0_SOURCE_PDO_6;               
    pasCfgStatusData->sPerPortData[PORT0].u32aSourcePDO[INDEX_6] = CFG_PORT_0_SOURCE_PDO_7;
	
	 pasCfgStatusData->sPerPortData[PORT1].u8SourcePDOCnt = CFG_PORT_1_SOURCE_NUM_OF_PDOS;
    /* PDO Update*/
    pasCfgStatusData->sPerPortData[PORT1].u32aSourcePDO[INDEX_0] = CFG_PORT_1_SOURCE_PDO_1;    
    pasCfgStatusData->sPerPortData[PORT1].u32aSourcePDO[INDEX_1] = CFG_PORT_1_SOURCE_PDO_2;
    pasCfgStatusData->sPerPortData[PORT1].u32aSourcePDO[INDEX_2] = CFG_PORT_1_SOURCE_PDO_3;
    pasCfgStatusData->sPerPortData[PORT1].u32aSourcePDO[INDEX_3] = CFG_PORT_1_SOURCE_PDO_4;
    pasCfgStatusData->sPerPortData[PORT1].u32aSourcePDO[INDEX_4] = CFG_PORT_1_SOURCE_PDO_5;
    pasCfgStatusData->sPerPortData[PORT1].u32aSourcePDO[INDEX_5] = CFG_PORT_1_SOURCE_PDO_6;
    pasCfgStatusData->sPerPortData[PORT1].u32aSourcePDO[INDEX_6] = CFG_PORT_1_SOURCE_PDO_7;
    
    #if ((TRUE == INCLUDE_POWER_BALANCING) || (TRUE == INCLUDE_POWER_THROTTLING))
    pasCfgStatusData->sPBPerPortData[PORT0].u16MaxPrtPwrBankAIn250mW = CFG_PB_MAX_PORT_0_POWER_BANKA_IN_250mW;
    pasCfgStatusData->sPBPerPortData[PORT0].u16MaxPrtPwrBankBIn250mW = CFG_PB_MAX_PORT_0_POWER_BANKB_IN_250mW;
    pasCfgStatusData->sPBPerPortData[PORT0].u16MaxPrtPwrBankCIn250mW = CFG_PB_MAX_PORT_0_POWER_BANKC_IN_250mW;
    pasCfgStatusData->sPBPerPortData[PORT0].u8PortPriority = PORT0;
    
    pasCfgStatusData->sPBPerPortData[PORT1].u16MaxPrtPwrBankAIn250mW = CFG_PB_MAX_PORT_1_POWER_BANKA_IN_250mW;
    pasCfgStatusData->sPBPerPortData[PORT1].u16MaxPrtPwrBankBIn250mW = CFG_PB_MAX_PORT_1_POWER_BANKB_IN_250mW;
    pasCfgStatusData->sPBPerPortData[PORT1].u16MaxPrtPwrBankCIn250mW = CFG_PB_MAX_PORT_1_POWER_BANKC_IN_250mW;
    pasCfgStatusData->sPBPerPortData[PORT1].u8PortPriority = PORT1;
    #endif

}



/* *****************************************************************************
 End of File
 */
