/**
 * \brief Component description for SERCOM
 *
 * Copyright (c) 2020 Microchip Technology Inc. and its subsidiaries.
 *
 * Subject to your compliance with these terms, you may use Microchip software and any derivatives
 * exclusively with Microchip products. It is your responsibility to comply with third party license
 * terms applicable to your use of third party software (including open source software) that may
 * accompany Microchip software.
 *
 * THIS SOFTWARE IS SUPPLIED BY MICROCHIP "AS IS". NO WARRANTIES, WHETHER EXPRESS, IMPLIED OR STATUTORY,
 * APPLY TO THIS SOFTWARE, INCLUDING ANY IMPLIED WARRANTIES OF NON-INFRINGEMENT, MERCHANTABILITY, AND
 * FITNESS FOR A PARTICULAR PURPOSE.
 *
 * IN NO EVENT WILL MICROCHIP BE LIABLE FOR ANY INDIRECT, SPECIAL, PUNITIVE, INCIDENTAL OR CONSEQUENTIAL
 * LOSS, DAMAGE, COST OR EXPENSE OF ANY KIND WHATSOEVER RELATED TO THE SOFTWARE, HOWEVER CAUSED, EVEN IF
 * MICROCHIP HAS BEEN ADVISED OF THE POSSIBILITY OR THE DAMAGES ARE FORESEEABLE. TO THE FULLEST EXTENT
 * ALLOWED BY LAW, MICROCHIP'S TOTAL LIABILITY ON ALL CLAIMS IN ANY WAY RELATED TO THIS SOFTWARE WILL NOT
 * EXCEED THE AMOUNT OF FEES, IF ANY, THAT YOU HAVE PAID DIRECTLY TO MICROCHIP FOR THIS SOFTWARE.
 *
 */

/* file generated from device description version 2019-11-25T06:48:59Z */
#ifndef _SAMD20_SERCOM_COMPONENT_H_
#define _SAMD20_SERCOM_COMPONENT_H_

/* ************************************************************************** */
/*   SOFTWARE API DEFINITION FOR SERCOM                                       */
/* ************************************************************************** */

/* -------- SERCOM_I2CM_CTRLA : (SERCOM Offset: 0x00) (R/W 32) I2CM Control A -------- */
#define SERCOM_I2CM_CTRLA_RESETVALUE          _U_(0x00)                                            /**<  (SERCOM_I2CM_CTRLA) I2CM Control A  Reset Value */

#define SERCOM_I2CM_CTRLA_SWRST_Pos           _U_(0)                                               /**< (SERCOM_I2CM_CTRLA) Software Reset Position */
#define SERCOM_I2CM_CTRLA_SWRST_Msk           (_U_(0x1) << SERCOM_I2CM_CTRLA_SWRST_Pos)            /**< (SERCOM_I2CM_CTRLA) Software Reset Mask */
#define SERCOM_I2CM_CTRLA_SWRST(value)        (SERCOM_I2CM_CTRLA_SWRST_Msk & ((value) << SERCOM_I2CM_CTRLA_SWRST_Pos))
#define SERCOM_I2CM_CTRLA_ENABLE_Pos          _U_(1)                                               /**< (SERCOM_I2CM_CTRLA) Enable Position */
#define SERCOM_I2CM_CTRLA_ENABLE_Msk          (_U_(0x1) << SERCOM_I2CM_CTRLA_ENABLE_Pos)           /**< (SERCOM_I2CM_CTRLA) Enable Mask */
#define SERCOM_I2CM_CTRLA_ENABLE(value)       (SERCOM_I2CM_CTRLA_ENABLE_Msk & ((value) << SERCOM_I2CM_CTRLA_ENABLE_Pos))
#define SERCOM_I2CM_CTRLA_MODE_Pos            _U_(2)                                               /**< (SERCOM_I2CM_CTRLA) Operating Mode Position */
#define SERCOM_I2CM_CTRLA_MODE_Msk            (_U_(0x7) << SERCOM_I2CM_CTRLA_MODE_Pos)             /**< (SERCOM_I2CM_CTRLA) Operating Mode Mask */
#define SERCOM_I2CM_CTRLA_MODE(value)         (SERCOM_I2CM_CTRLA_MODE_Msk & ((value) << SERCOM_I2CM_CTRLA_MODE_Pos))
#define   SERCOM_I2CM_CTRLA_MODE_USART_EXT_CLK_Val _U_(0x0)                                             /**< (SERCOM_I2CM_CTRLA) USART with external clock  */
#define   SERCOM_I2CM_CTRLA_MODE_USART_INT_CLK_Val _U_(0x1)                                             /**< (SERCOM_I2CM_CTRLA) USART with internal clock  */
#define   SERCOM_I2CM_CTRLA_MODE_SPI_SLAVE_Val _U_(0x2)                                             /**< (SERCOM_I2CM_CTRLA) SPI in slave operation  */
#define   SERCOM_I2CM_CTRLA_MODE_SPI_MASTER_Val _U_(0x3)                                             /**< (SERCOM_I2CM_CTRLA) SPI in master operation  */
#define   SERCOM_I2CM_CTRLA_MODE_I2C_SLAVE_Val _U_(0x4)                                             /**< (SERCOM_I2CM_CTRLA) I2C slave operation  */
#define   SERCOM_I2CM_CTRLA_MODE_I2C_MASTER_Val _U_(0x5)                                             /**< (SERCOM_I2CM_CTRLA) I2C master operation  */
#define SERCOM_I2CM_CTRLA_MODE_USART_EXT_CLK  (SERCOM_I2CM_CTRLA_MODE_USART_EXT_CLK_Val << SERCOM_I2CM_CTRLA_MODE_Pos) /**< (SERCOM_I2CM_CTRLA) USART with external clock Position  */
#define SERCOM_I2CM_CTRLA_MODE_USART_INT_CLK  (SERCOM_I2CM_CTRLA_MODE_USART_INT_CLK_Val << SERCOM_I2CM_CTRLA_MODE_Pos) /**< (SERCOM_I2CM_CTRLA) USART with internal clock Position  */
#define SERCOM_I2CM_CTRLA_MODE_SPI_SLAVE      (SERCOM_I2CM_CTRLA_MODE_SPI_SLAVE_Val << SERCOM_I2CM_CTRLA_MODE_Pos) /**< (SERCOM_I2CM_CTRLA) SPI in slave operation Position  */
#define SERCOM_I2CM_CTRLA_MODE_SPI_MASTER     (SERCOM_I2CM_CTRLA_MODE_SPI_MASTER_Val << SERCOM_I2CM_CTRLA_MODE_Pos) /**< (SERCOM_I2CM_CTRLA) SPI in master operation Position  */
#define SERCOM_I2CM_CTRLA_MODE_I2C_SLAVE      (SERCOM_I2CM_CTRLA_MODE_I2C_SLAVE_Val << SERCOM_I2CM_CTRLA_MODE_Pos) /**< (SERCOM_I2CM_CTRLA) I2C slave operation Position  */
#define SERCOM_I2CM_CTRLA_MODE_I2C_MASTER     (SERCOM_I2CM_CTRLA_MODE_I2C_MASTER_Val << SERCOM_I2CM_CTRLA_MODE_Pos) /**< (SERCOM_I2CM_CTRLA) I2C master operation Position  */
#define SERCOM_I2CM_CTRLA_RUNSTDBY_Pos        _U_(7)                                               /**< (SERCOM_I2CM_CTRLA) Run in Standby Position */
#define SERCOM_I2CM_CTRLA_RUNSTDBY_Msk        (_U_(0x1) << SERCOM_I2CM_CTRLA_RUNSTDBY_Pos)         /**< (SERCOM_I2CM_CTRLA) Run in Standby Mask */
#define SERCOM_I2CM_CTRLA_RUNSTDBY(value)     (SERCOM_I2CM_CTRLA_RUNSTDBY_Msk & ((value) << SERCOM_I2CM_CTRLA_RUNSTDBY_Pos))
#define SERCOM_I2CM_CTRLA_PINOUT_Pos          _U_(16)                                              /**< (SERCOM_I2CM_CTRLA) Pin Usage Position */
#define SERCOM_I2CM_CTRLA_PINOUT_Msk          (_U_(0x1) << SERCOM_I2CM_CTRLA_PINOUT_Pos)           /**< (SERCOM_I2CM_CTRLA) Pin Usage Mask */
#define SERCOM_I2CM_CTRLA_PINOUT(value)       (SERCOM_I2CM_CTRLA_PINOUT_Msk & ((value) << SERCOM_I2CM_CTRLA_PINOUT_Pos))
#define SERCOM_I2CM_CTRLA_SDAHOLD_Pos         _U_(20)                                              /**< (SERCOM_I2CM_CTRLA) SDA Hold Time Position */
#define SERCOM_I2CM_CTRLA_SDAHOLD_Msk         (_U_(0x3) << SERCOM_I2CM_CTRLA_SDAHOLD_Pos)          /**< (SERCOM_I2CM_CTRLA) SDA Hold Time Mask */
#define SERCOM_I2CM_CTRLA_SDAHOLD(value)      (SERCOM_I2CM_CTRLA_SDAHOLD_Msk & ((value) << SERCOM_I2CM_CTRLA_SDAHOLD_Pos))
#define   SERCOM_I2CM_CTRLA_SDAHOLD_DISABLE_Val _U_(0x0)                                             /**< (SERCOM_I2CM_CTRLA) Disabled  */
#define   SERCOM_I2CM_CTRLA_SDAHOLD_75NS_Val  _U_(0x1)                                             /**< (SERCOM_I2CM_CTRLA) 50-100ns hold time  */
#define   SERCOM_I2CM_CTRLA_SDAHOLD_450NS_Val _U_(0x2)                                             /**< (SERCOM_I2CM_CTRLA) 300-600ns hold time  */
#define   SERCOM_I2CM_CTRLA_SDAHOLD_600NS_Val _U_(0x3)                                             /**< (SERCOM_I2CM_CTRLA) 400-800ns hold time  */
#define SERCOM_I2CM_CTRLA_SDAHOLD_DISABLE     (SERCOM_I2CM_CTRLA_SDAHOLD_DISABLE_Val << SERCOM_I2CM_CTRLA_SDAHOLD_Pos) /**< (SERCOM_I2CM_CTRLA) Disabled Position  */
#define SERCOM_I2CM_CTRLA_SDAHOLD_75NS        (SERCOM_I2CM_CTRLA_SDAHOLD_75NS_Val << SERCOM_I2CM_CTRLA_SDAHOLD_Pos) /**< (SERCOM_I2CM_CTRLA) 50-100ns hold time Position  */
#define SERCOM_I2CM_CTRLA_SDAHOLD_450NS       (SERCOM_I2CM_CTRLA_SDAHOLD_450NS_Val << SERCOM_I2CM_CTRLA_SDAHOLD_Pos) /**< (SERCOM_I2CM_CTRLA) 300-600ns hold time Position  */
#define SERCOM_I2CM_CTRLA_SDAHOLD_600NS       (SERCOM_I2CM_CTRLA_SDAHOLD_600NS_Val << SERCOM_I2CM_CTRLA_SDAHOLD_Pos) /**< (SERCOM_I2CM_CTRLA) 400-800ns hold time Position  */
#define SERCOM_I2CM_CTRLA_INACTOUT_Pos        _U_(28)                                              /**< (SERCOM_I2CM_CTRLA) Inactive Time-out Position */
#define SERCOM_I2CM_CTRLA_INACTOUT_Msk        (_U_(0x3) << SERCOM_I2CM_CTRLA_INACTOUT_Pos)         /**< (SERCOM_I2CM_CTRLA) Inactive Time-out Mask */
#define SERCOM_I2CM_CTRLA_INACTOUT(value)     (SERCOM_I2CM_CTRLA_INACTOUT_Msk & ((value) << SERCOM_I2CM_CTRLA_INACTOUT_Pos))
#define   SERCOM_I2CM_CTRLA_INACTOUT_DISABLE_Val _U_(0x0)                                             /**< (SERCOM_I2CM_CTRLA) Disabled  */
#define   SERCOM_I2CM_CTRLA_INACTOUT_55US_Val _U_(0x1)                                             /**< (SERCOM_I2CM_CTRLA) 5-6 SCL Time-Out(50-60us)  */
#define   SERCOM_I2CM_CTRLA_INACTOUT_105US_Val _U_(0x2)                                             /**< (SERCOM_I2CM_CTRLA) 10-11 SCL Time-Out(100-110us)  */
#define   SERCOM_I2CM_CTRLA_INACTOUT_205US_Val _U_(0x3)                                             /**< (SERCOM_I2CM_CTRLA) 20-21 SCL Time-Out(200-210us)  */
#define SERCOM_I2CM_CTRLA_INACTOUT_DISABLE    (SERCOM_I2CM_CTRLA_INACTOUT_DISABLE_Val << SERCOM_I2CM_CTRLA_INACTOUT_Pos) /**< (SERCOM_I2CM_CTRLA) Disabled Position  */
#define SERCOM_I2CM_CTRLA_INACTOUT_55US       (SERCOM_I2CM_CTRLA_INACTOUT_55US_Val << SERCOM_I2CM_CTRLA_INACTOUT_Pos) /**< (SERCOM_I2CM_CTRLA) 5-6 SCL Time-Out(50-60us) Position  */
#define SERCOM_I2CM_CTRLA_INACTOUT_105US      (SERCOM_I2CM_CTRLA_INACTOUT_105US_Val << SERCOM_I2CM_CTRLA_INACTOUT_Pos) /**< (SERCOM_I2CM_CTRLA) 10-11 SCL Time-Out(100-110us) Position  */
#define SERCOM_I2CM_CTRLA_INACTOUT_205US      (SERCOM_I2CM_CTRLA_INACTOUT_205US_Val << SERCOM_I2CM_CTRLA_INACTOUT_Pos) /**< (SERCOM_I2CM_CTRLA) 20-21 SCL Time-Out(200-210us) Position  */
#define SERCOM_I2CM_CTRLA_LOWTOUT_Pos         _U_(30)                                              /**< (SERCOM_I2CM_CTRLA) SCL Low Time-out Position */
#define SERCOM_I2CM_CTRLA_LOWTOUT_Msk         (_U_(0x1) << SERCOM_I2CM_CTRLA_LOWTOUT_Pos)          /**< (SERCOM_I2CM_CTRLA) SCL Low Time-out Mask */
#define SERCOM_I2CM_CTRLA_LOWTOUT(value)      (SERCOM_I2CM_CTRLA_LOWTOUT_Msk & ((value) << SERCOM_I2CM_CTRLA_LOWTOUT_Pos))
#define SERCOM_I2CM_CTRLA_Msk                 _U_(0x7031009F)                                      /**< (SERCOM_I2CM_CTRLA) Register Mask  */


/* -------- SERCOM_I2CS_CTRLA : (SERCOM Offset: 0x00) (R/W 32) I2CS Control A -------- */
#define SERCOM_I2CS_CTRLA_RESETVALUE          _U_(0x00)                                            /**<  (SERCOM_I2CS_CTRLA) I2CS Control A  Reset Value */

#define SERCOM_I2CS_CTRLA_SWRST_Pos           _U_(0)                                               /**< (SERCOM_I2CS_CTRLA) Software Reset Position */
#define SERCOM_I2CS_CTRLA_SWRST_Msk           (_U_(0x1) << SERCOM_I2CS_CTRLA_SWRST_Pos)            /**< (SERCOM_I2CS_CTRLA) Software Reset Mask */
#define SERCOM_I2CS_CTRLA_SWRST(value)        (SERCOM_I2CS_CTRLA_SWRST_Msk & ((value) << SERCOM_I2CS_CTRLA_SWRST_Pos))
#define SERCOM_I2CS_CTRLA_ENABLE_Pos          _U_(1)                                               /**< (SERCOM_I2CS_CTRLA) Enable Position */
#define SERCOM_I2CS_CTRLA_ENABLE_Msk          (_U_(0x1) << SERCOM_I2CS_CTRLA_ENABLE_Pos)           /**< (SERCOM_I2CS_CTRLA) Enable Mask */
#define SERCOM_I2CS_CTRLA_ENABLE(value)       (SERCOM_I2CS_CTRLA_ENABLE_Msk & ((value) << SERCOM_I2CS_CTRLA_ENABLE_Pos))
#define SERCOM_I2CS_CTRLA_MODE_Pos            _U_(2)                                               /**< (SERCOM_I2CS_CTRLA) Operating Mode Position */
#define SERCOM_I2CS_CTRLA_MODE_Msk            (_U_(0x7) << SERCOM_I2CS_CTRLA_MODE_Pos)             /**< (SERCOM_I2CS_CTRLA) Operating Mode Mask */
#define SERCOM_I2CS_CTRLA_MODE(value)         (SERCOM_I2CS_CTRLA_MODE_Msk & ((value) << SERCOM_I2CS_CTRLA_MODE_Pos))
#define   SERCOM_I2CS_CTRLA_MODE_USART_EXT_CLK_Val _U_(0x0)                                             /**< (SERCOM_I2CS_CTRLA) USART with external clock  */
#define   SERCOM_I2CS_CTRLA_MODE_USART_INT_CLK_Val _U_(0x1)                                             /**< (SERCOM_I2CS_CTRLA) USART with internal clock  */
#define   SERCOM_I2CS_CTRLA_MODE_SPI_SLAVE_Val _U_(0x2)                                             /**< (SERCOM_I2CS_CTRLA) SPI in slave operation  */
#define   SERCOM_I2CS_CTRLA_MODE_SPI_MASTER_Val _U_(0x3)                                             /**< (SERCOM_I2CS_CTRLA) SPI in master operation  */
#define   SERCOM_I2CS_CTRLA_MODE_I2C_SLAVE_Val _U_(0x4)                                             /**< (SERCOM_I2CS_CTRLA) I2C slave operation  */
#define   SERCOM_I2CS_CTRLA_MODE_I2C_MASTER_Val _U_(0x5)                                             /**< (SERCOM_I2CS_CTRLA) I2C master operation  */
#define SERCOM_I2CS_CTRLA_MODE_USART_EXT_CLK  (SERCOM_I2CS_CTRLA_MODE_USART_EXT_CLK_Val << SERCOM_I2CS_CTRLA_MODE_Pos) /**< (SERCOM_I2CS_CTRLA) USART with external clock Position  */
#define SERCOM_I2CS_CTRLA_MODE_USART_INT_CLK  (SERCOM_I2CS_CTRLA_MODE_USART_INT_CLK_Val << SERCOM_I2CS_CTRLA_MODE_Pos) /**< (SERCOM_I2CS_CTRLA) USART with internal clock Position  */
#define SERCOM_I2CS_CTRLA_MODE_SPI_SLAVE      (SERCOM_I2CS_CTRLA_MODE_SPI_SLAVE_Val << SERCOM_I2CS_CTRLA_MODE_Pos) /**< (SERCOM_I2CS_CTRLA) SPI in slave operation Position  */
#define SERCOM_I2CS_CTRLA_MODE_SPI_MASTER     (SERCOM_I2CS_CTRLA_MODE_SPI_MASTER_Val << SERCOM_I2CS_CTRLA_MODE_Pos) /**< (SERCOM_I2CS_CTRLA) SPI in master operation Position  */
#define SERCOM_I2CS_CTRLA_MODE_I2C_SLAVE      (SERCOM_I2CS_CTRLA_MODE_I2C_SLAVE_Val << SERCOM_I2CS_CTRLA_MODE_Pos) /**< (SERCOM_I2CS_CTRLA) I2C slave operation Position  */
#define SERCOM_I2CS_CTRLA_MODE_I2C_MASTER     (SERCOM_I2CS_CTRLA_MODE_I2C_MASTER_Val << SERCOM_I2CS_CTRLA_MODE_Pos) /**< (SERCOM_I2CS_CTRLA) I2C master operation Position  */
#define SERCOM_I2CS_CTRLA_RUNSTDBY_Pos        _U_(7)                                               /**< (SERCOM_I2CS_CTRLA) Run in Standby Position */
#define SERCOM_I2CS_CTRLA_RUNSTDBY_Msk        (_U_(0x1) << SERCOM_I2CS_CTRLA_RUNSTDBY_Pos)         /**< (SERCOM_I2CS_CTRLA) Run in Standby Mask */
#define SERCOM_I2CS_CTRLA_RUNSTDBY(value)     (SERCOM_I2CS_CTRLA_RUNSTDBY_Msk & ((value) << SERCOM_I2CS_CTRLA_RUNSTDBY_Pos))
#define SERCOM_I2CS_CTRLA_PINOUT_Pos          _U_(16)                                              /**< (SERCOM_I2CS_CTRLA) Pin Usage Position */
#define SERCOM_I2CS_CTRLA_PINOUT_Msk          (_U_(0x1) << SERCOM_I2CS_CTRLA_PINOUT_Pos)           /**< (SERCOM_I2CS_CTRLA) Pin Usage Mask */
#define SERCOM_I2CS_CTRLA_PINOUT(value)       (SERCOM_I2CS_CTRLA_PINOUT_Msk & ((value) << SERCOM_I2CS_CTRLA_PINOUT_Pos))
#define SERCOM_I2CS_CTRLA_SDAHOLD_Pos         _U_(20)                                              /**< (SERCOM_I2CS_CTRLA) SDA Hold Time Position */
#define SERCOM_I2CS_CTRLA_SDAHOLD_Msk         (_U_(0x3) << SERCOM_I2CS_CTRLA_SDAHOLD_Pos)          /**< (SERCOM_I2CS_CTRLA) SDA Hold Time Mask */
#define SERCOM_I2CS_CTRLA_SDAHOLD(value)      (SERCOM_I2CS_CTRLA_SDAHOLD_Msk & ((value) << SERCOM_I2CS_CTRLA_SDAHOLD_Pos))
#define   SERCOM_I2CS_CTRLA_SDAHOLD_DISABLE_Val _U_(0x0)                                             /**< (SERCOM_I2CS_CTRLA) Disabled  */
#define   SERCOM_I2CS_CTRLA_SDAHOLD_75NS_Val  _U_(0x1)                                             /**< (SERCOM_I2CS_CTRLA) 50-100ns hold time  */
#define   SERCOM_I2CS_CTRLA_SDAHOLD_450NS_Val _U_(0x2)                                             /**< (SERCOM_I2CS_CTRLA) 300-600ns hold time  */
#define   SERCOM_I2CS_CTRLA_SDAHOLD_600NS_Val _U_(0x3)                                             /**< (SERCOM_I2CS_CTRLA) 400-800ns hold time  */
#define SERCOM_I2CS_CTRLA_SDAHOLD_DISABLE     (SERCOM_I2CS_CTRLA_SDAHOLD_DISABLE_Val << SERCOM_I2CS_CTRLA_SDAHOLD_Pos) /**< (SERCOM_I2CS_CTRLA) Disabled Position  */
#define SERCOM_I2CS_CTRLA_SDAHOLD_75NS        (SERCOM_I2CS_CTRLA_SDAHOLD_75NS_Val << SERCOM_I2CS_CTRLA_SDAHOLD_Pos) /**< (SERCOM_I2CS_CTRLA) 50-100ns hold time Position  */
#define SERCOM_I2CS_CTRLA_SDAHOLD_450NS       (SERCOM_I2CS_CTRLA_SDAHOLD_450NS_Val << SERCOM_I2CS_CTRLA_SDAHOLD_Pos) /**< (SERCOM_I2CS_CTRLA) 300-600ns hold time Position  */
#define SERCOM_I2CS_CTRLA_SDAHOLD_600NS       (SERCOM_I2CS_CTRLA_SDAHOLD_600NS_Val << SERCOM_I2CS_CTRLA_SDAHOLD_Pos) /**< (SERCOM_I2CS_CTRLA) 400-800ns hold time Position  */
#define SERCOM_I2CS_CTRLA_LOWTOUT_Pos         _U_(30)                                              /**< (SERCOM_I2CS_CTRLA) SCL Low Time-out Position */
#define SERCOM_I2CS_CTRLA_LOWTOUT_Msk         (_U_(0x1) << SERCOM_I2CS_CTRLA_LOWTOUT_Pos)          /**< (SERCOM_I2CS_CTRLA) SCL Low Time-out Mask */
#define SERCOM_I2CS_CTRLA_LOWTOUT(value)      (SERCOM_I2CS_CTRLA_LOWTOUT_Msk & ((value) << SERCOM_I2CS_CTRLA_LOWTOUT_Pos))
#define SERCOM_I2CS_CTRLA_Msk                 _U_(0x4031009F)                                      /**< (SERCOM_I2CS_CTRLA) Register Mask  */


/* -------- SERCOM_SPIM_CTRLA : (SERCOM Offset: 0x00) (R/W 32) SPIM Control A -------- */
#define SERCOM_SPIM_CTRLA_RESETVALUE          _U_(0x00)                                            /**<  (SERCOM_SPIM_CTRLA) SPIM Control A  Reset Value */

#define SERCOM_SPIM_CTRLA_SWRST_Pos           _U_(0)                                               /**< (SERCOM_SPIM_CTRLA) Software Reset Position */
#define SERCOM_SPIM_CTRLA_SWRST_Msk           (_U_(0x1) << SERCOM_SPIM_CTRLA_SWRST_Pos)            /**< (SERCOM_SPIM_CTRLA) Software Reset Mask */
#define SERCOM_SPIM_CTRLA_SWRST(value)        (SERCOM_SPIM_CTRLA_SWRST_Msk & ((value) << SERCOM_SPIM_CTRLA_SWRST_Pos))
#define SERCOM_SPIM_CTRLA_ENABLE_Pos          _U_(1)                                               /**< (SERCOM_SPIM_CTRLA) Enable Position */
#define SERCOM_SPIM_CTRLA_ENABLE_Msk          (_U_(0x1) << SERCOM_SPIM_CTRLA_ENABLE_Pos)           /**< (SERCOM_SPIM_CTRLA) Enable Mask */
#define SERCOM_SPIM_CTRLA_ENABLE(value)       (SERCOM_SPIM_CTRLA_ENABLE_Msk & ((value) << SERCOM_SPIM_CTRLA_ENABLE_Pos))
#define SERCOM_SPIM_CTRLA_MODE_Pos            _U_(2)                                               /**< (SERCOM_SPIM_CTRLA) Operating Mode Position */
#define SERCOM_SPIM_CTRLA_MODE_Msk            (_U_(0x7) << SERCOM_SPIM_CTRLA_MODE_Pos)             /**< (SERCOM_SPIM_CTRLA) Operating Mode Mask */
#define SERCOM_SPIM_CTRLA_MODE(value)         (SERCOM_SPIM_CTRLA_MODE_Msk & ((value) << SERCOM_SPIM_CTRLA_MODE_Pos))
#define   SERCOM_SPIM_CTRLA_MODE_USART_EXT_CLK_Val _U_(0x0)                                             /**< (SERCOM_SPIM_CTRLA) USART with external clock  */
#define   SERCOM_SPIM_CTRLA_MODE_USART_INT_CLK_Val _U_(0x1)                                             /**< (SERCOM_SPIM_CTRLA) USART with internal clock  */
#define   SERCOM_SPIM_CTRLA_MODE_SPI_SLAVE_Val _U_(0x2)                                             /**< (SERCOM_SPIM_CTRLA) SPI in slave operation  */
#define   SERCOM_SPIM_CTRLA_MODE_SPI_MASTER_Val _U_(0x3)                                             /**< (SERCOM_SPIM_CTRLA) SPI in master operation  */
#define   SERCOM_SPIM_CTRLA_MODE_I2C_SLAVE_Val _U_(0x4)                                             /**< (SERCOM_SPIM_CTRLA) I2C slave operation  */
#define   SERCOM_SPIM_CTRLA_MODE_I2C_MASTER_Val _U_(0x5)                                             /**< (SERCOM_SPIM_CTRLA) I2C master operation  */
#define SERCOM_SPIM_CTRLA_MODE_USART_EXT_CLK  (SERCOM_SPIM_CTRLA_MODE_USART_EXT_CLK_Val << SERCOM_SPIM_CTRLA_MODE_Pos) /**< (SERCOM_SPIM_CTRLA) USART with external clock Position  */
#define SERCOM_SPIM_CTRLA_MODE_USART_INT_CLK  (SERCOM_SPIM_CTRLA_MODE_USART_INT_CLK_Val << SERCOM_SPIM_CTRLA_MODE_Pos) /**< (SERCOM_SPIM_CTRLA) USART with internal clock Position  */
#define SERCOM_SPIM_CTRLA_MODE_SPI_SLAVE      (SERCOM_SPIM_CTRLA_MODE_SPI_SLAVE_Val << SERCOM_SPIM_CTRLA_MODE_Pos) /**< (SERCOM_SPIM_CTRLA) SPI in slave operation Position  */
#define SERCOM_SPIM_CTRLA_MODE_SPI_MASTER     (SERCOM_SPIM_CTRLA_MODE_SPI_MASTER_Val << SERCOM_SPIM_CTRLA_MODE_Pos) /**< (SERCOM_SPIM_CTRLA) SPI in master operation Position  */
#define SERCOM_SPIM_CTRLA_MODE_I2C_SLAVE      (SERCOM_SPIM_CTRLA_MODE_I2C_SLAVE_Val << SERCOM_SPIM_CTRLA_MODE_Pos) /**< (SERCOM_SPIM_CTRLA) I2C slave operation Position  */
#define SERCOM_SPIM_CTRLA_MODE_I2C_MASTER     (SERCOM_SPIM_CTRLA_MODE_I2C_MASTER_Val << SERCOM_SPIM_CTRLA_MODE_Pos) /**< (SERCOM_SPIM_CTRLA) I2C master operation Position  */
#define SERCOM_SPIM_CTRLA_RUNSTDBY_Pos        _U_(7)                                               /**< (SERCOM_SPIM_CTRLA) Run In Standby Position */
#define SERCOM_SPIM_CTRLA_RUNSTDBY_Msk        (_U_(0x1) << SERCOM_SPIM_CTRLA_RUNSTDBY_Pos)         /**< (SERCOM_SPIM_CTRLA) Run In Standby Mask */
#define SERCOM_SPIM_CTRLA_RUNSTDBY(value)     (SERCOM_SPIM_CTRLA_RUNSTDBY_Msk & ((value) << SERCOM_SPIM_CTRLA_RUNSTDBY_Pos))
#define SERCOM_SPIM_CTRLA_IBON_Pos            _U_(8)                                               /**< (SERCOM_SPIM_CTRLA) Immediate Buffer Overflow Notification Position */
#define SERCOM_SPIM_CTRLA_IBON_Msk            (_U_(0x1) << SERCOM_SPIM_CTRLA_IBON_Pos)             /**< (SERCOM_SPIM_CTRLA) Immediate Buffer Overflow Notification Mask */
#define SERCOM_SPIM_CTRLA_IBON(value)         (SERCOM_SPIM_CTRLA_IBON_Msk & ((value) << SERCOM_SPIM_CTRLA_IBON_Pos))
#define SERCOM_SPIM_CTRLA_DOPO_Pos            _U_(16)                                              /**< (SERCOM_SPIM_CTRLA) Data Out Pinout Position */
#define SERCOM_SPIM_CTRLA_DOPO_Msk            (_U_(0x3) << SERCOM_SPIM_CTRLA_DOPO_Pos)             /**< (SERCOM_SPIM_CTRLA) Data Out Pinout Mask */
#define SERCOM_SPIM_CTRLA_DOPO(value)         (SERCOM_SPIM_CTRLA_DOPO_Msk & ((value) << SERCOM_SPIM_CTRLA_DOPO_Pos))
#define   SERCOM_SPIM_CTRLA_DOPO_PAD0_Val     _U_(0x0)                                             /**< (SERCOM_SPIM_CTRLA) DO on PAD[0], SCK on PAD[1] and SS on PAD[2]  */
#define   SERCOM_SPIM_CTRLA_DOPO_PAD1_Val     _U_(0x1)                                             /**< (SERCOM_SPIM_CTRLA) DO on PAD[2], SCK on PAD[3] and SS on PAD[1]  */
#define   SERCOM_SPIM_CTRLA_DOPO_PAD2_Val     _U_(0x2)                                             /**< (SERCOM_SPIM_CTRLA) DO on PAD[3], SCK on PAD[1] and SS on PAD[2]  */
#define   SERCOM_SPIM_CTRLA_DOPO_PAD3_Val     _U_(0x3)                                             /**< (SERCOM_SPIM_CTRLA) DO on PAD[0], SCK on PAD[3] and SS on PAD[1]  */
#define SERCOM_SPIM_CTRLA_DOPO_PAD0           (SERCOM_SPIM_CTRLA_DOPO_PAD0_Val << SERCOM_SPIM_CTRLA_DOPO_Pos) /**< (SERCOM_SPIM_CTRLA) DO on PAD[0], SCK on PAD[1] and SS on PAD[2] Position  */
#define SERCOM_SPIM_CTRLA_DOPO_PAD1           (SERCOM_SPIM_CTRLA_DOPO_PAD1_Val << SERCOM_SPIM_CTRLA_DOPO_Pos) /**< (SERCOM_SPIM_CTRLA) DO on PAD[2], SCK on PAD[3] and SS on PAD[1] Position  */
#define SERCOM_SPIM_CTRLA_DOPO_PAD2           (SERCOM_SPIM_CTRLA_DOPO_PAD2_Val << SERCOM_SPIM_CTRLA_DOPO_Pos) /**< (SERCOM_SPIM_CTRLA) DO on PAD[3], SCK on PAD[1] and SS on PAD[2] Position  */
#define SERCOM_SPIM_CTRLA_DOPO_PAD3           (SERCOM_SPIM_CTRLA_DOPO_PAD3_Val << SERCOM_SPIM_CTRLA_DOPO_Pos) /**< (SERCOM_SPIM_CTRLA) DO on PAD[0], SCK on PAD[3] and SS on PAD[1] Position  */
#define SERCOM_SPIM_CTRLA_DIPO_Pos            _U_(20)                                              /**< (SERCOM_SPIM_CTRLA) Data In Pinout Position */
#define SERCOM_SPIM_CTRLA_DIPO_Msk            (_U_(0x3) << SERCOM_SPIM_CTRLA_DIPO_Pos)             /**< (SERCOM_SPIM_CTRLA) Data In Pinout Mask */
#define SERCOM_SPIM_CTRLA_DIPO(value)         (SERCOM_SPIM_CTRLA_DIPO_Msk & ((value) << SERCOM_SPIM_CTRLA_DIPO_Pos))
#define   SERCOM_SPIM_CTRLA_DIPO_PAD0_Val     _U_(0x0)                                             /**< (SERCOM_SPIM_CTRLA) SERCOM PAD[0]  */
#define   SERCOM_SPIM_CTRLA_DIPO_PAD1_Val     _U_(0x1)                                             /**< (SERCOM_SPIM_CTRLA) SERCOM PAD[1]  */
#define   SERCOM_SPIM_CTRLA_DIPO_PAD2_Val     _U_(0x2)                                             /**< (SERCOM_SPIM_CTRLA) SERCOM PAD[2]  */
#define   SERCOM_SPIM_CTRLA_DIPO_PAD3_Val     _U_(0x3)                                             /**< (SERCOM_SPIM_CTRLA) SERCOM PAD[3]  */
#define SERCOM_SPIM_CTRLA_DIPO_PAD0           (SERCOM_SPIM_CTRLA_DIPO_PAD0_Val << SERCOM_SPIM_CTRLA_DIPO_Pos) /**< (SERCOM_SPIM_CTRLA) SERCOM PAD[0] Position  */
#define SERCOM_SPIM_CTRLA_DIPO_PAD1           (SERCOM_SPIM_CTRLA_DIPO_PAD1_Val << SERCOM_SPIM_CTRLA_DIPO_Pos) /**< (SERCOM_SPIM_CTRLA) SERCOM PAD[1] Position  */
#define SERCOM_SPIM_CTRLA_DIPO_PAD2           (SERCOM_SPIM_CTRLA_DIPO_PAD2_Val << SERCOM_SPIM_CTRLA_DIPO_Pos) /**< (SERCOM_SPIM_CTRLA) SERCOM PAD[2] Position  */
#define SERCOM_SPIM_CTRLA_DIPO_PAD3           (SERCOM_SPIM_CTRLA_DIPO_PAD3_Val << SERCOM_SPIM_CTRLA_DIPO_Pos) /**< (SERCOM_SPIM_CTRLA) SERCOM PAD[3] Position  */
#define SERCOM_SPIM_CTRLA_FORM_Pos            _U_(24)                                              /**< (SERCOM_SPIM_CTRLA) Frame Format Position */
#define SERCOM_SPIM_CTRLA_FORM_Msk            (_U_(0xF) << SERCOM_SPIM_CTRLA_FORM_Pos)             /**< (SERCOM_SPIM_CTRLA) Frame Format Mask */
#define SERCOM_SPIM_CTRLA_FORM(value)         (SERCOM_SPIM_CTRLA_FORM_Msk & ((value) << SERCOM_SPIM_CTRLA_FORM_Pos))
#define   SERCOM_SPIM_CTRLA_FORM_SPI_FRAME_Val _U_(0x0)                                             /**< (SERCOM_SPIM_CTRLA) SPI Frame  */
#define   SERCOM_SPIM_CTRLA_FORM_SPI_FRAME_WITH_ADDR_Val _U_(0x2)                                             /**< (SERCOM_SPIM_CTRLA) SPI Frame with Addr  */
#define SERCOM_SPIM_CTRLA_FORM_SPI_FRAME      (SERCOM_SPIM_CTRLA_FORM_SPI_FRAME_Val << SERCOM_SPIM_CTRLA_FORM_Pos) /**< (SERCOM_SPIM_CTRLA) SPI Frame Position  */
#define SERCOM_SPIM_CTRLA_FORM_SPI_FRAME_WITH_ADDR (SERCOM_SPIM_CTRLA_FORM_SPI_FRAME_WITH_ADDR_Val << SERCOM_SPIM_CTRLA_FORM_Pos) /**< (SERCOM_SPIM_CTRLA) SPI Frame with Addr Position  */
#define SERCOM_SPIM_CTRLA_CPHA_Pos            _U_(28)                                              /**< (SERCOM_SPIM_CTRLA) Clock Phase Position */
#define SERCOM_SPIM_CTRLA_CPHA_Msk            (_U_(0x1) << SERCOM_SPIM_CTRLA_CPHA_Pos)             /**< (SERCOM_SPIM_CTRLA) Clock Phase Mask */
#define SERCOM_SPIM_CTRLA_CPHA(value)         (SERCOM_SPIM_CTRLA_CPHA_Msk & ((value) << SERCOM_SPIM_CTRLA_CPHA_Pos))
#define   SERCOM_SPIM_CTRLA_CPHA_LEADING_EDGE_Val _U_(0x0)                                             /**< (SERCOM_SPIM_CTRLA) The data is sampled on a leading SCK edge and changed on a trailing SCK edge  */
#define   SERCOM_SPIM_CTRLA_CPHA_TRAILING_EDGE_Val _U_(0x1)                                             /**< (SERCOM_SPIM_CTRLA) The data is sampled on a trailing SCK edge and changed on a leading SCK edge  */
#define SERCOM_SPIM_CTRLA_CPHA_LEADING_EDGE   (SERCOM_SPIM_CTRLA_CPHA_LEADING_EDGE_Val << SERCOM_SPIM_CTRLA_CPHA_Pos) /**< (SERCOM_SPIM_CTRLA) The data is sampled on a leading SCK edge and changed on a trailing SCK edge Position  */
#define SERCOM_SPIM_CTRLA_CPHA_TRAILING_EDGE  (SERCOM_SPIM_CTRLA_CPHA_TRAILING_EDGE_Val << SERCOM_SPIM_CTRLA_CPHA_Pos) /**< (SERCOM_SPIM_CTRLA) The data is sampled on a trailing SCK edge and changed on a leading SCK edge Position  */
#define SERCOM_SPIM_CTRLA_CPOL_Pos            _U_(29)                                              /**< (SERCOM_SPIM_CTRLA) Clock Polarity Position */
#define SERCOM_SPIM_CTRLA_CPOL_Msk            (_U_(0x1) << SERCOM_SPIM_CTRLA_CPOL_Pos)             /**< (SERCOM_SPIM_CTRLA) Clock Polarity Mask */
#define SERCOM_SPIM_CTRLA_CPOL(value)         (SERCOM_SPIM_CTRLA_CPOL_Msk & ((value) << SERCOM_SPIM_CTRLA_CPOL_Pos))
#define   SERCOM_SPIM_CTRLA_CPOL_IDLE_LOW_Val _U_(0x0)                                             /**< (SERCOM_SPIM_CTRLA) SCK is low when idle  */
#define   SERCOM_SPIM_CTRLA_CPOL_IDLE_HIGH_Val _U_(0x1)                                             /**< (SERCOM_SPIM_CTRLA) SCK is high when idle  */
#define SERCOM_SPIM_CTRLA_CPOL_IDLE_LOW       (SERCOM_SPIM_CTRLA_CPOL_IDLE_LOW_Val << SERCOM_SPIM_CTRLA_CPOL_Pos) /**< (SERCOM_SPIM_CTRLA) SCK is low when idle Position  */
#define SERCOM_SPIM_CTRLA_CPOL_IDLE_HIGH      (SERCOM_SPIM_CTRLA_CPOL_IDLE_HIGH_Val << SERCOM_SPIM_CTRLA_CPOL_Pos) /**< (SERCOM_SPIM_CTRLA) SCK is high when idle Position  */
#define SERCOM_SPIM_CTRLA_DORD_Pos            _U_(30)                                              /**< (SERCOM_SPIM_CTRLA) Data Order Position */
#define SERCOM_SPIM_CTRLA_DORD_Msk            (_U_(0x1) << SERCOM_SPIM_CTRLA_DORD_Pos)             /**< (SERCOM_SPIM_CTRLA) Data Order Mask */
#define SERCOM_SPIM_CTRLA_DORD(value)         (SERCOM_SPIM_CTRLA_DORD_Msk & ((value) << SERCOM_SPIM_CTRLA_DORD_Pos))
#define   SERCOM_SPIM_CTRLA_DORD_MSB_Val      _U_(0x0)                                             /**< (SERCOM_SPIM_CTRLA) MSB is transferred first  */
#define   SERCOM_SPIM_CTRLA_DORD_LSB_Val      _U_(0x1)                                             /**< (SERCOM_SPIM_CTRLA) LSB is transferred first  */
#define SERCOM_SPIM_CTRLA_DORD_MSB            (SERCOM_SPIM_CTRLA_DORD_MSB_Val << SERCOM_SPIM_CTRLA_DORD_Pos) /**< (SERCOM_SPIM_CTRLA) MSB is transferred first Position  */
#define SERCOM_SPIM_CTRLA_DORD_LSB            (SERCOM_SPIM_CTRLA_DORD_LSB_Val << SERCOM_SPIM_CTRLA_DORD_Pos) /**< (SERCOM_SPIM_CTRLA) LSB is transferred first Position  */
#define SERCOM_SPIM_CTRLA_Msk                 _U_(0x7F33019F)                                      /**< (SERCOM_SPIM_CTRLA) Register Mask  */


/* -------- SERCOM_SPIS_CTRLA : (SERCOM Offset: 0x00) (R/W 32) SPIS Control A -------- */
#define SERCOM_SPIS_CTRLA_RESETVALUE          _U_(0x00)                                            /**<  (SERCOM_SPIS_CTRLA) SPIS Control A  Reset Value */

#define SERCOM_SPIS_CTRLA_SWRST_Pos           _U_(0)                                               /**< (SERCOM_SPIS_CTRLA) Software Reset Position */
#define SERCOM_SPIS_CTRLA_SWRST_Msk           (_U_(0x1) << SERCOM_SPIS_CTRLA_SWRST_Pos)            /**< (SERCOM_SPIS_CTRLA) Software Reset Mask */
#define SERCOM_SPIS_CTRLA_SWRST(value)        (SERCOM_SPIS_CTRLA_SWRST_Msk & ((value) << SERCOM_SPIS_CTRLA_SWRST_Pos))
#define SERCOM_SPIS_CTRLA_ENABLE_Pos          _U_(1)                                               /**< (SERCOM_SPIS_CTRLA) Enable Position */
#define SERCOM_SPIS_CTRLA_ENABLE_Msk          (_U_(0x1) << SERCOM_SPIS_CTRLA_ENABLE_Pos)           /**< (SERCOM_SPIS_CTRLA) Enable Mask */
#define SERCOM_SPIS_CTRLA_ENABLE(value)       (SERCOM_SPIS_CTRLA_ENABLE_Msk & ((value) << SERCOM_SPIS_CTRLA_ENABLE_Pos))
#define SERCOM_SPIS_CTRLA_MODE_Pos            _U_(2)                                               /**< (SERCOM_SPIS_CTRLA) Operating Mode Position */
#define SERCOM_SPIS_CTRLA_MODE_Msk            (_U_(0x7) << SERCOM_SPIS_CTRLA_MODE_Pos)             /**< (SERCOM_SPIS_CTRLA) Operating Mode Mask */
#define SERCOM_SPIS_CTRLA_MODE(value)         (SERCOM_SPIS_CTRLA_MODE_Msk & ((value) << SERCOM_SPIS_CTRLA_MODE_Pos))
#define   SERCOM_SPIS_CTRLA_MODE_USART_EXT_CLK_Val _U_(0x0)                                             /**< (SERCOM_SPIS_CTRLA) USART with external clock  */
#define   SERCOM_SPIS_CTRLA_MODE_USART_INT_CLK_Val _U_(0x1)                                             /**< (SERCOM_SPIS_CTRLA) USART with internal clock  */
#define   SERCOM_SPIS_CTRLA_MODE_SPI_SLAVE_Val _U_(0x2)                                             /**< (SERCOM_SPIS_CTRLA) SPI in slave operation  */
#define   SERCOM_SPIS_CTRLA_MODE_SPI_MASTER_Val _U_(0x3)                                             /**< (SERCOM_SPIS_CTRLA) SPI in master operation  */
#define   SERCOM_SPIS_CTRLA_MODE_I2C_SLAVE_Val _U_(0x4)                                             /**< (SERCOM_SPIS_CTRLA) I2C slave operation  */
#define   SERCOM_SPIS_CTRLA_MODE_I2C_MASTER_Val _U_(0x5)                                             /**< (SERCOM_SPIS_CTRLA) I2C master operation  */
#define SERCOM_SPIS_CTRLA_MODE_USART_EXT_CLK  (SERCOM_SPIS_CTRLA_MODE_USART_EXT_CLK_Val << SERCOM_SPIS_CTRLA_MODE_Pos) /**< (SERCOM_SPIS_CTRLA) USART with external clock Position  */
#define SERCOM_SPIS_CTRLA_MODE_USART_INT_CLK  (SERCOM_SPIS_CTRLA_MODE_USART_INT_CLK_Val << SERCOM_SPIS_CTRLA_MODE_Pos) /**< (SERCOM_SPIS_CTRLA) USART with internal clock Position  */
#define SERCOM_SPIS_CTRLA_MODE_SPI_SLAVE      (SERCOM_SPIS_CTRLA_MODE_SPI_SLAVE_Val << SERCOM_SPIS_CTRLA_MODE_Pos) /**< (SERCOM_SPIS_CTRLA) SPI in slave operation Position  */
#define SERCOM_SPIS_CTRLA_MODE_SPI_MASTER     (SERCOM_SPIS_CTRLA_MODE_SPI_MASTER_Val << SERCOM_SPIS_CTRLA_MODE_Pos) /**< (SERCOM_SPIS_CTRLA) SPI in master operation Position  */
#define SERCOM_SPIS_CTRLA_MODE_I2C_SLAVE      (SERCOM_SPIS_CTRLA_MODE_I2C_SLAVE_Val << SERCOM_SPIS_CTRLA_MODE_Pos) /**< (SERCOM_SPIS_CTRLA) I2C slave operation Position  */
#define SERCOM_SPIS_CTRLA_MODE_I2C_MASTER     (SERCOM_SPIS_CTRLA_MODE_I2C_MASTER_Val << SERCOM_SPIS_CTRLA_MODE_Pos) /**< (SERCOM_SPIS_CTRLA) I2C master operation Position  */
#define SERCOM_SPIS_CTRLA_RUNSTDBY_Pos        _U_(7)                                               /**< (SERCOM_SPIS_CTRLA) Run In Standby Position */
#define SERCOM_SPIS_CTRLA_RUNSTDBY_Msk        (_U_(0x1) << SERCOM_SPIS_CTRLA_RUNSTDBY_Pos)         /**< (SERCOM_SPIS_CTRLA) Run In Standby Mask */
#define SERCOM_SPIS_CTRLA_RUNSTDBY(value)     (SERCOM_SPIS_CTRLA_RUNSTDBY_Msk & ((value) << SERCOM_SPIS_CTRLA_RUNSTDBY_Pos))
#define SERCOM_SPIS_CTRLA_IBON_Pos            _U_(8)                                               /**< (SERCOM_SPIS_CTRLA) Immediate Buffer Overflow Notification Position */
#define SERCOM_SPIS_CTRLA_IBON_Msk            (_U_(0x1) << SERCOM_SPIS_CTRLA_IBON_Pos)             /**< (SERCOM_SPIS_CTRLA) Immediate Buffer Overflow Notification Mask */
#define SERCOM_SPIS_CTRLA_IBON(value)         (SERCOM_SPIS_CTRLA_IBON_Msk & ((value) << SERCOM_SPIS_CTRLA_IBON_Pos))
#define SERCOM_SPIS_CTRLA_DOPO_Pos            _U_(16)                                              /**< (SERCOM_SPIS_CTRLA) Data Out Pinout Position */
#define SERCOM_SPIS_CTRLA_DOPO_Msk            (_U_(0x3) << SERCOM_SPIS_CTRLA_DOPO_Pos)             /**< (SERCOM_SPIS_CTRLA) Data Out Pinout Mask */
#define SERCOM_SPIS_CTRLA_DOPO(value)         (SERCOM_SPIS_CTRLA_DOPO_Msk & ((value) << SERCOM_SPIS_CTRLA_DOPO_Pos))
#define   SERCOM_SPIS_CTRLA_DOPO_PAD0_Val     _U_(0x0)                                             /**< (SERCOM_SPIS_CTRLA) DO on PAD[0], SCK on PAD[1] and SS on PAD[2]  */
#define   SERCOM_SPIS_CTRLA_DOPO_PAD1_Val     _U_(0x1)                                             /**< (SERCOM_SPIS_CTRLA) DO on PAD[2], SCK on PAD[3] and SS on PAD[1]  */
#define   SERCOM_SPIS_CTRLA_DOPO_PAD2_Val     _U_(0x2)                                             /**< (SERCOM_SPIS_CTRLA) DO on PAD[3], SCK on PAD[1] and SS on PAD[2]  */
#define   SERCOM_SPIS_CTRLA_DOPO_PAD3_Val     _U_(0x3)                                             /**< (SERCOM_SPIS_CTRLA) DO on PAD[0], SCK on PAD[3] and SS on PAD[1]  */
#define SERCOM_SPIS_CTRLA_DOPO_PAD0           (SERCOM_SPIS_CTRLA_DOPO_PAD0_Val << SERCOM_SPIS_CTRLA_DOPO_Pos) /**< (SERCOM_SPIS_CTRLA) DO on PAD[0], SCK on PAD[1] and SS on PAD[2] Position  */
#define SERCOM_SPIS_CTRLA_DOPO_PAD1           (SERCOM_SPIS_CTRLA_DOPO_PAD1_Val << SERCOM_SPIS_CTRLA_DOPO_Pos) /**< (SERCOM_SPIS_CTRLA) DO on PAD[2], SCK on PAD[3] and SS on PAD[1] Position  */
#define SERCOM_SPIS_CTRLA_DOPO_PAD2           (SERCOM_SPIS_CTRLA_DOPO_PAD2_Val << SERCOM_SPIS_CTRLA_DOPO_Pos) /**< (SERCOM_SPIS_CTRLA) DO on PAD[3], SCK on PAD[1] and SS on PAD[2] Position  */
#define SERCOM_SPIS_CTRLA_DOPO_PAD3           (SERCOM_SPIS_CTRLA_DOPO_PAD3_Val << SERCOM_SPIS_CTRLA_DOPO_Pos) /**< (SERCOM_SPIS_CTRLA) DO on PAD[0], SCK on PAD[3] and SS on PAD[1] Position  */
#define SERCOM_SPIS_CTRLA_DIPO_Pos            _U_(20)                                              /**< (SERCOM_SPIS_CTRLA) Data In Pinout Position */
#define SERCOM_SPIS_CTRLA_DIPO_Msk            (_U_(0x3) << SERCOM_SPIS_CTRLA_DIPO_Pos)             /**< (SERCOM_SPIS_CTRLA) Data In Pinout Mask */
#define SERCOM_SPIS_CTRLA_DIPO(value)         (SERCOM_SPIS_CTRLA_DIPO_Msk & ((value) << SERCOM_SPIS_CTRLA_DIPO_Pos))
#define   SERCOM_SPIS_CTRLA_DIPO_PAD0_Val     _U_(0x0)                                             /**< (SERCOM_SPIS_CTRLA) SERCOM PAD[0]  */
#define   SERCOM_SPIS_CTRLA_DIPO_PAD1_Val     _U_(0x1)                                             /**< (SERCOM_SPIS_CTRLA) SERCOM PAD[1]  */
#define   SERCOM_SPIS_CTRLA_DIPO_PAD2_Val     _U_(0x2)                                             /**< (SERCOM_SPIS_CTRLA) SERCOM PAD[2]  */
#define   SERCOM_SPIS_CTRLA_DIPO_PAD3_Val     _U_(0x3)                                             /**< (SERCOM_SPIS_CTRLA) SERCOM PAD[3]  */
#define SERCOM_SPIS_CTRLA_DIPO_PAD0           (SERCOM_SPIS_CTRLA_DIPO_PAD0_Val << SERCOM_SPIS_CTRLA_DIPO_Pos) /**< (SERCOM_SPIS_CTRLA) SERCOM PAD[0] Position  */
#define SERCOM_SPIS_CTRLA_DIPO_PAD1           (SERCOM_SPIS_CTRLA_DIPO_PAD1_Val << SERCOM_SPIS_CTRLA_DIPO_Pos) /**< (SERCOM_SPIS_CTRLA) SERCOM PAD[1] Position  */
#define SERCOM_SPIS_CTRLA_DIPO_PAD2           (SERCOM_SPIS_CTRLA_DIPO_PAD2_Val << SERCOM_SPIS_CTRLA_DIPO_Pos) /**< (SERCOM_SPIS_CTRLA) SERCOM PAD[2] Position  */
#define SERCOM_SPIS_CTRLA_DIPO_PAD3           (SERCOM_SPIS_CTRLA_DIPO_PAD3_Val << SERCOM_SPIS_CTRLA_DIPO_Pos) /**< (SERCOM_SPIS_CTRLA) SERCOM PAD[3] Position  */
#define SERCOM_SPIS_CTRLA_FORM_Pos            _U_(24)                                              /**< (SERCOM_SPIS_CTRLA) Frame Format Position */
#define SERCOM_SPIS_CTRLA_FORM_Msk            (_U_(0xF) << SERCOM_SPIS_CTRLA_FORM_Pos)             /**< (SERCOM_SPIS_CTRLA) Frame Format Mask */
#define SERCOM_SPIS_CTRLA_FORM(value)         (SERCOM_SPIS_CTRLA_FORM_Msk & ((value) << SERCOM_SPIS_CTRLA_FORM_Pos))
#define   SERCOM_SPIS_CTRLA_FORM_SPI_FRAME_Val _U_(0x0)                                             /**< (SERCOM_SPIS_CTRLA) SPI Frame  */
#define   SERCOM_SPIS_CTRLA_FORM_SPI_FRAME_WITH_ADDR_Val _U_(0x2)                                             /**< (SERCOM_SPIS_CTRLA) SPI Frame with Addr  */
#define SERCOM_SPIS_CTRLA_FORM_SPI_FRAME      (SERCOM_SPIS_CTRLA_FORM_SPI_FRAME_Val << SERCOM_SPIS_CTRLA_FORM_Pos) /**< (SERCOM_SPIS_CTRLA) SPI Frame Position  */
#define SERCOM_SPIS_CTRLA_FORM_SPI_FRAME_WITH_ADDR (SERCOM_SPIS_CTRLA_FORM_SPI_FRAME_WITH_ADDR_Val << SERCOM_SPIS_CTRLA_FORM_Pos) /**< (SERCOM_SPIS_CTRLA) SPI Frame with Addr Position  */
#define SERCOM_SPIS_CTRLA_CPHA_Pos            _U_(28)                                              /**< (SERCOM_SPIS_CTRLA) Clock Phase Position */
#define SERCOM_SPIS_CTRLA_CPHA_Msk            (_U_(0x1) << SERCOM_SPIS_CTRLA_CPHA_Pos)             /**< (SERCOM_SPIS_CTRLA) Clock Phase Mask */
#define SERCOM_SPIS_CTRLA_CPHA(value)         (SERCOM_SPIS_CTRLA_CPHA_Msk & ((value) << SERCOM_SPIS_CTRLA_CPHA_Pos))
#define   SERCOM_SPIS_CTRLA_CPHA_LEADING_EDGE_Val _U_(0x0)                                             /**< (SERCOM_SPIS_CTRLA) The data is sampled on a leading SCK edge and changed on a trailing SCK edge  */
#define   SERCOM_SPIS_CTRLA_CPHA_TRAILING_EDGE_Val _U_(0x1)                                             /**< (SERCOM_SPIS_CTRLA) The data is sampled on a trailing SCK edge and changed on a leading SCK edge  */
#define SERCOM_SPIS_CTRLA_CPHA_LEADING_EDGE   (SERCOM_SPIS_CTRLA_CPHA_LEADING_EDGE_Val << SERCOM_SPIS_CTRLA_CPHA_Pos) /**< (SERCOM_SPIS_CTRLA) The data is sampled on a leading SCK edge and changed on a trailing SCK edge Position  */
#define SERCOM_SPIS_CTRLA_CPHA_TRAILING_EDGE  (SERCOM_SPIS_CTRLA_CPHA_TRAILING_EDGE_Val << SERCOM_SPIS_CTRLA_CPHA_Pos) /**< (SERCOM_SPIS_CTRLA) The data is sampled on a trailing SCK edge and changed on a leading SCK edge Position  */
#define SERCOM_SPIS_CTRLA_CPOL_Pos            _U_(29)                                              /**< (SERCOM_SPIS_CTRLA) Clock Polarity Position */
#define SERCOM_SPIS_CTRLA_CPOL_Msk            (_U_(0x1) << SERCOM_SPIS_CTRLA_CPOL_Pos)             /**< (SERCOM_SPIS_CTRLA) Clock Polarity Mask */
#define SERCOM_SPIS_CTRLA_CPOL(value)         (SERCOM_SPIS_CTRLA_CPOL_Msk & ((value) << SERCOM_SPIS_CTRLA_CPOL_Pos))
#define   SERCOM_SPIS_CTRLA_CPOL_IDLE_LOW_Val _U_(0x0)                                             /**< (SERCOM_SPIS_CTRLA) SCK is low when idle  */
#define   SERCOM_SPIS_CTRLA_CPOL_IDLE_HIGH_Val _U_(0x1)                                             /**< (SERCOM_SPIS_CTRLA) SCK is high when idle  */
#define SERCOM_SPIS_CTRLA_CPOL_IDLE_LOW       (SERCOM_SPIS_CTRLA_CPOL_IDLE_LOW_Val << SERCOM_SPIS_CTRLA_CPOL_Pos) /**< (SERCOM_SPIS_CTRLA) SCK is low when idle Position  */
#define SERCOM_SPIS_CTRLA_CPOL_IDLE_HIGH      (SERCOM_SPIS_CTRLA_CPOL_IDLE_HIGH_Val << SERCOM_SPIS_CTRLA_CPOL_Pos) /**< (SERCOM_SPIS_CTRLA) SCK is high when idle Position  */
#define SERCOM_SPIS_CTRLA_DORD_Pos            _U_(30)                                              /**< (SERCOM_SPIS_CTRLA) Data Order Position */
#define SERCOM_SPIS_CTRLA_DORD_Msk            (_U_(0x1) << SERCOM_SPIS_CTRLA_DORD_Pos)             /**< (SERCOM_SPIS_CTRLA) Data Order Mask */
#define SERCOM_SPIS_CTRLA_DORD(value)         (SERCOM_SPIS_CTRLA_DORD_Msk & ((value) << SERCOM_SPIS_CTRLA_DORD_Pos))
#define   SERCOM_SPIS_CTRLA_DORD_MSB_Val      _U_(0x0)                                             /**< (SERCOM_SPIS_CTRLA) MSB is transferred first  */
#define   SERCOM_SPIS_CTRLA_DORD_LSB_Val      _U_(0x1)                                             /**< (SERCOM_SPIS_CTRLA) LSB is transferred first  */
#define SERCOM_SPIS_CTRLA_DORD_MSB            (SERCOM_SPIS_CTRLA_DORD_MSB_Val << SERCOM_SPIS_CTRLA_DORD_Pos) /**< (SERCOM_SPIS_CTRLA) MSB is transferred first Position  */
#define SERCOM_SPIS_CTRLA_DORD_LSB            (SERCOM_SPIS_CTRLA_DORD_LSB_Val << SERCOM_SPIS_CTRLA_DORD_Pos) /**< (SERCOM_SPIS_CTRLA) LSB is transferred first Position  */
#define SERCOM_SPIS_CTRLA_Msk                 _U_(0x7F33019F)                                      /**< (SERCOM_SPIS_CTRLA) Register Mask  */


/* -------- SERCOM_USART_EXT_CTRLA : (SERCOM Offset: 0x00) (R/W 32) USART_EXT Control A -------- */
#define SERCOM_USART_EXT_CTRLA_RESETVALUE     _U_(0x00)                                            /**<  (SERCOM_USART_EXT_CTRLA) USART_EXT Control A  Reset Value */

#define SERCOM_USART_EXT_CTRLA_SWRST_Pos      _U_(0)                                               /**< (SERCOM_USART_EXT_CTRLA) Software Reset Position */
#define SERCOM_USART_EXT_CTRLA_SWRST_Msk      (_U_(0x1) << SERCOM_USART_EXT_CTRLA_SWRST_Pos)       /**< (SERCOM_USART_EXT_CTRLA) Software Reset Mask */
#define SERCOM_USART_EXT_CTRLA_SWRST(value)   (SERCOM_USART_EXT_CTRLA_SWRST_Msk & ((value) << SERCOM_USART_EXT_CTRLA_SWRST_Pos))
#define SERCOM_USART_EXT_CTRLA_ENABLE_Pos     _U_(1)                                               /**< (SERCOM_USART_EXT_CTRLA) Enable Position */
#define SERCOM_USART_EXT_CTRLA_ENABLE_Msk     (_U_(0x1) << SERCOM_USART_EXT_CTRLA_ENABLE_Pos)      /**< (SERCOM_USART_EXT_CTRLA) Enable Mask */
#define SERCOM_USART_EXT_CTRLA_ENABLE(value)  (SERCOM_USART_EXT_CTRLA_ENABLE_Msk & ((value) << SERCOM_USART_EXT_CTRLA_ENABLE_Pos))
#define SERCOM_USART_EXT_CTRLA_MODE_Pos       _U_(2)                                               /**< (SERCOM_USART_EXT_CTRLA) Operating Mode Position */
#define SERCOM_USART_EXT_CTRLA_MODE_Msk       (_U_(0x7) << SERCOM_USART_EXT_CTRLA_MODE_Pos)        /**< (SERCOM_USART_EXT_CTRLA) Operating Mode Mask */
#define SERCOM_USART_EXT_CTRLA_MODE(value)    (SERCOM_USART_EXT_CTRLA_MODE_Msk & ((value) << SERCOM_USART_EXT_CTRLA_MODE_Pos))
#define   SERCOM_USART_EXT_CTRLA_MODE_USART_EXT_CLK_Val _U_(0x0)                                             /**< (SERCOM_USART_EXT_CTRLA) USART with external clock  */
#define   SERCOM_USART_EXT_CTRLA_MODE_USART_INT_CLK_Val _U_(0x1)                                             /**< (SERCOM_USART_EXT_CTRLA) USART with internal clock  */
#define   SERCOM_USART_EXT_CTRLA_MODE_SPI_SLAVE_Val _U_(0x2)                                             /**< (SERCOM_USART_EXT_CTRLA) SPI in slave operation  */
#define   SERCOM_USART_EXT_CTRLA_MODE_SPI_MASTER_Val _U_(0x3)                                             /**< (SERCOM_USART_EXT_CTRLA) SPI in master operation  */
#define   SERCOM_USART_EXT_CTRLA_MODE_I2C_SLAVE_Val _U_(0x4)                                             /**< (SERCOM_USART_EXT_CTRLA) I2C slave operation  */
#define   SERCOM_USART_EXT_CTRLA_MODE_I2C_MASTER_Val _U_(0x5)                                             /**< (SERCOM_USART_EXT_CTRLA) I2C master operation  */
#define SERCOM_USART_EXT_CTRLA_MODE_USART_EXT_CLK (SERCOM_USART_EXT_CTRLA_MODE_USART_EXT_CLK_Val << SERCOM_USART_EXT_CTRLA_MODE_Pos) /**< (SERCOM_USART_EXT_CTRLA) USART with external clock Position  */
#define SERCOM_USART_EXT_CTRLA_MODE_USART_INT_CLK (SERCOM_USART_EXT_CTRLA_MODE_USART_INT_CLK_Val << SERCOM_USART_EXT_CTRLA_MODE_Pos) /**< (SERCOM_USART_EXT_CTRLA) USART with internal clock Position  */
#define SERCOM_USART_EXT_CTRLA_MODE_SPI_SLAVE (SERCOM_USART_EXT_CTRLA_MODE_SPI_SLAVE_Val << SERCOM_USART_EXT_CTRLA_MODE_Pos) /**< (SERCOM_USART_EXT_CTRLA) SPI in slave operation Position  */
#define SERCOM_USART_EXT_CTRLA_MODE_SPI_MASTER (SERCOM_USART_EXT_CTRLA_MODE_SPI_MASTER_Val << SERCOM_USART_EXT_CTRLA_MODE_Pos) /**< (SERCOM_USART_EXT_CTRLA) SPI in master operation Position  */
#define SERCOM_USART_EXT_CTRLA_MODE_I2C_SLAVE (SERCOM_USART_EXT_CTRLA_MODE_I2C_SLAVE_Val << SERCOM_USART_EXT_CTRLA_MODE_Pos) /**< (SERCOM_USART_EXT_CTRLA) I2C slave operation Position  */
#define SERCOM_USART_EXT_CTRLA_MODE_I2C_MASTER (SERCOM_USART_EXT_CTRLA_MODE_I2C_MASTER_Val << SERCOM_USART_EXT_CTRLA_MODE_Pos) /**< (SERCOM_USART_EXT_CTRLA) I2C master operation Position  */
#define SERCOM_USART_EXT_CTRLA_RUNSTDBY_Pos   _U_(7)                                               /**< (SERCOM_USART_EXT_CTRLA) Run In Standby Position */
#define SERCOM_USART_EXT_CTRLA_RUNSTDBY_Msk   (_U_(0x1) << SERCOM_USART_EXT_CTRLA_RUNSTDBY_Pos)    /**< (SERCOM_USART_EXT_CTRLA) Run In Standby Mask */
#define SERCOM_USART_EXT_CTRLA_RUNSTDBY(value) (SERCOM_USART_EXT_CTRLA_RUNSTDBY_Msk & ((value) << SERCOM_USART_EXT_CTRLA_RUNSTDBY_Pos))
#define SERCOM_USART_EXT_CTRLA_IBON_Pos       _U_(8)                                               /**< (SERCOM_USART_EXT_CTRLA) Immediate Buffer Overflow Notification Position */
#define SERCOM_USART_EXT_CTRLA_IBON_Msk       (_U_(0x1) << SERCOM_USART_EXT_CTRLA_IBON_Pos)        /**< (SERCOM_USART_EXT_CTRLA) Immediate Buffer Overflow Notification Mask */
#define SERCOM_USART_EXT_CTRLA_IBON(value)    (SERCOM_USART_EXT_CTRLA_IBON_Msk & ((value) << SERCOM_USART_EXT_CTRLA_IBON_Pos))
#define SERCOM_USART_EXT_CTRLA_TXPO_Pos       _U_(16)                                              /**< (SERCOM_USART_EXT_CTRLA) Transmit Data Pinout Position */
#define SERCOM_USART_EXT_CTRLA_TXPO_Msk       (_U_(0x1) << SERCOM_USART_EXT_CTRLA_TXPO_Pos)        /**< (SERCOM_USART_EXT_CTRLA) Transmit Data Pinout Mask */
#define SERCOM_USART_EXT_CTRLA_TXPO(value)    (SERCOM_USART_EXT_CTRLA_TXPO_Msk & ((value) << SERCOM_USART_EXT_CTRLA_TXPO_Pos))
#define   SERCOM_USART_EXT_CTRLA_TXPO_PAD0_Val _U_(0x0)                                             /**< (SERCOM_USART_EXT_CTRLA) PAD[0] = TxD; PAD[1] = XCK  */
#define   SERCOM_USART_EXT_CTRLA_TXPO_PAD1_Val _U_(0x1)                                             /**< (SERCOM_USART_EXT_CTRLA) PAD[2] = TxD; PAD[3] = XCK  */
#define SERCOM_USART_EXT_CTRLA_TXPO_PAD0      (SERCOM_USART_EXT_CTRLA_TXPO_PAD0_Val << SERCOM_USART_EXT_CTRLA_TXPO_Pos) /**< (SERCOM_USART_EXT_CTRLA) PAD[0] = TxD; PAD[1] = XCK Position  */
#define SERCOM_USART_EXT_CTRLA_TXPO_PAD1      (SERCOM_USART_EXT_CTRLA_TXPO_PAD1_Val << SERCOM_USART_EXT_CTRLA_TXPO_Pos) /**< (SERCOM_USART_EXT_CTRLA) PAD[2] = TxD; PAD[3] = XCK Position  */
#define SERCOM_USART_EXT_CTRLA_RXPO_Pos       _U_(20)                                              /**< (SERCOM_USART_EXT_CTRLA) Receive Data Pinout Position */
#define SERCOM_USART_EXT_CTRLA_RXPO_Msk       (_U_(0x3) << SERCOM_USART_EXT_CTRLA_RXPO_Pos)        /**< (SERCOM_USART_EXT_CTRLA) Receive Data Pinout Mask */
#define SERCOM_USART_EXT_CTRLA_RXPO(value)    (SERCOM_USART_EXT_CTRLA_RXPO_Msk & ((value) << SERCOM_USART_EXT_CTRLA_RXPO_Pos))
#define   SERCOM_USART_EXT_CTRLA_RXPO_PAD0_Val _U_(0x0)                                             /**< (SERCOM_USART_EXT_CTRLA) SERCOM PAD[0] is used for data reception  */
#define   SERCOM_USART_EXT_CTRLA_RXPO_PAD1_Val _U_(0x1)                                             /**< (SERCOM_USART_EXT_CTRLA) SERCOM PAD[1] is used for data reception  */
#define   SERCOM_USART_EXT_CTRLA_RXPO_PAD2_Val _U_(0x2)                                             /**< (SERCOM_USART_EXT_CTRLA) SERCOM PAD[2] is used for data reception  */
#define   SERCOM_USART_EXT_CTRLA_RXPO_PAD3_Val _U_(0x3)                                             /**< (SERCOM_USART_EXT_CTRLA) SERCOM PAD[3] is used for data reception  */
#define SERCOM_USART_EXT_CTRLA_RXPO_PAD0      (SERCOM_USART_EXT_CTRLA_RXPO_PAD0_Val << SERCOM_USART_EXT_CTRLA_RXPO_Pos) /**< (SERCOM_USART_EXT_CTRLA) SERCOM PAD[0] is used for data reception Position  */
#define SERCOM_USART_EXT_CTRLA_RXPO_PAD1      (SERCOM_USART_EXT_CTRLA_RXPO_PAD1_Val << SERCOM_USART_EXT_CTRLA_RXPO_Pos) /**< (SERCOM_USART_EXT_CTRLA) SERCOM PAD[1] is used for data reception Position  */
#define SERCOM_USART_EXT_CTRLA_RXPO_PAD2      (SERCOM_USART_EXT_CTRLA_RXPO_PAD2_Val << SERCOM_USART_EXT_CTRLA_RXPO_Pos) /**< (SERCOM_USART_EXT_CTRLA) SERCOM PAD[2] is used for data reception Position  */
#define SERCOM_USART_EXT_CTRLA_RXPO_PAD3      (SERCOM_USART_EXT_CTRLA_RXPO_PAD3_Val << SERCOM_USART_EXT_CTRLA_RXPO_Pos) /**< (SERCOM_USART_EXT_CTRLA) SERCOM PAD[3] is used for data reception Position  */
#define SERCOM_USART_EXT_CTRLA_FORM_Pos       _U_(24)                                              /**< (SERCOM_USART_EXT_CTRLA) Frame Format Position */
#define SERCOM_USART_EXT_CTRLA_FORM_Msk       (_U_(0xF) << SERCOM_USART_EXT_CTRLA_FORM_Pos)        /**< (SERCOM_USART_EXT_CTRLA) Frame Format Mask */
#define SERCOM_USART_EXT_CTRLA_FORM(value)    (SERCOM_USART_EXT_CTRLA_FORM_Msk & ((value) << SERCOM_USART_EXT_CTRLA_FORM_Pos))
#define   SERCOM_USART_EXT_CTRLA_FORM_USART_FRAME_NO_PARITY_Val _U_(0x0)                                             /**< (SERCOM_USART_EXT_CTRLA) USART frame  */
#define   SERCOM_USART_EXT_CTRLA_FORM_USART_FRAME_WITH_PARITY_Val _U_(0x1)                                             /**< (SERCOM_USART_EXT_CTRLA) USART frame with parity  */
#define SERCOM_USART_EXT_CTRLA_FORM_USART_FRAME_NO_PARITY (SERCOM_USART_EXT_CTRLA_FORM_USART_FRAME_NO_PARITY_Val << SERCOM_USART_EXT_CTRLA_FORM_Pos) /**< (SERCOM_USART_EXT_CTRLA) USART frame Position  */
#define SERCOM_USART_EXT_CTRLA_FORM_USART_FRAME_WITH_PARITY (SERCOM_USART_EXT_CTRLA_FORM_USART_FRAME_WITH_PARITY_Val << SERCOM_USART_EXT_CTRLA_FORM_Pos) /**< (SERCOM_USART_EXT_CTRLA) USART frame with parity Position  */
#define SERCOM_USART_EXT_CTRLA_CMODE_Pos      _U_(28)                                              /**< (SERCOM_USART_EXT_CTRLA) Communication Mode Position */
#define SERCOM_USART_EXT_CTRLA_CMODE_Msk      (_U_(0x1) << SERCOM_USART_EXT_CTRLA_CMODE_Pos)       /**< (SERCOM_USART_EXT_CTRLA) Communication Mode Mask */
#define SERCOM_USART_EXT_CTRLA_CMODE(value)   (SERCOM_USART_EXT_CTRLA_CMODE_Msk & ((value) << SERCOM_USART_EXT_CTRLA_CMODE_Pos))
#define   SERCOM_USART_EXT_CTRLA_CMODE_ASYNC_Val _U_(0x0)                                             /**< (SERCOM_USART_EXT_CTRLA) Asynchronous Communication  */
#define   SERCOM_USART_EXT_CTRLA_CMODE_SYNC_Val _U_(0x1)                                             /**< (SERCOM_USART_EXT_CTRLA) Synchronous Communication  */
#define SERCOM_USART_EXT_CTRLA_CMODE_ASYNC    (SERCOM_USART_EXT_CTRLA_CMODE_ASYNC_Val << SERCOM_USART_EXT_CTRLA_CMODE_Pos) /**< (SERCOM_USART_EXT_CTRLA) Asynchronous Communication Position  */
#define SERCOM_USART_EXT_CTRLA_CMODE_SYNC     (SERCOM_USART_EXT_CTRLA_CMODE_SYNC_Val << SERCOM_USART_EXT_CTRLA_CMODE_Pos) /**< (SERCOM_USART_EXT_CTRLA) Synchronous Communication Position  */
#define SERCOM_USART_EXT_CTRLA_CPOL_Pos       _U_(29)                                              /**< (SERCOM_USART_EXT_CTRLA) Clock Polarity Position */
#define SERCOM_USART_EXT_CTRLA_CPOL_Msk       (_U_(0x1) << SERCOM_USART_EXT_CTRLA_CPOL_Pos)        /**< (SERCOM_USART_EXT_CTRLA) Clock Polarity Mask */
#define SERCOM_USART_EXT_CTRLA_CPOL(value)    (SERCOM_USART_EXT_CTRLA_CPOL_Msk & ((value) << SERCOM_USART_EXT_CTRLA_CPOL_Pos))
#define   SERCOM_USART_EXT_CTRLA_CPOL_IDLE_LOW_Val _U_(0x0)                                             /**< (SERCOM_USART_EXT_CTRLA) TxD Change:- Rising XCK edge, RxD Sample:- Falling XCK edge  */
#define   SERCOM_USART_EXT_CTRLA_CPOL_IDLE_HIGH_Val _U_(0x1)                                             /**< (SERCOM_USART_EXT_CTRLA) TxD Change:- Falling XCK edge, RxD Sample:- Rising XCK edge  */
#define SERCOM_USART_EXT_CTRLA_CPOL_IDLE_LOW  (SERCOM_USART_EXT_CTRLA_CPOL_IDLE_LOW_Val << SERCOM_USART_EXT_CTRLA_CPOL_Pos) /**< (SERCOM_USART_EXT_CTRLA) TxD Change:- Rising XCK edge, RxD Sample:- Falling XCK edge Position  */
#define SERCOM_USART_EXT_CTRLA_CPOL_IDLE_HIGH (SERCOM_USART_EXT_CTRLA_CPOL_IDLE_HIGH_Val << SERCOM_USART_EXT_CTRLA_CPOL_Pos) /**< (SERCOM_USART_EXT_CTRLA) TxD Change:- Falling XCK edge, RxD Sample:- Rising XCK edge Position  */
#define SERCOM_USART_EXT_CTRLA_DORD_Pos       _U_(30)                                              /**< (SERCOM_USART_EXT_CTRLA) Data Order Position */
#define SERCOM_USART_EXT_CTRLA_DORD_Msk       (_U_(0x1) << SERCOM_USART_EXT_CTRLA_DORD_Pos)        /**< (SERCOM_USART_EXT_CTRLA) Data Order Mask */
#define SERCOM_USART_EXT_CTRLA_DORD(value)    (SERCOM_USART_EXT_CTRLA_DORD_Msk & ((value) << SERCOM_USART_EXT_CTRLA_DORD_Pos))
#define   SERCOM_USART_EXT_CTRLA_DORD_MSB_Val _U_(0x0)                                             /**< (SERCOM_USART_EXT_CTRLA) MSB is transmitted first  */
#define   SERCOM_USART_EXT_CTRLA_DORD_LSB_Val _U_(0x1)                                             /**< (SERCOM_USART_EXT_CTRLA) LSB is transmitted first  */
#define SERCOM_USART_EXT_CTRLA_DORD_MSB       (SERCOM_USART_EXT_CTRLA_DORD_MSB_Val << SERCOM_USART_EXT_CTRLA_DORD_Pos) /**< (SERCOM_USART_EXT_CTRLA) MSB is transmitted first Position  */
#define SERCOM_USART_EXT_CTRLA_DORD_LSB       (SERCOM_USART_EXT_CTRLA_DORD_LSB_Val << SERCOM_USART_EXT_CTRLA_DORD_Pos) /**< (SERCOM_USART_EXT_CTRLA) LSB is transmitted first Position  */
#define SERCOM_USART_EXT_CTRLA_Msk            _U_(0x7F31019F)                                      /**< (SERCOM_USART_EXT_CTRLA) Register Mask  */


/* -------- SERCOM_USART_INT_CTRLA : (SERCOM Offset: 0x00) (R/W 32) USART_INT Control A -------- */
#define SERCOM_USART_INT_CTRLA_RESETVALUE     _U_(0x00)                                            /**<  (SERCOM_USART_INT_CTRLA) USART_INT Control A  Reset Value */

#define SERCOM_USART_INT_CTRLA_SWRST_Pos      _U_(0)                                               /**< (SERCOM_USART_INT_CTRLA) Software Reset Position */
#define SERCOM_USART_INT_CTRLA_SWRST_Msk      (_U_(0x1) << SERCOM_USART_INT_CTRLA_SWRST_Pos)       /**< (SERCOM_USART_INT_CTRLA) Software Reset Mask */
#define SERCOM_USART_INT_CTRLA_SWRST(value)   (SERCOM_USART_INT_CTRLA_SWRST_Msk & ((value) << SERCOM_USART_INT_CTRLA_SWRST_Pos))
#define SERCOM_USART_INT_CTRLA_ENABLE_Pos     _U_(1)                                               /**< (SERCOM_USART_INT_CTRLA) Enable Position */
#define SERCOM_USART_INT_CTRLA_ENABLE_Msk     (_U_(0x1) << SERCOM_USART_INT_CTRLA_ENABLE_Pos)      /**< (SERCOM_USART_INT_CTRLA) Enable Mask */
#define SERCOM_USART_INT_CTRLA_ENABLE(value)  (SERCOM_USART_INT_CTRLA_ENABLE_Msk & ((value) << SERCOM_USART_INT_CTRLA_ENABLE_Pos))
#define SERCOM_USART_INT_CTRLA_MODE_Pos       _U_(2)                                               /**< (SERCOM_USART_INT_CTRLA) Operating Mode Position */
#define SERCOM_USART_INT_CTRLA_MODE_Msk       (_U_(0x7) << SERCOM_USART_INT_CTRLA_MODE_Pos)        /**< (SERCOM_USART_INT_CTRLA) Operating Mode Mask */
#define SERCOM_USART_INT_CTRLA_MODE(value)    (SERCOM_USART_INT_CTRLA_MODE_Msk & ((value) << SERCOM_USART_INT_CTRLA_MODE_Pos))
#define   SERCOM_USART_INT_CTRLA_MODE_USART_EXT_CLK_Val _U_(0x0)                                             /**< (SERCOM_USART_INT_CTRLA) USART with external clock  */
#define   SERCOM_USART_INT_CTRLA_MODE_USART_INT_CLK_Val _U_(0x1)                                             /**< (SERCOM_USART_INT_CTRLA) USART with internal clock  */
#define   SERCOM_USART_INT_CTRLA_MODE_SPI_SLAVE_Val _U_(0x2)                                             /**< (SERCOM_USART_INT_CTRLA) SPI in slave operation  */
#define   SERCOM_USART_INT_CTRLA_MODE_SPI_MASTER_Val _U_(0x3)                                             /**< (SERCOM_USART_INT_CTRLA) SPI in master operation  */
#define   SERCOM_USART_INT_CTRLA_MODE_I2C_SLAVE_Val _U_(0x4)                                             /**< (SERCOM_USART_INT_CTRLA) I2C slave operation  */
#define   SERCOM_USART_INT_CTRLA_MODE_I2C_MASTER_Val _U_(0x5)                                             /**< (SERCOM_USART_INT_CTRLA) I2C master operation  */
#define SERCOM_USART_INT_CTRLA_MODE_USART_EXT_CLK (SERCOM_USART_INT_CTRLA_MODE_USART_EXT_CLK_Val << SERCOM_USART_INT_CTRLA_MODE_Pos) /**< (SERCOM_USART_INT_CTRLA) USART with external clock Position  */
#define SERCOM_USART_INT_CTRLA_MODE_USART_INT_CLK (SERCOM_USART_INT_CTRLA_MODE_USART_INT_CLK_Val << SERCOM_USART_INT_CTRLA_MODE_Pos) /**< (SERCOM_USART_INT_CTRLA) USART with internal clock Position  */
#define SERCOM_USART_INT_CTRLA_MODE_SPI_SLAVE (SERCOM_USART_INT_CTRLA_MODE_SPI_SLAVE_Val << SERCOM_USART_INT_CTRLA_MODE_Pos) /**< (SERCOM_USART_INT_CTRLA) SPI in slave operation Position  */
#define SERCOM_USART_INT_CTRLA_MODE_SPI_MASTER (SERCOM_USART_INT_CTRLA_MODE_SPI_MASTER_Val << SERCOM_USART_INT_CTRLA_MODE_Pos) /**< (SERCOM_USART_INT_CTRLA) SPI in master operation Position  */
#define SERCOM_USART_INT_CTRLA_MODE_I2C_SLAVE (SERCOM_USART_INT_CTRLA_MODE_I2C_SLAVE_Val << SERCOM_USART_INT_CTRLA_MODE_Pos) /**< (SERCOM_USART_INT_CTRLA) I2C slave operation Position  */
#define SERCOM_USART_INT_CTRLA_MODE_I2C_MASTER (SERCOM_USART_INT_CTRLA_MODE_I2C_MASTER_Val << SERCOM_USART_INT_CTRLA_MODE_Pos) /**< (SERCOM_USART_INT_CTRLA) I2C master operation Position  */
#define SERCOM_USART_INT_CTRLA_RUNSTDBY_Pos   _U_(7)                                               /**< (SERCOM_USART_INT_CTRLA) Run In Standby Position */
#define SERCOM_USART_INT_CTRLA_RUNSTDBY_Msk   (_U_(0x1) << SERCOM_USART_INT_CTRLA_RUNSTDBY_Pos)    /**< (SERCOM_USART_INT_CTRLA) Run In Standby Mask */
#define SERCOM_USART_INT_CTRLA_RUNSTDBY(value) (SERCOM_USART_INT_CTRLA_RUNSTDBY_Msk & ((value) << SERCOM_USART_INT_CTRLA_RUNSTDBY_Pos))
#define SERCOM_USART_INT_CTRLA_IBON_Pos       _U_(8)                                               /**< (SERCOM_USART_INT_CTRLA) Immediate Buffer Overflow Notification Position */
#define SERCOM_USART_INT_CTRLA_IBON_Msk       (_U_(0x1) << SERCOM_USART_INT_CTRLA_IBON_Pos)        /**< (SERCOM_USART_INT_CTRLA) Immediate Buffer Overflow Notification Mask */
#define SERCOM_USART_INT_CTRLA_IBON(value)    (SERCOM_USART_INT_CTRLA_IBON_Msk & ((value) << SERCOM_USART_INT_CTRLA_IBON_Pos))
#define SERCOM_USART_INT_CTRLA_TXPO_Pos       _U_(16)                                              /**< (SERCOM_USART_INT_CTRLA) Transmit Data Pinout Position */
#define SERCOM_USART_INT_CTRLA_TXPO_Msk       (_U_(0x1) << SERCOM_USART_INT_CTRLA_TXPO_Pos)        /**< (SERCOM_USART_INT_CTRLA) Transmit Data Pinout Mask */
#define SERCOM_USART_INT_CTRLA_TXPO(value)    (SERCOM_USART_INT_CTRLA_TXPO_Msk & ((value) << SERCOM_USART_INT_CTRLA_TXPO_Pos))
#define   SERCOM_USART_INT_CTRLA_TXPO_PAD0_Val _U_(0x0)                                             /**< (SERCOM_USART_INT_CTRLA) PAD[0] = TxD; PAD[1] = XCK  */
#define   SERCOM_USART_INT_CTRLA_TXPO_PAD1_Val _U_(0x1)                                             /**< (SERCOM_USART_INT_CTRLA) PAD[2] = TxD; PAD[3] = XCK  */
#define SERCOM_USART_INT_CTRLA_TXPO_PAD0      (SERCOM_USART_INT_CTRLA_TXPO_PAD0_Val << SERCOM_USART_INT_CTRLA_TXPO_Pos) /**< (SERCOM_USART_INT_CTRLA) PAD[0] = TxD; PAD[1] = XCK Position  */
#define SERCOM_USART_INT_CTRLA_TXPO_PAD1      (SERCOM_USART_INT_CTRLA_TXPO_PAD1_Val << SERCOM_USART_INT_CTRLA_TXPO_Pos) /**< (SERCOM_USART_INT_CTRLA) PAD[2] = TxD; PAD[3] = XCK Position  */
#define SERCOM_USART_INT_CTRLA_RXPO_Pos       _U_(20)                                              /**< (SERCOM_USART_INT_CTRLA) Receive Data Pinout Position */
#define SERCOM_USART_INT_CTRLA_RXPO_Msk       (_U_(0x3) << SERCOM_USART_INT_CTRLA_RXPO_Pos)        /**< (SERCOM_USART_INT_CTRLA) Receive Data Pinout Mask */
#define SERCOM_USART_INT_CTRLA_RXPO(value)    (SERCOM_USART_INT_CTRLA_RXPO_Msk & ((value) << SERCOM_USART_INT_CTRLA_RXPO_Pos))
#define   SERCOM_USART_INT_CTRLA_RXPO_PAD0_Val _U_(0x0)                                             /**< (SERCOM_USART_INT_CTRLA) SERCOM PAD[0] is used for data reception  */
#define   SERCOM_USART_INT_CTRLA_RXPO_PAD1_Val _U_(0x1)                                             /**< (SERCOM_USART_INT_CTRLA) SERCOM PAD[1] is used for data reception  */
#define   SERCOM_USART_INT_CTRLA_RXPO_PAD2_Val _U_(0x2)                                             /**< (SERCOM_USART_INT_CTRLA) SERCOM PAD[2] is used for data reception  */
#define   SERCOM_USART_INT_CTRLA_RXPO_PAD3_Val _U_(0x3)                                             /**< (SERCOM_USART_INT_CTRLA) SERCOM PAD[3] is used for data reception  */
#define SERCOM_USART_INT_CTRLA_RXPO_PAD0      (SERCOM_USART_INT_CTRLA_RXPO_PAD0_Val << SERCOM_USART_INT_CTRLA_RXPO_Pos) /**< (SERCOM_USART_INT_CTRLA) SERCOM PAD[0] is used for data reception Position  */
#define SERCOM_USART_INT_CTRLA_RXPO_PAD1      (SERCOM_USART_INT_CTRLA_RXPO_PAD1_Val << SERCOM_USART_INT_CTRLA_RXPO_Pos) /**< (SERCOM_USART_INT_CTRLA) SERCOM PAD[1] is used for data reception Position  */
#define SERCOM_USART_INT_CTRLA_RXPO_PAD2      (SERCOM_USART_INT_CTRLA_RXPO_PAD2_Val << SERCOM_USART_INT_CTRLA_RXPO_Pos) /**< (SERCOM_USART_INT_CTRLA) SERCOM PAD[2] is used for data reception Position  */
#define SERCOM_USART_INT_CTRLA_RXPO_PAD3      (SERCOM_USART_INT_CTRLA_RXPO_PAD3_Val << SERCOM_USART_INT_CTRLA_RXPO_Pos) /**< (SERCOM_USART_INT_CTRLA) SERCOM PAD[3] is used for data reception Position  */
#define SERCOM_USART_INT_CTRLA_FORM_Pos       _U_(24)                                              /**< (SERCOM_USART_INT_CTRLA) Frame Format Position */
#define SERCOM_USART_INT_CTRLA_FORM_Msk       (_U_(0xF) << SERCOM_USART_INT_CTRLA_FORM_Pos)        /**< (SERCOM_USART_INT_CTRLA) Frame Format Mask */
#define SERCOM_USART_INT_CTRLA_FORM(value)    (SERCOM_USART_INT_CTRLA_FORM_Msk & ((value) << SERCOM_USART_INT_CTRLA_FORM_Pos))
#define   SERCOM_USART_INT_CTRLA_FORM_USART_FRAME_NO_PARITY_Val _U_(0x0)                                             /**< (SERCOM_USART_INT_CTRLA) USART frame  */
#define   SERCOM_USART_INT_CTRLA_FORM_USART_FRAME_WITH_PARITY_Val _U_(0x1)                                             /**< (SERCOM_USART_INT_CTRLA) USART frame with parity  */
#define SERCOM_USART_INT_CTRLA_FORM_USART_FRAME_NO_PARITY (SERCOM_USART_INT_CTRLA_FORM_USART_FRAME_NO_PARITY_Val << SERCOM_USART_INT_CTRLA_FORM_Pos) /**< (SERCOM_USART_INT_CTRLA) USART frame Position  */
#define SERCOM_USART_INT_CTRLA_FORM_USART_FRAME_WITH_PARITY (SERCOM_USART_INT_CTRLA_FORM_USART_FRAME_WITH_PARITY_Val << SERCOM_USART_INT_CTRLA_FORM_Pos) /**< (SERCOM_USART_INT_CTRLA) USART frame with parity Position  */
#define SERCOM_USART_INT_CTRLA_CMODE_Pos      _U_(28)                                              /**< (SERCOM_USART_INT_CTRLA) Communication Mode Position */
#define SERCOM_USART_INT_CTRLA_CMODE_Msk      (_U_(0x1) << SERCOM_USART_INT_CTRLA_CMODE_Pos)       /**< (SERCOM_USART_INT_CTRLA) Communication Mode Mask */
#define SERCOM_USART_INT_CTRLA_CMODE(value)   (SERCOM_USART_INT_CTRLA_CMODE_Msk & ((value) << SERCOM_USART_INT_CTRLA_CMODE_Pos))
#define   SERCOM_USART_INT_CTRLA_CMODE_ASYNC_Val _U_(0x0)                                             /**< (SERCOM_USART_INT_CTRLA) Asynchronous Communication  */
#define   SERCOM_USART_INT_CTRLA_CMODE_SYNC_Val _U_(0x1)                                             /**< (SERCOM_USART_INT_CTRLA) Synchronous Communication  */
#define SERCOM_USART_INT_CTRLA_CMODE_ASYNC    (SERCOM_USART_INT_CTRLA_CMODE_ASYNC_Val << SERCOM_USART_INT_CTRLA_CMODE_Pos) /**< (SERCOM_USART_INT_CTRLA) Asynchronous Communication Position  */
#define SERCOM_USART_INT_CTRLA_CMODE_SYNC     (SERCOM_USART_INT_CTRLA_CMODE_SYNC_Val << SERCOM_USART_INT_CTRLA_CMODE_Pos) /**< (SERCOM_USART_INT_CTRLA) Synchronous Communication Position  */
#define SERCOM_USART_INT_CTRLA_CPOL_Pos       _U_(29)                                              /**< (SERCOM_USART_INT_CTRLA) Clock Polarity Position */
#define SERCOM_USART_INT_CTRLA_CPOL_Msk       (_U_(0x1) << SERCOM_USART_INT_CTRLA_CPOL_Pos)        /**< (SERCOM_USART_INT_CTRLA) Clock Polarity Mask */
#define SERCOM_USART_INT_CTRLA_CPOL(value)    (SERCOM_USART_INT_CTRLA_CPOL_Msk & ((value) << SERCOM_USART_INT_CTRLA_CPOL_Pos))
#define   SERCOM_USART_INT_CTRLA_CPOL_IDLE_LOW_Val _U_(0x0)                                             /**< (SERCOM_USART_INT_CTRLA) TxD Change:- Rising XCK edge, RxD Sample:- Falling XCK edge  */
#define   SERCOM_USART_INT_CTRLA_CPOL_IDLE_HIGH_Val _U_(0x1)                                             /**< (SERCOM_USART_INT_CTRLA) TxD Change:- Falling XCK edge, RxD Sample:- Rising XCK edge  */
#define SERCOM_USART_INT_CTRLA_CPOL_IDLE_LOW  (SERCOM_USART_INT_CTRLA_CPOL_IDLE_LOW_Val << SERCOM_USART_INT_CTRLA_CPOL_Pos) /**< (SERCOM_USART_INT_CTRLA) TxD Change:- Rising XCK edge, RxD Sample:- Falling XCK edge Position  */
#define SERCOM_USART_INT_CTRLA_CPOL_IDLE_HIGH (SERCOM_USART_INT_CTRLA_CPOL_IDLE_HIGH_Val << SERCOM_USART_INT_CTRLA_CPOL_Pos) /**< (SERCOM_USART_INT_CTRLA) TxD Change:- Falling XCK edge, RxD Sample:- Rising XCK edge Position  */
#define SERCOM_USART_INT_CTRLA_DORD_Pos       _U_(30)                                              /**< (SERCOM_USART_INT_CTRLA) Data Order Position */
#define SERCOM_USART_INT_CTRLA_DORD_Msk       (_U_(0x1) << SERCOM_USART_INT_CTRLA_DORD_Pos)        /**< (SERCOM_USART_INT_CTRLA) Data Order Mask */
#define SERCOM_USART_INT_CTRLA_DORD(value)    (SERCOM_USART_INT_CTRLA_DORD_Msk & ((value) << SERCOM_USART_INT_CTRLA_DORD_Pos))
#define   SERCOM_USART_INT_CTRLA_DORD_MSB_Val _U_(0x0)                                             /**< (SERCOM_USART_INT_CTRLA) MSB is transmitted first  */
#define   SERCOM_USART_INT_CTRLA_DORD_LSB_Val _U_(0x1)                                             /**< (SERCOM_USART_INT_CTRLA) LSB is transmitted first  */
#define SERCOM_USART_INT_CTRLA_DORD_MSB       (SERCOM_USART_INT_CTRLA_DORD_MSB_Val << SERCOM_USART_INT_CTRLA_DORD_Pos) /**< (SERCOM_USART_INT_CTRLA) MSB is transmitted first Position  */
#define SERCOM_USART_INT_CTRLA_DORD_LSB       (SERCOM_USART_INT_CTRLA_DORD_LSB_Val << SERCOM_USART_INT_CTRLA_DORD_Pos) /**< (SERCOM_USART_INT_CTRLA) LSB is transmitted first Position  */
#define SERCOM_USART_INT_CTRLA_Msk            _U_(0x7F31019F)                                      /**< (SERCOM_USART_INT_CTRLA) Register Mask  */


/* -------- SERCOM_I2CM_CTRLB : (SERCOM Offset: 0x04) (R/W 32) I2CM Control B -------- */
#define SERCOM_I2CM_CTRLB_RESETVALUE          _U_(0x00)                                            /**<  (SERCOM_I2CM_CTRLB) I2CM Control B  Reset Value */

#define SERCOM_I2CM_CTRLB_SMEN_Pos            _U_(8)                                               /**< (SERCOM_I2CM_CTRLB) Smart Mode Enable Position */
#define SERCOM_I2CM_CTRLB_SMEN_Msk            (_U_(0x1) << SERCOM_I2CM_CTRLB_SMEN_Pos)             /**< (SERCOM_I2CM_CTRLB) Smart Mode Enable Mask */
#define SERCOM_I2CM_CTRLB_SMEN(value)         (SERCOM_I2CM_CTRLB_SMEN_Msk & ((value) << SERCOM_I2CM_CTRLB_SMEN_Pos))
#define SERCOM_I2CM_CTRLB_QCEN_Pos            _U_(9)                                               /**< (SERCOM_I2CM_CTRLB) Quick Command Enable Position */
#define SERCOM_I2CM_CTRLB_QCEN_Msk            (_U_(0x1) << SERCOM_I2CM_CTRLB_QCEN_Pos)             /**< (SERCOM_I2CM_CTRLB) Quick Command Enable Mask */
#define SERCOM_I2CM_CTRLB_QCEN(value)         (SERCOM_I2CM_CTRLB_QCEN_Msk & ((value) << SERCOM_I2CM_CTRLB_QCEN_Pos))
#define SERCOM_I2CM_CTRLB_CMD_Pos             _U_(16)                                              /**< (SERCOM_I2CM_CTRLB) Command Position */
#define SERCOM_I2CM_CTRLB_CMD_Msk             (_U_(0x3) << SERCOM_I2CM_CTRLB_CMD_Pos)              /**< (SERCOM_I2CM_CTRLB) Command Mask */
#define SERCOM_I2CM_CTRLB_CMD(value)          (SERCOM_I2CM_CTRLB_CMD_Msk & ((value) << SERCOM_I2CM_CTRLB_CMD_Pos))
#define SERCOM_I2CM_CTRLB_ACKACT_Pos          _U_(18)                                              /**< (SERCOM_I2CM_CTRLB) Acknowledge Action Position */
#define SERCOM_I2CM_CTRLB_ACKACT_Msk          (_U_(0x1) << SERCOM_I2CM_CTRLB_ACKACT_Pos)           /**< (SERCOM_I2CM_CTRLB) Acknowledge Action Mask */
#define SERCOM_I2CM_CTRLB_ACKACT(value)       (SERCOM_I2CM_CTRLB_ACKACT_Msk & ((value) << SERCOM_I2CM_CTRLB_ACKACT_Pos))
#define SERCOM_I2CM_CTRLB_Msk                 _U_(0x00070300)                                      /**< (SERCOM_I2CM_CTRLB) Register Mask  */


/* -------- SERCOM_I2CS_CTRLB : (SERCOM Offset: 0x04) (R/W 32) I2CS Control B -------- */
#define SERCOM_I2CS_CTRLB_RESETVALUE          _U_(0x00)                                            /**<  (SERCOM_I2CS_CTRLB) I2CS Control B  Reset Value */

#define SERCOM_I2CS_CTRLB_SMEN_Pos            _U_(8)                                               /**< (SERCOM_I2CS_CTRLB) Smart Mode Enable Position */
#define SERCOM_I2CS_CTRLB_SMEN_Msk            (_U_(0x1) << SERCOM_I2CS_CTRLB_SMEN_Pos)             /**< (SERCOM_I2CS_CTRLB) Smart Mode Enable Mask */
#define SERCOM_I2CS_CTRLB_SMEN(value)         (SERCOM_I2CS_CTRLB_SMEN_Msk & ((value) << SERCOM_I2CS_CTRLB_SMEN_Pos))
#define SERCOM_I2CS_CTRLB_AMODE_Pos           _U_(14)                                              /**< (SERCOM_I2CS_CTRLB) Address Mode Position */
#define SERCOM_I2CS_CTRLB_AMODE_Msk           (_U_(0x3) << SERCOM_I2CS_CTRLB_AMODE_Pos)            /**< (SERCOM_I2CS_CTRLB) Address Mode Mask */
#define SERCOM_I2CS_CTRLB_AMODE(value)        (SERCOM_I2CS_CTRLB_AMODE_Msk & ((value) << SERCOM_I2CS_CTRLB_AMODE_Pos))
#define SERCOM_I2CS_CTRLB_CMD_Pos             _U_(16)                                              /**< (SERCOM_I2CS_CTRLB) Command Position */
#define SERCOM_I2CS_CTRLB_CMD_Msk             (_U_(0x3) << SERCOM_I2CS_CTRLB_CMD_Pos)              /**< (SERCOM_I2CS_CTRLB) Command Mask */
#define SERCOM_I2CS_CTRLB_CMD(value)          (SERCOM_I2CS_CTRLB_CMD_Msk & ((value) << SERCOM_I2CS_CTRLB_CMD_Pos))
#define SERCOM_I2CS_CTRLB_ACKACT_Pos          _U_(18)                                              /**< (SERCOM_I2CS_CTRLB) Acknowledge Action Position */
#define SERCOM_I2CS_CTRLB_ACKACT_Msk          (_U_(0x1) << SERCOM_I2CS_CTRLB_ACKACT_Pos)           /**< (SERCOM_I2CS_CTRLB) Acknowledge Action Mask */
#define SERCOM_I2CS_CTRLB_ACKACT(value)       (SERCOM_I2CS_CTRLB_ACKACT_Msk & ((value) << SERCOM_I2CS_CTRLB_ACKACT_Pos))
#define SERCOM_I2CS_CTRLB_Msk                 _U_(0x0007C100)                                      /**< (SERCOM_I2CS_CTRLB) Register Mask  */


/* -------- SERCOM_SPIM_CTRLB : (SERCOM Offset: 0x04) (R/W 32) SPIM Control B -------- */
#define SERCOM_SPIM_CTRLB_RESETVALUE          _U_(0x00)                                            /**<  (SERCOM_SPIM_CTRLB) SPIM Control B  Reset Value */

#define SERCOM_SPIM_CTRLB_CHSIZE_Pos          _U_(0)                                               /**< (SERCOM_SPIM_CTRLB) Character Size Position */
#define SERCOM_SPIM_CTRLB_CHSIZE_Msk          (_U_(0x7) << SERCOM_SPIM_CTRLB_CHSIZE_Pos)           /**< (SERCOM_SPIM_CTRLB) Character Size Mask */
#define SERCOM_SPIM_CTRLB_CHSIZE(value)       (SERCOM_SPIM_CTRLB_CHSIZE_Msk & ((value) << SERCOM_SPIM_CTRLB_CHSIZE_Pos))
#define   SERCOM_SPIM_CTRLB_CHSIZE_8_BIT_Val  _U_(0x0)                                             /**< (SERCOM_SPIM_CTRLB) 8 bits  */
#define   SERCOM_SPIM_CTRLB_CHSIZE_9_BIT_Val  _U_(0x1)                                             /**< (SERCOM_SPIM_CTRLB) 9 bits  */
#define SERCOM_SPIM_CTRLB_CHSIZE_8_BIT        (SERCOM_SPIM_CTRLB_CHSIZE_8_BIT_Val << SERCOM_SPIM_CTRLB_CHSIZE_Pos) /**< (SERCOM_SPIM_CTRLB) 8 bits Position  */
#define SERCOM_SPIM_CTRLB_CHSIZE_9_BIT        (SERCOM_SPIM_CTRLB_CHSIZE_9_BIT_Val << SERCOM_SPIM_CTRLB_CHSIZE_Pos) /**< (SERCOM_SPIM_CTRLB) 9 bits Position  */
#define SERCOM_SPIM_CTRLB_PLOADEN_Pos         _U_(6)                                               /**< (SERCOM_SPIM_CTRLB) Slave Data Preload Enable Position */
#define SERCOM_SPIM_CTRLB_PLOADEN_Msk         (_U_(0x1) << SERCOM_SPIM_CTRLB_PLOADEN_Pos)          /**< (SERCOM_SPIM_CTRLB) Slave Data Preload Enable Mask */
#define SERCOM_SPIM_CTRLB_PLOADEN(value)      (SERCOM_SPIM_CTRLB_PLOADEN_Msk & ((value) << SERCOM_SPIM_CTRLB_PLOADEN_Pos))
#define SERCOM_SPIM_CTRLB_AMODE_Pos           _U_(14)                                              /**< (SERCOM_SPIM_CTRLB) Address Mode Position */
#define SERCOM_SPIM_CTRLB_AMODE_Msk           (_U_(0x3) << SERCOM_SPIM_CTRLB_AMODE_Pos)            /**< (SERCOM_SPIM_CTRLB) Address Mode Mask */
#define SERCOM_SPIM_CTRLB_AMODE(value)        (SERCOM_SPIM_CTRLB_AMODE_Msk & ((value) << SERCOM_SPIM_CTRLB_AMODE_Pos))
#define   SERCOM_SPIM_CTRLB_AMODE_MASK_Val    _U_(0x0)                                             /**< (SERCOM_SPIM_CTRLB) ADDRMASK is used as a mask to the ADDR register.  */
#define   SERCOM_SPIM_CTRLB_AMODE_2_ADDRESSES_Val _U_(0x1)                                             /**< (SERCOM_SPIM_CTRLB) The slave responds to the 2 unique addresses in ADDR and ADDRMASK.  */
#define   SERCOM_SPIM_CTRLB_AMODE_RANGE_Val   _U_(0x2)                                             /**< (SERCOM_SPIM_CTRLB) The slave responds to the range of addresses between and including ADDR and ADDRMASK. ADDR is the upper limit.  */
#define SERCOM_SPIM_CTRLB_AMODE_MASK          (SERCOM_SPIM_CTRLB_AMODE_MASK_Val << SERCOM_SPIM_CTRLB_AMODE_Pos) /**< (SERCOM_SPIM_CTRLB) ADDRMASK is used as a mask to the ADDR register. Position  */
#define SERCOM_SPIM_CTRLB_AMODE_2_ADDRESSES   (SERCOM_SPIM_CTRLB_AMODE_2_ADDRESSES_Val << SERCOM_SPIM_CTRLB_AMODE_Pos) /**< (SERCOM_SPIM_CTRLB) The slave responds to the 2 unique addresses in ADDR and ADDRMASK. Position  */
#define SERCOM_SPIM_CTRLB_AMODE_RANGE         (SERCOM_SPIM_CTRLB_AMODE_RANGE_Val << SERCOM_SPIM_CTRLB_AMODE_Pos) /**< (SERCOM_SPIM_CTRLB) The slave responds to the range of addresses between and including ADDR and ADDRMASK. ADDR is the upper limit. Position  */
#define SERCOM_SPIM_CTRLB_RXEN_Pos            _U_(17)                                              /**< (SERCOM_SPIM_CTRLB) Receiver Enable Position */
#define SERCOM_SPIM_CTRLB_RXEN_Msk            (_U_(0x1) << SERCOM_SPIM_CTRLB_RXEN_Pos)             /**< (SERCOM_SPIM_CTRLB) Receiver Enable Mask */
#define SERCOM_SPIM_CTRLB_RXEN(value)         (SERCOM_SPIM_CTRLB_RXEN_Msk & ((value) << SERCOM_SPIM_CTRLB_RXEN_Pos))
#define SERCOM_SPIM_CTRLB_Msk                 _U_(0x0002C047)                                      /**< (SERCOM_SPIM_CTRLB) Register Mask  */


/* -------- SERCOM_SPIS_CTRLB : (SERCOM Offset: 0x04) (R/W 32) SPIS Control B -------- */
#define SERCOM_SPIS_CTRLB_RESETVALUE          _U_(0x00)                                            /**<  (SERCOM_SPIS_CTRLB) SPIS Control B  Reset Value */

#define SERCOM_SPIS_CTRLB_CHSIZE_Pos          _U_(0)                                               /**< (SERCOM_SPIS_CTRLB) Character Size Position */
#define SERCOM_SPIS_CTRLB_CHSIZE_Msk          (_U_(0x7) << SERCOM_SPIS_CTRLB_CHSIZE_Pos)           /**< (SERCOM_SPIS_CTRLB) Character Size Mask */
#define SERCOM_SPIS_CTRLB_CHSIZE(value)       (SERCOM_SPIS_CTRLB_CHSIZE_Msk & ((value) << SERCOM_SPIS_CTRLB_CHSIZE_Pos))
#define   SERCOM_SPIS_CTRLB_CHSIZE_8_BIT_Val  _U_(0x0)                                             /**< (SERCOM_SPIS_CTRLB) 8 bits  */
#define   SERCOM_SPIS_CTRLB_CHSIZE_9_BIT_Val  _U_(0x1)                                             /**< (SERCOM_SPIS_CTRLB) 9 bits  */
#define SERCOM_SPIS_CTRLB_CHSIZE_8_BIT        (SERCOM_SPIS_CTRLB_CHSIZE_8_BIT_Val << SERCOM_SPIS_CTRLB_CHSIZE_Pos) /**< (SERCOM_SPIS_CTRLB) 8 bits Position  */
#define SERCOM_SPIS_CTRLB_CHSIZE_9_BIT        (SERCOM_SPIS_CTRLB_CHSIZE_9_BIT_Val << SERCOM_SPIS_CTRLB_CHSIZE_Pos) /**< (SERCOM_SPIS_CTRLB) 9 bits Position  */
#define SERCOM_SPIS_CTRLB_PLOADEN_Pos         _U_(6)                                               /**< (SERCOM_SPIS_CTRLB) Slave Data Preload Enable Position */
#define SERCOM_SPIS_CTRLB_PLOADEN_Msk         (_U_(0x1) << SERCOM_SPIS_CTRLB_PLOADEN_Pos)          /**< (SERCOM_SPIS_CTRLB) Slave Data Preload Enable Mask */
#define SERCOM_SPIS_CTRLB_PLOADEN(value)      (SERCOM_SPIS_CTRLB_PLOADEN_Msk & ((value) << SERCOM_SPIS_CTRLB_PLOADEN_Pos))
#define SERCOM_SPIS_CTRLB_AMODE_Pos           _U_(14)                                              /**< (SERCOM_SPIS_CTRLB) Address Mode Position */
#define SERCOM_SPIS_CTRLB_AMODE_Msk           (_U_(0x3) << SERCOM_SPIS_CTRLB_AMODE_Pos)            /**< (SERCOM_SPIS_CTRLB) Address Mode Mask */
#define SERCOM_SPIS_CTRLB_AMODE(value)        (SERCOM_SPIS_CTRLB_AMODE_Msk & ((value) << SERCOM_SPIS_CTRLB_AMODE_Pos))
#define   SERCOM_SPIS_CTRLB_AMODE_MASK_Val    _U_(0x0)                                             /**< (SERCOM_SPIS_CTRLB) ADDRMASK is used as a mask to the ADDR register.  */
#define   SERCOM_SPIS_CTRLB_AMODE_2_ADDRESSES_Val _U_(0x1)                                             /**< (SERCOM_SPIS_CTRLB) The slave responds to the 2 unique addresses in ADDR and ADDRMASK.  */
#define   SERCOM_SPIS_CTRLB_AMODE_RANGE_Val   _U_(0x2)                                             /**< (SERCOM_SPIS_CTRLB) The slave responds to the range of addresses between and including ADDR and ADDRMASK. ADDR is the upper limit.  */
#define SERCOM_SPIS_CTRLB_AMODE_MASK          (SERCOM_SPIS_CTRLB_AMODE_MASK_Val << SERCOM_SPIS_CTRLB_AMODE_Pos) /**< (SERCOM_SPIS_CTRLB) ADDRMASK is used as a mask to the ADDR register. Position  */
#define SERCOM_SPIS_CTRLB_AMODE_2_ADDRESSES   (SERCOM_SPIS_CTRLB_AMODE_2_ADDRESSES_Val << SERCOM_SPIS_CTRLB_AMODE_Pos) /**< (SERCOM_SPIS_CTRLB) The slave responds to the 2 unique addresses in ADDR and ADDRMASK. Position  */
#define SERCOM_SPIS_CTRLB_AMODE_RANGE         (SERCOM_SPIS_CTRLB_AMODE_RANGE_Val << SERCOM_SPIS_CTRLB_AMODE_Pos) /**< (SERCOM_SPIS_CTRLB) The slave responds to the range of addresses between and including ADDR and ADDRMASK. ADDR is the upper limit. Position  */
#define SERCOM_SPIS_CTRLB_RXEN_Pos            _U_(17)                                              /**< (SERCOM_SPIS_CTRLB) Receiver Enable Position */
#define SERCOM_SPIS_CTRLB_RXEN_Msk            (_U_(0x1) << SERCOM_SPIS_CTRLB_RXEN_Pos)             /**< (SERCOM_SPIS_CTRLB) Receiver Enable Mask */
#define SERCOM_SPIS_CTRLB_RXEN(value)         (SERCOM_SPIS_CTRLB_RXEN_Msk & ((value) << SERCOM_SPIS_CTRLB_RXEN_Pos))
#define SERCOM_SPIS_CTRLB_Msk                 _U_(0x0002C047)                                      /**< (SERCOM_SPIS_CTRLB) Register Mask  */


/* -------- SERCOM_USART_EXT_CTRLB : (SERCOM Offset: 0x04) (R/W 32) USART_EXT Control B -------- */
#define SERCOM_USART_EXT_CTRLB_RESETVALUE     _U_(0x00)                                            /**<  (SERCOM_USART_EXT_CTRLB) USART_EXT Control B  Reset Value */

#define SERCOM_USART_EXT_CTRLB_CHSIZE_Pos     _U_(0)                                               /**< (SERCOM_USART_EXT_CTRLB) Character Size Position */
#define SERCOM_USART_EXT_CTRLB_CHSIZE_Msk     (_U_(0x7) << SERCOM_USART_EXT_CTRLB_CHSIZE_Pos)      /**< (SERCOM_USART_EXT_CTRLB) Character Size Mask */
#define SERCOM_USART_EXT_CTRLB_CHSIZE(value)  (SERCOM_USART_EXT_CTRLB_CHSIZE_Msk & ((value) << SERCOM_USART_EXT_CTRLB_CHSIZE_Pos))
#define   SERCOM_USART_EXT_CTRLB_CHSIZE_8_BIT_Val _U_(0x0)                                             /**< (SERCOM_USART_EXT_CTRLB) 8 Bits  */
#define   SERCOM_USART_EXT_CTRLB_CHSIZE_9_BIT_Val _U_(0x1)                                             /**< (SERCOM_USART_EXT_CTRLB) 9 Bits  */
#define   SERCOM_USART_EXT_CTRLB_CHSIZE_5_BIT_Val _U_(0x5)                                             /**< (SERCOM_USART_EXT_CTRLB) 5 Bits  */
#define   SERCOM_USART_EXT_CTRLB_CHSIZE_6_BIT_Val _U_(0x6)                                             /**< (SERCOM_USART_EXT_CTRLB) 6 Bits  */
#define   SERCOM_USART_EXT_CTRLB_CHSIZE_7_BIT_Val _U_(0x7)                                             /**< (SERCOM_USART_EXT_CTRLB) 7 Bits  */
#define SERCOM_USART_EXT_CTRLB_CHSIZE_8_BIT   (SERCOM_USART_EXT_CTRLB_CHSIZE_8_BIT_Val << SERCOM_USART_EXT_CTRLB_CHSIZE_Pos) /**< (SERCOM_USART_EXT_CTRLB) 8 Bits Position  */
#define SERCOM_USART_EXT_CTRLB_CHSIZE_9_BIT   (SERCOM_USART_EXT_CTRLB_CHSIZE_9_BIT_Val << SERCOM_USART_EXT_CTRLB_CHSIZE_Pos) /**< (SERCOM_USART_EXT_CTRLB) 9 Bits Position  */
#define SERCOM_USART_EXT_CTRLB_CHSIZE_5_BIT   (SERCOM_USART_EXT_CTRLB_CHSIZE_5_BIT_Val << SERCOM_USART_EXT_CTRLB_CHSIZE_Pos) /**< (SERCOM_USART_EXT_CTRLB) 5 Bits Position  */
#define SERCOM_USART_EXT_CTRLB_CHSIZE_6_BIT   (SERCOM_USART_EXT_CTRLB_CHSIZE_6_BIT_Val << SERCOM_USART_EXT_CTRLB_CHSIZE_Pos) /**< (SERCOM_USART_EXT_CTRLB) 6 Bits Position  */
#define SERCOM_USART_EXT_CTRLB_CHSIZE_7_BIT   (SERCOM_USART_EXT_CTRLB_CHSIZE_7_BIT_Val << SERCOM_USART_EXT_CTRLB_CHSIZE_Pos) /**< (SERCOM_USART_EXT_CTRLB) 7 Bits Position  */
#define SERCOM_USART_EXT_CTRLB_SBMODE_Pos     _U_(6)                                               /**< (SERCOM_USART_EXT_CTRLB) Stop Bit Mode Position */
#define SERCOM_USART_EXT_CTRLB_SBMODE_Msk     (_U_(0x1) << SERCOM_USART_EXT_CTRLB_SBMODE_Pos)      /**< (SERCOM_USART_EXT_CTRLB) Stop Bit Mode Mask */
#define SERCOM_USART_EXT_CTRLB_SBMODE(value)  (SERCOM_USART_EXT_CTRLB_SBMODE_Msk & ((value) << SERCOM_USART_EXT_CTRLB_SBMODE_Pos))
#define   SERCOM_USART_EXT_CTRLB_SBMODE_1_BIT_Val _U_(0x0)                                             /**< (SERCOM_USART_EXT_CTRLB) One Stop Bit  */
#define   SERCOM_USART_EXT_CTRLB_SBMODE_2_BIT_Val _U_(0x1)                                             /**< (SERCOM_USART_EXT_CTRLB) Two Stop Bits  */
#define SERCOM_USART_EXT_CTRLB_SBMODE_1_BIT   (SERCOM_USART_EXT_CTRLB_SBMODE_1_BIT_Val << SERCOM_USART_EXT_CTRLB_SBMODE_Pos) /**< (SERCOM_USART_EXT_CTRLB) One Stop Bit Position  */
#define SERCOM_USART_EXT_CTRLB_SBMODE_2_BIT   (SERCOM_USART_EXT_CTRLB_SBMODE_2_BIT_Val << SERCOM_USART_EXT_CTRLB_SBMODE_Pos) /**< (SERCOM_USART_EXT_CTRLB) Two Stop Bits Position  */
#define SERCOM_USART_EXT_CTRLB_SFDE_Pos       _U_(9)                                               /**< (SERCOM_USART_EXT_CTRLB) Start of Frame Detection Enable Position */
#define SERCOM_USART_EXT_CTRLB_SFDE_Msk       (_U_(0x1) << SERCOM_USART_EXT_CTRLB_SFDE_Pos)        /**< (SERCOM_USART_EXT_CTRLB) Start of Frame Detection Enable Mask */
#define SERCOM_USART_EXT_CTRLB_SFDE(value)    (SERCOM_USART_EXT_CTRLB_SFDE_Msk & ((value) << SERCOM_USART_EXT_CTRLB_SFDE_Pos))
#define SERCOM_USART_EXT_CTRLB_PMODE_Pos      _U_(13)                                              /**< (SERCOM_USART_EXT_CTRLB) Parity Mode Position */
#define SERCOM_USART_EXT_CTRLB_PMODE_Msk      (_U_(0x1) << SERCOM_USART_EXT_CTRLB_PMODE_Pos)       /**< (SERCOM_USART_EXT_CTRLB) Parity Mode Mask */
#define SERCOM_USART_EXT_CTRLB_PMODE(value)   (SERCOM_USART_EXT_CTRLB_PMODE_Msk & ((value) << SERCOM_USART_EXT_CTRLB_PMODE_Pos))
#define   SERCOM_USART_EXT_CTRLB_PMODE_EVEN_Val _U_(0x0)                                             /**< (SERCOM_USART_EXT_CTRLB) Even Parity  */
#define   SERCOM_USART_EXT_CTRLB_PMODE_ODD_Val _U_(0x1)                                             /**< (SERCOM_USART_EXT_CTRLB) Odd Parity  */
#define SERCOM_USART_EXT_CTRLB_PMODE_EVEN     (SERCOM_USART_EXT_CTRLB_PMODE_EVEN_Val << SERCOM_USART_EXT_CTRLB_PMODE_Pos) /**< (SERCOM_USART_EXT_CTRLB) Even Parity Position  */
#define SERCOM_USART_EXT_CTRLB_PMODE_ODD      (SERCOM_USART_EXT_CTRLB_PMODE_ODD_Val << SERCOM_USART_EXT_CTRLB_PMODE_Pos) /**< (SERCOM_USART_EXT_CTRLB) Odd Parity Position  */
#define SERCOM_USART_EXT_CTRLB_TXEN_Pos       _U_(16)                                              /**< (SERCOM_USART_EXT_CTRLB) Transmitter Enable Position */
#define SERCOM_USART_EXT_CTRLB_TXEN_Msk       (_U_(0x1) << SERCOM_USART_EXT_CTRLB_TXEN_Pos)        /**< (SERCOM_USART_EXT_CTRLB) Transmitter Enable Mask */
#define SERCOM_USART_EXT_CTRLB_TXEN(value)    (SERCOM_USART_EXT_CTRLB_TXEN_Msk & ((value) << SERCOM_USART_EXT_CTRLB_TXEN_Pos))
#define SERCOM_USART_EXT_CTRLB_RXEN_Pos       _U_(17)                                              /**< (SERCOM_USART_EXT_CTRLB) Receiver Enable Position */
#define SERCOM_USART_EXT_CTRLB_RXEN_Msk       (_U_(0x1) << SERCOM_USART_EXT_CTRLB_RXEN_Pos)        /**< (SERCOM_USART_EXT_CTRLB) Receiver Enable Mask */
#define SERCOM_USART_EXT_CTRLB_RXEN(value)    (SERCOM_USART_EXT_CTRLB_RXEN_Msk & ((value) << SERCOM_USART_EXT_CTRLB_RXEN_Pos))
#define SERCOM_USART_EXT_CTRLB_Msk            _U_(0x00032247)                                      /**< (SERCOM_USART_EXT_CTRLB) Register Mask  */


/* -------- SERCOM_USART_INT_CTRLB : (SERCOM Offset: 0x04) (R/W 32) USART_INT Control B -------- */
#define SERCOM_USART_INT_CTRLB_RESETVALUE     _U_(0x00)                                            /**<  (SERCOM_USART_INT_CTRLB) USART_INT Control B  Reset Value */

#define SERCOM_USART_INT_CTRLB_CHSIZE_Pos     _U_(0)                                               /**< (SERCOM_USART_INT_CTRLB) Character Size Position */
#define SERCOM_USART_INT_CTRLB_CHSIZE_Msk     (_U_(0x7) << SERCOM_USART_INT_CTRLB_CHSIZE_Pos)      /**< (SERCOM_USART_INT_CTRLB) Character Size Mask */
#define SERCOM_USART_INT_CTRLB_CHSIZE(value)  (SERCOM_USART_INT_CTRLB_CHSIZE_Msk & ((value) << SERCOM_USART_INT_CTRLB_CHSIZE_Pos))
#define   SERCOM_USART_INT_CTRLB_CHSIZE_8_BIT_Val _U_(0x0)                                             /**< (SERCOM_USART_INT_CTRLB) 8 Bits  */
#define   SERCOM_USART_INT_CTRLB_CHSIZE_9_BIT_Val _U_(0x1)                                             /**< (SERCOM_USART_INT_CTRLB) 9 Bits  */
#define   SERCOM_USART_INT_CTRLB_CHSIZE_5_BIT_Val _U_(0x5)                                             /**< (SERCOM_USART_INT_CTRLB) 5 Bits  */
#define   SERCOM_USART_INT_CTRLB_CHSIZE_6_BIT_Val _U_(0x6)                                             /**< (SERCOM_USART_INT_CTRLB) 6 Bits  */
#define   SERCOM_USART_INT_CTRLB_CHSIZE_7_BIT_Val _U_(0x7)                                             /**< (SERCOM_USART_INT_CTRLB) 7 Bits  */
#define SERCOM_USART_INT_CTRLB_CHSIZE_8_BIT   (SERCOM_USART_INT_CTRLB_CHSIZE_8_BIT_Val << SERCOM_USART_INT_CTRLB_CHSIZE_Pos) /**< (SERCOM_USART_INT_CTRLB) 8 Bits Position  */
#define SERCOM_USART_INT_CTRLB_CHSIZE_9_BIT   (SERCOM_USART_INT_CTRLB_CHSIZE_9_BIT_Val << SERCOM_USART_INT_CTRLB_CHSIZE_Pos) /**< (SERCOM_USART_INT_CTRLB) 9 Bits Position  */
#define SERCOM_USART_INT_CTRLB_CHSIZE_5_BIT   (SERCOM_USART_INT_CTRLB_CHSIZE_5_BIT_Val << SERCOM_USART_INT_CTRLB_CHSIZE_Pos) /**< (SERCOM_USART_INT_CTRLB) 5 Bits Position  */
#define SERCOM_USART_INT_CTRLB_CHSIZE_6_BIT   (SERCOM_USART_INT_CTRLB_CHSIZE_6_BIT_Val << SERCOM_USART_INT_CTRLB_CHSIZE_Pos) /**< (SERCOM_USART_INT_CTRLB) 6 Bits Position  */
#define SERCOM_USART_INT_CTRLB_CHSIZE_7_BIT   (SERCOM_USART_INT_CTRLB_CHSIZE_7_BIT_Val << SERCOM_USART_INT_CTRLB_CHSIZE_Pos) /**< (SERCOM_USART_INT_CTRLB) 7 Bits Position  */
#define SERCOM_USART_INT_CTRLB_SBMODE_Pos     _U_(6)                                               /**< (SERCOM_USART_INT_CTRLB) Stop Bit Mode Position */
#define SERCOM_USART_INT_CTRLB_SBMODE_Msk     (_U_(0x1) << SERCOM_USART_INT_CTRLB_SBMODE_Pos)      /**< (SERCOM_USART_INT_CTRLB) Stop Bit Mode Mask */
#define SERCOM_USART_INT_CTRLB_SBMODE(value)  (SERCOM_USART_INT_CTRLB_SBMODE_Msk & ((value) << SERCOM_USART_INT_CTRLB_SBMODE_Pos))
#define   SERCOM_USART_INT_CTRLB_SBMODE_1_BIT_Val _U_(0x0)                                             /**< (SERCOM_USART_INT_CTRLB) One Stop Bit  */
#define   SERCOM_USART_INT_CTRLB_SBMODE_2_BIT_Val _U_(0x1)                                             /**< (SERCOM_USART_INT_CTRLB) Two Stop Bits  */
#define SERCOM_USART_INT_CTRLB_SBMODE_1_BIT   (SERCOM_USART_INT_CTRLB_SBMODE_1_BIT_Val << SERCOM_USART_INT_CTRLB_SBMODE_Pos) /**< (SERCOM_USART_INT_CTRLB) One Stop Bit Position  */
#define SERCOM_USART_INT_CTRLB_SBMODE_2_BIT   (SERCOM_USART_INT_CTRLB_SBMODE_2_BIT_Val << SERCOM_USART_INT_CTRLB_SBMODE_Pos) /**< (SERCOM_USART_INT_CTRLB) Two Stop Bits Position  */
#define SERCOM_USART_INT_CTRLB_SFDE_Pos       _U_(9)                                               /**< (SERCOM_USART_INT_CTRLB) Start of Frame Detection Enable Position */
#define SERCOM_USART_INT_CTRLB_SFDE_Msk       (_U_(0x1) << SERCOM_USART_INT_CTRLB_SFDE_Pos)        /**< (SERCOM_USART_INT_CTRLB) Start of Frame Detection Enable Mask */
#define SERCOM_USART_INT_CTRLB_SFDE(value)    (SERCOM_USART_INT_CTRLB_SFDE_Msk & ((value) << SERCOM_USART_INT_CTRLB_SFDE_Pos))
#define SERCOM_USART_INT_CTRLB_PMODE_Pos      _U_(13)                                              /**< (SERCOM_USART_INT_CTRLB) Parity Mode Position */
#define SERCOM_USART_INT_CTRLB_PMODE_Msk      (_U_(0x1) << SERCOM_USART_INT_CTRLB_PMODE_Pos)       /**< (SERCOM_USART_INT_CTRLB) Parity Mode Mask */
#define SERCOM_USART_INT_CTRLB_PMODE(value)   (SERCOM_USART_INT_CTRLB_PMODE_Msk & ((value) << SERCOM_USART_INT_CTRLB_PMODE_Pos))
#define   SERCOM_USART_INT_CTRLB_PMODE_EVEN_Val _U_(0x0)                                             /**< (SERCOM_USART_INT_CTRLB) Even Parity  */
#define   SERCOM_USART_INT_CTRLB_PMODE_ODD_Val _U_(0x1)                                             /**< (SERCOM_USART_INT_CTRLB) Odd Parity  */
#define SERCOM_USART_INT_CTRLB_PMODE_EVEN     (SERCOM_USART_INT_CTRLB_PMODE_EVEN_Val << SERCOM_USART_INT_CTRLB_PMODE_Pos) /**< (SERCOM_USART_INT_CTRLB) Even Parity Position  */
#define SERCOM_USART_INT_CTRLB_PMODE_ODD      (SERCOM_USART_INT_CTRLB_PMODE_ODD_Val << SERCOM_USART_INT_CTRLB_PMODE_Pos) /**< (SERCOM_USART_INT_CTRLB) Odd Parity Position  */
#define SERCOM_USART_INT_CTRLB_TXEN_Pos       _U_(16)                                              /**< (SERCOM_USART_INT_CTRLB) Transmitter Enable Position */
#define SERCOM_USART_INT_CTRLB_TXEN_Msk       (_U_(0x1) << SERCOM_USART_INT_CTRLB_TXEN_Pos)        /**< (SERCOM_USART_INT_CTRLB) Transmitter Enable Mask */
#define SERCOM_USART_INT_CTRLB_TXEN(value)    (SERCOM_USART_INT_CTRLB_TXEN_Msk & ((value) << SERCOM_USART_INT_CTRLB_TXEN_Pos))
#define SERCOM_USART_INT_CTRLB_RXEN_Pos       _U_(17)                                              /**< (SERCOM_USART_INT_CTRLB) Receiver Enable Position */
#define SERCOM_USART_INT_CTRLB_RXEN_Msk       (_U_(0x1) << SERCOM_USART_INT_CTRLB_RXEN_Pos)        /**< (SERCOM_USART_INT_CTRLB) Receiver Enable Mask */
#define SERCOM_USART_INT_CTRLB_RXEN(value)    (SERCOM_USART_INT_CTRLB_RXEN_Msk & ((value) << SERCOM_USART_INT_CTRLB_RXEN_Pos))
#define SERCOM_USART_INT_CTRLB_Msk            _U_(0x00032247)                                      /**< (SERCOM_USART_INT_CTRLB) Register Mask  */


/* -------- SERCOM_I2CM_DBGCTRL : (SERCOM Offset: 0x08) (R/W 8) I2CM Debug Control -------- */
#define SERCOM_I2CM_DBGCTRL_RESETVALUE        _U_(0x00)                                            /**<  (SERCOM_I2CM_DBGCTRL) I2CM Debug Control  Reset Value */

#define SERCOM_I2CM_DBGCTRL_DBGSTOP_Pos       _U_(0)                                               /**< (SERCOM_I2CM_DBGCTRL) Debug Stop Mode Position */
#define SERCOM_I2CM_DBGCTRL_DBGSTOP_Msk       (_U_(0x1) << SERCOM_I2CM_DBGCTRL_DBGSTOP_Pos)        /**< (SERCOM_I2CM_DBGCTRL) Debug Stop Mode Mask */
#define SERCOM_I2CM_DBGCTRL_DBGSTOP(value)    (SERCOM_I2CM_DBGCTRL_DBGSTOP_Msk & ((value) << SERCOM_I2CM_DBGCTRL_DBGSTOP_Pos))
#define SERCOM_I2CM_DBGCTRL_Msk               _U_(0x01)                                            /**< (SERCOM_I2CM_DBGCTRL) Register Mask  */


/* -------- SERCOM_SPIM_DBGCTRL : (SERCOM Offset: 0x08) (R/W 8) SPIM Debug Control -------- */
#define SERCOM_SPIM_DBGCTRL_RESETVALUE        _U_(0x00)                                            /**<  (SERCOM_SPIM_DBGCTRL) SPIM Debug Control  Reset Value */

#define SERCOM_SPIM_DBGCTRL_DBGSTOP_Pos       _U_(0)                                               /**< (SERCOM_SPIM_DBGCTRL) Debug Stop Mode Position */
#define SERCOM_SPIM_DBGCTRL_DBGSTOP_Msk       (_U_(0x1) << SERCOM_SPIM_DBGCTRL_DBGSTOP_Pos)        /**< (SERCOM_SPIM_DBGCTRL) Debug Stop Mode Mask */
#define SERCOM_SPIM_DBGCTRL_DBGSTOP(value)    (SERCOM_SPIM_DBGCTRL_DBGSTOP_Msk & ((value) << SERCOM_SPIM_DBGCTRL_DBGSTOP_Pos))
#define SERCOM_SPIM_DBGCTRL_Msk               _U_(0x01)                                            /**< (SERCOM_SPIM_DBGCTRL) Register Mask  */


/* -------- SERCOM_SPIS_DBGCTRL : (SERCOM Offset: 0x08) (R/W 8) SPIS Debug Control -------- */
#define SERCOM_SPIS_DBGCTRL_RESETVALUE        _U_(0x00)                                            /**<  (SERCOM_SPIS_DBGCTRL) SPIS Debug Control  Reset Value */

#define SERCOM_SPIS_DBGCTRL_DBGSTOP_Pos       _U_(0)                                               /**< (SERCOM_SPIS_DBGCTRL) Debug Stop Mode Position */
#define SERCOM_SPIS_DBGCTRL_DBGSTOP_Msk       (_U_(0x1) << SERCOM_SPIS_DBGCTRL_DBGSTOP_Pos)        /**< (SERCOM_SPIS_DBGCTRL) Debug Stop Mode Mask */
#define SERCOM_SPIS_DBGCTRL_DBGSTOP(value)    (SERCOM_SPIS_DBGCTRL_DBGSTOP_Msk & ((value) << SERCOM_SPIS_DBGCTRL_DBGSTOP_Pos))
#define SERCOM_SPIS_DBGCTRL_Msk               _U_(0x01)                                            /**< (SERCOM_SPIS_DBGCTRL) Register Mask  */


/* -------- SERCOM_USART_EXT_DBGCTRL : (SERCOM Offset: 0x08) (R/W 8) USART_EXT Debug Control -------- */
#define SERCOM_USART_EXT_DBGCTRL_RESETVALUE   _U_(0x00)                                            /**<  (SERCOM_USART_EXT_DBGCTRL) USART_EXT Debug Control  Reset Value */

#define SERCOM_USART_EXT_DBGCTRL_DBGSTOP_Pos  _U_(0)                                               /**< (SERCOM_USART_EXT_DBGCTRL) Debug Stop Mode Position */
#define SERCOM_USART_EXT_DBGCTRL_DBGSTOP_Msk  (_U_(0x1) << SERCOM_USART_EXT_DBGCTRL_DBGSTOP_Pos)   /**< (SERCOM_USART_EXT_DBGCTRL) Debug Stop Mode Mask */
#define SERCOM_USART_EXT_DBGCTRL_DBGSTOP(value) (SERCOM_USART_EXT_DBGCTRL_DBGSTOP_Msk & ((value) << SERCOM_USART_EXT_DBGCTRL_DBGSTOP_Pos))
#define SERCOM_USART_EXT_DBGCTRL_Msk          _U_(0x01)                                            /**< (SERCOM_USART_EXT_DBGCTRL) Register Mask  */


/* -------- SERCOM_USART_INT_DBGCTRL : (SERCOM Offset: 0x08) (R/W 8) USART_INT Debug Control -------- */
#define SERCOM_USART_INT_DBGCTRL_RESETVALUE   _U_(0x00)                                            /**<  (SERCOM_USART_INT_DBGCTRL) USART_INT Debug Control  Reset Value */

#define SERCOM_USART_INT_DBGCTRL_DBGSTOP_Pos  _U_(0)                                               /**< (SERCOM_USART_INT_DBGCTRL) Debug Stop Mode Position */
#define SERCOM_USART_INT_DBGCTRL_DBGSTOP_Msk  (_U_(0x1) << SERCOM_USART_INT_DBGCTRL_DBGSTOP_Pos)   /**< (SERCOM_USART_INT_DBGCTRL) Debug Stop Mode Mask */
#define SERCOM_USART_INT_DBGCTRL_DBGSTOP(value) (SERCOM_USART_INT_DBGCTRL_DBGSTOP_Msk & ((value) << SERCOM_USART_INT_DBGCTRL_DBGSTOP_Pos))
#define SERCOM_USART_INT_DBGCTRL_Msk          _U_(0x01)                                            /**< (SERCOM_USART_INT_DBGCTRL) Register Mask  */


/* -------- SERCOM_I2CM_BAUD : (SERCOM Offset: 0x0A) (R/W 16) I2CM Baud Rate -------- */
#define SERCOM_I2CM_BAUD_RESETVALUE           _U_(0x00)                                            /**<  (SERCOM_I2CM_BAUD) I2CM Baud Rate  Reset Value */

#define SERCOM_I2CM_BAUD_BAUD_Pos             _U_(0)                                               /**< (SERCOM_I2CM_BAUD) Master Baud Rate Position */
#define SERCOM_I2CM_BAUD_BAUD_Msk             (_U_(0xFF) << SERCOM_I2CM_BAUD_BAUD_Pos)             /**< (SERCOM_I2CM_BAUD) Master Baud Rate Mask */
#define SERCOM_I2CM_BAUD_BAUD(value)          (SERCOM_I2CM_BAUD_BAUD_Msk & ((value) << SERCOM_I2CM_BAUD_BAUD_Pos))
#define SERCOM_I2CM_BAUD_BAUDLOW_Pos          _U_(8)                                               /**< (SERCOM_I2CM_BAUD) Master Baud Rate Low Position */
#define SERCOM_I2CM_BAUD_BAUDLOW_Msk          (_U_(0xFF) << SERCOM_I2CM_BAUD_BAUDLOW_Pos)          /**< (SERCOM_I2CM_BAUD) Master Baud Rate Low Mask */
#define SERCOM_I2CM_BAUD_BAUDLOW(value)       (SERCOM_I2CM_BAUD_BAUDLOW_Msk & ((value) << SERCOM_I2CM_BAUD_BAUDLOW_Pos))
#define SERCOM_I2CM_BAUD_Msk                  _U_(0xFFFF)                                          /**< (SERCOM_I2CM_BAUD) Register Mask  */


/* -------- SERCOM_SPIM_BAUD : (SERCOM Offset: 0x0A) (R/W 8) SPIM Baud Rate -------- */
#define SERCOM_SPIM_BAUD_RESETVALUE           _U_(0x00)                                            /**<  (SERCOM_SPIM_BAUD) SPIM Baud Rate  Reset Value */

#define SERCOM_SPIM_BAUD_BAUD_Pos             _U_(0)                                               /**< (SERCOM_SPIM_BAUD) Baud Register Position */
#define SERCOM_SPIM_BAUD_BAUD_Msk             (_U_(0xFF) << SERCOM_SPIM_BAUD_BAUD_Pos)             /**< (SERCOM_SPIM_BAUD) Baud Register Mask */
#define SERCOM_SPIM_BAUD_BAUD(value)          (SERCOM_SPIM_BAUD_BAUD_Msk & ((value) << SERCOM_SPIM_BAUD_BAUD_Pos))
#define SERCOM_SPIM_BAUD_Msk                  _U_(0xFF)                                            /**< (SERCOM_SPIM_BAUD) Register Mask  */


/* -------- SERCOM_SPIS_BAUD : (SERCOM Offset: 0x0A) (R/W 8) SPIS Baud Rate -------- */
#define SERCOM_SPIS_BAUD_RESETVALUE           _U_(0x00)                                            /**<  (SERCOM_SPIS_BAUD) SPIS Baud Rate  Reset Value */

#define SERCOM_SPIS_BAUD_BAUD_Pos             _U_(0)                                               /**< (SERCOM_SPIS_BAUD) Baud Register Position */
#define SERCOM_SPIS_BAUD_BAUD_Msk             (_U_(0xFF) << SERCOM_SPIS_BAUD_BAUD_Pos)             /**< (SERCOM_SPIS_BAUD) Baud Register Mask */
#define SERCOM_SPIS_BAUD_BAUD(value)          (SERCOM_SPIS_BAUD_BAUD_Msk & ((value) << SERCOM_SPIS_BAUD_BAUD_Pos))
#define SERCOM_SPIS_BAUD_Msk                  _U_(0xFF)                                            /**< (SERCOM_SPIS_BAUD) Register Mask  */


/* -------- SERCOM_USART_EXT_BAUD : (SERCOM Offset: 0x0A) (R/W 16) USART_EXT Baud -------- */
#define SERCOM_USART_EXT_BAUD_RESETVALUE      _U_(0x00)                                            /**<  (SERCOM_USART_EXT_BAUD) USART_EXT Baud  Reset Value */

#define SERCOM_USART_EXT_BAUD_BAUD_Pos        _U_(0)                                               /**< (SERCOM_USART_EXT_BAUD) Baud Value Position */
#define SERCOM_USART_EXT_BAUD_BAUD_Msk        (_U_(0xFFFF) << SERCOM_USART_EXT_BAUD_BAUD_Pos)      /**< (SERCOM_USART_EXT_BAUD) Baud Value Mask */
#define SERCOM_USART_EXT_BAUD_BAUD(value)     (SERCOM_USART_EXT_BAUD_BAUD_Msk & ((value) << SERCOM_USART_EXT_BAUD_BAUD_Pos))
#define SERCOM_USART_EXT_BAUD_Msk             _U_(0xFFFF)                                          /**< (SERCOM_USART_EXT_BAUD) Register Mask  */


/* -------- SERCOM_USART_INT_BAUD : (SERCOM Offset: 0x0A) (R/W 16) USART_INT Baud -------- */
#define SERCOM_USART_INT_BAUD_RESETVALUE      _U_(0x00)                                            /**<  (SERCOM_USART_INT_BAUD) USART_INT Baud  Reset Value */

#define SERCOM_USART_INT_BAUD_BAUD_Pos        _U_(0)                                               /**< (SERCOM_USART_INT_BAUD) Baud Value Position */
#define SERCOM_USART_INT_BAUD_BAUD_Msk        (_U_(0xFFFF) << SERCOM_USART_INT_BAUD_BAUD_Pos)      /**< (SERCOM_USART_INT_BAUD) Baud Value Mask */
#define SERCOM_USART_INT_BAUD_BAUD(value)     (SERCOM_USART_INT_BAUD_BAUD_Msk & ((value) << SERCOM_USART_INT_BAUD_BAUD_Pos))
#define SERCOM_USART_INT_BAUD_Msk             _U_(0xFFFF)                                          /**< (SERCOM_USART_INT_BAUD) Register Mask  */


/* -------- SERCOM_I2CM_INTENCLR : (SERCOM Offset: 0x0C) (R/W 8) I2CM Interrupt Enable Clear -------- */
#define SERCOM_I2CM_INTENCLR_RESETVALUE       _U_(0x00)                                            /**<  (SERCOM_I2CM_INTENCLR) I2CM Interrupt Enable Clear  Reset Value */

#define SERCOM_I2CM_INTENCLR_MB_Pos           _U_(0)                                               /**< (SERCOM_I2CM_INTENCLR) Master on Bus Interrupt Enable Position */
#define SERCOM_I2CM_INTENCLR_MB_Msk           (_U_(0x1) << SERCOM_I2CM_INTENCLR_MB_Pos)            /**< (SERCOM_I2CM_INTENCLR) Master on Bus Interrupt Enable Mask */
#define SERCOM_I2CM_INTENCLR_MB(value)        (SERCOM_I2CM_INTENCLR_MB_Msk & ((value) << SERCOM_I2CM_INTENCLR_MB_Pos))
#define SERCOM_I2CM_INTENCLR_SB_Pos           _U_(1)                                               /**< (SERCOM_I2CM_INTENCLR) Slave on Bus Interrupt Enable Position */
#define SERCOM_I2CM_INTENCLR_SB_Msk           (_U_(0x1) << SERCOM_I2CM_INTENCLR_SB_Pos)            /**< (SERCOM_I2CM_INTENCLR) Slave on Bus Interrupt Enable Mask */
#define SERCOM_I2CM_INTENCLR_SB(value)        (SERCOM_I2CM_INTENCLR_SB_Msk & ((value) << SERCOM_I2CM_INTENCLR_SB_Pos))
#define SERCOM_I2CM_INTENCLR_Msk              _U_(0x03)                                            /**< (SERCOM_I2CM_INTENCLR) Register Mask  */


/* -------- SERCOM_I2CS_INTENCLR : (SERCOM Offset: 0x0C) (R/W 8) I2CS Interrupt Enable Clear -------- */
#define SERCOM_I2CS_INTENCLR_RESETVALUE       _U_(0x00)                                            /**<  (SERCOM_I2CS_INTENCLR) I2CS Interrupt Enable Clear  Reset Value */

#define SERCOM_I2CS_INTENCLR_PREC_Pos         _U_(0)                                               /**< (SERCOM_I2CS_INTENCLR) Stop Received Interrupt Enable Position */
#define SERCOM_I2CS_INTENCLR_PREC_Msk         (_U_(0x1) << SERCOM_I2CS_INTENCLR_PREC_Pos)          /**< (SERCOM_I2CS_INTENCLR) Stop Received Interrupt Enable Mask */
#define SERCOM_I2CS_INTENCLR_PREC(value)      (SERCOM_I2CS_INTENCLR_PREC_Msk & ((value) << SERCOM_I2CS_INTENCLR_PREC_Pos))
#define SERCOM_I2CS_INTENCLR_AMATCH_Pos       _U_(1)                                               /**< (SERCOM_I2CS_INTENCLR) Address Match Interrupt Enable Position */
#define SERCOM_I2CS_INTENCLR_AMATCH_Msk       (_U_(0x1) << SERCOM_I2CS_INTENCLR_AMATCH_Pos)        /**< (SERCOM_I2CS_INTENCLR) Address Match Interrupt Enable Mask */
#define SERCOM_I2CS_INTENCLR_AMATCH(value)    (SERCOM_I2CS_INTENCLR_AMATCH_Msk & ((value) << SERCOM_I2CS_INTENCLR_AMATCH_Pos))
#define SERCOM_I2CS_INTENCLR_DRDY_Pos         _U_(2)                                               /**< (SERCOM_I2CS_INTENCLR) Data Ready Interrupt Enable Position */
#define SERCOM_I2CS_INTENCLR_DRDY_Msk         (_U_(0x1) << SERCOM_I2CS_INTENCLR_DRDY_Pos)          /**< (SERCOM_I2CS_INTENCLR) Data Ready Interrupt Enable Mask */
#define SERCOM_I2CS_INTENCLR_DRDY(value)      (SERCOM_I2CS_INTENCLR_DRDY_Msk & ((value) << SERCOM_I2CS_INTENCLR_DRDY_Pos))
#define SERCOM_I2CS_INTENCLR_Msk              _U_(0x07)                                            /**< (SERCOM_I2CS_INTENCLR) Register Mask  */


/* -------- SERCOM_SPIM_INTENCLR : (SERCOM Offset: 0x0C) (R/W 8) SPIM Interrupt Enable Clear -------- */
#define SERCOM_SPIM_INTENCLR_RESETVALUE       _U_(0x00)                                            /**<  (SERCOM_SPIM_INTENCLR) SPIM Interrupt Enable Clear  Reset Value */

#define SERCOM_SPIM_INTENCLR_DRE_Pos          _U_(0)                                               /**< (SERCOM_SPIM_INTENCLR) Data Register Empty Interrupt Enable Position */
#define SERCOM_SPIM_INTENCLR_DRE_Msk          (_U_(0x1) << SERCOM_SPIM_INTENCLR_DRE_Pos)           /**< (SERCOM_SPIM_INTENCLR) Data Register Empty Interrupt Enable Mask */
#define SERCOM_SPIM_INTENCLR_DRE(value)       (SERCOM_SPIM_INTENCLR_DRE_Msk & ((value) << SERCOM_SPIM_INTENCLR_DRE_Pos))
#define SERCOM_SPIM_INTENCLR_TXC_Pos          _U_(1)                                               /**< (SERCOM_SPIM_INTENCLR) Transmit Complete Interrupt Enable Position */
#define SERCOM_SPIM_INTENCLR_TXC_Msk          (_U_(0x1) << SERCOM_SPIM_INTENCLR_TXC_Pos)           /**< (SERCOM_SPIM_INTENCLR) Transmit Complete Interrupt Enable Mask */
#define SERCOM_SPIM_INTENCLR_TXC(value)       (SERCOM_SPIM_INTENCLR_TXC_Msk & ((value) << SERCOM_SPIM_INTENCLR_TXC_Pos))
#define SERCOM_SPIM_INTENCLR_RXC_Pos          _U_(2)                                               /**< (SERCOM_SPIM_INTENCLR) Receive Complete Interrupt Enable Position */
#define SERCOM_SPIM_INTENCLR_RXC_Msk          (_U_(0x1) << SERCOM_SPIM_INTENCLR_RXC_Pos)           /**< (SERCOM_SPIM_INTENCLR) Receive Complete Interrupt Enable Mask */
#define SERCOM_SPIM_INTENCLR_RXC(value)       (SERCOM_SPIM_INTENCLR_RXC_Msk & ((value) << SERCOM_SPIM_INTENCLR_RXC_Pos))
#define SERCOM_SPIM_INTENCLR_Msk              _U_(0x07)                                            /**< (SERCOM_SPIM_INTENCLR) Register Mask  */


/* -------- SERCOM_SPIS_INTENCLR : (SERCOM Offset: 0x0C) (R/W 8) SPIS Interrupt Enable Clear -------- */
#define SERCOM_SPIS_INTENCLR_RESETVALUE       _U_(0x00)                                            /**<  (SERCOM_SPIS_INTENCLR) SPIS Interrupt Enable Clear  Reset Value */

#define SERCOM_SPIS_INTENCLR_DRE_Pos          _U_(0)                                               /**< (SERCOM_SPIS_INTENCLR) Data Register Empty Interrupt Enable Position */
#define SERCOM_SPIS_INTENCLR_DRE_Msk          (_U_(0x1) << SERCOM_SPIS_INTENCLR_DRE_Pos)           /**< (SERCOM_SPIS_INTENCLR) Data Register Empty Interrupt Enable Mask */
#define SERCOM_SPIS_INTENCLR_DRE(value)       (SERCOM_SPIS_INTENCLR_DRE_Msk & ((value) << SERCOM_SPIS_INTENCLR_DRE_Pos))
#define SERCOM_SPIS_INTENCLR_TXC_Pos          _U_(1)                                               /**< (SERCOM_SPIS_INTENCLR) Transmit Complete Interrupt Enable Position */
#define SERCOM_SPIS_INTENCLR_TXC_Msk          (_U_(0x1) << SERCOM_SPIS_INTENCLR_TXC_Pos)           /**< (SERCOM_SPIS_INTENCLR) Transmit Complete Interrupt Enable Mask */
#define SERCOM_SPIS_INTENCLR_TXC(value)       (SERCOM_SPIS_INTENCLR_TXC_Msk & ((value) << SERCOM_SPIS_INTENCLR_TXC_Pos))
#define SERCOM_SPIS_INTENCLR_RXC_Pos          _U_(2)                                               /**< (SERCOM_SPIS_INTENCLR) Receive Complete Interrupt Enable Position */
#define SERCOM_SPIS_INTENCLR_RXC_Msk          (_U_(0x1) << SERCOM_SPIS_INTENCLR_RXC_Pos)           /**< (SERCOM_SPIS_INTENCLR) Receive Complete Interrupt Enable Mask */
#define SERCOM_SPIS_INTENCLR_RXC(value)       (SERCOM_SPIS_INTENCLR_RXC_Msk & ((value) << SERCOM_SPIS_INTENCLR_RXC_Pos))
#define SERCOM_SPIS_INTENCLR_Msk              _U_(0x07)                                            /**< (SERCOM_SPIS_INTENCLR) Register Mask  */


/* -------- SERCOM_USART_EXT_INTENCLR : (SERCOM Offset: 0x0C) (R/W 8) USART_EXT Interrupt Enable Clear -------- */
#define SERCOM_USART_EXT_INTENCLR_RESETVALUE  _U_(0x00)                                            /**<  (SERCOM_USART_EXT_INTENCLR) USART_EXT Interrupt Enable Clear  Reset Value */

#define SERCOM_USART_EXT_INTENCLR_DRE_Pos     _U_(0)                                               /**< (SERCOM_USART_EXT_INTENCLR) Data Register Empty Interrupt Enable Position */
#define SERCOM_USART_EXT_INTENCLR_DRE_Msk     (_U_(0x1) << SERCOM_USART_EXT_INTENCLR_DRE_Pos)      /**< (SERCOM_USART_EXT_INTENCLR) Data Register Empty Interrupt Enable Mask */
#define SERCOM_USART_EXT_INTENCLR_DRE(value)  (SERCOM_USART_EXT_INTENCLR_DRE_Msk & ((value) << SERCOM_USART_EXT_INTENCLR_DRE_Pos))
#define SERCOM_USART_EXT_INTENCLR_TXC_Pos     _U_(1)                                               /**< (SERCOM_USART_EXT_INTENCLR) Transmit Complete Interrupt Enable Position */
#define SERCOM_USART_EXT_INTENCLR_TXC_Msk     (_U_(0x1) << SERCOM_USART_EXT_INTENCLR_TXC_Pos)      /**< (SERCOM_USART_EXT_INTENCLR) Transmit Complete Interrupt Enable Mask */
#define SERCOM_USART_EXT_INTENCLR_TXC(value)  (SERCOM_USART_EXT_INTENCLR_TXC_Msk & ((value) << SERCOM_USART_EXT_INTENCLR_TXC_Pos))
#define SERCOM_USART_EXT_INTENCLR_RXC_Pos     _U_(2)                                               /**< (SERCOM_USART_EXT_INTENCLR) Receive Complete Interrupt Enable Position */
#define SERCOM_USART_EXT_INTENCLR_RXC_Msk     (_U_(0x1) << SERCOM_USART_EXT_INTENCLR_RXC_Pos)      /**< (SERCOM_USART_EXT_INTENCLR) Receive Complete Interrupt Enable Mask */
#define SERCOM_USART_EXT_INTENCLR_RXC(value)  (SERCOM_USART_EXT_INTENCLR_RXC_Msk & ((value) << SERCOM_USART_EXT_INTENCLR_RXC_Pos))
#define SERCOM_USART_EXT_INTENCLR_RXS_Pos     _U_(3)                                               /**< (SERCOM_USART_EXT_INTENCLR) Receive Start Interrupt Disable Position */
#define SERCOM_USART_EXT_INTENCLR_RXS_Msk     (_U_(0x1) << SERCOM_USART_EXT_INTENCLR_RXS_Pos)      /**< (SERCOM_USART_EXT_INTENCLR) Receive Start Interrupt Disable Mask */
#define SERCOM_USART_EXT_INTENCLR_RXS(value)  (SERCOM_USART_EXT_INTENCLR_RXS_Msk & ((value) << SERCOM_USART_EXT_INTENCLR_RXS_Pos))
#define SERCOM_USART_EXT_INTENCLR_Msk         _U_(0x0F)                                            /**< (SERCOM_USART_EXT_INTENCLR) Register Mask  */


/* -------- SERCOM_USART_INT_INTENCLR : (SERCOM Offset: 0x0C) (R/W 8) USART_INT Interrupt Enable Clear -------- */
#define SERCOM_USART_INT_INTENCLR_RESETVALUE  _U_(0x00)                                            /**<  (SERCOM_USART_INT_INTENCLR) USART_INT Interrupt Enable Clear  Reset Value */

#define SERCOM_USART_INT_INTENCLR_DRE_Pos     _U_(0)                                               /**< (SERCOM_USART_INT_INTENCLR) Data Register Empty Interrupt Enable Position */
#define SERCOM_USART_INT_INTENCLR_DRE_Msk     (_U_(0x1) << SERCOM_USART_INT_INTENCLR_DRE_Pos)      /**< (SERCOM_USART_INT_INTENCLR) Data Register Empty Interrupt Enable Mask */
#define SERCOM_USART_INT_INTENCLR_DRE(value)  (SERCOM_USART_INT_INTENCLR_DRE_Msk & ((value) << SERCOM_USART_INT_INTENCLR_DRE_Pos))
#define SERCOM_USART_INT_INTENCLR_TXC_Pos     _U_(1)                                               /**< (SERCOM_USART_INT_INTENCLR) Transmit Complete Interrupt Enable Position */
#define SERCOM_USART_INT_INTENCLR_TXC_Msk     (_U_(0x1) << SERCOM_USART_INT_INTENCLR_TXC_Pos)      /**< (SERCOM_USART_INT_INTENCLR) Transmit Complete Interrupt Enable Mask */
#define SERCOM_USART_INT_INTENCLR_TXC(value)  (SERCOM_USART_INT_INTENCLR_TXC_Msk & ((value) << SERCOM_USART_INT_INTENCLR_TXC_Pos))
#define SERCOM_USART_INT_INTENCLR_RXC_Pos     _U_(2)                                               /**< (SERCOM_USART_INT_INTENCLR) Receive Complete Interrupt Enable Position */
#define SERCOM_USART_INT_INTENCLR_RXC_Msk     (_U_(0x1) << SERCOM_USART_INT_INTENCLR_RXC_Pos)      /**< (SERCOM_USART_INT_INTENCLR) Receive Complete Interrupt Enable Mask */
#define SERCOM_USART_INT_INTENCLR_RXC(value)  (SERCOM_USART_INT_INTENCLR_RXC_Msk & ((value) << SERCOM_USART_INT_INTENCLR_RXC_Pos))
#define SERCOM_USART_INT_INTENCLR_RXS_Pos     _U_(3)                                               /**< (SERCOM_USART_INT_INTENCLR) Receive Start Interrupt Disable Position */
#define SERCOM_USART_INT_INTENCLR_RXS_Msk     (_U_(0x1) << SERCOM_USART_INT_INTENCLR_RXS_Pos)      /**< (SERCOM_USART_INT_INTENCLR) Receive Start Interrupt Disable Mask */
#define SERCOM_USART_INT_INTENCLR_RXS(value)  (SERCOM_USART_INT_INTENCLR_RXS_Msk & ((value) << SERCOM_USART_INT_INTENCLR_RXS_Pos))
#define SERCOM_USART_INT_INTENCLR_Msk         _U_(0x0F)                                            /**< (SERCOM_USART_INT_INTENCLR) Register Mask  */


/* -------- SERCOM_I2CM_INTENSET : (SERCOM Offset: 0x0D) (R/W 8) I2CM Interrupt Enable Set -------- */
#define SERCOM_I2CM_INTENSET_RESETVALUE       _U_(0x00)                                            /**<  (SERCOM_I2CM_INTENSET) I2CM Interrupt Enable Set  Reset Value */

#define SERCOM_I2CM_INTENSET_MB_Pos           _U_(0)                                               /**< (SERCOM_I2CM_INTENSET) Master on Bus Interrupt Enable Position */
#define SERCOM_I2CM_INTENSET_MB_Msk           (_U_(0x1) << SERCOM_I2CM_INTENSET_MB_Pos)            /**< (SERCOM_I2CM_INTENSET) Master on Bus Interrupt Enable Mask */
#define SERCOM_I2CM_INTENSET_MB(value)        (SERCOM_I2CM_INTENSET_MB_Msk & ((value) << SERCOM_I2CM_INTENSET_MB_Pos))
#define SERCOM_I2CM_INTENSET_SB_Pos           _U_(1)                                               /**< (SERCOM_I2CM_INTENSET) Slave on Bus Interrupt Enable Position */
#define SERCOM_I2CM_INTENSET_SB_Msk           (_U_(0x1) << SERCOM_I2CM_INTENSET_SB_Pos)            /**< (SERCOM_I2CM_INTENSET) Slave on Bus Interrupt Enable Mask */
#define SERCOM_I2CM_INTENSET_SB(value)        (SERCOM_I2CM_INTENSET_SB_Msk & ((value) << SERCOM_I2CM_INTENSET_SB_Pos))
#define SERCOM_I2CM_INTENSET_Msk              _U_(0x03)                                            /**< (SERCOM_I2CM_INTENSET) Register Mask  */


/* -------- SERCOM_I2CS_INTENSET : (SERCOM Offset: 0x0D) (R/W 8) I2CS Interrupt Enable Set -------- */
#define SERCOM_I2CS_INTENSET_RESETVALUE       _U_(0x00)                                            /**<  (SERCOM_I2CS_INTENSET) I2CS Interrupt Enable Set  Reset Value */

#define SERCOM_I2CS_INTENSET_PREC_Pos         _U_(0)                                               /**< (SERCOM_I2CS_INTENSET) Stop Received Interrupt Enable Position */
#define SERCOM_I2CS_INTENSET_PREC_Msk         (_U_(0x1) << SERCOM_I2CS_INTENSET_PREC_Pos)          /**< (SERCOM_I2CS_INTENSET) Stop Received Interrupt Enable Mask */
#define SERCOM_I2CS_INTENSET_PREC(value)      (SERCOM_I2CS_INTENSET_PREC_Msk & ((value) << SERCOM_I2CS_INTENSET_PREC_Pos))
#define SERCOM_I2CS_INTENSET_AMATCH_Pos       _U_(1)                                               /**< (SERCOM_I2CS_INTENSET) Address Match Interrupt Enable Position */
#define SERCOM_I2CS_INTENSET_AMATCH_Msk       (_U_(0x1) << SERCOM_I2CS_INTENSET_AMATCH_Pos)        /**< (SERCOM_I2CS_INTENSET) Address Match Interrupt Enable Mask */
#define SERCOM_I2CS_INTENSET_AMATCH(value)    (SERCOM_I2CS_INTENSET_AMATCH_Msk & ((value) << SERCOM_I2CS_INTENSET_AMATCH_Pos))
#define SERCOM_I2CS_INTENSET_DRDY_Pos         _U_(2)                                               /**< (SERCOM_I2CS_INTENSET) Data Ready Interrupt Enable Position */
#define SERCOM_I2CS_INTENSET_DRDY_Msk         (_U_(0x1) << SERCOM_I2CS_INTENSET_DRDY_Pos)          /**< (SERCOM_I2CS_INTENSET) Data Ready Interrupt Enable Mask */
#define SERCOM_I2CS_INTENSET_DRDY(value)      (SERCOM_I2CS_INTENSET_DRDY_Msk & ((value) << SERCOM_I2CS_INTENSET_DRDY_Pos))
#define SERCOM_I2CS_INTENSET_Msk              _U_(0x07)                                            /**< (SERCOM_I2CS_INTENSET) Register Mask  */


/* -------- SERCOM_SPIM_INTENSET : (SERCOM Offset: 0x0D) (R/W 8) SPIM Interrupt Enable Set -------- */
#define SERCOM_SPIM_INTENSET_RESETVALUE       _U_(0x00)                                            /**<  (SERCOM_SPIM_INTENSET) SPIM Interrupt Enable Set  Reset Value */

#define SERCOM_SPIM_INTENSET_DRE_Pos          _U_(0)                                               /**< (SERCOM_SPIM_INTENSET) Data Register Empty Interrupt Enable Position */
#define SERCOM_SPIM_INTENSET_DRE_Msk          (_U_(0x1) << SERCOM_SPIM_INTENSET_DRE_Pos)           /**< (SERCOM_SPIM_INTENSET) Data Register Empty Interrupt Enable Mask */
#define SERCOM_SPIM_INTENSET_DRE(value)       (SERCOM_SPIM_INTENSET_DRE_Msk & ((value) << SERCOM_SPIM_INTENSET_DRE_Pos))
#define SERCOM_SPIM_INTENSET_TXC_Pos          _U_(1)                                               /**< (SERCOM_SPIM_INTENSET) Transmit Complete Interrupt Enable Position */
#define SERCOM_SPIM_INTENSET_TXC_Msk          (_U_(0x1) << SERCOM_SPIM_INTENSET_TXC_Pos)           /**< (SERCOM_SPIM_INTENSET) Transmit Complete Interrupt Enable Mask */
#define SERCOM_SPIM_INTENSET_TXC(value)       (SERCOM_SPIM_INTENSET_TXC_Msk & ((value) << SERCOM_SPIM_INTENSET_TXC_Pos))
#define SERCOM_SPIM_INTENSET_RXC_Pos          _U_(2)                                               /**< (SERCOM_SPIM_INTENSET) Receive Complete Interrupt Enable Position */
#define SERCOM_SPIM_INTENSET_RXC_Msk          (_U_(0x1) << SERCOM_SPIM_INTENSET_RXC_Pos)           /**< (SERCOM_SPIM_INTENSET) Receive Complete Interrupt Enable Mask */
#define SERCOM_SPIM_INTENSET_RXC(value)       (SERCOM_SPIM_INTENSET_RXC_Msk & ((value) << SERCOM_SPIM_INTENSET_RXC_Pos))
#define SERCOM_SPIM_INTENSET_Msk              _U_(0x07)                                            /**< (SERCOM_SPIM_INTENSET) Register Mask  */


/* -------- SERCOM_SPIS_INTENSET : (SERCOM Offset: 0x0D) (R/W 8) SPIS Interrupt Enable Set -------- */
#define SERCOM_SPIS_INTENSET_RESETVALUE       _U_(0x00)                                            /**<  (SERCOM_SPIS_INTENSET) SPIS Interrupt Enable Set  Reset Value */

#define SERCOM_SPIS_INTENSET_DRE_Pos          _U_(0)                                               /**< (SERCOM_SPIS_INTENSET) Data Register Empty Interrupt Enable Position */
#define SERCOM_SPIS_INTENSET_DRE_Msk          (_U_(0x1) << SERCOM_SPIS_INTENSET_DRE_Pos)           /**< (SERCOM_SPIS_INTENSET) Data Register Empty Interrupt Enable Mask */
#define SERCOM_SPIS_INTENSET_DRE(value)       (SERCOM_SPIS_INTENSET_DRE_Msk & ((value) << SERCOM_SPIS_INTENSET_DRE_Pos))
#define SERCOM_SPIS_INTENSET_TXC_Pos          _U_(1)                                               /**< (SERCOM_SPIS_INTENSET) Transmit Complete Interrupt Enable Position */
#define SERCOM_SPIS_INTENSET_TXC_Msk          (_U_(0x1) << SERCOM_SPIS_INTENSET_TXC_Pos)           /**< (SERCOM_SPIS_INTENSET) Transmit Complete Interrupt Enable Mask */
#define SERCOM_SPIS_INTENSET_TXC(value)       (SERCOM_SPIS_INTENSET_TXC_Msk & ((value) << SERCOM_SPIS_INTENSET_TXC_Pos))
#define SERCOM_SPIS_INTENSET_RXC_Pos          _U_(2)                                               /**< (SERCOM_SPIS_INTENSET) Receive Complete Interrupt Enable Position */
#define SERCOM_SPIS_INTENSET_RXC_Msk          (_U_(0x1) << SERCOM_SPIS_INTENSET_RXC_Pos)           /**< (SERCOM_SPIS_INTENSET) Receive Complete Interrupt Enable Mask */
#define SERCOM_SPIS_INTENSET_RXC(value)       (SERCOM_SPIS_INTENSET_RXC_Msk & ((value) << SERCOM_SPIS_INTENSET_RXC_Pos))
#define SERCOM_SPIS_INTENSET_Msk              _U_(0x07)                                            /**< (SERCOM_SPIS_INTENSET) Register Mask  */


/* -------- SERCOM_USART_EXT_INTENSET : (SERCOM Offset: 0x0D) (R/W 8) USART_EXT Interrupt Enable Set -------- */
#define SERCOM_USART_EXT_INTENSET_RESETVALUE  _U_(0x00)                                            /**<  (SERCOM_USART_EXT_INTENSET) USART_EXT Interrupt Enable Set  Reset Value */

#define SERCOM_USART_EXT_INTENSET_DRE_Pos     _U_(0)                                               /**< (SERCOM_USART_EXT_INTENSET) Data Register Empty Interrupt Enable Position */
#define SERCOM_USART_EXT_INTENSET_DRE_Msk     (_U_(0x1) << SERCOM_USART_EXT_INTENSET_DRE_Pos)      /**< (SERCOM_USART_EXT_INTENSET) Data Register Empty Interrupt Enable Mask */
#define SERCOM_USART_EXT_INTENSET_DRE(value)  (SERCOM_USART_EXT_INTENSET_DRE_Msk & ((value) << SERCOM_USART_EXT_INTENSET_DRE_Pos))
#define SERCOM_USART_EXT_INTENSET_TXC_Pos     _U_(1)                                               /**< (SERCOM_USART_EXT_INTENSET) Transmit Complete Interrupt Enable Position */
#define SERCOM_USART_EXT_INTENSET_TXC_Msk     (_U_(0x1) << SERCOM_USART_EXT_INTENSET_TXC_Pos)      /**< (SERCOM_USART_EXT_INTENSET) Transmit Complete Interrupt Enable Mask */
#define SERCOM_USART_EXT_INTENSET_TXC(value)  (SERCOM_USART_EXT_INTENSET_TXC_Msk & ((value) << SERCOM_USART_EXT_INTENSET_TXC_Pos))
#define SERCOM_USART_EXT_INTENSET_RXC_Pos     _U_(2)                                               /**< (SERCOM_USART_EXT_INTENSET) Receive Complete Interrupt Enable Position */
#define SERCOM_USART_EXT_INTENSET_RXC_Msk     (_U_(0x1) << SERCOM_USART_EXT_INTENSET_RXC_Pos)      /**< (SERCOM_USART_EXT_INTENSET) Receive Complete Interrupt Enable Mask */
#define SERCOM_USART_EXT_INTENSET_RXC(value)  (SERCOM_USART_EXT_INTENSET_RXC_Msk & ((value) << SERCOM_USART_EXT_INTENSET_RXC_Pos))
#define SERCOM_USART_EXT_INTENSET_RXS_Pos     _U_(3)                                               /**< (SERCOM_USART_EXT_INTENSET) Receive Start Interrupt Enable Position */
#define SERCOM_USART_EXT_INTENSET_RXS_Msk     (_U_(0x1) << SERCOM_USART_EXT_INTENSET_RXS_Pos)      /**< (SERCOM_USART_EXT_INTENSET) Receive Start Interrupt Enable Mask */
#define SERCOM_USART_EXT_INTENSET_RXS(value)  (SERCOM_USART_EXT_INTENSET_RXS_Msk & ((value) << SERCOM_USART_EXT_INTENSET_RXS_Pos))
#define SERCOM_USART_EXT_INTENSET_Msk         _U_(0x0F)                                            /**< (SERCOM_USART_EXT_INTENSET) Register Mask  */


/* -------- SERCOM_USART_INT_INTENSET : (SERCOM Offset: 0x0D) (R/W 8) USART_INT Interrupt Enable Set -------- */
#define SERCOM_USART_INT_INTENSET_RESETVALUE  _U_(0x00)                                            /**<  (SERCOM_USART_INT_INTENSET) USART_INT Interrupt Enable Set  Reset Value */

#define SERCOM_USART_INT_INTENSET_DRE_Pos     _U_(0)                                               /**< (SERCOM_USART_INT_INTENSET) Data Register Empty Interrupt Enable Position */
#define SERCOM_USART_INT_INTENSET_DRE_Msk     (_U_(0x1) << SERCOM_USART_INT_INTENSET_DRE_Pos)      /**< (SERCOM_USART_INT_INTENSET) Data Register Empty Interrupt Enable Mask */
#define SERCOM_USART_INT_INTENSET_DRE(value)  (SERCOM_USART_INT_INTENSET_DRE_Msk & ((value) << SERCOM_USART_INT_INTENSET_DRE_Pos))
#define SERCOM_USART_INT_INTENSET_TXC_Pos     _U_(1)                                               /**< (SERCOM_USART_INT_INTENSET) Transmit Complete Interrupt Enable Position */
#define SERCOM_USART_INT_INTENSET_TXC_Msk     (_U_(0x1) << SERCOM_USART_INT_INTENSET_TXC_Pos)      /**< (SERCOM_USART_INT_INTENSET) Transmit Complete Interrupt Enable Mask */
#define SERCOM_USART_INT_INTENSET_TXC(value)  (SERCOM_USART_INT_INTENSET_TXC_Msk & ((value) << SERCOM_USART_INT_INTENSET_TXC_Pos))
#define SERCOM_USART_INT_INTENSET_RXC_Pos     _U_(2)                                               /**< (SERCOM_USART_INT_INTENSET) Receive Complete Interrupt Enable Position */
#define SERCOM_USART_INT_INTENSET_RXC_Msk     (_U_(0x1) << SERCOM_USART_INT_INTENSET_RXC_Pos)      /**< (SERCOM_USART_INT_INTENSET) Receive Complete Interrupt Enable Mask */
#define SERCOM_USART_INT_INTENSET_RXC(value)  (SERCOM_USART_INT_INTENSET_RXC_Msk & ((value) << SERCOM_USART_INT_INTENSET_RXC_Pos))
#define SERCOM_USART_INT_INTENSET_RXS_Pos     _U_(3)                                               /**< (SERCOM_USART_INT_INTENSET) Receive Start Interrupt Enable Position */
#define SERCOM_USART_INT_INTENSET_RXS_Msk     (_U_(0x1) << SERCOM_USART_INT_INTENSET_RXS_Pos)      /**< (SERCOM_USART_INT_INTENSET) Receive Start Interrupt Enable Mask */
#define SERCOM_USART_INT_INTENSET_RXS(value)  (SERCOM_USART_INT_INTENSET_RXS_Msk & ((value) << SERCOM_USART_INT_INTENSET_RXS_Pos))
#define SERCOM_USART_INT_INTENSET_Msk         _U_(0x0F)                                            /**< (SERCOM_USART_INT_INTENSET) Register Mask  */


/* -------- SERCOM_I2CM_INTFLAG : (SERCOM Offset: 0x0E) (R/W 8) I2CM Interrupt Flag Status and Clear -------- */
#define SERCOM_I2CM_INTFLAG_RESETVALUE        _U_(0x00)                                            /**<  (SERCOM_I2CM_INTFLAG) I2CM Interrupt Flag Status and Clear  Reset Value */

#define SERCOM_I2CM_INTFLAG_MB_Pos            _U_(0)                                               /**< (SERCOM_I2CM_INTFLAG) Master on Bus Position */
#define SERCOM_I2CM_INTFLAG_MB_Msk            (_U_(0x1) << SERCOM_I2CM_INTFLAG_MB_Pos)             /**< (SERCOM_I2CM_INTFLAG) Master on Bus Mask */
#define SERCOM_I2CM_INTFLAG_MB(value)         (SERCOM_I2CM_INTFLAG_MB_Msk & ((value) << SERCOM_I2CM_INTFLAG_MB_Pos))
#define SERCOM_I2CM_INTFLAG_SB_Pos            _U_(1)                                               /**< (SERCOM_I2CM_INTFLAG) Slave on Bus Position */
#define SERCOM_I2CM_INTFLAG_SB_Msk            (_U_(0x1) << SERCOM_I2CM_INTFLAG_SB_Pos)             /**< (SERCOM_I2CM_INTFLAG) Slave on Bus Mask */
#define SERCOM_I2CM_INTFLAG_SB(value)         (SERCOM_I2CM_INTFLAG_SB_Msk & ((value) << SERCOM_I2CM_INTFLAG_SB_Pos))
#define SERCOM_I2CM_INTFLAG_Msk               _U_(0x03)                                            /**< (SERCOM_I2CM_INTFLAG) Register Mask  */


/* -------- SERCOM_I2CS_INTFLAG : (SERCOM Offset: 0x0E) (R/W 8) I2CS Interrupt Flag Status and Clear -------- */
#define SERCOM_I2CS_INTFLAG_RESETVALUE        _U_(0x00)                                            /**<  (SERCOM_I2CS_INTFLAG) I2CS Interrupt Flag Status and Clear  Reset Value */

#define SERCOM_I2CS_INTFLAG_PREC_Pos          _U_(0)                                               /**< (SERCOM_I2CS_INTFLAG) Stop Received Position */
#define SERCOM_I2CS_INTFLAG_PREC_Msk          (_U_(0x1) << SERCOM_I2CS_INTFLAG_PREC_Pos)           /**< (SERCOM_I2CS_INTFLAG) Stop Received Mask */
#define SERCOM_I2CS_INTFLAG_PREC(value)       (SERCOM_I2CS_INTFLAG_PREC_Msk & ((value) << SERCOM_I2CS_INTFLAG_PREC_Pos))
#define SERCOM_I2CS_INTFLAG_AMATCH_Pos        _U_(1)                                               /**< (SERCOM_I2CS_INTFLAG) Address Match Position */
#define SERCOM_I2CS_INTFLAG_AMATCH_Msk        (_U_(0x1) << SERCOM_I2CS_INTFLAG_AMATCH_Pos)         /**< (SERCOM_I2CS_INTFLAG) Address Match Mask */
#define SERCOM_I2CS_INTFLAG_AMATCH(value)     (SERCOM_I2CS_INTFLAG_AMATCH_Msk & ((value) << SERCOM_I2CS_INTFLAG_AMATCH_Pos))
#define SERCOM_I2CS_INTFLAG_DRDY_Pos          _U_(2)                                               /**< (SERCOM_I2CS_INTFLAG) Data Ready Position */
#define SERCOM_I2CS_INTFLAG_DRDY_Msk          (_U_(0x1) << SERCOM_I2CS_INTFLAG_DRDY_Pos)           /**< (SERCOM_I2CS_INTFLAG) Data Ready Mask */
#define SERCOM_I2CS_INTFLAG_DRDY(value)       (SERCOM_I2CS_INTFLAG_DRDY_Msk & ((value) << SERCOM_I2CS_INTFLAG_DRDY_Pos))
#define SERCOM_I2CS_INTFLAG_Msk               _U_(0x07)                                            /**< (SERCOM_I2CS_INTFLAG) Register Mask  */


/* -------- SERCOM_SPIM_INTFLAG : (SERCOM Offset: 0x0E) (R/W 8) SPIM Interrupt Flag Status and Clear -------- */
#define SERCOM_SPIM_INTFLAG_RESETVALUE        _U_(0x00)                                            /**<  (SERCOM_SPIM_INTFLAG) SPIM Interrupt Flag Status and Clear  Reset Value */

#define SERCOM_SPIM_INTFLAG_DRE_Pos           _U_(0)                                               /**< (SERCOM_SPIM_INTFLAG) Data Register Empty Position */
#define SERCOM_SPIM_INTFLAG_DRE_Msk           (_U_(0x1) << SERCOM_SPIM_INTFLAG_DRE_Pos)            /**< (SERCOM_SPIM_INTFLAG) Data Register Empty Mask */
#define SERCOM_SPIM_INTFLAG_DRE(value)        (SERCOM_SPIM_INTFLAG_DRE_Msk & ((value) << SERCOM_SPIM_INTFLAG_DRE_Pos))
#define SERCOM_SPIM_INTFLAG_TXC_Pos           _U_(1)                                               /**< (SERCOM_SPIM_INTFLAG) Transmit Complete Position */
#define SERCOM_SPIM_INTFLAG_TXC_Msk           (_U_(0x1) << SERCOM_SPIM_INTFLAG_TXC_Pos)            /**< (SERCOM_SPIM_INTFLAG) Transmit Complete Mask */
#define SERCOM_SPIM_INTFLAG_TXC(value)        (SERCOM_SPIM_INTFLAG_TXC_Msk & ((value) << SERCOM_SPIM_INTFLAG_TXC_Pos))
#define SERCOM_SPIM_INTFLAG_RXC_Pos           _U_(2)                                               /**< (SERCOM_SPIM_INTFLAG) Receive Complete Position */
#define SERCOM_SPIM_INTFLAG_RXC_Msk           (_U_(0x1) << SERCOM_SPIM_INTFLAG_RXC_Pos)            /**< (SERCOM_SPIM_INTFLAG) Receive Complete Mask */
#define SERCOM_SPIM_INTFLAG_RXC(value)        (SERCOM_SPIM_INTFLAG_RXC_Msk & ((value) << SERCOM_SPIM_INTFLAG_RXC_Pos))
#define SERCOM_SPIM_INTFLAG_Msk               _U_(0x07)                                            /**< (SERCOM_SPIM_INTFLAG) Register Mask  */


/* -------- SERCOM_SPIS_INTFLAG : (SERCOM Offset: 0x0E) (R/W 8) SPIS Interrupt Flag Status and Clear -------- */
#define SERCOM_SPIS_INTFLAG_RESETVALUE        _U_(0x00)                                            /**<  (SERCOM_SPIS_INTFLAG) SPIS Interrupt Flag Status and Clear  Reset Value */

#define SERCOM_SPIS_INTFLAG_DRE_Pos           _U_(0)                                               /**< (SERCOM_SPIS_INTFLAG) Data Register Empty Position */
#define SERCOM_SPIS_INTFLAG_DRE_Msk           (_U_(0x1) << SERCOM_SPIS_INTFLAG_DRE_Pos)            /**< (SERCOM_SPIS_INTFLAG) Data Register Empty Mask */
#define SERCOM_SPIS_INTFLAG_DRE(value)        (SERCOM_SPIS_INTFLAG_DRE_Msk & ((value) << SERCOM_SPIS_INTFLAG_DRE_Pos))
#define SERCOM_SPIS_INTFLAG_TXC_Pos           _U_(1)                                               /**< (SERCOM_SPIS_INTFLAG) Transmit Complete Position */
#define SERCOM_SPIS_INTFLAG_TXC_Msk           (_U_(0x1) << SERCOM_SPIS_INTFLAG_TXC_Pos)            /**< (SERCOM_SPIS_INTFLAG) Transmit Complete Mask */
#define SERCOM_SPIS_INTFLAG_TXC(value)        (SERCOM_SPIS_INTFLAG_TXC_Msk & ((value) << SERCOM_SPIS_INTFLAG_TXC_Pos))
#define SERCOM_SPIS_INTFLAG_RXC_Pos           _U_(2)                                               /**< (SERCOM_SPIS_INTFLAG) Receive Complete Position */
#define SERCOM_SPIS_INTFLAG_RXC_Msk           (_U_(0x1) << SERCOM_SPIS_INTFLAG_RXC_Pos)            /**< (SERCOM_SPIS_INTFLAG) Receive Complete Mask */
#define SERCOM_SPIS_INTFLAG_RXC(value)        (SERCOM_SPIS_INTFLAG_RXC_Msk & ((value) << SERCOM_SPIS_INTFLAG_RXC_Pos))
#define SERCOM_SPIS_INTFLAG_Msk               _U_(0x07)                                            /**< (SERCOM_SPIS_INTFLAG) Register Mask  */


/* -------- SERCOM_USART_EXT_INTFLAG : (SERCOM Offset: 0x0E) (R/W 8) USART_EXT Interrupt Flag Status and Clear -------- */
#define SERCOM_USART_EXT_INTFLAG_RESETVALUE   _U_(0x00)                                            /**<  (SERCOM_USART_EXT_INTFLAG) USART_EXT Interrupt Flag Status and Clear  Reset Value */

#define SERCOM_USART_EXT_INTFLAG_DRE_Pos      _U_(0)                                               /**< (SERCOM_USART_EXT_INTFLAG) Data Register Empty Position */
#define SERCOM_USART_EXT_INTFLAG_DRE_Msk      (_U_(0x1) << SERCOM_USART_EXT_INTFLAG_DRE_Pos)       /**< (SERCOM_USART_EXT_INTFLAG) Data Register Empty Mask */
#define SERCOM_USART_EXT_INTFLAG_DRE(value)   (SERCOM_USART_EXT_INTFLAG_DRE_Msk & ((value) << SERCOM_USART_EXT_INTFLAG_DRE_Pos))
#define SERCOM_USART_EXT_INTFLAG_TXC_Pos      _U_(1)                                               /**< (SERCOM_USART_EXT_INTFLAG) Transmit Complete Position */
#define SERCOM_USART_EXT_INTFLAG_TXC_Msk      (_U_(0x1) << SERCOM_USART_EXT_INTFLAG_TXC_Pos)       /**< (SERCOM_USART_EXT_INTFLAG) Transmit Complete Mask */
#define SERCOM_USART_EXT_INTFLAG_TXC(value)   (SERCOM_USART_EXT_INTFLAG_TXC_Msk & ((value) << SERCOM_USART_EXT_INTFLAG_TXC_Pos))
#define SERCOM_USART_EXT_INTFLAG_RXC_Pos      _U_(2)                                               /**< (SERCOM_USART_EXT_INTFLAG) Receive Complete Position */
#define SERCOM_USART_EXT_INTFLAG_RXC_Msk      (_U_(0x1) << SERCOM_USART_EXT_INTFLAG_RXC_Pos)       /**< (SERCOM_USART_EXT_INTFLAG) Receive Complete Mask */
#define SERCOM_USART_EXT_INTFLAG_RXC(value)   (SERCOM_USART_EXT_INTFLAG_RXC_Msk & ((value) << SERCOM_USART_EXT_INTFLAG_RXC_Pos))
#define SERCOM_USART_EXT_INTFLAG_RXS_Pos      _U_(3)                                               /**< (SERCOM_USART_EXT_INTFLAG) Receive Start Interrupt Position */
#define SERCOM_USART_EXT_INTFLAG_RXS_Msk      (_U_(0x1) << SERCOM_USART_EXT_INTFLAG_RXS_Pos)       /**< (SERCOM_USART_EXT_INTFLAG) Receive Start Interrupt Mask */
#define SERCOM_USART_EXT_INTFLAG_RXS(value)   (SERCOM_USART_EXT_INTFLAG_RXS_Msk & ((value) << SERCOM_USART_EXT_INTFLAG_RXS_Pos))
#define SERCOM_USART_EXT_INTFLAG_Msk          _U_(0x0F)                                            /**< (SERCOM_USART_EXT_INTFLAG) Register Mask  */


/* -------- SERCOM_USART_INT_INTFLAG : (SERCOM Offset: 0x0E) (R/W 8) USART_INT Interrupt Flag Status and Clear -------- */
#define SERCOM_USART_INT_INTFLAG_RESETVALUE   _U_(0x00)                                            /**<  (SERCOM_USART_INT_INTFLAG) USART_INT Interrupt Flag Status and Clear  Reset Value */

#define SERCOM_USART_INT_INTFLAG_DRE_Pos      _U_(0)                                               /**< (SERCOM_USART_INT_INTFLAG) Data Register Empty Position */
#define SERCOM_USART_INT_INTFLAG_DRE_Msk      (_U_(0x1) << SERCOM_USART_INT_INTFLAG_DRE_Pos)       /**< (SERCOM_USART_INT_INTFLAG) Data Register Empty Mask */
#define SERCOM_USART_INT_INTFLAG_DRE(value)   (SERCOM_USART_INT_INTFLAG_DRE_Msk & ((value) << SERCOM_USART_INT_INTFLAG_DRE_Pos))
#define SERCOM_USART_INT_INTFLAG_TXC_Pos      _U_(1)                                               /**< (SERCOM_USART_INT_INTFLAG) Transmit Complete Position */
#define SERCOM_USART_INT_INTFLAG_TXC_Msk      (_U_(0x1) << SERCOM_USART_INT_INTFLAG_TXC_Pos)       /**< (SERCOM_USART_INT_INTFLAG) Transmit Complete Mask */
#define SERCOM_USART_INT_INTFLAG_TXC(value)   (SERCOM_USART_INT_INTFLAG_TXC_Msk & ((value) << SERCOM_USART_INT_INTFLAG_TXC_Pos))
#define SERCOM_USART_INT_INTFLAG_RXC_Pos      _U_(2)                                               /**< (SERCOM_USART_INT_INTFLAG) Receive Complete Position */
#define SERCOM_USART_INT_INTFLAG_RXC_Msk      (_U_(0x1) << SERCOM_USART_INT_INTFLAG_RXC_Pos)       /**< (SERCOM_USART_INT_INTFLAG) Receive Complete Mask */
#define SERCOM_USART_INT_INTFLAG_RXC(value)   (SERCOM_USART_INT_INTFLAG_RXC_Msk & ((value) << SERCOM_USART_INT_INTFLAG_RXC_Pos))
#define SERCOM_USART_INT_INTFLAG_RXS_Pos      _U_(3)                                               /**< (SERCOM_USART_INT_INTFLAG) Receive Start Interrupt Position */
#define SERCOM_USART_INT_INTFLAG_RXS_Msk      (_U_(0x1) << SERCOM_USART_INT_INTFLAG_RXS_Pos)       /**< (SERCOM_USART_INT_INTFLAG) Receive Start Interrupt Mask */
#define SERCOM_USART_INT_INTFLAG_RXS(value)   (SERCOM_USART_INT_INTFLAG_RXS_Msk & ((value) << SERCOM_USART_INT_INTFLAG_RXS_Pos))
#define SERCOM_USART_INT_INTFLAG_Msk          _U_(0x0F)                                            /**< (SERCOM_USART_INT_INTFLAG) Register Mask  */


/* -------- SERCOM_I2CM_STATUS : (SERCOM Offset: 0x10) (R/W 16) I2CM Status -------- */
#define SERCOM_I2CM_STATUS_RESETVALUE         _U_(0x00)                                            /**<  (SERCOM_I2CM_STATUS) I2CM Status  Reset Value */

#define SERCOM_I2CM_STATUS_BUSERR_Pos         _U_(0)                                               /**< (SERCOM_I2CM_STATUS) Bus Error Position */
#define SERCOM_I2CM_STATUS_BUSERR_Msk         (_U_(0x1) << SERCOM_I2CM_STATUS_BUSERR_Pos)          /**< (SERCOM_I2CM_STATUS) Bus Error Mask */
#define SERCOM_I2CM_STATUS_BUSERR(value)      (SERCOM_I2CM_STATUS_BUSERR_Msk & ((value) << SERCOM_I2CM_STATUS_BUSERR_Pos))
#define SERCOM_I2CM_STATUS_ARBLOST_Pos        _U_(1)                                               /**< (SERCOM_I2CM_STATUS) Arbitration Lost Position */
#define SERCOM_I2CM_STATUS_ARBLOST_Msk        (_U_(0x1) << SERCOM_I2CM_STATUS_ARBLOST_Pos)         /**< (SERCOM_I2CM_STATUS) Arbitration Lost Mask */
#define SERCOM_I2CM_STATUS_ARBLOST(value)     (SERCOM_I2CM_STATUS_ARBLOST_Msk & ((value) << SERCOM_I2CM_STATUS_ARBLOST_Pos))
#define SERCOM_I2CM_STATUS_RXNACK_Pos         _U_(2)                                               /**< (SERCOM_I2CM_STATUS) Received Not Acknowledge Position */
#define SERCOM_I2CM_STATUS_RXNACK_Msk         (_U_(0x1) << SERCOM_I2CM_STATUS_RXNACK_Pos)          /**< (SERCOM_I2CM_STATUS) Received Not Acknowledge Mask */
#define SERCOM_I2CM_STATUS_RXNACK(value)      (SERCOM_I2CM_STATUS_RXNACK_Msk & ((value) << SERCOM_I2CM_STATUS_RXNACK_Pos))
#define SERCOM_I2CM_STATUS_BUSSTATE_Pos       _U_(4)                                               /**< (SERCOM_I2CM_STATUS) Bus State Position */
#define SERCOM_I2CM_STATUS_BUSSTATE_Msk       (_U_(0x3) << SERCOM_I2CM_STATUS_BUSSTATE_Pos)        /**< (SERCOM_I2CM_STATUS) Bus State Mask */
#define SERCOM_I2CM_STATUS_BUSSTATE(value)    (SERCOM_I2CM_STATUS_BUSSTATE_Msk & ((value) << SERCOM_I2CM_STATUS_BUSSTATE_Pos))
#define SERCOM_I2CM_STATUS_LOWTOUT_Pos        _U_(6)                                               /**< (SERCOM_I2CM_STATUS) SCL Low Time-out Position */
#define SERCOM_I2CM_STATUS_LOWTOUT_Msk        (_U_(0x1) << SERCOM_I2CM_STATUS_LOWTOUT_Pos)         /**< (SERCOM_I2CM_STATUS) SCL Low Time-out Mask */
#define SERCOM_I2CM_STATUS_LOWTOUT(value)     (SERCOM_I2CM_STATUS_LOWTOUT_Msk & ((value) << SERCOM_I2CM_STATUS_LOWTOUT_Pos))
#define SERCOM_I2CM_STATUS_CLKHOLD_Pos        _U_(7)                                               /**< (SERCOM_I2CM_STATUS) Clock Hold Position */
#define SERCOM_I2CM_STATUS_CLKHOLD_Msk        (_U_(0x1) << SERCOM_I2CM_STATUS_CLKHOLD_Pos)         /**< (SERCOM_I2CM_STATUS) Clock Hold Mask */
#define SERCOM_I2CM_STATUS_CLKHOLD(value)     (SERCOM_I2CM_STATUS_CLKHOLD_Msk & ((value) << SERCOM_I2CM_STATUS_CLKHOLD_Pos))
#define SERCOM_I2CM_STATUS_SYNCBUSY_Pos       _U_(15)                                              /**< (SERCOM_I2CM_STATUS) Synchronization Busy Position */
#define SERCOM_I2CM_STATUS_SYNCBUSY_Msk       (_U_(0x1) << SERCOM_I2CM_STATUS_SYNCBUSY_Pos)        /**< (SERCOM_I2CM_STATUS) Synchronization Busy Mask */
#define SERCOM_I2CM_STATUS_SYNCBUSY(value)    (SERCOM_I2CM_STATUS_SYNCBUSY_Msk & ((value) << SERCOM_I2CM_STATUS_SYNCBUSY_Pos))
#define SERCOM_I2CM_STATUS_Msk                _U_(0x80F7)                                          /**< (SERCOM_I2CM_STATUS) Register Mask  */


/* -------- SERCOM_I2CS_STATUS : (SERCOM Offset: 0x10) (R/W 16) I2CS Status -------- */
#define SERCOM_I2CS_STATUS_RESETVALUE         _U_(0x00)                                            /**<  (SERCOM_I2CS_STATUS) I2CS Status  Reset Value */

#define SERCOM_I2CS_STATUS_BUSERR_Pos         _U_(0)                                               /**< (SERCOM_I2CS_STATUS) Bus Error Position */
#define SERCOM_I2CS_STATUS_BUSERR_Msk         (_U_(0x1) << SERCOM_I2CS_STATUS_BUSERR_Pos)          /**< (SERCOM_I2CS_STATUS) Bus Error Mask */
#define SERCOM_I2CS_STATUS_BUSERR(value)      (SERCOM_I2CS_STATUS_BUSERR_Msk & ((value) << SERCOM_I2CS_STATUS_BUSERR_Pos))
#define SERCOM_I2CS_STATUS_COLL_Pos           _U_(1)                                               /**< (SERCOM_I2CS_STATUS) Transmit Collision Position */
#define SERCOM_I2CS_STATUS_COLL_Msk           (_U_(0x1) << SERCOM_I2CS_STATUS_COLL_Pos)            /**< (SERCOM_I2CS_STATUS) Transmit Collision Mask */
#define SERCOM_I2CS_STATUS_COLL(value)        (SERCOM_I2CS_STATUS_COLL_Msk & ((value) << SERCOM_I2CS_STATUS_COLL_Pos))
#define SERCOM_I2CS_STATUS_RXNACK_Pos         _U_(2)                                               /**< (SERCOM_I2CS_STATUS) Received Not Acknowledge Position */
#define SERCOM_I2CS_STATUS_RXNACK_Msk         (_U_(0x1) << SERCOM_I2CS_STATUS_RXNACK_Pos)          /**< (SERCOM_I2CS_STATUS) Received Not Acknowledge Mask */
#define SERCOM_I2CS_STATUS_RXNACK(value)      (SERCOM_I2CS_STATUS_RXNACK_Msk & ((value) << SERCOM_I2CS_STATUS_RXNACK_Pos))
#define SERCOM_I2CS_STATUS_DIR_Pos            _U_(3)                                               /**< (SERCOM_I2CS_STATUS) Read / Write Direction Position */
#define SERCOM_I2CS_STATUS_DIR_Msk            (_U_(0x1) << SERCOM_I2CS_STATUS_DIR_Pos)             /**< (SERCOM_I2CS_STATUS) Read / Write Direction Mask */
#define SERCOM_I2CS_STATUS_DIR(value)         (SERCOM_I2CS_STATUS_DIR_Msk & ((value) << SERCOM_I2CS_STATUS_DIR_Pos))
#define SERCOM_I2CS_STATUS_SR_Pos             _U_(4)                                               /**< (SERCOM_I2CS_STATUS) Repeated Start Position */
#define SERCOM_I2CS_STATUS_SR_Msk             (_U_(0x1) << SERCOM_I2CS_STATUS_SR_Pos)              /**< (SERCOM_I2CS_STATUS) Repeated Start Mask */
#define SERCOM_I2CS_STATUS_SR(value)          (SERCOM_I2CS_STATUS_SR_Msk & ((value) << SERCOM_I2CS_STATUS_SR_Pos))
#define SERCOM_I2CS_STATUS_LOWTOUT_Pos        _U_(6)                                               /**< (SERCOM_I2CS_STATUS) SCL Low Time-out Position */
#define SERCOM_I2CS_STATUS_LOWTOUT_Msk        (_U_(0x1) << SERCOM_I2CS_STATUS_LOWTOUT_Pos)         /**< (SERCOM_I2CS_STATUS) SCL Low Time-out Mask */
#define SERCOM_I2CS_STATUS_LOWTOUT(value)     (SERCOM_I2CS_STATUS_LOWTOUT_Msk & ((value) << SERCOM_I2CS_STATUS_LOWTOUT_Pos))
#define SERCOM_I2CS_STATUS_CLKHOLD_Pos        _U_(7)                                               /**< (SERCOM_I2CS_STATUS) Clock Hold Position */
#define SERCOM_I2CS_STATUS_CLKHOLD_Msk        (_U_(0x1) << SERCOM_I2CS_STATUS_CLKHOLD_Pos)         /**< (SERCOM_I2CS_STATUS) Clock Hold Mask */
#define SERCOM_I2CS_STATUS_CLKHOLD(value)     (SERCOM_I2CS_STATUS_CLKHOLD_Msk & ((value) << SERCOM_I2CS_STATUS_CLKHOLD_Pos))
#define SERCOM_I2CS_STATUS_SYNCBUSY_Pos       _U_(15)                                              /**< (SERCOM_I2CS_STATUS) Synchronization Busy Position */
#define SERCOM_I2CS_STATUS_SYNCBUSY_Msk       (_U_(0x1) << SERCOM_I2CS_STATUS_SYNCBUSY_Pos)        /**< (SERCOM_I2CS_STATUS) Synchronization Busy Mask */
#define SERCOM_I2CS_STATUS_SYNCBUSY(value)    (SERCOM_I2CS_STATUS_SYNCBUSY_Msk & ((value) << SERCOM_I2CS_STATUS_SYNCBUSY_Pos))
#define SERCOM_I2CS_STATUS_Msk                _U_(0x80DF)                                          /**< (SERCOM_I2CS_STATUS) Register Mask  */


/* -------- SERCOM_SPIM_STATUS : (SERCOM Offset: 0x10) (R/W 16) SPIM Status -------- */
#define SERCOM_SPIM_STATUS_RESETVALUE         _U_(0x00)                                            /**<  (SERCOM_SPIM_STATUS) SPIM Status  Reset Value */

#define SERCOM_SPIM_STATUS_BUFOVF_Pos         _U_(2)                                               /**< (SERCOM_SPIM_STATUS) Buffer Overflow Position */
#define SERCOM_SPIM_STATUS_BUFOVF_Msk         (_U_(0x1) << SERCOM_SPIM_STATUS_BUFOVF_Pos)          /**< (SERCOM_SPIM_STATUS) Buffer Overflow Mask */
#define SERCOM_SPIM_STATUS_BUFOVF(value)      (SERCOM_SPIM_STATUS_BUFOVF_Msk & ((value) << SERCOM_SPIM_STATUS_BUFOVF_Pos))
#define SERCOM_SPIM_STATUS_SYNCBUSY_Pos       _U_(15)                                              /**< (SERCOM_SPIM_STATUS) Synchronization Busy Position */
#define SERCOM_SPIM_STATUS_SYNCBUSY_Msk       (_U_(0x1) << SERCOM_SPIM_STATUS_SYNCBUSY_Pos)        /**< (SERCOM_SPIM_STATUS) Synchronization Busy Mask */
#define SERCOM_SPIM_STATUS_SYNCBUSY(value)    (SERCOM_SPIM_STATUS_SYNCBUSY_Msk & ((value) << SERCOM_SPIM_STATUS_SYNCBUSY_Pos))
#define SERCOM_SPIM_STATUS_Msk                _U_(0x8004)                                          /**< (SERCOM_SPIM_STATUS) Register Mask  */


/* -------- SERCOM_SPIS_STATUS : (SERCOM Offset: 0x10) (R/W 16) SPIS Status -------- */
#define SERCOM_SPIS_STATUS_RESETVALUE         _U_(0x00)                                            /**<  (SERCOM_SPIS_STATUS) SPIS Status  Reset Value */

#define SERCOM_SPIS_STATUS_BUFOVF_Pos         _U_(2)                                               /**< (SERCOM_SPIS_STATUS) Buffer Overflow Position */
#define SERCOM_SPIS_STATUS_BUFOVF_Msk         (_U_(0x1) << SERCOM_SPIS_STATUS_BUFOVF_Pos)          /**< (SERCOM_SPIS_STATUS) Buffer Overflow Mask */
#define SERCOM_SPIS_STATUS_BUFOVF(value)      (SERCOM_SPIS_STATUS_BUFOVF_Msk & ((value) << SERCOM_SPIS_STATUS_BUFOVF_Pos))
#define SERCOM_SPIS_STATUS_SYNCBUSY_Pos       _U_(15)                                              /**< (SERCOM_SPIS_STATUS) Synchronization Busy Position */
#define SERCOM_SPIS_STATUS_SYNCBUSY_Msk       (_U_(0x1) << SERCOM_SPIS_STATUS_SYNCBUSY_Pos)        /**< (SERCOM_SPIS_STATUS) Synchronization Busy Mask */
#define SERCOM_SPIS_STATUS_SYNCBUSY(value)    (SERCOM_SPIS_STATUS_SYNCBUSY_Msk & ((value) << SERCOM_SPIS_STATUS_SYNCBUSY_Pos))
#define SERCOM_SPIS_STATUS_Msk                _U_(0x8004)                                          /**< (SERCOM_SPIS_STATUS) Register Mask  */


/* -------- SERCOM_USART_EXT_STATUS : (SERCOM Offset: 0x10) (R/W 16) USART_EXT Status -------- */
#define SERCOM_USART_EXT_STATUS_RESETVALUE    _U_(0x00)                                            /**<  (SERCOM_USART_EXT_STATUS) USART_EXT Status  Reset Value */

#define SERCOM_USART_EXT_STATUS_PERR_Pos      _U_(0)                                               /**< (SERCOM_USART_EXT_STATUS) Parity Error Position */
#define SERCOM_USART_EXT_STATUS_PERR_Msk      (_U_(0x1) << SERCOM_USART_EXT_STATUS_PERR_Pos)       /**< (SERCOM_USART_EXT_STATUS) Parity Error Mask */
#define SERCOM_USART_EXT_STATUS_PERR(value)   (SERCOM_USART_EXT_STATUS_PERR_Msk & ((value) << SERCOM_USART_EXT_STATUS_PERR_Pos))
#define SERCOM_USART_EXT_STATUS_FERR_Pos      _U_(1)                                               /**< (SERCOM_USART_EXT_STATUS) Frame Error Position */
#define SERCOM_USART_EXT_STATUS_FERR_Msk      (_U_(0x1) << SERCOM_USART_EXT_STATUS_FERR_Pos)       /**< (SERCOM_USART_EXT_STATUS) Frame Error Mask */
#define SERCOM_USART_EXT_STATUS_FERR(value)   (SERCOM_USART_EXT_STATUS_FERR_Msk & ((value) << SERCOM_USART_EXT_STATUS_FERR_Pos))
#define SERCOM_USART_EXT_STATUS_BUFOVF_Pos    _U_(2)                                               /**< (SERCOM_USART_EXT_STATUS) Buffer Overflow Position */
#define SERCOM_USART_EXT_STATUS_BUFOVF_Msk    (_U_(0x1) << SERCOM_USART_EXT_STATUS_BUFOVF_Pos)     /**< (SERCOM_USART_EXT_STATUS) Buffer Overflow Mask */
#define SERCOM_USART_EXT_STATUS_BUFOVF(value) (SERCOM_USART_EXT_STATUS_BUFOVF_Msk & ((value) << SERCOM_USART_EXT_STATUS_BUFOVF_Pos))
#define SERCOM_USART_EXT_STATUS_SYNCBUSY_Pos  _U_(15)                                              /**< (SERCOM_USART_EXT_STATUS) Synchronization Busy Position */
#define SERCOM_USART_EXT_STATUS_SYNCBUSY_Msk  (_U_(0x1) << SERCOM_USART_EXT_STATUS_SYNCBUSY_Pos)   /**< (SERCOM_USART_EXT_STATUS) Synchronization Busy Mask */
#define SERCOM_USART_EXT_STATUS_SYNCBUSY(value) (SERCOM_USART_EXT_STATUS_SYNCBUSY_Msk & ((value) << SERCOM_USART_EXT_STATUS_SYNCBUSY_Pos))
#define SERCOM_USART_EXT_STATUS_Msk           _U_(0x8007)                                          /**< (SERCOM_USART_EXT_STATUS) Register Mask  */


/* -------- SERCOM_USART_INT_STATUS : (SERCOM Offset: 0x10) (R/W 16) USART_INT Status -------- */
#define SERCOM_USART_INT_STATUS_RESETVALUE    _U_(0x00)                                            /**<  (SERCOM_USART_INT_STATUS) USART_INT Status  Reset Value */

#define SERCOM_USART_INT_STATUS_PERR_Pos      _U_(0)                                               /**< (SERCOM_USART_INT_STATUS) Parity Error Position */
#define SERCOM_USART_INT_STATUS_PERR_Msk      (_U_(0x1) << SERCOM_USART_INT_STATUS_PERR_Pos)       /**< (SERCOM_USART_INT_STATUS) Parity Error Mask */
#define SERCOM_USART_INT_STATUS_PERR(value)   (SERCOM_USART_INT_STATUS_PERR_Msk & ((value) << SERCOM_USART_INT_STATUS_PERR_Pos))
#define SERCOM_USART_INT_STATUS_FERR_Pos      _U_(1)                                               /**< (SERCOM_USART_INT_STATUS) Frame Error Position */
#define SERCOM_USART_INT_STATUS_FERR_Msk      (_U_(0x1) << SERCOM_USART_INT_STATUS_FERR_Pos)       /**< (SERCOM_USART_INT_STATUS) Frame Error Mask */
#define SERCOM_USART_INT_STATUS_FERR(value)   (SERCOM_USART_INT_STATUS_FERR_Msk & ((value) << SERCOM_USART_INT_STATUS_FERR_Pos))
#define SERCOM_USART_INT_STATUS_BUFOVF_Pos    _U_(2)                                               /**< (SERCOM_USART_INT_STATUS) Buffer Overflow Position */
#define SERCOM_USART_INT_STATUS_BUFOVF_Msk    (_U_(0x1) << SERCOM_USART_INT_STATUS_BUFOVF_Pos)     /**< (SERCOM_USART_INT_STATUS) Buffer Overflow Mask */
#define SERCOM_USART_INT_STATUS_BUFOVF(value) (SERCOM_USART_INT_STATUS_BUFOVF_Msk & ((value) << SERCOM_USART_INT_STATUS_BUFOVF_Pos))
#define SERCOM_USART_INT_STATUS_SYNCBUSY_Pos  _U_(15)                                              /**< (SERCOM_USART_INT_STATUS) Synchronization Busy Position */
#define SERCOM_USART_INT_STATUS_SYNCBUSY_Msk  (_U_(0x1) << SERCOM_USART_INT_STATUS_SYNCBUSY_Pos)   /**< (SERCOM_USART_INT_STATUS) Synchronization Busy Mask */
#define SERCOM_USART_INT_STATUS_SYNCBUSY(value) (SERCOM_USART_INT_STATUS_SYNCBUSY_Msk & ((value) << SERCOM_USART_INT_STATUS_SYNCBUSY_Pos))
#define SERCOM_USART_INT_STATUS_Msk           _U_(0x8007)                                          /**< (SERCOM_USART_INT_STATUS) Register Mask  */


/* -------- SERCOM_I2CM_ADDR : (SERCOM Offset: 0x14) (R/W 8) I2CM Address -------- */
#define SERCOM_I2CM_ADDR_RESETVALUE           _U_(0x00)                                            /**<  (SERCOM_I2CM_ADDR) I2CM Address  Reset Value */

#define SERCOM_I2CM_ADDR_ADDR_Pos             _U_(0)                                               /**< (SERCOM_I2CM_ADDR) Address Position */
#define SERCOM_I2CM_ADDR_ADDR_Msk             (_U_(0xFF) << SERCOM_I2CM_ADDR_ADDR_Pos)             /**< (SERCOM_I2CM_ADDR) Address Mask */
#define SERCOM_I2CM_ADDR_ADDR(value)          (SERCOM_I2CM_ADDR_ADDR_Msk & ((value) << SERCOM_I2CM_ADDR_ADDR_Pos))
#define SERCOM_I2CM_ADDR_Msk                  _U_(0xFF)                                            /**< (SERCOM_I2CM_ADDR) Register Mask  */


/* -------- SERCOM_I2CS_ADDR : (SERCOM Offset: 0x14) (R/W 32) I2CS Address -------- */
#define SERCOM_I2CS_ADDR_RESETVALUE           _U_(0x00)                                            /**<  (SERCOM_I2CS_ADDR) I2CS Address  Reset Value */

#define SERCOM_I2CS_ADDR_GENCEN_Pos           _U_(0)                                               /**< (SERCOM_I2CS_ADDR) General Call Address Enable Position */
#define SERCOM_I2CS_ADDR_GENCEN_Msk           (_U_(0x1) << SERCOM_I2CS_ADDR_GENCEN_Pos)            /**< (SERCOM_I2CS_ADDR) General Call Address Enable Mask */
#define SERCOM_I2CS_ADDR_GENCEN(value)        (SERCOM_I2CS_ADDR_GENCEN_Msk & ((value) << SERCOM_I2CS_ADDR_GENCEN_Pos))
#define SERCOM_I2CS_ADDR_ADDR_Pos             _U_(1)                                               /**< (SERCOM_I2CS_ADDR) Address Position */
#define SERCOM_I2CS_ADDR_ADDR_Msk             (_U_(0x7F) << SERCOM_I2CS_ADDR_ADDR_Pos)             /**< (SERCOM_I2CS_ADDR) Address Mask */
#define SERCOM_I2CS_ADDR_ADDR(value)          (SERCOM_I2CS_ADDR_ADDR_Msk & ((value) << SERCOM_I2CS_ADDR_ADDR_Pos))
#define SERCOM_I2CS_ADDR_ADDRMASK_Pos         _U_(17)                                              /**< (SERCOM_I2CS_ADDR) Address Mask Position */
#define SERCOM_I2CS_ADDR_ADDRMASK_Msk         (_U_(0x7F) << SERCOM_I2CS_ADDR_ADDRMASK_Pos)         /**< (SERCOM_I2CS_ADDR) Address Mask Mask */
#define SERCOM_I2CS_ADDR_ADDRMASK(value)      (SERCOM_I2CS_ADDR_ADDRMASK_Msk & ((value) << SERCOM_I2CS_ADDR_ADDRMASK_Pos))
#define SERCOM_I2CS_ADDR_Msk                  _U_(0x00FE00FF)                                      /**< (SERCOM_I2CS_ADDR) Register Mask  */


/* -------- SERCOM_SPIM_ADDR : (SERCOM Offset: 0x14) (R/W 32) SPIM Address -------- */
#define SERCOM_SPIM_ADDR_RESETVALUE           _U_(0x00)                                            /**<  (SERCOM_SPIM_ADDR) SPIM Address  Reset Value */

#define SERCOM_SPIM_ADDR_ADDR_Pos             _U_(0)                                               /**< (SERCOM_SPIM_ADDR) Address Position */
#define SERCOM_SPIM_ADDR_ADDR_Msk             (_U_(0xFF) << SERCOM_SPIM_ADDR_ADDR_Pos)             /**< (SERCOM_SPIM_ADDR) Address Mask */
#define SERCOM_SPIM_ADDR_ADDR(value)          (SERCOM_SPIM_ADDR_ADDR_Msk & ((value) << SERCOM_SPIM_ADDR_ADDR_Pos))
#define SERCOM_SPIM_ADDR_ADDRMASK_Pos         _U_(16)                                              /**< (SERCOM_SPIM_ADDR) Address Mask Position */
#define SERCOM_SPIM_ADDR_ADDRMASK_Msk         (_U_(0xFF) << SERCOM_SPIM_ADDR_ADDRMASK_Pos)         /**< (SERCOM_SPIM_ADDR) Address Mask Mask */
#define SERCOM_SPIM_ADDR_ADDRMASK(value)      (SERCOM_SPIM_ADDR_ADDRMASK_Msk & ((value) << SERCOM_SPIM_ADDR_ADDRMASK_Pos))
#define SERCOM_SPIM_ADDR_Msk                  _U_(0x00FF00FF)                                      /**< (SERCOM_SPIM_ADDR) Register Mask  */


/* -------- SERCOM_SPIS_ADDR : (SERCOM Offset: 0x14) (R/W 32) SPIS Address -------- */
#define SERCOM_SPIS_ADDR_RESETVALUE           _U_(0x00)                                            /**<  (SERCOM_SPIS_ADDR) SPIS Address  Reset Value */

#define SERCOM_SPIS_ADDR_ADDR_Pos             _U_(0)                                               /**< (SERCOM_SPIS_ADDR) Address Position */
#define SERCOM_SPIS_ADDR_ADDR_Msk             (_U_(0xFF) << SERCOM_SPIS_ADDR_ADDR_Pos)             /**< (SERCOM_SPIS_ADDR) Address Mask */
#define SERCOM_SPIS_ADDR_ADDR(value)          (SERCOM_SPIS_ADDR_ADDR_Msk & ((value) << SERCOM_SPIS_ADDR_ADDR_Pos))
#define SERCOM_SPIS_ADDR_ADDRMASK_Pos         _U_(16)                                              /**< (SERCOM_SPIS_ADDR) Address Mask Position */
#define SERCOM_SPIS_ADDR_ADDRMASK_Msk         (_U_(0xFF) << SERCOM_SPIS_ADDR_ADDRMASK_Pos)         /**< (SERCOM_SPIS_ADDR) Address Mask Mask */
#define SERCOM_SPIS_ADDR_ADDRMASK(value)      (SERCOM_SPIS_ADDR_ADDRMASK_Msk & ((value) << SERCOM_SPIS_ADDR_ADDRMASK_Pos))
#define SERCOM_SPIS_ADDR_Msk                  _U_(0x00FF00FF)                                      /**< (SERCOM_SPIS_ADDR) Register Mask  */


/* -------- SERCOM_I2CM_DATA : (SERCOM Offset: 0x18) (R/W 8) I2CM Data -------- */
#define SERCOM_I2CM_DATA_RESETVALUE           _U_(0x00)                                            /**<  (SERCOM_I2CM_DATA) I2CM Data  Reset Value */

#define SERCOM_I2CM_DATA_DATA_Pos             _U_(0)                                               /**< (SERCOM_I2CM_DATA) Data Position */
#define SERCOM_I2CM_DATA_DATA_Msk             (_U_(0xFF) << SERCOM_I2CM_DATA_DATA_Pos)             /**< (SERCOM_I2CM_DATA) Data Mask */
#define SERCOM_I2CM_DATA_DATA(value)          (SERCOM_I2CM_DATA_DATA_Msk & ((value) << SERCOM_I2CM_DATA_DATA_Pos))
#define SERCOM_I2CM_DATA_Msk                  _U_(0xFF)                                            /**< (SERCOM_I2CM_DATA) Register Mask  */


/* -------- SERCOM_I2CS_DATA : (SERCOM Offset: 0x18) (R/W 8) I2CS Data -------- */
#define SERCOM_I2CS_DATA_RESETVALUE           _U_(0x00)                                            /**<  (SERCOM_I2CS_DATA) I2CS Data  Reset Value */

#define SERCOM_I2CS_DATA_DATA_Pos             _U_(0)                                               /**< (SERCOM_I2CS_DATA) Data Position */
#define SERCOM_I2CS_DATA_DATA_Msk             (_U_(0xFF) << SERCOM_I2CS_DATA_DATA_Pos)             /**< (SERCOM_I2CS_DATA) Data Mask */
#define SERCOM_I2CS_DATA_DATA(value)          (SERCOM_I2CS_DATA_DATA_Msk & ((value) << SERCOM_I2CS_DATA_DATA_Pos))
#define SERCOM_I2CS_DATA_Msk                  _U_(0xFF)                                            /**< (SERCOM_I2CS_DATA) Register Mask  */


/* -------- SERCOM_SPIM_DATA : (SERCOM Offset: 0x18) (R/W 16) SPIM Data -------- */
#define SERCOM_SPIM_DATA_RESETVALUE           _U_(0x00)                                            /**<  (SERCOM_SPIM_DATA) SPIM Data  Reset Value */

#define SERCOM_SPIM_DATA_DATA_Pos             _U_(0)                                               /**< (SERCOM_SPIM_DATA) Data Position */
#define SERCOM_SPIM_DATA_DATA_Msk             (_U_(0x1FF) << SERCOM_SPIM_DATA_DATA_Pos)            /**< (SERCOM_SPIM_DATA) Data Mask */
#define SERCOM_SPIM_DATA_DATA(value)          (SERCOM_SPIM_DATA_DATA_Msk & ((value) << SERCOM_SPIM_DATA_DATA_Pos))
#define SERCOM_SPIM_DATA_Msk                  _U_(0x01FF)                                          /**< (SERCOM_SPIM_DATA) Register Mask  */


/* -------- SERCOM_SPIS_DATA : (SERCOM Offset: 0x18) (R/W 16) SPIS Data -------- */
#define SERCOM_SPIS_DATA_RESETVALUE           _U_(0x00)                                            /**<  (SERCOM_SPIS_DATA) SPIS Data  Reset Value */

#define SERCOM_SPIS_DATA_DATA_Pos             _U_(0)                                               /**< (SERCOM_SPIS_DATA) Data Value Position */
#define SERCOM_SPIS_DATA_DATA_Msk             (_U_(0x1FF) << SERCOM_SPIS_DATA_DATA_Pos)            /**< (SERCOM_SPIS_DATA) Data Value Mask */
#define SERCOM_SPIS_DATA_DATA(value)          (SERCOM_SPIS_DATA_DATA_Msk & ((value) << SERCOM_SPIS_DATA_DATA_Pos))
#define SERCOM_SPIS_DATA_Msk                  _U_(0x01FF)                                          /**< (SERCOM_SPIS_DATA) Register Mask  */


/* -------- SERCOM_USART_EXT_DATA : (SERCOM Offset: 0x18) (R/W 16) USART_EXT Data -------- */
#define SERCOM_USART_EXT_DATA_RESETVALUE      _U_(0x00)                                            /**<  (SERCOM_USART_EXT_DATA) USART_EXT Data  Reset Value */

#define SERCOM_USART_EXT_DATA_DATA_Pos        _U_(0)                                               /**< (SERCOM_USART_EXT_DATA) Data Position */
#define SERCOM_USART_EXT_DATA_DATA_Msk        (_U_(0x1FF) << SERCOM_USART_EXT_DATA_DATA_Pos)       /**< (SERCOM_USART_EXT_DATA) Data Mask */
#define SERCOM_USART_EXT_DATA_DATA(value)     (SERCOM_USART_EXT_DATA_DATA_Msk & ((value) << SERCOM_USART_EXT_DATA_DATA_Pos))
#define SERCOM_USART_EXT_DATA_Msk             _U_(0x01FF)                                          /**< (SERCOM_USART_EXT_DATA) Register Mask  */


/* -------- SERCOM_USART_INT_DATA : (SERCOM Offset: 0x18) (R/W 16) USART_INT Data -------- */
#define SERCOM_USART_INT_DATA_RESETVALUE      _U_(0x00)                                            /**<  (SERCOM_USART_INT_DATA) USART_INT Data  Reset Value */

#define SERCOM_USART_INT_DATA_DATA_Pos        _U_(0)                                               /**< (SERCOM_USART_INT_DATA) Data Position */
#define SERCOM_USART_INT_DATA_DATA_Msk        (_U_(0x1FF) << SERCOM_USART_INT_DATA_DATA_Pos)       /**< (SERCOM_USART_INT_DATA) Data Mask */
#define SERCOM_USART_INT_DATA_DATA(value)     (SERCOM_USART_INT_DATA_DATA_Msk & ((value) << SERCOM_USART_INT_DATA_DATA_Pos))
#define SERCOM_USART_INT_DATA_Msk             _U_(0x01FF)                                          /**< (SERCOM_USART_INT_DATA) Register Mask  */


/** \brief SERCOM register offsets definitions */
#define SERCOM_I2CM_CTRLA_REG_OFST     (0x00)              /**< (SERCOM_I2CM_CTRLA) I2CM Control A Offset */
#define SERCOM_I2CS_CTRLA_REG_OFST     (0x00)              /**< (SERCOM_I2CS_CTRLA) I2CS Control A Offset */
#define SERCOM_SPIM_CTRLA_REG_OFST     (0x00)              /**< (SERCOM_SPIM_CTRLA) SPIM Control A Offset */
#define SERCOM_SPIS_CTRLA_REG_OFST     (0x00)              /**< (SERCOM_SPIS_CTRLA) SPIS Control A Offset */
#define SERCOM_USART_EXT_CTRLA_REG_OFST (0x00)              /**< (SERCOM_USART_EXT_CTRLA) USART_EXT Control A Offset */
#define SERCOM_USART_INT_CTRLA_REG_OFST (0x00)              /**< (SERCOM_USART_INT_CTRLA) USART_INT Control A Offset */
#define SERCOM_I2CM_CTRLB_REG_OFST     (0x04)              /**< (SERCOM_I2CM_CTRLB) I2CM Control B Offset */
#define SERCOM_I2CS_CTRLB_REG_OFST     (0x04)              /**< (SERCOM_I2CS_CTRLB) I2CS Control B Offset */
#define SERCOM_SPIM_CTRLB_REG_OFST     (0x04)              /**< (SERCOM_SPIM_CTRLB) SPIM Control B Offset */
#define SERCOM_SPIS_CTRLB_REG_OFST     (0x04)              /**< (SERCOM_SPIS_CTRLB) SPIS Control B Offset */
#define SERCOM_USART_EXT_CTRLB_REG_OFST (0x04)              /**< (SERCOM_USART_EXT_CTRLB) USART_EXT Control B Offset */
#define SERCOM_USART_INT_CTRLB_REG_OFST (0x04)              /**< (SERCOM_USART_INT_CTRLB) USART_INT Control B Offset */
#define SERCOM_I2CM_DBGCTRL_REG_OFST   (0x08)              /**< (SERCOM_I2CM_DBGCTRL) I2CM Debug Control Offset */
#define SERCOM_SPIM_DBGCTRL_REG_OFST   (0x08)              /**< (SERCOM_SPIM_DBGCTRL) SPIM Debug Control Offset */
#define SERCOM_SPIS_DBGCTRL_REG_OFST   (0x08)              /**< (SERCOM_SPIS_DBGCTRL) SPIS Debug Control Offset */
#define SERCOM_USART_EXT_DBGCTRL_REG_OFST (0x08)              /**< (SERCOM_USART_EXT_DBGCTRL) USART_EXT Debug Control Offset */
#define SERCOM_USART_INT_DBGCTRL_REG_OFST (0x08)              /**< (SERCOM_USART_INT_DBGCTRL) USART_INT Debug Control Offset */
#define SERCOM_I2CM_BAUD_REG_OFST      (0x0A)              /**< (SERCOM_I2CM_BAUD) I2CM Baud Rate Offset */
#define SERCOM_SPIM_BAUD_REG_OFST      (0x0A)              /**< (SERCOM_SPIM_BAUD) SPIM Baud Rate Offset */
#define SERCOM_SPIS_BAUD_REG_OFST      (0x0A)              /**< (SERCOM_SPIS_BAUD) SPIS Baud Rate Offset */
#define SERCOM_USART_EXT_BAUD_REG_OFST (0x0A)              /**< (SERCOM_USART_EXT_BAUD) USART_EXT Baud Offset */
#define SERCOM_USART_INT_BAUD_REG_OFST (0x0A)              /**< (SERCOM_USART_INT_BAUD) USART_INT Baud Offset */
#define SERCOM_I2CM_INTENCLR_REG_OFST  (0x0C)              /**< (SERCOM_I2CM_INTENCLR) I2CM Interrupt Enable Clear Offset */
#define SERCOM_I2CS_INTENCLR_REG_OFST  (0x0C)              /**< (SERCOM_I2CS_INTENCLR) I2CS Interrupt Enable Clear Offset */
#define SERCOM_SPIM_INTENCLR_REG_OFST  (0x0C)              /**< (SERCOM_SPIM_INTENCLR) SPIM Interrupt Enable Clear Offset */
#define SERCOM_SPIS_INTENCLR_REG_OFST  (0x0C)              /**< (SERCOM_SPIS_INTENCLR) SPIS Interrupt Enable Clear Offset */
#define SERCOM_USART_EXT_INTENCLR_REG_OFST (0x0C)              /**< (SERCOM_USART_EXT_INTENCLR) USART_EXT Interrupt Enable Clear Offset */
#define SERCOM_USART_INT_INTENCLR_REG_OFST (0x0C)              /**< (SERCOM_USART_INT_INTENCLR) USART_INT Interrupt Enable Clear Offset */
#define SERCOM_I2CM_INTENSET_REG_OFST  (0x0D)              /**< (SERCOM_I2CM_INTENSET) I2CM Interrupt Enable Set Offset */
#define SERCOM_I2CS_INTENSET_REG_OFST  (0x0D)              /**< (SERCOM_I2CS_INTENSET) I2CS Interrupt Enable Set Offset */
#define SERCOM_SPIM_INTENSET_REG_OFST  (0x0D)              /**< (SERCOM_SPIM_INTENSET) SPIM Interrupt Enable Set Offset */
#define SERCOM_SPIS_INTENSET_REG_OFST  (0x0D)              /**< (SERCOM_SPIS_INTENSET) SPIS Interrupt Enable Set Offset */
#define SERCOM_USART_EXT_INTENSET_REG_OFST (0x0D)              /**< (SERCOM_USART_EXT_INTENSET) USART_EXT Interrupt Enable Set Offset */
#define SERCOM_USART_INT_INTENSET_REG_OFST (0x0D)              /**< (SERCOM_USART_INT_INTENSET) USART_INT Interrupt Enable Set Offset */
#define SERCOM_I2CM_INTFLAG_REG_OFST   (0x0E)              /**< (SERCOM_I2CM_INTFLAG) I2CM Interrupt Flag Status and Clear Offset */
#define SERCOM_I2CS_INTFLAG_REG_OFST   (0x0E)              /**< (SERCOM_I2CS_INTFLAG) I2CS Interrupt Flag Status and Clear Offset */
#define SERCOM_SPIM_INTFLAG_REG_OFST   (0x0E)              /**< (SERCOM_SPIM_INTFLAG) SPIM Interrupt Flag Status and Clear Offset */
#define SERCOM_SPIS_INTFLAG_REG_OFST   (0x0E)              /**< (SERCOM_SPIS_INTFLAG) SPIS Interrupt Flag Status and Clear Offset */
#define SERCOM_USART_EXT_INTFLAG_REG_OFST (0x0E)              /**< (SERCOM_USART_EXT_INTFLAG) USART_EXT Interrupt Flag Status and Clear Offset */
#define SERCOM_USART_INT_INTFLAG_REG_OFST (0x0E)              /**< (SERCOM_USART_INT_INTFLAG) USART_INT Interrupt Flag Status and Clear Offset */
#define SERCOM_I2CM_STATUS_REG_OFST    (0x10)              /**< (SERCOM_I2CM_STATUS) I2CM Status Offset */
#define SERCOM_I2CS_STATUS_REG_OFST    (0x10)              /**< (SERCOM_I2CS_STATUS) I2CS Status Offset */
#define SERCOM_SPIM_STATUS_REG_OFST    (0x10)              /**< (SERCOM_SPIM_STATUS) SPIM Status Offset */
#define SERCOM_SPIS_STATUS_REG_OFST    (0x10)              /**< (SERCOM_SPIS_STATUS) SPIS Status Offset */
#define SERCOM_USART_EXT_STATUS_REG_OFST (0x10)              /**< (SERCOM_USART_EXT_STATUS) USART_EXT Status Offset */
#define SERCOM_USART_INT_STATUS_REG_OFST (0x10)              /**< (SERCOM_USART_INT_STATUS) USART_INT Status Offset */
#define SERCOM_I2CM_ADDR_REG_OFST      (0x14)              /**< (SERCOM_I2CM_ADDR) I2CM Address Offset */
#define SERCOM_I2CS_ADDR_REG_OFST      (0x14)              /**< (SERCOM_I2CS_ADDR) I2CS Address Offset */
#define SERCOM_SPIM_ADDR_REG_OFST      (0x14)              /**< (SERCOM_SPIM_ADDR) SPIM Address Offset */
#define SERCOM_SPIS_ADDR_REG_OFST      (0x14)              /**< (SERCOM_SPIS_ADDR) SPIS Address Offset */
#define SERCOM_I2CM_DATA_REG_OFST      (0x18)              /**< (SERCOM_I2CM_DATA) I2CM Data Offset */
#define SERCOM_I2CS_DATA_REG_OFST      (0x18)              /**< (SERCOM_I2CS_DATA) I2CS Data Offset */
#define SERCOM_SPIM_DATA_REG_OFST      (0x18)              /**< (SERCOM_SPIM_DATA) SPIM Data Offset */
#define SERCOM_SPIS_DATA_REG_OFST      (0x18)              /**< (SERCOM_SPIS_DATA) SPIS Data Offset */
#define SERCOM_USART_EXT_DATA_REG_OFST (0x18)              /**< (SERCOM_USART_EXT_DATA) USART_EXT Data Offset */
#define SERCOM_USART_INT_DATA_REG_OFST (0x18)              /**< (SERCOM_USART_INT_DATA) USART_INT Data Offset */

#if !(defined(__ASSEMBLER__) || defined(__IAR_SYSTEMS_ASM__))
/** \brief SERCOM register API structure */
typedef struct
{  /* Serial Communication Interface */
  __IO  uint32_t                       SERCOM_CTRLA;       /**< Offset: 0x00 (R/W  32) I2CM Control A */
  __IO  uint32_t                       SERCOM_CTRLB;       /**< Offset: 0x04 (R/W  32) I2CM Control B */
  __IO  uint8_t                        SERCOM_DBGCTRL;     /**< Offset: 0x08 (R/W  8) I2CM Debug Control */
  __I   uint8_t                        Reserved1[0x01];
  __IO  uint16_t                       SERCOM_BAUD;        /**< Offset: 0x0A (R/W  16) I2CM Baud Rate */
  __IO  uint8_t                        SERCOM_INTENCLR;    /**< Offset: 0x0C (R/W  8) I2CM Interrupt Enable Clear */
  __IO  uint8_t                        SERCOM_INTENSET;    /**< Offset: 0x0D (R/W  8) I2CM Interrupt Enable Set */
  __IO  uint8_t                        SERCOM_INTFLAG;     /**< Offset: 0x0E (R/W  8) I2CM Interrupt Flag Status and Clear */
  __I   uint8_t                        Reserved2[0x01];
  __IO  uint16_t                       SERCOM_STATUS;      /**< Offset: 0x10 (R/W  16) I2CM Status */
  __I   uint8_t                        Reserved3[0x02];
  __IO  uint8_t                        SERCOM_ADDR;        /**< Offset: 0x14 (R/W  8) I2CM Address */
  __I   uint8_t                        Reserved4[0x03];
  __IO  uint8_t                        SERCOM_DATA;        /**< Offset: 0x18 (R/W  8) I2CM Data */
} sercom_i2cm_registers_t;

/** \brief SERCOM register API structure */
typedef struct
{  /* Serial Communication Interface */
  __IO  uint32_t                       SERCOM_CTRLA;       /**< Offset: 0x00 (R/W  32) I2CS Control A */
  __IO  uint32_t                       SERCOM_CTRLB;       /**< Offset: 0x04 (R/W  32) I2CS Control B */
  __I   uint8_t                        Reserved1[0x04];
  __IO  uint8_t                        SERCOM_INTENCLR;    /**< Offset: 0x0C (R/W  8) I2CS Interrupt Enable Clear */
  __IO  uint8_t                        SERCOM_INTENSET;    /**< Offset: 0x0D (R/W  8) I2CS Interrupt Enable Set */
  __IO  uint8_t                        SERCOM_INTFLAG;     /**< Offset: 0x0E (R/W  8) I2CS Interrupt Flag Status and Clear */
  __I   uint8_t                        Reserved2[0x01];
  __IO  uint16_t                       SERCOM_STATUS;      /**< Offset: 0x10 (R/W  16) I2CS Status */
  __I   uint8_t                        Reserved3[0x02];
  __IO  uint32_t                       SERCOM_ADDR;        /**< Offset: 0x14 (R/W  32) I2CS Address */
  __IO  uint8_t                        SERCOM_DATA;        /**< Offset: 0x18 (R/W  8) I2CS Data */
} sercom_i2cs_registers_t;

/** \brief SERCOM register API structure */
typedef struct
{  /* Serial Communication Interface */
  __IO  uint32_t                       SERCOM_CTRLA;       /**< Offset: 0x00 (R/W  32) SPIS Control A */
  __IO  uint32_t                       SERCOM_CTRLB;       /**< Offset: 0x04 (R/W  32) SPIS Control B */
  __IO  uint8_t                        SERCOM_DBGCTRL;     /**< Offset: 0x08 (R/W  8) SPIS Debug Control */
  __I   uint8_t                        Reserved1[0x01];
  __IO  uint8_t                        SERCOM_BAUD;        /**< Offset: 0x0A (R/W  8) SPIS Baud Rate */
  __I   uint8_t                        Reserved2[0x01];
  __IO  uint8_t                        SERCOM_INTENCLR;    /**< Offset: 0x0C (R/W  8) SPIS Interrupt Enable Clear */
  __IO  uint8_t                        SERCOM_INTENSET;    /**< Offset: 0x0D (R/W  8) SPIS Interrupt Enable Set */
  __IO  uint8_t                        SERCOM_INTFLAG;     /**< Offset: 0x0E (R/W  8) SPIS Interrupt Flag Status and Clear */
  __I   uint8_t                        Reserved3[0x01];
  __IO  uint16_t                       SERCOM_STATUS;      /**< Offset: 0x10 (R/W  16) SPIS Status */
  __I   uint8_t                        Reserved4[0x02];
  __IO  uint32_t                       SERCOM_ADDR;        /**< Offset: 0x14 (R/W  32) SPIS Address */
  __IO  uint16_t                       SERCOM_DATA;        /**< Offset: 0x18 (R/W  16) SPIS Data */
} sercom_spis_registers_t;

/** \brief SERCOM register API structure */
typedef struct
{  /* Serial Communication Interface */
  __IO  uint32_t                       SERCOM_CTRLA;       /**< Offset: 0x00 (R/W  32) SPIM Control A */
  __IO  uint32_t                       SERCOM_CTRLB;       /**< Offset: 0x04 (R/W  32) SPIM Control B */
  __IO  uint8_t                        SERCOM_DBGCTRL;     /**< Offset: 0x08 (R/W  8) SPIM Debug Control */
  __I   uint8_t                        Reserved1[0x01];
  __IO  uint8_t                        SERCOM_BAUD;        /**< Offset: 0x0A (R/W  8) SPIM Baud Rate */
  __I   uint8_t                        Reserved2[0x01];
  __IO  uint8_t                        SERCOM_INTENCLR;    /**< Offset: 0x0C (R/W  8) SPIM Interrupt Enable Clear */
  __IO  uint8_t                        SERCOM_INTENSET;    /**< Offset: 0x0D (R/W  8) SPIM Interrupt Enable Set */
  __IO  uint8_t                        SERCOM_INTFLAG;     /**< Offset: 0x0E (R/W  8) SPIM Interrupt Flag Status and Clear */
  __I   uint8_t                        Reserved3[0x01];
  __IO  uint16_t                       SERCOM_STATUS;      /**< Offset: 0x10 (R/W  16) SPIM Status */
  __I   uint8_t                        Reserved4[0x02];
  __IO  uint32_t                       SERCOM_ADDR;        /**< Offset: 0x14 (R/W  32) SPIM Address */
  __IO  uint16_t                       SERCOM_DATA;        /**< Offset: 0x18 (R/W  16) SPIM Data */
} sercom_spim_registers_t;

/** \brief SERCOM register API structure */
typedef struct
{  /* Serial Communication Interface */
  __IO  uint32_t                       SERCOM_CTRLA;       /**< Offset: 0x00 (R/W  32) USART_EXT Control A */
  __IO  uint32_t                       SERCOM_CTRLB;       /**< Offset: 0x04 (R/W  32) USART_EXT Control B */
  __IO  uint8_t                        SERCOM_DBGCTRL;     /**< Offset: 0x08 (R/W  8) USART_EXT Debug Control */
  __I   uint8_t                        Reserved1[0x01];
  __IO  uint16_t                       SERCOM_BAUD;        /**< Offset: 0x0A (R/W  16) USART_EXT Baud */
  __IO  uint8_t                        SERCOM_INTENCLR;    /**< Offset: 0x0C (R/W  8) USART_EXT Interrupt Enable Clear */
  __IO  uint8_t                        SERCOM_INTENSET;    /**< Offset: 0x0D (R/W  8) USART_EXT Interrupt Enable Set */
  __IO  uint8_t                        SERCOM_INTFLAG;     /**< Offset: 0x0E (R/W  8) USART_EXT Interrupt Flag Status and Clear */
  __I   uint8_t                        Reserved2[0x01];
  __IO  uint16_t                       SERCOM_STATUS;      /**< Offset: 0x10 (R/W  16) USART_EXT Status */
  __I   uint8_t                        Reserved3[0x06];
  __IO  uint16_t                       SERCOM_DATA;        /**< Offset: 0x18 (R/W  16) USART_EXT Data */
} sercom_usart_ext_registers_t;

/** \brief SERCOM register API structure */
typedef struct
{  /* Serial Communication Interface */
  __IO  uint32_t                       SERCOM_CTRLA;       /**< Offset: 0x00 (R/W  32) USART_INT Control A */
  __IO  uint32_t                       SERCOM_CTRLB;       /**< Offset: 0x04 (R/W  32) USART_INT Control B */
  __IO  uint8_t                        SERCOM_DBGCTRL;     /**< Offset: 0x08 (R/W  8) USART_INT Debug Control */
  __I   uint8_t                        Reserved1[0x01];
  __IO  uint16_t                       SERCOM_BAUD;        /**< Offset: 0x0A (R/W  16) USART_INT Baud */
  __IO  uint8_t                        SERCOM_INTENCLR;    /**< Offset: 0x0C (R/W  8) USART_INT Interrupt Enable Clear */
  __IO  uint8_t                        SERCOM_INTENSET;    /**< Offset: 0x0D (R/W  8) USART_INT Interrupt Enable Set */
  __IO  uint8_t                        SERCOM_INTFLAG;     /**< Offset: 0x0E (R/W  8) USART_INT Interrupt Flag Status and Clear */
  __I   uint8_t                        Reserved2[0x01];
  __IO  uint16_t                       SERCOM_STATUS;      /**< Offset: 0x10 (R/W  16) USART_INT Status */
  __I   uint8_t                        Reserved3[0x06];
  __IO  uint16_t                       SERCOM_DATA;        /**< Offset: 0x18 (R/W  16) USART_INT Data */
} sercom_usart_int_registers_t;

/** \brief SERCOM hardware registers */
typedef union
{  /* Serial Communication Interface */
       sercom_i2cm_registers_t        I2CM;           /**< I2C Master Mode */
       sercom_i2cs_registers_t        I2CS;           /**< I2C Slave Mode */
       sercom_spis_registers_t        SPIS;           /**< SPI Slave Mode */
       sercom_spim_registers_t        SPIM;           /**< SPI Master Mode */
       sercom_usart_ext_registers_t   USART_EXT;      /**< USART EXTERNAL CLOCK Mode */
       sercom_usart_int_registers_t   USART_INT;      /**< USART INTERNAL CLOCK Mode */
} sercom_registers_t;


#endif /* !(defined(__ASSEMBLER__) || defined(__IAR_SYSTEMS_ASM__)) */
#endif /* _SAMD20_SERCOM_COMPONENT_H_ */
