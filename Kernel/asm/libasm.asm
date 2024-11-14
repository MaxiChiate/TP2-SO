GLOBAL cpuVendor
GLOBAL inb
GLOBAL outb

section .text

cpuVendor:
	push rbp
	mov rbp, rsp

	push rbx

	mov rax, 0
	cpuid


	mov [rdi], ebx
	mov [rdi + 4], edx
	mov [rdi + 8], ecx

	mov byte [rdi+13], 0

	mov rax, rdi

	pop rbx

	mov rsp, rbp
	pop rbp
	ret

inb:				; Funciones para el funcionamiento del speaker
	push rbp
	mov rbp, rsp

    mov rdx,rdi
    in al,dx		; pasaje en 8 bits

	mov rsp, rbp
	pop rbp
	ret

outb:
	push rbp
	mov rbp, rsp

    mov rax, rsi    
    mov rdx, rdi
	out dx, al		; pasaje en 8 bits

	mov rsp, rbp
	pop rbp
	ret


section .note.GNU-stack
