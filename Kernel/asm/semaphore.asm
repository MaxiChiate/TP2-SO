GLOBAL enter_region
GLOBAL leave_region

.text:

 enter_region:
 
    mov rax, 0x01
    XCHG rax, [rdi]
    ret
 

 leave_region:

   mov byte [rdi], 0x00
   ret
 