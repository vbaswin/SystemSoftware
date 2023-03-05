l1:	mov al,00h
l2:	out c8,al
	inc al
	jnz l2
	jmp l1
	hlt