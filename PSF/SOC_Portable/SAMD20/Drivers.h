/*******************************************************************************
  SAMD20 HAL Drivers porting Header File

  Company:
    Microchip Technology Inc.

  File Name:
    Drivers.h

  Description:
    This file contains provides the HAL drivers required for PSF Porting and integration. 
 *******************************************************************************/

// DOM-IGNORE-BEGIN
/*******************************************************************************
* Copyright (C) 2019-2020 Microchip Technology Inc. and its subsidiaries.
*
* Subject to your compliance with these terms, you may use Microchip software
* and any derivatives exclusively with Microchip products. It is your
* responsibility to comply with third party license terms applicable to your
* use of third party software (including open source software) that may
* accompany Microchip software.
*
* THIS SOFTWARE IS SUPPLIED BY MICROCHIP "AS IS". NO WARRANTIES, WHETHER
* EXPRESS, IMPLIED OR STATUTORY, APPLY TO THIS SOFTWARE, INCLUDING ANY IMPLIED
* WARRANTIES OF NON-INFRINGEMENT, MERCHANTABILITY, AND FITNESS FOR A
* PARTICULAR PURPOSE.
*
* IN NO EVENT WILL MICROCHIP BE LIABLE FOR ANY INDIRECT, SPECIAL, PUNITIVE,
* INCIDENTAL OR CONSEQUENTIAL LOSS, DAMAGE, COST OR EXPENSE OF ANY KIND
* WHATSOEVER RELATED TO THE SOFTWARE, HOWEVER CAUSED, EVEN IF MICROCHIP HAS
* BEEN ADVISED OF THE POSSIBILITY OR THE DAMAGES ARE FORESEEABLE. TO THE
* FULLEST EXTENT ALLOWED BY LAW, MICROCHIP'S TOTAL LIABILITY ON ALL CLAIMS IN
* ANY WAY RELATED TO THIS SOFTWARE WILL NOT EXCEED THE AMOUNT OF FEES, IF ANY,
* THAT YOU HAVE PAID DIRECTLY TO MICROCHIP FOR THIS SOFTWARE.
*******************************************************************************/
// DOM-IGNORE-END

#ifndef _DRIVERS_H    /* Guard against multiple inclusion */
#define _DRIVERS_H

/* ************************************************************************** */
/* ************************************************************************** */
/* Section: Included Files                                                    */
/* ************************************************************************** */
/* ************************************************************************** */
#include <stdbool.h>
#include <stddef.h>

#include <generic_defs.h>
        
#include "PSF_Config.h"

#if (CONFIG_DCDC_CTRL == I2C_DC_DC_CONTROL_CONFIG) 
//#if (CONFIG_I2C_DCDC_TYPE == 1)
    #include "Mpq_dc_dc_control.h"
//#endif
#endif
#include "../../firmware/src/config/default/peripheral/tc/plib_tc0.h"
#include "../../firmware/src/config/default/peripheral/sercom/spim/plib_sercom0_spi.h"
#include "../../firmware/src/config/default/peripheral/sercom/usart/plib_sercom3_usart.h"
#if (CONFIG_DCDC_CTRL == I2C_DC_DC_CONTROL_CONFIG)
#include "../../firmware/src/config/default/peripheral/sercom/i2cm/plib_sercom3_i2c.h"
#endif
#include "../../firmware/src/config/default/peripheral/port/plib_port.h"
#include "../../firmware/src/config/default/peripheral/eic/plib_eic.h"
#if (TRUE == INCLUDE_PD_SINK)
#include "../../firmware/src/config/default/peripheral/dac/plib_dac.h"
#endif
// *****************************************************************************
// *****************************************************************************
// Section: SAMD20 Configuration
// *****************************************************************************
// *****************************************************************************

/* SAMD20 Hardware configuration:
 * 
 * All the SAMD20 Hardware drivers are generated via Harmony configurator 3
 * 
 * The harmony configuration file is available at path
 * ../../firmware/src/config/default/SAMD20_PSFHarmonyConfiguration.xml
 *
 * Hardware Interface configuration 
 * -------------------------------
 *  - SERCOM0 is configured as SPI Master of frequency 8MHz
 *  - DO/MOSI on PAD[3] (PA11), SCK/CLK on PAD[1] (PA09), DI/MISO on PAD[0] (PA08)
 *  - PORT_PIN_PA10 & PORT_PIN_PA01 are configured as GPIO in Output mode and driven high by default.
 *  - PORT_PIN_PA10 is renamed as SPI_SS_0_PIN  in Harmony GUI and used as SPI Chip Select for Port 0
 *  - PORT_PIN_PA01 is renamed as SPI_SS_1_PIN in Harmony GUI and used as SPI Chip Select for Port 1
 *  - SPI drivers are generated in polling mode as SPI read and write API are called in critical section
 *  - SPI drivers for different SERCOM can be configured by changing SAMD20_SPI_INSTANCE in the file if 
 *      and only all the required SPI drivers are generated using Harmony for that instance.
 *  - SERCOM0_SPI_Initialize() and SERCOM0_SPI_WriteRead() drivers are used for this example
 *
 * 
 * Timer Configuration  
 * -------------------
 * - TC0 timer instance configured for 1ms for timer interrupt frequency
 * - Different timer instance can be used  by changing SAMD20_TIMER_INSTANCE in the file if and only
 *      all the required timer drivers are generated using Harmony for that instance.
 * -TC0_TimerInitialize(), TC0_TimerStart(), and TC0_TimerCallbackRegister() driver APIs are used.
 * 
 * 
 * UPD350 Alert interrupt configuration
 * ------------------------------------
 *  - PA14 and PA15 are configured for UPD350 Alert lines for Port0 and Port1 respectively
 *  - They are configured as active low detection pins with wake up capable.
 *  - The PIO can be changed for this port by changing SAMD20_PORT0_EIC_PIN, SAMD20_PORT1_EIC_PIN d
 *      define in this file with required PIO's EIC enum definition provided pin is configured 
 *      for EIC functionality as active low and wake up capable signal in Harmony GUI.
 * 
 * UPD350 Reset Configuration
 * --------------------------
 *  - PA00 is configured as UPD350_Reset pin. By default, it is configured as input pin with internal
 *     pull up. 
 *  - PORT_PIN_PA00 renamed as UPD350_RESET_PIN in GUI is used.
 *  - If the user like to configure a different pin as UPD350_Reset pin, Rename it as UPD350_RESET_PIN
 *      and input with internal pullup by default.
 */

#define SAMD20_TIMER_INSTANCE   0
#define SAMD20_SPI_INSTANCE     0
#define SAMD20_PORT0_EIC_PIN    EIC_PIN_14
#define SAMD20_PORT1_EIC_PIN    EIC_PIN_15

#if (CONFIG_DCDC_CTRL == I2C_DC_DC_CONTROL_CONFIG)
#define SAMD20_DCDC_ALERT0      PORT_PIN_PA02
#define SAMD20_DCDC_ALERT1      PORT_PIN_PA03
#define SAMD20_I2C_INSTANCE     3
#endif //#if (CONFIG_DCDC_CTRL == I2C_DC_DC_CONTROL_CONFIG)

#define SAMD20_UART_INSTANCE  3

// *****************************************************************************
// *****************************************************************************
// Section: Interface Functions
// *****************************************************************************
// *****************************************************************************
/****************************************************************************
    Function:
        UINT8 SAMD20_HWTimerInit(void)
    Summary:
        Wrapper function to initialize SAMD20 Hardware Timer  
    Description:
        This API serves as a wrapper between PSF stack defined Timer initialization function 
        MCHP_PSF_HOOK_HW_PDTIMER_INIT and Harmony generated timer drivers. It shall initialize and 
        start the timer.
    Conditions:
        None
    Input:
        None
    Return:
        UINT8 - Returns TRUE - If the Hardware timer initialization is successful.
            Returns FALSE - If the Hardware timer initialization is not successful.
    Remarks:
        None
**************************************************************************************************/
UINT8 SAMD20_HWTimerInit(void);

/****************************************************************************
    Function:
        UINT8 SAMD20_SPIInitialisation(void)
    Summary:
        Wrapper function to initialize SAMD20 Hardware Interface for UPD350 communication  
    Description:
        This API serves as a wrapper between PSF stack defined Hardware interface initialization 
        function MCHP_PSF_HOOK_UPDHW_INTF_INIT and Harmony generated SPI drivers. It shall initialize 
        the Hardware interface for communication. In this case, it is SPI.
    Conditions:
        None
    Input:
        None
    Return:
        UINT8 - Returns TRUE - If the SPI initialization is successful.
                Returns FALSE - If the SPI initialization is not successful.
    Remarks:
        None
**************************************************************************************************/
UINT8 SAMD20_SPIInitialisation(void);

/****************************************************************************
    Function:
        void SAMD20_DriveChipSelect(UINT8 u8PortNum, UINT8 u8EnableComm)
    Summary:
        Wrapper function to enable or disable the SAMD20 HW interface communication with Port's UPD350.  
    Description:
        This API serves as a wrapper between PSF stack defined UPD350 communication enable disable 
        function MCHP_PSF_HOOK_UPD_COMM_ENABLE and Harmony generated code to drive Chip select to 
        enable or disable SPI communication to specific port's UPD350.
    Conditions:
        None
    Input:
        u8PortNum - Port number. Value passed will be less than CONFIG_PD_PORT_COUNT
        u8EnableComm- Enables or disables the SPI communication for the port based on this value 
                        TRUE and FALSE respectively.
    Return:
        None
    Remarks:
        None
**************************************************************************************************/
void SAMD20_DriveChipSelect(UINT8 u8PortNum, UINT8 u8EnableComm);

/****************************************************************************
    Function:
        UINT8 SAMD20_SPIReaddriver (UINT8 u8PortNum, UINT8 *pu8WriteBuffer, UINT8 u8Writelength,\
                UINT8 *pu8ReadBuffer, UINT8 u8Readlength)
    Summary:
        Wrapper function for SPI read drivers.  
    Description:
        This API serves as a wrapper between PSF stack defined UPD350 read driver 
        MCHP_PSF_HOOK_UPD_READ and Harmony generated SAMD20 SPI read driver.
    Conditions:
        None
    Input:
        u8PortNum - Port number. Value passed will be less than CONFIG_PD_PORT_COUNT
        pu8WriteBuffer- Pointer to Write Buffer to be written first before read take place 
        u8Writelength - Write Buffer length to written on SPI bus
        pu8ReadBuffer - Pointer to Read buffer
        u8Readlength - Length of bytes to read
    Return:
        UINT8 - Returns TRUE - If the SPI read is successful.
                Returns FALSE - If the SPI read is not successful
    Remarks:
        None
**************************************************************************************************/

UINT8 SAMD20_SPIReaddriver (UINT8 u8PortNum, UINT8 *pu8WriteBuffer, UINT8 u8Writelength,\
                UINT8 *pu8ReadBuffer, UINT8 u8Readlength);
/****************************************************************************
    Function:
        UINT8 SAMD20_SPIWritedriver (UINT8 u8PortNum, UINT8 *pu8WriteBuffer, UINT8 u8Writelength)
    Summary:
        Wrapper function for SPI write drivers.  
    Description:
        This API serves as a wrapper between PSF stack defined UPD350 write driver 
        MCHP_PSF_HOOK_UPD_WRITEand Harmony generated SAMD20 SPI write driver.
    Conditions:
        None
    Input:
        u8PortNum - Port number. Value passed will be less than CONFIG_PD_PORT_COUNT
        pu8WriteBuffer- Pointer to Write Buffer to be written 
        u8Writelength - Write Buffer length to written on SPI bus
    Return:
        UINT8 - Returns TRUE - If the SPI write is successful.
                Returns FALSE - If the SPI write is not successful
    Remarks:
        None
**************************************************************************************************/
UINT8 SAMD20_SPIWritedriver (UINT8 u8PortNum, UINT8 *pu8WriteBuffer, UINT8 u8Writelength);

/*****************************************************************************/
#if (CONFIG_DCDC_CTRL == I2C_DC_DC_CONTROL_CONFIG)
UINT8 SAMD20_I2CDCDCInitialisation (void);
UINT8 SAMD20_I2CDCDCReadDriver (UINT16 u16Address,UINT8 *pu8ReadBuf,UINT8 u8ReadLen);
UINT8 SAMD20_I2CDCDCWriteDriver(UINT16 u16Address,UINT8 *pu8WriteBuf,UINT8 u8WriteLen);
UINT8 SAMD20_I2CDCDCWriteReadDriver(UINT16 u16Address,UINT8 *pu8WriteBuf,UINT8 u8WriteLen,\
                                              UINT8 *pu8ReadBuf,UINT8 u8ReadLen);
bool SAMD20_I2CDCDCIsBusyDriver(void);
void SAMD20_I2CDCDCAlertInit(UINT8 u8PortNum);

#endif
/****************************************************************************
    Function:
        void SAMD20_UPD350AlertInit(UINT8 u8PortNum)
    Summary:
        Wrapper function to initialize UPD350 Alert configured GPIOs  
    Description:
        This API serves as a wrapper between PSF stack defined Alert initialization function 
        MCHP_PSF_HOOK_UPD_IRQ_GPIO_INIT and Harmony generated generated code to initialize the GPIO for
        UPD350 Alert functionality port specifically.
    Conditions:
        None
    Input:
        u8PortNum - Port number. Value passed will be less than CONFIG_PD_PORT_COUNT
    Return:
        None
    Remarks:
        None
**************************************************************************************************/
void SAMD20_UPD350AlertInit(UINT8 u8PortNum);

/****************************************************************************
    Function:
        void SAMD20_UPD350ResetGPIOInit(UINT8 u8PortNum)
    Summary:
        Wrapper function Initialize the GPIO mapped as UPD350 
    Description:
        This API serves as a wrapper between PSF stack defined UPD350 Reset initialization function
        MCHP_PSF_HOOK_UPD_RESET_GPIO_INIT and Harmony generated code to initialize the GPIO for
        UPD350 Reset functionality
    Conditions:
        None
    Input:
        u8PortNum - Port number. Value passed will be less than CONFIG_PD_PORT_COUNT
    Return:
        None
    Remarks:
        None
**************************************************************************************************/
void SAMD20_UPD350ResetGPIOInit(UINT8 u8PortNum);

/****************************************************************************
    Function:
        void SAMD20_ResetUPD350(UINT8 u8PortNum)
    Summary:
        Wrapper function reset the UPD350 through GPIO configured 
    Description:
        This API serves as a wrapper between PSF stack defined UPD350 Reset function
        MCHP_PSF_HOOK_UPD_RESET_THRU_GPIO and Harmony generated code to drive the UPD350 Reset 
        configured GPIO port specifically.
    Conditions:
        None
    Input:
        u8PortNum - Port number. Value passed will be less than CONFIG_PD_PORT_COUNT
    Return:
        None
    Remarks:
        None
**************************************************************************************************/
void SAMD20_ResetUPD350(UINT8 u8PortNum);

/****************************************************************************
    Function:
        void SAMD20_EnterCriticalSection(void)
    Summary:
        Wrapper function to SAMD20 disable interrupts globally to provide critical section
    Description:
        This API serves as a wrapper between PSF stack defined SOC interrupt disable function
        MCHP_PSF_HOOK_DISABLE_GLOBAL_INTERRUPT and SAMD20 global interrupt disable option.
    Conditions:
        None
    Input:
        None
    Return:
        None
    Remarks:
        None
**************************************************************************************************/
void SAMD20_EnterCriticalSection(void);

/****************************************************************************
    Function:
        void SAMD20_EnterCriticalSection(void)
    Summary:
        Wrapper function to SAMD20 enable interrupts globally to provide critical section
    Description:
        This API serves as a wrapper between PSF stack defined SOC interrupt enable function
        MCHP_PSF_HOOK_ENABLE_GLOBAL_INTERRUPT and SAMD20 global interrupt enable option.
    Conditions:
        None
    Input:
        None
    Return:
        None
    Remarks:
        None
**************************************************************************************************/
void SAMD20_ExitCriticalSection(void);

/**************************************************************************
    Function:
        void* SAMD20_MemCpy(void *dest, const void *src, int n)
    Summary:
        Wrapper to Copy one memory area to another memory area
    Description:
        It is a wrapper for PSF stack's MCHP_PSF_HOOK_MEMCPY
    Conditions:
        None.
    Input:
        dest -  pointer to block of destination memory region
        src -   pointer to block of source memory region
        n -   number of bytes to be copied.
    Return:
        None.
    Remarks:
        None                    
 **************************************************************************/
void* SAMD20_MemCpy(void *pdest, const void *psrc, int ilen);

/**************************************************************************
    Function:
        int SAMD20_MemCmp(const void *pau8Data1, const void *pau8Data2, int ilen)
    Summary:
        Wrapper to compare two memory location
    Description:
        It is a wrapper for PSF stack's MCHP_PSF_HOOK_MEMCMP
    Conditions:
        None.
    Input:
        pau8Data1 -  pointer to block of Memory region 1
        pau8Data2 -  pointer to block of Memory region 2
        ilen -   number of bytes to be compared.
    Return:
        Returns 0 if two memory regions are same.
    Remarks:
        None                    
**************************************************************************/
int SAMD20_MemCmp(const void *pau8Data1, const void *pau8Data2, int ilen);

#if(TRUE == INCLUDE_PD_SINK)
/**************************************************************************
    Function:
        void SAMD20_ConfigureSinkHardware(UINT8 u8PortNum,UINT16 u16VBUSVoltage,UINT16 u16Current)
    Summary:
        Function to configure sink Hardware
    Description:
        It is a wrapper for PSF stack's MCHP_PSF_HOOK_PORTPWR_CONFIG_SINK_HW function
    Conditions:
        None.
    Input:
        u8PortNum -  PortNumber; Value passed will be less than CONFIG_PD_PORT_COUNT
        u16VBUSVoltage -  Voltage value in mV to which sink circuitry has to be configured
                           if required
        u16Current -   Current value in mA for which sink hardware circuitry has to be
                        configured.
    Return:
        None.
    Remarks:
        None                    
**************************************************************************/
void SAMD20_ConfigureSinkHardware(UINT8 u8PortNum,UINT16 u16VBUSVoltage,UINT16 u16Current);

/**************************************************************************
    Function:
        void SAMD20_Drive_DAC_I(UINT16 u16DACData)
    Summary:
        Wrapper to initialize DAC from SoC.
    Description:
        This API serves as a wrapper between PSF stack's MCHP_PSF_HOOK_DRIVE_DAC_I()
        and Harmony generated code to output a voltage on output pin of SoC's
        Digital to Analog Converter(DAC) to indicate the implicit/explicit current
        capability of attached source partner.
    Conditions:
        None.
    Input:
        None.
    Return:
        None.
    Remarks:
        None                    
**************************************************************************/
void SAMD20_Drive_DAC_I(UINT16 u16DACData);

#endif /*INCLUDE_PD_SINK*/



/*Debug UART APIs*/
#if (TRUE == CONFIG_HOOK_DEBUG_MSG)
void SAMD20_UART_Initialisation(void);

void SAMD20_UART_Write_Char(char);

void SAMD20_UART_Write_Int(UINT32, UINT8);

void SAMD20_UART_Write_String(char*);

#endif //CONFIG_HOOK_DEBUG_MSG


#endif /*_DRIVERS_H */

/* *****************************************************************************
 End of File
 */
