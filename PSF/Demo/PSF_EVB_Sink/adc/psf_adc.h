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

