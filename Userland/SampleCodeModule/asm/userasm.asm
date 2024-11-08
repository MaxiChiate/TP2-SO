GLOBAL _int80
GLOBAL invalidOpcode
GLOBAL _hlt

extern strLength

section .text
	

_int80:

    int 0x80
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

    