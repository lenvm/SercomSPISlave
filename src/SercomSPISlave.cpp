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
SercomSPISlave::SercomSPISlave()
{
    
}

// Public Methods //
void SercomSPISlave::Sercom0init()
{
  /* Explanation:
  The selection of peripheral function A to H is done by writing to the Peripheral Multiplexing Odd and Even bits in the Peripheral Multiplexing register (PMUXn.PMUXE/O) in the PORT.
  Reference: Atmel-42181G-SAM-D21_Datasheet section 6.1 on page 21
  
  In general:
  Px(2n+0/1) corresponds to Portx, PMUX[n], 0=Even=PMUXE/1=Odd=PMUXO
  
  Example:
  PA11 corresponds to PortA, PMUX[5], PMUXO
  */

  // Configure SERCOM0     SPI Pins  PAD  Explanation
    // Set PA08 as input   (MOSI)    0    PA08 corresponds to: PORTA, PMUX[4], Even
    // Set PA09 as input   (SCK)     1    PA09 corresponds to: PORTA, PMUX[4], Odd
    // Set PA10 as input   (SS)      2    PA10 corresponds to: PORTA, PMUX[5], Even
    // Set PA11 as output  (MISO)    3    PA11 corresponds to: PORTA, PMUX[5], Odd
  PORT->Group[PORTA].PINCFG[8].bit.PMUXEN = 0x1; // Enable Peripheral Multiplexing for SERCOM0 SPI PA08
  PORT->Group[PORTA].PMUX[4].bit.PMUXE = 0x2; // SERCOM 0 is selected for peripheral use of this pad (0x2 selects peripheral function C: SERCOM)
  PORT->Group[PORTA].PINCFG[9].bit.PMUXEN = 0x1; // Enable Peripheral Multiplexing for SERCOM0 SPI PA09
  PORT->Group[PORTA].PMUX[4].bit.PMUXO = 0x2; // SERCOM 0 is selected for peripheral use of this pad (0x2 selects peripheral function C: SERCOM)
  PORT->Group[PORTA].PINCFG[10].bit.PMUXEN = 0x1; // Enable Peripheral Multiplexing for SERCOM0 SPI PA10
  PORT->Group[PORTA].PMUX[5].bit.PMUXE = 0x2; // SERCOM 0 is selected for peripheral use of this pad (0x2 selects peripheral function C: SERCOM)
  PORT->Group[PORTA].PINCFG[11].bit.PMUXEN = 0x1; // Enable Peripheral Multiplexing for SERCOM0 SPI PA11
  PORT->Group[PORTA].PMUX[5].bit.PMUXO = 0x2; // SERCOM 0 is selected for peripheral use of this pad (0x2 selects peripheral function C: SERCOM)
  
  // Disable SPI 1
  SERCOM0->SPI.CTRLA.bit.ENABLE = 0; // page 481
  while(SERCOM0->SPI.SYNCBUSY.bit.ENABLE); // Wait until bit is enabled
  
  // Reset SPI 1
  SERCOM0->SPI.CTRLA.bit.SWRST = 1; // page 481
  while(SERCOM0->SPI.CTRLA.bit.SWRST || SERCOM0->SPI.SYNCBUSY.bit.SWRST); // Wait until software reset is complete
  
  // Setting up Nested Vectored Interrupt Controller (NVIC)
  NVIC_EnableIRQ(SERCOM0_IRQn);
  NVIC_SetPriority(SERCOM0_IRQn,2);
  
  // Setting Generic Clock Controller
  GCLK->CLKCTRL.reg = GCLK_CLKCTRL_ID(GCM_SERCOM0_CORE) | // Generic Clock 0
            GCLK_CLKCTRL_GEN_GCLK0 | // Generic Clock Generator 0 is the source
            GCLK_CLKCTRL_CLKEN; // Enable Generic Clock Generator
  
  while(GCLK->STATUS.reg & GCLK_STATUS_SYNCBUSY); // Wait for synchronisation
  
  // Set up SPI control A register
  SERCOM0->SPI.CTRLA.bit.DORD = 0; // MSB is transferred first. // page 492
  SERCOM0->SPI.CTRLA.bit.CPOL = 0; // SCK is low when idle. The leading edge of a clock cycle is a rising edge, while the trailing edge is a falling edge. // page 492
  SERCOM0->SPI.CTRLA.bit.CPHA = 0; // Data is sampled on a leading SCK edge and changed on a trailing SCK edge. // page 492
  SERCOM0->SPI.CTRLA.bit.FORM = 0x0; // SPI frame // page 493
  SERCOM0->SPI.CTRLA.bit.DIPO = 0x0; // DATA PAD0 is used as slave input: MOSI // (slave mode) page 493
  SERCOM0->SPI.CTRLA.bit.DOPO = 0x2; // DATA PAD2 is used as slave output: MISO // (slave mode) page 493
  SERCOM0->SPI.CTRLA.bit.MODE = 0x2; // SPI slave operation. // page 494
  SERCOM0->SPI.CTRLA.bit.IBON = 0x1; // Immediate Buffer Overflow Notification. STATUS.BUFOVF is asserted immediately upon buffer overflow. // page 494
  SERCOM0->SPI.CTRLA.bit.RUNSTDBY = 1; // Wake on Receive Complete interrupt. // page 494
  
  // Set up SPI control B register
  //SERCOM0->SPI.CTRLB.bit.RXEN = 0x1; // Enable Receiver // page 496
  SERCOM0->SPI.CTRLB.bit.SSDE = 0x1; // Enable Slave Select Low Detect // page 497
  SERCOM0->SPI.CTRLB.bit.CHSIZE = 0; // Character Size 8 bits // page 497
  //SERCOM0->SPI.CTRLB.bit.PLOADEN = 0x1; // Enable Slave Data Preload // page 497
  //while(SERCOM0->SPI.SYNCBUSY.bit.CTRLB); // Wait until receiver is enabled
  
  // Set up SPI interrupts
  SERCOM0->SPI.INTENSET.bit.SSL = 0x1; // Enable Slave Select Low interrupt. // page 501
  SERCOM0->SPI.INTENSET.bit.RXC = 0x1; // Enable Receive Complete interrupt. // page 501
  SERCOM0->SPI.INTENSET.bit.TXC = 0x1; // Enable Transmit Complete interrupt. // page 501
  SERCOM0->SPI.INTENSET.bit.ERROR = 0x1; // Enable Error interrupt. // page 501
  SERCOM0->SPI.INTENSET.bit.DRE = 0x1; // Enable Data Register Empty interrupt. // page 501

  // Init SPI CLK // not used in SPI slave operation // page 481
  //SERCOM0->SPI.BAUD.reg = SERCOM_FREQ_REF / (2*4000000u)-1;

  // Enable SPI
  SERCOM0->SPI.CTRLA.bit.ENABLE = 1; // page 481
  while(SERCOM0->SPI.SYNCBUSY.bit.ENABLE); // Wait until bit is enabled
  SERCOM0->SPI.CTRLB.bit.RXEN = 0x1; // Enable Receiver // page 496. This is done here rather than in section "Set up SPI control B register" due to errate issue
  while(SERCOM0->SPI.SYNCBUSY.bit.CTRLB); // Wait until receiver is enabled

}

void SercomSPISlave::Sercom1init()
{
  // Configure SERCOM1     SPI Pins  PAD  Explanation
    // Set PA16 as input   (MOSI)    0    PA16 corresponds to: PORTA, PMUX[8], Even
    // Set PA17 as input   (SCK)     1    PA17 corresponds to: PORTA, PMUX[8], Odd
    // Set PA18 as input   (SS)      2    PA18 corresponds to: PORTA, PMUX[9], Even
    // Set PA19 as output  (MISO)    3    PA19 corresponds to: PORTA, PMUX[9], Odd
  PORT->Group[PORTA].PINCFG[16].bit.PMUXEN = 0x1; // Enable Peripheral Multiplexing for SERCOM1 SPI PA16, Arduino PIN11
  PORT->Group[PORTA].PMUX[8].bit.PMUXE = 0x2; // SERCOM 1 is selected for peripheral use of this pad (0x2 selects peripheral function C: SERCOM)
  PORT->Group[PORTA].PINCFG[17].bit.PMUXEN = 0x1; // Enable Peripheral Multiplexing for SERCOM1 SPI PA17, Arduino PIN13
  PORT->Group[PORTA].PMUX[8].bit.PMUXO = 0x2; // SERCOM 1 is selected for peripheral use of this pad (0x2 selects peripheral function C: SERCOM)
  PORT->Group[PORTA].PINCFG[18].bit.PMUXEN = 0x1; // Enable Peripheral Multiplexing for SERCOM1 SPI PA18, Arduino PIN10
  PORT->Group[PORTA].PMUX[9].bit.PMUXE = 0x2; // SERCOM 1 is selected for peripheral use of this pad (0x2 selects peripheral function C: SERCOM)
  PORT->Group[PORTA].PINCFG[19].bit.PMUXEN = 0x1; // Enable Peripheral Multiplexing for SERCOM1 SPI PA19, Arduino PIN12
  PORT->Group[PORTA].PMUX[9].bit.PMUXO = 0x2; // SERCOM 1 is selected for peripheral use of this pad (0x2 selects peripheral function C: SERCOM)

  // Disable SPI 1
  SERCOM1->SPI.CTRLA.bit.ENABLE = 0; // page 481
  while(SERCOM1->SPI.SYNCBUSY.bit.ENABLE); // Wait until bit is enabled
  
  // Reset SPI 1
  SERCOM1->SPI.CTRLA.bit.SWRST = 1; // page 481
  while(SERCOM1->SPI.CTRLA.bit.SWRST || SERCOM1->SPI.SYNCBUSY.bit.SWRST); // Wait until software reset is complete
  
  // Setting up Nested Vectored Interrupt Controller (NVIC)
  NVIC_EnableIRQ(SERCOM1_IRQn);
  NVIC_SetPriority(SERCOM1_IRQn,2);
  
  // Setting Generic Clock Controller
  GCLK->CLKCTRL.reg = GCLK_CLKCTRL_ID(GCM_SERCOM1_CORE) | // Generic Clock 0
            GCLK_CLKCTRL_GEN_GCLK0 | // Generic Clock Generator 0 is the source
            GCLK_CLKCTRL_CLKEN; // Enable Generic Clock Generator
  
  while(GCLK->STATUS.reg & GCLK_STATUS_SYNCBUSY); // Wait for synchronisation
  
  // Set up SPI control A register
  SERCOM1->SPI.CTRLA.bit.DORD = 0; // MSB is transferred first. // page 492
  SERCOM1->SPI.CTRLA.bit.CPOL = 0; // SCK is low when idle. The leading edge of a clock cycle is a rising edge, while the trailing edge is a falling edge. // page 492
  SERCOM1->SPI.CTRLA.bit.CPHA = 0; // Data is sampled on a leading SCK edge and changed on a trailing SCK edge. // page 492
  SERCOM1->SPI.CTRLA.bit.FORM = 0x0; // SPI frame // page 493
  SERCOM1->SPI.CTRLA.bit.DIPO = 0x0; // DATA PAD0 is used as slave input: MOSI // (slave mode) page 493
  SERCOM1->SPI.CTRLA.bit.DOPO = 0x2; // DATA PAD2 is used as slave output: MISO // (slave mode) page 493
  SERCOM1->SPI.CTRLA.bit.MODE = 0x2; // SPI slave operation. // page 494
  SERCOM1->SPI.CTRLA.bit.IBON = 0x1; // Immediate Buffer Overflow Notification. STATUS.BUFOVF is asserted immediately upon buffer overflow. // page 494
  SERCOM1->SPI.CTRLA.bit.RUNSTDBY = 1; // Wake on Receive Complete interrupt. // page 494
  
  // Set up SPI control B register
  //SERCOM1->SPI.CTRLB.bit.RXEN = 0x1; // Enable Receiver // page 496
  SERCOM1->SPI.CTRLB.bit.SSDE = 0x1; // Enable Slave Select Low Detect // page 497
  SERCOM1->SPI.CTRLB.bit.CHSIZE = 0; // Character Size 8 bits // page 497
  //SERCOM1->SPI.CTRLB.bit.PLOADEN = 0x1; // Enable Slave Data Preload // page 497
  //while(SERCOM1->SPI.SYNCBUSY.bit.CTRLB); // Wait until receiver is enabled
  
  // Set up SPI interrupts
  SERCOM1->SPI.INTENSET.bit.SSL = 0x1; // Enable Slave Select Low interrupt. // page 501
  SERCOM1->SPI.INTENSET.bit.RXC = 0x1; // Enable Receive Complete interrupt. // page 501
  SERCOM1->SPI.INTENSET.bit.TXC = 0x1; // Enable Transmit Complete interrupt. // page 501
  SERCOM1->SPI.INTENSET.bit.ERROR = 0x1; // Enable Error interrupt. // page 501
  SERCOM1->SPI.INTENSET.bit.DRE = 0x1; // Enable Data Register Empty interrupt. // page 501
  
  // Init SPI CLK // not used in SPI slave operation // page 481
  //SERCOM1->SPI.BAUD.reg = SERCOM_FREQ_REF / (2*4000000u)-1;
  
  // Enable SPI
  SERCOM1->SPI.CTRLA.bit.ENABLE = 1; // page 481
  while(SERCOM1->SPI.SYNCBUSY.bit.ENABLE); // Wait until bit is enabled
  SERCOM1->SPI.CTRLB.bit.RXEN = 0x1; // Enable Receiver // page 496. This is done here rather than in section "Set up SPI control B register" due to errate issue
  while(SERCOM1->SPI.SYNCBUSY.bit.CTRLB); // Wait until receiver is enabled

}

void SercomSPISlave::Sercom4init()
{
  // Configure SERCOM4     SPI Pins                PAD  Explanation
    // Set PA12 as input   (MOSI) - on D22 / MISO  0    PA12 corresponds to: PORTA, PMUX[6], Even
    // Set PB09 as input   (SCK) - on A2           1    PB09 corresponds to: PORTB, PMUX[4], Odd
    // Set PB10 as input   (SS) - on D23 / MOSI    2    PB10 corresponds to: PORTB, PMUX[5], Even
    // Set PB11 as output  (MISO) - on D24 / SCK   3    PB11 corresponds to: PORTB, PMUX[5], Odd
  PORT->Group[PORTA].PINCFG[12].bit.PMUXEN = 0x1; // Enable Peripheral Multiplexing for SERCOM4 SPI PA12, Arduino PIN22
  PORT->Group[PORTA].PMUX[6].bit.PMUXE = 0x3; // SERCOM 4 is selected for peripheral use of this pad (0x3 selects peripheral function D: SERCOM-ALT)
  PORT->Group[PORTB].PINCFG[9].bit.PMUXEN = 0x1; // Enable Peripheral Multiplexing for SERCOM4 SPI PB09, Arduino PIN16
  PORT->Group[PORTB].PMUX[4].bit.PMUXO = 0x3; // SERCOM 4 is selected for peripheral use of this pad (0x3 selects peripheral function D: SERCOM-ALT)
  PORT->Group[PORTB].PINCFG[10].bit.PMUXEN = 0x1; // Enable Peripheral Multiplexing for SERCOM4 SPI PB10, Arduino PIN23
  PORT->Group[PORTB].PMUX[5].bit.PMUXE = 0x3; // SERCOM 4 is selected for peripheral use of this pad (0x3 selects peripheral function D: SERCOM-ALT)
  PORT->Group[PORTB].PINCFG[11].bit.PMUXEN = 0x1; // Enable Peripheral Multiplexing for SERCOM4 SPI PB11, Arduino PIN24
  PORT->Group[PORTB].PMUX[5].bit.PMUXO = 0x3; // SERCOM 4 is selected for peripheral use of this pad (0x3 selects peripheral function D: SERCOM-ALT)

  // Disable SPI 1
  SERCOM4->SPI.CTRLA.bit.ENABLE = 0; // page 481
  while(SERCOM4->SPI.SYNCBUSY.bit.ENABLE); // Wait until bit is enabled
  
  // Reset SPI 1
  SERCOM4->SPI.CTRLA.bit.SWRST = 1; // page 481
  while(SERCOM4->SPI.CTRLA.bit.SWRST || SERCOM4->SPI.SYNCBUSY.bit.SWRST); // Wait until software reset is complete
  
  // Setting up Nested Vectored Interrupt Controller (NVIC)
  NVIC_EnableIRQ(SERCOM4_IRQn);
  NVIC_SetPriority(SERCOM4_IRQn,2);
  
  // Setting Generic Clock Controller
  GCLK->CLKCTRL.reg = GCLK_CLKCTRL_ID(GCM_SERCOM4_CORE) | // Generic Clock 0
            GCLK_CLKCTRL_GEN_GCLK0 | // Generic Clock Generator 0 is the source
            GCLK_CLKCTRL_CLKEN; // Enable Generic Clock Generator
  
  while(GCLK->STATUS.reg & GCLK_STATUS_SYNCBUSY); // Wait for synchronisation
  
  // Set up SPI control A register
  SERCOM4->SPI.CTRLA.bit.DORD = 0; // MSB is transferred first. // page 492
  SERCOM4->SPI.CTRLA.bit.CPOL = 0; // SCK is low when idle. The leading edge of a clock cycle is a rising edge, while the trailing edge is a falling edge. // page 492
  SERCOM4->SPI.CTRLA.bit.CPHA = 0; // Data is sampled on a leading SCK edge and changed on a trailing SCK edge. // page 492
  SERCOM4->SPI.CTRLA.bit.FORM = 0x0; // SPI frame // page 493
  SERCOM4->SPI.CTRLA.bit.DIPO = 0x0; // DATA PAD0 is used as slave input: MOSI // (slave mode) page 493
  SERCOM4->SPI.CTRLA.bit.DOPO = 0x2; // DATA PAD2 is used as slave output: MISO // (slave mode) page 493
  SERCOM4->SPI.CTRLA.bit.MODE = 0x2; // SPI slave operation. // page 494
  SERCOM4->SPI.CTRLA.bit.IBON = 0x1; // Immediate Buffer Overflow Notification. STATUS.BUFOVF is asserted immediately upon buffer overflow. // page 494
  SERCOM4->SPI.CTRLA.bit.RUNSTDBY = 1; // Wake on Receive Complete interrupt. // page 494
  
  // Set up SPI control B register
  //SERCOM4->SPI.CTRLB.bit.RXEN = 0x1; // Enable Receiver // page 496
  SERCOM4->SPI.CTRLB.bit.SSDE = 0x1; // Enable Slave Select Low Detect // page 497
  SERCOM4->SPI.CTRLB.bit.CHSIZE = 0; // Character Size 8 bits // page 497
  //SERCOM4->SPI.CTRLB.bit.PLOADEN = 0x1; // Enable Slave Data Preload // page 497
  //while(SERCOM4->SPI.SYNCBUSY.bit.CTRLB); // Wait until receiver is enabled
  
  // Set up SPI interrupts
  SERCOM4->SPI.INTENSET.bit.SSL = 0x1; // Enable Slave Select Low interrupt. // page 501
  SERCOM4->SPI.INTENSET.bit.RXC = 0x1; // Enable Receive Complete interrupt. // page 501
  SERCOM4->SPI.INTENSET.bit.TXC = 0x1; // Enable Transmit Complete interrupt. // page 501
  SERCOM4->SPI.INTENSET.bit.ERROR = 0x1; // Enable Error interrupt. // page 501
  SERCOM4->SPI.INTENSET.bit.DRE = 0x1; // Enable Data Register Empty interrupt. // page 501
  
  // Init SPI CLK // not used in SPI slave operation // page 481
  //SERCOM4->SPI.BAUD.reg = SERCOM_FREQ_REF / (2*4000000u)-1;
  
  // Enable SPI
  SERCOM4->SPI.CTRLA.bit.ENABLE = 1; // page 481
  while(SERCOM4->SPI.SYNCBUSY.bit.ENABLE); // Wait until bit is enabled
  SERCOM4->SPI.CTRLB.bit.RXEN = 0x1; // Enable Receiver // page 496. This is done here rather than in section "Set up SPI control B register" due to errate issue
  while(SERCOM4->SPI.SYNCBUSY.bit.CTRLB); // Wait until receiver is enabled

}
