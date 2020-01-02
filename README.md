# **Microchip USB Power Delivery Software Framework (PSF)**

Microchip's USB Power Delivery Software Framework (PSF) is an open-source USB Power Delivery (PD) stack that is designed to integrate any suitable powerful MCU with Microchip's [UPD350](https://www.microchip.com/wwwproducts/en/UPD350) Type-C&trade; PD controller to realize multi-port custom PD solutions. The stack also supports Microchip's UPD301C standalone Type-C&trade; PD controller.

PSF provides a range of configuration options for your custom PD design and is portable to many MCUs. The stack currently supports multi-port PD source application. Other PD features will be added to the stack in a planned fashion.

| PD Feature Support		| Version Tag               |
|-----------------------|---------------------------|
| Multi-port PD Source  |PSF_STACK_v0.91 and later 	|

For more details, refer to the [Software Release Notes](Software%20Release%20Notes.pdf).

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

Please refer to the readme file for your chosen demo. You can find the application example projects and the associated readme files under [Demo](PSF/Demo) folder.

## Versioning

Release version numbers will have a prefix *PSF_STACK_V* and a version number in the  format *XX.YZ* where:

*XX*: Represents the major version number.
*YZ*: Represents the minor version number. All minor versions with *Z*=0 represent a SQA qualified (stable) release. For e.g. PSF_STACK_v1.20.

For the versions available, see the [tags on this repository](https://github.com/MicrochipTech/usb-pd-software-framework/tags).

## Support

Need help with adopting PSF for your next PD solution ? Contact [Microchip Support](https://microchipsupport.force.com/s/).

## License

Please refer to the [LICENSE.md](LICENSE.md) file for the licensing agreement.
