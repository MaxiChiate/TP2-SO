
; Scrpit para probar el uso de la interrupcion 0x80
; usando la instruccion int 0x80 ya estoy llamando a la syscall
GLOBAL script

section .text

script:
    push rbp
	mov rbp, rsp

    push rax
    push rbx
    push rcx

    mov rax, 0x04
    mov rbx, 0x01
    mov rcx, string

    int 0x80

    xor rcx, rcx
.continue:
    pop rcx
    pop rbx
    pop rax

    mov rsp, rbp
    pop rbp
    ret

section .data

string db 'I am a system call!', 0