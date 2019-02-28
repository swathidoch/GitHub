// dac.c
// This software configures DAC output
// Runs on LM4F120 or TM4C123
// Program written by: Carson Schubert and Swathi Dochibhotla
// Date Created: 3/6/17 
// Last Modified: 10/20/17
// Lab number: 6
// Hardware connections
// DAC bits 0-3: PB0-PB3

#include <stdint.h>
#include "tm4c123gh6pm.h"
#include "SysTickInts.h" 
#include "TExaS.h"


// Code files contain the actual implemenation for public functions
// this file also contains an private functions and private data

// **************DAC_Init*********************
// Initialize 4-bit DAC, called once 
// Input: none
// Output: none
void DAC_Init(void){
	SYSCTL_RCGCGPIO_R = 0x02;        // Port B
	SysTick_Wait(50);
  GPIO_PORTB_AMSEL_R &= ~0x0F;     	// disable analog functionality on PB3-0
  GPIO_PORTB_PCTL_R &= ~0x0000FFFF;	// configure PB3-0 as GPIO
  GPIO_PORTB_DIR_R = 0x0F;        	// make PB3-0 out
	GPIO_PORTB_DR8R_R = 0x0F;				// enable 8 mA drive on PB3-0
  GPIO_PORTB_AFSEL_R &= ~0x0F;     	// disable alt funct on PB3-0
  GPIO_PORTB_DEN_R = 0x0F;        	// enable digital I/O on PB3-0
}

// **************DAC_Out*********************
// output to DAC
// Input: 4-bit data, 0 to 15 
// Output: none
void DAC_Out(uint32_t data){
}
