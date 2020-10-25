/*************************************************************************** 
  User Configuration File

  Company:
    Microchip Technology Inc.

  File Name:
    PSFDrp_BootCfg.c

  Summary:
    User Configuration Source file

  Description:
    This source file contains user configurable globals and functions.

************************************************************************** */
/*******************************************************************************
Copyright ©  [2020] Microchip Technology Inc. and its subsidiaries.

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
#include <psf_stdinc.h>

/*******************************************************************/
/******************* Global variables********************************/
/*******************************************************************/

/* ************************************************************************** */
/* ************************************************************************** */
// Section: Local Functions                                                   */
/* ************************************************************************** */
/* ************************************************************************** */
static void CFG_Port_0_SourcePDOs(GLOBAL_CFG_STATUS_DATA *pasCfgStatusData);
static void CFG_Port_0_SinkPDOs(GLOBAL_CFG_STATUS_DATA *pasCfgStatusData);

#if (PORT_COUNT_2 == CONFIG_PD_PORT_COUNT)

static void CFG_Port_1_SourcePDOs(GLOBAL_CFG_STATUS_DATA *pasCfgStatusData);
static void CFG_Port_1_SinkPDOs(GLOBAL_CFG_STATUS_DATA *pasCfgStatusData);

static void CFG_Port_1_PDOs (GLOBAL_CFG_STATUS_DATA *pasCfgStatusData)
{
    switch(CFG_GET_CONFIGURED_POWER_ROLE(PORT1))
    {
        case PD_ROLE_DRP:
        {
            CFG_Port_1_SourcePDOs(pasCfgStatusData);
            CFG_Port_1_SinkPDOs(pasCfgStatusData);
            break;
        }
        case PD_ROLE_SOURCE:
        {
            CFG_Port_1_SourcePDOs(pasCfgStatusData);
            break;
        }
        case PD_ROLE_SINK:
        {
            CFG_Port_1_SinkPDOs(pasCfgStatusData);
            break;
        }
        default:
            break;
    }
}

static void CFG_Port_1_SourcePDOs(GLOBAL_CFG_STATUS_DATA *pasCfgStatusData)
{
    pasCfgStatusData->sPerPortData[PORT1].u8SourcePDOCnt = CFG_PORT_1_SOURCE_NUM_OF_PDOS;
    /* PDO Update*/
    pasCfgStatusData->sPerPortData[PORT1].u32aSourcePDO[INDEX_0] = CFG_PORT_1_SOURCE_PDO_1;    
    pasCfgStatusData->sPerPortData[PORT1].u32aSourcePDO[INDEX_1] = CFG_PORT_1_SOURCE_PDO_2;
    pasCfgStatusData->sPerPortData[PORT1].u32aSourcePDO[INDEX_2] = CFG_PORT_1_SOURCE_PDO_3;
    pasCfgStatusData->sPerPortData[PORT1].u32aSourcePDO[INDEX_3] = CFG_PORT_1_SOURCE_PDO_4;
    pasCfgStatusData->sPerPortData[PORT1].u32aSourcePDO[INDEX_4] = CFG_PORT_1_SOURCE_PDO_5;
    pasCfgStatusData->sPerPortData[PORT1].u32aSourcePDO[INDEX_5] = CFG_PORT_1_SOURCE_PDO_6;
    pasCfgStatusData->sPerPortData[PORT1].u32aSourcePDO[INDEX_6] = CFG_PORT_1_SOURCE_PDO_7;
}

static void CFG_Port_1_SinkPDOs(GLOBAL_CFG_STATUS_DATA *pasCfgStatusData)
{
    pasCfgStatusData->sPerPortData[PORT1].u8SinkPDOCnt = CFG_PORT_1_SINK_NUM_OF_PDOS;
    /*PDO Updated*/
    pasCfgStatusData->sPerPortData[PORT1].u32aSinkPDO[INDEX_0] = CFG_PORT_1_SINK_PDO_1;  
    pasCfgStatusData->sPerPortData[PORT1].u32aSinkPDO[INDEX_1] = CFG_PORT_1_SINK_PDO_2;
    pasCfgStatusData->sPerPortData[PORT1].u32aSinkPDO[INDEX_2] = CFG_PORT_1_SINK_PDO_3;
    pasCfgStatusData->sPerPortData[PORT1].u32aSinkPDO[INDEX_3] = CFG_PORT_1_SINK_PDO_4;
    pasCfgStatusData->sPerPortData[PORT1].u32aSinkPDO[INDEX_4] = CFG_PORT_1_SINK_PDO_5;
    pasCfgStatusData->sPerPortData[PORT1].u32aSinkPDO[INDEX_5] = CFG_PORT_1_SINK_PDO_6;
    pasCfgStatusData->sPerPortData[PORT1].u32aSinkPDO[INDEX_6] = CFG_PORT_1_SINK_PDO_7;
     
    /*Assigning both maximum operating current and minimum operating current for Dock
    type port to 0A respectively*/
    pasCfgStatusData->sPerPortData[PORT1].u16SnkMaxOperatingCurInmA = \
            CFG_PORT_1_SINK_MAX_OPERATING_CURRENT_InmA;
    pasCfgStatusData->sPerPortData[PORT1].u16SnkMinOperatingCurInmA = \
            CFG_PORT_1_SINK_MIN_OPERATING_CURRENT_InmA;
    
	pasCfgStatusData->sPerPortData[PORT1].u8SinkConfigSel = ((CFG_PORT_1_SINK_MODE)| \
            (CFG_PORT_1_SINK_USB_SUSP) | (CFG_PORT_1_SINK_GIVE_BACK_FLAG));
    
}

#endif /*(PORT_COUNT_2 == CONFIG_PD_PORT_COUNT)*/

static void CFG_Port_0_PDOs (GLOBAL_CFG_STATUS_DATA *pasCfgStatusData)
{
    switch(CFG_GET_CONFIGURED_POWER_ROLE(PORT0))
    {
        case PD_ROLE_DRP:
        {
            CFG_Port_0_SourcePDOs(pasCfgStatusData);
            CFG_Port_0_SinkPDOs(pasCfgStatusData);
            break;
        }
        case PD_ROLE_SOURCE:
        {
            CFG_Port_0_SourcePDOs(pasCfgStatusData);
            break;
        }
        case PD_ROLE_SINK:
        {
            CFG_Port_0_SinkPDOs(pasCfgStatusData);
            break;
        }
        default:
            break;
    }
}

static void CFG_Port_0_SourcePDOs(GLOBAL_CFG_STATUS_DATA *pasCfgStatusData)
{
    pasCfgStatusData->sPerPortData[PORT0].u8SourcePDOCnt = CFG_PORT_0_SOURCE_NUM_OF_PDOS;  
    
    /*PDOs update*/
    pasCfgStatusData->sPerPortData[PORT0].u32aSourcePDO[INDEX_0] = CFG_PORT_0_SOURCE_PDO_1;   
    pasCfgStatusData->sPerPortData[PORT0].u32aSourcePDO[INDEX_1] = CFG_PORT_0_SOURCE_PDO_2;
    pasCfgStatusData->sPerPortData[PORT0].u32aSourcePDO[INDEX_2] = CFG_PORT_0_SOURCE_PDO_3;
    pasCfgStatusData->sPerPortData[PORT0].u32aSourcePDO[INDEX_3] = CFG_PORT_0_SOURCE_PDO_4;
    pasCfgStatusData->sPerPortData[PORT0].u32aSourcePDO[INDEX_4] = CFG_PORT_0_SOURCE_PDO_5;
    pasCfgStatusData->sPerPortData[PORT0].u32aSourcePDO[INDEX_5] = CFG_PORT_0_SOURCE_PDO_6;               
    pasCfgStatusData->sPerPortData[PORT0].u32aSourcePDO[INDEX_6] = CFG_PORT_0_SOURCE_PDO_7;
    
}

static void CFG_Port_0_SinkPDOs(GLOBAL_CFG_STATUS_DATA *pasCfgStatusData)
{
    pasCfgStatusData->sPerPortData[PORT0].u8SinkConfigSel = ((CFG_PORT_0_SINK_MODE)| \
            (CFG_PORT_0_SINK_USB_SUSP) | (CFG_PORT_0_SINK_GIVE_BACK_FLAG ));
    
    /*Assigning PDOs*/
    pasCfgStatusData->sPerPortData[PORT0].u8SinkPDOCnt = CFG_PORT_0_SINK_NUM_OF_PDOS;
    
    pasCfgStatusData->sPerPortData[PORT0].u32aSinkPDO[INDEX_0] = CFG_PORT_0_SINK_PDO_1;
    pasCfgStatusData->sPerPortData[PORT0].u32aSinkPDO[INDEX_1] = CFG_PORT_0_SINK_PDO_2;
    pasCfgStatusData->sPerPortData[PORT0].u32aSinkPDO[INDEX_2] = CFG_PORT_0_SINK_PDO_3;
    pasCfgStatusData->sPerPortData[PORT0].u32aSinkPDO[INDEX_3] = CFG_PORT_0_SINK_PDO_4;
    pasCfgStatusData->sPerPortData[PORT0].u32aSinkPDO[INDEX_4] = CFG_PORT_0_SINK_PDO_5;
    pasCfgStatusData->sPerPortData[PORT0].u32aSinkPDO[INDEX_5] = CFG_PORT_0_SINK_PDO_6;
    pasCfgStatusData->sPerPortData[PORT0].u32aSinkPDO[INDEX_6] = CFG_PORT_0_SINK_PDO_7;
    
    /*Assigning maximum operating current and minimum operating current for Note
      type port to 3000mA and 1000mA respectively*/
    pasCfgStatusData->sPerPortData[PORT0].u16SnkMaxOperatingCurInmA = \
            CFG_PORT_0_SINK_MAX_OPERATING_CURRENT_InmA;
    pasCfgStatusData->sPerPortData[PORT0].u16SnkMinOperatingCurInmA = \
            CFG_PORT_0_SINK_MIN_OPERATING_CURRENT_InmA;
    
    /*Assigning PDO preferred minimum current for Note type port*/
    pasCfgStatusData->sPerPortData[PORT0].u16aMinPDOPreferredCurInmA[INDEX_0] = \
            CFG_PORT_0_SINK_PDO_1_PREFERRED_MIN_CURRENT;
    pasCfgStatusData->sPerPortData[PORT0].u16aMinPDOPreferredCurInmA[INDEX_1] = \
            CFG_PORT_0_SINK_PDO_2_PREFERRED_MIN_CURRENT;
    pasCfgStatusData->sPerPortData[PORT0].u16aMinPDOPreferredCurInmA[INDEX_2] = \
            CFG_PORT_0_SINK_PDO_3_PREFERRED_MIN_CURRENT;
    pasCfgStatusData->sPerPortData[PORT0].u16aMinPDOPreferredCurInmA[INDEX_3] = \
            CFG_PORT_0_SINK_PDO_4_PREFERRED_MIN_CURRENT;
    pasCfgStatusData->sPerPortData[PORT0].u16DAC_I_CurrentInd_MaxInA = \
            CFG_PORT_0_SINK_DAC_I_CUR_INDICATION_MAX;
    pasCfgStatusData->sPerPortData[PORT0].u16DAC_I_MaxOutVoltInmV = \
            CFG_PORT_0_SINK_DAC_I_MAX_OP_VOLTAGE;
    pasCfgStatusData->sPerPortData[PORT0].u16DAC_I_MinOutVoltInmV = \
            CFG_PORT_0_SINK_DAC_I_MIN_OP_VOLTAGE;
    pasCfgStatusData->sPerPortData[PORT0].u8DAC_I_Direction = \
            CFG_PORT_0_SINK_DAC_I_DIR;
}


/* ************************************************************************** */
/* ************************************************************************** */
// Section: Interface Functions                                               */
/* ************************************************************************** */
/* ************************************************************************** */
void PSF_LoadConfig(GLOBAL_CFG_STATUS_DATA *pasCfgStatusData)
{
    UINT8 u8Length = SET_TO_ZERO;

    /*System Configuration*/    
    pasCfgStatusData->u16ProducdID = CFG_PRODUCT_ID;
    pasCfgStatusData->u16VendorID = CFG_VENDOR_ID;
    pasCfgStatusData->u8HWVersion = CFG_HW_VERSION;
    pasCfgStatusData->u8SiVersion = CFG_SILICON_VERSION;
    pasCfgStatusData->u8PwrThrottleCfg = ((CFG_PT_POWER_THROTTLE_BANK_SEL << 1) | CFG_PT_ENABLE);
    pasCfgStatusData->u8ManfStringLength = CFG_MANUFACTURER_STR_LEN;

    while(u8Length < pasCfgStatusData->u8ManfStringLength)
    {
        pasCfgStatusData->u8aManfString[u8Length] = CFG_MANUFACTURER_STR[u8Length];
        u8Length++;
    }

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

    /*Per port configuration for Port 0*/    
    pasCfgStatusData->sPerPortData[PORT0].u32CfgData =       
        ((CFG_PORT_0_VCONN_OCS_ENABLE) | (CFG_PORT_0_ENABLE) | (CFG_PORT_0_RP_CURRENT_VALUE)| \
        (CFG_PORT_0_DUAL_ROLE_DATA_SUPPORT) | (CFG_PORT_0_POWER_ROLE));
    
    pasCfgStatusData->sPerPortData[PORT0].u8Pio_FAULT_IN = (UINT8)CFG_PORT_0_UPD_FAULT_IN_PIO_NO;
    pasCfgStatusData->sPerPortData[PORT0].u8Mode_FAULT_IN = (UINT8)CFG_PORT_0_UPD_FAULT_IN_MODE;
    pasCfgStatusData->sPerPortData[PORT0].u8Pio_EN_VBUS = (UINT8)CFG_PORT_0_UPD_EN_VBUS;
    pasCfgStatusData->sPerPortData[PORT0].u8Mode_EN_VBUS = (UINT8)CFG_PORT_0_UPD_EN_VBUS_PIO_MODE;
    pasCfgStatusData->sPerPortData[PORT0].u8Pio_EN_SINK  = (UINT8)CFG_PORT_0_UPD_EN_SNK;
    pasCfgStatusData->sPerPortData[PORT0].u8Mode_EN_SINK  = (UINT8)CFG_PORT_0_UPD_EN_SNK_PIO_MODE;
     
    pasCfgStatusData->sPerPortData[PORT0].u8OCSThresholdPercentage = SET_TO_ZERO;
    pasCfgStatusData->sPerPortData[PORT0].u8OVThresholdPercentage = CFG_OVER_VOLTAGE_FACTOR;
    pasCfgStatusData->sPerPortData[PORT0].u8UVThresholdPercentage = CFG_UNDER_VOLTAGE_FACTOR;
    pasCfgStatusData->sPerPortData[PORT0].u8FaultInDebounceInms = CFG_FAULT_IN_OCS_DEBOUNCE_MS;
    pasCfgStatusData->sPerPortData[PORT0].u8VCONNOCSDebounceInms = CFG_VCONN_OCS_DEBOUNCE_IN_MS;
    pasCfgStatusData->sPerPortData[PORT0].u8VBUSMaxFaultCnt = CFG_MAX_VBUS_POWER_FAULT_COUNT;
    pasCfgStatusData->sPerPortData[PORT0].u8VCONNMaxFaultCnt = CFG_MAX_VCONN_FAULT_COUNT;
    pasCfgStatusData->sPerPortData[PORT0].u16PowerGoodTimerInms = MILLISECONDS_TO_TICKS(CFG_POWER_GOOD_TIMER_MS);
    pasCfgStatusData->sPerPortData[PORT0].u16MaxSrcPrtCurrentIn10mA = CFG_MAX_PORT_CURRENT_IN_10mA;
   
    pasCfgStatusData->sPerPortData[PORT0].u16FeatureSelect = (CFG_PB_PORT_ENABLE | CFG_PORT_0_ENABLE_VDM_SUPPORT);

    CFG_Port_0_PDOs (pasCfgStatusData);  
                  
    /*Configure role swap policy for note type port*/
    pasCfgStatusData->sPerPortData[PORT0].u16SwapPolicy = CFG_PORT_0_ROLE_SWAP_POLICY;
    
    #if ((TRUE == INCLUDE_POWER_BALANCING) || (TRUE == INCLUDE_POWER_THROTTLING))
    pasCfgStatusData->sPBPerPortData[PORT0].u16MaxPrtPwrBankAIn250mW = CFG_PB_MAX_PORT_0_POWER_BANKA_IN_250mW;
    pasCfgStatusData->sPBPerPortData[PORT0].u16MaxPrtPwrBankBIn250mW = CFG_PB_MAX_PORT_0_POWER_BANKB_IN_250mW;
    pasCfgStatusData->sPBPerPortData[PORT0].u16MaxPrtPwrBankCIn250mW = CFG_PB_MAX_PORT_0_POWER_BANKC_IN_250mW;
    pasCfgStatusData->sPBPerPortData[PORT0].u8PortPriority = PORT0;
    #endif

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

    /*Per port configuration for Port 1*/      
#if (PORT_COUNT_2 == CONFIG_PD_PORT_COUNT) 
    pasCfgStatusData->sPerPortData[PORT1].u32CfgData =       
        ((CFG_PORT_1_VCONN_OCS_ENABLE) | (CFG_PORT_1_ENABLE) | (CFG_PORT_1_RP_CURRENT_VALUE)| \
        (CFG_PORT_1_DUAL_ROLE_DATA_SUPPORT) | (CFG_PORT_1_POWER_ROLE));
        
    pasCfgStatusData->sPerPortData[PORT1].u8Pio_FAULT_IN = (UINT8)CFG_PORT_1_UPD_FAULT_IN_PIO_NO;
    pasCfgStatusData->sPerPortData[PORT1].u8Mode_FAULT_IN = (UINT8)CFG_PORT_1_UPD_FAULT_IN_MODE;
    pasCfgStatusData->sPerPortData[PORT1].u8Pio_EN_VBUS = (UINT8)CFG_PORT_1_UPD_EN_VBUS;
    pasCfgStatusData->sPerPortData[PORT1].u8Mode_EN_VBUS = (UINT8)CFG_PORT_1_UPD_EN_VBUS_PIO_MODE;
    pasCfgStatusData->sPerPortData[PORT1].u8Pio_EN_SINK  = (UINT8)CFG_PORT_1_UPD_EN_SNK;
    pasCfgStatusData->sPerPortData[PORT1].u8Mode_EN_SINK  = (UINT8)CFG_PORT_1_UPD_EN_SNK_PIO_MODE;
    
    pasCfgStatusData->sPerPortData[PORT1].u8OCSThresholdPercentage = SET_TO_ZERO;
    pasCfgStatusData->sPerPortData[PORT1].u8FaultInDebounceInms = CFG_FAULT_IN_OCS_DEBOUNCE_MS;
    pasCfgStatusData->sPerPortData[PORT1].u8OVThresholdPercentage = CFG_OVER_VOLTAGE_FACTOR;
    pasCfgStatusData->sPerPortData[PORT1].u8UVThresholdPercentage = CFG_UNDER_VOLTAGE_FACTOR;
    pasCfgStatusData->sPerPortData[PORT1].u8VCONNOCSDebounceInms = CFG_VCONN_OCS_DEBOUNCE_IN_MS;
    pasCfgStatusData->sPerPortData[PORT1].u8VBUSMaxFaultCnt = CFG_MAX_VBUS_POWER_FAULT_COUNT;
    pasCfgStatusData->sPerPortData[PORT1].u8VCONNMaxFaultCnt = CFG_MAX_VCONN_FAULT_COUNT;
    pasCfgStatusData->sPerPortData[PORT1].u16PowerGoodTimerInms = MILLISECONDS_TO_TICKS(CFG_POWER_GOOD_TIMER_MS);
    pasCfgStatusData->sPerPortData[PORT1].u16MaxSrcPrtCurrentIn10mA = CFG_MAX_PORT_CURRENT_IN_10mA;
    
    pasCfgStatusData->sPerPortData[PORT1].u16FeatureSelect = (CFG_PB_PORT_ENABLE | CFG_PORT_1_ENABLE_VDM_SUPPORT);

    CFG_Port_1_PDOs (pasCfgStatusData);  

    /*Configure role swap policy for dock type port*/
    pasCfgStatusData->sPerPortData[PORT1].u16SwapPolicy = CFG_PORT_1_ROLE_SWAP_POLICY;
    
    #if ((TRUE == INCLUDE_POWER_BALANCING) || (TRUE == INCLUDE_POWER_THROTTLING))
    pasCfgStatusData->sPBPerPortData[PORT1].u16MaxPrtPwrBankAIn250mW = CFG_PB_MAX_PORT_1_POWER_BANKA_IN_250mW;
    pasCfgStatusData->sPBPerPortData[PORT1].u16MaxPrtPwrBankBIn250mW = CFG_PB_MAX_PORT_1_POWER_BANKB_IN_250mW;
    pasCfgStatusData->sPBPerPortData[PORT1].u16MaxPrtPwrBankCIn250mW = CFG_PB_MAX_PORT_1_POWER_BANKC_IN_250mW;
    pasCfgStatusData->sPBPerPortData[PORT1].u8PortPriority = PORT1;
    #endif
    
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
