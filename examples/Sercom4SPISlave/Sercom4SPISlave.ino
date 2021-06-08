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
  Example code for the SercomSPISlave library.
  This code initializes a Sercom4 SPI Slave and prints the data received.

  2020 July 15 written by lenvm
  2021 June 8 updated by lenvm
*/

#include <SercomSPISlave.h>
SercomSPISlave SPISlave;

#define DEBUG // comment this line out to not print debug data on the serial bus

// initialize variables
byte buf[1];

void setup()
{  
  Serial.begin(115200);
  Serial.println("Serial started");
  SPISlave.Sercom4init();
  Serial.println("Sercom4 SPI slave initialized");
}

void loop()
{
  Serial.println(buf[0]);
  delay(1);
}

void SERCOM4_Handler() // 25.7 Register Summary, page 454 atmel 42181, samd21
{
  #ifdef DEBUG
    Serial.println("In SPI Interrupt");
  #endif
  uint8_t data = 0;
  uint8_t interrupts = SERCOM4->SPI.INTFLAG.reg; //Read SPI interrupt register
  #ifdef DEBUG
    Serial.print("Interrupt: "); Serial.println(interrupts);
  #endif
  
  if(interrupts & (1<<3)) // 8 = 1000 = SSL
  {
  #ifdef DEBUG
    Serial.println("SPI SSL Interupt");
  #endif
    SERCOM4->SPI.INTFLAG.bit.SSL = 1; //clear slave select interrupt
    //data = SERCOM4->SPI.DATA.reg; //Read data register
    #ifdef DEBUG
      Serial.print("DATA: "); Serial.println(data);
    #endif
    //SERCOM4->SPI.INTFLAG.bit.RXC = 1; //clear receive complete interrupt
  }
  
  // This is where data is received, and is written to a buffer, which is used in the main loop
  if(interrupts & (1<<2)) // 4 = 0100 = RXC
  {
    #ifdef DEBUG
      Serial.println("SPI Data Received Complete Interrupt");
    #endif
    data = SERCOM4->SPI.DATA.reg; //Read data register
    buf[0] = data; // copy data to buffer
    #ifdef DEBUG
      Serial.print("DATA: ");
      Serial.println(data);
    #endif
    SERCOM4->SPI.INTFLAG.bit.RXC = 1; //clear receive complete interrupt
  }
  
  if(interrupts & (1<<1)) // 2 = 0010 = TXC
  {
  #ifdef DEBUG
    Serial.println("SPI Data Transmit Complete Interrupt");
  #endif
    SERCOM4->SPI.INTFLAG.bit.TXC = 1; //clear receive complete interrupt
  }
  
  if(interrupts & (1<<0)) // 1 = 0001 = DRE
  {
    #ifdef DEBUG
      Serial.println("SPI Data Register Empty Interrupt");
    #endif
    SERCOM4->SPI.DATA.reg = 0xAA;
    //SERCOM4->SPI.INTFLAG.bit.DRE = 1;
  }
  
  #ifdef DEBUG
    Serial.println("----------");
  #endif
}
