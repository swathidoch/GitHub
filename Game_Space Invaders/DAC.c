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
// Code files contain the actual implemenation for public functions
// this file also contains an private functions and private data

// **************DAC_Init*********************
// Initialize 4-bit DAC, called once 
// Input: none
// Output: none
void DAC_Init(void){
	SYSCTL_RCGCGPIO_R |= 0x02;
	for(int i = 0; i<50; i++){};
	GPIO_PORTB_DEN_R = 0x0F;
	GPIO_PORTB_DIR_R = 0x0F;
	GPIO_PORTB_DATA_R = 0x00;
}

// **************DAC_Out*********************
// output to DAC
// Input: 4-bit data, 0 to 15 
// Output: none
void DAC_Out(uint32_t data){
	GPIO_PORTB_DATA_R = data;
}
