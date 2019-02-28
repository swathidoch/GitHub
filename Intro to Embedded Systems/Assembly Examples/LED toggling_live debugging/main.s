;****************** main.s ***************
; Program written by: Carson Schubert and Swathi Dochibhotla
; Date Created: 2/14/2017
; Last Modified: 10/8/2017
; Brief description of the program
;   The LED toggles at 8 Hz and a varying duty-cycle
;   Repeat the functionality from Lab3 but now we want you to 
;   insert debugging instruments which gather data (state and timing)
;   to verify that the system is functioning as expected.
; Hardware connections (External: One button and one LED)
;  PE1 is Button input  (1 means pressed, 0 means not pressed)
;  PE0 is LED output (1 activates external LED on protoboard)
;  PF2 is Blue LED on Launchpad used as a heartbeat
; Instrumentation data to be gathered is as follows:
; After Button(PE1) press collect one state and time entry. 
; After Buttin(PE1) release, collect 7 state and
; time entries on each change in state of the LED(PE0): 
; An entry is one 8-bit entry in the Data Buffer and one 
; 32-bit entry in the Time Buffer
;  The Data Buffer entry (byte) content has:
;    Lower nibble is state of LED (PE0)
;    Higher nibble is state of Button (PE1)
;  The Time Buffer entry (32-bit) has:
;    24-bit value of the SysTick's Current register (NVIC_ST_CURRENT_R)
; Note: The size of both buffers is 50 entries. Once you fill these
;       entries you should stop collecting data
; The heartbeat is an indicator of the running of the program. 
; On each iteration of the main loop of your program toggle the 
; LED to indicate that your code(system) is live (not stuck or dead).

GPIO_PORTE_DATA_R  EQU 0x400243FC
GPIO_PORTE_DIR_R   EQU 0x40024400
GPIO_PORTE_AFSEL_R EQU 0x40024420
GPIO_PORTE_DEN_R   EQU 0x4002451C

GPIO_PORTF_DATA_R  EQU 0x400253FC
GPIO_PORTF_DIR_R   EQU 0x40025400
GPIO_PORTF_AFSEL_R EQU 0x40025420
GPIO_PORTF_PUR_R   EQU 0x40025510
GPIO_PORTF_DEN_R   EQU 0x4002551C
SYSCTL_RCGCGPIO_R  EQU 0x400FE608
GPIO_PORTF_LOCK_R  EQU 0x40025520
GPIO_PORTF_CR_R    EQU 0x40025524
GPIO_LOCK_KEY      EQU 0x4C4F434B
	
NVIC_ST_CTRL_R        EQU 0xE000E010
NVIC_ST_RELOAD_R      EQU 0xE000E014
NVIC_ST_CURRENT_R     EQU 0xE000E018

COUNT EQU 0x4C4B4 ;1/40s delay

; RAM Area
	   THUMB
	   AREA    DATA, ALIGN=2
;-UUU-Declare  and allocate space for your Buffers 
;    and any variables (like pointers and counters) here
COUNT_OFF SPACE 4                  ;global variable for the amount of delay cycles to stay off
COUNT_ON  SPACE 4                  ;global variable for the amount of delay cycles to stay on
DataBuffer SPACE 50
TimeBuffer SPACE 50*4
DataPt SPACE 4
TimePt SPACE 4
NEntries SPACE 4
; ROM Area
       ;IMPORT  TExaS_Init
	   ;IMPORT  SysTick_Init	  
;-UUU-Import routine(s) from other assembly files (like SysTick.s) here
       AREA    |.text|, CODE, READONLY, ALIGN=2
       THUMB
       EXPORT  Start
	   IMPORT  TExaS_Init
	   IMPORT  SysTick_Init

Start
 ; TExaS_Init sets bus clock at 80 MHz
     BL  TExaS_Init ; voltmeter, scope on PD3
     CPSIE  I    ; TExaS voltmeter, scope runs on interrupts
	 BL  PortF_Init
	 BL  PortE_Init
	 MOV R0, #4
	 LDR R1, =COUNT_OFF
	 STR R0, [R1]
	 MOV R0, #1
	 LDR R1, =COUNT_ON
	 STR R0, [R1]                  ;initialize with a 20% duty cycle
	 LDR R1, =DataPt
	 LDR R0, =DataBuffer
	 STR R0, [R1]				   ;initialize pointer to dataBuffer
	 LDR R1, =TimePt
	 LDR R0, =TimeBuffer
	 STR R0, [R1]                  ;initialize pointer to timeBuffer
	 BL  Debug_Init
	 LDR R1, =NEntries             ;set NEntries to 0
	 MOV R0, #0
	 STR R0, [R1]
	 MOV R10, #1
loop  
reg
	 BL  ToggleHB
			   ; button press indicator
	 LDR R5, =GPIO_PORTE_DATA_R 
     LDR R6, [R5]
	 AND R6, #0x02
	 CMP R6, #0x02
	 BNE Check                     ;if the button is not currently pressed we check for a release case
	 MOV R7, #1                    ;when the button is clicked, a 1 is stored in R7
	 CMP R10, #1
	 BNE noRecord
	 BL Debug_Capture
	 SUB R10, #1
noRecord	 
	 B   sts                       ;skips the release case check while the button is held
Check
     CMP R7, #1                    
	 BNE sts 
     BL CycleChange                ;change the duty cycle if the button was just released
	 MOV R10, #1
	 MOV R9, #7

sts  LDR R1, =COUNT_OFF            
	 LDR R2, [R1]
	 MOV R3, R2                    ;store current COUNT_OFF value in R3 for later use
	 CMP R3, #5                    ;check if duty cycle is 0% (always off)
	 BNE gud                       
	 BL  TurnOff                   ;make sure light is off if duty cycle is 0%
gud	 CMP R2, #0                    ;reset condition codes
test BEQ try                       ;check if we have more delay cycles to execute and exits loop if done
do   LDR R0, =COUNT                ;this loop executes a delay of 1/40s (derived from 8Hz toggle)
wait SUBS R0, #0x01
	 BNE wait                      
	 SUBS R2, #1
	 B    test

try                                ;check for edge cases and toggle if necessary
     CMP R3, #5          
	 BNE not0                      ;if we are at a duty cycle of 0%, skip COUNT_ON code and toggle altogether
	 CMP R9, #0
	 BEQ wDone
	 BL  Debug_Capture
	 SUB R9, #1
	 B   loop
wDone 
     MOV R9, #0
	 B loop
not0
	 CMP R3, #0
	 BEQ go                        ;if we are at a duty cycle of 100%, skip toggle
	 BL  Toggle

go   LDR R1, =COUNT_ON             
	 LDR R2, [R1]
	 MOV R3, R2                    ;store current COUNT_ON value in R3 for later use
	 CMP R3, #5                    ;check if duty cycle is 100% (always on)
	 BNE st
	 BL  TurnOn                    ;make sure light is on if duty cycle is 100%
st	 CMP R2, #0                    ;reset condition codes
tet  BEQ try2                      ;check if we habe more delay cycles to execute and exits loop if done
	 LDR R0, =COUNT                ;executes a delay of 1/40s
wait2 SUBS R0, #1
	 BNE wait2
	 SUBS R2, #1
	 B    tet
	 
try2                               ;check for edge cases and toggle if neccessary
     CMP R3, #5                    
	 BNE not100                     ;if we are at a duty cycle of 100%, skip toggle
	 CMP R9, #0
	 BEQ weDone
	 BL  Debug_Capture
	 SUB R9, #1
	 B   loop
weDone 
     MOV R9, #0
	 B   loop
not100
     BL Toggle
     B    loop 
	 
Debug_Init  
	LDR R0, =DataPt 
	LDR R5, [R0]
	LDR R1, =TimePt
	LDR R6, [R1]
	MOV R2, #0xFF
	MOV R3, #0xFFFFFFFF			   ; R2 = default value for data buffer, R3 = default for time buffer
	MOV R4, #50                    ; R4 = loop counter (50 bc array sizes are 50)
loadNulls
	STRB R2, [R5]                  
	STR R3, [R6]                   ;store the default values at the current pointer
	ADD R5, #1
	ADD R6, #4                     ;increment poiner based on data size (8 bit for data, 32 bit for time)
	SUB R4, #1                    ;decrement loop counter
	CMP R4, #0                     
	BNE loadNulls                  ;loop again if counter is not 0
	MOV R7, LR
	BL SysTick_Init                ;initialize sysTick
	MOV LR, R7
	BX LR

Debug_Capture
    PUSH{R0-R5}

	LDR R1, =NEntries
	LDR R0, [R1]
	CMP R0, #50
	BEQ Done                       ; if there are already 50 entries, skip adding more
	ADD R0, #1
	STR R0, [R1]                   ; increment number of entries
	LDR R1, =GPIO_PORTE_DATA_R
	LDR R0, [R1]
	LDR R3, =NVIC_ST_CURRENT_R
	LDR R2, [R3]                   ; read Port E data and SysTick data
	MOV R4, R0                     ; copy Port E data into R4
	AND R0, #0x01               
	AND R4, #0x02                  ;mask for bit 1 in R4, mask for bit 0 in R0  
	LSL R4, #3                     ;shift bit 1 to bit 4 in R4
	ORR R0, R4                     ;put the contents of bit 4 in R4 into R0
	LDR R4, =DataPt
	LDR R5, [R4]
	STRB R0, [R5]                     ;store this entry in the data buffer
	ADD R5, #1
	STR R5, [R4]                   ;increment data buffer pointer
	LDR R4, =TimePt
	LDR R5, [R4]
	STR R2, [R5]                   ;store time data in time buffer
	ADD R5, #4
	STR R5, [R4]                   ;increment time buffer pointer
Done
    POP{R0-R5}
	BX  LR

CycleChange
;this subroutine will change the duty cycle by 20%
     LDR R1, =COUNT_OFF
	 LDR R2, [R1]
	 CMP R2, #0
	 BNE norm                      ;checks if COUNT_OFF needs to be wrapped around from 0
	 MOV R2, #5
	 B   mem
norm SUB R2, #1
mem  STR R2, [R1]
     LDR R1, =COUNT_ON
	 LDR R2, [R1]
	 CMP R2, #5
	 BNE nor                       ;checks if COUNT_ON needs to be wrapped around from 5
	 MOV R2, #0
	 B   meme
nor  ADD R2, #1
meme STR R2, [R1]
     MOV R7, #0                    ;resets the release case register
	 BX  LR
	 
Toggle   
;toggles the LED on PE1
	LDR R1, =GPIO_PORTE_DATA_R
	LDR R0, [R1]
	EOR R0, #0x01
	STR R0, [R1]
	CMP R9, #0
	BEQ beDone
	MOV R11, LR
	BL  Debug_Capture
	MOV LR, R11
	SUB R9, #1
	BX  LR
beDone
	MOV R9, #0
    BX  LR
	
ToggleHB
	LDR R1, =GPIO_PORTF_DATA_R
	LDR R0, [R1]
	EOR R0, #0x04
	STR R0, [R1]
	BX  LR

TurnOn                             
;turns PE1 on
	LDR R1, =GPIO_PORTE_DATA_R
	LDR R0, [R1]
	ORR R0, #0x01
	STR R0, [R1]
	BX  LR
 
TurnOff                            
;turns PE1 off
    LDR R1, =GPIO_PORTE_DATA_R
	LDR R0, [R1]
	AND R0, #0x00
	STR R0, [R1]
	BX  LR
		  
PortF_Init
	LDR R1, =SYSCTL_RCGCGPIO_R		;enable clock for Port F   
    LDR R0, [R1]                	
	ORR R0, R0, #0x20
    STR R0, [R1]                  
    NOP								;wait for stabilized
    NOP
	LDR R1, =GPIO_PORTF_LOCK_R		;unlock lock register
	LDR R0, =GPIO_LOCK_KEY
	STR R0, [R1]
	LDR R1, =GPIO_PORTF_CR_R		;enable commit for Port F
	MOV R0, #0xFF
	STR R0, [R1]
	LDR R1, =GPIO_PORTF_DIR_R		;set PF2 as output
	LDR R0, [R1]
	ORR R0, R0, #0x04
	STR R0, [R1]
	LDR R1, =GPIO_PORTF_DEN_R       
	MOV R0, #0xFF
	STR R0, [R1]
	BX  LR	 

PortE_Init
	LDR R1, =SYSCTL_RCGCGPIO_R
	LDR R0, [R1]
	ORR R0, R0, #0x10
	STR R0, [R1]
	NOP
	NOP
	LDR R1, =GPIO_PORTE_DIR_R
	LDR R0, [R1]
	ORR R0, R0, #0x01
	STR R0, [R1]
	LDR R1, =GPIO_PORTE_DEN_R
	MOV R0, #0xFF
	STR R0, [R1]
	BX  LR
	
      ALIGN      ; make sure the end of this section is aligned
      END        ; end of file
