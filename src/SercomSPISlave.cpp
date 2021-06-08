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

#include "SercomSPISlave.h"

// Constructors //
SercomSPISlave::SercomSPISlave()
{
    
}

// Public Methods //
void SercomSPISlave::Sercom0init()
{
    //Configure SERCOM0  SPI PINS  PAD
    //Set PA08 as input  (MOSI)    00
    //Set PA09 as input  (SCK)     01
    //Set PA10 as input  (SS)      02
    //Set PA11 as output (MISO)    03
    PORT->Group[PORTA].PINCFG[8].bit.PMUXEN = 0x1; //Enable Peripheral Multiplexing for SERCOM0 SPI PA08
    PORT->Group[PORTA].PMUX[4].bit.PMUXE = 0x2; //SERCOM 0 is selected for peripheral use of this pad (0x2 selects peripheral function C: SERCOM)
    PORT->Group[PORTA].PINCFG[9].bit.PMUXEN = 0x1; //Enable Peripheral Multiplexing for SERCOM0 SPI PA09
    PORT->Group[PORTA].PMUX[4].bit.PMUXO = 0x2; //SERCOM 0 is selected for peripheral use of this pad (0x2 selects peripheral function C: SERCOM)
    PORT->Group[PORTA].PINCFG[10].bit.PMUXEN = 0x1; //Enable Peripheral Multiplexing for SERCOM0 SPI PA10
    PORT->Group[PORTA].PMUX[5].bit.PMUXE = 0x2; //SERCOM 0 is selected for peripheral use of this pad (0x2 selects peripheral function C: SERCOM)
    PORT->Group[PORTA].PINCFG[11].bit.PMUXEN = 0x1; //Enable Peripheral Multiplexing for SERCOM0 SPI PA11
    PORT->Group[PORTA].PMUX[5].bit.PMUXO = 0x2; //SERCOM 0 is selected for peripheral use of this pad (0x2 selects peripheral function C: SERCOM)
    /*
    Explanation:
    PMUXEN stands for Peripheral Multiplexing Enable
    PMUXE stands for Even bits in the Peripheral Multiplexing register
    PMUXO stands for Odd bits in the Peripheral Multiplexing register
    The selection of peripheral function A to H is done by writing to the Peripheral Multiplexing Odd and Even bits in the Peripheral Multiplexing register (PMUXn.PMUXE/O) in the PORT.
    Reference: Atmel-42181G-SAM-D21_Datasheet section 6.1 on page 21

    PA08 corresponds to: PORTA, PMUX[4] Even
    PA09 corresponds to: PORTA, PMUX[4] Odd
    PA10 corresponds to: PORTA, PMUX[5] Even
    PA11 corresponds to: PORTA, PMUX[5] Odd
    In general:
    Px(2n+0/1) corresponds to Portx, PMUX[n] Even=0/Odd=1
    */
    
  //Disable SPI 1
  SERCOM0->SPI.CTRLA.bit.ENABLE =0;
  while(SERCOM0->SPI.SYNCBUSY.bit.ENABLE);
  
  //Reset SPI 1
  SERCOM0->SPI.CTRLA.bit.SWRST = 1;
  while(SERCOM0->SPI.CTRLA.bit.SWRST || SERCOM0->SPI.SYNCBUSY.bit.SWRST);
  
  //Setting up NVIC
  NVIC_EnableIRQ(SERCOM0_IRQn);
  NVIC_SetPriority(SERCOM0_IRQn,2);
  
  //Setting Generic Clock Controller!!!!
  GCLK->CLKCTRL.reg = GCLK_CLKCTRL_ID(GCM_SERCOM0_CORE) | //Generic Clock 0
            GCLK_CLKCTRL_GEN_GCLK0 | // Generic Clock Generator 0 is the source
            GCLK_CLKCTRL_CLKEN; // Enable Generic Clock Generator
  
  while(GCLK->STATUS.reg & GCLK_STATUS_SYNCBUSY); //Wait for synchronisation
  
  //Set up SPI Control A Register
  SERCOM0->SPI.CTRLA.bit.DORD = 0; //MSB first
  SERCOM0->SPI.CTRLA.bit.CPOL = 0; //SCK is low when idle, leading edge is rising edge
  SERCOM0->SPI.CTRLA.bit.CPHA = 0; //data sampled on leading sck edge and changed on a trailing sck edge
  SERCOM0->SPI.CTRLA.bit.FORM = 0x0; //Frame format = SPI
  SERCOM0->SPI.CTRLA.bit.DIPO = 0x0; //DATA PAD 00 MOSI is used as slave input (slave mode) // page 492
  SERCOM0->SPI.CTRLA.bit.DOPO = 0x2; //DATA PAD 03 MISO is used as slave output
  
  SERCOM0->SPI.CTRLA.bit.MODE = 0x2; //SPI in Slave mode
  SERCOM0->SPI.CTRLA.bit.IBON = 0x1; //Buffer Overflow notification
  SERCOM0->SPI.CTRLA.bit.RUNSTDBY = 1; //wake on receiver complete
  
  //Set up SPI control B register
  SERCOM0->SPI.CTRLB.bit.SSDE = 0x1; //Slave Selecte Detection Enabled
  SERCOM0->SPI.CTRLB.bit.CHSIZE = 0; //character size 8 Bit
  
  //Set up SPI interrupts
  SERCOM0->SPI.INTENSET.bit.SSL = 0x1; //Enable Slave Select low interrupt        
  SERCOM0->SPI.INTENSET.bit.RXC = 0x1; //Receive complete interrupt
  SERCOM0->SPI.INTENSET.bit.TXC = 0x1; //Receive complete interrupt
  SERCOM0->SPI.INTENSET.bit.ERROR = 0x1; //Receive complete interrupt
  SERCOM0->SPI.INTENSET.bit.DRE = 0x1; //Data Register Empty interrupt

  //Enable SPI
  SERCOM0->SPI.CTRLA.bit.ENABLE = 1;
  while(SERCOM0->SPI.SYNCBUSY.bit.ENABLE);
  SERCOM0->SPI.CTRLB.bit.RXEN = 0x1; //Enable Receiver, this is done here due to errate issue
  while(SERCOM0->SPI.SYNCBUSY.bit.CTRLB); //wait until receiver is enabled

}

void SercomSPISlave::Sercom1init()
{
    //Configure SERCOM1 SPI PINS
    //Set PA16 as input (MOSI)
    //Set PA17 as input (SCK)
    //Set PA18 as input (SS)
    //Set PA19 as output (MISO)
    PORT->Group[PORTA].PINCFG[16].bit.PMUXEN = 0x1; //Enable Peripheral Multiplexing for SERCOM1 SPI PA16 Arduino PIN11
    PORT->Group[PORTA].PMUX[8].bit.PMUXE = 0x2; //SERCOM 1 is selected for peripheral use of this pad (0x2 selects peripheral function C: SERCOM)
    PORT->Group[PORTA].PINCFG[17].bit.PMUXEN = 0x1; //Enable Peripheral Multiplexing for SERCOM1 SPI PA17 Arduino PIN13
    PORT->Group[PORTA].PMUX[8].bit.PMUXO = 0x2; //SERCOM 1 is selected for peripheral use of this pad (0x2 selects peripheral function C: SERCOM)
    PORT->Group[PORTA].PINCFG[18].bit.PMUXEN = 0x1; //Enable Peripheral Multiplexing for SERCOM1 SPI PA18 Arduino PIN10
    PORT->Group[PORTA].PMUX[9].bit.PMUXE = 0x2; //SERCOM 1 is selected for peripheral use of this pad (0x2 selects peripheral function C: SERCOM)
    PORT->Group[PORTA].PINCFG[19].bit.PMUXEN = 0x1; //Enable Peripheral Multiplexing for SERCOM1 SPI PA19 Arduino PIN12
    PORT->Group[PORTA].PMUX[9].bit.PMUXO = 0x2; //SERCOM 1 is selected for peripheral use of this pad (0x2 selects peripheral function C: SERCOM)
/*
Explanation:
PMUXEN stands for Peripheral Multiplexing Enable
PMUXE stands for Even bits in the Peripheral Multiplexing register
PMUXO stands for Odd bits in the Peripheral Multiplexing register
The selection of peripheral function A to H is done by writing to the Peripheral Multiplexing Odd and Even bits in the Peripheral Multiplexing register (PMUXn.PMUXE/O) in the PORT.
Reference: Atmel-42181G-SAM-D21_Datasheet section 6.1 on page 21

PA16 corresponds to: PORTA, PMUX[8] Even
PA17 corresponds to: PORTA, PMUX[8] Odd
PA18 corresponds to: PORTA, PMUX[9] Even
PA19 corresponds to: PORTA, PMUX[9] Odd
In general:
Px(2n+0/1) corresponds to Portx, PMUX[n] Even=0/Odd=1
*/
    
  //Disable SPI 1
  SERCOM1->SPI.CTRLA.bit.ENABLE =0;
  while(SERCOM1->SPI.SYNCBUSY.bit.ENABLE);
  
  //Reset SPI 1
  SERCOM1->SPI.CTRLA.bit.SWRST = 1;
  while(SERCOM1->SPI.CTRLA.bit.SWRST || SERCOM1->SPI.SYNCBUSY.bit.SWRST);
  
  //Setting up NVIC
  NVIC_EnableIRQ(SERCOM1_IRQn);
  NVIC_SetPriority(SERCOM1_IRQn,2);
  
  //Setting Generic Clock Controller!!!!
  GCLK->CLKCTRL.reg = GCLK_CLKCTRL_ID(GCM_SERCOM1_CORE) | //Generic Clock 0
            GCLK_CLKCTRL_GEN_GCLK0 | // Generic Clock Generator 0 is the source
            GCLK_CLKCTRL_CLKEN; // Enable Generic Clock Generator
  
  while(GCLK->STATUS.reg & GCLK_STATUS_SYNCBUSY); //Wait for synchronisation
  
  //Set up SPI Control A Register
  SERCOM1->SPI.CTRLA.bit.DORD = 0; //MSB first
  SERCOM1->SPI.CTRLA.bit.CPOL = 0; //SCK is low when idle, leading edge is rising edge
  SERCOM1->SPI.CTRLA.bit.CPHA = 0; //data sampled on leading sck edge and changed on a trailing sck edge
  SERCOM1->SPI.CTRLA.bit.FORM = 0x0; //Frame format = SPI
  SERCOM1->SPI.CTRLA.bit.DIPO = 0x0; //DATA PAD0 MOSI is used as slave input (slave mode) // page 492
  SERCOM1->SPI.CTRLA.bit.DOPO = 0x2; //DATA PAD2 MISO is used as slave output
  SERCOM1->SPI.CTRLA.bit.MODE = 0x2; //SPI in Slave mode
  SERCOM1->SPI.CTRLA.bit.IBON = 0x1; //Buffer Overflow notification
  SERCOM1->SPI.CTRLA.bit.RUNSTDBY = 1; //wake on receiver complete
  
  //Set up SPI control B register
  //SERCOM1->SPI.CTRLB.bit.RXEN = 0x1; //Enable Receiver
  SERCOM1->SPI.CTRLB.bit.SSDE = 0x1; //Slave Selecte Detection Enabled
  SERCOM1->SPI.CTRLB.bit.CHSIZE = 0; //character size 8 Bit
  //SERCOM1->SPI.CTRLB.bit.PLOADEN = 0x1; //Enable Preload Data Register
  //while (SERCOM1->SPI.SYNCBUSY.bit.CTRLB);  
  
  //Set up SPI interrupts
  SERCOM1->SPI.INTENSET.bit.SSL = 0x1; //Enable Slave Select low interrupt        
  SERCOM1->SPI.INTENSET.bit.RXC = 0x1; //Receive complete interrupt
  SERCOM1->SPI.INTENSET.bit.TXC = 0x1; //Receive complete interrupt
  SERCOM1->SPI.INTENSET.bit.ERROR = 0x1; //Receive complete interrupt
  SERCOM1->SPI.INTENSET.bit.DRE = 0x1; //Data Register Empty interrupt
  //init SPI CLK  
  //SERCOM1->SPI.BAUD.reg = SERCOM_FREQ_REF / (2*4000000u)-1;
  //Enable SPI
  SERCOM1->SPI.CTRLA.bit.ENABLE = 1;
  while(SERCOM1->SPI.SYNCBUSY.bit.ENABLE);
  SERCOM1->SPI.CTRLB.bit.RXEN = 0x1; //Enable Receiver, this is done here due to errate issue
  while(SERCOM1->SPI.SYNCBUSY.bit.CTRLB); //wait until receiver is enabled

}

void SercomSPISlave::Sercom4init()
{
  //Configure SERCOM4 SPI PINS  
    //Set PA12 as input (MOSI) - on D22 / MISO
    //Set PB09 as input (SCK) - on A2
    //Set PB10 as input (SS) - on D23 / MOSI
    //Set PB11 as output (MISO) - on D24 / SCK
  PORT->Group[PORTA].PINCFG[12].bit.PMUXEN = 0x1; //Enable Peripheral Multiplexing for SERCOM4 SPI PA12 Arduino PIN22
  PORT->Group[PORTA].PMUX[6].bit.PMUXE = 0x3; //SERCOM 4 is selected for peripheral use of this pad (0x3 selects peripheral function D: SERCOM-ALT)
  PORT->Group[PORTB].PINCFG[9].bit.PMUXEN = 0x1; //Enable Peripheral Multiplexing for SERCOM4 SPI PB09 Arduino PIN16
  PORT->Group[PORTB].PMUX[4].bit.PMUXO = 0x3; //SERCOM 4 is selected for peripheral use of this pad (0x3 selects peripheral function D: SERCOM-ALT)
  PORT->Group[PORTB].PINCFG[10].bit.PMUXEN = 0x1; //Enable Peripheral Multiplexing for SERCOM4 SPI PB10 Arduino PIN23
  PORT->Group[PORTB].PMUX[5].bit.PMUXE = 0x3; //SERCOM 4 is selected for peripheral use of this pad (0x3 selects peripheral function D: SERCOM-ALT)
  PORT->Group[PORTB].PINCFG[11].bit.PMUXEN = 0x1; //Enable Peripheral Multiplexing for SERCOM4 SPI PB11 Arduino PIN24
  PORT->Group[PORTB].PMUX[5].bit.PMUXO = 0x3; //SERCOM 4 is selected for peripheral use of this pad (0x3 selects peripheral function D: SERCOM-ALT)
/*
Explanation:
PMUXEN stands for Peripheral Multiplexing Enable
PMUXE stands for Even bits in the Peripheral Multiplexing register
PMUXO stands for Odd bits in the Peripheral Multiplexing register
The selection of peripheral function A to H is done by writing to the Peripheral Multiplexing Odd and Even bits in the Peripheral Multiplexing register (PMUXn.PMUXE/O) in the PORT.
Reference: Atmel-42181G-SAM-D21_Datasheet section 6.1 on page 21

PA12 corresponds to: PORTA, PMUX[6] Even
PB09 corresponds to: PORTB, PMUX[4] Odd
PB10 corresponds to: PORTB, PMUX[5] Even
PB11 corresponds to: PORTB, PMUX[5] Odd
In general:
Px(2n+0/1) corresponds to Portx, PMUX[n] Even=0/Odd=1
*/

  //Disable SPI 1
  SERCOM4->SPI.CTRLA.bit.ENABLE =0;
  while(SERCOM4->SPI.SYNCBUSY.bit.ENABLE);
  
  //Reset SPI 1
  SERCOM4->SPI.CTRLA.bit.SWRST = 1;
  while(SERCOM4->SPI.CTRLA.bit.SWRST || SERCOM4->SPI.SYNCBUSY.bit.SWRST);
  
  //Setting up NVIC
  NVIC_EnableIRQ(SERCOM4_IRQn);
  NVIC_SetPriority(SERCOM4_IRQn,2);
  
  //Setting Generic Clock Controller!!!!
  GCLK->CLKCTRL.reg = GCLK_CLKCTRL_ID(GCM_SERCOM4_CORE) | //Generic Clock 0
            GCLK_CLKCTRL_GEN_GCLK0 | // Generic Clock Generator 0 is the source
            GCLK_CLKCTRL_CLKEN; // Enable Generic Clock Generator
  
  while(GCLK->STATUS.reg & GCLK_STATUS_SYNCBUSY); //Wait for synchronisation
  
  //Set up SPI Control A Register
  SERCOM4->SPI.CTRLA.bit.DORD = 0; //MSB first
  SERCOM4->SPI.CTRLA.bit.CPOL = 0; //SCK is low when idle, leading edge is rising edge
  SERCOM4->SPI.CTRLA.bit.CPHA = 0; //data sampled on leading sck edge and changed on a trailing sck edge
  SERCOM4->SPI.CTRLA.bit.FORM = 0x0; //Frame format = SPI
  SERCOM4->SPI.CTRLA.bit.DIPO = 0x0; //DATA PAD0 MOSI is used as slave input (slave mode) // page 492
  SERCOM4->SPI.CTRLA.bit.DOPO = 0x2; //DATA PAD2 MISO is used as slave output
  SERCOM4->SPI.CTRLA.bit.MODE = 0x2; //SPI in Slave mode
  SERCOM4->SPI.CTRLA.bit.IBON = 0x1; //Buffer Overflow notification
  SERCOM4->SPI.CTRLA.bit.RUNSTDBY = 1; //wake on receiver complete
  
  //Set up SPI control B register
  //SERCOM4->SPI.CTRLB.bit.RXEN = 0x1; //Enable Receiver
  SERCOM4->SPI.CTRLB.bit.SSDE = 0x1; //Slave Selecte Detection Enabled
  SERCOM4->SPI.CTRLB.bit.CHSIZE = 0; //character size 8 Bit
  //SERCOM4->SPI.CTRLB.bit.PLOADEN = 0x1; //Enable Preload Data Register
  //while (SERCOM4->SPI.SYNCBUSY.bit.CTRLB);  
  
  //Set up SPI interrupts
  SERCOM4->SPI.INTENSET.bit.SSL = 0x1; //Enable Slave Select low interrupt        
  SERCOM4->SPI.INTENSET.bit.RXC = 0x1; //Receive complete interrupt
  SERCOM4->SPI.INTENSET.bit.TXC = 0x1; //Receive complete interrupt
  SERCOM4->SPI.INTENSET.bit.ERROR = 0x1; //Receive complete interrupt
  SERCOM4->SPI.INTENSET.bit.DRE = 0x1; //Data Register Empty interrupt
  //init SPI CLK  
  //SERCOM4->SPI.BAUD.reg = SERCOM_FREQ_REF / (2*4000000u)-1;
  //Enable SPI
  SERCOM4->SPI.CTRLA.bit.ENABLE = 1;
  while(SERCOM4->SPI.SYNCBUSY.bit.ENABLE);
  SERCOM4->SPI.CTRLB.bit.RXEN = 0x1; //Enable Receiver, this is done here due to errate issue
  while(SERCOM4->SPI.SYNCBUSY.bit.CTRLB); //wait until receiver is enabled

}

