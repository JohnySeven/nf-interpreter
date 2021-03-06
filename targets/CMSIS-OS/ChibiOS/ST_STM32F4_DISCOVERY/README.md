## Configuration of Chibios, HAL and MCU ##

For a successful build the following changes are required:

In _halconf.g_ (in both nanoBooter and nanoCLR folders), when compared with a default file:
- HAL_USE_SERIAL to TRUE
- HAL_USE_SERIAL_USB to TRUE
- HAL_USE_USB to TRUE
- SERIAL_DEFAULT_BITRATE to 115200

In _mcuconf.h_ (in both nanoBooter and nanoCLR folders), when compared with a default file:
- STM32_SERIAL_USE_USART2 to TRUE
- STM32_USB_USE_OTG1 to TRUE

In _chconf.h_ (_**only**_ for nanoCLR folder), when compared with a default file:
- set the CORTEX_VTOR_INIT with the appropriate address of the vector table for nanoCLR


## Definitions for each image ## 

Any defines specific to an image (nanoBooter or nanoCLR) that are to be made available throughout the code should be added to the _target_board.h.in_. There is one in [nanoBooter](nanoBooter/target_board.h.in) and another in [nanoCLR](nanoCLR/target_board.h.in) directories, allowing different defines for each one.
Defines that are common to a target board should be added to the [_target_common.h.in_](target_common.h.in).
The content of these files is parsed by CMake. It can include CMake variables. Check the documentation of CMake config files [here](https://cmake.org/cmake/help/v3.7/command/configure_file.html?highlight=configure_file).


**NOTE: this configuration was successfully tested in a ST_STM32F4_DISCOVERY board using the Serial over USB connection on USB port 1 that creates a virtual COM port.**


## ADC configurations

The following ADC channels (and respective GPIO pins) are available to the managed API, in the respective index:
- PC1, ADC1 IN11
- PC2  ADC1 IN12
- PA6  ADC2 IN6
- PA7  ADC2 IN7
- PB0  ADC2 IN8
- Temp Sensor ADC1
- VrefInt ADC1
- Vbatt ADC1


## Floating point

The current build is set to add support for single-precision floating point.
Meaning that `System.Math` API supports only the `float` overloads. The `double` ones will throw a `NotImplementedException`.
