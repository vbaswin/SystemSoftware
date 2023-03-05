data segment
	enterMsg db 0ah,0dh,"Enter string: $"
	affirm db 0ah,0dh,"Palindrome$"
	deny db 0ah,0dh,"Not a palindrome$"
	str db 0fh dup('$')
data ends

print macro msg
	lea dx,msg
	mov ah,09h
	int 21h
endm

read macro str
	local l1,exit
		lea di,str
		mov ax,0000h
		mov cx,0000h
	l1: 	mov ah,01h
		int 21h
		cmp al,0dh
		je exit
		push ax
		mov [di],al
		inc di
		inc cx
		jmp l1
	exit: nop
endm

code segment
		assume cs:code,ds:data
	start: 	mov ax,data
		mov ds,ax
		print enterMsg
		read str
		lea di, str
		; Got error while trying to use only pop insturciton in another macro
		; so that's' why everyone was using a single code for these
		; kind of operations
	l1:	pop dx
		mov bx,[di]
		inc di
		cmp bl,dl
		jne notPalindrome
		loop l1
		print affirm
		jmp exit
notPalindrome:	print deny
	exit: 	nop

		mov ah,4ch
		int 21h
code ends
end start