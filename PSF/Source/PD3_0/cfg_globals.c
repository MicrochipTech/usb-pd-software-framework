/*******************************************************************************
  Configuration Globals Source File

  Company:
    Microchip Technology Inc.

  File Name:
    cfg_globals.c

  Description:
    This file contains user configurable globals and functions. 
 *******************************************************************************/
/*******************************************************************************
Copyright ©  [2019] Microchip Technology Inc. and its subsidiaries.

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
#include <psf_stdinc.h>
#include <string.h>
/*******************************************************************/
/******************* Global variables********************************/
/*******************************************************************/

GLOBAL_CFG_STATUS_DATA gasCfgStatusData;

static void CFG_PerPortParams (UINT8 u8PortNum);
static void CFG_PBPerPortParams (UINT8 u8PortNum);

static void CFG_PerPortParams (UINT8 u8PortNum)
{
    
    gasCfgStatusData.sPerPortData[u8PortNum].u32CfgData =       \
                    ((CONFIG_PORT_ENABLE << TYPEC_PORT_ENDIS_POS)| \
                    ((CONFIG_PORT_RP_CURRENT_VALUE) << TYPEC_PORT_RPVAL_POS)|\
                    (CONFIG_PORT_POWER_ROLE));
    gasCfgStatusData.sPerPortData[u8PortNum].u32FixedPDO[0] =           \
                  (((CONFIG_PORT_SOURCE_USB_SUSP) << 28) | \
                  ((CONFIG_PORT_SOURCE_UNCONSTARINED_PWR) << 27) | \
                   ((CONFIG_PORT_SOURCE_USB_COMM) << 26) | \
                (((CONFIG_PORT_SOURCE_PDO_1_VOLTAGE)/50) << 10) | \
                    ((CONFIG_PORT_SOURCE_PDO_1_CURRENT)/10));
    gasCfgStatusData.sPerPortData[u8PortNum].u32FixedPDO[1] =           \
                          ((((CONFIG_PORT_SOURCE_PDO_2_VOLTAGE)/50) << 10) | \
                           ((CONFIG_PORT_SOURCE_PDO_2_CURRENT)/10));
    gasCfgStatusData.sPerPortData[u8PortNum].u32FixedPDO[2] =           \
                          ((((CONFIG_PORT_SOURCE_PDO_3_VOLTAGE)/50) << 10) | \
                           ((CONFIG_PORT_SOURCE_PDO_3_CURRENT)/10));
    gasCfgStatusData.sPerPortData[u8PortNum].u32FixedPDO[3] =           \
                          ((((CONFIG_PORT_SOURCE_PDO_4_VOLTAGE)/50) << 10) | \
                           ((CONFIG_PORT_SOURCE_PDO_4_CURRENT)/10));
    gasCfgStatusData.sPerPortData[u8PortNum].u32FixedPDO[4] =           \
                          ((((CONFIG_PORT_SOURCE_PDO_5_VOLTAGE)/50) << 10) | \
                           ((CONFIG_PORT_SOURCE_PDO_5_CURRENT)/10));
    gasCfgStatusData.sPerPortData[u8PortNum].u32FixedPDO[5] =           \
                          ((((CONFIG_PORT_SOURCE_PDO_6_VOLTAGE)/50) << 10) | \
                           ((CONFIG_PORT_SOURCE_PDO_6_CURRENT)/10));
    gasCfgStatusData.sPerPortData[u8PortNum].u32FixedPDO[6] =           \
                          ((((CONFIG_PORT_SOURCE_PDO_7_VOLTAGE)/50) << 10) | \
                           ((CONFIG_PORT_SOURCE_PDO_7_CURRENT)/10));
    gasCfgStatusData.sPerPortData[u8PortNum].u8FixedPDOCnt = CONFIG_PORT_SOURCE_NUM_OF_PDOS;

    gasCfgStatusData.sPerPortData[u8PortNum].u8VSELTruthTable[0] =  \
                                             CONFIG_PORT_VSAFE0V_VSEL_MAPPING;
    gasCfgStatusData.sPerPortData[u8PortNum].u8VSELTruthTable[1] =  \
                                             CONFIG_PORT_PDO_1_VSEL_MAPPING;
    gasCfgStatusData.sPerPortData[u8PortNum].u8VSELTruthTable[2] =  \
                                             CONFIG_PORT_PDO_2_VSEL_MAPPING;
    gasCfgStatusData.sPerPortData[u8PortNum].u8VSELTruthTable[3] =  \
                                             CONFIG_PORT_PDO_3_VSEL_MAPPING;
    gasCfgStatusData.sPerPortData[u8PortNum].u8VSELTruthTable[4] =  \
                                             CONFIG_PORT_PDO_4_VSEL_MAPPING;
    gasCfgStatusData.sPerPortData[u8PortNum].u8VSELTruthTable[5] =  \
                                             CONFIG_PORT_PDO_5_VSEL_MAPPING;
    gasCfgStatusData.sPerPortData[u8PortNum].u8VSELTruthTable[6] =  \
                                             CONFIG_PORT_PDO_6_VSEL_MAPPING;
    gasCfgStatusData.sPerPortData[u8PortNum].u8VSELTruthTable[7] =  \
                                             CONFIG_PORT_PDO_7_VSEL_MAPPING;
    
    gasCfgStatusData.sPerPortData[u8PortNum].u8OCSThresholdPercentage = 0x00;
    gasCfgStatusData.sPerPortData[u8PortNum].u8FaultInDebounce = CONFIG_FAULT_IN_OCS_DEBOUNCE_MS;
    gasCfgStatusData.sPerPortData[u8PortNum].u8OVThresholdPercentage = CONFIG_OVER_VOLTAGE_FACTOR;
    gasCfgStatusData.sPerPortData[u8PortNum].u8UVThresholdPercentage = CONFIG_UNDER_VOLTAGE_FACTOR;
    gasCfgStatusData.sPerPortData[u8PortNum].u8VCONNOCS = CONFIG_VCONN_OCS_ENABLE;
    gasCfgStatusData.sPerPortData[u8PortNum].u8VCONNOCSDebounce = CONFIG_VCONN_OCS_DEBOUNCE_IN_MS;
    gasCfgStatusData.sPerPortData[u8PortNum].u8MaxFaultCntVBUS = CONFIG_MAX_VBUS_POWER_FAULT_COUNT;
    gasCfgStatusData.sPerPortData[u8PortNum].u8MaxFaultCntVCONN = CONFIG_MAX_VCONN_FAULT_COUNT;
    gasCfgStatusData.sPerPortData[u8PortNum].u16PowerGoodTimer = CONFIG_POWER_GOOD_TIMER_MS;

    gasCfgStatusData.sPerPortData[u8PortNum].u8Pio_VBUS_EN = (UINT8)CONFIG_PORT_UPD_EN_VBUS;
    gasCfgStatusData.sPerPortData[u8PortNum].u8Mode_VBUS_EN = (UINT8)CONFIG_PORT_UPD_EN_VBUS_PIO_MODE;
    gasCfgStatusData.sPerPortData[u8PortNum].u8Pio_FAULT_IN = (UINT8)CONFIG_PORT_UPD_FAULT_IN_PIO_NO;
    gasCfgStatusData.sPerPortData[u8PortNum].u8Mode_FAULT_IN = (UINT8)CONFIG_PORT_UPD_FAULT_IN_MODE;
    gasCfgStatusData.sPerPortData[u8PortNum].u8Pio_VBUS_DIS = (UINT8)CONFIG_PORT_UPD_VBUS_DIS_PIO_NO;
    gasCfgStatusData.sPerPortData[u8PortNum].u8mode_VBUS_DIS = (UINT8)CONFIG_PORT_UPD_VBUS_DIS_PIO_MODE;
    gasCfgStatusData.sPerPortData[u8PortNum].u8Pio_DC_DC_EN = (UINT8)CONFIG_PORT_UPD_DC_DC_EN_PIO_NO;
    gasCfgStatusData.sPerPortData[u8PortNum].u8Mode_DC_DC_EN = (UINT8)CONFIG_PORT_UPD_DC_DC_EN_PIO_MODE;

    gasCfgStatusData.sPerPortData[u8PortNum].u8Pio_VSEL[0] =         \
                                                (UINT8)CONFIG_PORT_UPD_VSEL0_PIO_NO;
    gasCfgStatusData.sPerPortData[u8PortNum].u8Pio_VSEL[1] =         \
                                                (UINT8)CONFIG_PORT_UPD_VSEL1_PIO_NO;
    gasCfgStatusData.sPerPortData[u8PortNum].u8Pio_VSEL[2] =         \
                                                (UINT8)CONFIG_PORT_UPD_VSEL2_PIO_NO;

    gasCfgStatusData.sPerPortData[u8PortNum].u8Mode_VSEL[0] =       \
                                               (UINT8)CONFIG_PORT_UPD_VSEL0_PIO_MODE;
    gasCfgStatusData.sPerPortData[u8PortNum].u8Mode_VSEL[1] =       \
                                               (UINT8)CONFIG_PORT_UPD_VSEL1_PIO_MODE;
    gasCfgStatusData.sPerPortData[u8PortNum].u8Mode_VSEL[2] =       \
                                               (UINT8)CONFIG_PORT_UPD_VSEL2_PIO_MODE;
    
}

void CFG_PBPerPortParams (UINT8 u8PortNum)
{
    gasCfgStatusData.sPBPerPortData[u8PortNum].u16MaxPrtPwrBankA = CFG_PB_MAX_PORT_POWER_BANKA;
    gasCfgStatusData.sPBPerPortData[u8PortNum].u16MaxPrtPwrBankB = CFG_PB_MAX_PORT_POWER_BANKB;
    gasCfgStatusData.sPBPerPortData[u8PortNum].u16MaxPrtPwrBankC = CFG_PB_MAX_PORT_POWER_BANKC;
    gasCfgStatusData.sPBPerPortData[u8PortNum].u16MaxPrtCurrent = CFG_PB_MAX_PORT_CURRENT;
    gasCfgStatusData.sPBPerPortData[u8PortNum].u8PBEnablePriority = ((u8PortNum << 1) | CFG_PB_PORT_ENABLE);
}

void CFG_LoadDefaults ()
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
    gasCfgStatusData.u8PwrThrottleCfg = PD_THROTTLE_BANK_A;
    
    gasCfgStatusData.u16SystemPowerBankA = CFG_PB_TOT_SYS_POWER_BANKA;
    gasCfgStatusData.u16MinPowerBankA = CFG_PB_MIN_POWER_BANKA;
    gasCfgStatusData.u16SystemPowerBankB = CFG_PB_TOT_SYS_POWER_BANKB;
    gasCfgStatusData.u16MinPowerBankB = CFG_PB_MIN_POWER_BANKB;
    gasCfgStatusData.u16SystemPowerBankC =CFG_PB_TOT_SYS_POWER_BANKC;
    gasCfgStatusData.u16MinPowerBankC = CFG_PB_MIN_POWER_BANKC;
    
    for (u8PortNum = 0; u8PortNum < CONFIG_PD_PORT_COUNT; u8PortNum++)
    {
        CFG_PerPortParams (u8PortNum);
        CFG_PBPerPortParams (u8PortNum);
    }

}

