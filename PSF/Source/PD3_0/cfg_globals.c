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

/*******************************************************************/
/******************* Global variables********************************/
/*******************************************************************/

GLOBAL_CONFIG_DATA gasPortConfigurationData;

GLOBAL_STATUS_DATA gsPortStatusData;

static void CFG_PerPortParams (UINT8 u8PortNum);

static void CFG_PerPortParams (UINT8 u8PortNum)
{
    
    gasPortConfigurationData.sPortConfigData[u8PortNum].u32PortCfgData =       \
                    ((CONFIG_PORT_n_ENABLE << TYPEC_PORT_ENDIS_POS)| \
                    ((CONFIG_PORT_n_RP_CURRENT_VALUE) << TYPEC_PORT_RPVAL_POS)|\
                    (CONFIG_PORT_n_POWER_ROLE));
    gasPortConfigurationData.sPortConfigData[u8PortNum].u32PDO[0] =           \
                  (((CONFIG_PORT_n_SOURCE_USB_SUSPEND) << 28) | \
                  ((CONFIG_PORT_n_SOURCE_UNCONSTRAINED_PWR) << 27) | \
                   ((CONFIG_PORT_n_SOURCE_USB_COM) << 26) | \
                (((CONFIG_PORT_n_SOURCE_PDO_1_VOLTAGE)/50) << 10) | \
                    ((CONFIG_PORT_n_SOURCE_PDO_1_CURRENT)/10));
    gasPortConfigurationData.sPortConfigData[u8PortNum].u32PDO[1] =           \
                          ((((CONFIG_PORT_n_SOURCE_PDO_2_VOLTAGE)/50) << 10) | \
                           ((CONFIG_PORT_n_SOURCE_PDO_2_CURRENT)/10));
    gasPortConfigurationData.sPortConfigData[u8PortNum].u32PDO[2] =           \
                          ((((CONFIG_PORT_n_SOURCE_PDO_3_VOLTAGE)/50) << 10) | \
                           ((CONFIG_PORT_n_SOURCE_PDO_3_CURRENT)/10));
    gasPortConfigurationData.sPortConfigData[u8PortNum].u32PDO[3] =           \
                          ((((CONFIG_PORT_n_SOURCE_PDO_4_VOLTAGE)/50) << 10) | \
                           ((CONFIG_PORT_n_SOURCE_PDO_4_CURRENT)/10));
    gasPortConfigurationData.sPortConfigData[u8PortNum].u32PDO[4] =           \
                          ((((CONFIG_PORT_n_SOURCE_PDO_5_VOLTAGE)/50) << 10) | \
                           ((CONFIG_PORT_n_SOURCE_PDO_5_CURRENT)/10));
    gasPortConfigurationData.sPortConfigData[u8PortNum].u32PDO[5] =           \
                          ((((CONFIG_PORT_n_SOURCE_PDO_6_VOLTAGE)/50) << 10) | \
                           ((CONFIG_PORT_n_SOURCE_PDO_6_CURRENT)/10));
    gasPortConfigurationData.sPortConfigData[u8PortNum].u32PDO[6] =           \
                          ((((CONFIG_PORT_n_SOURCE_PDO_7_VOLTAGE)/50) << 10) | \
                           ((CONFIG_PORT_n_SOURCE_PDO_7_CURRENT)/10));
    gasPortConfigurationData.sPortConfigData[u8PortNum].u8PDOCnt = CONFIG_PORT_n_SOURCE_NUM_OF_PDOS;

    gasPortConfigurationData.sPortConfigData[u8PortNum].u8VBUSEnPio = CONFIG_PORT_n_UPD_EN_VBUS;
    gasPortConfigurationData.sPortConfigData[u8PortNum].u8VBUSEnMode = CONFIG_PORT_n_UPD_EN_VBUS_PIO_MODE;
    gasPortConfigurationData.sPortConfigData[u8PortNum].u8FaultInPio = CONFIG_PORT_n_UPD_FAULT_IN_PIO_NO;
    gasPortConfigurationData.sPortConfigData[u8PortNum].u8FaultInPioMode = CONFIG_PORT_n_UPD_FAULT_IN_MODE;
    gasPortConfigurationData.sPortConfigData[u8PortNum].u8VBUSDisPio = CONFIG_PORT_n_UPD_VBUS_DIS_PIO_NO;
    gasPortConfigurationData.sPortConfigData[u8PortNum].u8VBUSDisPioMode = CONFIG_PORT_n_UPD_VBUS_DIS_PIO_MODE;
    gasPortConfigurationData.sPortConfigData[u8PortNum].u8DCDCEnPio = CONFIG_PORT_n_UPD_DC_DC_EN_PIO_NO;
    gasPortConfigurationData.sPortConfigData[u8PortNum].u8DCDCEnPioMode = CONFIG_PORT_n_UPD_DC_DC_EN_PIO_MODE;

    gasPortConfigurationData.sPortConfigData[u8PortNum].u8VSELPio[0] =         \
                                                CONFIG_PORT_n_UPD_VSEL0_PIO_NO;
    gasPortConfigurationData.sPortConfigData[u8PortNum].u8VSELPio[1] =         \
                                                CONFIG_PORT_n_UPD_VSEL1_PIO_NO;
    gasPortConfigurationData.sPortConfigData[u8PortNum].u8VSELPio[2] =         \
                                                CONFIG_PORT_n_UPD_VSEL2_PIO_NO;

    gasPortConfigurationData.sPortConfigData[u8PortNum].u8VSELPioMode[0] =       \
                                               CONFIG_PORT_n_UPD_VSEL0_PIO_MODE;
    gasPortConfigurationData.sPortConfigData[u8PortNum].u8VSELPioMode[1] =       \
                                               CONFIG_PORT_n_UPD_VSEL1_PIO_MODE;
    gasPortConfigurationData.sPortConfigData[u8PortNum].u8VSELPioMode[2] =       \
                                               CONFIG_PORT_n_UPD_VSEL2_PIO_MODE;
    
    gasPortConfigurationData.sPortConfigData[u8PortNum].u8VSELTruthTable[0] =  \
                                             CONFIG_PORT_n_VSAFE0V_VSEL_MAPPING;
    gasPortConfigurationData.sPortConfigData[u8PortNum].u8VSELTruthTable[1] =  \
                                             CONFIG_PORT_n_PDO_1_VSEL_MAPPING;
    gasPortConfigurationData.sPortConfigData[u8PortNum].u8VSELTruthTable[2] =  \
                                             CONFIG_PORT_n_PDO_2_VSEL_MAPPING;
    gasPortConfigurationData.sPortConfigData[u8PortNum].u8VSELTruthTable[3] =  \
                                             CONFIG_PORT_n_PDO_3_VSEL_MAPPING;
    gasPortConfigurationData.sPortConfigData[u8PortNum].u8VSELTruthTable[4] =  \
                                             CONFIG_PORT_n_PDO_4_VSEL_MAPPING;
    gasPortConfigurationData.sPortConfigData[u8PortNum].u8VSELTruthTable[5] =  \
                                             CONFIG_PORT_n_PDO_5_VSEL_MAPPING;
    gasPortConfigurationData.sPortConfigData[u8PortNum].u8VSELTruthTable[6] =  \
                                             CONFIG_PORT_n_PDO_6_VSEL_MAPPING;
    gasPortConfigurationData.sPortConfigData[u8PortNum].u8VSELTruthTable[7] =  \
                                             CONFIG_PORT_n_PDO_7_VSEL_MAPPING;
    
    gasPortConfigurationData.sPortConfigData[u8PortNum].u8OCSThreshold = 0x00;
    gasPortConfigurationData.sPortConfigData[u8PortNum].u8FaultInDebounce = CONFIG_FAULT_IN_OCS_DEBOUNCE_MS;
    gasPortConfigurationData.sPortConfigData[u8PortNum].u8OVThreshold = CONFIG_OVER_VOLTAGE_FACTOR;
    gasPortConfigurationData.sPortConfigData[u8PortNum].u8UVThreshold = CONFIG_UNDER_VOLTAGE_FACTOR;
    gasPortConfigurationData.sPortConfigData[u8PortNum].u8VCONNOCS = CONFIG_VCONN_OCS_ENABLE;
    gasPortConfigurationData.sPortConfigData[u8PortNum].u8VCONNOCSDebounce = CONFIG_VCONN_OCS_DEBOUNCE_IN_MS;
    gasPortConfigurationData.sPortConfigData[u8PortNum].u8MaxFaultCntVBUS = CONFIG_MAX_VBUS_POWER_FAULT_COUNT;
    gasPortConfigurationData.sPortConfigData[u8PortNum].u8MaxFaultCntVCONN = CONFIG_MAX_VCONN_FAULT_COUNT;
    gasPortConfigurationData.sPortConfigData[u8PortNum].u16PowerGoodTimer = MILLISECONDS_TO_TICKS(10000);    
    gasPortConfigurationData.sPBPortConfigData[u8PortNum].u16MaxPrtCurrent = 0x12C; 
    gasPortConfigurationData.sPBPortConfigData[u8PortNum].u16MaxPrtPwrBankA = 0x00F0; 
    gasPortConfigurationData.sPBPortConfigData[u8PortNum].u8PBEn = 0x01; 
}

void CFG_LoadDefaults ()
{
    UINT8 u8PortNum = 0;
    
    gasPortConfigurationData.u8MinorVersion = STRUCT_MINOR_VERSION;
    gasPortConfigurationData.u8MajorVersion = STRUCT_MAJOR_VERSION;
    gasPortConfigurationData.u16ProducdID = PRODUCT_ID;
    gasPortConfigurationData.u16VendorID = VENDOR_ID;
    gasPortConfigurationData.u16ProductTypeVDO = PRODUCT_TYPE_VDO;
    gasPortConfigurationData.u16ProductVDO = PRODUCT_VDO;
    gasPortConfigurationData.u16CertStatVDO = CERT_STAT_VDO;
    gasPortConfigurationData.u16IDHeaderVDO = ID_HEADER_VDO;
    gasPortConfigurationData.u8HWVersion = HW_VERSION;
    gasPortConfigurationData.u8SiVersion = SILICON_VERSION;
    gasPortConfigurationData.u8PwrThrottleCfg = PD_THROTTLE_BANK_A;
    gasPortConfigurationData.u8PBEnableSelect = 0x10; 
    gasPortConfigurationData.u16SystemPpwerBankA = 0x01E0; 
    gasPortConfigurationData.u16MinPowerBankA = 0x003C; 
    
    for (u8PortNum = 0; u8PortNum < CONFIG_PD_PORT_COUNT; u8PortNum++)
    {
        CFG_PerPortParams (u8PortNum);
    }

}

