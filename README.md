# **Microchip USB Power Delivery Software Framework (PSF)**

Microchip's USB Power Delivery Software Framework (PSF) is a light weight USB Power Delivery (PD) stack that is designed to integrate any suitable powerful MCU with Microchip's [UPD350](https://www.microchip.com/en-us/product/UPD350) Type-C&trade; PD controller to realize multi-port custom PD solutions. The stack also supports Microchip's [UPD301B/C](https://www.microchip.com/en-us/product/UPD301C) standalone Type-C&trade; PD controller.

PSF provides a range of configuration options for your custom PD design and is portable to many MCUs. PSF is constantly evolving as we add PD functionality and improvements to it in a planned fashion. A summary of the planned and the supported features are as follows:

| Sr No	| Features 	| Status 	|
|:-----------------:	|-------------------------------------------------------------------------------------------------------------------------------------------	|-------------	|
| 1. | Multi-Port PD Source, VCONN sourcing and Fault Handling 	| **Supported**	|
| 2. | I2C Mode DC-DC control, PD Balancing, PD Throttling and Automotive source-only PPS | **Supported** 	|
| 3. | PD sink, Multiple Sink PDO selection algorithms and Custom Sink PDO selection algorithm	| **Supported** |
| 4. | Dual Role Power (DRP) and Dual Role Data (DRD)	|  **Supported** 	|
| 5.| Generic Alternate mode, Hot Plug Detection (HPD) and Fast Role Swap (FRS)	| **Supported**	|
| 6.| DisplayPort alternate mode  | *In Development* |
| 7.| PD policy management support from  external SOC 	| Planned |
| 8.| PD charge through device support 	| Planned |

Refer the [Software Release Notes](Software%20Release%20Notes.pdf) for more information about a specific release version.

## Table of Contents

* [Getting Started](#getting-started)
* [Tool Prerequisites](#tool-prerequisites)
   * [Software](#software)
   * [Hardware](#hardware)
* [Supported Hardware Platforms](#supported-hardware-platforms)
* [Running the Stack](#running-the-stack)
* [Versioning](#versioning)
* [Support](#support)
* [License](#license)

## Getting Started

* Please refer to the [Getting Started with PSF](Docs/Getting%20Started%20with%20PSF.pdf) guide for a quickly getting up and running with PSF.
* A more comprehensive guide to PSF can be found in the [PSF User Guide](Docs/PSF%20User%20Guide.pdf).

## Tool Prerequisites

#### Software:
* [Microchip MPLAB<sup>&reg;</sup> X IDE](https://www.microchip.com/en-us/development-tools-tools-and-software/mplab-x-ide) (*v5.30 or later*)
* [Microchip MPLAB<sup>&reg;</sup> XC32 Compiler](https://www.microchip.com/en-us/development-tools-tools-and-software/mplab-xc-compilers)

#### Hardware:

* [Atmel ICE Programmer/Debugger](https://www.microchip.com/en-us/development-tool/ATATMEL-ICE)

## Supported Hardware Platforms

|Sr No| Platform Name	| Platform Type	| Description	|
|:-----------------:|:-----------------:	|-------------------------------------------------------------------------------------------------------------------------------------------	|-------------	|
| 1. 	| [USB Power Delivery Software Framework Evaluation Kit (EV65D44A)](https://www.microchip.com/en-us/development-tool/EV65D44A)	| Development/ Evaluation	| Full featured two port USB PD platform for evaluation of PSF features and custom software development	|
| 2. 	| UPD301C Plug-In Module (PIM) (EV71C90A)	| Development/ Evaluation	| Add-on module for EV65D44A. Can be used to evaluate PSF and develop custom software for UPD301B/C standalone USB PD controller. Contact [Microchip Support](https://microchipsupport.force.com/s/).|
| 3. 	| [UPD301C Basic Sink Application Example (EV11L78A)](https://www.microchip.com/en-us/development-tool/EV11L78A)	| Evaluation | Low cost, single port, sink-only application reference design|
| 4. 	| UPD301B Basic Source Application Example (EV56W72A)	| Evaluation	| Low cost, single port, source-only application reference. Contact [Microchip Support](https://microchipsupport.force.com/s/).|


## Running the Stack

We recommend using the pre-configured application example projects under the [Demo](PSF/Demo) folder as a starting point for your design. PSF currently supports the following pre-configured application examples.

|Sr No| Demo Name	| Hardware Required	| Description	|
|:-----------------:|:-----------------:	|-------------------------------------------------------------------------------------------------------------------------------------------	|-------------	|
| 1.| [PSF Source Lite](PSF/Demo/PSF_EVB_Source_Lite)	| EV65D44A | Two port PD source only application example that uses GPIO mode DC-DC control. Can support up to four PD ports|
| 2.| [PSF Source Pro](PSF/Demo/PSF_EVB_Source_Pro) |  EV65D44A | Two-port PD source application example using I2C mode DC-DC control. Supports advanced PD features like PD Power Balancing and Power Throttling |
| 3. | [PSF Sink](PSF/Demo/PSF_EVB_Sink) | EV65D44A | PD sink application example that supports two sink PDO selection algorithms. Mode A enables selection of the highest wattage at highest voltage supported by the port partner. Mode B enables selection of highest wattage at lowest voltage supported by the port partner |
| 4. | [PSF DRP](PSF/Demo/PSF_EVB_DRP) | EV65D44A | PD dual role application example that supports two different types of DRP ports. Port 1 is configured as a notebook/laptop type port which can source 5V/3A and sink up to 20V/3A. Port 2 is configured as a dock type port which can source up to 20V/3A and sink 5V/0A. Both ports support power and data role swaps |
| 5. | [PSF FRS](PSF/Demo/PSF_EVB_FRS) | EV65D44A | Fast Role Swap (FRS) application example that supports two different types of FRS enabled DRP ports. Port 1 is configured as a FRS capable notebook/laptop which is capable of performing a sink-to-source role swap in the event of a power loss. Port 2 is configured as a FRS capable dock which is capable of performing a source-to-sink role swap in the event of a power loss |
| 6.| [UPD301C PIM Source Lite](PSF/Demo/UPD301C_PIM_Source_Lite)	| EV65D44A, EV71C90A | Two port PD source only application example for UPD301C that uses GPIO mode DC-DC control. Can support up to four PD ports|
| 7.| [UPD301C PIM Source Pro](PSF/Demo/UPD301C_PIM_Source_Pro) |  EV65D44A, EV71C90A | Two-port PD source application example for UPD301C using I2C mode DC-DC control. Supports advanced PD features like PD Power Balancing and Power Throttling |
| 8. | [UPD301C PIM Sink](PSF/Demo/UPD301C_PIM_Sink) | EV65D44A, EV71C90A | PD sink application example for UPD301C that supports two sink PDO selection algorithms. Mode A enables selection of the highest wattage at highest voltage supported by the port partner. Mode B enables selection of highest wattage at lowest voltage supported by the port partner |
| 9. | [UPD301C PIM DRP](PSF/Demo/UPD301C_PIM_DRP) | EV65D44A, EV71C90A | PD dual role application example for UPD301C that supports two different types of DRP ports. Port 1 is configured as a notebook/laptop type port which can source 5V/3A and sink up to 20V/3A. Port 2 is configured as a dock type port which can source up to 20V/3A and sink 5V/0A. Both ports support power and data role swaps |
| 10. | [UPD301C Basic Sink](PSF/Demo/UPD301C_Basic_Sink_AE) | EV11L78A | Single port sink-only application example for UPD301C. Supports dynamic PDO configuration using rotary switch and advanced control options through UART terminal |
| 11. | [UPD301B Basic Source](PSF/Demo/UPD301B_Basic_Source_AE) | EV56W72A | Single port source-only application example for UPD301B. Supports advanced control options through UART terminal|

This release includes MCC support for automatic code generation. This support is available only for PSF Source Lite, PSF Source Pro, PSF Sink, PSF DRP, PSF FRS, UPD301C Basic Sink and UPD301B Basic Source.
Please refer the readme file of your chosen application example for instructions on running the example project. This space will be updated as more features get added to PSF.

## Versioning

Release version numbers will have a prefix *PSF_STACK_V* and a version number in the  format *XX.YZ* where:

*XX*: Represents the major version number.

*YZ*: Represents the minor version number. All minor versions with *Z*=0 represent an SQA qualified release. For e.g. PSF_STACK_v1.20.

For the versions available, see the [tags on this repository](https://github.com/MicrochipTech/usb-pd-software-framework/tags).

## Support

Need help with adopting PSF for your next PD solution ? Contact [Microchip Support](https://microchipsupport.force.com/s/).

## License

Please refer to the [LICENSE.md](LICENSE.md) file for the licensing agreement.
