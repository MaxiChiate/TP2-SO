GLOBAL _cli
GLOBAL _sti
GLOBAL picMasterMask
GLOBAL picSlaveMask
GLOBAL haltcpu
GLOBAL _hlt
GLOBAL _irq00Handler
GLOBAL _irq01Handler
GLOBAL _irq02Handler
GLOBAL _irq03Handler
GLOBAL _irq04Handler
GLOBAL _irq05Handler
GLOBAL _irq60Handler

GLOBAL _exception0Handler
GLOBAL _exception6Handler

EXTERN irqDispatcher
EXTERN int_80
EXTERN exceptionDispatcher
EXTERN getStackBase

SECTION .text

%macro pushState 0
	push rax
	push rbx
	push rcx
	push rdx
	push rbp
	push rdi
	push rsi
	push r8
	push r9
	push r10
	push r11
	push r12
	push r13
	push r14
	push r15
%endmacro



%macro popState 0
	pop r15
	pop r14
	pop r13
	pop r12
	pop r11
	pop r10
	pop r9
	pop r8
	pop rsi
	pop rdi
	pop rbp
	pop rdx
	pop rcx
	pop rbx
	pop rax
%endmacro


%macro irqHandlerMaster 1
	pushState

	mov rdi, %1 ; pasaje de parametro
	
	call irqDispatcher
	
	; signal pic EOI (End of Interrupt)
	mov al, 20h
	out 20h, al

	popState
	iretq
%endmacro



%macro exceptionHandler 1

	mov rdi, %1 ; pasaje de parametro

	call exceptionDispatcher

	call getStackBase
    mov [rsp+24], rax

	mov byte [rsp+8], 0x08
	mov byte [rsp+32], 0x00
	mov byte [rsp+16], 0x202	

    mov rax, userland
    mov [rsp], rax
    iretq

%endmacro


_hlt:
	sti
	hlt
	ret

_cli:
	cli
	ret


_sti:
	sti
	ret

picMasterMask:
	push rbp
    mov rbp, rsp
    mov ax, di
    out	21h,al
    pop rbp
    retn

picSlaveMask:
	push    rbp
    mov     rbp, rsp
    mov     ax, di  ; ax = mascara de 16 bits
    out	0A1h,al
    pop     rbp
    retn


;8254 Timer (Timer Tick)
_irq00Handler:
	irqHandlerMaster 0		; acá ya llama al contador sumando ticks

;Keyboard
_irq01Handler:
	irqHandlerMaster 1

;Cascade pic never called
_irq02Handler:
	irqHandlerMaster 2

;Serial Port 2 and 4
_irq03Handler:
	irqHandlerMaster 3

;Serial Port 1 and 3
_irq04Handler:
	irqHandlerMaster 4

;USB
_irq05Handler:
	irqHandlerMaster 5

;syscalls
_irq60Handler: 
		; push rax ;<- valor de retorno
	push rbx
	push rcx
	push rdx
	push rbp
	push rdi
	push rsi
	push r8
	push r9
	push r10
	push r11
	push r12
	push r13
	push r14
	push r15

; Argumentos de las syscalls:
	push rax
	push rbx
	push rcx
	push rdx
	push rsi
	push rdi

; Argumentos por convencion de C en 64 bits:	
	pop r9
	pop r8
	pop rcx
	pop rdx
	pop rsi
	pop rdi
	call int_80
	
	push rax
	; signal pic EOI (End of Interrupt)
	mov al, 20h
	out 20h, al

	pop rax

	pop r15
	pop r14
	pop r13
	pop r12
	pop r11
	pop r10
	pop r9
	pop r8
	pop rsi
	pop rdi
	pop rbp
	pop rdx
	pop rcx
	pop rbx
	; pop rax
	iretq
	


haltcpu:
	cli
	hlt
	ret


;Zero Division Exception
_exception0Handler:
    exceptionHandler 0


;Invalid Opcode Exception
_exception6Handler:
    exceptionHandler 6


SECTION .rodata
    userland equ 0x400000
