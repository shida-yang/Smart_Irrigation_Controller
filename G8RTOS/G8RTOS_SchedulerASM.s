; G8RTOS_SchedulerASM.s
; Holds all ASM functions needed for the scheduler
; Note: If you have an h file, do not have a C file and an S file of the same name

	; Functions Defined
	.def G8RTOS_Start, PendSV_Handler

	; Dependencies
	.ref CurrentlyRunningThread, G8RTOS_Scheduler

	.thumb		; Set to thumb mode
	.align 2	; Align by 2 bytes (thumb mode uses allignment by 2 or 4)
	.text		; Text section

; Need to have the address defined in file 
; (label needs to be close enough to asm code to be reached with PC relative addressing)
RunningPtr: .field CurrentlyRunningThread, 32

; G8RTOS_Start
;	Sets the first thread to be the currently running thread
;	Starts the currently running thread by setting Link Register to tcb's Program Counter
G8RTOS_Start:

	.asmfunc
	; Implement this

	ldr R4, RunningPtr
	ldr R4, [R4]
	ldr R5, [R4, #8]	;R5=new SP value
	msr msp, R5			;update SP
	;retrieve reg
	pop {R0-R12, R14}
	pop {R14}
	pop {EPSR}

	CPSIE I


	bx lr

	.endasmfunc

; PendSV_Handler
; - Performs a context switch in G8RTOS
; 	- Saves remaining registers into thread stack
;	- Saves current stack pointer to tcb
;	- Calls G8RTOS_Scheduler to get new tcb
;	- Set stack pointer to new stack pointer from new tcb
;	- Pops registers from thread stack
PendSV_Handler:
	
	.asmfunc
	;Implement this

	;save remaining reg into stack
	push {R4-R11}
	;save current SP to tcb
	mrs R5, msp
	ldr R4, RunningPtr
	ldr R4, [R4]
	str R5, [R4, #8]
	push {LR}
	;call scheduler to update current TCB
	bl G8RTOS_Scheduler
	pop {LR}
	;new TCB address now in CurrentlyRunningThread
	ldr R4, RunningPtr
	ldr R4, [R4]
	ldr R5, [R4, #8]	;R5=new SP value
	msr msp, R5			;update SP
	;retrieve reg
	pop {R4-R11}

	bx lr

	.endasmfunc
	
	; end of the asm file
	.align
	.end
