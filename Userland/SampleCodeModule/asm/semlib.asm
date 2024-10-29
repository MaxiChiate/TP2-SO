GLOBAL new_sem
GLOBAL is_sem_alive
GLOBAL kill_sem
GLOBAL up
GLOBAL down

section .text


new_sem:

    push rbp
    mov  rbp, rsp

    mov rax, 0xC0
    mov rbx, rdi
    
    int 0x80

    mov rsp, rbp
    pop rbp

    ret

is_sem_alive:

    push rbp
    mov  rbp, rsp

    mov rax, 0xC1
    mov rbx, rdi
    
    int 0x80

    mov rsp, rbp
    pop rbp

    ret

kill_sem:

    push rbp
    mov  rbp, rsp

    mov rax, 0xC2
    mov rbx, rdi
    
    int 0x80

    mov rsp, rbp
    pop rbp

    ret

up:

    push rbp
    mov  rbp, rsp

    mov rax, 0xC3
    mov rbx, rdi
    
    int 0x80

    mov rsp, rbp
    pop rbp

    ret

down:

    push rbp
    mov  rbp, rsp

    mov rax, 0xC4
    mov rbx, rdi
    
    int 0x80

    mov rsp, rbp
    pop rbp

    ret