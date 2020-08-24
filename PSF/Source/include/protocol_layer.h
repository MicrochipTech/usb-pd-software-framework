/*******************************************************************************
Protocol Layer Header

Company:
    Microchip Technology Inc.

File Name:
    protocol_layer.h

Description:
  This header file contains the data structures, constant definition,
  function prototypes and definitions for Protocol Layer.
*******************************************************************************/
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
********************************************************************************/
#ifndef _PROTOCOL_LAYER_H_
#define _PROTOCOL_LAYER_H_

// *****************************************************************************
// *****************************************************************************
// Section: Constants
// *****************************************************************************
// *****************************************************************************

/***************************************************************************************************
								UPD_MAC & BASEBAND REG BASE
***************************************************************************************************/

/* Register Base Address */

#define PRL_PDMAC_REG_BASE								0x1800
#define PRL_BASEBAND_REG_BASE							0x2800

/***************************************************************************************************
                              PD_MAC Tx Registers configuration
***************************************************************************************************/

/* Tx Registers */

#define PRL_TX_CTL_A                         			(PRL_PDMAC_REG_BASE + 0x200)
#define PRL_TX_STAT                          			(PRL_PDMAC_REG_BASE + 0x201)
#define PRL_TX_PARAM_C                       			(PRL_PDMAC_REG_BASE + 0x202)
#define PRL_TX_PKT_LEN                       			(PRL_PDMAC_REG_BASE + 0x203)
#define PRL_TX_PARAM_A                       			(PRL_PDMAC_REG_BASE + 0x204)
#define PRL_TX_CTL_B                         			(PRL_PDMAC_REG_BASE + 0x205)
#define PRL_TX_PARAM_B                       			(PRL_PDMAC_REG_BASE + 0x206)
#define PRL_TX_BITTIME_CNT                   			(PRL_PDMAC_REG_BASE + 0x207)
#define PRL_TX_TA_TIME                       			(PRL_PDMAC_REG_BASE + 0x208)
#define PRL_TX_ABORT_STAT                    			(PRL_PDMAC_REG_BASE + 0x209)
#define PRL_TX_AR_ABORT_STAT                 			(PRL_PDMAC_REG_BASE + 0x20A)
#define PRL_TX_POWER_UP_TIME                 			(PRL_PDMAC_REG_BASE + 0x20B)
#define PRL_TX_POWER_DOWN_TIME               			(PRL_PDMAC_REG_BASE + 0x20C)


/* TX_CTL_A bit definitions */
#define PRL_TX_CTL_A_RETRY_ON_LINE_BUSY             	BIT(6)
#define PRL_TX_CTL_A_DIS_SPCL_SR_GCRC_ACK           	BIT(4)
#define PRL_TX_CTL_A_WAIT4LINE_IDLE                 	BIT(3)
#define PRL_TX_CTL_A_EN_AUTO_RSP_MODE               	BIT(2)
#define PRL_TX_CTL_A_EN_FMQ                         	BIT(1)
#define PRL_TX_CTL_A_RMDP                           	BIT(0)

/* TX_PARAM_A bit definitions */
#define PRL_TX_PARAM_A_EXPECT_GOODCRC               	BIT(7)
#define PRL_TX_PARAM_A_EN_FW_TX                     	BIT(3)
#define PRL_TX_PARAM_A_MSG_ID                       	BIT(0)

/* TX_SOP_SELECT BIT[6:4]*/
#define PRL_TX_PARAM_A_TX_SOP_SELECT_SOP            	0x0
#define PRL_TX_PARAM_A_TX_SOP_SELECT_SOP_P          	0x10
#define PRL_TX_PARAM_A_TX_SOP_SELECT_SOP_PP         	0x20
#define PRL_TX_PARAM_A_TX_SOP_SELECT_SOP_P_DEBUG    	0x30
#define PRL_TX_PARAM_A_TX_SOP_SELECT_SOP_PP_DEBUG   	0x40
#define PRL_TX_PARAM_A_TX_SOP_SELECT_MASK				(BIT(4) | BIT(5) | BIT(6))

/* TX_CTL_B bit definitions */
#define PRL_TX_CTL_B_TX_CABLE_RESET                 	BIT(5)
#define PRL_TX_CTL_B_OK_TO_TX                       	BIT(4)
#define PRL_TX_CTL_B_RST_TXQ_FIFO_WRI_PTR           	BIT(3)
#define PRL_TX_CTL_B_TX_HARD_RESET                  	BIT(2)
#define PRL_TX_CTL_B_ABORT                          	BIT(1)
#define PRL_TX_CTL_B_GO                             	BIT(0)

/* TX_PARAM_C bit & field definitions */

/* UPD_SPEC_REV[1:0] - 0x00 Version 1.0, 0x01 Version 2.0*/
#define PRL_TX_PARAM_C_UPD_SPEC_REV_1_0					0x0
#define PRL_TX_PARAM_C_UPD_SPEC_REV_2_0					BIT(0)

/* PORT_POWER_ROLE (BIT 2) - 1'b0-Sink & 1'b1-Source*/
#define PRL_TX_PARAM_C_PORT_POWER_ROLE_SOURCE          		BIT(2)
#define PRL_TX_PARAM_C_PORT_POWER_ROLE_BIT_POS				2

/* define to update Port Power Role in Tx_PARAM_C*/
#define PRL_UPDATE_TX_PARAM_C_PORT_POWER_ROLE(u8PowerRole) (u8PowerRole << \
															PRL_TX_PARAM_C_PORT_POWER_ROLE_BIT_POS)

/* CABLE_PLUG (BIT7) -1'b0-DFP or UFP & 1'b1-Cable Plug*/
#define PRL_TX_PARAM_C_CABLE_PLUG_CABLE                 	BIT(3)
#define PRL_TX_PARAM_C_CABLE_PLUG_CABLE_BIT_POS				3

/* N_RETRY_CNT (BIT [6:4]) */
#define PRL_TX_PARAM_C_N_RETRY_CNT_BIT_POS					4
#define PRL_TX_PARAM_C_N_RETRY_CNT_FIELD_MASK				0x70

/* define to update nHWRetryCount in Tx_PARAM_C*/
#define PRL_UPDATE_TX_PARAM_C_N_RETRY_CNT(u8RetryCnt)		(u8RetryCnt	<< \
															PRL_TX_PARAM_C_N_RETRY_CNT_BIT_POS)

/* PORT_DATA_ROLE (BIT 7) - 1'b0-UFP & 1'b1-DFP*/
#define PRL_TX_PARAM_C_PORT_DATA_ROLE_DFP					BIT(7)
#define PRL_TX_PARAM_C_PORT_DATA_ROLE_BIT_POS				7

/* define to update Port Data Role in Tx_PARAM_C*/
#define PRL_UPDATE_TX_PARAM_C_PORT_DATA_ROLE(u8DataRole)	(u8DataRole << \
															PRL_TX_PARAM_C_PORT_DATA_ROLE_BIT_POS)

/**************************************************************************************************/
/***************************************************************************************************
                              PRL Rx Register configuration
***************************************************************************************************/
#define PRL_RX_CTL_A                                 	(PRL_PDMAC_REG_BASE + 0x240)
#define PRL_RX_CTL_B									(PRL_PDMAC_REG_BASE + 0x241)
#define PRL_RX_BIT_PER_CNT_MAX_BR						(PRL_PDMAC_REG_BASE + 0x242)
#define PRL_RX_BIT_PER_CNT_MIN_BR                    	(PRL_PDMAC_REG_BASE + 0x243)
#define PRL_RX_STAT                                  	(PRL_PDMAC_REG_BASE + 0x244)
#define PRL_RX_PKT_STAT                              	(PRL_PDMAC_REG_BASE + 0x245)
#define PRL_RX_TRECEIVE_TIME                         	(PRL_PDMAC_REG_BASE + 0x246)
#define PRL_RX_BADCRC_PKT_CNT                        	(PRL_PDMAC_REG_BASE + 0x247)
#define PRL_RX_DUP_PKT_CNT                           	(PRL_PDMAC_REG_BASE + 0x248)
#define PRL_RX_HR_DET_WINDOW                         	(PRL_PDMAC_REG_BASE + 0x249)
#define PRL_RX_LAST_GCRC_PKT_HI                      	(PRL_PDMAC_REG_BASE + 0x24A)
#define PRL_RX_LAST_GCRC_PKT_LO                      	(PRL_PDMAC_REG_BASE + 0x24B)
#define PRL_RX_MSG_ID_STORED                         	(PRL_PDMAC_REG_BASE + 0x24C)
#define PRL_RX_MAX_SIZE                              	(PRL_PDMAC_REG_BASE + 0x24D)
#define PRL_RX_FIFO_RD_PTR                           	(PRL_PDMAC_REG_BASE + 0x250)
#define PRL_RX_FIFO_RD_PTR_CTL_BITS                  	(PRL_PDMAC_REG_BASE + 0x251)
#define PRL_RX_FIFO_WR_CURRENT_PTR                   	(PRL_PDMAC_REG_BASE + 0x252)
#define PRL_RX_FIFO_WR_CURRENT_PTR_CTL_BITS          	(PRL_PDMAC_REG_BASE + 0x253)
#define PRL_RX_FIFO_WR_HEAD_PTR                      	(PRL_PDMAC_REG_BASE + 0x254)
#define PRL_RX_FIFO_WR_HEAD_PTR_CTL_BITS             	(PRL_PDMAC_REG_BASE + 0x255)
#define PRL_RX_FIFO_WR_HEAD_PTR_PLUS_ONE             	(PRL_PDMAC_REG_BASE + 0x256)
#define PRL_RX_FIFO_WR_HEAD_PTR_PLUS_ONE_CTL_BITS    	(PRL_PDMAC_REG_BASE + 0x257)
#define PRL_RX_HR_DET_MINBITS                        	(PRL_PDMAC_REG_BASE + 0x258)
#define PRL_RX_DET_PREBITS                           	(PRL_PDMAC_REG_BASE + 0x259)

/* RX_CTL_A bit definitions */
#define PRL_RX_CTL_A_RTERM_MODE								BIT(7)
#define PRL_RX_CTL_A_EN_RTERM                               BIT(6)
#define PRL_RX_CTL_A_EN_LOOPBACK                            BIT(5)
#define PRL_RX_CTL_A_EN_RCV                                 BIT(4)
#define PRL_RX_CTL_A_RST_RECEIVER                           BIT(3)
#define PRL_RX_CTL_A_EN_CRC_RCV_TMR                         BIT(2)
#define PRL_RX_CTL_A_EN_SMBUS_MODE                          BIT(1)
#define PRL_RX_CTL_A_EN_CABLE_RESET                         BIT(0)

/* RX_CTL_B bit definitions*/
#define PRL_RX_CTL_B_PD3_ENABLE								BIT(7)
#define PRL_RX_CTL_B_RX_SOP_ENABLE_SOP_PP_DEBUG				BIT(6)
#define PRL_RX_CTL_B_RX_SOP_ENABLE_SOP_P_DEBUG				BIT(5)
#define PRL_RX_CTL_B_RX_SOP_ENABLE_SOP_PP					BIT(4)
#define PRL_RX_CTL_B_RX_SOP_ENABLE_SOP_P					BIT(3)
#define PRL_RX_CTL_B_RX_SOP_ENABLE_SOP						BIT(2)
#define PRL_RX_CTL_B_PD3_AUTO_DECODE						BIT(1)

/* RX_MSG_ID_STORED bit definitions */
#define PRL_RX_MSG_ID_STORED_SOP                            BIT(0)
#define PRL_RX_MSG_ID_STORED_SOP_P                          BIT(1)
#define PRL_RX_MSG_ID_STORED_SOP_PP                         BIT(2)
#define PRL_RX_MSG_ID_STORED_SOP_P_DEBUG                    BIT(3)
#define PRL_RX_MSG_ID_STORED_SOP_PP_DEBUG                   BIT(4)

/* RXQ Status Field */
#define PRL_RXQ_STAT_RX_SOP_TYPE_MASK 						(BIT(4) | BIT(5) | BIT(6))
#define PRL_RXQ_STAT_RX_SOP_TYPE_FIELD_START_BIT			4

/* define to get PRL_SOP_TYPE type from Rx Status */
#define PRL_GET_SOP_TYPE_FROM_RX_STATUS(u8RxSTATUS)	\
	((u8RxSTATUS & PRL_RXQ_STAT_RX_SOP_TYPE_MASK) >> PRL_RXQ_STAT_RX_SOP_TYPE_FIELD_START_BIT)
/**************************************************************************************************/
/***************************************************************************************************
                              PD_MAC Interrupt Register Configuration
***************************************************************************************************/

/* MAC Interrupt Status Registers */
#define PRL_MAC_IRQ_STAT                             		(PRL_PDMAC_REG_BASE + 0x280)
#define PRL_TX_IRQ_STAT                              		(PRL_PDMAC_REG_BASE + 0x281)
#define PRL_RX_IRQ_STAT                              		(PRL_PDMAC_REG_BASE + 0x282)
#define PRL_RX_ERR_IRQ_STAT                          		(PRL_PDMAC_REG_BASE + 0x283)
#define PRL_PM_IRQ_STAT                              		(PRL_PDMAC_REG_BASE + 0x284)
#define PRL_RX_SOP_IRQ_STAT                          		(PRL_PDMAC_REG_BASE + 0x285)

/* MAC Interrupt Enable Register */
#define PRL_TX_IRQ_EN                                		(PRL_PDMAC_REG_BASE + 0x286)
#define PRL_RX_IRQ_EN                                		(PRL_PDMAC_REG_BASE + 0x287)
#define PRL_RX_ERR_IRQ_EN                            		(PRL_PDMAC_REG_BASE + 0x288)
#define PRL_PM_IRQ_EN                                		(PRL_PDMAC_REG_BASE + 0x289)
#define PRL_RX_SOP_IRQ_EN                            		(PRL_PDMAC_REG_BASE + 0x28A)

/* MAC_IRQ_STAT field bit definitions */
#define PRL_MAC_IRQ_RX_SOP                                  	BIT(4)
#define PRL_MAC_IRQ_PM                                      	BIT(3)
#define PRL_MAC_IRQ_RX_ERR                                  	BIT(2)
#define PRL_MAC_IRQ_RX                                      	BIT(1)
#define PRL_MAC_IRQ_TX                                      	BIT(0)

/* Tx_IRQ_STAT bit fields */
#define PRL_TX_IRQ_AUTO_RSP_ABORTED                        		BIT(7)
#define PRL_TX_IRQ_AUTO_RSP_SENT                           		BIT(6)
#define PRL_TX_IRQ_OK_TO_TX                                		BIT(5)
#define PRL_TX_IRQ_CRC_RCV_TIMEOUT                         		BIT(4)
#define PRL_TX_IRQ_TX_EOP                                  		BIT(3)
#define PRL_TX_IRQ_TX_ABORTED                              		BIT(2)
#define PRL_TX_IRQ_TX_FAILED                               		BIT(1)
#define PRL_TX_IRQ_TX_DONE                                 		BIT(0)

/* Rx_IRQ_STAT bit field */
#define PRL_RX_IRQ_RX_FIFO_NOT_EMPTY                       		BIT(7)
#define PRL_RX_IRQ_LINE_WENT_IDLE                          		BIT(6)
#define PRL_RX_IRQ_CABLE_RST                               		BIT(5)
#define PRL_RX_IRQ_RX_DB_BS_DET                            		BIT(4)
#define PRL_RX_IRQ_RX_HARD_RST                             		BIT(3)
#define PRL_RX_IRQ_RX_EOP                                  		BIT(2)
#define PRL_RX_IRQ_RX_SOP                                  		BIT(1)
#define PRL_RX_IRQ_RX_DONE                                 		BIT(0)

/* PM_IRQ_STAT bit field definitions */
#define PRL_PM_IRQ_UPD_CLK_STOP_EN                         		BIT(3)
#define PRL_PM_IRQ_UPD_WU_EN                               		BIT(2)
#define PRL_PM_IRQ_DB_WFPS_TIMER_EXP_WS                    		BIT(1)
#define PRL_PM_IRQ_UPD_ACT_WS                              		BIT(0)

/**************************************************************************************************/
/***************************************************************************************************
                              PD_MAC Reset Register Configuration
***************************************************************************************************/

/* PD_MAC Reset Control */
#define PRL_RESET_CTL                                		(PRL_PDMAC_REG_BASE + 0x28B)

/* RESET_CTL BIT field definitions */
#define PRL_RESET_CTL_PD_RESET                              BIT(0)
#define PRL_RESET_CTL_SW_RESET                              BIT(7)

/**************************************************************************************************/
/***************************************************************************************************
						PD_MAC BMC Encoder and Decoder Register Configuration
***************************************************************************************************/
/* BMC Encoder/Decoder Registers*/
#define PRL_BMC_RX_HI_FB_MAX_TIME                    (PRL_PDMAC_REG_BASE + 0x2A0)
#define PRL_BMC_RX_HI_FB_MIN_TIME                    (PRL_PDMAC_REG_BASE + 0x2A1)
#define PRL_BMC_RX_LO_FB_MAX_TIME                    (PRL_PDMAC_REG_BASE + 0x2A2)
#define PRL_BMC_RX_LO_FB_MIN_TIME                    (PRL_PDMAC_REG_BASE + 0x2A3)
#define PRL_BMC_RX_HI_HB_MAX_TIME                    (PRL_PDMAC_REG_BASE + 0x2A4)
#define PRL_BMC_RX_HI_HB_MIN_TIME                    (PRL_PDMAC_REG_BASE + 0x2A5)
#define PRL_BMC_RX_LO_HB_MAX_TIME                    (PRL_PDMAC_REG_BASE + 0x2A6)
#define PRL_BMC_RX_LO_HB_MIN_TIME                    (PRL_PDMAC_REG_BASE + 0x2A7)
#define PRL_BMC_RX_SQL_ASSERT_TIME                   (PRL_PDMAC_REG_BASE + 0x2A8)
#define PRL_BMC_RX_SQL_HOLD_TIME                     (PRL_PDMAC_REG_BASE + 0x2A9)
#define PRL_BMC_TX_BITTIME_CNT                       (PRL_PDMAC_REG_BASE + 0x2B0)
#define PRL_BMC_TRANSITION_WINDOW_TIME               (PRL_PDMAC_REG_BASE + 0x2B1)

/**************************************************************************************************/
/***************************************************************************************************
                              PD_MAC BIST Control Register Configuration
***************************************************************************************************/

/* BIST Control Register */
#define PRL_BIST_CTL_A                               (PRL_PDMAC_REG_BASE + 0x2C0)
#define PRL_BIST_CTL_B                               (PRL_PDMAC_REG_BASE + 0x2C1)
#define PRL_BIST_ERR_CNT_HI                          (PRL_PDMAC_REG_BASE + 0x2C2)
#define PRL_BIST_ERR_CNT_LO                          (PRL_PDMAC_REG_BASE + 0x2C3)
#define PRL_DB_WFPS_TMR_CNT                          (PRL_PDMAC_REG_BASE + 0x2C4)
#define PRL_DB_BS_BIT_CNT                            (PRL_PDMAC_REG_BASE + 0x2C5)
#define PRL_DB_FUNC_CTL                              (PRL_PDMAC_REG_BASE + 0x2C6)
#define PRL_BIST_RX_STAT                             (PRL_PDMAC_REG_BASE + 0x2C7)
#define PRL_DB_TX_BIT_TIME_CNT                       (PRL_PDMAC_REG_BASE + 0x2C8)
/**************************************************************************************************/
/***************************************************************************************************
                              PD_MAC Tx and Rx FIFO Register Configuration
***************************************************************************************************/
/* Tx and Rx Buffer Address */
#define PRL_PDMAC_TRANSMITTER_BUFF_ADDR				(PRL_PDMAC_REG_BASE + 0x0000)
#define PRL_PDMAC_RECEIVER_BUFF_ADDR  				(PRL_PDMAC_REG_BASE + 0x0100)

/**************************************************************************************************/
/**************************************************************************************************/
/**************************************************************************************************/

/* Tx time configurations */
/* Values are interrupted from Cronus stack */
#define PRL_CLK_FREQ_KHZ                             48000
#define PRL_TX_BITRATE_KBPS                          300
#define PRL_TX_POWER_UP_TIME_REG_VALUE 				60
#define PRL_TX_POWER_DOWN_TIME_REG_VALUE            30
#define PRL_TX_TA_TIME_REG_VALUE         			50
#define PRL_TX_BITTIME_CNT_REG_VALUE         		((PRL_CLK_FREQ_KHZ / PRL_TX_BITRATE_KBPS) - 1)


/* Rx time configurations */
#define PRL_RX_BIT_PER_CNT_MAX_BR_TIME_VALUE                128
#define PRL_RX_BIT_PER_CNT_MIN_BR_TIME_VALUE                196
/* by PD_SPEC, value this must be 900 - 1100 us */
#define PRL_RX_TRECEIVE_TIME_VALUE_US                       1000
#define PRL_RX_TRECEIVE_TIME_VALUE_IN_10US_UNITS			(PRL_RX_TRECEIVE_TIME_VALUE_US / 10)
/* To avoid false HR/CBR detection in the case of no PRL_SOP_TYPE. The number of bits inclusive of Preamble */
#define PRL_RX_HR_DET_WINDOW_BITS_VALUE                     94
#define PRL_HB_MIN_TIME_VALUE                               30
#define PRL_HB_MAX_TIME_VALUE                               114
#define PRL_FB_MIN_TIME_VALUE                               (PRL_HB_MAX_TIME_VALUE + 1)
#define PRL_FB_MAX_TIME_VALUE                               206
#define PRL_BMC_RX_SQL_ASSERT_TIME_VALUE                    101
/**************************************************************************************************/

/* Reg values to be configured for BMC Encoder Decoder */
#define PRL_BMC_RX_HI_FB_MAX_TIME_REG_VALUE					PRL_FB_MAX_TIME_VALUE
#define PRL_BMC_RX_HI_FB_MIN_TIME_REG_VALUE					PRL_FB_MIN_TIME_VALUE
#define PRL_BMC_RX_LO_FB_MAX_TIME_REG_VALUE					PRL_FB_MAX_TIME_VALUE
#define PRL_BMC_RX_LO_FB_MIN_TIME_REG_VALUE					PRL_FB_MIN_TIME_VALUE
#define PRL_BMC_RX_HI_HB_MAX_TIME_REG_VALUE					PRL_HB_MAX_TIME_VALUE
#define PRL_BMC_RX_HI_HB_MIN_TIME_REG_VALUE					PRL_HB_MIN_TIME_VALUE
#define PRL_BMC_RX_LO_HB_MAX_TIME_REG_VALUE					PRL_HB_MAX_TIME_VALUE
#define PRL_BMC_RX_LO_HB_MIN_TIME_REG_VALUE					PRL_HB_MIN_TIME_VALUE

#define PRL_BMC_RX_SQL_ASSERT_TIME_REG_VALUE				PRL_BMC_RX_SQL_ASSERT_TIME_VALUE
#define PRL_BMC_RX_SQL_HOLD_TIME_REG_VALUE					23
#define PRL_BMC_TX_BITTIME_CNT_REG_VALUE					159
#define PRL_BMC_TRANSITION_WINDOW_TIME_REG_VALUE			200
/***************************************************************************************************
                UPD Baseband Interface Control and Status Regsiter
***************************************************************************************************/
/* CC DAC Control Register */
#define PRL_BB_CC_RX_DAC_CTL                 				(PRL_BASEBAND_REG_BASE + 0x00)
#define PRL_BB_CC_RX_DAC_FILT                				(PRL_BASEBAND_REG_BASE + 0x02)
#define PRL_BB_CC_TX_DAC_FILT                				(PRL_BASEBAND_REG_BASE + 0x04)

/* CC_RX_DAC_CTL bit field definition */
#define PRL_CC_RX_DAC_CTL_RX_DAC_ENABLE                     BIT(15)
#define PRL_CC_RX_DAC_CTL_CC_RX_PRL_DAC_VALUE_BIT_POS      	BIT(0)

/* CC_RX_DAC_FILT bit field definition */
#define PRL_CC_RX_DAC_FILT_SECLECT_CC_RX_FILTER_CONF        BIT(1)
#define PRL_CC_RX_DAC_FILT_CC_RX_DAC_FILTER_ENABLE          BIT(0)

/* CC_TX_DAC_FILT */
#define PRL_CC_TX_DAC_FILT_CC_TX_FILTER_ENABLE              BIT(4)
#define PRL_CC_TX_DAC_FILT_CC_TX_FILTER_BIT_POS             BIT(0)

#define PRL_BB_TX_RISE0                      				(PRL_BASEBAND_REG_BASE + 0x10)
#define PRL_BB_TX_RISE1                      				(PRL_BASEBAND_REG_BASE + 0x12)
#define PRL_BB_TX_RISE2                      				(PRL_BASEBAND_REG_BASE + 0x14)
#define PRL_BB_TX_RISE3                      				(PRL_BASEBAND_REG_BASE + 0x16)
#define PRL_BB_TX_RISE4                      				(PRL_BASEBAND_REG_BASE + 0x18)
#define PRL_BB_TX_RISE5                      				(PRL_BASEBAND_REG_BASE + 0x1A)
#define PRL_BB_TX_RISE6                      				(PRL_BASEBAND_REG_BASE + 0x1C)
#define PRL_BB_TX_RISE7                      				(PRL_BASEBAND_REG_BASE + 0x1E)
#define PRL_BB_TX_RISE8                      				(PRL_BASEBAND_REG_BASE + 0x20)
#define PRL_BB_TX_RISE9                      				(PRL_BASEBAND_REG_BASE + 0x22)
#define PRL_BB_TX_RISE10                     				(PRL_BASEBAND_REG_BASE + 0x24)
#define PRL_BB_TX_RISE11                     				(PRL_BASEBAND_REG_BASE + 0x26)

#define PRL_BB_TX_FALL0                      				(PRL_BASEBAND_REG_BASE + 0x30)
#define PRL_BB_TX_FALL1                      				(PRL_BASEBAND_REG_BASE + 0x32)
#define PRL_BB_TX_FALL2                      				(PRL_BASEBAND_REG_BASE + 0x34)
#define PRL_BB_TX_FALL3                      				(PRL_BASEBAND_REG_BASE + 0x36)
#define PRL_BB_TX_FALL4                      				(PRL_BASEBAND_REG_BASE + 0x38)
#define PRL_BB_TX_FALL5                      				(PRL_BASEBAND_REG_BASE + 0x3A)
#define PRL_BB_TX_FALL6                      				(PRL_BASEBAND_REG_BASE + 0x3C)
#define PRL_BB_TX_FALL7                      				(PRL_BASEBAND_REG_BASE + 0x3E)
#define PRL_BB_TX_FALL8                      				(PRL_BASEBAND_REG_BASE + 0x40)
#define PRL_BB_TX_FALL9                      				(PRL_BASEBAND_REG_BASE + 0x42)
#define PRL_BB_TX_FALL10                     				(PRL_BASEBAND_REG_BASE + 0x44)
#define PRL_BB_TX_FALL11                     				(PRL_BASEBAND_REG_BASE + 0x46)

#define PRL_CC_RX_DAC_TEST                   				(PRL_BASEBAND_REG_BASE + 0x50)
#define PRL_CC_TX_DAC_TEST                   				(PRL_BASEBAND_REG_BASE + 0x52)

#define PRL_BB_TX_RISE0_REG_VALUE							PRL_DAC_VALUE (0)
#define PRL_BB_TX_RISE1_REG_VALUE							PRL_DAC_VALUE (57)
#define PRL_BB_TX_RISE2_REG_VALUE							PRL_DAC_VALUE (115)
#define PRL_BB_TX_RISE3_REG_VALUE							PRL_DAC_VALUE (172)
#define PRL_BB_TX_RISE4_REG_VALUE							PRL_DAC_VALUE (230)
#define PRL_BB_TX_RISE5_REG_VALUE							PRL_DAC_VALUE (287)
#define PRL_BB_TX_RISE6_REG_VALUE							PRL_DAC_VALUE (345)
#define PRL_BB_TX_RISE7_REG_VALUE							PRL_DAC_VALUE (402)
#define PRL_BB_TX_RISE8_REG_VALUE							PRL_DAC_VALUE (460)
#define PRL_BB_TX_RISE9_REG_VALUE							PRL_DAC_VALUE (517)
#define PRL_BB_TX_RISE10_REG_VALUE							PRL_DAC_VALUE (574)
#define PRL_BB_TX_RISE11_REG_VALUE							PRL_DAC_VALUE (631)

#define PRL_BB_TX_FALL0_REG_VALUE 							PRL_DAC_VALUE (631)
#define PRL_BB_TX_FALL1_REG_VALUE							PRL_DAC_VALUE (574)
#define PRL_BB_TX_FALL2_REG_VALUE							PRL_DAC_VALUE (517)
#define PRL_BB_TX_FALL3_REG_VALUE							PRL_DAC_VALUE (460)
#define PRL_BB_TX_FALL4_REG_VALUE							PRL_DAC_VALUE (402)
#define PRL_BB_TX_FALL5_REG_VALUE							PRL_DAC_VALUE (345)
#define PRL_BB_TX_FALL6_REG_VALUE							PRL_DAC_VALUE (287)
#define PRL_BB_TX_FALL7_REG_VALUE							PRL_DAC_VALUE (230)
#define PRL_BB_TX_FALL8_REG_VALUE							PRL_DAC_VALUE (172)
#define PRL_BB_TX_FALL9_REG_VALUE							PRL_DAC_VALUE (115)
#define PRL_BB_TX_FALL10_REG_VALUE                          PRL_DAC_VALUE (57)
#define PRL_BB_TX_FALL11_REG_VALUE                          PRL_DAC_VALUE (0)

#define PRL_BB_CC_RX_DAC_CTL_CC_RX_DAC_SRC_VALUE			PRL_DAC_VALUE(288)
#define PRL_BB_CC_RX_DAC_CTL_CC_RX_DAC_NEU_VALUE			PRL_DAC_VALUE(217)
#define PRL_BB_CC_RX_DAC_CTL_CC_RX_DAC_SNK_VALUE			PRL_DAC_VALUE(146)

#define PRL_DAC_VALUE(value)                        		(value)
/***************************************************************************************************
                				UPD BIST CSRs
***************************************************************************************************/
#define PRL_BIST_CTL_A							(PRL_PDMAC_REG_BASE + 0x2C0)
#define PRL_BIST_CTL_B							(PRL_PDMAC_REG_BASE + 0x2C1)
#define PRL_BIST_ERR_CNT_HI						(PRL_PDMAC_REG_BASE + 0x2C2)
#define PRL_BIST_ERR_CNT_LO						(PRL_PDMAC_REG_BASE + 0x2C3)
#define PRL_DB_WFPS_TMR_CNT						(PRL_PDMAC_REG_BASE + 0x2C4)
#define PRL_DB_BS_BIT_CNT						(PRL_PDMAC_REG_BASE + 0x2C5)
#define PRL_DB_FUNC_CTL							(PRL_PDMAC_REG_BASE + 0x2C6)
#define PRL_BIST_RX_STAT						(PRL_PDMAC_REG_BASE + 0x2C7)
#define PRL_DB_BIT_TIME_CNT						(PRL_PDMAC_REG_BASE + 0x2C8)

/* BIST_CTL_A bit definitions */
#define PRL_BIST_TX_MODE_CARRIER_PURITY_TEST_MODE_0		0x00
#define PRL_BIST_TX_MODE_CARRIER_PURITY_TEST_MODE_1		0x01
#define PRL_BIST_TX_MODE_BIST_CARRIER_TEST_MODE_2		0x02
#define PRL_BIST_TX_MODE_BIST_CARRIER_TEST_MODE_3		0x03
#define PRL_BIST_TX_MODE_BIST_PRBS_PATTERN				0x04
#define PRL_BIST_TX_MODE_BIST_EYE_PATTERN_TEST_MODE		0x05

#define PRL_BIST_CTL_A_BIST_EN							BIT(3)
#define PRL_BIST_CTL_A_RX_EN							BIT(4)

/* BIST_CTL_B bit definitions */
#define PRL_BIST_CTL_B_BIST_CLR_ERR_CNT					BIT(2)
#define PRL_BIST_CTL_B_BIST_TX_RST						BIT(1)
#define PRL_BIST_CTL_B_BIST_TX_START					BIT(0)

#define PRL_BIST_TEST_DATA_OBJ							BIT(31)
#define PRL_BIST_CARRIER_MODE_OBJ						(BIT(28) | BIT(30))

/**************************************************************************************************/
/***************************************************************************************************
 	              					UPD_SNIFF_CTL
***************************************************************************************************/
/* UPD_SNIFF_CTL Reg Base */
#define PRL_PD_SNIFFER_UPD_SNIFF_CTL						0x2400

/* UPD_SNIFF_CTL bit definitions */
#define PRL_UPD_SNIFF_CTL_UPD_SNIFFER_ENABLE                BIT(0)

/*CC Detection DAC Reference Current Test Enable */
#define PRL_UPD_SNIFF_CTL_CC_DET_DAC_REF_CURR_TST_ENABLE    BIT(1)
/**************************************************************************************************/
/***************************************************************************************************
								PRL Module defines
***************************************************************************************************/
/* PRL_SOP_TYPE* defines*/
#define  PRL_SOP_TYPE								0x0
#define  PRL_SOP_P_TYPE								0x1
#define  PRL_SOP_PP_TYPE							0x2
#define  PRL_SOP_P_DEBUG_TYPE						0x3
#define  PRL_SOP_PP_DEBUG_TYPE						0x4
/***************************************************************************************************
								Message Header defines & macros
***************************************************************************************************/
/* Message Header Defines*/

/* Message Header bit defines*/
#define PRL_MESSAGE_TYPE_FIELD_START_BIT_POS		0		/* Message Type - Bit[4:0] */
#define PRL_PORT_DATA_ROLE_BIT_POS					5		/* Port Data Role - BIT(5) */
#define PRL_SPEC_REV_FIELD_START_BIT_POS			6		/* Specification Revision - Bit[7:6]*/
#define PRL_PORT_POWER_ROLE_OR_CABLE_PLUG_BIT_POS	8		/* SOP- Port Power role.SOP'/SOP''-Cable Plug Bit8*/
#define PRL_MESSAGE_ID_FIELD_START_BIT_POS			9		/* MessageID Bit[11:9]*/
#define PRL_DATA_OBJECTS_FIELD_START_BIT_POS		12		/* Number of Data Objects - Bit[14:12]*/
#define PRL_EXTENDED_BIT_POS						15		/* Extended Bit 15*/
#define PRL_EXTENDED_MSG							1

/* Message Header fields*/
#define PRL_MESSAGE_TYPE_FIELD_MASK					0x001F		/* Bit 0 to Bit 4*/
#define PRL_SPEC_REV_FIELD_MASK						0x00C0		/* Bit 6 to Bit 7*/
#define PRL_MESSAGE_ID_FIELD_MASK					0x0E00		/* Bit 9 to Bit 11*/
#define PRL_NUMBER_OF_DATA_OBJECTS_FIELD_MASK		0x7000		/* Bit 12 to Bit 14 */

/**************************************************************************************************/

/*Macros for getting data from Message Header*/

/* define to obtain Message Type field from a Message Header*/
#define PRL_GET_MESSAGE_TYPE(MsgHeader)       			\
	  (MsgHeader & PRL_MESSAGE_TYPE_FIELD_MASK)

/* define to obtain MEssageID field of Message Header*/
#define PRL_GET_MESSAGE_ID(MsgHeader)     				\
		((MsgHeader & PRL_MESSAGE_ID_FIELD_MASK) >> PRL_MESSAGE_ID_FIELD_START_BIT_POS)

/* define to obtain Specification Revision from Message Header*/
#define PRL_GET_PD_SPEC_REV(MsgHeader)      			\
		  ((MsgHeader & PRL_SPEC_REV_FIELD_MASK) >> PRL_SPEC_REV_FIELD_START_BIT_POS)

/* define to obtain Number of Data Objects from Message Header*/
#define PRL_GET_OBJECT_COUNT(MsgHeader)					\
		((MsgHeader & PRL_NUMBER_OF_DATA_OBJECTS_FIELD_MASK) >> PRL_DATA_OBJECTS_FIELD_START_BIT_POS)

/* define to check whether the message received is extended message or not from its message header.
	If 1- Extended message, 0- non extended message*/
#define PRL_IS_EXTENDED_MSG(MsgHeader)				(MsgHeader & BIT(PRL_EXTENDED_BIT_POS))

/* define to update the data object count in Message Header.
	Updates Number of Data Objects - Bit[14:12] field */
#define PRL_UPDATE_MSG_HEADER_DATA_OBJECT_COUNT(u16MsgHeader,u8DataObjectCount)				\
			((u16MsgHeader & ~PRL_NUMBER_OF_DATA_OBJECTS_FIELD_MASK) 					|	\
			(((UINT16) u8DataObjectCount << PRL_DATA_OBJECTS_FIELD_START_BIT_POS)))

/* Macro to increment Tx MessageID. It is a 3bit MessageIDCounter */
#define PRL_INCREMENT_MSG_ID(MessageID) 					((MessageID + 1)%8)

/**************************************************************************************************/
/**************************************************************************************************/

/* size of various fields in PD packet expressed in bytes */
#define PRL_SINGLE_DATAOBJ_SIZE_IN_BYTES          	4	/* size of single Data Object in terms of bytes */
#define PRL_MSG_HEADER_SIZE_IN_BYTES             	2	/* size of Message Header in terms of bytes */
#define PRL_EXTN_MSG_HEADER_SIZE_IN_BYTES			2 	/* size of Extended Message Header in terms of bytes */
#define PRL_MAX_EXTN_MSG_LEN_IN_BYTES				260	/* MaxExtendedMsgLen */
#define PRL_MAX_EXTN_MSG_CHUNK_LEN_IN_BYTES			26	/* MaxExtendedMsgChunkLen */

/* Max number of data bytes in legacy packet*/
#define PRL_MAX_DATA_OBJ_SIZE				(PRL_MAX_DATA_OBJ_COUNT * PRL_SINGLE_DATAOBJ_SIZE_IN_BYTES)
/* size of PD legacy packet in bytes (2 bytes of header + 28 bytes of data)*/
#define PRL_MAX_PD_LEGACY_PKT_LEN			(PRL_MAX_DATA_OBJ_SIZE + PRL_MSG_HEADER_SIZE_IN_BYTES)

/* max number of data object that can be sent in a legacy packet*/
#define PRL_MAX_DATA_OBJ_COUNT						7
/**************************************************************************************************/
/**************************************************************************************************/
 /* Extended Message Fields & bit definitions*/
#define PRL_EXTMSG_DATA_FIELD_MASK					0x01FF		/* BIT 0 to BIT 8 of Extended Message Header is Data size*/
#define PRL_EXTMSG_CHUNK_NUMBER_FIELD_MASK			0x7800		/* BIT 11 to BIT 14 of Extended Message Header is Chunk Number*/
#define PRL_EXT_REQUEST_CHUNK_BIT_MASK				BIT(PRL_EXTMSG_REQUEST_CHUNK_BIT_POS)
#define PRL_EXTMSG_CHUNK_NUMBER_FIELD_START_BIT_POS	11
#define PRL_EXTMSG_REQUEST_CHUNK_BIT_POS			10			/* BIT 10 of Extended message is Request Chunk*/
#define PRL_EXTMSG_CHUNKED_BIT_POS					15			/* BIT 15 of Extended message is Chunked field*/
#define PRL_EXTMSG_DATASIZE_BIT_POS					0x00		/* BIT 0 to BIT 8 of Extended Message Header is Data size*/

/**************************************************************************************************/
/* define to check whether the extended message received is Request Chunk message from extended message header.
	If 1- Request Chunk packet, 0- Response chunk packet */
#define PRL_IS_REQUEST_CHUNK_MSG(u16ExtMsgHeader)		(u16ExtMsgHeader & PRL_EXT_REQUEST_CHUNK_BIT_MASK)

/* define to check whether the extended message received is Chunked from extended message header.
	If 1- Chunked, 0- unchunked extended message */
#define PRL_IS_MSG_CHUNKED(u16ExtMsgHeader)				(u16ExtMsgHeader & BIT(PRL_EXTMSG_CHUNKED_BIT_POS))

/* define to obtain the ChunkNumber field from Extended Message Header.
	Returns Chunk Number field (Bit 11 to 14) from Extended message header */
#define PRL_GET_CHUNK_NUMBER(u16ExtMsgHeader)		((u16ExtMsgHeader & PRL_EXTMSG_CHUNK_NUMBER_FIELD_MASK) \
																>> PRL_EXTMSG_CHUNK_NUMBER_FIELD_START_BIT_POS )
/* define to obtain the Data size field from Extended Message Header.
	Returns Data Size field (Bit 0 to 8) from Extended message header */
#define PRL_GET_DATA_SIZE(u16ExtendedMsgHeader)		(u16ExtendedMsgHeader & PRL_EXTMSG_DATA_FIELD_MASK)

/* define to form Extended Message header for Request Chunk Message with ChunkNumber as parameter
	BIT(PRL_EXTMSG_CHUNKED_BIT_POS) - Bit 15 is set for Chunked field
	u8ChunkNo - ChunkNumber is updated to Header
	BIT(PRL_EXTMSG_REQUEST_CHUNK_BIT_POS) - Request Chunk bit is set
	For Request Chunk Packet, Data Size is zero always */
#define PRL_FORM_REQUEST_CHUNK_EXT_MSG_HEADER(u8ChunkNo) 									\
			(((UINT16) BIT(PRL_EXTMSG_CHUNKED_BIT_POS)) 							| 		\
			(((UINT16)u8ChunkNo << PRL_EXTMSG_CHUNK_NUMBER_FIELD_START_BIT_POS ) & 			\
			 			PRL_EXTMSG_CHUNK_NUMBER_FIELD_MASK) 						|		\
			((UINT16) BIT(PRL_EXTMSG_REQUEST_CHUNK_BIT_POS)))

/* define to form Extended Message header for Response Chunk Message with ChunkNumber &
	Datasize as parameter
	BIT(PRL_EXTMSG_CHUNKED_BIT_POS) - Bit 15 is set for Chunked field
	u8ChunkNo - ChunkNumber is updated to Header
	Request Chunk bit(BIT10)(PRL_EXTMSG_REQUEST_CHUNK_BIT_POS) is set to zero to indicate Response Chunk
	u16Datasize -(bit 0 to 8) Data Size field is updated */
#define PRL_FORM_RESPONSE_CHUNK_EXT_MSG_HEADER(u8ChunkNo, u16Datasize) 						\
			(((UINT16) BIT(PRL_EXTMSG_CHUNKED_BIT_POS)) 							| 		\
			(((UINT16)u8ChunkNo << PRL_EXTMSG_CHUNK_NUMBER_FIELD_START_BIT_POS ) & 			\
			 			PRL_EXTMSG_CHUNK_NUMBER_FIELD_MASK) 						|		\
			(u16Datasize & PRL_EXTMSG_DATA_FIELD_MASK))

/* define to update Chunk number field in a Extended message Header */
#define PRL_UPDATE_EXT_MSG_HEADER_CHUNK_NUMBER(u16ExtMsgHeader,u8ChunkNumber) 			\
			((u16ExtMsgHeader & ~PRL_EXTMSG_CHUNK_NUMBER_FIELD_MASK) 				|	\
			((UINT16)u8ChunkNumber << PRL_EXTMSG_CHUNK_NUMBER_FIELD_START_BIT_POS))

/* define to form UINT32 header of which HIWORD is ExtendedMessage Header & LOWORD is PD Message Header*/
#define PRL_FORM_COMBINED_MSG_HEADER(u16ExtendedMsgHeader,u16MsgHeader) 	\
			  MAKE_UINT32(u16ExtendedMsgHeader, u16MsgHeader)

/* define to get Message Header from Combined UINT32 message header*/
#define PRL_GET_MSG_HEADER(u32CombinedMsgHeader)			LOWORD(u32CombinedMsgHeader)

/* define to obtain Extended Message Header from Combined UINT32 message header*/
#define PRL_GET_EXTENDED_MSG_HEADER(u32CombinedMsgHeader)	HIWORD(u32CombinedMsgHeader)

#define PRL_FIRST_CHUNK_PACKET								0

/* zero padding byte for chunk padding */
#define PRL_ZERO_PADDING_BYTE								0x00

/* Request chunk has only two bytes of extended message header. Hence two byte has to be padded
	defines to aid it in the code */
#define PRL_REQUEST_CHUNK_PADDING_BYTE_NUMBER				2
#define PRL_REQUEST_CHUNK_DATA_SIZE							0
#define PRL_REQUEST_CHUNK_DATA_OBJ_CNT						1
/*************************************************************************************************/
/* function prototype for Tx call back */
/* 1st Param - Port Number, 2nd Param - Tx Done PE state, 3rd Param - Tx Done PE Sub-state 
 4th Param - Tx Failed PE state, 5th Param - Tx Failed PE sub-*/
typedef void(*PRLTxCallback)(UINT8 u8PortNum,UINT8 u8TxDonePEst,UINT8 u8TxDonePESubst,UINT8 u8TxFailedPEst, UINT8 u8TxFailedPESubst);

/* function prototype for Rx callback*/
/* 1st Param - Port Number, 2nd Param - RxRcvStatus -  Rx Receiver Status */
typedef void(*PRLRxCallback) (UINT8 u8PortNum, UINT8 u8RxRcvStatus);
/*************************************************************************************************/
/* define to make a Packed 32bit with  PE next state & sub-state on Tx Interrups */
/*	In PkdPEstOnTxStatus
	Bit Fields 0:7 - u8TxDonePEst	- 	This is to pass the Policy engine STATE corresponding to
										GOODCRC reception (Tx_DONE interrupt from PRL) in case of PD message
										or Message completion (Tx_EOP interrupt from PRL) in case of HardReset
										or CableReset
	Bit Fields 8:15 - u8TxDonePESubst - This is to pass the Policy engine SUBSTATE on corresponding to
										GOODCRC reception (Tx_DONE interrupt from PRL) in case of PD message
										or Message completion (Tx_EOP interrupt from PRL) in case of HardReset
	Bit Fields 16:23 - u8TxFailedPEst - This is to pass the Policy engine STATE corresponding to
										GOODCRC not received (Tx_FAILED or Tx_ABORTED interrupt from PRL)
										for PD message or HardReset or CableReset
										or Message completion (Tx_EOP interrupt from PRL) in case of HardReset
	Bit Fields 24:31 - u8TxFailedPESubst - This is to pass the Policy engine SUBSTATE corresponding to
										GOODCRC not received (Tx_FAILED or Tx_ABORTED interrupt from PRL)
										for PD message or HardReset or CableReset
										or Message completion (Tx_EOP interrupt from PRL) in case of HardReset
*/

#define PRL_BUILD_PKD_TXST_U32(u8TxDoneorEOPPEst, u8TxDoneorEOPPESubst, u8TxFailedPEst, u8TxFailedPESubst) 	\
			MAKE_UINT32_FROM_BYTES(u8TxDoneorEOPPEst, u8TxDoneorEOPPESubst, u8TxFailedPEst, u8TxFailedPESubst)

/********************************************************************************************************
                                PRL State defines
********************************************************************************************************/

/* PRL Tx states */

#define PRL_TX_IDLE_ST								0
#define PRL_TX_DONE_ST								1
#define PRL_TX_ABORTED_ST							2
#define PRL_TX_FAILED_ST							3
#define PRL_TX_EOP_ST								4
#define PRL_TX_MSG_BUFFERED_ON_CA_ST				5
#define PRL_TX_MSG_ON_LINE_ST						6
#define PRL_TX_HARD_RESET_ST						7
#define PRL_TX_CABLE_RESET_ST						8
#define PRL_Tx_CA_SRC_SINKTXTIMER_ON_ST				9
#define PRL_TX_CA_SINK_TXNG_ST						10

/* Chunk States*/
#define PRL_CH_CHUNK_IDLE_ST						0
#define PRL_RCH_EXPECT_RESPONSE_CHUNK_WAIT_ST		1
#define PRL_RCH_SEND_CHUNK_REQUEST_ST				2
#define PRL_RCH_WAIT_FOR_CHUNK_REQUEST_STATUS_ST	3
#define PRL_RCH_CHUNK_RECV_ERROR_ST					4
#define PRL_TCH_SEND_RESPONSE_CHUNK_ST				5
#define PRL_TCH_WAIT_FOR_REQUEST_CHUNK_STATUS_ST	6
#define PRL_TCH_EXPECT_CHUNK_REQUEST_WAIT_ST		7
#define PRL_TCH_CHUNK_TX_ERROR_ST					8
#define PRL_TCH_CHUNKSENDERREQUEST_TIMEOUT_ST		9
#define PRL_CH_ABORT_ST                             10

/*Tx API PRL_TransmitMsg return Value*/
#define PRL_RET_TX_MSG_DISCARD_ON_RCV				0
#define PRL_RET_TX_MSG_TRANSMITTED_ON_LINE			1
#define PRL_RET_TX_MSG_BUFFERED						2
#define PRL_RET_TX_MSG_CHUNKING_ENABLED				3

/* Rx Chunk Status bit define*/
#define PRL_RX_CHUNK_RCV_ERROR						BIT(0)
																							  
/* PRL_Receive Return value*/
#define PRL_RET_NO_MSG_RCVD							0x0
#define PRL_RET_MSG_RCVD							BIT(1)
#define PRL_RET_HARD_RESET_RCVD						BIT(2)
#define PRL_RET_EXT_MSG_RCVD						BIT(3)
#define PRL_RET_RCV_CHUNK_ERROR						BIT(4)
/*************************************************************************************************/
/* define to be used for API PRL_SendCableorHardReset's parameter u8CableorHardReset*/
#define PRL_SEND_CABLE_RESET		1
#define PRL_SEND_HARD_RESET			0

/**************************** Defines for Hardware Retry Count *******************/
#define PRL_HW_RETRY_CNT_2_0        3
#define PRL_HW_RETRY_CNT_3_0        2 

// *****************************************************************************
// *****************************************************************************
// Section: Data Structures - PRL Structs
// *****************************************************************************
// *****************************************************************************
typedef struct  MCHP_PSF_STRUCT_PACKED_START _PROTOCOL_LAYER 
{
	PRLTxCallback	pFnTxCallback;		/* Tx Callback*/
	UINT32	u32PkdPEstOnTxStatus;		/* Variable to store Policy engine state & Policy substate on Tx Interrupts to be passed to Callback */
	UINT8	u8TxStateISR;				/* Protocol Layer state variable*/
	UINT8	u8SOPMsgID : 3;				/* SOP MessageID */
	UINT8	u8SOP_P_MsgID : 3;			/* SOP' MessageID*/
	UINT8	u8SOP_PP_MsgID : 3;			/* SOP'' MessageID*/
	UINT8	u8RxError : 1;              /* Rx Error - intimates Chunk error to Policy engine */
	UINT8 	u8RxRcvdISR : 1;            /* ISR to Foreground Sync variable indicating Message Recv*/
    UINT8   u8RxHRRcvdISR : 1;          /* ISR to Foreground Sync variable indicating HR Recv*/
    UINT8   u8TxStsDPMSyncISR : 1;      /* ISR to Foreground sync variable indicating DPM internal event  1st Tx completed*/
    UINT8   u8RxDisable:1;

} MCHP_PSF_STRUCT_PACKED_END PROTOCOL_LAYER_STRUCT;
/*************************************************************************************************/
typedef struct MCHP_PSF_STRUCT_PACKED_START _CHUNK
{
  	PRLTxCallback  pFnTxCallback;
	UINT32 	u32pkdTmrID_TxSt;
	UINT8 	u8CAorChunkSMTimerID;
	UINT16 	u16RxReceivedBytes:9;			/* Number of Data Bytes received in RCH*/
	UINT8 	u8TotalChunkPkt:4 ;				/* Total Chunk packet to be transmitted in TCH or expected RCH*/
	UINT8 	u8ChunkNumExpectedOrSent:4;		/* ChunkNumber of Chunk packet lastly sent in TCH or received in RCV*/
	UINT8 	u8ChunkState : 4;				/* State variable for Chunk SM*/
  	UINT8	u8EnableChunkSM:1;				/* To know whether Chunk SM is enabled or disabled*/
	UINT8	u8AbortFlag:1;					/* Chunk abort flag to abort chunking*/

} MCHP_PSF_STRUCT_PACKED_END PRL_CHUNK_STRUCT;
/*************************************************************************************************/
typedef struct MCHP_PSF_STRUCT_PACKED_START _PRL_TX_DESC 
{
	UINT8	u8TimerID;
	UINT8	u8TxDoneSt;
	UINT8	u8TxAbortSt;
	UINT8	u8TxFailSt;
	PRLTxCallback	pfnTxCallback;
} MCHP_PSF_STRUCT_PACKED_END PRL_TRANSMITTER_DESC;
/*************************************************************************************************/
/* In the order of struct element, Rx FIFO is filled on message reception*/
typedef struct MCHP_PSF_STRUCT_PACKED_START _PRL_RECV
{
	UINT8	u8RxNbytes;
	UINT8 	u8SOPtype;				//u8RxStatus - field from Rx FIFO
	UINT16	u16Header;
	UINT8	u8DataObj [PRL_MAX_DATA_OBJ_SIZE];
	UINT32	u32CRC;

} MCHP_PSF_STRUCT_PACKED_END PRL_RECVBUFF;
/*************************************************************************************************/
typedef struct MCHP_PSF_STRUCT_PACKED_START _PRL_EXTENDED_MESSAGE_BUFF 
{
	UINT16 	u16Header;
	UINT16	u16ExtendedMsgHeader;
	UINT8 	u8SOPtype;
	UINT8 	u8Data [PRL_MAX_EXTN_MSG_LEN_IN_BYTES];

} MCHP_PSF_STRUCT_PACKED_END PRL_EXTMSG_BUFF;
/*************************************************************************************************/
// *****************************************************************************
// *****************************************************************************
// Section: Interface Routines
// *****************************************************************************
// *****************************************************************************
/**************************************************************************************************
    Function:
        void  PRL_Init (UINT8 u8PortNum);

    Summary:
        This API initialize the UPD_MAC Registers and Protocol Layer variables
		based on port number passed.

    Devices Supported:
        UPD350 REV A

    Description:
        This API initializes BMC Encoder-Decoder, Baseband configuration,
		UPD_MAC Tx- Rx configurations, Protocol Layer globals and interrupts.

    Conditions:
        This API has to be called only after Type c & DPM init.
		Protocol Layer init depends on Spec Rev, Data & Power Role global variable updated by above modules.

    Input:
        u8PortNum - Port number of the device.
					Value passed will be less than CONFIG_PD_PORT_COUNT.

    Return:
        None.

    Remarks:
        This API has to be called for all the ports (0 to (CONFIG_PD_PORT_COUNT -1))
		to initialize the Protocol Layer.

**************************************************************************************************/
void  PRL_Init (UINT8 u8PortNum);

/**************************************************************************************************
    Function:
		void PRL_UpdateSpecAndDeviceRoles (UINT8 u8PortNum);

    Summary:
        This API updates the current specification Revision, Data Role, Power role to UPD_MAC layer.

    Devices Supported:
        UPD350 REV A

    Description:
        This API updates UPD_MAC register with Stack's current Spec Rev, Data Role & Power Role from DPM globals.
		This updates are used to fill the fields in GOOD_CRC packets sent in auto response mode.

    Conditions:
        None.

    Input:
        u8PortNum - Port number of the device.
					Value passed will be less than CONFIG_PD_PORT_COUNT.
    Return:
        None.

    Remarks:
		This API is called as part of init sequence. Apart from this,
		Policy Engine has to call this function whenever there is a change in Data Role or Spec Rev or power role.

**************************************************************************************************/
void PRL_UpdateSpecAndDeviceRoles (UINT8 u8PortNum);

/**************************************************************************************************
    Function:
        UINT16 PRL_FormSOPTypeMsgHeader (UINT8 u8PortNum, UINT8 u8MessageType, UINT8 u8ObjectCount, UINT8 u8Extended);

    Summary:
        This API forms SOP type header.

    Devices Supported:
        UPD350 REV A

    Description:
		This API forms SOP Message Header with the parameter passed. Other parameters in a PD message Header
		(Spec Rev, Data Role, Power Role) are obtained from globals.

    Conditions:
		None.

    Input:
        u8PortNum 		- Port number of the device.
							Value passed will be less than CONFIG_PD_PORT_COUNT.
		u8MessageType 	- Message Type field in PD message header.

		u8ObjectCount	- Number of Data Objects field in PD message header.

		u8Extended		- Extended field in PD message header.

    Return:
        UINT16	- Returns 16-bit SOP header.

    Remarks:
        None.

**************************************************************************************************/
UINT16 PRL_FormSOPTypeMsgHeader (UINT8 u8PortNum, UINT8 u8MessageType, UINT8 u8ObjectCount, UINT8 u8Extended);

/**************************************************************************************************
    Function:
        UINT16 PRL_FormNonSOPTypeMsgHeader (UINT8 u8PortNum, UINT8 u8MessageType, UINT8 u8ObjectCount, UINT8 u8Extended);
 
    Summary:
        This API forms Non-SOP type header.

    Devices Supported:
        UPD350 REV A

    Description:
		This API forms Non-SOP Message Header with the parameter passed.

    Conditions:
		None.

    Input:
        u8PortNum 		- Port number of the device.
							Value passed will be less than CONFIG_PD_PORT_COUNT.
		u8MessageType 	- Message Type field in PD message header.

		u8ObjectCount	- Number of Data Objects field in PD message header.

		u8Extended		- Extended field in PD message header.

    Return:
        UINT16	- Returns 16-bit Non-SOP header.

    Remarks:
        None.

**************************************************************************************************/
UINT16 PRL_FormNonSOPTypeMsgHeader (UINT8 u8PortNum, UINT8 u8MessageType, UINT8 u8ObjectCount, UINT8 u8Extended);

/**************************************************************************************************
    Function:
        void PRL_EnableRx (UINT8 u8PortNum, UINT8 bEnable);

    Summary:
        This API enables or disables UPD_MAC PHY for Reception.

    Devices Supported:
        UPD350 REV A

    Description:
		This API enables or disables UPD_MAC PHY for Reception.

    Conditions:
        DPM module has to call this API after detecting a Source/Sink Attach to enable further communication.

    Input:
        u8PortNum 	- Port number of the device.
							Value passed will be less than CONFIG_PD_PORT_COUNT.
		bEnable		- Passing 'true' enables the Receiver UPD_MAC PHY
							Passing 'false' disables the Receiver UPD_MAC PHY

    Return:
        None.

    Remarks:
        Policy Engine can use this API enable or Disable Rx as required.

**************************************************************************************************/
void PRL_EnableRx (UINT8 u8PortNum, UINT8 u8Enable);

/**************************************************************************************************
    Function:
        UINT8 PRL_TransmitMsg (UINT8 u8PortNum, UINT8 u8SOPType, UINT32 u32Header, UINT8 *pu8DataBuffer, PRLTxCallback pfnTxCallback, UINT32  u32PkdPEstOnTxStatus);

    Summary:
        This API is to form & send PD message on PHY.

    Devices Supported:
        UPD350 REV A

    Description:
		This API forms & send PD message on UPD_MAC PHY.

    Conditions:
        In the parameter u32Header -	HIWORD holds Extended Message Header
										LOWORD holds PD Message Header
		Combined Message Header can be using Macro PRL_FORM_COMBINED_MSG_HEADER(u16ExtendedMsgHeader,u16MsgHeader)

    Input:
        u8PortNum			- Port number of the device.
								Value passed will be less than CONFIG_PD_PORT_COUNT.
		u8SOPType			- SOP type of the message can be following value
								PRL_SOP_TYPE (0x00)		- SOP  type
								PRL_SOP_P_TYPE (0x1)	- SOP' type
								PRL_SOP_PP_TYPE (0x2)	- SOP'' type

		u32Header			- Combined Message header.
								HIWORD holds Extended Message Header. LOWORD holds PD Message Header
		pu8DataBuffer		- Pointer to the buffer holding Data objects.
		pfnTxCallback		- Pointer to Tx Callback.
		u32PkdPEstOnTxStatus- Parameters for Tx Callback function. Formed using macro,
								PRL_BUILD_PKD_TXST_U32(u8TxDoneorEOPPEst, u8TxDoneorEOPPESubst, u8TxFailedPEst, u8TxFailedPESubst)

    Return:
		The return value can be the following,
			PRL_RET_TX_MSG_DISCARD_ON_RCV(0x00)		-	Message transmission aborted as SOP type Message is received
			PRL_RET_TX_MSG_TRANSMITTED_ON_LINE(0x01)-	Message transmitted on line
			PRL_RET_TX_MSG_BUFFERED(0x02)			-	Message is buffered for CA & will be sent later
			PRL_RET_TX_MSG_CHUNKING_ENABLED(0x03)	- 	Message is tranmitted via Chunking SM

    Remarks:
        Policy Engine calls this API to send message.

**************************************************************************************************/
UINT8 PRL_TransmitMsg (UINT8 u8PortNum, UINT8 u8SOPType, UINT32 u32Header, \
        UINT8 *pu8DataBuffer, PRLTxCallback pfnTxCallback, UINT32  u32PkdPEstOnTxStatus);

/**************************************************************************************************
    Function:
        UINT8 PRL_BuildTxPacket (UINT8 u8PortNum, UINT32 u32Header, UINT8 *pu8DataBuffer, UINT8 *pu8TxPkt);
    Summary:
        This API is to form the PD message to transmitted into single buffer.

    Devices Supported:
        UPD350 REV A

    Description:
		This API converts PD message into single buffer to send it through TX_FIFO.

    Conditions:
        None.

    Input:
        u8PortNum		- Port number of the device.
							Value passed will be less than CONFIG_PD_PORT_COUNT.
		8SOPType		- SOP type of the message can be following value
							PRL_SOP_TYPE (0x00)		- SOP  type
							PRL_SOP_P_TYPE (0x1)	- SOP' type
							PRL_SOP_PP_TYPE (0x2)	- SOP'' type

		u32Header		- Combined Message header.
							HIWORD holds Extended Message Header. LOWORD holds PD Message Header
		pu8DataBuffer	- Pointer to the buffer holding Data objects.
		pu8TxPkt		- Pointer to buffer where PD message is converted to single buffer & stored.
							This buffer is written to TX_FIFO

    Return:
        UINT8 - Returns Packet length in terms of bytes.

    Remarks:
        It is called by PRL_TransmitMsg() API internally.

**************************************************************************************************/
UINT8 PRL_BuildTxPacket (UINT8 u8PortNum, UINT32 u32Header, UINT8 *pu8DataBuffer, UINT8 *pu8TxPkt);

/**************************************************************************************************
    Function:
        void PRL_SendCableorHardReset (UINT8 u8PortNum, UINT8 u8CableorHardReset, PRLTxCallback pfnTxCallback, UINT32  u32PkdPEstOnTxStatus);

    Summary:
        This API sends Cable or Hard Reset.

    Devices Supported:
        UPD350 REV A

    Description:
		This API is to send Cable or Hard Reset on UPD_MAC PHY.

    Conditions:
        None.

    Input:
        u8PortNum			- Port number of the device.
								Value passed will be less than CONFIG_PD_PORT_COUNT.
		u8CableorHardReset	- This can values,
									PRL_SEND_HARD_RESET(0)	- Sends Hard Reset
									PRL_SEND_CABLE_RESET(1)	- Sends Cable Reset
		pfnTxCallback		- Pointer to Tx Callback.
		u32PkdPEstOnTxStatus- Parameters for Tx Callback function. Formed using macro
								PRL_BUILD_PKD_TXST_U32(u8TxDoneorEOPPEst, u8TxDoneorEOPPESubst, u8TxFailedPEst, u8TxFailedPESubst)


    Return:
        None.

    Remarks:
        None.

**************************************************************************************************/
void PRL_SendCableorHardReset (UINT8 u8PortNum, UINT8 u8CableorHardReset, PRLTxCallback pfnTxCallback, UINT32  u32PkdPEstOnTxStatus);

/**************************************************************************************************
    Function:
        UINT8 PRL_ReceiveMsg (UINT8 u8PortNum, UINT8  *pu8SOPType, UINT32 *pu32Header, UINT8 *pu8DataBuffer, PRLRxCallback pfnRxCallback);

    Summary:
        This API is used to receive message from Protocol Layer.

    Devices Supported:
        UPD350 REV A

    Description:
		This API is called to receive messages or any receive error intimation from Protocol Layer

    Conditions:
        None.

    Input:
        u8PortNum		- Port number of the device.
							Value passed will be less than CONFIG_PD_PORT_COUNT.
		pu8SOPType		- Pointer to SOP type variable to which SOP type of the received message is returned.
							It can take following value,
							PRL_SOP_TYPE (0x00)		- SOP  type
							PRL_SOP_P_TYPE (0x1)	- SOP' type
							PRL_SOP_PP_TYPE (0x2)	- SOP'' type

		pu32Header		- Pointer to header variable to which Combined Message header of received message is returned.
							HIWORD holds Extended Message Header. LOWORD holds PD Message Header
		pu8DataBuffer	- Pointer to data buffer to which received message data is updated.
		pfnRxCallback	- Pointer to Rx callback which will be called in the API.

    Return:
        Return values can be as follows,
			PRL_RET_NO_MSG_RCVD(0x0) 	- No message received
			PRL_RET_MSG_RCVD(0x1)		- Message is received
			PRL_RET_HARD_RESET_RCVD(0x2)- Hardreset is received
			PRL_RET_EXT_MSG_RCVD(0x3)	- Extended message is received
			PRL_RET_RCV_CHUNK_ERROR(0x4)- Receiver Chunk error


    Remarks:
        None.

**************************************************************************************************/
UINT8 PRL_ReceiveMsg (UINT8 u8PortNum, UINT8  *pu8SOPType, UINT32 *pu32Header, UINT8 *pu8DataBuffer, PRLRxCallback pfnRxCallback);

/**************************************************************************************************
    Function:
        UINT8 PRL_ProcessRecvdMsg(UINT8 u8PortNum);

    Summary:
        This API processes raw FIFO data received to Message format.

    Devices Supported:
        UPD350 REV A

    Description:
		This API is called by PRL_ReceiveMsg() to process data read from RX FIFO to message format.

    Conditions:
        None.

    Input:
        u8PortNum	- Port number of the device.
						Value passed will be less than CONFIG_PD_PORT_COUNT.

    Return:
        Return values can be as follows,
			PRL_RET_NO_MSG_RCVD(0x0) 	- No message received
			PRL_RET_MSG_RCVD(0x1)		- Message is received
			PRL_RET_EXT_MSG_RCVD(0x3)	- Extended message is received

    Remarks:
        It is internal API called by PRL_ReceiveMsg().

**************************************************************************************************/
UINT8 PRL_ProcessRecvdMsg(UINT8 u8PortNum);

/**************************************************************************************************
    Function:
        void PRL_HandleISR (UINT8 u8PortNumber);

    Summary:
        This API is called to process MAC interrupt.

    Devices Supported:
        UPD350 REV A

    Description:
		This API is called UPD_InterruptHandler to process UPD_MAC interrupt.

    Conditions:
        None.

    Input:
        u8PortNum	- Port number of the device.
						Value passed will be less than CONFIG_PD_PORT_COUNT.

    Return:
        None.

    Remarks:
        None.

**************************************************************************************************/
void PRL_HandleISR (UINT8 u8PortNumber);

/**************************************************************************************************
    Function:
        void PRL_ProcessRxFIFOISR (UINT8 u8PortNum);

    Summary:
        This API is called to process FIFO_NOT_EMPTY interrupt.

    Devices Supported:
        UPD350 REV A

    Description:
		This API reads the message from RX FIFO on FIFO_NOT_EMPTY interrupt.

    Conditions:
        None.

    Input:
        u8PortNum	- Port number of the device.
						Value passed will be less than CONFIG_PD_PORT_COUNT.

    Return:
        None.

    Remarks:
        It is called by PRL_HandleISR() internally.

**************************************************************************************************/
void PRL_ProcessRxFIFOISR (UINT8 u8PortNum);

/**************************************************************************************************
    Function:
        void PRL_IncrementMsgID (UINT8 u8PortNum);

    Summary:
        This API maintains Tx Message ID counter for all SOP's.

    Devices Supported:
        UPD350 REV A

    Description:
		This API increment the Tx Message ID counter depending on the SOP type sent lastly.

    Conditions:
        None.

    Input:
        u8PortNum	- Port number of the device.
						Value passed will be less than CONFIG_PD_PORT_COUNT.

    Return:
        None.

    Remarks:
        It is called by PRL_HandleISR() internally on receiving TX UPD_MAC interrupt.

**************************************************************************************************/
void PRL_IncrementMsgID (UINT8 u8PortNum);

/**************************************************************************************************
    Function:
        void PRL_ProtocolSpecificSOPReset(UINT8 u8PortNum, UINT8 u8SOPType);

    Summary:
        This API is to reset message ID counter of Tx & Rx SOP type specifically.

    Devices Supported:
        UPD350 REV A

    Description:
		This API resets Tx Message ID counter & Rx message ID stored for the SOP type.

    Conditions:
        None.

    Input:
        u8PortNum	- Port number of the device.
						Value passed will be less than CONFIG_PD_PORT_COUNT.
		u8SOPType	- SOP Type for message ID has to be reset.
						It can take following value,
							PRL_SOP_TYPE (0x00)		- SOP  type
							PRL_SOP_P_TYPE (0x1)	- SOP' type
							PRL_SOP_PP_TYPE (0x2)	- SOP'' type


    Return:
        None.

    Remarks:
        This API should be called by Policy engine on receiving Soft Reset for specific SOP type.

**************************************************************************************************/
void PRL_ProtocolSpecificSOPReset(UINT8 u8PortNum, UINT8 u8SOPType);

/**************************************************************************************************
    Function:
        void PRL_ProtocolResetAllSOPs(UINT8 u8PortNum);

    Summary:
        This API is to reset message ID counter of Tx & Rx for all SOPs.

    Devices Supported:
        UPD350 REV A

    Description:
		This API resets Tx Message ID counter & Rx message ID stored for all SOP types.

    Conditions:
        None.

    Input:
        u8PortNum	- Port number of the device.
						Value passed will be less than CONFIG_PD_PORT_COUNT.

    Return:
        None.

    Remarks:
        This API should be called by Policy engine on receiving Hard reset to reset the protocol layer.

**************************************************************************************************/
void PRL_ProtocolResetAllSOPs(UINT8 u8PortNum);

/**************************************************************************************************
    Function:
        void PRL_PHYLayerReset (UINT8 u8PortNum);

    Summary:
        This API is to reset the PHY layer completely.

    Devices Supported:
        UPD350 REV A

    Description:
		This API Completely resets the PHY layer.

    Conditions:
        PRL_ProtocolResetAllSOPs() has to be called along with this API
			 for complete reset of PHY & protocol layer.

    Input:
        u8PortNum	- Port number of the device.
						Value passed will be less than CONFIG_PD_PORT_COUNT.

    Return:
        None.

    Remarks:
        None.

**************************************************************************************************/
void PRL_PHYLayerReset (UINT8 u8PortNum);

/**************************************************************************************************
    Function:
        void PRL_HRorCRCompltIndicationFromPE (UINT8 u8PortNum);

    Summary:
        This API is for the Policy Engine to inform the Protocol Layer on processing Hard Reset.

    Devices Supported:
        UPD350 REV A

    Description:
		This API has to be called by Policy Engine on completing the Hard Reset.

    Conditions:
        None.

    Input:
        u8PortNum	- Port number of the device.
						Value passed will be less than CONFIG_PD_PORT_COUNT.

    Return:
        None.

    Remarks:
        None.

**************************************************************************************************/
void PRL_HRorCRCompltIndicationFromPE (UINT8 u8PortNum);

/**************************************************************************************************
    Function:
        void PRL_SetCollisionAvoidance (UINT8 u8PortNum, UINT8 u8Enable);

    Summary:
        This API is to set or clear Collision Avoidance.

    Devices Supported:
        UPD350 REV A

    Description:
		This API is for the Policy Engine to set the Collision Avoidance (Rp value SinkTxNG) or
			clear(Rp value to SinkTxOK).

    Conditions:
        None.

    Input:
        u8PortNum	- Port number of the device.
						Value passed will be less than CONFIG_PD_PORT_COUNT.
		u8Enable	- Passing 'true' sets Rp to SinkTxNG(1.5A)
					  Passing 'false' sets Rp value to SinkTxOK(3A)

    Return:
        None.

    Remarks:
        This function confined to INCLUDE_PD_3_0 define.

**************************************************************************************************/
void PRL_SetCollisionAvoidance (UINT8 u8PortNum, UINT8 u8Enable);

/**************************************************************************************************
    Function:
        void PRL_CASinkTxTimerOut_TimerCB (UINT8 u8PortNum, UINT8 u8DummyVariable);

    Summary:
        This API is callback for SinTxTimer timeout.

    Devices Supported:
        UPD350 REV A

    Description:
		This API is callback for SinkTxTimer which started on setting Collision Avoidance
			to Rp value Rp SinkTxNG. This callback transmits buffered Tx message due
			CA handling if any on PHY.

    Conditions:
        None.

    Input:
        u8PortNum	- Port number of the device.
						Value passed will be less than CONFIG_PD_PORT_COUNT.

		u8DummyVariable - Place holder parameter to maintain TimerCB define format.

    Return:
        None.

    Remarks:
        This function confined to INCLUDE_PD_3_0 define.

**************************************************************************************************/
void PRL_CASinkTxTimerOut_TimerCB (UINT8 u8PortNum, UINT8 u8DummyVariable);

/**************************************************************************************************
    Function:
        void PRL_CommitPendingTxOnCAISR (UINT8 u8PortNum);

    Summary:
        This API commits buffered Tx message due CA handling on PHY.

    Devices Supported:
        UPD350 REV A

    Description:
		This API is transmits Tx message buffered in Tx_FIFO to PHY.

    Conditions:
        None.

    Input:
        u8PortNum	- Port number of the device.
						Value passed will be less than CONFIG_PD_PORT_COUNT.

    Return:
        None.

    Remarks:
        This function confined to INCLUDE_PD_3_0 define.
		It is called by PRL_CASinkTxTimerOut_TimerCB() internally.
		It is also called when Sink buffers Tx message to send on PHY when Rp value is set to SinkTxOK.

**************************************************************************************************/
void PRL_CommitPendingTxOnCAISR (UINT8 u8PortNum);

/**************************************************************************************************
    Function:
        UINT8 PRL_IsAmsInitiatable (UINT8 u8PortNum);

    Summary:
        This API returns whether an Ams can be initiated.

    Devices Supported:
        UPD350 REV A

    Description:
		This API returns a value to decide whether an AMS can be initiated based
        Rp value whether it is SinkTxOK or SinkTxNG in case of Sink and Tx buffer
        idleness in case of Source.
 
    Conditions:
        None.

    Input:
        u8PortNum	- Port number of the device.
						Value passed will be less than CONFIG_PD_PORT_COUNT.

    Return:
        This API returns
		FALSE(0x0)	-  AMS cannot be initiated
        TRUE(0x1) - AMS can be initiated.

    Remarks:
        This function confined to INCLUDE_PD_3_0 define.

**************************************************************************************************/
UINT8 PRL_IsAMSInitiatable (UINT8 u8PortNum);

/**************************************************************************************************
    Function:
        void PRL_RunChunkStateMachine (UINT8 u8PortNum);

    Summary:
        This API is to run Chunk state machine.

    Devices Supported:
        UPD350 REV A

    Description:
		This API runs Chunk state machine if it is enabled.

    Conditions:
        None.

    Input:
        u8PortNum	- Port number of the device.
						Value passed will be less than CONFIG_PD_PORT_COUNT.

    Return:
        None.

    Remarks:
        This function confined to INCLUDE_PD_3_0 define.
		It should called by DPM port specifically and periodically.

**************************************************************************************************/
void PRL_RunChunkStateMachine (UINT8 u8PortNum);

/**************************************************************************************************
    Function:
        void PRL_ResetChunkSM (UINT8 u8PortNum);

    Summary:
        This API is to resets & disables Chunk State machine.

    Devices Supported:
        UPD350 REV A

    Description:
		This API resets all the Chunk State machine related globals. It also disables the SM.

    Conditions:
        None.

    Input:
        u8PortNum	- Port number of the device.
						Value passed will be less than CONFIG_PD_PORT_COUNT.

    Return:
        None.

    Remarks:
        This function confined to INCLUDE_PD_3_0 define.
		It is called by Protocol layer once the Chunking process is done.

**************************************************************************************************/
void PRL_ResetChunkSM (UINT8 u8PortNum);

/**************************************************************************************************
    Function:
        void PRL_TxOriginalCBfromCH (UINT8 u8PortNum, UINT8 u8TxStateforCB);

    Summary:
        This API calls the Tx callback given by Policy engine
			once chunking is completed or resulted in tranmission error.

    Devices Supported:
        UPD350 REV A

    Description:
		This API calls the Tx callback given by Policy engine to PRL_TransmitMsg() API
		for Extended message transmission. It is called on extended message transmission complete
		or transmission error.

    Conditions:
        None.

    Input:
        u8PortNum		- Port number of the device.
							Value passed will be less than CONFIG_PD_PORT_COUNT.

		u8TxStateforCB - Protocol layer Tx state variable.

    Return:
        None.

    Remarks:
        This function confined to INCLUDE_PD_3_0 define.

**************************************************************************************************/
void PRL_TxOriginalCBfromCH (UINT8 u8PortNum, UINT8 u8TxStateforCB);

/**************************************************************************************************
    Function:
        void PRL_ChunkStateChange_TimerCB (UINT8 u8PortNum, UINT8 u8ChunkState);

    Summary:
        This API is callback for Chunk SM Timer timeout.

    Devices Supported:
        UPD350 REV A

    Description:
		This API is timer callback for Chunk state machine related timers.
		It changes Chunk state machine state.

    Conditions:
        None.

    Input:
       u8PortNum	- Port number of the device.
						Value passed will be less than CONFIG_PD_PORT_COUNT.

	   u8ChunkState - Chunk State machine State variable.

    Return:
        None.

    Remarks:
        This function confined to INCLUDE_PD_3_0 define.

**************************************************************************************************/
void PRL_ChunkStateChange_TimerCB (UINT8 u8PortNum, UINT8 u8ChunkState);

/**************************************************************************************************
    Function:
        void PRL_UpdateTotalChunkNumVar (UINT8 u8PortNum);

    Summary:
        This API updates Total Chunk number to be sent or received to Chunk state machine global.

    Devices Supported:
        UPD350 REV A

    Description:
		This API updates Total Chunk number to be sent or received by the chunk state machine to a global.

    Conditions:
        None.

    Input:
        u8PortNum	- Port number of the device.
						Value passed will be less than CONFIG_PD_PORT_COUNT.

    Return:
        None.

    Remarks:
        This function confined to INCLUDE_PD_3_0 define.

**************************************************************************************************/
void PRL_UpdateTotalChunkNumVar (UINT8 u8PortNum);

/**************************************************************************************************
    Function:
        UINT32 PRL_FormRequestChunkMsgHeader(UINT8 u8PortNum);

    Summary:
        This API forms & returns Combined header for Chunk request packet.

    Devices Supported:
        UPD350 REV A

    Description:
		This API forms & returns Combined header(HIWORD is Extended message header &
			LOWORD is PD message header) for Chunk request packet.

    Conditions:
        None.

    Input:
        u8PortNum	- Port number of the device.
						Value passed will be less than CONFIG_PD_PORT_COUNT.

    Return:
        UINT32 - Combined message header with Extended & PD message header.

    Remarks:
        This function confined to INCLUDE_PD_3_0 define.

**************************************************************************************************/
UINT32 PRL_FormRequestChunkMsgHeader(UINT8 u8PortNum);

/**************************************************************************************************
    Function:
        UINT32 PRL_IsAnyMsgPendinginPRL (UINT8 u8PortNum);

    Summary:
        This API returns if there are any unprocessed message pending in Protocol layer.

    Devices Supported:
        UPD350 REV A

    Description:
		This API returns combined message header of the message pending in Protocol Layer
			to be passed to Policy Engine.

    Conditions:
        None.

    Input:
        u8PortNum	- Port number of the device.
						Value passed will be less than CONFIG_PD_PORT_COUNT.

    Return:
        UINT32 - Combined message header with Extended & PD message header.

    Remarks:
        None.

**************************************************************************************************/
UINT32 PRL_IsAnyMsgPendinginPRL (UINT8 u8PortNum);

/*************************************************************************************************/
/**************************************************************************************************
    Function:
        void PRL_ConfigureBISTCarrierMode (UINT8 u8PortNum, UINT8 u8BISTCarriermode);

    Summary:
        This API enables or disables BIST carrier mode.

    Devices Supported:
        UPD350 REV A

    Description:
		This API configures or resets BIST to transmit in BIST Carrier Mode depending on u8BISTCarriermode.

    Conditions:
        None.

    Input:
        u8PortNum			- Port number of the device.
								Value passed will be less than CONFIG_PD_PORT_COUNT.
		u8BISTCarriermode 	- It takes following values,
								TRUE- Sets BIST carrier mode Tx
								FALSE - Resets BIST carrier mode

    Return:
        None.

    Remarks:
        None.

**************************************************************************************************/
void PRL_ConfigureBISTCarrierMode (UINT8 u8PortNum, UINT8 u8BISTCarriermode);

/**************************************************************************************************
    Function:
        void PRL_ChangeTxState(UINT8 u8PortNum, UINT8 u8TxStateISR);

    Summary:
        This API changes the state of the Variable gasPRL[u8PortNum].u8TxStateISR

    Devices Supported:
        UPD350 REV A

    Description:
		This API changes ISR prone variable gasPRL[u8PortNum].u8TxStateISR in foreground
        with interrupts disabled

    Conditions:
        None.

    Input:
        u8PortNum			- Port number of the device.
								Value passed will be less than CONFIG_PD_PORT_COUNT.
		u8TxStateISR 	    - TxStateISR states 
    Return:
        None.

    Remarks:
        None.

**************************************************************************************************/
void PRL_ChangeTxState(UINT8 u8PortNum, UINT8 u8TxStateISR);

/**************************************************************************************************
    Function:
        void PRL_KillCAorChunkSMTimer (UINT8 u8PortNum);

    Summary:
        This API changes kills gasChunkSM [u8PortNum].u8CAorChunkSMTimerID timer

    Devices Supported:
        UPD350 REV A

    Description:
		This API changes kills chunk state machine timer (gasChunkSM [u8PortNum].u8CAorChunkSMTimerID)

    Conditions:
        None.

    Input:
        u8PortNum			- Port number of the device.
								Value passed will be less than CONFIG_PD_PORT_COUNT.
    Return:
        None.

    Remarks:
        None.

**************************************************************************************************/
void PRL_KillCAorChunkSMTimer (UINT8 u8PortNum);

/**************************************************************************************************
    Function:
        void PRL_ProtocolReset(UINT8 u8PortNum);

    Summary:
        This API resets the PRL at Sink & SRC startup

    Devices Supported:
        UPD350 REV A

    Description:
		This API resets PRL PHY, Tx & Rx message ID counter & Tx Interrupts

    Conditions:
        None.

    Input:
        u8PortNum			- Port number of the device.
								Value passed will be less than CONFIG_PD_PORT_COUNT.
    Return:
        None.

    Remarks:
        None.

**************************************************************************************************/
void PRL_ProtocolReset(UINT8 u8PortNum);

#endif /*_PROTOCOL_LAYER_H_*/
