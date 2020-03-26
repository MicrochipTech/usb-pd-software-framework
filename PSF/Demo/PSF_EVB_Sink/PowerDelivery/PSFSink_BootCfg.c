/*************************************************************************** 
  User Configuration File

  Company:
    Microchip Technology Inc.

  File Name:
    PSFSink_BootCfg.c

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


/* ************************************************************************** */
/* ************************************************************************** */
// Section: Local Functions                                                   */
/* ************************************************************************** */
/* ************************************************************************** */
static void CFG_PerPortParams (UINT8 u8PortNum);

static void CFG_PerPortParams (UINT8 u8PortNum)
{    
    gasCfgStatusData.sPerPortData[u8PortNum].u32CfgData =       
                    ((CFG_VCONN_OCS_ENABLE) | (CFG_PORT_ENABLE) | (CFG_PORT_RP_CURRENT_VALUE)| \
                    (CFG_PORT_POWER_ROLE));
    
    gasCfgStatusData.sPerPortData[u8PortNum].u32aSinkPDO[0] = CFG_FORM_SINK_FIXED_PDO1(CFG_PORT_SINK_PDO_1_CURRENT, \
            CFG_PORT_SINK_PDO_1_VOLTAGE, CFG_PORT_SINK_USB_COMM, \
            CFG_PORT_SINK_UNCONSTRAINED_PWR,CFG_PORT_SINK_HIGHER_CAPABILITY);    

    gasCfgStatusData.sPerPortData[u8PortNum].u32aSinkPDO[1] = CFG_FORM_FIXED_PDOx(CFG_PORT_SINK_PDO_2_VOLTAGE, \
                                                    CFG_PORT_SINK_PDO_2_CURRENT);        

    gasCfgStatusData.sPerPortData[u8PortNum].u32aSinkPDO[2] = CFG_FORM_FIXED_PDOx(CFG_PORT_SINK_PDO_3_VOLTAGE, 
                                                    CFG_PORT_SINK_PDO_3_CURRENT);     

    gasCfgStatusData.sPerPortData[u8PortNum].u32aSinkPDO[3] = CFG_FORM_FIXED_PDOx(CFG_PORT_SINK_PDO_4_VOLTAGE, 
                                                    CFG_PORT_SINK_PDO_4_CURRENT);         \

    gasCfgStatusData.sPerPortData[u8PortNum].u32aSinkPDO[4] = CFG_FORM_FIXED_PDOx(CFG_PORT_SINK_PDO_5_VOLTAGE, 
                                                    CFG_PORT_SINK_PDO_5_CURRENT);            \

    gasCfgStatusData.sPerPortData[u8PortNum].u32aSinkPDO[5] = CFG_FORM_FIXED_PDOx(CFG_PORT_SINK_PDO_6_VOLTAGE, 
                                                    CFG_PORT_SINK_PDO_6_CURRENT);            \
    
    gasCfgStatusData.sPerPortData[u8PortNum].u32aSinkPDO[6] = CFG_FORM_FIXED_PDOx(CFG_PORT_SINK_PDO_7_VOLTAGE, 
                                                    CFG_PORT_SINK_PDO_7_CURRENT); 
    
    gasCfgStatusData.sPerPortData[u8PortNum].u8SinkPDOCnt = CFG_PORT_SINK_NUM_OF_PDOS;

    /*Assigning PDO preferred minimum current*/
    gasCfgStatusData.sPerPortData[u8PortNum].u16aMinPDOPreferredCurInmA[0] = CFG_PORT_SINK_PDO_1_PREFERRED_MIN_CURRENT;
    gasCfgStatusData.sPerPortData[u8PortNum].u16aMinPDOPreferredCurInmA[1] = CFG_PORT_SINK_PDO_2_PREFERRED_MIN_CURRENT;
    gasCfgStatusData.sPerPortData[u8PortNum].u16aMinPDOPreferredCurInmA[2] = CFG_PORT_SINK_PDO_3_PREFERRED_MIN_CURRENT;
    gasCfgStatusData.sPerPortData[u8PortNum].u16aMinPDOPreferredCurInmA[3] = CFG_PORT_SINK_PDO_4_PREFERRED_MIN_CURRENT;
    gasCfgStatusData.sPerPortData[u8PortNum].u16aMinPDOPreferredCurInmA[4] = CFG_PORT_SINK_PDO_5_PREFERRED_MIN_CURRENT;
    gasCfgStatusData.sPerPortData[u8PortNum].u16aMinPDOPreferredCurInmA[5] = CFG_PORT_SINK_PDO_6_PREFERRED_MIN_CURRENT;
    gasCfgStatusData.sPerPortData[u8PortNum].u16aMinPDOPreferredCurInmA[6] = CFG_PORT_SINK_PDO_7_PREFERRED_MIN_CURRENT;
    
   
    /*Assigning maximum operating current and minimum operating current to 
     * 3000mA and 1000mA respectively*/
    gasCfgStatusData.sPerPortData[u8PortNum].u16MaximumOperatingCurInmA = 3000;
    gasCfgStatusData.sPerPortData[u8PortNum].u16MinimumOperatingCurInmA = 1000;
    
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

	gasCfgStatusData.sPerPortData[u8PortNum].u8SinkConfigSel = ((CFG_PORT_SINK_MODE_A)| \
                                                                 (CFG_PORT_SINK_USB_SUSP << DPM_SINK_CONFIG_NO_USB_SUSP_POS) |\
                                                                 (CFG_PORT_SINK_GIVE_BACK_FLAG << DPM_SINK_CONFIG_GIVE_BACK_FLAG_POS));
}


/* ************************************************************************** */
/* ************************************************************************** */
// Section: Interface Functions                                               */
/* ************************************************************************** */
/* ************************************************************************** */
void PSF_LoadConfig()
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
    
    for (u8PortNum = 0; u8PortNum < CONFIG_PD_PORT_COUNT; u8PortNum++)
    {
        CFG_PerPortParams (u8PortNum);  
    }

}



/* *****************************************************************************
 End of File
 */
