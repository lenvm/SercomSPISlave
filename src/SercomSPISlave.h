/*
  Copyright (C) 2021 lenvm

  This program is free software: you can redistribute it and/or modify
  it under the terms of the GNU General Public License as published by
  the Free Software Foundation, either version 3 of the License, or
  (at your option) any later version.

  This program is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
  GNU General Public License for more details.

  For the GNU General Public License see https://www.gnu.org/licenses/

  Contact Information
  -------------------
  lenvm
  GitHub   : https://github.com/lenvm
*/

#ifndef SercomSPISlave_h
#define SercomSPISlave_h

#include <Arduino.h>

class Sercom0SPISlave {
 public:
  // Types //
  enum MOSI_Pins {PA04, PA08};
  enum SCK_Pins {PA05, PA09};
  enum SS_Pins {PA06, PA10};
  enum MISO_Pins {PA07, PA11};

  // Constructors //
  Sercom0SPISlave();

  // Public methods //
  /**
   * @brief SPI slave initialization using SERCOM0
   * 
   * This function initializes an SPI slave using SERCOM0. It defines the pins used and sets the SERCOM registries.
   * A combination of pins highlighted in the readme is: PA08, PA09, PA10, PA11
   * 
   * @param[in] MOSI_Pin PA04, PA08
   * @param[in] SCK_Pin PA05, PA09
   * @param[in] SS_Pin PA06, PA10
   * @param[in] MISO_Pin PA07, PA11
   * 
   */
  void SercomInit(MOSI_Pins MOSI_Pin, SCK_Pins SCK_Pin, SS_Pins SS_Pin, MISO_Pins MISO_Pin);

};

class Sercom1SPISlave {
 public:

  // Types //
  enum MOSI_Pins {PA00, PA16};
  enum SCK_Pins {PA01, PA17};
  enum SS_Pins {PA18, PA30};
  enum MISO_Pins {PA19, PA31};

  // Constructors //
  Sercom1SPISlave();

  // Public methods //
  /**
   * @brief SPI slave initialization using SERCOM1
   * 
   * This function initializes an SPI slave using SERCOM1. It defines the pins used and sets the SERCOM registries.
   * A combination of pins highlighted in the readme is: PA16, PA17, PA18, PA19
   * 
   * @param[in] MOSI_Pin PA00, PA16
   * @param[in] SCK_Pin PA01, PA17
   * @param[in] SS_Pin PA18, PA30
   * @param[in] MISO_Pin PA19, PA31
   * 
   */
  void SercomInit(MOSI_Pins MOSI_Pin, SCK_Pins SCK_Pin, SS_Pins SS_Pin, MISO_Pins MISO_Pin);
};

class Sercom2SPISlave {
 public:

  // Types //
  enum MOSI_Pins {PA08, PA12};
  enum SCK_Pins {PA09, PA13};
  enum SS_Pins {PA10, PA14};
  enum MISO_Pins {PA11, PA15};

  // Constructors //
  Sercom2SPISlave();

  // Public methods //
  /**
   * @brief SPI slave initialization using SERCOM2
   * 
   * This function initializes an SPI slave using SERCOM2. It defines the pins used and sets the SERCOM registries.
   * 
   * @param[in] MOSI_Pin PA08, PA12
   * @param[in] SCK_Pin PA09, PA13
   * @param[in] SS_Pin PA10, PA14
   * @param[in] MISO_Pin PA11, PA15
   * 
   */
  void SercomInit(MOSI_Pins MOSI_Pin, SCK_Pins SCK_Pin, SS_Pins SS_Pin, MISO_Pins MISO_Pin);
};

class Sercom3SPISlave {
 public:

  // Types //
  enum MOSI_Pins {PA16, PA22};
  enum SCK_Pins {PA17, PA23};
  enum SS_Pins {PA18, PA20, PA24};
  enum MISO_Pins {PA19, PA21, PA25};

  // Constructors //
  Sercom3SPISlave();

  // Public methods //
  /**
   * @brief SPI slave initialization using SERCOM3
   * 
   * This function initializes an SPI slave using SERCOM3. It defines the pins used and sets the SERCOM registries.
   * 
   * @param[in] MOSI_Pin PA16, PA22
   * @param[in] SCK_Pin PA17, PA23
   * @param[in] SS_Pin PA18, PA20, PA24
   * @param[in] MISO_Pin PA19, PA21, PA25
   */
  void SercomInit(MOSI_Pins MOSI_Pin, SCK_Pins SCK_Pin, SS_Pins SS_Pin, MISO_Pins MISO_Pin);
};

class Sercom4SPISlave {
 public:

  // Types //
  enum MOSI_Pins {PA12, PB08, PB12};
  enum SCK_Pins {PA13, PB09, PB13};
  enum SS_Pins {PA14, PB10, PB14};
  enum MISO_Pins {PA15, PB11, PB15};

  // Constructors //
  Sercom4SPISlave();

  // Public methods //
  /**
   * @brief SPI slave initialization using SERCOM4
   * 
   * This function initializes an SPI slave using SERCOM4. It defines the pins used and sets the SERCOM registries.
   * A combination of pins highlighted in the readme is: PA12, PB09, PB10, PB11
   * 
   * @param[in] MOSI_Pin PA12, PB08, PB12
   * @param[in] SCK_Pin PA13, PB09, PB13
   * @param[in] SS_Pin PA14, PB10, PB14
   * @param[in] MISO_Pin PA15, PB11, PB15
   * 
   */
  void SercomInit(MOSI_Pins MOSI_Pin, SCK_Pins SCK_Pin, SS_Pins SS_Pin, MISO_Pins MISO_Pin);
};

class Sercom5SPISlave {
 public:

  // Types //
  enum MOSI_Pins {PB02, PB16, PB30};
  enum SCK_Pins {PB03, PB17, PB31};
  enum SS_Pins {PA20, PA24, PB00, PB22};
  enum MISO_Pins {PA21, PA25, PB01, PB23};

  // Constructors //
  Sercom5SPISlave();

  // Public methods //
  /**
   * @brief SPI slave initialization using SERCOM5
   * 
   * This function initializes an SPI slave using SERCOM5. It defines the pins used and sets the SERCOM registries.
   * 
   * @param[in] MOSI_Pin PB02, PB16, PB30
   * @param[in] SCK_Pin PB03, PB17, PB31
   * @param[in] SS_Pin PA20, PA24, PB00, PB22
   * @param[in] MISO_Pin PA21, PA25, PB01, PB23
   * 
   */
  void SercomInit(MOSI_Pins MOSI_Pin, SCK_Pins SCK_Pin, SS_Pins SS_Pin, MISO_Pins MISO_Pin);
};

class SercomSPISlave {
 private:
  // Types //
  friend class Sercom0SPISlave; // Sercom0SPISlave has full access to SercomSPISlave
  friend class Sercom1SPISlave;
  friend class Sercom2SPISlave;
  friend class Sercom3SPISlave;
  friend class Sercom4SPISlave;
  friend class Sercom5SPISlave;

  // Constructors //
  SercomSPISlave();

  // Private methods //
  /**
   * @brief SERCOM registry initialization.
   * 
   * This function initializes the SERCOM registries of an SPI slave.
   * 
   * @param[in] sercom_x The following are supported: SERCOM0, SERCOM1, SERCOM2, SERCOM3, SERCOM4, SERCOM5
   * 
   * @return void
   * 
   */
  void SercomRegistryInit(Sercom* sercom_x);
};

#endif
