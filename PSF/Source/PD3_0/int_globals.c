/*******************************************************************************
  Internal globals Source file
 
  Company:
    Microchip Technology Inc.
 
  File Name:
    int_globals.c

  Description:
    File contains definitions of globals used internally within the PSF stack
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

/*******************************************************************/
/******************* Global variables********************************/
/*******************************************************************/

/*Timer module globals*/
PDTIMER gasPDTimers[MAX_CONCURRENT_TIMERS];

/*Type C module globals*/
TYPEC_CONTROL  gasTypeCcontrol[CONFIG_PD_PORT_COUNT];

/****************************Protocol Layer globals************************************/
/* PRL State variables */
PROTOCOL_LAYER_STRUCT gasPRL [CONFIG_PD_PORT_COUNT];

 /* PRL receiver Struct*/
PRL_RECVBUFF gasPRLRecvBuff [CONFIG_PD_PORT_COUNT];

#if (TRUE == INCLUDE_PD_3_0)
    /* PRL extended message buffer for Tx & Rx*/
    PRL_EXTMSG_BUFF gasExtendedMsgBuff [CONFIG_PD_PORT_COUNT];

    /* PRL chunk state variable struct*/
    PRL_CHUNK_STRUCT gasChunkSM [CONFIG_PD_PORT_COUNT];

#endif 
/***************************************************************************************/
    
/*Policy Engine globals*/
PolicyEngine_Status gasPolicy_Engine[CONFIG_PD_PORT_COUNT];

/*Policy manager globals*/
DEVICE_POLICY_MANAGER gasDPM[CONFIG_PD_PORT_COUNT];

/********************Power Management Control globals************************************/
#if (TRUE == INCLUDE_POWER_MANAGEMENT_CTRL)
	/*State of the port whether IDLE or Active*/
    UINT8 gau8ISRPortState [CONFIG_PD_PORT_COUNT];
	/*Timer ID of IDLE Timer*/
    UINT8 gau8PortIdleTimerID [CONFIG_PD_PORT_COUNT];
    /*MCU Idle flag*/
    UINT8 gu8SetMCUidle;

#endif
/***************************************************************************************/

/*PDFU globals*/
#if (FALSE != INCLUDE_PDFU)    
    UINT8 gu8PDFUResBuffer[260] = {0};
#endif

/**************************************************************************************************/

/********************Power Balancing globals************************************/
#if (TRUE == INCLUDE_POWER_BALANCING) 
    /* Power Balancing Internal System Parameters */
    PB_INT_SYS_PARAM gsPBIntSysParam; 
    /* Power Balancing Internal Port Parameters */
    PB_INT_PORT_PARAM gasPBIntPortParam[CONFIG_PD_PORT_COUNT]; 
    /* Sequence in which ports are attached */
    UINT8 gu8AttachSeq; 
    /* Timer used to wait for Asynchronous request from the Sink */
    UINT8 gu8PBTimerID; 
#endif 
/**************************************************************************************************/ 
    
/********************Power Throttling globals************************************/
#if (TRUE == INCLUDE_POWER_THROTTLING)
    /* Power Throttling Port Parameters */
    PT_PORT_PARAM gasPTPortParam[CONFIG_PD_PORT_COUNT];  
#endif 
/**************************************************************************************************/  
    
/********************Configuration and Status register ************************************/    
GLOBAL_CFG_STATUS_DATA gasCfgStatusData = {0};
/**************************************************************************************************/ 

/*******************************************************************/
/******************* Functions**************************************/
/*******************************************************************/

void IntGlobals_PDInitialization(void)
{
    for (UINT8 u8PortNum = SET_TO_ZERO; u8PortNum < CONFIG_PD_PORT_COUNT; u8PortNum++)
    {
#if (TRUE == INCLUDE_POWER_MANAGEMENT_CTRL)
        UPD_PwrManagementInit(u8PortNum);        
#endif
        DPM_Init(u8PortNum);

        PE_InitPort(u8PortNum);
        
#if (FALSE != INCLUDE_PDFU)
        PE_FwUpdtInitialize();
#endif
        
#if (TRUE == INCLUDE_POWER_THROTTLING)
        PT_Init(u8PortNum); 
#endif 
    }
    
#if (TRUE == INCLUDE_POWER_BALANCING)
    /* Initialize PB System and Port Parameters */
    PB_Init();     
#endif 
}

void IntGlobals_StackStructVersion(void)
{
    gasCfgStatusData.u8MinorVersion = STRUCT_MINOR_VERSION;
    gasCfgStatusData.u8MajorVersion = STRUCT_MAJOR_VERSION;
    gasCfgStatusData.u8PSFMajorVersion = HIBYTE(SYSTEM_FW_REV); 
    gasCfgStatusData.u8PSFMinorVersion = LOBYTE(SYSTEM_FW_REV);
}
/**************************************************************************************************/
