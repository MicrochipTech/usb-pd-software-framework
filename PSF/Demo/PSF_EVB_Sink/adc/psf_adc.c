/*******************************************************************************
 User Interface File

  Company:
    Microchip Technology Inc.

  File Name:
    psf_adc.c

  Summary:
    This file interacts with user through the terminal.

  Description:
    This file is based on UART functionality which interacts with user through the terminal.
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

// *****************************************************************************
// *****************************************************************************
// Section: Included Files
// *****************************************************************************
// *****************************************************************************
#include <stdlib.h>		// Defines EXIT_FAILURE
#include <string.h>
#include <math.h>
#include "definitions.h"	// SYS function prototypes
#include "psf_stdinc.h"		// PSF include file
#include "psf_adc.h"
#include "psf_control_terminal.h"
#define   ADC_VREF         (3300U)	// 2500mV (2.5V)

void PSF_ADCRun()
{
    static UINT8 u8PrevPos = 3;	/* By default PDO's are available
					 * at position 3*/
	static UINT8 u8CurrentPos = 0;
	UINT16 u16adc_count;
	UINT32 u32input_voltage;
	UINT8 *pu8PrintString;
	static ADC_RUN_STATE u8State = eADC_INIT;
	UINT8 StrPrint[] = "\n\n\rKnob Voltage";
	switch (u8State) {
	case eADC_INIT:
		{
			ADC_Enable();
			ADC_ConversionStart();
			u8State = eADC_WAIT_FOR_CONVERSION;
			break;
		}

	case eADC_WAIT_FOR_CONVERSION:
		{
			if (!ADC_ConversionStatusGet()) {
				u8State = eADC_WAIT_FOR_CONVERSION;
			} else {
				u8State = eADC_SET_NEW_PDO;

			}
			break;
		}
	case eADC_SET_NEW_PDO:
		{
			/*
			 * Read the ADC Result 
			 */
			u16adc_count = ADC_ConversionResultGet();
			u32input_voltage = u16adc_count * ADC_VREF / 4095U;
            memset(gasCfgStatusData.sPerPortData
				    [PORT0].u32aNewSinkPDO,0,7);
     if (u32input_voltage < 650U) {
//                DEBUG_PRINT_PORT_STR(PORT0,
//                    "Pos 0\r\n");
				gasCfgStatusData.sPerPortData
				    [PORT0].u32aNewSinkPDO[0] =
				    CFG_PORT_0_SINK_PDO_1;
				gasCfgStatusData.sPerPortData[PORT0].
				    u8NewSinkPDOCnt = 1;
				u8CurrentPos = 0;
            } else if (u32input_voltage < 1000U) {
//                DEBUG_PRINT_PORT_STR(PORT0,
//                    "Pos 1\r\n");
				gasCfgStatusData.sPerPortData
				    [PORT0].u32aNewSinkPDO[0] =
				    CFG_PORT_0_SINK_PDO_1;
				gasCfgStatusData.sPerPortData[PORT0].
				    u32aNewSinkPDO[1] = CFG_PORT_0_SINK_PDO_2;
				gasCfgStatusData.sPerPortData[PORT0].
				    u8NewSinkPDOCnt = 2;
				u8CurrentPos = 1;
			} else if (u32input_voltage < 1400U) {
//                  DEBUG_PRINT_PORT_STR(PORT0,
//				     "Pos 2\r\n");
                memset(gasCfgStatusData.sPerPortData
				    [PORT0].u32aNewSinkPDO,0,7);
				gasCfgStatusData.sPerPortData
				    [PORT0].u32aNewSinkPDO[0] =
				    CFG_PORT_0_SINK_PDO_1;
				gasCfgStatusData.sPerPortData[PORT0].
				    u32aNewSinkPDO[1] = CFG_PORT_0_SINK_PDO_3;
				gasCfgStatusData.sPerPortData[PORT0].
				    u8NewSinkPDOCnt = 2;
				u8CurrentPos = 2;
			} else if (u32input_voltage < 1800U) {
//                    DEBUG_PRINT_PORT_STR(PORT0,
//                    "Pos 3\r\n");
				gasCfgStatusData.sPerPortData
				    [PORT0].u32aNewSinkPDO[0] =
				    CFG_PORT_0_SINK_PDO_1;
				gasCfgStatusData.sPerPortData[PORT0].
				    u32aNewSinkPDO[1] = CFG_PORT_0_SINK_PDO_2;
				gasCfgStatusData.sPerPortData[PORT0].
				    u32aNewSinkPDO[2] = CFG_PORT_0_SINK_PDO_3;
				gasCfgStatusData.sPerPortData[PORT0].
				    u32aNewSinkPDO[3] = CFG_PORT_0_SINK_PDO_4;
				gasCfgStatusData.sPerPortData[PORT0].
				    u8NewSinkPDOCnt = 4;
				u8CurrentPos = 3;
			} else if (u32input_voltage < 2200U) {
//                DEBUG_PRINT_PORT_STR(PORT0,
//                    "Pos 4\r\n");
               memset(gasCfgStatusData.sPerPortData
				    [PORT0].u32aNewSinkPDO,0,7);
				gasCfgStatusData.sPerPortData
				    [PORT0].u32aNewSinkPDO[0] =
				    CFG_PORT_0_SINK_PDO_1;
				gasCfgStatusData.sPerPortData[PORT0].
				    u32aNewSinkPDO[1] = CFG_PORT_0_SINK_PDO_3;
				gasCfgStatusData.sPerPortData[PORT0].
				    u8NewSinkPDOCnt = 2;
				u8CurrentPos = 4;
			} else if (u32input_voltage < 2600U) {
//                DEBUG_PRINT_PORT_STR(PORT0,
//				     "Pos 5\r\n");
				// Invalid position as per REV A board
				u8CurrentPos = 5;
			} else {
				// Invalid position as per REV A board
				u8CurrentPos = 6;
			}
            if (u8CurrentPos != u8PrevPos){
				u8PrevPos = u8CurrentPos;
				/*
				 * modify the pdo list as per the new position 
				 */
				/*
				 * User application may request PSF to renegotiate based
				 * on default PDOs
				 * gasCfgStatusData.sPerPortData[u8PortNum].u32aSinkPDO)
				 * or
				 * gasCfgStatusData.sPerPortData[u8PortNum].u32aNewSinkPDO).
				 * 
				 * To renegotiate with new PDOs, user application must
				 * ensure that new PDOs
				 * (gasCfgStatusData.sPerPortData[u8PortNum].u32aNewSinkPDO) 
				 * are configured and BIT(10) in
				 * gasCfgStatusData.sPerPortData[u8PortNum].u32CfgData
				 * variable is set. Then BIT(5) in
				 * gasCfgStatusData.sPerPortData[u8PortNum].u32ClientRequest
				 * variable needs to be set by the user application. 
				 */
                gasCfgStatusData.sPerPortData[PORT0].
				    u32CfgData |=
				    DPM_CLIENT_REQ_GET_PARTNER_IDENTITY;
				gasCfgStatusData.sPerPortData
				    [PORT0].u32ClientRequest |=
				    DPM_CLIENT_REQ_RENEGOTIATE;
				
				/*
				 * Print the Voltage of the corresponding ADC position 
				 */
				pu8PrintString =
				    HextoAscii(u32input_voltage,
					       sizeof(u32input_voltage));
				PCTWrite(StrPrint,
					 (UINT8 *) & pu8PrintString[0],
					 sizeof(pu8PrintString),
					 sizeof(StrPrint));
			}
           
			u8State = eADC_INIT;
			break;
		}
	}
}
