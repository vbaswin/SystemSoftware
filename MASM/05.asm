data segment
	enterMsg db 0ah,0dh,"Enter string: $"
	affirm	db 0ah,0dh,"Palindrome$"
	deny	db 0ah,0dh,"Not a Palindrome$"
	str db 0fh dup('$')
data ends
code segment
	assume ds:data,cs:code
	start:	mov ax,data
		mov ds,ax
		lea dx,enterMsg
		mov ah,09h
		int 21h
		mov cx,0000h
		lea di,str

	l1:	mov ah,01h
		int 21h
		cmp al,0dh
		je break
		mov [di],al
		inc di
		inc cx
		push ax
		jmp l1

	break: 	lea di,str

	l2:	pop dx
		mov bx,[di]
		inc di
		cmp bl,dl
		jne notPalindrome
		loop l2
		lea dx,affirm
		jmp exit
notPalindrome:  lea dx,deny
	exit: 	mov ah,09h
		int 21h
		mov ah,4ch
		int 21h
code ends
end start