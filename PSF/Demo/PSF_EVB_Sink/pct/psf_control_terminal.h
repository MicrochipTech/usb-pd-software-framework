/* 
 * File:   psf_control_terminal.h
 * Author: I53191
 *
 * Created on August 24, 2020, 3:39 PM
 */

#ifndef PSF_CONTROL_TERMINAL_H
#define	PSF_CONTROL_TERMINAL_H
#define DEMO_VERSION                  0x0100U /* version 1.00 */
// DOM-IGNORE-BEGIN
#ifdef __cplusplus // Provide C++ Compatibility

	extern "C" {

#endif
        
typedef enum
{
    ePCT_UNLOCK, //PCT waits for the command from the user and PCT runs until it gets unlocked by space bar
    ePCT_LOCK
            
}PCT_LOCK_STATE;

void MchpPSF_PCTInit(void);
void PSF_monitorandwait(void);
void MchpPSF_PCTRUN(PCT_LOCK_STATE);
UINT8* HextoAscii(UINT32 u32HexVal,UINT8 Length);
void PCTWrite(const UINT8 *u8aPrintString,const UINT8 *u8aValue,UINT8 u8Size,UINT8 u8StrSize);

typedef enum
{
    eINIT,
    eREAD_CONTINUE,
    eREAD_PROCESS,
    eHALT,   
    eTOGGLE_BLOCK
            
}STATE;

#ifdef	__cplusplus

    }
    
#endif

#endif	/* PSF_CONTROL_TERMINAL_H */

