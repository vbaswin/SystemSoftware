mov si,1000h
mov [si],0005h
mov [si+2],0002h

mov ax,0000h
mov dx,0000h
mov ax,[si]
mov bx,[si+2]
div bx
mov [si+4],ax
mov [si+6],dx
hlt