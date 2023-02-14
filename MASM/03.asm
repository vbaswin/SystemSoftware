data segment
	enterMsg db 0ah,0dh,"Enter string: $"
	affirm	db 0ah,0dh,"Palindrome$"
	deny	db 0ah,0dh,"Not a Palindrome$"
	; data allocation initialised
	chumma db 0Ah dup('I am Aswin$')
	; uninitialised
	str db 0fh dup('$')
data ends
print macro msg
	lea dx,msg
	mov ah,09h
	int 21h
endm
read macro str
	local l1,exit
	mov cx,0000h
	mov ax,0000h
	lea di,str
	l1:
		mov ah,01h
		int 21h
		cmp al,0dh
		je break
		mov [di],al
		inc di
		inc cx
		push ax
		jmp l1
	break: lea di,str
	l2:
		pop dx
		mov bx,[di]
		inc di
		; do not use comparison after many steps
		; the flags may get affected
		; cmp and then use the comparison as soon as possible
		cmp bl,dl
		jne notPalindrome
		loop l2
		print affirm
		jmp exit
	notPalindrome:  print deny
	exit: 	nop
endm

code segment
	assume ds:data,cs:code
	start:
		mov ax,data
		mov ds,ax
		print enterMsg
		read str
		mov ah,04ch
		int 21h
code ends
end start