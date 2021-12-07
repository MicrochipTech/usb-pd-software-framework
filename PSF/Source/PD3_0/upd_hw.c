/*******************************************************************************
 UPD350 hardware source file 

  Company:
    Microchip Technology Inc.

  File Name:
    upd_hw.c

  Description:
    This file has UPD350 Hardware related GPIO, UPD350 Idle, PIO override related 
    API definitions.
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

/*******************************************************************************************/
/**************************UPD Register Read/Write APIs*************************************/
/*******************************************************************************************/

void UPD_RegWriteByte (UINT8 u8PortNum, UINT16 u16RegOffset, UINT8 u8WriteValue)
{
	UPD_RegisterWrite (u8PortNum, u16RegOffset, &u8WriteValue, BYTE_LEN_1);
}

void UPD_RegWriteWord (UINT8 u8PortNum, UINT16 u16RegOffset, UINT16 u16WriteValue)
{
    UPD_RegisterWrite (u8PortNum, u16RegOffset, (UINT8 *)&u16WriteValue, BYTE_LEN_2);
}

/******************************************************************************************************/
UINT8 UPD_RegReadByte (UINT8 u8PortNum, UINT16 u16RegOffset)
{
	UINT8 u8ReadData;
	UPD_RegisterRead (u8PortNum, u16RegOffset, &u8ReadData, BYTE_LEN_1);
	return u8ReadData;
}

UINT16 UPD_RegReadWord (UINT8 u8PortNum, UINT16 u16RegOffset)
{
	UINT16 u16ReadData;
	UPD_RegisterRead (u8PortNum, u16RegOffset, (UINT8 *)&u16ReadData, BYTE_LEN_2);
	return u16ReadData;
}

/******************************************************************************************************/

void UPD_RegByteSetBit (UINT8 u8PortNum, UINT16 u16RegOffset, UINT8 u8BitMsk)
{
	UINT8 u8Data = SET_TO_ZERO;
	UPD_RegisterRead (u8PortNum, u16RegOffset, &u8Data, BYTE_LEN_1);
	u8Data |= u8BitMsk; 
	UPD_RegisterWrite (u8PortNum, u16RegOffset, &u8Data, BYTE_LEN_1);
}

void UPD_RegByteClearBit (UINT8 u8PortNum, UINT16 u16RegOffset, UINT8 u8BitMsk)
{
    UINT8 u8Data = SET_TO_ZERO;
    UPD_RegisterRead (u8PortNum, u16RegOffset, &u8Data, BYTE_LEN_1);
    u8Data &= ~u8BitMsk;
    UPD_RegisterWrite (u8PortNum, u16RegOffset, &u8Data, BYTE_LEN_1);
}

/******************************************************************************************************/
void UPD_RegisterWrite (UINT8 u8PortNum, UINT16 u16RegOffset, \
        UINT8 *pu8WriteData, UINT8 u8WriteDataLen)
{
    MCHP_PSF_HOOK_DISABLE_GLOBAL_INTERRUPT();
    
    UPD_RegisterWriteISR (u8PortNum, u16RegOffset, pu8WriteData, u8WriteDataLen);
    
    MCHP_PSF_HOOK_ENABLE_GLOBAL_INTERRUPT();
}
/******************************************************************************************************/
void UPD_RegisterRead (UINT8 u8PortNum, UINT16 u16RegOffset, UINT8 *pu8ReadData, \
        UINT8 u8Readlen)
{
	MCHP_PSF_HOOK_DISABLE_GLOBAL_INTERRUPT();
    
    UPD_RegisterReadISR (u8PortNum, u16RegOffset, pu8ReadData, u8Readlen);
    
    MCHP_PSF_HOOK_ENABLE_GLOBAL_INTERRUPT();
}

/*******************************************************************************************/
/********************UPD Register Read/Write ISR APIs***************************************/
/*******************************************************************************************/

void UPD_RegisterWriteISR (UINT8 u8PortNum, UINT16 u16RegOffset, 
        UINT8 *pu8WriteData, UINT8 u8WriteDataLen)
{
    #if (CONFIG_DEFINE_UPD350_HW_INTF_SEL == CONFIG_UPD350_SPI)
    
    UINT8 u8WriteBuf [UPD_SPI_MAX_BYTE_WRITE];
    UINT8 u8WriteBufLen = UPD_SPI_WRITE_CMD_LEN;

    u8WriteBuf[INDEX_0] = UPD_SPI_WRITE_OPCODE;
    u8WriteBuf[INDEX_1] = HIBYTE(u16RegOffset); /* HiByte of Register to be written*/ 
    u8WriteBuf[INDEX_2] = LOBYTE(u16RegOffset); /* LoByte of Register to be written */

    for (; u8WriteBufLen < (u8WriteDataLen + UPD_SPI_WRITE_CMD_LEN); u8WriteBufLen++)
    {        
        u8WriteBuf[u8WriteBufLen] = pu8WriteData[u8WriteBufLen - UPD_SPI_WRITE_CMD_LEN]; 
    }
    
    /* Enable SPI Select for communication */
    MCHP_PSF_HOOK_GPIO_FUNC_DRIVE(u8PortNum, eSPI_CHIP_SELECT_FUNC, eGPIO_ASSERT);
    
    (void)MCHP_PSF_HOOK_UPD_WRITE (u8PortNum, u8WriteBuf, u8WriteBufLen);

    /*Disable SPI Select communication */
    MCHP_PSF_HOOK_GPIO_FUNC_DRIVE(u8PortNum, eSPI_CHIP_SELECT_FUNC, eGPIO_DEASSERT);
    
    #else
    UINT8 u8WriteBuf [UPD_I2C_MAX_BYTE_WRITE];
    UINT8 u8WriteBufLen = 2;
    
    u8WriteBuf[INDEX_0] = HIBYTE (u16RegOffset);
    u8WriteBuf[INDEX_1] = LOBYTE (u16RegOffset);
    
    for (; u8WriteBufLen < (u8WriteDataLen+2); u8WriteBufLen++)
    {
         u8WriteBuf[u8WriteBufLen] = pu8WriteData[u8WriteBufLen - 2];
    }

    (void)MCHP_PSF_HOOK_UPD_WRITE (u8PortNum, u8WriteBuf, u8WriteBufLen);
    
    #endif
}

/******************************************************************************************************/
void UPD_RegisterReadISR (UINT8 u8PortNum, UINT16 u16RegOffset, \
        UINT8 *pu8ReadData, UINT8 u8Readlen)
{
   #if (CONFIG_DEFINE_UPD350_HW_INTF_SEL == CONFIG_UPD350_SPI)
  
    UINT8 u8Command [UPD_SPI_READ_CMD_LEN];
    
	u8Command[INDEX_0] = UPD_SPI_READ_OPCODE;
	u8Command[INDEX_1] = HIBYTE (u16RegOffset);
	u8Command[INDEX_2] = LOBYTE (u16RegOffset);
	u8Command[INDEX_3] = UPD_SPI_DUMMY_BYTE;
    
    /*Enable SPI Select for communication*/
    MCHP_PSF_HOOK_GPIO_FUNC_DRIVE(u8PortNum, eSPI_CHIP_SELECT_FUNC, eGPIO_ASSERT);
    
    (void)MCHP_PSF_HOOK_UPD_READ (u8PortNum, u8Command, (UINT8)sizeof(u8Command), pu8ReadData, u8Readlen);
    
     /*Disable SPI Select for communication*/
    MCHP_PSF_HOOK_GPIO_FUNC_DRIVE(u8PortNum, eSPI_CHIP_SELECT_FUNC, eGPIO_DEASSERT);
    
    #else

    UINT8 u8Command [UPD_I2C_REG_CMD_LEN];
    
    u8Command[INDEX_0] = HIBYTE (u16RegOffset);
    u8Command[INDEX_1] = LOBYTE (u16RegOffset);
    
    (void)MCHP_PSF_HOOK_UPD_READ (u8PortNum, u8Command, (UINT8)sizeof(u8Command), pu8ReadData, u8Readlen);
    
    #endif
}

/******************************************************************************************************/
void UPD_DisablePIOOutputISR (UINT8 u8PortNum)
{
    UINT8 u8CurrPwrRole = DPM_GET_CURRENT_POWER_ROLE(u8PortNum);
    UINT8 u8PioNum = SET_TO_ZERO;
    UINT16 u16PIORegVal;    
       
    if (u8CurrPwrRole != PD_ROLE_DRP)
    {
        if (PD_ROLE_SOURCE == u8CurrPwrRole)
        {
            #if (TRUE == INCLUDE_PD_SOURCE) 
            u8PioNum = gasCfgStatusData.sPerPortData[u8PortNum].u8Pio_EN_VBUS;
            #endif  
            /* Clear the Port IO Status of EN_VBUS */
            gasCfgStatusData.sPerPortData[u8PortNum].u32PortIOStatus &= 
                                            ~(DPM_PORT_IO_EN_VBUS_STATUS);
        }
        else /* u8CurrPwrRole = PD_ROLE_SINK */
        {
            #if (TRUE == INCLUDE_PD_SINK) 
            u8PioNum = gasCfgStatusData.sPerPortData[u8PortNum].u8Pio_EN_SINK;
            #endif 
            /* Clear the Port IO Status of EN_SINK */
            gasCfgStatusData.sPerPortData[u8PortNum].u32PortIOStatus &= 
                                            ~(DPM_PORT_IO_EN_SINK_STATUS);
        }    
        UPD_RegisterReadISR (u8PortNum, (UPD_CFG_PIO_BASE + u8PioNum),\
                                    (UINT8 *)&u16PIORegVal, BYTE_LEN_1);

        u16PIORegVal &= ~UPD_CFG_PIO_DATAOUTPUT;

        UPD_RegisterWriteISR (u8PortNum, (UPD_CFG_PIO_BASE + u8PioNum),\
                                    (UINT8 *)&u16PIORegVal, BYTE_LEN_1);         
    }
    else
    {
        /*Execution should not hit here ideally*/
    }              
}

/*******************************************************************************************/
/**************************UPD PIO Configuration APIs***************************************/
/*******************************************************************************************/

void UPD_UpdatePIOOutput (UINT8 u8PortNum, UINT8 u8PIONum, UINT8 u8PioMode, UINT8 u8DriveType)
{
    if ((UINT8)eUPD_PIO_UN_DEF != u8PIONum)
    {
        /*read the GPIO register*/
        UINT8 u8PioData = UPD_RegReadByte (u8PortNum, (UPD_CFG_PIO_BASE + u8PIONum));
        
        /*clear the data output in the register*/
        u8PioData &= ~UPD_CFG_PIO_DATAOUTPUT;
        
        /*get the resultant drive value*/
        u8PioData |= ((u8PioMode ^ u8DriveType) & UPD_CFG_PIO_DATAOUTPUT);
        
        /*write the value to the GPIO register*/
        UPD_RegWriteByte (u8PortNum, (UPD_CFG_PIO_BASE + u8PIONum), u8PioData);
    }
}

/******************************************************************************************************/
void UPD_SetPIOIntrAlert (UINT8 u8PortNum, UINT8 u8PIONum, UINT8 u8IntrType)
{
	if (u8IntrType)
    {
         UPD_RegByteSetBit (u8PortNum, (UPD_CFG_PIO_BASE + u8PIONum), u8IntrType); 
    }
	else
    {
        UPD_RegByteClearBit (u8PortNum, (UPD_CFG_PIO_BASE + u8PIONum), \
		  						(UPD_CFG_PIO_FALLING_ALERT | UPD_CFG_PIO_RISING_ALERT));
    } 
}

/******************************************************************************************************/
void UPD_ConfigurePIODebounceCount(UINT8 u8PortNum, UINT8 u8CountType, UINT8 u8CountValue)
{
    UINT16 u16RegOffset = SET_TO_ZERO;
    switch (u8CountType)
    {
        case UPD_PIO_DEBOUNCE_CNT_TYP_1_US:
        {
            u16RegOffset= UPD_PIO_DEBOUNCE_1US_COUNT;
            break;
        }
        case UPD_PIO_DEBOUNCE_CNT_TYP_1_MS:
        {
            u16RegOffset = UPD_PIO_DEBOUNCE_1MS_COUNT;
            break;
        }
        case UPD_PIO_DEBOUNCE_CNT_TYP_10_MS:
        {
            u16RegOffset = UPD_PIO_DEBOUNCE_10MS_COUNT;
            break;
        }
        default:
        {
            break;
        }
    }
    UPD_RegWriteByte (u8PortNum, u16RegOffset, u8CountValue);
     
}

/******************************************************************************************************/
void UPD_SetPIODebounce (UINT8 u8PortNum, UINT8 u8PIONum, UINT8 u8DebounceEnType)
{
    UINT32 u32Data = SET_TO_ZERO;
    
    UPD_RegisterRead (u8PortNum, UPD_PIO_DEBOUNCE_EN, (UINT8 *)&u32Data, BYTE_LEN_4);
    
    if (UPD_PIO_DEBOUNCE_DISABLE == u8DebounceEnType)
    {                
        u32Data &= ~(((UINT32)UPD_PIO_DEBOUNCE_DISABLE_MASK) << (u8PIONum * UPD_PIO_DEBOUNCE_FIELD_WIDTH));        
    }
    else
    {     
        u32Data |= (((UINT32)u8DebounceEnType) << (u8PIONum * UPD_PIO_DEBOUNCE_FIELD_WIDTH));     
    } 
    
    UPD_RegisterWrite (u8PortNum, UPD_PIO_DEBOUNCE_EN, (UINT8 *)&u32Data, BYTE_LEN_4);
}

/******************************************************************************************************/
void UPD_InitGPIO (UINT8 u8PortNum)
{
    
#if (TRUE == INCLUDE_PD_FR_SWAP)    
    /* Initialize EN_FRS PIO based on the configured Power/Data state */
    if (PD_ROLE_SOURCE_UFP == DPM_GET_CONFIGURED_FRS_POWER_DATA_STATE(u8PortNum))
    {
        UPD_InitInputPIO (u8PortNum, eUPDPIO_EN_FRS);
    }
    else if (PD_ROLE_SINK_DFP == DPM_GET_CONFIGURED_FRS_POWER_DATA_STATE(u8PortNum))
    {
        UPD_InitOutputPIO (u8PortNum, gasCfgStatusData.sPerPortData[u8PortNum].u8Pio_EN_FRS, \
                                        gasCfgStatusData.sPerPortData[u8PortNum].u8Mode_EN_FRS);
    }
    else
    {
        /* FRS is disabled */
    }        
    
    /* Configure PIO Override for FRS conditions */
    #if (TRUE == INCLUDE_UPD_PIO_OVERRIDE_SUPPORT)
    UPD_ConfigureFRSPIOOverride (u8PortNum);
    #endif                    
#endif /* endif of INCLUDE_PD_FR_SWAP */
            
#if (TRUE == INCLUDE_POWER_FAULT_HANDLING)
    /* Configure Fault In Pin */
    UPD_InitInputPIO (u8PortNum, eUPDPIO_FAULT_IN);
    
    /* Configure PIO override for Fault conditions */
    #if (TRUE == INCLUDE_UPD_PIO_OVERRIDE_SUPPORT)		
        (void)UPD_ConfigPwrFaultPIOOverride (u8PortNum);
    #endif 		
#endif /* endif of INCLUDE_POWER_FAULT_HANDLING */    
}

/******************************************************************************************************/
void UPD_PIOHandleISR(UINT8 u8PortNum, UINT16 u16InterruptStatus)
{
	UINT16 u16IntrSts = SET_TO_ZERO;
	
    /* Check for PIO Interrupt */
    if (u16InterruptStatus & UPDINTR_PIO_INT)
    {
        /* Read the interrupt status*/
        UPD_RegisterReadISR (u8PortNum, UPD_PIO_INT_STS, (UINT8 *)&u16IntrSts, BYTE_LEN_2);

    #if (TRUE == INCLUDE_POWER_FAULT_HANDLING)

        UINT8 u8Pio_FAULT_IN = gasCfgStatusData.sPerPortData[u8PortNum].u8Pio_FAULT_IN;

        if ((BIT(u8Pio_FAULT_IN)) & u16IntrSts)
        {	
            UINT16 u16PIORegVal;

            /* Clear the OCS interrupt Configuration*/
            UPD_RegisterReadISR (u8PortNum, (UPD_CFG_PIO_BASE + u8Pio_FAULT_IN),\
                                        (UINT8 *)&u16PIORegVal, BYTE_LEN_1);

            u16PIORegVal &= ~(UPD_CFG_PIO_FALLING_ALERT | UPD_CFG_PIO_RISING_ALERT);

            UPD_RegisterWriteISR (u8PortNum, (UPD_CFG_PIO_BASE + u8Pio_FAULT_IN),\
                                            (UINT8 *)&u16PIORegVal, BYTE_LEN_1);

            /*Notify Power fault to DPM only none of the Power fault recovery is not in progress*/
            if (FALSE == (gasDPM[u8PortNum].u8PowerFaultFlags & DPM_HR_COMPLETE_WAIT_MASK))
            {
                /* Notify DPM about the power fault*/
                gasDPM[u8PortNum].u8PowerFaultISR |= DPM_POWER_FAULT_VBUS_OCS;
            }

            /*When PIO override is disabled; disable EN_VBUS/EN_SINK 
              based on the power role of the port */        
            #if (FALSE == INCLUDE_UPD_PIO_OVERRIDE_SUPPORT)
                UPD_DisablePIOOutputISR (u8PortNum);            
            #endif 
        }
    #endif /*INCLUDE_POWER_FAULT_HANDLING*/

    #if (TRUE == INCLUDE_PD_FR_SWAP)

        UINT8 u8Pio_EN_FRS = gasCfgStatusData.sPerPortData[u8PortNum].u8Pio_EN_FRS;
        UINT8 u8Mode_EN_FRS = gasCfgStatusData.sPerPortData[u8PortNum].u8Mode_EN_FRS;
        UINT8 u8PwrBackDetectionEdge;
        
        if ((BIT(u8Pio_EN_FRS)) & u16IntrSts)
        {
            UINT16 u16PIORegVal;

            /* Clear the EN_FRS interrupt Configuration*/
            UPD_RegisterReadISR (u8PortNum, (UPD_CFG_PIO_BASE + u8Pio_EN_FRS),\
                                        (UINT8 *)&u16PIORegVal, BYTE_LEN_1);

            if (((UINT8)eINPUT_ACTIVE_LOW == u8Mode_EN_FRS) || ((UINT8)eINPUT_ACTIVE_LOW_PU == u8Mode_EN_FRS))
            {
                u8PwrBackDetectionEdge = UPD_CFG_PIO_RISING_ALERT;
            }
            else
            {
                u8PwrBackDetectionEdge = UPD_CFG_PIO_FALLING_ALERT;
            }
            
            if (u16PIORegVal & u8PwrBackDetectionEdge)
            {
                DPM_RegisterInternalEvent (u8PortNum, DPM_INT_EVT_HANDLE_SYSTEM_POWER_RECOVERY);
            }
            /*System power loss event (DPM_INT_EVT_HANDLE_SYSTEM_POWER_LOSS) is not handled here.
              It will be done after source to sink FR_Swap is complete.*/
            
            u16PIORegVal &= ~(UPD_CFG_PIO_FALLING_ALERT | UPD_CFG_PIO_RISING_ALERT);

            UPD_RegisterWriteISR (u8PortNum, (UPD_CFG_PIO_BASE + u8Pio_EN_FRS),\
                                        (UINT8 *)&u16PIORegVal, BYTE_LEN_1);                             
            
            /* When PIO Override gets disabled, this pin will be automatically
               driven by UPD based on the previous value stored in CFG_PIO_BASE
               register. To avoid this, clear the Data Output value 
               of EN_VBUS pin */              
            UPD_DisablePIOOutputISR (u8PortNum);
        }

    #endif /* INCLUDE_PD_FR_SWAP */    

        /* Clear the interrupt status */
        UPD_RegisterWriteISR (u8PortNum, UPD_PIO_INT_STS, (UINT8 *)&u16IntrSts, BYTE_LEN_2);
    }
    
    /* Check for PIO Override Interrupt */
    if (u16InterruptStatus & UPDINTR_PIO_OVERRIDE_INT)
    {
        /* Read the Interrupt status */
        UPD_RegisterReadISR (u8PortNum, UPD_PIO_OVR_INT_STS, (UINT8 *)&u16IntrSts, BYTE_LEN_1);            
        
        #if (TRUE == INCLUDE_PD_FR_SWAP)
            
            UINT8 u8Data; 
            
            if (u16IntrSts & UPD_PIO_OVR_3)
            {          
                /* UPD DOS Reference: After being cleared by FW this bit will not be set again 
                   by HW until the FRS Detect Enable (FRS_DET_EN) is cleared. This is
                   irrespective of the reception of additional FRS signaling */                             
                UPD_RegisterReadISR (u8PortNum, TYPEC_FRS_CTL_HIGH, &u8Data, BYTE_LEN_1);
                u8Data &= ~(TYPEC_FRS_DET_EN); 
                UPD_RegisterWriteISR (u8PortNum, TYPEC_FRS_CTL_HIGH, &u8Data, BYTE_LEN_1);                                                
                
                /* When PIO Override gets disabled, this pin will be automatically
                   driven by UPD based on the previous value stored in CFG_PIO_BASE
                   register. To avoid this, clear the Data Output value 
                   of EN_SINK pin */               
                UPD_DisablePIOOutputISR (u8PortNum);

                /* Set the Port IO Status of EN_FRS pin */
                gasCfgStatusData.sPerPortData[u8PortNum].u32PortIOStatus |= DPM_PORT_IO_EN_FRS_STATUS;                
            }                 
        #endif 

        /* Clear the interrupt status */
        UPD_RegisterWriteISR (u8PortNum, UPD_PIO_OVR_INT_STS, (UINT8 *)&u16IntrSts, BYTE_LEN_1);        
    }
}

/*******************************************************************************************/
/***********************UPD Output PIO Configuration API ***********************************/
/*******************************************************************************************/
void UPD_InitOutputPIO (UINT8 u8PortNum, UINT8 u8PIONum, UINT8 u8PIOMode)
{
    if ((UINT8)eUPD_PIO_UN_DEF != u8PIONum)
    {
        /*clear bits 6:4 and 1:0 in the read value to avoid invalid combinations.*/
        u8PIOMode &= (UPD_CFG_PIO_PULL_UP_ENABLE | UPD_CFG_PIO_DATAOUTPUT | UPD_CFG_PIO_BUFFER_TYPE);
        /*enable GPIO and direction as output*/
        u8PIOMode |= (UPD_CFG_PIO_DIRECTION | UPD_CFG_PIO_GPIO_ENABLE);
        /*de-assert the pin*/
        u8PIOMode ^= UPD_CFG_PIO_DATAOUTPUT;
        /*update the value to GPIO register.*/
        UPD_RegWriteByte (u8PortNum, UPD_CFG_PIO_REGADDR(u8PIONum), u8PIOMode);
    }
}

/*******************************************************************************************/
/***********************UPD Input PIO Configuration APIs ***********************************/
/*******************************************************************************************/

#if ((TRUE == INCLUDE_POWER_FAULT_HANDLING) || (TRUE == INCLUDE_PD_FR_SWAP))

void UPD_InitInputPIO (UINT8 u8PortNum, eUPD_INPUT_PIO eUPDInputPio)
{
	UINT8 u8PIONum = SET_TO_ZERO;	
    UINT8 u8PIOMode = SET_TO_ZERO;
 
    if (eUPDPIO_FAULT_IN == eUPDInputPio)
    {
        u8PIONum = gasCfgStatusData.sPerPortData[u8PortNum].u8Pio_FAULT_IN;
        u8PIOMode = gasCfgStatusData.sPerPortData[u8PortNum].u8Mode_FAULT_IN;
    }
#if (TRUE == INCLUDE_PD_FR_SWAP)
    else if (eUPDPIO_EN_FRS == eUPDInputPio)
    {
        u8PIONum = gasCfgStatusData.sPerPortData[u8PortNum].u8Pio_EN_FRS;
        u8PIOMode = gasCfgStatusData.sPerPortData[u8PortNum].u8Mode_EN_FRS;        
    }
#endif     
    else
    {
        /* Do Nothing */
    }    
    
    /* Return without initializing the pin if EN_FRS/FAULT_IN pin functionality 
       is disabled by the user */
    if ((UINT8)eUPD_PIO_UN_DEF == u8PIONum)
    {
        return; 
    }
    
    UINT16 u16PIOIntrEnPos;
    
    /* Clear bits 3:0 from user input.*/
    u8PIOMode &= (UPD_CFG_PIO_PULL_UP_ENABLE | UPD_CFG_PIO_PULL_DOWN_ENABLE | \
                        UPD_CFG_PIO_FALLING_ALERT | UPD_CFG_PIO_RISING_ALERT);
    
    /* Set direction to input and enable GPIO.*/
    u8PIOMode |= UPD_CFG_PIO_GPIO_ENABLE;
    
    /* Write the value to the PIO config register.*/
    UPD_RegWriteByte (u8PortNum, UPD_CFG_PIO_REGADDR(u8PIONum), u8PIOMode);
    
    if (eUPDPIO_FAULT_IN == eUPDInputPio)
    {
        /*Write Debounce count*/
        UPD_ConfigurePIODebounceCount (u8PortNum, UPD_PIO_DEBOUNCE_CNT_TYP_1_MS, gasCfgStatusData.sPerPortData[u8PortNum].u8FaultInDebounceInms);
    
        /* Enable Debounce*/
        UPD_SetPIODebounce (u8PortNum, u8PIONum, UPD_PIO_DEBOUNCE_CNT_TYP_1_MS);
    }
#if (TRUE == INCLUDE_PD_FR_SWAP)    
    else if (eUPDPIO_EN_FRS == eUPDInputPio)
    {
        /* In order to minimize response time the PIO selected by 
        FRS Request PIO Select should have debouncing disabled */
        UPD_SetPIODebounce (u8PortNum, u8PIONum, UPD_PIO_DEBOUNCE_DISABLE);
    }
#endif 
    else
    {
        /* Do Nothing */
    }
	
    u16PIOIntrEnPos = BIT(u8PIONum);
    UPD_RegisterWrite (u8PortNum, UPD_PIO_INT_STS, (UINT8 *)&u16PIOIntrEnPos, BYTE_LEN_2);
    
	/* Enable the PIO interrupt*/
	u16PIOIntrEnPos |= UPD_RegReadWord(u8PortNum, UPD_PIO_INT_EN);
	UPD_RegWriteWord (u8PortNum, UPD_PIO_INT_EN, u16PIOIntrEnPos);
}

/*******************************************************************************************/
void UPD_EnableInputPIO (UINT8 u8PortNum, eUPD_INPUT_PIO eUPDInputPio)
{
	UINT8 u8PIONum = SET_TO_ZERO;	
    UINT8 u8PIOMode = SET_TO_ZERO;
 
    if (eUPDPIO_FAULT_IN == eUPDInputPio)
    {
        u8PIONum = gasCfgStatusData.sPerPortData[u8PortNum].u8Pio_FAULT_IN;
        u8PIOMode = gasCfgStatusData.sPerPortData[u8PortNum].u8Mode_FAULT_IN;
    }
#if (TRUE == INCLUDE_PD_FR_SWAP)    
    else if (eUPDPIO_EN_FRS == eUPDInputPio)
    {
        u8PIONum = gasCfgStatusData.sPerPortData[u8PortNum].u8Pio_EN_FRS;
        u8PIOMode = gasCfgStatusData.sPerPortData[u8PortNum].u8Mode_EN_FRS;        
    }
#endif 
    else
    {
        /* Do Nothing */
    }
    
    UINT16 u16IntrSts = BIT(u8PIONum);
    
    /* Get the edge type from the user input.*/
    u8PIOMode &= (UPD_CFG_PIO_RISING_ALERT | UPD_CFG_PIO_FALLING_ALERT);

	/* Clear the interrupt and configure the edge type*/
	UPD_RegisterWrite (u8PortNum, UPD_PIO_INT_STS, (UINT8 *)&u16IntrSts, BYTE_LEN_2);
	
    UPD_SetPIOIntrAlert (u8PortNum, u8PIONum, u8PIOMode);	
}

#endif 

/*******************************************************************************************/
/************************ UPD PIO Override APIs ********************************************/
/*******************************************************************************************/

#if (TRUE == INCLUDE_UPD_PIO_OVERRIDE_SUPPORT)

void UPD_ConfigPwrFaultPIOOverride (UINT8 u8PortNum)
{
	/* Override 0 - Overvoltage Threshold*/
    /* Override 1 - UnderVoltage Threshold */
  	/* Override 2 - Fault Low*/
    
#if (TRUE == INCLUDE_PD_DRP)   
    UINT16 u16PIOPos = (BIT(gasCfgStatusData.sPerPortData[u8PortNum].u8Pio_EN_VBUS) | \
                        BIT(gasCfgStatusData.sPerPortData[u8PortNum].u8Pio_EN_SINK));
#elif (TRUE == INCLUDE_PD_SOURCE)
    UINT16 u16PIOPos = BIT(gasCfgStatusData.sPerPortData[u8PortNum].u8Pio_EN_VBUS);
#else
    UINT16 u16PIOPos = BIT(gasCfgStatusData.sPerPortData[u8PortNum].u8Pio_EN_SINK);
#endif
    UINT16 u16OvrData;
    
    /*Setting Monitoring bit as '1' checks whether voltage exceeds the configured source 
        selection VBUS threshold value or the source selection PIO goes high */
    /* Setting Monitoring bit as '0' checks whether voltage falls below the source 
            selection VBUS threshold value or the source selection PIO goes low */
    /* Enable monitoring for Override 0 - Overvoltage alone. Default value is 0,
       so no need to clear bits 1 and 2 for Override 1 and Override 2 */
    UPD_RegByteSetBit (u8PortNum, UPD_PIO_MON_VAL, (UINT8)UPD_PIO_OVR_0);

    /* PIO override output is set as low */
    u16OvrData = UPD_RegReadWord (u8PortNum, UPD_PIO_OVR_OUT);
    u16OvrData &= ~u16PIOPos; 
    UPD_RegWriteWord (u8PortNum, UPD_PIO_OVR_OUT, u16OvrData); 
        
    /* Configure the Source for override 0 as OverVoltage Threshold 2*/
    UPD_RegWriteByte (u8PortNum, UPD_PIO_OVR0_SRC_SEL, \
      (UPD_PIO_OVR_SRC_SEL_VBUS_THR | UPD_PIO_OVR_VBUS2_THR_MATCH));

    /* Configure the Source for override 1 as under-voltage*/
    UPD_RegWriteByte (u8PortNum, UPD_PIO_OVR1_SRC_SEL, \
      (UPD_PIO_OVR_SRC_SEL_VBUS_THR | UPD_PIO_OVR_VBUS3_THR_MATCH));

    /* Configure the Source for override 2 as Fault_IN/PRT_CTL pin low from Load switch */
    UPD_RegWriteByte (u8PortNum, UPD_PIO_OVR2_SRC_SEL, gasCfgStatusData.sPerPortData[u8PortNum].u8Pio_FAULT_IN);

    /* EN_VBUS is configured as override Pin in output mode */
    u16OvrData = UPD_RegReadWord (u8PortNum, UPD_PIO_OVR_DIR);
    u16OvrData |= u16PIOPos; 
    UPD_RegWriteWord (u8PortNum, UPD_PIO_OVR_DIR, u16OvrData);   
        
    /* Override 0*/
    UPD_RegWriteWord (u8PortNum, UPD_PIO_OVR0_OUT_EN, u16PIOPos);

    /* Override 1*/
    UPD_RegWriteWord (u8PortNum, UPD_PIO_OVR1_OUT_EN, u16PIOPos);

    /* Override 2*/
    UPD_RegWriteWord (u8PortNum, UPD_PIO_OVR2_OUT_EN, u16PIOPos);

    /* Set Override Enable Mask for Override 1. This will suppress related
       events until Under Voltage threshold detection is enabled */
    UPD_RegByteSetBit (u8PortNum, UPD_FRS_PIO_OVR_EN_MSK, (UINT8)UPD_PIO_OVR_1);     
    
    /* Enable the override for FAULT_IN*/
    /* Under voltage and overvoltage override is enabled after configuring the
        threshold*/
    UPD_RegByteSetBit (u8PortNum, UPD_PIO_OVR_EN, UPD_PIO_OVR_2);
}

/*******************************************************************************************/
#if (TRUE == INCLUDE_PD_FR_SWAP)

void UPD_ConfigureFRSPIOOverride (UINT8 u8PortNum)
{
	/* Override 2:0 - used for Power Fault conditions 
       Override 3 - EN_FRS PIO assertion in case of Source UFP;
                    FRS Signal Detected and VBUS falling below 
                    vSafe5V (upper threshold) in case of Sink DFP */ 
    
    UINT8 u8CfgPwrDataSt = DPM_GET_CONFIGURED_FRS_POWER_DATA_STATE(u8PortNum);
    
    if ((u8CfgPwrDataSt != PD_ROLE_SOURCE_UFP) && (u8CfgPwrDataSt != PD_ROLE_SINK_DFP))
    {
        return;        
    }
    
    /* Get the PIO positions of EN_VBUS, EN_SINK and EN_FRS PIOs */
    UINT8 u8Mode_EN_FRS = gasCfgStatusData.sPerPortData[u8PortNum].u8Mode_EN_FRS;
    UINT8 u8PIOMonVal = FALSE, u8PIOOvrIntEn = FALSE;  
    UINT16 u16PIOPos;  
    UINT16 u16EnVBUSPIOPos = BIT(gasCfgStatusData.sPerPortData[u8PortNum].u8Pio_EN_VBUS);    
    UINT16 u16EnSinkPIOPos = BIT(gasCfgStatusData.sPerPortData[u8PortNum].u8Pio_EN_SINK);  
    UINT16 u16EnFRSPIOPos = BIT(gasCfgStatusData.sPerPortData[u8PortNum].u8Pio_EN_FRS);
    UINT16 u16PIOOvrOut = UPD_RegReadWord (u8PortNum, UPD_PIO_OVR_OUT);
    UINT16 u16PIOOvrDir = UPD_RegReadWord (u8PortNum, UPD_PIO_OVR_DIR);               
    
    /* PIO Override Configuration for Source UFP type of Port */
    if (PD_ROLE_SOURCE_UFP == u8CfgPwrDataSt)
    {
        /* Override Monitor value for Override 3 based on the polarity of u8Pio_EN_FRS */
        if (((UINT8)eINPUT_ACTIVE_HIGH == u8Mode_EN_FRS) || ((UINT8)eINPUT_ACTIVE_HIGH_PD == u8Mode_EN_FRS))
        {
            u8PIOMonVal = TRUE;             
        }
      
        /* Configure the Source for override 3 as FRS_Request pin */
        UPD_RegWriteByte (u8PortNum, UPD_PIO_OVR3_SRC_SEL, gasCfgStatusData.sPerPortData[u8PortNum].u8Pio_EN_FRS);
        
        /* PIO Override output is low */        
        u16PIOOvrOut &= ~u16EnVBUSPIOPos;  
    
        /* Direction of EN_VBUS as override pin is output */        
        u16PIOOvrDir |= u16EnVBUSPIOPos;         
        
        /* Override 3 Output Enable */
        u16PIOPos = u16EnVBUSPIOPos;                
    }
    /* PIO Override Configuration for Sink DFP type of Port */
    else 
    {
        /* UPD DOS Reference: 
           PIO override source select must be set to 0x12 in PIO Overridex Source 
           Select Register. The respective PIO_OVR_MON must be set to 0x0, in PIO 
           Overridex Monitor Value Register to trip on VBUS falling below 
           vSAFE5v (upper) per VBUS Threshold Select */               
        /* PIO Override source for Sink port will be set in TypeC_EnableFRSXMTOrDET() 
           depending on the negotiated voltage */
        
        /* PIO Override output is low for EN_SINK and high for EN_FRS */                
        u16PIOOvrOut &= ~(u16EnSinkPIOPos); 
        u16PIOOvrOut |= u16EnFRSPIOPos;         
        
        /* Configure EN_FRS and EN_SINK as override pins in output mode
           De-asserting EN_SINK will stop the sink circuitry from sinking power
           De-asserting EN_FRS will turn on the internal MOSFET of Load switch, so
           that 5V will be driven in VBUS */                
        u16PIOOvrDir |= (u16EnFRSPIOPos | u16EnSinkPIOPos);        
        
        /* Override 3 Output Enable */
        u16PIOPos = (u16EnFRSPIOPos | u16EnSinkPIOPos);   
        
        /* PIO Override Interrupt is needed for Sink to Source FRS 
           to disable FRS_DET_EN on override completion */
        u8PIOOvrIntEn = TRUE; 
    }
    
    if (u8PIOMonVal)
    {
        /* Set Override Monitor value only if value is 1. Else, default value is 0 */
        UPD_RegByteSetBit (u8PortNum, UPD_PIO_MON_VAL, (UINT8)UPD_PIO_OVR_3);
    }    
    
    /* Set PIO Override Output */
    UPD_RegWriteWord (u8PortNum, UPD_PIO_OVR_OUT, u16PIOOvrOut);
    
    /* Configure override direction for the override pins */
    UPD_RegWriteWord (u8PortNum, UPD_PIO_OVR_DIR, u16PIOOvrDir);
    
    /* Set Override 3 Output Enable */
    UPD_RegWriteWord (u8PortNum, UPD_PIO_OVR3_OUT_EN, u16PIOPos);  
    
    if (u8PIOOvrIntEn)
    {        
        /* Enable high level UPD350 PIO Override Interrupt */
        UPD_RegByteSetBit (u8PortNum, UPDINTR_INT_EN, UPDINTR_PIO_OVERRIDE_INT);
        
        /* Enable interrupt for PIO Override 3 */
        UPD_RegByteSetBit (u8PortNum, UPD_PIO_OVR_INT_EN, (UINT8)UPD_PIO_OVR_3);

        /* Set PIO Override Interrupt Enable Mask. This will prevent the 
           PIO Override interrupt from firing when FRS_DET_EN is disabled */
        UPD_RegByteSetBit (u8PortNum, UPD_FRS_PIO_OVR_EN_MSK, (UINT8)UPD_PIO_OVR_3);        
    }      
    
    /* PIO Override function will be enabled in the Ready state when 
       it is discovered that port partners are FRS capable */
}

#endif
#endif

/*******************************************************************************************/
/************************ UPD Common Support APIs ******************************************/
/*******************************************************************************************/

void UPD_ResetThroughGPIO()
{
    /*Since, all UPD350 PIOs are tied to single PIO, Reset is done for PORT0
     alone to avoid multiple reset, PORT0 is passed as dummy variable */
    /* Pull down is driven to reset the UPD350*/
    MCHP_PSF_HOOK_GPIO_FUNC_DRIVE(PORT0, eUPD350_RESET_FUNC,eGPIO_ASSERT);

    /* Set to default state*/
    MCHP_PSF_HOOK_GPIO_FUNC_DRIVE(PORT0, eUPD350_RESET_FUNC,eGPIO_DEASSERT);
}

/********************************************************************************************/
void UPD_CheckAndDisablePorts (void)
{
    UINT8 u8ReadData[BYTE_LEN_4];
    
    /*variable to hold the timer id*/
    UINT8 u8TimerID;
    
     /* Reset the Port's UPD350 present*/
    UPD_ResetThroughGPIO ();

    /*run a loop for all the number of CONFIG_PD_PORT_COUNT to check all ports*/
    for (UINT8 u8PortNum = SET_TO_ZERO; u8PortNum < CONFIG_PD_PORT_COUNT; u8PortNum++)
    {
        /*Check if timer is Active, if Timer expired, come out of this loop */
        
        if (UPD_PORT_ENABLED == DPM_GET_CONFIGURED_PORT_EN(u8PortNum))
        {
            /*Start 10ms timer*/
            u8TimerID = PDTimer_Start (MILLISECONDS_TO_TICKS(BYTE_LEN_10), NULL, \
                                        (UINT8)SET_TO_ZERO, (UINT8)SET_TO_ZERO);
            
            while ((gasPDTimers[u8TimerID].u8TimerStPortNum & PDTIMER_STATE) != PDTIMER_EXPIRED)
            {
#if (CONFIG_UPD350_SPI == CONFIG_DEFINE_UPD350_HW_INTF_SEL)            
                /*Read SPI_TEST register*/
                UPD_RegisterRead (u8PortNum, (UINT16)UPD_SPI_TEST, u8ReadData, BYTE_LEN_4);
                    
                /*Check the SPI_TEST register value is 0x02*/
                if (UPD_SPI_TEST_VAL == u8ReadData[INDEX_0])
                {
#endif
                    /*Read VID & PID register*/
                    UPD_RegisterRead (u8PortNum, (UINT16)UPD_VID, u8ReadData, BYTE_LEN_4);          
             
                    /*Verify the default values*/
                    if ((UPD_VID_LSB == u8ReadData[INDEX_0]) && (UPD_VID_MSB == u8ReadData[INDEX_1]) && \
                        (UPD_PID_LSB == u8ReadData[INDEX_2]) && (UPD_PID_MSB == u8ReadData[INDEX_3])) 
                    {  
                        /*Value read from this port is right, so enable the ports, Set SPI 
                           Communication is active for this port*/
                        DPM_ENABLE_CONFIGURED_PORT_EN(u8PortNum);
                        break;
                    }
                    else
                    {
                        /* If the VID and PID doesn't match, Disable the ports */
                        DPM_DISABLE_CONFIGURED_PORT_EN(u8PortNum);                        
                    }
#if (CONFIG_UPD350_SPI == CONFIG_DEFINE_UPD350_HW_INTF_SEL)            
                } /*end of UPD_SPI_TEST_VAL check if*/
                else
                {
                    DPM_DISABLE_CONFIGURED_PORT_EN(u8PortNum);
                }   /*end of UPD_SPI_TEST_VAL check if else*/
#endif            
            } /* end of while*/
            /*kill the timer if the UPD is identified.*/
            PDTimer_Kill (u8TimerID);
        } /*end of if for port check*/
    } /*end of for*/
    
    /* Work around - If port-0 as source and port-1 as sink interrupt issued continuously */
    for (UINT8 u8PortNum = SET_TO_ZERO; u8PortNum < CONFIG_PD_PORT_COUNT; u8PortNum++)
  	{
        if (UPD_PORT_DISABLED == DPM_GET_CONFIGURED_PORT_EN(u8PortNum))
        {
            gasCfgStatusData.sPerPortData[u8PortNum].u32CfgData = SET_TO_ZERO;
        }
    }
}

/********************************************************************************************/
void UPD_FindVBusCorrectionFactor(void)
{
    UINT16 u16VBUSTHR3 = SET_TO_ZERO;
      
    for (UINT8 u8PortNum = SET_TO_ZERO; u8PortNum < CONFIG_PD_PORT_COUNT; u8PortNum++)
    {
        if (UPD_PORT_ENABLED == DPM_GET_CONFIGURED_PORT_EN(u8PortNum))
        {
            /* Read VBUS threshold register value from OTP */    
            u16VBUSTHR3 = UPD_RegReadWord (u8PortNum, TYPEC_VBUS_THR3);
            
            gasTypeCcontrol[u8PortNum].fVBUSCorrectionFactor = \
                                    (float)((float)u16VBUSTHR3/(float)UPD_VBUS_THRS_DEFAULT);
        }
    }        
}

/*******************************************************************************************/
/************************ UPD Hot Plug Detect(HPD) APIs ************************************/
/*******************************************************************************************/

#if(TRUE == INCLUDE_UPD_HPD)

void UPD_InitHPD (UINT8 u8PortNum)
{
    UINT16 u16Data;
    
    /*Configure IRQ_HPD_MIN_TIME to be 350us (i.e) greater than 250us*/
    UPD_RegWriteByte (u8PortNum, UPD_IRQ_HPD_MIN_TIME, HPD_IRQ_MIN_TIME_350US);
    
    /*Configure IRQ_HPD_MAX_TIME to be 2ms*/
    UPD_RegWriteByte (u8PortNum, UPD_IRQ_HPD_MAX_TIME, HPD_IRQ_MIN_TIME_2_1MS);
    
    /*Configure HPD_HIGH_DET_TIME to be 100ms*/
    UPD_RegWriteWord (u8PortNum, UPD_HPD_HIGH_DET_TIME, UPD_HPD_HIGH_DET_TIME_100_1MS);
    
    /*Configure HPD_LOW_DET_TIME to be 2ms*/
    UPD_RegWriteWord (u8PortNum, UPD_HPD_LOW_DET_TIME, UPD_HPD_LOW_DET_TIME_2_1MS);
    
    /*Enable QUEUE_NOT_EMPTY interrupt*/
    UPD_RegWriteByte (u8PortNum, UPD_HPD_INT_EN, UPD_QUEUE_NOT_EMPTY_EN);
    
    /*Setting the UPD350 high level HPD interrupt*/ 
    u16Data = UPD_RegReadWord (u8PortNum, UPDINTR_INT_EN);
    u16Data |= UPDINTR_HPD_INT;
	UPD_RegWriteWord (u8PortNum, UPDINTR_INT_EN, u16Data);
    
    /*Disable u8PIO_HPD as gpio, to enable it to act as HPD IO*/
    UPD_RegByteClearBit (u8PortNum, (UPD_CFG_PIO_BASE + gasCfgStatusData.sPerPortData[u8PortNum].u8PIO_HPD), \
                        UPD_CFG_PIO_ENABLE);
    
    /*Configure HPD peripheral in input mode*/
    UPD_RegByteClearBit (u8PortNum, UPD_HPD_CTL, UPD_HPD_CFG);
    
    /*Initially, disable HPD*/
    UPD_RegByteClearBit (u8PortNum, UPD_HPD_CTL, UPD_HPD_ENABLE);
    
    //DEBUG_PRINT_PORT_STR(u8PortNum, "UPD_HPD Initialized and disabled\r\n");
    
    /*HPD peripheral will be enabled by user application via client request.*/
}

/********************************************************************************************/
void UPD_HPDHandleISR (UINT8 u8PortNum)
{
    UINT8 u8Data;
    UINT8 u8HPDClr = SET_TO_ZERO;
    UINT8 u8Mask = SET_TO_ZERO;

    UPD_RegisterReadISR (u8PortNum, UPD_HPD_QUEUE, &u8Data, BYTE_LEN_1);

    /*If an entry in the queue is non-zero, write b'01 to the queue slot to clear h/w status*/
    for (u8Mask = UPD_HPD_EVENT_MASK; u8Mask != SET_TO_ZERO; u8Mask <<= UPD_HPD_EVENT_SIZE)
    {
        if (u8Mask & u8Data)
        { 
            u8HPDClr |= (u8Mask & UPD_HPD_WRITE_CLR); 
        }
    }

    UPD_RegisterWriteISR (u8PortNum, UPD_HPD_QUEUE, &u8HPDClr, BYTE_LEN_1);

    /*Copy the queue events to gu16HPDStsISR[u8PortNum]*/
    gu16HPDStsISR[u8PortNum] = u8Data;
    gu16HPDStsISR[u8PortNum] |= UPD_HPD_INTERRUPT_OCCURRED;
}

#endif

/*******************************************************************************************/
/************************ Power Management Control APIs ************************************/
/*******************************************************************************************/

#if (TRUE == INCLUDE_POWER_MANAGEMENT_CTRL)

UINT8 UPD_ReturnUPDsActiveSts ()
{
    UINT8 u8IsAllUPDsActive = FALSE;
    
    for (UINT8 u8PortNum = SET_TO_ZERO; u8PortNum < CONFIG_PD_PORT_COUNT; u8PortNum++)
  	{
		/*Ignore if port is disabled, so consider only for enabled ports*/
		if (UPD_PORT_ENABLED == DPM_GET_CONFIGURED_PORT_EN(u8PortNum))
		{
			/*UPD_STATE_ACTIVE will be set frequently by respective Alert ISR.
			  It means that the appropriate port is active, so skip MCU IDLE*/
			if ((UPD_STATE_ACTIVE == gau8ISRPortState[u8PortNum]) ||
			/*Verify any other IDLE timer is running for all other ports.
			if its running, then lets handle in that timer expire event, so skip MCU
			IDLE for now*/
             (((gau8PortIdleTimerID[u8PortNum]< MAX_CONCURRENT_TIMERS) && \
             (gasPDTimers[gau8PortIdleTimerID[u8PortNum]].u8TimerStPortNum & PDTIMER_STATE ) == PDTIMER_ACTIVE)))
			{
				u8IsAllUPDsActive = TRUE;
				break;
			}
		}
    }
  
    return u8IsAllUPDsActive;
}

/******************************************************************************************************/
void UPD_SetIdleCB (UINT8 u8PortNum, UINT8 u8DummyVariable)
{
	/*Invalidate this Timer ID since it is stored in global*/
    gau8PortIdleTimerID[u8PortNum] = MAX_CONCURRENT_TIMERS;
	
	/*Set gau8PortState to UPD_STATE_IDLE*/
	gau8ISRPortState[u8PortNum] = UPD_STATE_IDLE;

	/*Enable RX_AFE: In order to receive a PD message the RX AFE shall be enabled by SW and the
      trip point set via CC RX DAC Control Register (CC_RX_DAC_CTL) and  CC RX DAC Filter Register
	  (CC_RX_DAC_FILT)*/
	
	/*Disable Ring , 48 MHz Oscillator and System clock to put UPD350 to idle*/
	UPD_RegByteClearBit (u8PortNum, UPD_CLK_CTL, \
            (UPD_RING_OSC_ENABLE | UPD_48MHZ_OSC_ENABLE | UPD_SYS_CLK_ENABLE));
    
    //DEBUG_PRINT_PORT_STR (u8PortNum,"UPDHW: Set Port Idle\r\n");
	
	/* Put MCU into IDLE */

	if (FALSE == UPD_ReturnUPDsActiveSts ())
	{
        //DEBUG_PRINT_PORT_STR (u8PortNum,"UPDHW: Set MCU IDLE\r\n");
                
		gu8SetMCUIdle = UPD_MCU_IDLE;
	}	
}

/******************************************************************************************************/
void PD_StartIdleTimer (UINT8 u8PortNum)
{
    MCHP_PSF_HOOK_DISABLE_GLOBAL_INTERRUPT();
                
    /*if UPD350 is active; Restart UPD IDLE Timer*/
    if (UPD_STATE_ACTIVE == gau8ISRPortState[u8PortNum])
    {        
        PDTimer_Kill (gau8PortIdleTimerID[u8PortNum]);
        
        gau8PortIdleTimerID[u8PortNum] = MAX_CONCURRENT_TIMERS;
        
        gau8PortIdleTimerID[u8PortNum] = PDTimer_Start (\
                    UPD_IDLE_TIMEOUT_MS,\
                    UPD_SetIdleCB,\
                    u8PortNum, \
                    (UINT8)SET_TO_ZERO);
                    
        gau8ISRPortState[u8PortNum] = UPD_STATE_STARTED_IDLE_TIMER;
    }
    
    MCHP_PSF_HOOK_ENABLE_GLOBAL_INTERRUPT();
} 

/******************************************************************************************************/
void UPD_PwrManagementCtrl (UINT8 u8PortNum)
{
   /*Restart IDLE Timer if UPD350 is Active*/
    PD_StartIdleTimer (u8PortNum);

    if (UPD_MCU_IDLE == gu8SetMCUIdle)
    {
        /*Invalidate MCU Idle, Before keeping MCU in Idle clear this set 
         gu8SetMCUidle to Active state for next run */
        gu8SetMCUIdle = UPD_MCU_ACTIVE;

        if (FALSE == UPD_ReturnUPDsActiveSts())
        {
            /* Notification CallBack to the Client, to indicate all the
             UPD350s are in idle state */
            (void)DPM_NotifyClient(u8PortNum, eMCHP_PSF_UPDS_IN_IDLE);
        }
    } 
}

/********************************************************************************************/
void UPD_InitPwrManagement (UINT8 u8PortNum)
{
    /*Set UPD state as Active in the initialization*/
    gau8ISRPortState[u8PortNum] = UPD_STATE_ACTIVE;

    /*Start idle timer */
    gau8PortIdleTimerID[u8PortNum] = PDTimer_Start(\
                                        UPD_IDLE_TIMEOUT_MS,\
                                        UPD_SetIdleCB,\
                                        u8PortNum, \
                                        (UINT8)SET_TO_ZERO);
    
    /*Set MCU Idle flag as UPD_MCU_ACTIVE*/
    gu8SetMCUIdle = UPD_MCU_ACTIVE;
}

/********************************************************************************************/
#endif /* INCLUDE_POWER_MANAGEMENT_CTRL*/


/*******************************************************************************************/
/************************ UPD Debug Support APIs *******************************************/
/*******************************************************************************************/

#if(TRUE == CONFIG_HOOK_DEBUG_MSG)

void UPD_RegDump (UINT8 u8PortNum)
{
    UINT8 u8Data = SET_TO_ZERO;
    
    u8Data = UPD_RegReadByte (u8PortNum, TYPEC_CC_HW_CTL_LOW);
    DEBUG_PRINT_PORT_UINT32_STR(u8PortNum, "TYPEC_CC_HW_CTL_LOW - ", u8Data, BYTE_LEN_1, "\r\n");

    u8Data = UPD_RegReadByte (u8PortNum, TYPEC_CC_HW_CTL_HIGH);
    DEBUG_PRINT_PORT_UINT32_STR(u8PortNum, "TYPEC_CC_HW_CTL_HIGH - ", u8Data, BYTE_LEN_1, "\r\n");
  
    u8Data = UPD_RegReadByte (u8PortNum, TYPEC_CC_INT_STS);
    DEBUG_PRINT_PORT_UINT32_STR(u8PortNum, "TYPEC_CC_INT_STS - ", u8Data, BYTE_LEN_1, "\r\n");
    
    u8Data = UPD_RegReadByte (u8PortNum, TYPEC_CC1_CHG_STS);
    DEBUG_PRINT_PORT_UINT32_STR(u8PortNum, "TYPEC_CC1_CHG_STS - ", u8Data, BYTE_LEN_1, "\r\n");
 
    u8Data = UPD_RegReadByte (u8PortNum, TYPEC_CC2_CHG_STS);
    DEBUG_PRINT_PORT_UINT32_STR(u8PortNum, "TYPEC_CC_HW_CTL_LOW - ", u8Data, BYTE_LEN_1, "\r\n");

    u8Data = UPD_RegReadByte (u8PortNum, TYPEC_CC1_MATCH);
    DEBUG_PRINT_PORT_UINT32_STR(u8PortNum, "TYPEC_CC1_MATCH - ", u8Data, BYTE_LEN_1, "\r\n");

    u8Data = UPD_RegReadByte (u8PortNum, TYPEC_CC2_MATCH);
    DEBUG_PRINT_PORT_UINT32_STR(u8PortNum, "TYPEC_CC2_MATCH - ", u8Data, BYTE_LEN_1, "\r\n");

    u8Data = UPD_RegReadByte (u8PortNum, TYPEC_CC_INT_EN);
    DEBUG_PRINT_PORT_UINT32_STR(u8PortNum, "TYPEC_CC_INT_EN - ", u8Data, BYTE_LEN_1, "\r\n");
 
    u8Data = UPD_RegReadByte (u8PortNum, TYPEC_CC1_MATCH_EN);
    DEBUG_PRINT_PORT_UINT32_STR(u8PortNum, "TYPEC_CC1_MATCH_EN - ", u8Data, BYTE_LEN_1, "\r\n");

    u8Data = UPD_RegReadByte (u8PortNum, TYPEC_CC2_MATCH_EN);
    DEBUG_PRINT_PORT_UINT32_STR(u8PortNum, "TYPEC_CC2_MATCH_EN - ", u8Data, BYTE_LEN_1, "\r\n");

    u8Data = UPD_RegReadByte (u8PortNum, TYPEC_VBUS_MATCH_EN);
    DEBUG_PRINT_PORT_UINT32_STR(u8PortNum, "TYPEC_VBUS_MATCH_EN - ", u8Data, BYTE_LEN_1, "\r\n");
    
    u8Data = UPD_RegReadByte (u8PortNum, TYPEC_PWR_INT_EN);
    DEBUG_PRINT_PORT_UINT32_STR(u8PortNum, "TYPEC_PWR_INT_EN - ", u8Data, BYTE_LEN_1, "\r\n");

    u8Data = UPD_RegReadByte (u8PortNum, TYPEC_MATCH_DEB);
    DEBUG_PRINT_PORT_UINT32_STR(u8PortNum, "TYPEC_MATCH_DEB - ", u8Data, BYTE_LEN_1, "\r\n");

    u8Data = UPD_RegReadByte (u8PortNum, TYPEC_VCONN_DEB);
    DEBUG_PRINT_PORT_UINT32_STR(u8PortNum, "TYPEC_VCONN_DEB - ", u8Data, BYTE_LEN_1, "\r\n");

    u8Data = UPD_RegReadByte (u8PortNum, TYPEC_CC1_DBCLR_EN);
    DEBUG_PRINT_PORT_UINT32_STR(u8PortNum, "TYPEC_CC1_DBCLR_EN - ", u8Data, BYTE_LEN_1, "\r\n");

    u8Data = UPD_RegReadByte (u8PortNum, TYPEC_CC2_DBCLR_EN);
    DEBUG_PRINT_PORT_UINT32_STR(u8PortNum, "TYPEC_CC2_DBCLR_EN - ", u8Data, BYTE_LEN_1, "\r\n");

    u8Data = UPD_RegReadByte (u8PortNum, TYPEC_VBUS_DBCLR_EN);
    DEBUG_PRINT_PORT_UINT32_STR(u8PortNum, "TYPEC_VBUS_DBCLR_EN - ", u8Data, BYTE_LEN_1, "\r\n");

    u8Data = UPD_RegReadByte (u8PortNum, TYPEC_CC1_SAMP_EN);
    DEBUG_PRINT_PORT_UINT32_STR(u8PortNum, "TYPEC_CC1_SAMP_EN - ", u8Data, BYTE_LEN_1, "\r\n");
    
    u8Data = UPD_RegReadByte (u8PortNum, TYPEC_CC2_SAMP_EN);
    DEBUG_PRINT_PORT_UINT32_STR(u8PortNum, "TYPEC_CC2_SAMP_EN - ", u8Data, BYTE_LEN_1, "\r\n");

    u8Data = UPD_RegReadByte (u8PortNum, TYPEC_VBUS_SAMP_EN);
    DEBUG_PRINT_PORT_UINT32_STR(u8PortNum, "TYPEC_VBUS_SAMP_EN - ", u8Data, BYTE_LEN_1, "\r\n");

    u8Data = UPD_RegReadByte (u8PortNum, TYPEC_CC_CTL1_LOW);
    DEBUG_PRINT_PORT_UINT32_STR(u8PortNum, "TYPEC_CC_CTL1_LOW - ", u8Data, BYTE_LEN_1, "\r\n");
        
    u8Data = UPD_RegReadByte (u8PortNum, TYPEC_CC_CTL1_HIGH);
    DEBUG_PRINT_PORT_UINT32_STR(u8PortNum, "TYPEC_CC_CTL1_HIGH - ", u8Data, BYTE_LEN_1, "\r\n");

    u8Data = UPD_RegReadByte (u8PortNum, TYPEC_VBUS_CTL2);
    DEBUG_PRINT_PORT_UINT32_STR(u8PortNum, "TYPEC_VBUS_CTL2 - ", u8Data, BYTE_LEN_1, "\r\n");
    
    u8Data = UPD_RegReadByte (u8PortNum, TYPEC_VBUS_CTL1);
    DEBUG_PRINT_PORT_UINT32_STR(u8PortNum, "TYPEC_VBUS_CTL1 - ", u8Data, BYTE_LEN_1, "\r\n");

    u8Data = UPD_RegReadByte (u8PortNum, TYPEC_VBUS_DEB);
    DEBUG_PRINT_PORT_UINT32_STR(u8PortNum, "TYPEC_VBUS_DEB - ", u8Data, BYTE_LEN_1, "\r\n");
 
    u8Data = UPD_RegReadByte (u8PortNum, TYPEC_DRP_CTL_LOW);
    DEBUG_PRINT_PORT_UINT32_STR(u8PortNum, "TYPEC_DRP_CTL_LOW - ", u8Data, BYTE_LEN_1, "\r\n");

    u8Data = UPD_RegReadByte (u8PortNum, TYPEC_DRP_CTL_HIGH);
    DEBUG_PRINT_PORT_UINT32_STR(u8PortNum, "TYPEC_DRP_CTL_HIGH - ", u8Data, BYTE_LEN_1, "\r\n");

    u8Data = UPD_RegReadByte (u8PortNum, TYPEC_DRP_CC_SNK_MATCH_EN);
    DEBUG_PRINT_PORT_UINT32_STR(u8PortNum, "TYPEC_DRP_CC_SNK_MATCH_EN - ", u8Data, BYTE_LEN_1, "\r\n");

    u8Data = UPD_RegReadByte (u8PortNum, TYPEC_DRP_CC_SNK_DBCLR_EN);
    DEBUG_PRINT_PORT_UINT32_STR(u8PortNum, "TYPEC_DRP_CC_SNK_DBCLR_EN - ", u8Data, BYTE_LEN_1, "\r\n");
 
    u8Data = UPD_RegReadByte (u8PortNum, TYPEC_DRP_CC_SRC_MATCH_EN);
    DEBUG_PRINT_PORT_UINT32_STR(u8PortNum, "TYPEC_DRP_CC_SRC_MATCH_EN - ", u8Data, BYTE_LEN_1, "\r\n");
    
    u8Data = UPD_RegReadByte (u8PortNum, TYPEC_DRP_CC_SRC_DBCLR_EN);
    DEBUG_PRINT_PORT_UINT32_STR(u8PortNum, "TYPEC_DRP_CC_SRC_DBCLR_EN - ", u8Data, BYTE_LEN_1, "\r\n");

    u8Data = UPD_RegReadByte (u8PortNum, TYPEC_DRP_SNK_SAMP_EN);
    DEBUG_PRINT_PORT_UINT32_STR(u8PortNum, "TYPEC_DRP_SNK_SAMP_EN - ", u8Data, BYTE_LEN_1, "\r\n");

    u8Data = UPD_RegReadByte (u8PortNum, TYPEC_DRP_SRC_SAMP_EN);
    DEBUG_PRINT_PORT_UINT32_STR(u8PortNum, "TYPEC_DRP_SRC_SAMP_EN - ", u8Data, BYTE_LEN_1, "\r\n");
    
    u8Data = UPD_RegReadByte (u8PortNum, TYPEC_DRP_DUTY_CYC);
    DEBUG_PRINT_PORT_UINT32_STR(u8PortNum, "TYPEC_DRP_DUTY_CYC - ", u8Data, BYTE_LEN_1, "\r\n");
}
/********************************************************************************************/
#endif