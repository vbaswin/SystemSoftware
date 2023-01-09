; stepper rotation clockwise and anitclockwise with a specific angle

mov bl,45
mov di,[2000]
mov cx,04h
l1:
    mov al,[di]
    out c0,al
    l3:
        mov dl,bl
        dec dl
        jnz l3
    mov dx,1111
    l2:
        dec dx
        jnz l2
    inc di
    loop l1
hlt