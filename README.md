# **Microchip USB Power Delivery Software Framework (PSF)**

Microchip's USB Power Delivery Software Framework (PSF) is a light weight USB Power Delivery (PD) stack that is designed to integrate any suitable powerful MCU with Microchip's [UPD350](https://www.microchip.com/wwwproducts/en/UPD350) Type-C&trade; PD controller to realize multi-port custom PD solutions. The stack also supports Microchip's [UPD301B/C](https://www.microchip.com/wwwproducts/en/UPD301C) standalone Type-C&trade; PD controller.

PSF provides a range of configuration options for your custom PD design and is portable to many MCUs. PSF is constantly evolving as we seek to add PD functionality and improvements to it in a planned fashion. A summary of the planned phases and the supported features are as follows:

| Development Phase 	| Planned Features 	| Status 	| Support Added in 	|
|:-----------------:	|-------------------------------------------------------------------------------------------------------------------------------------------	|-------------	|----------------------------------	|
| I 	| Multi-Port PD Source Only, VCONN sourcing, Fault Handling 	| **Completed** 	| [PSF_STACK_V1.00](https://github.com/MicrochipTech/usb-pd-software-framework/releases/tag/PSF_STACK_V1.00) (Stable) 	|
| II 	| I2C Mode DC-DC control, PD Balancing, PD Throttling,  Automotive source-only PPS support, PD policy management support from  external SOC 	| *In-Progress* 	| [PSF_STACK_V1.04](https://github.com/MicrochipTech/usb-pd-software-framework/releases/tag/PSF_STACK_V1.04) (Beta Release) 	|
| III 	| PD sink support, Multiple Sink PDO selection algorithms, Custom Sink PDO  selection algorithm, PD sink policy management from external SOC  	| *In-Progress* 	| [PSF_STACK_V1.03](https://github.com/MicrochipTech/usb-pd-software-framework/releases/tag/PSF_STACK_V1.03)  (Alpha Release) 	|  	|
| IV 	| Dual Role Power (DRP) and Dual Role Data (DRD) support,  DRP/DRD policy management from external SOC 	|  *In-Progress* 	|  	|
| V 	| Alternate modes support, Alternate mode policy management  from external SOC 	| Planned 	|  	|
| VI 	| PD charge through device support, Fast role swap 	| Planned 	|  	|

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

* [Atmel ICE Programmer/Debugger](https://www.microchip.com/DevelopmentTools/ProductDetails/ATATMEL-ICE)
* Microchip PSF Evaluation Board (PSF-EVB)

## Running the Stack

We recommend using the pre-configured application example projects under the [Demo](PSF/Demo) folder as a starting point for your design. PSF currently supports the following pre-configured application examples.

* [PSF Source Lite](PSF/Demo/PSF_EVB_Source_Lite): A two port PD source only application example that uses GPIO mode DC-DC control. Can support up to four PD ports
* [PSF Source Pro](PSF/Demo/PSF_EVB_Source_Pro): A two-port PD source application example using I2C mode DC-DC control. Supports advanced PD features like PD Power Balancing.
* [PSF Sink](PSF/Demo/PSF_EVB_Sink): A PD sink application example that supports two sink PDO selection algorithms. Mode A enables selction of the highest wattage at highest voltage supported by the port partner. Mode B enables selection of highest wattage at lowest voltage supported by the port partner.

Please refer the readme file of your chosen application example for instructions on running the example project. We will be updating this space as more features get added to PSF.

## Versioning

Release version numbers will have a prefix *PSF_STACK_V* and a version number in the  format *XX.YZ* where:

*XX*: Represents the major version number.

*YZ*: Represents the minor version number. All minor versions with *Z*=0 represent an SQA qualified (stable) release. For e.g. PSF_STACK_v1.20.

For the versions available, see the [tags on this repository](https://github.com/MicrochipTech/usb-pd-software-framework/tags).

## Support

Need help with adopting PSF for your next PD solution ? Contact [Microchip Support](https://microchipsupport.force.com/s/).

## License

Please refer to the [LICENSE.md](LICENSE.md) file for the licensing agreement.
