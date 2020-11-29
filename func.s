global _start
section .data
_msg1 db "Foram lidos "
_msg1_s EQU $-_msg1
_msg2 db " caracteres.", 0dh, 0ah
_msg2_s EQU $-_msg2

section .bss
TMP_DATA resd 1
_bufferData resb 5
_result resd 1

section .text
_start:
;------------------------------------ INPUT
push TMP_DATA
call LeerInteiro
mov [_result], eax
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
;------------------------------------
mov eax, 1
mov ebx, 0
int 80h

;-------------------------------------
LeerInteiro:
enter 0,0
push ecx
push edx
push ebx
push esi
mov eax, 3
mov ebx, 2
mov ecx, _bufferData
mov edx, 5
int 80h
mov eax, 0
mov ecx, 4
mov esi, 0
mov bl, [_bufferData]
cmp bl, 0x2d
je _negative
_positive:
cmp bl, 0x0a
je _endLeer
inc eax
inc esi
mov bl, [_bufferData+esi]
loop _positive
_negative:
inc esi
mov bl, [_bufferData+esi]
jmp _positive
_endLeer:
push eax
mov eax, DWORD [_bufferData]
mov ebx, [EBP + 8]
mov [ebx], eax
pop eax
add eax, 0x30
push ecx
push edx
push ebx
push esi
leave
ret 4
;-------------------------------------

_output:
enter 0,0
mov eax, 4
mov ebx, 1
mov ecx, TMP_DATA
mov edx, 5
int 80h
leave
ret