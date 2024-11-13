GLOBAL enter_region
GLOBAL leave_region

.text:

enter_region:

    mov al, 0x01           
    XCHG al, [rdi]         
    ret

leave_region:

    mov byte [rdi], 0x00   
    ret
