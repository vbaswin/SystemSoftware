        mov bl,45

l1:     mov di,1000h
        mov cl,04h
l2:     mov al,[di]
        out c0,al
        dec bl
        jz  exit
        mov dx,1010h
l3:     dec dx
        jnz     l3
        inc di
        loop l2
        jmp l1

exit:   hlt