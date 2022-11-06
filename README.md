# SercomSPISlave

![CI Arduino Lint workflow](https://github.com/lenvm/SercomSPISlave/actions/workflows/CI-arduino-lint-action.yml/badge.svg)

## Introduction
This is a Sercom SPI library used for ATSAMD21 boards, such as the Arduino Zero, Arduino MKR Zero and Adafruit Feather M0. It provides functions to setup SPI Slaves using Sercom0, Sercom1, Sercom2, Sercom3, Sercom4 and Sercom5.

To use the library with the Arduino IDE, simply install it via the [Arduino Library Manager](https://www.arduino.cc/en/Guide/Libraries) or clone or copy the the folder into the Arduino library folder.

Examples are provided to set up a:
- Sercom0 SPI Slave
- Sercom1 SPI Slave
- Sercom4 SPI Slave

### Why would I set up a board as an SPI slave?
Setting up a board as an SPI slave can be used to for instance communicate over SPI between a Raspberry Pi (SPI master) and an ATSAMD21-based board (SPI slave).

## Hardware implementation
Any Sercom pin on an ATSAMD21 board is supported by this library.

The pins available for each Sercom are easily seen using [Visual Studio Code](https://code.visualstudio.com). Please try it out by modifying the example Sercom4SPISlave in Visual Studio Code. Remove `::PA12` and type `::` instead. You will find all pins that can be used for this Sercom PAD listed for autocompletion.

> **Note**
> Not all pins are physically available on each ATSAMD21-based board.

> **Note**
> Not all Sercom may be available to set up an SPI slave on each ATSAMD21-based board, as some Sercom may already be used by default for other communication interfaces.

The pins used in the examples are:
### Sercom0
| SPI pin | description         | ATSAMD21 pin |
|---------|---------------------|--------------|
| MOSI    | Master Out Slave In | PA08         |
| SCK     | Serial Clock        | PA09         |
| SS      | Slave Select        | PA10         |
| MISO    | Master In Slave Out | PA11         |

### Sercom1
| SPI pin | description         | ATSAMD21 pin |
|---------|---------------------|--------------|
| MOSI    | Master Out Slave In | PA16         |
| SCK     | Serial Clock        | PA17         |
| SS      | Slave Select        | PA18         |
| MISO    | Master In Slave Out | PA19         |

### Sercom4
| SPI pin | description         | ATSAMD21 pin |
|---------|---------------------|--------------|
| MOSI    | Master Out Slave In | PA12         |
| SCK     | Serial Clock        | PB09         |
| SS      | Slave Select        | PB10         |
| MISO    | Master In Slave Out | PB11         |

### Example: Adafruit Feather M0
To setup a Sercom4 SPI slave on an Adafruit Feather M0, connect the pins as follows:
| SPI pin | ATSAMD21 pin | Arduino pin | Sercom#/PAD[#] |
|---------|--------------|-------------|----------------|
| MOSI    | PA12         | PIN22       | Sercom4/PAD[0] |
| SCK     | PB09         | PIN16       | Sercom4/PAD[1] |
| SS      | PB10         | PIN23       | Sercom4/PAD[2] |
| MISO    | PB11         | PIN24       | Sercom4/PAD[3] |

The pinout of the Adafruit Feather M0 is found [here](https://cdn-learn.adafruit.com/assets/assets/000/046/244/original/adafruit_products_Feather_M0_Basic_Proto_v2.2-1.png).

> **Note**
> On the Adafruit Feather M0 the pin descriptions on the board do not match the names of the Sercom4 pins.

## References
- The development of this code was made possible with the support I received on the Arduino Forum in the following [topic](https://forum.arduino.cc/index.php?topic=360026.15). My username is Maverick123.

- The datasheet of the Atmel SAM D21G microcontroller is found [here](https://cdn.sparkfun.com/datasheets/Dev/Arduino/Boards/Atmel-42181-SAM-D21_Datasheet.pdf). See Table 6-1 "PORT Function Multiplexing" to find the pins that can be used for Sercom0, Sercom1, Sercom2, Sercom3, Sercom4 and Sercom5.

- A detailed description how to use the table is found [here](https://learn.adafruit.com/using-atsamd21-sercom-to-add-more-spi-i2c-serial-ports/muxing-it-up).

### Further references
- This repository aims to follow the [Arduino CLI library specification](https://arduino.github.io/arduino-cli/library-specification/).

- This repository aims to follow the [Google C++ Style Guide](https://google.github.io/styleguide/cppguide.html).

- This repository adheres to [Semantic Versioning Specification 2.0.0](https://semver.org/#semantic-versioning-200).
