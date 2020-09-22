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
#include <stddef.h>                     // Defines NULL
#include <stdbool.h>                    // Defines true
#include <stdlib.h>                     // Defines EXIT_FAILURE
#include <string.h>
#include <math.h>
#include "definitions.h"                // SYS function prototypes
#include "psf_stdinc.h"                 // PSF include file
#include "psf_adc.h"
#include "psf_control_terminal.h"
#define   ADC_VREF         (2500U)            //2500mV (2.5V) 


#define ADC_P0_CFG_PORT_0_SINK_PDO_1     CFG_FORM_SINK_FIXED_PDO1(5000U, 2400U, \
                                    CFG_PORT_0_DUAL_ROLE_DATA, \
                                    CFG_PORT_0_SINK_USB_COMM, \
                                    CFG_PORT_0_SINK_UNCONSTRAINED_PWR, \
                                    CFG_PORT_0_SINK_HIGHER_CAPABILITY, \
                                    CFG_PORT_0_DUAL_ROLE_POWER);


#define ADC_P1_CFG_PORT_0_SINK_PDO_1     CFG_FORM_SINK_FIXED_PDO1(5000U, 2400U, \
                                    CFG_PORT_0_DUAL_ROLE_DATA, \
                                    CFG_PORT_0_SINK_USB_COMM, \
                                    CFG_PORT_0_SINK_UNCONSTRAINED_PWR, \
                                    CFG_PORT_0_SINK_HIGHER_CURRENT_CAPABILITY.\
                                      CFG_PORT_0_DUAL_ROLE_POWER);


#define ADC_P2_CFG_PORT_0_SINK_PDO_1     CFG_FORM_SINK_FIXED_PDO1(5000U, 2400U, \
                                    CFG_PORT_0_DUAL_ROLE_DATA, \
                                    CFG_PORT_0_SINK_USB_COMM, \
                                    CFG_PORT_0_SINK_UNCONSTRAINED_PWR, \
                                    CFG_PORT_0_SINK_HIGHER_CAPABILITY, \
                                    CFG_PORT_0_DUAL_ROLE_POWER);

#define ADC_P2_CFG_PORT_0_SINK_PDO_2     CFG_FORM_FIXED_PDOx(9000U,3000U) 




#define ADC_P3_CFG_PORT_0_SINK_PDO_1     CFG_FORM_SINK_FIXED_PDO1(5000U, 2400U, \
                                    CFG_PORT_0_DUAL_ROLE_DATA, \
                                    CFG_PORT_0_SINK_USB_COMM, \
                                    CFG_PORT_0_SINK_UNCONSTRAINED_PWR, \
                                    CFG_PORT_0_SINK_HIGHER_CURRENT_CAPABILITY, \
                                    CFG_PORT_0_DUAL_ROLE_POWER);

#define ADC_P3_CFG_PORT_0_SINK_PDO_2     CFG_FORM_FIXED_PDOx(9000U,3000U) 





void PSF_ADCRun()
{
    static UINT8 u8PrevPos=7;/*By default PDO's are available at position 7*/
    UINT8 u8CurrentPos=0;
    UINT16 u16adc_count;
    UINT32 u32input_voltage;
    char *VConvert;
    static State u8State = INITIALIZE;
    UINT8 StrPrint[]="Voltage";
    
    switch(u8State)
    {
        case INITIALIZE:
        {
            ADC_Enable();
            ADC_ConversionStart();
            u8State=WAIT;
            break;
        }
            
        case WAIT:
        {
            if(!ADC_ConversionStatusGet())
            {
                u8State=WAIT;
            }
            else
            {
                u8State=PROCESS;
                
            }
            break;
        }
        case PROCESS:
        {
            /*Read the ADC Result*/
            u16adc_count=ADC_ConversionResultGet();
            u32input_voltage=u16adc_count*ADC_VREF/4095U;

            if(u32input_voltage<420)
            {
                u8CurrentPos=1;
                //newpdo[0] //5,3
            }
            else if(u32input_voltage<830)
            {
                u8CurrentPos=2;
                //newpdo[0] 5,3 9,3
            }
             else if(u32input_voltage<1250)
            {
                u8CurrentPos=3;
                //newpdo[0]
                //newpdo[1]] 5,3 15,3
            }
             else if(u32input_voltage<1660)
            {
                u8CurrentPos=4;
                 //newpdo[0] - default
                //newpdo[1]]
            }
             else if(u32input_voltage<2080)
            {
                u8CurrentPos=5;
            }
             else if(u32input_voltage<2500)
            {
                u8CurrentPos=6;
            }
            else if(u32input_voltage==0)
            {
                u8CurrentPos=7;   
                 
            }
            if(u8CurrentPos!=u8PrevPos)
            {
                u8PrevPos=u8CurrentPos;
                /*modify the pdo list as per the new position*/
                
            }
            
            VConvert=HextoAscii(u32input_voltage,sizeof(u32input_voltage));
            
            PCTWrite(StrPrint,(UINT8*)&VConvert[0],sizeof(VConvert),sizeof(StrPrint));
            //SERCOM1_USART_Write(&A[0],sizeof(A));
            u8State=INITIALIZE;
            break;
        }
    }
}
