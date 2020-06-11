/*******************************************************************************
 UPD350 PIO Configuration Source File

 Company:
   Microchip Technology Inc.

 File Name:
   upd350_pio_drivers.c

 Description:
   This file contains the driver functions for UPD350 PIO Configuration.
*******************************************************************************/
/*******************************************************************************
Copyright ©  [2020] Microchip Technology Inc. and its subsidiaries.

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

void UPDPIO_EnableOutput(UINT8 u8PortNum, eUPD_PIO_NUM_TYPE ePIONum)
{
    UPD_RegByteSetBit (u8PortNum, UPD_CFG_PIO_REGADDR((UINT8)ePIONum), 
                    (UPD_CFG_PIO_GPIO_ENABLE | UPD_CFG_PIO_DIRECTION));
}

void UPDPIO_EnableInput(UINT8 u8PortNum, eUPD_PIO_NUM_TYPE ePIONum)
{
    /* Enable the GPIO functionality */
    UPD_RegByteSetBit (u8PortNum, UPD_CFG_PIO_REGADDR((UINT8)ePIONum), UPD_CFG_PIO_GPIO_ENABLE);
    
    /* Set direction as input */
    UPD_RegByteClearBit (u8PortNum, UPD_CFG_PIO_REGADDR((UINT8)ePIONum), UPD_CFG_PIO_DIRECTION);
}

void UPDPIO_Disable(UINT8 u8PortNum, eUPD_PIO_NUM_TYPE ePIONum)
{
    UPD_RegByteClearBit (u8PortNum, UPD_CFG_PIO_REGADDR((UINT8)ePIONum), UPD_CFG_PIO_GPIO_ENABLE);
}

void UPDPIO_SetBufferType(UINT8 u8PortNum, eUPD_PIO_NUM_TYPE ePIONum, UINT8 u8BufType)
{
    if(UPD_PIO_SETBUF_PUSHPULL == u8BufType)
    {
        UPD_RegByteSetBit (u8PortNum, UPD_CFG_PIO_REGADDR((UINT8)ePIONum), UPD_CFG_PIO_BUFFER_TYPE);
    } 
    else
    {
        UPD_RegByteClearBit (u8PortNum, UPD_CFG_PIO_REGADDR((UINT8)ePIONum), UPD_CFG_PIO_BUFFER_TYPE);
    } 
}

void UPDPIO_DriveHigh(UINT8 u8PortNum, eUPD_PIO_NUM_TYPE ePIONum)
{
    UPD_RegByteSetBit (u8PortNum, UPD_CFG_PIO_REGADDR((UINT8)ePIONum), UPD_CFG_PIO_DATAOUTPUT);
}

void UPDPIO_DriveLow(UINT8 u8PortNum, eUPD_PIO_NUM_TYPE ePIONum)
{
    UPD_RegByteClearBit (u8PortNum, UPD_CFG_PIO_REGADDR((UINT8)ePIONum), UPD_CFG_PIO_DATAOUTPUT);
}

void UPDPIO_EnablePullUp(UINT8 u8PortNum, eUPD_PIO_NUM_TYPE ePIONum, UINT8 u8PUEnable)
{
    if (UPD_PIO_ENABLE_PULLUP == u8PUEnable)
    {
        UPD_RegByteSetBit (u8PortNum, UPD_CFG_PIO_REGADDR((UINT8)ePIONum), UPD_CFG_PIO_PULL_UP_ENABLE);
    }
    else
    {
        UPD_RegByteClearBit (u8PortNum, UPD_CFG_PIO_REGADDR((UINT8)ePIONum), UPD_CFG_PIO_PULL_UP_ENABLE);
    }    
}

void UPDPIO_EnablePullDown(UINT8 u8PortNum, eUPD_PIO_NUM_TYPE ePIONum, UINT8 u8PDEnable)
{
    if (UPD_PIO_ENABLE_PULLDOWN == u8PDEnable)
    {
        UPD_RegByteSetBit (u8PortNum, UPD_CFG_PIO_REGADDR((UINT8)ePIONum), UPD_CFG_PIO_PULL_DOWN_ENABLE);
    }
    else
    {
        UPD_RegByteClearBit (u8PortNum, UPD_CFG_PIO_REGADDR((UINT8)ePIONum), UPD_CFG_PIO_PULL_DOWN_ENABLE);
    }     
}