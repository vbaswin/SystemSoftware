mov ax,1200h
mov ds,ax
mov si,1000h
; mov cx,0000h
; 0000 of ds 1200 will have the value 45a8h no need to store in
; registers and all :)
mov [0000],45a8h
mov [0002],0e87h

;and
mov ax,[0000]
mov bx,[0002]
and ax,bx
mov [si],ax

;or
mov ax,[0000]
or ax,bx
add si,02h
mov [si],ax

;not
mov ax,[0000]
not ax
add si,02h
mov [si],ax

;xor
mov ax,[0000]
xor ax,bx
add si,02h
mov [si],ax

hlt