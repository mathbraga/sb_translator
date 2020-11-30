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
_result resd 1

section .text
_start:
;------------------------------------ INPUT
push eax
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
pop eax
;----------------------------------------

;--------------------------------- OUTPUT
push eax
push DWORD [TMP_DATA]
call EscreverInteiro
pop eax
;------------------------------------
mov eax, 1
mov ebx, 0
int 80h

;-------------------------------------
LeerInteiro:
enter 0,0
mov DWORD [_bufferData], 0
mov BYTE [_bufferData + 4], 0
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
mov eax, 0
mov esi, 0
mov ecx, 4
mov ebx, 0
mov edx, 10
_converteint:
mov bl, [_bufferData+esi]
cmp bl, 0x2d
je _skipneg
cmp bl, 0x0a
je _endconvert
sub bx, 0x30
mul dl
add al, bl
inc esi
loop _converteint
_skipneg:
inc esi
jmp _converteint
_endconvert:
mov bl, [_bufferData]
cmp bl, 0x2d
jne _returnconvert
mov ecx, -1
xor eax, ecx
add eax, 1
_returnconvert:
mov ebx, [EBP + 8]
mov [ebx], eax
pop eax
add eax, 0x30
pop esi
pop ebx
pop edx
pop ecx
leave
ret 4
;-------------------------------------

;---------------------------------
EscreverInteiro:
enter 0,0
mov DWORD [_bufferData], 0
mov BYTE [_bufferData + 4], 0
mov DWORD [_aux], 0
mov BYTE [_aux + 4], 0
push ebx
push ecx
push edx
push esi
mov esi, 0
mov ecx, 3
mov eax, [EBP + 8]
cmp eax, 0
jl _converttopos
_convertasc:
mov edx, 0
mov ebx, 10
div ebx
add dl, 0x30
mov [_aux + esi], dl
cmp eax, 0
je _convertsymbol
inc esi
loop _convertasc
_converttopos:
sub eax, 1
xor eax, -1
jmp _convertasc
_convertsymbol:
mov eax, [EBP + 8]
cmp eax, 0
jge _invertstr
inc esi
mov BYTE [_aux + esi], '-'
_invertstr:
mov ecx, 0
_loopiteration:
mov bl, [_aux + esi]
mov [_bufferData + ecx], bl
dec esi
inc ecx
cmp esi, 0
jge _loopiteration
mov BYTE [_bufferData + ecx], 0x0a
cmp BYTE [_bufferData], 0x2d
jne _endescrever
dec ecx
_endescrever:
mov eax, ecx
push eax
mov eax, 4
mov ebx, 1
mov ecx, _bufferData
mov edx, 5
int 80h
pop eax
pop esi
pop edx
pop ecx
pop ebx
leave
ret 4
;---------------------------------