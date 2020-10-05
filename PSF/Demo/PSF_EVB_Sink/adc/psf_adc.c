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
#define   ADC_VREF         (3300U)	// 3300mV (3.3V)
extern UINT8 gu8PSFIdle;
extern UINT8 gu8PDContract;
void PSF_ADCRun()
{
    static UINT8 u8PrevPos = 4;	/* By default PDO's are available
					            * at position 4*/
	static UINT8 u8CurrentPos = 4;
	UINT16 u16adc_count;
	UINT32 u32input_voltage;
	UINT8 *pu8PrintString;
	static ADC_RUN_STATE u8State = eADC_INIT;
	UINT8 StrPrint[] = "\n\n\r> Knob Voltage";
	switch (u8State) 
    {
        case eADC_INIT:
        {
            ADC_Enable();
            ADC_ConversionStart();
            u8State = eADC_WAIT_FOR_CONVERSION;
            break;
        }
        case eADC_WAIT_FOR_CONVERSION:
        {
            if (!ADC_ConversionStatusGet()) 
            {
                u8State = eADC_WAIT_FOR_CONVERSION;
            } 
            else 
            {
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
            memset(gasCfgStatusData.sPerPortData[PORT0].u32aNewSinkPDO,0,7);
            if(!((gu8PDContract == true)&&(gu8PSFIdle == true)))
            {
                u8State = eADC_INIT;
                break;
            }
            if (u32input_voltage < 650U) 
            {
                /*Supported PDO is (5V,3A)*/
                gasCfgStatusData.sPerPortData[PORT0].u32aNewSinkPDO[0] = CFG_PORT_0_SINK_PDO_1;
                gasCfgStatusData.sPerPortData[PORT0].u8NewSinkPDOCnt = 1;
                u8CurrentPos = 0;
            } 
            else if (u32input_voltage < 1000U) 
            {
                /*Supported PDOs are (5V,3A),(9V,3A)*/
                gasCfgStatusData.sPerPortData[PORT0].u32aNewSinkPDO[0] = CFG_PORT_0_SINK_PDO_1;
                gasCfgStatusData.sPerPortData[PORT0].u32aNewSinkPDO[1] = CFG_PORT_0_SINK_PDO_2;
                gasCfgStatusData.sPerPortData[PORT0].u8NewSinkPDOCnt = 2;
                u8CurrentPos = 1;
            } 
            else if (u32input_voltage < 1400U) 
            {
                /*Supported PDOs are (5V,3A),(15V,3A)*/
                gasCfgStatusData.sPerPortData[PORT0].u32aNewSinkPDO[0] = CFG_PORT_0_SINK_PDO_1;
                gasCfgStatusData.sPerPortData[PORT0].u32aNewSinkPDO[1] = CFG_PORT_0_SINK_PDO_3;
                gasCfgStatusData.sPerPortData[PORT0].u8NewSinkPDOCnt = 2;
                u8CurrentPos = 2;
            } 
            else if (u32input_voltage < 1800U) 
            {
                /*Supported PDOs are (5V,3A),(20V,3A)*/
                gasCfgStatusData.sPerPortData[PORT0].u32aNewSinkPDO[0] = CFG_PORT_0_SINK_PDO_1;
                gasCfgStatusData.sPerPortData[PORT0].u32aNewSinkPDO[1] = CFG_PORT_0_SINK_PDO_4;
                gasCfgStatusData.sPerPortData[PORT0].u8NewSinkPDOCnt = 2;
                u8CurrentPos = 3;
            }
            else if (u32input_voltage < 2200U) 
            {
                /*Supported PDOs are (5V,3A),(9V,3A),(15V,3A),(20V,3A) in Sink Mode A*/
                gasCfgStatusData.sPerPortData[PORT0].u32aNewSinkPDO[0] = CFG_PORT_0_SINK_PDO_1;
                gasCfgStatusData.sPerPortData[PORT0].u32aNewSinkPDO[1] = CFG_PORT_0_SINK_PDO_2;
                gasCfgStatusData.sPerPortData[PORT0].u32aNewSinkPDO[2] = CFG_PORT_0_SINK_PDO_3;
                gasCfgStatusData.sPerPortData[PORT0].u32aNewSinkPDO[3] = CFG_PORT_0_SINK_PDO_4;
                gasCfgStatusData.sPerPortData[PORT0].u8NewSinkPDOCnt = 4;
                u8CurrentPos = 4;
            } 
            else if (u32input_voltage < 2600U) 
            {
                /*Supported PDOs are (5V,3A),(9V,3A),(15V,3A),(20V,3A) in Sink Mode B*/
                gasCfgStatusData.sPerPortData[PORT0].u32aNewSinkPDO[0] = CFG_PORT_0_SINK_PDO_1;
                gasCfgStatusData.sPerPortData[PORT0].u32aNewSinkPDO[0] |= CFG_PORT_0_SINK_MODEB;
                gasCfgStatusData.sPerPortData[PORT0].u32aNewSinkPDO[1] = CFG_PORT_0_SINK_PDO_2;
                gasCfgStatusData.sPerPortData[PORT0].u32aNewSinkPDO[2] = CFG_PORT_0_SINK_PDO_3;
                gasCfgStatusData.sPerPortData[PORT0].u32aNewSinkPDO[3] = CFG_PORT_0_SINK_PDO_4;
                gasCfgStatusData.sPerPortData[PORT0].u8NewSinkPDOCnt = 4;
                u8CurrentPos = 5;
            } 
            else
            {
                // Invalid position as per REV A board
                u8CurrentPos = 6;
            }
            if (u8CurrentPos != u8PrevPos)
            {
                u8PrevPos = u8CurrentPos;
                /*
                 * modify the pdo list as per the new position 
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
                gasCfgStatusData.sPerPortData[PORT0].u32CfgData |= DPM_CLIENT_REQ_GET_PARTNER_IDENTITY;
                gasCfgStatusData.sPerPortData[PORT0].u32ClientRequest |= DPM_CLIENT_REQ_RENEGOTIATE;

                DEBUG_PRINT_PORT_STR(PORT0,"\n\n\r> Client Req set");

                /*clear the variables since the request is handled */
                gu8PDContract = false;
                gu8PSFIdle = false;
                
                /*Print the selected knob voltage*/
                pu8PrintString = HextoAscii(u32input_voltage,sizeof(u32input_voltage));
                PCTWrite(StrPrint,(UINT8 *) & pu8PrintString[0],sizeof(pu8PrintString),sizeof(StrPrint));
            }
            u8State = eADC_INIT;
            break;
        }
	}
}
