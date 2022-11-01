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

class SercomSPISlave
{
    public:
    
    // Constructors //
    SercomSPISlave();
    
    // Public Methods //
    /**
     * @brief SPI slave initialization using SERCOM0
     * 
     * This function initializes an SPI slave using SERCOM0. It defines the pins used and sets the SERCOM registries. 
     * 
     */
    void Sercom0init();
    
    /**
     * @brief SPI slave initialization using SERCOM1
     * 
     * This function initializes an SPI slave using SERCOM1. It defines the pins used and sets the SERCOM registries. 
     * 
     */
    void Sercom1init();
    
    /**
     * @brief SPI slave initialization using SERCOM4
     * 
     * This function initializes an SPI slave using SERCOM4. It defines the pins used and sets the SERCOM registries. 
     * 
     */
    void Sercom4init();
};

#endif
