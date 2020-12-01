global _start
section .data
_msg1 db "Foram lidos "
_msg1_s EQU $-_msg1
_msg2 db " caracteres.", 0dh, 0ah
_msg2_s EQU $-_msg2

section .bss
TMP_DATA resd 2
_bufferData resb 5
_aux resb 5
_result resd 1

section .text
_start:
push eax ; C_INPUT
push TMP_DATA
call LeerChar
mov [_result], eax
call _displayNchars
pop eax

push eax ; C_OUTPUT
push DWORD [TMP_DATA]
call EscreverChar
pop eax

;------------------------------------
mov eax, 1
mov ebx, 0
int 80h

LeerChar:
enter 0,0
mov DWORD [_bufferData], 0
mov BYTE [_bufferData + 4], 0
push ecx
push edx
push ebx
mov eax, 3
mov ebx, 2
mov ecx, _bufferData
mov edx, 2
int 80h
mov ebx, [EBP + 8]
mov al, [_bufferData]
cbw
cwde
mov [ebx], eax
mov eax, 1
add eax, 0x30
pop ebx
pop edx
pop ecx
leave
ret 4

EscreverChar:
enter 0,0
mov DWORD [_bufferData], 0
mov BYTE [_bufferData + 4], 0
push ecx
push edx
push ebx
mov bl, [EBP + 8]
mov [_bufferData], bl
mov [_bufferData + 1], BYTE 0x0a
mov eax, 4
mov ebx, 1
mov ecx, _bufferData
mov edx, 2
int 80h
mov eax, 1
add eax, 0x30
pop ebx
pop edx
pop ecx
leave
ret 4

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