/* 
 * File:   psf_adc_h
 * Author: I53191
 *
 * Created on August 24, 2020, 3:39 PM
 */

#ifndef PSF_ADC_H
#define	PSF_ADC_H
#ifdef	__cplusplus
extern "C" {
#endif
    
extern GLOBAL_CFG_STATUS_DATA gasCfgStatusData; 

#define ADC_P0_CFG_PORT_0_SINK_PDO_1    CFG_FORM_SINK_FIXED_PDO1(5000U, 3000U, \
                                    CFG_PORT_0_DUAL_ROLE_DATA, \
                                    CFG_PORT_0_SINK_USB_COMM, \
                                    CFG_PORT_0_SINK_UNCONSTRAINED_PWR, \
                                    CFG_PORT_0_SINK_HIGHER_CAPABILITY, \
                                    CFG_PORT_0_DUAL_ROLE_POWER);


#define ADC_P1_CFG_PORT_0_SINK_PDO_2     CFG_FORM_SINK_FIXED_PDO1(9000U, 3000U, \
                                    CFG_PORT_0_DUAL_ROLE_DATA, \
                                    CFG_PORT_0_SINK_USB_COMM, \
                                    CFG_PORT_0_SINK_UNCONSTRAINED_PWR, \
                                    CFG_PORT_0_SINK_HIGHER_CAPABILITY, \
                                    CFG_PORT_0_DUAL_ROLE_POWER);


#define ADC_P2_CFG_PORT_0_SINK_PDO_3     CFG_FORM_SINK_FIXED_PDO1(15000U, 3000U, \
                                    CFG_PORT_0_DUAL_ROLE_DATA, \
                                    CFG_PORT_0_SINK_USB_COMM, \
                                    CFG_PORT_0_SINK_UNCONSTRAINED_PWR, \
                                    CFG_PORT_0_SINK_HIGHER_CAPABILITY, \
                                    CFG_PORT_0_DUAL_ROLE_POWER);

#define ADC_P3_CFG_PORT_0_SINK_PDO_4     CFG_FORM_SINK_FIXED_PDO1(20000U, 3000U, \
                                    CFG_PORT_0_DUAL_ROLE_DATA, \
                                    CFG_PORT_0_SINK_USB_COMM, \
                                    CFG_PORT_0_SINK_UNCONSTRAINED_PWR, \
                                    CFG_PORT_0_SINK_HIGHER_CAPABILITY, \
                                    CFG_PORT_0_DUAL_ROLE_POWER);
void PSF_ADCRun();
typedef enum
{
    INITIALIZE,
    WAIT,
    PROCESS,
            
}State;
#ifdef	__cplusplus
}
#endif

#endif	/* PSF_CONTROL_TERMINAL_H */

