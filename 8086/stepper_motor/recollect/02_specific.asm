	mov bl,45h
l1: 	mov di,1200h
	mov cl,04h
l2: 	mov al,[di]
	out c0,al
	dec	bl
	jz	exit
	call delay
	inc di
	loop l2
	jmp l1

delay: 	mov dx,1010
l3: 	dec dx
	jnz l3
	ret
exit: 	hlt