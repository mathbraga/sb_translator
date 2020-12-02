global _start
section .data
_msg1 db "Foram lidos "
_msg1_s EQU $-_msg1
_msg2 db " caracteres.", 0dh, 0ah
_msg2_s EQU $-_msg2

section .bss
_bufferData resb 5
_aux resb 5
_result resd 1

section .data
L1 dd 3
section .bss
L2 resd 1
section .text
_start:
mov eax, -10
cdq
mov ebx, -2
idiv ebx
_here:

;------------------------------------
mov eax, 1
mov ebx, 0
int 80h