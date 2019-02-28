// PLL.h
// Runs on LM3S1968
// A software function to change the bus speed using the PLL.
// Commented lines in the function PLL_Init() initialize the PWM
// to either 25 MHz or 50 MHz.  When using an oscilloscope to
// look at LED0, it should be clear to see that the LED flashes
// about 2 (50/25) times faster with a 50 MHz clock than with a
// 25 MHz clock.
// Daniel Valvano
// February 21, 2012

/* This example accompanies the book
   "Embedded Systems: Real Time Interfacing to the Arm Cortex M3",
   ISBN: 978-1463590154, Jonathan Valvano, copyright (c) 2011
   Program 2.10, Figure 2.31

 Copyright 2011 by Jonathan W. Valvano, valvano@mail.utexas.edu
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

// configure the system to get its clock from the PLL 50 MHz
void PLL_Init(void);
