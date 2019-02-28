;****************** main.s ***************
; Program written by: Carson Schubert and Swathi Dochibhotla
; Date Created: 2/4/2017
; Last Modified: 9/2/2017
; Brief description of the program
;   The LED toggles at 8 Hz and a varying duty-cycle
; Hardware connections (External: One button and one LED)
;  PE1 is Button input  (1 means pressed, 0 means not pressed)
;  PE0 is LED output (1 activates external LED on protoboard)
;  PF4 is builtin button SW1 on Launchpad (Internal) 
;        Negative Logic (0 means pressed, 1 means not pressed)
; Overall functionality of this system is to operate like this
;   1) Make PE0 an output and make PE1 and PF4 inputs.
;   2) The system starts with the the LED toggling at 8Hz,
;      which is 8 times per second with a duty-cycle of 20%.
;      Therefore, the LED is ON for (0.2*1/8)th of a second
;      and OFF for (0.8*1/8)th of a second.
;   3) When the button on (PE1) is pressed-and-released increase
;      the duty cycle by 20% (modulo 100%). Therefore for each
;      press-and-release the duty cycle changes from 20% to 40% to 60%
;      to 80% to 100%(ON) to 0%(Off) to 20% to 40% so on
;   4) Implement a "breathing LED" when SW1 (PF4) on the Launchpad is pressed:
;      a) Be creative and play around with what "breathing" means.
;         An example of "breathing" is most computers power LED in sleep mode
;         (e.g., https://www.youtube.com/watch?v=ZT6siXyIjvQ).
;      b) When (PF4) is released while in breathing mode, resume blinking at 8Hz.
;         The duty cycle can either match the most recent duty-
;         cycle or reset to 20%.
;      TIP: debugging the breathing LED algorithm and feel on the simulator is impossible.
; PortE device registers
GPIO_PORTE_DATA_R  EQU 0x400243FC
GPIO_PORTE_DIR_R   EQU 0x40024400
GPIO_PORTE_AFSEL_R EQU 0x40024420
GPIO_PORTE_DEN_R   EQU 0x4002451C
; PortF device registers
GPIO_PORTF_DATA_R  EQU 0x400253FC
GPIO_PORTF_DIR_R   EQU 0x40025400
GPIO_PORTF_AFSEL_R EQU 0x40025420
GPIO_PORTF_PUR_R   EQU 0x40025510
GPIO_PORTF_DEN_R   EQU 0x4002551C
GPIO_PORTF_LOCK_R  EQU 0x40025520
GPIO_PORTF_CR_R    EQU 0x40025524
GPIO_LOCK_KEY      EQU 0x4C4F434B  ; Unlocks the GPIO_CR register
SYSCTL_RCGCGPIO_R  EQU 0x400FE608

COUNT EQU 0x4C4B4 ;1/40s delay
BCOUNT EQU 0xC35  ;1/8000s delay

     IMPORT  TExaS_Init
     THUMB
     AREA    DATA, ALIGN=2
;global variables go here
COUNT_OFF SPACE 4                  ;global variable for the amount of delay cycles to stay off
COUNT_ON  SPACE 4                  ;global variable for the amount of delay cycles to stay on
BCOUNT_OFF SPACE 4                 
BCOUNT_ON  SPACE 4                 ;variables for breathe duty cycle
BLOOP_COUNTER SPACE 4              ;loop counter for breathe function
BDIR SPACE 4                       ;boolean 
     AREA    |.text|, CODE, READONLY, ALIGN=2
     THUMB
     EXPORT  Start
Start
 ; TExaS_Init sets bus clock at 80 MHz
     BL  TExaS_Init ; voltmeter, scope on PD3
 ; Initialization goes here
     BL  PortF_Init
	 BL  PortE_Init
	 MOV R0, #4
	 LDR R1, =COUNT_OFF
	 STR R0, [R1]
	 MOV R0, #1
	 LDR R1, =COUNT_ON
	 STR R0, [R1]                  ;initialize with a 20% duty cycle
	 MOV R0, #79
	 LDR R1, =BCOUNT_OFF
	 STR R0, [R1]
	 MOV R0, #1
	 LDR R1, =BCOUNT_ON
	 STR R0, [R1]                  ;initialize breathe with 0% duty cycle
	 LDR R1, =BLOOP_COUNTER
	 MOV R0, #2
	 STR R0, [R1]                  ;set breathe loop counter to 2
	 LDR R1, =BDIR
	 MOV R0, #1
	 STR R0, [R1]                  ;set BDIR to default going up
;a	 BL  TurnOn
;	 B   a
     CPSIE  I    ; TExaS voltmeter, scope runs on interrupts
	 
loop
     ;check if SW1 (onboard switch) is pressed
     LDR R5, =GPIO_PORTF_DATA_R
	 LDR R6, [R5]
	 AND R6, #0x10
	 CMP R6, #0x10
	 BNE BreatheStart              ;if the button is pressed, jump to breathing routine
     ;check for activity on PE1 (button)
reg  LDR R5, =GPIO_PORTE_DATA_R 
     LDR R6, [R5]
	 AND R6, #0x02
	 CMP R6, #0x02
	 BNE Check                     ;if the button is not currently pressed we check for a release case
	 MOV R7, #1                    ;when the button is clicked, a 1 is stored in R7
	 B   sts                       ;skips the release case check while the button is held
Check
     CMP R7, #1                    
	 BNE sts 
     BL CycleChange                ;change the duty cycle if the button was just released

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
	 BEQ loop                      ;if we are at a duty cycle of 0%, skip COUNT_ON code and toggle altogether
	 CMP R3, #0
	 BEQ go                        ;if we are at a duty cycle of 100%, skip toggle
	 BL  Toggle

go   LDR R1, =COUNT_ON             
	 LDR R2, [R1]
	 MOV R3, R2                    ;store current COUNT_ON value in R3 for later use
	 CMP R3, #5                    ;check if duty cycle is 100% (always on)
	 .BNE st
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
	 BEQ loop                      ;if we are at a duty cycle of 100%, skip toggle
     BL Toggle
	 
     B    loop   

Breathe
;implement a breathing LED
CheckLoop
     LDR R8, =BLOOP_COUNTER
	 LDR R9, [R8]
	 SUBS R9, #1
	 STR R9, [R8]
	 CMP R9, #0
	 BNE  rch
	 BL  BCycleChange
rch  LDR R5, =GPIO_PORTF_DATA_R    ;checks if SW1 is still pressed
	 LDR R6, [R5]
	 AND R6, #0x10
	 CMP R6, #0x10
	 BNE BreatheStart              ;if the button is still pressed, continue breathing
	 BL  TurnOff                   
	 B   reg                       ;if not pressed, reset LED and jump back to regular blinking loop
BreatheStart
     LDR R1, =BCOUNT_OFF            
	 LDR R2, [R1]
	 MOV R3, R2                     ;store current BCOUNT_OFF value in R3 for later use
	 CMP R3, #80                    ;check if duty cycle is 0% (always off)
	 BNE gudd                       
	 BL  TurnOff                   ;make sure light is off if duty cycle is 0%
gudd CMP R2, #0                    ;reset condition codes
test2 BEQ tryy                       ;check if we have more delay cycles to execute and exits loop if done
     LDR R0, =BCOUNT                ;this loop executes a delay of 1/4000s (derived from 80Hz toggle)
wait3 SUBS R0, #0x01
	 BNE wait3                      
	 SUBS R2, #1
	 B    test2

tryy                                ;toggle
	 BL  Toggle

go2  LDR R1, =BCOUNT_ON             
	 LDR R2, [R1]
	 MOV R3, R2                    ;store current COUNT_ON value in R3 for later use
	 CMP R3, #80                    ;check if duty cycle is 100% (always on)
	 BNE st2
	 BL  TurnOn                    ;make sure light is on if duty cycle is 100%
st2	 CMP R2, #0                    ;reset condition codes
tet2 BEQ try4                      ;check if we have more delay cycles to execute and exits loop if done
	 LDR R0, =BCOUNT                ;executes a delay of 1/4000s
wait4 SUBS R0, #1
	 BNE wait4
	 SUBS R2, #1
	 B    tet2
	 
try4                              ;toggle
     BL Toggle
	 
     B   CheckLoop

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
	 
BCycleChange
;changes breathing duty cycle by 10%
     LDR R4, =BDIR
	 LDR R5, [R4]                 ;R5 now contains the BDIR bit
     LDR R1, =BCOUNT_OFF
	 LDR R2, [R1]
	 CMP R2, #0
	 BNE nom                      ;checks if BCOUNT_OFF has reached the low point
	 ADD R2, #1                   ;if it is 0, we switch directions and begin adding instead
	 STR R2, [R1]
	 LDR R1, =BCOUNT_ON
	 LDR R2, [R1]
	 SUB R2, #1
	 STR R2, [R1]
	 EOR R5, #1
	 STR R5, [R4]                 ;sets direction bit to 0 indicating we are now rising
	 ;BL  TurnOff
	 B   d                        ;store new direction bit and continue to BCOUNT_ON
nom  CMP R2, #80
	 BNE num                      ;checks if BCOUNT_OFF has reached the high point
	 SUB R2, #1                   ;if it is 0, we switch directions and begin subtracting instead
	 STR R2, [R1]
	 LDR R1, =BCOUNT_ON
	 LDR R2, [R1]
	 ADD R2, #1
	 STR R2, [R1]
	 EOR R5, #1
	 STR R5, [R4]                 ;toggles direction bit back to 1 to indicate we should begin falling
	 B   d
num  CMP R5, #1
     BNE ad                       ;check direction bit (1 = falling, 0 = rising)
     SUB R2, #1
	 B   mim
ad   ADD R2, #1
mim  STR R2, [R1]
     LDR R1, =BCOUNT_ON
	 LDR R2, [R1]
	 CMP R5, #1
	 BNE subt                     ;check direction bit (1=falling, 0=rising) 
	 ADD R2, #1                   ;add if falling
	 B   memy
subt  SUB R2, #1                  ;subtract if rising
memy STR R2, [R1]
d    LDR R1, =BLOOP_COUNTER
	 MOV R0, #2
	 STR R0, [R1]                  ;reset breathe loop counter
a	 BX  LR
	
Toggle   
;toggles the LED on PE1
	LDR R1, =GPIO_PORTE_DATA_R
	LDR R0, [R1]
	EOR R0, #0x01
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
	LDR R1, =GPIO_PORTF_DIR_R		;set PF4 as input
	LDR R0, [R1]
	ORR R0, R0, #0x00
	STR R0, [R1]
	LDR R1, =GPIO_PORTF_PUR_R		;enable pull up resistor for PF4
	MOV R0, #0x10
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

