/*******************************************************************************
 I2C DC DC Control File

  Company:
    Microchip Technology Inc.

  File Name:
    mpq_dc_dc_control.c

  Description:
    This file contains the function definitions of I2C DC DC control interface for
    the ports
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

#include <psf_stdinc.h>
#include "mpq_dc_dc_control.h"


/***********************************************************************************/
#if (CONFIG_DCDC_CTRL == I2C_DC_DC_CONTROL_CONFIG)

#if (I2C_DCDC_TYPE == MPQ)

UINT8 gu8MPQAlertPortMsk = 0;
        
static const UINT8 u8aMPQI2CSlvAddr[4] = {MPQ_I2C_SLV_ADDR_PORT_1,MPQ_I2C_SLV_ADDR_PORT_2, MPQ_I2C_SLV_ADDR_PORT_3, MPQ_I2C_SLV_ADDR_PORT_4};
        
static UINT32 u32aPDOPower[5] ={SET_TO_ZERO, I2C_VALUE_PDO_ID_1, I2C_VALUE_PDO_ID_2, \
                                         I2C_VALUE_PDO_ID_3, I2C_VALUE_PDO_ID_4};

UINT8 MPQDCDC_Write(UINT8 u8I2CAddress,UINT8* pu8I2CCmd,UINT8 u8Length)
{
    UINT8 u8RetVal = FALSE;
            
    for(int i=0; i<3; i++)
    {
        if (TRUE == MCHP_PSF_HOOK_UPDI2C_DCDC_WRITE (u8I2CAddress, pu8I2CCmd, u8Length))
        {
            u8RetVal = TRUE;
            break;
        }
    }
            
    /* wait for the current transfer to complete */ 
    while(MCHP_PSF_HOOK_UPDI2C_DCDC_IsBusy( ));
    __NOP(); 
            
    return u8RetVal;
}
        
UINT8 MPQDCDC_Initialize(UINT8 u8PortNum)
{
    UINT8 u8length;
    UINT32 u32I2CCmd;
    UINT8 u8Return = TRUE;

    /*Assert VBUS_EN*/
    UINT8 u8EnVbusMode = gasCfgStatusData.sPerPortData[u8PortNum].u8Mode_VBUS_EN;
    UPD_GPIOUpdateOutput(u8PortNum, gasCfgStatusData.sPerPortData[u8PortNum].u8Pio_VBUS_EN, 
                                    u8EnVbusMode, (UINT8)UPD_GPIO_ASSERT);

    MCHP_PSF_HOOK_ENABLE_GLOBAL_INTERRUPT();
    /* Clear the faults */
    u32I2CCmd = MPQ_CMD_CLEAR_FAULT;
    u8length = I2C_CMD_LENGTH_2;
    u8Return= MPQDCDC_Write (u8aMPQI2CSlvAddr[u8PortNum], (UINT8*)&u32I2CCmd, u8length);           

    /* Mask the faults */
    u32I2CCmd = MPQ_CMD_MASK_FAULT;
    u8length = I2C_CMD_LENGTH_2;
    u8Return= MPQDCDC_Write (u8aMPQI2CSlvAddr[u8PortNum], (UINT8*)&u32I2CCmd, u8length);

    /* Disable Vout */
    u32I2CCmd = MPQ_CMD_DISABLE_VOUT;
    u8length = I2C_CMD_LENGTH_2;
    u8Return= MPQDCDC_Write (u8aMPQI2CSlvAddr[u8PortNum], (UINT8*)&u32I2CCmd, u8length);
 
    /* Write Vout scale loop */
    u32I2CCmd = MPQ_CMD_WRITE_VOUT_SCALE_LOOP;
    u8length = I2C_CMD_LENGTH_3;
    u8Return= MPQDCDC_Write (u8aMPQI2CSlvAddr[u8PortNum], (UINT8*)&u32I2CCmd, u8length);

    /* Write 0V */
    u32I2CCmd = MPQ_CMD_WRITE_0V;
    u8length = I2C_CMD_LENGTH_3;
    u8Return= MPQDCDC_Write (u8aMPQI2CSlvAddr[u8PortNum], (UINT8*)&u32I2CCmd, u8length);

    /* Unmask VOUT Voltage faults(UV, OV) and Current faults(OC) */
    u32I2CCmd = MPQ_CMD_UNMASK_VOUT_AND_OC;
    u8length = I2C_CMD_LENGTH_2;
    u8Return= MPQDCDC_Write (u8aMPQI2CSlvAddr[u8PortNum], (UINT8*)&u32I2CCmd, u8length);

    /* Current Threshold */
    u32I2CCmd = MPQ_CMD_CURRENT_THRESHOLD;
    u8length = I2C_CMD_LENGTH_2;
    u8Return= MPQDCDC_Write (u8aMPQI2CSlvAddr[u8PortNum], (UINT8*)&u32I2CCmd, u8length);

    return u8Return;
}

void MPQDCDC_SetPortPower(UINT8 u8PortNum, UINT8 u8PDOIndex, UINT16 u16VBUSVoltage, UINT16 u16Current)
{
    UINT8 u8length = SET_TO_ZERO;;
    UINT32 u32I2CDCDCPower;
    UINT8 u8Return = TRUE;

    if (PWRCTRL_VBUS_0V == u16VBUSVoltage)
    {
        u32I2CDCDCPower = MPQ_CMD_DISABLE_VOUT;
        u8length = I2C_CMD_LENGTH_2;

        u8Return= MPQDCDC_Write (u8aMPQI2CSlvAddr[u8PortNum], (UINT8*)&u32I2CDCDCPower, u8length);
      
    }
    else 
    {
        u8length = I2C_CMD_LENGTH_3;
        u32I2CDCDCPower = u32aPDOPower[u8PDOIndex];

        u8Return= MPQDCDC_Write (u8aMPQI2CSlvAddr[u8PortNum], (UINT8*)&u32I2CDCDCPower, u8length);

        u32I2CDCDCPower = MPQ_CMD_ENABLE_VBUS; 
        u8length = I2C_CMD_LENGTH_2;

        u8Return= MPQDCDC_Write (u8aMPQI2CSlvAddr[u8PortNum], (UINT8*)&u32I2CDCDCPower, u8length);
    }

    if (u8Return)
        __NOP();

}
 
UINT16 MPQDCDC_GetFaultStatus(UINT8 u8PortNum, UINT8 u8Cmd, UINT8 u8ReadLen)
{
    UINT16 u16FaultStatus;
    
    MCHP_PSF_HOOK_UPDI2C_DCDC_WRITE_READ(u8aMPQI2CSlvAddr[u8PortNum],&u8Cmd,I2C_CMD_LENGTH_1,(UINT8*)&u16FaultStatus,u8ReadLen); 

    return u16FaultStatus;
}

UINT8 MPQDCDC_FaultHandler()
{
    UINT16 u16FaultMask = SET_TO_ZERO;  
    UINT16 u16I2CCmd = SET_TO_ZERO; 
    UINT8 u8PortNum = gu8MPQAlertPortMsk - 1; 
    UINT8 u8RetVal = TRUE; 
    
    /* Send 'STATUS_WORD' command to know which fault has occurred */
    u16I2CCmd = MPQ_STATUS_WORD_CMD; 
    u16FaultMask = MPQDCDC_GetFaultStatus(u8PortNum, u16I2CCmd, I2C_READ_LEN_2);

    /* Note : 'STATUS_WORD' command indicates an OC or VOUT fault has occurred. 
       In order to know the exact VOUT fault, we have to send 'STATUS VOUT' command
       which indicates whether it is OV or UV fault. Here, it is avoided since 
       PSF handles all the faults in a similar manner. Thereby, we reduce 
       one I2C read/write */
    
    /* Raise a client request to DPM for handling the fault */
    if((u16FaultMask & MPQ_IOUT_OC_FAULT) || (u16FaultMask & MPQ_VOUT_FAULT))
    {        
        u8RetVal = DPM_HandleClientRequest(u8PortNum, eMCHP_PSF_DPM_HANDLE_VBUS_FAULT);
    } 
    
    /* Clear the Fault condition by sending 'CLEAR_FAULTS' command, so that 
       Alert line gets de asserted */
    u16I2CCmd = MPQ_CMD_CLEAR_FAULT;
    u8RetVal = MPQDCDC_Write (u8aMPQI2CSlvAddr[u8PortNum], (UINT8*)&u16I2CCmd, I2C_CMD_LENGTH_2);           
    
    return u8RetVal;
}
        
void MPQDCDC_HandleAlertISR(UINT8 u8PortNum)
{
    /* Update the DC-DC Alert Port Mask with the port number */
    gu8MPQAlertPortMsk = u8PortNum + 1;
/*    if (u8PortNum > SET_TO_ZERO)
    {
        gu8DCDCAlertPortMsk <<= u8PortNum; 
    } */ 
}

#endif //#if (I2C_DCDC_TYPE == MPQ)
#endif