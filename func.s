global _start
section .data
_msg1 db "Foram lidos "
_msg1_s EQU $-_msg1
_msg2 db " caracteres.", 0dh, 0ah
_msg2_s EQU $-_msg2

section .bss
TMP_DATA resd 8
_bufferData resb 5
_aux resb 5
_result resd 1

section .text
_start:
push eax ; S_INPUT
push TMP_DATA
push DWORD 32
call LeerString
mov [_result], eax
call _displayNchars
pop eax

push eax ; S_OUTPUT
push TMP_DATA
push DWORD 20
call EscreverString
pop eax

;------------------------------------
mov eax, 1
mov ebx, 0
int 80h

LeerString:
enter 0,0
and DWORD [_bufferData], 0x00000000
and BYTE [_bufferData + 4], 0x00
and DWORD [_aux], 0x00000000
and BYTE [_aux + 4], 0x00
push ecx
push edx
push ebx
push esi
mov esi, 0
mov ecx, 25
mov ebx, [EBP + 12]
_cleanStringBuffer:
and DWORD [ebx + esi], 0x00000000
add esi, 4
loop _cleanStringBuffer
mov eax, 3
mov ebx, 2
mov ecx, [EBP + 12] ; param1
mov edx, [EBP + 8] ; param2
int 80h
mov eax, [EBP + 12] ; param1
mov ecx, [EBP + 8] ; param2
mov esi, 0
_loopString:
mov bl, [eax + esi]
cmp bl, 0x0a
je _endLeerString
inc esi
loop _loopString
_endLeerString:
mov eax, esi
mov esi, 0
mov ecx, 3
_convertasc:
mov edx, 0
mov ebx, 10
div ebx
add dl, 0x30
mov [_aux + esi], dl
cmp eax, 0
je _endStrConvert
inc esi
loop _convertasc
_endStrConvert:
mov ecx, 0
_invertIteration:
mov bl, [_aux + esi]
mov [_bufferData + ecx], bl
dec esi
inc ecx
cmp esi, 0
jge _invertIteration
mov eax, [_bufferData]
pop esi
pop ebx
pop edx
pop ecx
leave
ret 8

EscreverString:
enter 0,0
and DWORD [_bufferData], 0x00000000
and BYTE [_bufferData + 4], 0x00
and DWORD [_aux], 0x00000000
and BYTE [_aux + 4], 0x00
push ecx
push edx
push ebx
push esi
mov eax, 4
mov ebx, 1
mov ecx, [EBP + 12] ; param1
mov edx, [EBP + 8] ; param2
int 80h
mov eax, [EBP + 12] ; param1
mov ecx, DWORD [EBP + 8] ; param2
mov esi, 0
_loopEscString:
mov bl, [eax + esi]
cmp bl, 0x0a
je _endEscString
inc esi
loop _loopEscString
_endEscString:
mov eax, esi
mov esi, 0
mov ecx, 3
_convertascEsc:
mov edx, 0
mov ebx, 10
div ebx
add dl, 0x30
mov [_aux + esi], dl
cmp eax, 0
je _endStrConvertEsc
inc esi
loop _convertascEsc
_endStrConvertEsc:
mov ecx, 0
_invertIterationEsc:
mov bl, [_aux + esi]
mov [_bufferData + ecx], bl
dec esi
inc ecx
cmp esi, 0
jge _invertIterationEsc
mov eax, DWORD [_bufferData]
pop esi
pop ebx
pop edx
pop ecx
leave
ret 8

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