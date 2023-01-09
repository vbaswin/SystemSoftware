start:
    mov al,00
    out c8,al
    call ahead1
    mov al,0ffh
    out c8,al
    call ahead1
    jmp start
ahead1:
    mov cx,0ffh
here: loop here
    ret
hlt