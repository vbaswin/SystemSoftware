start:
    mov al,00h
    mov cx,0ffh
    here:
        out c8,al
        inc al
        loop here
    mov cx,al
    here2:
        out c8,al
        dec al
        loop here
    jmp start
hlt