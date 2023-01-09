mov ax,01000h
mov bx,02000h

mov ds,ax
mov es,bx

mov ds:si,00000h
mov [si],001h
mov [si + 1],002h
mov [si + 2],003h
mov [si + 3],004h
mov [si + 4],005h

mov ds:[00005],004h
mov cx,05h

abc:
    mov al,[si]
    cmp al,[000005]
    je present
    inc si
loop abc
mov ds:[00006h],00
hlt

present:
    mov ds:[00006h],si
hlt