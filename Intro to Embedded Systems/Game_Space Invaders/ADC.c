// ADC.c
// Runs on LM4F120/TM4C123
// Provide functions that initialize ADC0
// Last Modified: 11/15/2017
// Student names: Carson Schubert and Swathi Dochibhotla
// Last modification date: 11/22/17
// ADC initialization function 
// Input: none
// Output: none

#include <stdint.h>
#include "tm4c123gh6pm.h"

void ADC_Init(void){
	SYSCTL_RCGCGPIO_R |= 0x10;
	for(int i = 0; i<50; i++){};
	while((SYSCTL_RCGCGPIO_R&0x10)==0){};
	GPIO_PORTE_DIR_R &= ~0x04;
	GPIO_PORTE_AFSEL_R |= 0x04;
	GPIO_PORTE_DEN_R &= ~0x04;
	GPIO_PORTE_AMSEL_R |= 0x04;
	SYSCTL_RCGCADC_R |= 0x01;
	for(int i = 0; i<50; i++){};
	while((SYSCTL_RCGCADC_R&0x01)==0){};
	ADC0_PC_R = 0x01; //sampling rate
	ADC0_SSPRI_R = 0x0123; //priority
	ADC0_ACTSS_R &= ~0x0008; //disable sequencer 3 during config
	ADC0_EMUX_R &= ~0xF000; //software trigger
	ADC0_SSMUX3_R = (ADC0_SSMUX3_R&0xFFFFFFF0) + 1; //use AIN1 (PE2)
	ADC0_SSCTL3_R = 0x0006;
	//ADC0_SAC_R |= 0x4; //hardware oversampling
	ADC0_IM_R &= ~0x0008;
	ADC0_ACTSS_R |= 0x0008;
}

//------------ADC_In------------
// Busy-wait Analog to digital conversion
// Input: none
// Output: 12-bit result of ADC conversion
uint32_t ADC_In(void){  
  uint32_t result;
	ADC0_PSSI_R = 0x0008; //ask for sample
	while((ADC0_RIS_R&0x08)==0){};
	result = ADC0_SSFIFO3_R & 0xFFF;
	ADC0_ISC_R = 0x0008;
  return result; // remove this, replace with real code
}
