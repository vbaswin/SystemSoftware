; 2 bytes => one word is copied instead of one byte at a time
; source is DS segment with si
; destination is ES seg with di

mov ax,00200h
mov ds,ax

mov ax,00300h
mov es,ax

;mov si,2000h
;mov di,2000h

mov ds:[0000],07a54h
mov ds:[0002],07a8bh
mov ds:[0004],078f4h

;mov ds:[si],52h
;add si,02h
;mov ds:[si],74h
;add si,02h
;mov ds:[si],8ah
;; reset location of si to 2000
;mov si,2000h

; both si and di pointers move and the corresponding values are
; copied for ds:[si] to es:[di]
movsw
movsw
movsw

hlt