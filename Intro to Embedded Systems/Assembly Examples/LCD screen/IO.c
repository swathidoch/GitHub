// IO.c
// This software configures the switch and LED
// You are allowed to use any switch and any LED, 
// although the Lab suggests the SW1 switch PF4 and Red LED PF1
// Runs on LM4F120 or TM4C123
// Program written by: Carson Schubert and Swathi Dochibhotla
// Date Created: October 25, 2017
// Last Modified:  10/27/17
// Lab number: 7


#include "tm4c123gh6pm.h"
#include <stdint.h>

void delay10ms(uint32_t n){
	uint32_t volatile time;
	while(n){
    time = 72724*2/91;  // 1msec, tuned at 80 MHz
    while(time){
	  	time--;
    }
    n--;
  }
}

//------------IO_Init------------
// Initialize GPIO Port for a switch and an LED
// Input: none
// Output: none
void IO_Init(void) {
  SYSCTL_RCGCGPIO_R |= 0x20;
	GPIO_PORTF_LOCK_R = GPIO_LOCK_KEY;
	GPIO_PORTF_CR_R = 0xFF;
	GPIO_PORTF_DEN_R = 0x14;
	GPIO_PORTF_DIR_R = 0x04;
	GPIO_PORTF_PUR_R = 0x10;
}

//------------IO_HeartBeat------------
// Toggle the output state of the  LED.
// Input: none
// Output: none
void IO_HeartBeat(void) {
  GPIO_PORTF_DATA_R = ((GPIO_PORTF_DATA_R)^0x04);
}


//------------IO_Touch------------
// wait for release and press of the switch
// Delay to debounce the switch
// Input: none
// Output: none
void IO_Touch(void) {
	while((GPIO_PORTF_DATA_R)&0x10){};                 //wait for button press
	delay10ms(2);                                      //debounce
	while(!((GPIO_PORTF_DATA_R)&0x10)){};              //wait for button release
}

