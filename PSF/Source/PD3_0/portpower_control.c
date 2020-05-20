/*******************************************************************************
 Port Power Control File

  Company:
    Microchip Technology Inc.

  File Name:
    portpower_control.c

  Description:
    This header file contains the function definition of interface to control
    port power functionality like drive power and discharge VBUS
 *******************************************************************************/
/*******************************************************************************

Copyright ©  [2019-2020] Microchip Technology Inc. and its subsidiaries.

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

static void UPD_GPIOGenericOutputInit(UINT8 u8PortNum,UINT8 u8PIONum, UINT8 u8PioMode);

static void UPD_GPIOGenericOutputInit(UINT8 u8PortNum,UINT8 u8PIONum, UINT8 u8PioMode)
{
    if ((UINT8)eUPD_PIO_UN_DEF != u8PIONum)
    {
        /*clear bits 6:4 and 1:0 in the read value to avoid invalid combinations.*/
        u8PioMode &= (UPD_CFG_PIO_PULL_UP_ENABLE | UPD_CFG_PIO_DATAOUTPUT | UPD_CFG_PIO_BUFFER_TYPE);
        /*enable GPIO and direction as output*/
        u8PioMode |= (UPD_CFG_PIO_DIRECTION | UPD_CFG_PIO_GPIO_ENABLE);
        /*de-assert the pin*/
        u8PioMode ^= UPD_CFG_PIO_DATAOUTPUT;
        /*update the value to GPIO register.*/
        UPD_RegWriteByte(u8PortNum, UPD_CFG_PIO_REGADDR(u8PIONum), u8PioMode);
    }
}
/***********************************************************************************/

void PWRCTRL_initialization(UINT8 u8PortNum)
{
    UPD_GPIOGenericOutputInit(u8PortNum, gasCfgStatusData.sPerPortData[u8PortNum].u8Pio_VBUS_DIS, \
                                        gasCfgStatusData.sPerPortData[u8PortNum].u8mode_VBUS_DIS);
    #if (TRUE == INCLUDE_PD_SOURCE)
    UPD_GPIOGenericOutputInit(u8PortNum, gasCfgStatusData.sPerPortData[u8PortNum].u8Pio_EN_VBUS, \
                                    gasCfgStatusData.sPerPortData[u8PortNum].u8Mode_EN_VBUS);

    UPD_GPIOGenericOutputInit(u8PortNum, gasCfgStatusData.sPerPortData[u8PortNum].u8Pio_DC_DC_EN, \
                                    gasCfgStatusData.sPerPortData[u8PortNum].u8Mode_DC_DC_EN);
       
    #if (CONFIG_DCDC_CTRL == PWRCTRL_GPIO_DC_DC)
    for(UINT8 u8VSELIndex = SET_TO_ZERO; u8VSELIndex < PWRCTRL_VSEL_PIO_MAX_COUNT; u8VSELIndex++)
    {
        UPD_GPIOGenericOutputInit(u8PortNum, gasCfgStatusData.sPerPortData[u8PortNum].u8aPio_VSEL[u8VSELIndex], \
                                gasCfgStatusData.sPerPortData[u8PortNum].u8aMode_VSEL[u8VSELIndex]);
    }
    #endif /*CONFIG_DCDC_CTRL*/

    PWRCTRL_ConfigDCDCEn(u8PortNum, TRUE);

    #endif

    #if (TRUE == INCLUDE_PD_SINK)
    
    UPD_GPIOGenericOutputInit(u8PortNum, gasCfgStatusData.sPerPortData[u8PortNum].u8Pio_EN_SINK, \
                                    gasCfgStatusData.sPerPortData[u8PortNum].u8Mode_EN_SINK);
    #endif
    
    /*Hook to modify or overwrite the default Port Power control initialization */
    MCHP_PSF_HOOK_HW_PORTPWR_INIT(u8PortNum);

}
/************************************************************************************/
void PWRCTRL_SetPortPower (UINT8 u8PortNum, UINT8 u8PDOIndex, UINT16 u16VBUSVoltage, UINT16 u16Current)
{
    #if (TRUE == INCLUDE_PD_SOURCE)
    #if (CONFIG_DCDC_CTRL == PWRCTRL_GPIO_DC_DC)

    UINT8 u8EnVbusMode = gasCfgStatusData.sPerPortData[u8PortNum].u8Mode_EN_VBUS;
    UINT8 u8VSELAssert; 
    
    if (PWRCTRL_VBUS_0V == u16VBUSVoltage)
    {
        /*De-assert EN_VBUS if voltage is 0V*/
        UPD_GPIOUpdateOutput(u8PortNum, gasCfgStatusData.sPerPortData[u8PortNum].u8Pio_EN_VBUS, 
                u8EnVbusMode, (UINT8)UPD_GPIO_DE_ASSERT);
        
        /* Clear the status of EN_VBUS and VSEL 2:0 */
        gasCfgStatusData.sPerPortData[u8PortNum].u16PortIOStatus &= 
                ~(DPM_PORT_IO_EN_VBUS_STATUS | DPM_PORT_IO_VSEL0_STATUS | DPM_PORT_IO_VSEL1_STATUS | DPM_PORT_IO_VSEL2_STATUS); 
    }
    else
    {
        /*Assert EN_VBUS*/
        UPD_GPIOUpdateOutput(u8PortNum, gasCfgStatusData.sPerPortData[u8PortNum].u8Pio_EN_VBUS, 
                u8EnVbusMode, (UINT8)UPD_GPIO_ASSERT);
        
        gasCfgStatusData.sPerPortData[u8PortNum].u16PortIOStatus |= DPM_PORT_IO_EN_VBUS_STATUS;
    }
    
    for(UINT8 u8VSELIndex = SET_TO_ZERO; u8VSELIndex < PWRCTRL_VSEL_PIO_MAX_COUNT; u8VSELIndex++)
    {
        u8VSELAssert = gasCfgStatusData.sPerPortData[u8PortNum].u8aVSELTruthTable[u8PDOIndex] & BIT(u8VSELIndex); 
        
        if (BIT(u8VSELIndex) == u8VSELAssert)
        {
            UPD_GPIOUpdateOutput(u8PortNum, gasCfgStatusData.sPerPortData[u8PortNum].u8aPio_VSEL[u8VSELIndex], \
                    gasCfgStatusData.sPerPortData[u8PortNum].u8aMode_VSEL[u8VSELIndex], \
                        (UINT8)UPD_GPIO_ASSERT);

            gasCfgStatusData.sPerPortData[u8PortNum].u16PortIOStatus |= (DPM_PORT_IO_VSEL0_STATUS << u8VSELIndex);            
        }
        else
        {
            UPD_GPIOUpdateOutput(u8PortNum, gasCfgStatusData.sPerPortData[u8PortNum].u8aPio_VSEL[u8VSELIndex], \
                gasCfgStatusData.sPerPortData[u8PortNum].u8aMode_VSEL[u8VSELIndex], \
                    (UINT8)UPD_GPIO_DE_ASSERT);
            
            gasCfgStatusData.sPerPortData[u8PortNum].u16PortIOStatus &= ~(DPM_PORT_IO_VSEL0_STATUS << u8VSELIndex);            
        }       
    }   
    
    #endif /*CONFIG_DCDC_CTRL*/
    #endif
    
    /*Hook to modify or overwrite the default Port Power control VBUS drive */
    MCHP_PSF_HOOK_PORTPWR_DRIVE_VBUS (u8PortNum, u8PDOIndex, u16VBUSVoltage, u16Current);
}
/************************************************************************************/
void PWRCTRL_ConfigVBUSDischarge (UINT8 u8PortNum, UINT8 u8EnaDisVBUSDIS)
{ 
    UINT8 u8VbusDisMode = gasCfgStatusData.sPerPortData[u8PortNum].u8mode_VBUS_DIS;
    if (u8EnaDisVBUSDIS == TRUE)
    {
        UPD_GPIOUpdateOutput(u8PortNum, gasCfgStatusData.sPerPortData[u8PortNum].u8Pio_VBUS_DIS, \
                u8VbusDisMode, (UINT8)UPD_GPIO_ASSERT);
        
        gasCfgStatusData.sPerPortData[u8PortNum].u16PortIOStatus |= DPM_PORT_IO_VBUS_DIS_STATUS; 
    }
    else
    {
        UPD_GPIOUpdateOutput(u8PortNum, gasCfgStatusData.sPerPortData[u8PortNum].u8Pio_VBUS_DIS, \
                u8VbusDisMode, (UINT8)UPD_GPIO_DE_ASSERT);
        
        gasCfgStatusData.sPerPortData[u8PortNum].u16PortIOStatus &= ~(DPM_PORT_IO_VBUS_DIS_STATUS); 
    }

    /*Hook to modify or overwrite the default Port Power control VBUS discharge*/
    MCHP_PSF_HOOK_PORTPWR_ENDIS_VBUSDISCH(u8PortNum, u8EnaDisVBUSDIS);

}

void PWRCTRL_ConfigDCDCEn(UINT8 u8PortNum, UINT8 u8EnaDisDCDCEn)
{
    #if (TRUE == INCLUDE_PD_SOURCE)
    UINT8 u8DCDCEnMode = gasCfgStatusData.sPerPortData[u8PortNum].u8Mode_DC_DC_EN; 
    UINT8 u8DCDCEnPio = gasCfgStatusData.sPerPortData[u8PortNum].u8Pio_DC_DC_EN;
    
    if (TRUE == u8EnaDisDCDCEn)
    {
        UPD_GPIOUpdateOutput(u8PortNum, u8DCDCEnPio, u8DCDCEnMode, (UINT8)UPD_GPIO_ASSERT);
        
        gasCfgStatusData.sPerPortData[u8PortNum].u16PortIOStatus |= DPM_PORT_IO_EN_DC_DC_STATUS;
    }
    else
    {
        UPD_GPIOUpdateOutput(u8PortNum, u8DCDCEnPio, u8DCDCEnMode, (UINT8)UPD_GPIO_DE_ASSERT);
        
        gasCfgStatusData.sPerPortData[u8PortNum].u16PortIOStatus &= ~(DPM_PORT_IO_EN_DC_DC_STATUS);
    }
    
    #endif
    
    /* Hook to modify or overwrite the Port Control DC_DC_EN enable/disable */
    MCHP_PSF_HOOK_PORTPWR_ENDIS_DCDCEN(u8PortNum, u8EnaDisDCDCEn); 
}
/***********************************************************************************************/
void PWRCTRL_ConfigEnSink(UINT8 u8PortNum, UINT8 u8EnaDisEnSink)
{
   #if (TRUE == INCLUDE_PD_SINK)

    UINT8 u8EnSinkMode = gasCfgStatusData.sPerPortData[u8PortNum].u8Mode_EN_SINK; 
    UINT8 u8EnSinkPio = gasCfgStatusData.sPerPortData[u8PortNum].u8Pio_EN_SINK;

    if(gasDPM[u8PortNum].u16SinkOperatingCurrInmA < 
                            gasCfgStatusData.sPerPortData[u8PortNum].u16MinimumOperatingCurInmA)
    {    
        /*When EN_SINK is enabled for Type-C alone Source detection, this check 
         make sure the minimum operating current of Sink is satisfied by the 
         Rp value presented by Source. Based on Rp value of Source detected, 
         u16SinkOperatingCurrInmA variable is updated*/
        u8EnaDisEnSink = FALSE;
    }
    else
    {
        /*do nothing*/
    }
    
    if (TRUE == u8EnaDisEnSink) 
    {
       UPD_GPIOUpdateOutput(u8PortNum, u8EnSinkPio, u8EnSinkMode, (UINT8)UPD_GPIO_ASSERT);

       gasCfgStatusData.sPerPortData[u8PortNum].u16PortIOStatus |= DPM_PORT_IO_EN_SINK_STATUS;
    }
    else
    {
        UPD_GPIOUpdateOutput(u8PortNum, u8EnSinkPio, u8EnSinkMode, (UINT8)UPD_GPIO_DE_ASSERT);

        gasCfgStatusData.sPerPortData[u8PortNum].u16PortIOStatus &= ~(DPM_PORT_IO_EN_SINK_STATUS);
    }
    
    #endif
    
}
/************************************************************************************/
void PWRCTRL_ConfigSinkHW(UINT8 u8PortNum, UINT16 u16VBUSVoltage, UINT16 u16Current)
{
    #if (TRUE == INCLUDE_PD_SINK)

    if (u16Current > gasCfgStatusData.sPerPortData[u8PortNum].u16MaximumOperatingCurInmA)
    {
        u16Current = gasCfgStatusData.sPerPortData[u8PortNum].u16MaximumOperatingCurInmA;
    }
    
    if (TYPEC_VBUS_0V == u16VBUSVoltage)
    {
        /*Disable EN_SINK Voltage is VSafe0V*/
        PWRCTRL_ConfigEnSink(u8PortNum, FALSE);
    }
    else
    {
        /* EN_SINK is enabled when PSF detects a Type-C alone device or
          when PD negotiation is complete.*/
    }
    
    /* clear the 3A and 1.5A IND status*/
    gasCfgStatusData.sPerPortData[u8PortNum].u16PortIOStatus &= \
            ~(DPM_PORT_IO_30_IND_STATUS | DPM_PORT_IO_15_IND_STATUS);
    
    if (u16Current >= DPM_3000mA)
    {
        gasCfgStatusData.sPerPortData[u8PortNum].u16PortIOStatus |= DPM_PORT_IO_30_IND_STATUS;
    }
    else if (u16Current >= DPM_1500mA)
    {
        gasCfgStatusData.sPerPortData[u8PortNum].u16PortIOStatus |= DPM_PORT_IO_15_IND_STATUS;
    }
    else
    {
        //Do nothing
    }
   
    PWRCTRL_Drive_DAC_I(u8PortNum, u16Current);
    
    MCHP_PSF_HOOK_PORTPWR_CONFIG_SINK_HW(u8PortNum, u16VBUSVoltage,u16Current);
    
    #endif
}

void PWRCTRL_Drive_DAC_I (UINT8 u8PortNum, UINT16 u16VBUSCurrent)
{
#if (TRUE == INCLUDE_PD_SINK)
    UINT16 u16MaxNegoCurInmA =0, u16DacData =0;
    UINT16 u16MaxOpVoltInmV = gasCfgStatusData.sPerPortData[u8PortNum].u16DAC_I_MaxOutVoltInmV;
    UINT16 u16MinOpVoltInmV = gasCfgStatusData.sPerPortData[u8PortNum].u16DAC_I_MinOutVoltInmV;
    UINT16 u16MaxCurIndInmA = gasCfgStatusData.sPerPortData[u8PortNum].u16DAC_I_CurrentInd_MaxInA;
    
    //Approximate the current value
    if(u16VBUSCurrent > DPM_5000mA)
    {
        u16MaxNegoCurInmA = DPM_5000mA;
    }
    else if(u16VBUSCurrent > DPM_4000mA)
    {
        u16MaxNegoCurInmA = DPM_4000mA;
    }
    else if(u16VBUSCurrent > DPM_3000mA)
    {
        u16MaxNegoCurInmA = DPM_3000mA;
    }
    else if(u16VBUSCurrent > DPM_2000mA)
    {
        u16MaxNegoCurInmA = DPM_2000mA;
    }
    else if(u16VBUSCurrent > DPM_1500mA)
    {
        u16MaxNegoCurInmA = DPM_1500mA;
    }
    else if(u16VBUSCurrent > DPM_500mA)
    {
        u16MaxNegoCurInmA = DPM_500mA;
    }
    else
    {
        u16MaxNegoCurInmA = DPM_0mA;
    }
    
    if (u16MaxNegoCurInmA > u16MaxCurIndInmA)
    {
        u16MaxNegoCurInmA = u16MaxCurIndInmA;
    }
    
    if(DPM_DAC_DIR_HIGH_AMP_MAX_VOLT == gasCfgStatusData.sPerPortData[u8PortNum].u8DAC_I_Direction)
    {
        u16DacData = u16MinOpVoltInmV + ((u16MaxNegoCurInmA *\
                (u16MaxOpVoltInmV - u16MinOpVoltInmV )) / u16MaxCurIndInmA);          
    }
    else if(DPM_DAC_DIR_HIGH_AMP_MIN_VOLT == gasCfgStatusData.sPerPortData[u8PortNum].u8DAC_I_Direction)
    {
        u16DacData = u16MaxOpVoltInmV - ((u16MaxNegoCurInmA *\
                (u16MaxOpVoltInmV - u16MinOpVoltInmV )) / u16MaxCurIndInmA);

    }
    else
    {
        //Do nothing
    }
    
    //value calculated in u16DacData should reflect in DAC_I pin
    MCHP_PSF_HOOK_DRIVE_DAC_I(u16DacData);
    
#endif //#if (TRUE == INCLUDE_PD_SINK)
}
