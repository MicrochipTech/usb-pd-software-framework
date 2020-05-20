/*******************************************************************************
  MPQ I2C DC DC Control File

  Company:
    Microchip Technology Inc.

  File Name:
    Mpq_dc_dc_control.c

  Description:
    This file contains the function definitions of I2C DC DC control interface for
    the ports
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
#include <Drivers.h>

/***********************************************************************************/
#if (CONFIG_DCDC_CTRL == PWRCTRL_I2C_DC_DC)

volatile UINT8 gu8MPQAlertPortMsk[CONFIG_PD_PORT_COUNT] = {0};
        
static const UINT8 u8aMPQI2CSlvAddr[4] = {MPQ_I2C_SLV_ADDR_PORT_1,MPQ_I2C_SLV_ADDR_PORT_2, MPQ_I2C_SLV_ADDR_PORT_3, MPQ_I2C_SLV_ADDR_PORT_4};
UINT8 u16PrevCurrent[CONFIG_PD_PORT_COUNT] = {0};
UINT8 u16PrevVoltage[CONFIG_PD_PORT_COUNT] = {0};

static void MPQDCDC_SetVoltageOutput (UINT8 u8PortNum, UINT16 u16VBUSVoltage);
static void MPQDCDC_SetCurrentOutput (UINT8 u8PortNum, UINT16 u16Current);
/*****************************************************************************/
        
UINT8 MPQDCDC_Write(UINT8 u8I2CAddress,UINT8* pu8I2CCmd,UINT8 u8Length)
{
    UINT8 u8RetVal = FALSE;
            
    for(int i=0; i<3; i++)
    {
        if (TRUE == SAMD20_I2CDCDCWriteDriver (u8I2CAddress, pu8I2CCmd, u8Length))
        {
            /* wait for the current transfer to complete */ 
            while(SAMD20_I2CDCDCIsBusyDriver( ));
            __NOP(); 
            
            u8RetVal = TRUE;
            break;
        }
    }
            
    return u8RetVal;
}
        
UINT8 MPQDCDC_Initialize(UINT8 u8PortNum)
{
    UINT8 u8length;
    UINT32 u32I2CCmd;
    UINT8 u8Return = TRUE;
    UINT16 i;

    MCHP_PSF_HOOK_ENABLE_GLOBAL_INTERRUPT();

    /*Assert EN_VBUS*/
    UINT8 u8EnVbusMode = gasCfgStatusData.sPerPortData[u8PortNum].u8Mode_EN_VBUS;
    UPD_GPIOUpdateOutput(u8PortNum, gasCfgStatusData.sPerPortData[u8PortNum].u8Pio_EN_VBUS, 
                                    u8EnVbusMode, (UINT8)UPD_GPIO_ASSERT);
    
    /* Set EN_VBUS Status in Port IO status register */
    gasCfgStatusData.sPerPortData[u8PortNum].u16PortIOStatus |= MPQ_SET_PORT_IO_EN_VBUS_STATUS;
    
    /* Delay of ~2.5ms for I2C module to stabilize after DC DC Enable*/
    for(i=0;i<20000;i++)
    {
         __NOP();
    }
    /* Clear the faults */
    u32I2CCmd = MPQ_CMD_CLEAR_FAULT;
    u8length = I2C_CMD_LENGTH_1;
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

    /* Unmask Current faults(OC) */
    /* Vout fault is not unmasked as we have issue with MPQ module */
    u32I2CCmd = MPQ_CMD_UNMASK_OC;
    u8length = I2C_CMD_LENGTH_2;
    u8Return= MPQDCDC_Write (u8aMPQI2CSlvAddr[u8PortNum], (UINT8*)&u32I2CCmd, u8length);

    /* Current Threshold */
//    u32I2CCmd = MPQ_CMD_CURRENT_THRESHOLD;
//    u8length = I2C_CMD_LENGTH_2;
//    u8Return= MPQDCDC_Write (u8aMPQI2CSlvAddr[u8PortNum], (UINT8*)&u32I2CCmd, u8length);

    MCHP_PSF_HOOK_DISABLE_GLOBAL_INTERRUPT();

    return u8Return;
}

void MPQDCDC_SetPortPower(UINT8 u8PortNum, UINT8 u8PDOIndex, UINT16 u16VBUSVoltage, UINT16 u16Current)
{
    /*Assert EN_VBUS */
    UINT8 u8EnVbusMode = gasCfgStatusData.sPerPortData[u8PortNum].u8Mode_EN_VBUS;
    UPD_GPIOUpdateOutput(u8PortNum, gasCfgStatusData.sPerPortData[u8PortNum].u8Pio_EN_VBUS,
                                    u8EnVbusMode, (UINT8)UPD_GPIO_ASSERT);
    
    if (u16PrevCurrent[u8PortNum] != u16Current)
    {
        if (u16Current > u16PrevCurrent[u8PortNum])
        {
            MPQDCDC_SetCurrentOutput (u8PortNum, u16Current);
            MPQDCDC_SetVoltageOutput (u8PortNum, u16VBUSVoltage);
        }
        else
        {
            MPQDCDC_SetVoltageOutput (u8PortNum, u16VBUSVoltage);
            MPQDCDC_SetCurrentOutput (u8PortNum, u16Current);
        }
        u16PrevCurrent[u8PortNum] = u16Current;
    }
    else
    {
        MPQDCDC_SetVoltageOutput (u8PortNum, u16VBUSVoltage);
    } 
}

static void MPQDCDC_SetVoltageOutput (UINT8 u8PortNum, UINT16 u16VBUSVoltage)
{
    UINT32 u32I2CDCDCVoltage = SET_TO_ZERO;
    UINT8 u8Return = TRUE;
    UINT8 u8length = SET_TO_ZERO;

    if(u16PrevVoltage[u8PortNum] != u16VBUSVoltage)
    {
        u16PrevVoltage[u8PortNum] = u16VBUSVoltage;
        if (PWRCTRL_VBUS_0V == u16VBUSVoltage)
        {
            u32I2CDCDCVoltage = MPQ_CMD_DISABLE_VOUT;
            u8length = I2C_CMD_LENGTH_2;

            u8Return= MPQDCDC_Write (u8aMPQI2CSlvAddr[u8PortNum], (UINT8*)&u32I2CDCDCVoltage, u8length);

        }
        else 
        {
            u8length = I2C_CMD_LENGTH_3;
            u32I2CDCDCVoltage = (UINT32)u16VBUSVoltage;
            u32I2CDCDCVoltage = (UINT32) ((u32I2CDCDCVoltage * MPQ_REGISTER_COUNTS_1V)/MPQ_1V_IN_MILLIVOLT);
            u32I2CDCDCVoltage = u32I2CDCDCVoltage << 8;
            u32I2CDCDCVoltage = u32I2CDCDCVoltage | MPQ_CMD_WRITE_VOLTAGE;

            u8Return= MPQDCDC_Write (u8aMPQI2CSlvAddr[u8PortNum], (UINT8*)&u32I2CDCDCVoltage, u8length);

            u32I2CDCDCVoltage = MPQ_CMD_ENABLE_VBUS; 
            u8length = I2C_CMD_LENGTH_2;

            u8Return= MPQDCDC_Write (u8aMPQI2CSlvAddr[u8PortNum], (UINT8*)&u32I2CDCDCVoltage, u8length);
        }

        if (u8Return)
            __NOP();
    }
}


static void MPQDCDC_SetCurrentOutput (UINT8 u8PortNum, UINT16 u16Current)
{
    UINT32 u32I2CDCDCCurrent = SET_TO_ZERO;
    UINT8 u8Return = TRUE;
    UINT8 u8length = SET_TO_ZERO;
    
    u16Current = (UINT16) (u16Current + ((float) u16Current * ((float)MPQ_CURRENT_OFFSET_VALUE/ (float) 100)));
    u16Current = (UINT16) (u16Current/(UINT16) MPQ_CURRENT_COUNT_FACTOR);
    u32I2CDCDCCurrent = (u16Current << 8) | MPQ_CMD_WRITE_CURRENT;
    u8length = I2C_CMD_LENGTH_2;
    u8Return= MPQDCDC_Write (u8aMPQI2CSlvAddr[u8PortNum], (UINT8*)&u32I2CDCDCCurrent, u8length);

    if (u8Return)
        __NOP();
}
 
UINT16 MPQDCDC_GetFaultStatus(UINT8 u8PortNum, UINT8 u8Cmd, UINT8 u8ReadLen)
{
    UINT16 u16FaultStatus;
    
    (void)SAMD20_I2CDCDCWriteReadDriver (u8aMPQI2CSlvAddr[u8PortNum],&u8Cmd,I2C_CMD_LENGTH_1,(UINT8*)&u16FaultStatus,u8ReadLen); 
    /* wait for the current transfer to complete */ 
    while(SAMD20_I2CDCDCIsBusyDriver( ));

    return u16FaultStatus;
}

UINT8 MPQDCDC_FaultHandler(UINT8 u8PortNum)
{
    UINT16 u16FaultMask = SET_TO_ZERO;  
    UINT16 u16I2CCmd = SET_TO_ZERO; 
    UINT8 u8RetVal = TRUE; 
    
    /* Send 'STATUS_WORD' command to know which fault has occurred */
    u16I2CCmd = MPQ_STATUS_WORD_CMD; 
    u16FaultMask = MPQDCDC_GetFaultStatus(u8PortNum, u16I2CCmd, I2C_READ_LEN_2);

    /* Note : 'STATUS_WORD' command indicates an OC or VOUT fault has occurred. 
       In order to know the exact VOUT fault, we have to send 'STATUS VOUT' command
       which indicates whether it is OV or UV fault. Here, it is avoided since 
       PSF handles all the faults in a similar manner. Thereby, we reduce 
       one I2C read/write */
    if (FALSE != u16FaultMask)
    {
        /* Inform PSF to handle the VBUS Fault */
        if(FALSE != (u16FaultMask & MPQ_IOUT_OC_FAULT))
        { 
            /*Todo:Jega handle it new client request defines*/
            gasCfgStatusData.sPerPortData[u8PortNum].u32ClientRequest |= DPM_CLIENT_REQ_HANDLE_FAULT_VBUS_OCS;
        } 

        if(FALSE != (u16FaultMask & MPQ_IOUT_OC_EXIT_FAULT))
        { 
            /*Todo:Jega handle it new client request defines*/
            gasCfgStatusData.sPerPortData[u8PortNum].u32ClientRequest |= DPM_CLIENT_REQ_HANDLE_VBUS_OCS_EXIT;
        } 

        if(FALSE != (u16FaultMask & MPQ_VOUT_OV_FAULT))
        { 
            /*Todo:Jega handle it new client request defines*/
            gasCfgStatusData.sPerPortData[u8PortNum].u32ClientRequest |= DPM_CLIENT_REQ_HANDLE_FAULT_VBUS_OV;
        } 

        /* Clear the Fault condition by sending 'CLEAR_FAULTS' command, so that 
           Alert line gets de asserted */
        u16I2CCmd = MPQ_CMD_CLEAR_FAULT;
        u8RetVal = MPQDCDC_Write (u8aMPQI2CSlvAddr[u8PortNum], (UINT8*)&u16I2CCmd, I2C_CMD_LENGTH_1);           
        /* wait for the current transfer to complete */ 
        while(SAMD20_I2CDCDCIsBusyDriver( ));
    }
    
    return u8RetVal;
}         
		
void MPQDCDC_HandleAlertISR(UINT8 u8PortNum)
{
    /* Update the DC-DC Alert Port Mask with the port number */
    gu8MPQAlertPortMsk[u8PortNum] = 1;
}

UINT16 MPQDCDC_ReadVoltage(UINT8 u8PortNum)
{
    volatile UINT16 u16VoltageOutput;
    UINT32 u32VotageAvg = 0;
    UINT8 u8Cmd,i;
    
    u8Cmd = 0x8B;
    
    for(i=0; i<5;i++)
    { 
        (void)SAMD20_I2CDCDCWriteReadDriver (u8aMPQI2CSlvAddr[u8PortNum],&u8Cmd,1U,(UINT8*)&u16VoltageOutput,2U);
        
        /* wait for the current transfer to complete */ 
        while(SAMD20_I2CDCDCIsBusyDriver( ));
        
        u32VotageAvg += u16VoltageOutput;
    }
    u16VoltageOutput = (UINT16) (u32VotageAvg/((UINT16)5));
    return u16VoltageOutput;
}


#endif