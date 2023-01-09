mov ax,01234h
mov bx,01234h
xor ax,bx
; jump to abc if zero flag is one
; or after xor operation is ax is completely 0
jz abc
mov cx,05678h
hlt
abc: mov dx,05678h
hlt