/*************************************************************************** 
  User Configuration File

  Company:
    Microchip Technology Inc.

  File Name:
    PSF_BootCfg.c

  Summary:
    User Configuration Source file

  Description:
    This source file contains user configurable globals and functions.

************************************************************************** */
/*******************************************************************************
Copyright © [2022] Microchip Technology Inc. and its subsidiaries.

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
	UINT8 u8Length = SET_TO_ZERO;
	
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



/***********************************Port Specific configuration***********************/    

/*************************************************************************************/
/**************************************Port 0 Configurations**************************/
/*************************************************************************************/
    pasCfgStatusData->sPerPortData[PORT0].u32CfgData = CFG_PORT_0_CFG_DATA;
    
    pasCfgStatusData->sPerPortData[PORT0].u8Pio_FAULT_IN = (UINT8)CFG_PORT_0_UPD_FAULT_IN_PIO_NO;
    pasCfgStatusData->sPerPortData[PORT0].u8Mode_FAULT_IN = (UINT8)CFG_PORT_0_UPD_FAULT_IN_MODE;
    pasCfgStatusData->sPerPortData[PORT0].u8Pio_EN_SINK  = (UINT8)CFG_PORT_0_UPD_EN_SNK;
    pasCfgStatusData->sPerPortData[PORT0].u8Mode_EN_SINK  = (UINT8)CFG_PORT_0_UPD_EN_SNK_PIO_MODE;

    pasCfgStatusData->sPerPortData[PORT0].u8OCSThresholdPercentage = SET_TO_ZERO;
    pasCfgStatusData->sPerPortData[PORT0].u8OVThresholdPercentage = CFG_PORT_0_OVER_VOLTAGE_FACTOR;
    pasCfgStatusData->sPerPortData[PORT0].u8UVThresholdPercentage = CFG_PORT_0_UNDER_VOLTAGE_FACTOR;
    pasCfgStatusData->sPerPortData[PORT0].u8FaultInDebounceInms = CFG_PORT_0_FAULT_IN_OCS_DEBOUNCE_MS;
    pasCfgStatusData->sPerPortData[PORT0].u8VCONNOCSDebounceInms = CFG_PORT_0_VCONN_OCS_DEBOUNCE_IN_MS;
    pasCfgStatusData->sPerPortData[PORT0].u8VBUSMaxFaultCnt = CFG_PORT_0_MAX_VBUS_POWER_FAULT_COUNT;
    pasCfgStatusData->sPerPortData[PORT0].u8VCONNMaxFaultCnt = CFG_PORT_0_MAX_VCONN_FAULT_COUNT;
    pasCfgStatusData->sPerPortData[PORT0].u16PowerGoodTimerInms = MILLISECONDS_TO_TICKS(CFG_PORT_0_POWER_GOOD_TIMER_MS);
    pasCfgStatusData->sPerPortData[PORT0].u16MaxSrcPrtCurrentIn10mA = CFG_PORT_0_MAX_PORT_CURRENT_IN_10mA;

/***************************************Sink Configurations***************************/
    pasCfgStatusData->sPerPortData[PORT0].u8SinkConfigSel = CFG_PORT_0_SINK_CONFIG_SEL;
    pasCfgStatusData->sPerPortData[PORT0].u8SinkPDOCnt = CFG_PORT_0_SINK_NUM_OF_PDOS; 

    /*PDOs update*/ 
    pasCfgStatusData->sPerPortData[PORT0].u32aSinkPDO[INDEX_0] = CFG_PORT_0_SINK_PDO_1;
    pasCfgStatusData->sPerPortData[PORT0].u32aSinkPDO[INDEX_1] = CFG_PORT_0_SINK_PDO_2;
    pasCfgStatusData->sPerPortData[PORT0].u32aSinkPDO[INDEX_2] = CFG_PORT_0_SINK_PDO_3;
    pasCfgStatusData->sPerPortData[PORT0].u32aSinkPDO[INDEX_3] = CFG_PORT_0_SINK_PDO_4;

/*Assigning maximum operating current and minimum operating current for Note
      type port to 3000mA and 1000mA respectively*/
    pasCfgStatusData->sPerPortData[PORT0].u16SnkMaxOperatingCurInmA = \
            CFG_PORT_0_SINK_MAX_OPERATING_CURRENT_InmA;
    pasCfgStatusData->sPerPortData[PORT0].u16SnkMinOperatingCurInmA = \
            CFG_PORT_0_SINK_MIN_OPERATING_CURRENT_InmA;

/*Assigning PDO preferred minimum current*/
pasCfgStatusData->sPerPortData[PORT0].u16aMinPDOPreferredCurInmA[INDEX_0] = \
            CFG_PORT_0_SINK_PDO_1_PREFERRED_MIN_CURRENT;
pasCfgStatusData->sPerPortData[PORT0].u16aMinPDOPreferredCurInmA[INDEX_1] = \
            CFG_PORT_0_SINK_PDO_2_PREFERRED_MIN_CURRENT;
pasCfgStatusData->sPerPortData[PORT0].u16aMinPDOPreferredCurInmA[INDEX_2] = \
            CFG_PORT_0_SINK_PDO_3_PREFERRED_MIN_CURRENT;
pasCfgStatusData->sPerPortData[PORT0].u16aMinPDOPreferredCurInmA[INDEX_3] = \
            CFG_PORT_0_SINK_PDO_4_PREFERRED_MIN_CURRENT;

/*Assigning DAC_I configuration*/
    pasCfgStatusData->sPerPortData[PORT0].u16DAC_I_CurrentInd_MaxInA = \
            CFG_PORT_0_SINK_DAC_I_CUR_INDICATION_MAX;
    pasCfgStatusData->sPerPortData[PORT0].u16DAC_I_MaxOutVoltInmV = \
            CFG_PORT_0_SINK_DAC_I_MAX_OP_VOLTAGE;
    pasCfgStatusData->sPerPortData[PORT0].u16DAC_I_MinOutVoltInmV = \
            CFG_PORT_0_SINK_DAC_I_MIN_OP_VOLTAGE;
    pasCfgStatusData->sPerPortData[PORT0].u8DAC_I_Direction = \
            CFG_PORT_0_SINK_DAC_I_DIR;

/* Load Extended Sink Capabilities */
    pasCfgStatusData->sPerPortData[PORT0].u8aSinkCapsExtd[INDEX_0] = \
                            LOBYTE(CFG_PORT_0_SINK_CAPS_EXTD_VID); 
    pasCfgStatusData->sPerPortData[PORT0].u8aSinkCapsExtd[INDEX_1] = \
                            HIBYTE(CFG_PORT_0_SINK_CAPS_EXTD_VID); 
    pasCfgStatusData->sPerPortData[PORT0].u8aSinkCapsExtd[INDEX_2] = \
                            LOBYTE(CFG_PORT_0_SINK_CAPS_EXTD_PID); 
    pasCfgStatusData->sPerPortData[PORT0].u8aSinkCapsExtd[INDEX_3] = \
                            HIBYTE(CFG_PORT_0_SINK_CAPS_EXTD_PID); 
    pasCfgStatusData->sPerPortData[PORT0].u8aSinkCapsExtd[INDEX_4] = \
                            LOBYTE(LOWORD(CFG_PORT_0_SINK_CAPS_EXTD_XID)); 
    pasCfgStatusData->sPerPortData[PORT0].u8aSinkCapsExtd[INDEX_5] = \
                            HIBYTE(LOWORD(CFG_PORT_0_SINK_CAPS_EXTD_XID)); 
    pasCfgStatusData->sPerPortData[PORT0].u8aSinkCapsExtd[INDEX_6] = \
                            LOBYTE(HIWORD(CFG_PORT_0_SINK_CAPS_EXTD_XID)); 
    pasCfgStatusData->sPerPortData[PORT0].u8aSinkCapsExtd[INDEX_7] = \
                            LOBYTE(HIWORD(CFG_PORT_0_SINK_CAPS_EXTD_XID)); 
    pasCfgStatusData->sPerPortData[PORT0].u8aSinkCapsExtd[INDEX_8] = \
                            CFG_PORT_0_SINK_CAPS_EXTD_FW_VERSION;
    pasCfgStatusData->sPerPortData[PORT0].u8aSinkCapsExtd[INDEX_9] = \
                            CFG_PORT_0_SINK_CAPS_EXTD_HW_VERSION;
    pasCfgStatusData->sPerPortData[PORT0].u8aSinkCapsExtd[INDEX_10] = \
                            CFG_PORT_0_SINK_CAPS_EXTD_SKEDB_VERSION;
    pasCfgStatusData->sPerPortData[PORT0].u8aSinkCapsExtd[INDEX_11] = \
                            CFG_PORT_0_SINK_CAPS_EXTD_LOAD_STEP;
    pasCfgStatusData->sPerPortData[PORT0].u8aSinkCapsExtd[INDEX_12] = \
                            LOBYTE(CFG_PORT_0_SINK_CAPS_EXTD_LOAD_CHARACTERISTICS);
    pasCfgStatusData->sPerPortData[PORT0].u8aSinkCapsExtd[INDEX_13] = \
                            HIBYTE(CFG_PORT_0_SINK_CAPS_EXTD_LOAD_CHARACTERISTICS);
    pasCfgStatusData->sPerPortData[PORT0].u8aSinkCapsExtd[INDEX_14] = \
                            CFG_PORT_0_SINK_CAPS_EXTD_COMPLIANCE;
    pasCfgStatusData->sPerPortData[PORT0].u8aSinkCapsExtd[INDEX_15] = \
                            CFG_PORT_0_SINK_CAPS_EXTD_TOUCH_TEMP;
    pasCfgStatusData->sPerPortData[PORT0].u8aSinkCapsExtd[INDEX_16] = \
                            CFG_PORT_0_SINK_CAPS_EXTD_BATTERY_INFO;
    pasCfgStatusData->sPerPortData[PORT0].u8aSinkCapsExtd[INDEX_17] = \
                            CFG_PORT_0_SINK_CAPS_EXTD_SINK_MODES;
    pasCfgStatusData->sPerPortData[PORT0].u8aSinkCapsExtd[INDEX_18] = \
                            CFG_PORT_0_SINK_CAPS_EXTD_MIN_PDP_In_WATT;
    pasCfgStatusData->sPerPortData[PORT0].u8aSinkCapsExtd[INDEX_19] = \
                            CFG_PORT_0_SINK_CAPS_EXTD_OPERATIONAL_PDP_In_WATT;
    pasCfgStatusData->sPerPortData[PORT0].u8aSinkCapsExtd[INDEX_20] = \
                            CFG_PORT_0_SINK_CAPS_EXTD_MAX_PDP_In_WATT;

 /*Configure role swap policy for note type port*/
    pasCfgStatusData->sPerPortData[PORT0].u16SwapPolicy = CFG_PORT_0_ROLE_SWAP_POLICY;

    pasCfgStatusData->sPerPortData[PORT0].u16FeatureSelect = CFG_PORT_0_FEATURE_SELECT;                  
}



/* *****************************************************************************
 End of File
 */
