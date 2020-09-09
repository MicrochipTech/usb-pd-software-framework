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
Copyright �  [2019-2020] Microchip Technology Inc. and its subsidiaries.

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
/******************************************************************************************************/
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
void UPD_RegisterRead(UINT8 u8PortNum, UINT16 u16RegOffset, UINT8 *pu8ReadData, \
        UINT8 u8Readlen)
{
	MCHP_PSF_HOOK_DISABLE_GLOBAL_INTERRUPT();
    
    UPD_RegisterReadISR(u8PortNum, u16RegOffset, pu8ReadData, u8Readlen);
    
    MCHP_PSF_HOOK_ENABLE_GLOBAL_INTERRUPT();
}
/******************************************************************************************************/

void UPD_RegisterWriteISR (UINT8 u8PortNum, UINT16 u16RegOffset, 
        UINT8 *pu8WriteData, UINT8 u8WriteDataLen)
{
    #if (CONFIG_DEFINE_UPD350_HW_INTF_SEL == CONFIG_UPD350_SPI)
    
	UINT8 u8Command [UPD_SPI_WRITE_CMD_LEN] = {UPD_SPI_WRITE_OPCODE,
                                                HIBYTE(u16RegOffset),   /*HiByte of Register to written*/
                                                LOBYTE(u16RegOffset)};  /*LoByte of Register to written*/
	  
    /*Enable SPI Select for communication*/
    MCHP_PSF_HOOK_GPIO_FUNC_DRIVE(u8PortNum, eSPI_CHIP_SELECT_FUNC, eGPIO_ASSERT);
    
	(void)MCHP_PSF_HOOK_UPD_WRITE (u8PortNum, u8Command, \
            (UINT16)UPD_SPI_WRITE_CMD_LEN);
    
	(void)MCHP_PSF_HOOK_UPD_WRITE (u8PortNum, pu8WriteData, u8WriteDataLen);
    
    /*Disable SPI Select communication */
    MCHP_PSF_HOOK_GPIO_FUNC_DRIVE(u8PortNum, eSPI_CHIP_SELECT_FUNC, eGPIO_DEASSERT);
    
    #else
    UINT8 u8Writebuffer [UPD_I2C_MAX_BYTE_WRITE];
    UINT8 u8WriteBufLen = 2;
    
    u8Writebuffer[INDEX_0] = HIBYTE (u16RegOffset);
    u8Writebuffer[INDEX_1] = LOBYTE (u16RegOffset);
    
    for (; u8WriteBufLen < (u8WriteDataLen+2); u8WriteBufLen++)
    {
         u8Writebuffer[u8WriteBufLen] = pu8WriteData[u8WriteBufLen - 2];
    }

    (void)MCHP_PSF_HOOK_UPD_WRITE (u8PortNum, u8Writebuffer, u8WriteBufLen);
    
    #endif
}

/******************************************************************************************************/
void UPD_RegisterReadISR(UINT8 u8PortNum, UINT16 u16RegOffset, \
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

void UPD_GPIOUpdateOutput(UINT8 u8PortNum, UINT8 u8PIONum, UINT8 u8PioMode, UINT8 u8DriveType)
{
    if ((UINT8)eUPD_PIO_UN_DEF != u8PIONum)
    {
        /*read the GPIO register*/
        UINT8 u8PioData = UPD_RegReadByte(u8PortNum, (UPD_CFG_PIO_BASE + u8PIONum));
        
        /*clear the data output in the register*/
        u8PioData &= ~UPD_CFG_PIO_DATAOUTPUT;
        
        /*get the resultant drive value*/
        u8PioData |= ((u8PioMode ^ u8DriveType) & UPD_CFG_PIO_DATAOUTPUT);
        
        /*write the value to the GPIO register*/
        UPD_RegWriteByte(u8PortNum, (UPD_CFG_PIO_BASE + u8PIONum), u8PioData);
    }
}

/******************************************************************************************************/

void UPD_GPIOSetIntrAlert (UINT8 u8PortNum, UINT8 u8PIONum, UINT8 u8IntrType)
{
	if(u8IntrType)
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
    switch(u8CountType)
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
    UPD_RegWriteByte(u8PortNum, u16RegOffset, u8CountValue);
     
}
/******************************************************************************************************/

void UPD_GPIOSetDebounce (UINT8 u8PortNum, UINT8 u8PIONum, UINT8 u8DebounceEnType)
{
    UINT32 u32Data = SET_TO_ZERO;
    if (UPD_PIO_DEBOUNCE_DISABLE == u8DebounceEnType)
    {
        
        UPD_RegisterRead (u8PortNum, UPD_PIO_DEBOUNCE_EN, (UINT8 *)&u32Data, BYTE_LEN_4);
        u32Data &= ~(((UINT32)UPD_PIO_DEBOUNCE_DISABLE_MASK) << (u8PIONum * UPD_PIO_DEBOUNCE_FIELD_WIDTH));
        UPD_RegisterWrite (u8PortNum, UPD_PIO_DEBOUNCE_EN, (UINT8 *)&u32Data, BYTE_LEN_4);

    }
    else
    {
        UPD_RegisterRead (u8PortNum, UPD_PIO_DEBOUNCE_EN, (UINT8 *)&u32Data, BYTE_LEN_4);
        u32Data |= (((UINT32)u8DebounceEnType) << (u8PIONum * UPD_PIO_DEBOUNCE_FIELD_WIDTH));
        UPD_RegisterWrite (u8PortNum, UPD_PIO_DEBOUNCE_EN, (UINT8 *)&u32Data, BYTE_LEN_4);
    }   
}
/**********************************************************************************/
#if (TRUE == INCLUDE_POWER_FAULT_HANDLING)

void UPD_FaultInInit (UINT8 u8PortNum)
{
	/* Get the PIO number*/
	UINT8 u8PIONum = gasCfgStatusData.sPerPortData[u8PortNum].u8Pio_FAULT_IN;
    
    /* Get the Fault in PIO mode*/
    UINT8 u8FaultInMode = gasCfgStatusData.sPerPortData[u8PortNum].u8Mode_FAULT_IN;
    
    UINT16 u16Data;
    
    /* Clear bits 3:0 from user input.*/
    u8FaultInMode &= (UPD_CFG_PIO_PULL_UP_ENABLE | UPD_CFG_PIO_PULL_DOWN_ENABLE | \
                        UPD_CFG_PIO_FALLING_ALERT | UPD_CFG_PIO_RISING_ALERT);
    
    /* Set direction to input and enable GPIO.*/
    u8FaultInMode |= UPD_CFG_PIO_GPIO_ENABLE;
    
    /* Write the value to the PIO config register.*/
    UPD_RegWriteByte(u8PortNum, UPD_CFG_PIO_REGADDR(u8PIONum), u8FaultInMode);
    
    /*Write Debounce count*/
    UPD_ConfigurePIODebounceCount(u8PortNum, UPD_PIO_DEBOUNCE_CNT_TYP_1_MS, gasCfgStatusData.sPerPortData[u8PortNum].u8FaultInDebounceInms);
    
    /* Enable Debounce*/
    UPD_GPIOSetDebounce (u8PortNum, u8PIONum, UPD_PIO_DEBOUNCE_CNT_TYP_1_MS);
	
    u16Data = BIT(u8PIONum);
    UPD_RegisterWrite (u8PortNum, UPD_PIO_INT_STS, (UINT8 *)&u16Data, BYTE_LEN_2);
    
	/* Enable the PIO interrupt*/
	u16Data |= UPD_RegReadWord(u8PortNum, UPD_PIO_INT_EN);
	UPD_RegWriteWord(u8PortNum, UPD_PIO_INT_EN, u16Data);
}

/*******************************************************************************/
void UPD_EnableFaultIn(UINT8 u8PortNum)
{
	UINT8 u8PIONum = gasCfgStatusData.sPerPortData[u8PortNum].u8Pio_FAULT_IN;
	UINT16 u16INTSTS = BIT(u8PIONum);
    UINT8 u8FaultInMode = gasCfgStatusData.sPerPortData[u8PortNum].u8Mode_FAULT_IN;
    
    /* Get the edge type from the user input.*/
    u8FaultInMode &= (UPD_CFG_PIO_RISING_ALERT | UPD_CFG_PIO_FALLING_ALERT);

	/* Fault-in interrupt configuration*/
	UPD_RegisterWrite (u8PortNum, UPD_PIO_INT_STS, (UINT8 *)&u16INTSTS, BYTE_LEN_2);
	UPD_GPIOSetIntrAlert (u8PortNum, u8PIONum, u8FaultInMode);
	
}

#endif /* endif of INCLUDE_POWER_FAULT_HANDLING */

/******************************************************************************************************/
void UPD_GPIOInit(UINT8 u8PortNum)
{
	/*Enable GPIO interrupt for UPD350*/
	UPD_RegWriteWord (u8PortNum, UPDINTR_INT_EN, UPDINTR_PIO_INT);
	
	#if (TRUE == INCLUDE_POWER_FAULT_HANDLING)
        /* Configure Fault Pin */
        UPD_FaultInInit(u8PortNum);
	#if (TRUE == INCLUDE_UPD_PIO_OVERRIDE_SUPPORT)
		/* Configure PIO override for fault conditions */
		(void)UPD_ConfigPwrFaultPIOOvverride(u8PortNum);
	#endif 
		
	#endif /* endif of INCLUDE_POWER_FAULT_HANDLING */
}

/******************************************************************************************************/

void UPD_PIOHandleISR(UINT8 u8PortNum)
{
	UINT16 u16PIOIntSts = SET_TO_ZERO;
	
 	/* Read the interrupt status*/
	UPD_RegisterReadISR (u8PortNum, UPD_PIO_INT_STS, (UINT8 *)&u16PIOIntSts, BYTE_LEN_2);
	
#if (TRUE == INCLUDE_POWER_FAULT_HANDLING)
	if ((BIT(gasCfgStatusData.sPerPortData[u8PortNum].u8Pio_FAULT_IN)) & u16PIOIntSts)
	{	
        UINT16 u16PIORegVal;
        /* Clear the OCS interrupt Configuration*/
        UPD_RegisterReadISR (u8PortNum, (UPD_CFG_PIO_BASE + gasCfgStatusData.sPerPortData[u8PortNum].u8Pio_FAULT_IN),\
									(UINT8 *)&u16PIORegVal, BYTE_LEN_1);
        u16PIORegVal &= ~ (UPD_CFG_PIO_FALLING_ALERT | UPD_CFG_PIO_RISING_ALERT);
		UPD_RegisterWriteISR (u8PortNum, (UPD_CFG_PIO_BASE + gasCfgStatusData.sPerPortData[u8PortNum].u8Pio_FAULT_IN),\
										(UINT8 *)&u16PIORegVal, BYTE_LEN_1);
        /*Notify Power fault to DPM only none of the Power fault recovery is not in progress*/
        if(FALSE == (gasDPM[u8PortNum].u8PowerFaultFlags & DPM_HR_COMPLETE_WAIT_MASK))
        {
            /* Notify DPM about the power fault*/
            gasDPM[u8PortNum].u8PowerFaultISR |= DPM_POWER_FAULT_VBUS_OCS;
        }
    
#if (FALSE == INCLUDE_UPD_PIO_OVERRIDE_SUPPORT)
            UINT8 u8PioNum = SET_TO_ZERO;
            UINT8 u8CurPowerRole = DPM_GET_CURRENT_POWER_ROLE(u8PortNum);
            /*When PIO override is disabled; disable EN_VBUS/EN_SINK based on the
             role on a power fault*/
            
            if(PD_ROLE_DRP != u8CurPowerRole)
            {
#if (TRUE == INCLUDE_PD_SOURCE)
                if(PD_ROLE_SOURCE == u8CurPowerRole)
                {
                    u8PioNum = gasCfgStatusData.sPerPortData[u8PortNum].u8Pio_EN_VBUS;
                }
                else
#endif
                {
#if (TRUE == INCLUDE_PD_SINK)
                    u8PioNum = gasCfgStatusData.sPerPortData[u8PortNum].u8Pio_EN_SINK;
#endif
                }
  
                UPD_RegisterReadISR (u8PortNum, (UPD_CFG_PIO_BASE + u8PioNum),\
                                        (UINT8 *)&u16PIORegVal, BYTE_LEN_1);
                u16PIORegVal &= ~ UPD_CFG_PIO_DATAOUTPUT;
                UPD_RegisterWriteISR (u8PortNum, (UPD_CFG_PIO_BASE + u8PioNum),\
                                            (UINT8 *)&u16PIORegVal, BYTE_LEN_1);  
            }
            else
            {
                /*Execution should not hit here ideally*/
            }
#endif /*(FALSE == INCLUDE_UPD_PIO_OVERRIDE_SUPPORT)*/
	}
#endif /*INCLUDE_POWER_FAULT_HANDLING*/
	
	/* clear the interrupt status */
	UPD_RegisterWriteISR (u8PortNum, UPD_PIO_INT_STS, (UINT8 *)&u16PIOIntSts, BYTE_LEN_2);
}
/******************************************************************************************************/

#if (TRUE == INCLUDE_UPD_PIO_OVERRIDE_SUPPORT)

void UPD_ConfigPwrFaultPIOOvverride (UINT8 u8PortNum)
{
	/* Override 0 - Overvoltage Threshold*/
    /* Override 1 - UnderVoltage Threshold */
  	/* Override 2 - Fault Low*/
    UINT16 u16PIOPos;
    
#if (TRUE == INCLUDE_PD_DRP)   
    UINT8 u8PIOEnVBUS;
    UINT8 u8PIOEnSink;
    u8PIOEnVBUS = gasCfgStatusData.sPerPortData[u8PortNum].u8Pio_EN_VBUS;
    u8PIOEnSink = gasCfgStatusData.sPerPortData[u8PortNum].u8Pio_EN_SINK;
    u16PIOPos = (BIT(u8PIOEnVBUS) | BIT(u8PIOEnSink));
#elif (TRUE == INCLUDE_PD_SOURCE)
    UINT8 u8PIOEnVBUS;
    u8PIOEnVBUS = gasCfgStatusData.sPerPortData[u8PortNum].u8Pio_EN_VBUS;
    u16PIOPos = BIT(u8PIOEnVBUS);
#else
    UINT8 u8PIOEnSink;
    u8PIOEnSink = gasCfgStatusData.sPerPortData[u8PortNum].u8Pio_EN_SINK;
    u16PIOPos = BIT(u8PIOEnSink);
#endif

    /*Setting Monitoring bit as '1' checks whether voltage exceeds the configured source 
        selection VBUS threshold value or the source selection PIO goes high */
    /* Setting Monitoring bit as '0' checks whether voltage falls below the source 
            selection VBUS threshold value or the source selection PIO goes low */
    /* Enable monitoring for Override 0 - Overvoltage alone */
    UPD_RegWriteByte (u8PortNum, UPD_PIO_MON_VAL, UPD_PIO_OVR_0);

    /* PIO override output is set as low */
    UPD_RegWriteWord (u8PortNum, UPD_PIO_OVR_OUT, SET_TO_ZERO);

    /* Configure the Source for override 0 as OverVoltage Threshold 2*/
    UPD_RegWriteByte (u8PortNum, UPD_PIO_OVR0_SRC_SEL, \
      (UPD_PIO_OVR_SRC_SEL_VBUS_THR | UPD_PIO_OVR_VBUS2_THR_MATCH));

    /* Configure the Source for override 1 as under-voltage*/
    UPD_RegWriteByte (u8PortNum, UPD_PIO_OVR1_SRC_SEL, \
      (UPD_PIO_OVR_SRC_SEL_VBUS_THR | UPD_PIO_OVR_VBUS3_THR_MATCH));

    /* Configure the Source for override 2 as Fault_IN/PRT_CTL pin low from Load switch */
    UPD_RegWriteByte (u8PortNum, UPD_PIO_OVR2_SRC_SEL, gasCfgStatusData.sPerPortData[u8PortNum].u8Pio_FAULT_IN);

    /* EN_VBUS is configured as override Pin in output mode */
    UPD_RegWriteWord (u8PortNum, UPD_PIO_OVR_DIR, u16PIOPos);

    /* Override 0*/
    UPD_RegWriteWord (u8PortNum, UPD_PIO_OVR0_OUT_EN, u16PIOPos);

    /* Override 1*/
    UPD_RegWriteWord (u8PortNum, UPD_PIO_OVR1_OUT_EN, u16PIOPos);

    /* Override 2*/
    UPD_RegWriteWord (u8PortNum, UPD_PIO_OVR2_OUT_EN, u16PIOPos);

    /* Enable the override for FAULT_IN*/
    /* Under voltage and overvoltage override is enabled after configuring the
        threshold*/
    UPD_RegByteSetBit (u8PortNum, UPD_PIO_OVR_EN,  UPD_PIO_OVR_2);

}
#endif

/******************************************************************************************************/

#if (TRUE == INCLUDE_POWER_MANAGEMENT_CTRL)

UINT8 UPD_CheckUPDsActive()
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
	//TBD
	
	/*Disable Ring , 48 MHz Oscillator and System clock to put UPD350 to idle*/
	UPD_RegByteClearBit (u8PortNum, UPD_CLK_CTL , \
            (UPD_RING_OSC_ENABLE | UPD_48MHZ_OSC_ENABLE | UPD_SYS_CLK_ENABLE) );
    
    DEBUG_PRINT_PORT_STR (u8PortNum,"UPDHW: Set Port Idle\r\n");
	
	//Put MCU into IDLE

	if (FALSE == UPD_CheckUPDsActive())
	{
        DEBUG_PRINT_PORT_STR (u8PortNum,"UPDHW: Set MCU IDLE\r\n");
                
		gu8SetMCUidle = UPD_MCU_IDLE;
	}
	
}
/******************************************************************************************************/

void PD_StartIdleTimer(UINT8 u8PortNum)
{
    MCHP_PSF_HOOK_DISABLE_GLOBAL_INTERRUPT();
                
    /*if UPD350 is active; Restart UPD IDLE Timer*/
    if (UPD_STATE_ACTIVE == gau8ISRPortState[u8PortNum])
    {        
        PDTimer_Kill (gau8PortIdleTimerID[u8PortNum]);
        
        gau8PortIdleTimerID[u8PortNum] = MAX_CONCURRENT_TIMERS;
        
        gau8PortIdleTimerID[u8PortNum] = PDTimer_Start(\
                    UPD_IDLE_TIMEOUT_MS,\
                    UPD_SetIdleCB,\
                    u8PortNum, \
                    (UINT8)SET_TO_ZERO);
                    
        gau8ISRPortState[u8PortNum] = UPD_STATE_STARTED_IDLE_TIMER;
    }
    
    MCHP_PSF_HOOK_ENABLE_GLOBAL_INTERRUPT();
} 

/******************************************************************************************************/

/********************************************************************************************/
void UPD_PwrManagementCtrl(UINT8 u8PortNum)
{
   /*Restart IDLE Timer if UPD350 is Active*/
    PD_StartIdleTimer (u8PortNum);

    if (UPD_MCU_IDLE == gu8SetMCUidle)
    {
        /*Invalidate MCU Idle, Before keeping MCU in Idle clear this set 
         gu8SetMCUidle to Active state for next run */
        gu8SetMCUidle = UPD_MCU_ACTIVE;

        if (FALSE == UPD_CheckUPDsActive())
        {
            /* Notification CallBack to the Client, to indicate all the
             UPD350s are in idle state */
            (void)DPM_NotifyClient(u8PortNum, eMCHP_PSF_UPDS_IN_IDLE);
        }
    } 
}
/********************************************************************************************/

void UPD_PwrManagementInit(UINT8 u8PortNum)
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
    gu8SetMCUidle = UPD_MCU_ACTIVE;
}
/********************************************************************************************/
#endif /* INCLUDE_POWER_MANAGEMENT_CTRL*/
/********************************************************************************************/
void UPD_ResetThroughGPIO()
{
    /*Since, all UPD350 PIOs are tied to single PIO, Reset is done for PORT0
     alone to avoid multiple reset, PORT0 is passed as dummy variable */
    /* Pull down is driven to reset the UPD350*/
    MCHP_PSF_HOOK_GPIO_FUNC_DRIVE(PORT0, eUPD350_RESET_FUNC,eGPIO_ASSERT);

    /* Set to default state*/
    MCHP_PSF_HOOK_GPIO_FUNC_DRIVE(PORT0, eUPD350_RESET_FUNC,eGPIO_DEASSERT);
}

void UPD_CheckAndDisablePorts (void)
{
    UINT8 u8ReadData[BYTE_LEN_4];
    
    /*variable to hold the timer id*/
    UINT8 u8TimerID;
    
     /* Reset the Port's UPD350 present*/
    UPD_ResetThroughGPIO();

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
                UPD_RegisterRead(u8PortNum, (UINT16)UPD_SPI_TEST, u8ReadData, BYTE_LEN_4);
                    
                /*Check the SPI_TEST register value is 0x02*/
                if (UPD_SPI_TEST_VAL == u8ReadData[INDEX_0])
                {
#endif
                    /*Read VID & PID register*/
                    UPD_RegisterRead(u8PortNum, (UINT16)UPD_VID, u8ReadData, BYTE_LEN_4);          
             
                    /*Verify the default values*/
                    if((UPD_VID_LSB == u8ReadData[INDEX_0]) && (UPD_VID_MSB == u8ReadData[INDEX_1]) && \
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

/********************************************************************************************/
void UPD_FindVBusCorrectionFactor(void)
{
    UINT16 u16VBUSTHR3 = SET_TO_ZERO;
      
    for(UINT8 u8PortNum = SET_TO_ZERO; u8PortNum < CONFIG_PD_PORT_COUNT; u8PortNum++)
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
/********************************************************************************************/

#if(TRUE == CONFIG_HOOK_DEBUG_MSG)
/********************************************************************************************/
void UPD_RegDump(UINT8 u8PortNum)
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
