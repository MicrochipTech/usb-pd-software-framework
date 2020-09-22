/* 
 * File:   psf_control_terminal.h
 * Author: I53191
 *
 * Created on August 24, 2020, 3:39 PM
 */

#ifndef PSF_CONTROL_TERMINAL_H
#define	PSF_CONTROL_TERMINAL_H
#define DEMO_VERSION                  0x0100U /* version 1.00 */
#ifdef	__cplusplus
extern "C" {
#endif

void MchpPSF_PCTInit();
void PSF_Control_terminal();
void PSF_monitorandwait();
void MchpPSF_PCTRUN(bool);
char* HextoAscii(UINT32 u32HexVal,UINT8 Length);
void PCTWrite(UINT8 var[],UINT8 value[],UINT8 size,UINT8 StrSize);
typedef enum
{
    INIT,
    READ_CONTINUE,
    READ_PROCESS,
    HALT,   
    TOGGLE_BLOCK,
            
}STATE;

#ifdef	__cplusplus
}
#endif

#endif	/* PSF_CONTROL_TERMINAL_H */

