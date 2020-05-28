/***************************************************************************
  Power Delivery Source Pro Configurable globals Header File

  Company:
    Microchip Technology Inc.

  File Name:
    PSFSourcePro_BootCfg.h

  Description:
    This header file contains user configurable globals and function prototypes.
************************************************************************** */
/*******************************************************************************
Copyright ©  [2019-2020] Microchip Technology Inc. and its subsidiaries.

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

#ifndef _PSFSOURCEPRO_BOOTCFG_H    /* Guard against multiple inclusion */
#define _PSFSOURCEPRO_BOOTCFG_H

#define STRUCT_MAJOR_VERSION    0x01U
#define STRUCT_MINOR_VERSION    0x00U
#define PRODUCT_ID              0x301CU
#define VENDOR_ID               0x0424U
#define PRODUCT_TYPE_VDO        0x0000U
#define PRODUCT_VDO             0x0000U  
#define CERT_STAT_VDO           0x0000U
#define ID_HEADER_VDO           0x0000U
#define HW_VERSION              0x00U
#define SILICON_VERSION         0x00U

#define CFG_PORT_POWER_ROLE                1U
#define CFG_PORT_RP_CURRENT_VALUE          (3U << TYPEC_PORT_RPVAL_POS)
#define CFG_PORT_ENABLE                    (1U << TYPEC_PORT_ENDIS_POS)
#define CFG_PORT_SOURCE_NUM_OF_PDOS        4U
#define CFG_PORT_SOURCE_USB_SUSP           0U
#define CFG_PORT_SOURCE_UNCONSTARINED_PWR  1U
#define CFG_PORT_SOURCE_USB_COMM           0U

#define CFG_PORT_SOURCE_PDO_1_CURRENT      3000U 
#define CFG_PORT_SOURCE_PDO_2_CURRENT      3000U
#define CFG_PORT_SOURCE_PDO_3_CURRENT      3000U
#define CFG_PORT_SOURCE_PDO_4_CURRENT      3000U
#define CFG_PORT_SOURCE_PDO_5_CURRENT      0U
#define CFG_PORT_SOURCE_PDO_6_CURRENT      0U
#define CFG_PORT_SOURCE_PDO_7_CURRENT      0U

#define CFG_PORT_SOURCE_PDO_1_VOLTAGE      5000
#define CFG_PORT_SOURCE_PDO_2_VOLTAGE      9000
#define CFG_PORT_SOURCE_PDO_3_VOLTAGE      15000
#define CFG_PORT_SOURCE_PDO_4_VOLTAGE      20000
#define CFG_PORT_SOURCE_PDO_5_VOLTAGE      0
#define CFG_PORT_SOURCE_PDO_6_VOLTAGE      0
#define CFG_PORT_SOURCE_PDO_7_VOLTAGE      0

#define CFG_MAX_PDO_COUNT                 7
#define CFG_OVER_VOLTAGE_FACTOR			  115
#define CFG_UNDER_VOLTAGE_FACTOR		  85
#define CFG_MAX_VBUS_POWER_FAULT_COUNT	  3
#define CFG_MAX_VCONN_FAULT_COUNT		  3
#define CFG_POWER_GOOD_TIMER_MS			  10000

#define CFG_PORT_UPD_EN_VBUS               eUPD_PIO3
#define CFG_PORT_UPD_EN_VBUS_PIO_MODE      ePUSH_PULL_ACTIVE_HIGH
#define CFG_PORT_UPD_VBUS_DIS_PIO_NO       eUPD_PIO4
#define CFG_PORT_UPD_VBUS_DIS_PIO_MODE     ePUSH_PULL_ACTIVE_HIGH
#define CFG_PORT_UPD_DC_DC_EN_PIO_NO       eUPD_PIO6
#define CFG_PORT_UPD_DC_DC_EN_PIO_MODE     ePUSH_PULL_ACTIVE_HIGH
#define CFG_PORT_UPD_VSEL0_PIO_NO          eUPD_PIO7
#define CFG_PORT_UPD_VSEL1_PIO_NO          eUPD_PIO8
#define CFG_PORT_UPD_VSEL2_PIO_NO          eUPD_PIO9
#define CFG_PORT_UPD_FAULT_IN_PIO_NO       eUPD_PIO5
#define CFG_PORT_UPD_FAULT_IN_MODE         eFAULT_IN_ACTIVE_LOW
#define CFG_PORT_UPD_VSEL0_PIO_MODE        ePUSH_PULL_ACTIVE_HIGH
#define CFG_PORT_UPD_VSEL1_PIO_MODE        ePUSH_PULL_ACTIVE_HIGH
#define CFG_PORT_UPD_VSEL2_PIO_MODE        ePUSH_PULL_ACTIVE_HIGH

#define CFG_PORT_VSAFE0V_VSEL_MAPPING      0x00
#define CFG_PORT_PDO_1_VSEL_MAPPING        0x00
#define CFG_PORT_PDO_2_VSEL_MAPPING        0x01
#define CFG_PORT_PDO_3_VSEL_MAPPING        0x02
#define CFG_PORT_PDO_4_VSEL_MAPPING        0x04
#define CFG_PORT_PDO_5_VSEL_MAPPING        0x00
#define CFG_PORT_PDO_6_VSEL_MAPPING        0x00
#define CFG_PORT_PDO_7_VSEL_MAPPING        0x00

#define CFG_VCONN_OCS_ENABLE               (1 << TYPEC_VCONN_OCS_EN_POS)
#define CFG_VCONN_OCS_DEBOUNCE_IN_MS       2
#define CFG_FAULT_IN_OCS_DEBOUNCE_MS       5

#define CFG_MAX_PORT_CURRENT_IN_10mA       300U 

/* Power Throttling Bank values */
#define CFG_PD_THROTTLE_BANK_A             0U 
#define CFG_PD_THROTTLE_BANK_B             1U
#define CFG_PD_THROTTLE_BANK_C             2U
#define CFG_PD_THROTTLE_SHUTDOWN_MODE      3U

/* Default Total System Power Bank A is 120W */
#define CFG_PB_TOT_SYS_POWER_BANKA            0x01E0U
/* Guaranteed Minimum Power for Bank A is 15W */
#define CFG_PB_MIN_POWER_BANKA                0x003CU
/* Default Total System Power Bank B is 120W */
#define CFG_PB_TOT_SYS_POWER_BANKB            0x01E0U
/* Guaranteed Minimum Power for Bank A is 15W */
#define CFG_PB_MIN_POWER_BANKB                0x003CU
/* Default Total System Power Bank C is 120W */
#define CFG_PB_TOT_SYS_POWER_BANKC            0x01E0U
/* Guaranteed Minimum Power for Bank A is 15W */
#define CFG_PB_MIN_POWER_BANKC                0x003CU

/* Default Max Port Power for Bank A is 60W */
#define CFG_PB_MAX_PORT_POWER_BANKA           0x00F0U
/* Default Max Port Power for Bank B is 60W */
#define CFG_PB_MAX_PORT_POWER_BANKB           0x00F0U
/* Default Max Port Power for Bank C is 60W */
#define CFG_PB_MAX_PORT_POWER_BANKC           0x00F0U
/* Default Max Port current is 3A */
#define CFG_PB_MAX_PORT_CURRENT               0x012CU

/* Note: By default, PB is disabled for the system since support for 
   PPS + PB coexistence is planned for future phases. If PB needs to be 
   enabled, change the value of CFG_PB_ENABLE macro to 0x10U and 
   CFG_PB_PORT_ENABLE to 0x01U. This will enable PB for the system 
   with FCFS as Selection Algorithm and for the ports. */
#define CFG_PB_ENABLE                         0x00U 
#define CFG_PB_PORT_ENABLE                    0x00U

#define CFG_PDO_VOLTAGE_POS                   10 
#define CFG_PDO_VOLTAGE_UNIT                  50
#define CFG_PDO_CURRENT_UNIT                  10
#define CFG_PDO_USB_SUSPEND_POS               28 
#define CFG_PDO_USB_COMMN_POS                 26 
#define CFG_PDO_UNCONSTRAINED_PWR             27 

/* Power Supply type - Bits 31:10 of Power Data Object */
#define CFG_POWER_SUPPLY_TYPE_FIXED           0x00U
#define CFG_POWER_SUPPLY_TYPE_VARIABLE        0x01U
#define CFG_POWER_SUPPLY_TYPE_BATTERY         0x02U
#define CFG_POWER_SUPPLY_TYPE_PROGRAMMABLE    0x03U

/************************* PPS APDO Fields *******************/
#if (TRUE == INCLUDE_PD_SOURCE_PPS) 

/* APDO Minimum Voltage */
#define CFG_PORT_SOURCE_APDO_1_MIN_VOLTAGE    3300
#define CFG_PORT_SOURCE_APDO_2_MIN_VOLTAGE    0
#define CFG_PORT_SOURCE_APDO_3_MIN_VOLTAGE    0

/* APDO Maximum Voltage */
#define CFG_PORT_SOURCE_APDO_1_MAX_VOLTAGE    21000
#define CFG_PORT_SOURCE_APDO_2_MAX_VOLTAGE    0
#define CFG_PORT_SOURCE_APDO_3_MAX_VOLTAGE    0

/* APDO Minimum Current */
#define CFG_PORT_SOURCE_APDO_1_MAX_CURRENT    3000 
#define CFG_PORT_SOURCE_APDO_2_MAX_CURRENT    0 
#define CFG_PORT_SOURCE_APDO_3_MAX_CURRENT    0 

/* APDO PPS Power Limited */
#define CFG_PORT_SOURCE_APDO_1_PPS_PWR_LIMITED    0 
#define CFG_PORT_SOURCE_APDO_2_PPS_PWR_LIMITED    0 
#define CFG_PORT_SOURCE_APDO_3_PPS_PWR_LIMITED    0 

/* Position of various APDO Fields */
#define CFG_APDO_MIN_VOLTAGE_POS               8
#define CFG_APDO_MAX_VOLTAGE_POS               17
#define CFG_APDO_MAX_CURRENT_POS               0
#define CFG_APDO_PPS_PWR_LIMITED_POS           27
#define CFG_APDO_PWR_SUPPLY_TYPE_POS           30

/* Units of various APDO Fields */
#define CFG_APDO_MIN_VOLTAGE_UNIT              100
#define CFG_APDO_MAX_VOLTAGE_UNIT              100
#define CFG_APDO_MAX_CURRENT_UNIT              50

/* Macros for PPS Config Data Enable/Disable */
#define CFG_PPS_ENABLE                         1U 
#define CFG_PPS_APDO_1_ENABLE                  1U
#define CFG_PPS_APDO_2_ENABLE                  0U
#define CFG_PPS_APDO_3_ENABLE                  0U 

/* Macros for PPS Cfg Data position */
#define CFG_PPS_APDO_1_ENABLE_POS              1
#define CFG_PPS_APDO_2_ENABLE_POS              2
#define CFG_PPS_APDO_3_ENABLE_POS              3 

/* Macro used to form PPS APDO */
#define CFG_FORM_PPS_APDO(pdoType,minVolt,maxVolt,maxCurrent,pwrLtd) (((pdoType) << CFG_APDO_PWR_SUPPLY_TYPE_POS) | \
                                        ((pwrLtd) << CFG_APDO_PPS_PWR_LIMITED_POS) | \
                                        (((maxVolt) / CFG_APDO_MAX_VOLTAGE_UNIT) << CFG_APDO_MAX_VOLTAGE_POS) | \
                                        (((minVolt) / CFG_APDO_MIN_VOLTAGE_UNIT) << CFG_APDO_MIN_VOLTAGE_POS) | \
                                        (((maxCurrent) / CFG_APDO_MAX_CURRENT_UNIT) << CFG_APDO_MAX_CURRENT_POS)) 
#endif

/* Macro used to form Fixed PDO 1 */
#define CFG_FORM_FIXED_PDO1(voltage,current,usbCommn,usbSusp,unconstrainedPwr)  (((usbSusp) << CFG_PDO_USB_SUSPEND_POS) | \
                                         ((unconstrainedPwr) << CFG_PDO_UNCONSTRAINED_PWR) | \
                                         ((usbCommn) << CFG_PDO_USB_COMMN_POS) | \
                                         (((voltage)/CFG_PDO_VOLTAGE_UNIT) << CFG_PDO_VOLTAGE_POS) | \
                                         ((current)/CFG_PDO_CURRENT_UNIT))            

/* Macro used to form Fixed PDOs 2 to 7 */
#define CFG_FORM_FIXED_PDOx(voltage,current)        ((((voltage)/CFG_PDO_VOLTAGE_UNIT) << CFG_PDO_VOLTAGE_POS) | \
                                                            ((current)/CFG_PDO_CURRENT_UNIT))

void PSF_LoadConfig(); 

#endif /* _PSFSOURCEPRO_BOOTCFG_H */

/* *****************************************************************************
 End of File
 */
