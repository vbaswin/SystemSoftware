mov ax,0124fh
mov dx,ax
mov cl,03h
; rol [src & dest],count(no of bits to rotate)
rol ax,cl
ror dx,03h
hlt