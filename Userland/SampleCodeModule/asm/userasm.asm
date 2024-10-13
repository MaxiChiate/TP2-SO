GLOBAL print
GLOBAL clear
GLOBAL getChar
GLOBAL getAndPrintChar
GLOBAL killBuffer
GLOBAL putChar
GLOBAL exit_shell
GLOBAL time_getter
GLOBAL rand
GLOBAL invalidOpcode
GLOBAL _hlt

extern strLength

section .text
	
; void print(unsigned * char string)
print:
	push rbp
	mov rbp, rsp
    
    push rax
    push rbx
    push rcx
    push rdx

    call strLength  ; rdi = @string
    mov rdx, rax

    mov rax, 0x03   ; sysRead:
    mov rbx, 0x02   ;   Read String
    mov rcx, rdi    ;   @string
                    ;
    int 0x80        ; 

    mov rax, 0x04   ; sysWrite:
    mov rbx, 0x01   ;
                    ; rdx = @count
    int 0x80        ;

    pop rdx
    pop rcx
    pop rbx
    pop rax

	mov rsp, rbp
	pop rbp
	ret

; char getChar()
getChar:

    push rbp
    mov rbp, rsp

    push rbx

    mov rax, 0x04
    mov rbx, 0xA0
    int 0x80
    
    pop rbx

    mov rsp, rbp
    pop rbp
    
    ret

killBuffer:

    push rbp
	mov rbp, rsp

    push rax
    mov rax, 0x99
    int 0x80
    pop rax

    mov rsp, rbp
    pop rbp
    ret

getAndPrintChar:

    push rbp
	mov rbp, rsp

    push rbx

    mov rax, 0x04
    mov rbx, 0xA1
    int 0x80

    pop rbx

    mov rsp, rbp
    pop rbp
    ret

putChar:

    push rbp
	mov rbp, rsp

    push rax
    push rbx
    push rsi

    mov rax, 0x03   ; sysRead:
    mov rbx, 0x01   ;   read Char
    mov rsi, rdi    ;   @char
                    ;
    int 0x80  

    mov rax, 0x04   ; sysWrite:
    mov rbx, 0x02   ;
                    ;
    int 0x80        ;

    pop rsi 
    pop rbx
    pop rax

    mov rsp, rbp
    pop rbp
    ret

exit_shell:
    push rbp
    mov rbp, rsp

    mov rax, 0x01
    int 80h

    mov rsp, rbp
    pop rbp
    ret


time_getter:
    push rbp
    mov rbp, rsp

    push rax
    mov rax, 0x98
    int 80h
    pop rax

    mov rsp, rbp
    pop rbp
    ret


rand:

    push rbp
    mov rbp, rsp

    mov rax, 0x12
    int 0x80

    mov rsp, rbp
    pop rbp
    ret

clear:

    push rbp
    mov rbp, rsp

    push rax

    mov rax, 0x45
    int 0x80

    pop rax

    mov rsp, rbp
    pop rbp
    ret


invalidOpcode:

    ud2 ; Instruccion diseñada para disparar invalid opcode

; Otra forma seria:
    ;db 0xFF

    ret

_hlt:
	sti
	hlt
	ret