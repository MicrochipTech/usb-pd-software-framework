/*******************************************************************************
  Internal globals Header file

  Company:
    Microchip Technology Inc.

  File Name:
    int_globals.h

  Description:
    This file contains globals and related functions used within the PSF stack 
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
/*******************************************************************************
  File Name:
    int_globals.h

  Summary:
    Internal globals header files

  Description:
    File contains globals used internally within the PSF stack
*******************************************************************************/

#ifndef _INT_GLOBALS_H_
#define _INT_GLOBALS_H_


// *****************************************************************************
// *****************************************************************************
// Section: Data types and constants
// *****************************************************************************
// *****************************************************************************

/*Timer module globals*/
extern PDTIMER gasPDTimers[MAX_CONCURRENT_TIMERS];

/*Type C module globals*/
extern TYPEC_CONTROL  gasTypeCcontrol[CONFIG_PD_PORT_COUNT];

/*Protocol Layer globals*/

/* PRL State variables */
extern PROTOCOL_LAYER_STRUCT gasPRL [CONFIG_PD_PORT_COUNT];

 /* PRL receiver Struct*/
extern PRL_RECVBUFF gasPRLRecvBuff [CONFIG_PD_PORT_COUNT];

#if (TRUE == INCLUDE_PD_3_0)
    /* PRL extended message buffer for Tx & Rx*/
    extern PRL_EXTMSG_BUFF gasExtendedMsgBuff [CONFIG_PD_PORT_COUNT];

    /* PRL chunk state variable struct*/
    extern PRL_CHUNK_STRUCT gasChunkSM [CONFIG_PD_PORT_COUNT];

#endif 
   
/*Policy Engine globals*/
extern POLICY_ENGINE_STATUS gasPolicyEngine[CONFIG_PD_PORT_COUNT];

/*Policy manager globals*/
extern DEVICE_POLICY_MANAGER gasDPM[CONFIG_PD_PORT_COUNT];

/*Power Management Control globals*/
#if (TRUE == INCLUDE_POWER_MANAGEMENT_CTRL)
	/*State of the port whether IDLE or Active*/
    extern UINT8 gau8ISRPortState [CONFIG_PD_PORT_COUNT];
	/*Timer ID of IDLE Timer*/
    extern UINT8 gau8PortIdleTimerID [CONFIG_PD_PORT_COUNT];
    /*MCU Idle flag*/
    extern UINT8 gu8SetMCUidle;

#endif

/*PDFU globals*/
#if (FALSE != INCLUDE_PDFU)    
    extern UINT8 gu8PDFUResBuffer[PRL_MAX_EXTN_MSG_LEN_IN_BYTES];
#endif
    
/********************Power Balancing globals************************************/
#if (TRUE == INCLUDE_POWER_BALANCING) 
    /* Power Balancing Internal System Parameters */
    extern PB_INT_SYS_PARAM gsPBIntSysParam; 
    /* Power Balancing Internal Port Parameters */
    extern PB_INT_PORT_PARAM gasPBIntPortParam[CONFIG_PD_PORT_COUNT]; 
    /* Sequence in which ports are attached */
    extern UINT8 gu8AttachSeq; 
    /* Timer used to wait for Asynchronous request from the Sink */
    extern UINT8 gu8PBTimerID; 
#endif 
/**************************************************************************************************/ 
       
/********************Power Throttling globals************************************/
#if (TRUE == INCLUDE_POWER_THROTTLING)
    /* Power Throttling Port Parameters */
    extern PT_PORT_PARAM gasPTPortParam[CONFIG_PD_PORT_COUNT];  
#endif  
/**************************************************************************************************/  

// *****************************************************************************
// *****************************************************************************
//  Section: Interface Routines
// *****************************************************************************
// *****************************************************************************
 /****************************************************************************
    Function:
        void IntGlobals_PDInitialization(void)

    Summary:
        Internal globals initialization API

    Devices Supported:
        UPD350 REV A

    Description:
        This API initializes internal globals of various modules per port 
        specifically 

    Conditions:
        None

    Input:
        None

    Return:
        None

    Remarks:
        None

*****************************************************************************/
void IntGlobals_PDInitialization(void);
/*****************************************************************************/

 /****************************************************************************
    Function:
        void IntGlobals_StackStructVersion(void)

    Summary:
        Internal PSF version and configuration structure version initialization API

    Description:
        Initializes the PSF version and configuration Structure version 

    Conditions:
        None

    Input:
        None

    Return:
        None

    Remarks:
        None

*****************************************************************************/

void IntGlobals_StackStructVersion(void);
/*****************************************************************************/
#endif /*_INT_GLOBALS_H_*/
