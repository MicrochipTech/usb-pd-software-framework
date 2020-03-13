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

#define CONFIG_PORT_POWER_ROLE                1U
#define CONFIG_PORT_RP_CURRENT_VALUE          (3U << TYPEC_PORT_RPVAL_POS)
#define CONFIG_PORT_ENABLE                    (1U << TYPEC_PORT_ENDIS_POS)
#define CONFIG_PORT_SOURCE_NUM_OF_PDOS        4U
#define CONFIG_PORT_SOURCE_USB_SUSP           0U
#define CONFIG_PORT_SOURCE_UNCONSTARINED_PWR  1U
#define CONFIG_PORT_SOURCE_USB_COMM           0U

#define CONFIG_PORT_SOURCE_PDO_1_CURRENT      3000U 
#define CONFIG_PORT_SOURCE_PDO_2_CURRENT      3000U
#define CONFIG_PORT_SOURCE_PDO_3_CURRENT      3000U
#define CONFIG_PORT_SOURCE_PDO_4_CURRENT      3000U
#define CONFIG_PORT_SOURCE_PDO_5_CURRENT      0U
#define CONFIG_PORT_SOURCE_PDO_6_CURRENT      0U
#define CONFIG_PORT_SOURCE_PDO_7_CURRENT      0U

#define CONFIG_PORT_SOURCE_PDO_1_VOLTAGE      5000
#define CONFIG_PORT_SOURCE_PDO_2_VOLTAGE      9000
#define CONFIG_PORT_SOURCE_PDO_3_VOLTAGE      15000
#define CONFIG_PORT_SOURCE_PDO_4_VOLTAGE      20000
#define CONFIG_PORT_SOURCE_PDO_5_VOLTAGE      0
#define CONFIG_PORT_SOURCE_PDO_6_VOLTAGE      0
#define CONFIG_PORT_SOURCE_PDO_7_VOLTAGE      0

#define CONFIG_PORT_SINK_NUM_OF_PDOS          0U
#define CONFIG_PORT_SINK_HIGHER_CAPABILITY    0U
#define CONFIG_PORT_SINK_UNCONSTARINED_PWR    0U
#define CONFIG_PORT_SINK_USB_COMM             0U

#define CONFIG_PORT_SINK_PDO_1_CURRENT        3000U 
#define CONFIG_PORT_SINK_PDO_2_CURRENT        3000U
#define CONFIG_PORT_SINK_PDO_3_CURRENT        3000U
#define CONFIG_PORT_SINK_PDO_4_CURRENT        3000U
#define CONFIG_PORT_SINK_PDO_5_CURRENT        0U
#define CONFIG_PORT_SINK_PDO_6_CURRENT        0U
#define CONFIG_PORT_SINK_PDO_7_CURRENT        0U

#define CONFIG_PORT_SINK_PDO_1_VOLTAGE        5000
#define CONFIG_PORT_SINK_PDO_2_VOLTAGE        9000
#define CONFIG_PORT_SINK_PDO_3_VOLTAGE        15000
#define CONFIG_PORT_SINK_PDO_4_VOLTAGE        20000
#define CONFIG_PORT_SINK_PDO_5_VOLTAGE        0
#define CONFIG_PORT_SINK_PDO_6_VOLTAGE        0
#define CONFIG_PORT_SINK_PDO_7_VOLTAGE        0

#define CONFIG_MAX_PDO_COUNT                  7
#define CONFIG_OVER_VOLTAGE_FACTOR			  115
#define CONFIG_UNDER_VOLTAGE_FACTOR			  85
#define CONFIG_MAX_VBUS_POWER_FAULT_COUNT	  3
#define CONFIG_MAX_VCONN_FAULT_COUNT		  3
#define CONFIG_POWER_GOOD_TIMER_MS			  10000

// *****************************************************************************
// *****************************************************************************
// Section: PIO configurations
// *****************************************************************************
// *****************************************************************************
/**************************************************************************************************
Summary:
    UPD350 GPIO Output mode enum.
Description:
	eUPD_OUTPUT_PIN_MODES_TYPE enum defines the various combination modes applicable for UPD350 GPIO
    in output mode.
Remarks:
        None
**************************************************************************************************/
typedef enum
{
    ePUSH_PULL_ACTIVE_HIGH       = 0x0CU,   // Active High output signal
    ePUSH_PULL_ACTIVE_LOW        = 0x04U,   // Active low output signal
    eOPEN_DRAIN_ACTIVE_HIGH      = 0x08U,   // Active High Open Drain output signal
    eOPEN_DRAIN_ACTIVE_LOW       = 0x00U,   // Active Low Open Drain output signal
    eOPEN_DRAIN_ACTIVE_HIGH_PU   = 0x88U,   // Active High Open Drain output signal with internal pull up
    eOPEN_DRAIN_ACTIVE_LOW_PU    = 0x80U    // Active Low Open Drain output signal with internal pull up
}eUPD_OUTPUT_PIN_MODES_TYPE;
/**************************************************************************************************
Summary:
    UPD350 Fault_IN GPIO mode enum.
Description:
	eFAULT_IN_MODE_TYPE enum defines the various combination modes applicable for UPD350 GPIO
    in input mode.
Remarks:
    None
**************************************************************************************************/
typedef enum
{
    eFAULT_IN_ACTIVE_LOW         = 0x20U,   //Active low input signal
    eFAULT_IN_ACTIVE_HIGH        = 0x10U,   //Active high input signal
    eFAULT_IN_ACTIVE_LOW_PU      = 0xA0U,   //Active low signal with internal pull up
    eFAULT_IN_ACTIVE_HIGH_PD     = 0x50U    //Active high signal with internal pull down
}eFAULT_IN_MODE_TYPE;

#define CONFIG_PORT_UPD_EN_VBUS               eUPD_PIO3
#define CONFIG_PORT_UPD_EN_VBUS_PIO_MODE      ePUSH_PULL_ACTIVE_HIGH
#define CONFIG_PORT_UPD_VBUS_DIS_PIO_NO       eUPD_PIO4
#define CONFIG_PORT_UPD_VBUS_DIS_PIO_MODE     ePUSH_PULL_ACTIVE_HIGH
#define CONFIG_PORT_UPD_DC_DC_EN_PIO_NO       eUPD_PIO6
#define CONFIG_PORT_UPD_DC_DC_EN_PIO_MODE     ePUSH_PULL_ACTIVE_HIGH
#define CONFIG_PORT_UPD_VSEL0_PIO_NO          eUPD_PIO7
#define CONFIG_PORT_UPD_VSEL1_PIO_NO          eUPD_PIO8
#define CONFIG_PORT_UPD_VSEL2_PIO_NO          eUPD_PIO9
#define CONFIG_PORT_UPD_FAULT_IN_PIO_NO       eUPD_PIO5
#define CONFIG_PORT_UPD_FAULT_IN_MODE         eFAULT_IN_ACTIVE_LOW
#define CONFIG_PORT_UPD_VSEL0_PIO_MODE        ePUSH_PULL_ACTIVE_HIGH
#define CONFIG_PORT_UPD_VSEL1_PIO_MODE        ePUSH_PULL_ACTIVE_HIGH
#define CONFIG_PORT_UPD_VSEL2_PIO_MODE        ePUSH_PULL_ACTIVE_HIGH

#define CONFIG_PORT_VSAFE0V_VSEL_MAPPING      0x00
#define CONFIG_PORT_PDO_1_VSEL_MAPPING        0x00
#define CONFIG_PORT_PDO_2_VSEL_MAPPING        0x01
#define CONFIG_PORT_PDO_3_VSEL_MAPPING        0x02
#define CONFIG_PORT_PDO_4_VSEL_MAPPING        0x04
#define CONFIG_PORT_PDO_5_VSEL_MAPPING        0x00
#define CONFIG_PORT_PDO_6_VSEL_MAPPING        0x00
#define CONFIG_PORT_PDO_7_VSEL_MAPPING        0x00

#define CONFIG_VCONN_OCS_ENABLE               1
#define CONFIG_VCONN_OCS_DEBOUNCE_IN_MS       2
#define CONFIG_FAULT_IN_OCS_DEBOUNCE_MS       5

/* Power Throttling Bank values */
#define PD_THROTTLE_BANK_A                    0x00U 
#define PD_THROTTLE_BANK_B                    0x01U
#define PD_THROTTLE_BANK_C                    0x10U
#define PD_THROTTLE_SHUTDOWN_MODE             0x11U

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
/* Default PB Enable for System with Algorithm as FCFS */
#define CFG_PB_ENABLE                         0x10U 
/* Default Enable PB for port 0 with Priority as 0*/
#define CFG_PB_PORT_ENABLE                    0x0001U

#define CFG_PDO_VOLTAGE_POS                   10 
#define CFG_PDO_VOLTAGE_UNIT                  50
#define CFG_PDO_CURRENT_UNIT                  10
#define CFG_PDO_USB_SUSPEND_POS               28 
#define CFG_PDO_USB_COMMN_POS                 26 
#define CFG_PDO_UNCONSTRAINED_PWR             27 

/* Macro used to form Fixed PDO 1 */
#define CFG_FORM_FIXED_PDO1(voltage,current,usbCommn,usbSusp,unconstrainedPwr)  (((usbSusp) << CFG_PDO_USB_SUSPEND_POS) | \
                                         ((unconstrainedPwr) << CFG_PDO_UNCONSTRAINED_PWR) | \
                                         ((usbCommn) << CFG_PDO_USB_COMMN_POS) | \
                                         (((voltage)/CFG_PDO_VOLTAGE_UNIT) << CFG_PDO_VOLTAGE_POS) | \
                                         ((current)/CFG_PDO_CURRENT_UNIT))            

/* Macro used to form Fixed PDOs 2 to 7 */
#define CFG_FORM_FIXED_PDOx(voltage,current)        ((((voltage)/CFG_PDO_VOLTAGE_UNIT) << CFG_PDO_VOLTAGE_POS) | \
                                                            ((current)/CFG_PDO_CURRENT_UNIT))

/* Port Connection Status parameters */
#define PORT_CONNECT_STS_ATTACHED                             BIT(0)
#define PORT_CONNECT_STS_ORIENTATION_FLIPPED                  BIT(1)
#define PORT_CONNECT_STS_DATA_ROLE                            BIT(2)
#define PORT_CONNECT_STS_POWER_ROLE                           BIT(3)
#define PORT_CONNECT_STS_VCONN_STATUS                         BIT(4)
#define PORT_CONNECT_STS_CABLE_REDUCED_SRC_CAPABILITIES       BIT(5)
#define PORT_CONNECT_STS_PD_BAL_REDUCED_SRC_CAPABILITIES      BIT(6)
#define PORT_CONNECT_STS_SOURCE_CAPABILITY_MISMATCH           BIT(7)
#define PORT_CONNECT_STS_AS_SRC_PD_CONTRACT_GOOD              BIT(8)
#define PORT_CONNECT_STS_AS_SRC_RDO_ACCEPTED                  BIT(9)
#define PORT_CONNECT_STS_AS_SRC_RDO_REJECTED                  BIT(10)
#define PORT_CONNECT_STS_AS_SNK_LAST_REQ_ACCEPT               BIT(11)
#define PORT_CONNECT_STS_AS_SNK_LAST_REQ_REJECT               BIT(12)
#define PORT_CONNECT_STS_AS_SNK_LAST_REQ_PS_RDY               BIT(13)
#define PORT_CONNECT_STS_SINK_CAPABILITY_MISMATCH             BIT(14)
#define PORT_CONNECT_STS_RP_VAL_DETECT_DEFAULT_USB            BIT(15)
#define PORT_CONNECT_STS_RP_VAL_DETECT_1_5A                   BIT(16)
#define PORT_CONNECT_STS_RP_VAL_DETECT_3A                    (BIT(15)|BIT(16))
#define PORT_CONNECT_STS_RP_VAL_DETECT_MASK                  (BIT(15)|BIT(16))

/* Port IO Status parameters */
#define PORT_IO_EN_DC_DC_STATUS                      BIT(0)
#define PORT_IO_VSEL0_STATUS                         BIT(1)
#define PORT_IO_VSEL1_STATUS                         BIT(2)
#define PORT_IO_VSEL2_STATUS                         BIT(3)
#define PORT_IO_EN_VBUS_STATUS                       BIT(4)
#define PORT_IO_VBUS_DIS_STATUS                      BIT(5)
#define PORT_IO_EN_SINK_STATUS                       BIT(6)
#define PORT_IO_15_IND_STATUS                        BIT(7)
#define PORT_IO_30_IND_STATUS                        BIT(8)
#define PORT_IO_PS_RDY_RECVD_STATUS                  BIT(9)
#define PORT_IO_CAP_MISMATCH_STATUS                  BIT(10)

void PSF_LoadConfig(); 

#endif /* _PSFSOURCEPRO_BOOTCFG_H */

/* *****************************************************************************
 End of File
 */
