/*******************************************************************************
  Power Delivery Generic Timer Header

  Company:
    Microchip Technology Inc.

  File Name:
    pd_timer.h

  Description:
    This header file contains the function prototypes and definitions of the
    data types and constants that make up the interface to the Power delivery modules
 *******************************************************************************/
/*******************************************************************************
Copyright © [2022] Microchip Technology Inc. and its subsidiaries.

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
#ifndef _PD_TIMER_H_
#define _PD_TIMER_H_
// *****************************************************************************
// *****************************************************************************
// Section: Constants
// *****************************************************************************
// *****************************************************************************

/*Macros for Seconds,Milliseconds,Microseconds to PD Hardware timer Ticks conversion*/

#define SECONDS_TO_TICKS(TmrvalueinSec)     \
                        ((1000000 / MCHP_PSF_PDTIMER_INTERRUPT_RATE)* TmrvalueinSec)

#define MILLISECONDS_TO_TICKS(Tmrvalueinms) \
                        ((1000 / MCHP_PSF_PDTIMER_INTERRUPT_RATE)* Tmrvalueinms)

/***************************************************************************************/

/*Bit definition for u8TimerSt_PortNum in PDTIMER struct*/
#define PDTIMER_PORT_NUM        (BIT(3) | BIT(2) | BIT(1) | BIT(0))
#define PDTIMER_STATE           (BIT(4) | BIT(5))
#define PDTIMER_NON_ACTIVE          0x00
#define PDTIMER_ACTIVE              0x10
#define PDTIMER_EXPIRED             0x20
/***************************************************************************************/

/*Maximum Active concurrent timers for a port identified from PD Specification 3.0 */
#define PD_SYS_MAX_CONCURRENT_TIMERS        3

/*Maximum active concurrent timer for a port when Power fault is enabled*/
#if (TRUE == INCLUDE_POWER_FAULT_HANDLING)
	#define PD_SYS_POWER_FAULT_TIMER            2
#else
	#define PD_SYS_POWER_FAULT_TIMER		    0
#endif

/*Maximum active concurrent timers for a port when Power management control is enabled*/
#if (TRUE == INCLUDE_POWER_MANAGEMENT_CTRL)
	#define PD_SYS_PWR_MNGMNT_CTRL              1
#else
	#define PD_SYS_PWR_MNGMNT_CTRL              0
#endif

/*Maximum active concurrent timers for a port when PPS is enabled*/
#if (TRUE == INCLUDE_PD_SOURCE_PPS)
	#define PD_SYS_PPS_STATUS_TIMER              1
#else
	#define PD_SYS_PPS_STATUS_TIMER              0
#endif

/*Maximum active concurrent timers for a port when VCONN_SWAP is enabled*/
#if (TRUE == INCLUDE_PD_VCONN_SWAP)
	#define PD_SYS_VCONNSWAP_WAIT_TIMER           1
    #define PD_SYS_DISC_IDENTITY_TIMER            1 
#else
	#define PD_SYS_VCONNSWAP_WAIT_TIMER           0
    #define PD_SYS_DISC_IDENTITY_TIMER            0 
#endif

/*Maximum active concurrent timers for a port when DR_SWAP is enabled*/
#if (TRUE == INCLUDE_PD_DR_SWAP)
	#define PD_SYS_DRSWAP_WAIT_TIMER              1
#else
	#define PD_SYS_DRSWAP_WAIT_TIMER              0
#endif

/*Maximum active concurrent timers for a port when PR_SWAP is enabled*/
#if (TRUE == INCLUDE_PD_PR_SWAP)
	#define PD_SYS_PRSWAP_WAIT_TIMER              1
#else
	#define PD_SYS_PRSWAP_WAIT_TIMER              0
#endif

/*Maximum active concurrent timers for a port when VDM is enabled*/
#if (TRUE == INCLUDE_PD_VDM)
    #define PD_SYS_VDM_BUSY_TIMER                  1 
#else 
    #define PD_SYS_VDM_BUSY_TIMER                  0
#endif 

/*Maximum active concurrent timers for a port when AltMode is enabled*/
#if (TRUE == INCLUDE_PD_ALT_MODE)
    #define PD_SYS_ALT_MODE_ENTRY_TIMER             1 
#else 
    #define PD_SYS_ALT_MODE_ENTRY_TIMER             0
#endif 

/***********************************************************************************/
/*Maximum concurrent timer per port*/
#define MAX_CONCURRENT_TIMERS_PER_PORT           (PD_SYS_MAX_CONCURRENT_TIMERS + \
                                                 PD_SYS_POWER_FAULT_TIMER + \
                                                 PD_SYS_PWR_MNGMNT_CTRL + \
                                                 PD_SYS_PPS_STATUS_TIMER + \
                                                 PD_SYS_VCONNSWAP_WAIT_TIMER + \
                                                 PD_SYS_DRSWAP_WAIT_TIMER + \
                                                 PD_SYS_PRSWAP_WAIT_TIMER + \
                                                 PD_SYS_VDM_BUSY_TIMER + \
                                                 PD_SYS_ALT_MODE_ENTRY_TIMER + \
                                                 PD_SYS_DISC_IDENTITY_TIMER)

/* This variable of size MAX_CONCURRENT_TIMERS is the software timer which stores the timeout value, timer state,
 call back function and arguments to be passed to call back function*/
#define MAX_CONCURRENT_TIMERS				 (CONFIG_PD_PORT_COUNT * MAX_CONCURRENT_TIMERS_PER_PORT)

/***************************************************************************************/

/* Typedef PD Timer callback*/
typedef void(*PDTimerCallback) (UINT8,UINT8);

/***************************************************************************************/
// *****************************************************************************
// *****************************************************************************
// Section: Data Structures
// *****************************************************************************
// *****************************************************************************

/*****************************************************************************
 PD GENERIC TIMER
  Summary:
    Identifies the generic PD timer information.

  Description:
    This type identifies the generic PD Timer information. Each new timer must have these parameters
    before starting new timer.

  Remarks:
    Need to be packed always based on type of microcontroller.
**********************************************************************************/
typedef struct MCHP_PSF_STRUCT_PACKED_START _Timer 
{
  
    #if (TRUE == MCHP_PSF_CONFIG_16BIT_PDTIMER_COUNTER)
	UINT16 u16TimeoutTickCnt; /*Stores Timer value in terms of tick count*/			
    #else
	UINT32 u32TimeoutTickCnt;			
    #endif	
	
	PDTimerCallback pfnTimerCallback;	

	volatile UINT8 u8TimerStPortNum; /*[3:0] - PortNum  [5:4] - TimerState*/
	
	UINT8 u8PDState;		/* Stores SM states of TypeC or PE state machine if 
                             required in callback for reference*/			

} MCHP_PSF_STRUCT_PACKED_END  PDTIMER;

// *****************************************************************************
// *****************************************************************************
// Section: Interface Routines
// *****************************************************************************
// *****************************************************************************

/**************************************************************************************************
    Function:
        void PDTimer_Init (void)

	Summary:
		Initializes the PD Timer variables and starts the PD Hardware timer

	Devices Supported:
		UPD350 REV A

	Description:
		This API is called in the PD Stack initialization sequence to start the Hardware Timer 
        dedicated for the PSF. This API configures all the software Timers instances 
        to disabled state and enables the Hardware timer for specified 
        Interrupt rate through the User implemented API.

	Precondition:
		None.

	Parameters:
		None

	Returns:
		UINT8 - Returns Timer initialization status.

	Remarks:
		This API calls MCHP_PSF_HOOK_HW_PDTIMER_INIT() API which is defined by 
        the user to configure and start the Hardware timer.
**************************************************************************************************/
UINT8 PDTimer_Init (void);
/**************************************************************************************************
    Function:
        UINT8 PDTimer_Start (UINT32 u32TimeoutTicks, PDTimerCallback pfnTimerCallback, 
                                      UINT8 u8PortNum, UINT8 u8PDState)

	Summary:
		Starts the Software timer for the given timeout.

	Devices Supported:
		UPD350 REV A

	Description:
		This API will start the software timer for a given timeout (u32TimeoutTicks).
        pfnTimerCallback Timer call back function is registered and called on timeout.
        This API also returns the Timer ID of the software instance.

	Precondition:
		None.

	Parameters:
		u32TimeoutTicks - Timeout value in ticks
		pfnTimerCallback - Address of the Callback function to be executed after the
                            software timer expiration
		u8PortNum - Port Number for which the timeout has to be set
		u8PDState - PD State that will be passed as an argument to the callback function

	Returns:
		UINT8 - returns the Timer ID of the software instance.
        This Timer ID can be used to kill the timer started via PDTimer_Kill if required
        before its expiration.

	Remarks:
	Callback function address passed as an argument to the API can be NULL 
    if there are no functions to be executed after the timer expiration
    Similarly, u8PDState parameter is not mandatory.
**************************************************************************************************/
UINT8 PDTimer_Start (UINT32 u32TimeoutTicks, PDTimerCallback pfnTimerCallback, \
                        UINT8 u8PortNum, UINT8 u8PDState);
/**************************************************************************************************
    Function:
		void PDTimer_WaitforTicks (UINT32 u32TimeoutTicks);

	Summary:
		API to start a blocking timer
	
	Devices Supported:
		UPD350 REV A

	Description:
		This API is called to start a blocking timer for u32TimeoutTicks.

	Precondition:
		None.

	Parameters:
		u32TimeoutTicks - Timeout for which blocking timer has to be started

	Return:
		None.

	Remarks:
		None.
**************************************************************************************************/
void PDTimer_WaitforTicks (UINT32 u32TimeoutTicks);
/**************************************************************************************************

    Function:
        void PDTimer_Kill (UINT8 u8TimerID)

	Summary:
		Stops the Software timer for a given Timer ID started using PDTimer_Start API.

	Devices Supported:
		UPD350 REV A

	Description:
		This API will stop the software timer with a given timer ID 
        and set the timer state as "NON ACTIVE"

	Precondition:
		None.

	Parameters:
		u8TimerID - Timer ID of the Software Timer to be stopped

	Returns:
		None.

	Remarks:
		None
**************************************************************************************************/
void PDTimer_Kill (UINT8 u8TimerID);
/**************************************************************************************************
	Function:
        void PDTimer_KillPortTimers (UINT8 u8PortNum)

	Summary:
		Stops all the active Software timers for a given Port Number

	Devices Supported:
		UPD350 REV A

	Description:
		This API will stop all the software timer for a given Port and 
        set the timer state as "NON_ACTIVE"

	Precondition:
		None.

	Parameters:
		u8PortNum - Port Number for which all the Software Timer to be stopped

	Return:
		None.

	Remarks:
		None
**************************************************************************************************/
void PDTimer_KillPortTimers (UINT8 u8PortNum);
/*************************************************************************************************
	Function:
		void PDTimer_InterruptHandler (void);

	Summary:
		Interrupt Handler for the USB Power delivery Hardware Timer
	
	Devices Supported:
		UPD350 REV A

	Description:
		This API is the interrupt service routine for the interrupts generated by PD Hardware timer. 
		All the software timer's timeout values are updated and checked for timeout expiration. 
        If the timeout has expired , then the callback function for the software timer is executed

	Precondition:
		None.

	Parameters:
		None.

	Return:
		None.

	Remarks:
        None
**************************************************************************************************/
void PDTimer_InterruptHandler (void);
/**************************************************************************************************/
#endif /*_PD_TIMER_H_*/
