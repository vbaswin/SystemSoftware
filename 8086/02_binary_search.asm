mov ax,01000h
mov bx,02000h
mov ds,ax
mov es,bx
mov si,0000h

mov cx,000Ah
mov ax,0000h
abc:
    mov [si],ax
    inc ax
    inc si
loop abc:

mov ds:[000Ah],02
mov ds:[000Bh],00

;mov cx,0010h
mov [000Ch],00h
mov [000Dh],09h

mov si,0009h
loop1:
    mov ax,[000Ch]
    add ax,[000Dh]
    mov bl,[000Bh]
    div bl
    mov ah,00h
    mov si,ax
    div ds:[000Ah]

    cmp ds:[0000 +si],00h
    je present

    hlt
jmp loop1

mov [000Eh],00h
hlt

present:
    mov [000Eh],05h
hlt