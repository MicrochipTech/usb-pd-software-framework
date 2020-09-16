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
#include <string.h>
#include <math.h>
#include "definitions.h"                // SYS function prototypes
#include "psf_stdinc.h"                 // PSF include file
#include "psf_control_terminal.h"
#include "ProjectVersion.h"
#include "PSF_Config.h"
#include "PSFSink_BootCfg.h"

#define DEMO_VERSION                   0x01
extern GLOBAL_CFG_STATUS_DATA gasCfgStatusData; 

/*Function PCTReadByte converts Captital alphabets to small if given by the user as per pointer number 7 of 2.10 in the design document */

int PCTReadByte(void)
{
    int ReadByte;
    ReadByte=SERCOM1_USART_ReadByte();
    if(ReadByte>0x40&&ReadByte<0x5B)
    {
        ReadByte=ReadByte+0x20;         
    }
    SERCOM1_USART_WriteByte(ReadByte);
    return (ReadByte);
}
/* Function writes when ? is pressed*/

void PCTPrintCommands()
{   
    UINT8 StrPrint[]="?\n\n\r> get * - Displays the list of all the parameters and their current values\n\n\r> exit - Exit from the PSF Control Terminal and continue to run the sample application\n\n\r> "
            "get version - Gets the current version\n\n\r> get br - Gets the existing baud rate\n\n\r> get pdo - Gets the sink power details\n\n\r> "
            "get sr[GlobalCfgStatusData] - Gets the config status data\n\n\r> get sr[gasCfgStatusData] - Gets the gas config status\n\n\r> get sr[PortCfgStatus] - Gets the port Config Status\n\n\r> "
            "get mem[memory_address][length] -  Reads the memory at the given location\n\n\r> set br[value] - Sets the baud rate to the given value\n\n\r> set mem[memory_address][byte value] - Writes memory to SAMD20\n\n\r> "
            "set pdo[position][value] -  Insert a new PDO at the specified location\n\n\r";
    SERCOM1_USART_Write(&StrPrint[0],sizeof(StrPrint));
  
}

/*Fuction PCTWrite writes the string and it's corresponding value on to the terminal */

void PCTWrite(UINT8 var[],UINT8 value[],UINT8 size,UINT8 StrSize)
{
    UINT8 i=0;
    SERCOM1_USART_Write(&var[0],StrSize);
    while(size --)
    {
        SERCOM1_USART_WriteByte(value[i]);
        i++;
   }
}
void PCTWriteConvert(UINT8 var[],UINT8 value[],UINT8 size,UINT8 StrSize)
{
    UINT8 i=(size-1);
    SERCOM1_USART_Write(&var[0],StrSize);
    while(size --)
    {
        SERCOM1_USART_WriteByte(value[i]);
        i--;
    }
}

char* HextoAscii(UINT32 u32HexVal,UINT8 Length)
{
    UINT8 i=0,j=0;
    UINT8 Digit=0,Value=0;
    static char AsciiValue[10];
    while(i<=Length)
    {
        /*Gets the 1st digit*/
        
        Digit=u32HexVal&0x0F;
        if (Digit>=0x00 && Digit<=0x09) 
        {
            /*conversion to ascii*/
            Value=Digit+0x30;
            AsciiValue[j]=Value;
            j++;
        } 
  
        else if (Digit>=0x0A && Digit<=0x0F) 
        { 
            Value=Digit+0x57;
            AsciiValue[j]=Value;
            j++;     
        }
        u32HexVal=u32HexVal>>0x04;
        i++;
    }
        return AsciiValue;
}
void PCTPrintBaudRate(UINT8 u8array[],UINT8 length)
{   
    UINT32 u32BaudRate;
    
    char *u8BaudRateConvert;
    
    UINT8 StrPrint[]="Baud Rate=";   
    
    u32BaudRate=SERCOM1_USART_ReturnBaud();
   
    u8BaudRateConvert=HextoAscii(u32BaudRate,sizeof(u32BaudRate));
  
    PCTWriteConvert(StrPrint,(UINT8*)&u8BaudRateConvert[0],sizeof(u8BaudRateConvert),sizeof(StrPrint));
}

void PCTPrintVersion(UINT8 u8array[],UINT8 length)
{
    UINT8 u8DemoVersion;
    
    UINT32 u32FirmwareVersion;
    
    char *u8DemoVersionConvert,*u32FirmwareVersionConvert;
    
    UINT8 StrPrint[]="> demo version=";
    
    UINT8 StrPrint2[]="\n\n\r> PSF version=" ;  
    
    u8DemoVersion=DEMO_VERSION;
    
    u32FirmwareVersion=SYSTEM_FW_REV;
    
    u8DemoVersionConvert=HextoAscii(u8DemoVersion,sizeof(u8DemoVersion));
    
    PCTWriteConvert(StrPrint,(UINT8*)&u8DemoVersionConvert[0],sizeof(u8DemoVersionConvert),sizeof(StrPrint));
     
    u32FirmwareVersionConvert=HextoAscii(u32FirmwareVersion,sizeof(u32FirmwareVersion));
    
    PCTWriteConvert(StrPrint2,(UINT8*)&u32FirmwareVersionConvert[0],sizeof(u32FirmwareVersionConvert),sizeof(StrPrint2));
}

void PCTPrintPDO(UINT8 u8array[],UINT8 length)
{
    char *u32aSinkPDOCurrentConvert;
    char *u32aSinkPDOVolatgeConvert;
    UINT32 u32PDOVoltage1,u32PDOVoltage2,u32PDOVoltage3,u32PDOVoltage4,u32PDOVoltage5,u32PDOVoltage6,u32PDOVoltage7;
    UINT32 u32PDOCurrent1,u32PDOCurrent2,u32PDOCurrent3,u32PDOCurrent4,u32PDOCurrent5,u32PDOCurrent6,u32PDOCurrent7;
    
    UINT8 StrVoltage1[]="Voltage1=";
    
    u32PDOVoltage1=DPM_GET_VOLTAGE_FROM_PDO_MILLI_V(gasCfgStatusData.sPerPortData[PORT0].u32aSinkPDO[0]);
    
    u32aSinkPDOVolatgeConvert=HextoAscii(u32PDOVoltage1,sizeof(u32PDOVoltage1));
    
    PCTWriteConvert(StrVoltage1,(UINT8*)&u32aSinkPDOVolatgeConvert[0],sizeof(u32aSinkPDOVolatgeConvert),sizeof(StrVoltage1));
    
    UINT8 StrCurrent1[]="Current1=";
    
    u32PDOCurrent1=DPM_GET_PDO_CURRENT(gasCfgStatusData.sPerPortData[PORT0].u32aSinkPDO[0]);
    
    u32aSinkPDOCurrentConvert=HextoAscii(u32PDOCurrent1,sizeof(u32PDOCurrent1));
    
    PCTWriteConvert(StrCurrent1,(UINT8*)&u32aSinkPDOCurrentConvert[0],sizeof(u32aSinkPDOCurrentConvert),sizeof(StrCurrent1));
    
    UINT8 StrVoltage2[]="\n\n\rVoltage2=";
    
    u32PDOVoltage2=DPM_GET_VOLTAGE_FROM_PDO_MILLI_V(gasCfgStatusData.sPerPortData[PORT0].u32aSinkPDO[1]);
    
    u32aSinkPDOVolatgeConvert=HextoAscii(u32PDOVoltage2,sizeof(u32PDOVoltage2));
    
    PCTWriteConvert(StrVoltage2,(UINT8*)&u32aSinkPDOVolatgeConvert[0],sizeof(u32aSinkPDOVolatgeConvert),sizeof(StrVoltage2));
    
    UINT8 StrCurrent2[]="Current2=";
    
    u32PDOCurrent2=DPM_GET_PDO_CURRENT(gasCfgStatusData.sPerPortData[PORT0].u32aSinkPDO[1]);
    
    u32aSinkPDOCurrentConvert=HextoAscii(u32PDOCurrent2,sizeof(u32PDOCurrent2));
    
    PCTWriteConvert(StrCurrent2,(UINT8*)&u32aSinkPDOCurrentConvert[0],sizeof(u32aSinkPDOCurrentConvert),sizeof(StrCurrent2));

    UINT8 StrVoltage3[]="\n\n\rVoltage3=";
    
    u32PDOVoltage3=DPM_GET_VOLTAGE_FROM_PDO_MILLI_V(gasCfgStatusData.sPerPortData[PORT0].u32aSinkPDO[2]);
    
    u32aSinkPDOVolatgeConvert=HextoAscii(u32PDOVoltage3,sizeof(u32PDOVoltage3));
    
    PCTWriteConvert(StrVoltage3,(UINT8*)&u32aSinkPDOVolatgeConvert[0],sizeof(u32aSinkPDOVolatgeConvert),sizeof(StrVoltage3));
    
    UINT8 StrCurrent3[]="Current3=";
    
    u32PDOCurrent3=DPM_GET_PDO_CURRENT(gasCfgStatusData.sPerPortData[PORT0].u32aSinkPDO[2]);
    
    u32aSinkPDOCurrentConvert=HextoAscii(u32PDOCurrent3,sizeof(u32PDOCurrent3));
    
    PCTWriteConvert(StrCurrent3,(UINT8*)&u32aSinkPDOCurrentConvert[0],sizeof(u32aSinkPDOCurrentConvert),sizeof(StrCurrent3));

    UINT8 StrVoltage4[]="\n\n\rVoltage4=";
    
    u32PDOVoltage4=DPM_GET_VOLTAGE_FROM_PDO_MILLI_V(gasCfgStatusData.sPerPortData[PORT0].u32aSinkPDO[3]);
    
    u32aSinkPDOVolatgeConvert=HextoAscii(u32PDOVoltage4,sizeof(u32PDOVoltage4));
    
    PCTWriteConvert(StrVoltage4,(UINT8*)&u32aSinkPDOVolatgeConvert[0],sizeof(u32aSinkPDOVolatgeConvert),sizeof(StrVoltage4));
    
    UINT8 StrCurrent4[]="Current4=";
    
    u32PDOCurrent4=DPM_GET_PDO_CURRENT(gasCfgStatusData.sPerPortData[PORT0].u32aSinkPDO[3]);
    
    u32aSinkPDOCurrentConvert=HextoAscii(u32PDOCurrent4,sizeof(u32PDOCurrent4));
    
    PCTWriteConvert(StrCurrent4,(UINT8*)&u32aSinkPDOCurrentConvert[0],sizeof(u32aSinkPDOCurrentConvert),sizeof(StrCurrent4));

    UINT8 StrVoltage5[]="\n\n\rVoltage5=";
    
    u32PDOVoltage5=DPM_GET_VOLTAGE_FROM_PDO_MILLI_V(gasCfgStatusData.sPerPortData[PORT0].u32aSinkPDO[4]);
    
    u32aSinkPDOVolatgeConvert=HextoAscii(u32PDOVoltage5,sizeof(u32PDOVoltage5));
    
    PCTWriteConvert(StrVoltage5,(UINT8*)&u32aSinkPDOVolatgeConvert[0],sizeof(u32aSinkPDOVolatgeConvert),sizeof(StrVoltage5));
    
    UINT8 StrCurrent5[]="Current5=";
    
    u32PDOCurrent5=DPM_GET_PDO_CURRENT(gasCfgStatusData.sPerPortData[PORT0].u32aSinkPDO[4]);
    
    u32aSinkPDOCurrentConvert=HextoAscii(u32PDOCurrent5,sizeof(u32PDOCurrent5));
    
    PCTWriteConvert(StrCurrent5,(UINT8*)&u32aSinkPDOCurrentConvert[0],sizeof(u32aSinkPDOCurrentConvert),sizeof(StrCurrent5));

    UINT8 StrVoltage6[]="\n\n\rVoltage6=";
    
    u32PDOVoltage6=DPM_GET_VOLTAGE_FROM_PDO_MILLI_V(gasCfgStatusData.sPerPortData[PORT0].u32aSinkPDO[5]);
    
    u32aSinkPDOVolatgeConvert=HextoAscii(u32PDOVoltage6,sizeof(u32PDOVoltage6));
    
    PCTWriteConvert(StrVoltage5,(UINT8*)&u32aSinkPDOVolatgeConvert[0],sizeof(u32aSinkPDOVolatgeConvert),sizeof(StrVoltage6));
    
    UINT8 StrCurrent6[]="Current6=";
    
    u32PDOCurrent6=DPM_GET_PDO_CURRENT(gasCfgStatusData.sPerPortData[PORT0].u32aSinkPDO[5]);
    
    u32aSinkPDOCurrentConvert=HextoAscii(u32PDOCurrent6,sizeof(u32PDOCurrent6));
    
    PCTWriteConvert(StrCurrent6,(UINT8*)&u32aSinkPDOCurrentConvert[0],sizeof(u32aSinkPDOCurrentConvert),sizeof(StrCurrent6));
    
    UINT8 StrVoltage7[]="\n\n\rVoltage7=";
    
    u32PDOVoltage7=DPM_GET_VOLTAGE_FROM_PDO_MILLI_V(gasCfgStatusData.sPerPortData[PORT0].u32aSinkPDO[6]);
    
    u32aSinkPDOVolatgeConvert=HextoAscii(u32PDOVoltage7,sizeof(u32PDOVoltage7));
    
    PCTWriteConvert(StrVoltage7,(UINT8*)&u32aSinkPDOVolatgeConvert[0],sizeof(u32aSinkPDOVolatgeConvert),sizeof(StrVoltage7));
    
    UINT8 StrCurrent7[]="Current7=";
    
    u32PDOCurrent7=DPM_GET_PDO_CURRENT(gasCfgStatusData.sPerPortData[PORT0].u32aSinkPDO[6]);
    
    u32aSinkPDOCurrentConvert=HextoAscii(u32PDOCurrent7,sizeof(u32PDOCurrent7));
    
    PCTWriteConvert(StrCurrent7,(UINT8*)&u32aSinkPDOCurrentConvert[0],sizeof(u32aSinkPDOCurrentConvert),sizeof(StrCurrent7));

    
}

void PCTPrintStatusData(UINT8 u8array[],UINT8 length)
{
    UINT8 chHelper[]="get sr[g";
    UINT8 chHelper2[]="get sr[p";
    if(0==memcmp(u8array,chHelper,8))    
    {
        char *u8MinorVersionConvert,*u8MajorVersionConvert,*u8HWVersionConvert,*u8SiVersionConvert,*u8PSFMajorVersionConvert,*u8PSFMinorVersionConvert;
        char *u8PwrThrottleCfgConvert,*u16ProducdIDConvert,*u16VendorIDConvert,*u16ProductTypeVDOConvert,*u16ProductVDOConvert,*u16CertStatVDOConvert,*u16IDHeaderVDOConvert;
        
        UINT8 StrPrint[]=">\n\n\rUINT8 u8MinorVersion=";
        
        UINT8 StrPrint2[]="\n\n\rUINT8 u8MajorVersion=";
        
        UINT8 StrPrint3[]="\n\n\rUINT8 u8HWVersion";
        
        UINT8 StrPrint4[]="\n\n\rUINT8 u8SiVersion";
        
 //       UINT8 StrPrint5[]="UINT8 u8aManfString[8]";
        UINT8 StrPrint6[]="\n\n\rUINT8 u8PSFMajorVersion";
        
        UINT8 StrPrint7[]="\n\n\rUINT8 u8PSFMinorVersion";
        
        UINT8 StrPrint8[]="\n\n\rUINT8 u8PwrThrottleCfg";
//        UINT8 StrPrint9[]="UINT8 u8aReserved3";
        UINT8 StrPrint10[]="\n\n\rUINT16 u16ProducdID";
        
        UINT8 StrPrint11[]="\n\n\rUINT16 u16VendorID";
        
        UINT8 StrPrint12[]="\n\n\rUINT16 u16ProductTypeVDO";
        
        UINT8 StrPrint13[]="\n\n\rUINT16 u16ProductVDO";
        
        UINT8 StrPrint14[]="\n\n\rUINT16 u16CertStatVDO";
        
        UINT8 StrPrint15[]="\n\n\rUINT16 u16IDHeaderVDO";
        
      
        u8MinorVersionConvert=HextoAscii(gasCfgStatusData.u8MinorVersion,4);
        
        PCTWriteConvert(StrPrint,(UINT8*)&u8MinorVersionConvert[0],sizeof(u8MinorVersionConvert),sizeof(StrPrint));
      
        
        u8MajorVersionConvert=HextoAscii(gasCfgStatusData.u8MajorVersion,4);
        
        PCTWriteConvert(StrPrint2,(UINT8*)&u8MajorVersionConvert[0],sizeof(u8MajorVersionConvert),sizeof(StrPrint2));
        
        u8HWVersionConvert=HextoAscii(gasCfgStatusData.u8HWVersion,4);
        
        PCTWriteConvert(StrPrint3,(UINT8*)&u8HWVersionConvert[0],sizeof(u8HWVersionConvert),sizeof(StrPrint3));
        
        u8SiVersionConvert=HextoAscii(gasCfgStatusData.u8SiVersion,4);
        
        PCTWriteConvert(StrPrint4,(UINT8*)&u8SiVersionConvert[0],sizeof(u8SiVersionConvert),sizeof(StrPrint4));
       // gasCfgStatusData.u8aManfString[8];
        
        u8PSFMajorVersionConvert=HextoAscii(gasCfgStatusData.u8PSFMajorVersion,4);
        
        PCTWriteConvert(StrPrint6,(UINT8*)&u8PSFMajorVersionConvert[0],sizeof(u8PSFMajorVersionConvert),sizeof(StrPrint6));
        
        u8PSFMinorVersionConvert=HextoAscii(gasCfgStatusData.u8PSFMinorVersion,4);
        
        PCTWriteConvert(StrPrint7,(UINT8*)&u8PSFMinorVersionConvert[0],sizeof(u8PSFMinorVersionConvert),sizeof(StrPrint7));
        
        u8PwrThrottleCfgConvert=HextoAscii(gasCfgStatusData.u8PwrThrottleCfg,4);
        
        PCTWriteConvert(StrPrint8,(UINT8*)&u8PwrThrottleCfgConvert[0],sizeof(u8PwrThrottleCfgConvert),sizeof(StrPrint8));
      //  gasCfgStatusData.u8aReserved3;
        
        u16ProducdIDConvert=HextoAscii(gasCfgStatusData.u16ProducdID,4);
        
        PCTWriteConvert(StrPrint10,(UINT8*)&u16ProducdIDConvert[0],sizeof(u16ProducdIDConvert),sizeof(StrPrint10));
        
        u16VendorIDConvert=HextoAscii(gasCfgStatusData.u16VendorID,4);
        
        PCTWriteConvert(StrPrint11,(UINT8*)&u16VendorIDConvert[0],sizeof(u16VendorIDConvert),sizeof(StrPrint11));
        
        u16ProductTypeVDOConvert=HextoAscii(gasCfgStatusData.u16ProductTypeVDO,4);
        
        PCTWriteConvert(StrPrint12,(UINT8*)&u16ProductTypeVDOConvert[0],sizeof(u16ProductTypeVDOConvert),sizeof(StrPrint12));
        
        u16ProductVDOConvert=HextoAscii(gasCfgStatusData.u16ProductVDO,4);
        
        PCTWriteConvert(StrPrint13,(UINT8*)&u16ProductVDOConvert[0],sizeof(u16ProductVDOConvert),sizeof(StrPrint13));
        
        u16CertStatVDOConvert=HextoAscii(gasCfgStatusData.u16CertStatVDO,4);
        
        PCTWriteConvert(StrPrint14,(UINT8*)&u16CertStatVDOConvert[0],sizeof(u16CertStatVDOConvert),sizeof(StrPrint14));
        
        u16IDHeaderVDOConvert=HextoAscii(gasCfgStatusData.u16IDHeaderVDO,4);
        
        PCTWriteConvert(StrPrint15,(UINT8*)&u16IDHeaderVDOConvert[0],sizeof(u16IDHeaderVDOConvert),sizeof(StrPrint15));
        
    }
    else if(0==memcmp(u8array,chHelper2,8))
    {
        char *u32CfgDataConvert,*u16PowerGoodTimerInmsConvert,*u16SnkMaxOperatingCurInmAConvert,*u16SnkMinOperatingCurInmAConvert;
        char *u16DAC_I_CurrentInd_MaxInAConvert,*u16DAC_I_MinOutVoltInmVConvert,*u16MaxSrcPrtCurrentIn10mAConvert,*u16DAC_I_MaxOutVoltInmVConvert;
        char *u8OVThresholdPercentageConvert,*u8FaultInDebounceInmsConvert,*u8UVThresholdPercentageConvert,*u8VCONNOCSDebounceInmsConvert,*u8VCONNMaxFaultCntConvert;
        char *u8Pio_FAULT_INConvert,*u8VBUSMaxFaultCntConvert,*u8Mode_FAULT_INConvert,*u8Pio_EN_SINKConvert,*u8Mode_EN_SINKConvert,*u8SinkPDOCntConvert,*u8SinkConfigSelConvert,*u8OCSThresholdPercentageConvert;
        char *u8DAC_I_DirectionConvert;
        char *u16aMinPDOPreferredCurInmA1Convert,*u16aMinPDOPreferredCurInmA2Convert,*u16aMinPDOPreferredCurInmA3Convert,*u16aMinPDOPreferredCurInmA4Convert,*u16aMinPDOPreferredCurInmA5Convert,*u16aMinPDOPreferredCurInmA6Convert,*u16aMinPDOPreferredCurInmA7Convert;
        
        
        UINT8 StrPrint16[]=">\n\n\rUINT32 u32CfgData=";
        
        UINT8 StrPrint24[]=">\n\n\rUINT16 u16PowerGoodTimerInms";
        
        UINT8 StrPrint25[]=">\n\n\rUINT16 u16aMinPDOPreferredCurInmA1;";
        
        UINT8 StrPrint26[]=">\n\n\rUINT16 u16aMinPDOPreferredCurInmA2;";
        
        UINT8 StrPrint27[]=">\n\n\rUINT16 u16aMinPDOPreferredCurInmA3;";
        
        UINT8 StrPrint28[]=">\n\n\rUINT16 u16aMinPDOPreferredCurInmA4;";
        
        UINT8 StrPrint29[]=">\n\n\rUINT16 u16aMinPDOPreferredCurInmA5;";
        
        UINT8 StrPrint30[]=">\n\n\rUINT16 u16aMinPDOPreferredCurInmA6;";
        
        UINT8 StrPrint31[]=">\n\n\rUINT16 u16aMinPDOPreferredCurInmA7;";
        
        UINT8 StrPrint32[]=">\n\n\rUINT16 u16SnkMaxOperatingCurInmA";
        
        UINT8 StrPrint33[]=">\n\n\rUINT16 u16SnkMinOperatingCurInmA";
        
        UINT8 StrPrint34[]=">\n\n\rUINT16 u16DAC_I_MaxOutVoltInmV";
        
        UINT8 StrPrint35[]=">\n\n\rUINT16 u16DAC_I_MinOutVoltInmV";
        
        UINT8 StrPrint36[]=">\n\n\rUINT16 u16DAC_I_CurrentInd_MaxInA";
        
        UINT8 StrPrint37[]=">\n\n\rUINT8 u8SinkPDOCnt";
        
        UINT8 StrPrint38[]=">\n\n\rUINT8 u8SinkConfigSel";
        
        UINT8 StrPrint39[]=">\n\n\rUINT8 u8FaultInDebounceInms";
        
        UINT8 StrPrint40[]=">\n\n\rUINT8 u8OCSThresholdPercentage";//not done
        
        UINT8 StrPrint41[]=">\n\n\rUINT8 u8OVThresholdPercentage";
        
        UINT8 StrPrint42[]=">\n\n\rUINT8 u8UVThresholdPercentage";
        
        UINT8 StrPrint43[]=">\n\n\rUINT8 u8VCONNOCSDebounceInms";
        
        UINT8 StrPrint44[]=">\n\n\rUINT8 u8VBUSMaxFaultCnt"; //set to 3 by pasCfgStatusData
        
        UINT8 StrPrint45[]=">\n\n\rUINT8 u8VCONNMaxFaultCnt";
        
        UINT8 StrPrint46[]=">\n\n\rUINT8 u8Pio_FAULT_IN";
        
        UINT8 StrPrint47[]=">\n\n\rUINT8 u8Mode_FAULT_IN";
        
        UINT8 StrPrint48[]=">\n\n\rUINT8 u8Pio_EN_SINK";
        
        UINT8 StrPrint49[]=">\n\n\rUINT8 u8Mode_EN_SINK";
        
        UINT8 StrPrint50[]=">\n\n\rUINT8 u8DAC_I_Direction";
        
        u32CfgDataConvert=HextoAscii(gasCfgStatusData.sPerPortData[PORT0].u32CfgData,sizeof(gasCfgStatusData.sPerPortData[PORT0].u32CfgData));
        
        PCTWriteConvert(StrPrint16,(UINT8*)&u32CfgDataConvert[0],sizeof(u32CfgDataConvert),sizeof(StrPrint16));
     
        
        u16PowerGoodTimerInmsConvert=HextoAscii(gasCfgStatusData.sPerPortData[PORT0].u16PowerGoodTimerInms,4);
        
        PCTWriteConvert(StrPrint24,(UINT8*)&u16PowerGoodTimerInmsConvert[0],sizeof(u16PowerGoodTimerInmsConvert),sizeof(StrPrint24)); 
        
        u16SnkMaxOperatingCurInmAConvert=HextoAscii(gasCfgStatusData.sPerPortData[PORT0].u16SnkMaxOperatingCurInmA,4);
        
        PCTWriteConvert(StrPrint32,(UINT8*)&u16SnkMaxOperatingCurInmAConvert[0],sizeof(u16SnkMaxOperatingCurInmAConvert),sizeof(StrPrint32));
       
        u16SnkMinOperatingCurInmAConvert=HextoAscii(gasCfgStatusData.sPerPortData[PORT0].u16SnkMinOperatingCurInmA,4);
        
        PCTWriteConvert(StrPrint33,(UINT8*)&u16SnkMinOperatingCurInmAConvert[0],sizeof(u16SnkMinOperatingCurInmAConvert),sizeof(StrPrint33));
     
        u16DAC_I_CurrentInd_MaxInAConvert=HextoAscii(gasCfgStatusData.sPerPortData[PORT0].u16DAC_I_CurrentInd_MaxInA,4);
        
        PCTWriteConvert(StrPrint36,(UINT8*)&u16DAC_I_CurrentInd_MaxInAConvert[0],sizeof(u16DAC_I_CurrentInd_MaxInAConvert),sizeof(StrPrint36));
      
        u16DAC_I_MaxOutVoltInmVConvert=HextoAscii(gasCfgStatusData.sPerPortData[PORT0].u16DAC_I_MaxOutVoltInmV,4);
        
        PCTWriteConvert(StrPrint34,(UINT8*)&u16DAC_I_MaxOutVoltInmVConvert[0],sizeof(u16DAC_I_MaxOutVoltInmVConvert),sizeof(StrPrint34));
      
        
        u16DAC_I_MinOutVoltInmVConvert=HextoAscii(gasCfgStatusData.sPerPortData[PORT0].u16DAC_I_MinOutVoltInmV,4);
        
        PCTWriteConvert(StrPrint35,(UINT8*)&u16DAC_I_MinOutVoltInmVConvert[0],sizeof(u16DAC_I_MinOutVoltInmVConvert),sizeof(StrPrint35));
        
       
        u16MaxSrcPrtCurrentIn10mAConvert=HextoAscii(gasCfgStatusData.sPerPortData[PORT0].u16MaxSrcPrtCurrentIn10mA,4);
        
        PCTWriteConvert(StrPrint16,(UINT8*)&u16MaxSrcPrtCurrentIn10mAConvert[0],sizeof(u16MaxSrcPrtCurrentIn10mAConvert),sizeof(StrPrint16));
       
        
        u8OVThresholdPercentageConvert=HextoAscii(gasCfgStatusData.sPerPortData[PORT0].u8OVThresholdPercentage,4);
        
        PCTWriteConvert(StrPrint41,(UINT8*)&u8OVThresholdPercentageConvert[0],sizeof(u8OVThresholdPercentageConvert),sizeof(StrPrint41));
        
        
        
        u8FaultInDebounceInmsConvert=HextoAscii(gasCfgStatusData.sPerPortData[PORT0].u8FaultInDebounceInms,4);
        
        PCTWriteConvert(StrPrint39,(UINT8*)&u8FaultInDebounceInmsConvert[0],sizeof(u8FaultInDebounceInmsConvert),sizeof(StrPrint39));
        
    
        u8UVThresholdPercentageConvert=HextoAscii(gasCfgStatusData.sPerPortData[PORT0].u8UVThresholdPercentage,4);
        
        PCTWriteConvert(StrPrint42,(UINT8*)&u8UVThresholdPercentageConvert[0],sizeof(u8UVThresholdPercentageConvert),sizeof(StrPrint42));
        
  
        
        u8VCONNOCSDebounceInmsConvert=HextoAscii(gasCfgStatusData.sPerPortData[PORT0].u8VCONNOCSDebounceInms,4);
        
        PCTWriteConvert(StrPrint43,(UINT8*)&u8VCONNOCSDebounceInmsConvert[0],sizeof(u8VCONNOCSDebounceInmsConvert),sizeof(StrPrint43));
     
        
        u8VCONNMaxFaultCntConvert=HextoAscii(gasCfgStatusData.sPerPortData[PORT0].u8VCONNMaxFaultCnt,4);
        
        PCTWriteConvert(StrPrint45,(UINT8*)&u8VCONNMaxFaultCntConvert[0],sizeof(u8VCONNMaxFaultCntConvert),sizeof(StrPrint45));
      
        u8Pio_FAULT_INConvert=HextoAscii(gasCfgStatusData.sPerPortData[PORT0].u8Pio_FAULT_IN,4);
        
        PCTWriteConvert(StrPrint46,(UINT8*)&u8Pio_FAULT_INConvert[0],sizeof(u8Pio_FAULT_INConvert),sizeof(StrPrint46));
        
        
        u8Mode_FAULT_INConvert=HextoAscii(gasCfgStatusData.sPerPortData[PORT0].u8Mode_FAULT_IN,4);
        
        PCTWriteConvert(StrPrint47,(UINT8*)&u8Mode_FAULT_INConvert[0],sizeof(u8Mode_FAULT_INConvert),sizeof(StrPrint47));
        
        
        u8VBUSMaxFaultCntConvert=HextoAscii(gasCfgStatusData.sPerPortData[PORT0].u8VBUSMaxFaultCnt,4);
        
        PCTWriteConvert(StrPrint44,(UINT8*)&u8VBUSMaxFaultCntConvert[0],sizeof(u8VBUSMaxFaultCntConvert),sizeof(StrPrint44));
      
        
        u8Pio_EN_SINKConvert=HextoAscii(gasCfgStatusData.sPerPortData[PORT0].u8Pio_EN_SINK,4);
        
        PCTWriteConvert(StrPrint48,(UINT8*)&u8Pio_EN_SINKConvert[0],sizeof(u8Pio_EN_SINKConvert),sizeof(StrPrint48));
          
        u8Mode_EN_SINKConvert=HextoAscii(gasCfgStatusData.sPerPortData[PORT0].u8Mode_EN_SINK,4);
        
        PCTWriteConvert(StrPrint49,(UINT8*)&u8Mode_EN_SINKConvert[0],sizeof(u8Mode_EN_SINKConvert),sizeof(StrPrint49));
        
        PCTPrintPDO(u8array,length);
        
        u8SinkPDOCntConvert=HextoAscii(gasCfgStatusData.sPerPortData[PORT0].u8SinkPDOCnt,4);
        
        PCTWriteConvert(StrPrint37,(UINT8*)&u8SinkPDOCntConvert[0],sizeof(u8SinkPDOCntConvert),sizeof(StrPrint37));
        
        u8DAC_I_DirectionConvert=HextoAscii(gasCfgStatusData.sPerPortData[PORT0].u8DAC_I_Direction,7);
        
        PCTWriteConvert(StrPrint50,(UINT8*)&u8DAC_I_DirectionConvert[0],sizeof(u8DAC_I_DirectionConvert),sizeof(StrPrint50));
        
        u16aMinPDOPreferredCurInmA1Convert=HextoAscii(gasCfgStatusData.sPerPortData[PORT0].u16aMinPDOPreferredCurInmA[0],4);
        
        PCTWriteConvert(StrPrint25,(UINT8*)&u16aMinPDOPreferredCurInmA1Convert[0],sizeof(u16aMinPDOPreferredCurInmA1Convert),sizeof(StrPrint25));
        
        u16aMinPDOPreferredCurInmA2Convert=HextoAscii(gasCfgStatusData.sPerPortData[PORT0].u16aMinPDOPreferredCurInmA[1],4);
        
        PCTWriteConvert(StrPrint26,(UINT8*)&u16aMinPDOPreferredCurInmA2Convert[0],sizeof(u16aMinPDOPreferredCurInmA2Convert),sizeof(StrPrint26));
        
        u16aMinPDOPreferredCurInmA3Convert=HextoAscii(gasCfgStatusData.sPerPortData[PORT0].u16aMinPDOPreferredCurInmA[2],4);
        
        PCTWriteConvert(StrPrint27,(UINT8*)&u16aMinPDOPreferredCurInmA3Convert[0],sizeof(u16aMinPDOPreferredCurInmA3Convert),sizeof(StrPrint27));
        
        u16aMinPDOPreferredCurInmA4Convert=HextoAscii(gasCfgStatusData.sPerPortData[PORT0].u16aMinPDOPreferredCurInmA[3],4);
        
        PCTWriteConvert(StrPrint28,(UINT8*)&u16aMinPDOPreferredCurInmA4Convert[0],sizeof(u16aMinPDOPreferredCurInmA4Convert),sizeof(StrPrint28));
        
        u16aMinPDOPreferredCurInmA5Convert=HextoAscii(gasCfgStatusData.sPerPortData[PORT0].u16aMinPDOPreferredCurInmA[4],4);
        
        PCTWriteConvert(StrPrint29,(UINT8*)&u16aMinPDOPreferredCurInmA5Convert[0],sizeof(u16aMinPDOPreferredCurInmA5Convert),sizeof(StrPrint29));
        
        u16aMinPDOPreferredCurInmA6Convert=HextoAscii(gasCfgStatusData.sPerPortData[PORT0].u16aMinPDOPreferredCurInmA[5],4);
        
        PCTWriteConvert(StrPrint30,(UINT8*)&u16aMinPDOPreferredCurInmA6Convert[0],sizeof(u16aMinPDOPreferredCurInmA6Convert),sizeof(StrPrint30));
        
        u16aMinPDOPreferredCurInmA7Convert=HextoAscii(gasCfgStatusData.sPerPortData[PORT0].u16aMinPDOPreferredCurInmA[6],4);
        
        PCTWriteConvert(StrPrint31,(UINT8*)&u16aMinPDOPreferredCurInmA7Convert[0],sizeof(u16aMinPDOPreferredCurInmA7Convert),sizeof(StrPrint31));
        
        u8SinkConfigSelConvert=HextoAscii(gasCfgStatusData.sPerPortData[PORT0].u8SinkConfigSel,4);
        
        PCTWriteConvert(StrPrint38,(UINT8*)&u8SinkConfigSelConvert[0],sizeof(u8SinkConfigSelConvert),sizeof(StrPrint38));
        
        u8OCSThresholdPercentageConvert=HextoAscii(gasCfgStatusData.sPerPortData[PORT0].u8OCSThresholdPercentage,4);
        
        PCTWriteConvert(StrPrint40,(UINT8*)&u8OCSThresholdPercentageConvert[0],sizeof(u8OCSThresholdPercentageConvert),sizeof(StrPrint40));
    }
    
}

int ASCIIHexStringtoInteger(UINT8 u8array[],UINT8 length)
{
    UINT8 i=0,b=0,n=0;
    UINT32 u32Address=0;
    while(i<length)
    {
        if(u8array[i]>=0x30&&u8array[i]<=0x39)
        {
            b=u8array[i]-0x30;
            u32Address=((u32Address<<4)|b);
        }
        else if(u8array[i]>=0x61&&u8array[i]<=0x66)
        {
            n=u8array[i]-0x57;
            u32Address=((u32Address<<4)|n);
        }
        else
        {
            u32Address=0x00;
        }
       i++;
            
    }
    return u32Address;
   
}
void PCTPrintMemory(UINT8 u8array[],UINT8 length)
{
    //UINT8 StrPrint[]=">Memory=";
    UINT32 u32Address,u32ByteLength;
    UINT8 u8Length=0,i=0,u8ByteValStart=0,u8ByteValStart2=0;
    //,u8ByteLength=0;
    UINT8 *u32Addressptr;
  //  UINT32 *ptr2;
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
        u32Addressptr=(UINT8*)u32Address;
       
            for(i=SET_TO_ZERO;i<=u32ByteLength;i++)
            {
                   
                SERCOM1_USART_WriteByte(u32Addressptr[i]);
            }
     }
}
/* Function writes all current values of all the parameters */
void PCTPrintCurrentValues(UINT8 u8array[],UINT8 length)
{
    PCTPrintBaudRate(u8array,length);
    PCTPrintVersion(u8array,length);
    PCTPrintPDO(u8array,length);
    PCTPrintStatusData(u8array,length);
}
void PCTSetBaudRate(UINT8 u8array[],UINT8 length)
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

void PCTSetMemory(UINT8 u8array[],UINT8 length)
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

void PCTSetPDO(UINT8 u8array[],UINT8 length)
{
//    gasCfgStatusData.sPerPortData[PORT0].u32aSinkPDO[1] = CFG_PORT_0_SINK_PDO_1;
//    gasCfgStatusData.sPerPortData[PORT0].u32aSinkPDO[2] = CFG_PORT_0_SINK_PDO_2;
//    gasCfgStatusData.sPerPortData[PORT0].u32aSinkPDO[3] = CFG_PORT_0_SINK_PDO_3;
//    gasCfgStatusData.sPerPortData[PORT0].u32aSinkPDO[4] = CFG_PORT_0_SINK_PDO_4;
//    gasCfgStatusData.sPerPortData[PORT0].u32aSinkPDO[5] = CFG_PORT_0_SINK_PDO_5;
//    gasCfgStatusData.sPerPortData[PORT0].u32aSinkPDO[6] = CFG_PORT_0_SINK_PDO_6;
//    gasCfgStatusData.sPerPortData[PORT0].u32aSinkPDO[7] = CFG_PORT_0_SINK_PDO_7;
    UINT32 u32PDOPosition,u32PDOValue;
    UINT8 u8Length=0,u8ByteValStart2=0,u8ByteValStart=0;
    while((u8array[8+u8Length])!=0x5d)
    {
        
        u8Length++;       
    }
     
    u32PDOPosition =ASCIIHexStringtoInteger(&u8array[8],u8Length);
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
    if(u32PDOPosition==2)
    {        
        gasCfgStatusData.sPerPortData[PORT0].u32aSinkPDO[7]=gasCfgStatusData.sPerPortData[PORT0].u32aSinkPDO[6];
        gasCfgStatusData.sPerPortData[PORT0].u32aSinkPDO[6]=gasCfgStatusData.sPerPortData[PORT0].u32aSinkPDO[5];
        gasCfgStatusData.sPerPortData[PORT0].u32aSinkPDO[5]=gasCfgStatusData.sPerPortData[PORT0].u32aSinkPDO[4];
        gasCfgStatusData.sPerPortData[PORT0].u32aSinkPDO[4]=gasCfgStatusData.sPerPortData[PORT0].u32aSinkPDO[3];
        gasCfgStatusData.sPerPortData[PORT0].u32aSinkPDO[3]=gasCfgStatusData.sPerPortData[PORT0].u32aSinkPDO[2];
        gasCfgStatusData.sPerPortData[PORT0].u32aSinkPDO[2]=CFG_FORM_FIXED_PDOx(u32PDOValue,3000U);
    }
    else if(u32PDOPosition==3)
    {
        gasCfgStatusData.sPerPortData[PORT0].u32aSinkPDO[3]=CFG_FORM_FIXED_PDOx(u32PDOValue,3000U);
        gasCfgStatusData.sPerPortData[PORT0].u32aSinkPDO[4]=CFG_FORM_FIXED_PDOx(15000U,3000U);
        gasCfgStatusData.sPerPortData[PORT0].u32aSinkPDO[5]=CFG_FORM_FIXED_PDOx(20000U,3000U);
        gasCfgStatusData.sPerPortData[PORT0].u32aSinkPDO[6]=CFG_FORM_FIXED_PDOx(20000U,3000U);
        gasCfgStatusData.sPerPortData[PORT0].u32aSinkPDO[7]=CFG_FORM_FIXED_PDOx(20000U,3000U);
    }
    else if(u32PDOPosition==4)
    {
        gasCfgStatusData.sPerPortData[PORT0].u32aSinkPDO[4]=CFG_FORM_FIXED_PDOx(u32PDOValue,3000U);
        gasCfgStatusData.sPerPortData[PORT0].u32aSinkPDO[5]=CFG_FORM_FIXED_PDOx(20000U,3000U);
        gasCfgStatusData.sPerPortData[PORT0].u32aSinkPDO[6]=CFG_FORM_FIXED_PDOx(20000U,3000U);
        gasCfgStatusData.sPerPortData[PORT0].u32aSinkPDO[7]=CFG_FORM_FIXED_PDOx(20000U,3000U);
    }
    
}


//Function process Get commands
void PCTGetProcessCommands(UINT8 u8array[], UINT8 length)
{    
    UINT8 chGetHelper[]="get *";
    UINT8 chGetHelper2[]="get b";
    UINT8 chGetHelper3[]="get v";
    UINT8 chGetHelper4[]="get p";
    UINT8 chGetHelper5[]="get sr[";
    UINT8 chGetHelper8[]="get m";
    if(0== memcmp(u8array,chGetHelper,5))
    {    
        PCTPrintCurrentValues(u8array,length);
    }
    if(0== memcmp(u8array,chGetHelper2,5))
    {    
        PCTPrintBaudRate(u8array,length);
    }
    if(0== memcmp(u8array,chGetHelper3,5))
    {    
        PCTPrintVersion(u8array,length);
    }
    if(0== memcmp(u8array,chGetHelper4,5))
    {    
        PCTPrintPDO(u8array,length);
    }
    if(0== memcmp(u8array,chGetHelper5,7))
    {    
        PCTPrintStatusData(u8array,length);
    }
    if(0== memcmp(u8array,chGetHelper8,5))
    {    
        PCTPrintMemory(u8array,length);
    }
       
}



/* Function process set commands*/

void PCTSetProcessCommands(UINT8 u8array[], UINT8 length)
{
    char chSetBaudHelper[]="set b";
    char chSetMemHelper[]="set m";
    char chSetPDOHelper[]="set p";
    
    if(0== memcmp(u8array,chSetBaudHelper,5))
    {    
        PCTSetBaudRate(u8array,length);
    }
    if(0== memcmp(u8array,chSetMemHelper,5))
    {    
        PCTSetMemory(u8array,length);
    }
    if(0== memcmp(u8array,chSetPDOHelper,5))
    {    
        PCTSetPDO(u8array,length);
    }
     
}

void PCTExitProcessCommands(UINT8 u8array[], UINT8 length)
{
    UINT8 StrPrint[]="> exit\n\n\r> exiting PSF Control Terminal\n\n\r";
    SERCOM1_USART_Write(&StrPrint[0],sizeof(StrPrint)); 
}
void PCTCommandhandler(UINT8 u8array[], UINT8 length)
{
    
    UINT8 chGetHelper[] ="get";
    UINT8 chSetHelper[]="set";
    UINT8 chExitHelper[]="exit";
    UINT8 chHelpHelper[]="help";
    if(0 == memcmp(u8array, chGetHelper, 3))
    {
        /*processes commands starting with "get"*/
        PCTGetProcessCommands(u8array,length);
    }  
    else if(0== memcmp(u8array, chSetHelper, 3))
    {
        /*processes commands staring with "set"*/
         PCTSetProcessCommands(u8array, length);
    }
    else if(0== memcmp(u8array, chExitHelper, 4))
    {
        
        /*processes exit command*/
         PCTExitProcessCommands(u8array, length);
    }
    else if(0== memcmp(u8array, chHelpHelper, 4))
    {
        
        /*processes help command*/
        PCTPrintCommands();
    }
    else
    {
        UINT8 StrPrint[]="> invalid command! Type help to know more\n\n\r";
        SERCOM1_USART_Write(&StrPrint[0],sizeof(StrPrint)); 
    }
}
void MchpPSF_PCTRUN(bool bBlocking)
{
    UINT8 u8array[40];    
    UINT8 u8Length = 0;
    UINT8 u8ReadByte;
    static STATE state = INIT;
    
    do{
        switch(state)
        {
            case INIT:
                /*checks if UART receiver is ready and reads if it ready*/
                if(true == SERCOM1_USART_ReceiverIsReady())
                {
                    u8array[u8Length] = PCTReadByte();
                    
                    
                    
                    if((u8array[u8Length]=='?') || (u8array[u8Length]==0x0d))
                    {
                        /*checks if '?' or enter is pressed*/
                    
                        state = READ_PROCESS;

                    }

                    else if(u8array[u8Length]==0x20)
                    {
                        /*space key press toggles the operation going on.
                         when UART debug is going on, it toggles to Command prompt operation and vice versa*/
                    
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
                    }
                    if(u8ReadByte==' ' &&(u8array[u8Length]==' '))
                    {
                        continue;
                    }
                    u8array[u8Length]=u8ReadByte;
                    u8Length++;
                }

                 state = READ_CONTINUE;
            break;

            case READ_PROCESS:
                if(u8array[u8Length] =='?')
                {
                    PCTPrintCommands();
                    u8Length=0;

                }
                else
                {
                  
                    PCTCommandhandler(u8array, u8Length);
                    u8Length=0;
                }
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
                
                state = INIT;
            break;
        }
    }while(bBlocking);
}


void MchpPSF_PCTInit()
{
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


