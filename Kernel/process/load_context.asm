SECTION .text

GLOBAL loadContext 

loadContext:

 
    mov ss, rdi
    mov rsp, rsi
    sub rsp, 8      //align 
    mov rflags, rdx
    mov cs, rcx
    mov rip, r8
ret 
