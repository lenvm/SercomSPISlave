/*  
  Copyright (C) 2022 lenvm

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
  Example code for the SercomSPISlave library.
  This code initializes a SERCOM0 SPI Slave and prints the data received.

  Written 2020 July 15
  by lenvm
  Modified 2020 December 30
  by stubb
  Updated 2021 June 8
  by lenvm
  Updated 2022 November 1
  by lenvm
*/

#include <SercomSPISlave.h>
Sercom0SPISlave SPISlave; // to use a different SERCOM, change this line and find and replace all SERCOM0 with the SERCOM of your choice

#define DEBUG // uncomment this line to print debug data to the serial bus
#define INTERRUPT2BUFFER // uncomment this line to copy the data received in the Data Received Complete interrupt to a buffer to be used in the main loop
//#define INTERRUPT2SERIAL // uncomment this line to print the data to the serial bus whenever the Data Received Complete interrupt is triggered

// initialize variables
byte buf[1]; // initialize a buffer of 1 byte

void setup()
{
  Serial.begin(115200);
  Serial.println("Serial started");
  SPISlave.SercomInit(SPISlave.MOSI_Pins::PA08, SPISlave.SCK_Pins::PA09, SPISlave.SS_Pins::PA10, SPISlave.MISO_Pins::PA11);
  Serial.println("SERCOM0 SPI slave initialized");
}

void loop()
{
  #ifdef INTERRUPT2BUFFER
    Serial.println(buf[0]); // Print latest data written into the buffer by the interrupt
    delay(1); // Delay of 1 ms
  #endif
  #ifdef INTERRUPT2SERIAL
    delay(1000); // Delay of 1 s to keep the main loop running, while data is written to the serial every time the Data Received Interrupt is triggered
  #endif
}

void SERCOM0_Handler()
/*
Reference: Atmel-42181G-SAM-D21_Datasheet section 26.8.6 on page 503
*/
{ 
  #ifdef DEBUG
    Serial.println("In SPI Interrupt");
  #endif
  uint8_t data = 0;
  data = (uint8_t)SERCOM0->SPI.DATA.reg;
  uint8_t interrupts = SERCOM0->SPI.INTFLAG.reg; // Read SPI interrupt register
  #ifdef DEBUG
    Serial.print("Interrupt: "); Serial.println(interrupts);
  #endif
  
  // Slave Select Low interrupt
  if (interrupts & (1 << 3)) // 1000 = bit 3 = SSL // page 503
  {
    #ifdef DEBUG
      Serial.println("SPI Slave Select Low interupt");
    #endif
    SERCOM0->SPI.INTFLAG.bit.SSL = 1; // Clear Slave Select Low interrupt
  }
  
  // Data Received Complete interrupt: this is where the data is received, which is used in the main loop
  if (interrupts & (1 << 2)) // 0100 = bit 2 = RXC // page 503
  {
    #ifdef DEBUG
      Serial.println("SPI Data Received Complete interrupt");
    #endif
    data = SERCOM0->SPI.DATA.reg; // Read data register
    SERCOM0->SPI.INTFLAG.bit.RXC = 1; // Clear Receive Complete interrupt
  }
  
  // Data Transmit Complete interrupt
  if (interrupts & (1 << 1)) // 0010 = bit 1 = TXC // page 503
  {
    #ifdef DEBUG
      Serial.println("SPI Data Transmit Complete interrupt");
    #endif
    SERCOM0->SPI.INTFLAG.bit.TXC = 1; // Clear Transmit Complete interrupt
  }
  
  // Data Register Empty interrupt
  if (interrupts & (1 << 0)) // 0001 = bit 0 = DRE // page 503
  {
    #ifdef DEBUG
      Serial.println("SPI Data Register Empty interrupt");
    #endif
    SERCOM0->SPI.DATA.reg = 0xAA;
  }
  
  #ifdef INTERRUPT2BUFFER
    // Write data to buffer, to be used in main loop
    buf[0] = data;
  #endif
  #ifdef INTERRUPT2SERIAL
    // Print data received during the Data Receive Complete interrupt
    char _data = data;
    Serial.print("DATA: ");
    Serial.println(_data); // Print received data
  #endif

}
