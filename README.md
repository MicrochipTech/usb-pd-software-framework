# **Microchip USB Power Delivery Software Framework (PSF)**

Microchip's USB Power Delivery Software Framework (PSF) is a light weight USB Power Delivery (PD) stack that is designed to integrate any suitable powerful MCU with Microchip's [UPD350](https://www.microchip.com/wwwproducts/en/UPD350) Type-C&trade; PD controller to realize multi-port custom PD solutions. The stack also supports Microchip's [UPD301B/C](https://www.microchip.com/wwwproducts/en/UPD301C) standalone Type-C&trade; PD controller.

PSF provides a range of configuration options for your custom PD design and is portable to many MCUs. PSF is constantly evolving as we seek to add PD functionality and improvements to it in a planned fashion. A summary of the planned phases and the supported features are as follows:

| Development Phase 	| Planned Features 	| Status 	| Latest Release 	|
|:-----------------:	|-------------------------------------------------------------------------------------------------------------------------------------------	|-------------	|----------------------------------	|
| I 	| Multi-Port PD Source Only, VCONN sourcing, Fault Handling 	| **Completed** 	| [PSF_STACK_V1.11](https://github.com/MicrochipTech/usb-pd-software-framework/releases/tag/PSF_STACK_V1.11) (Bug fix) 	|
| II 	| I2C Mode DC-DC control, PD Balancing, PD Throttling,  Automotive source-only PPS support, PD policy management support from  external SOC 	| **Completed**  	| [PSF_STACK_V1.11](https://github.com/MicrochipTech/usb-pd-software-framework/releases/tag/PSF_STACK_V1.11) (Bug fix) 	|
| III 	| PD sink support, Multiple Sink PDO selection algorithms, Custom Sink PDO  selection algorithm, PD sink policy management from external SOC  	| **Completed**  	| [PSF_STACK_V1.11](https://github.com/MicrochipTech/usb-pd-software-framework/releases/tag/PSF_STACK_V1.11) (Bug fix)	|  	
| IV 	| Dual Role Power (DRP) and Dual Role Data (DRD) support,  DRP/DRD policy management from external SOC 	|  **Completed** 	|  [PSF_STACK_V1.11](https://github.com/MicrochipTech/usb-pd-software-framework/releases/tag/PSF_STACK_V1.11)  (Bug fix)  	|
| V 	| Alternate modes support, Hot Plug Detection (HPD), Fast Role Swap (FRS) and Alternate mode policy management  from external SOC 	| **Completed**	| [PSF_STACK_V1.11](https://github.com/MicrochipTech/usb-pd-software-framework/releases/tag/PSF_STACK_V1.11)  (Beta release)  	 	|
| VI 	| PD charge through device support 	| Planned 	|  	|

Each development phase will add functionality to the existing feature set. Refer the [Software Release Notes](Software%20Release%20Notes.pdf) for more information about a specific release version.

## Table of Contents

* [Getting Started](#getting-started)
* [Prerequisites](#prerequisites)
   * [Software](#software)
   * [Hardware](#hardware)
* [Running the Stack](#running-the-stack)
* [Versioning](#versioning)
* [Support](#support)
* [License](#license)

## Getting Started

* Please refer to the [Getting Started with PSF](Docs/Getting%20Started%20with%20PSF.pdf) guide for a quickly getting up and running with PSF.
* A more comprehensive guide to PSF can be found in the [PSF User Guide](Docs/PSF%20User%20Guide.pdf).

## Prerequisites

#### Software:
* [Microchip MPLAB<sup>&reg;</sup> X IDE](https://www.microchip.com/mplab/mplab-x-ide) (*v5.30 or later*)
* [Microchip MPLAB<sup>&reg;</sup> XC32 Compiler](https://www.microchip.com/mplab/compilers)

#### Hardware:

* [USB Power Delivery Software Framework Evaluation Kit (EV65D44A)](https://www.microchip.com/DevelopmentTools/ProductDetails/PartNO/EV65D44A)
* [Atmel ICE Programmer/Debugger](https://www.microchip.com/DevelopmentTools/ProductDetails/ATATMEL-ICE)

## Running the Stack

We recommend using the pre-configured application example projects under the [Demo](PSF/Demo) folder as a starting point for your design. PSF currently supports the following pre-configured application examples.

* [PSF Source Lite](PSF/Demo/PSF_EVB_Source_Lite): A two port PD source only application example that uses GPIO mode DC-DC control. Can support up to four PD ports
* [PSF Source Pro](PSF/Demo/PSF_EVB_Source_Pro): A two-port PD source application example using I2C mode DC-DC control. Supports advanced PD features like PD Power Balancing.
* [PSF Sink](PSF/Demo/PSF_EVB_Sink): A PD sink application example that supports two sink PDO selection algorithms. Mode A enables selection of the highest wattage at highest voltage supported by the port partner. Mode B enables selection of highest wattage at lowest voltage supported by the port partner.
* [PSF DRP](PSF/Demo/PSF_EVB_DRP): A PD dual role application example that supports two different types of DRP ports. Port 1 is configured as a notebook/laptop type port which can source 5V/3A and sink up to 20V/3A. Port 2 is configured as a dock type port which can source up to 20V/3A and sink 5V/0A. Both ports support power and data role swaps.
* [PSF FRS](PSF/Demo/PSF_EVB_FRS): A Fast Role Swap (FRS) application example that supports two different types of FRS enabled DRP ports. Port 1 is configured as a FRS capable notebook/laptop which is capable of performing a sink-to-source role swap in the event of a power loss. Port 2 is configured as a FRS capable dock which is capable of performing a source-to-sink role swap in the event of a power loss.


Please refer the readme file of your chosen application example for instructions on running the example project. We will be updating this space as more features get added to PSF.

## Versioning

Release version numbers will have a prefix *PSF_STACK_V* and a version number in the  format *XX.YZ* where:

*XX*: Represents the major version number.

*YZ*: Represents the minor version number. All minor versions with *Z*=0 represent an SQA qualified release. For e.g. PSF_STACK_v1.20.

For the versions available, see the [tags on this repository](https://github.com/MicrochipTech/usb-pd-software-framework/tags).

## Support

Need help with adopting PSF for your next PD solution ? Contact [Microchip Support](https://microchipsupport.force.com/s/).

## License

Please refer to the [LICENSE.md](LICENSE.md) file for the licensing agreement.
