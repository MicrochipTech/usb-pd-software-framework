/*******************************************************************************
  MPQ I2C DC DC Control File
 
  Company:
    Microchip Technology Inc.

  File Name:
    Mpq_dc_dc_control.h

  Description:
    This header file contains the function prototypes for port power control to
	drive power and discharge VBUS
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

#ifndef _MPQ_DCDC_CONTROL_H_
#define _MPQ_DCDC_CONTROL_H_


#if (CONFIG_DCDC_CTRL == I2C_DC_DC_CONTROL_CONFIG)

// *****************************************************************************
#if (CONFIG_I2C_DCDC_TYPE == MPQ)

/* ************************************************************************** */
/* ************************************************************************** */
/* Section: File Scope or Global Data, constants                              */
/* ************************************************************************** */
/* ************************************************************************** */

/* Lower byte corresponds to register address and higher byte corresponds 
   to command type */
#define MPQ_CMD_CLEAR_FAULT             0x0003U
#define MPQ_CMD_MASK_FAULT              0xFFD8U
#define MPQ_CMD_DISABLE_VOUT            0x0001U
#define MPQ_CMD_WRITE_VOUT_SCALE_LOOP   0xB04229U
#define MPQ_CMD_WRITE_0V                0x000021U
#define MPQ_CMD_UNMASK_VOUT_AND_OC      0x3FD8U
#define MPQ_CMD_CURRENT_THRESHOLD       0x3FD1U //0x1ED1U
#define MPQ_CMD_ENABLE_VBUS             0x8001U

#define I2C_CMD_LENGTH_1                1
#define I2C_CMD_LENGTH_2                2
#define I2C_CMD_LENGTH_3                3               

#define I2C_READ_LEN_2                  2

#define I2C_VALUE_PDO_ID_1              0x140021U
#define I2C_VALUE_PDO_ID_2              0x240021U
#define I2C_VALUE_PDO_ID_3              0x3C0021U
#define I2C_VALUE_PDO_ID_4              0x500021U
        
#define MPQ_I2C_SLV_ADDR_PORT_1         0x61U 
#define MPQ_I2C_SLV_ADDR_PORT_2         0x62U 
#define MPQ_I2C_SLV_ADDR_PORT_3         0x63U 
#define MPQ_I2C_SLV_ADDR_PORT_4         0x64U 

/* Status commands */
#define MPQ_STATUS_BYTE_CMD             0x78U
#define MPQ_STATUS_WORD_CMD             0x79U
#define MPQ_STATUS_VOUT_CMD             0x7AU
#define MPQ_STATUS_INPUT_CMD            0x7CU
#define MPQ_STATUS_TEMPERATURE_CMD      0x7DU
#define MPQ_STATUS_CML_CMD              0x7EU

/* Status bit masks */
/* Fault Status returned by 'Status Word' command */
#define MPQ_IOUT_OC_FAULT               0x0010U
#define MPQ_VOUT_FAULT                  0x8000U 
/* ************************************************************************** */
/* ************************************************************************** */
/* Section: Data Structure                                                    */
/* ************************************************************************** */
/* ************************************************************************** */
extern UINT8 gu8MPQAlertPortMsk;
// *****************************************************************************
// *****************************************************************************
// Section: Interface Functions
// *****************************************************************************
// *****************************************************************************
/****************************************************************************
    Function:
        void MPQDCDC_Initialize(UINT8 u8PortNum)
    Summary:
        Initializes I2C DC DC Control for the port
    Description:
        This API is called for initializing the I2C DC DC Module for the given port 
        during MchpPSF_Init to initialize Port power control. 
    Conditions:
        None.
    Input:
        u8PortNum - Corresponding Port Number. 
                    Value passed will be less than CONFIG_PD_PORT_COUNT.
    Return:
      None
    Remarks:
        None
**************************************************************************************************/
UINT8 MPQDCDC_Initialize(UINT8 u8PortNum);

/****************************************************************************
    Function:
        void MPQDCDC_Write(UINT8 u8I2CAddress,UINT8* pu8I2CCmd,UINT8 u8Length)
    Summary:
        Writes the data through I2C master interface 
    Description:
        This API is called for writing the data to the DC DC controller 
        attached in the respective port through I2C master interface of SAMD20. 
    Conditions:
        None.
    Input:
        u8I2CAddress - I2C Slave Address of the DC-DC Controller 
        pu8I2CCmd - Data that needs to be written
        u8Length - Length of data to be written
    Return:
      None
    Remarks:
        None
**************************************************************************************************/
UINT8 MPQDCDC_Write(UINT8 u8I2CAddress,UINT8* pu8I2CCmd,UINT8 u8Length); 

/****************************************************************************
    Function:
        void MPQDCDC_SetPortPower(UINT8 u8PortNum, UINT8 u8PDOIndex, UINT16 u16VBUSVoltage, UINT16 u16Current)
    Summary:
        API to drive Power on VBUS
    Description:
        This API drives Power on VBUS line based on the input parameters.
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
void MPQDCDC_SetPortPower(UINT8 u8PortNum, UINT8 u8PDOIndex, UINT16 u16VBUSVoltage, UINT16 u16Current);

/****************************************************************************
    Function:
        void MPQDCDC_HandleAlertISR(UINT8 u8PortNum); 
    Summary:
        Interrupt Handler for I2C DC DC Alert  
    Description:
        This API is the ISR for handling Alert signal from DC DC controller. 
        Any fault condition say, VBUS OCS will trigger an alert in the 
        corresponding port and needs to be handled properly.
    Conditions:
        None.
    Input:
        u8PortNum - Port Number in which Alert has triggered
    Return:
      None
    Remarks:
        None
**************************************************************************************************/

void MPQDCDC_HandleAlertISR(UINT8 u8PortNum);

/****************************************************************************
    Function:
        void MPQDCDC_FaultHandler(void); 
    Summary:
        Wrapper function for DPM's HandleClientRequest API.  
    Description:
        This API gets the Fault Status code from DC-DC Controller and based 
        on the fault code, requests DPM to Handle the Fault. 
    Conditions:
        None.
    Input:
        None.
    Return:
      None
    Remarks:
        None
**************************************************************************************************/

UINT8 MPQDCDC_FaultHandler(void);

#endif //#if (CONFIG_I2C_DCDC_TYPE == MPQ)

#endif
#endif /*_MPQ_DCDC_CONTROL_H_*/