/*******************************************************************************
  Configuration Globals Source File

  Company:
    Microchip Technology Inc.

  File Name:
    cfg_globals.c

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
#include <psf_stdinc.h>

/*******************************************************************/
/******************* Global variables********************************/
/*******************************************************************/
/***************************************************************************************************/
/****************************Fixed PDO generation for Source ***************************************/

#define PE_SRC_PDO_1_GENERATION(n) \
  (((CONFIG_PORT_##n##_SOURCE_USB_SUSPEND) << 28) |\
  ((CONFIG_PORT_##n##_SOURCE_UNCONSTRAINED_PWR) << 27) |\
  ((CONFIG_PORT_##n##_SOURCE_USB_COM) << 26) |\
  (((CONFIG_PORT_##n##_SOURCE_PDO_1_VOLTAGE)/50) << 10) |\
  ((CONFIG_PORT_##n##_SOURCE_PDO_1_CURRENT)/10))

#define PE_SRC_PDO_GENERATION(n,x) \
  ((((CONFIG_PORT_##n##_SOURCE_PDO_##x##_VOLTAGE)/50) << 10) |\
  ((CONFIG_PORT_##n##_SOURCE_PDO_##x##_CURRENT)/10))

#define PE_PORT_SRC_PDO_GENERATION(n) \
            (PE_SRC_PDO_1_GENERATION(n)), \
            (PE_SRC_PDO_GENERATION(n,2)), \
            (PE_SRC_PDO_GENERATION(n,3)), \
            (PE_SRC_PDO_GENERATION(n,4)), \
            (PE_SRC_PDO_GENERATION(n,5)), \
            (PE_SRC_PDO_GENERATION(n,6)), \
            (PE_SRC_PDO_GENERATION(n,7))

#define PE_SRC_PORT_FIXED_PDO(x)  \
        {PE_PORT_SRC_PDO_GENERATION(x)},(CONFIG_PORT_##x##_SOURCE_NUM_OF_PDOS)

/***************************************************************************************************/
/******************************Fixed PDO generation for Sink ***************************************/

#define PE_SNK_PDO_1_GENERATION(n) \
  (((CONFIG_PORT_##n##_SINK_HIGHER_CAPABILITY) << 28) |\
  ((CONFIG_PORT_##n##_SOURCE_UNCONSTRAINED_PWR) << 27) |\
  ((CONFIG_PORT_##n##_SOURCE_USB_COM) << 26) |\
  (((CONFIG_PORT_##n##_SINK_PDO_1_VOLTAGE)/50) << 10) |\
  ((CONFIG_PORT_##n##_SINK_PDO_1_CURRENT)/10))

#define PE_SNK_PDO_GENERATION(n,x) \
  ((((CONFIG_PORT_##n##_SINK_PDO_##x##_VOLTAGE)/50) << 10) |\
  ((CONFIG_PORT_##n##_SINK_PDO_##x##_CURRENT)/10))
 
#define PE_PORT_SNK_PDO_GENERATION(n) \
            (PE_SNK_PDO_1_GENERATION(n)), \
            (PE_SNK_PDO_GENERATION(n,2)), \
            (PE_SNK_PDO_GENERATION(n,3)), \
            (PE_SNK_PDO_GENERATION(n,4)), \
            (PE_SNK_PDO_GENERATION(n,5)), \
            (PE_SNK_PDO_GENERATION(n,6)), \
            (PE_SNK_PDO_GENERATION(n,7))

#define PE_SNK_PORT_FIXED_PDO(x)  {PE_PORT_SNK_PDO_GENERATION(x)},(CONFIG_PORT_##x##_SINK_NUM_OF_PDOS)                           



/***************************************************************************************************/
/* Type C configuration data generation */
#define TYPEC_CONFIG_DATA(n) ((CONFIG_PORT_##n##_ENDIS << TYPEC_PORT_ENDIS_POS)| \
                             ((CONFIG_PORT_##n##_RP_CURRENT_VALUE) << TYPEC_PORT_RPVAL_POS)|\
                                    (CONFIG_PORT_##n##_POWER_ROLE))

/***************************************************************************************************/

/* Source or Sink Fixed PDO assignment depending upon Power Role*/

#if (CONFIG_PD_PORT_COUNT >= 1) && (CONFIG_PORT_0_POWER_ROLE == PD_ROLE_SOURCE)
#define CONFIG_PORT_0_CONFIGURATION_DATA    TYPEC_CONFIG_DATA(0),\
                                            PE_SRC_PORT_FIXED_PDO(0)
                                              
#else
#define CONFIG_PORT_0_CONFIGURATION_DATA      TYPEC_CONFIG_DATA(0),\
                                            PE_SNK_PORT_FIXED_PDO(0)
#endif

#if (CONFIG_PD_PORT_COUNT >= 2) && (CONFIG_PORT_1_POWER_ROLE == PD_ROLE_SOURCE)
#define CONFIG_PORT_1_CONFIGURATION_DATA      TYPEC_CONFIG_DATA(1),\
                                            PE_SRC_PORT_FIXED_PDO(1)
                                              
#else
#define CONFIG_PORT_1_CONFIGURATION_DATA      TYPEC_CONFIG_DATA(1),\
                                            PE_SNK_PORT_FIXED_PDO(1)
#endif

#if (CONFIG_PD_PORT_COUNT >= 3) && (CONFIG_PORT_2_POWER_ROLE == PD_ROLE_SOURCE)
#define CONFIG_PORT_2_CONFIGURATION_DATA      TYPEC_CONFIG_DATA(2),\
                                            PE_SRC_PORT_FIXED_PDO(2)
                                              
#else
#define CONFIG_PORT_2_CONFIGURATION_DATA      TYPEC_CONFIG_DATA(2),\
                                            PE_SNK_PORT_FIXED_PDO(2)
#endif

#if (CONFIG_PD_PORT_COUNT >= 4) && (CONFIG_PORT_3_POWER_ROLE == PD_ROLE_SOURCE)
#define CONFIG_PORT_3_CONFIGURATION_DATA      TYPEC_CONFIG_DATA(3),\
                                            PE_SRC_PORT_FIXED_PDO(3)
                                              
#else
#define CONFIG_PORT_3_CONFIGURATION_DATA      TYPEC_CONFIG_DATA(3),\
                                            PE_SNK_PORT_FIXED_PDO(3)
#endif

/*******************************************************************************/

/*Per port configuration assignment depending on the number of port*/

#if (CONFIG_PD_PORT_COUNT == 1)
#define CONFIG_PORT_CONFIGURATION_DATA      {CONFIG_PORT_0_CONFIGURATION_DATA}
#endif

#if (CONFIG_PD_PORT_COUNT == 2)
#define CONFIG_PORT_CONFIGURATION_DATA      {CONFIG_PORT_0_CONFIGURATION_DATA},\
                                            {CONFIG_PORT_1_CONFIGURATION_DATA}
#endif

#if (CONFIG_PD_PORT_COUNT == 3)
#define CONFIG_PORT_CONFIGURATION_DATA      {CONFIG_PORT_0_CONFIGURATION_DATA},\
                                            {CONFIG_PORT_1_CONFIGURATION_DATA},\
                                            {CONFIG_PORT_2_CONFIGURATION_DATA}
#endif

#if (CONFIG_PD_PORT_COUNT == 4)
#define CONFIG_PORT_CONFIGURATION_DATA      {CONFIG_PORT_0_CONFIGURATION_DATA},\
                                            {CONFIG_PORT_1_CONFIGURATION_DATA},\
                                            {CONFIG_PORT_2_CONFIGURATION_DATA},\
                                            {CONFIG_PORT_3_CONFIGURATION_DATA}
#endif 


/*******************************************************************************/

PORT_CONFIG_DATA gasPortConfigurationData[CONFIG_PD_PORT_COUNT] = {CONFIG_PORT_CONFIGURATION_DATA};

/*******************************************************************************/

/* Obtaining UPD PIO configuration from defines */

/* Obtains VBUS ENABLE PIO*/                                                     
#define UPD_EN_VBUS_PIO_MNG(n)          (UINT8)(CONFIG_PORT_##n##_UPD_EN_VBUS)
/* Obtains VBUS ENABLE PIO mode*/                                                     
#define UPD_EN_VBUS_PIO_MODE_MNG(n)		(UINT8)(CONFIG_PORT_##n##_UPD_EN_VBUS_PIO_MODE)
/* Obtains PRT_CTL/OCS PIO */                                                     
#define UPD_FAULT_IN_PIO_MNG(n)         (UINT8)(CONFIG_PORT_##n##_UPD_FAULT_IN_PIO_NO)
/* Obtains Fault In pin PIO mode*/
#define UPD_FAULT_IN_PIO_MODE_MNG(n)    (UINT8)(CONFIG_PORT_##n##_UPD_FAULT_IN_MODE)
/* Obtains VBUS DISCHARGE PIO*/                                                     
#define UPD_VBUS_DIS_PIO_MNG(n)         (UINT8)(CONFIG_PORT_##n##_UPD_VBUS_DIS_PIO_NO)
/* Obtains VBUS DISCHARGE PIO mode*/                                                     
#define UPD_VBUS_DIS_PIO_MODE_MNG(n)    (UINT8)(CONFIG_PORT_##n##_UPD_VBUS_DIS_PIO_MODE)
/* Obtains DC DC ENABLE PIO*/                                                     
#define UPD_DC_DC_EN_PIO_MNG(n)         (UINT8)(CONFIG_PORT_##n##_UPD_DC_DC_EN_PIO_NO)
/* Obtains DC DC ENABLE PIO mode*/                                                     
#define UPD_DC_DC_EN_PIO_MODE_MNG(n)    (UINT8)(CONFIG_PORT_##n##_UPD_DC_DC_EN_PIO_MODE)
/* Obtains VSEL0 PIO*/                                                     
#define UPD_VSEL0_PIO_MNG(n)            (UINT8)(CONFIG_PORT_##n##_UPD_VSEL0_PIO_NO)
/* Obtains VSEL0 PIO mode*/                                                     
#define UPD_VSEL0_PIO_MODE_MNG(n)       (UINT8)(CONFIG_PORT_##n##_UPD_VSEL0_PIO_MODE)
/* Obtains VSEL1 PIO*/                                                     
#define UPD_VSEL1_PIO_MNG(n)            (UINT8)(CONFIG_PORT_##n##_UPD_VSEL1_PIO_NO)
/* Obtains VSEL1 PIO mode*/                                                     
#define UPD_VSEL1_PIO_MODE_MNG(n)       (UINT8)(CONFIG_PORT_##n##_UPD_VSEL1_PIO_MODE)
/* Obtains VSEL2 PIO*/                                                     
#define UPD_VSEL2_PIO_MNG(n)            (UINT8)(CONFIG_PORT_##n##_UPD_VSEL2_PIO_NO)
/* Obtains VSEL2 PIO mode*/                                                     
#define UPD_VSEL2_PIO_MODE_MNG(n)       (UINT8)(CONFIG_PORT_##n##_UPD_VSEL2_PIO_MODE)
/*Obtain Vsafe0V VSEL mapping*/
#define UPD_VSEL_MAP_FOR_VSAFE0V(n)     (UINT8)(CONFIG_PORT_##n##_VSAFE0V_VSEL_MAPPING)
/*Obtain VSEL mapping for PDOs*/
#define UPD_VSEL_MAP_FOR_PDO(n)         (UINT8)(CONFIG_PORT_##n##_PDO_1_VSEL_MAPPING), \
                                        (UINT8)(CONFIG_PORT_##n##_PDO_2_VSEL_MAPPING), \
                                        (UINT8)(CONFIG_PORT_##n##_PDO_3_VSEL_MAPPING), \
                                        (UINT8)(CONFIG_PORT_##n##_PDO_4_VSEL_MAPPING), \
                                        (UINT8)(CONFIG_PORT_##n##_PDO_5_VSEL_MAPPING), \
                                        (UINT8)(CONFIG_PORT_##n##_PDO_6_VSEL_MAPPING), \
                                        (UINT8)(CONFIG_PORT_##n##_PDO_7_VSEL_MAPPING)

/* defines for obtaining PIO number*/
#define UPD_PIO_MNG_CONFIG(n)           UPD_EN_VBUS_PIO_MNG(n), \
                                        UPD_EN_VBUS_PIO_MODE_MNG(n), \
                                        UPD_FAULT_IN_PIO_MNG(n), \
                                        UPD_FAULT_IN_PIO_MODE_MNG(n), \
                                        UPD_VBUS_DIS_PIO_MNG(n), \
                                        UPD_VBUS_DIS_PIO_MODE_MNG(n), \
                                        UPD_DC_DC_EN_PIO_MNG(n), \
                                        UPD_DC_DC_EN_PIO_MODE_MNG(n), \
                                        {UPD_VSEL0_PIO_MNG(n), \
                                        UPD_VSEL1_PIO_MNG(n), \
                                        UPD_VSEL2_PIO_MNG(n)}, \
                                        {UPD_VSEL0_PIO_MODE_MNG(n), \
                                        UPD_VSEL1_PIO_MODE_MNG(n), \
                                        UPD_VSEL2_PIO_MODE_MNG(n)}, \
                                        {UPD_VSEL_MAP_FOR_VSAFE0V(n), \
                                        UPD_VSEL_MAP_FOR_PDO(n)}


/********************************************************************************/
#if (CONFIG_PD_PORT_COUNT == 1)
#define CONFIG_PORT_UPD_PIO_CONFIGURATION_DATA      {UPD_PIO_MNG_CONFIG(0)}
#endif

#if (CONFIG_PD_PORT_COUNT == 2)
#define CONFIG_PORT_UPD_PIO_CONFIGURATION_DATA      {UPD_PIO_MNG_CONFIG(0)},	\
                                                    {UPD_PIO_MNG_CONFIG(1)}
#endif

#if (CONFIG_PD_PORT_COUNT == 3)
#define CONFIG_PORT_UPD_PIO_CONFIGURATION_DATA      {UPD_PIO_MNG_CONFIG(0)},	\
                                                    {UPD_PIO_MNG_CONFIG(1)},  \
                                                    {UPD_PIO_MNG_CONFIG(2)}
#endif

#if (CONFIG_PD_PORT_COUNT == 4)
#define CONFIG_PORT_UPD_PIO_CONFIGURATION_DATA      {UPD_PIO_MNG_CONFIG(0)},	\
                                                    {UPD_PIO_MNG_CONFIG(1)},  \
                                                    {UPD_PIO_MNG_CONFIG(2)},  \
                                                    {UPD_PIO_MNG_CONFIG(3)}
#endif 



/*******************************************************************************/
#if(PWRCTRL_DEFAULT_PSF_GPIO_CONFIG == CONFIG_DCDC_CTRL)   

UPD_PIO_CONFIG_DATA const gasUpdPioDcDcConfig[CONFIG_PD_PORT_COUNT] = {CONFIG_PORT_UPD_PIO_CONFIGURATION_DATA};

#endif

/*******************************************************************************/


