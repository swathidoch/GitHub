; SysTick.s
; Module written by: Carson Schubert and Swathi Dochibhotla
; Date Created: 2/14/2017
; Last Modified: 10/8/2017 
; Brief Description: Initializes SysTick

NVIC_ST_CTRL_R        EQU 0xE000E010
NVIC_ST_RELOAD_R      EQU 0xE000E014
NVIC_ST_CURRENT_R     EQU 0xE000E018
    THUMB
    AREA    DATA, ALIGN=2
;global variables go here
    ALIGN
    AREA    |.text|, CODE, READONLY, ALIGN=2
    EXPORT  SysTick_Init
; -UUU- You add code here to export your routine(s) from SysTick.s to main.s

;------------SysTick_Init------------
; ;-UUU-Complete this subroutine
; Initialize SysTick running at bus clock.
; Make it so NVIC_ST_CURRENT_R can be used as a 24-bit time
; Input: none
; Output: none
; Modifies: R1, R0
SysTick_Init
 ; **-UUU-**Implement this function****
	;disable during setup
	LDR R1, =NVIC_ST_CTRL_R
	MOV R0, #0
	STR R0, [R1]
	;set reloard to maximum reload value
	LDR R1, =NVIC_ST_RELOAD_R
	LDR R0, =0x00FFFFFF
	STR R0, [R1]
	;writing any value to CURRENT will clear it
	LDR R1, =NVIC_ST_CURRENT_R
	MOV R0, #0
	STR R0, [R1]
	;enable SysTick with core clock
	LDR R1, =NVIC_ST_CTRL_R
	MOV R0, #0x05 ; enable with no interrupts
	STR R0, [R1] ; ENABLE and CLK_SRC bits set
    BX  LR                          ; return

    ALIGN                           ; make sure the end of this section is aligned
    END                             ; end of file
