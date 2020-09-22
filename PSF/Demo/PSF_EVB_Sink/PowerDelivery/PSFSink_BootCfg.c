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
#include <string.h>

/*******************************************************************/
/******************* Global variables********************************/
/*******************************************************************/

/* ************************************************************************** */
/* ************************************************************************** */
// Section: Local Functions                                                   */
/* ************************************************************************** */
/* ************************************************************************** */
<<<<<<< HEAD
static void CFG_PerPortParams (UINT8 u8PortNum, GLOBAL_CFG_STATUS_DATA *pasCfgStatusData);

static void CFG_PerPortParams (UINT8 u8PortNum, GLOBAL_CFG_STATUS_DATA *pasCfgStatusData)
{    
    pasCfgStatusData->sPerPortData[u8PortNum].u32CfgData =       
                    ((CFG_VCONN_OCS_ENABLE) | (CFG_PORT_ENABLE) | (CFG_PORT_RP_CURRENT_VALUE)| \
                    (CFG_PORT_POWER_ROLE)); 
    
    
    pasCfgStatusData->sPerPortData[u8PortNum].u32aSinkPDO[0] = CFG_FORM_SINK_FIXED_PDO1(CFG_PORT_SINK_PDO_1_CURRENT, \
            CFG_PORT_SINK_PDO_1_VOLTAGE, CFG_PORT_SINK_USB_COMM, \
            CFG_PORT_SINK_UNCONSTRAINED_PWR,CFG_PORT_SINK_HIGHER_CAPABILITY);    

    pasCfgStatusData->sPerPortData[u8PortNum].u32aSinkPDO[1] = CFG_FORM_FIXED_PDOx(CFG_PORT_SINK_PDO_2_VOLTAGE, \
                                                    CFG_PORT_SINK_PDO_2_CURRENT);        

    pasCfgStatusData->sPerPortData[u8PortNum].u32aSinkPDO[2] = CFG_FORM_FIXED_PDOx(CFG_PORT_SINK_PDO_3_VOLTAGE, 
                                                    CFG_PORT_SINK_PDO_3_CURRENT);     

    pasCfgStatusData->sPerPortData[u8PortNum].u32aSinkPDO[3] = CFG_FORM_FIXED_PDOx(CFG_PORT_SINK_PDO_4_VOLTAGE, 
                                                    CFG_PORT_SINK_PDO_4_CURRENT);         \

    pasCfgStatusData->sPerPortData[u8PortNum].u32aSinkPDO[4] = CFG_FORM_FIXED_PDOx(CFG_PORT_SINK_PDO_5_VOLTAGE, 
                                                    CFG_PORT_SINK_PDO_5_CURRENT);            \

    pasCfgStatusData->sPerPortData[u8PortNum].u32aSinkPDO[5] = CFG_FORM_FIXED_PDOx(CFG_PORT_SINK_PDO_6_VOLTAGE, 
                                                    CFG_PORT_SINK_PDO_6_CURRENT);            \
    
    pasCfgStatusData->sPerPortData[u8PortNum].u32aSinkPDO[6] = CFG_FORM_FIXED_PDOx(CFG_PORT_SINK_PDO_7_VOLTAGE, 
                                                    CFG_PORT_SINK_PDO_7_CURRENT); 
    
    pasCfgStatusData->sPerPortData[u8PortNum].u8SinkPDOCnt = CFG_PORT_SINK_NUM_OF_PDOS;

    /*Assigning PDO preferred minimum current*/
    pasCfgStatusData->sPerPortData[u8PortNum].u16aMinPDOPreferredCurInmA[0] = CFG_PORT_SINK_PDO_1_PREFERRED_MIN_CURRENT;
    pasCfgStatusData->sPerPortData[u8PortNum].u16aMinPDOPreferredCurInmA[1] = CFG_PORT_SINK_PDO_2_PREFERRED_MIN_CURRENT;
    pasCfgStatusData->sPerPortData[u8PortNum].u16aMinPDOPreferredCurInmA[2] = CFG_PORT_SINK_PDO_3_PREFERRED_MIN_CURRENT;
    pasCfgStatusData->sPerPortData[u8PortNum].u16aMinPDOPreferredCurInmA[3] = CFG_PORT_SINK_PDO_4_PREFERRED_MIN_CURRENT;
    pasCfgStatusData->sPerPortData[u8PortNum].u16aMinPDOPreferredCurInmA[4] = CFG_PORT_SINK_PDO_5_PREFERRED_MIN_CURRENT;
    pasCfgStatusData->sPerPortData[u8PortNum].u16aMinPDOPreferredCurInmA[5] = CFG_PORT_SINK_PDO_6_PREFERRED_MIN_CURRENT;
    pasCfgStatusData->sPerPortData[u8PortNum].u16aMinPDOPreferredCurInmA[6] = CFG_PORT_SINK_PDO_7_PREFERRED_MIN_CURRENT;

    /*Assigning maximum operating current and minimum operating current to 
     * 3000mA and 1000mA respectively*/
    pasCfgStatusData->sPerPortData[u8PortNum].u16SnkMaxOperatingCurInmA = CFG_PORT_SINK_MAX_OPERATING_CURRENT_InmA;
    pasCfgStatusData->sPerPortData[u8PortNum].u16SnkMinOperatingCurInmA = CFG_PORT_SINK_MIN_OPERATING_CURRENT_InmA;
        
    pasCfgStatusData->sPerPortData[u8PortNum].u8OCSThresholdPercentage = 0x00;
    pasCfgStatusData->sPerPortData[u8PortNum].u8FaultInDebounceInms = CFG_FAULT_IN_OCS_DEBOUNCE_MS;
    pasCfgStatusData->sPerPortData[u8PortNum].u8OVThresholdPercentage = CFG_OVER_VOLTAGE_FACTOR;
    pasCfgStatusData->sPerPortData[u8PortNum].u8UVThresholdPercentage = CFG_UNDER_VOLTAGE_FACTOR;
    pasCfgStatusData->sPerPortData[u8PortNum].u8VCONNOCSDebounceInms = CFG_VCONN_OCS_DEBOUNCE_IN_MS;
    pasCfgStatusData->sPerPortData[u8PortNum].u8VBUSMaxFaultCnt = CFG_MAX_VBUS_POWER_FAULT_COUNT;
    pasCfgStatusData->sPerPortData[u8PortNum].u8VCONNMaxFaultCnt = CFG_MAX_VCONN_FAULT_COUNT;
    pasCfgStatusData->sPerPortData[u8PortNum].u16PowerGoodTimerInms = MILLISECONDS_TO_TICKS(CFG_POWER_GOOD_TIMER_MS);

    pasCfgStatusData->sPerPortData[u8PortNum].u8Pio_FAULT_IN = (UINT8)CFG_PORT_UPD_FAULT_IN_PIO_NO;
    pasCfgStatusData->sPerPortData[u8PortNum].u8Mode_FAULT_IN = (UINT8)CFG_PORT_UPD_FAULT_IN_MODE;
    pasCfgStatusData->sPerPortData[u8PortNum].u8Pio_EN_SINK  = (UINT8)CFG_PORT_UPD_EN_SNK;
    pasCfgStatusData->sPerPortData[u8PortNum].u8Mode_EN_SINK  = (UINT8)CFG_PORT_UPD_EN_SNK_PIO_MODE;
    
	pasCfgStatusData->sPerPortData[u8PortNum].u8SinkConfigSel = ((CFG_PORT_SINK_MODE_A)| \
                                                                 (CFG_PORT_SINK_USB_SUSP << DPM_SINK_CONFIG_NO_USB_SUSP_POS) |\
                                                                 (CFG_PORT_SINK_GIVE_BACK_FLAG << DPM_SINK_CONFIG_GIVE_BACK_FLAG_POS));
    
    pasCfgStatusData->sPerPortData[u8PortNum].u16DAC_I_CurrentInd_MaxInA = CFG_PORT_SINK_DAC_I_CUR_INDICATION_MAX;
    pasCfgStatusData->sPerPortData[u8PortNum].u16DAC_I_MaxOutVoltInmV = CFG_PORT_SINK_DAC_I_MAX_OP_VOLTAGE;
    pasCfgStatusData->sPerPortData[u8PortNum].u16DAC_I_MinOutVoltInmV = CFG_PORT_SINK_DAC_I_MIN_OP_VOLTAGE;
    pasCfgStatusData->sPerPortData[u8PortNum].u8DAC_I_Direction = CFG_PORT_SINK_DAC_I_DIR_HIGH_AMP_MAX_VOLT;
    
    
}

=======
>>>>>>> a4609973cab2642e945869ebc909f4506513aff1

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
               
}



/* *****************************************************************************
 End of File
 */
