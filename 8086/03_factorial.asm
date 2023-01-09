mov ax,01000h
mov bx,02000h

mov ds,ax
mov es,bx

mov cx,0008h

mov ax,0001h
mov dx,0000h

abc:
    mul cx
loop abc
mov ds:[00000h],dx
mov ds:[00002h],ax
hlt