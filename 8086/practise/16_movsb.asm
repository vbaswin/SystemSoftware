; source is DS segment with si
; destination is ES seg with di

mov ax,00200h
mov ds,ax

mov ax,00300h
mov es,ax

mov si,2000h
mov di,2000h

;mov ds:[0000],52h
;mov ds:[0001],74h
;mov ds:[0002],8ah

mov ds:[si],52h
inc si
mov ds:[si],74h
inc si
mov ds:[si],8ah
; reset location of si to 2000
mov si,2000h

; both si and di pointers move and the corresponding values are
; copied for ds:[si] to es:[di]
movsb
movsb
movsb

hlt