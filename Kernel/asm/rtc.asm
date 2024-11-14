GLOBAL timeParam

section .text

timeParam:                  ;;en rax se tendria que poner el parámetro del tiempo que quiero
    push rbp
    mov rbp, rsp

    mov rax, rdi
    out 70h, al
    xor rax, rax
    in al, 71h

    mov rsp, rbp
    pop rbp
    ret

section .note.GNU-stack
