// TableTrafficLight.c solution to edX lab 10, EE319KLab 5
// Runs on LM4F120 or TM4C123
// Index implementation of a Moore finite state machine to operate a traffic light.  
// Daniel Valvano, Jonathan Valvano
// October 12, 2017

/* 
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

// See TExaS.h for other possible hardware connections that can be simulated
// east/west red light connected to PB5
// east/west yellow light connected to PB4
// east/west green light connected to PB3
// north/south facing red light connected to PB2
// north/south facing yellow light connected to PB1
// north/south facing green light connected to PB0
// pedestrian detector connected to PE2 (1=pedestrian present)
// north/south car detector connected to PE1 (1=car present)
// east/west car detector connected to PE0 (1=car present)
// "walk" light connected to PF3 (built-in green LED)
// "don't walk" light connected to PF1 (built-in red LED)
#include <stdint.h>
#include "tm4c123gh6pm.h"
#include "SysTick.h"
#include "TExaS.h"

#define SENSOR                  (GPIO_PORTE_DATA_R)
#define LIGHT                   (GPIO_PORTB_DATA_R)
#define WALKLIGHT               (GPIO_PORTF_DATA_R)

// Declare your FSM linked structure here
struct State {
	uint32_t outPB;
	uint32_t outPF;
	uint32_t time;
	uint32_t next[8];};
typedef const struct State STyp;
#define goS        0
#define waitS      1
#define waitSWalk  2
#define goW        3
#define waitW      4
#define waitWWalk  5
#define walk       6
#define FFlash     7
#define FOff       8
#define SFlash     9
#define SOff       10
#define TFlash     11
#define TOff       12
	
STyp FSM[13]={
	//goS state fixed
	{0x21, 0x02, 2000, {goS, waitS, goS, waitS, waitSWalk, waitSWalk, waitSWalk, waitSWalk}},
	//waitS state
	{0x22, 0x02, 500, {goW, goW, goW, goW, goW, goW, goW, goW}},
	//waitSWalk state
	{0x22, 0x02, 500, {walk, walk, walk, walk, walk, walk, walk, walk}},
	//goW state fixed
	{0x0C, 0x02, 2000, {goW, goW, waitW, waitW, waitWWalk, waitWWalk, waitW, waitW}},
	//waitW state
	{0x14, 0x02, 500, {goS, goS, goS, goS, goS, goS, goS, goS}},
	//waitWWalk state
	{0x14, 0x02, 500, {walk, walk, walk, walk, walk, walk, walk, walk}},
	//walk state fixed
	{0x24, 0x08, 2000, {walk, FFlash, FFlash, FFlash, walk, FFlash, FFlash, FFlash}},
	//FFlash state
	{0x24, 0x02, 250, {FOff, FOff, FOff, FOff, FOff, FOff, FOff, FOff}},
	//FOff state
	{0x24, 0x00, 250, {SFlash, SFlash, SFlash, SFlash, SFlash, SFlash, SFlash, SFlash}},
	//SFlash state
	{0x24, 0x02, 250, {SOff, SOff, SOff, SOff, SOff, SOff, SOff, SOff}},
	//SOff state
	{0x24, 0x00, 250, {TFlash, TFlash, TFlash, TFlash, TFlash, TFlash, TFlash, TFlash}},
	//TFlash state
	{0x24, 0x02, 250, {TOff, TOff, TOff, TOff, TOff, TOff, TOff, TOff}},
	//TOff state fixed
	{0x24, 0x00, 250, {walk, goW, goS, goW, walk, goW, goS, goW}}
};
uint32_t S;
uint32_t INPUT;

void EnableInterrupts(void);
	
void PortFInit(void){
	SYSCTL_RCGC2_R |= 0x00000020;
	SysTick_Wait(50);
	GPIO_PORTF_LOCK_R = GPIO_LOCK_KEY;
  GPIO_PORTF_CR_R = 0x1F;
  GPIO_PORTF_DIR_R = 0x0A;
  GPIO_PORTF_DEN_R = 0x0A;	
}

void PortBInit(void){
	SYSCTL_RCGC2_R |= 0x01;
  SysTick_Wait(50);
	GPIO_PORTB_DEN_R = 0xFF;
	GPIO_PORTB_DIR_R = 0x00;
}

void PortEInit(void){
	SYSCTL_RCGC2_R |= 0x10;
	SysTick_Wait(50);
	GPIO_PORTE_DEN_R = 0x3F;
	GPIO_PORTE_DIR_R = 0x3F;
}

int main(void){ volatile unsigned long delay;
  TExaS_Init(SW_PIN_PE210, LED_PIN_PB543210); // activate traffic simulation and set system clock to 80 MHz
  SysTick_Init();     
  // Initialize ports and FSM you write this as part of Lab 5
	PortFInit();
	PortBInit();
	PortEInit();
  EnableInterrupts(); // TExaS uses interrupts
	S = goS;
  while(1){
 // FSM Engine
 // you write this as part of Lab 5
    LIGHT = FSM[S].outPB;
		WALKLIGHT = FSM[S].outPF;
		SysTick_Wait10ms(FSM[S].time);
		INPUT = GPIO_PORTE_DATA_R;
		S = FSM[S].next[INPUT];
  }
}
