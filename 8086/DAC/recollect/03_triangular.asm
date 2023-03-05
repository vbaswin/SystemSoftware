l1: 	mov al,00h
l2: 	out c8,al
	inc al
	jnz l2
l3: 	mov al,0ffh
	dec al
	jnz l3
	jmp l1
	hlt