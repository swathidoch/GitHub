// SpaceInvaders.c
// Runs on LM4F120/TM4C123
// Jonathan Valvano and Daniel Valvano
// This is a starter project for the EE319K Lab 10

// Last Modified: 11/21/2017 
// http://www.spaceinvaders.de/
// sounds at http://www.classicgaming.cc/classics/spaceinvaders/sounds.php
// http://www.classicgaming.cc/classics/spaceinvaders/playguide.php
/* This example accompanies the books
   "Embedded Systems: Real Time Interfacing to Arm Cortex M Microcontrollers",
   ISBN: 978-1463590154, Jonathan Valvano, copyright (c) 2017

   "Embedded Systems: Introduction to Arm Cortex M Microcontrollers",
   ISBN: 978-1469998749, Jonathan Valvano, copyright (c) 2017

 Copyright 2017 by Jonathan W. Valvano, valvano@mail.utexas.edu
    You may use, edit, run or distribute this file
    as long as the above copyright notice remains
 THIS SOFTWARE IS PROVIDED "AS IS".  NO WARRANTIES, WHETHER EXPRESS, IMPLIED
 OR STATUTORY, INCLUDING, BUT NOT LIMITED TO, IMPLIED WARRANTIES OF
 MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE APPLY TO THIS SOFTWARE.
 VALVANO SHALL NOT, IN ANY CIRCUMSTANCES, BE LIABLE FOR SPECIAL, INCIDENTAL,
 OR CONSEQUENTIAL DAMAGES, FOR ANY REASON WHATSOEVER.
 For more information about my classes, my research, and my books, see
 http://users.ece.utexas.edu/~valvano/
 */
// ******* Possible Hardware I/O connections*******************
// Slide pot pin 1 connected to ground
// Slide pot pin 2 connected to PE2/AIN1
// Slide pot pin 3 connected to +3.3V 
// fire button connected to PE0
// special weapon fire button connected to PE1
// 8*R resistor DAC bit 0 on PB0 (least significant bit)
// 4*R resistor DAC bit 1 on PB1
// 2*R resistor DAC bit 2 on PB2
// 1*R resistor DAC bit 3 on PB3 (most significant bit)
// LED on PB4
// LED on PB5

// Backlight (pin 10) connected to +3.3 V
// MISO (pin 9) unconnected
// SCK (pin 8) connected to PA2 (SSI0Clk)
// MOSI (pin 7) connected to PA5 (SSI0Tx)
// TFT_CS (pin 6) connected to PA3 (SSI0Fss)
// CARD_CS (pin 5) unconnected
// Data/Command (pin 4) connected to PA6 (GPIO), high for data, low for command
// RESET (pin 3) connected to PA7 (GPIO)
// VCC (pin 2) connected to +3.3 V
// Gnd (pin 1) connected to ground

#include <stdint.h>
#include "tm4c123gh6pm.h"
#include "ST7735.h"
#include "Random.h"
#include "PLL.h"
#include "ADC.h"
#include "Print.h"
#include "Timer0.h"

#define PF2       (*((volatile uint32_t *)0x40025010))

void DisableInterrupts(void); // Disable interrupts
void EnableInterrupts(void);  // Enable interrupts
void Delay100ms(uint32_t count); // time delay in 0.1 seconds


// *************************** Images ***************************
// enemy ship that starts at the top of the screen (arms/mouth closed)
// width=16 x height=10
const unsigned short SmallEnemy10pointA[] = {
 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,
 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0xFFFF, 0xFFFF, 0x0000, 0x0000, 0xFFFF, 0xFFFF, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,
 0x0000, 0x0000, 0xFFFF, 0x0000, 0xFFFF, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0xFFFF, 0x0000, 0xFFFF, 0x0000, 0x0000,
 0x0000, 0x0000, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0x0000, 0x0000,
 0x0000, 0x0000, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0x0000, 0x0000,
 0x0000, 0x0000, 0x0000, 0xFFFF, 0xFFFF, 0x0000, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0x0000, 0xFFFF, 0xFFFF, 0x0000, 0x0000, 0x0000,
 0x0000, 0x0000, 0x0000, 0x0000, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0x0000, 0x0000, 0x0000, 0x0000,
 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0xFFFF, 0x0000, 0x0000, 0x0000, 0x0000, 0xFFFF, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,
 0x0000, 0x0000, 0x0000, 0x0000, 0xFFFF, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0xFFFF, 0x0000, 0x0000, 0x0000, 0x0000,
 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,
};

// enemy ship that starts at the top of the screen (arms/mouth open)
// width=16 x height=10
const unsigned short SmallEnemy10pointB[] = {
 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,
 0x0000, 0x0000, 0x0000, 0xFFFF, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0xFFFF, 0x0000, 0x0000, 0x0000,
 0x0000, 0x0000, 0x0000, 0x0000, 0xFFFF, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0xFFFF, 0x0000, 0x0000, 0x0000, 0x0000,
 0x0000, 0x0000, 0x0000, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0x0000, 0x0000, 0x0000,
 0x0000, 0x0000, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0x0000, 0x0000,
 0x0000, 0x0000, 0xFFFF, 0xFFFF, 0xFFFF, 0x0000, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0x0000, 0xFFFF, 0xFFFF, 0xFFFF, 0x0000, 0x0000,
 0x0000, 0x0000, 0xFFFF, 0x0000, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0x0000, 0xFFFF, 0x0000, 0x0000,
 0x0000, 0x0000, 0xFFFF, 0x0000, 0x0000, 0xFFFF, 0x0000, 0x0000, 0x0000, 0x0000, 0xFFFF, 0x0000, 0x0000, 0xFFFF, 0x0000, 0x0000,
 0x0000, 0x0000, 0x0000, 0x0000, 0xFFFF, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0xFFFF, 0x0000, 0x0000, 0x0000, 0x0000,
 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,
};

// enemy ship that starts in the middle of the screen (arms together)
// width=16 x height=10
const unsigned short SmallEnemy20pointA[] = {
 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,
 0x0000, 0x0000, 0x0000, 0x0000, 0xFFFF, 0xFFFF, 0x0000, 0x0000, 0x0000, 0x0000, 0xFFFF, 0xFFFF, 0x0000, 0x0000, 0x0000, 0x0000,
 0x0000, 0x0000, 0x0000, 0xFFFF, 0xFFFF, 0x0000, 0x0000, 0xFFFF, 0xFFFF, 0x0000, 0x0000, 0xFFFF, 0xFFFF, 0x0000, 0x0000, 0x0000,
 0x0000, 0x0000, 0x0000, 0x0000, 0xFFFF, 0xFFFF, 0xFFFF, 0x0000, 0x0000, 0xFFFF, 0xFFFF, 0xFFFF, 0x0000, 0x0000, 0x0000, 0x0000,
 0x0000, 0x0000, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0x0000, 0x0000,
 0x0000, 0x0000, 0xFFFF, 0xFFFF, 0xFFFF, 0x0000, 0x0000, 0xFFFF, 0xFFFF, 0x0000, 0x0000, 0xFFFF, 0xFFFF, 0xFFFF, 0x0000, 0x0000,
 0x0000, 0x0000, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0x0000, 0x0000,
 0x0000, 0x0000, 0x0000, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0x0000, 0x0000, 0x0000,
 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,
 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,
};

// enemy ship that starts in the middle of the screen (arms apart)
// width=16 x height=10
const unsigned short SmallEnemy20pointB[] = {
 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,
 0x0000, 0xFFFF, 0xFFFF, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0xFFFF, 0xFFFF, 0x0000,
 0x0000, 0x0000, 0x0000, 0xFFFF, 0xFFFF, 0x0000, 0x0000, 0xFFFF, 0xFFFF, 0x0000, 0x0000, 0xFFFF, 0xFFFF, 0x0000, 0x0000, 0x0000,
 0x0000, 0x0000, 0x0000, 0x0000, 0xFFFF, 0xFFFF, 0xFFFF, 0x0000, 0x0000, 0xFFFF, 0xFFFF, 0xFFFF, 0x0000, 0x0000, 0x0000, 0x0000,
 0x0000, 0x0000, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0x0000, 0x0000,
 0x0000, 0x0000, 0xFFFF, 0xFFFF, 0xFFFF, 0x0000, 0x0000, 0xFFFF, 0xFFFF, 0x0000, 0x0000, 0xFFFF, 0xFFFF, 0xFFFF, 0x0000, 0x0000,
 0x0000, 0x0000, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0x0000, 0x0000,
 0x0000, 0x0000, 0x0000, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0x0000, 0x0000, 0x0000,
 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,
 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,
};



// enemy ship that starts at the bottom of the screen (arms down)
// width=16 x height=10
const unsigned short SmallEnemy30pointA[] = {
 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,
 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0xFFFF, 0x0000, 0x0000, 0x0000, 0x0000, 0xFFFF, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,
 0x0000, 0x0000, 0x0000, 0x0000, 0xFFFF, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0xFFFF, 0x0000, 0x0000, 0x0000, 0x0000,
 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0xFFFF, 0x0000, 0xFFFF, 0xFFFF, 0x0000, 0xFFFF, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,
 0x0000, 0x0000, 0x0000, 0x0000, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0x0000, 0x0000, 0x0000, 0x0000,
 0x0000, 0x0000, 0x0000, 0x0000, 0xFFFF, 0xFFFF, 0x0000, 0xFFFF, 0xFFFF, 0x0000, 0xFFFF, 0xFFFF, 0x0000, 0x0000, 0x0000, 0x0000,
 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,
 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,
 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0xFFFF, 0xFFFF, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,
 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,
};

// enemy ship that starts at the bottom of the screen (arms up)
// width=16 x height=10
const unsigned short SmallEnemy30pointB[] = {
 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,
 0x0000, 0x0000, 0x0000, 0xFFFF, 0x0000, 0xFFFF, 0x0000, 0x0000, 0x0000, 0x0000, 0xFFFF, 0x0000, 0xFFFF, 0x0000, 0x0000, 0x0000,
 0x0000, 0x0000, 0x0000, 0x0000, 0xFFFF, 0x0000, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0x0000, 0xFFFF, 0x0000, 0x0000, 0x0000, 0x0000,
 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0xFFFF, 0x0000, 0x0000, 0x0000, 0x0000, 0xFFFF, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,
 0x0000, 0x0000, 0x0000, 0x0000, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0x0000, 0x0000, 0x0000, 0x0000,
 0x0000, 0x0000, 0x0000, 0x0000, 0xFFFF, 0xFFFF, 0x0000, 0xFFFF, 0xFFFF, 0x0000, 0xFFFF, 0xFFFF, 0x0000, 0x0000, 0x0000, 0x0000,
 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,
 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,
 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0xFFFF, 0xFFFF, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,
 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,
};

// image of the player's ship
// includes two blacked out columns on the left and right sides of the image to prevent smearing when moved 2 pixels to the left or right
// width=18 x height=8
const unsigned short PlayerShip0[] = {
 0x0000, 0x0000, 0x07E0, 0x07E0, 0x07E0, 0x07E0, 0x07E0, 0x07E0, 0x07E0, 0x07E0, 0x07E0, 0x07E0, 0x07E0, 0x07E0, 0x07E0, 0x07E0,
 0x0000, 0x0000, 0x0000, 0x0000, 0x07E0, 0x07E0, 0x07E0, 0x07E0, 0x07E0, 0x07E0, 0x07E0, 0x07E0, 0x07E0, 0x07E0, 0x07E0, 0x07E0,
 0x07E0, 0x07E0, 0x0000, 0x0000, 0x0000, 0x0000, 0x07E0, 0x07E0, 0x07E0, 0x07E0, 0x07E0, 0x07E0, 0x07E0, 0x07E0, 0x07E0, 0x07E0,
 0x07E0, 0x07E0, 0x07E0, 0x07E0, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x07E0, 0x07E0, 0x07E0, 0x07E0, 0x07E0, 0x07E0, 0x07E0,
 0x07E0, 0x07E0, 0x07E0, 0x07E0, 0x07E0, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x07E0,
 0x07E0, 0x07E0, 0x07E0, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,
 0x0000, 0x07E0, 0x07E0, 0x07E0, 0x07E0, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,
 0x0000, 0x0000, 0x0000, 0x0000, 0x07E0, 0x07E0, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,
 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,


};

// small shield floating in space to cover the player's ship from enemy fire (undamaged)
// width=18 x height=5
const unsigned short Bunker0[] = {
 0x079F, 0x079F, 0x079F, 0x079F, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x079F, 0x079F,
 0x079F, 0x079F, 0x079F, 0x079F, 0x079F, 0x079F, 0x079F, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x079F,
 0x079F, 0x079F, 0x079F, 0x079F, 0x079F, 0x079F, 0x079F, 0x079F, 0x079F, 0x079F, 0x079F, 0x079F, 0x079F, 0x079F, 0x079F, 0x079F,
 0x079F, 0x079F, 0x079F, 0x079F, 0x079F, 0x079F, 0x0000, 0x079F, 0x079F, 0x079F, 0x079F, 0x079F, 0x079F, 0x079F, 0x079F, 0x079F,
 0x079F, 0x079F, 0x079F, 0x079F, 0x079F, 0x079F, 0x079F, 0x0000, 0x0000, 0x0000, 0x079F, 0x079F, 0x079F, 0x079F, 0x079F, 0x079F,
 0x079F, 0x079F, 0x079F, 0x079F, 0x079F, 0x079F, 0x079F, 0x079F, 0x0000, 0x0000,

};

const unsigned short Laser[] = {0x001F, 0x001F, 0x001F, 0x001F, 0x001F, 0x001F, 0x001F, 0x001F};
const unsigned short Bullet[] = {0x04D5, 0x04D5, 0x04D5, 0x04D5, 0x04D5, 0x04D5, 0x04D5, 0x04D5};
const unsigned short EnemyLaser[] = {0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF};

struct sprite{
	int32_t x;
	int32_t y;
	const unsigned short *bmp;
	uint8_t alive;
	int8_t v; 
};
typedef struct sprite sprite_t;

uint32_t Data;        // 12-bit ADC
uint32_t Position;    // 32-bit fixed-point 0.001 cm
static uint32_t ADCMail;
static uint32_t ADCStatus;
uint32_t enemyY = 25;
uint32_t bunkerY = 150;
static sprite_t Projectiles[10];
static sprite_t EnemyProjectiles[10];
static uint8_t numProjectiles = 0;
static uint8_t numEnProjectiles = 0;
static uint8_t numLives = 3;
static uint8_t running = 1;
static uint8_t enemiesLeft = 6;

void SysTick_Init(void){
	DisableInterrupts();
	NVIC_ST_CTRL_R = 0x00;
	NVIC_ST_RELOAD_R = 0xC3500;
	NVIC_ST_CURRENT_R = 0x00;
	NVIC_SYS_PRI3_R = (NVIC_SYS_PRI3_R & 0x00FFFFFF) | 0x20000000;
	NVIC_ST_CTRL_R = 0x0007;
	EnableInterrupts();
}

void PortF_Init(void){
	SYSCTL_RCGCGPIO_R |= 0x20;
	while((SYSCTL_RCGCGPIO_R&0x20)==0){};
	GPIO_PORTF_LOCK_R = GPIO_LOCK_KEY;
	GPIO_PORTF_CR_R = 0xFF;
	GPIO_PORTF_DEN_R = 0x11;
	GPIO_PORTF_DIR_R = 0x00;
	GPIO_PORTF_PUR_R = 0x11;
	//falling edge interrupts on PF4
	GPIO_PORTF_IS_R &= ~0x10;
	GPIO_PORTF_IBE_R &= ~0x10;
	GPIO_PORTF_IEV_R &= ~0x10;
	GPIO_PORTF_ICR_R = 0x10;
	GPIO_PORTF_IM_R |= 0x10;
	//falling edge interrupts on PF0
	GPIO_PORTF_IS_R &= ~0x01;
	GPIO_PORTF_IBE_R &= ~0x01;
	GPIO_PORTF_IEV_R &= ~0x01;
	GPIO_PORTF_ICR_R = 0x01;
	GPIO_PORTF_IM_R |= 0x01;
	//set up interrupts
	NVIC_PRI7_R = (NVIC_PRI7_R&0xFF00FFFF)|0x00A00000;
	NVIC_EN0_R = 0x40000000;
}

void GPIOPortF_Handler(void){
	if(GPIO_PORTF_RIS_R&0x10){
		GPIO_PORTF_ICR_R = 0x10;
		if(numProjectiles < 10){
			sprite_t proj = {Position+9, 150, Bullet, 1, -2};
			uint8_t index = 0;
			for(int i = 0; i<10; i++){
				if(Projectiles[i].alive == 0)
					index = i;
			}
			Projectiles[index] = proj;
			numProjectiles++;
		}
	}else{
		GPIO_PORTF_ICR_R = 0x01;
		if(numProjectiles < 10){
			sprite_t proj = {Position+8, 150, Laser, 1, -2};
			uint8_t index = 0;
			for(int i = 0; i<10; i++){
				if(Projectiles[i].alive == 0)
					index = i;
			}
			Projectiles[index] = proj;
			numProjectiles++;
		}
	}
}

void SysTick_Handler(void){
	ADCMail = ADC_In();
	ADCStatus = 0x01;
}

void MyDelay1ms(uint32_t n){uint32_t volatile time;
  while(n){
    time = 72724*2/91;  // 1msec, tuned at 80 MHz
    while(time){
	  	time--;
    }
    n--;
  }
}

uint32_t Convert(uint32_t data){
	return data/37;
}

void endGame(){
	running = 0;
}

void draw(sprite_t arr[], uint8_t w, uint8_t h, uint8_t l){
	for(int i = 0; i<l; i++){
		if(arr[i].alive)
			ST7735_DrawBitmap(arr[i].x, arr[i].y, arr[i].bmp, w,h);
	}
}

void tick(sprite_t arr[], char dir, uint8_t l, char type){
	for(int i = 0; i<l; i++){
		if(dir=='x')
			arr[i].x += arr[i].v;
		if(arr[i].alive){
			if(dir == 'x'){
				arr[i].x += arr[i].v;
			}
			else{
				arr[i].y += arr[i].v;
				if(arr[i].y < 1){
					arr[i].alive = 0;
					switch(type){
						case 'p':
							numProjectiles--;
							break;
						case 'e':
							numEnProjectiles--;
							break;
					}
				}
				if(arr[i].y > 159){
					arr[i].alive = 0;
					switch(type){
						case 'p':
							numProjectiles--;
							break;
						case 'e':
							numEnProjectiles--;
							break;
					}
				}
			}
		}
	}
	if (dir == 'x'){
		if(arr[5].x > 112){
			for(int i = 0; i<6; i++){
				arr[i].v *= -1;
				arr[i].y += 1;
			}
		}
		if(arr[0].x < 1){
			for(int i = 0; i<6; i++){
				arr[i].v *= -1;
				arr[i].y += 1;
			}
		}
	}
}

void handleCollisions(sprite_t enemies[], sprite_t bunks[]){
	for(int i = 0; i<10; i++){
		if(Projectiles[i].alive != 0){
			for(int j = 0; j<6; j++){
				if(enemies[j].alive){
					if((enemies[j].y == (Projectiles[i].y + 4))&&(enemies[j].x<=Projectiles[i].x)&&((enemies[j].x)+16)>=Projectiles[i].x){
						enemies[j].alive = 0;
						enemiesLeft--;
						if(enemiesLeft==0)
							endGame();
						Projectiles[i].alive = 0;
						numProjectiles--;
					}
				}
			}
			for(int k = 0; k<3; k++){
				if((bunks[k].y == (Projectiles[i].y + 4))&&(bunks[k].x<=Projectiles[i].x)&&((bunks[k].x)+16)>=Projectiles[i].x){
					Projectiles[i].alive = 0;	
					numProjectiles--;
				}
			}
		}
	}
	for(int i = 0; i<10; i++){
		if(EnemyProjectiles[i].alive != 0){
			if(EnemyProjectiles[i].y == 151 && EnemyProjectiles[i].x>=Position && EnemyProjectiles[i].x<=Position+18){
				numLives--;
				numEnProjectiles--;
				EnemyProjectiles[i].alive = 0;
				if(numLives==0){
					endGame();
				}
			}
			for(int k = 0; k<3; k++){
				if((bunks[k].y == (EnemyProjectiles[i].y + 4))&&(bunks[k].x<=EnemyProjectiles[i].x)&&((bunks[k].x)+16)>=EnemyProjectiles[i].x){
					EnemyProjectiles[i].alive = 0;	
					numEnProjectiles--;
				}
			}
		}
	}
}

// *************************** Capture image dimensions out of BMP**********
	int main(void){
  PLL_Init(Bus80MHz);       // Bus clock is 80 MHz 
	Position = 52;
	PortF_Init();
  Random_Init(1);
	Timer0_Init(0x2625A00);
	SysTick_Init();
	ADC_Init(); 
	//DAC_Init();
  ST7735_InitR(INITR_REDTAB);
  ST7735_FillScreen(0x0000);            // set screen to black
	//create enemies
	sprite_t Enemies1[6] = {{0, enemyY, SmallEnemy10pointA, 1, 1}, {20, enemyY, SmallEnemy10pointB, 1, 1}, {40, enemyY, SmallEnemy20pointA, 1, 1}, {60, enemyY, SmallEnemy20pointB, 1, 1},
	{80, enemyY, SmallEnemy30pointA, 1, 1}, {100, enemyY, SmallEnemy30pointB, 1, 1}};
	//create empty array of projectiles
	sprite_t defaultProj = {0, 0, Bullet, 0, 0};
	for(int i = 0; i<10; i++){
		Projectiles[i] = defaultProj;
	}
	//create empty array of enemy lasers
	sprite_t defaultEnProj = {0, 0, EnemyLaser, 0, 0};
	for(int i = 0; i<10; i++){
		EnemyProjectiles[i] = defaultEnProj;
	}
	//create bunkers
	sprite_t Bunkers[3] = {{13, bunkerY, Bunker0, 1, 0}, {53, bunkerY, Bunker0, 1, 0}, {93, bunkerY, Bunker0, 1, 0}};

	while(running){
		if(ADCStatus == 1){
			Position = Convert(ADCMail);
			ADCStatus = 0;
		}
		if(getAnimate()){
			if(getFlag()){
				Enemies1[0].bmp = SmallEnemy10pointA;
				Enemies1[1].bmp = SmallEnemy10pointB;
				Enemies1[2].bmp = SmallEnemy20pointA;
				Enemies1[3].bmp = SmallEnemy20pointB;
				Enemies1[4].bmp = SmallEnemy30pointA;
				Enemies1[5].bmp = SmallEnemy30pointB;
				if(numEnProjectiles < 10){
					uint32_t m = (Random()/51);
					sprite_t proj = {Enemies1[m].x+7, Enemies1[m].y+10, EnemyLaser, 1, 2};
					uint8_t index = 0;
					for(int i = 0; i<10; i++){
						if(EnemyProjectiles[i].alive == 0)
							index = i;
					}
					EnemyProjectiles[index] = proj;
					numEnProjectiles++;
				}
				setFlag(0);
			}
		}else{
			if(getFlag()){
				Enemies1[0].bmp = SmallEnemy10pointB;
				Enemies1[1].bmp = SmallEnemy10pointA;
				Enemies1[2].bmp = SmallEnemy20pointB;
				Enemies1[3].bmp = SmallEnemy20pointA;
				Enemies1[4].bmp = SmallEnemy30pointB;
				Enemies1[5].bmp = SmallEnemy30pointA;
				if(numEnProjectiles < 10){
					uint32_t m = (Random()/51);
					sprite_t proj = {Enemies1[m].x+7, Enemies1[m].y+5, EnemyLaser, 1, 2};
					uint8_t index = 0;
					for(int i = 0; i<10; i++){
						if(EnemyProjectiles[i].alive == 0)
							index = i;
					}
					EnemyProjectiles[index] = proj;
					numEnProjectiles++;
				}
				setFlag(0);
			}
		}	
		ST7735_FillScreen(0x0000);            // set screen to black
		handleCollisions(Enemies1, Bunkers);
		draw(Enemies1, 16, 10, 6);
		draw(Projectiles, 2, 4, 10);
		draw(Bunkers, 18, 5, 3);
		draw(EnemyProjectiles, 2, 4, 10);
		tick(Projectiles, 'y', 10, 'p');
		tick(Enemies1, 'x', 6, 'a');
		tick(EnemyProjectiles, 'y', 10, 'e');
		ST7735_DrawBitmap(Position, 159, PlayerShip0, 18,8); // player ship 
		ST7735_SetCursor(0,0);
		ST7735_OutString("Lives: ");
		LCD_OutDec(numLives);
		MyDelay1ms(32);
	}
	if(enemiesLeft == 0){
		ST7735_FillScreen(0x0000);            // set screen to black
		ST7735_SetCursor(1, 1);
		ST7735_OutString("YOU WIN");
		ST7735_SetCursor(1, 2);
		ST7735_OutString("Great Job");
		ST7735_SetCursor(1, 3);
		ST7735_OutString("Earthling!");
		ST7735_SetCursor(2, 4);
		LCD_OutDec(numLives);
	}else{
		ST7735_FillScreen(0x0000);            // set screen to black
		ST7735_SetCursor(1, 1);
		ST7735_OutString("GAME OVER");
		ST7735_SetCursor(1, 2);
		ST7735_OutString("Nice try,");
		ST7735_SetCursor(1, 3);
		ST7735_OutString("Earthling!");
		ST7735_SetCursor(2, 4);
  }
}


// You can use this timer only if you learn how it works

void Delay100ms(uint32_t count){uint32_t volatile time;
  while(count>0){
    time = 727240;  // 0.1sec at 80 MHz
    while(time){
	  	time--;
    }
    count--;
  }
}


