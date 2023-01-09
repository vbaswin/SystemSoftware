; writing data to data segment
mov ax,0300h
; we cannot put address directly to data segment
mov ds,ax
mov cx,0005h
mov di,0000h
mov ax,0000h
abc: mov [di],ax
inc di
add ax,02h
loop abc:
hlt