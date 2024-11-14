GLOBAL _int80
GLOBAL invalidOpcode
GLOBAL _hlt

extern strLength

section .text
	

_int80:

    int 0x80
    ret

section .note.GNU-stack
