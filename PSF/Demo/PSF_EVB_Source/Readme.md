### 1 Introduction

USB Power Delivery Software Framework (PSF) with USB-PD Port Controller UPD350 is an effective USB-PD solution compliant to USB-PD 3.0 Specification.

This document is intended to guide a user on setting up the PSF-EVB to work properly with PSF Firmware along with a demonstration of a PD device attached to the PSF-EVB.

### 2 Terms and Abbreviations

| Term     | Definition                                                        |
|:---------|:------------------------------------------------------------------|
| PSF      | Power Delivery Software Framework                                 |
| EVB      | Engineering Validation Board                                      |
| PD       | Power Delivery                                                    |
| IDE      | Integrated Development Environment                                |
| MHC      | MPLAB Harmony Configurator                                        |
| PDO      | Power Data Object                                                 |
| Building | Process of converting source code files into standalone software artifact that can be run on a computer |

### 3 Software License Agreement

Copyright ©[2019] Microchip Technology Inc. and its subsidiaries.

Subject to your compliance with these terms, you may use Microchip software and any derivatives exclusively with Microchip products. It is your responsibility to comply with third party license terms applicable to your use of third party software (including open source software) that may accompany Microchip software.


THIS SOFTWARE IS SUPPLIED BY MICROCHIP "AS IS".  NO WARRANTIES, WHETHER EXPRESS, IMPLIED OR STATUTORY, APPLY TO THIS SOFTWARE, INCLUDING ANY IMPLIED WARRANTIES OF NON-INFRINGEMENT, MERCHANTABILITY, AND FITNESS FOR A PARTICULAR PURPOSE.

IN NO EVENT WILL MICROCHIP BE LIABLE FOR ANY INDIRECT, SPECIAL, PUNITIVE, INCIDENTAL OR CONSEQUENTIAL LOSS, DAMAGE, COST OR EXPENSE OF ANY KIND WHATSOEVER RELATED TO THE SOFTWARE, HOWEVER CAUSED, EVEN IF MICROCHIP HAS BEEN ADVISED OF THE POSSIBILITY OR THE DAMAGES ARE FORESEEABLE.  TO THE FULLEST EXTENT ALLOWED BY LAW, MICROCHIP'S TOTAL LIABILITY ON ALL CLAIMS IN ANY WAY RELATED TO THIS SOFTWARE WILL NOT EXCEED THE AMOUNT OF FEES, IF ANY, THAT YOU HAVE PAID DIRECTLY TO MICROCHIP FOR THIS SOFTWARE.

### 4 Setting up the PSF-EVB board for “PSF_EVB_Source”

1. Connect PM PD power modules to J35 and J44 of the PSF-EVB

2. Connect 150W power adapter to J49 of the PSF-EVB

3. Connect Atmel ICE Debugger to J19 of PSF-EVB. A dot will be present in Atmel ICE which gives an indication that this pin should be connected to 3v3 of J19.

4. An image showing the full EVB setup is given here.


    ![](Images/Setting_up_EVB.jpeg)


### 5 Running the demo

Refer ‘Getting Started with PSF’ document for the detailed steps on setting up the build environment, building the PSF project and programming the hex file in the PSF-EVB.

Refer Appendix 5.1 to change any SAMD20 Harmony configuration.

1. Power on the PSF-EVB

2. Connect a PD device to Port 1 in the EVB using an electronically marked Type C cable.

3. The image demonstrates a scenario where two 20V capable PD Laptops are connected to both the PD Ports of the EVB.

      ![](Images/Running_Demo.png)

     Setup where two PD Laptops are connected to two PD Ports                                                                          

### 6 Expected Results

1. Once a PD Device is attached to a PD port, the device will start charging which indicates that power contract negotiation has happened.

2. The PDOs negotiated by the PD device will be indicated through LEDs at the bottom of the EVB, say for ex 5V is negotiated by the device, LED in the D20 of Port 1 PDO will glow. If 9V is negotiated, then LEDs in D20 and D25 will glow. If 20V is negotiated, all the LEDs will glow.


    ![](Images/expected_Results.jpg)

    
    Setup where a 20V capable laptop is connected to Port 1 and no device in Port 2

### 7 Appendix

##### 7.1 Harmony Framework Usage

1.	Download MPLAB Harmony Configurator 3 by choosing *Tools -> Plugins -> Available Plugins -> MPLAB Harmony 3 Configurator -> Install* in the MPLAB X IDE.

    ![](Images/Install_Harmony.JPG)

2.	To download basic Harmony Configuration, Go to *Tools -> Embedded -> MPLAB Harmony 3 Content Manager*


    ![](Images/harmony_1a.JPG)


3.	Choose a local *Framework Path* and choose github Harmony path for *Remote* and click *Next*. The local Framework Path shall not have any spaces in the folder name.
The following screen will appear indicating Connection was successful.


    ![](Images/harmony_2.JPG)


4.	Select the Remote Packages – ‘mhc, dev_packs, bsp and csp’ and click *Download selected*.


    ![](Images/harmony_1b.JPG)


5.	A License Window will pop up. Check *Accept All License* and give *Next* and give *Close*.


    ![](Images/harmony_1e.jpg)


6.	Current Download Status will be shown until remote packages are downloaded to local.


    ![](Images/harmony_1c.JPG)


7.	Once all the remote packages are downloaded, they will be listed under *Local Packages* section.


    ![](Images/harmony_6.JPG)


8.	To launch Harmony 3 Configurator, choose *Tools -> Embedded -> MPLAB Harmony 3 Configurator*

9.	Select *MPLAB Harmony Project Path* and choose *Reconfigure Paths*


    ![](Images/harmony_10.JPG)


10.	Choose the Harmony Framework path by clicking the folder icon(Green box) and click *Ok*


    ![](Images/harmony_11.JPG)


11.	Click *Launch.*

12.	*Window Manager Selection Dialog* window will open. Select Native window management mode if you want the Harmony tabs to be embedded into MPLABX IDE else choose *Standalone* mode for separate MHC window. This will be prompted during first time only.


    ![](Images/harmony_8.JPG)


13.	After having an overview of Native Window Manager Quick Tour, click *I Understand.* This will also appear for the first time only.

14.	*Configuration Database Setup* page appears with DFP and CMSIS path shown by default. Click Launch.


    ![](Images/harmony_13.jpg)


15.	Load the Harmony Configuration File by choosing *MHC -> Load State*. Give the file path as *{Local_Path}\PSF\Demo\PSF_EVB_Source\firmware\src\config\default\ SAMD20_PSFHarmonyConfiguration.xml*

16.	Project Graph will be shown with the peripherals and any changes can be done through *Configuration Options* page


    ![](Images/harmony_14.JPG)


Refer MPLAB Harmony Release Notes for any clarifications/settings specific to MHC from https://www.microchip.com/mplab/mplab-harmony

For more information on how to use MHC, refer
https://github.com/Microchip-MPLAB-Harmony/mhc/wiki
