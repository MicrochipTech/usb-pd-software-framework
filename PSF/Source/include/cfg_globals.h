/*******************************************************************************
  Configurable globals Header File

  Company:
    Microchip Technology Inc.

  File Name:
    cfg_globals.h

  Description:
    This file contains user configurable globals and functions. 
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

#ifndef _CFG_GLOBALS_H_
#define _CFG_GLOBALS_H_

#define STRUCT_MAJOR_VERSION    0x01U
#define STRUCT_MINOR_VERSION    0x00U
#define PRODUCT_ID              0x0424U
#define VENDOR_ID               0x301CU
#define PRODUCT_TYPE_VDO        0x0000U
#define PRODUCT_VDO             0x0000U  
#define CERT_STAT_VDO           0x0000U
#define ID_HEADER_VDO           0x0000U
#define HW_VERSION              0x00U
#define SILICON_VERSION         0x00U

#define PD_THROTTLE_BANK_A              0x00U 
#define PD_THROTTLE_BANK_B              0x01U
#define PD_THROTTLE_BANK_C              0x10U
#define PD_THROTTLE_SHUTDOWN_MODE       0x11U

/* Default Total System Power Bank A is 120W */
#define CFG_PB_TOT_SYS_POWER_BANKA 0x01E0U
/* Guaranteed Minimum Power for Bank A is 15W */
#define CFG_PB_MIN_POWER_BANKA       0x003CU
/* Default Total System Power Bank B is 120W */
#define CFG_PB_TOT_SYS_POWER_BANKB 0x01E0U
/* Guaranteed Minimum Power for Bank A is 15W */
#define CFG_PB_MIN_POWER_BANKB       0x003CU
/* Default Total System Power Bank C is 120W */
#define CFG_PB_TOT_SYS_POWER_BANKC 0x01E0U
/* Guaranteed Minimum Power for Bank A is 15W */
#define CFG_PB_MIN_POWER_BANKC       0x003CU

/* Default Max Port Power for Bank A is 60W */
#define CFG_PB_MAX_PORT_POWER_BANKA  0x00F0U
/* Default Max Port Power for Bank B is 60W */
#define CFG_PB_MAX_PORT_POWER_BANKB  0x00F0U
/* Default Max Port Power for Bank C is 60W */
#define CFG_PB_MAX_PORT_POWER_BANKC  0x00F0U
/* Default Max Port current is 3A */
#define CFG_PB_MAX_PORT_CURRENT      0x012CU
/* Default Enable PB for port 0 with Priority as 0*/
#define CFG_PB_PORT_ENABLE           0x0001U

// *****************************************************************************
// *****************************************************************************
// Section: Data types and constants
// *****************************************************************************
// *****************************************************************************

extern void CFG_LoadDefaults ();

#endif /*_CFG_GLOBALS_H_*/

