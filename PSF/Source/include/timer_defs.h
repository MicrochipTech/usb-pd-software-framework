/*******************************************************************************
  Type C and Policy Engine Timer Configuration file

  Company:
    Microchip Technology Inc.

  File Name:
    timer_defs.h

  Description:
    This header file contains the definitions of the timers used in Type C and 
    Policy Engine state machine. 
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

#ifndef _TIMER_DEFS_H    /* Guard against multiple inclusion */
#define _TIMER_DEFS_H

// *****************************************************************************
// *****************************************************************************
// Section: Constants
// *****************************************************************************
// *****************************************************************************

// *****************************************************************************
// Section: Type C Timeout configuration
// *****************************************************************************

/*  CONFIG_TYPEC_TCCDEBOUNCE_TIMEOUT_MS defines the tCCDebounce timeout specified in the USB 
    Type C Specification. Default value of CONFIG_TYPEC_TCCDEBOUNCE_TIMEOUT_MS is set as 
    150 milliseconds. */
#define CONFIG_TYPEC_TCCDEBOUNCE_TIMEOUT_MS			MILLISECONDS_TO_TICKS(150)

/*  CONFIG_TYPEC_TPDEBOUNCE_TIMEOUT_MS defines the tPDDebounce timeout specified in the 
    USB Type C Specification. Default value of this macro is set as 10 milliseconds.*/
#define CONFIG_TYPEC_TPDEBOUNCE_TIMEOUT_MS			MILLISECONDS_TO_TICKS(10)

/*  CONFIG_TYPEC_ERRORRECOVERY_TIMEOUT_MS defines the tErrorRecovery timeout specified in the 
    USB Type C Specification.  Default value of CONFIG_TYPEC_ERRORRECOVERY_TIMEOUT_MS is set as 
    500 milliseconds.*/
#define CONFIG_TYPEC_ERRORRECOVERY_TIMEOUT_MS       MILLISECONDS_TO_TICKS(500)

/*  CONFIG_TYPEC_VCONNDISCHARGE_TIMEOUT_MS defines the tVCONNDischarge timeout specified in the 
    USB Type C Specification. Default value of CONFIG_TYPEC_VCONNDISCHARGE_TIMEOUT_MS is set 
    as 35 milliseconds. */ 
#define CONFIG_TYPEC_VCONNDISCHARGE_TIMEOUT_MS      MILLISECONDS_TO_TICKS(35)

/*  CONFIG_TYPEC_VBUS_ON_TIMER_MS defines the tVBUSON specified in the USB-TypeC Specification. 
    Default value of CONFIG_TYPEC_VBUS_ON_TIMER_MS is set as 275 milliseconds.*/
#define CONFIG_TYPEC_VBUS_ON_TIMER_MS			    MILLISECONDS_TO_TICKS(275)

/*  CONFIG_TYPEC_VBUS_OFF_TIMER_MS defines the tVBUSOFF specified in the USB-TypeC Specification. 
    Default value of CONFIG_TYPEC_VBUS_OFF_TIMER_MS is set as 650 milliseconds.*/
#define CONFIG_TYPEC_VBUS_OFF_TIMER_MS              MILLISECONDS_TO_TICKS(650)

/*  CONFIG_TYPEC_VCONNON_TIMEOUT_MS defines the tVCONNON specified in the USB-Type C Specification. 
    Default value of CONFIG_TYPEC_VCONNON_TIMEOUT_MS is set as 10 milliseconds.*/
#define CONFIG_TYPEC_VCONNON_TIMEOUT_MS			    MILLISECONDS_TO_TICKS(10)

/*  CONFIG_TYPEC_VCONNOFF_TIMEOUT_MS defines the tVCONNOFF specified in the USB-Type C Specification. 
    Default value of CONFIG_TYPEC_VCONNOFF_TIMEOUT_MS is set as 25 milliseconds.*/
#define CONFIG_TYPEC_VCONNOFF_TIMEOUT_MS            MILLISECONDS_TO_TICKS(25)

// *****************************************************************************
// Section: Policy Engine Timeout configuration
// *****************************************************************************

/*  CONFIG_PE_SOURCECAPABILITY_TIMEOUT_MS defines the SourceCapabilityTimer specified in the 
    USB-PD Specification. Default value of CONFIG_PE_SOURCECAPABILITY_TIMEOUT_MS is set as 
    150 milliseconds.*/
#define CONFIG_PE_SOURCECAPABILITY_TIMEOUT_MS          MILLISECONDS_TO_TICKS(150)

/*  CONFIG_PE_SRC_READY_TIMEOUT_MS defines the tSrcReady specified in the PD 3.0 Specification. 
    Default value of CONFIG_PE_SRC_READY_TIMEOUT_MS is set as 285 milliseconds.*/
#define CONFIG_PE_SRC_READY_TIMEOUT_MS                 MILLISECONDS_TO_TICKS(285)

/*  CONFIG_PE_VCONNON_TIMEOUT_MS defines the tVCONNSourceOn specified in the USB PD Specification. 
    Default value of CONFIG_PE_VCONNON_TIMEOUT_MS is set as 50 milliseconds. */
#define CONFIG_PE_VCONNON_TIMEOUT_MS                   MILLISECONDS_TO_TICKS(50)

/*  CONFIG_PE_VCONNON_SELF_TIMEOUT_MS defines the Self timer used to monitor VCONN ON
    and send hard reset in case of VCONN ON Failure. The value of Self timer is 
    set to a value greater than tVCONNSourceOn. Default value of CONFIG_PE_VCONNON_SELF_TIMEOUT_MS 
    is set as 75 milliseconds.*/
#define CONFIG_PE_VCONNON_SELF_TIMEOUT_MS			   MILLISECONDS_TO_TICKS(75)

/*  CONFIG_PE_VCONNOFF_TIMEOUT_MS defines the tVCONNSourceOff specified in the USB PD Specification. 
    Default value of CONFIG_PE_VCONNOFF_TIMEOUT_MS is set as 25 milliseconds.*/
#define CONFIG_PE_VCONNOFF_TIMEOUT_MS                  MILLISECONDS_TO_TICKS(25)

/*  CONFIG_PE_NORESPONSE_TIMEOUT_MS defines the NoResponseTimer specified in the USB-PD Specification. 
    Default value of CONFIG_PE_NORESPONSE_TIMEOUT_MS is set as 5.5 seconds.*/
#define CONFIG_PE_NORESPONSE_TIMEOUT_MS                MILLISECONDS_TO_TICKS(5500)

/*  CONFIG_PE_SENDERRESPONSE_TIMEOUT_MS defines the SenderResponseTimer specified in the USB-PD Specification. 
    Default value of CONFIG_PE_SENDERRESPONSE_TIMEOUT_MS is set as 24 milliseconds.*/
#define CONFIG_PE_SENDERRESPONSE_TIMEOUT_MS            MILLISECONDS_TO_TICKS(25)

/*  CONFIG_PE_SINKWAITCAP_TIMEOUT_MS defines the SinkWaitCapTimer specified in the USB-PD Specification. 
    Default value of CONFIG_PE_SINKWAITCAP_TIMEOUT_MS is set as 465 milliseconds.*/
#define CONFIG_PE_SINKWAITCAP_TIMEOUT_MS               MILLISECONDS_TO_TICKS(465)

/*  CONFIG_PE_PSTRANSITION_TIMEOUT_MS defines the PSTransitionTimer specified in the USB-PD Specification. 
    Default value of CONFIG_PE_PSTRANSITION_TIMEOUT_MS is set as 500 milliseconds.*/
#define CONFIG_PE_PSTRANSITION_TIMEOUT_MS              MILLISECONDS_TO_TICKS(500)

/*  CONFIG_PE_SINKREQUEST_TIMEOUT_MS defines the SinkRequestTimer specified in the USB-PD Specification. 
    Default value of CONFIG_PE_SINKREQUEST_TIMEOUT_MS is set as 100 milliseconds.*/
#define CONFIG_PE_SINKREQUEST_TIMEOUT_MS               MILLISECONDS_TO_TICKS(100)

/*  CONFIG_PE_VDMRESPONSE_TIMEOUT_MS defines the VDMResponseTimer specified in the USB-PD Specification. 
    Default value of CONFIG_PE_VDMRESPONSE_TIMEOUT_MS is set as 28 milliseconds.*/
#define CONFIG_PE_VDMRESPONSE_TIMEOUT_MS               MILLISECONDS_TO_TICKS(28)

/*  CONFIG_PE_PSHARDRESET_TIMEOUT_MS defines the PSHardResetTimer specified in the USB-PD Specification. 
    Default value of CONFIG_PE_PSHARDRESET_TIMEOUT_MS is set as 28 milliseconds.*/
#define CONFIG_PE_PSHARDRESET_TIMEOUT_MS               MILLISECONDS_TO_TICKS(28)

/*  CONFIG_PE_SRCRECOVER_TIMEOUT_MS defines the tSrcRecover specified in the USB-PD Specification. 
    Default value of CONFIG_PE_SRCRECOVER_TIMEOUT_MS is set as 800 milliseconds.*/
#define CONFIG_PE_SRCRECOVER_TIMEOUT_MS                MILLISECONDS_TO_TICKS(800)

/*  CONFIG_PE_SRCTRANSISTION_TIMEOUT_MS defines the tSrcTransistionTimer specified in the 
    USB-PD Specification. By default, it is set to 28 milliseconds.*/
#define CONFIG_PE_SRCTRANSISTION_TIMEOUT_MS	           MILLISECONDS_TO_TICKS(28)

/*  CONFIG_PRL_CHUNKSENDERREQUEST_TIMEOUT_MS defines the ChunkSenderRequestTimer specified in the USB-PD Specification. 
    Default value of CONFIG_PRL_CHUNKSENDERREQUEST_TIMEOUT_MS is set as 26 milliseconds.*/
#define CONFIG_PRL_CHUNKSENDERREQUEST_TIMEOUT_MS       MILLISECONDS_TO_TICKS(26)

/*  CONFIG_PRL_CHUNKSENDERRESPONSE_TIMEOUT_MS defines the ChunkSenderResponseTimer specified in the USB-PD Specification. 
    Default value of CONFIG_PRL_CHUNKSENDERRESPONSE_TIMEOUT_MS is set as 26 milliseconds.*/
#define CONFIG_PRL_CHUNKSENDERRESPONSE_TIMEOUT_MS      MILLISECONDS_TO_TICKS(26)

/*  CONFIG_PRL_SINKTX_TIMEOUT_MS defines the SinkTxTimer specified in the USB-PD Specification. 
    Default value of CONFIG_PRL_SINKTX_TIMEOUT_MS is set as 16 milliseconds.*/
#define CONFIG_PRL_SINKTX_TIMEOUT_MS				   MILLISECONDS_TO_TICKS(16)

/*  CONFIG_PRL_BIST_CONTMODE_TIMEOUT_MS defines the BISTContModeTimer specified in the USB-PD Specification.
    Default value of CONFIG_PRL_BIST_CONTMODE_TIMEOUT_MS is set as 45 milliseconds.*/
#define CONFIG_PRL_BIST_CONTMODE_TIMEOUT_MS            MILLISECONDS_TO_TICKS(45)                                             


#endif /* _TIMER_DEFS_H */

/* *****************************************************************************
 End of File
 */
