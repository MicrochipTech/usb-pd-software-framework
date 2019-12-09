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
Copyright �  [2019] Microchip Technology Inc. and its subsidiaries.

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

/*VSEL PIO count VSEL[2:0]*/
#define PWRCTRL_VSEL_PIO_MAX_COUNT      3

/* Values for CONFIG_DCDC_CTRL*/
#define PWRCTRL_DEFAULT_PSF_GPIO_CONFIG     1


/* ************************************************************************** */
/* ************************************************************************** */
/* Section: Data Structure                                                    */
/* ************************************************************************** */
/* ************************************************************************** */
/* User Config Structure: This structure contains the user configure variables and PDOs*/

typedef struct _Upd_Pio_DcDc_Config
{   
	UINT8   u8VBUSEnPio;        //PIO for UPD350  VBus Enable pin
    UINT8   u8VBUSEnPioMode;    // VBUS enable PIO mode
	UINT8   u8FaultInPio;       //PIO for UPD350 Port Control selection pin
    UINT8   u8FaultInMode;      //Fault In PIO mode, active high or active low etc.
    UINT8   u8VBUSDisPio;       // PIO to discharge VBUS
    UINT8   u8VBUSDisPioMode;   // VBUS discharger PIO mode, active high or active low etc.
    UINT8   u8DcDcEnPio;        // DC_DC_EN PIO for controlling the DC_DC enable
    UINT8   u8DcDcEnPioMode;    // DC_DC_EN PIO mode
    UINT8   u8VSELPio[3];       // VSEL[2:0] PIO for controlling GPIO based DC_DC
    UINT8   u8VSELPioMode[3];   // VSEL[2:0] PIO mode
    UINT8   u8VSELmapforPDO[8]; // VSEL map value corresponding to each PDO
}UPD_PIO_CONFIG_DATA;

// *****************************************************************************
// *****************************************************************************
// Section: Interface Functions
// *****************************************************************************
// *****************************************************************************
/****************************************************************************
    Function:
        void PWRCTRL_initialization(UINT8 u8PortNum)
    Summary:
        Initializes Port power control
    Description:
        This API is called for enabled port during PSF_Init to initialize Port power control. 
        Defining CONFIG_DCDC_CTRL as PWRCTRL_DEFAULT_PSF_GPIO_CONFIG uses PSF's default
        GPIO based control for DC-DC controller. API also provides MCHP_PSF_HOOK_HW_PORTPWR_INIT to 
        modify or overwrite the default setting.
    Conditions:
        None.
    Input:
        u8PortNum - Corresponding Port Number. Value passed will be less than CONFIG_PD_PORT_COUNT.
    Return:
      None
    Remarks:
        None
**************************************************************************************************/
void PWRCTRL_initialization(UINT8 u8PortNum);

/****************************************************************************
    Function:
        void PWRCTRL_SetPortPower (UINT8 u8PortNum,UINT8 u8PDOIndex, UINT16 u16VBUSVoltage, UINT16 u16Current)
    Summary:
        API to drive Power on VBUS
    Description:
        This API drives Power based on the input parameter on VBUS line. Defining 
        CONFIG_DCDC_CTRL as PWRCTRL_DEFAULT_PSF_GPIO_CONFIG uses PSF's default
        GPIO based control for DC-DC controller. This API also provides hook MCHP_PSF_HOOK_PORTPWR_DRIVE_VBUS 
        to modify or overwrite the default setting.
    Conditions:
        None.
    Input:
        u8PortNum - Corresponding Port Number. Value passed will be less than CONFIG_PD_PORT_COUNT.
        u8PDOIndex - Provides current PDO index negotiated to drive power.
                        For TypeC attach, PDO index is passed as '1'. As it is mandatory to have
                        PDO 1 as 5V PDO. For Vsafe0V, PDOIndex is passed as 'Zero'.
        u16VBUSVoltage - Provides negotiated voltage to drive power
        u16Current - Provides negotiated current to drive power
    Return:
      None
    Remarks:
        None
**************************************************************************************************/
void PWRCTRL_SetPortPower (UINT8 u8PortNum,UINT8 u8PDOIndex, UINT16 u16VBUSVoltage, UINT16 u16Current);

/****************************************************************************
    Function:
        void PWRCTRL_ConfigVBUSDischarge (UINT8 u8PortNum, UINT8 u8EnaDisVBUSDIS)
    Summary:
        API to configure VBUS Discharge feature as required
    Description:
        This API enables or disables VBUS discharge feature based on u8EnaDisVBUSDIS parameter for the
        port. Defining CONFIG_DCDC_CTRL as PWRCTRL_DEFAULT_PSF_GPIO_CONFIG uses PSF's default
        GPIO based control for DC-DC controller. API as provides MCHP_PSF_HOOK_PORTPWR_ENDIS_VBUSDISCH to 
        modify or overwrite the default setting.
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

#endif /*_PORTPOWER_CONTROL_H_*/
