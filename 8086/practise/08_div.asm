mov ax,1100h
mov ds,ax
mov si,0000h

; 8-bit division
mov ax,1389h
mov bl,19h

div bl

mov [si],ax

; 16-bit division
mov dx,1004h
mov ax,1004h
mov cx,1234h

div cx

add si,02h
mov [si],ax
add si,02h
mov [si],dx

hlt