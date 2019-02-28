// Lab8.c
// Runs on LM4F120 or TM4C123
// Student names: Carson Schubert andn Swathi Dochibhotla
// Last modification date: 11/12/17
// Last Modified: 11/8/2017 

// Analog Input connected to PE2=ADC1
// displays on Sitronox ST7735
// PF3, PF2, PF1 are heartbeats

#include <stdint.h>

#include "ST7735.h"
#include "TExaS.h"
#include "ADC.h"
#include "print.h"
#include "tm4c123gh6pm.h"

//*****the first three main programs are for debugging *****
// main1 tests just the ADC and slide pot, use debugger to see data
// main2 adds the LCD to the ADC and slide pot, ADC data is on Nokia
// main3 adds your convert function, position data is no Nokia

void DisableInterrupts(void); // Disable interrupts
void EnableInterrupts(void);  // Enable interrupts

#define PF1       (*((volatile uint32_t *)0x40025008))
#define PF2       (*((volatile uint32_t *)0x40025010))
#define PF3       (*((volatile uint32_t *)0x40025020))
// Initialize Port F so PF1, PF2 and PF3 are heartbeats
void PortF_Init(void){
	SYSCTL_RCGCGPIO_R |= 0x20;
	GPIO_PORTF_LOCK_R = GPIO_LOCK_KEY;
	GPIO_PORTF_CR_R = 0xFF;
	GPIO_PORTF_DEN_R = 0x0E;
	GPIO_PORTF_DIR_R = 0x0E;
}


void SysTick_Init(void){
	DisableInterrupts();
	NVIC_ST_CTRL_R = 0x00;
	NVIC_ST_RELOAD_R = 0xC3500;
	NVIC_ST_CURRENT_R = 0x00;
	NVIC_SYS_PRI3_R = (NVIC_SYS_PRI3_R & 0x00FFFFFF) | 0x20000000;
	NVIC_ST_CTRL_R = 0x0007;
	EnableInterrupts();
}

uint32_t Data;        // 12-bit ADC
uint32_t Position;    // 32-bit fixed-point 0.001 cm
static uint32_t ADCMail;
static uint32_t ADCStatus;

/*
int main(void){       // single step this program and look at Data
  TExaS_Init();       // Bus clock is 80 MHz 
  ADC_Init();         // turn on ADC, set channel to 1
  while(1){                
    Data = ADC_In();  // sample 12-bit channel 1
  }
}
*/

/*
int main(void){
  TExaS_Init();       // Bus clock is 80 MHz 
  ADC_Init();         // turn on ADC, set channel to 1
  ST7735_InitR(INITR_REDTAB); 
  PortF_Init();
  while(1){           // use scope to measure execution time for ADC_In and LCD_OutDec           
    PF2 = 0x04;       // Profile ADC
    Data = ADC_In();  // sample 12-bit channel 1
    PF2 = 0x00;       // end of ADC Profile
    ST7735_SetCursor(0,0);
    PF1 = 0x02;       // Profile LCD
    LCD_OutDec(Data); 
    ST7735_OutString("    ");  // spaces cover up characters from last output
    PF1 = 0;          // end of LCD Profile
  }
}
*/

uint32_t Convert(uint32_t input){
	uint32_t res = input * 4523;
	res /= 10000;
	res += 750;
  return res;
}

void SysTick_Handler(void){
	PF2 ^= 0x04;
	PF2 ^= 0x04;
	ADCMail = ADC_In();
	ADCStatus = 0x01;
	PF2 ^= 0x04; 
}

/*
int main(void){ 
  TExaS_Init();         // Bus clock is 80 MHz 
  ST7735_InitR(INITR_REDTAB); 
  PortF_Init();
  ADC_Init();         // turn on ADC, set channel to 1
  while(1){  
    PF2 ^= 0x04;      // Heartbeat
    Data = ADC_In();  // sample 12-bit channel 1
    PF3 = 0x08;       // Profile Convert
    Position = Convert(Data); 
    PF3 = 0;          // end of Convert Profile
    PF1 = 0x02;       // Profile LCD
    ST7735_SetCursor(0,0);
    LCD_OutDec(Data); ST7735_OutString("    "); 
    ST7735_SetCursor(6,0);
    LCD_OutFix(Position);
    PF1 = 0;          // end of LCD Profile
  }
} 
*/

int main(void){
  TExaS_Init();
  // your Lab 8
	PortF_Init();
	ADC_Init();
	ST7735_InitR(INITR_REDTAB); 
	SysTick_Init();
  while(1){
		if(ADCStatus == 1){
			Position = Convert(ADCMail);
			ADCStatus = 0;
			ST7735_SetCursor(0,0);
			LCD_OutFix(Position);
			ST7735_OutString(" cm");
		}
  }
}


