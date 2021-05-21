/*******************************************************************************
  Port Power Control File
 
  Company:
    Microchip Technology Inc.

  File Name:
    portpower_control.h

  Description:
    This header file contains the function prototypes for port power control to
	drive power and discharge VBUS
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

#ifndef _PORTPOWER_CONTROL_H_
#define _PORTPOWER_CONTROL_H_


/* ************************************************************************** */
/* ************************************************************************** */
/* Section: File Scope or Global Data, constants                              */
/* ************************************************************************** */
/* ************************************************************************** */

/*Initialization function of Hardware Module used for Port Power Control*/
#define PWRCTRL_VBUS_0V		0

/* ************************************************************************** */
/* ************************************************************************** */
/* Section: Data Structure                                                    */
/* ************************************************************************** */
/* ************************************************************************** */
// *****************************************************************************
// *****************************************************************************
// Section: Interface Functions
// *****************************************************************************
// *****************************************************************************
/****************************************************************************
    Function:
        UINT8 PWRCTRL_Init (UINT8 u8PortNum)
    Summary:
        Initializes Port power control
    Description:
        This API is called for enabled port during PSF_Init to initialize Port power control. 
        API also provides MCHP_PSF_HOOK_HW_PORTPWR_INIT to modify or overwrite the default setting.
    Conditions:
        None.
    Input:
        u8PortNum - Corresponding Port Number. Value passed will be less than CONFIG_PD_PORT_COUNT.
    Return:
      None
    Remarks:
        None
**************************************************************************************************/
UINT8 PWRCTRL_Init (UINT8 u8PortNum);

/****************************************************************************
    Function:
        void PWRCTRL_SetPortPower (UINT8 u8PortNum, UINT16 u16VBUSVoltage, UINT16 u16Current)
    Summary:
        API to drive Power on VBUS
    Description:
        This API drives Power based on the input parameter on VBUS line. This API also 
        provides hook MCHP_PSF_HOOK_PORTPWR_DRIVE_VBUS to modify or overwrite the default setting.
    Conditions:
        None.
    Input:
        u8PortNum - Corresponding Port Number. Value passed will be less than CONFIG_PD_PORT_COUNT.
        u16VBUSVoltage - Provides negotiated voltage to drive power
        u16Current - Provides negotiated current to drive power
    Return:
      None
    Remarks:
        None
**************************************************************************************************/
void PWRCTRL_SetPortPower (UINT8 u8PortNum, UINT16 u16VBUSVoltage, UINT16 u16Current);

/****************************************************************************
    Function:
        void PWRCTRL_ConfigVBUSDischarge (UINT8 u8PortNum, UINT8 u8EnaDisVBUSDIS)
    Summary:
        API to configure VBUS Discharge feature as required
    Description:
        This API enables or disables VBUS discharge feature based on u8EnaDisVBUSDIS parameter for the
        port. API as provides MCHP_PSF_HOOK_PORTPWR_ENDIS_VBUSDISCH to modify or overwrite the 
        default setting.
    Conditions:
        None.
    Input:
        u8PortNum - Corresponding Port Number. Value passed will be less than CONFIG_PD_PORT_COUNT.
        u8EnaDisVBUSDIS - u8EnaDisVBUSDIS takes following value 
                           'TRUE' Enables VBUS Discharge
                           'FALSE' Disables VBUS Discharge
    Return:
      None
    Remarks:
        None

**************************************************************************************************/
void PWRCTRL_ConfigVBUSDischarge (UINT8 u8PortNum, UINT8 u8EnaDisVBUSDIS);

/****************************************************************************
    Function:
        void PWRCTRL_ConfigDCDCEn (UINT8 u8PortNum, UINT8 u8EnaDisDCDCEn)
    Summary:
        API to configure DC_DC_EN as required
    Description:
        This API enables or disables DC_DC_EN based on u8EnaDisDCDCEn 
        parameter for the port. Also, updates the enable/disable status in 
        Port I/O Status register. 
    Conditions:
        None.
    Input:
        u8PortNum - Corresponding Port Number. Value passed will be less than CONFIG_PD_PORT_COUNT.
        u8EnaDisDCDCEn - u8EnaDisDCDCEn takes following value 
                           'TRUE' Enables DC_DC_EN 
                           'FALSE' Disables DC_DC_EN
    Return:
      None.
    Remarks:
      None.

**************************************************************************************************/
void PWRCTRL_ConfigDCDCEn (UINT8 u8PortNum, UINT8 u8EnaDisDCDCEn);

/****************************************************************************
    Function:
        void PWRCTRL_ConfigEnSink (UINT8 u8PortNum, UINT8 u8EnaDisEnSink)
    Summary:
        API to configure EN_SINK as required
    Description:
        This API enables or disables EN_SINK based on u8EnaDisEnSink 
        parameter for the port. Also, updates the enable/disable status in 
        Port I/O Status register. 
    Conditions:
        None.
    Input:
        u8PortNum - Corresponding Port Number. Value passed will be less than CONFIG_PD_PORT_COUNT.
        u8EnaDisEnSink - u8EnaDisEnSink takes following value 
                           'TRUE' Enables EN_SINK 
                           'FALSE' Disables EN_SINK
    Return:
      None.
    Remarks:
      None.

**************************************************************************************************/
void PWRCTRL_ConfigEnSink (UINT8 u8PortNum, UINT8 u8EnaDisEnSink);

/****************************************************************************
    Function:
        void PWRCTRL_ConfigSinkHW (UINT8 u8PortNum, UINT16 u16VBUSVoltage, UINT16 u16Current)
    Summary:
        API to configure Sink Hardware
    Description:
        This API is to configure the Sink hardware as per the passed voltage and
        current limit passed.
    Conditions:
        None.
    Input:
        u8PortNum - Corresponding Port Number. Value passed will be less than CONFIG_PD_PORT_COUNT.
        u16VBUSVoltage - Voltage limit the hardware has to be configured
        u16Current - Current limit the Sink hardware has to be configured
    Return:
      None.
    Remarks:
      None.

**************************************************************************************************/
void PWRCTRL_ConfigSinkHW (UINT8 u8PortNum, UINT16 u16VBUSVoltage, UINT16 u16Current);

/****************************************************************************
    Function:
        void PWRCTRL_DriveDAC_I (UINT8 u8PortNum, UINT16 u16VBUSCurrent)
    Summary:
        API to indicate Sink Hardware about the negotiated current
    Description:
        This API is to indicate the implicit or negotiated current capability of 
        the attached Source port partner through a DAC output.
    Conditions:
        None.
    Input:
        u8PortNum - Corresponding Port Number. Value passed will be less than CONFIG_PD_PORT_COUNT.
        u16VBUSCurrent - Maximum Current provided by the source port partner at VBUS 
    Return:
      None.
    Remarks:
      None.

**************************************************************************************************/
void PWRCTRL_DriveDAC_I (UINT8 u8PortNum, UINT16 u16VBUSCurrent);

/****************************************************************************
    Function:
        void PWRCTRL_DisableEnFRS (UINT8 u8PortNum)
    Summary:
        API to disable EN_FRS pin as required
    Description:
        This API disables EN_FRS pin of the FRS capable load switch. It 
        will be enabled by the PIO Override feature of UPD350.
        Also, updates the disable status in Port I/O Status register. 
    Conditions:
        None.
    Input:
        u8PortNum - Corresponding Port Number. Value passed will be less than CONFIG_PD_PORT_COUNT.
    Return:
      None.
    Remarks:
      None.

**************************************************************************************************/
void PWRCTRL_DisableEnFRS (UINT8 u8PortNum);

#endif /*_PORTPOWER_CONTROL_H_*/
