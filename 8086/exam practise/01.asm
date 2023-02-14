; array multiplication
mov ax,2000h
mov ds,ax

mov si,1000h

mov [si],01h
mov [si + 1],02h
mov [si + 2],03h
mov [si + 3],04h
mov [si + 4],05h

mov di,1500h
mov [di],0Ah
mov [di + 1],0Bh
mov [di + 2],0Ch
mov [di + 3],0Dh
mov [di + 4],0Eh

mov dx,2000h

mov cx,0005h
loop1:
	mov al,[si]
	mov bl,[di]
	mul bl
	mov [dx],ax
	inc si
	inc di
	add dx,0002h
	loop loop1
hlt