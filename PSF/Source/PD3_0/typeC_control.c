/*******************************************************************************
 Type C layer source file

  Company:
    Microchip Technology Inc.

  File Name:
    typeC_control.c

  Description:
    This file contains functions to UPD hardware for Type C configuration, 
    TypeC Software State machine.
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

/*Default values given in the UPD350 datasheet to program the CC threshold registers*/
const UINT16 u16aCCThrVal[] = { TYPEC_CC_THR0_VAL, 
                                TYPEC_CC_THR1_VAL, 
                                TYPEC_CC_THR2_VAL,
                                TYPEC_CC_THR3_VAL,
                                TYPEC_CC_THR4_VAL, 
                                TYPEC_CC_THR5_VAL,
                                TYPEC_CC_THR6_VAL, 
                                TYPEC_CC_THR7_VAL                                  
                            };
/****************************************************************************************/
/*******************************************************************************************/
/*********************************TypeC Port Initialization**************************************/
/*******************************************************************************************/

#if(TRUE == INCLUDE_PD_DRP)
void TypeC_InitDRPPort (UINT8 u8PortNum)
{
    UINT16 u16Data;
    UINT8 u8MatchDebVal;
    
    /*Clearing UPD350 role to be advertised. Done in cronus*/
    UPD_RegByteClearBit (u8PortNum, TYPEC_CC_HW_CTL_LOW, TYPEC_DEV_ROLE);
    
    /*Setting CC Comparator OFF*/
    TypeC_ConfigCCComp (u8PortNum, TYPEC_CC_COMP_CTL_DIS);
    
    /*Setting VBUS Comparator OFF*/
    TypeC_SetVBUSCompONOFF (u8PortNum, TYPEC_VBUSCOMP_OFF);
 
    /*To check VBUS below vSafe0V*/
    UPD_RegByteSetBit (u8PortNum, TYPEC_DRP_CTL_HIGH, TYPEC_DRP_VSAFE0V_EN);
    
    /*Setting VBUS_DEB_TO_EN bit in VBUS Control 2 Register*/
    UPD_RegByteSetBit (u8PortNum, TYPEC_VBUS_CTL2, (TYPEC_VBUS_DEB_TO_EN | TYPEC_VBUS_DEB_BLK_EN));
    
    /*Setting Match debounce register value as 4 times the number of thresholds enabled 
    for debouncing*/
    u8MatchDebVal = (BYTE_LEN_4 * TYPEC_DRP_CCTHRES_CNT);
    /*Setting Match debounce register */
    UPD_RegisterWrite (u8PortNum, TYPEC_MATCH_DEB, &u8MatchDebVal, BYTE_LEN_1);	
    
    u16Data = DPM_GET_CONFIGURED_SOURCE_RP_VAL(u8PortNum);
    
    TypeC_DRPSetCCSampleEnable (u8PortNum, u16Data);
    
    /*FW shall mask the CC1_MATCH_CHG, CC2_MATCH_CHG and CC_MATCH_VLD interrupts to
    prevent an interrupt due to Ra/Open or Open/Ra detection.*/  
    UPD_RegByteClearBit (u8PortNum, TYPEC_CC_INT_EN,\
                      (UINT8)(TYPEC_CC1_MATCH_CHG | TYPEC_CC2_MATCH_CHG | TYPEC_CC_MATCH_VLD));	
    
    /*Setting VBUS Debounce enable clear register and VBUS Match Enable Register */
    u16Data = TYPEC_VBUS_VSAFE0V_MATCH;
	UPD_RegisterWrite (u8PortNum, TYPEC_VBUS_MATCH_EN, (UINT8*)&u16Data, BYTE_LEN_1);
    UPD_RegisterWrite (u8PortNum, TYPEC_VBUS_DBCLR_EN,	(UINT8*)&u16Data, BYTE_LEN_1);
	UPD_RegWriteByte (u8PortNum, TYPEC_VBUS_SAMP_EN, u16Data);
    
    /*Clear Rp value in u8PortSts*/
    gasTypeCcontrol[u8PortNum].u8PortSts &= ~TYPEC_CURR_RPVAL_MASK;

    if (FALSE == (gasCfgStatusData.sPerPortData[u8PortNum].u32ClientRequest & DPM_CLIENT_REQ_PORT_DISABLE))
    {
        /*FW programs DRP Pull-Down Value (DRP_PD_VAL) in DRP Control Register. 
        This value will be advertised when DRP advertises Sink capabilities.*/
        UPD_RegByteClearBit (u8PortNum, TYPEC_DRP_CTL_LOW, TYPEC_DRP_PD_VAL_MASK);
        UPD_RegByteSetBit (u8PortNum, TYPEC_DRP_CTL_LOW, TYPEC_DRP_PD_VAL_TRIMMED_RD);   

        /*FW programs DRP Current Advertisement (DRP_CUR_ADV) in DRP Control Register. This value
        will be advertised when DRP advertises Source capabilities.*/
        u16Data = DPM_GET_CONFIGURED_SOURCE_RP_VAL(u8PortNum);

        UPD_RegByteSetBit (u8PortNum, TYPEC_DRP_CTL_LOW, (u16Data << TYPEC_DRP_RP_POS));

        /*Setting the Current Rp value status in u8PortSts variable as user given Rp value*/
        gasTypeCcontrol[u8PortNum].u8PortSts |= (u16Data << TYPEC_CURR_RPVAL_POS);
    }
    else
    {
        /*FW Disables Rd resistor.*/
        UPD_RegByteClearBit (u8PortNum, TYPEC_DRP_CTL_LOW, TYPEC_DRP_PD_VAL_MASK);
        UPD_RegByteSetBit (u8PortNum, TYPEC_DRP_CTL_LOW, TYPEC_DRP_PD_VAL_OPEN_DIS);   

        /*Clear DRP_RP value to disable it*/
        UPD_RegByteClearBit (u8PortNum, TYPEC_DRP_CTL_LOW, TYPEC_DRP_RP_VAL_MASK);        
    }
    
    /*FW sets the LFSR Enable (LFSR_EN) in DRP Control Register to enable the LFSR generation.
    When this bit is set the internal LFSR is enabled and updates at 10 KHz.*/
    UPD_RegByteSetBit (u8PortNum, TYPEC_DRP_CTL_LOW, TYPEC_LFSR_EN);

    /*Enabling TYPEC_DRP_DONE interrupt in CC_INT_EN register */   
    UPD_RegByteSetBit (u8PortNum, TYPEC_CC_INT_EN, (UINT8)TYPEC_DRP_DONE);	
    
    /*Setting VBUS Comparator ON*/
    TypeC_SetVBUSCompONOFF (u8PortNum, TYPEC_VBUSCOMP_ON);
	
    /*Setting the VBUS to vSafe0V before entering the Source State machine*/
    DPM_DriveVBUS (u8PortNum, DPM_VBUS_OFF);
    
    gasTypeCcontrol[u8PortNum].u8DRPLastAttachedState = PD_ROLE_DRP;
    
    if (FALSE == (gasCfgStatusData.sPerPortData[u8PortNum].u32ClientRequest & DPM_CLIENT_REQ_PORT_DISABLE))
    {
        gasTypeCcontrol[u8PortNum].u8TypeCState = (UINT8)TYPEC_UNATTACHED_SRC;
        gasTypeCcontrol[u8PortNum].u8TypeCSubState = TYPEC_UNATTACHED_SRC_WAIT_DRPDONE_SS;
    
        /*FW sets the DRP Enable (DRP_EN) in DRP Control Register to enable DRP offload.*/
        UPD_RegByteSetBit (u8PortNum, TYPEC_DRP_CTL_LOW, TYPEC_DRP_EN);
    
        /*Clearing BLK_PD_MSG again. Done in cronus*/
        UPD_RegByteClearBit (u8PortNum, TYPEC_CC_HW_CTL_HIGH, TYPEC_BLK_PD_MSG);

        /* Notify external DPM of DRP Port enabled event through a user defined call back*/
        (void)DPM_NotifyClient (u8PortNum, eMCHP_PSF_PORT_ENABLED);
    }
    else
    {
        gasTypeCcontrol[u8PortNum].u8TypeCState = TYPEC_DISABLED;
        gasTypeCcontrol[u8PortNum].u8TypeCSubState = TYPEC_DISABLED_ENTRY_SS; 
        
        /* Change Policy Engine state and sub-state to invalid state */
        gasPolicyEngine[u8PortNum].ePEState = ePE_INVALIDSTATE;
        gasPolicyEngine[u8PortNum].ePESubState = ePE_INVALIDSUBSTATE; 
    }
    
    //DEBUG_PRINT_PORT_STR (u8PortNum,"TYPEC: DRP Port Init Done\r\n");
}
#endif
/**************************************************************************************/
void TypeC_InitPort (UINT8 u8PortNum)
{
	UINT16 u16Data;
    UINT8 u8MatchDebVal;
    UINT8 u8VBUSDebVal; 
    
	/*Setting CC Comparator OFF*/
    TypeC_ConfigCCComp (u8PortNum, TYPEC_CC_COMP_CTL_DIS);
    
    /*Setting VBUS Comparator OFF*/
    TypeC_SetVBUSCompONOFF (u8PortNum, TYPEC_VBUSCOMP_OFF);
 
	/*Enabling the CC Interrupts in CC_INT_EN register (TYPEC_CC_MATCH_VLD, TYPEC_CC1_MATCH_CHG and 
    TYPEC_CC2_MATCH_CHG interrupt)*/   
    UPD_RegByteSetBit (u8PortNum, TYPEC_CC_INT_EN,\
                      (UINT8)(TYPEC_CC1_MATCH_CHG | TYPEC_CC2_MATCH_CHG | TYPEC_CC_MATCH_VLD));	
    
    /* Setting VSinkDisconnect value in VBUS THR0*/
    UPD_RegWriteWord (u8PortNum, TYPEC_VBUS_THR0, \
      (UINT16)((float)TYPEC_VSINKDISCONNECT_THR * gasTypeCcontrol[u8PortNum].fVBUSCorrectionFactor));
		
	/*Setting VBUS_BLK_EN bit in VBUS Control 2 Register*/
    UPD_RegByteSetBit (u8PortNum, TYPEC_VBUS_CTL2, TYPEC_VBUS_DEB_BLK_EN);
 
	switch (DPM_GET_CURRENT_POWER_ROLE(u8PortNum))
    {
      
#if (TRUE == INCLUDE_PD_SOURCE)
        case PD_ROLE_SOURCE:
		{               
            /*Setting Match debounce register value as 4 times the number of thresholds enabled for 
            debouncing*/
            u8MatchDebVal = (BYTE_LEN_4 * TYPEC_SRC_CCTHRES_CNT);
            
            /*Setting VBUS Debounce period in VBUS Debounce Register as 2 times the number of 
            thresholds enabled for debouncing */
            u8VBUSDebVal = (BYTE_LEN_4 * TYPEC_SRC_VBUSTHRES_CNT);         
            					
            /*Write CCx Debounce clear enable register, CCx Match Enable register, 
            CCx sample enable register*/            
            TypeC_SetCCDebounceVariable (u8PortNum, DPM_GET_CONFIGURED_SOURCE_RP_VAL(u8PortNum));
            
            /*Setting Port Role as DFP in TYPEC_CC_HW_CTL register*/
            TypeC_SetCCDeviceRole (u8PortNum,PD_ROLE_DFP);
            
            /* Clear TypeC Current Rp Val bits in u8PortSts*/
            gasTypeCcontrol[u8PortNum].u8PortSts &= ~TYPEC_CURR_RPVAL_MASK;
            
            if (FALSE == (gasCfgStatusData.sPerPortData[u8PortNum].u32ClientRequest & DPM_CLIENT_REQ_PORT_DISABLE))
            {      
	            /*Setting Rp Current Source as user given and Rd as Open disconnect in both CC1 and 
	            CC2*/ 
	            u16Data = DPM_GET_CONFIGURED_SOURCE_RP_VAL(u8PortNum);
            
	            /*Setting the Power role as Source*/
	            TypeC_SetCCPowerRole (u8PortNum, TYPEC_ROLE_SOURCE, (UINT8) u16Data, TYPEC_ENABLE_CC1_CC2);
            
	            /*Setting the Current Rp value status in u8PortSts variable as user given Rp value*/
	            gasTypeCcontrol[u8PortNum].u8PortSts |= (u16Data << TYPEC_CURR_RPVAL_POS);
            
	            /*Setting the Initial State and Sub state for Source*/
	            gasTypeCcontrol[u8PortNum].u8TypeCState = TYPEC_UNATTACHED_SRC;
	            gasTypeCcontrol[u8PortNum].u8TypeCSubState = TYPEC_UNATTACHED_SRC_INIT_VSAFE0V_SS;
            }
            else
            {
                /*Setting the CC1 and CC2 line as Open Disconnect*/
                TypeC_SetCCPowerRole (u8PortNum, PD_ROLE_SOURCE, TYPEC_ROLE_SOURCE_OPEN_DIS, TYPEC_ENABLE_CC1_CC2);
                
                gasTypeCcontrol[u8PortNum].u8TypeCState = TYPEC_DISABLED;
                gasTypeCcontrol[u8PortNum].u8TypeCSubState = TYPEC_DISABLED_ENTRY_SS;
                
                /* Change Policy Engine state and sub-state to invalid state */
                gasPolicyEngine[u8PortNum].ePEState = ePE_INVALIDSTATE;
                gasPolicyEngine[u8PortNum].ePESubState = ePE_INVALIDSUBSTATE;
            }
            break;
        }
            
#endif /*INCLUDE_PD_SOURCE*/
        
#if (TRUE == INCLUDE_PD_SINK)
		case PD_ROLE_SINK:
		{          
            /*Setting Match debounce register value as 4 times the number of thresholds enabled 
            for debouncing*/
            u8MatchDebVal = (BYTE_LEN_4 * TYPEC_SNK_CCTHRES_CNT);
            
            /*Setting VBUS Debounce period in VBUS Debounce Register as as 2 times the number of 
            thresholds enabled for debouncing */
            u8VBUSDebVal = (BYTE_LEN_4 * TYPEC_SNK_VBUSTHRES_CNT);
          	
			TypeC_SetCCDebounceVariable (u8PortNum, TYPEC_UFP); 
            
			/*Setting Port Role as UFP in TYPEC_CC_HW_CTL register */
            TypeC_SetCCDeviceRole (u8PortNum, PD_ROLE_UFP);
            
            if (FALSE == (gasCfgStatusData.sPerPortData[u8PortNum].u32ClientRequest & DPM_CLIENT_REQ_PORT_DISABLE))
            {
	            /*Setting the Rd Value */ 
	            TypeC_SetCCPowerRole (u8PortNum, TYPEC_ROLE_SINK, TYPEC_ROLE_SINK_RD, TYPEC_ENABLE_CC1_CC2);
          
	            /*Setting the Initial State and Sub-state for Sink*/
	            gasTypeCcontrol[u8PortNum].u8TypeCState = TYPEC_UNATTACHED_SNK;
	            gasTypeCcontrol[u8PortNum].u8TypeCSubState = TYPEC_UNATTACHED_SNK_ENTRY_SS; 
            }
            else
            {
			    /*Setting the CC1 and CC2 line as Open Disconnect*/
            	TypeC_SetCCPowerRole (u8PortNum, TYPEC_ROLE_SINK, TYPEC_ROLE_SINK_OPEN_DIS, TYPEC_ENABLE_CC1_CC2);
                
                gasTypeCcontrol[u8PortNum].u8TypeCState = TYPEC_DISABLED;
                gasTypeCcontrol[u8PortNum].u8TypeCSubState = TYPEC_DISABLED_ENTRY_SS;
                
                /* Change Policy Engine state and sub-state to invalid state */
                gasPolicyEngine[u8PortNum].ePEState = ePE_INVALIDSTATE;
                gasPolicyEngine[u8PortNum].ePESubState = ePE_INVALIDSUBSTATE;
            }
            break;
         }
           
#endif/*INCLUDE_PD_SINK*/
			
		default:
		break;  
	}

    /*Setting Match debounce register */
    UPD_RegisterWrite (u8PortNum, TYPEC_MATCH_DEB, &u8MatchDebVal, BYTE_LEN_1);	
    
    /*Setting VBUS Debounce period in VBUS Debounce Register*/
    UPD_RegisterWrite (u8PortNum, TYPEC_VBUS_DEB, &u8VBUSDebVal, BYTE_LEN_1);
    
    /*Setting the CC Debounce register to detect both the sink and powered cable on CC1 and CC2*/
    TypeC_SetCCSampleEnable (u8PortNum, TYPEC_ENABLE_CC1_CC2);
  
    /*Setting VCONN Debounce register for 2ms as given in DOS*/
    UPD_RegWriteByte (u8PortNum, TYPEC_VCONN_DEB, gasCfgStatusData.sPerPortData[u8PortNum].u8VCONNOCSDebounceInms);
     
    
    /*Setting VBUS Debounce enable clear register and VBUS Match Enable Register */
	u16Data = (TYPEC_VBUS_VSAFE0V_MATCH | TYPEC_VBUS_THRES0_MATCH | TYPEC_VBUS_THRES1_MATCH |\
                    TYPEC_VBUS_THRES2_MATCH | TYPEC_VBUS_THRES3_MATCH | TYPEC_VBUS_THRES4_MATCH);
	UPD_RegisterWrite (u8PortNum, TYPEC_VBUS_MATCH_EN, (UINT8*)&u16Data, BYTE_LEN_1);
	UPD_RegisterWrite (u8PortNum, TYPEC_VBUS_DBCLR_EN, (UINT8*)&u16Data, BYTE_LEN_1);
    
    /*Setting VBUS Comparator ON*/
    TypeC_SetVBUSCompONOFF (u8PortNum, TYPEC_VBUSCOMP_ON);

#if(TRUE == INCLUDE_PD_DRP)
    /*Disable DRP offload.*/
    UPD_RegByteClearBit (u8PortNum, TYPEC_DRP_CTL_LOW, TYPEC_DRP_EN);
#endif

    /*Setting CC Comparator ON*/
    TypeC_ConfigCCComp (u8PortNum, TYPEC_CC_COMP_CTL_CC1_CC2);    
    
    /*Setting the Power Module as per the port role*/
    if (PD_ROLE_SOURCE == DPM_GET_CURRENT_POWER_ROLE(u8PortNum))
    {
        /*Setting the VBUS to vSafe0V before entering the State machine*/
        #if (TRUE == INCLUDE_PD_SOURCE)
            DPM_DriveVBUS (u8PortNum, DPM_VBUS_OFF);
        #endif 
    }
    else
    {
        /*Disable the Sink circuitry to stop sinking the power from source*/
        #if (TRUE == INCLUDE_PD_SINK)        
            PWRCTRL_ConfigSinkHW (u8PortNum, TYPEC_VBUS_0V, gasDPM[u8PortNum].u16SinkOperatingCurrInmA);
        #endif
    }
    //DEBUG_PRINT_PORT_STR (u8PortNum,"TYPEC: Port Init Done\r\n");             
}

void TypeC_GenericInitPort (UINT8 u8PortNum)
{
    UINT16 u16Data;
    
    /*Setting PM V2I Enable bit to enable the current source*/
    UPD_RegByteSetBit (u8PortNum, UPD_TRIM_ZTC_BYTE_3, UPD_PM_V2I_ENABLE);
    
    /*Setting the MATCH_DB_UNITS bit in CC_HW_CTL register*/
    UPD_RegByteSetBit (u8PortNum, TYPEC_CC_HW_CTL_HIGH, TYPEC_MATCH_DB_UNITS);
    
    /*Setting the CC_SAMP_CLK bit as 0. Sampling period will be 100us*/
	UPD_RegWriteByte (u8PortNum, UPD_CC_SAMP_CLK, \
      (UINT8) (UPD_CC_SAMP_GEN_10_KS | UPD_CC_CLK_20_KHZ));
    
    /*Setting the VBUS_DB_UNITS bit in VBUS_CTL1 register*/
    UPD_RegByteSetBit (u8PortNum, TYPEC_VBUS_CTL1_HIGH, TYPEC_VBUS_DB_UNITS);

    /*Setting the VBUS_SAMP_CLK as 0. Sampling period will be 100us*/
	UPD_RegWriteByte (u8PortNum, UPD_VBUS_SAMP_CLK,\
                     (UINT8)(UPD_VBUS_SAMP_GEN_10_KS | UPD_VBUS_CLK_20_KHZ));
         
    
    /*Enable the VBUS interrupt (VBUS_MATCH_VLD interrupt) and VCONN OCS interrupt*/
    UPD_RegByteSetBit (u8PortNum, TYPEC_PWR_INT_EN,\
                       (TYPEC_VBUS_MATCH_VLD | TYPEC_VCONN_OVER_CURR_ERR));
    
    /*Setting the UPD350 high level interrupt register for CC interrupt, VBUS interrupt and Power 
    interrupt*/
	u16Data = ((UPD_RegReadWord (u8PortNum, UPDINTR_INT_EN)) | \
	  			(UPDINTR_CC_INT | UPDINTR_VBUS_INT | UPDINTR_PWR_INT | UPDINTR_PIO_INT)) ;
	UPD_RegWriteWord (u8PortNum, UPDINTR_INT_EN, u16Data);
		
	/*Clearing the TYPEC_MODE bit will set UPD350's Operating Mode as Companion mode */
    UPD_RegByteClearBit (u8PortNum, TYPEC_CC_HW_CTL_LOW, TYPEC_MODE);
    
    /*Setting the CC threshold registers for default values given in DOS*/
    UPD_RegisterWrite (u8PortNum, TYPEC_CC_THR_BASE, (UINT8 *)u16aCCThrVal, BYTE_LEN_16);
    
    /*Setting the default threshold values in VBUS VSAFE0V Threshold register*/
	UPD_RegWriteWord (u8PortNum, TYPEC_VBUS_THR_VSAFE0V, \
      (UINT16)((float)TYPEC_VSAFE0V_VBUS_THR * gasTypeCcontrol[u8PortNum].fVBUSCorrectionFactor));
  
}
/*******************************************************************************************/
/*********************************TypeC State machine**************************************/
/*******************************************************************************************/
void TypeC_RunStateMachine (UINT8 u8PortNum)
{
    UINT8 u8CC1MatchISR = SET_TO_ZERO;
#if (TRUE == INCLUDE_PD_SOURCE)
    UINT8 u8CC2MatchISR  = SET_TO_ZERO;
#endif
    
    UINT8 u8IntStsISR = SET_TO_ZERO;
    
    /*Handle CC and VBUS Interrupt if any*/
    TypeC_CCVBUSIntrHandler (u8PortNum);
    
    MCHP_PSF_HOOK_DISABLE_GLOBAL_INTERRUPT();

    u8CC1MatchISR = gasTypeCcontrol[u8PortNum].u8CC1MatchISR;
#if (TRUE == INCLUDE_PD_SOURCE)
    u8CC2MatchISR = gasTypeCcontrol[u8PortNum].u8CC2MatchISR;
#endif
    
    u8IntStsISR = gasTypeCcontrol[u8PortNum].u8IntStsISR;

    MCHP_PSF_HOOK_ENABLE_GLOBAL_INTERRUPT();
    
    switch (gasTypeCcontrol[u8PortNum].u8TypeCState)
    {      
        case TYPEC_INIT:
        {
            TypeC_InitPort (u8PortNum);

            PRL_UpdatePowerRole (u8PortNum);
            
            break;
        }
      /*-----------------------------------------TypeC Source States------------------------------*/
#if (TRUE == INCLUDE_PD_SOURCE)
      
        case TYPEC_UNATTACHED_SRC:
        {          
            switch (gasTypeCcontrol[u8PortNum].u8TypeCSubState)
            {
#if(TRUE == INCLUDE_PD_DRP)
                case TYPEC_UNATTACHED_SRC_WAIT_DRPDONE_SS:
                {
                    /*Wait here initially until a partner is found*/
                    if (DPM_GET_CURRENT_POWER_ROLE(u8PortNum) != PD_ROLE_DRP)
                    {
                        gasTypeCcontrol[u8PortNum].u8TypeCSubState = TYPEC_UNATTACHED_SRC_INIT_VSAFE0V_SS;   
                    }
                    break;
                }
#endif
                case TYPEC_UNATTACHED_SRC_INIT_VSAFE0V_SS:
                {
                    /* Check for VBUS VSafe0V for Voltage driven in TypeC Initialization */
                    if (TYPEC_VSAFE_0V_MATCH_VAL == UPD_RegReadByte (u8PortNum, TYPEC_VBUS_MATCH))
                    {
                        //DEBUG_PRINT_PORT_STR (u8PortNum,"TYPEC_UNATTACHED_SRC_INIT_VSAFE0V_SS\r\n");
                        
                        PWRCTRL_ConfigVBUSDischarge (u8PortNum, FALSE);
                        
                        gasTypeCcontrol[u8PortNum].u8TypeCSubState = TYPEC_UNATTACHED_SRC_ENTRY_SS;                        
                    }
                    else
                    {
                        /* Hook to notify PE state machine entry into idle sub-state */
                        MCHP_PSF_HOOK_NOTIFY_IDLE(u8PortNum, eIDLE_TYPEC_NOTIFY);                                                
                    }
                    break;
                }
                
                case TYPEC_UNATTACHED_SRC_ENTRY_SS:
                {     
                    //DEBUG_PRINT_PORT_STR (u8PortNum,"TYPEC_UNATTACHED_SRC_ENTRY_SS\r\n");
                    
                    /* Disable the receiver*/
                    PRL_EnableRx (u8PortNum, FALSE);                                                             
                    
                    /*Check whether the current Rp value is same as the default Rp value*/
                    if (((gasTypeCcontrol[u8PortNum].u8PortSts & TYPEC_CURR_RPVAL_MASK) \
                        >> TYPEC_CURR_RPVAL_POS) != DPM_GET_CONFIGURED_SOURCE_RP_VAL(u8PortNum))
                    {     
                        /*Power down the CC comparator*/
                        TypeC_ConfigCCComp (u8PortNum, TYPEC_CC_COMP_CTL_DIS);
                        
                        /*Setting the user given Rp value since it is changed by collision 
                        avoidance*/
                        TypeC_SetCCDefaultRpValue (u8PortNum); 
                        
                        /*Set the CC Comparator to sample both CC1 and CC2*/
                        TypeC_ConfigCCComp (u8PortNum, TYPEC_CC_COMP_CTL_CC1_CC2);   
                        
                        /*DEBUG_PRINT_PORT_STR (u8PortNum,"TYPEC: RP Value of Source set to"\
                                             "User given Value\r\n"); */                        
                    }                
                    
#if(TRUE == INCLUDE_PD_DRP)
                    /*Disable DC_DC_EN if DRP does not act as source*/
                    PWRCTRL_ConfigDCDCEn (u8PortNum, FALSE);
                        
                    if ((PD_ROLE_DRP == DPM_GET_DEFAULT_POWER_ROLE(u8PortNum)) && \
                         (PD_ROLE_DRP != gasTypeCcontrol[u8PortNum].u8DRPLastAttachedState))
                    {                         
                        /*Setting CC Comparator OFF*/
                        TypeC_ConfigCCComp (u8PortNum, TYPEC_CC_COMP_CTL_DIS);

                        /*Mask CC match interrupts to avoid interrupts due to Ra/Open termination*/
                        UPD_RegByteClearBit (u8PortNum, TYPEC_CC_INT_EN,\
                        (UINT8)(TYPEC_CC1_MATCH_CHG | TYPEC_CC2_MATCH_CHG | TYPEC_CC_MATCH_VLD));	

                        
                        if (PD_ROLE_SOURCE == gasTypeCcontrol[u8PortNum].u8DRPLastAttachedState)
                        {
                            TypeC_SetCCPowerRole (u8PortNum, TYPEC_ROLE_SINK, TYPEC_ROLE_SINK_RD, TYPEC_ENABLE_CC1_CC2);

                            gasTypeCcontrol[u8PortNum].u8TypeCState = TYPEC_UNATTACHED_SNK;
                            gasTypeCcontrol[u8PortNum].u8TypeCSubState = TYPEC_UNATTACHED_SNK_ENTRY_SS;
                        }
                        else if (PD_ROLE_SINK == gasTypeCcontrol[u8PortNum].u8DRPLastAttachedState)
                        {
                            gasTypeCcontrol[u8PortNum].u8DRPLastAttachedState = PD_ROLE_DRP;

                            /*Set power and data role status*/
                            DPM_UpdatePowerRole (u8PortNum, PD_ROLE_DRP);
                            DPM_UpdateDataRole (u8PortNum, PD_ROLE_TOGGLING);

                            /*Enable DRP offload.*/
                            UPD_RegByteSetBit (u8PortNum, TYPEC_DRP_CTL_LOW, TYPEC_DRP_EN); 
                            
                            gasTypeCcontrol[u8PortNum].u8TypeCSubState = TYPEC_UNATTACHED_SRC_INIT_SS;
                        }
                        else
                        {
                            /* Do Nothing */
                        }                                              
                    }
                    else
#endif
                    {
                        gasTypeCcontrol[u8PortNum].u8TypeCSubState = TYPEC_UNATTACHED_SRC_INIT_SS;                                            
                    }                                       
                                        
                    break;
                }
				
                case TYPEC_UNATTACHED_SRC_INIT_SS:
                {
                    /*DPM power fault handler resets SM by polling this state 
                     TYPEC_UNATTACHED_SRC_INIT_SS in the handler for Src unattach.
                     Any module can reset its SM by polling this state*/                       
                    gasTypeCcontrol[u8PortNum].u8TypeCSubState = TYPEC_UNATTACHED_SRC_IDLE_SS;
                    
                    /* Notify external DPM of Port enabled event through a user defined call back*/
                    (void)DPM_NotifyClient (u8PortNum, eMCHP_PSF_PORT_ENABLED);

                    /* Notify external DPM of Type C Detach event through a user defined call back*/
                    (void)DPM_NotifyClient (u8PortNum, eMCHP_PSF_TYPEC_DETACH_EVENT);                        
                    
                    break;
                }
              
                /*Source waits in this sub-state for sink attachment*/
                case TYPEC_UNATTACHED_SRC_IDLE_SS:
                {
                    /* Hook to notify Type C state machine entry idle sub-state */
                    MCHP_PSF_HOOK_NOTIFY_IDLE (u8PortNum, eIDLE_TYPEC_NOTIFY);
                    
                    break; 
                }
                    
                default:
                    break;                    
            } 
            break;                
        }  
                    
        case TYPEC_ATTACHWAIT_SRC:
        {          
            switch (gasTypeCcontrol[u8PortNum].u8TypeCSubState)
		    {
                /*This SubState is used to start a tCCDebounce timer for Sink attachment*/
                case TYPEC_ATTACHWAIT_SRC_ENTRY_SS:
                {
                    //DEBUG_PRINT_PORT_STR (u8PortNum,"TYPEC_ATTACHWAIT_SRC_ENTRY_SS\r\n");                    
                    
                    #if(TRUE == INCLUDE_PD_DRP)
                    if (PD_ROLE_DRP == DPM_GET_DEFAULT_POWER_ROLE(u8PortNum))
                    {
                        gasTypeCcontrol[u8PortNum].u8DRPLastAttachedState = PD_ROLE_SOURCE;
                        /*Enable DC_DC_EN if DRP acts as source*/
                        /*For configurations other than DRP, DC_DC_EN is high by default*/
                        PWRCTRL_ConfigDCDCEn (u8PortNum, TRUE);
                    }
                    #endif   
                    gasTypeCcontrol[u8PortNum].u8TypeCTimerID = PDTimer_Start ( \
                                                      (TYPEC_TCCDEBOUNCE_TIMEOUT_MS),\
                                                      TypeC_SubStateChange_TimerCB, u8PortNum,\
                                                      TYPEC_ATTACHWAIT_SRC_CC_DEB_TIMEOUT_SS);
                   
				    gasTypeCcontrol[u8PortNum].u8TypeCSubState = TYPEC_ATTACHWAIT_SRC_IDLE_SS;
                    
                    break;
                }   
                    
                /*Source waits in this sub-state until the tCCDebounce timer timeouts*/
                case TYPEC_ATTACHWAIT_SRC_IDLE_SS:
                {
                    /* Hook to notify Type C state machine entry into idle sub-state */
                    MCHP_PSF_HOOK_NOTIFY_IDLE (u8PortNum, eIDLE_TYPEC_NOTIFY);
                    
                    break;
                }
                
                /*Source enters this sub-state after the tCCDebounce timer timeouts*/
                case TYPEC_ATTACHWAIT_SRC_CC_DEB_TIMEOUT_SS:
                {
                    /*VBUS should be at vSafe0V before transition to AttachedSRC State*/
                    /*Check for VBUS Absence before moving to Attached SRC state */
                    if (TYPEC_VBUS_0V_PRES == (u8IntStsISR & TYPEC_VBUS_PRESENCE_MASK))
                    {                     
                        /* Turn off VBUS Discharge */
                        PWRCTRL_ConfigVBUSDischarge (u8PortNum, FALSE);
                        
                        if (u8CC1MatchISR == u8CC2MatchISR)
                        {
                            /*Ra is present on both CC lines*/
                            if (u8CC1MatchISR == gasTypeCcontrol[u8PortNum].u8CCDebMatch)
                            {
                                gasTypeCcontrol[u8PortNum].u8TypeCState = TYPEC_AUDIO_ACCESSORY; 
                            }
                            /*Rd is present on both CC lines*/
                            else
                            {
                                gasTypeCcontrol[u8PortNum].u8TypeCState = TYPEC_UNORIENTED_DEBUG_ACCESSORY_SRC;
                            }              
                        }
                        /*Rd is present on either CC1 or CC2 line and Ra may be present on either CC1 or CC2 line*/
                        else
                        {
                            /*Set BLK_PD_MSG. Done in cronus*/
                            UPD_RegByteSetBit (u8PortNum, TYPEC_CC_HW_CTL_HIGH, TYPEC_BLK_PD_MSG);
                            
                            gasTypeCcontrol[u8PortNum].u8TypeCState = TYPEC_ATTACHED_SRC;
                            gasTypeCcontrol[u8PortNum].u8TypeCSubState = TYPEC_ATTACHED_SRC_DRIVE_PWR_SS;      
                        }                                                
                    }
                    else
                    {
                        /* Hook to notify Type C state machine entry into idle sub-state */
                        MCHP_PSF_HOOK_NOTIFY_IDLE (u8PortNum, eIDLE_TYPEC_NOTIFY);                        
                    }
                    break;
                }
                    
                default:
                    break;
             }
             break;
        }
            
        case TYPEC_ATTACHED_SRC:
        {          
            switch (gasTypeCcontrol[u8PortNum].u8TypeCSubState)
            {   
#if (TRUE == (INCLUDE_PD_PR_SWAP || INCLUDE_PD_FR_SWAP))
                /* This sub-state will be entered from 
                   Attached Sink state during Sink to Source PR_Swap/FR_Swap */
                case TYPEC_ATTACHED_SRC_SWAP_ASSERT_RP_SS:
                {
                    /* Initial Source would have turned off VBUS.
                       Before placing Rp, check if VBUS is at 0V */
                    UINT8 u8VBUSValueToCheck;
                    
                    /*If DPM_IS_FRS_XMT_OR_DET_ENABLED returns TRUE, and a swap is in progress,
                      the swap is an FR_Swap. */
                    if (TRUE == DPM_IS_FRS_XMT_OR_DET_ENABLED(u8PortNum))
                    {
                        u8VBUSValueToCheck = TYPEC_VBUS_5V_PRES;
                    }
                    else
                    {
                        u8VBUSValueToCheck = TYPEC_VBUS_0V_PRES;
                    }
                    
                    if (u8VBUSValueToCheck == (u8IntStsISR & TYPEC_VBUS_PRESENCE_MASK))
                    {        
                        //DEBUG_PRINT_PORT_STR (u8PortNum,"TYPEC_ATTACHED_SRC_SWAP_ASSERT_RP_SS\r\n");
                        
                        gasTypeCcontrol[u8PortNum].u8DRPLastAttachedState = PD_ROLE_SOURCE;

                        /* Disable VBUS Discharge which would have been 
                           enabled while detecting TYPEC_MAX_VSAFE_0V_VAL */
                        PWRCTRL_ConfigVBUSDischarge (u8PortNum, FALSE);  
                        
                        /* Set the Port Power role variable as Source, so that 
                           moving forward all the APIs would act upon the new power role */
                        DPM_UpdatePowerRole (u8PortNum, PD_ROLE_SOURCE);
                        
                        /* Enable DC_DC_EN pin for Source functionality */
                        PWRCTRL_ConfigDCDCEn (u8PortNum, TRUE);                                                
                        
                        /* CC Comparator would have been turned off in 
                         TYPEC_ATTACHED_SNK_SWAP_TRANS_TO_SRC_SS sub-state. No need
                         to do it here once again */
                        /* Configure the device role as DFP for the 
                           UPD350 to act as Source */
                        TypeC_SetCCDeviceRole (u8PortNum, PD_ROLE_DFP);
                        /* Get Rp value from user configuration */
                        UINT8 u8Data = DPM_GET_CONFIGURED_SOURCE_RP_VAL(u8PortNum);
                        /*Set the u8CCDebMatch and u8CCSrcSnkMatch variable accordingly as per Rp Value*/
                        TypeC_SetCCDebounceVariable (u8PortNum, u8Data);
                          
                        /* When transitioning to Source, it is needed to detect the presence 
                           of powered cable. So, irrespective of whether the port is VCONN 
                           Source or not, enable CC Debounce thresholds on both the CC pins */
                        /*Reset the CC Debounce clear enable,CC Match Enable,CC Sample Enable register*/                                                                    
                        TypeC_SetCCSampleEnable (u8PortNum, TYPEC_ENABLE_CC1_CC2);
                        
                        /* Turn on the CC Comparator */
                        TypeC_ConfigCCComp (u8PortNum, TYPEC_CC_COMP_CTL_CC1_CC2);
                        
                        /* Wait for CC RD match valid interrupt */
                        gasTypeCcontrol[u8PortNum].u8TypeCSubState = TYPEC_ATTACHED_SRC_SWAP_RD_PRES_DETECT_SS;                    
                    }
                    else
                    {
                        /* Hook to notify Type C state machine entry into idle sub-state */
                        MCHP_PSF_HOOK_NOTIFY_IDLE (u8PortNum, eIDLE_TYPEC_NOTIFY);                                                
                    }
                    break; 
                }
                /* Source waits in this sub-state to detect the presence of Sink */
                case TYPEC_ATTACHED_SRC_SWAP_RD_PRES_DETECT_SS:
                {                    
                    /* Hook to notify Type C state machine entry into idle sub-state */
                    MCHP_PSF_HOOK_NOTIFY_IDLE (u8PortNum, eIDLE_TYPEC_NOTIFY);                        
                        
                    break; 
                }
#endif 
                /*Source Drives VBUS and VCONN required in this sub-state*/
                case TYPEC_ATTACHED_SRC_DRIVE_PWR_SS:
                {
                    //DEBUG_PRINT_PORT_STR (u8PortNum,"TYPEC_ATTACHED_SRC_DRIVE_PWR_SS\r\n");
                    
                    /* Drive VBus for vSafe5V */
                    DPM_DriveVBUS (u8PortNum, DPM_VBUS_ON);
                    
                    /*Start the VBUS ON timer for monitoring the time taken for 
                    power module to reach vSafe5V*/
                    gasTypeCcontrol[u8PortNum].u8TypeCTimerID = PDTimer_Start (
                                                              (TYPEC_VBUS_ON_TIMER_MS),
                                                              DPM_VBUSOnOffOrVCONNOff_TimerCB, u8PortNum,  
                                                              (UINT8)SET_TO_ZERO);
					
					/*Sink Attached in CC1 pin*/
                    if (u8CC1MatchISR == gasTypeCcontrol[u8PortNum].u8CCSrcSnkMatch)
                    {
                        gasTypeCcontrol[u8PortNum].u8PortSts &= ~(TYPEC_CC_ATTACHED_ORIENTATION_MASK); 
                        UPD_RegByteClearBit (u8PortNum, TYPEC_CC_CTL1_HIGH, TYPEC_CC_COM_SEL); 
                    }
					/*Sink attached in CC2*/
                    else
                    {
                        gasTypeCcontrol[u8PortNum].u8PortSts |= TYPEC_CC_ATTACHED_ORIENTATION_MASK; 
                        UPD_RegByteSetBit (u8PortNum, TYPEC_CC_CTL1_HIGH, TYPEC_CC_COM_SEL);
                    } 
		   
                    /*Enable VCONN FETs if Powered cable is present. Do not 
                      disturb VCONN when a PR_Swap/FR_Swap is in progress */
                    if ((gasTypeCcontrol[u8PortNum].u8PortSts & TYPEC_PWDCABLE_PRES_MASK) && 
                            (FALSE == DPM_IS_PR_OR_FR_SWAP_IN_PROGRESS(u8PortNum)))
                    {                                        
                        /*Powered cable attached in CC1*/
                        if (u8CC1MatchISR > u8CC2MatchISR)
                        {                                               
                            /*DEBUG_PRINT_PORT_STR (u8PortNum,"TYPEC_ATTACHED_SRC: VCONN"\
                                                 "Enabled in CC1\r\n"); */                                                           
                        }
                        /*Powered cable attached in CC2*/
                        else
                        {                             
                            /*DEBUG_PRINT_PORT_STR (u8PortNum,"TYPEC_ATTACHED_SRC: VCONN"\
                                                 "Enabled in CC2\r\n");  */                                              
                        }
                        
                        /*Enable VCONN for powered cable attached in CC1 or CC2 line */
                        TypeC_EnabDisVCONN (u8PortNum, TYPEC_VCONN_ENABLE);  
                        
                        /* Set VCONN Source Responsibility status */
                        DPM_SET_VCONN_SRC_RESPONSIBILITY(u8PortNum); 
                    }

                    gasTypeCcontrol[u8PortNum].u8TypeCSubState = TYPEC_ATTACHED_SRC_WAIT_FOR_VBUS_ON_SS;                   
                                     
                    break;
                }
                
                case TYPEC_ATTACHED_SRC_WAIT_FOR_VBUS_ON_SS:
                {   
                    if (TYPEC_VBUS_5V_PRES == (u8IntStsISR & TYPEC_VBUS_PRESENCE_MASK))
                    {                      
                        //DEBUG_PRINT_PORT_STR(u8PortNum, "TYPEC_ATTACHED_SRC_WAIT_FOR_VBUS_ON_SS\r\n");
                        /*Kill the VBUS ON timer since vSafe5V is reached*/
                        TypeC_KillTypeCTimer (u8PortNum);
                                                              
                        if ((gasTypeCcontrol[u8PortNum].u8PortSts & TYPEC_PWDCABLE_PRES_MASK)
                                && (FALSE == DPM_IS_PR_OR_FR_SWAP_IN_PROGRESS(u8PortNum)))
                        {
                            /*Start the VCONN ON timer for monitoring the time taken for 
                            VCONN to reach its Min value*/
                            /*Failure to reach the VCONN Min will result in Error Recovery state*/
                            gasTypeCcontrol[u8PortNum].u8TypeCTimerID = PDTimer_Start (
                                                         TYPEC_VCONNON_TIMEOUT_MS,
                                                         DPM_VCONNONError_TimerCB, u8PortNum,  
                                                         (UINT8)SET_TO_ZERO);
                            
                            gasTypeCcontrol[u8PortNum].u8TypeCSubState = TYPEC_ATTACHED_SRC_WAIT_FOR_VCONN_ON_SS;
                        }
                        else
                        {
                            gasTypeCcontrol[u8PortNum].u8TypeCSubState = TYPEC_ATTACHED_SRC_SET_PRL_SS;
                        }                       
                    }
                    else
                    {
                        /* Hook to notify Type C state machine entry into idle sub-state */
                        MCHP_PSF_HOOK_NOTIFY_IDLE (u8PortNum, eIDLE_TYPEC_NOTIFY);                        
                    }
                    break;
                }
               
                case TYPEC_ATTACHED_SRC_WAIT_FOR_VCONN_ON_SS:
                {                          
                    if (u8IntStsISR & TYPEC_VCONN_SOURCE_MASK)
                    {                      
                        //DEBUG_PRINT_PORT_STR(u8PortNum, "TYPEC_ATTACHED_SRC_WAIT_FOR_VCONN_ON_SS\r\n");
                        
                        /*Kill the VCONN ON timer since VCONN is turned ON*/
                        TypeC_KillTypeCTimer (u8PortNum); 
                          
                        gasTypeCcontrol[u8PortNum].u8TypeCSubState = TYPEC_ATTACHED_SRC_SET_PRL_SS;                     
                    }  
                    else
                    {
                        /* Hook to notify Type C state machine entry into idle sub-state */
                        MCHP_PSF_HOOK_NOTIFY_IDLE (u8PortNum, eIDLE_TYPEC_NOTIFY);                        
                    }                    
                    break;
                }
                
                case TYPEC_ATTACHED_SRC_SET_PRL_SS:
                {                    
                    //DEBUG_PRINT_PORT_STR(u8PortNum, "TYPEC_ATTACHED_SRC_SET_PRL_SS\r\n");                    
                                    
                    /* Enabling PRL Rx */
                    PRL_EnableRx (u8PortNum, TRUE);                  

                    /* Enable Power Fault Threshold for TYPEC_VBUS_5V */
                    DPM_EnablePowerFaultDetection (u8PortNum);
                    
                    /* Enable AME Monitoring */
                    #if(TRUE == INCLUDE_PD_ALT_MODE)                    
                        DPM_EnableAMEMonitoring (u8PortNum);
                    #endif     
                    
                    gasTypeCcontrol[u8PortNum].u8TypeCSubState = TYPEC_ATTACHED_SRC_RUN_SM_SS;
                       
                    /*Sink Attached in CC1 pin*/
                    if (u8CC1MatchISR == gasTypeCcontrol[u8PortNum].u8CCSrcSnkMatch)
                    {
                        (void)DPM_NotifyClient (u8PortNum, eMCHP_PSF_TYPEC_CC1_ATTACH);
                    }
                    /*Sink attached in CC2*/
                    else
                    {
                        (void)DPM_NotifyClient (u8PortNum, eMCHP_PSF_TYPEC_CC2_ATTACH);
                    }
                    
                    break;                  
                }
                             
                case TYPEC_ATTACHED_SRC_RUN_SM_SS:
                case TYPEC_ATTACHED_SRC_IDLE_SS:
                {
                    /* Hook to notify Type C state machine entry into idle sub-state */
                    MCHP_PSF_HOOK_NOTIFY_IDLE (u8PortNum, eIDLE_TYPEC_NOTIFY);
                    
                    break;
                }
                    
#if (TRUE == (INCLUDE_PD_PR_SWAP || INCLUDE_PD_FR_SWAP))                
                case TYPEC_ATTACHED_SRC_SWAP_TRANS_TO_SNK_SS:
                {
                    /* This sub-state would be entered only during 
                       Power role swap from Source to Sink */
                    //DEBUG_PRINT_PORT_STR (u8PortNum,"TYPEC_ATTACHED_SRC_SWAP_TRANS_TO_SNK_SS\r\n");
                    
                    /* Power down the CC comparator */
                    TypeC_ConfigCCComp (u8PortNum, TYPEC_CC_COMP_CTL_DIS);
                    
                    /* Disable Rp terminations from both the CC pins and enable 
                       Rd only in the connected CC pin. 
                       Type C Spec Reference: 
                       Note: If the port has entered this state from the Attached.SRC state as 
                       the result of a USB PD PR_Swap/FR_Swap, the port shall terminate the 
                       connected CC pin to ground through Rd and monitor its state */                                        
                    if (TYPEC_ORIENTATION_CC1 == TYPEC_GET_CC_ORIENTATION_STS(u8PortNum))
                    {
                        TypeC_SetCCPowerRole (u8PortNum,TYPEC_ROLE_SINK, TYPEC_ROLE_SINK_RD, TYPEC_ENABLE_CC1);
                    }
                    else /* Attached Orientation - CC2 */
                    {
                        TypeC_SetCCPowerRole (u8PortNum,TYPEC_ROLE_SINK, TYPEC_ROLE_SINK_RD, TYPEC_ENABLE_CC2);
                    }            
                    
                    /* Move the Type C SM to Attached SNK state since the 
                       port is ready to act as Sink */
                    gasTypeCcontrol[u8PortNum].u8TypeCState = TYPEC_ATTACHED_SNK;
                    gasTypeCcontrol[u8PortNum].u8TypeCSubState = TYPEC_ATTACHED_SNK_SWAP_ASSERT_RD_SS;
                    
                    break; 
                }
#endif                 

                default:
                    break;
                
            }
            break;
      }
           
      case TYPEC_UNATTACHED_WAIT_SRC:
      {        
        switch (gasTypeCcontrol[u8PortNum].u8TypeCSubState)
		{   
            /*This SubState is used to start a tPDDeounce Software timer for sink detach event*/         
            case TYPEC_UNATTACHED_WAIT_SRC_ENTRY_SS:
            {                    
                //DEBUG_PRINT_PORT_STR (u8PortNum,"TYPEC_UNATTACHED_WAIT_SRC_ENTRY_SS\r\n"); 
                
                /*Kill all the timers for a port */
                PDTimer_KillPortTimers (u8PortNum);
                
                /*Disable VBUS by driving to vSafe0V*/
                DPM_DriveVBUS (u8PortNum, DPM_VBUS_OFF);
                
               /*Start the VBUS OFF timer for monitoring the time taken for 
                power module to reach vSafe0V*/
                gasTypeCcontrol[u8PortNum].u8TypeCTimerID = PDTimer_Start (
                                                              (TYPEC_VBUS_OFF_TIMER_MS),
                                                              DPM_VBUSOnOffOrVCONNOff_TimerCB, u8PortNum,  
                                                              (UINT8)SET_TO_ZERO);                		
                 
                /*Enabling the VCONN Discharge if we are supplying VCONN or VCONN Req was ON*/
                if (((u8IntStsISR & TYPEC_VCONN_SOURCE_MASK) != TYPEC_VCONN_DISABLED) \
                        || (gasTypeCcontrol[u8PortNum].u8PortSts & TYPEC_VCONN_ON_REQ_MASK))
                {                       
                    /*Disable VCONN by switching off the VCONN FETS*/
                    TypeC_EnabDisVCONN (u8PortNum, TYPEC_VCONN_DISABLE);
                    
                    /*Clearing the VCONN ON Request mask if VCONN has turned OFF*/
                    gasTypeCcontrol[u8PortNum].u8PortSts &= ~TYPEC_VCONN_ON_REQ_MASK;
                    
                    gasTypeCcontrol[u8PortNum].u8TypeCSubState = TYPEC_UNATTACHED_WAIT_SRC_WAIT_FOR_VCONN_OFF_SS;
                }
                else
                {
                    gasTypeCcontrol[u8PortNum].u8TypeCSubState = TYPEC_UNATTACHED_WAIT_SRC_WAIT_FOR_VBUS_OFF_SS;
                }
                
                break;
            }
                                
            /*Source waits in this SubState until the tPDDeounce Software timer expires or the VCONN
            Discharge is completed*/    
            case TYPEC_UNATTACHED_WAIT_SRC_IDLE_SS:  
            {
                /* Hook to notify Type C state machine entry into idle sub-state */
                MCHP_PSF_HOOK_NOTIFY_IDLE (u8PortNum, eIDLE_TYPEC_NOTIFY);
             
                break;
            }
             
            case TYPEC_UNATTACHED_WAIT_SRC_WAIT_FOR_VCONN_OFF_SS:
            {
                if (!(u8IntStsISR & TYPEC_VCONN_SOURCE_MASK))
                {
                    gasTypeCcontrol[u8PortNum].u8TypeCSubState = TYPEC_UNATTACHED_WAIT_SRC_WAIT_FOR_VBUS_OFF_SS;                    
                }
                else
                {
                    /* Hook to notify Type C state machine entry into idle sub-state */
                    MCHP_PSF_HOOK_NOTIFY_IDLE (u8PortNum, eIDLE_TYPEC_NOTIFY);                        
                }                
                break;
            }
            
            /*Wait in this sub-state until the VBUS goes to 0V before moving into Unattached state*/
            case TYPEC_UNATTACHED_WAIT_SRC_WAIT_FOR_VBUS_OFF_SS:
            {
                if (TYPEC_VBUS_0V_PRES == (u8IntStsISR & TYPEC_VBUS_PRESENCE_MASK))
                {              
                    /* Turn off VBUS Discharge */
                    PWRCTRL_ConfigVBUSDischarge (u8PortNum, FALSE);
                        
                    /*Kill the VBUS OFF timer since vSafe0V is reached*/
                    TypeC_KillTypeCTimer (u8PortNum);
                    
                    gasTypeCcontrol[u8PortNum].u8TypeCSubState = TYPEC_UNATTACHED_SRC_ENTRY_SS;
                    gasTypeCcontrol[u8PortNum].u8TypeCState = TYPEC_UNATTACHED_SRC;
                }
                else
                {
                    /* Hook to notify Type C state machine entry into idle sub-state */
                    MCHP_PSF_HOOK_NOTIFY_IDLE (u8PortNum, eIDLE_TYPEC_NOTIFY);                    
                }
                break; 
            }
                
            default:
                break;
        }
            break;
      }
        
        case TYPEC_UNORIENTED_DEBUG_ACCESSORY_SRC:      
            break;

#endif
/*INCLUDE_PD_SOURCE*/
        
       /*-----------------------------------------TypeC Sink States-------------------------------*/
#if (TRUE == INCLUDE_PD_SINK)
        
       case TYPEC_UNATTACHED_SNK:
       {          
            switch (gasTypeCcontrol[u8PortNum].u8TypeCSubState)
		    {             
                case TYPEC_UNATTACHED_SNK_ENTRY_SS: 
                {
                    //DEBUG_PRINT_PORT_STR(u8PortNum,"TYPEC_UNATTACHED_SNK_ENTRY_SS\r\n");
                    
#if (TRUE == INCLUDE_PD_DRP)
                    /*Setting the Rd Value */ 
                    TypeC_SetCCPowerRole (u8PortNum, TYPEC_ROLE_SINK, TYPEC_ROLE_SINK_RD, TYPEC_ENABLE_CC1_CC2);
#endif
                    
                    gasDPM[u8PortNum].u16SinkOperatingCurrInmA = DPM_0mA;
                                            
                    PWRCTRL_ConfigSinkHW (u8PortNum, TYPEC_VBUS_0V, \
                            gasDPM[u8PortNum].u16SinkOperatingCurrInmA);
         
                    PRL_EnableRx (u8PortNum, FALSE);      
                                        
#if(TRUE == INCLUDE_PD_DRP)                    
                    if ((PD_ROLE_DRP != gasTypeCcontrol[u8PortNum].u8DRPLastAttachedState) && \
                            (PD_ROLE_DRP == DPM_GET_DEFAULT_POWER_ROLE(u8PortNum)))
                    {                                                                        
#if (TRUE == INCLUDE_PD_FR_SWAP)
                        /*Disable DC_DC_EN which was turned on when enabling FRS detection */
                        PWRCTRL_ConfigDCDCEn (u8PortNum, FALSE);
                            
                        if (DPM_IS_FRS_XMT_OR_DET_ENABLED(u8PortNum))
                        {
                            UPD_RegByteClearBit (u8PortNum, TYPEC_CC_HW_CTL_LOW, (UINT8)TYPEC_CC_DET_HBW_EN);	

                            UPD_RegWriteByte (u8PortNum, UPD_CC_SAMP_CLK, \
                                    (UINT8) (UPD_CC_SAMP_GEN_10_KS | UPD_CC_CLK_20_KHZ));                            
                        }
#endif  

                        /*Drive DAC_I to 0V if DRP is not sink*/
                        MCHP_PSF_HOOK_DRIVE_DAC_I(u8PortNum, SET_TO_ZERO);

                        /*Setting CC Comparator OFF*/
                        TypeC_ConfigCCComp (u8PortNum, TYPEC_CC_COMP_CTL_DIS);

                        /*Mask CC match interrupts to avoid interrupts due to Ra/Open termination*/
                        UPD_RegByteClearBit (u8PortNum,  TYPEC_CC_INT_EN,\
                        (UINT8)(TYPEC_CC1_MATCH_CHG | TYPEC_CC2_MATCH_CHG | TYPEC_CC_MATCH_VLD));	

                        gasTypeCcontrol[u8PortNum].u8DRPLastAttachedState = PD_ROLE_DRP;

                        /*Set power and data role status*/
                        DPM_UpdatePowerRole (u8PortNum, PD_ROLE_DRP);
                        DPM_UpdateDataRole (u8PortNum, PD_ROLE_TOGGLING);

                        /*Enable DRP offload.*/
                        UPD_RegByteSetBit (u8PortNum, TYPEC_DRP_CTL_LOW, TYPEC_DRP_EN);
                    }
#endif
                    gasTypeCcontrol[u8PortNum].u8TypeCSubState = TYPEC_UNATTACHED_SNK_WAIT_FOR_VSAFE0V_SS;
                                        
                    break;
                }
                
                case TYPEC_UNATTACHED_SNK_WAIT_FOR_VSAFE0V_SS:
                {
                    if (TYPEC_VBUS_0V_PRES == (u8IntStsISR & TYPEC_VBUS_PRESENCE_MASK))
                    { 
                        /*DEBUG_PRINT_PORT_STR(u8PortNum,"TYPEC_UNATTACHED_SNK_WAIT_FOR_VSAFE0V_SS" \
                                            " - VBUS_0V Present\r\n");*/
                        
                        /* Disabling the VBUS discharge functionality since VBUS has reached vSafe0V */                  
                        PWRCTRL_ConfigVBUSDischarge (u8PortNum, FALSE);
             
                        /* Configure VBUS threshold to detect 5V*/
                        TypeC_ConfigureVBUSThr (u8PortNum, TYPEC_VBUS_5V, \
                                gasDPM[u8PortNum].u16SinkOperatingCurrInmA, TYPEC_CONFIG_NON_PWR_FAULT_THR); 
                        
                        if (PD_ROLE_SINK == DPM_GET_DEFAULT_POWER_ROLE(u8PortNum))
                        {
                            /*Setting CC Comparator OFF*/
                            TypeC_ConfigCCComp (u8PortNum, TYPEC_CC_COMP_CTL_DIS);

                            /*Setting the CC Debounce register to detect both the sink and powered cable on CC1 and CC2*/
                            TypeC_SetCCSampleEnable (u8PortNum, TYPEC_ENABLE_CC1_CC2);

                            /*Setting CC Comparator ON*/
                            TypeC_ConfigCCComp (u8PortNum, TYPEC_CC_COMP_CTL_CC1_CC2);
                        }       
                        
                        gasTypeCcontrol[u8PortNum].u8TypeCSubState = TYPEC_UNATTACHED_SNK_INIT_SS; 
                    }
                    else
                    {
                        /* Hook to notify Type C state machine entry into idle sub-state */
                        MCHP_PSF_HOOK_NOTIFY_IDLE (u8PortNum, eIDLE_TYPEC_NOTIFY);                        
                    }
                    break;
                }
				
                case TYPEC_UNATTACHED_SNK_INIT_SS:
                {
                    /*DPM power fault handler resets SM by polling this state TYPEC_UNATTACHED_SNK_INIT_SS
                    in the handler for Sink unattach.
                    Any module can reset its SM by polling this state*/
                    gasTypeCcontrol[u8PortNum].u8TypeCSubState = TYPEC_UNATTACHED_SNK_IDLE_SS;                    
                    
                    /* Notify external DPM of Port enabled event through a user defined call back*/
                    (void)DPM_NotifyClient (u8PortNum, eMCHP_PSF_PORT_ENABLED);
                    
                    /*Notify external DPM of Type Detach event through a user defined call back*/
                    (void)DPM_NotifyClient (u8PortNum, eMCHP_PSF_TYPEC_DETACH_EVENT);
                    
                    break;
                }
                                   
                /*Sink Waits in this Sub-state for Source attachment*/    
                case TYPEC_UNATTACHED_SNK_IDLE_SS:
                {
                    /* Hook to notify Type C state machine entry into idle sub-state */
                    MCHP_PSF_HOOK_NOTIFY_IDLE (u8PortNum, eIDLE_TYPEC_NOTIFY);
                    
                    break;
                }
                    
                default:
                    break;           
            }          
            break;
       }
          
        case TYPEC_ATTACHWAIT_SNK:
        {
            switch (gasTypeCcontrol[u8PortNum].u8TypeCSubState)
		    {   
                /*This SubState is used to start a tCCDebounce Software timer for Source attachment*/ 
                case TYPEC_ATTACHWAIT_SNK_ENTRY_SS:
                {                  
                    //DEBUG_PRINT_PORT_STR (u8PortNum,"TYPEC_ATTACHWAIT_SNK_ENTRY_SS\r\n");                  
                    
                    /* Configure VBUS threshold to detect 5V*/
                    TypeC_ConfigureVBUSThr (u8PortNum, TYPEC_VBUS_5V, \
                            gasDPM[u8PortNum].u16SinkOperatingCurrInmA, TYPEC_CONFIG_NON_PWR_FAULT_THR); 
                        
                    gasTypeCcontrol[u8PortNum].u8TypeCTimerID = PDTimer_Start ( \
                                                      (TYPEC_TCCDEBOUNCE_TIMEOUT_MS),\
                                                      TypeC_SubStateChange_TimerCB, u8PortNum,\
                                                      TYPEC_ATTACHWAIT_SNK_CC_DEB_TIMEOUT_SS);
                    
                    gasTypeCcontrol[u8PortNum].u8TypeCSubState = TYPEC_ATTACHWAIT_SNK_IDLE_SS; 
                    
                    break;
                }
                   
                    
                /*Sink Waits in this sub-state until either the tCCDebounce or tPDDebounce timer 
                expires */
                case TYPEC_ATTACHWAIT_SNK_IDLE_SS:
                {
                    /* Hook to notify Type C state machine entry into idle sub-state */
                    MCHP_PSF_HOOK_NOTIFY_IDLE (u8PortNum, eIDLE_TYPEC_NOTIFY);
                    
                    break;
                }
                    
                /*This SubState is used to start a tPDDebounce Software timer for Source detachment*/ 
                case TYPEC_ATTACHWAIT_SNK_START_PD_DEB_SS:
                {                  
#if(TRUE == INCLUDE_PD_DRP)
                    if (PD_ROLE_DRP == DPM_GET_DEFAULT_POWER_ROLE(u8PortNum))
                    {
                        /*Intentionally assigning u8DRPLastAttachedState as source to achieve
                         triggering of DRP offload enable in TYPEC_UNATTACHED_SRC_ENTRY_SS state 
                         and thereby discovering a valid attach*/
                        gasTypeCcontrol[u8PortNum].u8DRPLastAttachedState = PD_ROLE_SOURCE;
                        
						/*If source is detached during TYPEC_ATTACHWAIT_SNK state, according to
						DRP state machine, the next state should be TYPEC_UNATTACHED_SRC*/
                        gasTypeCcontrol[u8PortNum].u8TypeCTimerID = PDTimer_Start ( \
                                                      (TYPEC_TPDEBOUNCE_TIMEOUT_MS),\
                                                      TypeC_StateChange_TimerCB, u8PortNum,\
                                                      TYPEC_UNATTACHED_SRC);
                    }
                    else
#endif
                    {
                        gasTypeCcontrol[u8PortNum].u8TypeCTimerID = PDTimer_Start ( \
                                                      (TYPEC_TPDEBOUNCE_TIMEOUT_MS),\
                                                      TypeC_StateChange_TimerCB, u8PortNum,\
                                                      TYPEC_UNATTACHED_SNK);
                    }
                    gasTypeCcontrol[u8PortNum].u8TypeCSubState = TYPEC_ATTACHWAIT_SNK_IDLE_SS;
                    
                    break;
                }
                    
                
                /*Sink enters this sub-state after the tCCDebounce timer is expired */
                case TYPEC_ATTACHWAIT_SNK_CC_DEB_TIMEOUT_SS:
                {                  
                    /*Check for VBUS Presence before moving to Attached SNK state */
                    if (TYPEC_VBUS_5V_PRES == (u8IntStsISR & TYPEC_VBUS_PRESENCE_MASK))
                    {
                        /*DEBUG_PRINT_PORT_STR (u8PortNum,"TYPEC_ATTACHWAIT_SNK_CC_DEB_TIMEOUT_SS "\
                                "- VBUS_5V_PRESENT\r\n");*/
                        /*Set BLK_PD_MSG. Done in cronus*/
                        UPD_RegByteSetBit (u8PortNum, TYPEC_CC_HW_CTL_HIGH, TYPEC_BLK_PD_MSG);
                        
                        gasTypeCcontrol[u8PortNum].u8TypeCSubState = TYPEC_ATTACHED_SNK_ENTRY_SS;
                        gasTypeCcontrol[u8PortNum].u8TypeCState = TYPEC_ATTACHED_SNK;                    
                    }      
                    else
                    {
                        /* Hook to notify Type C state machine entry into idle sub-state */
                        MCHP_PSF_HOOK_NOTIFY_IDLE (u8PortNum, eIDLE_TYPEC_NOTIFY);                        
                    }
                    break;
                }
                                   
                default:
                    break;  
             }
            break;
        }
            
        case TYPEC_ATTACHED_SNK:
        {          
            switch (gasTypeCcontrol[u8PortNum].u8TypeCSubState)
            {
#if (TRUE == (INCLUDE_PD_PR_SWAP || INCLUDE_PD_FR_SWAP))
                /* This sub-state would be entered from 
                   Attached Source state during Source to Sink PR_Swap/FR_Swap */
                case TYPEC_ATTACHED_SNK_SWAP_ASSERT_RD_SS:
                {
                    //DEBUG_PRINT_PORT_STR (u8PortNum,"TYPEC_ATTACHED_SNK_SWAP_ASSERT_RD_SS\r\n");
                    
                    /* CC Comparator would have been turned off in 
                       TYPEC_ATTACHED_SRC_SWAP_TRANS_TO_SNK_SS sub-state. No need
                       to do it here once again */
                    
                    /* Configure the device role as UFP for the UPD350 to act as Sink */
                    TypeC_SetCCDeviceRole (u8PortNum, PD_ROLE_UFP);

                    /*Set the u8CCDebMatch variable accordingly */
                    TypeC_SetCCDebounceVariable (u8PortNum, TYPEC_UFP);
                    
                    UINT8 u8CCEnablePin, u8CCCompCtrl; 
                    /* It is always recommended to enable thresholds on the other pin when we (as UPD350) 
                       supply VCONN, to monitor changes in VCONN voltage, because VCONN is visible only to 
                       the VCONN supplier and the cable on the supply end and is not visible to the port partner. */
                    if (gasTypeCcontrol[u8PortNum].u8IntStsISR & TYPEC_VCONN_SOURCE_MASK)
                    {
                        u8CCEnablePin = TYPEC_ENABLE_CC1_CC2; 
                        u8CCCompCtrl = TYPEC_CC_COMP_CTL_CC1_CC2; 
                    }
                    else /* Not VCONN Source */
                    {
                        /* We are not VCONN Source, enable CC Debounce thresholds only for 
                           the connected CC pin. */
                        if (TYPEC_ORIENTATION_CC1 == TYPEC_GET_CC_ORIENTATION_STS(u8PortNum))
                        {
                            u8CCEnablePin = TYPEC_ENABLE_CC1;
                            u8CCCompCtrl = TYPEC_CC_COMP_CTL_CC1; 
                        }
                        else
                        {
                            u8CCEnablePin = TYPEC_ENABLE_CC2;
                            u8CCCompCtrl = TYPEC_CC_COMP_CTL_CC2; 
                        }
                    }
                    /*Reset the CC Debounce clear enable,CC Match Enable,CC Sample Enable register*/                                                                    
                    TypeC_SetCCSampleEnable (u8PortNum, u8CCEnablePin);
                        
                    /* Turn on the CC Comparator */
                    TypeC_ConfigCCComp (u8PortNum, u8CCCompCtrl);
                                                           
                    /* Configure VBUS threshold to detect 5V*/
                    TypeC_ConfigureVBUSThr (u8PortNum, TYPEC_VBUS_5V, \
                            gasDPM[u8PortNum].u16SinkOperatingCurrInmA, TYPEC_CONFIG_NON_PWR_FAULT_THR); 

                    if (TRUE == DPM_IS_FRS_XMT_OR_DET_ENABLED(u8PortNum))
                    {
                        /* During FR_Swap, 5V will have already been present in VBUS. 
                           Again checking for 5V presence is not necessary. Therefore, 
                           wait for CC attach from initial sink */
                        gasTypeCcontrol[u8PortNum].u8TypeCSubState = TYPEC_ATTACHED_SNK_SWAP_ATTACH_DETECT_SS;                     
                    }
                    else
                    {
                        /* During PR_Swap, wait for vSafe5V from Original Sink */
                        gasTypeCcontrol[u8PortNum].u8TypeCSubState = TYPEC_ATTACHED_SNK_SWAP_VBUS_PRES_DETECT_SS; 
                    }
                    break; 
                }                
                
                case TYPEC_ATTACHED_SNK_SWAP_VBUS_PRES_DETECT_SS:
                {
                    /*Check for VBUS Presence before moving to Attached SNK state */
                    if (TYPEC_VBUS_5V_PRES == (u8IntStsISR & TYPEC_VBUS_PRESENCE_MASK))
                    {
                        /* If VSink_Disconnect interrupt had occurred, VBUS_DIS would have been 
                           turned on TypeC_HandleISR(). Since this interrupt will not be handled
                           during a Role Swap, turning off the VBUS Discharge here*/
                        PWRCTRL_ConfigVBUSDischarge (u8PortNum, FALSE);
                        
                        //DEBUG_PRINT_PORT_STR (u8PortNum,"TYPEC_ATTACHED_SNK_SWAP_VBUS_PRES_DETECT_SS\r\n");                    
                        
                        gasTypeCcontrol[u8PortNum].u8TypeCSubState = TYPEC_ATTACHED_SNK_ENTRY_SS;
                    }      
                    else
                    {
                        /* Hook to notify Type C state machine entry into idle sub-state */
                        MCHP_PSF_HOOK_NOTIFY_IDLE (u8PortNum, eIDLE_TYPEC_NOTIFY);                        
                    }
                    break; 
                }
                
                case TYPEC_ATTACHED_SNK_SWAP_ATTACH_DETECT_SS:
                {
                    /* Wait for CC attach*/
                    /* Hook to notify Type C state machine entry into idle sub-state */
                    MCHP_PSF_HOOK_NOTIFY_IDLE (u8PortNum, eIDLE_TYPEC_NOTIFY);
                    break;
                }
#endif 
                 /*Sink enables the CC Communication channel and notifies the external DPM
                 about the Type C Attached event this sub-state */
                case TYPEC_ATTACHED_SNK_ENTRY_SS:
                {
                    //DEBUG_PRINT_PORT_STR (u8PortNum,"TYPEC_ATTACHED_SNK_ENTRY_SS\r\n");
                    
                    /* Assign Operating current based on the Rp value*/
                    gasDPM[u8PortNum].u16SinkOperatingCurrInmA = TypeC_ObtainCurrentValueFrmRp(u8PortNum);                                                         

                    #if(TRUE == INCLUDE_PD_DRP)       
                    if(PD_ROLE_DRP == DPM_GET_DEFAULT_POWER_ROLE(u8PortNum))
                    {
                        gasTypeCcontrol[u8PortNum].u8DRPLastAttachedState = PD_ROLE_SINK;
                    }
                    #endif

                    PWRCTRL_ConfigSinkHW (u8PortNum, TYPEC_VBUS_5V, gasDPM[u8PortNum].u16SinkOperatingCurrInmA);                               
                    
					/* Enabling PRL Rx*/
                    PRL_EnableRx (u8PortNum, TRUE);
							   
				    /* Enable Power Threshold for TYPEC_VBUS_5V */
                    TypeC_ConfigureVBUSThr (u8PortNum, TYPEC_VBUS_5V,
                            gasDPM[u8PortNum].u16SinkOperatingCurrInmA, TYPEC_CONFIG_PWR_FAULT_THR);
                    
                    /* Enable AME Monitoring  */ 
                    #if(TRUE == INCLUDE_PD_ALT_MODE)                                                           
                        DPM_EnableAMEMonitoring (u8PortNum);                    
                    #endif 

                    gasTypeCcontrol[u8PortNum].u8TypeCSubState = TYPEC_ATTACHED_SNK_RUN_SM_SS; 
                    
                    /* Update connected CC orientation status */
                    /*Source Attached in CC1 pin*/
                    if (u8CC1MatchISR == gasTypeCcontrol[u8PortNum].u8CCSrcSnkMatch)
                    {
                        gasTypeCcontrol[u8PortNum].u8PortSts &= ~(TYPEC_CC_ATTACHED_ORIENTATION_MASK); 
                        UPD_RegByteClearBit (u8PortNum, TYPEC_CC_CTL1_HIGH, TYPEC_CC_COM_SEL);
                        
                        (void)DPM_NotifyClient (u8PortNum, eMCHP_PSF_TYPEC_CC1_ATTACH);                        
                    }
                    /*Source Attached in CC2 pin*/
                    else
                    {
                        gasTypeCcontrol[u8PortNum].u8PortSts |= TYPEC_CC_ATTACHED_ORIENTATION_MASK;
                        UPD_RegByteSetBit (u8PortNum, TYPEC_CC_CTL1_HIGH, TYPEC_CC_COM_SEL);
                        
                        (void)DPM_NotifyClient (u8PortNum, eMCHP_PSF_TYPEC_CC2_ATTACH);
                    }
                    
                    break;
                }
                
                case TYPEC_ATTACHED_SNK_RUN_SM_SS:	
                {
                    /* Hook to notify Type C state machine entry into idle sub-state */
                    MCHP_PSF_HOOK_NOTIFY_IDLE (u8PortNum, eIDLE_TYPEC_NOTIFY);
                    
                    break;
                }
                
                /*Sink enters this sub-state to start a tPDDeounce timer for source detach event*/
                case TYPEC_ATTACHED_SNK_START_PD_DEB_SS:
                {
                    //DEBUG_PRINT_PORT_STR (u8PortNum,"TYPEC_ATTACHED_SNK_START_PD_DEB_SS\r\n");
                    
                    /*Kill all the timers for a port before starting a Type C Debounce timer*/
                    PDTimer_KillPortTimers (u8PortNum);
                    
                    /* Start a Debounce timer of tPDDebounce for detach event*/
                    gasTypeCcontrol[u8PortNum].u8TypeCTimerID = PDTimer_Start ( \
                                                       (TYPEC_TPDEBOUNCE_TIMEOUT_MS),\
                                                       TypeC_SubStateChange_TimerCB, u8PortNum,\
                                                       TYPEC_ATTACHED_SNK_PD_DEB_TIMEOUT_SS);
                    
                    gasTypeCcontrol[u8PortNum].u8TypeCSubState = TYPEC_ATTACHED_SNK_IDLE_SS; 
                    
                    break;
                }
                                   
                /*Sink enters this sub-state after the tPDDeounce timer is expired or VBUS drops below 
                vSinkDisconnect*/
                case TYPEC_ATTACHED_SNK_PD_DEB_TIMEOUT_SS:
                {    
                    //DEBUG_PRINT_PORT_STR (u8PortNum,"TYPEC_ATTACHED_SNK_PD_DEB_TIMEOUT_SS\r\n");
                                         
                    if ((u8IntStsISR & TYPEC_VBUS_PRESENCE_MASK) != TYPEC_VBUS_0V_PRES)
                    {
                        /*Enabling the VBUS discharge functionality for VBUS to go to vSafe0V*/                  
                        PWRCTRL_ConfigVBUSDischarge (u8PortNum, TRUE);
                    }

                    gasDPM[u8PortNum].u16SinkOperatingCurrInmA = DPM_0mA;
                    /*Disable the Sink circuitry to stop sinking the power from source*/
                    PWRCTRL_ConfigSinkHW (u8PortNum, TYPEC_VBUS_0V, gasDPM[u8PortNum].u16SinkOperatingCurrInmA);
                    
                    /*Enabling the VCONN Discharge if we are supplying VCONN or VCONN ON Req is set*/
                    if (((u8IntStsISR & TYPEC_VCONN_SOURCE_MASK) != TYPEC_VCONN_DISABLED) || \
                       (gasTypeCcontrol[u8PortNum].u8PortSts & TYPEC_VCONN_ON_REQ_MASK))
                    {                        
                        /*Disable VCONN by switching off the VCONN FETS*/
                        TypeC_EnabDisVCONN (u8PortNum, TYPEC_VCONN_DISABLE);
                        
                        /*Clearing the VCONN ON Request mask if VCONN has turned ON*/
                        gasTypeCcontrol[u8PortNum].u8PortSts &= ~TYPEC_VCONN_ON_REQ_MASK;
                        
                        gasTypeCcontrol[u8PortNum].u8TypeCSubState = TYPEC_ATTACHED_SNK_IDLE_SS;
                    }
                    else
                    {
                        gasTypeCcontrol[u8PortNum].u8TypeCState = TYPEC_UNATTACHED_SNK;
                        gasTypeCcontrol[u8PortNum].u8TypeCSubState = TYPEC_UNATTACHED_SNK_ENTRY_SS;
                    }
                    break;                    
                }
                
                /*Sink waits in this sub-state until the tPDDeounce timer expires or until the VCONN 
                Discharge is completed*/
                case TYPEC_ATTACHED_SNK_IDLE_SS: 
                {
                    /* Hook to notify Type C state machine entry into idle sub-state */
                    MCHP_PSF_HOOK_NOTIFY_IDLE (u8PortNum, eIDLE_TYPEC_NOTIFY);
                    
                    break;
                }
                    
#if (TRUE == (INCLUDE_PD_PR_SWAP || INCLUDE_PD_FR_SWAP))                
                case TYPEC_ATTACHED_SNK_SWAP_TRANS_TO_SRC_SS:
                {
                    /* This sub-state would be entered only during 
                       Power role swap from Sink to Source */
                    //DEBUG_PRINT_PORT_STR (u8PortNum,"TYPEC_ATTACHED_SNK_SWAP_TRANS_TO_SRC_SS\r\n");                                        
                    /* Power down the CC comparator */
                    TypeC_ConfigCCComp (u8PortNum, TYPEC_CC_COMP_CTL_DIS);
                    
                    /* Get Rp current source value from user configuration */
                    UINT8 u8RpValue = DPM_GET_CONFIGURED_SOURCE_RP_VAL(u8PortNum);
                    
                    /* Disable Rd terminations from both the CC pins and enable 
                       Rp only in the connected CC pin. 
                       Type C Spec Reference: 
                       Note: If the port has entered this state from the Attached.SNK state as 
                       the result of a USB PD PR_Swap/FR_Swap, the port shall source current on the 
                       connected CC pin and monitor its state */
                    if (TYPEC_ORIENTATION_CC1 == TYPEC_GET_CC_ORIENTATION_STS(u8PortNum))
                    {
                        TypeC_SetCCPowerRole (u8PortNum, TYPEC_ROLE_SOURCE, u8RpValue, TYPEC_ENABLE_CC1);
                    }
                    else /* Attached Orientation - CC2 */
                    {
                        TypeC_SetCCPowerRole (u8PortNum, TYPEC_ROLE_SOURCE, u8RpValue, TYPEC_ENABLE_CC2);
                    }                    
                    
                    /*Setting user given Rp Value as Current Rp Value in CurrentRpVal bit of u8PortSts variable*/
                    gasTypeCcontrol[u8PortNum].u8PortSts &= ~TYPEC_CURR_RPVAL_MASK;
                    gasTypeCcontrol[u8PortNum].u8PortSts |= (u8RpValue << TYPEC_CURR_RPVAL_POS);

                    /* Move the Type C SM to Attached Src state since the 
                       port is ready to act as Source */
                    gasTypeCcontrol[u8PortNum].u8TypeCState = TYPEC_ATTACHED_SRC;
                    gasTypeCcontrol[u8PortNum].u8TypeCSubState = TYPEC_ATTACHED_SRC_SWAP_ASSERT_RP_SS;
                    break;                    
                }
#endif 
                default:
                    break;  
            }
            break;
        }
        case TYPEC_DEBUG_ACCESSORY_SNK:         
            break;     

#endif
/*INCLUDE_PD_SINK*/
          
        case TYPEC_ERROR_RECOVERY:
        {         
            switch (gasTypeCcontrol[u8PortNum].u8TypeCSubState)
            {
                /*In this SubState, VBUS and VCONN are disabled.Pull down and Pull up resistors
                are set to open disconnect*/
                case TYPEC_ERROR_RECOVERY_ENTRY_SS:
                {                  
                    //DEBUG_PRINT_PORT_STR (u8PortNum,"TYPEC_ERROR_RECOVERY_ENTRY_SS\r\n"); 
                    
                    /* Disable the receiver*/
                    PRL_EnableRx (u8PortNum, FALSE);
                    
                    /* Disable EN_FRS pin when FRS is enabled for the port */
                    #if (TRUE == INCLUDE_PD_FR_SWAP)
                        PWRCTRL_DisableEnFRS (u8PortNum);
                    #endif 

                    /*Setting CC Comparator OFF*/
                    TypeC_ConfigCCComp (u8PortNum, TYPEC_CC_COMP_CTL_DIS);
                    
                    if (PD_ROLE_SOURCE == DPM_GET_CURRENT_POWER_ROLE(u8PortNum))
                    {    
#if(TRUE == INCLUDE_PD_SOURCE)                        
                        /*Disable VBUS by driving to vSafe0V*/
                        DPM_DriveVBUS (u8PortNum, DPM_VBUS_OFF);
     
                        /*Disable DC_DC EN on VBUS fault to reset the DC-DC controller*/
                        PWRCTRL_ConfigDCDCEn (u8PortNum, FALSE);
                        
                        #if (TRUE == INCLUDE_UPD_PIO_OVERRIDE_SUPPORT)
                        /*Disable PIO override enable*/
                        UPD_RegByteClearBit (u8PortNum, UPD_PIO_OVR_EN,  UPD_PIO_OVR_2);
                        #endif
            
                        /*Setting the CC1 and CC2 line as Open Disconnect*/
                        TypeC_SetCCPowerRole (u8PortNum, PD_ROLE_SOURCE, TYPEC_ROLE_SOURCE_OPEN_DIS, TYPEC_ENABLE_CC1_CC2);                     
#endif
                    }
                    else
                    {
#if (TRUE == INCLUDE_PD_SINK)                        
                        /*Kill active timers if any*/
                        PDTimer_KillPortTimers (u8PortNum);
                        
                        gasDPM[u8PortNum].u16SinkOperatingCurrInmA = DPM_0mA;
                        
                        /*Disable the Sink circuitry to stop sinking the power from source*/
                        PWRCTRL_ConfigSinkHW (u8PortNum, TYPEC_VBUS_0V, \
                                gasDPM[u8PortNum].u16SinkOperatingCurrInmA);                        
                        
                         /*Setting the CC1 and CC2 line as Open Disconnect*/
                        TypeC_SetCCPowerRole (u8PortNum, PD_ROLE_SINK, TYPEC_ROLE_SINK_OPEN_DIS,                                  
                                     TYPEC_ENABLE_CC1_CC2); 
#endif                        
                    }
                    
                    #if (INCLUDE_PD_VCONN_SWAP | INCLUDE_PD_SOURCE)
                    /*Disable VCONN if the port sources the VCONN already or failed while trying to
                    source VCONN*/
                    if (((u8IntStsISR & TYPEC_VCONN_SOURCE_MASK) \
                        != TYPEC_VCONN_DISABLED) || (gasTypeCcontrol[u8PortNum].u8PortSts & TYPEC_VCONN_ON_REQ_MASK))
                    {
                        /*Clearing the VCONN ON Request mask if VCONN has turned ON*/
                        gasTypeCcontrol[u8PortNum].u8PortSts &= ~TYPEC_VCONN_ON_REQ_MASK;

                        /*Disable VCONN by switching off the VCONN FETS*/
                        TypeC_EnabDisVCONN (u8PortNum, TYPEC_VCONN_DISABLE);
                    }
                  
                    gasTypeCcontrol[u8PortNum].u8TypeCSubState = TYPEC_ERROR_RECOVERY_WAIT_FOR_VCONN_OFF_SS;
                    
                    #else
                    gasTypeCcontrol[u8PortNum].u8TypeCSubState = TYPEC_ERROR_RECOVERY_WAIT_FOR_VBUS_OFF_SS; 
                    #endif
                    break;                    
                }
                
                #if (INCLUDE_PD_VCONN_SWAP | INCLUDE_PD_SOURCE)
                case TYPEC_ERROR_RECOVERY_WAIT_FOR_VCONN_OFF_SS:
                {                   
                    if (!(u8IntStsISR & TYPEC_VCONN_SOURCE_MASK))
                    {
                        gasTypeCcontrol[u8PortNum].u8TypeCSubState = TYPEC_ERROR_RECOVERY_WAIT_FOR_VBUS_OFF_SS;                    
                    }
                    else
                    {
                        /* Hook to notify Type C state machine entry into idle sub-state */
                        MCHP_PSF_HOOK_NOTIFY_IDLE (u8PortNum, eIDLE_TYPEC_NOTIFY);                        
                    }
                    break;
                }
                #endif
                
                case TYPEC_ERROR_RECOVERY_WAIT_FOR_VBUS_OFF_SS:
                {
                    if (TYPEC_VBUS_0V_PRES == (u8IntStsISR & TYPEC_VBUS_PRESENCE_MASK))
                    {                  
                        /* Turn off VBUS Discharge */
                        PWRCTRL_ConfigVBUSDischarge (u8PortNum, FALSE);                        
                        
                        /*Setting VBUS Comparator OFF once the VBUS line goes off to 0V*/
                        TypeC_SetVBUSCompONOFF (u8PortNum, TYPEC_VBUSCOMP_OFF);
                        
                        gasTypeCcontrol[u8PortNum].u8TypeCTimerID = PDTimer_Start ( \
                                  (TYPEC_ERRORRECOVERY_TIMEOUT_MS),\
                                  TypeC_SubStateChange_TimerCB, u8PortNum,\
                                  TYPEC_ERROR_RECOVERY_TIMEOUT_SS);
                        
                        gasTypeCcontrol[u8PortNum].u8TypeCSubState = TYPEC_ERROR_RECOVERY_IDLE_SS;                    
                    }
                    else
                    {
                        /* Hook to notify Type C state machine entry into idle sub-state */
                        MCHP_PSF_HOOK_NOTIFY_IDLE (u8PortNum, eIDLE_TYPEC_NOTIFY);                        
                    }                    
                    break;
                }

                /*Device waits in this sub-state until the tErrorRecovery software timer expires*/
                case TYPEC_ERROR_RECOVERY_IDLE_SS:
                {
                    /* Hook to notify Type C state machine entry into idle sub-state */
                    MCHP_PSF_HOOK_NOTIFY_IDLE (u8PortNum, eIDLE_TYPEC_NOTIFY);
                    
                    break;
                }

                /*Device enters this sub-state after the tErrorRecovery software timer is expired*/
                case TYPEC_ERROR_RECOVERY_TIMEOUT_SS: 
                {                        
                    /*Resetting the Port Power Role*/
                    if (PD_ROLE_SOURCE == DPM_GET_CURRENT_POWER_ROLE(u8PortNum))
                    {
#if(TRUE == INCLUDE_PD_SOURCE)                        
                        UINT8 u8Data;
                        
                        /*Setting Rp Current Source as user given and Rd as Open disconnect in both 
                        CC1 and CC2*/ 
                        u8Data = DPM_GET_CONFIGURED_SOURCE_RP_VAL(u8PortNum);
                        
                        TypeC_SetCCPowerRole (u8PortNum, TYPEC_ROLE_SOURCE, u8Data, TYPEC_ENABLE_CC1_CC2);
                        
                        gasTypeCcontrol[u8PortNum].u8TypeCState = TYPEC_UNATTACHED_SRC;
                        gasTypeCcontrol[u8PortNum].u8TypeCSubState = TYPEC_UNATTACHED_SRC_ENTRY_SS; 
                           
                        /*Enable DC_DC EN on VBUS fault to reset the DC-DC controller*/
                        PWRCTRL_ConfigDCDCEn (u8PortNum, TRUE);
                        
                        /*Enable PIO override enable*/
                        #if (TRUE == INCLUDE_UPD_PIO_OVERRIDE_SUPPORT)                        
                            UPD_RegByteSetBit (u8PortNum, UPD_PIO_OVR_EN,  UPD_PIO_OVR_2);
                        #endif
#endif
                    }
                    else
                    {
#if(TRUE == INCLUDE_PD_SINK)                        
                        TypeC_SetCCPowerRole (u8PortNum,TYPEC_ROLE_SINK, TYPEC_ROLE_SINK_RD, TYPEC_ENABLE_CC1_CC2);                    
                        
                        gasTypeCcontrol[u8PortNum].u8TypeCState = TYPEC_UNATTACHED_SNK;
                        gasTypeCcontrol[u8PortNum].u8TypeCSubState = TYPEC_UNATTACHED_SNK_ENTRY_SS; 
#endif 
                    }
                    
                    /*Setting CC Comparator ON*/
                    TypeC_ConfigCCComp (u8PortNum, TYPEC_CC_COMP_CTL_CC1_CC2);   
                    /*Setting VBUS Comparator ON*/
                    TypeC_SetVBUSCompONOFF (u8PortNum, TYPEC_VBUSCOMP_ON);                         
                    break;                   
                }                    

                default:
                    break;  
            }
            break; 
        }
	  
        case TYPEC_DISABLED: 
        {
            switch (gasTypeCcontrol[u8PortNum].u8TypeCSubState)
            {
                /* As per Type C Spec, Disabled State Requirements are, 
                The port shall not drive VBUS or VCONN, and shall 
                present a high-impedance to ground (above zOPEN) on
                its CC1 and CC2 pins. */ 
                case TYPEC_DISABLED_ENTRY_SS:
                {
                    //DEBUG_PRINT_PORT_STR (u8PortNum,"TYPEC_DISABLED_ENTRY_SS\r\n"); 
                    
                    /* Disable the receiver*/
                    PRL_EnableRx (u8PortNum, FALSE);
                    
                    /*Setting CC Comparator OFF*/
                    TypeC_ConfigCCComp (u8PortNum, TYPEC_CC_COMP_CTL_DIS);
					
					/*Kill active timers if any*/
                    PDTimer_KillPortTimers (u8PortNum);
                    
                    if (PD_ROLE_SOURCE == DPM_GET_CURRENT_POWER_ROLE(u8PortNum))
                    {
#if (TRUE == INCLUDE_PD_SOURCE)                        
                        /*Disable VBUS by driving to vSafe0V*/
                        DPM_DriveVBUS (u8PortNum, DPM_VBUS_OFF);
                             
                         /*Disable DC_DC EN on VBUS fault to reset the DC-DC controller*/
                        PWRCTRL_ConfigDCDCEn (u8PortNum, FALSE);
                        
#if (TRUE == INCLUDE_UPD_PIO_OVERRIDE_SUPPORT)
                        /*Disable PIO override enable*/
                        UPD_RegByteClearBit (u8PortNum, UPD_PIO_OVR_EN,  UPD_PIO_OVR_2);
#endif
                        /*Setting the CC1 and CC2 line as Open Disconnect*/
                        TypeC_SetCCPowerRole (u8PortNum, PD_ROLE_SOURCE, TYPEC_ROLE_SOURCE_OPEN_DIS, TYPEC_ENABLE_CC1_CC2); 
                        
                        /* Clear TypeC Current Rp Val bits in u8PortSts so that in unattached
                        SRC state, the Rp value from user configuration would be set. */
                        gasTypeCcontrol[u8PortNum].u8PortSts &= ~TYPEC_CURR_RPVAL_MASK;
#endif                        
                    }
                    else if (PD_ROLE_SINK == DPM_GET_CURRENT_POWER_ROLE(u8PortNum))
                    {  
#if (TRUE == INCLUDE_PD_SINK)                        
                        gasDPM[u8PortNum].u16SinkOperatingCurrInmA = DPM_0mA;
                                                
                        TypeC_ConfigureVBUSThr (u8PortNum, TYPEC_VBUS_0V,
                                gasDPM[u8PortNum].u16SinkOperatingCurrInmA, TYPEC_CONFIG_NON_PWR_FAULT_THR);
                                                        
                        /*Disable the Sink circuitry to stop sinking the power from source*/
                        PWRCTRL_ConfigSinkHW (u8PortNum, TYPEC_VBUS_0V, \
                                gasDPM[u8PortNum].u16SinkOperatingCurrInmA);
                        
                         /*Setting the CC1 and CC2 line as Open Disconnect*/
                        TypeC_SetCCPowerRole (u8PortNum, PD_ROLE_SINK, TYPEC_ROLE_SINK_OPEN_DIS, TYPEC_ENABLE_CC1_CC2);                        
#endif                        
                    }
                    else
                    {
                        /*Do nothing*/
                    }
           
                    /*Disable VCONN if the port sources the VCONN already or failed while trying to
                    source VCONN*/
                    if (((u8IntStsISR & TYPEC_VCONN_SOURCE_MASK) \
                        != TYPEC_VCONN_DISABLED) || (gasTypeCcontrol[u8PortNum].u8PortSts & TYPEC_VCONN_ON_REQ_MASK))
                    {
                        /*Clearing the VCONN ON Request mask if VCONN has turned ON*/
                        gasTypeCcontrol[u8PortNum].u8PortSts &= ~TYPEC_VCONN_ON_REQ_MASK;

                        /*Disable VCONN by switching off the VCONN FETS*/
                        TypeC_EnabDisVCONN (u8PortNum, TYPEC_VCONN_DISABLE);

                        gasTypeCcontrol[u8PortNum].u8TypeCSubState = TYPEC_DISABLED_WAIT_FOR_VCONN_OFF_SS;
                    }
                    else
                    {
                        gasTypeCcontrol[u8PortNum].u8TypeCSubState = TYPEC_DISABLED_WAIT_FOR_VBUS_OFF_SS;
                    }
                           
                    break; 
                }
                
                case TYPEC_DISABLED_WAIT_FOR_VCONN_OFF_SS:
                {
                    if (!(u8IntStsISR & TYPEC_VCONN_SOURCE_MASK))
                    {
                        gasTypeCcontrol[u8PortNum].u8TypeCSubState = TYPEC_DISABLED_WAIT_FOR_VBUS_OFF_SS;                    
                    }
                    else
                    {
                        /* Hook to notify Type C state machine entry into idle sub-state */
                        MCHP_PSF_HOOK_NOTIFY_IDLE (u8PortNum, eIDLE_TYPEC_NOTIFY);                        
                    }                      
                    break; 
                }
                
                case TYPEC_DISABLED_WAIT_FOR_VBUS_OFF_SS:
                {
                    if (TYPEC_VBUS_0V_PRES == (u8IntStsISR & TYPEC_VBUS_PRESENCE_MASK))
                    {
                        /*Setting VBUS Comparator OFF once the VBUS line goes off to 0V*/
                        TypeC_SetVBUSCompONOFF (u8PortNum, TYPEC_VBUSCOMP_OFF);
                        
                        /*Disable the VBUS discharge functionality since VBUS has reached vSafe0V*/                  
                        PWRCTRL_ConfigVBUSDischarge (u8PortNum, FALSE);
                
                        /*Setting CC Comparator OFF*/
                        TypeC_ConfigCCComp (u8PortNum, TYPEC_CC_COMP_CTL_DIS);
                    
                        if (PD_ROLE_SINK == DPM_GET_CURRENT_POWER_ROLE(u8PortNum))
                        {
                            /*Wait for TYPEC_ERRORRECOVERY_TIMEOUT_MS time as specified in Spec*/
                            gasTypeCcontrol[u8PortNum].u8TypeCTimerID = PDTimer_Start ( \
                                  (TYPEC_ERRORRECOVERY_TIMEOUT_MS),\
                                  TypeC_SubStateChange_TimerCB, u8PortNum,\
                                  TYPEC_DISABLED_TIMEOUT_SS);

                            gasTypeCcontrol[u8PortNum].u8TypeCSubState = TYPEC_DISABLED_IDLE_SS;
                        }
                        else
                        {
                            gasTypeCcontrol[u8PortNum].u8TypeCSubState = TYPEC_DISABLED_TIMEOUT_SS;
                        }                        
                    }
                    else
                    {
                        /* Hook to notify Type C state machine entry into idle sub-state */
                        MCHP_PSF_HOOK_NOTIFY_IDLE (u8PortNum, eIDLE_TYPEC_NOTIFY);                        
                    }
                    break;
                }

                /*Device waits in this sub-state until the tErrorRecovery software timer expires*/
                case TYPEC_DISABLED_IDLE_SS:
                {
                    /* Hook to notify Type C state machine entry into idle sub-state */
                    MCHP_PSF_HOOK_NOTIFY_IDLE (u8PortNum, eIDLE_TYPEC_NOTIFY);
                    
                    break;
                }
                
                case TYPEC_DISABLED_TIMEOUT_SS:    
                {
                    gasTypeCcontrol[u8PortNum].u8TypeCSubState = TYPEC_DISABLED_DONE_SS;
                    
                    /* Notify external DPM of Port disabled event through a user defined call back*/
                    (void)DPM_NotifyClient (u8PortNum, eMCHP_PSF_PORT_DISABLED);                                        
                    
                    break;
                }
                    
                case TYPEC_DISABLED_DONE_SS:
                {
                    /* Hook to notify Type C state machine entry into idle sub-state */
                    MCHP_PSF_HOOK_NOTIFY_IDLE (u8PortNum, eIDLE_TYPEC_NOTIFY);
                    
                    break;
                }
                default:
                {
                    break; 
                }
            }
            break; 
        }
        
        case TYPEC_AUDIO_ACCESSORY:
            break;
              
      default:
      break;
    }
  
}
/*******************************************************************************************/
/*********************************TypeC Interrupt Handler**************************************/
/*******************************************************************************************/
void TypeC_HandleISR (UINT8 u8PortNum, UINT16 u16InterruptStatus)
{
    UINT8 u8Data;
    UINT16 u16Data; 
    UINT8 u8VbusMatchValid = FALSE;
    
    UINT8 u8IntStsISR = gasTypeCcontrol[u8PortNum].u8IntStsISR;
    
    /*Check for EXT_INT*/
    if (u16InterruptStatus & UPDINTR_EXT_INT)
    {
        UPD_RegisterReadISR (u8PortNum, TYPEC_EXT_INT_STS, &u8Data, BYTE_LEN_1);
#if(TRUE == INCLUDE_PD_FR_SWAP)        
        if (u8Data & (TYPEC_FRS_XMT_STS | TYPEC_FRS_RCV_STS))
        {
            /*Clearing the FRS_XMT_STS/FRS_RCV_STS interrupt */
            UPD_RegisterWriteISR (u8PortNum, TYPEC_EXT_INT_STS, &u8Data, BYTE_LEN_1);
                     
            if (u8Data & TYPEC_FRS_XMT_STS)
            {
                /* Same u8Data variable is used for EXT INT STS and FRS CTL HIGH registers */
                UPD_RegisterReadISR (u8PortNum, TYPEC_FRS_CTL_HIGH, &u8Data, BYTE_LEN_1);
                u8Data &= ~(TYPEC_FRS_REQ_PIO);
                UPD_RegisterWriteISR (u8PortNum, TYPEC_FRS_CTL_HIGH, &u8Data, BYTE_LEN_1);                
            }   
            /* Set FRS XMT/RCV status mask in u8DRPStsISR */
            gasTypeCcontrol[u8PortNum].u8DRPStsISR |= TYPEC_FRS_XMT_RCV_STS_INTERRUPT;
        }
#endif
    }
    
    /*Check for CC Change interrupt*/
    if (u16InterruptStatus & UPDINTR_CC_INT)
    {			       
        UPD_RegisterReadISR (u8PortNum, TYPEC_CC_INT_STS, &u8Data, BYTE_LEN_1);

#if(TRUE == INCLUDE_PD_DRP)        
        if (u8Data & TYPEC_DRP_DONE)
        {
            /*Clearing the DRP_DONE interrupt */
            UPD_RegisterWriteISR (u8PortNum, TYPEC_CC_INT_STS, &u8Data, BYTE_LEN_1);
            
            gasTypeCcontrol[u8PortNum].u8DRPStsISR |= TYPEC_DRP_DONE_INTERRUPT;
        }
        else
#endif
        {
            if (u8Data & TYPEC_CC_MATCH_VLD)
            {
                /*Clearing the CC Match valid interrupt */
                /*This Interrupt is usually occurred while switching on the CC Comparator
                and Comparator has finished one round of sampling*/
                UPD_RegisterWriteISR (u8PortNum, TYPEC_CC_INT_STS, &u8Data, BYTE_LEN_1);

                /*Setting the CC Match Valid interrupt as CC interrupt for the firmware to handle.
                This is done to detect the detach event while Comparator goes from OFF state to ON state
                when the firmware is in attached state such as Collision avoidance and VCONN discharge*/
            }

            if (u8Data & (TYPEC_CC1_MATCH_CHG | TYPEC_CC2_MATCH_CHG))
            {   			
                /*Clearing the CC Change status interrupt */
                UPD_RegisterReadISR (u8PortNum, TYPEC_CC_CHG_STS, (UINT8 *)&u16Data, BYTE_LEN_2);           
                UPD_RegisterWriteISR (u8PortNum, TYPEC_CC_CHG_STS, (UINT8 *)&u16Data, BYTE_LEN_2);
            }

            /*Setting the CC interrupt flag in "u8IntStsISR" variable*/
            u8IntStsISR |= TYPEC_CCINT_STATUS_MASK;
            UPD_RegisterReadISR (u8PortNum, TYPEC_CC_MATCH, (UINT8 *)&u16Data, BYTE_LEN_2); 

            /*For Source Port*/
            if (PD_ROLE_SOURCE == DPM_GET_CURRENT_POWER_ROLE(u8PortNum))
            {             
                /*For a source port, a bit in CC Match register will be "cleared" , if a 
                match of the particular bit occurs. Hence the CC match register is negated while
                storing*/
                gasTypeCcontrol[u8PortNum].u8CC1MatchISR = (UINT8)~LOBYTE(u16Data);			
                gasTypeCcontrol[u8PortNum].u8CC2MatchISR = (UINT8)~HIBYTE(u16Data);           
            }
            /*For Sink Port*/
            else
            {   
                gasTypeCcontrol[u8PortNum].u8CC1MatchISR = (UINT8)LOBYTE(u16Data);			
                gasTypeCcontrol[u8PortNum].u8CC2MatchISR = (UINT8)HIBYTE(u16Data);            
            }
        }
    }
    
    /*Check for power interrupt*/
    if (u16InterruptStatus & UPDINTR_PWR_INT)
    {   
        /*Reading the power interrupt status register*/
        UPD_RegisterReadISR (u8PortNum, TYPEC_PWR_INT_STS, &u8Data, BYTE_LEN_1); 
        
        if (u8Data & TYPEC_VBUS_MATCH_VLD)
        {
            u8VbusMatchValid = TRUE;
        }
        
        #if (TRUE == INCLUDE_POWER_FAULT_HANDLING)
        /*Checking for VCONN over current error*/
        if (u8Data & TYPEC_VCONN_OVER_CURR_ERR)
        {
            /*VCONN FETS will be disabled automatically upon this error*/
            /*So Setting VCONN source as VCONN Disabled*/
            /*Setting the VCONN source bit in u8IntStsISR variable as VCONN disabled */
            u8IntStsISR &= ~TYPEC_VCONN_SOURCE_MASK; 
            
            /* Inform DPM about the power fault*/
            gasDPM[u8PortNum].u8PowerFaultISR |= DPM_POWER_FAULT_VCONN_OCS;    
        }
        #endif
        
        /*Clearing the Power interrupt*/    
		UPD_RegisterWriteISR (u8PortNum, TYPEC_PWR_INT_STS, &u8Data, BYTE_LEN_1);
    }
    
    /*Check for VBUS Change interrupt*/    
    if ((TRUE == u8VbusMatchValid) || (u16InterruptStatus & UPDINTR_VBUS_INT))
    {
        UPD_RegisterReadISR (u8PortNum, TYPEC_VBUS_MATCH, &u8Data, BYTE_LEN_1);
        
#if (TRUE == INCLUDE_POWER_FAULT_HANDLING)
        UINT8 u8HandleUV = FALSE;
#endif
        UINT8 u8IsSourceport = DPM_GET_CURRENT_POWER_ROLE(u8PortNum);

        if (TYPEC_VSAFE_0V_MATCH_VAL == u8Data)
        {
            /*Setting the VBUS Status flag as 0V in u8IntStsISR variable*/
            u8IntStsISR &= ~TYPEC_VBUS_PRESENCE_MASK;                                        
            
            if (u8IsSourceport)
            {
                if (u16InterruptStatus & UPDINTR_VBUS_INT)
                {
#if (TRUE == INCLUDE_POWER_FAULT_HANDLING)
                    u8HandleUV = TRUE;
#endif /* #if INCLUDE_POWER_FAULT_HANDLING */
                }
            }
            else
            {
				/* Verifies whether VBUS Drop is due to Source detach*/
	            if ((TYPEC_ATTACHED_SNK == gasTypeCcontrol[u8PortNum].u8TypeCState) &&
	                  (!DPM_IS_HARDRESET_IN_PROGRESS(u8PortNum)) && 
                      (FALSE == DPM_IS_PR_OR_FR_SWAP_IN_PROGRESS(u8PortNum))) 
	            {
	                /*Setting the flag that VBUS has gone below VSinkDisconnect*/
	                u8IntStsISR |= TYPEC_VSINKDISCONNECT_STATUS_MASK;                    
	            }		
            }
        }
        
        if (TYPEC_MAX_VSAFE_0V_MATCH_VAL == u8Data)
        {
            if (u8IsSourceport)
            {
#if (TRUE == INCLUDE_POWER_FAULT_HANDLING)               
                u8HandleUV = TRUE;
#endif                 
            }
            else
            {
                /* Verifies whether VBUS Drop is due to Source detach*/
                if ((TYPEC_ATTACHED_SNK == gasTypeCcontrol[u8PortNum].u8TypeCState) &&
                      (!DPM_IS_HARDRESET_IN_PROGRESS(u8PortNum)))                       
                {                                                              
                    if (FALSE == DPM_IS_PR_OR_FR_SWAP_IN_PROGRESS(u8PortNum))
                    {
                        /*Setting the flag that VBUS has gone below VSinkDisconnect*/
                        u8IntStsISR |= TYPEC_VSINKDISCONNECT_STATUS_MASK;
                    }                         
                }            
            }
        }

        if (TYPEC_VBUS_SNKDISCNT_MATCH_VAL == u8Data)
        {
            if (u8IsSourceport)
            {
#if (TRUE == INCLUDE_POWER_FAULT_HANDLING)                
                u8HandleUV = TRUE;
#endif                
            }
            else
            {
                /* Verifies whether VBUS Drop is due to Source detach*/
               if ((TYPEC_VBUS_5V_PRES == (gasTypeCcontrol[u8PortNum].u8IntStsISR & TYPEC_VBUS_PRESENCE_MASK))
                    || ((TYPEC_ATTACHED_SNK == gasTypeCcontrol[u8PortNum].u8TypeCState) &&
				   (TYPEC_ATTACHED_SNK_RUN_SM_SS != gasTypeCcontrol[u8PortNum].u8TypeCSubState)))
               {
                   /*u8HandleUV is FALSE*/
               }
               else
               {
#if (TRUE == INCLUDE_POWER_FAULT_HANDLING)                  
                    u8HandleUV = TRUE;
#endif                    
               }
            }
        }
        
#if (TRUE == INCLUDE_POWER_FAULT_HANDLING)
        if (TRUE == u8HandleUV)
        {   
            u16Data = SET_TO_ZERO;
            /* Read the sample En to determine whether under voltage is enabled */
            UPD_RegisterReadISR (u8PortNum, TYPEC_VBUS_SAMP_EN, (UINT8 *)&u16Data, BYTE_LEN_1);
            /* Verifying whether under voltage is enabled */
            if (TYPEC_UNDER_VOLT_THR3_MATCH & u16Data)
            {
                /*When PIO override is disabled, EN_VBUS/EN_SINK is disabled by FW on Power fault*/
                #if (FALSE == INCLUDE_UPD_PIO_OVERRIDE_SUPPORT)                         
                    UPD_DisablePIOOutputISR (u8PortNum);
                #endif   

                /* under voltage is considered if VBUS not lowered as part of Over voltage
                  Power Loss during an FRS should not be treated as an UV condition */
                if (FALSE == (gasDPM[u8PortNum].u8PowerFaultFlags & \
                        (DPM_HR_COMPLETE_WAIT_MASK | DPM_IGNORE_UV_DURING_FRS_MASK)))
                {                    
                    gasDPM[u8PortNum].u8PowerFaultISR |= DPM_POWER_FAULT_UV;
                }
            }      
        }
        else if (u8Data & TYPEC_VBUS_OVERVOLT_MATCH_BIT)
        {
            /* Over voltage is checked before desired voltage as TYPEC_VBUS_MATCH_OVER_V 
                checks for only over voltage bit being set are not*/
            gasDPM[u8PortNum].u8PowerFaultISR |= DPM_POWER_FAULT_OVP;
            
            /*When PIO override is disabled, EN_VBUS/EN_SINK is disabled by FW on Power fault*/
            #if (FALSE == INCLUDE_UPD_PIO_OVERRIDE_SUPPORT)                
                UPD_DisablePIOOutputISR (u8PortNum);  
            #endif
        }
        else
#endif /* endif for INCLUDE_POWER_FAULT_HANDLING*/       
        if ((TYPEC_VBUS_DESIRED_V_MATCH_VAL == u8Data) || \
                        (TYPEC_VBUS_DESIRED_N_UNDER_MATCH_VAL == u8Data))
        {
            u8IntStsISR &= ~TYPEC_VBUS_PRESENCE_MASK;
            UPD_RegisterReadISR (u8PortNum, TYPEC_VBUS_THR1, (UINT8 *)&u16Data, BYTE_LEN_2);
            u16Data = (UINT16)((u16Data * TYPEC_VBUS_THRX_UNITS_MILLI_V)
                              /gasTypeCcontrol[u8PortNum].fVBUSCorrectionFactor);

            if (u16Data <= gasDPM[u8PortNum].u16ExpectedVBUSVoltageInmV)
            {
                if (TYPEC_VBUS_5V == gasDPM[u8PortNum].u16ExpectedVBUSVoltageInmV)
                {
                    u8IntStsISR |= TYPEC_VBUS_5V_PRES;
                }
                else
                {
                    u8IntStsISR |= (gasDPM[u8PortNum].u8NegotiatedPDOIndex << TYPEC_VBUS_PRESENCE_POS);
                }
            }                       
        }

        /*Clearing the VBUS Change status interrupt*/
        UPD_RegisterReadISR (u8PortNum, TYPEC_VBUS_CHG_STS, &u8Data, BYTE_LEN_1);
        UPD_RegisterWriteISR (u8PortNum, TYPEC_VBUS_CHG_STS, &u8Data, BYTE_LEN_1);
    }
    
    gasTypeCcontrol[u8PortNum].u8IntStsISR = u8IntStsISR;
}

/*******************************************************************************************/
/*********************************TypeC CC Support Functions**************************************/
/*******************************************************************************************/

void TypeC_SetCCDebounceVariable (UINT8 u8PortNum, UINT8 u8Pwrrole)
{
    UINT8 u8CCDebMatch = gasTypeCcontrol[u8PortNum].u8CCDebMatch;
    UINT8 u8CCSrcSnkMatch = gasTypeCcontrol[u8PortNum].u8CCSrcSnkMatch;
    
    switch (u8Pwrrole)
    {
        #if  (TRUE == INCLUDE_PD_SINK)
        case TYPEC_UFP:
        {
          /* UFP_DFP_DEF (CC_THR0) ,UFP_DFP_1A5(CC_THR2), UFP_DFP_3A0 (CC_THR4) */ 
            u8CCDebMatch = (TYPEC_CC_THRES0_MATCH | TYPEC_CC_THRES2_MATCH \
                                   | TYPEC_CC_THRES4_MATCH | TYPEC_CC_THRES7_MATCH); 
            /*u8CCSrcSnkMatch variable will be set depending on the type of Source Rp attached
            TypeC_DecodeCCSourceRpValue(u8PortNum); function*/
            break;
        }
        #endif 
        
        
        #if  (TRUE == INCLUDE_PD_SOURCE)
        case TYPEC_DFP_DEFAULT_CURRENT:
        {          
            /*DFP_ACT_DEF(CC_THR0),DFP_UFP_DEF(CC_THR5)*/
            u8CCDebMatch = (TYPEC_CC_THRES0_MATCH | TYPEC_CC_THRES5_MATCH); 
            
            /*u8CCSrcSnkMatch is useful for finding which CC line has Sink attachment*/
            u8CCSrcSnkMatch = (TYPEC_CC_THRES5_MATCH); 
            break;
        }
        
        case TYPEC_DFP_1A5_CURRENT:
        {
            /*DFP_ACT_1A5(CC_THR1),DFP_UFP_1A5(CC_THR5)*/
            u8CCDebMatch = (TYPEC_CC_THRES1_MATCH | TYPEC_CC_THRES5_MATCH);
            
            /*u8CCSrcSnkMatch is useful for finding which CC line has Sink attachment*/
            u8CCSrcSnkMatch = (TYPEC_CC_THRES5_MATCH); 
            break;
        }
        
        case TYPEC_DFP_3A0_CURRENT:
        {
            /*DFP_ACT_3A0 (CC_THR3),DFP_UFP_3A0 (CC_THR6)*/
            u8CCDebMatch = (TYPEC_CC_THRES3_MATCH | TYPEC_CC_THRES6_MATCH);
     
            /*u8CCSrcSnkMatch is useful for finding which CC line has Sink attachment*/
            u8CCSrcSnkMatch = (TYPEC_CC_THRES6_MATCH); 
            break;
        }
        #endif /*INCLUDE_PD_SOURCE*/
        
        default:
            break; 
    }
    
    gasTypeCcontrol[u8PortNum].u8CCDebMatch = u8CCDebMatch;
    gasTypeCcontrol[u8PortNum].u8CCSrcSnkMatch = u8CCSrcSnkMatch;

}
void TypeC_ConfigCCComp (UINT8 u8PortNum, UINT8 u8ConfigVal)
{     
    UINT8 u8Data;
    UINT8 u8DesiredDBState;
    
     /*Enabling the Corresponding lines for the CC Debouncer Sampling */  
     UPD_RegisterRead (u8PortNum, TYPEC_CC_CTL1_HIGH, &u8Data, BYTE_LEN_1);
     
     /*Clearing the CC Comparator sampling bits before setting a particular configuration*/
     u8Data &= ~TYPEC_CC_COMP_CTL;
     u8Data |= u8ConfigVal;
     UPD_RegisterWrite (u8PortNum, TYPEC_CC_CTL1_HIGH, &u8Data, BYTE_LEN_1);
     
    /*Set CC Comparator OFF*/
    if (TYPEC_CC_COMP_CTL_DIS == u8ConfigVal)
    {  
        /* Wait until the CC Debouncer goes to inactive state  */ 
        u8DesiredDBState = TYPEC_CC_DB_ACTIVE;             
    }
    else
    {
        /* Wait until the CC Debouncer goes to Active state  */
        u8DesiredDBState = FALSE;      
    }
    
    do
    {
        UPD_RegisterRead (u8PortNum, TYPEC_CC_HW_CTL_HIGH, &u8Data, BYTE_LEN_1);
        
    }while((u8Data & TYPEC_CC_DB_ACTIVE) == u8DesiredDBState); 
    
}
#if(TRUE == INCLUDE_PD_DRP)
void TypeC_DRPSetCCSampleEnable (UINT8 u8PortNum, UINT8 u8RpCurrent)
{
  
    UINT8 u8MatchSel = TYPEC_CC_THRES0_MATCH;
    UPD_RegisterWrite (u8PortNum, TYPEC_DRP_SNK_SAMP_EN, &u8MatchSel,\
                       BYTE_LEN_1); 
    UPD_RegisterWrite (u8PortNum, TYPEC_DRP_CC_SNK_MATCH_EN, &u8MatchSel,\
                       BYTE_LEN_1);   
    UPD_RegisterWrite (u8PortNum, TYPEC_DRP_CC_SNK_DBCLR_EN, &u8MatchSel,\
                       BYTE_LEN_1);
    
    switch (u8RpCurrent)
    {
        case TYPEC_DFP_DEFAULT_CURRENT:
        {
            /*DFP_ACT_DEF(CC_THR0),DFP_UFP_DEF(CC_THR5)*/
            u8MatchSel = (TYPEC_CC_THRES0_MATCH | TYPEC_CC_THRES5_MATCH);
            break;
        }
        case TYPEC_DFP_1A5_CURRENT:
        {
            /*DFP_ACT_1A5(CC_THR1),DFP_UFP_1A5(CC_THR5)*/
            u8MatchSel = (TYPEC_CC_THRES1_MATCH | TYPEC_CC_THRES5_MATCH);
            break;
        }
        case TYPEC_DFP_3A0_CURRENT:
        {
            /*DFP_ACT_3A0 (CC_THR3),DFP_UFP_3A0 (CC_THR6)*/
            u8MatchSel = (TYPEC_CC_THRES3_MATCH | TYPEC_CC_THRES6_MATCH);
            break;
        }
        default:
            break;
            
    }
    UPD_RegisterWrite (u8PortNum, TYPEC_DRP_SRC_SAMP_EN, &u8MatchSel,\
                       BYTE_LEN_1); 
    UPD_RegisterWrite (u8PortNum, TYPEC_DRP_CC_SRC_MATCH_EN, &u8MatchSel,\
                       BYTE_LEN_1);   
    UPD_RegisterWrite (u8PortNum, TYPEC_DRP_CC_SRC_DBCLR_EN, &u8MatchSel,\
                       BYTE_LEN_1);    
}
#endif /*INCLUDE_PD_DRP*/

void TypeC_SetCCSampleEnable (UINT8 u8PortNum, UINT8 u8CCEnablePins)
{   
    if (u8CCEnablePins & TYPEC_ENABLE_CC1)
    {
        /*Setting the CC1_DBCLR_EN, CC1_SAMP_EN to enable debouncing for specific CC thresholds*/
        UPD_RegisterWrite (u8PortNum, TYPEC_CC1_DBCLR_EN, &gasTypeCcontrol[u8PortNum].u8CCDebMatch,\
                           BYTE_LEN_1);   
        UPD_RegisterWrite (u8PortNum, TYPEC_CC1_MATCH_EN, &gasTypeCcontrol[u8PortNum].u8CCDebMatch,\
                           BYTE_LEN_1);
        UPD_RegisterWrite (u8PortNum, TYPEC_CC1_SAMP_EN, &gasTypeCcontrol[u8PortNum].u8CCDebMatch,\
                           BYTE_LEN_1);
    }
    
    if (u8CCEnablePins & TYPEC_ENABLE_CC2)
    {    
        /*Setting the CC2_DBCLR_EN,CC2_SAMP_EN to sample for specific CC thresholds*/
        UPD_RegisterWrite (u8PortNum, TYPEC_CC2_DBCLR_EN, &gasTypeCcontrol[u8PortNum].u8CCDebMatch,\
                           BYTE_LEN_1);
        UPD_RegisterWrite (u8PortNum, TYPEC_CC2_MATCH_EN, &gasTypeCcontrol[u8PortNum].u8CCDebMatch,\
                           BYTE_LEN_1);
        UPD_RegisterWrite (u8PortNum, TYPEC_CC2_SAMP_EN, &gasTypeCcontrol[u8PortNum].u8CCDebMatch,\
                           BYTE_LEN_1);
    }    
}

void TypeC_SetCCPowerRole(UINT8 u8PortNum, UINT8 u8PowerRole, UINT8 u8ConfigVal, UINT8 u8CCPin)
{   
     UINT16 u16CCControlReg1Val;
     
     /* Register Read added to avoid over writing the register CC_CTL1 register */
     UPD_RegisterRead (u8PortNum, TYPEC_CC_CTL1, (UINT8 *)&u16CCControlReg1Val, BYTE_LEN_2);
     
     /*Clearing the Existing Pull Down and Pull up resistor values*/
     u16CCControlReg1Val &= TYPEC_CC1_CC2_RP_MASK;
     u16CCControlReg1Val &= TYPEC_CC1_CC2_RD_MASK;  
     
     if (PD_ROLE_SINK == u8PowerRole)
     {                   
        if (u8CCPin & TYPEC_ENABLE_CC1)
        {
           /*Setting the given config value as pull down resistor */
            u16CCControlReg1Val |= (u8ConfigVal << TYPEC_CC1_PULL_DOWN_POS); 
        } 
        
        if (u8CCPin & TYPEC_ENABLE_CC2)
        {
            u16CCControlReg1Val |= (u8ConfigVal << TYPEC_CC2_PULL_DOWN_POS); 
        }
        /*Setting the pull up resistor as open disconnect*/
        u16CCControlReg1Val |= (TYPEC_CC_RP_DIS);
     }
     else if (PD_ROLE_SOURCE == u8PowerRole)
     {
         if (u8CCPin & TYPEC_ENABLE_CC1) 
         {
            u16CCControlReg1Val |= (u8ConfigVal << TYPEC_CC1_RP_VAL_POS);
         }

         if (u8CCPin & TYPEC_ENABLE_CC2)
         {
            u16CCControlReg1Val |= (u8ConfigVal << TYPEC_CC2_RP_VAL_POS);
         }                                   
        
        /*Setting the pull down resistor as open disconnect*/
        u16CCControlReg1Val |= (TYPEC_CC1_PULL_DOWN_OPEN|TYPEC_CC2_PULL_DOWN_OPEN);
     }
     else
     {
         /* Do Nothing */
     }
     
    /*Writing the CC control register directly because it wont affect the CC comparator Sample 
    operation as it would have been turned down at this time*/
    /*Setting the Rd and Rp Value*/ 
    UPD_RegisterWrite (u8PortNum, TYPEC_CC_CTL1, (UINT8 *)&u16CCControlReg1Val, BYTE_LEN_2);

}
void TypeC_SetCCDeviceRole (UINT8 u8PortNum, UINT8 u8DevRole)
{
    /* Setting Port Device Role in TYPEC_CC_HW_CTL register */
    if (PD_ROLE_UFP == u8DevRole)
    {      
        UPD_RegByteClearBit (u8PortNum, TYPEC_CC_HW_CTL_LOW, TYPEC_DEV_ROLE);
    }
    else if (PD_ROLE_DFP == u8DevRole)
    {
        UPD_RegByteSetBit (u8PortNum, TYPEC_CC_HW_CTL_LOW, TYPEC_DEV_ROLE);
    }  
    else
    {
        /* Do Nothing */
    }
}
#if (TRUE == INCLUDE_PD_SOURCE)
void TypeC_SetCCDefaultRpValue (UINT8 u8PortNum)
{
    UINT16 u16Data;    
    
    u16Data = DPM_GET_CONFIGURED_SOURCE_RP_VAL(u8PortNum);
    
    /*Setting Rp Current Source as user given and Rd as Open disconnect in both CC1 and CC2*/ 
    TypeC_SetCCPowerRole (u8PortNum, TYPEC_ROLE_SOURCE, (UINT8) u16Data, TYPEC_ENABLE_CC1_CC2);
  
    /*Setting the u8CCDebMatch and u8CCSrcSnkMatch variable accordingly as per Rp Value*/
    TypeC_SetCCDebounceVariable (u8PortNum, (UINT8) u16Data);
     
    /*Resetting the CC Debounce clear enable,CC Match Enable,CC Sample Enable register*/
    TypeC_SetCCSampleEnable (u8PortNum, TYPEC_ENABLE_CC1_CC2);
       
    /*Setting user given Rp Value as Current Rp Value in CurrentRpVal bit of u8PortSts variable*/
    gasTypeCcontrol[u8PortNum].u8PortSts &= ~TYPEC_CURR_RPVAL_MASK;
    gasTypeCcontrol[u8PortNum].u8PortSts |= (u16Data << TYPEC_CURR_RPVAL_POS);
}
#endif /*INCLUDE_PD_SOURCE*/

#if (TRUE == INCLUDE_PD_SINK)
void TypeC_DecodeCCSourceRpValue (UINT8 u8PortNum)
{   
    /*Assigning the structure variables to local variables to reduce the code size as
    accessing the structure members takes lot of code*/
    UINT8 u8PortSts = gasTypeCcontrol[u8PortNum].u8PortSts;
    
    UINT8 u8CCSrcSnkMatch = gasTypeCcontrol[u8PortNum].u8CCSrcSnkMatch;
    
    /*Clearing the Current Rp Value of Source*/    
    u8PortSts &= ~TYPEC_CURR_RPVAL_MASK;
     
    switch (gasTypeCcontrol[u8PortNum].u8CC1MatchISR ^ gasTypeCcontrol[u8PortNum].u8CC2MatchISR)	
	{
		case TYPEC_DFP_DEF_ATT:
        case (TYPEC_DFP_DEF_ATT ^ TYPEC_SNK_CCTHRES_VCONN_ON):
        {
            /*Setting the Current Rp Value of Source as Default Current*/
            u8PortSts |= (TYPEC_RP_DEFAULT_CURRENT << TYPEC_CURR_RPVAL_POS);
            
            u8CCSrcSnkMatch = TYPEC_DFP_DEF_ATT;
            break;
        }
        
		case TYPEC_DFP_1A5_ATT:
        case (TYPEC_DFP_1A5_ATT ^ TYPEC_SNK_CCTHRES_VCONN_ON):   
        {
            /*Setting the Current Rp Value of Source as 1.5A@5V */
            u8PortSts |= (TYPEC_RP_CURRENT_15 << TYPEC_CURR_RPVAL_POS); 
             
            u8CCSrcSnkMatch = TYPEC_DFP_1A5_ATT;             
            break;          
        }
        
		case TYPEC_DFP_3A0_ATT:
        case (TYPEC_DFP_3A0_ATT ^ TYPEC_SNK_CCTHRES_VCONN_ON):
		{
            /*Setting the Current Rp Value of Source as 3.0A@5V */
            u8PortSts |= (TYPEC_RP_CURRENT_30 << TYPEC_CURR_RPVAL_POS);
            
            u8CCSrcSnkMatch = TYPEC_DFP_3A0_ATT;
                          
            break;
        }
        
        default:
        {
            break;
        }         
    }
    
    gasTypeCcontrol[u8PortNum].u8PortSts = u8PortSts; 
    gasTypeCcontrol[u8PortNum].u8CCSrcSnkMatch = u8CCSrcSnkMatch;    
}
#endif/*INCLUDE_PD_SINK*/
/*******************************************************************************************/
/*********************************TypeC VCONN Support Functions**************************************/
/*******************************************************************************************/
void TypeC_EnabDisVCONN (UINT8 u8PortNum, UINT8 u8EnableDisable)
{
    UINT8 u8Data;
    
    if (TYPEC_VCONN_DISABLE == u8EnableDisable)
    {     
        gasTypeCcontrol[u8PortNum].u8PortSts |= TYPEC_VCONN_DISCHARGE_ON_MASK;

        /*Disable VCONN by switching off the VCONN FETS*/
        UPD_RegByteClearBit (u8PortNum, TYPEC_VBUS_CTL1, TYPEC_VCONN_FETS);

        /*Disable the VCONN OCS monitoring in VBUS_CTL1 register*/
        UPD_RegByteClearBit (u8PortNum, TYPEC_VBUS_CTL1_LOW, TYPEC_VCONN_OCS_ENABLE);

        /*Setting CC Comparator OFF*/
        TypeC_ConfigCCComp (u8PortNum, TYPEC_CC_COMP_CTL_DIS);

        /*Setting the CC threshold 0 register to 41d which is a VCONN discharge threshold */
        UPD_RegWriteWord (u8PortNum, TYPEC_CC_THR0, TYPEC_CC_THR_VAL_VCONN_DIS);

        UPD_RegisterRead (u8PortNum, TYPEC_CC_CTL1_LOW, &u8Data, BYTE_LEN_1);

        gasTypeCcontrol[u8PortNum].u8CCDebMatch = (TYPEC_CC_THRES0_MATCH); 

        /*Poll the variable to see which CC pin was VCONN Source*/
        if (TYPEC_VCONN_SOURCE_CC1 == (gasTypeCcontrol[u8PortNum].u8IntStsISR & TYPEC_VCONN_SOURCE_MASK))
        {
            /*CC1 is the source of VCONN*/
            u8Data &= ~TYPEC_CC1_PULL_DOWN;
            u8Data |= (TYPEC_CC1_PULL_DOWN_RA) ; 
             
            /*Setting Ra Pull down on either CC1 or CC2 based on VCONN Source pin*/
            UPD_RegisterWrite (u8PortNum, TYPEC_CC_CTL1_LOW, &u8Data, BYTE_LEN_1);

            /*Setting the Rp value as "current source disabled" in CC1*/   
            UPD_RegByteClearBit (u8PortNum, TYPEC_CC_CTL1_HIGH, (TYPEC_CC1_RP_VAL));

            TypeC_SetCCSampleEnable (u8PortNum,TYPEC_ENABLE_CC1);           
        }
        else
        {
            /*CC2 is the source of VCONN*/
            u8Data &= ~TYPEC_CC2_PULL_DOWN;
            u8Data |= (TYPEC_CC2_PULL_DOWN_RA); 

            /*Setting Ra Pull down on either CC1 or CC2 based on VCONN Source pin*/
            UPD_RegisterWrite (u8PortNum, TYPEC_CC_CTL1_LOW, &u8Data, BYTE_LEN_1);

            /*Setting the Rp value as "current source disabled" in CC2*/   
            UPD_RegByteClearBit (u8PortNum, TYPEC_CC_CTL1_HIGH, (TYPEC_CC2_RP_VAL));

            TypeC_SetCCSampleEnable (u8PortNum,TYPEC_ENABLE_CC2);
        }
           
        /*Setting CC Comparator to sample both the CC1 and CC2*/
        TypeC_ConfigCCComp (u8PortNum, TYPEC_CC_COMP_CTL_CC1_CC2);
                
        DEBUG_PRINT_PORT_STR (u8PortNum,"TYPEC: VCONN_DISCH INITIATED\r\n");        
    }
    else
    {   
        #if (TRUE == INCLUDE_POWER_FAULT_HANDLING)
        /*Start the VCONN power good timer if VCONN OCS fault counter is not zero*/
        if (gasDPM[u8PortNum].u8VCONNPowerFaultCount != SET_TO_ZERO)
        {
            PDTimer_Kill (gasDPM[u8PortNum].u8VCONNPowerGoodTmrID);
            gasDPM[u8PortNum].u8VCONNPowerGoodTmrID = PDTimer_Start (gasCfgStatusData.sPerPortData[u8PortNum].u16PowerGoodTimerInms,\
                                                      DPM_VCONNPowerGood_TimerCB, u8PortNum, (UINT8)SET_TO_ZERO);                  
        }
        #endif
            
        UINT8 u8VCONN1Enable = FALSE;
        
        UINT8 u8CCRead = UPD_RegReadByte (u8PortNum, TYPEC_CC_CTL1_HIGH);
               
        if (PD_ROLE_SOURCE == DPM_GET_CURRENT_POWER_ROLE(u8PortNum))
        {       
            /*Powered cable attached in CC1 when CC1 is greater than CC2*/
            /*Powered cable attached in CC1 when PD Communication is enabled in CC2*/
            if (((gasTypeCcontrol[u8PortNum].u8CC1MatchISR > gasTypeCcontrol[u8PortNum].u8CC2MatchISR) \
                && (gasTypeCcontrol[u8PortNum].u8PortSts & TYPEC_PWDCABLE_PRES_MASK)) || (u8CCRead & TYPEC_CC_COM_SEL))
            {
                u8VCONN1Enable = TRUE;
            }
        }       
        else
        {
            /*Powered cable attached in CC1*/
            if (gasTypeCcontrol[u8PortNum].u8CC1MatchISR < gasTypeCcontrol[u8PortNum].u8CC2MatchISR)
            {
                u8VCONN1Enable = TRUE;
            }        
        }
                
        /*Powered cable attached in CC1*/
        if (u8VCONN1Enable)
        {    
            gasTypeCcontrol[u8PortNum].u8PortSts &= ~TYPEC_VCONN_ON_REQ_MASK;
            gasTypeCcontrol[u8PortNum].u8PortSts |= TYPEC_VCONN1_ON_REQ;
                                       
            /*Enable VCONN FET in CC1*/
            UPD_RegByteSetBit (u8PortNum, TYPEC_VBUS_CTL1, TYPEC_VCONN1_ENABLE);          
        }
        /*Powered cable attached in CC2*/
        else
        {     
            gasTypeCcontrol[u8PortNum].u8PortSts &= ~TYPEC_VCONN_ON_REQ_MASK;
            gasTypeCcontrol[u8PortNum].u8PortSts |= TYPEC_VCONN2_ON_REQ;

            /*Enable VCONN FET in CC2*/
            UPD_RegByteSetBit (u8PortNum, TYPEC_VBUS_CTL1, TYPEC_VCONN2_ENABLE);
        }
        
        if (DPM_CFG_VCONN_OCS_ENABLE == DPM_GET_CONFIGURED_VCONN_OCS_EN(u8PortNum)) 
        {
            /*Enable VCONN OCS detection only if Bit 9(VCONN OCS En) of 
              gasCfgStatusData.sPerPortData[u8PortNum].u32CfgData defined as 1*/
            /*Enable the VCONN OCS monitoring in VBUS_CTL1 register*/
            UPD_RegByteSetBit (u8PortNum, TYPEC_VBUS_CTL1_LOW, TYPEC_VCONN_OCS_ENABLE);
        }                
    } 
}                               
/*******************************************************************************************/
/*********************************Type C modules Interrupt Functions**************************************/
/**********************************************************************************************/ 
void TypeC_VCONNDISCHOnIntrHandler (UINT8 u8PortNum)
{          
    /*Clearing the CC interrupt status in u8IntStsISR variable at the start of this function to 
    make this ISR safe as after TypeC_OnVCONNDISCHComplete function call,
    Interrupt will immediately be fired*/  
    gasTypeCcontrol[u8PortNum].u8IntStsISR &= ~TYPEC_CCINT_STATUS_MASK;
    
    /*Clearing the VCONN source bit in u8IntStsISR variable as VCONN is disabled */
    gasTypeCcontrol[u8PortNum].u8IntStsISR &= ~TYPEC_VCONN_SOURCE_MASK;
    
    /* Clearing VCONN_STATUS bit in Port Connection Status register */
    gasCfgStatusData.sPerPortData[u8PortNum].u32PortConnectStatus &= ~(DPM_PORT_VCONN_ENABLE_STATUS);

    /*VCONN discharge complete can occur while the sink is still attached or detached for source port*/
    /*VCONN discharge complete can occur while the source is still attached or detached for sink port*/
    
    /*Detach Event occurred during this VCONN Discharge process will be handled since the 
    TypeC_OnVCONNDISCHComplete function will again restart the CC Comparator*/
    TypeC_OnVCONNDISCHComplete (u8PortNum);
    
    gasTypeCcontrol[u8PortNum].u8PortSts &= ~TYPEC_VCONN_DISCHARGE_ON_MASK;     
}

void TypeC_VCONNOnIntrHandler (UINT8 u8PortNum)
{
    if (PD_ROLE_SOURCE == DPM_GET_CURRENT_POWER_ROLE(u8PortNum))
    {
        /*VCONN Enabled in CC1 and Sink Attached in CC2*/
        if ((gasTypeCcontrol[u8PortNum].u8PortSts & TYPEC_VCONN1_ON_REQ) && \
            (SET_TO_ZERO == gasTypeCcontrol[u8PortNum].u8CC1MatchISR) && 
              (gasTypeCcontrol[u8PortNum].u8CC2MatchISR == gasTypeCcontrol[u8PortNum].u8CCSrcSnkMatch))
        {            
            /*Setting the u8IntStsISR variable that CC1 is the VCONN source*/
            gasTypeCcontrol[u8PortNum].u8IntStsISR |= TYPEC_VCONN_SOURCE_CC1;
            
            //DEBUG_PRINT_PORT_STR(u8PortNum,"TYPEC: VCONN Enabled in CC1\r\n");            
        }
		 /*VCONN Enabled in CC2 and Sink Attached in CC1*/
        else if ((gasTypeCcontrol[u8PortNum].u8PortSts & TYPEC_VCONN2_ON_REQ) && \
            (SET_TO_ZERO == gasTypeCcontrol[u8PortNum].u8CC2MatchISR) &&
            (gasTypeCcontrol[u8PortNum].u8CC1MatchISR == gasTypeCcontrol[u8PortNum].u8CCSrcSnkMatch))
        {        
            /*Setting the u8IntStsISR variable that CC2 is the VCONN source*/
            gasTypeCcontrol[u8PortNum].u8IntStsISR |= TYPEC_VCONN_SOURCE_CC2;
            
            //DEBUG_PRINT_PORT_STR(u8PortNum,"TYPEC: VCONN Enabled in CC2\r\n");  
        }
        else
        {
            /* Do Nothing */
        }            
    }
    else
    {   
        /* During a Sink initiated VCONN Swap, Sink port which is presently not 
           the VCONN Source would drive the VCONN and wait for VCONN ON interrupt. 
           Before the VCONN Swap message reaches the Source port, Source port will set 
           the Rp value to SINK_TX_NG (1.5A@5V) for the Sink to not initiate 
           any AMS. During this scenario, both Rp change interrupt and VCONN ON interrupt 
           triggers at the same time. So, decode the current Rp value before handling 
           VCONN ON Interrupt */
        #if (TRUE == INCLUDE_PD_SINK)
        TypeC_DecodeCCSourceRpValue(u8PortNum);
        #endif 
        
        /*VCONN Enabled in CC1 and Source Attached in CC2*/ 
        if ((gasTypeCcontrol[u8PortNum].u8PortSts & TYPEC_VCONN1_ON_REQ) && \
            (TYPEC_SNK_CCTHRES_VCONN_ON == gasTypeCcontrol[u8PortNum].u8CC1MatchISR) &&
            ((gasTypeCcontrol[u8PortNum].u8CC2MatchISR == gasTypeCcontrol[u8PortNum].u8CCSrcSnkMatch)))
        {         
             /*Setting the u8IntStsISR variable that CC1 is the VCONN source*/
            gasTypeCcontrol[u8PortNum].u8IntStsISR |= TYPEC_VCONN_SOURCE_CC1;
            
            //DEBUG_PRINT_PORT_STR(u8PortNum,"TYPEC: VCONN Enabled in CC1\r\n");  
        }
         /*VCONN Enabled in CC2 and Source Attached in CC1*/
        else if ((gasTypeCcontrol[u8PortNum].u8PortSts & TYPEC_VCONN2_ON_REQ) && \
            (TYPEC_SNK_CCTHRES_VCONN_ON == gasTypeCcontrol[u8PortNum].u8CC2MatchISR) &&
             (gasTypeCcontrol[u8PortNum].u8CC1MatchISR == gasTypeCcontrol[u8PortNum].u8CCSrcSnkMatch))
        {        
             /*Setting the u8IntStsISR variable that CC2 is the VCONN source*/
            gasTypeCcontrol[u8PortNum].u8IntStsISR |= TYPEC_VCONN_SOURCE_CC2;          
            
            //DEBUG_PRINT_PORT_STR(u8PortNum,"TYPEC: VCONN Enabled in CC2\r\n");  
        }
        else
        {
            /* Do Nothing */
        }    
    }    
    /*Handle only the CC interrupt for VCONN_ON event.CC interrupts for detach 
    event or others should be handled in either SrcIntr Handler or SnkIntr Handler function*/
    if (gasTypeCcontrol[u8PortNum].u8IntStsISR & TYPEC_VCONN_SOURCE_MASK)
    {
         /*Clearing the VCONN ON Request mask if VCONN has turned ON*/
         gasTypeCcontrol[u8PortNum].u8PortSts &= ~TYPEC_VCONN_ON_REQ_MASK; 

         /*Clearing the CC Interrupt flag since CC change is because of the VCONN ON event*/
         gasTypeCcontrol[u8PortNum].u8IntStsISR &= ~TYPEC_CCINT_STATUS_MASK;
         
         /* Setting the Port Connect Status to indicate VCONN is enabled */
         gasCfgStatusData.sPerPortData[u8PortNum].u32PortConnectStatus |= DPM_PORT_VCONN_ENABLE_STATUS;    
    }
}

void TypeC_CCVBUSIntrHandler (UINT8 u8PortNum)
{     
    MCHP_PSF_HOOK_DISABLE_GLOBAL_INTERRUPT();
    
    /*Handling the state changes for CC and VBUS interrupts in foreground and not in ISR to 
    avoid variable corruption*/

#if(TRUE == INCLUDE_PD_DRP)
    TypeC_DRPIntrHandler (u8PortNum);
#endif

#if (TRUE == INCLUDE_PD_SINK)
    if (gasTypeCcontrol[u8PortNum].u8IntStsISR & TYPEC_VSINKDISCONNECT_STATUS_MASK)
    {
        /*Voltage dropped to vSinkDisconnect in Attached Sink State before the Source CC detach 
        event occurred*/
        /*VSinkDisconnect event occurred after the CC detach event is not processed
        which is a violation of specification*/
        /*This is done for easy handling*/
        if ((TYPEC_ATTACHED_SNK == gasTypeCcontrol[u8PortNum].u8TypeCState) && \
                (TYPEC_ATTACHED_SNK_RUN_SM_SS == gasTypeCcontrol[u8PortNum].u8TypeCSubState))
        {
            gasTypeCcontrol[u8PortNum].u8TypeCSubState = TYPEC_ATTACHED_SNK_PD_DEB_TIMEOUT_SS;
        }
        
        /*Clearing the VsinkDisconnect interrupt Status set*/
        gasTypeCcontrol[u8PortNum].u8IntStsISR &= ~TYPEC_VSINKDISCONNECT_STATUS_MASK;         
    }    
#endif 
    
    if (gasTypeCcontrol[u8PortNum].u8IntStsISR & TYPEC_CCINT_STATUS_MASK)
    {        
        //DEBUG_PRINT_PORT_STR(gasTypeCcontrol[u8PortNum].u8CC1MatchISR,"TYPEC: CC1 register\r\n");
        //DEBUG_PRINT_PORT_STR(gasTypeCcontrol[u8PortNum].u8CC2MatchISR,"TYPEC: CC2 register\r\n");
                 
        if (gasTypeCcontrol[u8PortNum].u8PortSts & TYPEC_VCONN_DISCHARGE_ON_MASK)
        {
            TypeC_VCONNDISCHOnIntrHandler (u8PortNum);
        }   
        else if (gasTypeCcontrol[u8PortNum].u8PortSts & TYPEC_VCONN_ON_REQ_MASK)
        {
            TypeC_VCONNOnIntrHandler (u8PortNum);
        }
        else
        {
            /* Do Nothing */
        }

        if (gasTypeCcontrol[u8PortNum].u8IntStsISR & TYPEC_CCINT_STATUS_MASK)
        {
            switch (gasTypeCcontrol[u8PortNum].u8TypeCState)
            {          
                #if (TRUE == INCLUDE_PD_SOURCE)
                case TYPEC_UNATTACHED_SRC:
                case TYPEC_ATTACHWAIT_SRC:
                case TYPEC_ATTACHED_SRC:   
                {            
                    TypeC_SrcIntrHandler (u8PortNum); 
                    break;
                }
                #endif      
                #if (TRUE == INCLUDE_PD_SINK)
                case TYPEC_UNATTACHED_SNK:
                case TYPEC_ATTACHWAIT_SNK:
                case TYPEC_ATTACHED_SNK:
                {                          
                    TypeC_SnkIntrHandler (u8PortNum);
                    break;
                }                
                #endif
                
                case TYPEC_AUDIO_ACCESSORY:
                case TYPEC_UNORIENTED_DEBUG_ACCESSORY_SRC:
                {                
                    /*Only the CC Detach is expected in both the above states.Hence CC interrupt leads 
                    to unattached state*/
                    gasTypeCcontrol[u8PortNum].u8TypeCSubState = TYPEC_UNATTACHED_SRC_ENTRY_SS;
                    gasTypeCcontrol[u8PortNum].u8TypeCState = TYPEC_UNATTACHED_SRC;
                    break; 
                }
                
                default:
                {
                    break;
                }                                
            }
            /*Clearing the CC interrupt status in u8IntStsISR variable*/
            gasTypeCcontrol[u8PortNum].u8IntStsISR &= ~TYPEC_CCINT_STATUS_MASK;        
        }		        
    }
    MCHP_PSF_HOOK_ENABLE_GLOBAL_INTERRUPT();
}

#if(TRUE == INCLUDE_PD_DRP)
void TypeC_DRPIntrHandler (UINT8 u8PortNum)
{
 	UINT8 u8Data = SET_TO_ZERO;
    
    if (TRUE == (gasTypeCcontrol[u8PortNum].u8DRPStsISR & TYPEC_DRP_DONE_INTERRUPT))
    {
        /*Get the state being advertised from DRP_CTL_HIGH register*/
        UPD_RegisterReadISR (u8PortNum, TYPEC_DRP_CTL_HIGH, &u8Data, BYTE_LEN_1);
        
        if (TRUE == (u8Data & TYPEC_DRP_ADVERTISING_STATE))
        {
            //DEBUG_PRINT_PORT_STR (u8PortNum,"DRP attached as Sink-UFP\r\n");
            /*Set power and data role status as Sink/UFP*/
            DPM_UpdatePowerRole (u8PortNum, PD_ROLE_SINK);
            DPM_UpdateDataRole (u8PortNum, PD_ROLE_UFP);
        }
        else
        {
            //DEBUG_PRINT_PORT_STR (u8PortNum,"DRP attached as Source-DFP\r\n");
            /*Set power and data role status as Source/DFP*/
            DPM_UpdatePowerRole (u8PortNum, PD_ROLE_SOURCE);
            DPM_UpdateDataRole (u8PortNum, PD_ROLE_DFP);
        }   
               
        gasTypeCcontrol[u8PortNum].u8TypeCState = TYPEC_INIT;
        gasTypeCcontrol[u8PortNum].u8TypeCSubState = SET_TO_ZERO;
        
        gasTypeCcontrol[u8PortNum].u8DRPStsISR &= (~TYPEC_DRP_DONE_INTERRUPT);        
    }
#if(TRUE == INCLUDE_PD_FR_SWAP)
    else if (gasTypeCcontrol[u8PortNum].u8DRPStsISR & TYPEC_FRS_XMT_RCV_STS_INTERRUPT)
    {       
        /* FRS PD MAC Override bit is automatically set when FRS signaling is sent/received
           by the FRS FSM. This bit stays asserted until cleared by FW. Clearing this bit 
           allows PD MAC communication to resume */
        UPD_RegByteClearBit (u8PortNum, TYPEC_FRS_CTL_HIGH, (UINT8)TYPEC_FRS_PD_MAC_OVR);
        
        if (PD_ROLE_SOURCE == DPM_GET_CURRENT_POWER_ROLE(u8PortNum))
        {
            //DEBUG_PRINT_PORT_STR (u8PortNum,"TYPEC:Handle FRS XMT INTR\r\n");            
            
            /* Set Rp value to SinkTxOK, so that sink partner can initiate
               FR_Swap message */            
            #if (TRUE == INCLUDE_PD_3_0)    
                PRL_SetCollisionAvoidance (u8PortNum, TYPEC_SINK_TXOK);
            #endif 
        }
        else
        {
            //DEBUG_PRINT_PORT_STR (u8PortNum,"TYPEC:Handle FRS RCV INTR\r\n");                   
                
            /* Register internal event to start FR_Swap AMS */
            DPM_RegisterInternalEvent (u8PortNum, DPM_INT_EVT_INITIATE_FR_SWAP);                
        }
        
        /* When a Fast Role Swap signal is transmitted or received, any active 
           transmission shall be overridden and any pending messages shall be discarded */
        PE_ResetParams (u8PortNum);
        
        /* Setting this flag will discard any pending messages in Protocol layer */
        DPM_SET_FRS_SIGNAL_XMT_OR_RCV_DONE_STS(u8PortNum);
        
        /* Setting this flag will ignore the processing of UV during an FRS event 
           since VBUS will trip down during an FRS. Also, clear the UV PIO Override */
        #if (TRUE == INCLUDE_POWER_FAULT_HANDLING)        
            gasDPM[u8PortNum].u8PowerFaultFlags |= DPM_IGNORE_UV_DURING_FRS_MASK;
        #if (TRUE == INCLUDE_UPD_PIO_OVERRIDE_SUPPORT)			
            UPD_RegByteClearBit (u8PortNum, UPD_PIO_OVR_EN, UPD_PIO_OVR_1);
        #endif         
        #endif

        gasTypeCcontrol[u8PortNum].u8DRPStsISR &= ~(TYPEC_FRS_XMT_RCV_STS_INTERRUPT);
    }
#endif
}
#endif

#if (TRUE == INCLUDE_PD_SOURCE)
void TypeC_SrcIntrHandler (UINT8 u8PortNum)
{	 
    /*Assigning the structure variables to local variables to reduce the code size as
    accessing the structure members takes lot of code*/  
  
    UINT8 u8TypeCState = gasTypeCcontrol[u8PortNum].u8TypeCState;
    UINT8 u8TypeCSubState = gasTypeCcontrol[u8PortNum].u8TypeCSubState;
        
	switch (gasTypeCcontrol[u8PortNum].u8CC1MatchISR ^ gasTypeCcontrol[u8PortNum].u8CC2MatchISR)
	{
        /*Only Powered Cable is present*/
		case TYPEC_PWD_CABLE_ATT_DEF:
		case TYPEC_PWD_CABLE_ATT_1A5:
		case TYPEC_PWD_CABLE_ATT_3A0:
		{	
            /*Setting the Powered cable presence in u8PortSts variable*/
            gasTypeCcontrol[u8PortNum].u8PortSts |= TYPEC_PWDCABLE_PRES_MASK;

            //DEBUG_PRINT_PORT_STR (u8PortNum,"TYPEC: Only Powered Cable is Present in CC");
            //DEBUG_PRINT_PORT_STR (((gasTypeCcontrol[u8PortNum].u8CC1MatchISR > gasTypeCcontrol[u8PortNum].u8CC2MatchISR) ? 1 : 2),"\r\n"); 
            
            /*Move to TYPEC_UNATTACHED_WAIT_SRC state if current state is TYPEC_ATTACHED_SRC. 
              A PR_Swap/FR_Swap from Sink to Source should not be considered 
              as a detach. So, don't do anything */
            if ((TYPEC_ATTACHED_SRC == u8TypeCState)
                #if (TRUE == (INCLUDE_PD_PR_SWAP || INCLUDE_PD_FR_SWAP))
                    && (u8TypeCSubState != TYPEC_ATTACHED_SRC_SWAP_RD_PRES_DETECT_SS)
                #endif
                    )
            {
                u8TypeCState = TYPEC_UNATTACHED_WAIT_SRC;
                u8TypeCSubState = TYPEC_UNATTACHED_WAIT_SRC_ENTRY_SS;            
            } 
            else if (TYPEC_ATTACHWAIT_SRC == u8TypeCState)
            {
                /*Kill the TCC debounce timer running already*/
                TypeC_KillTypeCTimer(u8PortNum);
                
                /*Move to TYPEC_UNATTACHED_SRC state if current state is  TYPEC_ATTACHWAIT_SRC*/
                u8TypeCState = TYPEC_UNATTACHED_SRC;
                u8TypeCSubState = TYPEC_UNATTACHED_SRC_ENTRY_SS;                             
            }
            else
            {
                /* Do Nothing */
            }
            break;
		}
		
		
        /*Only Sink Device is Present*/
		case TYPEC_UFP_ATT_DEF:
        case TYPEC_UFP_ATT_3A0:
		{
            //DEBUG_PRINT_PORT_STR (u8PortNum,"TYPEC: Only Sink is Present in CC");
            //DEBUG_PRINT_PORT_STR (((gasTypeCcontrol[u8PortNum].u8CC1MatchISR > gasTypeCcontrol[u8PortNum].u8CC2MatchISR) ? 1 : 2),"\r\n");               
            
            if (TYPEC_UNATTACHED_SRC == u8TypeCState)
            {                                  
                /*Setting the state for tCCDebounce*/
                u8TypeCState = TYPEC_ATTACHWAIT_SRC; 
                u8TypeCSubState = TYPEC_ATTACHWAIT_SRC_ENTRY_SS;                                

                /*Clearing the Powered cable presence in u16PortSts variable*/
                gasTypeCcontrol[u8PortNum].u8PortSts &= ~TYPEC_PWDCABLE_PRES_MASK;
            } 
#if (TRUE == (INCLUDE_PD_PR_SWAP || INCLUDE_PD_FR_SWAP))
            else if ((TYPEC_ATTACHED_SRC == u8TypeCState) && 
                         (TYPEC_ATTACHED_SRC_SWAP_RD_PRES_DETECT_SS == u8TypeCSubState))
            {                
                /* This condition would be hit during Sink to Source PR_Swap/FR_Swap */
                u8TypeCSubState = TYPEC_ATTACHED_SRC_DRIVE_PWR_SS;                                
                /*Clearing the Powered cable presence in u16PortSts variable*/
                gasTypeCcontrol[u8PortNum].u8PortSts &= ~TYPEC_PWDCABLE_PRES_MASK;
            }        
#endif 
            else 
            {
                /* Do Nothing */
            }
            break;            
		}		
		
        /*Both Powered cable and sink is Present*/
		case (TYPEC_PWD_CABLE_ATT_DEF ^ TYPEC_UFP_ATT_DEF):
		case (TYPEC_PWD_CABLE_ATT_1A5 ^ TYPEC_UFP_ATT_1A5):
		case (TYPEC_PWD_CABLE_ATT_3A0 ^ TYPEC_UFP_ATT_3A0):
		{          
            /*Setting the Powered cable presence in u8PortSts variable*/
            gasTypeCcontrol[u8PortNum].u8PortSts |= TYPEC_PWDCABLE_PRES_MASK;
          
            //DEBUG_PRINT_PORT_STR (u8PortNum,"TYPEC: Sink is Present in CC");
            //DEBUG_PRINT_PORT_STR (((gasTypeCcontrol[u8PortNum].u8CC1MatchISR > gasTypeCcontrol[u8PortNum].u8CC2MatchISR) ? 2 : 1),"\r\n");
            
            //DEBUG_PRINT_PORT_STR (u8PortNum,"TYPEC: Powered Cable is Present in CC");
            //DEBUG_PRINT_PORT_STR (((gasTypeCcontrol[u8PortNum].u8CC1MatchISR > gasTypeCcontrol[u8PortNum].u8CC2MatchISR) ? 1 : 2),"\r\n");
            
            if (TYPEC_UNATTACHED_SRC == u8TypeCState)
            {             
                /*Setting the state for tCCDebounce*/
                u8TypeCState = TYPEC_ATTACHWAIT_SRC;
                u8TypeCSubState = TYPEC_ATTACHWAIT_SRC_ENTRY_SS;                
            } 
#if (TRUE == (INCLUDE_PD_PR_SWAP || INCLUDE_PD_FR_SWAP))
            else if ((TYPEC_ATTACHED_SRC == u8TypeCState) &&
                            (TYPEC_ATTACHED_SRC_SWAP_RD_PRES_DETECT_SS == u8TypeCSubState))
            {                
                u8TypeCSubState = TYPEC_ATTACHED_SRC_DRIVE_PWR_SS;                                
            }
#endif 
            else 
            {
                /* Do Nothing */
            }
            break;
		}
		
		
		case TYPEC_CC1CC2_EQ:
		{           
            /*Audio Adapter Accessory is Present*/         
            if (gasTypeCcontrol[u8PortNum].u8CC1MatchISR == gasTypeCcontrol[u8PortNum].u8CCDebMatch)
            {   
                /*Consider the CC Sample values as valid Audio accessory attach only at the 
                unattached state*/
                /*This avoids unwanted transition in attached state*/
                if (TYPEC_UNATTACHED_SRC == u8TypeCState)
                {                
                    //DEBUG_PRINT_PORT_STR(u8PortNum,"TYPEC: AUDIO ACCESSORY is present\r\n");
                    
                    /*Setting the state for tCCDebounce*/
                    u8TypeCState = TYPEC_ATTACHWAIT_SRC;
                    u8TypeCSubState = TYPEC_ATTACHWAIT_SRC_ENTRY_SS;                
                }			    				
            }
            /*No Devices are Present*/
            else if (!gasTypeCcontrol[u8PortNum].u8CC1MatchISR)
            {             
                /*Clearing the Powered cable presence in u8PortSts variable*/
                gasTypeCcontrol[u8PortNum].u8PortSts &= ~TYPEC_PWDCABLE_PRES_MASK;
                
                //DEBUG_PRINT_PORT_STR (u8PortNum,"TYPEC: No Devices are present\r\n");                
                                                
                if (TYPEC_ATTACHWAIT_SRC == u8TypeCState)
                {
                    /*Kill the TCC debounce timer running already*/
                    TypeC_KillTypeCTimer(u8PortNum);
                    
                    /*Move to TYPEC_UNATTACHED_SRC state  if current state is 
                    TYPEC_ATTACHWAIT_SRC*/
                    u8TypeCState = TYPEC_UNATTACHED_SRC;
                    u8TypeCSubState = TYPEC_UNATTACHED_SRC_ENTRY_SS;                              
                }
                else if (TYPEC_ATTACHED_SRC == u8TypeCState)
                {                                         
                    /*Move to TYPEC_UNATTACHED_WAIT_SRC state if current state is 
                    TYPEC_ATTACHED_SRC. A FR_Swap from Sink to Source should 
                    not be considered as a detach. So, don't do anything */
                    if (!((DPM_IS_PR_OR_FR_SWAP_IN_PROGRESS(u8PortNum)) && \
                            (DPM_IS_FRS_XMT_OR_DET_ENABLED(u8PortNum))))
                    {               
                        u8TypeCState = TYPEC_UNATTACHED_WAIT_SRC;
                        u8TypeCSubState = TYPEC_UNATTACHED_WAIT_SRC_ENTRY_SS; 
                    }
                }
                else
                {
                    /* Do Nothing */
                }                
            }			
            /*Debug Accessory is Present*/
            else
            {    
                  /*Consider the CC Sample values as valid Debug accessory attach only at the 
                unattached state*/
                /*This avoids unwanted transition in attached state*/              
                if (TYPEC_UNATTACHED_SRC == u8TypeCState)
                {                  
                    //DEBUG_PRINT_PORT_STR(u8PortNum,"TYPEC: DEBUG ACCESSORY is present\r\n");
                    /*Setting the state for tCCDebounce	*/
                    u8TypeCState = TYPEC_ATTACHWAIT_SRC;
                    u8TypeCSubState = TYPEC_ATTACHWAIT_SRC_ENTRY_SS;
                }
            } 
            break;
		}		
        
		default:
		break;			
	}    
    gasTypeCcontrol[u8PortNum].u8TypeCState = u8TypeCState;
    gasTypeCcontrol[u8PortNum].u8TypeCSubState = u8TypeCSubState;         
}

#endif /*INCLUDE_PD_SOURCE*/

#if (TRUE == INCLUDE_PD_SINK)

void TypeC_SnkIntrHandler (UINT8 u8PortNum)
{
    /*Assigning the structure variables to local variables to reduce the code size as
    accessing the structure members takes lot of code*/   
    UINT8 u8TypeCState = gasTypeCcontrol[u8PortNum].u8TypeCState;
    UINT8 u8TypeCSubState = gasTypeCcontrol[u8PortNum].u8TypeCSubState;
    
	switch (gasTypeCcontrol[u8PortNum].u8CC1MatchISR ^ gasTypeCcontrol[u8PortNum].u8CC2MatchISR)	
	{
		case TYPEC_DFP_DEF_ATT:
		case TYPEC_DFP_1A5_ATT:
		case TYPEC_DFP_3A0_ATT:
        case (TYPEC_DFP_DEF_ATT ^ TYPEC_SNK_CCTHRES_VCONN_ON):
		case (TYPEC_DFP_1A5_ATT ^ TYPEC_SNK_CCTHRES_VCONN_ON):
		case (TYPEC_DFP_3A0_ATT ^ TYPEC_SNK_CCTHRES_VCONN_ON):            
		{    
            /*Decode and store the Rp value of Source attached*/ 
            TypeC_DecodeCCSourceRpValue (u8PortNum);
                       
            if (TYPEC_ATTACHWAIT_SNK == u8TypeCState)
            {                    
                /*Kill the TPD Debounce timer running for last detach event or
                Kill the TCC Debounce timer running for previous attach event*/
                TypeC_KillTypeCTimer(u8PortNum);                    
            }
            else if ((TYPEC_ATTACHED_SNK == u8TypeCState) && ((((UINT8)TYPEC_ATTACHED_SNK_RUN_SM_SS) == u8TypeCSubState) 
                        #if (TRUE == (INCLUDE_PD_PR_SWAP || INCLUDE_PD_FR_SWAP))
                        || (((UINT8)TYPEC_ATTACHED_SNK_SWAP_VBUS_PRES_DETECT_SS) == u8TypeCSubState)
                        #endif 
                        ))
            {                
                break;
            }
#if(TRUE == INCLUDE_PD_FR_SWAP)
            else if ((TYPEC_ATTACHED_SNK == u8TypeCState) && (TYPEC_ATTACHED_SNK_SWAP_ATTACH_DETECT_SS == u8TypeCSubState))
            {
                u8TypeCState = TYPEC_ATTACHED_SNK;
                u8TypeCSubState = TYPEC_ATTACHED_SNK_ENTRY_SS;
                break;
            }
#endif
            else
            {
                 /*Attach event has occurred within the TPD Debounce timeout of previous Detach event
                 So in this case moving to the Attach wait state and also killing the 
                 TPD Debounce timer which is active currently*/
                if ((TYPEC_ATTACHED_SNK == u8TypeCState) && (u8TypeCSubState != ((UINT8)TYPEC_ATTACHED_SNK_RUN_SM_SS)))
                {
                    /*Kill the TPD Debounce timer running previously for detach event*/
                    TypeC_KillTypeCTimer (u8PortNum);                 
                }
                 
                //DEBUG_PRINT_PORT_STR (u8PortNum,"TYPEC: Source is Present in CC");
                //DEBUG_PRINT_PORT_STR (((gasTypeCcontrol[u8PortNum].u8CC1MatchISR > gasTypeCcontrol[u8PortNum].u8CC2MatchISR) ? 1 : 2),"\r\n");                                                             
            }

            /*Disable the Protocol layer receiver function since moving to Attach wait state*/
            PRL_EnableRx (u8PortNum, FALSE);

            /*Setting the state as TYPEC_ATTACHWAIT_SNK for Tcc Debounce and VBUS presence 
            check*/
            u8TypeCState = TYPEC_ATTACHWAIT_SNK;
            u8TypeCSubState = TYPEC_ATTACHWAIT_SNK_ENTRY_SS;
            break;
		}

        case TYPEC_NO_THRESHOLD:
        {          
            /*Valid Detach event happens only when both the CC1 and CC2 are 0*/  
            if (SET_TO_ZERO == gasTypeCcontrol[u8PortNum].u8CC1MatchISR)
            {
                //DEBUG_PRINT_PORT_STR (u8PortNum,"TYPEC: NO DEVICES ARE PRESENT\r\n");                                

                if (TYPEC_ATTACHWAIT_SNK == u8TypeCState)
                {
                    /*Kill the TCC Debounce timer running previously attach event*/
                    TypeC_KillTypeCTimer (u8PortNum);
                    u8TypeCSubState = TYPEC_ATTACHWAIT_SNK_START_PD_DEB_SS;
                }
                /*Source detach occurs before the VBUS drops below VSinkdisconnect */
                else if ((TYPEC_ATTACHED_SNK == u8TypeCState) && \
                         (TYPEC_ATTACHED_SNK_RUN_SM_SS == u8TypeCSubState))
                {
                    /*During Sink to Source PR_Swap/FR_Swap, this condition would be hit when PS_RDY
                      is not received and PSSourceOf timer expires. At that point, both the partners 
                      might have Rd in CC wire. As a result of which, CC debounce thresholds 
                      will not match. This condition shall not be treated as a detach */
                    if (FALSE == DPM_IS_PR_OR_FR_SWAP_IN_PROGRESS(u8PortNum))
                    {
                        /* Go to TYPEC_ATTACHED_SNK_START_PD_DEB_SS sub-state for starting tPDDebounce */
                        u8TypeCSubState = TYPEC_ATTACHED_SNK_START_PD_DEB_SS;
                    }
                }
                /* This condition occurs if VCONN Discharge is enabled in 
                   Attached Sink State or Source detach occurs after VBUS drops 
                   below VSinkdisconnect. A PR_Swap/FR_Swap from Source to Sink should 
                   not be considered as a detach. So, don't do anything */ 
                else if ((TYPEC_ATTACHED_SNK == u8TypeCState) 
                #if (TRUE == INCLUDE_PD_PR_SWAP)
                    && (u8TypeCSubState != TYPEC_ATTACHED_SNK_SWAP_VBUS_PRES_DETECT_SS)
                #endif
                #if (TRUE == INCLUDE_PD_FR_SWAP)
                    && (u8TypeCSubState != TYPEC_ATTACHED_SNK_SWAP_ATTACH_DETECT_SS)
                    && (FALSE == DPM_IS_PR_OR_FR_SWAP_IN_PROGRESS(u8PortNum))    
                #endif 
                        )
                {
                    u8TypeCState = TYPEC_UNATTACHED_SNK;
                    u8TypeCSubState = TYPEC_UNATTACHED_SNK_ENTRY_SS;
                }
                else
                {
                    /* Do Nothing */
                }
            }             
            break;
        }   

        case TYPEC_SNK_CCTHRES_VCONN_ON:
        {
            /* This condition occurs during a detach when the power role of the 
               port is Sink and VCONN role is Source */
            if ((TYPEC_ATTACHED_SNK == u8TypeCState) && \
                         (TYPEC_ATTACHED_SNK_RUN_SM_SS == u8TypeCSubState))
            {			
                if (FALSE == DPM_IS_PR_OR_FR_SWAP_IN_PROGRESS(u8PortNum))
                {                    
                    /* Go to TYPEC_ATTACHED_SNK_START_PD_DEB_SS sub-state for starting tPDDebounce */
                    u8TypeCSubState = TYPEC_ATTACHED_SNK_START_PD_DEB_SS;
                }
            }
            else if ((TYPEC_UNATTACHED_SNK == u8TypeCState) || (TYPEC_ATTACHWAIT_SNK == u8TypeCState))
            {               
                /*This threshold will get hit when DFP Connected and advertising
                proprietary current. Setting the state as TYPEC_ATTACHWAIT_SNK 
                for Tcc Debounce and VBUS presence check*/
                
                TypeC_KillTypeCTimer (u8PortNum);
                
                /*Disable the Protocol layer receiver function since moving to Attach wait state*/
                PRL_EnableRx (u8PortNum, FALSE);
            
                u8TypeCState = TYPEC_ATTACHWAIT_SNK;
                u8TypeCSubState = TYPEC_ATTACHWAIT_SNK_ENTRY_SS;
            }
            else 
            {
                /*Execution is not expected to hit here.
                 Do nothing.*/
            }
            break; 
        } 
            		
		default:
		break;
	}
    
    gasTypeCcontrol[u8PortNum].u8TypeCState = u8TypeCState;
    gasTypeCcontrol[u8PortNum].u8TypeCSubState = u8TypeCSubState;
}
#endif
/*INCLUDE_PD_SINK*/

void TypeC_OnVCONNDISCHComplete (UINT8 u8PortNum)
{    
    //DEBUG_PRINT_PORT_STR (u8PortNum,"TYPEC: VCONN_DISCH COMPLETE\r\n");
    
    /*Power down the CC comparator*/
    TypeC_ConfigCCComp (u8PortNum, TYPEC_CC_COMP_CTL_DIS);
    
    /*Resetting the CC Threshold 0 register to default value*/
    UPD_RegWriteWord (u8PortNum, TYPEC_CC_THR0, TYPEC_CC_THR0_VAL);    
    
    #if (TRUE == INCLUDE_PD_SOURCE)
    if (PD_ROLE_SOURCE == DPM_GET_CURRENT_POWER_ROLE(u8PortNum))
    {
        /*Setting the user given Rp value for source since it is changed to open disconnect
        while VCONN Discharge*/
        TypeC_SetCCDefaultRpValue (u8PortNum);
    }
    #endif

    #if (TRUE == INCLUDE_PD_SINK)
    if (PD_ROLE_SINK == DPM_GET_CURRENT_POWER_ROLE(u8PortNum))
    {
        TypeC_SetCCDebounceVariable (u8PortNum, TYPEC_UFP);
          
        /*Reset only the Rd value for the CC pin in which VCONN was sourced*/
        TypeC_SetCCPowerRole (u8PortNum,TYPEC_ROLE_SINK, TYPEC_ROLE_SINK_RD, TYPEC_ENABLE_CC1_CC2);
        
        /*Resetting the CC Debounce clear enable,CC Match Enable,CC Sample Enable register*/
        TypeC_SetCCSampleEnable (u8PortNum, TYPEC_ENABLE_CC1_CC2);
    }
    #endif
  
    /*Set the CC Comparator to sample both CC1 and CC2*/
    TypeC_ConfigCCComp (u8PortNum, TYPEC_CC_COMP_CTL_CC1_CC2); 
}

/*******************************************************************************************/
/*********************************TypeC Collision avoidance Support APIs**************************************/
/*******************************************************************************************/
/*Change the Rp Value only for the CC pin in which source is connected*/
void TypeC_SetRpCollAvoidance (UINT8 u8PortNum, UINT8 u8RpValue)
{    
    UINT8 u8CCEnablePin, u8CCCompCtrl; 
    
    /* Setting the CC Comparator OFF */
    TypeC_ConfigCCComp (u8PortNum, TYPEC_CC_COMP_CTL_DIS);  
  
    /* Clearing the Rp value in u8PortSts */
    gasTypeCcontrol[u8PortNum].u8PortSts &= ~TYPEC_CURR_RPVAL_MASK;
    
    /*Rp value as 1.5A@5V */
    if (TYPEC_SINK_TXNG == u8RpValue)
    {
        gasTypeCcontrol[u8PortNum].u8CCDebMatch = (TYPEC_CC_THRES1_MATCH | TYPEC_CC_THRES5_MATCH);
        gasTypeCcontrol[u8PortNum].u8CCSrcSnkMatch = (TYPEC_CC_THRES5_MATCH);
        
        /*Setting the current Rp value as TYPEC_RP_CURRENT_15 in u8PortSts variable*/
        gasTypeCcontrol[u8PortNum].u8PortSts |= (TYPEC_RP_CURRENT_15 << TYPEC_CURR_RPVAL_POS);
        
        /*Setting the Rp Value as SinkTxNG*/
        TypeC_SetCCPowerRole (u8PortNum, TYPEC_ROLE_SOURCE, TYPEC_ROLE_SOURCE_15, TYPEC_ENABLE_CC1_CC2);    
        
        //DEBUG_PRINT_PORT_STR (u8PortNum,"TypeC-CA: Rp set to SinkTxNG\r\n");
    }
    /*Rp value as 3.0A@5V */
    else
    {
        gasTypeCcontrol[u8PortNum].u8CCDebMatch = (TYPEC_CC_THRES3_MATCH | TYPEC_CC_THRES6_MATCH);
        gasTypeCcontrol[u8PortNum].u8CCSrcSnkMatch = (TYPEC_CC_THRES6_MATCH);
        
        /*Setting the current Rp value as TYPEC_RP_CURRENT_30 in u8PortSts variable*/
        gasTypeCcontrol[u8PortNum].u8PortSts |= (TYPEC_RP_CURRENT_30 << TYPEC_CURR_RPVAL_POS);
        
        /*Setting the Rp Value as SinkTxOk*/
        TypeC_SetCCPowerRole (u8PortNum, TYPEC_ROLE_SOURCE, TYPEC_ROLE_SOURCE_30, TYPEC_ENABLE_CC1_CC2);
        
        //DEBUG_PRINT_PORT_STR (u8PortNum,"TypeC-CA: Rp set to SinkTxOK\r\n");
    }
    
    /*Enabling the CC Sampling in the sink attached CC pin if both sink and powered cable 
    are attached*/   
    /*Sink Attached in CC1 since VCONN is enabled in CC2*/
    if (TYPEC_VCONN_SOURCE_CC2 == (gasTypeCcontrol[u8PortNum].u8IntStsISR & TYPEC_VCONN_SOURCE_MASK))
    {                     
        u8CCEnablePin = TYPEC_ENABLE_CC1; 
        u8CCCompCtrl = TYPEC_CC_COMP_CTL_CC1; 
    }
    /*Sink Attached in CC2 since VCONN is enabled in CC1*/
    else if (TYPEC_VCONN_SOURCE_CC1 == (gasTypeCcontrol[u8PortNum].u8IntStsISR & TYPEC_VCONN_SOURCE_MASK))
    {                        
        u8CCEnablePin = TYPEC_ENABLE_CC2; 
        u8CCCompCtrl = TYPEC_CC_COMP_CTL_CC2;         
    }
    /*Enabling the CC Sampling in both the CC pins if only sink is attached*/
    else
    {                        
        u8CCEnablePin = TYPEC_ENABLE_CC1_CC2;
        u8CCCompCtrl = TYPEC_CC_COMP_CTL_CC1_CC2;         
    }

    /* Changing the CC Sampling value as per the Rp value set */
    TypeC_SetCCSampleEnable (u8PortNum, u8CCEnablePin);
    
    /* Setting the CC Comparator ON */
    TypeC_ConfigCCComp (u8PortNum, u8CCCompCtrl);    
}

/*******************************************************************************************/
/*********************************TypeC VBUS Comparator Support Functions**************************************/
/*******************************************************************************************/
void TypeC_SetVBUSCompONOFF (UINT8 u8PortNum, UINT8 u8ConfigVal)
{
    UINT8 u8Data;
    UINT8 u8DesiredDBState;
      
    /*Set VBUS Comparator OFF*/
    if (TYPEC_VBUSCOMP_OFF == u8ConfigVal)
    {
        /*Disabling the VBUS Debouncer */
        UPD_RegByteClearBit (u8PortNum, TYPEC_VBUS_CTL1_LOW, TYPEC_VBUS_DET);

        /* Wait until the VBUS Debouncer goes to inactive state  */
        u8DesiredDBState = TYPEC_VBUS_DB_ACTIVE;           
    }
    /*Set VBUS Comparator ON*/
    else
    {
        /* Setting the VBUS Comparator Control ON*/
        UPD_RegByteSetBit (u8PortNum, TYPEC_VBUS_CTL1_LOW, TYPEC_VBUS_DET);
        
        /* Wait until VBUS Comparator goes to active state */
        u8DesiredDBState = FALSE; 
    }
    
    do
    {
        UPD_RegisterRead (u8PortNum, TYPEC_VBUS_CTL1_LOW, &u8Data, BYTE_LEN_1);
        
    }while((u8Data & TYPEC_VBUS_DB_ACTIVE)  == u8DesiredDBState);
}
/**************************************************************/
void TypeC_ConfigureVBUSThr (UINT8 u8PortNum, UINT16 u16Voltage, UINT16 u16Current, UINT8 u8PowerFaultThrConfig)
{         
  	UINT16 u16PrevVoltInmV = SET_TO_ZERO;
    /* Used to check if the port is Source port */
    UINT8 u8IsSourcePort = ((PD_ROLE_SOURCE == DPM_GET_CURRENT_POWER_ROLE(u8PortNum)) ? TRUE : FALSE);
    /* Used to check if the current contract is a PPS contract */
    UINT8 u8IsPPSContract = (((gasPolicyEngine[u8PortNum].u8PEPortSts & PE_EXPLICIT_CONTRACT) &&
            (DPM_PD_PPS_CONTRACT == DPM_GET_CURRENT_EXPLICIT_CONTRACT(u8PortNum))) ? TRUE : FALSE);    
    UINT8 u8SampleEn = SET_TO_ZERO;
    float fVBUSCorrFactor = gasTypeCcontrol[u8PortNum].fVBUSCorrectionFactor; 

    /* Get the Previous VBUS voltage based on the type of contract established */
    if (gasPolicyEngine[u8PortNum].u8PEPortSts & PE_EXPLICIT_CONTRACT)
    {        
        u16PrevVoltInmV = gasDPM[u8PortNum].u16PrevVBUSVoltageInmV; 
    }
    else
    {
        u16PrevVoltInmV = DPM_GetVBUSVoltage (u8PortNum);
    }
               
    /* Set the voltage value that is expected in VBUS once power is driven.
       TypeC_HandleISR() would need this value to check if the same voltage 
       for which threshold was configured is driven in VBUS.
       For PPS, this variable needs to be 0 since a PPS Source shall not 
       rely on VBUS voltage for sending PS_RDY */
    if (u8IsPPSContract)
    {
        gasDPM[u8PortNum].u16ExpectedVBUSVoltageInmV = SET_TO_ZERO; 
    }
    else
    {
        gasDPM[u8PortNum].u16ExpectedVBUSVoltageInmV = u16Voltage; 
    }
    
    /* Clear the VBUS Presence mask bits except for vSafe0V before 
       configuring the VBUS threshold */    
    if (u16Voltage)
    {
        MCHP_PSF_HOOK_DISABLE_GLOBAL_INTERRUPT();
        gasTypeCcontrol[u8PortNum].u8IntStsISR &= ~TYPEC_VBUS_PRESENCE_MASK; 
        MCHP_PSF_HOOK_ENABLE_GLOBAL_INTERRUPT(); 
    }    
    
	/*Setting the VBUS Comparator OFF*/
	TypeC_SetVBUSCompONOFF (u8PortNum, TYPEC_VBUSCOMP_OFF);
	
	/* **************************Power Fault threshold configuration**************************/
	#if (TRUE == INCLUDE_POWER_FAULT_HANDLING)
    UINT16 u16aVBUSThrVal[BYTE_LEN_2] = {SET_TO_ZERO}; 
    
	if ((u8PowerFaultThrConfig) && (u16Voltage != TYPEC_VBUS_0V))
	{
        //DEBUG_PRINT_PORT_STR (u8PortNum,"TYPEC: Set PWR Fault VBUS Threshold\r\n");
        
        /* if fault has occurred start the Power Good Timer */
        if (gasDPM[u8PortNum].u8VBUSPowerFaultCount > SET_TO_ZERO)
        {
            /*  Power Fault threshold are configured only when the desired
                PDO voltage is reached. Power Good timer should be started only when 
                PDO voltage is attained. Hence started here.*/
            PDTimer_Kill (gasDPM[u8PortNum].u8VBUSPowerGoodTmrID);
            gasDPM[u8PortNum].u8VBUSPowerGoodTmrID = PDTimer_Start (gasCfgStatusData.sPerPortData[u8PortNum].u16PowerGoodTimerInms,\
                                                          DPM_VBUSPowerGood_TimerCB, u8PortNum, (UINT8)SET_TO_ZERO);
        }
        
        /* Over voltage threshold is set in TypeC_ConfigureVBUSThr */
        if (u8IsPPSContract)
        {
            u16aVBUSThrVal[INDEX_0] = (UINT16)((float)TYPEC_GET_OVER_VOLTAGE_VBUS_THR(DPM_GET_APDO_MAX_VOLTAGE_IN_mV(gasDPM[u8PortNum].u32NegotiatedPDO)) * fVBUSCorrFactor);
            u16aVBUSThrVal[INDEX_1] = (UINT16)((float)TYPEC_GET_UNDER_VOLTAGE_VBUS_THR(DPM_GET_APDO_MIN_VOLTAGE_IN_mV(gasDPM[u8PortNum].u32NegotiatedPDO)) * fVBUSCorrFactor);
        }
        else
        {
            u16aVBUSThrVal[INDEX_0] = (UINT16)((float)TYPEC_GET_OVER_VOLTAGE_VBUS_THR(u16Voltage) * fVBUSCorrFactor); 
            u16aVBUSThrVal[INDEX_1] = (UINT16)((float)TYPEC_GET_UNDER_VOLTAGE_VBUS_THR(u16Voltage) * fVBUSCorrFactor);
        }
        	
        /*Setting the default threshold values in VBUS threshold X(0-4) registers*/ 
        UPD_RegisterWrite (u8PortNum, TYPEC_VBUS_THR2, (UINT8 *)u16aVBUSThrVal, sizeof(u16aVBUSThrVal));  

        /*Enable the VBUS sample */
        u8SampleEn = (TYPEC_VSAFE0V_MAX_THR_MATCH | TYPEC_VSINKDISCONNECT_THR0_MATCH | \
                            TYPEC_DESIRED_MIN_V_THR1_MATCH | TYPEC_OVER_VOLT_THR2_MATCH| \
                              TYPEC_UNDER_VOLT_THR3_MATCH);

        #if (TRUE == INCLUDE_UPD_PIO_OVERRIDE_SUPPORT)
            /* Enable PIO override for over voltage and under voltage*/
            UPD_RegByteSetBit (u8PortNum, UPD_PIO_OVR_EN, (UPD_PIO_OVR_0 | UPD_PIO_OVR_1));
        #endif
	}
	else
	#endif /*INCLUDE_POWER_FAULT_HANDLING*/
	/*****************************VBUS threshold configuration for all voltages ****************/  
	{
        //DEBUG_PRINT_PORT_STR (u8PortNum,"TYPEC: Set Non PWR Fault VBUS Threshold\r\n");
        
        #if (TRUE == INCLUDE_POWER_FAULT_HANDLING)
        #if (TRUE == INCLUDE_UPD_PIO_OVERRIDE_SUPPORT)
			/* PIO override for under-voltage and overvoltage is cleared*/
            UPD_RegByteClearBit (u8PortNum, UPD_PIO_OVR_EN, (UPD_PIO_OVR_0 | UPD_PIO_OVR_1));
        #endif /*endif of INCLUDE_UPD_PIO_OVERRIDE_SUPPORT*/
        #endif /*endif of INCLUDE_POWER_FAULT_HANDLING*/
            
      UINT16 u16MinVoltageThr = SET_TO_ZERO;
      UINT16 u16MaxVoltageThr = SET_TO_ZERO;
      
	  switch (u16Voltage)
	  {
		case TYPEC_VBUS_0V:
		{
            /* Corresponding VSafe0V VBUS sample enable */
            u8SampleEn = TYPEC_VSAFE0V_MAX_THR_MATCH;
			
            if (u8IsSourcePort)
            {
                /* Enable VBUS discharge only if the port role is source*/
                PWRCTRL_ConfigVBUSDischarge (u8PortNum, TRUE);            
            }         
				
			break; 
		} /* end of TYPEC_VBUS_0V*/
		
		case TYPEC_VBUS_5V:
		{
            /* Minimum valid Vsafe5V threshold is configured in u16MinVoltageThr*/
            /* Maximum Vsafe5V threshold is configured in u16MaxVoltageThr*/
            if (u8IsSourcePort)
            {
                u16MinVoltageThr = ROUND_OFF_FLOAT_TO_INT(((float)TYPEC_DESIRED_MIN_SRC_VSAFE5V_VBUS_THR * fVBUSCorrFactor));
                u16MaxVoltageThr = ROUND_OFF_FLOAT_TO_INT(((float)TYPEC_DESIRED_MAX_SRC_VSAFE5V_VBUS_THR * fVBUSCorrFactor)); 
            }
            else
            {
                u16MinVoltageThr = ROUND_OFF_FLOAT_TO_INT(((float)TYPEC_DESIRED_MIN_SNK_VSAFE5V_VBUS_THR * fVBUSCorrFactor));
                u16MaxVoltageThr = ROUND_OFF_FLOAT_TO_INT(((float)TYPEC_DESIRED_MAX_SNK_VSAFE5V_VBUS_THR * fVBUSCorrFactor)); 
            }
            break;
				
		}/* end of TYPEC_VBUS_5V*/
		
	  	default:
		{
            if (u8IsPPSContract)
            {
                /* Minimum valid PDO voltage is configured in u16MinVoltageThr*/
                u16MinVoltageThr = ROUND_OFF_FLOAT_TO_INT(((float)TYPEC_GET_DESIRED_MIN_VBUS_THR(DPM_GET_APDO_MIN_VOLTAGE_IN_mV(gasDPM[u8PortNum].u32NegotiatedPDO)) * fVBUSCorrFactor)); 
                
                /* Maximum PDO voltage threshold is configured in u16MaxVoltageThr*/
                u16MaxVoltageThr = ROUND_OFF_FLOAT_TO_INT(((float)TYPEC_GET_DESIRED_MIN_VBUS_THR(DPM_GET_APDO_MAX_VOLTAGE_IN_mV(gasDPM[u8PortNum].u32NegotiatedPDO)) * fVBUSCorrFactor)); 
            }   
            else
            {
                /* Minimum valid PDO voltage is configured in u16MinVoltageThr*/
                u16MinVoltageThr = ROUND_OFF_FLOAT_TO_INT(((float)TYPEC_GET_DESIRED_MIN_VBUS_THR(u16Voltage) * fVBUSCorrFactor)); 

                /* Maximum PDO voltage threshold is configured in u16MaxVoltageThr*/
                u16MaxVoltageThr = ROUND_OFF_FLOAT_TO_INT(((float)TYPEC_GET_DESIRED_MAX_VBUS_THR(u16Voltage) * fVBUSCorrFactor));
            }
            break;
		}/* end of default*/
        
	  }/* end of switch*/
      
      if (u16Voltage != TYPEC_VBUS_0V)
      {
        //DEBUG_PRINT_PORT_STR (u16MinVoltageThr,"Min VBUS Threshold\r\n");
        
        /* Minimum valid voltage is configured to VBUS Threshold 1*/
        UPD_RegWriteWord (u8PortNum, TYPEC_VBUS_THR1, u16MinVoltageThr);

        if (u16PrevVoltInmV > u16Voltage)
        {
            if (u8IsSourcePort)
            {
               /* Enable VBUS discharge only if the port role is source*/
                PWRCTRL_ConfigVBUSDischarge (u8PortNum, TRUE);            
            }
            
            //DEBUG_PRINT_PORT_STR (u16MaxVoltageThr,"Max VBUS Threshold\r\n"); 
            
            /* Set Threshold 1 to previous voltage*/
            UPD_RegWriteWord (u8PortNum, TYPEC_VBUS_THR4, u16MaxVoltageThr);
            
            /* Corresponding VBUS sample enable */
            u8SampleEn = (TYPEC_VSAFE0V_MAX_THR_MATCH | TYPEC_VSINKDISCONNECT_THR0_MATCH |
                            TYPEC_DESIRED_MIN_V_THR1_MATCH | TYPEC_PREV_V_DROP_CHK_THR4_MATCH);
        }
        else
        {
           /* Corresponding VBUS sample enable */
           u8SampleEn = (TYPEC_VSAFE0V_MAX_THR_MATCH | TYPEC_VSINKDISCONNECT_THR0_MATCH |
                            TYPEC_DESIRED_MIN_V_THR1_MATCH);
        }
      }                      
	} /* end of u8PowerFaultThrConfig else*/
				
	/*Enabling VBUS sample*/
	UPD_RegWriteByte (u8PortNum, TYPEC_VBUS_SAMP_EN, u8SampleEn);
	
	/*Setting the VBUS Comparator ON*/
	TypeC_SetVBUSCompONOFF (u8PortNum, TYPEC_VBUSCOMP_ON);	   
}
/************************************************************************/								   
/*******************************************************************************************/
/*********************************TypeC Timer CB**************************************/
/*******************************************************************************************/
void TypeC_SubStateChange_TimerCB (UINT8 u8PortNum, UINT8 u8TypeCSubState)
{	
	gasTypeCcontrol[u8PortNum].u8TypeCSubState = u8TypeCSubState;
    gasTypeCcontrol[u8PortNum].u8TypeCTimerID = MAX_CONCURRENT_TIMERS;
}

void TypeC_StateChange_TimerCB (UINT8 u8PortNum, UINT8 u8TypeCState)
{
    gasTypeCcontrol[u8PortNum].u8TypeCState = u8TypeCState;
    gasTypeCcontrol[u8PortNum].u8TypeCSubState = SET_TO_ZERO;
    gasTypeCcontrol[u8PortNum].u8TypeCTimerID = MAX_CONCURRENT_TIMERS;  
}

void TypeC_KillTypeCTimer (UINT8 u8PortNum)
{
    PDTimer_Kill (gasTypeCcontrol[u8PortNum].u8TypeCTimerID);
    
    /*Setting the u8PETimerID to MAX_CONCURRENT_TIMERS to indicate that
    TimerID does not hold any valid timer IDs anymore*/
    gasTypeCcontrol[u8PortNum].u8TypeCTimerID = MAX_CONCURRENT_TIMERS;    
}

/*******************************************************************************************/
/*********************************TypeC Common Support APIs**************************************/
/*******************************************************************************************/
UINT16 TypeC_ObtainCurrentValueFrmRp (UINT8 u8PortNum)
{
    UINT16 u16ReturnVal;
    UINT8 u8RpVal = ((gasTypeCcontrol[u8PortNum].u8PortSts & TYPEC_CURR_RPVAL_MASK) >> TYPEC_CURR_RPVAL_POS);

    switch (u8RpVal)
    {
        case TYPEC_DFP_DEFAULT_CURRENT:
        {
            u16ReturnVal = DPM_900mA;
            break;  
        }
        case TYPEC_DFP_1A5_CURRENT:
        {
            u16ReturnVal = DPM_1500mA;
            break;  
        }
        case TYPEC_DFP_3A0_CURRENT:
        {                           
            u16ReturnVal = DPM_3000mA;
            break;  
        }
        default:
        {
            u16ReturnVal = DPM_0mA;
            break;
        }
    }       
  
#if (TRUE == INCLUDE_PD_SINK)    
    /* Set the Sink Rp val status in u32PortConnectStatus */
    if (PD_ROLE_SINK == DPM_GET_CURRENT_POWER_ROLE(u8PortNum))
    {       
        gasCfgStatusData.sPerPortData[u8PortNum].u32PortConnectStatus &= \
                            ~DPM_PORT_RP_VAL_DETECT_MASK_STATUS; 
        gasCfgStatusData.sPerPortData[u8PortNum].u32PortConnectStatus |= \
                        (u8RpVal << DPM_PORT_RP_VAL_DETECT_STATUS_POS);    
    }
#endif 
    
    return u16ReturnVal;
}

/*******************************************************************************************/
/**************************TypeC FRS Support Functions**************************************/
/*******************************************************************************************/
#if (TRUE == INCLUDE_PD_FR_SWAP)

void TypeC_ConfigureFRSSignalDET (UINT8 u8PortNum)
{      
    /* Ensure at minimum that pwr_sw_clk_gate_en, cable_plug_clk_gate_en, pd_mac_clk_gate_en, 
       pio_clk_gate_en, and i2c_clk_gate_en (or spi_clk_gate_en) are cleared in Clock Gate Register */    
    UPD_RegByteClearBit (u8PortNum, UPD_CLK_GATE_LOW, (UINT8)(UPD_PWR_SW_CLK_GATE_EN | \
            UPD_I2C_CLK_GATE_EN | UPD_SPI_CLK_GATE_EN | UPD_PIO_CLK_GATE_EN));                 
    UPD_RegByteClearBit (u8PortNum, UPD_CLK_GATE_HIGH, (UINT8)(UPD_PD_MAC_CLK_GATE_EN | \
            UPD_CABLE_PLUG_CLK_GATE_EN));
                
    /* Program the CC Comparator Control to 00b and disable the CC debouncer */
    TypeC_ConfigCCComp (u8PortNum, TYPEC_CC_COMP_CTL_DIS);    
    
    /* Disable the VBUS debouncer */
    TypeC_SetVBUSCompONOFF (u8PortNum, TYPEC_VBUSCOMP_OFF);
    
    /* Program FRS CC Debounce Register with FRS CC debounce time of 30us */
    UPD_RegWriteByte (u8PortNum, TYPEC_FRS_CC_DEB, TYPEC_FRS_CC_DEB_30US);
    
    /* Program FRS Threshold Select Register with its default value to point to the FRSWAP threshold */
    UPD_RegWriteByte (u8PortNum, TYPEC_FRS_THR_SEL, TYPEC_FRS_THRESHOLD);
    
    /* Enable the UPD350 high level Extended interrupt*/ 
    UINT16 u16Data = UPD_RegReadWord (u8PortNum, UPDINTR_INT_EN);
    u16Data |= UPDINTR_EXT_INT;
	UPD_RegWriteWord (u8PortNum, UPDINTR_INT_EN, u16Data); 
    
    /* Enable FRS_RCV_STS interrupt in Extended Interrupt Enable Register */
    UPD_RegByteSetBit (u8PortNum, TYPEC_EXT_INT_EN, (UINT8)TYPEC_FRS_RCV_STS);	
    
    /* Program FRS_VBUS_DEB register with FRS VBUS debounce time of 30us */
    UPD_RegWriteByte (u8PortNum, TYPEC_FRS_VBUS_DEB, TYPEC_FRS_VBUS_DEB_30US);
    
    /* Set CC High Bandwidth Mode Enable in CC Hardware Control Register */
    UPD_RegByteSetBit (u8PortNum, TYPEC_CC_HW_CTL_LOW, (UINT8)TYPEC_CC_DET_HBW_EN);	
    
    /* Program CC Sample Clock Register to enable >= 250KHz sampling rate */
	UPD_RegWriteByte (u8PortNum, UPD_CC_SAMP_CLK, \
                    (UINT8)(UPD_CC_SAMP_GEN_250_KS | UPD_CC_CLK_48_KHZ));    
    
    /* Set VBUS High Bandwidth Mode Enable in VBUS Control 1 Register */
    UPD_RegByteSetBit (u8PortNum, TYPEC_VBUS_CTL1_HIGH, (UINT8)TYPEC_VBUS_DET_HBW_EN);    
    
    /* Program VBUS Sample Clock Register to enable >= 250KHz sampling rate */    
	UPD_RegWriteByte (u8PortNum, UPD_VBUS_SAMP_CLK, \
                    (UINT8)(UPD_VBUS_SAMP_GEN_250_KS | UPD_VBUS_CLK_48_KHZ));    
                
    /* Enable the VBUS debouncer */
    TypeC_SetVBUSCompONOFF (u8PortNum, TYPEC_VBUSCOMP_ON);
    
    /* Program the CC Comparator Control in CC Control 1 Register to enable detection on the
       connected CC pin.*/     
    TypeC_ConfigCCComp (u8PortNum, TYPEC_CC_COMP_CTL_CC1_CC2);         
    
    //DEBUG_PRINT_PORT_STR (u8PortNum,"TYPEC:FRS Signal Detection Enabled\r\n");             
}

void TypeC_ConfigureFRSSignalXMT (UINT8 u8PortNum)
{
    /* Get the polarity of EN_FRS PIO */
    UINT8 u8EN_FRSPol = gasCfgStatusData.sPerPortData[u8PortNum].u8Mode_EN_FRS;

    /* Ensure at minimum that cable_plug_clk_gate_en, pd_mac_clk_gate_en, 
       pio_clk_gate_en, and i2c_clk_gate_en (or spi_clk_gate_en) are cleared in Clock Gate Register */    
    UPD_RegByteClearBit (u8PortNum, UPD_CLK_GATE_LOW, \
            (UINT8)(UPD_I2C_CLK_GATE_EN | UPD_SPI_CLK_GATE_EN | UPD_PIO_CLK_GATE_EN));                 
    UPD_RegByteClearBit (u8PortNum, UPD_CLK_GATE_HIGH, \
            (UINT8)(UPD_PD_MAC_CLK_GATE_EN | UPD_CABLE_PLUG_CLK_GATE_EN)); 
    
    /* Program FRS Transmission Length register with a value of 90us */
    UPD_RegWriteByte (u8PortNum, TYPEC_FRS_TX_LEN, TYPEC_FRS_TX_LEN_90US);
    
    /* Program the CC pin which has sink attached in FRS_CC_SEL in FRS Control Register */
    /* 5:4  Description 
       00b: CC1 pin
       01b: CC2 pin
       1xb: CC1 and CC2 pin */
    /* In case of CC1 attach, clear both the bits */
    UPD_RegByteClearBit (u8PortNum, TYPEC_FRS_CTL_LOW, (UINT8)TYPEC_FRS_CC_SEL);

    if (TYPEC_ORIENTATION_CC2 == TYPEC_GET_CC_ORIENTATION_STS(u8PortNum))
    {
        /* In case of CC2 attach, set Bit 4 */
        UPD_RegByteSetBit (u8PortNum, TYPEC_FRS_CTL_LOW, (UINT8)TYPEC_FRS_CC_SEL_CC2);    
    }
        
    /* Enable the UPD350 high level Extended interrupt*/ 
    UINT16 u16Data = UPD_RegReadWord (u8PortNum, UPDINTR_INT_EN);
    u16Data |= UPDINTR_EXT_INT;
	UPD_RegWriteWord (u8PortNum, UPDINTR_INT_EN, u16Data);
    
    /* Enable FRS_XMT_STS interrupt in Extended Interrupt Enable Register */
    UPD_RegByteSetBit (u8PortNum, TYPEC_EXT_INT_EN, (UINT8)TYPEC_FRS_XMT_STS);	
    
    /* PIO which is used to transmit FRS signal is indicated by programming FRS Request PIO Select Register*/
    UPD_RegWriteByte (u8PortNum, TYPEC_FRS_PIO_SEL, \
            (gasCfgStatusData.sPerPortData[u8PortNum].u8Pio_EN_FRS << TYPEC_FRS_REQ_PIO_SEL_POS));
        
    /* Set PIO value that initiates transmission of FRS signaling
       based on the polarity of u8Pio_EN_FRS */
    if (((UINT8)eINPUT_ACTIVE_LOW == u8EN_FRSPol) || ((UINT8)eINPUT_ACTIVE_LOW_PU == u8EN_FRSPol))
    {
        UPD_RegByteClearBit (u8PortNum, TYPEC_FRS_CTL_HIGH, (UINT8)TYPEC_FRS_REQ_VAL);
    }
    else if (((UINT8)eINPUT_ACTIVE_HIGH == u8EN_FRSPol) || ((UINT8)eINPUT_ACTIVE_HIGH_PD == u8EN_FRSPol))
    {
        UPD_RegByteSetBit (u8PortNum, TYPEC_FRS_CTL_HIGH, (UINT8)TYPEC_FRS_REQ_VAL);
    }
    else 
    {
        /* Do Nothing */
    }
        
    //DEBUG_PRINT_PORT_STR (u8PortNum,"TYPEC:FRS Signal Transmission Enabled\r\n");             
}

void TypeC_EnableFRSXMTOrDET (UINT8 u8PortNum, UINT8 u8IsFRSSupported)
{        
    UINT8 u8IsSourcePort = ((PD_ROLE_SOURCE == DPM_GET_CURRENT_POWER_ROLE(u8PortNum)) ? TRUE : FALSE);
            
    if (u8IsFRSSupported)
    {              
 		/* Enable non-Power fault thresholds for TYPEC_VBUS_5V */
        /* Note: Here, TypeC_ConfigureVBUSThr() API is not used for setting the 
           vSafe5V thresholds. This is done intentionally because calling that API
           with Non Pwr Fault Threshold argument will disable the Power fault detection
           and hence, we may fail to detect an actual OV/UV fault that
           could occur while waiting for an FRS event */        
        /* Update the u16ExpectedVBUSVoltageInmV so that gasTypeCcontrol[u8PortNum].u8IntStsISR
           will be updated with correct VBUS Presence Mask value */
        gasDPM[u8PortNum].u16ExpectedVBUSVoltageInmV = TYPEC_VBUS_5V;   
        
        /* Clear the VBUS Presence mask bits before configuring the VBUS threshold */    
        MCHP_PSF_HOOK_DISABLE_GLOBAL_INTERRUPT();
        gasTypeCcontrol[u8PortNum].u8IntStsISR &= ~TYPEC_VBUS_PRESENCE_MASK; 
        MCHP_PSF_HOOK_ENABLE_GLOBAL_INTERRUPT(); 
                           
        /* Turn off the VBUS Comparator */
        TypeC_SetVBUSCompONOFF (u8PortNum, TYPEC_VBUSCOMP_OFF);        
        
        /* Disable Under Voltage for Source role only. Power loss is expected
           during an FRS. It shall not be treated as an under-voltage */
        if (u8IsSourcePort)
        {
            UPD_RegByteClearBit (u8PortNum, UPD_PIO_OVR_EN, UPD_PIO_OVR_1);
        }
        
        /* Configure vSafe5v(min) threshold */        
        UPD_RegWriteWord (u8PortNum, TYPEC_VBUS_THR1, \
                ROUND_OFF_FLOAT_TO_INT(((float)TYPEC_DESIRED_MIN_SRC_VSAFE5V_VBUS_THR * gasTypeCcontrol[u8PortNum].fVBUSCorrectionFactor)));
        
        /* Configure vSafe5v(max) threshold */        
        UPD_RegWriteWord (u8PortNum, TYPEC_VBUS_THR4, \
                ROUND_OFF_FLOAT_TO_INT(((float)TYPEC_DESIRED_MAX_SRC_VSAFE5V_VBUS_THR * gasTypeCcontrol[u8PortNum].fVBUSCorrectionFactor))); 
        
        /* Configure the corresponding VBUS sample enable */
        UINT8 u8SampleEn = UPD_RegReadByte (u8PortNum, TYPEC_VBUS_SAMP_EN);
        if (u8IsSourcePort)
        {
            u8SampleEn &= ~(TYPEC_UNDER_VOLT_THR3_MATCH);
        }
        u8SampleEn |= (TYPEC_DESIRED_MIN_V_THR1_MATCH | TYPEC_PREV_V_DROP_CHK_THR4_MATCH);                        
        UPD_RegWriteByte (u8PortNum, TYPEC_VBUS_SAMP_EN, u8SampleEn);        
        
        /* Turn on the VBUS Comparator */
        TypeC_SetVBUSCompONOFF (u8PortNum, TYPEC_VBUSCOMP_ON);
            
        if (u8IsSourcePort)
        {
            /* Configure UPD350 for FRS Signal Transmission */
            TypeC_ConfigureFRSSignalXMT (u8PortNum);
                                       
            /* Enable PIO Override */
            UPD_RegByteSetBit (u8PortNum, UPD_PIO_OVR_EN, (UINT8)UPD_PIO_OVR_3);
                                
            /* Enable FRS_REQ_PIO bit in FRS_CTL register */
            UPD_RegByteSetBit (u8PortNum, TYPEC_FRS_CTL_HIGH, (UINT8)TYPEC_FRS_REQ_PIO);

            //DEBUG_PRINT_PORT_STR(u8PortNum, "FRS XMT Enabled\r\n");                            
        }        
        else /* PD_ROLE_SINK */
        {         
            /* Configure UPD350 for FRS Signal Detection */
            TypeC_ConfigureFRSSignalDET (u8PortNum);  
            
            /* Configure PIO Override Source 
               When initial VBUS (at new source) < vSafe5V (min), 
               New Source may turn on after detecting Fast Role Swap signal
               When initial VBUS (at new source) > vSafe5V (min), 
               New Source may turn on at any time after VBUS falls below vSafe5V (max) */            
            if (gasCfgStatusData.sPerPortData[u8PortNum].u16NegoVoltageInmV > TYPEC_VBUS_5V)
            {
                UPD_RegWriteByte (u8PortNum, UPD_PIO_OVR3_SRC_SEL, (UPD_PIO_OVR_SRC_SEL_VBUS_THR_AND_FRS_DET | UPD_PIO_OVR_VBUS4_THR_MATCH)); 
            }
            else
            {
                UPD_RegWriteByte (u8PortNum, UPD_PIO_OVR3_SRC_SEL, UPD_PIO_OVR_SRC_SEL_FRS_SIG_DETECTED); 
            }                                                       
                                   
            /* Enable PIO Override */
            UPD_RegByteSetBit (u8PortNum, UPD_PIO_OVR_EN, (UINT8)UPD_PIO_OVR_3);
            
            /* Enable DC/DC pin function */
            PWRCTRL_ConfigDCDCEn (u8PortNum, TRUE);  
            
            /* Enable FRS Signal Detection */
            UPD_RegByteSetBit (u8PortNum, TYPEC_FRS_CTL_HIGH, (UINT8)TYPEC_FRS_DET_EN);    
            
            //DEBUG_PRINT_PORT_STR(u8PortNum, "FRS DET Enabled\r\n"); 
        }
        /* Set FRS XMT or DET Enabled status */
        DPM_ENABLE_FRS_XMT_OT_DET(u8PortNum);
    }
    else  /* FRS Criteria Not Supported for the port */
    {               
        /* Disable FRS signal transmission and detection */
        if (u8IsSourcePort)
        {
            UPD_RegByteClearBit (u8PortNum, TYPEC_FRS_CTL_HIGH, (UINT8)TYPEC_FRS_REQ_PIO);
            
            //DEBUG_PRINT_PORT_STR(u8PortNum, "FRS XMT Disabled\r\n");
        }
        else /* PD_ROLE_SINK */
        {            
            UPD_RegByteClearBit (u8PortNum, TYPEC_FRS_CTL_HIGH, (UINT8)TYPEC_FRS_DET_EN);                        
            
            /* Disable DC/DC pin function */
            PWRCTRL_ConfigDCDCEn (u8PortNum, FALSE);  
            
            //DEBUG_PRINT_PORT_STR(u8PortNum, "FRS DET Disabled\r\n");
        }      
        
        TypeC_ConfigCCComp (u8PortNum, TYPEC_CC_COMP_CTL_DIS);

        /*Set the CC_SAMP_CLK bit as 0. Sampling period will be 100us*/
        UPD_RegWriteByte (u8PortNum, UPD_CC_SAMP_CLK, \
                    (UINT8) (UPD_CC_SAMP_GEN_10_KS | UPD_CC_CLK_20_KHZ));            

        TypeC_ConfigCCComp (u8PortNum, TYPEC_CC_COMP_CTL_CC1_CC2);    
        
        /* Disable PIO Override since FRS XMT/DET is disabled */
        UPD_RegByteClearBit (u8PortNum, UPD_PIO_OVR_EN, (UINT8)UPD_PIO_OVR_3);
        
        /* Clear FRS XMT or DET Enabled status */
        DPM_DISABLE_FRS_XMT_OR_DET(u8PortNum);        
    }       
}

#endif /* INCLUDE_PD_FR_SWAP */ 
