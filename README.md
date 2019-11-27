# **Microchip USB Power Delivery Software Framework (PSF)**

Microchip's USB Power Delivery Software Framework (PSF) is an open-source USB Power Delivery (PD) stack that is designed to integrate any suitable powerful MCU with Microchip's [UPD350](https://www.microchip.com/wwwproducts/en/UPD350) Type-C&trade; PD controller to realize multi-port custom PD solutions. The stack also supports Microchip's [UPD301C](insert.product.page.link) standalone Type-C&trade; PD controller.

PSF provides a range of configuration options for your custom PD design and is portable to many MCUs. The stack currently supports multi-port PD source application. Other PD features will be added to the stack in a planned fashion.

| PD Feature Support		| Version Tag               |
|-----------------------|---------------------------|
| Multi-port PD Source  |PSF_STACK_v0.91 and later 	|

## Getting Started

* Please refer to the [Getting Started with PSF](insert.document.link.here) guide for a quickly getting up and running with PSF.
* For porting PSF to other MCUs, please refer to the [System Level Integration of PSF](insert.doc.link) guide.

## Prerequisites

#### Software:
* [Microchip MPLAB<sup>&reg;</sup> X IDE](https://www.microchip.com/mplab/mplab-x-ide) (*v5.20 or later*)
* [Microchip MPLAB<sup>&reg;</sup> XC32 Compiler](https://www.microchip.com/mplab/compilers)

#### Hardware:

* [Atmel ICE Programmer/Debugger](https://www.microchip.com/DevelopmentTools/ProductDetails/ATATMEL-ICE)
* [Microchip PSF Evaluation Board (PSF-EVB)](insert.product.page.link)

## Running the Stack

Please refer to the readme file for your chosen demo. You can find the demos and the readme files under **../PSF/Demo/DemoName**

## Versioning

We use [SemVer](http://semver.org/) for versioning. For the versions available, see the [tags on this repository](https://github.com/your/project/tags).

## Support

Need help with adopting PSF for your next PD solution ? Contact [Microchip Support](https://microchipsupport.force.com/s/).

## License

Please refer to the [LICENSE.md](LICENSE.md) file for the licensing agreement.
