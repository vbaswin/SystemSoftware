l1:	mov al,0ffh
l2:	out c8,al
	call delay
	mov al,00h
	out c8,al
	call delay
	jmp l1

delay: 	mov cx,1010
l3: 	dec dx
	loop l3
	ret
	hlt