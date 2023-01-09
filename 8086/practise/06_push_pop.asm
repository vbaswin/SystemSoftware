mov ax,1234h
mov bx,0f454h
mov cx,0ffffh
mov ds,cx
; both si and di can be used
mov SI,1100h
mov [SI],1234h
; mov si,1200h
push ax
push bx
pop cx
pop bx
; si or di but if you give their addresses though the ds
; address does not change the value pointed by si and di will be
; different so check which pointer are you currently pointing in ram of emu8086
mov [si],ax
mov [si+2],bx
mov [si+4],cx
hlt