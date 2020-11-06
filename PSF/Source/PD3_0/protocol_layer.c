/*******************************************************************************
 Protocol Layer Source file

  Company:
    Microchip Technology Inc.

  File Name:
    protocol_layer.c

  Description:
    This header file contains the function definition of interface to configure 
    UPD HW to transmit and receive PD message,UPD HW Physical layer configuration, Chunking state machine
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

/***************************************************************************************************/

/***************************************************************************************************/
/* DAC Values to be programmed to PRL_BB_TX_RISE0 to PRL_BB_TX_RISE11 Registers */
const UINT16 u16aBBTxRiseDACValues [] = {
										PRL_BB_TX_RISE0_REG_VALUE,
										PRL_BB_TX_RISE1_REG_VALUE,								
										PRL_BB_TX_RISE2_REG_VALUE,
										PRL_BB_TX_RISE3_REG_VALUE,
										PRL_BB_TX_RISE4_REG_VALUE,
										PRL_BB_TX_RISE5_REG_VALUE,
										PRL_BB_TX_RISE6_REG_VALUE,
										PRL_BB_TX_RISE7_REG_VALUE,
										PRL_BB_TX_RISE8_REG_VALUE,
										PRL_BB_TX_RISE9_REG_VALUE,
										PRL_BB_TX_RISE10_REG_VALUE,
										PRL_BB_TX_RISE11_REG_VALUE,
										};

/***************************************************************************************************/

/* DAC Values to be programmed to PRL_BB_TX_FALL0 to PRL_BB_TX_FALL11 Registers */
const UINT16 u16aBBTxFallDACValues [] = {
										PRL_BB_TX_FALL0_REG_VALUE, 
										PRL_BB_TX_FALL1_REG_VALUE,
										PRL_BB_TX_FALL2_REG_VALUE,
										PRL_BB_TX_FALL3_REG_VALUE,
										PRL_BB_TX_FALL4_REG_VALUE,
										PRL_BB_TX_FALL5_REG_VALUE,
										PRL_BB_TX_FALL6_REG_VALUE,
										PRL_BB_TX_FALL7_REG_VALUE,
										PRL_BB_TX_FALL8_REG_VALUE,
										PRL_BB_TX_FALL9_REG_VALUE,
										PRL_BB_TX_FALL10_REG_VALUE,
										PRL_BB_TX_FALL11_REG_VALUE,
										};

/***************************************************************************************************/

/* BMC Encoder Decoder Register values  to br programmed to register PRL_BMC_RX_HI_FB_MAX_TIME to
	PRL_BMC_RX_SQL_HOLD_TIME */
const UINT8 u8aBMCEncoderRegValues [] = {
  										PRL_BMC_RX_HI_FB_MAX_TIME_REG_VALUE,	
										PRL_BMC_RX_HI_FB_MIN_TIME_REG_VALUE,
										PRL_BMC_RX_LO_FB_MAX_TIME_REG_VALUE,
										PRL_BMC_RX_LO_FB_MIN_TIME_REG_VALUE,
										PRL_BMC_RX_HI_HB_MAX_TIME_REG_VALUE,
										PRL_BMC_RX_HI_HB_MIN_TIME_REG_VALUE,
										PRL_BMC_RX_LO_HB_MAX_TIME_REG_VALUE,
										PRL_BMC_RX_LO_HB_MIN_TIME_REG_VALUE,
										PRL_BMC_RX_SQL_ASSERT_TIME_REG_VALUE,			
										PRL_BMC_RX_SQL_HOLD_TIME_REG_VALUE,	
										};
										

/***************************************************************************************************/

/***************************************************************************************************/
void  PRL_Init (UINT8 u8PortNum)
{
  	UINT16 u16RxDACValue = PRL_BB_CC_RX_DAC_CTL_CC_RX_DAC_NEU_VALUE;
    UINT8 u8CurrentPwrRole = DPM_GET_CURRENT_POWER_ROLE(u8PortNum);
	
	/* Protocol Tx PHY layer is Reset */
	PRL_PHYLayerReset (u8PortNum);

	/* Tx configurations */
	/* Configure Tx Power up & down time */
	UPD_RegWriteWord (u8PortNum, PRL_TX_POWER_UP_TIME, 
					  MAKE_UINT16 (PRL_TX_POWER_DOWN_TIME_REG_VALUE, PRL_TX_POWER_UP_TIME_REG_VALUE));

	/* Configuring bit time & turn around time  */
	UPD_RegWriteWord (u8PortNum, PRL_TX_BITTIME_CNT, 
					  MAKE_UINT16 (PRL_TX_TA_TIME_REG_VALUE, PRL_TX_BITTIME_CNT_REG_VALUE));

	/* PRL_TX_PARAM_B Preamble is left to default value 0x40 - 64d */

	/* configure the TX_CTL_A register Enable Auto response mode and retry on Line busy */
	UPD_RegWriteByte (u8PortNum, PRL_TX_CTL_A, 
					  (PRL_TX_CTL_A_RETRY_ON_LINE_BUSY | PRL_TX_CTL_A_DIS_SPCL_SR_GCRC_ACK | PRL_TX_CTL_A_EN_AUTO_RSP_MODE));
	
	/* Updates Device's Data & Power role, UPD Spec rev & corresponding retry counter*/
	PRL_UpdateSpecAndDeviceRoles (u8PortNum);
	
	
	/* Rx configurations */
	/* Set up Rx */
	/* Configuring Rx Maximum and Minimum Bit-Rate Bit Period Count Registers */
	UPD_RegWriteWord (u8PortNum, PRL_RX_BIT_PER_CNT_MAX_BR, 
					  MAKE_UINT16 (PRL_RX_BIT_PER_CNT_MIN_BR_TIME_VALUE, PRL_RX_BIT_PER_CNT_MAX_BR_TIME_VALUE));

	/* Configuring CRCReceiveTimer timeout value (tReceive) in terms of 10’s of micro seconds */
	UPD_RegWriteByte (u8PortNum, PRL_RX_TRECEIVE_TIME, PRL_RX_TRECEIVE_TIME_VALUE_IN_10US_UNITS);

	/* Configuring Value of Hard and Cable Reset detection window 
		(maximum packet bit count including preamble) in number of bits.*/
	UPD_RegWriteByte (u8PortNum, PRL_RX_HR_DET_WINDOW, PRL_RX_HR_DET_WINDOW_BITS_VALUE);

	/* BMC encoding configuration */
	UPD_RegisterWrite (u8PortNum, PRL_BMC_RX_HI_FB_MAX_TIME, 
					  (UINT8 *) u8aBMCEncoderRegValues, sizeof(u8aBMCEncoderRegValues));
	UPD_RegWriteWord (u8PortNum, PRL_BMC_TX_BITTIME_CNT, 
					  MAKE_UINT16(PRL_BMC_TRANSITION_WINDOW_TIME_REG_VALUE, PRL_BMC_TX_BITTIME_CNT_REG_VALUE));
	
	/* BB Tx Rise fall configuration */
	UPD_RegisterWrite (u8PortNum, PRL_BB_TX_RISE0, 
					   (UINT8 *) u16aBBTxRiseDACValues, sizeof(u16aBBTxRiseDACValues));

	UPD_RegisterWrite (u8PortNum, PRL_BB_TX_FALL0, 
					   (UINT8 *) u16aBBTxFallDACValues, sizeof(u16aBBTxFallDACValues));
	
	
	/* Baseband Configuration for Rx DAC */
	UPD_RegWriteByte (u8PortNum, PRL_BB_CC_RX_DAC_FILT, PRL_CC_RX_DAC_FILT_CC_RX_DAC_FILTER_ENABLE);
    
    /* Rx DAC value is selected depending upon the Power Role */
	if (PD_ROLE_SOURCE == u8CurrentPwrRole)
	{
		u16RxDACValue = PRL_BB_CC_RX_DAC_CTL_CC_RX_DAC_SRC_VALUE;
	}
	else if (PD_ROLE_SINK == u8CurrentPwrRole)
	{
		u16RxDACValue = PRL_BB_CC_RX_DAC_CTL_CC_RX_DAC_SNK_VALUE;
	}
    else
    {
        /* Do Nothing */
    }
	
	UPD_RegWriteWord (u8PortNum, PRL_BB_CC_RX_DAC_CTL, 
					  (PRL_CC_RX_DAC_CTL_RX_DAC_ENABLE | u16RxDACValue));


	/* Tx DAC is filter is enabled */
	UPD_RegWriteByte (u8PortNum, PRL_BB_CC_TX_DAC_FILT, PRL_CC_TX_DAC_FILT_CC_TX_FILTER_ENABLE);

	/* PD3_AUTO_DECODE is enabled so that HW decodes spec revision from received messages.*/
	/* At init, Rx SOP type is set to all SOP* type*/
	if(PD_ROLE_SOURCE == u8CurrentPwrRole)
    {	  
		UPD_RegWriteByte (u8PortNum, PRL_RX_CTL_B, 
						  (PRL_RX_CTL_B_PD3_AUTO_DECODE | PRL_RX_CTL_B_RX_SOP_ENABLE_SOP |
						   PRL_RX_CTL_B_RX_SOP_ENABLE_SOP_P | PRL_RX_CTL_B_RX_SOP_ENABLE_SOP_PP));
	}
	else
	{
		UPD_RegWriteByte (u8PortNum, PRL_RX_CTL_B, 
						  (PRL_RX_CTL_B_PD3_AUTO_DECODE | PRL_RX_CTL_B_RX_SOP_ENABLE_SOP));
	}

	/* Enabling PD message Filtering */
	UPD_RegByteSetBit (u8PortNum, TYPEC_CC_HW_CTL_HIGH, TYPEC_BLK_PD_MSG);

	/* Clear Rx Interrupt Src for HR and FIFO not empty */
	UPD_RegWriteByte (u8PortNum, PRL_RX_IRQ_STAT, 
					  (PRL_RX_IRQ_RX_HARD_RST | PRL_RX_IRQ_RX_FIFO_NOT_EMPTY));

	/* Enable Rx interrupts for HR and FIFO not empty */
	UPD_RegWriteByte (u8PortNum, PRL_RX_IRQ_EN, PRL_RX_IRQ_RX_FIFO_NOT_EMPTY);
	
	/* Clear Tx Interrupt Src */
	UPD_RegWriteByte (u8PortNum, PRL_TX_IRQ_STAT, 
					  (PRL_TX_IRQ_TX_DONE | PRL_TX_IRQ_TX_ABORTED | PRL_TX_IRQ_TX_FAILED));
	/* Enable the Tx interrupts */
	UPD_RegWriteByte (u8PortNum, PRL_TX_IRQ_EN, 
					  (PRL_TX_IRQ_TX_DONE | PRL_TX_IRQ_TX_ABORTED | PRL_TX_IRQ_TX_FAILED));
	
	/* Enabling UPD_SNIFFER  */
	UPD_RegWriteByte (u8PortNum, PRL_PD_SNIFFER_UPD_SNIFF_CTL, PRL_UPD_SNIFF_CTL_UPD_SNIFFER_ENABLE);

	/* Enable Wake up Enable */
	UPD_RegWriteByte (u8PortNum, PRL_PM_IRQ_EN, PRL_PM_IRQ_UPD_WU_EN);

	/* Enable Mac Overall Interrupt */
	UPD_RegByteSetBit (u8PortNum, UPDINTR_INT_EN, UPDINTR_MAC_INT);
	
	/* Initialize gasPRL globals*/
	gasPRL[u8PortNum].u32PkdPEstOnTxStatus = CLR_VAL;
	gasPRL[u8PortNum].pFnTxCallback = NULL;
	gasPRL[u8PortNum].u8TxStateISR = PRL_TX_IDLE_ST;
	gasPRL[u8PortNum].u8SOPMsgID = RESET_TO_ZERO;
	gasPRL[u8PortNum].u8SOP_P_MsgID = RESET_TO_ZERO;
	gasPRL[u8PortNum].u8SOP_PP_MsgID = RESET_TO_ZERO;
    
	/* Rx u8RxRcvdISR & u8RxHRRcvdISR are reset*/
    gasPRL[u8PortNum].u8RxRcvdISR = FALSE;
    gasPRL[u8PortNum].u8RxHRRcvdISR = FALSE;
	
	/* Rx Error bit is reset */
	gasPRL [u8PortNum].u8RxError = RESET_TO_ZERO;
	
    #if  (TRUE == INCLUDE_PD_3_0)
	/* Chunk SM is reset */
    gasChunkSM [u8PortNum].u8CAorChunkSMTimerID = MAX_CONCURRENT_TIMERS;
	PRL_ResetChunkSM (u8PortNum);
    #endif
    
    DEBUG_PRINT_PORT_STR (u8PortNum,"PRL: Initialization Done\r\n");
}


/***************************************************************************************************/

/***************************************************************************************************/
void PRL_UpdateSpecAndDeviceRoles (UINT8 u8PortNum)
{
  	UINT8 u8HwnRetryCount;
	
	/* HW Retry Counter is updated depending on Spec Rev */
	if(PD_SPEC_REVISION_2_0 == DPM_GET_CURRENT_PD_SPEC_REV(u8PortNum))
	{
		u8HwnRetryCount = PRL_HW_RETRY_CNT_2_0;
	}
	else
	{
		u8HwnRetryCount = PRL_HW_RETRY_CNT_3_0;
	}

	UPD_RegWriteByte (u8PortNum, PRL_TX_PARAM_C, 
					  	PRL_TX_PARAM_C_UPD_SPEC_REV_2_0 							|	 			/*	Spec Rev */
						PRL_UPDATE_TX_PARAM_C_N_RETRY_CNT(u8HwnRetryCount)			| 				/* nRetryCount corresponding to spec */
						PRL_UPDATE_TX_PARAM_C_PORT_DATA_ROLE(DPM_GET_CURRENT_DATA_ROLE(u8PortNum) ) |	/* Data Role*/
						PRL_UPDATE_TX_PARAM_C_PORT_POWER_ROLE(DPM_GET_CURRENT_POWER_ROLE(u8PortNum) )); 	/* Power Role*/	
}

/***************************************************************************************************/

UINT16 PRL_FormSOPTypeMsgHeader (UINT8 u8PortNum, UINT8 u8MessageType, UINT8 u8ObjectCount, UINT8 u8Extended)
{
	return((u8MessageType)
		   |((UINT16)(DPM_GET_CURRENT_DATA_ROLE(u8PortNum)) << PRL_PORT_DATA_ROLE_BIT_POS)
		   |((UINT16)(DPM_GET_CURRENT_PD_SPEC_REV(u8PortNum)) << PRL_SPEC_REV_FIELD_START_BIT_POS) 		
		   |((UINT16)(DPM_GET_CURRENT_POWER_ROLE(u8PortNum)) << PRL_PORT_POWER_ROLE_OR_CABLE_PLUG_BIT_POS)
		   |((UINT16)u8ObjectCount << PRL_DATA_OBJECTS_FIELD_START_BIT_POS) 						
		   |((UINT16)u8Extended << PRL_EXTENDED_BIT_POS));
}

UINT16 PRL_FormNonSOPTypeMsgHeader (UINT8 u8PortNum, UINT8 u8MessageType, UINT8 u8ObjectCount, UINT8 u8Extended)
{
	return((u8MessageType) 																		|
  			((UINT16) ((DPM_GET_CURRENT_PD_SPEC_REV(u8PortNum)) << PRL_SPEC_REV_FIELD_START_BIT_POS)) 	|
	  		((UINT16)u8ObjectCount << PRL_DATA_OBJECTS_FIELD_START_BIT_POS) 					|
			((UINT16)u8Extended << PRL_EXTENDED_BIT_POS));
}

/***************************************************************************************************/

UINT8 PRL_TransmitMsg (UINT8 u8PortNum, UINT8 u8SOPType, UINT32 u32Header, UINT8 *pu8DataBuffer, 
					   PRLTxCallback pfnTxCallback, UINT32  u32PkdPEstOnTxStatus)
{
  
	UINT8 u8MsgId, u8PktLen, au8TxPkt [PRL_MAX_PD_LEGACY_PKT_LEN], u8OKToTx, u8TxSOPSelect = SET_TO_ZERO;
    /* PD3_Auto_Decode and RX_SOP_ENABLE_SOP are enabled by default */
    UINT8 u8RxCtlBRegVal = (PRL_RX_CTL_B_PD3_AUTO_DECODE | PRL_RX_CTL_B_RX_SOP_ENABLE_SOP); 
    UINT8 u8HwnRetryCount = SET_TO_ZERO; 
            
    #if (TRUE == INCLUDE_PD_3_0)
    UINT16 u16MsgDataIndex;
	
	/* Checks whether the message is extended PD packet & chunk state machine is enabled*/
	if ((!gasChunkSM [u8PortNum].u8EnableChunkSM) && (PRL_IS_EXTENDED_MSG(u32Header)))
	{
	  	/* Spec Ref: TCH_Prepare_To_Send_Chunked_Message - "Chunk Number To Send = 0" */
	  
	  	/* Extended message is copied to global buffer & Chunk SM is enabled*/
	  
	  	/* Copying SOP type*/
	  	gasExtendedMsgBuff [u8PortNum].u8SOPtype = u8SOPType;
		
		/* Message Header is stored */
		gasExtendedMsgBuff [u8PortNum].u16Header = PRL_GET_MSG_HEADER(u32Header);
		
		/* ChunkNumber is set to zero*/
		gasChunkSM [u8PortNum].u8ChunkNumExpectedOrSent = RESET_TO_ZERO;
		
		/* PE will specify the data size alone. Other Extended Message Header fields are populated by PRL.
		Request Chunk bit is set as 0 indicating Response chunk.
		Chunked bit is enabled as unchunked extended messages is not supported in UPD.
		Response Chunk Extended Message header is formed using 
		PRL_FORM_RESPONSE_CHUNK_EXT_MSG_HEADER with chunk Number & Data Size parameter and it is stored.
		*/
		gasExtendedMsgBuff [u8PortNum].u16ExtendedMsgHeader = PRL_FORM_RESPONSE_CHUNK_EXT_MSG_HEADER (
																	gasChunkSM [u8PortNum].u8ChunkNumExpectedOrSent,
																 	PRL_GET_DATA_SIZE (PRL_GET_EXTENDED_MSG_HEADER(u32Header))
																   	);
		
		/* if pu8DataBuffer is not NULL*/
		if (pu8DataBuffer != NULL)
		{
			/* copying message data*/
			for (u16MsgDataIndex = SET_TO_ZERO; 
					u16MsgDataIndex <= PRL_GET_DATA_SIZE(gasExtendedMsgBuff [u8PortNum].u16ExtendedMsgHeader); 
						u16MsgDataIndex++)
			{
				
				gasExtendedMsgBuff [u8PortNum].u8Data [u16MsgDataIndex] =  pu8DataBuffer [u16MsgDataIndex];
			}
		}
		
		
		/* Copying Callback*/
		gasChunkSM [u8PortNum].u32pkdTmrID_TxSt = u32PkdPEstOnTxStatus;
		gasChunkSM [u8PortNum].pFnTxCallback = pfnTxCallback;
		
		/* Total Chunk packet to be sent is updated*/
		PRL_UpdateTotalChunkNumVar (u8PortNum);
		
		/* Assigning Chunk state to PRL_TCH_SEND_RESPONSE_CHUNK_ST */
		gasChunkSM [u8PortNum].u8ChunkState	= PRL_TCH_SEND_RESPONSE_CHUNK_ST;												
		
	  	/* Chunk state machine is enabled*/
		gasChunkSM [u8PortNum].u8EnableChunkSM = TRUE;
		DEBUG_PRINT_PORT_STR (u8PortNum, "PRL: Chunk SM Enabled\r\n");
		
		return PRL_RET_TX_MSG_CHUNKING_ENABLED;
	}
    
    #endif
	
	/* update the PDMAC globals */
	gasPRL[u8PortNum].u32PkdPEstOnTxStatus = u32PkdPEstOnTxStatus;
	gasPRL[u8PortNum].pFnTxCallback = pfnTxCallback;

    /* Enable RX_SOP_ENABLE_SOP_P and RX_SOP_ENABLE_SOP_PP based on the type of 
       packet transmitted */
    if (PRL_SOP_P_TYPE == u8SOPType)
    {   
        u8RxCtlBRegVal |= PRL_RX_CTL_B_RX_SOP_ENABLE_SOP_P;
    }
    else if (PRL_SOP_PP_TYPE == u8SOPType)
    {
        u8RxCtlBRegVal |= PRL_RX_CTL_B_RX_SOP_ENABLE_SOP_PP;
    }
    else 
    {
        /* Do Nothing */
    }
	/* Update the RX_CTL_B PD MAC register */
    UPD_RegWriteByte (u8PortNum, PRL_RX_CTL_B, u8RxCtlBRegVal);
    
    /* Set the Hardware retry count as 0 for VDMs */
    if (PE_DATA_VENDOR_DEFINED != PRL_GET_MESSAGE_TYPE(u32Header))
    {
        if(PD_SPEC_REVISION_2_0 == DPM_GET_CURRENT_PD_SPEC_REV(u8PortNum))
        {
            u8HwnRetryCount = PRL_HW_RETRY_CNT_2_0;
        }
        else
        {
            u8HwnRetryCount = PRL_HW_RETRY_CNT_3_0;
        }
    }
    PRL_UpdateHWRetryCount(u8PortNum, u8HwnRetryCount);
    
	/* Depending on the Packet type; MessageID is updated in the Pkt Header and 
		PD_MAC Reg is updated for Pkt type */
	switch (u8SOPType)
	{
		case PRL_SOP_TYPE:
		{
			u8MsgId = gasPRL[u8PortNum].u8SOPMsgID;
			u8TxSOPSelect = PRL_TX_PARAM_A_TX_SOP_SELECT_SOP;
			break;
		}

		case PRL_SOP_P_TYPE:
		{
			u8MsgId = gasPRL[u8PortNum].u8SOP_P_MsgID;
			u8TxSOPSelect = PRL_TX_PARAM_A_TX_SOP_SELECT_SOP_P;
			break;
			  
		}

		case PRL_SOP_PP_TYPE:
		{
			u8MsgId = gasPRL[u8PortNum].u8SOP_PP_MsgID;
			u8TxSOPSelect = PRL_TX_PARAM_A_TX_SOP_SELECT_SOP_PP;
			break;
		}
		  
		default:
		{
			u8MsgId = SET_TO_ZERO;
			DEBUG_PRINT_PORT_STR (u8PortNum,"Wrong SOP* type detected\r\n");
			break;
		}
	}

	/* MessageID is updated to header */
	u32Header |= (u8MsgId << PRL_MESSAGE_ID_FIELD_START_BIT_POS);
	
	/* Spec Ref: PRL_Tx_Construct_Message - Construct message
									Pass message to PHY Layer*/
	/* PD message is constructed */
	u8PktLen = PRL_BuildTxPacket (u8PortNum, u32Header, pu8DataBuffer, au8TxPkt);  
	
	/* Enable the interrupt for Tx_DONE, Tx_ABORT & Tx_FAILED */
	UPD_RegWriteByte (u8PortNum, PRL_TX_IRQ_EN, (PRL_TX_IRQ_TX_DONE | PRL_TX_IRQ_TX_ABORTED | PRL_TX_IRQ_TX_FAILED));
	
	/* PD Packet is copied to PD_MAC HW Buffer */
	UPD_RegisterWrite (u8PortNum, PRL_PDMAC_TRANSMITTER_BUFF_ADDR, au8TxPkt, u8PktLen);
	
	/* Update the PD Pkt length to PD_MAC HW Reg */
	UPD_RegWriteByte(u8PortNum, PRL_TX_PKT_LEN, u8PktLen);
	
	/* Update the Tx Param reg */
	UPD_RegWriteByte (u8PortNum, PRL_TX_PARAM_A, (PRL_TX_PARAM_A_EXPECT_GOODCRC | PRL_TX_PARAM_A_EN_FW_TX | u8TxSOPSelect | u8MsgId));
	
    #if (TRUE == INCLUDE_PD_3_0)
    /* If a Soft_Reset Message is pending, Needn't wait for Rp is set to SinkTxOk.*/
	/* Tx Buffering on CA*/
	if ((PRL_Tx_CA_SRC_SINKTXTIMER_ON_ST == gasPRL [u8PortNum].u8TxStateISR) || \
            ((PRL_TX_CA_SINK_TXNG_ST == gasPRL [u8PortNum].u8TxStateISR) && \
                (PRL_GET_MESSAGE_TYPE(u32Header) != PE_CTRL_SOFT_RESET)))
	{	  
		/* if Timer SinkTxTimer is ON, Tx Message is just buffered in Tx_FIFO by not setting Go bit*/
		PRL_ChangeTxState (u8PortNum, PRL_TX_MSG_BUFFERED_ON_CA_ST);
        
        DEBUG_PRINT_PORT_STR (u8PortNum,"PRL_TX_MSG_BUFFERED_ON_CA: Tx Msg Buffered due to CA\r\n");
        
		return PRL_RET_TX_MSG_BUFFERED;
	}
    #endif
	
	/* Tx_Discard handling*/
	
	if ((gasPRL[u8PortNum].u8RxRcvdISR) &&                			/* Checks whether a message is received */
            (PRL_SOP_TYPE == gasPRLRecvBuff[u8PortNum].u8SOPtype))     /* Checks whether received message is PRL_SOP_TYPE type*/
	{
	  	/* Spec Ref: PRL_Tx_Discard_Message - If any message is currently awaiting tranmission discard and Increment MessageID Counter
									It is entered on "Protocol Layer message reception in PRL_RX_STORE_MESSAGEID state" */
	  
	  	/* According to Message Discarding Rules, if SOP packet is received. Tx Packet in queue is discarded*/
	  	/* And Message ID Counter is incremented*/
	  	PRL_IncrementMsgID (u8PortNum);
		
		/* Spec Ref: PRL_Tx_PHY_Layer_Reset - Resets Tx PHY Layer
						Entered on condition "discarding Complete" */
		PRL_PHYLayerReset (u8PortNum);
        
        DEBUG_PRINT_PORT_STR (u8PortNum,"PRL_TX_MSG_DISCARD_ON_RCV: Tx Msg Discarded on Recv\r\n");
		
		return PRL_RET_TX_MSG_DISCARD_ON_RCV;
	}
	
	do
	{
		u8OKToTx = UPD_RegReadByte (u8PortNum, PRL_TX_CTL_B);
	}
	
	while (!(u8OKToTx & PRL_TX_CTL_B_OK_TO_TX));
	
	DEBUG_PRINT_PORT_STR (u8PortNum,"PRL_TX_MSG_ON_LINE: Tx Msg sent on line\r\n");
	
	/* Go bit is set & message sent to PHY Layer*/
	/* Spec Ref: PRL_Tx_Wait_for_PHY_response, PRL_Tx_Match_MessageID, PRL_Tx_Check_RetryCounter state is handled by UPD hardware */
	UPD_RegWriteByte (u8PortNum, PRL_TX_CTL_B, PRL_TX_CTL_B_GO);
	
	/* PRL state variable is set to PRL_TX_MSG_ON_LINE_ST*/
	PRL_ChangeTxState (u8PortNum, PRL_TX_MSG_ON_LINE_ST);
	
	return PRL_RET_TX_MSG_TRANSMITTED_ON_LINE;

}

/***************************************************************************************************/

/***************************************************************************************************/


UINT8 PRL_BuildTxPacket (UINT8 u8PortNum, UINT32 u32Header, UINT8 *pu8DataBuffer, UINT8 *pu8TxPkt)
{
	UINT8 u8DataIndex , u8PktIndex = SET_TO_ZERO, 
		u8DataObjSizeInBytes = (PRL_GET_OBJECT_COUNT(u32Header) * PRL_SINGLE_DATAOBJ_SIZE_IN_BYTES);
	/* Info: while handling u8DataObjSizeinBytes for extended message, size of extended message header
				is not accounted nor the Chunk padded bytes count if any */
    
    #if (TRUE == INCLUDE_PD_3_0)
    UINT8 u8ZeroPadding = SET_TO_ZERO, u8LastChunkDataObjCnt;
	UINT16 u16ExtendedMsgHeader = SET_TO_ZERO;
	
	if(PRL_IS_EXTENDED_MSG(u32Header))
	{
	  	/* Spec Ref: TCH_Sending_Chunked_Message - Entered on condition " Chunk Passed" */
	  	
	  u16ExtendedMsgHeader = PRL_GET_EXTENDED_MSG_HEADER(u32Header);
		/* if Request chunk packet */
		if (PRL_IS_REQUEST_CHUNK_MSG (u16ExtendedMsgHeader))
		{
		  	/* Request Chunk packet has two bytes of Extended Message Header alone.
				Hence, two bytes is zero padded & no data */
			u8ZeroPadding = PRL_REQUEST_CHUNK_PADDING_BYTE_NUMBER;
			u8DataObjSizeInBytes = PRL_REQUEST_CHUNK_DATA_SIZE;
		}
		else
		{
		  	/* if it is not last chunk response packet, MaxExtendedMsgChunkLen is assigned */
			u8DataObjSizeInBytes = PRL_MAX_EXTN_MSG_CHUNK_LEN_IN_BYTES;
				
		  	/* if it is Response Chunk packet & last chunk*/
			if (gasChunkSM [u8PortNum].u8ChunkNumExpectedOrSent == gasChunkSM [u8PortNum].u8TotalChunkPkt)
			{
			  	/* if last chunk response packet is not MaxExtendedMsgChunkLen */
				if ((PRL_GET_DATA_SIZE(u16ExtendedMsgHeader) % PRL_MAX_EXTN_MSG_CHUNK_LEN_IN_BYTES))
				{
				  	/* size of last chunk packet data is calculated*/
					u8DataObjSizeInBytes = (PRL_GET_DATA_SIZE(u16ExtendedMsgHeader) % PRL_MAX_EXTN_MSG_CHUNK_LEN_IN_BYTES);
					
					/* if the data size with Extended message header size should be 4 byte aligned . 
						Else The packet must be padded to 4 byte alignment*/
					u8ZeroPadding = ((u8DataObjSizeInBytes + PRL_EXTN_MSG_HEADER_SIZE_IN_BYTES) % PRL_SINGLE_DATAOBJ_SIZE_IN_BYTES);
					
					/* Data Object Count is calculated for last chunk packet */
					u8LastChunkDataObjCnt = ((u8DataObjSizeInBytes + PRL_EXTN_MSG_HEADER_SIZE_IN_BYTES) / PRL_SINGLE_DATAOBJ_SIZE_IN_BYTES);
					
					if (u8ZeroPadding)
					{
						/* if the packet is to be zero padded, number of bytes to be padded is updated
							as well as the Data Object count for last packet is incremented*/
						u8ZeroPadding = PRL_SINGLE_DATAOBJ_SIZE_IN_BYTES - u8ZeroPadding;
						u8LastChunkDataObjCnt++;
					}
					
					/* Data object count for the last chunk packet is updated to the header*/
					u32Header = PRL_FORM_COMBINED_MSG_HEADER (u16ExtendedMsgHeader,
				  				PRL_UPDATE_MSG_HEADER_DATA_OBJECT_COUNT(PRL_GET_MSG_HEADER(u32Header), u8LastChunkDataObjCnt));
				}
			}
				
			
		}	
	}
    
    #endif /*endif for INCLUDE_PD_3_0*/
	
	
	
	/* Load the header to the TxPKT */
	pu8TxPkt [u8PktIndex] = LOBYTE(u32Header);
	pu8TxPkt [++u8PktIndex] = HIBYTE(u32Header);
	
    #if (TRUE == INCLUDE_PD_3_0)
	if(PRL_IS_EXTENDED_MSG(u32Header))
	{
		/* If extended, Extended Message header is loaded */
		pu8TxPkt [++u8PktIndex] = LOBYTE(u16ExtendedMsgHeader);
		pu8TxPkt [++u8PktIndex] = HIBYTE(u16ExtendedMsgHeader);
	}
    #endif
	
	/* if pu8DataBuffer is not NULL*/
	if (pu8DataBuffer != NULL)
	{
		/* Load the Data Object or data*/
		for (u8DataIndex = SET_TO_ZERO; u8DataIndex < u8DataObjSizeInBytes; u8DataIndex++)
		{
			pu8TxPkt [++u8PktIndex] = pu8DataBuffer [u8DataIndex];
		}
	}
    
    #if (TRUE == INCLUDE_PD_3_0)
	/* zero padding in case the data byte is not 4 byte aligned*/
	while (u8ZeroPadding)
	{
		pu8TxPkt [++u8PktIndex] = PRL_ZERO_PADDING_BYTE;
		u8ZeroPadding --;
	}
    #endif
    
	/* Packet length is returned*/
	return (++u8PktIndex);
}

/***************************************************************************************************/

/***************************************************************************************************/

void PRL_SendCableorHardReset (UINT8 u8PortNum, UINT8 u8CableorHardReset, PRLTxCallback pfnTxCallback, UINT32 u32PkdPEstOnTxStatus)
{
  	/* update the PDMAC globals */
	gasPRL[u8PortNum].u32PkdPEstOnTxStatus = u32PkdPEstOnTxStatus;
	gasPRL[u8PortNum].pFnTxCallback = pfnTxCallback;
	
	/* Ack the Tx interrupt befor enabling */
	UPD_RegWriteByte (u8PortNum, PRL_TX_IRQ_STAT, (PRL_TX_IRQ_TX_EOP | PRL_TX_IRQ_TX_ABORTED | PRL_TX_IRQ_TX_FAILED));

	/* enable the SW issued Tx bit */
	UPD_RegWriteByte (u8PortNum, PRL_TX_PARAM_A, PRL_TX_PARAM_A_EN_FW_TX);
	
	if (u8CableorHardReset)
	{
		/* enable Cable reset and go bit to transmit CR */
		UPD_RegWriteByte (u8PortNum, PRL_TX_CTL_B, (PRL_TX_CTL_B_TX_CABLE_RESET | PRL_TX_CTL_B_GO));
		
		/* Tx state variable is set to Cable Reset*/
        PRL_ChangeTxState (u8PortNum, PRL_TX_CABLE_RESET_ST);
	}
	else
	{
		/* enable Hard reset and go bit to transmit HR */
		UPD_RegWriteByte (u8PortNum, PRL_TX_CTL_B, (PRL_TX_CTL_B_TX_HARD_RESET | PRL_TX_CTL_B_GO));
		
		/* Tx state variable is set to Hard Reset*/
		PRL_ChangeTxState (u8PortNum, PRL_TX_HARD_RESET_ST);
	}
	
	DEBUG_PRINT_PORT_STR (u8PortNum,"PRL_TX_HARD_RESET_SENT: Hard Reset sent on line\r\n");
	
	/* enable the interrupt for HR */
	UPD_RegWriteByte (u8PortNum, PRL_TX_IRQ_EN, (PRL_TX_IRQ_TX_EOP | PRL_TX_IRQ_TX_ABORTED | PRL_TX_IRQ_TX_FAILED));
}


/*******************************************************************************************************/

/******************************************************************************************************/

void PRL_EnableRx (UINT8 u8PortNum, UINT8 u8Enable)
{
	if (u8Enable)
	{
	  	/* Enable the PD_MAC Receiver */
	  	/* EN_SMBUS_MODE - 1 denotes First location in Rx packet in FIFO is packet length +1, 
			second location is status */
		UPD_RegWriteByte (u8PortNum, PRL_RX_CTL_A, (PRL_RX_CTL_A_EN_RCV | PRL_RX_CTL_A_EN_SMBUS_MODE) );
		/* Enable reception of Hardreset*/
		UPD_RegByteSetBit (u8PortNum, PRL_RX_IRQ_EN, PRL_RX_IRQ_RX_HARD_RST);
        
        DEBUG_PRINT_PORT_STR (u8PortNum,"PRL: Receiver enabled\r\n");
	}
	else
	{
	  	/* PD_MAC Receiver is disabled */
		UPD_RegWriteByte (u8PortNum, PRL_RX_CTL_A, PRL_RX_CTL_A_EN_SMBUS_MODE);
		/* Disable reception of Hardreset*/
		UPD_RegByteClearBit (u8PortNum, PRL_RX_IRQ_EN, PRL_RX_IRQ_RX_HARD_RST);
        
        DEBUG_PRINT_PORT_STR (u8PortNum,"PRL: Receiver disabled\r\n");
	}
}

/*******************************************************************************************************/

/******************************************************************************************************/


UINT8 PRL_ReceiveMsg (UINT8 u8PortNum, UINT8 *pu8SOPType, UINT32 *pu32Header, UINT8 *pu8DataBuffer, PRLRxCallback pfnRxCallback)
{
    UINT16 u16DataSize, u8Return;
 
    u8Return = PRL_ProcessRecvdMsg(u8PortNum);
	
    if ((u8Return == PRL_RET_NO_MSG_RCVD) && (!gasPRL [u8PortNum].u8RxError) && (!gasPRL [u8PortNum].u8RxHRRcvdISR))
    {
		/* No Message is received. Return PRL_RET_NO_MSG_RCVD */
        return PRL_RET_NO_MSG_RCVD;
    }
	
    if (u8Return & PRL_RET_MSG_RCVD)
    {
	  	/* Non - Extended message is returned here */
        /* RxPacket is copied from global buffer */
        
        *pu8SOPType = gasPRLRecvBuff[u8PortNum].u8SOPtype;
        *pu32Header = gasPRLRecvBuff[u8PortNum].u16Header;
		
		/* if pu8DataBuffer is not NULL*/
        if (pu8DataBuffer != NULL)
        {
		  	/* data from global buffer is copied to local buffer */
            for (u16DataSize = SET_TO_ZERO; 
				 u16DataSize < (PRL_GET_OBJECT_COUNT(gasPRLRecvBuff[u8PortNum].u16Header) * PRL_SINGLE_DATAOBJ_SIZE_IN_BYTES); 
				 u16DataSize++)
            {
                pu8DataBuffer [u16DataSize] = gasPRLRecvBuff[u8PortNum].u8DataObj[u16DataSize];
            }
        }
        
        DEBUG_PRINT_PORT_STR (u8PortNum,"PRL_RX_PKT_PASSED_TO_PE: Rx Msg received passed to PE\r\n");

    }
    
    #if (TRUE == INCLUDE_PD_3_0)
    else if (u8Return & PRL_RET_EXT_MSG_RCVD)
    {
		/* Extended message is received */
        *pu8SOPType = gasExtendedMsgBuff [u8PortNum].u8SOPtype;
        *pu32Header = PRL_FORM_COMBINED_MSG_HEADER (gasExtendedMsgBuff [u8PortNum].u16ExtendedMsgHeader, 
													gasExtendedMsgBuff [u8PortNum].u16Header);
		/* if pu8DataBuffer is not NULL*/
        if (pu8DataBuffer != NULL)
        {
		  	/* data from global buffer is copied to local buffer */
            for (u16DataSize = SET_TO_ZERO;	\
			  u16DataSize < (PRL_GET_DATA_SIZE(gasExtendedMsgBuff[u8PortNum].u16ExtendedMsgHeader));	\
			  u16DataSize++)
            {
                pu8DataBuffer [u16DataSize] = gasExtendedMsgBuff[u8PortNum].u8Data[u16DataSize];
            }
        }
		
        DEBUG_PRINT_PORT_STR (u8PortNum,"PRL_EXTN_RX_PKT_PASSED_TO_PE: Extended Msg received passed to PE\r\n");
		
    }
    /*Note: gasPRL [u8PortNum].u8RxError is used only for PRL_RX_CHUNK_RCV_ERROR*/
    else if(gasPRL [u8PortNum].u8RxError)
    {
	  	/* Chunk State Error*/
        u8Return = PRL_RET_RCV_CHUNK_ERROR;
		/* RxIntrStatus is cleared*/
        gasPRL [u8PortNum].u8RxError &= ~PRL_RX_CHUNK_RCV_ERROR;
		
        DEBUG_PRINT_PORT_STR (u8PortNum,"PRL_RX_CHUNK_RCV_ERROR: Rx Chunk Error \r\n");
    }
    #endif
    else
    {
       /* Hard Reset Process pending*/
        u8Return = PRL_RET_HARD_RESET_RCVD; 
    }
	
	/* if pfnRxCallback is not NULL, pfnRxCallback is called */
    if (pfnRxCallback != NULL)
    {
        pfnRxCallback(u8PortNum, u8Return);
    }
    
    return u8Return;
	
}

/*******************************************************************************************************/

/******************************************************************************************************/

void PRL_HandleISR (UINT8 u8PortNum)
{
  
	UINT8 u8MACIRQSTAT = SET_TO_ZERO, u8IRQSTAT = SET_TO_ZERO, u8IRQEN = SET_TO_ZERO, u8IntrStatus;
	
	UINT32 u32PkdPEstOnTxStatus;
    
    /*Read the overall Mac Interrupt */
    UPD_RegisterReadISR (u8PortNum, PRL_MAC_IRQ_STAT, &u8MACIRQSTAT, BYTE_LEN_1);

	/* Tx MAC interrupt Handling */
	if (u8MACIRQSTAT & PRL_MAC_IRQ_TX)
	{
	  	/* Read Tx interrupt */
        
        /* Read TX IRQ status register */
        UPD_RegisterReadISR (u8PortNum, PRL_TX_IRQ_STAT, &u8IRQSTAT, BYTE_LEN_1);
          
        /* Read TX IRQ Enable register */
        UPD_RegisterReadISR (u8PortNum, PRL_TX_IRQ_EN, &u8IRQEN, BYTE_LEN_1);
        
        /* Filter out relevant interrupt */
        u8IntrStatus = u8IRQSTAT & u8IRQEN;

		/* Clear the Tx interrupt Status Register */
		UPD_RegisterWriteISR (u8PortNum, PRL_TX_IRQ_STAT, &u8IntrStatus, BYTE_LEN_1);

		/* Update the PD_MAC state */
		if (PRL_TX_IRQ_TX_DONE & u8IntrStatus)
		{
			gasPRL [u8PortNum].u8TxStateISR = PRL_TX_DONE_ST;
            if (gasPRL[u8PortNum].u8TxStsDPMSyncISR)
            {
                gasPRL[u8PortNum].u8TxStsDPMSyncISR = FALSE;
            }
		}
		
		/* PD_MAC state variable is updated depending Tx interrupt received */
		if (PRL_TX_IRQ_TX_FAILED & u8IntrStatus)
		{
			gasPRL [u8PortNum].u8TxStateISR = PRL_TX_FAILED_ST;
		}
		
		if (PRL_TX_IRQ_TX_ABORTED & u8IntrStatus)
		{
			gasPRL [u8PortNum].u8TxStateISR = PRL_TX_ABORTED_ST;
		}
		
		if (PRL_TX_IRQ_TX_EOP & u8IntrStatus)
		{
			gasPRL [u8PortNum].u8TxStateISR = PRL_TX_EOP_ST;
		}
		
		/* Spec Ref: PRL_Tx_Message_Sent & PRL_Tx_Transmission_Error - Increment the MessageIDCounter.
															Inform Policy Engine message sent */
		/* Increment the MessageID based on SOP* Type */
		if (gasPRL [u8PortNum].u8TxStateISR != PRL_TX_EOP_ST)
		{
			PRL_IncrementMsgID (u8PortNum);
		}
		
		/* Tx callback is called */
		if (gasPRL [u8PortNum].pFnTxCallback != NULL)
		{
		  	u32PkdPEstOnTxStatus = gasPRL[u8PortNum].u32PkdPEstOnTxStatus;
			gasPRL [u8PortNum].pFnTxCallback(u8PortNum, 
												LOBYTE(u32PkdPEstOnTxStatus), 			/* u8TxDonePEst */
												HIBYTE(u32PkdPEstOnTxStatus),			/* u8TxDonePESubst*/                                 
												LOBYTE(HIWORD(u32PkdPEstOnTxStatus)),	/* u8TxFailedPEst */
												HIBYTE(HIWORD(u32PkdPEstOnTxStatus)));	/* u8TxFailedPESubst */
		}
		
		/* Txstate is set back to idle after callback*/
		gasPRL [u8PortNum].u8TxStateISR =  PRL_TX_IDLE_ST;

	}

	if (u8MACIRQSTAT & PRL_MAC_IRQ_RX)
	{
        /* Read Rx interrupt */
        
        /* Read RX IRQ status register */
        UPD_RegisterReadISR (u8PortNum, PRL_RX_IRQ_STAT, &u8IRQSTAT, BYTE_LEN_1);
          
        /* Read RX IRQ Enable register */
        UPD_RegisterReadISR (u8PortNum, PRL_RX_IRQ_EN, &u8IRQEN, BYTE_LEN_1);
        
        /* Filter out relevant interrupt */
        u8IntrStatus = u8IRQSTAT & u8IRQEN;
        
		/* Clear the Rx interrupt status register */
		UPD_RegisterWriteISR (u8PortNum, PRL_RX_IRQ_STAT, &u8IntrStatus, BYTE_LEN_1);

		/* update the PDMAC variable depending on Rx interrupt received */
		if (PRL_RX_IRQ_RX_FIFO_NOT_EMPTY & u8IntrStatus)
		{
			/* Received message is processed */
			PRL_ProcessRxFIFOISR (u8PortNum);
		}
		
		if (PRL_RX_IRQ_RX_HARD_RST & u8IntrStatus)
		{
			/*Enable the HR flag*/
			gasPRL [u8PortNum].u8RxHRRcvdISR = TRUE;
			/*Enable PD message processing by FW*/
            gasPRL[u8PortNum].u8RxDisable = FALSE;
            /*Setting the Hard reset received flag*/
            gasPolicyEngine[u8PortNum].u8HardResetRcvdISR = TRUE;
		}
	}
}


/******************************************************************************************************/

void PRL_IncrementMsgID (UINT8 u8PortNum)
{
  	/* Read the Tx SOP type from PRL_Tx_PARAM_A) */
	UINT8 u8TxSOPType = (PRL_TX_PARAM_A_TX_SOP_SELECT_MASK & (UPD_RegReadByte (u8PortNum, PRL_TX_PARAM_A)));
	
	/* Depending on the SOP* type, MessageID for that SOP* Type is incremented */ 
	switch (u8TxSOPType)
	{
		case PRL_TX_PARAM_A_TX_SOP_SELECT_SOP:
		{
			gasPRL[u8PortNum].u8SOPMsgID = PRL_INCREMENT_MSG_ID(gasPRL[u8PortNum].u8SOPMsgID);
			break;
		 }
			  
		case PRL_TX_PARAM_A_TX_SOP_SELECT_SOP_P:
		{
			gasPRL[u8PortNum].u8SOP_P_MsgID = PRL_INCREMENT_MSG_ID(gasPRL[u8PortNum].u8SOP_P_MsgID);
			break;
		}

		case PRL_TX_PARAM_A_TX_SOP_SELECT_SOP_PP:
		{
			gasPRL[u8PortNum].u8SOP_PP_MsgID = PRL_INCREMENT_MSG_ID(gasPRL[u8PortNum].u8SOP_PP_MsgID);
			break;
		}
		
		default:
		{
			break;
		}
	}

}


/*******************************************************************************************************/


void PRL_ProcessRxFIFOISR (UINT8 u8PortNum)
{
	UINT8* u8pFIFOBuffer = (UINT8 *) &gasPRLRecvBuff[u8PortNum];
    
	/*Receiver packet is read from Rx FIFO*/
	UPD_RegisterReadISR(u8PortNum, PRL_PDMAC_RECEIVER_BUFF_ADDR, u8pFIFOBuffer, sizeof(gasPRLRecvBuff [u8PortNum]));
	
	gasPRLRecvBuff [u8PortNum].u8SOPtype = PRL_GET_SOP_TYPE_FROM_RX_STATUS(gasPRLRecvBuff [u8PortNum].u8SOPtype);
	
    if (!gasPRL[u8PortNum].u8RxDisable)
    {
        /* Received bit is set */
        gasPRL[u8PortNum].u8RxRcvdISR = TRUE;
    }
	

}

/******************************************************************************************************/

void PRL_TxRxMsgID_Reset(UINT8 u8PortNum, UINT8 u8SOPType)
{
  	/* Spec Ref: PRL_Tx_Layer_Reset_for_Transmit - Entered on soft Reset Message request Received from Policy Engine 
											Reset MessageIDCounter.
											Protocol Layer message reception transition to PRL_Rx_WAIT_FOR_PHY_MESSAGE state */
  
  	/* corresponding Tx & RX Message ID of SOP type is reset is reset */
	UINT8 u8RxSOPSelect = SET_TO_ZERO;
	
	/* PRL_SOP_TYPE value is 0, it cannot be masked. Always SOP_TYPE & NON_SOP_TYPE message
		counter reset must be called separately */
	if (PRL_SOP_TYPE == u8SOPType)
	{
		gasPRL[u8PortNum].u8SOPMsgID = RESET_TO_ZERO;
		u8RxSOPSelect |= PRL_RX_MSG_ID_STORED_SOP;
	}
	
	if (PRL_SOP_P_TYPE & u8SOPType)
	{
		gasPRL[u8PortNum].u8SOP_P_MsgID = RESET_TO_ZERO;
		u8RxSOPSelect |= PRL_RX_MSG_ID_STORED_SOP_P;
	}
	
	if (PRL_SOP_PP_TYPE & u8SOPType)
	{
	  	gasPRL[u8PortNum].u8SOP_PP_MsgID = RESET_TO_ZERO;
		u8RxSOPSelect |= PRL_RX_MSG_ID_STORED_SOP_PP;
	}
	
	UPD_RegWriteByte (u8PortNum, PRL_RX_MSG_ID_STORED, u8RxSOPSelect);
   
}


/******************************************************************************************************/

/******************************************************************************************************/

void PRL_ProtocolSpecificSOPReset(UINT8 u8PortNum, UINT8 u8SOPType)
{
  	/* Spec Ref: PRL_Rx_Layer_Reset_for_Receive - Entered on condition Soft Reset Message received from PHY */
	PRL_TxRxMsgID_Reset (u8PortNum, u8SOPType);
}

/******************************************************************************************************/

void PRL_ProtocolResetAllSOPs(UINT8 u8PortNum)
{
  	/* Spec Ref: PRL_HR_Reset_Layer */
	PRL_TxRxMsgID_Reset (u8PortNum, PRL_SOP_TYPE);
	PRL_TxRxMsgID_Reset (u8PortNum, (PRL_SOP_P_TYPE | PRL_SOP_PP_TYPE));
}


/******************************************************************************************************/

void PRL_OnHardResetComplete (UINT8 u8PortNum)
{
    
    /* gasPRL [u8PortNum].u8RxHRRcvdISR is cleared as Hard Reset is processed*/
    /* gasPRL [u8PortNum].u8RxHRRcvdISR is ISR & foreground sync variable. 
        Interrupt is disabled to prevent variable corruption */
    MCHP_PSF_HOOK_DISABLE_GLOBAL_INTERRUPT();
	gasPRL [u8PortNum].u8RxHRRcvdISR = FALSE;
    MCHP_PSF_HOOK_ENABLE_GLOBAL_INTERRUPT();
	
	DEBUG_PRINT_PORT_STR (u8PortNum,"PRL_HR_COMPLETE: PRL is informed about HR complete\r\n");
    
	/*If Tx_EOP is enabled at the time of Auto mode response,For Good_CRC sent as auto 
     * response, Tx_EOP interrupt is fired. Thus, Disable the Tx interrupt*/
	UPD_RegWriteByte (u8PortNum, PRL_TX_IRQ_EN, CLR_VAL);

    /* When HR is received from port partner, EN_RCV bit is cleared. 
		Hence, after HR completion it is re-enabled */
    UPD_RegWriteByte (u8PortNum, PRL_RX_CTL_A, (PRL_RX_CTL_A_EN_RCV | PRL_RX_CTL_A_EN_SMBUS_MODE) );
   
#if (TRUE == INCLUDE_PD_DR_SWAP)
    /* If there has been a Data Role Swap the Hard Reset Shall cause the 
       Port Data Role to be changed back to DFP for a Port with the Rp resistor 
       asserted and UFP for a Port with the Rd resistor asserted.*/
    DPM_UpdateDataRole (u8PortNum, DPM_GET_CURRENT_POWER_ROLE(u8PortNum)); 
    
    PRL_UpdateSpecAndDeviceRoles (u8PortNum);
#endif     
		    
}

/******************************************************************************************************/

void PRL_PHYLayerReset (UINT8 u8PortNum)
{
  	/* Spec Ref: PRL_TX_PHY_Layer_Reset - Reset PHY Layer
								Entered on condition Soft Reset Message from PHY Layer or Exit from Hard Reset */
	/* Reset the hardware */
	UPD_RegWriteByte (u8PortNum, PRL_RESET_CTL, PRL_RESET_CTL_PD_RESET);

	/* clear the reset register */
	UPD_RegWriteByte (u8PortNum, PRL_RESET_CTL, CLR_VAL);

}

/******************************************************************************************************/


/****************************************************************************************************/


UINT8 PRL_ProcessRecvdMsg(UINT8 u8PortNum)
{
    UINT8 u8Return = PRL_RET_NO_MSG_RCVD;

    if (!(gasPRL[u8PortNum].u8RxRcvdISR))
    {
        return u8Return;
    }
    
    #if (TRUE == INCLUDE_PD_3_0)
    UINT8 u8PDOIndex;
/***********************************************Extended Message Handling**************************************************************/
	/* if received message is extended message*/
    if (PRL_IS_EXTENDED_MSG(gasPRLRecvBuff [u8PortNum].u16Header))
    {
		/* Extended Message header is formed with the DataObj obtained*/
		UINT16 u16ExtendedMsgHeader = MAKE_UINT16(gasPRLRecvBuff [u8PortNum].u8DataObj[INDEX_1],\
                                            gasPRLRecvBuff [u8PortNum].u8DataObj[INDEX_0]);
		
		/*******************************************************REQUEST_CHUNK Handling******************************************************/
	  	/* Checks whether received message is Chunk Response or Request */
        if (PRL_IS_REQUEST_CHUNK_MSG(u16ExtendedMsgHeader))
        {
		  	/* CONFIG_PRL_CHUNK_SENDER_REQUEST_TIMEOUT_MS is killed on receiving Chunk Request */
            PRL_KillCAorChunkSMTimer (u8PortNum);
			
            if ((PRL_GET_CHUNK_NUMBER(u16ExtendedMsgHeader) == gasChunkSM[u8PortNum].u8ChunkNumExpectedOrSent)
					&& PRL_IS_MSG_CHUNKED(u16ExtendedMsgHeader))
            {
                DEBUG_PRINT_PORT_STR (u8PortNum,"PRL_CHUNK_REQUEST_RCV: Chunk Request received\r\n");
				
			  	/* If Chunk number request received and ChunkNumber of next Chunk to be sent is equal,
				PRL_TCH_SEND_RESPONSE_CHUNK_ST is assigned to send next chunk packet*/
                gasChunkSM [u8PortNum].u8ChunkState	= PRL_TCH_SEND_RESPONSE_CHUNK_ST;
            }
            else
            {
                DEBUG_PRINT_PORT_STR (u8PortNum,"PRL_TCH_ERROR: Request Chunk number mismatch\r\n");
				
			  	/* Spec Ref: TCH_Report_Error - Report Error to Policy Engine. 
												Entered on condition 
											(Chunk Request Rcvd & Chunk Number != Chunk Number to Send) */
				/* if Chunk number does not match, error state is assigned.
					It is transmission error, Thus PE is informed through call back*/
                PRL_TxOriginalCBfromCH (u8PortNum, PRL_TX_FAILED_ST);
            }
			
        } /* end of if for IS_REQ_CHUNK_MSG*/
		
		/***********************************************************************************************************************************/
		/**********************************************RESPONSE CHUNK Handling**************************************************************/
        else
        {
		  	/* Chunk Response is handled here */
			
			/* Spec Ref: RCH_Processing_Extended_Message - Entered on Condition 
															Received Extended Message & (Chunking = 1 & Chunked = 1)*/
		  
			/* if the received chunk is 1st response, then Chunk SM is enabled
				ChunkSM related globals are updated & Chunk response is stored in global*/
		  
            if (PRL_FIRST_CHUNK_PACKET == PRL_GET_CHUNK_NUMBER(u16ExtendedMsgHeader))
            {
               /* if Chunk SM Chunk message is already in process & other message is received*/
                if (gasChunkSM [u8PortNum].u8EnableChunkSM)
                {
                    DEBUG_PRINT_PORT_STR (u8PortNum,"PRL_CHUNK_UNEXPECTED_MSG_RCV: Unexpected Chunk msg received\r\n");
                     
                    /* Unexpected message received*/
                    if ((PRL_RCH_EXPECT_RESPONSE_CHUNK_WAIT_ST == gasChunkSM [u8PortNum].u8ChunkState)
                        || (PRL_RCH_SEND_CHUNK_REQUEST_ST == gasChunkSM [u8PortNum].u8ChunkState)
                        || (PRL_RCH_WAIT_FOR_CHUNK_REQUEST_STATUS_ST == gasChunkSM [u8PortNum].u8ChunkState))
                    {
                        /* Chunk Rx Handling*/
                        /* Spec Ref: On RCH_WAITING_CHUNK if other message received Report RCH_Report_Error*/
                        gasChunkSM [u8PortNum].u8ChunkState = PRL_RCH_CHUNK_RECV_ERROR_ST;
                    }
                    else
                    {
                    
                    /* Spec Ref: TCH_Message_Received : Clear the Extended message buffers
                                Passed the received message to Chunked Rx Engine*/
                    }
                    /* Reset the Chunk State machine*/
                    PRL_ResetChunkSM (u8PortNum);
                    /* New message received is processed below*/
                }
                
                /* Spec Ref: RCH_Processing_Extended_Message: 
					If first chunk: Set Chunk_Number_Expected = 0 and Num bytes received = 0 */
                gasChunkSM [u8PortNum].u8EnableChunkSM = TRUE;
                DEBUG_PRINT_PORT_STR (u8PortNum, "PRL: Chunk SM Enabled\r\n");
					
				/* copying the message to global buffer */
				
				/* SOP* Type of received packet*/
                gasExtendedMsgBuff [u8PortNum].u8SOPtype = gasPRLRecvBuff [u8PortNum].u8SOPtype;
			
				/*Header is copied */
                gasExtendedMsgBuff [u8PortNum].u16Header = gasPRLRecvBuff [u8PortNum].u16Header;
				
				/* Extended message header is copied to the global*/
                gasExtendedMsgBuff [u8PortNum].u16ExtendedMsgHeader = u16ExtendedMsgHeader;
				
				/* Total Chunk packet to be sent is updated*/
                PRL_UpdateTotalChunkNumVar (u8PortNum);
				
            }
            else
            {
			  	/* CONFIG_PRL_CHUNK_SENDER_RESPONSE_TIMEOUT_MS is killed on receiving Chunk Response if it is not first Chunk Response packet*/
                PRL_KillCAorChunkSMTimer (u8PortNum);
            }
			
			
            if ((PRL_GET_CHUNK_NUMBER(u16ExtendedMsgHeader) == gasChunkSM [u8PortNum].u8ChunkNumExpectedOrSent)
					&& PRL_IS_MSG_CHUNKED(u16ExtendedMsgHeader))
            {
			  	/* Spec Ref: RCH_Processing_Extended_Message: If expected Chunk Number: 
							Append data to Extended_Message_Buffer;
							Increment Chunk_Number_Expected and adjust Num bytes received. */
			
				/* Data Buffer is copied*/
				/* Data buffer copy is started from 2 as first two bytes of data Extended Message Header */
                for (u8PDOIndex = PRL_EXTN_MSG_HEADER_SIZE_IN_BYTES; 
                        u8PDOIndex < PRL_GET_OBJECT_COUNT(gasPRLRecvBuff [u8PortNum].u16Header) * PRL_SINGLE_DATAOBJ_SIZE_IN_BYTES; 
                            u8PDOIndex++)
                {
                    gasExtendedMsgBuff [u8PortNum].u8Data[gasChunkSM [u8PortNum].u16RxReceivedBytes++] = gasPRLRecvBuff [u8PortNum].u8DataObj[u8PDOIndex];
                }
				
				/* if received chunk response is last chunk packet*/
                if (gasChunkSM [u8PortNum].u8ChunkNumExpectedOrSent == gasChunkSM [u8PortNum].u8TotalChunkPkt)
                {
					/* all the chunk packet expected is received. Hence PE is informed through INTR status*/
					
					/* Chunk SM is reset*/
                    PRL_ResetChunkSM (u8PortNum);
                    
                    /* Return value is set to PRL_RET_EXT_MSG_RCVD to indicate Extended Message has received */
                    u8Return =  PRL_RET_EXT_MSG_RCVD;
                    
                }
                else
                {
                    DEBUG_PRINT_PORT_STR (u8PortNum,"PRL_CHUNK_RESPONSE_RCV: Chunk Response Received\r\n");
					
                    /* If the received Chunk response is not the last chunk response packet
						PRL_RCH_SEND_CHUNK_REQUEST_ST is assigned to to request for next chunk*/
                    gasChunkSM [u8PortNum].u8ChunkState = PRL_RCH_SEND_CHUNK_REQUEST_ST;
					
					/* Increment the chunk number expected */
                    gasChunkSM [u8PortNum].u8ChunkNumExpectedOrSent++;
                }
            }
            else
            {
                DEBUG_PRINT_PORT_STR (u8PortNum,"PRL_CHUNK_RECV_ERROR: Response Chunk number mismatch\r\n");
			  	/* PRL_RCH_CHUNK_RECV_ERROR_ST is assigned to indicate PE*/
                gasChunkSM [u8PortNum].u8ChunkState = PRL_RCH_CHUNK_RECV_ERROR_ST;
            }
			
        } /* end of if for IS_REQ_CHUNK_MSG*/
		
		/***********************************************************************************************************************************/
	
    } /* end of if check for IsExtendedMsg*/
	/***************************************************Non-Extended Message Handling******************************************************/
    else    
    {
        if ((gasChunkSM [u8PortNum].u8EnableChunkSM) && (PRL_GET_MESSAGE_TYPE(gasPRLRecvBuff [u8PortNum].u16Header) != PE_CTRL_PING))
        {
		  	/* Unexpected message received and received message is not PING */
            if ((PRL_RCH_EXPECT_RESPONSE_CHUNK_WAIT_ST == gasChunkSM [u8PortNum].u8ChunkState)
                || (PRL_RCH_SEND_CHUNK_REQUEST_ST == gasChunkSM [u8PortNum].u8ChunkState)
                || (PRL_RCH_WAIT_FOR_CHUNK_REQUEST_STATUS_ST == gasChunkSM [u8PortNum].u8ChunkState))
            {
                DEBUG_PRINT_PORT_STR (u8PortNum,"PRL_CHUNK_UNEXPECTED_MSG_RCV: Unexpected msg received other than Ping & Chunk msg\r\n");
			  	
                /* Chunk SM is reset*/
                PRL_ResetChunkSM (u8PortNum);
                    
                /* Spec Ref: On RCH_WAITING_CHUNK if other message received Report RCH_Report_Error*/
                u8Return = PRL_RET_RCV_CHUNK_ERROR;
            }
            else
            {
                DEBUG_PRINT_PORT_STR (u8PortNum,"PRL_CHUNK_UNEXPECTED_MSG_RCV: Unexpected msg received other than Ping & Chunk msg\r\n");
				/* Spec Ref: TCH_Message_Received - Clear Extended Message Buffer. Pass message to Chunked Rx*/
                PRL_ResetChunkSM (u8PortNum);
            }
        }
        /* if it is not an extended message, Message is copied to globals & PE is informed*/
        /* Return value is set to PRL_RET_MSG_RCVD to indicate Message has received */
        u8Return |=  PRL_RET_MSG_RCVD;
		 	
    }/*end of else for IsExtendedMsg*/
    
    /* if the message is received in process of Collision avoidance, the pending message is discarded
        by setting the Tx state to PRL_TX_IDLE_ST*/
    if ((PRL_TX_MSG_BUFFERED_ON_CA_ST == gasPRL [u8PortNum].u8TxStateISR) ||
           (PRL_Tx_CA_SRC_SINKTXTIMER_ON_ST == gasPRL [u8PortNum].u8TxStateISR) || 
             (PRL_TX_CA_SINK_TXNG_ST == gasPRL [u8PortNum].u8TxStateISR))
    {
            /* Collision avoidance timer is killed in case active*/
           PRL_KillCAorChunkSMTimer (u8PortNum);
           
           /* PRL_TX_IDLE_ST is assigned*/
            PRL_ChangeTxState (u8PortNum, PRL_TX_IDLE_ST);
    }
    
    #else
        /* Return value is set to PRL_RET_MSG_RCVD to indicate Message has received */
        u8Return |=  PRL_RET_MSG_RCVD;
     #endif /* for INCLUDE_PD_3_0 */
    MCHP_PSF_HOOK_DISABLE_GLOBAL_INTERRUPT();
    /* gasPRL[u8PortNum].u8RxRcvdISR is ISR & foreground sync variable. 
        Interrupt is disabled to prevent variable corruption */
	gasPRL[u8PortNum].u8RxRcvdISR = FALSE;
    
    MCHP_PSF_HOOK_ENABLE_GLOBAL_INTERRUPT();
    
    return u8Return;
}

/******************************************************************************************************/


UINT32 PRL_IsAnyMsgPendinginPRL (UINT8 u8PortNum)
{
	/* If there are any pending message unprocessed by the PRL
        Message header is returned else zero */
	if (gasPRL[u8PortNum].u8RxRcvdISR)
	{
		return gasPRLRecvBuff[u8PortNum].u16Header;
	}
	else
	{
		return FALSE;
	}
}
/******************************************************************************************************/


/* Below are PD 3.0 Spec related functions */

#if (TRUE == INCLUDE_PD_3_0)

/******************************************************************************************************************************************

									COLLISION AVOIDANCE APIS for Source
*******************************************************************************************************************************************/

void PRL_SetCollisionAvoidance (UINT8 u8PortNum, UINT8 u8Enable)
{
    /*If the current spec role is not 3.0 return or default configured Rp value is not 3A*/
    if((PD_SPEC_REVISION_3_0 != DPM_GET_CURRENT_PD_SPEC_REV(u8PortNum)) ||\
            (TYPEC_DFP_3A0_CURRENT != DPM_GET_CONFIGURED_SOURCE_RP_VAL(u8PortNum)))
    {
        return;
    } 
	if (TYPEC_SINK_TXNG == u8Enable)
	{
	  	/* Spec Reference: PRL_Tx_Src_Source_Tx - Set Rp = SinkTxNG */
		/* Rp = SinkTxNG 1.5A @ 5v is set */
		TypeC_SetRpCollAvoidance(u8PortNum, TYPEC_SINK_TXNG);
		
		/* Spec Reference: PRL_Tx_Src_Pending - Start sinkTxTimer*/
        /* SinkTxTimer is started. */
		gasChunkSM [u8PortNum].u8CAorChunkSMTimerID = PDTimer_Start (PRL_SINKTX_TIMEOUT_MS,\
                                                        PRL_CASinkTxTimerOut_TimerCB, u8PortNum, (UINT8)SET_TO_ZERO);
		
		/* u8Txstate is set to PRL_Tx_CA_SRC_SINKTXTIMER_ON_ST*/
		PRL_ChangeTxState (u8PortNum, PRL_Tx_CA_SRC_SINKTXTIMER_ON_ST);
        		
        DEBUG_PRINT_PORT_STR (u8PortNum,"PRL: CONFIG_PRL_SINK_TX_TIMEOUT_MS is set\r\n");
	}
	else
	{
	  	/* Spec Reference: PRL_tx_Src_Sink_Tx - Set Rp = SinkTxOK */
		/* Rp = SinkTxOk 3A @ 5v is set*/
		TypeC_SetRpCollAvoidance(u8PortNum, TYPEC_SINK_TXOK);
	}
}

/******************************************************************************************************/

void PRL_CASinkTxTimerOut_TimerCB (UINT8 u8PortNum, UINT8 u8DummyVariable)
{
  	/* Spec Reference: PRL_Tx_Construct_Message or PRL_Tx_Layer_Reset_for_Transmit - Entered on condition
						Message pending (except Soft Reset) & SinkTxTimer timeout
						Soft Reset Message pending & SinkTxTimer timeout respectively */
    /* Pending message if any in the Tx FIFO is transmitted on SinkTxTimer Times out*/
    PRL_CommitPendingTxOnCAISR (u8PortNum);
    
    /* setting the Timer ID to Max value*/
    gasChunkSM [u8PortNum].u8CAorChunkSMTimerID = MAX_CONCURRENT_TIMERS;
	
}
/******************************************************************************************************/

UINT8 PRL_IsAMSInitiatable(UINT8 u8PortNum)
{
    UINT8 u8ReturnVal = TRUE, u8CurrentPwrRole = DPM_GET_CURRENT_POWER_ROLE(u8PortNum);
    /*If the port is 3.0 check whether the port is capable of initiating an AMS*/
    if(PD_SPEC_REVISION_3_0 == DPM_GET_CURRENT_PD_SPEC_REV(u8PortNum))
    {
        /*If Role is Sink, check whether Source Rp capability is 3A*/
        if ((PD_ROLE_SINK == u8CurrentPwrRole) && (DPM_PORT_RP_VAL_DETECT_3A_STATUS ==\
                      (gasCfgStatusData.sPerPortData[u8PortNum].u32PortConnectStatus &
                        DPM_PORT_RP_VAL_DETECT_MASK_STATUS)))
        {
            /* Spec Ref: PRL_Tx_Snk_Start_of_AMS */
            if (TYPEC_SINK_TXNG == TypeC_CheckRpValCollAvoidance(u8PortNum))
            {
                /* Spec Ref: PRL_Tx_Snk_Pending*/
                /* if Rp value is SinkTxNG, PRL_TX_CA_SINK_TXNG_ST is assigned*/
                PRL_ChangeTxState (u8PortNum, PRL_TX_CA_SINK_TXNG_ST);
                /*Sink Tx NG*/
                u8ReturnVal = FALSE;
            }
            else
            {
                /*Sink Tx OK -Return TRUE */
            }
        }
        else if ((PD_ROLE_SOURCE == u8CurrentPwrRole) &&
                (TYPEC_DFP_3A0_CURRENT == DPM_GET_CONFIGURED_SOURCE_RP_VAL(u8PortNum)))
        {
            if  (gasPRL [u8PortNum].u8TxStateISR != PRL_TX_IDLE_ST)
            {
                /* Tx is not  in Idle*/
                u8ReturnVal = FALSE;
            }
        }
        else
        {
            /*Do nothing*/
        }
    }
    return u8ReturnVal;
}
/******************************************************************************************************/
void PRL_CommitPendingTxOnCAISR (UINT8 u8PortNum)
{
    if (gasPRL[u8PortNum].u8RxRcvdISR)                			/* Checks whether a message is received */
    {
        /* if there is a pending message in Rx, discard the Tx commitment queued due to CA*/ 
		PRL_PHYLayerReset (u8PortNum);
        
        /* PRL_TX_IDLE_ST is assigned to PRL state*/
        gasPRL [u8PortNum].u8TxStateISR = PRL_TX_IDLE_ST;
        
        DEBUG_PRINT_PORT_STR (u8PortNum,"PRL_TX_MSG_DISCARD_ON_RCV: Tx Msg Discarded on Recv\r\n");
		
    }
    else if (PRL_TX_MSG_BUFFERED_ON_CA_ST == gasPRL [u8PortNum].u8TxStateISR)
    {
        /* Spec Ref: PRL_Tx_Layer_Reset_for_Transmit & PRL_Tx_Construct_Message - Entered on Condition
				Soft Reset Message pending & Rp = SinkTxOK and
				Message pending (except Soft Reset) & Rp = SinkTxOK respectively */
      
        /* Go bit is set to transmit the message buffered in Tx FIFO & message Pkt is transmitted on PHY */
        UPD_RegWriteByte (u8PortNum, PRL_TX_CTL_B, PRL_TX_CTL_B_GO);

        /* PRL_TX_MSG_ON_LINE_ST is assigned to PRL state as Buffered message is tranmitted on line*/
        gasPRL [u8PortNum].u8TxStateISR = PRL_TX_MSG_ON_LINE_ST;
        
        DEBUG_PRINT_PORT_STR (u8PortNum,"PRL: Buffered Tx Msg sent on line\r\n");
    }
    else
    {
        /* PRL_TX_IDLE_ST is assigned to PRL state*/
        gasPRL [u8PortNum].u8TxStateISR = PRL_TX_IDLE_ST;
    }
}
/********************************************************************************************************************************************/

void PRL_TCHChunkSMStateChange_TCHCB (UINT8 u8PortNum, UINT8 u8TimerID, UINT8 TxDoneSt, UINT8 TxAbortSt, UINT8 TxFailSt)
{
  	/* Callback for Chunk Request Messages*/ 
	/* Policy engine state is assigned based  on Tx Interrupt status*/
	if (PRL_TX_DONE_ST == gasPRL[u8PortNum].u8TxStateISR)
	{
		/* Checks whether GoodCRC received is not last Chunk Response packet*/
		if (gasChunkSM [u8PortNum].u8ChunkNumExpectedOrSent != gasChunkSM [u8PortNum].u8TotalChunkPkt)
		{
		  	/* TCH_WAIT_CHUNK_REQUEST - Increment Chunk Number to Send
										Start ChunkSenderRequest Timer
										TCH_WAIT_CHUNK_REQUEST is entered from TCH_SENDING_CHUNKED MESSAGE
										on "Message Transmitted from Protocol Layer & Not Last chunk".
										 */
			/* Increment the chunk number to send*/
			gasChunkSM [u8PortNum].u8ChunkNumExpectedOrSent++;
			
			/* Start ChunkSenderRequestTimer*/
			gasChunkSM [u8PortNum].u8CAorChunkSMTimerID = PDTimer_Start (PRL_CHUNKSENDERREQUEST_TIMEOUT_MS, 
																		 PRL_ChunkStateChange_TimerCB, 
																		 u8PortNum, 
																		 PRL_TCH_CHUNKSENDERREQUEST_TIMEOUT_ST);
			/* Assigning PRL_TCH_EXPECT_CHUNK_REQUEST_WAIT_ST state*/
			gasChunkSM [u8PortNum].u8ChunkState = PRL_TCH_EXPECT_CHUNK_REQUEST_WAIT_ST;
		}
		else
		{
		  	/* TCH_MESSAGE_SENT - Inform Policy Engine of Message Sent.
									TCH_MESSAGE_SENT is entered on condition
									"Message Transmitted received from Protocol Layer & Last Chunk" */
		  	/* GoodCRC for last Chunk is received*/
			/* all the Chunk packets are sent successfully. PE is informed through the Callback
				Chunk SM is Reset */
		  	PRL_TxOriginalCBfromCH (u8PortNum, PRL_TX_DONE_ST);
		}
	}
	
	/* In case of Transmission failure of Chunk packet, PE is informed through CB
		& Chunk State machine is reset*/
	else if ((PRL_TX_ABORTED_ST == gasPRL[u8PortNum].u8TxStateISR) || \
            (PRL_TX_FAILED_ST == gasPRL[u8PortNum].u8TxStateISR))
	{
	  	/* TCH_REPORT_ERROR - Report Error to Policy Engine. 
										TCH_REPORT_ERROR entered on condition 
											(Transmission Error) */
		PRL_TxOriginalCBfromCH (u8PortNum, gasPRL[u8PortNum].u8TxStateISR);
	}
    else
    {
        /* Do Nothing */
    }
}


/******************************************************************************************************/


/******************************************************************************************************/

void PRL_RCHChunkSMStateChange_RCHCB (UINT8 u8PortNum, UINT8 u8TimerID, UINT8 TxDoneSt, UINT8 TxAbortSt, UINT8 TxFailSt)
{
  	/* Callback for Chunk Request Tx Message*/
  
	if (PRL_TX_DONE_ST == gasPRL[u8PortNum].u8TxStateISR)
	{
	  	/* if GOODCRC is received, SenderRequestTimer is started*/
		gasChunkSM [u8PortNum].u8CAorChunkSMTimerID = PDTimer_Start (PRL_CHUNKSENDERRESPONSE_TIMEOUT_MS, 
																	 PRL_ChunkStateChange_TimerCB, 
																	 u8PortNum, 
																	 PRL_RCH_CHUNK_RECV_ERROR_ST);
		
		/* PRL_RCH_EXPECT_RESPONSE_CHUNK_WAIT_ST is assigned to wait for Chunk Request packet*/
		
		gasChunkSM [u8PortNum].u8ChunkState = PRL_RCH_EXPECT_RESPONSE_CHUNK_WAIT_ST;
	}
	else if ((PRL_TX_ABORTED_ST == gasPRL[u8PortNum].u8TxStateISR) || (PRL_TX_FAILED_ST == gasPRL[u8PortNum].u8TxStateISR))
	{
	  	/* Spec Ref: RCH_Report_Error - Report Error to Policy Engine 
										Entered on condition "Unexpected Chunk Number" */
		
	  	/* If at all, Request Chunk packet is fails. PE is informed*/
		gasChunkSM [u8PortNum].u8ChunkState = PRL_RCH_CHUNK_RECV_ERROR_ST;
		
	}
    else
    {
        /* Do Nothing */
    }
}


/******************************************************************************************************/

void PRL_ChunkStateChange_TimerCB (UINT8 u8PortNum, UINT8 u8ChunkState)
{
  	/* callback for SenderRequestTimer and SenderResponseTimer*/
	gasChunkSM [u8PortNum].u8ChunkState = u8ChunkState;

	 /* setting the Timer ID to Max value*/
    gasChunkSM [u8PortNum].u8CAorChunkSMTimerID = MAX_CONCURRENT_TIMERS;
	
	DEBUG_PRINT_PORT_STR (u8PortNum, "PRL: ChunkTimer time out \r\n");
	
	
}

/******************************************************************************************************/

void PRL_UpdateTotalChunkNumVar (UINT8 u8PortNum)
{
	/* Total Number of chunk to be sent or received is updated */
  	/* -1 is done as Chunk packet starts from Chunk 0 */
	gasChunkSM [u8PortNum].u8TotalChunkPkt = ((PRL_GET_DATA_SIZE(gasExtendedMsgBuff [u8PortNum].u16ExtendedMsgHeader) /
												   PRL_MAX_EXTN_MSG_CHUNK_LEN_IN_BYTES) - 1);
	if ((PRL_GET_DATA_SIZE(gasExtendedMsgBuff [u8PortNum].u16ExtendedMsgHeader) % PRL_MAX_EXTN_MSG_CHUNK_LEN_IN_BYTES))
	{
		gasChunkSM [u8PortNum].u8TotalChunkPkt++;
	}
}

/******************************************************************************************************/
void PRL_ResetChunkSM (UINT8 u8PortNum)
{
  	/* Chunk SM is reset Completely*/
	gasChunkSM [u8PortNum].u8EnableChunkSM = RESET_TO_ZERO;
	gasChunkSM [u8PortNum].u8ChunkNumExpectedOrSent = RESET_TO_ZERO;
	gasChunkSM [u8PortNum].u8TotalChunkPkt = RESET_TO_ZERO;
	gasChunkSM [u8PortNum].u16RxReceivedBytes = RESET_TO_ZERO;
	gasChunkSM [u8PortNum].u8ChunkState = PRL_CH_CHUNK_IDLE_ST;
    PRL_KillCAorChunkSMTimer (u8PortNum);  
}

/******************************************************************************************************/

void PRL_TxOriginalCBfromCH (UINT8 u8PortNum, UINT8 u8TxStateforCB)
{
    UINT32 u32pkdTmrIDTxSt;
    
    /* Interrupt enable disable is done as the call back is expected to be called in ISR*/
    MCHP_PSF_HOOK_DISABLE_GLOBAL_INTERRUPT();
  	/* Checks whether Tx Callback is not present */
    if (gasChunkSM [u8PortNum].pFnTxCallback != NULL)
    {
		/* PRL Txstate is assigned as PRL_TX_FAILED_ST & callback is called*/
        gasPRL [u8PortNum].u8TxStateISR = u8TxStateforCB;
        u32pkdTmrIDTxSt = gasChunkSM[u8PortNum].u32pkdTmrID_TxSt;
        gasChunkSM [u8PortNum].pFnTxCallback(u8PortNum, 
                                        LOBYTE(u32pkdTmrIDTxSt), 	/* Next PE state for PRL_TX_DONE_ST or PRL_TX_EOP_ST incase of HR */
										HIBYTE(u32pkdTmrIDTxSt), 	/* Next PE sub state for PRL_TX_DONE_ST or PRL_TX_EOP_ST incase of HR*/                                 
										LOBYTE(HIWORD(u32pkdTmrIDTxSt)),	/* Next PE state for PRL_TX_ABORT or TX_FAILED */
										HIBYTE(HIWORD(u32pkdTmrIDTxSt)));	/* Next PE sub state for PRL_TX_ABORT or TX_FAILED*/
		/* PRL Tx state is set back to PRL_TX_IDLE_ST after the callback*/
        gasPRL [u8PortNum].u8TxStateISR = PRL_TX_IDLE_ST;		
    }
    
    MCHP_PSF_HOOK_ENABLE_GLOBAL_INTERRUPT();
			
	/* Reset the CHUNK SM*/
    PRL_ResetChunkSM (u8PortNum);
	
}				
/******************************************************************************************************/

void PRL_RunChunkStateMachine (UINT8 u8PortNum)
{
	
    if (!gasChunkSM [u8PortNum].u8EnableChunkSM)
    {
	  	/* if Chunk SM is not enabled, Return*/
		return;
    }
	
	switch (gasChunkSM [u8PortNum].u8ChunkState)
	{
	  
		case PRL_CH_CHUNK_IDLE_ST:
		/* Common Idle state for RCH & TCH */
		  
		case PRL_RCH_EXPECT_RESPONSE_CHUNK_WAIT_ST:
		/* wait for Chunk response packet Reception */
		{
			break;
		}
		
		case PRL_RCH_SEND_CHUNK_REQUEST_ST:
		{
		  	DEBUG_PRINT_PORT_STR (u8PortNum, "PRL: Chunk SM PRL_RCH_SEND_CHUNK_REQUEST_ST\r\n");
            
            /* Check Abort flag before sending chunk Request*/
            if (gasChunkSM [u8PortNum].u8AbortFlag)
            {
                /* Set the abort state*/
                gasChunkSM [u8PortNum].u8ChunkState = PRL_CH_ABORT_ST;
            }
            else
            {
		  	
                /* Chunk Request to receive next chunk Response is transmitted*/
			
                /* Extended Message Header is formed with Chunk number excepted (u8ChunkNumExpectedOrSent).
				Message header is formed with received Message header stored (gasExtendedMsgBuff [u8PortNum].u16Header)
				and Number of data object applicable for Request packet
				Extended message Header & Message Header is combined as UINT32 & used in PRL_TransmitMsg */
			
                if(PRL_RET_TX_MSG_TRANSMITTED_ON_LINE == PRL_TransmitMsg (u8PortNum,
															gasExtendedMsgBuff [u8PortNum].u8SOPtype, 	/* SOP Type */
															PRL_FormRequestChunkMsgHeader(u8PortNum),	/* Extended Msg Header + Msg Header*/
															NULL, 										/* no data byte*/
															PRL_RCHChunkSMStateChange_RCHCB,			/* Callback*/
															PRL_BUILD_PKD_TXST_U32 (
																PRL_RCH_EXPECT_RESPONSE_CHUNK_WAIT_ST,	/* Tx_DONE Chunk st*/
											 					NULL,									
											  					PRL_RCH_CHUNK_RECV_ERROR_ST,			/* Tx_FAILED Chunk state*/
											  					NULL)))
                {
					/* Next CH SM state is assigned*/
                    gasChunkSM [u8PortNum].u8ChunkState = PRL_RCH_WAIT_FOR_CHUNK_REQUEST_STATUS_ST;
                }
                else
                {
				  	/* if message is not transmitted on line PRL_RCH_CHUNK_RECV_ERROR_ST is assigned*/
                    gasChunkSM [u8PortNum].u8ChunkState = PRL_RCH_CHUNK_RECV_ERROR_ST;
                }
			
            }
            break;
        }   /* end of case PRL_RCH_SEND_CHUNK_REQUEST_ST */
		
		case PRL_RCH_WAIT_FOR_CHUNK_REQUEST_STATUS_ST:
		/* Idle state waiting for GOODCRC or Transmission error for the Request Chunk sent */
		{
			break;
		}
		
		case PRL_RCH_CHUNK_RECV_ERROR_ST:
		{
		  	/* Error bit is set to indicate policy engine*/
		  	gasPRL [u8PortNum].u8RxError = PRL_RX_CHUNK_RCV_ERROR;
		  	PRL_ResetChunkSM (u8PortNum);
			break;
		}
		
		case PRL_TCH_SEND_RESPONSE_CHUNK_ST:
		{
            /* Check the Abort flag before sending the Response*/
            if (gasChunkSM [u8PortNum].u8AbortFlag)
            {
                /* Set the abort state*/
                gasChunkSM [u8PortNum].u8ChunkState = PRL_CH_ABORT_ST;
                
            }
            else
            {
		  	    /* TCH state to send Chunk packet*/
		  	    
		  	    /* Extended Header is updated for Current Chunk packet to be sent */
                UINT8 u8ChunkNumber	= gasChunkSM[u8PortNum].u8ChunkNumExpectedOrSent;
			    /*Extended message is updated and stored*/
                UINT32 u32CombinedMessageHeader = PRL_UPDATE_EXT_MSG_HEADER_CHUNK_NUMBER(
			    									gasExtendedMsgBuff [u8PortNum].u16ExtendedMsgHeader,
			    									u8ChunkNumber);
                u32CombinedMessageHeader = PRL_FORM_COMBINED_MSG_HEADER((UINT16)u32CombinedMessageHeader,
			    									(gasExtendedMsgBuff [u8PortNum].u16Header));			/* Extended Msg Header + Msg Header*/
                DEBUG_PRINT_PORT_STR (u8PortNum, "PRL: Chunk SM PRL_TCH_SEND_RESPONSE_CHUNK_ST\r\n");
			    
			    /* TCH_CONSTRUCT_CHUNKED_MESSAGE - Construct Message Chunk and pass to Protocol Layer */
			    /* Chunk Response is transmitted*/			  
                if(PRL_RET_TX_MSG_TRANSMITTED_ON_LINE == PRL_TransmitMsg (u8PortNum,
			    					gasExtendedMsgBuff [u8PortNum].u8SOPtype, 					/* SOP Type */
			    					u32CombinedMessageHeader,									/* message header*/
			    					&gasExtendedMsgBuff[u8PortNum].u8Data[
			    					  (u8ChunkNumber * PRL_MAX_EXTN_MSG_CHUNK_LEN_IN_BYTES)], 	/* Corresponding Chunk's Data*/
			    					PRL_TCHChunkSMStateChange_TCHCB,							/* Callback*/
			    					PRL_BUILD_PKD_TXST_U32 (MAX_CONCURRENT_TIMERS,				/* u32PkdPEstOnTxStatus*/
			    												PRL_TCH_EXPECT_CHUNK_REQUEST_WAIT_ST,	/* Tx_DONE Chunk state*/
			    												PRL_TCH_CHUNK_TX_ERROR_ST,				/* Tx_FAILED Chunk state*/
			    												PRL_TCH_CHUNK_TX_ERROR_ST)))			/* Tx_ABORT Chunk state*/
                {
			    	/* Assigning PRL_TCH_WAIT_FOR_REQUEST_CHUNK_STATUS_ST state*/
                    gasChunkSM [u8PortNum].u8ChunkState = PRL_TCH_WAIT_FOR_REQUEST_CHUNK_STATUS_ST;
                }
                else
                {
			      	/* If message is not transmitted on line, PRL_TCH_CHUNK_TX_ERROR_ST error state is assigned*/
                    gasChunkSM [u8PortNum].u8ChunkState = PRL_TCH_CHUNK_TX_ERROR_ST;
                }
            } /* end of PRL_TCH_SEND_RESPONSE_CHUNK_ST case*/
            break;
        }
		
		case PRL_TCH_CHUNKSENDERREQUEST_TIMEOUT_ST:
		{
		  	if (PRL_FIRST_CHUNK_PACKET == gasChunkSM [u8PortNum].u8ChunkNumExpectedOrSent)
            {
				/*Spec Ref: TCH Shall transition to the TCH_Message_Sent state when
			  	ChunkSenderRequestTimer has expired and Chunk Number equals zero.*/
			  	PRL_TxOriginalCBfromCH (u8PortNum, PRL_TX_DONE_ST);
            }
            else
            {
			  	/* Spec Ref: ChunkSenderRequestTimer has expired and Chunk Number is greater than zero.
							TCH transit to TCH_Report_Error*/
                gasChunkSM [u8PortNum].u8ChunkState = PRL_TCH_CHUNK_TX_ERROR_ST;
            }
		  	
            break;	
        }	
		
		case PRL_TCH_CHUNK_TX_ERROR_ST:
		{
		  	DEBUG_PRINT_PORT_STR (u8PortNum, "PRL: Chunk SM PRL_TCH_CHUNK_TX_ERROR_ST\r\n");
			
		  	/* On any transmission error, PE is informed through the callback */
		  	PRL_TxOriginalCBfromCH (u8PortNum, PRL_TX_FAILED_ST);
			
			break;
		}
        
        case PRL_CH_ABORT_ST:
        {
            /* if abort flag is set, reset the Chunking SM */
            PRL_ResetChunkSM (u8PortNum);
            gasChunkSM [u8PortNum].u8AbortFlag = RESET_TO_ZERO;
            break;
        }
		
		case PRL_TCH_WAIT_FOR_REQUEST_CHUNK_STATUS_ST:
		/* Idle wait state till we receive a GOODCRC or Transmission Error for sent Chunk packet*/
		case PRL_TCH_EXPECT_CHUNK_REQUEST_WAIT_ST:
		/* Idle state waiting for Chunk Request to be received*/
		default:
		{
			break;
		}
	  
	}
	
}

/******************************************************************************************************/


UINT32 PRL_FormRequestChunkMsgHeader(UINT8 u8PortNum)
{						
	UINT32 u32CombinedHeader;
  	if (PRL_SOP_TYPE == gasExtendedMsgBuff [u8PortNum].u8SOPtype)
    {
		u32CombinedHeader = PRL_FormSOPTypeMsgHeader (u8PortNum, 
								  PRL_GET_MESSAGE_TYPE(gasExtendedMsgBuff [u8PortNum].u16Header),
								  PRL_REQUEST_CHUNK_DATA_OBJ_CNT, PRL_EXTENDED_MSG);
    }
    else
    {
	
		u32CombinedHeader = PRL_FormNonSOPTypeMsgHeader (u8PortNum, 
									PRL_GET_MESSAGE_TYPE(gasExtendedMsgBuff [u8PortNum].u16Header),
									 PRL_REQUEST_CHUNK_DATA_OBJ_CNT, PRL_EXTENDED_MSG);
    }
	
    u32CombinedHeader =   PRL_FORM_COMBINED_MSG_HEADER(PRL_FORM_REQUEST_CHUNK_EXT_MSG_HEADER(gasChunkSM[u8PortNum].u8ChunkNumExpectedOrSent), 
									(UINT16)u32CombinedHeader);
    return u32CombinedHeader;
}

/******************************************************************************************************/
void PRL_KillCAorChunkSMTimer (UINT8 u8PortNum)
{
    PDTimer_Kill (gasChunkSM [u8PortNum].u8CAorChunkSMTimerID);
    gasChunkSM [u8PortNum].u8CAorChunkSMTimerID = MAX_CONCURRENT_TIMERS;
}

/******************************************************************************************************/
#endif /*INCLUDE_PD_3_0*/

/******************************************************************************************************/

void PRL_ConfigureBISTCarrierMode (UINT8 u8PortNum, UINT8 u8BISTCarriermode)
{
	UINT8 u8OKToTx;
	if (u8BISTCarriermode)
	{
	  	/* Check whether message can sent on line*/
		do
		{
			u8OKToTx = UPD_RegReadByte (u8PortNum, PRL_TX_CTL_B);
		}while (!(u8OKToTx & PRL_TX_CTL_B_OK_TO_TX));
		
	  	/* BIST Tx is reset*/
		UPD_RegWriteByte (u8PortNum, PRL_BIST_CTL_B, PRL_BIST_CTL_B_BIST_TX_RST);
		/* BIST Carrier mode is enabled*/
		UPD_RegWriteByte (u8PortNum, PRL_BIST_CTL_A, (PRL_BIST_CTL_A_BIST_EN | PRL_BIST_TX_MODE_BIST_CARRIER_TEST_MODE_2));
		/* BIST Transmission is started*/
		UPD_RegWriteByte (u8PortNum, PRL_BIST_CTL_B, PRL_BIST_CTL_B_BIST_TX_START);
	}
	else
	{
	  	/* BIST Tx is reset*/
		UPD_RegWriteByte (u8PortNum, PRL_BIST_CTL_B, PRL_BIST_CTL_B_BIST_TX_RST);
		UPD_RegWriteByte (u8PortNum, PRL_BIST_CTL_A, RESET_TO_ZERO);
	}
}

/******************************************************************************************************/

void PRL_ChangeTxState(UINT8 u8PortNum, UINT8 u8TxStateISR)
{
    MCHP_PSF_HOOK_DISABLE_GLOBAL_INTERRUPT();
    gasPRL[u8PortNum].u8TxStateISR = u8TxStateISR;
    MCHP_PSF_HOOK_ENABLE_GLOBAL_INTERRUPT();
}

/******************************************************************************************************/


void PRL_ProtocolReset(UINT8 u8PortNum)
{
    /*Reset the UPD Protocol Layer for all SOPs*/
    PRL_ProtocolResetAllSOPs (u8PortNum);
     
    /* Disable the Tx interrupt*/
	UPD_RegWriteByte (u8PortNum, PRL_TX_IRQ_EN, CLR_VAL);
    
    /*Enable Receiver*/
    gasPRL[u8PortNum].u8RxDisable = FALSE; 
    
    /* Reset the PHY layer*/
    PRL_PHYLayerReset (u8PortNum);
        
}

/******************************************************************************************************/

void PRL_UpdateHWRetryCount(UINT8 u8PortNum, UINT8 u8HwnRetryCnt)
{
    UINT8 u8TxParamC = UPD_RegReadByte (u8PortNum, PRL_TX_PARAM_C); 
    u8TxParamC &= ~PRL_TX_PARAM_C_N_RETRY_CNT_FIELD_MASK;
    u8TxParamC |= (u8HwnRetryCnt << PRL_TX_PARAM_C_N_RETRY_CNT_BIT_POS);
    UPD_RegWriteByte (u8PortNum, PRL_TX_PARAM_C, u8TxParamC); 
}

/******************************************************************************************************/