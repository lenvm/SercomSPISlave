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

/*
  Example code for the SercomSPISlave library
  This code initializes a SERCOM0 SPI Slave and prints the data received.

  Written 2020 July 15
  by lenvm
  Modified 2020 December 
  by stubb
  Updated 2021 June 8 by lenvm
*/

#include <SercomSPISlave.h>
SercomSPISlave SPISlave;

#define DEBUG // comment this line out to not print debug data on the serial bus

void setup()
{  
  Serial.begin(115200);
  Serial.println("Serial started");
  SPISlave.Sercom0init();
  Serial.println("SERCOM0 SPI slave initialized");
}

void loop()
{
  Serial.println("nodata");
  delay(1000);
}

void SERCOM0_Handler() // 25.7 Register Summary, page 454 atmel 42181, samd21
{ 
  #ifdef DEBUG
    Serial.println("In SPI Interrupt");
  #endif
  uint8_t data = 0;
  data = (uint8_t)SERCOM0->SPI.DATA.reg;
  uint8_t interrupts = SERCOM0->SPI.INTFLAG.reg; //Read SPI interrupt register
  #ifdef DEBUG
    Serial.print("Interrupt: "); Serial.println(interrupts);
  #endif
  
  if (interrupts & (1 << 3)) // 8 = 1000 = SSL
  {
    #ifdef DEBUG
      Serial.println("SPI SSL Interupt");
    #endif
    SERCOM0->SPI.INTFLAG.bit.SSL = 1; //clear slave select interrupt
  }
  
  if (interrupts & (1 << 2)) // 4 = 0100 = RXC
  {
    #ifdef DEBUG
      Serial.println("SPI Data Received Complete Interrupt");
    #endif
    data = SERCOM0->SPI.DATA.reg; //Read data register
    SERCOM0->SPI.INTFLAG.bit.RXC = 1; //clear receive complete interrupt
  }
  
  if (interrupts & (1 << 1)) // 2 = 0010 = TXC
  {
    #ifdef DEBUG
      Serial.println("SPI Data Transmit Complete Interrupt");
    #endif
    SERCOM0->SPI.INTFLAG.bit.TXC = 1; //clear receive complete interrupt
  }

  if (interrupts & (1 << 0)) // 1 = 0001 = DRE
  {
    #ifdef DEBUG
      Serial.println("SPI Data Register Empty Interrupt");
    #endif
    SERCOM0->SPI.DATA.reg = 0xAA;
  }
  
  char _data = data;
  Serial.println(_data); // print received data
}
