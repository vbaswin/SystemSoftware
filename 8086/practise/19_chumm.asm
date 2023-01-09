mov ax,1000h
mov ds,ax
mov si,0000h

; stores only one byte 24h
mov [bx],0024h
inc bx
; stores 2 bytes and inc accordingly
mov [bx],1578h
hlt