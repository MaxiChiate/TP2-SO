GLOBAL spawn_process
GLOBAL ps
GLOBAL kill
GLOBAL current_pid
GLOBAL nice
GLOBAL blockp
GLOBAL unblockp
GLOBAL giveup_cpu
GLOBAL waitp
GLOBAL waitpid

section .text

spawn_process:  

    push rbp
    mov  rbp, rsp

    push rbx

    push r9
    push r8

    mov rbx, rdi 
    mov r8, rsi
    ;mov rdx, rdx
    mov r9, rcx

    pop rdi ; rdi <- r8
    pop rsi ; rsi <- r9
    
    mov rax, 0xB1
    
    int 0x80

    pop rbx

    mov rsp, rbp
    pop rbp
    ret
ps:
    
    push rbp
    mov  rbp, rsp
    
    mov rax, 0xB0
    
    int 0x80

    mov rsp, rbp
    pop rbp
    ret
kill:

    push rbp
    mov  rbp, rsp

    push rbx

    mov rbx, rdi 

    mov rax, 0xB1
    
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
    int 0x80

    pop rbx

    mov rsp, rbp
    pop rbp
    ret