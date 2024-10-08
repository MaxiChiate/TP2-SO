GLOBAL print
GLOBAL draw
GLOBAL clear
GLOBAL getChar
GLOBAL getAndPrintChar
GLOBAL killBuffer
GLOBAL putChar
GLOBAL exit_shell
GLOBAL time_getter
GLOBAL make_text_smaller
GLOBAL make_text_bigger
GLOBAL rand
GLOBAL recolor
GLOBAL play_song
GLOBAL invalidOpcode

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

make_text_bigger:
    push rbp
    mov rbp, rsp

    push rax
    mov rax, 0xa1
    int 80h
    pop rax

    mov rsp, rbp
    pop rbp
    ret


make_text_smaller:
    push rbp
    mov rbp, rsp

    push rax
    
    mov rax, 0xa0
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


draw:

    push rbp
    mov rbp, rsp 

    push rbx
    push rdi
    push rdx
    push rsi

    mov rbx, rdi    ; 1er arg en C
    mov rdi, rdx    ; 3er arg
    mov rdx, rsi    ; 2do arg
    mov rsi, rcx    ; 4to arg

    mov rax, 0x44
    int 0x80

    pop rbx
    pop rdi
    pop rdx
    pop rsi

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

recolor:

    push rbp
    mov rbp, rsp

    push rbx

    mov rbx, rdi    ; 1er arg en C

    mov rax, 0xa2
    int 0x80

    pop rbx

    mov rsp, rbp
    pop rbp
    ret
play_song:
    push rbp
    mov rbp, rsp

    push rbx
    push rax

    mov rax, 0xDE
    mov rbx, rdi    ;el numero que responde a que canción llamo
    int 0x80

    pop rax
    pop rbx

    mov rsp, rbp
    pop rbp
    ret


invalidOpcode:

    ud2 ; Instruccion diseñada para disparar invalid opcode

; Otra forma seria:
    ;db 0xFF

    ret
