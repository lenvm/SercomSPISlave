# SercomSPISlave changelog
All notable changes to this repository will be documented in this file.

The format is based on [Keep a Changelog 1.0.0](https://keepachangelog.com/en/1.0.0)
and this repository adheres to [Semantic Versioning Specification 2.0.0](https://semver.org/spec/v2.0.0.html).


## [0.2.0](https://github.com/lenvm/SercomSPISlave/releases/tag/0.2.0) - 2022-11-15
[Download](https://downloads.arduino.cc/libraries/github.com/lenvm/SercomSPISlave-0.2.0.zip)

### Added
- Support for any SERCOM pin on an ATSAMD21 board.
- One class for each SERCOM.
- Doxygen style documentation for each function in `SercomSPISlave.h`, documenting which pins can be used for each SERCOM.
- Code comments in `SercomSPISlave.cpp`
  - Acronyms list.
  - Commets for each registry definition, to ease understanding what each definition is used for.
  - Datasheet page numbers to ease finding the reference of a registry definition in the datasheet.
- Readme:
  - Description how to install this repository via the [Arduino Library Manager](https://www.arduino.cc/en/Guide/Libraries).
  - Description that any SERCOM pin on an ATSAMD21 board is supported by this library, and how the pins available for each SERCOM PAD can be found using [Visual Studio Code](https://code.visualstudio.com).
  - Section on further references.
- Changelog.

### Changed
- How an instance of a SercomSPISlave class is created. From (for instance) `SercomSPISlave SPISlave;` to `Sercom0SPISlave SPISlave;`.
- How a SercomSPISlave is initialized. From (for instance) `SercomSPISlave::Sercom0init()` to `Sercom0SPISlave::SercomInit(MOSI_PIN, SCK_PIN, SS_PIN, MISO_PIN)`. This is done to support any SERCOM pin on an ATSAMD21 board. To help transition to version 0.2.0, the changes necessary to existing code based on version 0.1.0 are listed:
  - Change `SPISlave.Sercom0init()` to `SPISlave.SercomInit(SPISlave.MOSI_PINS::PA08, SPISlave.SCK_PINS::PA09, SPISlave.SS_PINS::PA10, SPISlave.MISO_PINS::PA11);`.
  - Change `SPISlave.Sercom1init()` to `SPISlave.SercomInit(SPISlave.MOSI_PINS::PA16, SPISlave.SCK_PINS::PA17, SPISlave.SS_PINS::PA18, SPISlave.MISO_PINS::PA19);`.
  - Change `SPISlave.Sercom4init()` to `SPISlave.SercomInit(SPISlave.MOSI_PINS::PA12, SPISlave.SCK_PINS::PB09, SPISlave.SS_PINS::PB10, SPISlave.MISO_PINS::PB11);`.
- `library.properties`: version.
- Examples:
  - Updated the creation of an SPISlave instance.
  - Synced the examples, such that each can be used to print the received data to the serial, or to use the received data in the main loop.


## [0.1.0](https://github.com/lenvm/SercomSPISlave/releases/tag/0.1.0) - 2021-06-09
[Download](https://downloads.arduino.cc/libraries/github.com/lenvm/SercomSPISlave-0.1.0.zip)

### Added
- SercomSPISlave class with 3 SERCOM initialization functions for SERCOM0, SERCOM1, SERCOM4.
- Examples for SERCOM0, SERCOM1, SERCOM4.
- Readme.
- License file.
- `library.properties` for Arduino CLI.


## Compare changes
[0.1.0...0.2.0](https://github.com/lenvm/SercomSPISlave/compare/0.1.0...0.2.0)
