; rotate left through carry
mov ax,01234h
; set carry flag
stc
mov cl,01h
rcl ax,cl

; rotate right throught carry
stc
mov bx,07fa4h
mov cl,02h
rcr bx,cl
hlt