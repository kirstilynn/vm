a		.BYT	'a'
b		.BYT	'b'
c		.BYT	'c'
d		.BYT	'd'
e		.BYT	'e'
f		.BYT	'f'
g		.BYT	'g'
h		.BYT	'h'
i		.BYT	'i'
j		.BYT	'j'
k		.BYT	'k'
l		.BYT	'l'
m		.BYT	'm'
n		.BYT	'n'
o		.BYT	'o'
p		.BYT	'p'
q		.BYT	'q'
r		.BYT	'r'
s		.BYT	's'
t		.BYT	't'
u		.BYT	'u'
v		.BYT	'v'
w		.BYT	'w'
x		.BYT	'x'
y		.BYT	'y'
z		.BYT	'z'
F		.BYT	'F'
prompt 	.BYT	'>'
NEWLINE	.BYT	'\n' ;28 
SPACE 	.BYT 	' '

ARR		.INT 	0
		.INT 	0
		.INT 	0
		.INT 	0
		.INT 	0
		.INT 	0
		.INT 	0
		.INT 	0
		.INT 	0
		.INT 	0
		.INT 	0
		.INT 	0
		.INT 	0
		.INT 	0
		.INT 	0
		.INT 	0
		.INT 	0
		.INT 	0
		.INT 	0
		.INT 	0
		.INT 	0
		.INT 	0
		.INT 	0
		.INT 	0
		.INT 	0
		.INT 	0
		.INT 	0
		.INT 	0
		.INT 	0
		.INT 	0
COUNT	.INT 	0
TYPE 	.INT 	4
SIZE 	.INT 	30

TARR	.INT 	0
		.INT 	0
		.INT 	0
		.INT 	0
		.INT 	0
		.INT 	0
		.INT 	0
		.INT 	0

TSIZE 	.INT 	8
TINDEX 	.INT 	0

TCOUNT 	.INT 	4
LARR	.INT 	-1 		;lock for ARR
FLOCK 	.INT 	-1 		;lock for factorial function
input 		.INT 	0
val 		.INT 	0



;PART 2
WHILE2	LDB R0 	prompt
		TRP 3
		TRP 2
		STR R0	input
		SUB	R1	R1	
		CMP R0	R1
		BRZ	R0	EWHILE2
		;save input into arr[count]
		LDA R1	ARR
		LDR	R2	TYPE
		LDR R3 	COUNT
		MUL R2	R3 				;CALCULATE OFFSET(INDEX * ARRAY TYPE)
		ADD R1	R2				;ADD TO BASE ADDR 
		LDR	R0	input
		STR R0	R1				;ARR[COUNT] = input
		ADI R3	1
		STR R3	COUNT 			;COUNT++

		;get factorial of input
; Test for overflow (SP < SL)
		MOV	R5	SP 		; move SP -> R5
		ADI	R5	-8			; Adjust for space needed (RA + PFP)
		MOV R0 	R5
		CMP	R5	SL			; 0 (SP=SL), Pos (SP > SL), Neg (SP < SL)
		BLT	R5	OFLW

; Create Activation Record and invoke function FACTORIAL

		MOV R3	FP			; Save FP in R3, this will be the PFP
		MOV FP	SP			; Point at Current Activation Record 
		ADI	SP	-4			; Adjust Stack Pointer for Return Addres
		STR	R3 	SP			; PFP to Top of Stack
		ADI SP	-4 			; Adjust Stack Pointer for PFP

; Passed Parameters onto the Stack (Pass by Value)
		LDR R5	input
		STR R5 	SP
		ADI SP 	-4 			; Place input on the Stack

		MOV R1 R8			; PC incremented by 1 instruction
		ADI R1 48 			; Compute Return Address (always a fixed amount)
		STR R1 FP 			; Return Address to the Beginning of the Frame
		JMP	FACTORIAL 		; Call Function FACTORIAL
; New Code to return value of function FACTORIAL
		LDR R5	SP 			; Instruction where we return – Access return value 
		STR R5	val
		;save factorial into arr[count]
		LDA R1	ARR
		SUB R2	R2
		ADI R2 	4
		LDR R3 	COUNT
		MUL R2	R3 				;CALCULATE OFFSET(INDEX * ARRAY TYPE)
		ADD R1	R2				;ADD TO BASE ADDR 
		STR R5	R1				;ARR[COUNT] = val
		ADI R3	1
		STR R3	COUNT 			;COUNT++

		LDB	R0 	F
		TRP 3
		LDB	R0 	a
		TRP 3
		LDB	R0 	c
		TRP 3
		LDB	R0 	t
		TRP 3
		LDB	R0 	o
		TRP 3
		LDB	R0 	r
		TRP 3
		LDB	R0 	i
		TRP 3
		LDB	R0 	a
		TRP 3
		LDB	R0 	l
		TRP 3

		LDB R0 SPACE
		TRP 3

		LDB	R0 	o
		TRP 3
		LDB	R0 	f
		TRP 3

		LDB R0 SPACE
		TRP 3

		LDR	R0	input
		TRP 1

		LDB R0 SPACE
		TRP 3

		LDB	R0 	i
		TRP 3
		LDB	R0 	s
		TRP 3

		LDB R0 SPACE
		TRP 3

		MOV	R0	R5	
		TRP 1
		LDB R0 NEWLINE
		TRP 3


		JMP WHILE2



EWHILE2		LDA	R7	ARR			;location of base address
			LDR R6 	COUNT
			ADI R6	-1 			;location of back, start at COUNT - 1
			SUB	R5	R5			;location of front, start at 0	
			LDR	R4	TYPE		;location of array type
WHILE 		MOV R2	R5			;move front over
			CMP	R2	R6			;compare front to back
			BGT	R2	ENDWHILE	
			;front
			MOV	R2	R5 			;MOVE INDEX OVER
			MUL R2	R4 			;CALCULATE OFFSET(INDEX * ARRAY TYPE)
			ADD R2	R7 			;ADD TO BASE ADDR 

			LDR	R1 	R2			;load the value of the address stored in R2
			MOV R0	R1
			TRP 1
			LDB	R0	SPACE
			TRP	3
			ADI	R5	1
			;back
			MOV	R2	R6 			;MOVE INDEX OVER
			MUL R2	R4 			;CALCULATE OFFSET(INDEX * ARRAY TYPE)
			ADD R2	R7 			;ADD TO BASE ADDR 

			LDR	R1 	R2			;load the value of the address stored in R2
			MOV R0	R1
			TRP 1
			LDB	R0	SPACE
			TRP	3
			ADI	R6	-1
			JMP	WHILE






	
;part3

ENDWHILE	LDB	R0	NEWLINE
			TRP 3
			LCK FLOCK
			LDR R5  TCOUNT
			SUB R4	R4
FOR			MOV R1 	R4
			CMP	R1	R5			;compare count to TCOUNT
			BRZ	R1	EFOR
			LDB R0 	prompt
			TRP 3	
			TRP 2
			MOV R6	R0
			RUN R7	THREAD
			ADI	R4	1
			JMP	FOR

EFOR		ULK	FLOCK
			BLK

			LDA	R7	TARR			;location of base address
			LDR R6 	TSIZE
			ADI R6	-1 			;location of back, start at TSIZE - 1
			SUB	R5	R5			;location of front, start at 0	
			LDR	R4	TYPE		;location of array type
WHILE3 		MOV R2	R5			;move front over
			CMP	R2	R6			;compare front to back
			BGT	R2	ENDWHILE3	
			;front
			MOV	R2	R5 			;MOVE INDEX OVER
			MUL R2	R4 			;CALCULATE OFFSET(INDEX * ARRAY TYPE)
			ADD R2	R7 			;ADD TO BASE ADDR 

			LDR	R1 	R2			;load the value of the address stored in R2
			MOV R0	R1
			TRP 1
			LDB	R0	SPACE
			TRP	3
			ADI	R5	1

			;back
			MOV	R2	R6 			;MOVE INDEX OVER
			MUL R2	R4 			;CALCULATE OFFSET(INDEX * ARRAY TYPE)
			ADD R2	R7 			;ADD TO BASE ADDR 

			LDR	R1 	R2			;load the value of the address stored in R2
			MOV R0	R1
			TRP 1
			LDB	R0	SPACE
			TRP	3
			ADI	R6	-1
			;MOV R0 	R5
			;TRP 1
			JMP	WHILE3


ENDWHILE3	TRP 0






THREAD 		LCK FLOCK
			ULK FLOCK
			LCK LARR
			LDA R1	TARR
			LDR	R2	TYPE
			LDR R3 	TINDEX
			MUL R2	R3 				;CALCULATE OFFSET(INDEX * ARRAY TYPE)
			ADD R1	R2				;ADD TO BASE ADDR 
			STR R6	R1				;ARR[COUNT] = input
			ADI R3	1
			STR R3 	TINDEX
			ULK LARR

	;get factorial of input
	; Test for overflow (SP < SL)
			MOV	R5	SP 		; move SP -> R5
			ADI	R5	-8			; Adjust for space needed (RA + PFP)
			MOV R0 	R5
			CMP	R5	SL			; 0 (SP=SL), Pos (SP > SL), Neg (SP < SL)
			BLT	R5	OFLW

	; Create Activation Record and invoke function FACTORIAL

			MOV R3	FP			; Save FP in R3, this will be the PFP
			MOV FP	SP			; Point at Current Activation Record 
			ADI	SP	-4			; Adjust Stack Pointer for Return Addres
			STR	R3 	SP			; PFP to Top of Stack
			ADI SP	-4 			; Adjust Stack Pointer for PFP

	; Passed Parameters onto the Stack (Pass by Value)
			;LDR R5	input
			STR R6 	SP
			ADI SP 	-4 			; Place input on the Stack

			MOV R1 R8			; PC incremented by 1 instruction
			ADI R1 48 			; Compute Return Address (always a fixed amount)
			STR R1 FP 			; Return Address to the Beginning of the Frame
			JMP	FACTORIAL 		; Call Function FACTORIAL
	; New Code to return value of function FACTORIAL
			LDR R5	SP 			; Instruction where we return – Access return value 
			LCK LARR
			LDA R1	TARR
			LDR	R2	TYPE
			LDR R3 	TINDEX
			MUL R2	R3 				;CALCULATE OFFSET(INDEX * ARRAY TYPE)
			ADD R1	R2				;ADD TO BASE ADDR 
			STR R5	R1				;ARR[COUNT] = input
			ADI R3	1
			STR R3 	TINDEX
			ULK LARR
			END







;int factorial(int number)
;R6 = number
;R5 = factorial return
;R4 = temp;
; Allocate Space for Local and two Temporary Variables
FACTORIAL	ADI SP -8 ; Space for Local temp, Local return and one temp variable
;  Access Data on the Stack
			SUB R1	R1
			ADI R1 	8
			MOV R0	FP
			SUB R0 	R1
	 		LDR R6	R0 			; Access number
	 		SUB R0	R0
	 		ADI R0	1
	 		CMP R0 	R6
;	 		TRP 1
	 		BLT	R0	SKIPIF
;if number <= 1
	 		SUB R4	R4
	 		ADI R4 	1
			SUB R1	R1
			ADI R1 	12
			MOV R0	FP
			SUB R0 	R1
	 		STR R4	R0 			; temp = 1

; New Code for return of 1
			LDR R9 	R0 			; Save the value of 1 in R9
; Test for underflow (SP > SB)
			MOV SP	FP			; De-allocate Current Activation Record (SP = FP)
			MOV R5 	SP
			CMP R5	SB 		; 0 (SP=SB), Pos (SP > SB), Neg (SP < SB)
			BGT R5	UFLW
; Set Previous Frame to Current Frame and Return
			LDR R5 	FP 		; Return Address Pointed to by FP 
			MOV R4	FP
			ADI	R4	-4			; (FP - 4)
			LDR FP	R4  		; Point at Previous Activation Record (FP = PFP) 
; New Code for return of 1
			STR R9	SP 			; Store the value of 1 on the top of the Stack
			JMR R5 				; Jump to Address in Register R5

; Test for overflow (SP < SL)
SKIPIF		MOV	R5	SP 		; move SP -> R5
			ADI	R5	-8			; Adjust for space needed (RA + PFP)
			MOV R0 	R5
			CMP	R5	SL			; 0 (SP=SL), Pos (SP > SL), Neg (SP < SL)
			BLT	R5	OFLW
; Create Activation Record and invoke function FACTORIAL

			MOV R3	FP			; Save FP in R3, this will be the PFP
			MOV FP	SP			; Point at Current Activation Record 
			ADI	SP	-4			; Adjust Stack Pointer for Return Addres
			STR	R3 	SP			; PFP to Top of Stack
			ADI SP	-4 			; Adjust Stack Pointer for PFP
; Passed Parameters onto the Stack (Pass by Value)
			MOV R5 	R6
			ADI R5 	-1
			STR R5 	SP
			ADI SP 	-4 			; Place number-1 on the Stack

			MOV R1 R8			; PC incremented by 1 instruction
			ADI R1 48 			; Compute Return Address (always a fixed amount)
			STR R1 FP 			; Return Address to the Beginning of the Frame
			JMP	FACTORIAL 		; Call Function FACTORIAL
; New Code to return value of function FACTORIAL
			SUB R1	R1
			ADI R1 	8
			MOV R0	FP
			SUB R0 	R1
	 		LDR R6	R0 			; Access number

			LDR R5	SP 			; Instruction where we return – Access return value 
			MOV R4 	R5			
			MUL R4 	R6			

			SUB R1	R1
			ADI R1 	12
			MOV R0	FP
			SUB R0 	R1
			STR R4	R0 			; temp = number * factorial(number - 1);

; New Code for return of temp
			LDR R9 	R0 			; Save the value of temp in R9
; Test for underflow (SP > SB)
			MOV SP	FP			; De-allocate Current Activation Record (SP = FP)
			MOV R5 	SP
			CMP R5	SB 		; 0 (SP=SB), Pos (SP > SB), Neg (SP < SB)
			BGT R5	UFLW
; Set Previous Frame to Current Frame and Return
			LDR R5 	FP 		; Return Address Pointed to by FP 
			MOV R4	FP
			ADI	R4	-4			; (FP - 4)
			LDR FP	R4  		; Point at Previous Activation Record (FP = PFP) 
; New Code for return of temp
			STR R9	SP 			; Store the value of temp on the top of the Stack
			JMR R5 				; Jump to Address in Register R5


OFLW 	LDB	R0 	o
		TRP 3
		LDB	R0 	v
		TRP 3
		LDB	R0 	e
		TRP 3
		LDB	R0 	r
		TRP 3
		LDB	R0 	f
		TRP 3
		LDB	R0 	l
		TRP 3
		LDB	R0 	o
		TRP 3
		LDB	R0 	w
		TRP 3
		LDB	R0 	NEWLINE
		TRP 3
		TRP 0


UFLW 	LDB	R0 	u
		TRP 3
		LDB	R0 	n
		TRP 3
		LDB	R0 	d
		TRP 3
		LDB	R0 	e
		TRP 3
		LDB	R0 	r
		TRP 3
		LDB	R0 	f
		TRP 3
		LDB	R0 	l
		TRP 3
		LDB	R0 	o
		TRP 3
		LDB	R0 	w
		TRP 3
		LDB	R0 	NEWLINE
		TRP 3
		TRP 0 