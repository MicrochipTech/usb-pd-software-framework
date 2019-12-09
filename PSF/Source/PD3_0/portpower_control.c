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
    #if (CONFIG_DCDC_CTRL == PWRCTRL_DEFAULT_PSF_GPIO_CONFIG)
    UPD_GPIOGenericOutputInit(u8PortNum, gasUpdPioDcDcConfig[u8PortNum].u8VBUSEnPio, \
                                    gasUpdPioDcDcConfig[u8PortNum].u8VBUSEnPioMode);

    UPD_GPIOGenericOutputInit(u8PortNum, gasUpdPioDcDcConfig[u8PortNum].u8VBUSDisPio, \
                                        gasUpdPioDcDcConfig[u8PortNum].u8VBUSDisPioMode);

    UPD_GPIOGenericOutputInit(u8PortNum, gasUpdPioDcDcConfig[u8PortNum].u8DcDcEnPio, \
                                    gasUpdPioDcDcConfig[u8PortNum].u8DcDcEnPioMode);
    
    for(UINT8 u8VSELIndex = SET_TO_ZERO; u8VSELIndex < PWRCTRL_VSEL_PIO_MAX_COUNT; u8VSELIndex++)
    {
        UPD_GPIOGenericOutputInit(u8PortNum, gasUpdPioDcDcConfig[u8PortNum].u8VSELPio[u8VSELIndex], \
                                gasUpdPioDcDcConfig[u8PortNum].u8VSELPioMode[u8VSELIndex]);
    }

    UPD_GPIOUpdateOutput(u8PortNum, gasUpdPioDcDcConfig[u8PortNum].u8DcDcEnPio, \
                gasUpdPioDcDcConfig[u8PortNum].u8DcDcEnPioMode, (UINT8)UPD_GPIO_ASSERT);
    #endif /*CONFIG_DCDC_CTRL*/

    /*Hook to modify or overwrite the default Port Power control initialization */
    MCHP_PSF_HOOK_HW_PORTPWR_INIT(u8PortNum);

}
/************************************************************************************/
void PWRCTRL_SetPortPower (UINT8 u8PortNum, UINT8 u8PDOIndex, UINT16 u16VBUSVoltage, UINT16 u16Current)
{
    #if (CONFIG_DCDC_CTRL == PWRCTRL_DEFAULT_PSF_GPIO_CONFIG)

    UINT8 u8EnVbusMode = gasUpdPioDcDcConfig[u8PortNum].u8VBUSEnPioMode;
    if (PWRCTRL_VBUS_0V == u16VBUSVoltage)
    {
        /*De-assert VBUS_EN if voltage is 0V*/
        UPD_GPIOUpdateOutput(u8PortNum, gasUpdPioDcDcConfig[u8PortNum].u8VBUSEnPio, 
                u8EnVbusMode, (UINT8)UPD_GPIO_DE_ASSERT);
    }
    else
    {
        /*Assert VBUS_EN*/
        UPD_GPIOUpdateOutput(u8PortNum, gasUpdPioDcDcConfig[u8PortNum].u8VBUSEnPio, 
                u8EnVbusMode, (UINT8)UPD_GPIO_ASSERT);
    }
    for(UINT8 u8VSELIndex = SET_TO_ZERO; u8VSELIndex < PWRCTRL_VSEL_PIO_MAX_COUNT; u8VSELIndex++)
    {
        UPD_GPIOUpdateOutput(u8PortNum, gasUpdPioDcDcConfig[u8PortNum].u8VSELPio[u8VSELIndex], \
            gasUpdPioDcDcConfig[u8PortNum].u8VSELPioMode[u8VSELIndex], \
            (((gasUpdPioDcDcConfig[u8PortNum].u8VSELmapforPDO[u8PDOIndex] & BIT(u8VSELIndex)) == BIT(u8VSELIndex)) \
            ? (UINT8)UPD_GPIO_ASSERT : (UINT8)UPD_GPIO_DE_ASSERT));
    }
    #endif /*CONFIG_DCDC_CTRL*/

    /*Hook to modify or overwrite the default Port Power control VBUS drive */
    MCHP_PSF_HOOK_PORTPWR_DRIVE_VBUS (u8PortNum, u16VBUSVoltage, u16Current);
}
/************************************************************************************/
void PWRCTRL_ConfigVBUSDischarge (UINT8 u8PortNum, UINT8 u8EnaDisVBUSDIS)
{
    #if (CONFIG_DCDC_CTRL == PWRCTRL_DEFAULT_PSF_GPIO_CONFIG) 
    UINT8 u8VbusDisMode = gasUpdPioDcDcConfig[u8PortNum].u8VBUSDisPioMode;
    if (u8EnaDisVBUSDIS == TRUE)
    {
        UPD_GPIOUpdateOutput(u8PortNum, gasUpdPioDcDcConfig[u8PortNum].u8VBUSDisPio, \
                u8VbusDisMode, (UINT8)UPD_GPIO_ASSERT);
    }
    else
    {
        UPD_GPIOUpdateOutput(u8PortNum, gasUpdPioDcDcConfig[u8PortNum].u8VBUSDisPio, \
                u8VbusDisMode, (UINT8)UPD_GPIO_DE_ASSERT);
    }
    #endif /*CONFIG_DCDC_CTRL*/

    /*Hook to modify or overwrite the default Port Power control VBUS discharge*/
    MCHP_PSF_HOOK_PORTPWR_ENDIS_VBUSDISCH(u8PortNum, u8EnaDisVBUSDIS);

}

/************************************************************************************/
