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

void CFG_NoteSourceParams(GLOBAL_CFG_STATUS_DATA *pasCfgStatusData);
void CFG_NoteSinkPDOs(GLOBAL_CFG_STATUS_DATA *pasCfgStatusData);
void CFG_DockSourceParams(GLOBAL_CFG_STATUS_DATA *pasCfgStatusData);
void CFG_DockSinkParams(GLOBAL_CFG_STATUS_DATA *pasCfgStatusData);

static void CFG_PerPortParams (UINT8 u8PortNum, GLOBAL_CFG_STATUS_DATA *pasCfgStatusData)
{    
    if(PORT0 == u8PortNum)
    {
        pasCfgStatusData->sPerPortData[u8PortNum].u32CfgData =       
                ((CFG_VCONN_OCS_ENABLE) | (CFG_PORT_ENABLE) | (CFG_PORT_RP_CURRENT_VALUE_0)| \
                (CFG_PORT_DUAL_ROLE_DATA_0) | (CFG_PORT_POWER_ROLE_0));
        
        switch(DPM_GET_CONFIGURED_POWER_ROLE(PORT0))
        {
            case PD_ROLE_DRP:
            {
                CFG_NoteSourceParams(pasCfgStatusData);
                CFG_NoteSinkPDOs(pasCfgStatusData);

/* TODO: <Role swap> <Enable the below line once role swap is stable> */
#if 0                
                /*Configure role swap policy for note type port*/
                gasCfgStatusData.sPerPortData[PORT0].u16SwapPolicy = \
                    (CFG_PORT_NOTE_AS_DFP_REQUEST_DR_SWAP | CFG_PORT_NOTE_AS_UFP_REQUEST_DR_SWAP | \
                    CFG_PORT_NOTE_AS_DFP_ACCEPT_DR_SWAP| CFG_PORT_NOTE_AS_UFP_ACCEPT_DR_SWAP | \
                    CFG_PORT_NOTE_AS_SRC_REQUEST_PR_SWAP | CFG_PORT_NOTE_AS_SNK_REQUEST_PR_SWAP |\
                    CFG_PORT_NOTE_AS_SRC_ACCEPT_PR_SWAP | CFG_PORT_NOTE_AS_SNK_ACCEPT_PR_SWAP);
#endif
                break;
            }
            case PD_ROLE_SOURCE:
            {
                CFG_NoteSourceParams(pasCfgStatusData);
                break;
            }
            case PD_ROLE_SINK:
            {
                CFG_NoteSinkPDOs(pasCfgStatusData);
                break;
            }
            default:
                break;
        }
    }
    else /*PORT1*/
    {
        pasCfgStatusData->sPerPortData[u8PortNum].u32CfgData =       
                ((CFG_VCONN_OCS_ENABLE) | (CFG_PORT_ENABLE) | (CFG_PORT_RP_CURRENT_VALUE_1)| \
                (CFG_PORT_DUAL_ROLE_DATA_1) | (CFG_PORT_POWER_ROLE_1));
        
        switch(DPM_GET_CONFIGURED_POWER_ROLE(PORT1))
        {
            case PD_ROLE_DRP:
            {
                CFG_DockSourceParams(pasCfgStatusData);
                CFG_DockSinkParams(pasCfgStatusData);

/* TODO: <Role swap> <Enable the below line once role swap is stable> */
#if 0                
                /*Configure role swap policy for dock type port*/
                gasCfgStatusData.sPerPortData[PORT1].u16SwapPolicy = \
                    (CFG_PORT_DOCK_AS_DFP_REQUEST_DR_SWAP | CFG_PORT_DOCK_AS_UFP_REQUEST_DR_SWAP | \
                    CFG_PORT_DOCK_AS_DFP_ACCEPT_DR_SWAP| CFG_PORT_DOCK_AS_UFP_ACCEPT_DR_SWAP | \
                    CFG_PORT_DOCK_AS_SRC_REQUEST_PR_SWAP | CFG_PORT_DOCK_AS_SNK_REQUEST_PR_SWAP |\
                    CFG_PORT_DOCK_AS_SRC_ACCEPT_PR_SWAP | CFG_PORT_DOCK_AS_SNK_ACCEPT_PR_SWAP);
#endif                
                break;
            }
            case PD_ROLE_SOURCE:
            {
                CFG_DockSourceParams(pasCfgStatusData);
                break;
            }
            case PD_ROLE_SINK:
            {
                CFG_DockSinkParams(pasCfgStatusData);
                break;
            }
            default:
                break;
        }
    }
    
    pasCfgStatusData->sPerPortData[u8PortNum].u8Pio_FAULT_IN = (UINT8)CFG_PORT_UPD_FAULT_IN_PIO_NO;
    pasCfgStatusData->sPerPortData[u8PortNum].u8Mode_FAULT_IN = (UINT8)CFG_PORT_UPD_FAULT_IN_MODE;
    
    pasCfgStatusData->sPerPortData[u8PortNum].u8OCSThresholdPercentage = SET_TO_ZERO;
    pasCfgStatusData->sPerPortData[u8PortNum].u8FaultInDebounceInms = CFG_FAULT_IN_OCS_DEBOUNCE_MS;
    pasCfgStatusData->sPerPortData[u8PortNum].u8OVThresholdPercentage = CFG_OVER_VOLTAGE_FACTOR;
    pasCfgStatusData->sPerPortData[u8PortNum].u8UVThresholdPercentage = CFG_UNDER_VOLTAGE_FACTOR;
    pasCfgStatusData->sPerPortData[u8PortNum].u8VCONNOCSDebounceInms = CFG_VCONN_OCS_DEBOUNCE_IN_MS;
    pasCfgStatusData->sPerPortData[u8PortNum].u8VBUSMaxFaultCnt = CFG_MAX_VBUS_POWER_FAULT_COUNT;
    pasCfgStatusData->sPerPortData[u8PortNum].u8VCONNMaxFaultCnt = CFG_MAX_VCONN_FAULT_COUNT;
    pasCfgStatusData->sPerPortData[u8PortNum].u16PowerGoodTimerInms = MILLISECONDS_TO_TICKS(CFG_POWER_GOOD_TIMER_MS);
    pasCfgStatusData->sPerPortData[u8PortNum].u16MaxSrcPrtCurrentIn10mA = CFG_MAX_PORT_CURRENT_IN_10mA; 
    pasCfgStatusData->sPerPortData[u8PortNum].u16FeatureSelect = CFG_PB_PORT_ENABLE;

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

void CFG_NoteSourceParams(GLOBAL_CFG_STATUS_DATA *pasCfgStatusData)
{
        /* PDO 1: Fixed PDO with 5V,3A capability */
    pasCfgStatusData->sPerPortData[PORT0].u32aSourcePDO[INDEX_0] = \
            CFG_FORM_SOURCE_FIXED_PDO1(CFG_PORT_NOTE_SOURCE_PDO_1_VOLTAGE, \
            CFG_PORT_NOTE_SOURCE_PDO_1_CURRENT, CFG_PORT_DUAL_ROLE_DATA, \
            CFG_PORT_SOURCE_USB_COMM, CFG_PORT_SOURCE_USB_SUSP,  \
            CFG_PORT_SOURCE_UNCONSTARINED_PWR, CFG_PORT_DUAL_ROLE_POWER);    

    /* TODO: <PSF Configuration> No need to load these registers since 
            PDO count for Notebook port is 1 */
    /* PDO 2: Fixed PDO with 9V,3A capability */
    pasCfgStatusData->sPerPortData[PORT0].u32aSourcePDO[INDEX_1] = \
            CFG_FORM_FIXED_PDOx(CFG_PORT_NOTE_SOURCE_PDO_2_VOLTAGE, \
            CFG_PORT_NOTE_SOURCE_PDO_2_CURRENT);        

    /* PDO 3: Fixed PDO with 15V,3A capability */
    pasCfgStatusData->sPerPortData[PORT0].u32aSourcePDO[INDEX_2] = \
            CFG_FORM_FIXED_PDOx(CFG_PORT_NOTE_SOURCE_PDO_3_VOLTAGE, \
            CFG_PORT_NOTE_SOURCE_PDO_3_CURRENT);     

    /* PDO 4: Fixed PDO with 20V,3A capability */
    pasCfgStatusData->sPerPortData[PORT0].u32aSourcePDO[INDEX_3] = \
            CFG_FORM_FIXED_PDOx(CFG_PORT_NOTE_SOURCE_PDO_4_VOLTAGE, \
            CFG_PORT_NOTE_SOURCE_PDO_4_CURRENT);         

    pasCfgStatusData->sPerPortData[PORT0].u8SourcePDOCnt = CFG_PORT_NOTE_SOURCE_NUM_OF_PDOS;
    
    pasCfgStatusData->sPerPortData[PORT0].u8Pio_EN_VBUS = (UINT8)CFG_PORT_UPD_EN_VBUS;
    pasCfgStatusData->sPerPortData[PORT0].u8Mode_EN_VBUS = (UINT8)CFG_PORT_UPD_EN_VBUS_PIO_MODE;
}

void CFG_NoteSinkPDOs(GLOBAL_CFG_STATUS_DATA *pasCfgStatusData)
{
    pasCfgStatusData->sPerPortData[PORT0].u32aSinkPDO[INDEX_0] = \
                   CFG_FORM_SINK_FIXED_PDO1(CFG_PORT_NOTE_SINK_PDO_1_CURRENT, \
                   CFG_PORT_NOTE_SINK_PDO_1_VOLTAGE, CFG_PORT_DUAL_ROLE_DATA, CFG_PORT_SINK_USB_COMM, \
                   CFG_PORT_SINK_UNCONSTRAINED_PWR,CFG_PORT_SINK_HIGHER_CAPABILITY, CFG_PORT_DUAL_ROLE_POWER);    

    pasCfgStatusData->sPerPortData[PORT0].u32aSinkPDO[INDEX_1] = \
            CFG_FORM_FIXED_PDOx(CFG_PORT_NOTE_SINK_PDO_2_VOLTAGE, \
            CFG_PORT_NOTE_SINK_PDO_2_CURRENT);        

    pasCfgStatusData->sPerPortData[PORT0].u32aSinkPDO[INDEX_2] = \
            CFG_FORM_FIXED_PDOx(CFG_PORT_NOTE_SINK_PDO_3_VOLTAGE, \
            CFG_PORT_NOTE_SINK_PDO_3_CURRENT);     

    pasCfgStatusData->sPerPortData[PORT0].u32aSinkPDO[INDEX_3] = \
            CFG_FORM_FIXED_PDOx(CFG_PORT_NOTE_SINK_PDO_4_VOLTAGE, \
            CFG_PORT_NOTE_SINK_PDO_4_CURRENT);         

    pasCfgStatusData->sPerPortData[PORT0].u8SinkPDOCnt = CFG_PORT_NOTE_SINK_NUM_OF_PDOS;
    
    /*Assigning maximum operating current and minimum operating current for Note
      type port to 3000mA and 1000mA respectively*/
    pasCfgStatusData->sPerPortData[PORT0].u16SnkMaxOperatingCurInmA = \
            CFG_PORT_NOTE_SINK_MAX_OPERATING_CURRENT_InmA;
    pasCfgStatusData->sPerPortData[PORT0].u16SnkMinOperatingCurInmA = \
            CFG_PORT_NOTE_SINK_MIN_OPERATING_CURRENT_InmA;
    
    /*Assigning PDO preferred minimum current for Note type port*/
    pasCfgStatusData->sPerPortData[PORT0].u16aMinPDOPreferredCurInmA[INDEX_0] = \
            CFG_PORT_SINK_PDO_1_PREFERRED_MIN_CURRENT;
    pasCfgStatusData->sPerPortData[PORT0].u16aMinPDOPreferredCurInmA[INDEX_1] = \
            CFG_PORT_SINK_PDO_2_PREFERRED_MIN_CURRENT;
    pasCfgStatusData->sPerPortData[PORT0].u16aMinPDOPreferredCurInmA[INDEX_2] = \
            CFG_PORT_SINK_PDO_3_PREFERRED_MIN_CURRENT;
    pasCfgStatusData->sPerPortData[PORT0].u16aMinPDOPreferredCurInmA[INDEX_3] = \
            CFG_PORT_SINK_PDO_4_PREFERRED_MIN_CURRENT;
    
    pasCfgStatusData->sPerPortData[PORT0].u8Pio_EN_SINK  = (UINT8)CFG_PORT_UPD_EN_SNK;
    pasCfgStatusData->sPerPortData[PORT0].u8Mode_EN_SINK  = (UINT8)CFG_PORT_UPD_EN_SNK_PIO_MODE;
    
	pasCfgStatusData->sPerPortData[PORT0].u8SinkConfigSel = ((CFG_PORT_SINK_MODE_A)| \
            (CFG_PORT_SINK_USB_SUSP << DPM_SINK_CONFIG_NO_USB_SUSP_POS) |\
            (CFG_PORT_SINK_GIVE_BACK_FLAG << DPM_SINK_CONFIG_GIVE_BACK_FLAG_POS));
    
    pasCfgStatusData->sPerPortData[PORT0].u16DAC_I_CurrentInd_MaxInA = \
            CFG_PORT_SINK_DAC_I_CUR_INDICATION_MAX;
    pasCfgStatusData->sPerPortData[PORT0].u16DAC_I_MaxOutVoltInmV = \
            CFG_PORT_SINK_DAC_I_MAX_OP_VOLTAGE;
    pasCfgStatusData->sPerPortData[PORT0].u16DAC_I_MinOutVoltInmV = \
            CFG_PORT_SINK_DAC_I_MIN_OP_VOLTAGE;
    pasCfgStatusData->sPerPortData[PORT0].u8DAC_I_Direction = \
            CFG_PORT_SINK_DAC_I_DIR_HIGH_AMP_MAX_VOLT;
}

void CFG_DockSourceParams(GLOBAL_CFG_STATUS_DATA *pasCfgStatusData)
{
    /* PDO 1: Fixed PDO with 5V,3A capability */
    pasCfgStatusData->sPerPortData[PORT1].u32aSourcePDO[INDEX_0] = \
            CFG_FORM_SOURCE_FIXED_PDO1(CFG_PORT_DOCK_SOURCE_PDO_1_VOLTAGE, \
            CFG_PORT_DOCK_SOURCE_PDO_1_CURRENT, CFG_PORT_DUAL_ROLE_DATA, \
            CFG_PORT_SOURCE_USB_COMM, CFG_PORT_SOURCE_USB_SUSP,  \
            CFG_PORT_SOURCE_UNCONSTARINED_PWR, CFG_PORT_DUAL_ROLE_POWER);    

    /* PDO 2: Fixed PDO with 9V,3A capability */
    pasCfgStatusData->sPerPortData[PORT1].u32aSourcePDO[INDEX_1] = \
            CFG_FORM_FIXED_PDOx(CFG_PORT_DOCK_SOURCE_PDO_2_VOLTAGE, \
            CFG_PORT_DOCK_SOURCE_PDO_2_CURRENT);        

    /* PDO 3: Fixed PDO with 15V,3A capability */
    pasCfgStatusData->sPerPortData[PORT1].u32aSourcePDO[INDEX_2] = \
            CFG_FORM_FIXED_PDOx(CFG_PORT_DOCK_SOURCE_PDO_3_VOLTAGE, \
            CFG_PORT_DOCK_SOURCE_PDO_3_CURRENT);     

    /* PDO 4: Fixed PDO with 20V,3A capability */
    pasCfgStatusData->sPerPortData[PORT1].u32aSourcePDO[INDEX_3] = \
            CFG_FORM_FIXED_PDOx(CFG_PORT_DOCK_SOURCE_PDO_4_VOLTAGE, \
            CFG_PORT_DOCK_SOURCE_PDO_4_CURRENT);         

    /* PDO 5: PPS APDO with 3.3V-21V, 3A capability */
    pasCfgStatusData->sPerPortData[PORT1].u32aSourcePDO[INDEX_4] = \
            CFG_FORM_PPS_APDO(CFG_POWER_SUPPLY_TYPE_PROGRAMMABLE, \
            CFG_PORT_SOURCE_APDO_5_MIN_VOLTAGE, CFG_PORT_SOURCE_APDO_5_MAX_VOLTAGE, \
            CFG_PORT_SOURCE_APDO_5_MAX_CURRENT, CFG_PORT_SOURCE_APDO_5_PPS_PWR_LTD);            

    pasCfgStatusData->sPerPortData[PORT1].u8SourcePDOCnt = CFG_PORT_DOCK_SOURCE_NUM_OF_PDOS;
    
    pasCfgStatusData->sPerPortData[PORT1].u8Pio_EN_VBUS = (UINT8)CFG_PORT_UPD_EN_VBUS;
    pasCfgStatusData->sPerPortData[PORT1].u8Mode_EN_VBUS = (UINT8)CFG_PORT_UPD_EN_VBUS_PIO_MODE;
}

void CFG_DockSinkParams(GLOBAL_CFG_STATUS_DATA *pasCfgStatusData)
{
    pasCfgStatusData->sPerPortData[PORT1].u32aSinkPDO[INDEX_0] = 
            CFG_FORM_SINK_FIXED_PDO1(CFG_PORT_DOCK_SINK_PDO_1_CURRENT, \
            CFG_PORT_DOCK_SINK_PDO_1_VOLTAGE, CFG_PORT_DUAL_ROLE_DATA, \
            CFG_PORT_SINK_USB_COMM, CFG_PORT_SINK_UNCONSTRAINED_PWR, \
            CFG_PORT_SINK_HIGHER_CAPABILITY, CFG_PORT_DUAL_ROLE_POWER);    

    pasCfgStatusData->sPerPortData[PORT1].u32aSinkPDO[INDEX_1] = \
            CFG_FORM_FIXED_PDOx(CFG_PORT_DOCK_SINK_PDO_2_VOLTAGE, \
            CFG_PORT_DOCK_SINK_PDO_2_CURRENT);        

    pasCfgStatusData->sPerPortData[PORT1].u32aSinkPDO[INDEX_2] = \
            CFG_FORM_FIXED_PDOx(CFG_PORT_DOCK_SINK_PDO_3_VOLTAGE, \
            CFG_PORT_DOCK_SINK_PDO_3_CURRENT);     

    pasCfgStatusData->sPerPortData[PORT1].u32aSinkPDO[INDEX_3] = \
            CFG_FORM_FIXED_PDOx(CFG_PORT_DOCK_SINK_PDO_4_VOLTAGE, \
            CFG_PORT_DOCK_SINK_PDO_4_CURRENT);         \


    pasCfgStatusData->sPerPortData[PORT1].u8SinkPDOCnt = CFG_PORT_DOCK_SINK_NUM_OF_PDOS;
    
    /*Assigning both maximum operating current and minimum operating current for Dock
    type port to 0A respectively*/
    pasCfgStatusData->sPerPortData[PORT1].u16SnkMaxOperatingCurInmA = \
            CFG_PORT_DOCK_SINK_MAX_OPERATING_CURRENT_InmA;
    pasCfgStatusData->sPerPortData[PORT1].u16SnkMinOperatingCurInmA = \
            CFG_PORT_DOCK_SINK_MIN_OPERATING_CURRENT_InmA;
    
    pasCfgStatusData->sPerPortData[PORT1].u8Pio_EN_SINK  = (UINT8)CFG_PORT_UPD_EN_SNK;
    pasCfgStatusData->sPerPortData[PORT1].u8Mode_EN_SINK  = (UINT8)CFG_PORT_UPD_EN_SNK_PIO_MODE;
    
	pasCfgStatusData->sPerPortData[PORT1].u8SinkConfigSel = ((CFG_PORT_SINK_MODE_A)| \
            (CFG_PORT_SINK_USB_SUSP << DPM_SINK_CONFIG_NO_USB_SUSP_POS) |\
            (CFG_PORT_SINK_GIVE_BACK_FLAG << DPM_SINK_CONFIG_GIVE_BACK_FLAG_POS));
    
}
/* ************************************************************************** */
/* ************************************************************************** */
// Section: Interface Functions                                               */
/* ************************************************************************** */
/* ************************************************************************** */
void PSF_LoadConfig(GLOBAL_CFG_STATUS_DATA *pasCfgStatusData)
{
    UINT8 u8PortNum = SET_TO_ZERO;
    
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
    
    for (u8PortNum = SET_TO_ZERO; u8PortNum < CONFIG_PD_PORT_COUNT; u8PortNum++)
    {
        CFG_PerPortParams (u8PortNum, pasCfgStatusData);  
        CFG_PBPerPortParams (u8PortNum, pasCfgStatusData);
    }

}


/* *****************************************************************************
 End of File
 */
