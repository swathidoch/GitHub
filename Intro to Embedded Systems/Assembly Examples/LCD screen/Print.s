; Print.s
; Student names: Carson Schubert and Swathi Dochibhotla
; Last modification date: 10/30/17
; Runs on LM4F120 or TM4C123
; EE319K lab 7 device driver for any LCD
;
; As part of Lab 7, students need to implement these LCD_OutDec and LCD_OutFix
; This driver assumes two low-level LCD functions
; ST7735_OutChar   outputs a single 8-bit ASCII character
; ST7735_OutString outputs a null-terminated string 

    IMPORT   ST7735_OutChar
    IMPORT   ST7735_OutString
    EXPORT   LCD_OutDec
    EXPORT   LCD_OutFix
		
	THUMB
	AREA     DATA, ALIGN=2
FixedPt      SPACE 6

    AREA    |.text|, CODE, READONLY, ALIGN=2
	PRESERVE8
    THUMB

  

;-----------------------LCD_OutDec-----------------------
; Output a 32-bit number in unsigned decimal format
; Input: R0 (call by value) 32-bit unsigned number
; Output: none
; Invariables: This function must not permanently modify registers R4 to R11
cnt    EQU 0

LCD_OutDec

      PUSH{R4,R10,R11,LR}
      SUB SP, #4                                ;allocate space on the stack for local variable cnt
	  MOV R1, #0
	  STR R1, [SP, #cnt]                        ;set default value of cnt to 0
	  MOV R11, SP                               ;set R11 to the stack frame pointer
	  MOV R10, #10                              ;use R10 as the divisor
ODloop                                          ;ODloop pushes the values onto the stack in order of MSB -> LSB
      LDR  R4, [R11, #cnt]                      ;load cnt
	  UDIV R3, R0, R10
	  MUL  R1, R3, R10
	  SUB  R1, R0, R1                           ;R1 = R0 % 10
	  PUSH{R1}                                  ;push R0 % 10 onto the stack
	  ADD  R4, #1                     
	  STR  R4, [R11, #cnt]                      ;increment and store cnt
	  MOVS R0, R3                               ;R0 = R0/10
	  CMP  R0, #0                               
	  BNE  ODloop                               ;if we have reached the end of the number, move on to printing
ODout
      POP{R0}
      ADD R0, #'0'                              ;add ASCII 0 to the value from the stack
	  BL  ST7735_OutChar                        ;print character to screen
	  LDR R4, [R11, #cnt]
	  SUBS R4, #1                               ;cnt--
	  STR R4, [R11, #cnt]
	  CMP R4, #0                   
	  BNE ODout                                 ;if we are out of values, move on
	  
	  ADD SP, #4                                ;deallocate
	  POP{R4,R10,R11,LR}                        ;restore

      BX  LR
;* * * * * * * * End of LCD_OutDec * * * * * * * *

; -----------------------LCD _OutFix----------------------
; Output characters to LCD display in fixed-point format
; unsigned decimal, resolution 0.001, range 0.000 to 9.999
; Inputs:  R0 is an unsigned 32-bit number
; Outputs: none
; E.g., R0=0,    then output "0.000 "
;       R0=3,    then output "0.003 "
;       R0=89,   then output "0.089 "
;       R0=123,  then output "0.123 "
;       R0=9999, then output "9.999 "
;       R0>9999, then output "*.*** "
; Invariables: This function must not permanently modify registers R4 to R11

LCD_OutFix

      MOV R1, #10000
	  CMP R0, R1
	  BLO st                                    ;if input is less than 10,000, go to standard output method 
	  MOV R2, #0                                ;R2 becomes counter
filler
      LDR R1, =FixedPt                          ;R1 holds string pointer
	  MOV R3, #0x2A                             ;0x2A = *
	  STR R3, [R1, R2]                          ;store at FixedPt + counter
	  ADD R2, #1
	  CMP R2, #1
	  BNE skipper                               ;if counter is 1, we need a decimal point
	  MOV R3, #0x2E                             ;0x2E = .
	  STR R3, [R1,R2]
	  ADD R2, #1
skipper
      CMP R2, #5
	  BNE filler                                ;keep filling the string until it has 5 characters
	  MOV R3, #0
	  STR R3, [R1, R2]                          ;null terminate the string
	  LDR R0, =FixedPt
	  PUSH{LR}
	  BL  ST7735_OutString                      ;output string to screen
	  POP{LR}
	  B   nope                                  ;return

st    PUSH{R4,R8,R9,R10,R11,LR}
      SUB SP, #4                                ;allocate space for cnt
	  MOV R1, #0
	  STR R1, [SP, #cnt]                        ;cnt = 0
	  MOV R11, SP                               ;set R11 as stack frame
	  MOV R10, #1000                            ;R10 is our initial divisor 
	  MOV R9, #10                               ;R9 is our divisor for the divisor
	  LDR R8, =FixedPt                          ;R8 is string pointer
OFloop
      LDR  R4, [R11, #cnt]
	  UDIV R3, R0, R10
	  ADD  R3, #0x30                            ;make R3 an ASCII character
	  STR  R3, [R8, R4]                         ;store R0/10 as an ASCII character in FixedPt[cnt]
	  SUB  R3, #0x30                            ;return R3 to its numerical value
	  MUL  R1, R3, R10
	  SUB  R0, R0, R1                           ;R0 = R0%10
	  ADD  R4, #1
	  UDIV R10, R10, R9                         ;divisor/10 (ex: 1000/10 = 100)
	  STR  R4, [R11, #cnt]
	  CMP  R4, #1
	  BNE  skip                                 ;if cnt is one, we need a decimal point
	  MOV  R1, #0x2E                            ;0x2E = *
	  STR  R1, [R8, R4]
	  ADD  R4, #1
	  STR  R4, [R11, #cnt]
skip  CMP  R4, #5
	  BNE  OFloop                               ;if we have 5 characters, move on
OFout
      MOV  R1, #0x00 
	  STR  R1, [R8, R4]                         ;null terminate string
      LDR  R0, =FixedPt
	  BL  ST7735_OutString                      ;output string to the display
	    
	  ADD SP, #4                                ;deallocate
	  POP{R4,R8,R9,R10,R11,LR}                  ;restore

nope  BX   LR
 
     ALIGN
;* * * * * * * * End of LCD_OutFix * * * * * * * *

     ALIGN                           ; make sure the end of this section is aligned
     END                             ; end of file
