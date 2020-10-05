/*******************************************************************************
 User Interface File

  Company:
    Microchip Technology Inc.

  File Name:
    psf_control_terminal.c

  Summary:
    This file interacts with user through the terminal.

  Description:
    This file is based on UART functionality which helps the user to configure various parameters on the stack.
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

#include <stdbool.h>		// Defines true
#include <stdlib.h>		// Defines EXIT_FAILURE
#include "definitions.h"	// SYS function prototypes
#include "psf_stdinc.h"		// PSF include file
#include "psf_control_terminal.h"
#define LENGTH 40

/*
 * Function PCTReadByte
 * 
 * This function converts Uppercase alphabets to Lowercase if given by the 
 * user as per point number 7 of section 2.10 in the design document
 * 
 * RETURNS the ReadByte from the terminal 
 */
UINT8 PCTReadByte(void)
{
	INT32 i32ReadByte;
	i32ReadByte = SERCOM1_USART_ReadByte();

	if ((i32ReadByte > 0x40) && (i32ReadByte < 0x5B)) 
    {
		/*
		 * Convert Uppercase to Lowercase Alphabet 
		 */
		i32ReadByte = i32ReadByte + 0x20;
	}
	SERCOM1_USART_WriteByte(i32ReadByte);
	return ((UINT8) (i32ReadByte));
}

/*
 * Function writes when '?' is the input 
 */
void PCTPrintCommands()
{
	UINT8 u8aPrintString[] =
	    "?\n\n\r> get version - Gets the current version\n\n\r> get pdo - Gets the sink power details\n\n\r> "
	    "get sr[GlobalCfgStatusData] - Gets the config status data\n\n\r> get sr[gasCfgStatusData] - Gets the gas config status\n\n\r> get sr[PortCfgStatus] - Gets the port Config Status\n\n\r> "
	    "get mem[memory_address][length] -  Reads the memory at the given location\n\n\r> set mem[memory_address][byte value] - Writes memory to SAMD20\n\n\r> "
	    "set pdo[position][value] -  Insert a new PDO at the specified location\n\n\r";
	(void)SERCOM1_USART_Write(&u8aPrintString[0], sizeof(u8aPrintString));
    (void)SERCOM1_USART_Write((UINT8 *)"\n\n\r> ",5);
}

/*
 * Function: PCTWrite
 * 
 * This function writes names of variables and their respective values
 * according to their size.
 * 
 */
void PCTWrite(const UINT8 * u8aPrintString, const UINT8 * u8aValue,UINT8 u8Size, UINT8 u8StrSize)
{
	UINT8 u8Index = (u8Size - 1);
	(void)SERCOM1_USART_Write((void *)u8aPrintString, u8StrSize);
	while (u8Size--) 
    {
		SERCOM1_USART_WriteByte(u8aValue[u8Index]);
		u8Index--;
	}
}

UINT8 *HextoAscii(UINT32 u32HexVal, UINT8 u8Length)
{
	UINT8 u8Index = 0, u8Size = 0;
	UINT8 u8Digit = 0;
	static UINT8 u8aAsciiString[8];
    memset(u8aAsciiString,0,8);
	while (u8Size <= u8Length) 
    {
		/*
		 * Gets the 1st digit 
		 */
		u8Digit = u32HexVal & 0x0F;
		if (u8Digit <= 0x09) 
        {
			/*
			 * conversion to ascii 
			 */
			u8Digit = u8Digit + 0x30;
			u8aAsciiString[u8Index] = u8Digit;
			u8Index++;
		}

		else if (u8Digit >= 0x0A && u8Digit <= 0x0F) 
        {
			/*
			 * conversion to ascii 
			 */
			u8Digit = u8Digit + 0x57;
			u8aAsciiString[u8Index] = u8Digit;
			u8Index++;
		}
		u32HexVal = u32HexVal >> 0x04;
		u8Size++;
	}
	return u8aAsciiString;
}

void PCTInvalidCommandMsg()
{
	UINT8 u8aPrintString[] = "\n\n\r> invalid command! Type help to know more\n\n\r";
	(void)SERCOM1_USART_Write(&u8aPrintString[0], sizeof(u8aPrintString));
    (void)SERCOM1_USART_Write((UINT8 *)"\n\n\r> ",5);
}

/*
 * Function: PCTPrintVersion
 * 
 * This function writes Demo version and firmware version onto the
 * terminal.
 * 
 */
void PCTPrintVersion()
{
	UINT32 u32FirmwareVersion, u32DemoVersion;
	UINT8 *pu8PrintValue;
	UINT8 u8aDemoVersionString[] = "\n\n\r> demo version=";
	UINT8 u8aPSFVersionString[] = "\n\n\r> PSF version=";
	u32DemoVersion = DEMO_VERSION;
	u32FirmwareVersion = SYSTEM_FW_REV;

	pu8PrintValue = HextoAscii(u32DemoVersion, sizeof(u32DemoVersion));
	PCTWrite(u8aDemoVersionString, (UINT8 *) & pu8PrintValue[0],\
		 sizeof(pu8PrintValue), sizeof(u8aDemoVersionString));

	pu8PrintValue = HextoAscii(u32FirmwareVersion, sizeof(u32FirmwareVersion));
	PCTWrite(u8aPSFVersionString, (UINT8 *) & pu8PrintValue[0],\
		 sizeof(pu8PrintValue), sizeof(u8aPSFVersionString));
    (void)SERCOM1_USART_Write((UINT8 *)"\n\n\r> ",5);
}

void PCTPrintSinkPDO()
{
	UINT8 *pu8PrintValue=0;
	UINT32 *pu32PDOList;
	UINT32 u32Data;
	UINT8 u8Index;
	UINT8 u8aVoltageString[] = "\rVoltage=";
	UINT8 u8aCurrentString[] = "\rCurrent=";

	pu32PDOList = gasCfgStatusData.sPerPortData[PORT0].u32aSinkPDO;

	for (u8Index = SET_TO_ZERO; u8Index < DPM_MAX_PDO_CNT; u8Index++) 
    {
		pu8PrintValue = HextoAscii(u8Index,sizeof(u8Index));
		PCTWrite((UINT8 *) "\n\n\r SinkPDO -", (UINT8 *) pu8PrintValue,\
			 sizeof(pu8PrintValue), 16);

		// Get the voltage form PDO list
		u32Data = DPM_GET_VOLTAGE_FROM_PDO_MILLI_V(pu32PDOList[u8Index]);
		pu8PrintValue = HextoAscii(u32Data, sizeof(u32Data));
		PCTWrite(u8aVoltageString, (UINT8 *) pu8PrintValue,\
			 sizeof(pu8PrintValue), sizeof(u8aVoltageString));

		// Get the current form PDO list
		u32Data = DPM_GET_PDO_CURRENT(pu32PDOList[u8Index]);
		pu8PrintValue = HextoAscii(u32Data, sizeof(u32Data));
		PCTWrite(u8aCurrentString, (UINT8 *) pu8PrintValue,\
			 sizeof(pu8PrintValue), sizeof(u8aCurrentString));
	}
    (void)SERCOM1_USART_Write((UINT8 *)"\n\n\r> ",5);
}

/*
 * Function: PCTPrintPartnerPDO
 * 
 * This function writes Partner PDO values onto the terminal.
 * 
 */
void PCTPrintPartnerPDO()
{
	UINT8 *pu8PrintValue;
	UINT32 *pu32PDOList;
	UINT32 u32Data;
	UINT8 u8Index;
	UINT8 u8aVoltageString[] = "\rVoltage=";
	UINT8 u8aCurrentString[] = "\rCurrent=";

	pu32PDOList = gasCfgStatusData.sPerPortData[PORT0].u32aPartnerPDO;

	for (u8Index = SET_TO_ZERO; u8Index < DPM_MAX_PDO_CNT; u8Index++) 
    {
		pu8PrintValue = HextoAscii(u8Index, sizeof(u8Index));
		PCTWrite((UINT8 *) "\n\n\r> PartnerPDO -",\
			 (UINT8 *) pu8PrintValue, sizeof(pu8PrintValue), 18);

		// Get the voltage form PDO list
		u32Data = DPM_GET_VOLTAGE_FROM_PDO_MILLI_V(pu32PDOList[u8Index]);
		pu8PrintValue = HextoAscii(u32Data, sizeof(u32Data));
		PCTWrite(u8aVoltageString, (UINT8 *) pu8PrintValue,\
			 sizeof(pu8PrintValue), sizeof(u8aVoltageString));

		// Get the current form PDO list
		u32Data = DPM_GET_PDO_CURRENT(pu32PDOList[u8Index]);
		pu8PrintValue = HextoAscii(u32Data, sizeof(u32Data));
		PCTWrite(u8aCurrentString, (UINT8 *) pu8PrintValue,\
			 sizeof(pu8PrintValue), sizeof(u8aCurrentString));
	}
    (void)SERCOM1_USART_Write((UINT8 *)"\n\n\r> ",5);
}

/*
 * Function: PCTPrintNewSinkPDO
 * 
 * This function writes New PDO values onto the terminal.
 * 
 */
void PCTPrintNewSinkPDO()
{
	UINT8 *pu8PrintValue;
	UINT32 *pu32PDOList;
	UINT32 u32Data;
	UINT8 u8Index;
	UINT8 u8aVoltageString[] = "\rVoltage=";
	UINT8 u8aCurrentString[] = "\rCurrent=";

	// If it is in New PDO list then get it from New list else get it from 
	// default
	if (gasCfgStatusData.sPerPortData[PORT0].u8NewSinkPDOCnt) 
    {
		pu32PDOList =
		    gasCfgStatusData.sPerPortData[PORT0].u32aNewSinkPDO;
        for (u8Index = SET_TO_ZERO; u8Index < gasCfgStatusData.sPerPortData[PORT0].u8NewSinkPDOCnt ; u8Index++) 
        {
            pu8PrintValue = HextoAscii(u8Index, sizeof(u8Index));
            PCTWrite((UINT8 *) "\n\n\r> NewSinkPDO -",\
                 (UINT8 *) pu8PrintValue, sizeof(pu8PrintValue), 18);

            // Get the voltage form PDO list
            u32Data = DPM_GET_VOLTAGE_FROM_PDO_MILLI_V(pu32PDOList[u8Index]);
            pu8PrintValue = HextoAscii(u32Data, sizeof(u32Data));
            PCTWrite(u8aVoltageString, (UINT8 *) pu8PrintValue,\
                 sizeof(pu8PrintValue), sizeof(u8aVoltageString));

            // Get the current form PDO list
            u32Data = DPM_GET_PDO_CURRENT(pu32PDOList[u8Index]);
            pu8PrintValue = HextoAscii(u32Data, sizeof(u32Data));
            PCTWrite(u8aCurrentString, (UINT8 *) pu8PrintValue,\
                 sizeof(pu8PrintValue), sizeof(u8aCurrentString));
        }
        (void)SERCOM1_USART_Write((UINT8 *)"\n\n\r> ",5);
	} 
    else 
    {
		pu32PDOList = gasCfgStatusData.sPerPortData[PORT0].u32aSinkPDO;
	}

	for (u8Index = SET_TO_ZERO; u8Index < DPM_MAX_PDO_CNT; u8Index++) 
    {
		pu8PrintValue = HextoAscii(u8Index, sizeof(u8Index));
		PCTWrite((UINT8 *) "\n\n\r> NewSinkPDO -",\
			 (UINT8 *) pu8PrintValue, sizeof(pu8PrintValue), 18);

		// Get the voltage form PDO list
		u32Data = DPM_GET_VOLTAGE_FROM_PDO_MILLI_V(pu32PDOList[u8Index]);
		pu8PrintValue = HextoAscii(u32Data, sizeof(u32Data));
		PCTWrite(u8aVoltageString, (UINT8 *) pu8PrintValue,\
			 sizeof(pu8PrintValue), sizeof(u8aVoltageString));

		// Get the current form PDO list
		u32Data = DPM_GET_PDO_CURRENT(pu32PDOList[u8Index]);
		pu8PrintValue = HextoAscii(u32Data, sizeof(u32Data));
		PCTWrite(u8aCurrentString, (UINT8 *) pu8PrintValue,\
			 sizeof(pu8PrintValue), sizeof(u8aCurrentString));
	}
    (void)SERCOM1_USART_Write((UINT8 *)"\n\n\r> ",5);
}

/*
 * Function: PCTPrintAdvertisedPDO
 * 
 * This function writes Advertised PDO values onto the terminal.
 * 
 */
void PCTPrintAdvertisedPDO()
{
	UINT8 *pu8PrintValue;
	UINT32 *pu32PDOList;
	UINT32 u32Data;
	UINT8 u8Index;
	UINT8 u8aVoltageString[] = "\rVoltage=";
	UINT8 u8aCurrentString[] = "\rCurrent=";

	// If it is already advertised then get it from advertised list else
	// get it from default
	if (gasCfgStatusData.sPerPortData[PORT0].u8AdvertisedPDOCnt) 
    {
		pu32PDOList = gasCfgStatusData.sPerPortData[PORT0].u32aAdvertisedPDO;
        for (u8Index = SET_TO_ZERO; u8Index <gasCfgStatusData.sPerPortData[PORT0].u8AdvertisedPDOCnt; u8Index++) 
        {
            pu8PrintValue = HextoAscii(u8Index, sizeof(u8Index));
            PCTWrite((UINT8 *) "\n\n\r> Advertised PDO -", (UINT8 *) pu8PrintValue,\
                 sizeof(pu8PrintValue), 11);

            // Get the voltage form PDO list
            u32Data = DPM_GET_VOLTAGE_FROM_PDO_MILLI_V(pu32PDOList[u8Index]);
            pu8PrintValue = HextoAscii(u32Data, sizeof(u32Data));
            PCTWrite(u8aVoltageString, (UINT8 *) pu8PrintValue,\
                 sizeof(pu8PrintValue), sizeof(u8aVoltageString));

            // Get the current form PDO list
            u32Data = DPM_GET_PDO_CURRENT(pu32PDOList[u8Index]);
            pu8PrintValue = HextoAscii(u32Data, sizeof(u32Data));
            PCTWrite(u8aCurrentString, (UINT8 *) pu8PrintValue,\
                 sizeof(pu8PrintValue), sizeof(u8aCurrentString));
        }
	}
    (void)SERCOM1_USART_Write((UINT8 *)"\n\n\r> ",5);
}
/*
 * Function: PCTPrintCableIdentity
 * 
 * This function writes Cable Identity values onto the terminal.
 * 
 */
void PCTPrintCableIdentity()
{
	UINT8 *pu8PrintValue;
	UINT32 *pu32CableIdentityList;
	UINT8 u8Index;
	UINT8 u8aCableString[] = "\rCableIdentity=";

	pu32CableIdentityList = gasCfgStatusData.sPerPortData[PORT0].u32aCableIdentity;

	for (u8Index = SET_TO_ZERO; u8Index < DPM_MAX_PDO_CNT; u8Index++) 
    {
		pu8PrintValue = HextoAscii(u8Index, sizeof(u8Index));
		PCTWrite((UINT8 *) "\n\n\r> ", (UINT8 *) pu8PrintValue,\
			 sizeof(pu8PrintValue), 4);

		pu8PrintValue = HextoAscii(pu32CableIdentityList[u8Index], 8);
		PCTWrite(u8aCableString, (UINT8 *) pu8PrintValue,\
			 sizeof(pu8PrintValue), sizeof(u8aCableString));
	}
}

/*
 * Function: PCTPrintPDO
 * 
 * This function writes Sink PDO values onto the terminal.
 * 
 */
void PCTPrintPDO()
{
	UINT8 *pu8PrintValue;
	UINT32 *pu32PDOList;
	UINT32 u32Data;
	UINT8 u8Index;
	UINT8 u8aVoltageString[] = "\rVoltage=";
	UINT8 u8aCurrentString[] = "\rCurrent=";

	// If it is already advertised then get it from advertised list else
	// get it from default
	if (gasCfgStatusData.sPerPortData[PORT0].u8AdvertisedPDOCnt) {
		pu32PDOList = gasCfgStatusData.sPerPortData[PORT0].u32aAdvertisedPDO;
        for (u8Index = SET_TO_ZERO; u8Index < gasCfgStatusData.sPerPortData[PORT0].u8AdvertisedPDOCnt; u8Index++) 
        {
            pu8PrintValue = HextoAscii(u8Index, sizeof(u8Index));
            PCTWrite((UINT8 *) "\n\n\r> AdvertisedPDO -", (UINT8 *) pu8PrintValue,\
                 sizeof(pu8PrintValue), 20);

            // Get the voltage form PDO list
            u32Data = DPM_GET_VOLTAGE_FROM_PDO_MILLI_V(pu32PDOList[u8Index]);
            pu8PrintValue = HextoAscii(u32Data, sizeof(u32Data));
            PCTWrite(u8aVoltageString, (UINT8 *) pu8PrintValue,\
                 sizeof(pu8PrintValue), sizeof(u8aVoltageString));

            // Get the current form PDO list
            u32Data = DPM_GET_PDO_CURRENT(pu32PDOList[u8Index]);
            pu8PrintValue = HextoAscii(u32Data, sizeof(u32Data));
            PCTWrite(u8aCurrentString, (UINT8 *) pu8PrintValue,\
                 sizeof(pu8PrintValue), sizeof(u8aCurrentString));
        }
        
	} 
    else 
    {
		pu32PDOList = gasCfgStatusData.sPerPortData[PORT0].u32aSinkPDO;

        for (u8Index = SET_TO_ZERO; u8Index < DPM_MAX_PDO_CNT; u8Index++) 
        {
            pu8PrintValue = HextoAscii(u8Index, sizeof(u8Index));
            PCTWrite((UINT8 *) "\n\n\r> PDO -", (UINT8 *) pu8PrintValue,\
                 sizeof(pu8PrintValue), 11);

            // Get the voltage form PDO list
            u32Data = DPM_GET_VOLTAGE_FROM_PDO_MILLI_V(pu32PDOList[u8Index]);
            pu8PrintValue = HextoAscii(u32Data, sizeof(u32Data));
            PCTWrite(u8aVoltageString, (UINT8 *) pu8PrintValue,\
                 sizeof(pu8PrintValue), sizeof(u8aVoltageString));

            // Get the current form PDO list
            u32Data = DPM_GET_PDO_CURRENT(pu32PDOList[u8Index]);
            pu8PrintValue = HextoAscii(u32Data, sizeof(u32Data));
            PCTWrite(u8aCurrentString, (UINT8 *) pu8PrintValue,\
                 sizeof(pu8PrintValue), sizeof(u8aCurrentString));
        }
    }
    (void)SERCOM1_USART_Write((UINT8 *)"\n\n\r> ",5);
}

/*
 * Function: PCTPrintStatusData
 * 
 * This function writes GlobalConfigStatusdata and PortConfigStatusData
 * values onto the terminal.
 * 
 */

void PCTPrintStatusData(const UINT8 u8array[])
{
	UINT8 u8aGlobalHelper[] = "get sr[globalcfgstatusdata]";
	UINT8 u8aPortHelper[] = "get sr[portcfgstatus]";
	UINT8 *pu8PrintValue;
	if (0 == memcmp(u8array, u8aGlobalHelper, 27)) 
    {
		UINT8 StrMinorVersion[] = "\n\n\r>UINT8 u8MinorVersion=";
		UINT8 StrMajorVersion[] = "\n\n\r>UINT8 u8MajorVersion=";
		UINT8 StrHWVersion[] = "\n\n\r>UINT8 u8HWVersion";
		UINT8 StrSiVersion[] = "\n\n\r>UINT8 u8SiVersion";
		UINT8 StrManfString[] = "\n\n\r>UINT8 u8aManfString";
		UINT8 StrPSFMajorVersion[] = "\n\n\r>UINT8 u8PSFMajorVersion";
		UINT8 StrPSFMinorVersion[] = "\n\n\r>UINT8 u8PSFMinorVersion";
		UINT8 StrPwrThrottleCfg[] = "\n\n\r>UINT8 u8PwrThrottleCfg";
		UINT8 StrProducdID[] = "\n\n\r>UINT16 u16ProducdID";
		UINT8 StrVendorID[] = "\n\n\r>UINT16 u16VendorID";
		UINT8 StrProductTypeVDO[] = "\n\n\r>UINT16 u16ProductTypeVDO";
		UINT8 StrProductVDO[] = "\n\n\r>UINT16 u16ProductVDO";
		UINT8 StrCertStatVDO[] = "\n\n\r>UINT16 u16CertStatVDO";
		UINT8 StrIDHeaderVDO[] = "\n\n\r>UINT16 u16IDHeaderVDO";

		pu8PrintValue = HextoAscii(gasCfgStatusData.u8MinorVersion, 4);
		PCTWrite(StrMinorVersion, (UINT8 *) & pu8PrintValue[0],\
			 sizeof(pu8PrintValue), sizeof(StrMinorVersion));

		pu8PrintValue = HextoAscii(gasCfgStatusData.u8MajorVersion, 4);
		PCTWrite(StrMajorVersion, (UINT8 *) & pu8PrintValue[0],\
			 sizeof(pu8PrintValue), sizeof(StrMajorVersion));

		pu8PrintValue = HextoAscii(gasCfgStatusData.u8HWVersion, 4);
		PCTWrite(StrHWVersion, (UINT8 *) & pu8PrintValue[0],\
			 sizeof(pu8PrintValue), sizeof(StrHWVersion));

		pu8PrintValue = HextoAscii(gasCfgStatusData.u8SiVersion, 4);
		PCTWrite(StrSiVersion, (UINT8 *) & pu8PrintValue[0],\
			 sizeof(pu8PrintValue), sizeof(StrSiVersion));

		(void)SERCOM1_USART_Write(&StrManfString[0],sizeof(StrManfString));
		(void)SERCOM1_USART_Write(&gasCfgStatusData.u8aManfString[0],\
					  gasCfgStatusData.u8ManfStringLength);

		pu8PrintValue = HextoAscii(gasCfgStatusData.u8PSFMajorVersion, 4);
		PCTWrite(StrPSFMajorVersion, (UINT8 *) & pu8PrintValue[0],\
			 sizeof(pu8PrintValue), sizeof(StrPSFMajorVersion));

		pu8PrintValue = HextoAscii(gasCfgStatusData.u8PSFMinorVersion, 4);
		PCTWrite(StrPSFMinorVersion, (UINT8 *) & pu8PrintValue[0],\
			 sizeof(pu8PrintValue), sizeof(StrPSFMinorVersion));

		pu8PrintValue = HextoAscii(gasCfgStatusData.u8PwrThrottleCfg, 4);
		PCTWrite(StrPwrThrottleCfg, (UINT8 *) & pu8PrintValue[0],\
			 sizeof(pu8PrintValue), sizeof(StrPwrThrottleCfg));

		pu8PrintValue = HextoAscii(gasCfgStatusData.u16ProducdID, 4);
		PCTWrite(StrProducdID, (UINT8 *) & pu8PrintValue[0],\
			 sizeof(pu8PrintValue), sizeof(StrProducdID));

		pu8PrintValue = HextoAscii(gasCfgStatusData.u16VendorID, 4);
		PCTWrite(StrVendorID, (UINT8 *) & pu8PrintValue[0],\
			 sizeof(pu8PrintValue), sizeof(StrVendorID));

		pu8PrintValue = HextoAscii(gasCfgStatusData.u16ProductTypeVDO, 4);
		PCTWrite(StrProductTypeVDO, (UINT8 *) & pu8PrintValue[0],\
			 sizeof(pu8PrintValue), sizeof(StrProductTypeVDO));

		pu8PrintValue = HextoAscii(gasCfgStatusData.u16ProductVDO, 4);
		PCTWrite(StrProductVDO, (UINT8 *) & pu8PrintValue[0],\
			 sizeof(pu8PrintValue), sizeof(StrProductVDO));

		pu8PrintValue = HextoAscii(gasCfgStatusData.u16CertStatVDO, 4);
		PCTWrite(StrCertStatVDO, (UINT8 *) & pu8PrintValue[0],\
			 sizeof(pu8PrintValue), sizeof(StrCertStatVDO));

		pu8PrintValue = HextoAscii(gasCfgStatusData.u16IDHeaderVDO, 4);
		PCTWrite(StrIDHeaderVDO, (UINT8 *) & pu8PrintValue[0],\
			 sizeof(pu8PrintValue), sizeof(StrIDHeaderVDO));
        (void)SERCOM1_USART_Write((UINT8 *)"\n\n\r> ",5);
	} 
    else if (0 == memcmp(u8array, u8aPortHelper, 21)) 
    {
		UINT8 u8Index;
		UINT8 StrCfgData[] = "\n\n\r>UINT32 u32CfgData=";
		UINT8 StrRDOVoltage[] = "\n\n\r>UINT32 u32RDOVoltage=";
		UINT8 StrRDOCurrent[] = "\n\n\r>UINT32 u32RDOCurrent=";
		UINT8 StrPortConnectStatus[] = "\n\n\r>UINT32 u32PortConnectStatus=";
		UINT8 StrPortStatusChange[] = "\n\n\r>UINT32 u32PortStatusChange=";
		UINT8 StrPortIOStatus[] = "\n\n\r>UINT32 u32PortIOStatus=";
		UINT8 u32ClientRequest[] = "\n\n\r>UINT32 u32ClientRequest=";
		UINT8 u16AllocatedPowerIn250mW[] = "\n\n\r>UINT16 u16AllocatedPowerIn250mW=";
		UINT8 u16NegoVoltageInmV[] = "\n\n\r>UINT16 u16NegoVoltageInmV=";
		UINT8 u16NegoCurrentInmA[] = "\n\n\r>UINT16 u16NegoCurrentInmA=";
		UINT8 u16MaxSrcPrtCurrentIn10mA[] = "\n\n\r>UINT16 u16MaxSrcPrtCurrentIn10mA=";
		UINT8 u16PortIntrMask[] = "\n\n\r>UINT16 u16PortIntrMask=";
		UINT8 u16FeatureSelect[] = "\n\n\r>UINT16 u16FeatureSelect=";
		UINT8 u16SwapPolicy[] = "\n\n\r>UINT16 u16SwapPolicy=";
		UINT8 StrPowerGoodTimerInms[] = "\n\n\r>UINT16 u16PowerGoodTimerInms=";
		UINT8 StraMinPDOPreferredCurInmA[] = "\n\n\r>UINT16 u16aMinPDOPreferredCurInmA=";
		UINT8 StrSnkMaxOperatingCurInmA[] = "\n\n\r>UINT16 u16SnkMaxOperatingCurInmA=";
		UINT8 StrSnkMinOperatingCurInmA[] = "\n\n\r>UINT16 u16SnkMinOperatingCurInmA=";
		UINT8 StrDAC_I_MaxOutVoltInmV[] = "\n\n\r>UINT16 u16DAC_I_MaxOutVoltInmV=";
		UINT8 StrDAC_I_MinOutVoltInmV[] = "\n\n\r>UINT16 u16DAC_I_MinOutVoltInmV=";
		UINT8 StrDAC_I_CurrentInd_MaxInA[] = "\n\n\r>UINT16 u16DAC_I_CurrentInd_MaxInA=";
		UINT8 StrSinkPDOCnt[] = "\n\n\r>UINT8 u8SinkPDOCnt=";
		UINT8 StrNewPDOCnt[] = "\n\n\r>UINT8 u8NewPDOCnt=";
		UINT8 StrAdvertisedPDOCnt[] = "\n\n\r>UINT8 u8AdvertisedPDOCnt=";
		UINT8 StrSinkConfigSel[] = "\n\n\r>UINT8 u8SinkConfigSel=";
		UINT8 StrFaultInDebounceInms[] = "\n\n\r>UINT8 u8FaultInDebounceInms=";
		UINT8 StrOCSThresholdPercentage[] = "\n\n\r>UINT8 u8OCSThresholdPercentage=";
		UINT8 StrOVThresholdPercentage[] = "\n\n\r>UINT8 u8OVThresholdPercentage=";
		UINT8 StrUVThresholdPercentage[] = "\n\n\r>UINT8 u8UVThresholdPercentage=";
		UINT8 StrVCONNOCSDebounceInms[] = "\n\n\r>UINT8 u8VCONNOCSDebounceInms=";
		UINT8 StrVBUSMaxFaultCnt[] = "\n\n\r>UINT8 u8VBUSMaxFaultCnt=";
		UINT8 StrVCONNMaxFaultCnt[] = "\n\n\r>UINT8 u8VCONNMaxFaultCnt=";
		UINT8 StrPio_FAULT_IN[] = "\n\n\r>UINT8 u8Pio_FAULT_IN=";
		UINT8 StrMode_FAULT_IN[] = "\n\n\r>UINT8 u8Mode_FAULT_IN=";
		UINT8 StrPio_EN_SINK[] = "\n\n\r>UINT8 u8Pio_EN_SINK=";
		UINT8 StrMode_EN_SINK[] = "\n\n\r>UINT8 u8Mode_EN_SINK=";
		UINT8 StrDAC_I_Direction[] = "\n\n\r>UINT8 u8DAC_I_Direction=";
		UINT8 StrPartnerPDOCnt[] = "\n\n\r>UINT8 u8PartnerPDOCnt";

		pu8PrintValue = HextoAscii(gasCfgStatusData.sPerPortData[PORT0].u32CfgData,\
			       sizeof(gasCfgStatusData.sPerPortData[PORT0].u32CfgData));
		PCTWrite(StrCfgData, (UINT8 *) & pu8PrintValue[0],\
			 sizeof(pu8PrintValue), sizeof(StrCfgData));

		PCTPrintSinkPDO();
		PCTPrintNewSinkPDO();
		PCTPrintAdvertisedPDO();
		PCTPrintPartnerPDO();
		PCTPrintCableIdentity();

		pu8PrintValue = HextoAscii(DPM_GET_OP_VOLTAGE_FROM_PROG_RDO_IN_mV \
			       (gasCfgStatusData.sPerPortData[PORT0].u32RDO),\
			       sizeof(gasCfgStatusData.sPerPortData[PORT0].u32CfgData));
		PCTWrite(StrRDOVoltage, (UINT8 *) & pu8PrintValue[0],\
			 sizeof(pu8PrintValue), sizeof(StrRDOVoltage));

		pu8PrintValue = HextoAscii(DPM_GET_PROG_RDO_OPR_CURRENT_IN_mA \
			       (gasCfgStatusData.sPerPortData[PORT0].u32RDO),\
			       sizeof(gasCfgStatusData.sPerPortData[PORT0].u32CfgData));
		PCTWrite(StrRDOCurrent, (UINT8 *) & pu8PrintValue[0],\
			 sizeof(pu8PrintValue), sizeof(StrRDOCurrent));

		pu8PrintValue = HextoAscii(gasCfgStatusData.sPerPortData[PORT0].
                u32PortConnectStatus, 4);
		PCTWrite(StrPortConnectStatus, (UINT8 *) & pu8PrintValue[0],\
			 sizeof(pu8PrintValue), sizeof(StrPortConnectStatus));

		pu8PrintValue = HextoAscii(gasCfgStatusData.sPerPortData[PORT0].
                u32PortStatusChange, 4);
		PCTWrite(StrPortStatusChange, (UINT8 *) & pu8PrintValue[0],\
			 sizeof(pu8PrintValue), sizeof(StrPortStatusChange));

		pu8PrintValue = HextoAscii(gasCfgStatusData.sPerPortData[PORT0].
                u32PortIOStatus, 4);
		PCTWrite(StrPortIOStatus, (UINT8 *) & pu8PrintValue[0],\
			 sizeof(pu8PrintValue), sizeof(StrPortIOStatus));

		pu8PrintValue = HextoAscii(gasCfgStatusData.sPerPortData[PORT0].
			       u32ClientRequest, 4);
		PCTWrite(u32ClientRequest, (UINT8 *) & pu8PrintValue[0],\
			 sizeof(pu8PrintValue), sizeof(u32ClientRequest));

		pu8PrintValue = HextoAscii(gasCfgStatusData.sPerPortData[PORT0].
			       u16AllocatedPowerIn250mW, 4);
		PCTWrite(u16AllocatedPowerIn250mW, (UINT8 *) & pu8PrintValue[0],\
			 sizeof(pu8PrintValue), sizeof(u16AllocatedPowerIn250mW));

		pu8PrintValue = HextoAscii(gasCfgStatusData.sPerPortData[PORT0].
			       u16NegoVoltageInmV, 4);
		PCTWrite(u16NegoVoltageInmV, (UINT8 *) & pu8PrintValue[0],\
			 sizeof(pu8PrintValue), sizeof(u16NegoVoltageInmV));

		pu8PrintValue = HextoAscii(gasCfgStatusData.sPerPortData[PORT0].
			       u16NegoCurrentInmA, 4);
		PCTWrite(u16NegoCurrentInmA, (UINT8 *) & pu8PrintValue[0],\
			 sizeof(pu8PrintValue), sizeof(u16NegoCurrentInmA));

		pu8PrintValue = HextoAscii(gasCfgStatusData.sPerPortData[PORT0].
			       u16MaxSrcPrtCurrentIn10mA, 4);
		PCTWrite(u16MaxSrcPrtCurrentIn10mA,\
			 (UINT8 *) & pu8PrintValue[0], sizeof(pu8PrintValue),\
			 sizeof(u16MaxSrcPrtCurrentIn10mA));

		pu8PrintValue = HextoAscii(gasCfgStatusData.sPerPortData[PORT0].
			       u16PortIntrMask, 4);
		PCTWrite(u16PortIntrMask, (UINT8 *) & pu8PrintValue[0],\
			 sizeof(pu8PrintValue), sizeof(u16PortIntrMask));

		pu8PrintValue = HextoAscii(gasCfgStatusData.sPerPortData[PORT0].
			       u16PowerGoodTimerInms, 4);
		PCTWrite(StrPowerGoodTimerInms, (UINT8 *) & pu8PrintValue[0],\
			 sizeof(pu8PrintValue), sizeof(StrPowerGoodTimerInms));

		pu8PrintValue = HextoAscii(gasCfgStatusData.sPerPortData[PORT0].
			       u16FeatureSelect, 4);
		PCTWrite(u16FeatureSelect, (UINT8 *) & pu8PrintValue[0],\
			 sizeof(pu8PrintValue), sizeof(u16FeatureSelect));

		pu8PrintValue = HextoAscii(gasCfgStatusData.sPerPortData[PORT0].
			       u16SwapPolicy, 4);
		PCTWrite(u16SwapPolicy, (UINT8 *) & pu8PrintValue[0],\
			 sizeof(pu8PrintValue), sizeof(u16SwapPolicy));

		for (u8Index = SET_TO_ZERO; u8Index < 7; u8Index++) {
			pu8PrintValue = HextoAscii(gasCfgStatusData.sPerPortData[PORT0].
				       u16aMinPDOPreferredCurInmA[u8Index], 4);
			PCTWrite(StraMinPDOPreferredCurInmA,\
				 (UINT8 *) & pu8PrintValue[0],\
				 sizeof(pu8PrintValue), sizeof(StraMinPDOPreferredCurInmA));
		}

		pu8PrintValue = HextoAscii(gasCfgStatusData.sPerPortData[PORT0].
			       u16SnkMaxOperatingCurInmA, 4);
		PCTWrite(StrSnkMaxOperatingCurInmA,\
			 (UINT8 *) & pu8PrintValue[0], sizeof(pu8PrintValue),\
			 sizeof(StrSnkMaxOperatingCurInmA));

		pu8PrintValue = HextoAscii(gasCfgStatusData.sPerPortData[PORT0].
			       u16SnkMinOperatingCurInmA, 4);
		PCTWrite(StrSnkMinOperatingCurInmA,\
			 (UINT8 *) & pu8PrintValue[0], sizeof(pu8PrintValue),\
			 sizeof(StrSnkMinOperatingCurInmA));

		pu8PrintValue = HextoAscii(gasCfgStatusData.sPerPortData[PORT0].
			       u16DAC_I_MaxOutVoltInmV, 4);
		PCTWrite(StrDAC_I_MaxOutVoltInmV, (UINT8 *) & pu8PrintValue[0],\
			 sizeof(pu8PrintValue), sizeof(StrDAC_I_MaxOutVoltInmV));

		pu8PrintValue = HextoAscii(gasCfgStatusData.sPerPortData[PORT0].
			       u16DAC_I_MinOutVoltInmV, 4);
		PCTWrite(StrDAC_I_MinOutVoltInmV, (UINT8 *) & pu8PrintValue[0],\
			 sizeof(pu8PrintValue), sizeof(StrDAC_I_MinOutVoltInmV));

		pu8PrintValue = HextoAscii(gasCfgStatusData.sPerPortData[PORT0].
			       u16DAC_I_CurrentInd_MaxInA, 4);
		PCTWrite(StrDAC_I_CurrentInd_MaxInA,\
			 (UINT8 *) & pu8PrintValue[0], sizeof(pu8PrintValue),\
			 sizeof(StrDAC_I_CurrentInd_MaxInA));

		pu8PrintValue = HextoAscii(gasCfgStatusData.sPerPortData[PORT0].
			       u8SinkPDOCnt, 4);
		PCTWrite(StrSinkPDOCnt, (UINT8 *) & pu8PrintValue[0],\
			 sizeof(pu8PrintValue), sizeof(StrSinkPDOCnt));

		pu8PrintValue = HextoAscii(gasCfgStatusData.sPerPortData[PORT0].
			       u8NewSinkPDOCnt, 4);
		PCTWrite(StrNewPDOCnt, (UINT8 *) & pu8PrintValue[0],\
			 sizeof(pu8PrintValue), sizeof(StrNewPDOCnt));

		pu8PrintValue = HextoAscii(gasCfgStatusData.sPerPortData[PORT0]. 
			       u8AdvertisedPDOCnt, 4);
		PCTWrite(StrAdvertisedPDOCnt, (UINT8 *) & pu8PrintValue[0],\
			 sizeof(pu8PrintValue), sizeof(StrAdvertisedPDOCnt));

		pu8PrintValue = HextoAscii(gasCfgStatusData.sPerPortData[PORT0].
			       u8PartnerPDOCnt, 4);
		PCTWrite(StrPartnerPDOCnt, (UINT8 *) & pu8PrintValue[0],\
			 sizeof(pu8PrintValue), sizeof(StrPartnerPDOCnt));

		pu8PrintValue = HextoAscii(gasCfgStatusData.sPerPortData[PORT0].
			       u8SinkConfigSel, 4);
		PCTWrite(StrSinkConfigSel, (UINT8 *) & pu8PrintValue[0],\
			 sizeof(pu8PrintValue), sizeof(StrSinkConfigSel));

		pu8PrintValue = HextoAscii(gasCfgStatusData.sPerPortData[PORT0].
			       u8FaultInDebounceInms, 4);
		PCTWrite(StrFaultInDebounceInms, (UINT8 *) & pu8PrintValue[0],
			 sizeof(pu8PrintValue), sizeof(StrFaultInDebounceInms));

		pu8PrintValue = HextoAscii(gasCfgStatusData.sPerPortData[PORT0].
			       u8OCSThresholdPercentage, 4);
		PCTWrite(StrOCSThresholdPercentage,\
			 (UINT8 *) & pu8PrintValue[0], sizeof(pu8PrintValue),\
			 sizeof(StrOCSThresholdPercentage));

		pu8PrintValue = HextoAscii(gasCfgStatusData.sPerPortData[PORT0].
			       u8OVThresholdPercentage, 4);
		PCTWrite(StrOVThresholdPercentage, (UINT8 *) & pu8PrintValue[0],\
			 sizeof(pu8PrintValue), sizeof(StrOVThresholdPercentage));

		pu8PrintValue = HextoAscii(gasCfgStatusData.sPerPortData[PORT0].
			       u8UVThresholdPercentage, 4);
		PCTWrite(StrUVThresholdPercentage, (UINT8 *) & pu8PrintValue[0],\
			 sizeof(pu8PrintValue), sizeof(StrUVThresholdPercentage));

		pu8PrintValue = HextoAscii(gasCfgStatusData.sPerPortData[PORT0].
			       u8VCONNOCSDebounceInms, 4);
		PCTWrite(StrVCONNOCSDebounceInms, (UINT8 *) & pu8PrintValue[0],\
			 sizeof(pu8PrintValue), sizeof(StrVCONNOCSDebounceInms));

		pu8PrintValue = HextoAscii(gasCfgStatusData.sPerPortData[PORT0].
			       u8VBUSMaxFaultCnt, 4);
		PCTWrite(StrVBUSMaxFaultCnt, (UINT8 *) & pu8PrintValue[0],\
			 sizeof(pu8PrintValue), sizeof(StrVBUSMaxFaultCnt));

		pu8PrintValue = HextoAscii(gasCfgStatusData.sPerPortData[PORT0].
			       u8VCONNMaxFaultCnt, 4);
		PCTWrite(StrVCONNMaxFaultCnt, (UINT8 *) & pu8PrintValue[0],\
			 sizeof(pu8PrintValue), sizeof(StrVCONNMaxFaultCnt));

		pu8PrintValue = HextoAscii(gasCfgStatusData.sPerPortData[PORT0].
			       u8Pio_FAULT_IN, 4);
		PCTWrite(StrPio_FAULT_IN, (UINT8 *) & pu8PrintValue[0],\
			 sizeof(pu8PrintValue), sizeof(StrPio_FAULT_IN));

		pu8PrintValue = HextoAscii(gasCfgStatusData.sPerPortData[PORT0].
			       u8Mode_FAULT_IN, 4);
		PCTWrite(StrMode_FAULT_IN, (UINT8 *) & pu8PrintValue[0],\
			 sizeof(pu8PrintValue), sizeof(StrMode_FAULT_IN));

		pu8PrintValue = HextoAscii(gasCfgStatusData.sPerPortData[PORT0].
			       u8Pio_EN_SINK, 4);
		PCTWrite(StrPio_EN_SINK, (UINT8 *) & pu8PrintValue[0],\
			 sizeof(pu8PrintValue), sizeof(StrPio_EN_SINK));

		pu8PrintValue = HextoAscii(gasCfgStatusData.sPerPortData[PORT0].
			       u8Mode_EN_SINK, 4);
		PCTWrite(StrMode_EN_SINK, (UINT8 *) & pu8PrintValue[0],\
			 sizeof(pu8PrintValue), sizeof(StrMode_EN_SINK));

		pu8PrintValue = HextoAscii(gasCfgStatusData.sPerPortData[PORT0].
			       u8DAC_I_Direction, 7);
		PCTWrite(StrDAC_I_Direction, (UINT8 *) & pu8PrintValue[0],\
			 sizeof(pu8PrintValue), sizeof(StrDAC_I_Direction));
        (void)SERCOM1_USART_Write((UINT8 *)"\n\n\r> ",5);
	} 
    else 
    {
		PCTInvalidCommandMsg();
	}

}

UINT32 ASCIIHexStringtoInteger(const UINT8 u8array[], UINT8 u8Length)
{
	UINT8 u8Index = 0, u8Variable = 0;
	UINT32 u32Address = 0;
	while (u8Index < u8Length) 
    {
		if (u8array[u8Index] >= 0x30 && u8array[u8Index] <= 0x39) 
        {
			/*
			 * convert to integer 
			 */
			u8Variable = u8array[u8Index] - 0x30;
			u32Address = ((u32Address << 4) | u8Variable);
		} 
        else if (u8array[u8Index] >= 0x61 && u8array[u8Index] <= 0x66) 
        {
			/*
			 * convert to integer 
			 */
			u8Variable = u8array[u8Index] - 0x57;
			u32Address = ((u32Address << 4) | u8Variable);
		} 
        else 
        {
			u32Address = 0x00;
		}
		u8Index++;

	}
	return u32Address;

}

/*
 * Function: PCTPrintMemory
 * 
 * This function writes the memory of length(given in terminal by user)
 * onto the terminal.
 * 
 */
void PCTPrintMemory(const UINT8 u8array[])
{
	UINT32 u32Address, u32ByteLength;
	UINT8 u8Length = 0, u8Index = 0, u8ByteValStart;
	UINT8 *pu8Addressptr;
	UINT8 *pu8PrintValue;
	while (((u8array[8 + u8Length]) != 0x5d) && (u8Length < 8)) 
    {
        /* MemoryAddress to be  given in the terminal starts from the 8th element of the u8array*
         * u8array - get mem[XXXXX][X]
         *           012345678.....
         */
		u8Length++;

	}
	u32Address = ASCIIHexStringtoInteger(&u8array[8], u8Length);
	u8ByteValStart = 0x0A + u8Length;
	u8Length = 0;
	while ((u8array[u8ByteValStart+u8Length]) != 0x5d && (u8Length <= 0x01)) 
    {
        /* ByteLength to be given in the terminal starts from the second '[' *
         * u8array - set mem[XXXXX][X]
         *           012345678.....(8 + Length of content in 1st bracket + Length of ']' + Length of '[')
         *                         (10 + Length of content in 1st bracket)
         */
		u8Length++;
	}
	u32ByteLength = ASCIIHexStringtoInteger(&u8array[u8ByteValStart], u8Length);
	if (u32Address) 
    {
		pu8Addressptr = (UINT8 *) u32Address;

		for (u8Index = SET_TO_ZERO; u8Index < u32ByteLength; u8Index++) 
        {
			pu8PrintValue = HextoAscii(pu8Addressptr[u8Index], 1);
			PCTWrite((UINT8 *) "\n\n\r> bytevalue=",(UINT8 *) & pu8PrintValue[u8Index], 2, 15);
		}
	}
    (void)SERCOM1_USART_Write((UINT8 *)"\n\n\r> ",5);
}

/*
 * Function: PCTSetMemory
 * 
 * This Function sets the memory with the byte value given on the
 * terminal.
 * 
 */
void PCTSetMemory(const UINT8 u8array[])
{
	UINT32 u32Address, u32ByteValue;
	UINT8 u8Length = 0, u8ByteValStart, u8Index = 0;
	UINT32 *pu32Addressptr;
	while ((u8array[8 + u8Length]) != 0x5d && u8Length < 8) 
    {
        /* MemoryAddress to be set given in the terminal starts from the 8th element of the u8array*
         * u8array - set mem[XXXXX][X]
         *           012345678.....
         */
		u8Length++;
	}

	u32Address = ASCIIHexStringtoInteger(&u8array[8], u8Length);
	u8ByteValStart = 0x0A + u8Length;
	u8Length = 0;

	while ((u8array[u8ByteValStart+u8Length]) != 0x5d) 
    {
		/* ByteValue to be set given in the terminal starts from the second '[' *
         * u8array - set mem[XXXXX][X]
         *           012345678.....(8 + Length of content in 1st bracket + Length of ']' + Length of '[')
         *                         (10 + Length of content in 1st bracket)
         */
		u8Length++;
	}
	u32ByteValue = ASCIIHexStringtoInteger(&u8array[u8ByteValStart], u8Length);

	if (u32Address) 
    {
		pu32Addressptr = (UINT32 *) u32Address;
		pu32Addressptr[u8Index] = u32ByteValue;
	}
}

/*
 * Function: PCTSetPDO
 * 
 * This Function sets the PDO value at the given position(if valid).
 * 
 */

void PCTSetPDO(const UINT8 u8array[])
{
	UINT32 u32PDOPosition, u32PDOValue;
	UINT8 u8Length = 0, u8ByteValStart, u8Index;
	while ((u8array[8 + u8Length]) != 0x5d) 
    {
        /* PDO position to be set given in the terminal starts from the 8th element of the u8array*
         * u8array - set mem[XXXXX][X]
         *           012345678.....
         */
		u8Length++;
	}

	u32PDOPosition = ASCIIHexStringtoInteger(&u8array[8], u8Length);
	u8ByteValStart = 0x0A + u8Length;
	u8Length = 0;

	while ((u8array[u8ByteValStart + u8Length]) != 0x5d) 
    {
        /* PDOValue to be set given in the terminal starts from the second '[' *
         * u8array - set pdo[XXXXX][X]
         *           012345678.....(8 + Length of content in 1st bracket + Length of ']' + Length of '[')
         *                         (10 + Length of content in 1st bracket)
         */
		u8Length++;
	}
	u32PDOValue = ASCIIHexStringtoInteger(&u8array[u8ByteValStart], u8Length);
	// Max PDO size allocated is 7 
	if (u32PDOPosition > 7) 
    {
		DEBUG_PRINT_PORT_STR(PORT0,"PDO_OVERFLOW_ERROR:Supported Pdo Size is 7\r\n");
		return;
	}
	for (u8Index = SET_TO_ZERO;u8Index < gasCfgStatusData.sPerPortData[PORT0].u8SinkPDOCnt;\
	     u8Index++) 
    {
		gasCfgStatusData.sPerPortData[PORT0].u32aNewSinkPDO[u8Index] =
		    gasCfgStatusData.sPerPortData[PORT0].
		    u32aAdvertisedPDO[u8Index];
		// gasCfgStatusData.sPerPortData[PORT0].u8NewSinkPDOCnt++;

	}

	gasCfgStatusData.sPerPortData[PORT0].u32aNewSinkPDO[u32PDOPosition] =
	    u32PDOValue;
	gasCfgStatusData.sPerPortData[PORT0].u8NewSinkPDOCnt =
	    gasCfgStatusData.sPerPortData[PORT0].u8SinkPDOCnt;
	if (gasCfgStatusData.sPerPortData[PORT0].u32aSinkPDO[u32PDOPosition] !=
	    gasCfgStatusData.sPerPortData[PORT0].
	    u32aNewSinkPDO[u32PDOPosition]) 
    {
		/*
		 * User application may request PSF to renegotiate based on
		 * default PDOs
		 * gasCfgStatusData.sPerPortData[u8PortNum].u32aSinkPDO) or
		 * gasCfgStatusData.sPerPortData[u8PortNum].u32aNewSinkPDO).
		 * 
		 * To renegotiate with new PDOs, user application must ensure that
		 * new PDOs
		 * (gasCfgStatusData.sPerPortData[u8PortNum].u32aNewSinkPDO) are
		 * configured and BIT(10) in
		 * gasCfgStatusData.sPerPortData[u8PortNum].u32CfgData variable is
		 * set. Then BIT(5) in
		 * gasCfgStatusData.sPerPortData[u8PortNum].u32ClientRequest
		 * variable needs to be set by the user application. 
		 */
		gasCfgStatusData.sPerPortData[PORT0].u32ClientRequest |=
		    DPM_CLIENT_REQ_RENEGOTIATE;
		gasCfgStatusData.sPerPortData[PORT0].u32CfgData |=
		    DPM_CLIENT_REQ_GET_PARTNER_IDENTITY;
	}
}

/*
 * This function processes Get commands
 */
void PCTGetProcessCommands(const UINT8 u8array[])
{
	UINT8 u8aGetVersionHelper[] = "get version";
	UINT8 u8aGetPDOHelper[] = "get pdo";
	UINT8 u8aGetStatusRegHelper[] = "get sr[";
	UINT8 u8aGetMemhelper[] = "get mem[";

	if (0 == memcmp(u8array, u8aGetVersionHelper, 11)) 
    {
		PCTPrintVersion();
	} 
    else if (0 == memcmp(u8array, u8aGetPDOHelper, 7)) 
    {
		PCTPrintPDO();
	} 
    else if (0 == memcmp(u8array, u8aGetStatusRegHelper, 7)) 
    {
		PCTPrintStatusData(u8array);
	} 
    else if (0 == memcmp(u8array, u8aGetMemhelper, 8)) 
    {
		PCTPrintMemory(u8array);
	} 
    else 
    {
		PCTInvalidCommandMsg();
	}
}

/*
 * This function processes set commands
 */
void PCTSetProcessCommands(const UINT8 u8array[])
{
	UINT8 u8aSetMemHelper[] = "set mem[";
	UINT8 u8aSetPDOHelper[] = "set pdo[";

	if (0 == memcmp(u8array, u8aSetMemHelper, 8)) 
    {
		PCTSetMemory(u8array);
        (void)SERCOM1_USART_Write((UINT8 *)"\n\n\r> ",5);
	} 
    else if (0 == memcmp(u8array, u8aSetPDOHelper, 8)) 
    {
		PCTSetPDO(u8array);
        (void)SERCOM1_USART_Write((UINT8 *)"\n\n\r> ",5);
	} 
    else 
    {
		PCTInvalidCommandMsg();
	}
}

void PCTCommandhandler(const UINT8 u8array[])
{

	UINT8 u8aGetHelper[] = "get";
	UINT8 u8aSetHelper[] = "set";
	UINT8 u8aHelpHelper[] = "help";
	if (0 == memcmp(u8array, u8aGetHelper, 3)) 
    {
		/*
		 * processes commands starting with "get" 
		 */
		PCTGetProcessCommands(u8array);
	} 
    else if (0 == memcmp(u8array, u8aSetHelper, 3)) 
    {
		/*
		 * processes commands staring with "set" 
		 */
		PCTSetProcessCommands(u8array);
	} 
    else if (0 == memcmp(u8array, u8aHelpHelper, 4)) 
    {
		/*
		 * processes help command 
		 */
		PCTPrintCommands();
	} 
    else 
    {
		PCTInvalidCommandMsg();
	}
}

void PCTSpaceBARMsg(PCT_LOCK_STATE lock_state)
{
	UINT8 u8aPrintString[] = "\n\n\r> Welcome to PCT..Hit space bar to exit";
	UINT8 u8aPrintUnlockString[] = "\n\n\r> Good bye\n\n\r";
	if (ePCT_LOCK==lock_state) 
    {
		(void)SERCOM1_USART_Write(&u8aPrintString[0],sizeof(u8aPrintString));
        (void)SERCOM1_USART_Write((UINT8 *)"\n\n\r> ",5);
	} 
    else 
    {
		(void)SERCOM1_USART_Write(&u8aPrintUnlockString[0],sizeof(u8aPrintUnlockString));
        (void)SERCOM1_USART_Write((UINT8 *)"\n\n\r> ",5);
	}
    
}

/*
 * Function: MchpPSF_PCTRUN
 * 
 * This function reads the input from the terminal and starts the PCT and
 * space key input does the toggling operation between DEBUG TRACE and PCT 
 */
void MchpPSF_PCTRUN(PCT_LOCK_STATE lock_state)
{
	static UINT8 u8array[LENGTH];
	static UINT8 u8Length = 0;
	UINT8 u8ReadByte;
	static STATE state = eINIT;

	do {
		switch (state) 
        {
		case eINIT:
			/*
			 * checks if UART receiver is ready 
			 */
			memset(u8array, 0, LENGTH);
			if (true == SERCOM1_USART_ReceiverIsReady()) 
            {
				/*
				 * Read the input 
				 */
				u8array[u8Length] = PCTReadByte();

				if ((u8array[u8Length] == '?')
				    || (u8array[u8Length] == 0x0d)) 
                {
					/*
					 * checks for '?' input 
					 */
					state = eREAD_PROCESS;
				}

				else if (u8array[u8Length] == 0x20) 
                {
					/*
					 * checks for space key input 
					 */
					state = eTOGGLE_BLOCK;
				} 
                else if (u8array[u8Length] == 0x71) 
                {
					/*
					 * Exits from the current state if 'Q/q' is given as
					 * input 
					 */
                    (void)SERCOM1_USART_Write((UINT8*)"\n\n\r> Exiting from PCT",24);
                    (void)SERCOM1_USART_Write((UINT8 *)"\n\n\r> ",5);
                    
					state = eINIT;
					u8Length = 0;
				} 
                else 
                {
					u8Length++;
					state = eREAD_CONTINUE;
				}

			}

			break;
		case eREAD_CONTINUE:

			if (true == SERCOM1_USART_ReceiverIsReady()) 
            {
				u8ReadByte = PCTReadByte();
				if ((u8ReadByte == '?') || (u8ReadByte == 0x0d)) 
                {
					state = eREAD_PROCESS;
					u8array[u8Length] = u8ReadByte;
					break;
				} 
                else if (u8ReadByte == ' '&& (u8array[u8Length - 1] == ' ')) 
                {
					// nothing to do if again one more space
				} 
                else 
                {
					u8array[u8Length] = u8ReadByte;
					u8Length++;
				}
			}
			state = eREAD_CONTINUE;
			break;

		case eREAD_PROCESS:
			if (u8array[u8Length] == '?') 
            {
				PCTPrintCommands();
			} 
            else 
            {
				PCTCommandhandler(u8array);
			}

			u8Length = 0;
			state = eINIT;

			break;
		case eHALT:
			state = eINIT;
			break;

		case eTOGGLE_BLOCK:

			if (lock_state == ePCT_LOCK) 
            {
				lock_state=ePCT_UNLOCK;
			} 
            else 
            {
				lock_state=ePCT_LOCK;
			}
			PCTSpaceBARMsg(lock_state);

			state = eINIT;
			break;
		}
	}
	while (ePCT_LOCK==lock_state);
}

void MchpPSF_PCTInit()
{
	/*
	 * writes the welcome message 
	 */
	UINT8 u8aPrintStirng[] =
	    "> Welcome to PSF Control Terminal\n\n\r> Type ? for the list of supported commands\n\n\r> ";
    /*If debug message is enabled in PSF then MCHP_PSF_HOOK_DEBUG_INIT is initialized as part of MchpPSF_Init by PSF stack*/
    #if (FALSE == CONFIG_HOOK_DEBUG_MSG)
        /*Initialize debug hardware*/
        MCHP_PSF_HOOK_DEBUG_INIT();
    #endif
        (void)SERCOM1_USART_Write(&u8aPrintStirng[0], sizeof(u8aPrintStirng));
        (void)SERCOM1_USART_Write((UINT8 *)"\n\n\r> ",5);
        
}

void PSF_monitorandwait()
{
	MchpPSF_PCTRUN(ePCT_LOCK);
}

/*******************************************************************************
 End of File
*/
