; stepper rotation clockwise and anitclockwise
mov di,[1020h]
mov cl,04h
l1:
    mov al,[di]
    out c0,al
    mov dx,1010
    l2:
        dec dx
        jnz l2
    inc di
    loop l1
hlt

