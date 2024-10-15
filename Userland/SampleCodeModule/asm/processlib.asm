GLOBAL spawn_process
; GLOBAL ps
GLOBAL kill
GLOBAL current_pid
GLOBAL nice
GLOBAL blockp
GLOBAL unblockp
GLOBAL giveup_cpu
GLOBAL waitp
GLOBAL waitpid
GLOBAL suicide 

section .text

spawn_process:  

    push rbp
    mov  rbp, rsp

    push r8

    mov r8, rdi
    mov rdx, rsi
    mov r9, rdx

    mov rdi, rcx
    pop rsi ; rsi <- r8
    
    mov rax, 0xB1
    
    int 0x80

    mov rsp, rbp
    pop rbp
    ret
; ps:
    
;     push rbp
;     mov  rbp, rsp
    
;     mov rax, 0xB0
    
;     int 0x80

;     mov rsp, rbp
;     pop rbp
;     ret
kill:

    push rbp
    mov  rbp, rsp

    push rbx

    mov rax, 0xB3
    mov rbx, rdi 

    int 0x80

    pop rbx

    mov rsp, rbp
    pop rbp
    ret
current_pid:
    
    push rbp
    mov  rbp, rsp

    mov rax, 0xB4
    
    int 0x80

    mov rsp, rbp
    pop rbp
    ret

nice:
    
    push rbp
    mov  rbp, rsp

    push rbx
    push rdx

    mov rbx, rdi 
    mov rdx, rsi
    
    mov rax, 0xB5
    
    int 0x80

    pop rdx
    pop rbx

    mov rsp, rbp
    pop rbp
    ret
blockp:
    
    push rbp
    mov  rbp, rsp

    push rbx

    mov rax, 0xB6
    mov rbx, rdi
    
    int 0x80

    pop rbx

    mov rsp, rbp
    pop rbp
    ret
unblockp:
    
    push rbp
    mov  rbp, rsp

    push rbx

    mov rax, 0xB7
    mov rbx, rdi    
    
    int 0x80

    pop rbx

    mov rsp, rbp
    pop rbp
    ret
giveup_cpu:
    
    push rbp
    mov  rbp, rsp

    mov rax, 0xB8    
    int 0x80

    mov rsp, rbp
    pop rbp
    ret
waitp:
    
    push rbp
    mov  rbp, rsp

    mov rax, 0xB9
    int 0x80

    mov rsp, rbp
    pop rbp
    ret
waitpid:
    
    push rbp
    mov  rbp, rsp

    push rbx
    
    mov rax, 0xBA
    mov rbx, rdi    
    int 0x80

    pop rbx

    mov rsp, rbp
    pop rbp
    ret

suicide:
    
    push rbp
    mov  rbp, rsp

    mov rax, 0xBB
    mov rbx, rdi    
    int 0x80

    mov rsp, rbp
    pop rbp
    ret
    