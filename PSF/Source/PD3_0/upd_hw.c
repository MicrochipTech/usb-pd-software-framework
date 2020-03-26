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
	UINT8 u8Data = 0;
	UPD_RegisterRead (u8PortNum, u16RegOffset, &u8Data, BYTE_LEN_1);
	u8Data |= u8BitMsk; 
	UPD_RegisterWrite (u8PortNum, u16RegOffset, &u8Data, BYTE_LEN_1);
}

void UPD_RegByteClearBit (UINT8 u8PortNum, UINT16 u16RegOffset, UINT8 u8BitMsk)
{
    UINT8 u8Data = 0;
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
    /*Enable Hardware Interface for Communication for the specific port*/
	MCHP_PSF_HOOK_UPD_COMM_ENABLE (u8PortNum, TRUE);
    
    #if (CONFIG_DEFINE_UPD350_HW_INTF_SEL == CONFIG_UPD350_SPI)
    
	UINT8 u8Command [UPD_SPI_WRITE_CMD_LEN] = {UPD_SPI_WRITE_OPCODE,
                                                HIBYTE(u16RegOffset),   /*HiByte of Register to written*/
                                                LOBYTE(u16RegOffset)};  /*LoByte of Register to written*/
	  
    
	(void)MCHP_PSF_HOOK_UPD_WRITE (u8PortNum, u8Command, \
            (UINT16)UPD_SPI_WRITE_CMD_LEN);
    
	(void)MCHP_PSF_HOOK_UPD_WRITE (u8PortNum, pu8WriteData, u8WriteDataLen);
    
    #else
    UINT8 u8Writebuffer [UPD_I2C_MAX_BYTE_WRITE];
    UINT8 u8WriteBufLen = 2;
    
    u8Writebuffer[0] = HIBYTE (u16RegOffset);
    u8Writebuffer[1] = LOBYTE (u16RegOffset);
    
    for (; u8WriteBufLen < (u8WriteDataLen+2); u8WriteBufLen++)
    {
         u8Writebuffer[u8WriteBufLen] = pu8WriteData[u8WriteBufLen - 2];
    }

    (void)MCHP_PSF_HOOK_UPD_WRITE (u8PortNum, u8Writebuffer, u8WriteBufLen);
    
    #endif
    
	MCHP_PSF_HOOK_UPD_COMM_ENABLE (u8PortNum, FALSE);
}

/******************************************************************************************************/

void UPD_RegisterReadISR(UINT8 u8PortNum, UINT16 u16RegOffset, \
        UINT8 *pu8ReadData, UINT8 u8Readlen)
{
    MCHP_PSF_HOOK_UPD_COMM_ENABLE (u8PortNum, TRUE);
  
   #if (CONFIG_DEFINE_UPD350_HW_INTF_SEL == CONFIG_UPD350_SPI)
  
    UINT8 u8Command [UPD_SPI_READ_CMD_LEN];
    
	u8Command[0] = UPD_SPI_READ_OPCODE;
	u8Command[1] = HIBYTE (u16RegOffset);
	u8Command[2] = LOBYTE (u16RegOffset);
	u8Command[3] = UPD_SPI_DUMMY_BYTE;
    
    #else

    UINT8 u8Command [UPD_I2C_REG_CMD_LEN];
    
    u8Command[0] = HIBYTE (u16RegOffset);
    u8Command[1] = LOBYTE (u16RegOffset);
    
    #endif
    
	(void)MCHP_PSF_HOOK_UPD_READ (u8PortNum, u8Command, (UINT8)sizeof(u8Command), pu8ReadData, u8Readlen);

	MCHP_PSF_HOOK_UPD_COMM_ENABLE (u8PortNum, FALSE);

}
/******************************************************************************************************/
void UPD_GPIOEnableDisable(UINT8 u8PortNum,UINT8 u8PIONum, UINT8 u8EnableDisable)
{
    if(u8EnableDisable == UPD_ENABLE_GPIO)
    {
         UPD_RegByteSetBit (u8PortNum, UPD_CFG_PIO_REGADDR(u8PIONum), UPD_CFG_PIO_GPIO_ENABLE);
    }
    else /*UPD_DISABLE_GPIO*/
    {
        UPD_RegByteClearBit (u8PortNum, UPD_CFG_PIO_REGADDR(u8PIONum), UPD_CFG_PIO_GPIO_ENABLE);
    }   

}
/******************************************************************************************************/

void UPD_GPIOSetDirection(UINT8 u8PortNum, UINT8 u8PIONum, UINT8 u8Direction)
{
  
    if(UPD_GPIO_SETDIR_OUTPUT == u8Direction)
    {
         UPD_RegByteSetBit (u8PortNum, UPD_CFG_PIO_REGADDR(u8PIONum), UPD_CFG_PIO_DIRECTION);
    }
   
    else
    {
        UPD_RegByteClearBit (u8PortNum, UPD_CFG_PIO_REGADDR(u8PIONum), UPD_CFG_PIO_DIRECTION);
    }   

}
/******************************************************************************************************/

void UPD_GPIOSetBufferType(UINT8 u8PortNum, UINT8 u8PIONum, UINT8 u8BufferType)
{
  
    if(UPD_GPIO_SETBUF_PUSHPULL == u8BufferType)
    {
         UPD_RegByteSetBit (u8PortNum, UPD_CFG_PIO_REGADDR(u8PIONum), UPD_CFG_PIO_BUFFER_TYPE);
    }
   
    else
    {
        UPD_RegByteClearBit (u8PortNum, UPD_CFG_PIO_REGADDR(u8PIONum), UPD_CFG_PIO_BUFFER_TYPE);
    }   
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

void UPD_GPIOSetClearOutput(UINT8 u8PortNum, UINT8 u8PIONum, UINT8 u8SetClear)
{
    if(UPD_GPIO_SET == u8SetClear)
    {
         UPD_RegByteSetBit (u8PortNum, (UPD_CFG_PIO_BASE + u8PIONum), UPD_CFG_PIO_DATAOUTPUT);
    }
   
    else
    {
        UPD_RegByteClearBit (u8PortNum, (UPD_CFG_PIO_BASE + u8PIONum), UPD_CFG_PIO_DATAOUTPUT);
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
    UINT32 u32Data = 0;
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
	UINT16 u16PIOIntSts = 0;
	
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
        if (!gasDPM[u8PortNum].u8HRCompleteWait)
        {
            /* Notify DPM about the power fault*/
            gasDPM[u8PortNum].u8PowerFaultISR |= DPM_POWER_FAULT_VBUS_OCS;
        }
    
		#if (FALSE == INCLUDE_UPD_PIO_OVERRIDE_SUPPORT)
			/* Disable EN_VBUS gasCfgStatusData.sPerPortData[u8PortNum].u8Pio_VBUS_EN*/
			UPD_RegisterReadISR (u8PortNum, (UPD_CFG_PIO_BASE + gasCfgStatusData.sPerPortData[u8PortNum].u8Pio_VBUS_EN),\
									(UINT8 *)&u16PIORegVal, BYTE_LEN_1);
			u16PIORegVal &= ~ UPD_CFG_PIO_DATAOUTPUT;
			UPD_RegisterWriteISR (u8PortNum, (UPD_CFG_PIO_BASE + gasCfgStatusData.sPerPortData[u8PortNum].u8Pio_VBUS_EN),\
										(UINT8 *)&u16PIORegVal, BYTE_LEN_1);
		#endif
	}
	#endif
	
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
	
	/* Get the PIO number for EN_VBUS */
	UINT8 u8PIONum = gasCfgStatusData.sPerPortData[u8PortNum].u8Pio_VBUS_EN;
	/*Setting Monitoring bit as '1' checks whether voltage exceeds the configured source 
        selection VBUS threshold value or the source selection PIO goes high */
    /* Setting Monitoring bit as '0' checks whether voltage falls below the source 
            selection VBUS threshold value or the source selection PIO goes low */
    
	/* Enable monitoring for Override 0 - Overvoltage alone */
	UPD_RegWriteByte (u8PortNum, UPD_PIO_MON_VAL, UPD_PIO_OVR_0 );
	
	/* PIO override output is set as low */
	UPD_RegWriteWord (u8PortNum, UPD_PIO_OVR_OUT, 0x0);
	
	/* Configure the Source for override 0 as OverVoltage Threshold 2*/
	UPD_RegWriteByte (u8PortNum, UPD_PIO_OVR0_SRC_SEL, \
	  (UPD_PIO_OVR_SRC_SEL_VBUS_THR | UPD_PIO_OVR_VBUS2_THR_MATCH));
    
    /* Configure the Source for override 1 as undervoltage*/
    UPD_RegWriteByte (u8PortNum, UPD_PIO_OVR1_SRC_SEL, \
	  (UPD_PIO_OVR_SRC_SEL_VBUS_THR | UPD_PIO_OVR_VBUS3_THR_MATCH));
	
	/* Configure the Source for override 2 as Fault_IN/PRT_CTL pin low from Load switch */
	UPD_RegWriteByte (u8PortNum, UPD_PIO_OVR2_SRC_SEL, gasCfgStatusData.sPerPortData[u8PortNum].u8Pio_FAULT_IN);
	
	/* VBUS_EN is configured as override Pin in output mode */
	UPD_RegWriteWord (u8PortNum, UPD_PIO_OVR_DIR, BIT(u8PIONum));
    
    /* Override 0*/
	UPD_RegWriteWord (u8PortNum, UPD_PIO_OVR0_OUT_EN, BIT(u8PIONum));
    
    /* Override 1*/
	UPD_RegWriteWord (u8PortNum, UPD_PIO_OVR1_OUT_EN, BIT(u8PIONum));

	/* Override 2*/
	UPD_RegWriteWord (u8PortNum, UPD_PIO_OVR2_OUT_EN, BIT(u8PIONum));
    
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
    
    for (UINT8 u8PortNo = 0; u8PortNo < CONFIG_PD_PORT_COUNT; u8PortNo++)
  	{
		/*Ignore if port is disabled, so consider only for enabled ports*/
		if (((gasCfgStatusData.sPerPortData[u8PortNo].u32CfgData \
            & TYPEC_PORT_ENDIS_MASK) >> TYPEC_PORT_ENDIS_POS) == UPD_PORT_ENABLED)
		{
			/*UPD_STATE_ACTIVE will be set frequently by respective Alert ISR.
			  It means that the appropriate port is active, so skip MCU IDLE*/
			if ((gau8ISRPortState[u8PortNo] == UPD_STATE_ACTIVE) ||
			/*Verify any other IDLE timer is running for all other ports.
			if its running, then lets handle in that timer expire event, so skip MCU
			IDLE for now*/
             (((gau8PortIdleTimerID[u8PortNo]< MAX_CONCURRENT_TIMERS) && (gasPDTimers[gau8PortIdleTimerID[u8PortNo]].u8TimerSt_PortNum & PDTIMER_STATE ) == PDTIMER_ACTIVE)))

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
    
    DEBUG_PRINT_PORT_STR (u8PortNum,"UPDHW: Set Port Idle");
	
	//Put MCU into IDLE

	if (FALSE == UPD_CheckUPDsActive())
	{
        DEBUG_PRINT_PORT_STR (u8PortNum,"UPDHW: Set MCU IDLE");
                
		gu8SetMCUidle = UPD_MCU_IDLE;
	}
	
}
/******************************************************************************************************/

void PD_StartIdleTimer(UINT8 u8PortNum)
{
    MCHP_PSF_HOOK_DISABLE_GLOBAL_INTERRUPT();
                
    /*if UPD350 is active; Restart UPD IDLE Timer*/
    if (gau8ISRPortState[u8PortNum] == UPD_STATE_ACTIVE)
    {
        
        PDTimer_Kill (gau8PortIdleTimerID[u8PortNum]);
        
        gau8PortIdleTimerID[u8PortNum] = MAX_CONCURRENT_TIMERS;
        
        gau8PortIdleTimerID[u8PortNum] = PDTimer_Start(\
                    CONFIG_PORT_UPD_IDLE_TIMEOUT_MS,\
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
                                        CONFIG_PORT_UPD_IDLE_TIMEOUT_MS,\
                                        UPD_SetIdleCB,\
                                        u8PortNum, \
                                        (UINT8)SET_TO_ZERO);
    
    /*Set MCU Idle flag as UPD_MCU_ACTIVE*/
    gu8SetMCUidle = UPD_MCU_ACTIVE;
}
/********************************************************************************************/
#endif /* INCLUDE_POWER_MANAGEMENT_CTRL*/
/********************************************************************************************/
void UPD_CheckAndDisablePorts (void)
{
    UINT8 u8ReadData[4];
    
    /*variable to hold the timer id*/
    UINT8 u8TimerID;
    
    /*run a loop for all the number of CONFIG_PD_PORT_COUNT to check all ports*/
    for (UINT8 u8PortNum = 0; u8PortNum < CONFIG_PD_PORT_COUNT; u8PortNum++)
    {
        /* Reset the Port's UPD350 present*/
        MCHP_PSF_HOOK_UPD_RESET_THRU_GPIO(u8PortNum);
        
        /*Check if timer is Active, if Timer expired, come out of this loop */
        
        if (((gasCfgStatusData.sPerPortData[u8PortNum].u32CfgData \
            & TYPEC_PORT_ENDIS_MASK) >> TYPEC_PORT_ENDIS_POS) == UPD_PORT_ENABLED)
        {
            /*Start 10ms timer*/
            u8TimerID = PDTimer_Start (MILLISECONDS_TO_TICKS(10), NULL, \
                                        (UINT8)SET_TO_ZERO, (UINT8)SET_TO_ZERO);
            
            while ((gasPDTimers[u8TimerID].u8TimerSt_PortNum & PDTIMER_STATE) != PDTIMER_EXPIRED)
            {
#if (CONFIG_UPD350_SPI == CONFIG_DEFINE_UPD350_HW_INTF_SEL)            
                /*Read SPI_TEST register*/
                UPD_RegisterRead(u8PortNum, (UINT16)UPD_SPI_TEST, u8ReadData, 4);
                    
                /*Check the SPI_TEST register value is 0x02*/
                if (u8ReadData[INDEX_0] == UPD_SPI_TEST_VAL)
                {
#endif
                    /*Read VID & PID register*/
                    UPD_RegisterRead(u8PortNum, (UINT16)UPD_VID, u8ReadData, 4);          
             
                    /*Verify the default values*/
                    if((u8ReadData[INDEX_0] == UPD_VID_LSB) && (u8ReadData[INDEX_1] == UPD_VID_MSB) && \
                      (u8ReadData[INDEX_2] == UPD_PID_LSB) && (u8ReadData[INDEX_3] == UPD_PID_MSB)) 
                    {  
                        /*Value read from this port is right, so enable the ports, Set SPI 
                           Communication is active for this port*/
                        gasCfgStatusData.sPerPortData[u8PortNum].u32CfgData |= \
                                (UPD_PORT_ENABLED << TYPEC_PORT_ENDIS_POS);
                        break;
                    }
                    else
                    {
                        /* If the VID and PID doesnt match, Disable the ports */
                        gasCfgStatusData.sPerPortData[u8PortNum].u32CfgData &= \
                                ~(TYPEC_PORT_ENDIS_MASK);
                        
                    }
#if (CONFIG_UPD350_SPI == CONFIG_DEFINE_UPD350_HW_INTF_SEL)            
                } /*end of UPD_SPI_TEST_VAL check if*/
                else
                {
                    gasCfgStatusData.sPerPortData[u8PortNum].u32CfgData &= \
                            ~(TYPEC_PORT_ENDIS_MASK);
                }   /*end of UPD_SPI_TEST_VAL check if else*/
#endif            
            } /* end of while*/
            /*kill the timer if the UPD is identified.*/
            PDTimer_Kill (u8TimerID);
        } /*endof if for port check*/
    } /*end of for*/
    
    /* Work around - If port-0 as source and port-1 as sink interrupt issued continuously */
    for (UINT8 u8PortNum = 0; u8PortNum < CONFIG_PD_PORT_COUNT; u8PortNum++)
  	{
        if (UPD_PORT_DISABLED == ((gasCfgStatusData.sPerPortData[u8PortNum].u32CfgData & TYPEC_PORT_ENDIS_MASK) \
            >> TYPEC_PORT_ENDIS_POS))
        {
            gasCfgStatusData.sPerPortData[u8PortNum].u32CfgData = 0;
        }
    }
}
/********************************************************************************************/

/********************************************************************************************/
void UPD_FindVBusCorrectionFactor(void)
{
    UINT16 u16VBUSTHR3 = 0;
      
    for(UINT8 u8PortNum = 0; u8PortNum < CONFIG_PD_PORT_COUNT; u8PortNum++)
    {
        if (((gasCfgStatusData.sPerPortData[u8PortNum].u32CfgData & TYPEC_PORT_ENDIS_MASK) \
            >> TYPEC_PORT_ENDIS_POS) == UPD_PORT_ENABLED)
        {
            /* Read VBUS threshold register value from OTP */    
            u16VBUSTHR3 = UPD_RegReadWord (u8PortNum, TYPEC_VBUS_THR3);
            
            gasTypeCcontrol[u8PortNum].fVBUSCorrectionFactor = \
                                    (float)((float)u16VBUSTHR3/(float)222);
        }
    }
        
}
/********************************************************************************************/


