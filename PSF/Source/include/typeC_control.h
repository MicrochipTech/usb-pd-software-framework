/*******************************************************************************
  Type-c control Header

  Company:
    Microchip Technology Inc.

  File Name:
    typeC_control.h

  Description:
    This header file contains the function prototypes and definitions of the
    data types and constants that make up the interface to the Power delivery modules
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
*******************************************************************************/
#ifndef _TYPECCONTROL_H_
#define _TYPECCONTROL_H_

// *****************************************************************************
// *****************************************************************************
// Section: Constants
// *****************************************************************************
// *****************************************************************************

/*Base addresses of UPD350 Registers*/
#define TYPEC_CABLE_PLUG_CSR_BASE_ADDR	            0x0800

/*Addresses of Cable Plug Control and Status Registers */
#define TYPEC_CC_HW_CTL       	    (TYPEC_CABLE_PLUG_CSR_BASE_ADDR + 0x00)

/*TYPEC_CC_HW_CTL_LOW is the lower byte of 2 byte register TYPEC_CC_HW_CTL*/
#define TYPEC_CC_HW_CTL_LOW   	    (TYPEC_CABLE_PLUG_CSR_BASE_ADDR + 0x00)
/*TYPEC_CC_HW_CTL_HIGH is the higher byte of 2 byte register TYPEC_CC_HW_CTL */
#define TYPEC_CC_HW_CTL_HIGH  	    (TYPEC_CABLE_PLUG_CSR_BASE_ADDR + 0x01)

#define TYPEC_CC_INT_STS      	    (TYPEC_CABLE_PLUG_CSR_BASE_ADDR + 0x03)
#define TYPEC_CC_CHG_STS		    (TYPEC_CABLE_PLUG_CSR_BASE_ADDR + 0x04)
#define TYPEC_CC1_CHG_STS		    (TYPEC_CABLE_PLUG_CSR_BASE_ADDR + 0x04)
#define TYPEC_CC2_CHG_STS		    (TYPEC_CABLE_PLUG_CSR_BASE_ADDR + 0x05)
#define TYPEC_CC_MATCH			    (TYPEC_CABLE_PLUG_CSR_BASE_ADDR + 0x06)
#define TYPEC_CC1_MATCH			    (TYPEC_CABLE_PLUG_CSR_BASE_ADDR + 0x06)
#define TYPEC_CC2_MATCH			    (TYPEC_CABLE_PLUG_CSR_BASE_ADDR + 0x07)
#define TYPEC_VBUS_MATCH		    (TYPEC_CABLE_PLUG_CSR_BASE_ADDR + 0x08)
#define TYPEC_VBUS_CHG_STS          (TYPEC_CABLE_PLUG_CSR_BASE_ADDR + 0x09)
#define TYPEC_PWR_INT_STS		    (TYPEC_CABLE_PLUG_CSR_BASE_ADDR + 0x0A)
#define TYPEC_EXT_INT_STS           (TYPEC_CABLE_PLUG_CSR_BASE_ADDR + 0x0B)
#define TYPEC_CC_INT_EN             (TYPEC_CABLE_PLUG_CSR_BASE_ADDR + 0x11)
#define TYPEC_CC1_MATCH_EN		    (TYPEC_CABLE_PLUG_CSR_BASE_ADDR + 0x12)
#define TYPEC_CC2_MATCH_EN		    (TYPEC_CABLE_PLUG_CSR_BASE_ADDR + 0x13)
#define TYPEC_VBUS_MATCH_EN		    (TYPEC_CABLE_PLUG_CSR_BASE_ADDR + 0x14)
#define TYPEC_PWR_INT_EN		    (TYPEC_CABLE_PLUG_CSR_BASE_ADDR + 0x15)
#define TYPEC_EXT_INT_EN            (TYPEC_CABLE_PLUG_CSR_BASE_ADDR + 0x16)
#define TYPEC_MATCH_DEB			    (TYPEC_CABLE_PLUG_CSR_BASE_ADDR + 0x17)
#define TYPEC_VCONN_DEB			    (TYPEC_CABLE_PLUG_CSR_BASE_ADDR + 0x19)
#define TYPEC_CC1_DBCLR_EN		    (TYPEC_CABLE_PLUG_CSR_BASE_ADDR + 0x1A)
#define TYPEC_CC2_DBCLR_EN		    (TYPEC_CABLE_PLUG_CSR_BASE_ADDR + 0x1B)
#define TYPEC_VBUS_DBCLR_EN		    (TYPEC_CABLE_PLUG_CSR_BASE_ADDR + 0x1C)
#define TYPEC_CC1_SAMP_EN     	    (TYPEC_CABLE_PLUG_CSR_BASE_ADDR + 0x1D)
#define TYPEC_CC2_SAMP_EN     	    (TYPEC_CABLE_PLUG_CSR_BASE_ADDR + 0x1E)
#define TYPEC_VBUS_SAMP_EN    	    (TYPEC_CABLE_PLUG_CSR_BASE_ADDR + 0x1F)
#define TYPEC_CC_CTL1   			(TYPEC_CABLE_PLUG_CSR_BASE_ADDR + 0x20)

/*TYPEC_CC_CTL1_LOW is the lower byte of 2 byte register TYPEC_CC_CTL1*/
#define TYPEC_CC_CTL1_LOW			(TYPEC_CABLE_PLUG_CSR_BASE_ADDR + 0x20)
/*TYPEC_CC_CTL1_HIGH is the higher byte of 2 byte register TYPEC_CC_CTL1*/
#define TYPEC_CC_CTL1_HIGH	        (TYPEC_CABLE_PLUG_CSR_BASE_ADDR + 0x21)

#define TYPEC_CC_THR_BASE		    (TYPEC_CABLE_PLUG_CSR_BASE_ADDR + 0x22)
#define TYPEC_CC_THR0				(TYPEC_CABLE_PLUG_CSR_BASE_ADDR + 0x22)
#define TYPEC_CC_THR1				(TYPEC_CABLE_PLUG_CSR_BASE_ADDR + 0x24)
#define TYPEC_CC_THR2				(TYPEC_CABLE_PLUG_CSR_BASE_ADDR + 0x26)
#define TYPEC_CC_THR3				(TYPEC_CABLE_PLUG_CSR_BASE_ADDR + 0x28)
#define TYPEC_CC_THR4				(TYPEC_CABLE_PLUG_CSR_BASE_ADDR + 0x2A)
#define TYPEC_CC_THR5				(TYPEC_CABLE_PLUG_CSR_BASE_ADDR + 0x2C)
#define TYPEC_CC_THR6				(TYPEC_CABLE_PLUG_CSR_BASE_ADDR + 0x2E)
#define TYPEC_CC_THR7				(TYPEC_CABLE_PLUG_CSR_BASE_ADDR + 0x30)
#define TYPEC_VBUS_CTL2			    (TYPEC_CABLE_PLUG_CSR_BASE_ADDR + 0x3F)
#define TYPEC_VBUS_CTL1			    (TYPEC_CABLE_PLUG_CSR_BASE_ADDR + 0x40)

/*TYPEC_VBUS_CTL1_LOW is the lower byte of 2 byte register TYPEC_VBUS_CTL1*/
#define TYPEC_VBUS_CTL1_LOW		    (TYPEC_CABLE_PLUG_CSR_BASE_ADDR + 0x40)
/*TYPEC_VBUS_CTL1_HIGH is the higher byte of 2 byte register TYPEC_VBUS_CTL1*/
#define TYPEC_VBUS_CTL1_HIGH		(TYPEC_CABLE_PLUG_CSR_BASE_ADDR + 0x41)

#define TYPEC_VBUS_DEB			    (TYPEC_CABLE_PLUG_CSR_BASE_ADDR + 0x4A)
#define TYPEC_VBUS_THR_VSAFE0V      (TYPEC_CABLE_PLUG_CSR_BASE_ADDR + 0x4E)

#define TYPEC_CFG_SEL0_MATCH   	    (TYPEC_CABLE_PLUG_CSR_BASE_ADDR + 0x50)
#define TYPEC_CFG_SEL1_MATCH   	    (TYPEC_CABLE_PLUG_CSR_BASE_ADDR + 0x51)
#define TYPEC_CFG_SEL_THR0    	    (TYPEC_CABLE_PLUG_CSR_BASE_ADDR + 0x54)
#define TYPEC_CFG_SEL_THR1    	    (TYPEC_CABLE_PLUG_CSR_BASE_ADDR + 0x56)
#define TYPEC_CFG_SEL_THR2    	    (TYPEC_CABLE_PLUG_CSR_BASE_ADDR + 0x58)
#define TYPEC_CFG_SEL_THR3    	    (TYPEC_CABLE_PLUG_CSR_BASE_ADDR + 0x5A)
#define TYPEC_CFG_SEL_THR4    	    (TYPEC_CABLE_PLUG_CSR_BASE_ADDR + 0x5C)
#define TYPEC_CFG_SEL_THR5    	    (TYPEC_CABLE_PLUG_CSR_BASE_ADDR + 0x5E)
#define TYPEC_CFG_SEL_THR6    	    (TYPEC_CABLE_PLUG_CSR_BASE_ADDR + 0x60)
#define TYPEC_CFG_SEL_THR7    	    (TYPEC_CABLE_PLUG_CSR_BASE_ADDR + 0x62)
#define TYPEC_CFG_SEL_THR8    	    (TYPEC_CABLE_PLUG_CSR_BASE_ADDR + 0x64)
#define TYPEC_CFG_SEL_THR9    	    (TYPEC_CABLE_PLUG_CSR_BASE_ADDR + 0x66)
#define TYPEC_CFG_SEL_THR10    	    (TYPEC_CABLE_PLUG_CSR_BASE_ADDR + 0x68)
#define TYPEC_CFG_SEL_THR11    	    (TYPEC_CABLE_PLUG_CSR_BASE_ADDR + 0x6A)
#define TYPEC_CFG_SEL_THR12    	    (TYPEC_CABLE_PLUG_CSR_BASE_ADDR + 0x6C)
#define TYPEC_CFG_SEL_THR13    	    (TYPEC_CABLE_PLUG_CSR_BASE_ADDR + 0x6E)
#define TYPEC_CFG_SEL_THR14     	(TYPEC_CABLE_PLUG_CSR_BASE_ADDR + 0x70)
#define TYPEC_CFG_SEL_THR15     	(TYPEC_CABLE_PLUG_CSR_BASE_ADDR + 0x72)

#define TYPEC_DRP_CTL_LOW           (TYPEC_CABLE_PLUG_CSR_BASE_ADDR + 0x90)
#define TYPEC_DRP_CTL_HIGH          (TYPEC_CABLE_PLUG_CSR_BASE_ADDR + 0x91)
#define TYPEC_DRP_LFSR_SEED         (TYPEC_CABLE_PLUG_CSR_BASE_ADDR + 0x92)
#define TYPEC_DRP_LFSR_SEED_LOW     (TYPEC_CABLE_PLUG_CSR_BASE_ADDR + 0x92)
#define TYPEC_DRP_LFSR_SEED_HIGH    (TYPEC_CABLE_PLUG_CSR_BASE_ADDR + 0x93)
#define TYPEC_DRP_TIME              (TYPEC_CABLE_PLUG_CSR_BASE_ADDR + 0x94)
#define TYPEC_DRP_CC_SNK_MATCH_EN   (TYPEC_CABLE_PLUG_CSR_BASE_ADDR + 0x98)
#define TYPEC_DRP_CC_SRC_MATCH_EN   (TYPEC_CABLE_PLUG_CSR_BASE_ADDR + 0x99)
#define TYPEC_DRP_CC_SNK_DBCLR_EN   (TYPEC_CABLE_PLUG_CSR_BASE_ADDR + 0x9A)
#define TYPEC_DRP_DUTY_CYC          (TYPEC_CABLE_PLUG_CSR_BASE_ADDR + 0x9C)
#define TYPEC_DRP_CC_SRC_DBCLR_EN   (TYPEC_CABLE_PLUG_CSR_BASE_ADDR + 0x9B)
#define TYPEC_DRP_SNK_SAMP_EN       (TYPEC_CABLE_PLUG_CSR_BASE_ADDR + 0x9D)
#define TYPEC_DRP_SRC_SAMP_EN       (TYPEC_CABLE_PLUG_CSR_BASE_ADDR + 0x9E)

#define TYPEC_FRS_CTL               (TYPEC_CABLE_PLUG_CSR_BASE_ADDR + 0xA0)
/*TYPEC_FRS_CTL_LOW is the lower byte of 2 byte register TYPEC_FRS_CTL*/
#define TYPEC_FRS_CTL_LOW		    (TYPEC_CABLE_PLUG_CSR_BASE_ADDR + 0xA0)
/*TYPEC_FRS_CTL_HIGH is the higher byte of 2 byte register TYPEC_FRS_CTL*/
#define TYPEC_FRS_CTL_HIGH          (TYPEC_CABLE_PLUG_CSR_BASE_ADDR + 0xA1)

#define TYPEC_FRS_CC_DEB            (TYPEC_CABLE_PLUG_CSR_BASE_ADDR + 0xA2)
#define TYPEC_FRS_TX_LEN            (TYPEC_CABLE_PLUG_CSR_BASE_ADDR + 0xA4)
#define TYPEC_FRS_THR_SEL           (TYPEC_CABLE_PLUG_CSR_BASE_ADDR + 0xA6)
#define TYPEC_FRS_PIO_SEL           (TYPEC_CABLE_PLUG_CSR_BASE_ADDR + 0xA7)
#define TYPEC_FRS_VBUS_DEB          (TYPEC_CABLE_PLUG_CSR_BASE_ADDR + 0xA8)

#define TYPEC_VBUS_THR_BASE         (TYPEC_CABLE_PLUG_CSR_BASE_ADDR + 0xB0)
#define TYPEC_VBUS_THR0             (TYPEC_CABLE_PLUG_CSR_BASE_ADDR + 0xB0)
#define TYPEC_VBUS_THR1             (TYPEC_CABLE_PLUG_CSR_BASE_ADDR + 0xB2)
#define TYPEC_VBUS_THR2             (TYPEC_CABLE_PLUG_CSR_BASE_ADDR + 0xB4)
#define TYPEC_VBUS_THR3             (TYPEC_CABLE_PLUG_CSR_BASE_ADDR + 0xB6)
#define TYPEC_VBUS_THR4             (TYPEC_CABLE_PLUG_CSR_BASE_ADDR + 0xB8)

/*Bit definitions of TYPEC_CC_CTL1_LOW register*/
#define TYPEC_CC1_PULL_DOWN		    (BIT(2) | BIT(1) | BIT(0))
#define TYPEC_CC1_PULL_DOWN_POS     0
#define TYPEC_CC1_PULL_DOWN_RD      (BIT(0))
#define TYPEC_CC1_PULL_DOWN_RA      (BIT(1))
#define TYPEC_CC1_PULL_DOWN_OPEN    (BIT(1) | BIT(0))

#define TYPEC_CC2_PULL_DOWN		    (BIT(5) | BIT(4) | BIT(3))
#define TYPEC_CC2_PULL_DOWN_POS     3
#define TYPEC_CC2_PULL_DOWN_RD      (BIT(3))
#define TYPEC_CC2_PULL_DOWN_RA      (BIT(4))
#define TYPEC_CC2_PULL_DOWN_OPEN    (BIT(4) | BIT(3))

/*Bit definitions of TYPEC_CC_CTL1_HIGH register*/
#define TYPEC_CC1_RP_VAL			(BIT(1) | BIT(0))
#define TYPEC_CC2_RP_VAL			(BIT(3) | BIT(2))
#define TYPEC_CC_COM_SEL			 BIT(4)

#define TYPEC_CC1_CC2_RP_MASK       0xf0ff
#define TYPEC_CC1_CC2_RD_MASK       0xffc0

/*Bit definitions of TYPEC_CC_CTL1 register*/
#define TYPEC_CC1_RP_VAL_POS         8
#define TYPEC_CC2_RP_VAL_POS         10
#define TYPEC_CC_RP_DIS              0

#define TYPEC_CC_COMP_CTL			(BIT(6) | BIT(5))
#define TYPEC_CC_COMP_CTL_DIS        0
#define TYPEC_CC_COMP_CTL_CC1        BIT(5)
#define TYPEC_CC_COMP_CTL_CC2        BIT(6)
#define TYPEC_CC_COMP_CTL_CC1_CC2   (BIT(6) | BIT(5))

/*Bit definitions of TYPEC_CC_HW_CTL_HIGH register*/
#define TYPEC_MATCH_DB_UNITS		BIT(3)
#define TYPEC_CC_DB_ACTIVE		    BIT(1)
#define TYPEC_BLK_PD_MSG			BIT(4)

/*Bit definitions of TYPEC_CC_HW_CTL_LOW register*/
#define TYPEC_MODE	                BIT(0)
#define TYPEC_CC_DET_HBW_EN         BIT(1)
#define TYPEC_DEV_ROLE			    BIT(2)

/*Bit definitions of CC_INT_STS register*/
#define TYPEC_CC1_MATCH_CHG		    BIT(0)
#define TYPEC_CC2_MATCH_CHG		    BIT(1)
#define TYPEC_CC_MATCH_VLD		    BIT(7)

/*Bit definitions for DRP_CTL_HIGH register*/
#define TYPEC_DRP_ADVERTISING_STATE BIT(0)
#define TYPEC_DRP_VSAFE0V_EN        BIT(2)

/*Definitions for DRP_CTL_LOW register*/
#define TYPEC_DRP_RP_POS                2
#define TYPEC_DRP_EN                    BIT(0)
#define TYPEC_LFSR_EN                   BIT(1)
#define TYPEC_DRP_DONE                  BIT(2)
#define TYPEC_DRP_PD_VAL_TRIMMED_RD     BIT(4)
#define TYPEC_DRP_PD_VAL_OPEN_DIS       (BIT(4) | BIT(5))
#define TYPEC_DRP_RP_VAL_MASK           (BIT(2)|BIT(3))
#define TYPEC_DRP_PD_VAL_MASK           (BIT(6)|BIT(5)|BIT(4))

/*Bit definitions of PWR_INT_STS register*/
#define TYPEC_VBUS_MATCH_VLD		BIT(7)
#define TYPEC_VCONN_OVER_CURR_ERR   BIT(0)

/*Bit definitions of TYPEC_VBUS_CTL1_LOW register*/
#define TYPEC_VBUS_CMP_CTL		   (BIT(1) | BIT(0))
#define TYPEC_VBUS_DET			    BIT(0)
#define TYPEC_VCONN_OCS_ENABLE      BIT(6)
#define TYPEC_VBUS_DB_ACTIVE	    BIT(7)

/*Bit definitions of TYPEC_VBUS_CTL1_HIGH register*/
#define TYPEC_VBUS_DB_UNITS		    BIT(6)
#define TYPEC_VBUS_DET_HBW_EN       BIT(7)

/*Bit definitions of TYPEC_VBUS_CTL2 register*/
#define TYPEC_VBUS_DEB_BLK_EN		BIT(0)
#define TYPEC_VBUS_DEB_TO_EN        BIT(1)

/*Bit definitions of TYPEC_EXT_INT_EN register */
#define TYPEC_CFG_SEL_DONE          BIT(0)
/* Bit 1 is reserved */
#define TYPEC_VBUS_DISCH            BIT(2)
#define TYPEC_VCONN_DISCH_STS       BIT(3)
#define TYPEC_VBUS_DB_TO            BIT(4)
#define TYPEC_FRS_RCV_STS           BIT(5)
#define TYPEC_FRS_XMT_STS           BIT(6)
/* Bit 7 is reserved */

/*Bit definitions of TYPEC_FRS_CTL_LOW register*/
#define TYPEC_FRS_CC_SEL_CC1        0
#define TYPEC_FRS_CC_SEL_CC2        BIT(4)
#define TYPEC_FRS_CC_SEL_CC1_CC2    BIT(5)
#define TYPEC_FRS_CC_SEL            (BIT(4) | BIT(5))

/*Bit definitions of TYPEC_FRS_CTL_HIGH register*/
#define TYPEC_FRS_PD_MAC_OVR        BIT(0)
#define TYPEC_FRS_REQ_VAL           BIT(4)
#define TYPEC_FRS_REQ_PIO           BIT(5)
#define TYPEC_FRS_DET_EN            BIT(6)
#define TYPEC_FRS_REQ_EN            BIT(7)

/*Defines for each of the CC_THRX values as given in UPD350 DOS*/
#define TYPEC_CC_THR_VAL_VCONN_DIS		    41
#define TYPEC_CC_THR0_VAL		            55
#define TYPEC_CC_THR1_VAL		            109
#define TYPEC_CC_THR2_VAL		            180
#define TYPEC_CC_THR3_VAL		            219
#define TYPEC_CC_THR4_VAL		            336
#define TYPEC_CC_THR5_VAL		            437
#define TYPEC_CC_THR6_VAL		            710
#define TYPEC_CC_THR7_VAL		            820

/*Bit definitions for VBUS_MATCH register*/
#define TYPEC_VBUS_VSAFE0V_MATCH     BIT(0)         
#define TYPEC_VBUS_THRES0_MATCH      BIT(2)
#define TYPEC_VBUS_THRES1_MATCH      BIT(3)
#define TYPEC_VBUS_THRES2_MATCH      BIT(4)
#define TYPEC_VBUS_THRES3_MATCH      BIT(5)
#define TYPEC_VBUS_THRES4_MATCH      BIT(6)

/*Bit definitions for CC_MATCH register*/       
#define TYPEC_CC_THRES0_MATCH      BIT(0)
#define TYPEC_CC_THRES1_MATCH      BIT(1)
#define TYPEC_CC_THRES2_MATCH      BIT(2)
#define TYPEC_CC_THRES3_MATCH      BIT(3)
#define TYPEC_CC_THRES4_MATCH      BIT(4)
#define TYPEC_CC_THRES5_MATCH      BIT(5)
#define TYPEC_CC_THRES6_MATCH      BIT(6)
#define TYPEC_CC_THRES7_MATCH      BIT(7)

/*Defines holding CC THRES match for different types of UFP attach and powered cable attach event for DFP */
/*TYPEC_DFP_ACT_DEF defines the Active cable attach event for DFP having default current as Type C current*/
/*TYPEC_DFP_UFP_1A5 defines the UFP attach event for DFP having 1.5A current as Type C current*/
#define TYPEC_DFP_ACT_DEF	        TYPEC_CC_THRES0_MATCH 
#define TYPEC_DFP_ACT_1A5		    TYPEC_CC_THRES1_MATCH  
#define TYPEC_DFP_ACT_3A0		    TYPEC_CC_THRES3_MATCH  
#define TYPEC_DFP_UFP_DEF		    TYPEC_CC_THRES5_MATCH 
#define TYPEC_DFP_UFP_1A5		    TYPEC_CC_THRES5_MATCH 
#define TYPEC_DFP_UFP_3A0		    TYPEC_CC_THRES6_MATCH 

/*Defines holding CC THRES match values for different types of DFP attach event for UFP */
/*TYPEC_UFP_DFP_DEF defines the DFP with default current as Type C current attach event for UFP*/
/*TYPEC_UFP_DFP_3A0 defines the DFP with 3.0A as Type C current attach event for UFP*/
#define TYPEC_UFP_DFP_DEF		    TYPEC_CC_THRES0_MATCH  
#define TYPEC_UFP_DFP_1A5			TYPEC_CC_THRES2_MATCH    
#define TYPEC_UFP_DFP_3A0			TYPEC_CC_THRES4_MATCH  

/*Defines holding CC THRES match reflected in CC match register for different types of UFP attach 
and powered cable attach event for DFP when the CC threshold match register is set to detect both 
powered cable and UFP*/
#define TYPEC_PWD_CABLE_ATT_DEF    (TYPEC_DFP_UFP_DEF | TYPEC_DFP_ACT_DEF )		
#define TYPEC_PWD_CABLE_ATT_1A5	   (TYPEC_DFP_UFP_1A5 | TYPEC_DFP_ACT_1A5 )       
#define TYPEC_PWD_CABLE_ATT_3A0	   (TYPEC_DFP_UFP_3A0 | TYPEC_DFP_ACT_3A0 )
#define TYPEC_UFP_ATT_DEF		    TYPEC_DFP_UFP_DEF
#define TYPEC_UFP_ATT_1A5	        TYPEC_DFP_UFP_1A5
#define TYPEC_UFP_ATT_3A0	        TYPEC_DFP_UFP_3A0
#define TYPEC_CC1CC2_EQ	            0x00
#define TYPEC_NO_THRESHOLD	        0x00

/*Defines holding CC THRES matches reflected in CC match register for different types of DFP attach 
event for UFP*/
#define TYPEC_DFP_DEF_ATT		            TYPEC_UFP_DFP_DEF  
#define TYPEC_DFP_1A5_ATT			        (TYPEC_UFP_DFP_DEF | TYPEC_UFP_DFP_1A5)    
#define TYPEC_DFP_3A0_ATT			        (TYPEC_UFP_DFP_DEF | TYPEC_UFP_DFP_1A5 | TYPEC_UFP_DFP_3A0)
#define TYPEC_SNK_CCTHRES_VCONN_ON          (TYPEC_DFP_3A0_ATT | TYPEC_CC_THRES7_MATCH)

/**************************VBUS Threshold match values*******************/
/*  VBUS threshold configuration
    Threshold 0 - vSinkDisconnect 
    Threshold 1 - Desired voltage level acceptable for given PDO voltage
	Threshold 2 - Over voltage corresponding to PDO voltage
	Threshold 3 - under voltage corresponding to PDO voltage
    Threshold 4 is enabled when voltage is driven from higher to lower.
                configured to Desired voltage's max accepted value*/
/* on VSafe0V Match value will be */
#define TYPEC_VSAFE_0V_MATCH_VAL    		0x0

/* VBUS match value for Desired voltage*/
#define TYPEC_VBUS_DESIRED_V_MATCH_VAL    \
(TYPEC_VSAFE0V_MAX_THR_MATCH | TYPEC_VSINKDISCONNECT_THR0_MATCH | TYPEC_DESIRED_MIN_V_THR1_MATCH)

/* VBUS match value when Power fault Threshold are enabled*/
#define TYPEC_VBUS_DESIRED_N_UNDER_MATCH_VAL  \
    (TYPEC_VSAFE0V_MAX_THR_MATCH | TYPEC_VSINKDISCONNECT_THR0_MATCH | \
      TYPEC_DESIRED_MIN_V_THR1_MATCH | TYPEC_UNDER_VOLT_THR3_MATCH)

/* vSinkDisconnect match value, this value is set when under-voltage is hit*/      
#define TYPEC_VBUS_SNKDISCNT_MATCH_VAL \
      (TYPEC_VSAFE0V_MAX_THR_MATCH | TYPEC_VSINKDISCONNECT_THR0_MATCH)

/* vSafe 0V max threshold max value*/        
#define TYPEC_MAX_VSAFE_0V_MATCH_VAL       TYPEC_VSAFE0V_MAX_THR_MATCH
        
/* VBUS Match bit set when overvoltage is detected*/        
#define TYPEC_VBUS_OVERVOLT_MATCH_BIT      TYPEC_OVER_VOLT_THR2_MATCH

/**************************VBUS Match redefines as per usage*******************/
#define TYPEC_VSAFE0V_MAX_THR_MATCH              TYPEC_VBUS_VSAFE0V_MATCH             
#define TYPEC_VSINKDISCONNECT_THR0_MATCH         TYPEC_VBUS_THRES0_MATCH
#define TYPEC_DESIRED_MIN_V_THR1_MATCH           TYPEC_VBUS_THRES1_MATCH
#define TYPEC_OVER_VOLT_THR2_MATCH               TYPEC_VBUS_THRES2_MATCH
#define TYPEC_UNDER_VOLT_THR3_MATCH              TYPEC_VBUS_THRES3_MATCH
#define TYPEC_PREV_V_DROP_CHK_THR4_MATCH         TYPEC_VBUS_THRES4_MATCH


/*****************************VBUS THRESHOLD Defines*******************/
/* VBUS_THR_UNITS (2.5V/1024) * 1R_BY_9R_DIVIDER (10)*/
#define TYPEC_VBUS_THRX_UNITS_MILLI_V			24.41

/* Vsafe0V valid range is 0V to 0.8V*/        
/* VBUS Threshold for VSAFE0V configured for 0.68V*/
#define TYPEC_VSAFE0V_VBUS_THR	    				28

/* VBUS Threshold for vSinkDisconnect */
#define TYPEC_VSINKDISCONNECT_THR                   \
            (UINT16)(TYPEC_VSINKDISCONNECT_VOLTAGE / TYPEC_VBUS_THRX_UNITS_MILLI_V)

/* VBUS Threshold for Vsafe5V for Source*/
#define TYPEC_DESIRED_MIN_SRC_VSAFE5V_VBUS_THR			\
			(TYPEC_SRC_VSAFE5V_DESIRED_MIN_VOLTAGE / TYPEC_VBUS_THRX_UNITS_MILLI_V)

#define TYPEC_DESIRED_MAX_SRC_VSAFE5V_VBUS_THR			\
			(TYPEC_SRC_VSAFE5V_DESIRED_MAX_VOLTAGE / TYPEC_VBUS_THRX_UNITS_MILLI_V)
              
/* VBUS Threshold for Vsafe5V for sink*/
#define TYPEC_DESIRED_MIN_SNK_VSAFE5V_VBUS_THR			\
			(TYPEC_SNK_VSAFE5V_DESIRED_MIN_VOLTAGE / TYPEC_VBUS_THRX_UNITS_MILLI_V)

#define TYPEC_DESIRED_MAX_SNK_VSAFE5V_VBUS_THR			\
			(TYPEC_SNK_VSAFE5V_DESIRED_MAX_VOLTAGE / TYPEC_VBUS_THRX_UNITS_MILLI_V)              
			  
/* As per PD Spec, For fixed PDO voltage range is 0.95 * PDO voltage to 
    1.05* PDO voltage */
/* the desired minimum voltage level is 0.95 * PDO voltage*/
#define TYPEC_DESIRED_MIN_PDO_VOLTAGE_FACTOR 	0.95
/* the desired maximum voltage level is 1.05 * PDO voltage*/
#define TYPEC_DESIRED_MAX_PDO_VOLTAGE_FACTOR 	1.05
			  
/* To get desired VBUS threshold value for minimum voltage range of a PDO voltage*/
/* It gives 8.5V for 9V detection
			13.8V for 15V detection
			18.5V for 20V detection*/
#define TYPEC_GET_DESIRED_MIN_VBUS_THR(u16voltagemV)	\
			((u16voltagemV * TYPEC_DESIRED_MIN_PDO_VOLTAGE_FACTOR) / \
			  TYPEC_VBUS_THRX_UNITS_MILLI_V)

/* To get desired VBUS threshold value for maximum voltage range of a PDO voltage*/		
#define TYPEC_GET_DESIRED_MAX_VBUS_THR(u16voltagemV)		\
				((u16voltagemV * TYPEC_DESIRED_MAX_PDO_VOLTAGE_FACTOR) / \
				  TYPEC_VBUS_THRX_UNITS_MILLI_V )

#if (TRUE == INCLUDE_POWER_FAULT_HANDLING)
/* To get over voltage VBUS threshold*/			
#define TYPEC_GET_OVER_VOLTAGE_VBUS_THR(u16voltagemV)		\
				 ((u16voltagemV * (gasCfgStatusData.sPerPortData[u8PortNum].u8OVThresholdPercentage / (float)100)) \
				  / TYPEC_VBUS_THRX_UNITS_MILLI_V)

/* To get under voltage VBUS threshold*/                  
#define TYPEC_GET_UNDER_VOLTAGE_VBUS_THR(u16voltagemV)		\
			     ((u16voltagemV * (gasCfgStatusData.sPerPortData[u8PortNum].u8UVThresholdPercentage / (float)100)) \
					/ TYPEC_VBUS_THRX_UNITS_MILLI_V)

#endif /* end of INCLUDE_POWER_FAULT_HANDLING*/
/**************************************************************************************/                

/*Bit definitions for gasTypeCcontrol[u8PortNum].u8DRPStsISR variable*/
#define TYPEC_DRP_DONE_INTERRUPT                    BIT(0)
#define TYPEC_FRS_XMT_RCV_STS_INTERRUPT             BIT(1)

/*Defines for different Type C current values of DFP*/
#define TYPEC_UFP                               PD_ROLE_UFP
#define TYPEC_DFP_DEFAULT_CURRENT				(0x01)
#define TYPEC_DFP_1A5_CURRENT				    (0x02)
#define TYPEC_DFP_3A0_CURRENT				    (0x03)

/*Defines for Number of active CC THRES to be sampled for Source and Sink*/
#define TYPEC_SRC_CCTHRES_CNT      2
#define TYPEC_SNK_CCTHRES_CNT      3
#define TYPEC_DRP_CCTHRES_CNT      TYPEC_SNK_CCTHRES_CNT

/*Defines for Number of active VBUS THRES to be sampled for Source and Sink*/
#define TYPEC_SRC_VBUSTHRES_CNT    5
#define TYPEC_SNK_VBUSTHRES_CNT    5


/*Defines that can be passed as an argument for variable "u8CCEnablePins" for 
TypeC_SetCCSampleEnable and "u8CCPin" argument of TypeC_SetCCPowerRole APIs */
#define TYPEC_ENABLE_CC1           0x01
#define TYPEC_ENABLE_CC2           0x02
#define TYPEC_ENABLE_CC1_CC2       (TYPEC_ENABLE_CC1 | TYPEC_ENABLE_CC2)

/*Defines that can be passed as an argument for variable "u8ConfigVal" for 
TypeC_SetVBUSCompONOFF API*/
#define TYPEC_VBUSCOMP_OFF                        0
#define TYPEC_VBUSCOMP_ON                         1

/*Defines that can be passed as an argument for variable "u8EnableDisable" in 
TypeC_EnabDisVCONN API */
#define TYPEC_VCONN1_ENABLE		    BIT(2)
#define TYPEC_VCONN2_ENABLE		    BIT(3)
#define TYPEC_VCONN_ENABLE          1
#define TYPEC_VCONN_DISABLE 		0
#define TYPEC_VCONN_FETS    	   (BIT(3) | BIT(2))

/*Defines that can be passed as an argument for variable "u8ConfigVal" for 
TypeC_SetCCPowerRole API for Source*/
#define TYPEC_ROLE_SOURCE_OPEN_DIS    0
#define TYPEC_ROLE_SOURCE_DC          1
#define TYPEC_ROLE_SOURCE_15          2
#define TYPEC_ROLE_SOURCE_30          3

/*Defines that can be passed as an argument for variable "u8ConfigVal" for 
TypeC_SetCCPowerRole API for Sink*/
#define TYPEC_ROLE_SINK_RD            1
#define TYPEC_ROLE_SINK_OPEN_DIS      3

/*Defines that can be passed as an argument for variable "u8PowerRole" for TypeC_SetCCPowerRole API*/
#define TYPEC_ROLE_SINK                0
#define TYPEC_ROLE_SOURCE              1

/*Defines that can be passed as an argument for variable "u8RpValue" for 
TypeC_SetRpCollAvoidance API*/
#define TYPEC_SINK_TXOK   0
#define TYPEC_SINK_TXNG   1 

/* define for TypeC_ConfigureVBUSThr API's parameter u8PowerFaultThrConfig*/
#define TYPEC_CONFIG_PWR_FAULT_THR		1
#define TYPEC_CONFIG_NON_PWR_FAULT_THR	0

/*Defines for TYPE C State machine states*/
#define TYPEC_UNATTACHED_SRC                    0
#define TYPEC_ATTACHWAIT_SRC                    1
#define TYPEC_ATTACHED_SRC                      2
#define TYPEC_UNATTACHED_WAIT_SRC               3
#define TYPEC_UNORIENTED_DEBUG_ACCESSORY_SRC    4
#define TYPEC_UNATTACHED_SNK                    5
#define TYPEC_ATTACHWAIT_SNK                    6
#define TYPEC_ATTACHED_SNK                      7
#define TYPEC_DEBUG_ACCESSORY_SNK               8
#define TYPEC_ERROR_RECOVERY                    9
#define TYPEC_AUDIO_ACCESSORY                  10
#define TYPEC_DISABLED                         11 
#define TYPEC_INVALID_STATE                    12

/*Setting invalid Substate as maximum value of UINT8*/
#define TYPEC_INVALID_SUBSTATE                 255

/*Defines for TYPEC_UNATTACHED_SRC's substates in TYPE C SM*/  
#define TYPEC_UNATTACHED_SRC_ENTRY_SS                 0
#define TYPEC_UNATTACHED_SRC_INIT_SS				  1
#define TYPEC_UNATTACHED_SRC_IDLE_SS                  2
#define TYPEC_UNATTACHED_SRC_INIT_VSAFE0V_SS          3
#define TYPEC_UNATTACHED_SRC_WAIT_DRPDONE_SS          4

/*Defines for TYPEC_ATTACHWAIT_SRC's substates in TYPE C SM*/
#define TYPEC_ATTACHWAIT_SRC_ENTRY_SS                 0
#define TYPEC_ATTACHWAIT_SRC_IDLE_SS                  1
#define TYPEC_ATTACHWAIT_SRC_CC_DEB_TIMEOUT_SS        2
    
/*Defines for TYPEC_ATTACHED_SRC's substates in TYPE C SM*/
#define TYPEC_ATTACHED_SRC_SWAP_ASSERT_RP_SS           0
#define TYPEC_ATTACHED_SRC_SWAP_RD_PRES_DETECT_SS      1
#define TYPEC_ATTACHED_SRC_DRIVE_PWR_SS               2
#define TYPEC_ATTACHED_SRC_WAIT_FOR_VBUS_ON_SS        3
#define TYPEC_ATTACHED_SRC_WAIT_FOR_VCONN_ON_SS       4
#define TYPEC_ATTACHED_SRC_SET_PRL_SS                 5
#define TYPEC_ATTACHED_SRC_RUN_SM_SS                  6
#define TYPEC_ATTACHED_SRC_IDLE_SS				      7
#define TYPEC_ATTACHED_SRC_SWAP_TRANS_TO_SNK_SS        8

/*Defines for TYPEC_UNATTACHED_WAIT_SRC's substates in TYPE C SM*/ 
#define TYPEC_UNATTACHED_WAIT_SRC_ENTRY_SS                0
#define TYPEC_UNATTACHED_WAIT_SRC_PD_DEB_TIMEOUT_SS       1
#define TYPEC_UNATTACHED_WAIT_SRC_IDLE_SS                 2
#define TYPEC_UNATTACHED_WAIT_SRC_WAIT_FOR_VBUS_OFF_SS    3
#define TYPEC_UNATTACHED_WAIT_SRC_WAIT_FOR_VCONN_OFF_SS   4 

/*Defines for TYPEC_UNATTACHED_SNK's substates in TYPE C SM*/
#define TYPEC_UNATTACHED_SNK_ENTRY_SS      	        0
#define TYPEC_UNATTACHED_SNK_WAIT_FOR_VSAFE0V_SS    1
#define TYPEC_UNATTACHED_SNK_INIT_SS	            2					
#define TYPEC_UNATTACHED_SNK_IDLE_SS                3

/*Defines for TYPEC_ATTACHWAIT_SNK's substates in TYPE C SM*/  
#define TYPEC_ATTACHWAIT_SNK_ENTRY_SS                 0
#define TYPEC_ATTACHWAIT_SNK_IDLE_SS                  1
#define TYPEC_ATTACHWAIT_SNK_START_PD_DEB_SS          2
#define TYPEC_ATTACHWAIT_SNK_CC_DEB_TIMEOUT_SS        3

/*Defines for TYPEC_ATTACHED_SNK's substates in TYPE C SM*/
#define TYPEC_ATTACHED_SNK_SWAP_ASSERT_RD_SS            0
#define TYPEC_ATTACHED_SNK_SWAP_VBUS_PRES_DETECT_SS     1
#define TYPEC_ATTACHED_SNK_SWAP_ATTACH_DETECT_SS        2
#define TYPEC_ATTACHED_SNK_ENTRY_SS                     3
#define TYPEC_ATTACHED_SNK_RUN_SM_SS                    4
#define TYPEC_ATTACHED_SNK_START_PD_DEB_SS              5
#define TYPEC_ATTACHED_SNK_PD_DEB_TIMEOUT_SS            6
#define TYPEC_ATTACHED_SNK_IDLE_SS                      7
#define TYPEC_ATTACHED_SNK_SWAP_TRANS_TO_SRC_SS         8

/*Defines for TYPEC_ERROR_RECOVERY's substates in TYPE C SM*/ 
#define TYPEC_ERROR_RECOVERY_ENTRY_SS                   0
#define TYPEC_ERROR_RECOVERY_WAIT_FOR_VCONN_OFF_SS      1
#define TYPEC_ERROR_RECOVERY_WAIT_FOR_VBUS_OFF_SS       2
#define TYPEC_ERROR_RECOVERY_IDLE_SS                    3
#define TYPEC_ERROR_RECOVERY_TIMEOUT_SS                 4
    
/*Defines for TYPEC_DISABLED state's sub-states in TYPE C SM*/
#define TYPEC_DISABLED_ENTRY_SS                         0
#define TYPEC_DISABLED_WAIT_FOR_VCONN_OFF_SS            1
#define TYPEC_DISABLED_WAIT_FOR_VBUS_OFF_SS             2
#define TYPEC_DISABLED_IDLE_SS                          3 
#define TYPEC_DISABLED_TIMEOUT_SS                       4
#define TYPEC_DISABLED_DONE_SS                          5
/*************************************************************/

/*Defines for setting Rp value of source*/
#define TYPEC_RP_DISABLED             0
#define TYPEC_RP_DEFAULT_CURRENT      1
#define TYPEC_RP_CURRENT_15           2
#define TYPEC_RP_CURRENT_30           3

/* Defines for CC Orientation */
#define TYPEC_ORIENTATION_CC1         0
#define TYPEC_ORIENTATION_CC2         1


/*************************************************************************************/

/*Masks used For getting powered cable presence status from u8PortSts variable*/ 
#define TYPEC_PWDCABLE_PRES_MASK       	 BIT(0)

/*Masks used For getting Current Rp value from u8PortSts variable*/
#define TYPEC_CURR_RPVAL_MASK	        (BIT(2) | BIT(1))
#define TYPEC_CURR_RPVAL_POS	        1
/*Defines for setting Rp value of source*/
#define TYPEC_RP_DISABLED             0
#define TYPEC_RP_DEFAULT_CURRENT      1
#define TYPEC_RP_CURRENT_15           2
#define TYPEC_RP_CURRENT_30           3

/*Masks used to get the collision avoidance status of policy engine(For Sink Configuration) 
from u8PortSts variable*/
#define TYPEC_COLLISION_AVOIDANCE_ACT	BIT(3)
#define TYPEC_COLLISION_AVOIDANCE_POS    3   
            
/*Masks used for getting whether VCONN discharge is in progress from u8PortSts variable*/ 
#define TYPEC_VCONN_DISCHARGE_ON_MASK	 BIT(4)

/*Masks used for getting whether VCONN ON process has initiated from u8PortSts variable*/ 
#define TYPEC_VCONN1_ON_REQ	         BIT(5)
#define TYPEC_VCONN2_ON_REQ	         BIT(6)
#define TYPEC_VCONN_ON_REQ_MASK	    (BIT(6) | BIT(5))

/* Mask to indicate current CC pin Orientation in u8PortSts variable */
#define TYPEC_CC_ATTACHED_ORIENTATION_MASK  BIT(7)
#define TYPEC_CC_ATTACHED_ORIENTATION_POS   7 

/* Define for getting the CC orientation status from u8PortSts variable */
#define TYPEC_GET_CC_ORIENTATION_STS(u8PortNum) ((gasTypeCcontrol[u8PortNum].u8PortSts & TYPEC_CC_ATTACHED_ORIENTATION_MASK) \
                                                    >> TYPEC_CC_ATTACHED_ORIENTATION_POS)
/*************************************************************************************/         
/*************************************************************************************/

/*Masks used For getting VBUS interrupt status from u8IntStsISR variable*/ 
#define TYPEC_VSINKDISCONNECT_STATUS_MASK	   BIT(0)

/*Masks used For getting CC interrupt status from u8IntStsISR variable*/ 
#define TYPEC_CCINT_STATUS_MASK                BIT(1)

/*Masks used For getting VCONN status from u8IntStsISR variable*/
#define TYPEC_VCONN_SOURCE_CC1                 BIT(2)
#define TYPEC_VCONN_SOURCE_CC2                 BIT(3)

#define TYPEC_VCONN_SOURCE_POS                 2
#define TYPEC_VCONN_SOURCE_MASK                (BIT(3) | BIT(2))
#define TYPEC_VCONN_DISABLED                   0

/*Masks used For getting VBUS value(0V or 5V) status from u8IntStsISR variable*/ 
#define TYPEC_VBUS_PRESENCE_MASK               (BIT(4) | BIT(5) | BIT(6))
#define TYPEC_VBUS_PRESENCE_POS                4

#define TYPEC_VBUS_0V_PRES                     0
#define TYPEC_VBUS_5V_PRES                     BIT(4)
                
#define TYPEC_VBUS_0V                          0
#define TYPEC_VBUS_5V                          5000

/*************************************************************************************/

/**********************VSAFE5V range for Source and Sink*******************/

/*  TYPEC_SRC_VSAFE5V_DESIRED_MAX_VOLTAGE is maximum voltage acceptable for
    VSafe5V expressed in terms of millivolts for source. The voltage will be considered as valid 
    Vsafe5V only if it is equal to or greater than TYPEC_SRC_VSAFE5V_DESIRED_MIN_VOLTAGE & less 
    than TYPEC_SRC_VSAFE5V_DESIRED_MAX_VOLTAGE.    
    (TYPEC_GET_OVER_VOLTAGE_VBUS_THR * 5000mV) will be considered as overvoltage for Vsafe5V for Source. 
    This parameter shall never be set to 0. */
#define TYPEC_SRC_VSAFE5V_DESIRED_MAX_VOLTAGE 		5500

/*  TYPEC_SRC_VSAFE5V_DESIRED_MIN_VOLTAGE is minimum voltage acceptable for VSafe5V expressed in 
    terms of millivolts for source. The voltage will be considered as valid Vsafe5V only if it is
    equal to or greater than TYPEC_SRC_VSAFE5V_DESIRED_MIN_VOLTAGE and less than 
    TYPEC_SRC_VSAFE5V_DESIRED_MAX_VOLTAGE. 
    This parameter shall never be set to 0. */
#define TYPEC_SRC_VSAFE5V_DESIRED_MIN_VOLTAGE 		4750

/*  TYPEC_SNK_VSAFE5V_DESIRED_MAX_VOLTAGE is maximum voltage acceptable for VSafe5V expressed 
    in terms of millivolts for sink. The voltage will be considered as valid Vsafe5V only if it 
    is equal to or greater than TYPEC_SNK_VSAFE5V_DESIRED_MIN_VOLTAGE and less than
    TYPEC_SNK_VSAFE5V_DESIRED_MAX_VOLTAGE. (TYPEC_GET_OVER_VOLTAGE_VBUS_THR * 5000mV) will be
    considered as overvoltage for vSafe5V for sink. 
    This parameter shall never be set to 0. */
#define TYPEC_SNK_VSAFE5V_DESIRED_MAX_VOLTAGE 		5500

/*  TYPEC_SNK_VSAFE5V_DESIRED_MIN_VOLTAGE is minimum voltage acceptable for VSafe5V expressed 
    in terms of millivolts for Sink. The voltage will be considered as valid Vsafe5V only if it is
    equal to or greater than TYPEC_SNK_VSAFE5V_DESIRED_MIN_VOLTAGE and less than
    TYPEC_SNK_VSAFE5V_DESIRED_MAX_VOLTAGE. 
    This parameter shall never be set to 0. */
#define TYPEC_SNK_VSAFE5V_DESIRED_MIN_VOLTAGE 		4400

/*  TYPEC_VSINKDISCONNECT_VOLTAGE is the vSinkDisconnect mentioned in Type c specification v1.3.
    Specification defines it as threshold used for transition from Attached.SNK to Unattached.SNK.
    In PSF, TYPEC_VSINKDISCONNECT_VOLTAGE is considered as under-voltage for Vsafe5V in case of 
    source. For Sink, if the voltage is below TYPEC_VSINKDISCONNECT_VOLTAGE, it is considered 
    as VBUS disconnect.
    This parameter shall never be set to 0. */
#define TYPEC_VSINKDISCONNECT_VOLTAGE              3670

/*************************Fast Role Swap Defines ************************/
#define TYPEC_FRS_CC_DEB_30US                 0x0F 

#define TYPEC_FRS_VBUS_DEB_30US               0x0F 

#define TYPEC_FRS_THRESHOLD                   0x00 

#define TYPEC_FRS_TX_LEN_90US                 90            

#define TYPEC_FRS_REQ_PIO_SEL_POS             4 
/*************************************************************************************/

// *****************************************************************************
// *****************************************************************************
// Section: Data Structures
// *****************************************************************************
// *****************************************************************************
/*****************************************************************************
Summary:
    Stores the Type C Port information.

  Description:
    This Structure holds the CC1,CC2,VBUS match register value.It also Stores the CC Debounce match 
    value programmed ,CC interrupt occurrence,VBUS interrupt occurrence, Powered cable presence status

  Remarks:
    Need to be packed always based on type of microcontroller.
*/
typedef struct MCHP_PSF_STRUCT_PACKED_START _TypeCcontrol 
{
  UINT8 u8TypeCState;
  UINT8 u8TypeCSubState;
  UINT8	u8CC1MatchISR;
  UINT8	u8CC2MatchISR;
  UINT8 u8CCDebMatch;
  UINT8 u8CCSrcSnkMatch;
  UINT8 u8TypeCTimerID;
  UINT8 u8PortSts;              /*BIT0 - > TYPEC_PWDCABLE_PRES Field
                                BIT[2:1] -> CURR_RPVAL Field
                                BIT3 -> COLLISION_AVOIDANCE_ACT
                                BIT4 -> VCONN_DISCHARGE_ON
                                BIT5 -> VCONN1_ON_REQ
                                BIT6 -> TYPEC_VCONN2_ON
                                BIT7 -> CC Pin Orientation */
  UINT8 u8IntStsISR;            /*BIT0 -> VSINKDISCONNECT_STATUS
                                BIT1 -> CCINT_STATUS
                                BIT2 -> VCONN_SOURCE_CC1
                                BIT3 -> VCONN_SOURCE_CC2
                                BIT[6:4] -> VBUS_PRESENCE */
#if(TRUE == INCLUDE_PD_DRP)
  UINT8 u8DRPStsISR ;           /*BIT 0 -> DRP_DONE interrupt status
                                  BIT 1 -> FRS XMT or RCV Status */
  UINT8 u8DRPLastAttachedState; /*BIT[1:0] -> Previous DRP attached power role*/
#endif
  float fVBUSCorrectionFactor;
  
}MCHP_PSF_STRUCT_PACKED_END TYPEC_CONTROL;

// *****************************************************************************
// *****************************************************************************
// Section: Interface Routines
// *****************************************************************************
// *****************************************************************************

/**************************************************************************************************
    Function:
        void TypeC_InitPort (UINT8 u8PortNum);

    Summary:
        This API initializes a given Port to the Power Role and Data role based on the port's global variable

    Devices Supported:
        UPD350 REV A

    Description:
        Port initialization of Power and data role is done based on the variable 
        "gasCfgStatusData[u8PortNum].u32CfgData"

    Conditions:
        This API is called inside the PD Stack initialization API call .

    Input:
        u8PortNum - Port Number.

    Return:
        None.

    Remarks:
        None.

**************************************************************************************************/
void TypeC_InitPort (UINT8 u8PortNum);

/**************************************************************************************************
    Function:
        void TypeC_InitDRPPort (UINT8 u8PortNum);

    Summary:
        This API initializes a given Port to support Dual role power.

    Devices Supported:
        UPD350 REV A

    Description:
        Port initialization of Power and data role is done based on the variable 
        "gasCfgStatusData[u8PortNum].u32CfgData"

    Conditions:
        This API is called inside the PD Stack initialization API call .

    Input:
        u8PortNum - Port Number.

    Return:
        None.

    Remarks:
        None.

**************************************************************************************************/
void TypeC_InitDRPPort(UINT8 u8PortNum);

/**************************************************************************************************

 Function:
        void TypeC_ConfigCCComp(UINT8 u8PortNum,UINT8 u8ConfigVal);

    Summary:
        This API is used to configure the CC Comparator to enable or disable sampling of CC pins

    Devices Supported:
        UPD350 REV A

    Description:
        This API is used to configure the CC Comparator to disable sampling of CC pins.
        It can be used to enable sampling of both the CC pins or either of the CC pins

    Conditions:
        None.

    Input:
        u8PortNum - Port Number.
        u8ConfigVal - Defines that describe whether to sample CC1 or CC2 or both
                    - TYPEC_CC_COMP_CTL_DIS (To disable CC Sampling)
                    - TYPEC_CC_COMP_CTL_CC1 (To enable CC1 Sampling only
                    - TYPEC_CC_COMP_CTL_CC2 (To enable CC2 Sampling only)
                    - TYPEC_CC_COMP_CTL_CC1_CC2 (To enable both CC1 and CC2 Sampling)

    Return:
        None.

    Remarks:
        None.
**************************************************************************************************/
void TypeC_ConfigCCComp(UINT8 u8PortNum,UINT8 u8ConfigVal);
/**************************************************************************************************

 Function:
        void TypeC_SetVBUSCompONOFF(UINT8 u8PortNum , UINT8 u8ConfigVal);

    Summary:
        This API is used to configure the VBUS Comparator to either ON or OFF State

    Devices Supported:
        UPD350 REV A

    Description:
        This API is used to configure the VBUS Comparator to either ON or OFF State. 
        Configuring the VBUS Comparator to ON State will enable the sampling of VBUS pins for a 
        given match voltage.Configuring the VBUS Comparator to OFF state will disable the VBUS Pin sampling.

    Conditions:
        None.

    Input:
        u8PortNum - Port Number.
        u8ConfigVal - TYPEC_VBUSCOMP_OFF (To set VBUS comparator ON)
                    - TYPEC_VBUSCOMP_ON  (To set VBUS comparator OFF)

    Return:
        None.

    Remarks:
        None.

**************************************************************************************************/
void TypeC_SetVBUSCompONOFF(UINT8 u8PortNum , UINT8 u8ConfigVal);
/**************************************************************************************************

 Function:
       void TypeC_SetCCSampleEnable(UINT8 u8PortNum,UINT8 u8CCEnablePins);

    Summary:
        This API is used to enable CC Comparator to sample for a particular given CC Thresholds

    Devices Supported:
        UPD350 REV A

    Description:
        This API is used to configure the CC Comparator to sample for a given CC threshold matches 
        in either CC1 or CC2 or both CC1 and CC2. The sampling CC thresholds set are taken from 
        gasTypeCcontrol[u8PortNum].u8CCDebMatch variable

    Conditions:
        None.

    Input:
        u8PortNum - Port Number.
        u8CCEnablePins - TYPEC_ENABLE_CC1(To enable Sampling of CC1 pin alone)
                       - TYPEC_ENABLE_CC2(To enable Sampling of CC2 pin alone)
                       - TYPEC_ENABLE_CC1_CC2(To enable Sampling of both the CC1 and CC2 pin)

    Return:
        None.

    Remarks:
        None.

**************************************************************************************************/
void TypeC_SetCCSampleEnable(UINT8 u8PortNum,UINT8 u8CCEnablePins);

/**************************************************************************************************

 Function:
	void TypeC_DRPSetCCSampleEnable (UINT8 u8PortNum, UINT8 u8RpCurrent);

    Summary:
        This API is used to enable DRP offload to sample for a particular given CC Thresholds

    Devices Supported:
        UPD350 REV A

    Description:
        This API is used to configure the DRP offload to sample for a given CC threshold matches.
		The sampling CC thresholds set are taken from gasCfgStatusData.sPerPortData[u8PortNum].u32CfgData 
		variable

    Conditions:
        None.

    Input:
        u8PortNum - Port Number.
        u8RpCurrent - TYPEC_DFP_DEFAULT_CURRENT
					- TYPEC_DFP_1A5_CURRENT
					- TYPEC_DFP_3A0_CURRENT
    Return:
        None.

    Remarks:
        None.

**************************************************************************************************/
void TypeC_DRPSetCCSampleEnable (UINT8 u8PortNum, UINT8 u8RpCurrent);

/**************************************************************************************************

 Function:
        void TypeC_SetCCPowerRole(UINT8 u8PortNum,UINT8 u8PowerRole, UINT8 u8ConfigVal, UINT8 u8CCPin);

    Summary:
        This API is used to Configure the given Rp or given Rd value for a port for a given
        CC pin.

    Devices Supported:
        UPD350 REV A

    Description:
        This API is used to set the power role of a device as either Source or Sink.For Source , 
        the Rp value can be set from one of the following values: open disconnect , default current, 
        1.5A current,3.0A Current. For Sink the Rd value can be set from one of the following 
        values:Open disconnect,Trimmed Rd, Trimmed Ra.

    Conditions:
        None.

    Input:
        u8PortNum - Port Number.
        u8PowerRole - TYPEC_ROLE_SOURCE (u8ConfigVal value passed is taken as value of Rp)
                    - TYPEC_ROLE_SINK(u8ConfigVal value passed is taken as value of Rd)
        u8ConfigVal - TYPEC_ROLE_SOURCE_OPEN_DIS(Setting the RP as Open disconnect)
                    - TYPEC_ROLE_SOURCE_DC(Setting the Rp value for Type C Default Current)
                    - TYPEC_ROLE_SOURCE_15(Setting the Rp value for Type C 1.5A Current)
                    - TYPEC_ROLE_SOURCE_30(Setting the Rp value for Type C 3.0A Current)
                    - TYPEC_ROLE_SINK_RD(Setting Rd value as Trimmed Rd)
                    - TYPEC_ROLE_SINK_OPEN_DIS(Setting Rd value as open disconnect)
        u8CCPin - TYPEC_ENABLE_CC1(To set power role of CC1 pin alone)
                - TYPEC_ENABLE_CC2(To set power role of CC2 pin alone)
                - TYPEC_ENABLE_CC1_CC2(To set power role of CC1 and CC2 pins) 

    Return:
        None.

    Remarks:
        None.
**************************************************************************************************/
void TypeC_SetCCPowerRole(UINT8 u8PortNum,UINT8 u8PowerRole, UINT8 u8ConfigVal, UINT8 u8CCPin);
/**************************************************************************************************

 Function:
        void TypeC_SetCCDeviceRole(UINT8 u8PortNum,UINT8 u8DevRole);

    Summary:
        This API is used to set the device role as either DFP or UFP for a given port.

    Devices Supported:
        UPD350 REV A

    Description:
         This API is used to set the device role as either DFP or UFP for a given port number

    Conditions:
        None.

    Input:
        u8PortNum - Port Number.
        u8DevRole - PD_ROLE_DFP(To Set the Port device role as DFP)
                  - PD_ROLE_UFP(To Set the Port device role as UFP)

    Return:
        None.

    Remarks:
        None.
**************************************************************************************************/
void TypeC_SetCCDeviceRole(UINT8 u8PortNum,UINT8 u8DevRole);
/**************************************************************************************************

 Function:
        void TypeC_EnabDisVCONN(UINT8 u8PortNum,UINT8 u8EnableDisable);

    Summary:
        This API is used to enable or disable the VCONN FETS for a given port

    Devices Supported:
        UPD350 REV A

    Description:
        This API is used to enable or disable the VCONN FETS. VCONN FETS are enabled on either 
        CC1 or CC2 pins.

    Conditions:
        None.

    Input:
        u8PortNum - Port Number.
        u8EnableDisable - Defines that describes whether to Enable or Disable VCONN for CC1 or CC2
                        - TYPEC_VCONN_DISABLED(To Disable VCONN FETS)
                        - TYPEC_VCONN_SOURCE_CC1(To Enable VCONN FET on CC1)
                        - TYPEC_VCONN_SOURCE_CC2(To Enable VCONN FET on CC2)

    Return:
        None.

    Remarks:
        None.
**************************************************************************************************/
void TypeC_EnabDisVCONN(UINT8 u8PortNum,UINT8 u8EnableDisable);
/**************************************************************************************************

 Function:
        void TypeC_SetRpCollAvoidance(UINT8 u8PortNum, UINT8 u8RpValue);

    Summary:
        This API is used to Set or reset the collision avoidance for a given source port.

    Devices Supported:
        UPD350 REV A

    Description:
        This API is used to set the collision avoidance for source by changing the Rp value as
        SinkTxNG(1.5A@5V) or resetting the collision avoidance by changing the Rp value as 
        SinkTxOk(3A@5V)

    Conditions:
        None.

    Input:
        u8PortNum - Port Number.
        u8RpValue - 0 , Rp set as 1.5A@5V
                  - 1 , Rp sets as 3.0A@5V

    Return:
        None.

    Remarks:
        None.
**************************************************************************************************/
void TypeC_SetRpCollAvoidance(UINT8 u8PortNum, UINT8 u8RpValue);
/**************************************************************************************************

 Function:
        UINT8 TypeC_CheckRpValCollAvoidance(UINT8 u8PortNum);

    Summary:
        This API is used to check whether the connected port partner(Source) has  set the collision 
        avoidance for a given port

    Devices Supported:
        UPD350 REV A

    Description:
        This API is called by the protocol layer to check whether the source has set the collision 
        avoidance as SinkTxOk or SinkTxNG.

    Conditions:
        None.

    Input:
        u8PortNum - Port Number.

    Return:
        None

    Remarks:
        None.
**************************************************************************************************/
UINT8 TypeC_CheckRpValCollAvoidance(UINT8 u8PortNum);
/**************************************************************************************************

    Function:
        void TypeC_CCVBUSIntrHandler(UINT8 u8PortNum);

    Summary:
        This API handles the CC and VBUS interrupts occurred for a given port.

    Devices Supported:
        UPD350 REV A

    Description:
        This API handles the CC and VBUS interrupts occurred.Based on the Current state of the 
        Type C State machine , each interrupt is handled differently.

    Conditions:
        None.

    Input:
        u8PortNum - Port Number.

    Return:
        None.

    Remarks:
        None.
**************************************************************************************************/
void TypeC_CCVBUSIntrHandler(UINT8 u8PortNum);
/**************************************************************************************************
 Function:
        void TypeC_HandleISR (UINT8 u8PortNum, UINT16 u16InterruptStatus);

    Summary:
        This API handles interrupt generated from UPD350 device for Cable Plug Control and Status Registers

    Devices Supported:
        UPD350 REV A

    Description:
        This API handles the interrupt generated from UPD350 device if the interrupt source is 
        Cable Plug Control and Status Registers. Source of interrupt is found as one of the 
        following: CC interrupt, VBUS interrupt, Extended interrupt and power interrupt.Interrupt
        is cleared by writing the read value in the register which has generated the interrupt.

    Conditions:
        None.

    Input:
        u8PortNum - Port Number.
        u16InterruptStatus - Value of the Interrupt Status Register

    Return:
        None.

    Remarks:
        None.
**************************************************************************************************/
void TypeC_HandleISR (UINT8 u8PortNum, UINT16 u16InterruptStatus);
/**************************************************************************************************
 Function:
        void TypeC_SrcIntrHandler(UINT8 u8PortNum);

    Summary:
        This API handles the CC interrupts generated in the source state machine

    Devices Supported:
        UPD350 REV A

    Description:

    Conditions:
        None.

    Input:
        u8PortNum - Port Number.

    Return:
        None.

    Remarks:
        None.
**************************************************************************************************/
void TypeC_SrcIntrHandler(UINT8 u8PortNum);

/**************************************************************************************************
 Function:
        void TypeC_DRPIntrHandler(UINT8 u8PortNum);

    Summary:
        This API handles the DRP_DONE interrupt.

    Devices Supported:
        UPD350 REV A

    Description:

    Conditions:
        None.

    Input:
        u8PortNum - Port Number.

    Return:
        None.

    Remarks:
        None.
**************************************************************************************************/
void TypeC_DRPIntrHandler(UINT8 u8PortNum);

/**************************************************************************************************
 Function:
        void TypeC_SnkIntrHandler(UINT8 u8PortNum);

    Summary:
        This API handles the CC interrupts generated in the sink state machine

    Devices Supported:
        UPD350 REV A

    Description:

    Conditions:
        None.

    Input:
        u8PortNum - Port Number.

    Return:
        None.

    Remarks:
        None.
**************************************************************************************************/
void TypeC_SnkIntrHandler(UINT8 u8PortNum);
/**************************************************************************************************
 Function:
        void TypeC_ResetVCONNDISSettings(UINT8 u8PortNum); 

    Summary:
        This API is called after the VCONN Discharge is completed to reset the settings done for
        VCONN Discharge

    Devices Supported:
        UPD350 REV A

    Description:
        This API is called after the VCONN Discharge is completed to reset the settings done for
        VCONN Discharge.

    Conditions:
        None.

    Input:
        u8PortNum - Port Number.

    Return:
        None.

    Remarks:
        None.
**************************************************************************************************/
void TypeC_ResetVCONNDISSettings(UINT8 u8PortNum); 
/**************************************************************************************************
 Function:
        void TypeC_SetCCDebounceVariable(UINT8 u8PortNum, UINT8 u8Pwrrole);

    Summary:

    Devices Supported:
        UPD350 REV A

    Description:
        This API is called to set the CC thresholds to be debounced by the CC Comparator.The value
        set for CC sampling is based on the u8Pwrrole argument

    Conditions:
        None.

    Input:
        u8PortNum - Port Number.
        u8Pwrrole - TYPEC_UFP (If CC Sampling to be set for UFP Port)
                  - TYPEC_DFP_DEFAULT_CURRENT 
                    (If CC Sampling to be set for Source with Default Rp Current)
                  - TYPEC_DFP_1A5_CURRENT (If CC Sampling to be set for Source with Rp 1.5 Current)
                  - TYPEC_DFP_3A0_CURRENT (If CC Sampling to be set for Source with Rp 3.0 Current)

    Return:
        None.

    Remarks:
        None.
**************************************************************************************************/
void TypeC_SetCCDebounceVariable(UINT8 u8PortNum, UINT8 u8Pwrrole);
/**************************************************************************************************
 Function:
        void TypeC_SetCCDefaultRpValue (UINT8 u8PortNum); 

    Summary:
        This API is called to reset the Rp value for a given source to its default user given value

    Devices Supported:
        UPD350 REV A

    Description:
        This API is called to reset the Rp value for a given source to its default user given value.
        It is called during the VCONN discharge reset configuration and in unattached state
        if the current Rp value is different from user given Rp value because of Collision avoidance

    Conditions:
        None.

    Input:
        u8PortNum - Port Number.

    Return:
        None.

    Remarks:
        None.
**************************************************************************************************/
void TypeC_SetCCDefaultRpValue (UINT8 u8PortNum); 
/**************************************************************************************************
 Function:
        void TypeC_DecodeCCSourceRpValue(UINT8 u8PortNum); 

    Summary:
        This API is called by the protocol layer to see whether the source port partner has set the
        Rp as TYPEC_SINK_TXNG or TYPEC_SINK_TXOK

    Devices Supported:
        UPD350 REV A

    Description:
        This API is called by the protocol layer to see whether the source port partner has set the
        Rp as TYPEC_SINK_TXNG or TYPEC_SINK_TXOK before transmitting the messsage initiated by the 
        Sink policy engine

    Conditions:
        None.

    Input:
        u8PortNum - Port Number.

    Return:
        None.

    Remarks:
        None.
**************************************************************************************************/
void TypeC_DecodeCCSourceRpValue(UINT8 u8PortNum); 
/**************************************************************************************************
 Function:
        void TypeC_RunStateMachine(UINT8 u8PortNum); 

    Summary:
        This API is called to run the Type C State machine for a given port.

    Devices Supported:
        UPD350 REV A

    Description:
        This API is called to run the Type C State machine for a given port.This API call has to be 
        presided over the policy engine State machine call.

    Conditions:
        None.

    Input:
        u8PortNum - Port Number.

    Return:
        None.

    Remarks:
        None.
**************************************************************************************************/
void TypeC_RunStateMachine (UINT8 u8PortNum);
/**************************************************************************************************
Function:
        void TypeC_StateChange_TimerCB (UINT8 u8PortNum, UINT8 u8TypeCState);

    Summary:
        This API is called inside the timer module after the software timer times out
        to set the given Type C State 

    Devices Supported:
        UPD350 REV A

    Description:
         This API is called inside the timer module after the software timer times out
        to set the given Type C State 

    Conditions:
        None.

    Input:
        u8PortNum - Port Number.
        u8TypeCState - Type C State to set once the software timer times out

    Return:
        None.

    Remarks:
        None.
**************************************************************************************************/
void TypeC_StateChange_TimerCB (UINT8 u8PortNum, UINT8 u8TypeCState);
/**************************************************************************************************
Function:
        void TypeC_SubStateChange_TimerCB (UINT8 u8PortNum,UINT8 u8TypeCSubState);

    Summary:
        This API is called inside the timer module after the software timer times out
        to set the given Type C sub State 

    Devices Supported:
        UPD350 REV A

    Description:
        This API is called inside the timer module after the software timer times out
        to set the given Type C sub State 

    Conditions:
        None.

    Input:
        u8PortNum - Port Number.
        u8TypeCSubState - Type C Sub State to set once the software timer times out

    Return:
        None.

    Remarks:
        None.
**************************************************************************************************/
void TypeC_SubStateChange_TimerCB (UINT8 u8PortNum,UINT8 u8TypeCSubState);
/**************************************************************************************************
Function:
       void TypeC_KillTypeCTimer (UINT8 u8PortNum)

    Summary:
        This API is called to kill the software timer with Type C timer ID

    Devices Supported:
        UPD350 REV A

    Description:
        This API is called to kill the software timer with Type C timer ID

    Conditions:
        None.

    Input:
        u8PortNum - Port Number.

    Return:
        None.

    Remarks:
        None.
**************************************************************************************************/
void TypeC_KillTypeCTimer (UINT8 u8PortNum);
/**************************************************************************************************
    Function:
       void TypeC_ConfigureVBUSThr(UINT8 u8PortNum, UINT16 u16Voltage, UINT16 u16Current,  UINT8 u8PowerFaultThrConfig)

    Summary:
        This API configures the VBUS threshold to detect the VBUS.

    Devices Supported:
        UPD350 REV A

    Description:
        This API is to configure VBUS threshold to detect VBUS, under-voltage and overvoltage

    Conditions:
        None.

    Input:
        u8PortNum - Port Number.
        u16Voltage - Voltage to which the VBUS threshold has to be configured.
        u8PowerFaultThrConfig - It can take following value
                                TYPEC_CONFIG_NON_PWR_FAULT_THR - Threshold for Non-Power Fault are configured
                                TYPEC_CONFIG_PWR_FAULT_THR  - Threshold for Power Fault (undervoltage and overvoltage)
                                                                are configured.
                    
    Return:
        None.

    Remarks:
        None.
**************************************************************************************************/
void TypeC_ConfigureVBUSThr(UINT8 u8PortNum, UINT16 u16Voltage, UINT16 u16Current,  UINT8 u8PowerFaultThrConfig);

/**************************************************************************************************
    Function:
       UINT16 TypeC_ObtainCurrentValueFrmRp(UINT8 u8PortNum)

    Summary:
        Obtains current value from Rp.

    Devices Supported:
        UPD350 REV A

    Description:
        This API is used to obtain the current value from the Rp value.

    Conditions:
        None.

    Input:
        u8PortNum - Port Number.

    Return:
        UINT16 - Returns the current value in mA.

    Remarks:
        None.
**************************************************************************************************/
UINT16 TypeC_ObtainCurrentValueFrmRp(UINT8 u8PortNum);

/**************************************************************************************************
    Function:
        void TypeC_ConfigureFRSSignalDET (UINT8 u8PortNum)

    Summary:
        API to enable detection of FRS signal 

    Devices Supported:
        UPD350 REV A

    Description:
        This API is used to configure UPD350 to enable it to detect FRS signal. 

    Conditions:
        None.

    Input:
        u8PortNum - Port Number.

    Return:
        None.

    Remarks:
        None.
**************************************************************************************************/
void TypeC_ConfigureFRSSignalDET (UINT8 u8PortNum);

/**************************************************************************************************
    Function:
        void TypeC_ConfigureFRSSignalXMT (UINT8 u8PortNum)

    Summary:
        API to enable transmission of FRS signal 

    Devices Supported:
        UPD350 REV A

    Description:
        This API is used to configure UPD350 to enable it to transmit FRS signal. 

    Conditions:
        None.

    Input:
        u8PortNum - Port Number.

    Return:
        None.

    Remarks:
        None.
**************************************************************************************************/
void TypeC_ConfigureFRSSignalXMT (UINT8 u8PortNum);

/**************************************************************************************************
    Function:
        void TypeC_EnableFRSXMTOrDET (UINT8 u8PortNum); 
    Summary:
        API to enable UPD350 to handle FR_Swap.
    Description:
        If the criteria to support FR_Swap are met by both partners, this API 
        enables UPD350 to support FR_Swap. 
        If FR_Swap is not supported by both partners, this API disables UPD350
        from supporting FR_Swap.
    Conditions:
        This API is applicable only when INCLUDE_PD_FR_SWAP is enabled.
    Input:
        u8PortNum - Port number
        u8IsFRSSupported - TRUE - FRS Criteria is supported 
                           FALSE - FRS Criteria is not supported 
    Return:
        None.
    Remarks:
        None. 
**************************************************************************************************/
void TypeC_EnableFRSXMTOrDET (UINT8 u8PortNum, UINT8 u8IsFRSSupported);

#endif /*_TYPECCONTROL_H_*/
