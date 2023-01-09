mov ax,01235h
mov bx,01234h
cmp ax,bx
jg abc
mov cx,bx
hlt
abc: mov cx,ax
hlt