/*******************************************************************************
  Main Source File

  Company:
    Microchip Technology Inc.

  File Name:
    main.c

  Summary:
    This file contains the "main" function for a project.

  Description:
    This file contains the "main" function for a project.  The
    "main" function calls the "SYS_Initialize" function to initialize the state
    machines of all modules in the system
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
#include <stdlib.h>                     // Defines EXIT_FAILURE
#include "definitions.h"                // SYS function prototypes
#include "psf_stdinc.h"                 // PSF include file
#include "psf_control_terminal.h"
#include "psf_adc.h"
// *****************************************************************************
// *****************************************************************************
// Section: Main Entry Point
// *****************************************************************************
// *****************************************************************************

int main ( void )
{
   
    /* Initialize all modules */
    (void)SYS_Initialize ( NULL );
    
    /*EIC will enable later as after UPD350 initialization*/
    EIC_REGS->EIC_INTENCLR = 0x4000;
    /*PSF init called*/
	(void)MchpPSF_Init();
    
    /*UART should be initialized after PSF init since UART line is connected 
     * GPIO4 of UPD350 where it is the TEST pin for it*/
     /*PCT is using UART and it is available in the default firmware*/
    (void)MchpPSF_PCTInit();
    
#ifdef WAIT_FOR_PCT_CONFIGURATON
    /*Always monitor SPACE BAR for Enter or exit PCT*/
    (void)PSF_monitorandwait();
#endif
        
    while (TRUE)
    {
        /* Maintain state machines of all polled MPLAB Harmony modules. */
        (void)SYS_Tasks ( );
        /*ADC Run called*/
        (void)PSF_ADCRun();
        /*PCT  monitors during the debug trace*/
        (void)MchpPSF_PCTRUN(ePCT_UNLOCK);
        /*PSF stack Run*/
        (void)MchpPSF_RUN();        
    }
}


/*******************************************************************************
 End of File
*/

