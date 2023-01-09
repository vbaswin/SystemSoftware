mov ax,00300h
mov bx,00400h
mov ds,ax
mov es,bx
mov si,00000h

; mov 042h to ds seg with offset si
mov [si],042h
; mov 0ffh to es seg with offset si
mov es:[si],0ffh
; you also don't need to give register in logical addressing
mov es:[0000h],0eeh


mov cx,ds:[si]

mov dh,ds:[si]

mov bh,es:[si]