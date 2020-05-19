/*******************************************************************************
  Power Throttling Header File 

  Company:
    Microchip Technology Inc.

  File Name:
    pt_mngr.h

  Description:
    This header file contains the data structures, constant definitions and 
	function prototypes for Power Throttling. 
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

#ifndef _PT_MNGR_H    /* Guard against multiple inclusion */
#define _PT_MNGR_H

/* ************************************************************************** */
/* ************************************************************************** */
/* Section: Constants                                                         */
/* ************************************************************************** */
/* ************************************************************************** */ 

/***************************************************************************************/
// *****************************************************************************
// *****************************************************************************
// Section: Data Structures
// *****************************************************************************
// *****************************************************************************

typedef enum RenegStatus
{
    ePT_RENEG_REQ_NOT_INITIATED = 1,
    ePT_RENEG_REQ_INITIATED, 
    ePT_RENEG_REQ_ACCEPTED
}PT_RENEG_STATUS;

/*****************************************************************************
  Summary:
    Power Throttling Port Parameters   

  Description:
    This structure contains the port parameters that are specific to PT layer. 

  Remarks:
    Need to be packed always based on type of microcontroller.
**********************************************************************************/
typedef struct MCHP_PSF_STRUCT_PACKED_START _PTPortParam 
{
    UINT8 u8PrevPTBank;      // Holds the previously selected Throttling Bank
    PT_RENEG_STATUS ePTRenegSts; // Holds the renegotiation request status 
} MCHP_PSF_STRUCT_PACKED_END  PT_PORT_PARAM;

// *****************************************************************************
// *****************************************************************************
// Section: Interface Functions
// *****************************************************************************
// *****************************************************************************
/**************************************************************************************************
    Function:
        void PT_Init(UINT8 u8PortNum);  

    Summary:
        Power Throttling Initialization API. 

    Description:
        This API is called to initialize the parameters of Power Throttling. 
 
    Conditions:
        None.

    Input:
        u8PortNum - Port Number 

    Return:
        None. 

    Remarks:
        None. 

**************************************************************************************************/
void PT_Init(UINT8 u8PortNum); 

/**************************************************************************************************
    Function:
        void PT_HandleBankSwitch(UINT8 u8PortNum); 

    Summary:
        Handles switching of Power Throttling Bank.

    Description:
        This API detects that the power throttling bank has been changed 
        and triggers renegotiation, informs PB, triggers alert message transmission, 
        disable/enable the port based on the new bank value.
 
    Conditions:
        None.

    Input:
        u8PortNum - Port Number 

    Return:
        None. 

    Remarks:
        None. 

**************************************************************************************************/
void PT_HandleBankSwitch(UINT8 u8PortNum); 

/**************************************************************************************************
    Function:
        void PT_CalculateSrcPDOs(UINT8 u8PortNum); 

    Summary:
        This API calculates the Source capabilities based on the currently 
        active throttling bank.

    Description:
        This API calculates the Source capabilities based on the currently 
        active throttling bank.        
 
    Conditions:
        None.

    Input:
        u8PortNum - Port number.

    Return:
        None. 

    Remarks:
        None. 

**************************************************************************************************/
void PT_CalculateSrcPDOs(UINT8 u8PortNum);

/**************************************************************************************************
    Function:
        void PT_HandleDPMBusy(UINT8 u8PortNum);

    Summary:
        Handles the busy notification posted by DPM.

    Description:
        DPM would post a busy notification in case the client request raised to 
        trigger renegotiation is rejected. This API handles the busy notification 
        by raising the client request again. 
 
    Conditions:
        None.

    Input:
        u8PortNum - Port Number 

    Return:
        None. 

    Remarks:
        None. 

**************************************************************************************************/
void PT_HandleDPMBusy(UINT8 u8PortNum); 

#endif /* _PT_MNGR_H */
/* *****************************************************************************
 End of File
 */
