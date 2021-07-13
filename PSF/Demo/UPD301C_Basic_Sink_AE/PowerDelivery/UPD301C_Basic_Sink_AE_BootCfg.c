/*************************************************************************** 
  User Configuration File

  Company:
    Microchip Technology Inc.

  File Name:
    UPD301C_Basic_Sink_AE_BootCfg.c

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
#include "psf_stdinc.h"
#include "psf_adc.h"
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
    pasCfgStatusData->sPerPortData[PORT0].u32CfgData =       
        ((CFG_PORT_0_VCONN_OCS_ENABLE) | (CFG_PORT_0_ENABLE) | (CFG_PORT_0_RP_CURRENT_VALUE)| \
        (CFG_PORT_0_DUAL_ROLE_DATA_SUPPORT) | (CFG_PORT_0_POWER_ROLE));

    
    pasCfgStatusData->sPerPortData[PORT0].u8Pio_FAULT_IN = (UINT8)CFG_PORT_0_UPD_FAULT_IN_PIO_NO;
    pasCfgStatusData->sPerPortData[PORT0].u8Mode_FAULT_IN = (UINT8)CFG_PORT_0_UPD_FAULT_IN_MODE;
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
    
	pasCfgStatusData->sPerPortData[PORT0].u8SinkConfigSel = ((CFG_PORT_0_SINK_MODE_A)| \
            (CFG_PORT_0_SINK_USB_SUSP) | (CFG_PORT_0_SINK_GIVE_BACK_FLAG ));
			
    /*Assigning PDOs according to the ADC position*/
    pasCfgStatusData->sPerPortData[PORT0].u8SinkPDOCnt = CFG_PORT_0_SINK_NUM_OF_PDOS;
    UINT8 u8ADCPosition = Get_ADCPosition();
    memset(gasCfgStatusData.sPerPortData[PORT0].u32aSinkPDO,0,7);
    switch(u8ADCPosition)
    {
        case 1:
        {
            /*Position 1*/
            /*Supported PDO is (5V,3A)*/
            gasCfgStatusData.sPerPortData[PORT0].u32aSinkPDO[0] = CFG_PORT_0_SINK_PDO_1;
            gasCfgStatusData.sPerPortData[PORT0].u8SinkPDOCnt = 1;
            break;
        }
        case 2:
        {
            /*Position 2*/
            /*Supported PDOs are (5V,3A), (9V,3A)*/
            gasCfgStatusData.sPerPortData[PORT0].u32aSinkPDO[0] = CFG_PORT_0_SINK_PDO_1;
            gasCfgStatusData.sPerPortData[PORT0].u32aSinkPDO[1] = CFG_PORT_0_SINK_PDO_2;
            gasCfgStatusData.sPerPortData[PORT0].u8SinkPDOCnt = 2;
            break;
        }
        case 3:
        {
            /*Position 3*/
            /*Supported PDOs are (5V,3A), (15V,3A)*/
            gasCfgStatusData.sPerPortData[PORT0].u32aSinkPDO[0] = CFG_PORT_0_SINK_PDO_1;
            gasCfgStatusData.sPerPortData[PORT0].u32aSinkPDO[1] = CFG_PORT_0_SINK_PDO_3;
            gasCfgStatusData.sPerPortData[PORT0].u8SinkPDOCnt = 2;
            break;
        }
        case 4:
        {
            /*Position 4*/
            /*Supported PDOs are (5V,3A), (20V,3A)*/
            gasCfgStatusData.sPerPortData[PORT0].u32aSinkPDO[0] = CFG_PORT_0_SINK_PDO_1;
            gasCfgStatusData.sPerPortData[PORT0].u32aSinkPDO[1] = CFG_PORT_0_SINK_PDO_4;
            gasCfgStatusData.sPerPortData[PORT0].u8SinkPDOCnt = 2;
            break;
        }
        case 5:
        {
            /*Position 5*/
            /*Operates in Sink Mode A*/
            /*Resetting sink PDOs for sink to negotiate appropriate PDO in Mode A*/
            gasCfgStatusData.sPerPortData[PORT0].u32aSinkPDO[0] = CFG_PORT_0_SINK_PDO_1;
            gasCfgStatusData.sPerPortData[PORT0].u32aSinkPDO[1] = CFG_PORT_0_SINK_PDO_2;
            gasCfgStatusData.sPerPortData[PORT0].u32aSinkPDO[2] = CFG_PORT_0_SINK_PDO_3;
            gasCfgStatusData.sPerPortData[PORT0].u32aSinkPDO[3] = CFG_PORT_0_SINK_PDO_4;
            gasCfgStatusData.sPerPortData[PORT0].u8SinkPDOCnt = 4;
            
            gasCfgStatusData.sPerPortData[PORT0].u8SinkConfigSel = ((CFG_PORT_0_SINK_MODE_A)| \
            (CFG_PORT_0_SINK_USB_SUSP) | (CFG_PORT_0_SINK_GIVE_BACK_FLAG ));
            
           /*Selects PDOs according to the mode selected*/
            //PSF_SinkSelectMode();
//            gasCfgStatusData.sPerPortData[PORT0].u32CfgData |= DPM_CFG_NEGOTIATE_USING_NEW_PDOS;
//            gasCfgStatusData.sPerPortData[PORT0].u32ClientRequest |= DPM_CLIENT_REQ_RENEGOTIATE;

            /*5V default negotiation required?*/
//            gasCfgStatusData.sPerPortData[PORT0].u32aSinkPDO[0] = CFG_PORT_0_SINK_PDO_1;
//            gasCfgStatusData.sPerPortData[PORT0].u32aSinkPDO[1] = CFG_FORM_FIXED_PDOx(gasCfgStatusData.sPerPortData[PORT0].u16NegoVoltageInmV, gasCfgStatusData.sPerPortData[PORT0].u16NegoCurrentInmA);
//            gasCfgStatusData.sPerPortData[PORT0].u8SinkPDOCnt = 2;
            break;
        }
        case 6:
        {
            /*Position 6*/
            /*Operates in Sink Mode B*/
            /*Resetting sink PDOs for sink to negotiate appropriate PDO in Mode B*/
            gasCfgStatusData.sPerPortData[PORT0].u32aSinkPDO[0] = CFG_PORT_0_SINK_PDO_1;
            gasCfgStatusData.sPerPortData[PORT0].u32aSinkPDO[1] = CFG_PORT_0_SINK_PDO_2;
            gasCfgStatusData.sPerPortData[PORT0].u32aSinkPDO[2] = CFG_PORT_0_SINK_PDO_3;
            gasCfgStatusData.sPerPortData[PORT0].u32aSinkPDO[3] = CFG_PORT_0_SINK_PDO_4;
            gasCfgStatusData.sPerPortData[PORT0].u8SinkPDOCnt = 4;
            
            gasCfgStatusData.sPerPortData[PORT0].u8SinkConfigSel = ((CFG_PORT_0_SINK_MODE_B)| \
            (CFG_PORT_0_SINK_USB_SUSP) | (CFG_PORT_0_SINK_GIVE_BACK_FLAG ));
            
            /*Selects PDOs according to the mode selected*/
            //PSF_SinkSelectMode();
//            gasCfgStatusData.sPerPortData[PORT0].u32CfgData |= DPM_CFG_NEGOTIATE_USING_NEW_PDOS;
//            gasCfgStatusData.sPerPortData[PORT0].u32ClientRequest |= DPM_CLIENT_REQ_RENEGOTIATE;

            /*5V default negotiation required?*/
//            gasCfgStatusData.sPerPortData[PORT0].u32aSinkPDO[0] = CFG_PORT_0_SINK_PDO_1;
//            gasCfgStatusData.sPerPortData[PORT0].u32aSinkPDO[1] = CFG_FORM_FIXED_PDOx(gasCfgStatusData.sPerPortData[PORT0].u16NegoVoltageInmV, gasCfgStatusData.sPerPortData[PORT0].u16NegoCurrentInmA);
//            gasCfgStatusData.sPerPortData[PORT0].u8SinkPDOCnt = 2;
            break;
        }
    }
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
			
    /*Configure role swap policy for note type port*/
    pasCfgStatusData->sPerPortData[PORT0].u16SwapPolicy = CFG_PORT_0_ROLE_SWAP_POLICY;

    pasCfgStatusData->sPerPortData[PORT0].u16FeatureSelect = (CFG_PORT_0_ENABLE_VDM_SUPPORT | \
                                            CFG_PORT_0_ENABLE_AME_MONITORING);
    
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
               
}



/* *****************************************************************************
 End of File
 */
