mov ax,01000h
mov bx,02000h

mov ds,ax
mov es,bx

mov [si],004h
mov [si + 1],003h
mov [si + 2],005h
mov [si + 3],001h
mov [si + 4],002h

mov cx,0005h
mov si,0000h

loop1:
    mov di,si
    add di,01h
    loop2:
        mov al,[si]
        mov bl,[si]
        cmp bl,[di]
        jle noless
            mov bl,[si]
            mov cl,[di]
            mov [si],cl
            mov [di],bl
        noless:
            inc di
            cmp di,00005h
        jne loop2
    inc si
    cmp si,0004h
    jne loop1

hlt