/*************************************************************************** 
  User Configuration File

  Company:
    Microchip Technology Inc.

  File Name:
    PSFSourceLite_BootCfg.c

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
#include <PSF_Config.h>
#include <string.h>

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
    UINT8 u8Length = SET_TO_ZERO;
    
    pasCfgStatusData->u16ProducdID = CFG_PRODUCT_ID;
    pasCfgStatusData->u16VendorID = CFG_VENDOR_ID;
    pasCfgStatusData->u16ProductTypeVDO = CFG_PRODUCT_TYPE_VDO;
    pasCfgStatusData->u16ProductVDO = CFG_PRODUCT_VDO;
    pasCfgStatusData->u16CertStatVDO = CFG_CERT_STAT_VDO;
    pasCfgStatusData->u16IDHeaderVDO = CFG_ID_HEADER_VDO;
    pasCfgStatusData->u8HWVersion = CFG_HW_VERSION;
    pasCfgStatusData->u8SiVersion = CFG_SILICON_VERSION;
    pasCfgStatusData->u8ManfStringLength = CFG_MANUFACTURER_STR_LEN;

    while(u8Length < pasCfgStatusData->u8ManfStringLength)
    {
        pasCfgStatusData->u8aManfString[u8Length] = CFG_MANUFACTURER_STR[u8Length];
        u8Length++;
    }

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
    
    /*Configure role swap policy for note type port*/
    gasCfgStatusData.sPerPortData[PORT0].u16SwapPolicy = CFG_PORT_0_ROLE_SWAP_POLICY;
               
    /*Configure role swap policy for dock type port*/
    gasCfgStatusData.sPerPortData[PORT1].u16SwapPolicy = CFG_PORT_1_ROLE_SWAP_POLICY;
    
}



/* *****************************************************************************
 End of File
 */
