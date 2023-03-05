l3: 	mov di,1200h
	mov cl,04h
l1:	mov al,[di]
	out c0,al
	call delay
	inc di
	loop l1
	jmp l3

delay: mov dx,1010
l2:	dec dx
	jnz l2
	ret
	hlt