global _start
section .data
_msg1 db "Foram lidos "
_msg1_s EQU $-_msg1
_msg2 db " caracteres.", 0dh, 0ah
_msg2_s EQU $-_msg2

section .bss
TMP_DATA resd 1
_bufferData resb 5
_aux resb 5
_bufferString resb 100
_result resd 1

section .text
_start:
push eax ; S_INPUT
push TMP_DATA
push DWORD 8
call LeerString
mov [_result], eax
call _displayNchars
pop eax

push eax ; S_OUTPUT
push TMP_DATA
push DWORD 8
call EscreverString
pop eax

;------------------------------------
mov eax, 1
mov ebx, 0
int 80h

_displayNchars:
enter 0,0
mov eax, 4
mov ebx, 1
mov ecx, _msg1
mov edx, _msg1_s
int 80h
mov eax, 4
mov ebx, 1
mov ecx, _result
mov edx, 4
int 80h
mov eax, 4
mov ebx, 1
mov ecx, _msg2
mov edx, _msg2_s
int 80h
leave
ret