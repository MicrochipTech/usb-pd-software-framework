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

    /*System Configuration*/    
    pasCfgStatusData->u16ProductID = CFG_PRODUCT_ID;
    pasCfgStatusData->u16VendorID = CFG_VENDOR_ID;
    pasCfgStatusData->u8HWVersion = CFG_HW_VERSION;
    pasCfgStatusData->u8SiVersion = CFG_SILICON_VERSION;
    pasCfgStatusData->u8ManfStringLength = CFG_MANUFACTURER_STR_LEN;

    while(u8Length < pasCfgStatusData->u8ManfStringLength)
    {
        pasCfgStatusData->u8aManfString[u8Length] = CFG_MANUFACTURER_STR[u8Length];
        u8Length++;
    }

    /*Per port configuration for Port 0*/    
    pasCfgStatusData->sPerPortData[PORT0].u32CfgData =       
        ((CFG_PORT_0_VCONN_OCS_ENABLE) | (CFG_PORT_0_ENABLE) | (CFG_PORT_0_RP_CURRENT_VALUE)| \
        (CFG_PORT_0_DUAL_ROLE_DATA_SUPPORT) | (CFG_PORT_0_POWER_ROLE));

    pasCfgStatusData->sPerPortData[PORT0].u8Pio_FAULT_IN = (UINT8)CFG_PORT_0_UPD_FAULT_IN_PIO_NO;
    pasCfgStatusData->sPerPortData[PORT0].u8Mode_FAULT_IN = (UINT8)CFG_PORT_0_UPD_FAULT_IN_MODE;
    pasCfgStatusData->sPerPortData[PORT0].u8Pio_EN_VBUS = (UINT8)CFG_PORT_0_UPD_EN_VBUS;
    pasCfgStatusData->sPerPortData[PORT0].u8Mode_EN_VBUS = (UINT8)CFG_PORT_0_UPD_EN_VBUS_PIO_MODE;
     
    pasCfgStatusData->sPerPortData[PORT0].u8OCSThresholdPercentage = SET_TO_ZERO;
    pasCfgStatusData->sPerPortData[PORT0].u8OVThresholdPercentage = CFG_OVER_VOLTAGE_FACTOR;
    pasCfgStatusData->sPerPortData[PORT0].u8UVThresholdPercentage = CFG_UNDER_VOLTAGE_FACTOR;
    pasCfgStatusData->sPerPortData[PORT0].u8FaultInDebounceInms = CFG_FAULT_IN_OCS_DEBOUNCE_MS;
    pasCfgStatusData->sPerPortData[PORT0].u8VCONNOCSDebounceInms = CFG_VCONN_OCS_DEBOUNCE_IN_MS;
    pasCfgStatusData->sPerPortData[PORT0].u8VBUSMaxFaultCnt = CFG_MAX_VBUS_POWER_FAULT_COUNT;
    pasCfgStatusData->sPerPortData[PORT0].u8VCONNMaxFaultCnt = CFG_MAX_VCONN_FAULT_COUNT;
    pasCfgStatusData->sPerPortData[PORT0].u16PowerGoodTimerInms = MILLISECONDS_TO_TICKS(CFG_POWER_GOOD_TIMER_MS);
    pasCfgStatusData->sPerPortData[PORT0].u16MaxSrcPrtCurrentIn10mA = CFG_MAX_PORT_CURRENT_IN_10mA;
   
    pasCfgStatusData->sPerPortData[PORT0].u8SourcePDOCnt = CFG_PORT_0_SOURCE_NUM_OF_PDOS;  
    
    /*PDOs update*/
    pasCfgStatusData->sPerPortData[PORT0].u32aSourcePDO[INDEX_0] = CFG_PORT_0_SOURCE_PDO_1;   
    pasCfgStatusData->sPerPortData[PORT0].u32aSourcePDO[INDEX_1] = CFG_PORT_0_SOURCE_PDO_2;
    pasCfgStatusData->sPerPortData[PORT0].u32aSourcePDO[INDEX_2] = CFG_PORT_0_SOURCE_PDO_3;
    pasCfgStatusData->sPerPortData[PORT0].u32aSourcePDO[INDEX_3] = CFG_PORT_0_SOURCE_PDO_4;
    pasCfgStatusData->sPerPortData[PORT0].u32aSourcePDO[INDEX_4] = CFG_PORT_0_SOURCE_PDO_5;
    pasCfgStatusData->sPerPortData[PORT0].u32aSourcePDO[INDEX_5] = CFG_PORT_0_SOURCE_PDO_6;               
    pasCfgStatusData->sPerPortData[PORT0].u32aSourcePDO[INDEX_6] = CFG_PORT_0_SOURCE_PDO_7;
    
    pasCfgStatusData->sPerPortData[PORT0].u16FeatureSelect = (CFG_PORT_0_ENABLE_VDM_SUPPORT | \
                                            CFG_PORT_0_ENABLE_AME_MONITORING);
    
    /*Configure role swap policy for Port 0 */
    pasCfgStatusData->sPerPortData[PORT0].u16SwapPolicy = CFG_PORT_0_ROLE_SWAP_POLICY;
    
    #if (TRUE == INCLUDE_PD_VDM)
    /* PD Identity Configuration for Port 0 */
    pasCfgStatusData->sVDMPerPortData[PORT0].u8PDIdentityCnt = CFG_PORT_0_PD_IDENTITY_COUNT;
    
    pasCfgStatusData->sVDMPerPortData[PORT0].u32aPDIdentity[INDEX_0] = CFG_PORT_0_ID_HEADER_VDO;
    pasCfgStatusData->sVDMPerPortData[PORT0].u32aPDIdentity[INDEX_1] = CFG_PORT_0_CERT_STAT_VDO_XID;
    pasCfgStatusData->sVDMPerPortData[PORT0].u32aPDIdentity[INDEX_2] = CFG_PORT_0_PRODUCT_VDO;    
    /* Note: Application shall configure the Product Type VDOs in indices 3-5
       depending upon the product type */
    #if (TRUE == INCLUDE_PD_ALT_MODE)
    /* SVIDs Configuration for Port 0 */
    pasCfgStatusData->sAltModePerPortData[PORT0].u8SVIDsCnt = CFG_PORT_0_SVID_COUNT;
    
    pasCfgStatusData->sAltModePerPortData[PORT0].u16aSVIDsTable[INDEX_0] = CFG_PORT_0_SVID_0;
    pasCfgStatusData->sAltModePerPortData[PORT0].u16aSVIDsTable[INDEX_1] = CFG_PORT_0_SVID_1;
    
    /* Modes Configuration for Port 0 */
    pasCfgStatusData->sAltModePerPortData[PORT0].u32aModesTable[INDEX_0] = CFG_PORT_0_SVID_0_MODE_0;
    pasCfgStatusData->sAltModePerPortData[PORT0].u32aModesTable[INDEX_1] = CFG_PORT_0_SVID_0_MODE_1;
    pasCfgStatusData->sAltModePerPortData[PORT0].u32aModesTable[INDEX_2] = CFG_PORT_0_SVID_1_MODE_0;
    pasCfgStatusData->sAltModePerPortData[PORT0].u32aModesTable[INDEX_3] = CFG_PORT_0_SVID_1_MODE_1;
    
    pasCfgStatusData->sAltModePerPortData[PORT0].u8aSVIDEntryTable[INDEX_0] = \
                      (CFG_PORT_0_SVID_0_MODES_COUNT | CFG_PORT_0_SVID_0_START_MODE_INDEX);
    pasCfgStatusData->sAltModePerPortData[PORT0].u8aSVIDEntryTable[INDEX_1] = \
                      (CFG_PORT_0_SVID_1_MODES_COUNT | CFG_PORT_0_SVID_1_START_MODE_INDEX);    
    #endif
    #endif
    
#if(TRUE == INCLUDE_UPD_HPD)
    pasCfgStatusData->sPerPortData[PORT0].u8PIO_HPD = CFG_PORT_0_HPD_IO;
#endif
/*Port 1 Per Port Configuration*/    
#if(CONFIG_PD_PORT_COUNT > PORT_COUNT_1)    
    pasCfgStatusData->sPerPortData[PORT1].u32CfgData =       
        ((CFG_PORT_1_VCONN_OCS_ENABLE) | (CFG_PORT_1_ENABLE) | (CFG_PORT_1_RP_CURRENT_VALUE)| \
        (CFG_PORT_1_DUAL_ROLE_DATA_SUPPORT) | (CFG_PORT_1_POWER_ROLE));

    pasCfgStatusData->sPerPortData[PORT1].u8Pio_FAULT_IN = (UINT8)CFG_PORT_1_UPD_FAULT_IN_PIO_NO;
    pasCfgStatusData->sPerPortData[PORT1].u8Mode_FAULT_IN = (UINT8)CFG_PORT_1_UPD_FAULT_IN_MODE;
    pasCfgStatusData->sPerPortData[PORT1].u8Pio_EN_VBUS = (UINT8)CFG_PORT_1_UPD_EN_VBUS;
    pasCfgStatusData->sPerPortData[PORT1].u8Mode_EN_VBUS = (UINT8)CFG_PORT_1_UPD_EN_VBUS_PIO_MODE;
    
    pasCfgStatusData->sPerPortData[PORT1].u8OCSThresholdPercentage = SET_TO_ZERO;
    pasCfgStatusData->sPerPortData[PORT1].u8FaultInDebounceInms = CFG_FAULT_IN_OCS_DEBOUNCE_MS;
    pasCfgStatusData->sPerPortData[PORT1].u8OVThresholdPercentage = CFG_OVER_VOLTAGE_FACTOR;
    pasCfgStatusData->sPerPortData[PORT1].u8UVThresholdPercentage = CFG_UNDER_VOLTAGE_FACTOR;
    pasCfgStatusData->sPerPortData[PORT1].u8VCONNOCSDebounceInms = CFG_VCONN_OCS_DEBOUNCE_IN_MS;
    pasCfgStatusData->sPerPortData[PORT1].u8VBUSMaxFaultCnt = CFG_MAX_VBUS_POWER_FAULT_COUNT;
    pasCfgStatusData->sPerPortData[PORT1].u8VCONNMaxFaultCnt = CFG_MAX_VCONN_FAULT_COUNT;
    pasCfgStatusData->sPerPortData[PORT1].u16PowerGoodTimerInms = MILLISECONDS_TO_TICKS(CFG_POWER_GOOD_TIMER_MS);
    pasCfgStatusData->sPerPortData[PORT1].u16MaxSrcPrtCurrentIn10mA = CFG_MAX_PORT_CURRENT_IN_10mA;     
    
    pasCfgStatusData->sPerPortData[PORT1].u8SourcePDOCnt = CFG_PORT_1_SOURCE_NUM_OF_PDOS;
    /* PDO Update*/
    pasCfgStatusData->sPerPortData[PORT1].u32aSourcePDO[INDEX_0] = CFG_PORT_1_SOURCE_PDO_1;    
    pasCfgStatusData->sPerPortData[PORT1].u32aSourcePDO[INDEX_1] = CFG_PORT_1_SOURCE_PDO_2;
    pasCfgStatusData->sPerPortData[PORT1].u32aSourcePDO[INDEX_2] = CFG_PORT_1_SOURCE_PDO_3;
    pasCfgStatusData->sPerPortData[PORT1].u32aSourcePDO[INDEX_3] = CFG_PORT_1_SOURCE_PDO_4;
    pasCfgStatusData->sPerPortData[PORT1].u32aSourcePDO[INDEX_4] = CFG_PORT_1_SOURCE_PDO_5;
    pasCfgStatusData->sPerPortData[PORT1].u32aSourcePDO[INDEX_5] = CFG_PORT_1_SOURCE_PDO_6;
    pasCfgStatusData->sPerPortData[PORT1].u32aSourcePDO[INDEX_6] = CFG_PORT_1_SOURCE_PDO_7;

    pasCfgStatusData->sPerPortData[PORT1].u16FeatureSelect = (CFG_PORT_1_ENABLE_VDM_SUPPORT | \
                                            CFG_PORT_1_ENABLE_AME_MONITORING);
    
    /*Configure role swap policy for Port 1 */
    pasCfgStatusData->sPerPortData[PORT1].u16SwapPolicy = CFG_PORT_1_ROLE_SWAP_POLICY;
    
    #if (TRUE == INCLUDE_PD_VDM)
    /* PD Identity Configuration for Port 1 */
    pasCfgStatusData->sVDMPerPortData[PORT1].u8PDIdentityCnt = CFG_PORT_1_PD_IDENTITY_COUNT;
    
    pasCfgStatusData->sVDMPerPortData[PORT1].u32aPDIdentity[INDEX_0] = CFG_PORT_1_ID_HEADER_VDO;
    pasCfgStatusData->sVDMPerPortData[PORT1].u32aPDIdentity[INDEX_1] = CFG_PORT_1_CERT_STAT_VDO_XID;
    pasCfgStatusData->sVDMPerPortData[PORT1].u32aPDIdentity[INDEX_2] = CFG_PORT_1_PRODUCT_VDO;   
    /* Note: Application shall configure the Product Type VDOs in indices 3-5
       depending upon the product type */    
    #if (TRUE == INCLUDE_PD_ALT_MODE)
    /* SVIDs Configuration for Port 1 */
    pasCfgStatusData->sAltModePerPortData[PORT1].u8SVIDsCnt = CFG_PORT_1_SVID_COUNT;
    
    pasCfgStatusData->sAltModePerPortData[PORT1].u16aSVIDsTable[INDEX_0] = CFG_PORT_1_SVID_0;
    pasCfgStatusData->sAltModePerPortData[PORT1].u16aSVIDsTable[INDEX_1] = CFG_PORT_1_SVID_1;

    /* Modes Configuration for Port 1 */
    pasCfgStatusData->sAltModePerPortData[PORT1].u32aModesTable[INDEX_0] = CFG_PORT_1_SVID_0_MODE_0;
    pasCfgStatusData->sAltModePerPortData[PORT1].u32aModesTable[INDEX_1] = CFG_PORT_1_SVID_0_MODE_1;
    pasCfgStatusData->sAltModePerPortData[PORT1].u32aModesTable[INDEX_2] = CFG_PORT_1_SVID_1_MODE_0;
    pasCfgStatusData->sAltModePerPortData[PORT1].u32aModesTable[INDEX_3] = CFG_PORT_1_SVID_1_MODE_1;
    
    pasCfgStatusData->sAltModePerPortData[PORT1].u8aSVIDEntryTable[INDEX_0] = \
                      (CFG_PORT_1_SVID_0_MODES_COUNT | CFG_PORT_1_SVID_0_START_MODE_INDEX);
    pasCfgStatusData->sAltModePerPortData[PORT1].u8aSVIDEntryTable[INDEX_1] = \
                      (CFG_PORT_1_SVID_1_MODES_COUNT | CFG_PORT_1_SVID_1_START_MODE_INDEX);        
    #endif    
    #endif 
    
    #if(TRUE == INCLUDE_UPD_HPD)
    pasCfgStatusData->sPerPortData[PORT1].u8PIO_HPD = CFG_PORT_1_HPD_IO;
    #endif
    
#endif
    
}



/* *****************************************************************************
 End of File
 */
