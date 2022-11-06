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

/* Acronyms
  C        | Complete
  CH       | Character
  CHSIZE   | Character Size
  CFG      | Configuration
  CLK      | Clock
  CPHA     | Clock Phase
  CPOL     | Clock Polarity
  CTRLA    | Control A register
  CTRLB    | Control B register
  DIPO     | Data In Pinout
  DOPO     | Data Out Pinout
  DORD     | Data Order
  DRE      | Data register empty
  E        | Even
  EN       | Enable
  FORM     | Frame Format
  GCLK     | Generic Clock
  IBON     | Immediate Buffer Overflow Notification
  INT      | Interrupt
  INTENSET | Interrupt Enable Set
  MSB      | Most Significant Bit
  MISO     | Master In Slave Out
  MODE     | Operating Mode
  MOSI     | Master Out Slave In
  O        | Odd
  PINCFG   | Pin Configuration
  PLOAD    | Slave Data Preload
  PLOADEN  | Slave Data Preload Enable
  PMUX     | Peripheral Multiplexing
  PMUXE    | Even bits in the Peripheral Multiplexing register
  PMUXEN   | Peripheral Multiplexing Enable
  PMUXO    | Odd bits in the Peripheral Multiplexing register
  RST      | Reset
  RX       | Receive
  RXC      | Receive Complete
  RXEN     | Receiver Enable
  RUNSTDBY | Run in Standby
  SCK      | Serial Clock
  SERCOM   | Serial Communication Interface
  SET      | Set
  SIZE     | Size
  SPI      | Serial Peripheral Interface
  SS       | Slave Select
  SSDE     | Slave Select Low Detect Enable
  SSL      | Slave Select Low
  STDBY    | Standby
  SW       | Software
  SWRST    | Software Reset
  TX       | Transmit
  TXC      | Transmit Complete
*/

#include "SercomSPISlave.h"

// Constructors //

Sercom0SPISlave::Sercom0SPISlave() {}
Sercom1SPISlave::Sercom1SPISlave() {}
Sercom2SPISlave::Sercom2SPISlave() {}
Sercom3SPISlave::Sercom3SPISlave() {}
Sercom4SPISlave::Sercom4SPISlave() {}
Sercom5SPISlave::Sercom5SPISlave() {}
SercomSPISlave::SercomSPISlave() {}

// Public Methods //

void Sercom0SPISlave::SercomInit(MOSI_Pins MOSI_Pin, SCK_Pins SCK_Pin, SS_Pins SS_Pin, MISO_Pins MISO_Pin) {
  /* Explanation:
  The selection of peripheral function A to H is done by writing to the Peripheral Multiplexing Odd and Even bits in the Peripheral Multiplexing register (PMUXn.PMUXE/O) in the PORT.
  Reference: Atmel-42181G-SAM-D21_Datasheet section 6.1 on page 21
  
  In general:
  Px(2n+0/1) corresponds to Portx, PMUX[n], 0=Even=PMUXE/1=Odd=PMUXO
  
  Example:
  PA07 corresponds to PortA, PMUX[3], PMUXO
  */
  // Configure SERCOM0       SPI Pins   PAD  Explanation
  switch (MOSI_Pin) {
    case PA04:
      // Set PA04 as input   (MOSI)    0    PA04 corresponds to: PORTA, PMUX[2], Even
      PORT->Group[PORTA].PINCFG[4].bit.PMUXEN = 0x1; // Enable Peripheral Multiplexing for SERCOM0 SPI PA04
      PORT->Group[PORTA].PMUX[2].bit.PMUXE = 0x3; // SERCOM0 is selected for peripheral use of this pad (0x3 selects peripheral function D: SERCOM-ALT)
      break;
    case PA08:
      // Set PA08 as input   (MOSI)    0    PA08 corresponds to: PORTA, PMUX[4], Even
      PORT->Group[PORTA].PINCFG[8].bit.PMUXEN = 0x1; // Enable Peripheral Multiplexing for SERCOM0 SPI PA08
      PORT->Group[PORTA].PMUX[4].bit.PMUXE = 0x2; // SERCOM0 is selected for peripheral use of this pad (0x2 selects peripheral function C: SERCOM)
      break;
    default:
      #if ((MOSI_Pin != PA04) || (MOSI_Pin != PA08))
       #error "The pin selected is not available as MOSI_Pin for SERCOM0"
      #endif
      break;
  }

  switch (SCK_Pin) {
    case PA05:
      // Set PA05 as input   (SCK)     1    PA05 corresponds to: PORTA, PMUX[2], Odd
      PORT->Group[PORTA].PINCFG[5].bit.PMUXEN = 0x1; // Enable Peripheral Multiplexing for SERCOM0 SPI PA05
      PORT->Group[PORTA].PMUX[2].bit.PMUXO = 0x3; // SERCOM0 is selected for peripheral use of this pad (0x3 selects peripheral function D: SERCOM-ALT)
      break;
    case PA09:
      // Set PA09 as input   (SCK)     1    PA09 corresponds to: PORTA, PMUX[4], Odd
      PORT->Group[PORTA].PINCFG[9].bit.PMUXEN = 0x1; // Enable Peripheral Multiplexing for SERCOM0 SPI PA09
      PORT->Group[PORTA].PMUX[4].bit.PMUXO = 0x2; // SERCOM0 is selected for peripheral use of this pad (0x2 selects peripheral function C: SERCOM)
      break;
    default:
      #if ((SCK_Pin != PA05) || (SCK_Pin != PA09))
       #error "The pin selected is not available as SCK_Pin for SERCOM0"
      #endif
      break;
  }

  switch (SS_Pin) {
    case PA06:
      // Set PA06 as input   (SS)      2    PA06 corresponds to: PORTA, PMUX[3], Even
      PORT->Group[PORTA].PINCFG[6].bit.PMUXEN = 0x1; // Enable Peripheral Multiplexing for SERCOM0 SPI PA06
      PORT->Group[PORTA].PMUX[3].bit.PMUXE = 0x3; // SERCOM0 is selected for peripheral use of this pad (0x3 selects peripheral function D: SERCOM-ALT)
      break;
    case PA10:
      // Set PA10 as input   (SS)      2    PA10 corresponds to: PORTA, PMUX[5], Even
      PORT->Group[PORTA].PINCFG[10].bit.PMUXEN = 0x1; // Enable Peripheral Multiplexing for SERCOM0 SPI PA10
      PORT->Group[PORTA].PMUX[5].bit.PMUXE = 0x2; // SERCOM0 is selected for peripheral use of this pad (0x2 selects peripheral function C: SERCOM)
      break;
    default:
      #if ((SS_Pin != PA06) || (SS_Pin != PA10))
       #error "The pin selected is not available as SS_Pin for SERCOM0"
      #endif
      break;
  }

  switch (MISO_Pin) {
    case PA07:
      // Set PA07 as output  (MISO)    3    PA07 corresponds to: PORTA, PMUX[3], Odd
      PORT->Group[PORTA].PINCFG[7].bit.PMUXEN = 0x1; // Enable Peripheral Multiplexing for SERCOM0 SPI PA07
      PORT->Group[PORTA].PMUX[3].bit.PMUXO = 0x3; // SERCOM0 is selected for peripheral use of this pad (0x3 selects peripheral function D: SERCOM-ALT)
      break;
    case PA11:
      // Set PA11 as output  (MISO)    3    PA11 corresponds to: PORTA, PMUX[5], Odd
      PORT->Group[PORTA].PINCFG[11].bit.PMUXEN = 0x1; // Enable Peripheral Multiplexing for SERCOM0 SPI PA11
      PORT->Group[PORTA].PMUX[5].bit.PMUXO = 0x2; // SERCOM0 is selected for peripheral use of this pad (0x2 selects peripheral function C: SERCOM)
      break;
    default:
      #if ((MISO_Pin != PA07) || (MISO_Pin != PA11))
       #error "The pin selected is not available as MISO_Pin for SERCOM0"
      #endif
      break;
  }  

  // Initialize SERCOM0 registries
  SercomSPISlave RegInit;
  RegInit.SercomRegistryInit(SERCOM0);
}

void Sercom1SPISlave::SercomInit(MOSI_Pins MOSI_Pin, SCK_Pins SCK_Pin, SS_Pins SS_Pin, MISO_Pins MISO_Pin) {
  // Configure SERCOM1       SPI Pins  PAD  Explanation
  switch (MOSI_Pin) {
    case PA00:
      // Set PA00 as input   (MOSI)    0    PA00 corresponds to: PORTA, PMUX[0], Even
      PORT->Group[PORTA].PINCFG[0].bit.PMUXEN = 0x1; // Enable Peripheral Multiplexing for SERCOM1 SPI PA00
      PORT->Group[PORTA].PMUX[0].bit.PMUXE = 0x3; // SERCOM1 is selected for peripheral use of this pad (0x3 selects peripheral function D: SERCOM-ALT)
      break;
    case PA16:
      // Set PA16 as input   (MOSI)    0    PA16 corresponds to: PORTA, PMUX[8], Even
      PORT->Group[PORTA].PINCFG[16].bit.PMUXEN = 0x1; // Enable Peripheral Multiplexing for SERCOM1 SPI PA16, Arduino PIN11
      PORT->Group[PORTA].PMUX[8].bit.PMUXE = 0x2; // SERCOM1 is selected for peripheral use of this pad (0x2 selects peripheral function C: SERCOM)
      break;
    default:
      #if ((MOSI_Pin != PA00) || (MOSI_Pin != PA16))
       #error "The pin selected is not available as MOSI_Pin for SERCOM1"
      #endif
      break;
  }

  switch (SCK_Pin) {
    case PA01:
      // Set PA01 as input   (SCK)     1    PA01 corresponds to: PORTA, PMUX[0], Odd
      PORT->Group[PORTA].PINCFG[1].bit.PMUXEN = 0x1; // Enable Peripheral Multiplexing for SERCOM1 SPI PA01
      PORT->Group[PORTA].PMUX[0].bit.PMUXO = 0x3; // SERCOM1 is selected for peripheral use of this pad (0x3 selects peripheral function D: SERCOM-ALT)
      break;
    case PA17:
      // Set PA17 as input   (SCK)     1    PA17 corresponds to: PORTA, PMUX[8], Odd
      PORT->Group[PORTA].PINCFG[17].bit.PMUXEN = 0x1; // Enable Peripheral Multiplexing for SERCOM1 SPI PA17, Arduino PIN13
      PORT->Group[PORTA].PMUX[8].bit.PMUXO = 0x2; // SERCOM1 is selected for peripheral use of this pad (0x2 selects peripheral function C: SERCOM)
      break;
    default:
      #if ((SCK_Pin != PA01) || (SCK_Pin != PA17))
       #error "The pin selected is not available as SCK_Pin for SERCOM1"
      #endif
      break;
  }

  switch (SS_Pin) {
    case PA18:
      // Set PA18 as input   (SS)      2    PA18 corresponds to: PORTA, PMUX[9], Even
      PORT->Group[PORTA].PINCFG[18].bit.PMUXEN = 0x1; // Enable Peripheral Multiplexing for SERCOM1 SPI PA18, Arduino PIN10
      PORT->Group[PORTA].PMUX[9].bit.PMUXE = 0x2; // SERCOM1 is selected for peripheral use of this pad (0x2 selects peripheral function C: SERCOM)
      break;
    case PA30:
      // Set PA30 as input   (SS)      2    PA30 corresponds to: PORTA, PMUX[15], Even
      PORT->Group[PORTA].PINCFG[30].bit.PMUXEN = 0x1; // Enable Peripheral Multiplexing for SERCOM1 SPI PA30
      PORT->Group[PORTA].PMUX[15].bit.PMUXE = 0x3; // SERCOM1 is selected for peripheral use of this pad (0x3 selects peripheral function D: SERCOM-ALT)
      break;
    default:
      #if ((SS_Pin != PA18) || (SS_Pin != PA30))
       #error "The pin selected is not available as SS_Pin for SERCOM1"
      #endif
      break;
  }

  switch (MISO_Pin) {
    case PA19:
      // Set PA19 as output  (MISO)    3    PA19 corresponds to: PORTA, PMUX[9], Odd
      PORT->Group[PORTA].PINCFG[19].bit.PMUXEN = 0x1; // Enable Peripheral Multiplexing for SERCOM1 SPI PA19, Arduino PIN12
      PORT->Group[PORTA].PMUX[9].bit.PMUXO = 0x2; // SERCOM1 is selected for peripheral use of this pad (0x2 selects peripheral function C: SERCOM)
      break;
    case PA31:
      // Set PA31 as output  (MISO)    3    PA31 corresponds to: PORTA, PMUX[15], Odd
      PORT->Group[PORTA].PINCFG[31].bit.PMUXEN = 0x1; // Enable Peripheral Multiplexing for SERCOM1 SPI PA31
      PORT->Group[PORTA].PMUX[15].bit.PMUXO = 0x3; // SERCOM1 is selected for peripheral use of this pad (0x3 selects peripheral function D: SERCOM-ALT)
      break;
    default:
      #if ((MISO_Pin != PA19) || (MISO_Pin != PA31))
       #error "The pin selected is not available as MISO_Pin for SERCOM1"
      #endif
      break;
  }

  // Initialize SERCOM1 registries
  SercomSPISlave RegInit;
  RegInit.SercomRegistryInit(SERCOM1);
}

void Sercom2SPISlave::SercomInit(MOSI_Pins MOSI_Pin, SCK_Pins SCK_Pin, SS_Pins SS_Pin, MISO_Pins MISO_Pin) {
  // Configure SERCOM2       SPI Pins  PAD  Explanation
  switch (MOSI_Pin) {
    case PA08:
      // Set PA08 as input   (MOSI)    0    PA08 corresponds to: PORTA, PMUX[4], Even
      PORT->Group[PORTA].PINCFG[8].bit.PMUXEN = 0x1; // Enable Peripheral Multiplexing for SERCOM2 SPI PA08
      PORT->Group[PORTA].PMUX[4].bit.PMUXE = 0x3; // SERCOM2 is selected for peripheral use of this pad (0x3 selects peripheral function D: SERCOM-ALT)
      break;
    case PA12:
      // Set PA12 as input   (MOSI)    0    PA12 corresponds to: PORTA, PMUX[6], Even
      PORT->Group[PORTA].PINCFG[12].bit.PMUXEN = 0x1; // Enable Peripheral Multiplexing for SERCOM2 SPI PA16, Arduino PIN11
      PORT->Group[PORTA].PMUX[6].bit.PMUXE = 0x2; // SERCOM2 is selected for peripheral use of this pad (0x2 selects peripheral function C: SERCOM)
      break;
    default:
      #if ((MOSI_Pin != PA08) || (MOSI_Pin != PA12))
       #error "The pin selected is not available as MOSI_Pin for SERCOM2"
      #endif
      break;
  }

  switch (SCK_Pin) {
    case PA09:
      // Set PA09 as input   (SCK)     1    PA09 corresponds to: PORTA, PMUX[4], Odd
      PORT->Group[PORTA].PINCFG[9].bit.PMUXEN = 0x1; // Enable Peripheral Multiplexing for SERCOM2 SPI PA09
      PORT->Group[PORTA].PMUX[4].bit.PMUXO = 0x3; // SERCOM2 is selected for peripheral use of this pad (0x3 selects peripheral function D: SERCOM-ALT)
      break;
    case PA13:
      // Set PA13 as input   (SCK)     1    PA13 corresponds to: PORTA, PMUX[6], Odd
      PORT->Group[PORTA].PINCFG[13].bit.PMUXEN = 0x1; // Enable Peripheral Multiplexing for SERCOM2 SPI PA17, Arduino PIN13
      PORT->Group[PORTA].PMUX[6].bit.PMUXO = 0x2; // SERCOM2 is selected for peripheral use of this pad (0x2 selects peripheral function C: SERCOM)
      break;
    default:
      #if ((SCK_Pin != PA09) || (SCK_Pin != PA13))
       #error "The pin selected is not available as SCK_Pin for SERCOM2"
      #endif
      break;
  }

  switch (SS_Pin) {
    case PA10:
      // Set PA10 as input   (SS)      2    PA10 corresponds to: PORTA, PMUX[5], Even
      PORT->Group[PORTA].PINCFG[10].bit.PMUXEN = 0x1; // Enable Peripheral Multiplexing for SERCOM2 SPI PA10
      PORT->Group[PORTA].PMUX[5].bit.PMUXE = 0x3; // SERCOM2 is selected for peripheral use of this pad (0x3 selects peripheral function D: SERCOM-ALT)
      break;
    case PA14:
      // Set PA14 as input   (SS)      2    PA14 corresponds to: PORTA, PMUX[7], Even
      PORT->Group[PORTA].PINCFG[14].bit.PMUXEN = 0x1; // Enable Peripheral Multiplexing for SERCOM2 SPI PA14
      PORT->Group[PORTA].PMUX[7].bit.PMUXE = 0x2; // SERCOM2 is selected for peripheral use of this pad (0x2 selects peripheral function C: SERCOM)
      break;
    default:
      #if ((SS_Pin != PA10) || (SS_Pin != PA14))
       #error "The pin selected is not available as SS_Pin for SERCOM2"
      #endif
      break;
  }

  switch (MISO_Pin) {
    case PA11:
      // Set PA11 as output  (MISO)    3    PA11 corresponds to: PORTA, PMUX[5], Odd
      PORT->Group[PORTA].PINCFG[11].bit.PMUXEN = 0x1; // Enable Peripheral Multiplexing for SERCOM2 SPI PA11
      PORT->Group[PORTA].PMUX[5].bit.PMUXO = 0x3; // SERCOM2 is selected for peripheral use of this pad (0x3 selects peripheral function D: SERCOM-ALT)
      break;
    case PA15:
      // Set PA15 as output  (MISO)    3    PA15 corresponds to: PORTA, PMUX[7], Odd
      PORT->Group[PORTA].PINCFG[15].bit.PMUXEN = 0x1; // Enable Peripheral Multiplexing for SERCOM2 SPI PA15
      PORT->Group[PORTA].PMUX[7].bit.PMUXO = 0x2; // SERCOM2 is selected for peripheral use of this pad (0x2 selects peripheral function C: SERCOM)
      break;
    default:
      #if ((MISO_Pin != PA11) || (MISO_Pin != PA15))
       #error "The pin selected is not available as MISO_Pin for SERCOM2"
      #endif
      break;
  }

  // Initialize SERCOM2 registries
  SercomSPISlave RegInit;
  RegInit.SercomRegistryInit(SERCOM2);
}

void Sercom3SPISlave::SercomInit(MOSI_Pins MOSI_Pin, SCK_Pins SCK_Pin, SS_Pins SS_Pin, MISO_Pins MISO_Pin) {
  // Configure SERCOM3       SPI Pins   PAD  Explanation
  switch (MOSI_Pin) {
    case PA16:
      // Set PA16 as input   (MOSI)    0    PA16 corresponds to: PORTA, PMUX[8], Even
      PORT->Group[PORTA].PINCFG[16].bit.PMUXEN = 0x1; // Enable Peripheral Multiplexing for SERCOM3 SPI PA16, Arduino PIN11
      PORT->Group[PORTA].PMUX[8].bit.PMUXE = 0x3; // SERCOM3 is selected for peripheral use of this pad (0x3 selects peripheral function D: SERCOM-ALT)
      break;
    case PA22:
      // Set PA22 as input   (MOSI)    0    PA22 corresponds to: PORTA, PMUX[11], Even
      PORT->Group[PORTA].PINCFG[22].bit.PMUXEN = 0x1; // Enable Peripheral Multiplexing for SERCOM3 SPI PA22
      PORT->Group[PORTA].PMUX[11].bit.PMUXE = 0x2; // SERCOM3 is selected for peripheral use of this pad (0x2 selects peripheral function C: SERCOM)
      break;
    default:
      #if ((MOSI_Pin != PA16) || (MOSI_Pin != PA22))
       #error "The pin selected is not available as MOSI_Pin for SERCOM3"
      #endif
      break;
  }

  switch (SCK_Pin) {
    case PA17:
      // Set PA17 as input   (SCK)     1    PA17 corresponds to: PORTA, PMUX[8], Odd
      PORT->Group[PORTA].PINCFG[17].bit.PMUXEN = 0x1; // Enable Peripheral Multiplexing for SERCOM3 SPI PA17, Arduino PIN13
      PORT->Group[PORTA].PMUX[8].bit.PMUXO = 0x3; // SERCOM3 is selected for peripheral use of this pad (0x3 selects peripheral function D: SERCOM-ALT)
      break;
    case PA23:
      // Set PA23 as input   (SCK)     1    PA23 corresponds to: PORTA, PMUX[11], Odd
      PORT->Group[PORTA].PINCFG[23].bit.PMUXEN = 0x1; // Enable Peripheral Multiplexing for SERCOM3 SPI PA23
      PORT->Group[PORTA].PMUX[11].bit.PMUXO = 0x2; // SERCOM3 is selected for peripheral use of this pad (0x2 selects peripheral function C: SERCOM)
      break;
    default:
      #if ((SCK_Pin != PA17) || (SCK_Pin != PA23))
       #error "The pin selected is not available as SCK_Pin for SERCOM3"
      #endif
      break;
  }

  switch (SS_Pin) {
    case PA18:
      // Set PA18 as input   (SS)      2    PA18 corresponds to: PORTA, PMUX[9], Even
      PORT->Group[PORTA].PINCFG[18].bit.PMUXEN = 0x1; // Enable Peripheral Multiplexing for SERCOM3 SPI PA18, Arduino PIN10
      PORT->Group[PORTA].PMUX[9].bit.PMUXE = 0x3; // SERCOM3 is selected for peripheral use of this pad (0x3 selects peripheral function D: SERCOM-ALT)
      break;
    case PA20:
      // Set PA20 as input   (SS)      2    PA20 corresponds to: PORTA, PMUX[10], Even
      PORT->Group[PORTA].PINCFG[20].bit.PMUXEN = 0x1; // Enable Peripheral Multiplexing for SERCOM3 SPI PA20
      PORT->Group[PORTA].PMUX[10].bit.PMUXE = 0x3; // SERCOM3 is selected for peripheral use of this pad (0x3 selects peripheral function D: SERCOM-ALT)
      break;
    case PA24:
      // Set PA24 as input   (SS)      2    PA24 corresponds to: PORTA, PMUX[12], Even
      PORT->Group[PORTA].PINCFG[24].bit.PMUXEN = 0x1; // Enable Peripheral Multiplexing for SERCOM3 SPI PA24
      PORT->Group[PORTA].PMUX[12].bit.PMUXE = 0x2; // SERCOM3 is selected for peripheral use of this pad (0x2 selects peripheral function C: SERCOM)
      break;
    default:
      #if ((SS_Pin != PA18) || (SS_Pin != PA20) || (SS_Pin != PA24))
       #error "The pin selected is not available as SS_Pin for SERCOM3"
      #endif
      break;
  }

  switch (MISO_Pin) {
    case PA19:
      // Set PA19 as output  (MISO)    3    PA19 corresponds to: PORTA, PMUX[9], Odd
      PORT->Group[PORTA].PINCFG[19].bit.PMUXEN = 0x1; // Enable Peripheral Multiplexing for SERCOM3 SPI PA19, Arduino PIN12
      PORT->Group[PORTA].PMUX[9].bit.PMUXO = 0x3; // SERCOM3 is selected for peripheral use of this pad (0x3 selects peripheral function D: SERCOM-ALT)
      break;
    case PA21:
      // Set PA21 as output  (MISO)    3    PA21 corresponds to: PORTA, PMUX[10], Odd
      PORT->Group[PORTA].PINCFG[21].bit.PMUXEN = 0x1; // Enable Peripheral Multiplexing for SERCOM3 SPI PA21
      PORT->Group[PORTA].PMUX[10].bit.PMUXO = 0x3; // SERCOM3 is selected for peripheral use of this pad (0x3 selects peripheral function D: SERCOM-ALT)
      break;
    case PA25:
      // Set PA25 as output  (MISO)    3    PA25 corresponds to: PORTA, PMUX[12], Odd
      PORT->Group[PORTA].PINCFG[25].bit.PMUXEN = 0x1; // Enable Peripheral Multiplexing for SERCOM3 SPI PA25
      PORT->Group[PORTA].PMUX[12].bit.PMUXO = 0x2; // SERCOM3 is selected for peripheral use of this pad (0x2 selects peripheral function C: SERCOM)      
      break;
    default:
      #if ((MISO_Pin != PA19) || (MISO_Pin != PA21) || (MISO_Pin != PA25))
       #error "The pin selected is not available as MISO_Pin for SERCOM3"
      #endif
      break;
  }

  // Initialize SERCOM3 registries
  SercomSPISlave RegInit;
  RegInit.SercomRegistryInit(SERCOM3);
}

void Sercom4SPISlave::SercomInit(MOSI_Pins MOSI_Pin, SCK_Pins SCK_Pin, SS_Pins SS_Pin, MISO_Pins MISO_Pin) {
  // Configure SERCOM4       SPI Pins   PAD  Explanation
  switch (MOSI_Pin) {
    case PA12:
      // Set PA12 as input   (MOSI)     0    PA12 corresponds to: PORTA, PMUX[6], Even
      PORT->Group[PORTA].PINCFG[12].bit.PMUXEN = 0x1; // Enable Peripheral Multiplexing for SERCOM4 SPI PA12, Arduino PIN22
      PORT->Group[PORTA].PMUX[6].bit.PMUXE = 0x3; // SERCOM4 is selected for peripheral use of this pad (0x3 selects peripheral function D: SERCOM-ALT)
      break;
    case PB08:
      // Set PB08 as input   (MOSI)     0    PB08 corresponds to: PORTB, PMUX[4], Even
      PORT->Group[PORTB].PINCFG[8].bit.PMUXEN = 0x1; // Enable Peripheral Multiplexing for SERCOM4 SPI PB08
      PORT->Group[PORTB].PMUX[4].bit.PMUXE = 0x3; // SERCOM4 is selected for peripheral use of this pad (0x3 selects peripheral function D: SERCOM-ALT)  
      break;
    case PB12:
      // Set PB12 as input   (MOSI)     0    PB12 corresponds to: PORTB, PMUX[6], Even
      PORT->Group[PORTB].PINCFG[12].bit.PMUXEN = 0x1; // Enable Peripheral Multiplexing for SERCOM4 SPI PB12
      PORT->Group[PORTB].PMUX[6].bit.PMUXE = 0x2; // SERCOM4 is selected for peripheral use of this pad (0x2 selects peripheral function C: SERCOM)  
      break;
    default:
      #if ((MOSI_Pin != PA12) || (MOSI_Pin != PB08) || (MOSI_Pin != PB12))
       #error "The pin selected is not available as MOSI_Pin for SERCOM4"
      #endif
      break;
  }

  switch (SCK_Pin) {
    case PA13:
      // Set PA13 as input   (SCK)      1    PA13 corresponds to: PORTA, PMUX[6], Odd
      PORT->Group[PORTA].PINCFG[13].bit.PMUXEN = 0x1; // Enable Peripheral Multiplexing for SERCOM4 SPI PA13
      PORT->Group[PORTA].PMUX[6].bit.PMUXO = 0x3; // SERCOM4 is selected for peripheral use of this pad (0x3 selects peripheral function D: SERCOM-ALT)
      break;
    case PB09:
      // Set PB09 as input   (SCK)      1    PB09 corresponds to: PORTB, PMUX[4], Odd
      PORT->Group[PORTB].PINCFG[9].bit.PMUXEN = 0x1; // Enable Peripheral Multiplexing for SERCOM4 SPI PB09, Arduino PIN16
      PORT->Group[PORTB].PMUX[4].bit.PMUXO = 0x3; // SERCOM4 is selected for peripheral use of this pad (0x3 selects peripheral function D: SERCOM-ALT)
      break;
    case PB13:
      // Set PB13 as input   (SCK)      1    PB13 corresponds to: PORTB, PMUX[6], Odd
      PORT->Group[PORTB].PINCFG[13].bit.PMUXEN = 0x1; // Enable Peripheral Multiplexing for SERCOM4 SPI PB13
      PORT->Group[PORTB].PMUX[6].bit.PMUXO = 0x2; // SERCOM4 is selected for peripheral use of this pad (0x2 selects peripheral function C: SERCOM)
      break;
    default:
      #if ((SCK_Pin != PA13) || (SCK_Pin != PB09) || (SCK_Pin != PB13))
        #error "The pin selected is not available as SCK_Pin for SERCOM4"
      #endif
      break;
  }

  switch (SS_Pin) {
    case PA14:
      // Set PA14 as input   (SS)       2    PA14 corresponds to: PORTA, PMUX[7], Even
      PORT->Group[PORTA].PINCFG[14].bit.PMUXEN = 0x1; // Enable Peripheral Multiplexing for SERCOM4 SPI PA14
      PORT->Group[PORTA].PMUX[7].bit.PMUXE = 0x3; // SERCOM4 is selected for peripheral use of this pad (0x3 selects peripheral function D: SERCOM-ALT)
      break;
    case PB10:
      // Set PB10 as input   (SS)       2    PB10 corresponds to: PORTB, PMUX[5], Even
      PORT->Group[PORTB].PINCFG[10].bit.PMUXEN = 0x1; // Enable Peripheral Multiplexing for SERCOM4 SPI PB10, Arduino PIN23
      PORT->Group[PORTB].PMUX[5].bit.PMUXE = 0x3; // SERCOM4 is selected for peripheral use of this pad (0x3 selects peripheral function D: SERCOM-ALT)
      break;
    case PB14:
      // Set PB14 as input   (SS)       2    PB14 corresponds to: PORTB, PMUX[7], Even
      PORT->Group[PORTB].PINCFG[14].bit.PMUXEN = 0x1; // Enable Peripheral Multiplexing for SERCOM4 SPI PB14
      PORT->Group[PORTB].PMUX[7].bit.PMUXE = 0x2; // SERCOM4 is selected for peripheral use of this pad (0x2 selects peripheral function C: SERCOM)
      break;
    default:
      #if ((SS_Pin != PA14) || (SS_Pin != PB10) || (SS_Pin != PB14))
        #error "The pin selected is not available as SS_Pin for SERCOM4"
      #endif
      break;
  }

  switch (MISO_Pin) {
    case PA15:
      // Set PA15 as output  (MISO)     3    PA15 corresponds to: PORTA, PMUX[7], Odd
      PORT->Group[PORTA].PINCFG[15].bit.PMUXEN = 0x1; // Enable Peripheral Multiplexing for SERCOM4 SPI PA15, Arduino PIN23
      PORT->Group[PORTA].PMUX[7].bit.PMUXO = 0x3; // SERCOM4 is selected for peripheral use of this pad (0x3 selects peripheral function D: SERCOM-ALT)
      break;
    case PB11:
      // Set PB11 as output  (MISO)     3    PB11 corresponds to: PORTB, PMUX[5], Odd
      PORT->Group[PORTB].PINCFG[11].bit.PMUXEN = 0x1; // Enable Peripheral Multiplexing for SERCOM4 SPI PB11, Arduino PIN24
      PORT->Group[PORTB].PMUX[5].bit.PMUXO = 0x3; // SERCOM4 is selected for peripheral use of this pad (0x3 selects peripheral function D: SERCOM-ALT)
      break;
    case PB15:
      // Set PB15 as output  (MISO)     3    PB15 corresponds to: PORTB, PMUX[7], Odd
      PORT->Group[PORTB].PINCFG[15].bit.PMUXEN = 0x1; // Enable Peripheral Multiplexing for SERCOM4 SPI PB15
      PORT->Group[PORTB].PMUX[7].bit.PMUXO = 0x2; // SERCOM4 is selected for peripheral use of this pad (0x2 selects peripheral function C: SERCOM)
      break;
    default:
      #if ((MISO_Pin != PA15) || (MISO_Pin != PB11) || (MISO_Pin != PB15))
        #error "The pin selected is not available as MISO_Pin for SERCOM4"
      #endif
      break;
  }

  // Initialize SERCOM4 registries
  SercomSPISlave RegInit;
  RegInit.SercomRegistryInit(SERCOM4);
}

void Sercom5SPISlave::SercomInit(MOSI_Pins MOSI_Pin, SCK_Pins SCK_Pin, SS_Pins SS_Pin, MISO_Pins MISO_Pin) {
  // Configure SERCOM5       SPI Pins  PAD  Explanation
  switch (MOSI_Pin) {
    case PB02:
      // Set PB02 as input   (MOSI)    0    PB02 corresponds to: PORTB, PMUX[1], Even
      PORT->Group[PORTB].PINCFG[2].bit.PMUXEN = 0x1; // Enable Peripheral Multiplexing for SERCOM5 SPI PB02
      PORT->Group[PORTB].PMUX[1].bit.PMUXE = 0x3; // SERCOM5 is selected for peripheral use of this pad (0x3 selects peripheral function D: SERCOM-ALT)
      break;
    case PB16:
      // Set PB16 as input   (MOSI)    0    PB16 corresponds to: PORTB, PMUX[8], Even
      PORT->Group[PORTB].PINCFG[16].bit.PMUXEN = 0x1; // Enable Peripheral Multiplexing for SERCOM5 SPI PB16
      PORT->Group[PORTB].PMUX[8].bit.PMUXE = 0x2; // SERCOM5 is selected for peripheral use of this pad (0x2 selects peripheral function C: SERCOM)
      break;
    case PB30:
      // Set PB30 as input   (MOSI)    0    PB30 corresponds to: PORTB, PMUX[15], Even
      PORT->Group[PORTB].PINCFG[30].bit.PMUXEN = 0x1; // Enable Peripheral Multiplexing for SERCOM5 SPI PB30
      PORT->Group[PORTB].PMUX[15].bit.PMUXE = 0x3; // SERCOM5 is selected for peripheral use of this pad (0x3 selects peripheral function D: SERCOM-ALT)
      break;
    default:
      #if ((MOSI_Pin != PB02) || (MOSI_Pin != PB16) || (MOSI_Pin != PB30))
       #error "The pin selected is not available as MOSI_Pin for SERCOM5"
      #endif
      break;
  }

  switch (SCK_Pin) {
    case PB03:
      // Set PB03 as input   (SCK)     1    PB03 corresponds to: PORTB, PMUX[1], Odd
      PORT->Group[PORTB].PINCFG[3].bit.PMUXEN = 0x1; // Enable Peripheral Multiplexing for SERCOM5 SPI PB03
      PORT->Group[PORTB].PMUX[1].bit.PMUXO = 0x3; // SERCOM5 is selected for peripheral use of this pad (0x3 selects peripheral function D: SERCOM-ALT)
      break;
    case PB17:
      // Set PB17 as input   (SCK)     1    PB17 corresponds to: PORTB, PMUX[8], Odd
      PORT->Group[PORTB].PINCFG[17].bit.PMUXEN = 0x1; // Enable Peripheral Multiplexing for SERCOM5 SPI PB17
      PORT->Group[PORTB].PMUX[8].bit.PMUXO = 0x2; // SERCOM5 is selected for peripheral use of this pad (0x2 selects peripheral function C: SERCOM)
      break;
    case PB31:
      // Set PB31 as input   (SCK)     1    PB31 corresponds to: PORTB, PMUX[15], Odd
      PORT->Group[PORTB].PINCFG[31].bit.PMUXEN = 0x1; // Enable Peripheral Multiplexing for SERCOM5 SPI PB31
      PORT->Group[PORTB].PMUX[15].bit.PMUXO = 0x3; // SERCOM5 is selected for peripheral use of this pad (0x3 selects peripheral function D: SERCOM-ALT)
      break;
    default:
      #if ((SCK_Pin != PB03) || (SCK_Pin != PB17) || (SCK_Pin != PB31))
        #error "The pin selected is not available as SCK_Pin for SERCOM5"
      #endif
      break;
  }

  switch (SS_Pin) {
    case PA20:
      // Set PA20 as input   (SS)      2    PA20 corresponds to: PORTA, PMUX[10], Even
      PORT->Group[PORTA].PINCFG[20].bit.PMUXEN = 0x1; // Enable Peripheral Multiplexing for SERCOM5 SPI PA20
      PORT->Group[PORTA].PMUX[10].bit.PMUXE = 0x2; // SERCOM5 is selected for peripheral use of this pad (0x2 selects peripheral function C: SERCOM)
      break;
    case PA24:
      // Set PA24 as input   (SS)      2    PA24 corresponds to: PORTA, PMUX[12], Even
      PORT->Group[PORTA].PINCFG[24].bit.PMUXEN = 0x1; // Enable Peripheral Multiplexing for SERCOM5 SPI PA24
      PORT->Group[PORTA].PMUX[12].bit.PMUXE = 0x3; // SERCOM5 is selected for peripheral use of this pad (0x3 selects peripheral function D: SERCOM-ALT)
      break;
    case PB00:
      // Set PB00 as input   (SS)      2    PB00 corresponds to: PORTB, PMUX[0], Even
      PORT->Group[PORTB].PINCFG[0].bit.PMUXEN = 0x1; // Enable Peripheral Multiplexing for SERCOM5 SPI PB00
      PORT->Group[PORTB].PMUX[0].bit.PMUXE = 0x3; // SERCOM5 is selected for peripheral use of this pad (0x3 selects peripheral function D: SERCOM-ALT)
      break;
    case PB22:
      // Set PB22 as input   (SS)      2    PB22 corresponds to: PORTB, PMUX[11], Even
      PORT->Group[PORTB].PINCFG[22].bit.PMUXEN = 0x1; // Enable Peripheral Multiplexing for SERCOM5 SPI PB22
      PORT->Group[PORTB].PMUX[11].bit.PMUXE = 0x3; // SERCOM5 is selected for peripheral use of this pad (0x3 selects peripheral function D: SERCOM-ALT)
      break;
    default:
      #if ((SS_Pin != PA20) || (SS_Pin != PA24) || (SS_Pin != PB00) || (SS_Pin != PB22))
        #error "The pin selected is not available as SS_Pin for SERCOM5"
      #endif
      break;
  }
  
  switch (MISO_Pin) {
    case PA21:
      // Set PA21 as output  (MISO)    3    PA21 corresponds to: PORTA, PMUX[10], Odd
      PORT->Group[PORTA].PINCFG[21].bit.PMUXEN = 0x1; // Enable Peripheral Multiplexing for SERCOM5 SPI PA21
      PORT->Group[PORTA].PMUX[10].bit.PMUXO = 0x2; // SERCOM5 is selected for peripheral use of this pad (0x2 selects peripheral function C: SERCOM)
      break;
    case PA25:
      // Set PA25 as output  (MISO)    3    PA25 corresponds to: PORTA, PMUX[12], Odd
      PORT->Group[PORTA].PINCFG[25].bit.PMUXEN = 0x1; // Enable Peripheral Multiplexing for SERCOM5 SPI PA25
      PORT->Group[PORTA].PMUX[12].bit.PMUXO = 0x3; // SERCOM5 is selected for peripheral use of this pad (0x3 selects peripheral function D: SERCOM-ALT)
      break;
    case PB01:
      // Set PB01 as output  (MISO)    3    PB01 corresponds to: PORTB, PMUX[0], Odd
      PORT->Group[PORTB].PINCFG[1].bit.PMUXEN = 0x1; // Enable Peripheral Multiplexing for SERCOM5 SPI PB01
      PORT->Group[PORTB].PMUX[0].bit.PMUXO = 0x3; // SERCOM5 is selected for peripheral use of this pad (0x3 selects peripheral function D: SERCOM-ALT)
      break;
    case PB23:
      // Set PB23 as output  (MISO)    3    PB23 corresponds to: PORTB, PMUX[11], Odd
      PORT->Group[PORTB].PINCFG[23].bit.PMUXEN = 0x1; // Enable Peripheral Multiplexing for SERCOM5 SPI PB23
      PORT->Group[PORTB].PMUX[11].bit.PMUXO = 0x3; // SERCOM5 is selected for peripheral use of this pad (0x3 selects peripheral function D: SERCOM-ALT)
      break;
    default:
      #if ((MISO_Pin != PA21) || (MISO_Pin != PA25) || (MISO_Pin != PB01) || (MISO_Pin != PB23))
        #error "The pin selected is not available as MISO_Pin for SERCOM5"
      #endif
      break;
  }

  // Initialize SERCOM5 registries
  SercomSPISlave RegInit;
  RegInit.SercomRegistryInit(SERCOM5);
}

// Private Methods //

void SercomSPISlave::SercomRegistryInit(Sercom* sercom_x) {
  int sercom_no = -1; // Initialize sercom number to be used in switch case. Initialize to -1 such that it has an invalid value if it is not explicitely defined.
  if (sercom_x == SERCOM0) {
    sercom_no = 0;
  } else if (sercom_x == SERCOM1) {
    sercom_no = 1;
  } else if (sercom_x == SERCOM2) {
    sercom_no = 2;
  } else if (sercom_x == SERCOM3) {
    sercom_no = 3;
  } else if (sercom_x == SERCOM4) {
    sercom_no = 4;
  } else if (sercom_x == SERCOM5) {
    sercom_no = 5;
  }

  // Disable SPI 1
  sercom_x->SPI.CTRLA.bit.ENABLE = 0; // page 481
  while (sercom_x->SPI.SYNCBUSY.bit.ENABLE); // Wait until bit is enabled.
  
  // Reset SPI 1
  sercom_x->SPI.CTRLA.bit.SWRST = 1; // page 481
  while (sercom_x->SPI.CTRLA.bit.SWRST || sercom_x->SPI.SYNCBUSY.bit.SWRST); // Wait until software reset is complete.
  
  // Setting up Nested Vectored Interrupt Controller (NVIC) and Generic Clock Controller
  switch (sercom_no) { // specific code for each SERCOM
    case 0:
      NVIC_EnableIRQ(SERCOM0_IRQn);
      NVIC_SetPriority(SERCOM0_IRQn, 2);
      GCLK->CLKCTRL.reg = GCLK_CLKCTRL_ID(GCM_SERCOM0_CORE) | // Generic Clock 0
                          GCLK_CLKCTRL_GEN_GCLK0 | // Generic Clock Generator 0 is the source
                          GCLK_CLKCTRL_CLKEN; // Enable Generic Clock Generator
      break;
    case 1:
      NVIC_EnableIRQ(SERCOM1_IRQn);
      NVIC_SetPriority(SERCOM1_IRQn, 2);
      GCLK->CLKCTRL.reg = GCLK_CLKCTRL_ID(GCM_SERCOM1_CORE) | // Generic Clock 0
                          GCLK_CLKCTRL_GEN_GCLK0 | // Generic Clock Generator 0 is the source
                          GCLK_CLKCTRL_CLKEN; // Enable Generic Clock Generator
      break;
    case 2:
      NVIC_EnableIRQ(SERCOM2_IRQn);
      NVIC_SetPriority(SERCOM2_IRQn, 2);
      GCLK->CLKCTRL.reg = GCLK_CLKCTRL_ID(GCM_SERCOM2_CORE) | // Generic Clock 0
                          GCLK_CLKCTRL_GEN_GCLK0 | // Generic Clock Generator 0 is the source
                          GCLK_CLKCTRL_CLKEN; // Enable Generic Clock Generator
      break;
    case 3:
      NVIC_EnableIRQ(SERCOM3_IRQn);
      NVIC_SetPriority(SERCOM3_IRQn, 2);
      GCLK->CLKCTRL.reg = GCLK_CLKCTRL_ID(GCM_SERCOM3_CORE) | // Generic Clock 0
                          GCLK_CLKCTRL_GEN_GCLK0 | // Generic Clock Generator 0 is the source
                          GCLK_CLKCTRL_CLKEN; // Enable Generic Clock Generator
      break;
    case 4:
      NVIC_EnableIRQ(SERCOM4_IRQn);
      NVIC_SetPriority(SERCOM4_IRQn, 2);
      GCLK->CLKCTRL.reg = GCLK_CLKCTRL_ID(GCM_SERCOM4_CORE) | // Generic Clock 0
                          GCLK_CLKCTRL_GEN_GCLK0 | // Generic Clock Generator 0 is the source
                          GCLK_CLKCTRL_CLKEN; // Enable Generic Clock Generator
      break;
    case 5:
      NVIC_EnableIRQ(SERCOM5_IRQn);
      NVIC_SetPriority(SERCOM5_IRQn, 2);
      GCLK->CLKCTRL.reg = GCLK_CLKCTRL_ID(GCM_SERCOM5_CORE) | // Generic Clock 0
                          GCLK_CLKCTRL_GEN_GCLK0 | // Generic Clock Generator 0 is the source
                          GCLK_CLKCTRL_CLKEN; // Enable Generic Clock Generator
      break;
    default:
      #if ((sercom_no < 0) || (sercom_no > 5))
        #warning "The SERCOM selected is not supported by this library."
      #endif
      break;
  }

  while (GCLK->STATUS.reg & GCLK_STATUS_SYNCBUSY); // Wait for synchronisation

  // Set up SPI control A register
  sercom_x->SPI.CTRLA.bit.DORD = 0; // MSB is transferred first. // page 492
  sercom_x->SPI.CTRLA.bit.CPOL = 0; // SCK is low when idle. The leading edge of a clock cycle is a rising edge, while the trailing edge is a falling edge. // page 492
  sercom_x->SPI.CTRLA.bit.CPHA = 0; // Data is sampled on a leading SCK edge and changed on a trailing SCK edge. // page 492
  sercom_x->SPI.CTRLA.bit.FORM = 0x0; // SPI frame // page 493
  sercom_x->SPI.CTRLA.bit.DIPO = 0x0; // DATA PAD0 is used as slave input: MOSI // (slave mode) page 493
  sercom_x->SPI.CTRLA.bit.DOPO = 0x2; // DATA PAD2 is used as slave output: MISO // (slave mode) page 493
  sercom_x->SPI.CTRLA.bit.MODE = 0x2; // SPI slave operation. // page 494
  sercom_x->SPI.CTRLA.bit.IBON = 0x1; // Immediate Buffer Overflow Notification. STATUS.BUFOVF is asserted immediately upon buffer overflow. // page 494
  sercom_x->SPI.CTRLA.bit.RUNSTDBY = 1; // Wake on Receive Complete interrupt. // page 494

  // Set up SPI control B register
  //sercom_x->SPI.CTRLB.bit.RXEN = 0x1; // Enable Receiver // page 496
  sercom_x->SPI.CTRLB.bit.SSDE = 0x1; // Enable Slave Select Low Detect // page 497
  sercom_x->SPI.CTRLB.bit.CHSIZE = 0; // Character Size 8 bits // page 497
  //sercom_x->SPI.CTRLB.bit.PLOADEN = 0x1; // Enable Slave Data Preload // page 497
  //while (sercom_x->SPI.SYNCBUSY.bit.CTRLB); // Wait until receiver is enabled

  // Set up SPI interrupts
  sercom_x->SPI.INTENSET.bit.SSL = 0x1; // Enable Slave Select Low interrupt. // page 501
  sercom_x->SPI.INTENSET.bit.RXC = 0x1; // Enable Receive Complete interrupt. // page 501
  sercom_x->SPI.INTENSET.bit.TXC = 0x1; // Enable Transmit Complete interrupt. // page 501
  sercom_x->SPI.INTENSET.bit.ERROR = 0x1; // Enable Error interrupt. // page 501
  sercom_x->SPI.INTENSET.bit.DRE = 0x1; // Enable Data Register Empty interrupt. // page 501

  // Init SPI CLK // not used in SPI slave operation // page 481
  //sercom_x->SPI.BAUD.reg = SERCOM_FREQ_REF / (2*4000000u)-1;

  // Enable SPI
  sercom_x->SPI.CTRLA.bit.ENABLE = 1; // page 481
  while (sercom_x->SPI.SYNCBUSY.bit.ENABLE); // Wait until bit is enabled.
  sercom_x->SPI.CTRLB.bit.RXEN = 0x1; // Enable Receiver // page 496. This is done here rather than in section "Set up SPI control B register" due to an errate issue.
  while (sercom_x->SPI.SYNCBUSY.bit.CTRLB); // Wait until receiver is enabled.
}
