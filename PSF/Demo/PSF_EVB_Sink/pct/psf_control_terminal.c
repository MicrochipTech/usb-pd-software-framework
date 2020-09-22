/*******************************************************************************
 User Interface File

  Company:
    Microchip Technology Inc.

  File Name:
    psf_control_terminal.c

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
#include "definitions.h"                // SYS function prototypes
#include "psf_stdinc.h"                 // PSF include file
#include "psf_control_terminal.h"
#include "ProjectVersion.h"
#include "PSF_Config.h"
#include "PSFSink_BootCfg.h"
#include "string.h"
#define LENGTH 40

extern GLOBAL_CFG_STATUS_DATA gasCfgStatusData; 

/*
Function PCTReadByte 

This function converts Uppercase alphabets to Lowercase if given by the user as per point number 7 of 2.10 in the design document 

RETURNS the ReadByte from the terminal
*/
int PCTReadByte(void)
{
    UINT8 u8ReadByte;
    u8ReadByte=SERCOM1_USART_ReadByte();
    
    if(u8ReadByte>0x40&&u8ReadByte<0x5B)
    {
        /*Convert Uppercase to Lowercase Alphabet*/
        u8ReadByte=u8ReadByte+0x20;         
    }
    SERCOM1_USART_WriteByte(u8ReadByte);
    return (u8ReadByte);
}

/* Function writes when '?' is the input */
void PCTPrintCommands()
{   
    UINT8 StrPrint[]="?\n\n\r> get * - Displays the list of all the parameters and their current values\n\n\r> exit - Exit from the PSF Control Terminal and continue to run the sample application\n\n\r> "
            "get version - Gets the current version\n\n\r> get br - Gets the existing baud rate\n\n\r> get pdo - Gets the sink power details\n\n\r> "
            "get sr[GlobalCfgStatusData] - Gets the config status data\n\n\r> get sr[gasCfgStatusData] - Gets the gas config status\n\n\r> get sr[PortCfgStatus] - Gets the port Config Status\n\n\r> "
            "get mem[memory_address][length] -  Reads the memory at the given location\n\n\r> set br[value] - Sets the baud rate to the given value\n\n\r> set mem[memory_address][byte value] - Writes memory to SAMD20\n\n\r> "
            "set pdo[position][value] -  Insert a new PDO at the specified location\n\n\r";
    SERCOM1_USART_Write(&StrPrint[0],sizeof(StrPrint));
  
}

/* 
Function: PCTWrite 

This function writes names of variables and their respective values according to their size.

*/
void PCTWrite(UINT8 u8String[],UINT8 u8Value[],UINT8 u8Size,UINT8 u8StrSize)
{
    UINT8 i=(u8Size-1);
    SERCOM1_USART_Write(&u8String[0],u8StrSize);
    while(u8Size --)
    {
        SERCOM1_USART_WriteByte(u8Value[i]);
        i--;
    }
}

char* HextoAscii(UINT32 u32HexVal,UINT8 u8Length)
{
    UINT8 i=0,j=0;
    UINT8 u8Digit=0,u8Value=0;
    static char AsciiValue[10];
    while(i<=u8Length)
    {
        /*Gets the 1st digit*/
        u8Digit=u32HexVal&0x0F;
        if (u8Digit>=0x00 && u8Digit<=0x09) 
        {
            /*conversion to ascii*/
            u8Value=u8Digit+0x30;
            AsciiValue[j]=u8Value;
            j++;
        } 
  
        else if (u8Digit>=0x0A && u8Digit<=0x0F) 
        { 
            /*conversion to ascii*/
            u8Value=u8Digit+0x57;
            AsciiValue[j]=u8Value;
            j++;     
        }
        u32HexVal=u32HexVal>>0x04;
        i++;
    }
    return AsciiValue;
}

/* 
Function: PCTPrintBaudRate 

This function writes BaudRate of the firmware onto the terminal.

*/
void PCTPrintBaudRate(UINT8 u8array[],UINT8 u8Length)
{   
    UINT32 u32BaudRate;
    
    char *u8BaudRateConvert;
    
    UINT8 u8StrBaud[]="> Baud Rate=";   
    
    /*Gets the BaudRate from firmware*/
    u32BaudRate=SERCOM1_USART_ReturnBaud();
   
    u8BaudRateConvert=HextoAscii(u32BaudRate,sizeof(u32BaudRate));
  
    PCTWrite(u8StrBaud,(UINT8*)&u8BaudRateConvert[0],sizeof(u8BaudRateConvert),sizeof(u8StrBaud));
    
}
/* 
Function: PCTPrintBaudRate 

This function writes Demo version and firmware version onto the terminal.

*/
void PCTPrintVersion(UINT8 u8array[],UINT8 u8Length)
{
    UINT32 u32FirmwareVersion,u32DemoVersion;
    
    char *u8DemoVersionConvert,*u32FirmwareVersionConvert;
    
    UINT8 u8StrDemoVersion[]="> demo version=";
    
    UINT8 u8StrPSFVersion[]="\n\n\r> PSF version=" ;  
    
    u32DemoVersion=DEMO_VERSION;
    
    u32FirmwareVersion=SYSTEM_FW_REV;
    
    u8DemoVersionConvert=HextoAscii(u32DemoVersion,sizeof(u32DemoVersion));
    
    PCTWrite(u8StrDemoVersion,(UINT8*)&u8DemoVersionConvert[0],sizeof(u8DemoVersionConvert),sizeof(u8StrDemoVersion));
     
    u32FirmwareVersionConvert=HextoAscii(u32FirmwareVersion,sizeof(u32FirmwareVersion));
    
    PCTWrite(u8StrPSFVersion,(UINT8*)&u32FirmwareVersionConvert[0],sizeof(u32FirmwareVersionConvert),sizeof(u8StrPSFVersion));
    
}
/* 
Function: PCTPrintPDO 

This function writes PDO values onto the terminal.

*/
void PCTPrintPDO(UINT8 u8array[],UINT8 u8Length)
{
    char *pu8PrintString;
    UINT32 *pu32PDOList;
    UINT32 u32Data;
    UINT32 u8Index;
    UINT8 u8aStrVoltage1[]="\rVoltage=";
    UINT8 u8aStrCurrent[]="\rCurrent=";
    
    //If it is already advertised then get it from advertised list or default
    if(gasCfgStatusData.sPerPortData[PORT0].u8AdvertisedPDOCnt)
    {
        pu32PDOList = gasCfgStatusData.sPerPortData[PORT0].u32aAdvertisedPDO;
    }else
    {
         pu32PDOList = gasCfgStatusData.sPerPortData[PORT0].u32aSinkPDO;
    }
    
    for(u8Index= SET_TO_ZERO; u8Index <DPM_MAX_PDO_CNT; u8Index++)
    {
        pu8PrintString=HextoAscii(u8Index,sizeof(u8Index));
        PCTWrite((UINT8*)"\n\r PDO -", (UINT8*)pu8PrintString, sizeof(pu8PrintString), 11);
        
        //Get the voltage form PDO list
        u32Data=DPM_GET_VOLTAGE_FROM_PDO_MILLI_V(pu32PDOList[u8Index]);
        pu8PrintString=HextoAscii(u32Data,sizeof(u32Data));
        PCTWrite(u8aStrVoltage1,(UINT8*)pu8PrintString,sizeof(pu8PrintString),sizeof(u8aStrVoltage1));
        
        //Get the voltage form PDO list
        u32Data=DPM_GET_PDO_CURRENT(pu32PDOList[u8Index]);
        pu8PrintString=HextoAscii(u32Data,sizeof(u32Data));
        PCTWrite(u8aStrCurrent,(UINT8*)pu8PrintString,sizeof(pu8PrintString),sizeof(u8aStrCurrent));
    }
 }

/* 
Function: PCTPrintStatusData 

This function writes GlobalConfigStatusdata and PortConfigStatusData values onto the terminal.

*/

void PCTPrintStatusData(UINT8 u8array[],UINT8 u8Length)
{
    UINT8 chGlobalHelper[]="get sr[g";
    UINT8 chPortHelper[]="get sr[p";
    if(0==memcmp(u8array,chGlobalHelper,8))    
    {
        char *u8MinorVersionConvert,*u8MajorVersionConvert,*u8HWVersionConvert,*u8SiVersionConvert,*u8PSFMajorVersionConvert,*u8PSFMinorVersionConvert;
        char *u8PwrThrottleCfgConvert,*u16ProducdIDConvert,*u16VendorIDConvert,*u16ProductTypeVDOConvert,*u16ProductVDOConvert,*u16CertStatVDOConvert,*u16IDHeaderVDOConvert;
        
        UINT8 StrMinorVersion[]=">\n\n\rUINT8 u8MinorVersion=";
        
        UINT8 StrMajorVersion[]="\n\n\rUINT8 u8MajorVersion=";
        
        UINT8 StrHWVersion[]="\n\n\rUINT8 u8HWVersion";
        
        UINT8 StrSiVersion[]="\n\n\rUINT8 u8SiVersion";
        
 //       UINT8 StrPrint5[]="UINT8 u8aManfString[8]";
        UINT8 StrPSFMajorVersion[]="\n\n\rUINT8 u8PSFMajorVersion";
        
        UINT8 StrPSFMinorVersion[]="\n\n\rUINT8 u8PSFMinorVersion";
        
        UINT8 StrPwrThrottleCfg[]="\n\n\rUINT8 u8PwrThrottleCfg";
//        UINT8 StrPrint9[]="UINT8 u8aReserved3";
        UINT8 StrProducdID[]="\n\n\rUINT16 u16ProducdID";
        
        UINT8 StrVendorID[]="\n\n\rUINT16 u16VendorID";
        
        UINT8 StrProductTypeVDO[]="\n\n\rUINT16 u16ProductTypeVDO";
        
        UINT8 StrProductVDO[]="\n\n\rUINT16 u16ProductVDO";
        
        UINT8 StrCertStatVDO[]="\n\n\rUINT16 u16CertStatVDO";
        
        UINT8 StrIDHeaderVDO[]="\n\n\rUINT16 u16IDHeaderVDO";
        
      
        u8MinorVersionConvert=HextoAscii(gasCfgStatusData.u8MinorVersion,4);
        
        PCTWrite(StrMinorVersion,(UINT8*)&u8MinorVersionConvert[0],sizeof(u8MinorVersionConvert),sizeof(StrMinorVersion));
      
        
        u8MajorVersionConvert=HextoAscii(gasCfgStatusData.u8MajorVersion,4);
        
        PCTWrite(StrMajorVersion,(UINT8*)&u8MajorVersionConvert[0],sizeof(u8MajorVersionConvert),sizeof(StrMajorVersion));
        
        u8HWVersionConvert=HextoAscii(gasCfgStatusData.u8HWVersion,4);
        
        PCTWrite(StrHWVersion,(UINT8*)&u8HWVersionConvert[0],sizeof(u8HWVersionConvert),sizeof(StrHWVersion));
        
        u8SiVersionConvert=HextoAscii(gasCfgStatusData.u8SiVersion,4);
        
        PCTWrite(StrSiVersion,(UINT8*)&u8SiVersionConvert[0],sizeof(u8SiVersionConvert),sizeof(StrSiVersion));
       // gasCfgStatusData.u8aManfString[8];
        
        u8PSFMajorVersionConvert=HextoAscii(gasCfgStatusData.u8PSFMajorVersion,4);
        
        PCTWrite(StrPSFMajorVersion,(UINT8*)&u8PSFMajorVersionConvert[0],sizeof(u8PSFMajorVersionConvert),sizeof(StrPSFMajorVersion));
        
        u8PSFMinorVersionConvert=HextoAscii(gasCfgStatusData.u8PSFMinorVersion,4);
        
        PCTWrite(StrPSFMinorVersion,(UINT8*)&u8PSFMinorVersionConvert[0],sizeof(u8PSFMinorVersionConvert),sizeof(StrPSFMinorVersion));
        
        u8PwrThrottleCfgConvert=HextoAscii(gasCfgStatusData.u8PwrThrottleCfg,4);
        
        PCTWrite(StrPwrThrottleCfg,(UINT8*)&u8PwrThrottleCfgConvert[0],sizeof(u8PwrThrottleCfgConvert),sizeof(StrPwrThrottleCfg));
      //  gasCfgStatusData.u8aReserved3;
        
        u16ProducdIDConvert=HextoAscii(gasCfgStatusData.u16ProducdID,4);
        
        PCTWrite(StrProducdID,(UINT8*)&u16ProducdIDConvert[0],sizeof(u16ProducdIDConvert),sizeof(StrProducdID));
        
        u16VendorIDConvert=HextoAscii(gasCfgStatusData.u16VendorID,4);
        
        PCTWrite(StrVendorID,(UINT8*)&u16VendorIDConvert[0],sizeof(u16VendorIDConvert),sizeof(StrVendorID));
        
        u16ProductTypeVDOConvert=HextoAscii(gasCfgStatusData.u16ProductTypeVDO,4);
        
        PCTWrite(StrProductTypeVDO,(UINT8*)&u16ProductTypeVDOConvert[0],sizeof(u16ProductTypeVDOConvert),sizeof(StrProductTypeVDO));
        
        u16ProductVDOConvert=HextoAscii(gasCfgStatusData.u16ProductVDO,4);
        
        PCTWrite(StrProductVDO,(UINT8*)&u16ProductVDOConvert[0],sizeof(u16ProductVDOConvert),sizeof(StrProductVDO));
        
        u16CertStatVDOConvert=HextoAscii(gasCfgStatusData.u16CertStatVDO,4);
        
        PCTWrite(StrCertStatVDO,(UINT8*)&u16CertStatVDOConvert[0],sizeof(u16CertStatVDOConvert),sizeof(StrCertStatVDO));
        
        u16IDHeaderVDOConvert=HextoAscii(gasCfgStatusData.u16IDHeaderVDO,4);
        
        PCTWrite(StrIDHeaderVDO,(UINT8*)&u16IDHeaderVDOConvert[0],sizeof(u16IDHeaderVDOConvert),sizeof(StrIDHeaderVDO));
        
    }
    else if(0==memcmp(u8array,chPortHelper,8))
    {
        char *u32CfgDataConvert,*u16PowerGoodTimerInmsConvert,*u16SnkMaxOperatingCurInmAConvert,*u16SnkMinOperatingCurInmAConvert;
        char *u16DAC_I_CurrentInd_MaxInAConvert,*u16DAC_I_MinOutVoltInmVConvert,*u16DAC_I_MaxOutVoltInmVConvert;
        char *u8OVThresholdPercentageConvert,*u8FaultInDebounceInmsConvert,*u8UVThresholdPercentageConvert,*u8VCONNOCSDebounceInmsConvert,*u8VCONNMaxFaultCntConvert;
        char *u8Pio_FAULT_INConvert,*u8VBUSMaxFaultCntConvert,*u8Mode_FAULT_INConvert,*u8Pio_EN_SINKConvert,*u8Mode_EN_SINKConvert,*u8SinkPDOCntConvert,*u8SinkConfigSelConvert,*u8OCSThresholdPercentageConvert;
        char *u8DAC_I_DirectionConvert;
        char *u16aMinPDOPreferredCurInmA1Convert,*u16aMinPDOPreferredCurInmA2Convert,*u16aMinPDOPreferredCurInmA3Convert,*u16aMinPDOPreferredCurInmA4Convert,*u16aMinPDOPreferredCurInmA5Convert,*u16aMinPDOPreferredCurInmA6Convert,*u16aMinPDOPreferredCurInmA7Convert;
        
        
        UINT8 StrCfgData[]=">\n\n\rUINT32 u32CfgData=";
        
        UINT8 StrPowerGoodTimerInms[]=">\n\n\rUINT16 u16PowerGoodTimerInms=";
        
        UINT8 StraMinPDOPreferredCurInmA1[]=">\n\n\rUINT16 u16aMinPDOPreferredCurInmA1=";
        
        UINT8 StraMinPDOPreferredCurInmA2[]=">\n\n\rUINT16 u16aMinPDOPreferredCurInmA2=";
        
        UINT8 StraMinPDOPreferredCurInmA3[]=">\n\n\rUINT16 u16aMinPDOPreferredCurInmA3=";
        
        UINT8 StraMinPDOPreferredCurInmA4[]=">\n\n\rUINT16 u16aMinPDOPreferredCurInmA4=";
        
        UINT8 StraMinPDOPreferredCurInmA5[]=">\n\n\rUINT16 u16aMinPDOPreferredCurInmA5=";
        
        UINT8 StraMinPDOPreferredCurInmA6[]=">\n\n\rUINT16 u16aMinPDOPreferredCurInmA6=";
        
        UINT8 StraMinPDOPreferredCurInmA7[]=">\n\n\rUINT16 u16aMinPDOPreferredCurInmA7=";
        
        UINT8 StrSnkMaxOperatingCurInmA[]=">\n\n\rUINT16 u16SnkMaxOperatingCurInmA=";
        
        UINT8 StrSnkMinOperatingCurInmA[]=">\n\n\rUINT16 u16SnkMinOperatingCurInmA=";
        
        UINT8 StrDAC_I_MaxOutVoltInmV[]=">\n\n\rUINT16 u16DAC_I_MaxOutVoltInmV=";
        
        UINT8 StrDAC_I_MinOutVoltInmV[]=">\n\n\rUINT16 u16DAC_I_MinOutVoltInmV=";
        
        UINT8 StrDAC_I_CurrentInd_MaxInA[]=">\n\n\rUINT16 u16DAC_I_CurrentInd_MaxInA=";
        
        UINT8 StrSinkPDOCnt[]=">\n\n\rUINT8 u8SinkPDOCnt=";
        
        UINT8 StrSinkConfigSel[]=">\n\n\rUINT8 u8SinkConfigSel=";
        
        UINT8 StrFaultInDebounceInms[]=">\n\n\rUINT8 u8FaultInDebounceInms=";
        
        UINT8 StrOCSThresholdPercentage[]=">\n\n\rUINT8 u8OCSThresholdPercentage=";
        
        UINT8 StrOVThresholdPercentage[]=">\n\n\rUINT8 u8OVThresholdPercentage=";
        
        UINT8 StrUVThresholdPercentage[]=">\n\n\rUINT8 u8UVThresholdPercentage=";
        
        UINT8 StrVCONNOCSDebounceInms[]=">\n\n\rUINT8 u8VCONNOCSDebounceInms=";
        
        UINT8 StrVBUSMaxFaultCnt[]=">\n\n\rUINT8 u8VBUSMaxFaultCnt="; //set to 3 by pasCfgStatusData
        
        UINT8 StrVCONNMaxFaultCnt[]=">\n\n\rUINT8 u8VCONNMaxFaultCnt=";
        
        UINT8 StrPio_FAULT_IN[]=">\n\n\rUINT8 u8Pio_FAULT_IN=";
        
        UINT8 StrMode_FAULT_IN[]=">\n\n\rUINT8 u8Mode_FAULT_IN=";
        
        UINT8 StrPio_EN_SINK[]=">\n\n\rUINT8 u8Pio_EN_SINK=";
        
        UINT8 StrMode_EN_SINK[]=">\n\n\rUINT8 u8Mode_EN_SINK=";
        
        UINT8 StrDAC_I_Direction[]=">\n\n\rUINT8 u8DAC_I_Direction=";
        
        u32CfgDataConvert=HextoAscii(gasCfgStatusData.sPerPortData[PORT0].u32CfgData,sizeof(gasCfgStatusData.sPerPortData[PORT0].u32CfgData));
        
        PCTWrite(StrCfgData,(UINT8*)&u32CfgDataConvert[0],sizeof(u32CfgDataConvert),sizeof(StrCfgData));
        
        PCTPrintPDO(u8array,u8Length);
 
        u16PowerGoodTimerInmsConvert=HextoAscii(gasCfgStatusData.sPerPortData[PORT0].u16PowerGoodTimerInms,4);
        
         PCTWrite(StrPowerGoodTimerInms,(UINT8*)&u16PowerGoodTimerInmsConvert[0],sizeof(u16PowerGoodTimerInmsConvert),sizeof(StrPowerGoodTimerInms)); 
        
        u16aMinPDOPreferredCurInmA1Convert=HextoAscii(gasCfgStatusData.sPerPortData[PORT0].u16aMinPDOPreferredCurInmA[0],4);
        
        PCTWrite(StraMinPDOPreferredCurInmA1,(UINT8*)&u16aMinPDOPreferredCurInmA1Convert[0],sizeof(u16aMinPDOPreferredCurInmA1Convert),sizeof(StraMinPDOPreferredCurInmA1));
        
        u16aMinPDOPreferredCurInmA2Convert=HextoAscii(gasCfgStatusData.sPerPortData[PORT0].u16aMinPDOPreferredCurInmA[1],4);
        
        PCTWrite(StraMinPDOPreferredCurInmA2,(UINT8*)&u16aMinPDOPreferredCurInmA2Convert[0],sizeof(u16aMinPDOPreferredCurInmA2Convert),sizeof(StraMinPDOPreferredCurInmA2));
        
        u16aMinPDOPreferredCurInmA3Convert=HextoAscii(gasCfgStatusData.sPerPortData[PORT0].u16aMinPDOPreferredCurInmA[2],4);
        
        PCTWrite(StraMinPDOPreferredCurInmA3,(UINT8*)&u16aMinPDOPreferredCurInmA3Convert[0],sizeof(u16aMinPDOPreferredCurInmA3Convert),sizeof(StraMinPDOPreferredCurInmA3));
        
        u16aMinPDOPreferredCurInmA4Convert=HextoAscii(gasCfgStatusData.sPerPortData[PORT0].u16aMinPDOPreferredCurInmA[3],4);
        
        PCTWrite(StraMinPDOPreferredCurInmA4,(UINT8*)&u16aMinPDOPreferredCurInmA4Convert[0],sizeof(u16aMinPDOPreferredCurInmA4Convert),sizeof(StraMinPDOPreferredCurInmA4));
        
        u16aMinPDOPreferredCurInmA5Convert=HextoAscii(gasCfgStatusData.sPerPortData[PORT0].u16aMinPDOPreferredCurInmA[4],4);
        
        PCTWrite(StraMinPDOPreferredCurInmA5,(UINT8*)&u16aMinPDOPreferredCurInmA5Convert[0],sizeof(u16aMinPDOPreferredCurInmA5Convert),sizeof(StraMinPDOPreferredCurInmA5));
        
        u16aMinPDOPreferredCurInmA6Convert=HextoAscii(gasCfgStatusData.sPerPortData[PORT0].u16aMinPDOPreferredCurInmA[5],4);
        
        PCTWrite(StraMinPDOPreferredCurInmA6,(UINT8*)&u16aMinPDOPreferredCurInmA6Convert[0],sizeof(u16aMinPDOPreferredCurInmA6Convert),sizeof(StraMinPDOPreferredCurInmA6));
        
        u16aMinPDOPreferredCurInmA7Convert=HextoAscii(gasCfgStatusData.sPerPortData[PORT0].u16aMinPDOPreferredCurInmA[6],4);
        
        PCTWrite(StraMinPDOPreferredCurInmA7,(UINT8*)&u16aMinPDOPreferredCurInmA7Convert[0],sizeof(u16aMinPDOPreferredCurInmA7Convert),sizeof(StraMinPDOPreferredCurInmA7));
        
        u16SnkMaxOperatingCurInmAConvert=HextoAscii(gasCfgStatusData.sPerPortData[PORT0].u16SnkMaxOperatingCurInmA,4);
        
        PCTWrite(StrSnkMaxOperatingCurInmA,(UINT8*)&u16SnkMaxOperatingCurInmAConvert[0],sizeof(u16SnkMaxOperatingCurInmAConvert),sizeof(StrSnkMaxOperatingCurInmA));
       
        u16SnkMinOperatingCurInmAConvert=HextoAscii(gasCfgStatusData.sPerPortData[PORT0].u16SnkMinOperatingCurInmA,4);
        
        PCTWrite(StrSnkMinOperatingCurInmA,(UINT8*)&u16SnkMinOperatingCurInmAConvert[0],sizeof(u16SnkMinOperatingCurInmAConvert),sizeof(StrSnkMinOperatingCurInmA));
     
        u16DAC_I_MaxOutVoltInmVConvert=HextoAscii(gasCfgStatusData.sPerPortData[PORT0].u16DAC_I_MaxOutVoltInmV,4);
        
        PCTWrite(StrDAC_I_MaxOutVoltInmV,(UINT8*)&u16DAC_I_MaxOutVoltInmVConvert[0],sizeof(u16DAC_I_MaxOutVoltInmVConvert),sizeof(StrDAC_I_MaxOutVoltInmV));
  
        u16DAC_I_MinOutVoltInmVConvert=HextoAscii(gasCfgStatusData.sPerPortData[PORT0].u16DAC_I_MinOutVoltInmV,4);
        
        PCTWrite(StrDAC_I_MinOutVoltInmV,(UINT8*)&u16DAC_I_MinOutVoltInmVConvert[0],sizeof(u16DAC_I_MinOutVoltInmVConvert),sizeof(StrDAC_I_MinOutVoltInmV));
        
        u16DAC_I_CurrentInd_MaxInAConvert=HextoAscii(gasCfgStatusData.sPerPortData[PORT0].u16DAC_I_CurrentInd_MaxInA,4);
        
        PCTWrite(StrDAC_I_CurrentInd_MaxInA,(UINT8*)&u16DAC_I_CurrentInd_MaxInAConvert[0],sizeof(u16DAC_I_CurrentInd_MaxInAConvert),sizeof(StrDAC_I_CurrentInd_MaxInA));
        
        u8SinkPDOCntConvert=HextoAscii(gasCfgStatusData.sPerPortData[PORT0].u8SinkPDOCnt,4);
        
        PCTWrite(StrSinkPDOCnt,(UINT8*)&u8SinkPDOCntConvert[0],sizeof(u8SinkPDOCntConvert),sizeof(StrSinkPDOCnt));
        
        u8SinkConfigSelConvert=HextoAscii(gasCfgStatusData.sPerPortData[PORT0].u8SinkConfigSel,4);
        
        PCTWrite(StrSinkConfigSel,(UINT8*)&u8SinkConfigSelConvert[0],sizeof(u8SinkConfigSelConvert),sizeof(StrSinkConfigSel));
        
        u8FaultInDebounceInmsConvert=HextoAscii(gasCfgStatusData.sPerPortData[PORT0].u8FaultInDebounceInms,4);
        
        PCTWrite(StrFaultInDebounceInms,(UINT8*)&u8FaultInDebounceInmsConvert[0],sizeof(u8FaultInDebounceInmsConvert),sizeof(StrFaultInDebounceInms));
        
        u8OCSThresholdPercentageConvert=HextoAscii(gasCfgStatusData.sPerPortData[PORT0].u8OCSThresholdPercentage,4);
        
        PCTWrite(StrOCSThresholdPercentage,(UINT8*)&u8OCSThresholdPercentageConvert[0],sizeof(u8OCSThresholdPercentageConvert),sizeof(StrOCSThresholdPercentage));
        
        u8OVThresholdPercentageConvert=HextoAscii(gasCfgStatusData.sPerPortData[PORT0].u8OVThresholdPercentage,4);
        
        PCTWrite(StrOVThresholdPercentage,(UINT8*)&u8OVThresholdPercentageConvert[0],sizeof(u8OVThresholdPercentageConvert),sizeof(StrOVThresholdPercentage));
   
        u8UVThresholdPercentageConvert=HextoAscii(gasCfgStatusData.sPerPortData[PORT0].u8UVThresholdPercentage,4);
        
        PCTWrite(StrUVThresholdPercentage,(UINT8*)&u8UVThresholdPercentageConvert[0],sizeof(u8UVThresholdPercentageConvert),sizeof(StrUVThresholdPercentage));
       
        u8VCONNOCSDebounceInmsConvert=HextoAscii(gasCfgStatusData.sPerPortData[PORT0].u8VCONNOCSDebounceInms,4);
        
        PCTWrite(StrVCONNOCSDebounceInms,(UINT8*)&u8VCONNOCSDebounceInmsConvert[0],sizeof(u8VCONNOCSDebounceInmsConvert),sizeof(StrVCONNOCSDebounceInms));
   
        u8VBUSMaxFaultCntConvert=HextoAscii(gasCfgStatusData.sPerPortData[PORT0].u8VBUSMaxFaultCnt,4);
        
        PCTWrite(StrVBUSMaxFaultCnt,(UINT8*)&u8VBUSMaxFaultCntConvert[0],sizeof(u8VBUSMaxFaultCntConvert),sizeof(StrVBUSMaxFaultCnt));
      
        u8VCONNMaxFaultCntConvert=HextoAscii(gasCfgStatusData.sPerPortData[PORT0].u8VCONNMaxFaultCnt,4);
        
        PCTWrite(StrVCONNMaxFaultCnt,(UINT8*)&u8VCONNMaxFaultCntConvert[0],sizeof(u8VCONNMaxFaultCntConvert),sizeof(StrVCONNMaxFaultCnt));
      
        u8Pio_FAULT_INConvert=HextoAscii(gasCfgStatusData.sPerPortData[PORT0].u8Pio_FAULT_IN,4);
        
        PCTWrite(StrPio_FAULT_IN,(UINT8*)&u8Pio_FAULT_INConvert[0],sizeof(u8Pio_FAULT_INConvert),sizeof(StrPio_FAULT_IN));
      
        u8Mode_FAULT_INConvert=HextoAscii(gasCfgStatusData.sPerPortData[PORT0].u8Mode_FAULT_IN,4);
        
        PCTWrite(StrMode_FAULT_IN,(UINT8*)&u8Mode_FAULT_INConvert[0],sizeof(u8Mode_FAULT_INConvert),sizeof(StrMode_FAULT_IN));
      
        u8Pio_EN_SINKConvert=HextoAscii(gasCfgStatusData.sPerPortData[PORT0].u8Pio_EN_SINK,4);
        
        PCTWrite(StrPio_EN_SINK,(UINT8*)&u8Pio_EN_SINKConvert[0],sizeof(u8Pio_EN_SINKConvert),sizeof(StrPio_EN_SINK));
          
        u8Mode_EN_SINKConvert=HextoAscii(gasCfgStatusData.sPerPortData[PORT0].u8Mode_EN_SINK,4);
        
        PCTWrite(StrMode_EN_SINK,(UINT8*)&u8Mode_EN_SINKConvert[0],sizeof(u8Mode_EN_SINKConvert),sizeof(StrMode_EN_SINK));
        
        u8DAC_I_DirectionConvert=HextoAscii(gasCfgStatusData.sPerPortData[PORT0].u8DAC_I_Direction,7);
        
        PCTWrite(StrDAC_I_Direction,(UINT8*)&u8DAC_I_DirectionConvert[0],sizeof(u8DAC_I_DirectionConvert),sizeof(StrDAC_I_Direction));
       
    }
    
}

int ASCIIHexStringtoInteger(UINT8 u8array[],UINT8 u8Length)
{
    UINT8 i=0,b=0;
    UINT32 u32Address=0;
    while(i<u8Length)
    {
        if(u8array[i]>=0x30&&u8array[i]<=0x39)
        {
            /*convert to integer*/
            b=u8array[i]-0x30;
            u32Address=((u32Address<<4)|b);
        }
        else if(u8array[i]>=0x61&&u8array[i]<=0x66)
        {
            /*convert to integer*/
            b=u8array[i]-0x57;
            u32Address=((u32Address<<4)|b);
        }
        else
        {
            u32Address=0x00;
        }
       i++;
            
    }
    return u32Address;
  
}
/* 
Function: PCTPrintMemory 

This function writes the memory of length(given in terminal by user) onto the terminal.

*/
void PCTPrintMemory(UINT8 u8array[],UINT8 u8length)
{
    UINT32 u32Address,u32ByteLength;
    UINT8 u8Length=0,i=0,u8ByteValStart=0,u8ByteValStart2=0;
    UINT8 *u8Addressptr;
    while((u8array[8+u8Length])!=0x5d&&u8Length<8)
    {
        
        u8Length++;
        
    }
     
    u32Address =ASCIIHexStringtoInteger(&u8array[8],u8Length);
    u8ByteValStart2=0x0A+u8Length;
    u8ByteValStart=0x0A+u8Length;
    u8Length=0;
    while((u8array[u8ByteValStart])!=0x5d&&(u8Length<=0x01))
    {
        u8Length++;      
        u8ByteValStart++;
    }
    u32ByteLength =ASCIIHexStringtoInteger(&u8array[u8ByteValStart2],u8Length);
    if(u32Address)
    {  
        u8Addressptr=(UINT8*)u32Address;
       
            for(i=SET_TO_ZERO;i<=u32ByteLength;i++)
            {
                   
                SERCOM1_USART_WriteByte(u8Addressptr[i]);
            }
     }
}
/* 
Function: PCTPrintMemory 

This Function writes all current values of all the parameters onto the terminal.

*/
void PCTPrintCurrentValues(UINT8 u8array[],UINT8 u8Length)
{
    PCTPrintBaudRate(u8array,u8Length);
    PCTPrintVersion(u8array,u8Length);
    PCTPrintPDO(u8array,u8Length);
    PCTPrintStatusData(u8array,u8Length);
}
void PCTSetBaudRate(UINT8 u8array[],UINT8 u8length)
{
    UINT32 u32BaudRate;
    UINT8 u8Length=0;
    
    while((u8array[7+u8Length])!=0x5d)
    {
        /*BaudRate to be set given in the terminal starts from the 8th element of the u8array*/
        u8Length++;        
    }
     
    u32BaudRate =ASCIIHexStringtoInteger(&u8array[7],u8Length);
    
    PCTSetBaud(u32BaudRate);

}
/* 
Function: PCTSetMemory 

This Function sets the memory with the byte value given on the terminal.

*/
void PCTSetMemory(UINT8 u8array[],UINT8 u8length)
{
    UINT32 u32Address,u32ByteValue=0;
    UINT8 u8Length=0,u8ByteValStart2=0,u8ByteValStart=0,i=0;
    UINT8 *u8ptr;
    while((u8array[8+u8Length])!=0x5d&&u8Length<8)
    {
        /*MemoryAddress to be set given in the terminal starts from the 8th element of the u8array*/
        u8Length++;       
    }
     
    u32Address =ASCIIHexStringtoInteger(&u8array[8],u8Length);
    u8ByteValStart2=0x0A+u8Length;
    u8ByteValStart=0x0A+u8Length;
    u8Length=0;
    
    while((u8array[u8ByteValStart])!=0x5d)
    {
        /*ByteValue to be set given in the terminal starts from the element after the first ']' of the u8array*/
        u8Length++;      
        u8ByteValStart++;
    }
    u32ByteValue =ASCIIHexStringtoInteger(&u8array[u8ByteValStart2],u8Length);

    if(u32Address)
    { 
        u8ptr=(UINT8*)u32Address;
        u8ptr[i]=u32ByteValue;
        u32ByteValue=0;
    }
} 

void PCTSetPDO(UINT8 u8array[],UINT8 u8length)
{
    UINT32 u32PDOPosition,u32PDOValue;
    UINT8 u8Length=0,u8ByteValStart2=0,u8ByteValStart=0, u8Index;
    while((u8array[8+u8Length])!=0x5d)
    {
        
        u8Length++;       
    }
     
    u32PDOPosition =ASCIIHexStringtoInteger(&u8array[8],u8Length);
    
    //Max PDO size allocated is 7
    if(u32PDOPosition > 7) 
    {
     //#TODO: Add error message
      return;
    }
    u8ByteValStart2=0x0A+u8Length;
    u8ByteValStart=0x0A+u8Length;
    u8Length=0;
    
    while((u8array[u8ByteValStart])!=0x5d&&(u8Length<=0x01))
    {
        u8Length++;      
        u8ByteValStart++;
    }
    u32PDOValue =ASCIIHexStringtoInteger(&u8array[u8ByteValStart2],u8Length);
    u32PDOValue=u32PDOValue*1000;
    
    for(u8Index= SET_TO_ZERO;u8Index< gasCfgStatusData.sPerPortData[PORT0].u8AdvertisedPDOCnt; u8Index++)
    {
        gasCfgStatusData.sPerPortData[PORT0].u32aNewSinkPDO[u8Index] = gasCfgStatusData.sPerPortData[PORT0].u32aAdvertisedPDO[u8Index];
    }
    
    gasCfgStatusData.sPerPortData[PORT0].u32aNewSinkPDO[u32PDOPosition]=u32PDOValue;
    
    gasCfgStatusData.sPerPortData[PORT0].u8NewSinkPDOCnt = gasCfgStatusData.sPerPortData[PORT0].u8SinkPDOCnt;
    if(gasCfgStatusData.sPerPortData[PORT0].u32aNewSinkPDO[u32PDOPosition]!=gasCfgStatusData.sPerPortData[PORT0].u32aAdvertisedPDO[u32PDOPosition])
    {
        /* User application may request PSF to renegotiate 
        based on default PDOs gasCfgStatusData.sPerPortData[u8PortNum].u32aSinkPDO) 
        or gasCfgStatusData.sPerPortData[u8PortNum].u32aNewSinkPDO).
       
        To renegotiate with new PDOs, user application must ensure that
        new PDOs (gasCfgStatusData.sPerPortData[u8PortNum].u32aNewSinkPDO) are configured
        and BIT(10) in gasCfgStatusData.sPerPortData[u8PortNum].u32CfgData variable
        is set. Then BIT(5) in gasCfgStatusData.sPerPortData[u8PortNum].u32ClientRequest
        variable needs to be set by the user application. */
        gasCfgStatusData.sPerPortData[PORT0].u32ClientRequest =  1<<5;
        gasCfgStatusData.sPerPortData[PORT0].u32CfgData |= 1<<10;
    }
    
}


/*This function processes Get commands*/
void PCTGetProcessCommands(UINT8 u8array[], UINT8 u8Length)
{    
    char chGetHelper[]="get *";
    char chGetBaudHelper[]="get b";
    char chGetVersionHelper[]="get v";
    char chGetPDOHelper[]="get p";
    char chGetStatusRegHelper[]="get sr[";
    char chGetMemhelper[]="get m";
    if(0== memcmp(u8array,chGetHelper,5))
    {    
        PCTPrintCurrentValues(u8array,u8Length);
    }
    else if(0== memcmp(u8array,chGetBaudHelper,5))
    {    
        PCTPrintBaudRate(u8array,u8Length);
    }
    else if(0== memcmp(u8array,chGetVersionHelper,5))
    {    
        PCTPrintVersion(u8array,u8Length);
    }
    else if(0== memcmp(u8array,chGetPDOHelper,5))
    {    
        PCTPrintPDO(u8array,u8Length);
    }
    else if(0== memcmp(u8array,chGetStatusRegHelper,7))
    {    
        PCTPrintStatusData(u8array,u8Length);
    }
    else if(0== memcmp(u8array,chGetMemhelper,5))
    {    
        PCTPrintMemory(u8array,u8Length);
    }
    else
    {
        PCTInvalidCommandMsg();
    }
       
}



/* This function processes set commands*/
void PCTSetProcessCommands(UINT8 u8array[], UINT8 u8Length)
{
    char chSetBaudHelper[]="set b";
    char chSetMemHelper[]="set m";
    char chSetPDOHelper[]="set p";
    
    if(0== memcmp(u8array,chSetBaudHelper,5))
    {    
        PCTSetBaudRate(u8array,u8Length);
    }
    else if(0== memcmp(u8array,chSetMemHelper,5))
    {    
        PCTSetMemory(u8array,u8Length);
    }
    else if(0== memcmp(u8array,chSetPDOHelper,5))
    {    
        PCTSetPDO(u8array,u8Length);
    }
    else
    {
        PCTInvalidCommandMsg();
    }
    
     
}

void PCTExitProcessCommands(UINT8 u8array[], UINT8 u8Length)
{
    UINT8 StrPrint[]="> exit\n\n\r> exiting PSF Control Terminal\n\n\r";
    SERCOM1_USART_Write(&StrPrint[0],sizeof(StrPrint)); 
}
void PCTCommandhandler(UINT8 u8array[], UINT8 u8Length)
{
    
    UINT8 chGetHelper[] ="get";
    UINT8 chSetHelper[]="set";
    UINT8 chExitHelper[]="exit";
    UINT8 chHelpHelper[]="help";
    if(0 == memcmp(u8array, chGetHelper, 3))
    {
        /*processes commands starting with "get"*/
        PCTGetProcessCommands(u8array,u8Length);
    }  
    else if(0== memcmp(u8array, chSetHelper, 3))
    {
        /*processes commands staring with "set"*/
         PCTSetProcessCommands(u8array, u8Length);
    }
    else if(0== memcmp(u8array, chExitHelper, 4))
    {
        
        /*processes exit command*/
         PCTExitProcessCommands(u8array, u8Length);
    }
    else if(0== memcmp(u8array, chHelpHelper, 4))
    {
        
        /*processes help command*/
        PCTPrintCommands();
    }
    else
    {
        PCTInvalidCommandMsg();
    }
}
void PCTInvalidCommandMsg()
{
    UINT8 StrPrint[]="> invalid command! Type help to know more\n\n\r";
    SERCOM1_USART_Write(&StrPrint[0],sizeof(StrPrint)); 
}
void PCTSpaceBARMsg(bool lock)
{
  UINT8 StrPrint[]="> Hit SPACE BAR to unlock\n\n\r";
  UINT8 StrPrintUnlock[]="> Unlocked..\n\n\r";
  if(lock)
  {
    SERCOM1_USART_Write(&StrPrint[0],sizeof(StrPrint));
  }else
  {
      SERCOM1_USART_Write(&StrPrintUnlock[0],sizeof(StrPrintUnlock));
  }
}
/* 
Function: MchpPSF_PCTRUN 

This function reads the input from the terminal and starts the PCT and space key input does the toggling operation between DEBUG TRACE and PCT 

*/
void MchpPSF_PCTRUN(bool bBlocking)
{
    UINT8 u8array[LENGTH];    
    static UINT8 u8Length = 0;
    UINT8 u8ReadByte;
    static STATE state = INIT;
    
    do{
        switch(state)
        {
            case INIT:
                /*checks if UART receiver is ready */
                memset(u8array,0,LENGTH);
                if(true == SERCOM1_USART_ReceiverIsReady())
                {
                    /*Read the input*/
                    u8array[u8Length] = PCTReadByte();
                    
                    if((u8array[u8Length]=='?') || (u8array[u8Length]==0x0d))
                    {
                        /*checks for '?' input*/
                        state = READ_PROCESS;
                    }

                    else if(u8array[u8Length]==0x20)
                    {
                        /*checks for space key input*/
                        state = TOGGLE_BLOCK;

                    }
                    else
                    {
                        u8Length++;
                        state = READ_CONTINUE;
                    }

                }

            break;
            case READ_CONTINUE:
                
                if(true == SERCOM1_USART_ReceiverIsReady())
                {
                    u8ReadByte = PCTReadByte();
                    if((u8ReadByte=='?') || (u8ReadByte==0x0d))
                    {
                        state =READ_PROCESS;
                        u8array[u8Length]=u8ReadByte;
                        break;
                    }else if(u8ReadByte==' ' &&(u8array[u8Length-1]==' '))
                    {
                       //nothing to do if again one more space
                    }else
                    {
                        u8array[u8Length]=u8ReadByte;
                        u8Length++;
                    }
                }
                state = READ_CONTINUE;
            break;

            case READ_PROCESS:
                if(u8array[u8Length] =='?')
                {
                    PCTPrintCommands();
                }
                else
                {
                    PCTCommandhandler(u8array, u8Length);  
                }
                
                u8Length=0;
                state = INIT;

            break; 
            case HALT:
                state = INIT;   
            break;

            case TOGGLE_BLOCK:
             
                if(bBlocking==true)
                {
                    bBlocking=false;
                }
                else
                {
                    bBlocking=true; 
                }
                PCTSpaceBARMsg(bBlocking);
                
                state = INIT;
            break;
        }
    }while(bBlocking);
}


void MchpPSF_PCTInit()
{
    /*writes the welcome message*/
    UINT8 a[]="> Welcome to PSF Control Terminal\n\n\r> Type ? for the list of supported commands\n\n\r> ";
    SERCOM1_USART_Write(&a[0],sizeof(a));
}
void PSF_monitorandwait()
{
    MchpPSF_PCTRUN(true);
}
           

/*******************************************************************************
 End of File
*/


