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
 
void PSF_ADCRun(void);
UINT8 getADCPosition(void);

typedef enum
{
    eADC_INIT,
    eADC_WAIT_FOR_CONVERSION,
    eADC_SET_NEW_PDO
            
}ADC_RUN_STATE;
#ifdef	__cplusplus
}
#endif

#endif	/* PSF_CONTROL_TERMINAL_H */

