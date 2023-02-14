data segment
	msg1 db 0ah,0dh,"Enter string: $"
	msg2 db 0ah,0dh,"Enter subString: $"
	affirm db 0ah,0dh,"Is a Substring$"
	deny db 0ah,0dh,"Is not a Substring$"
	str db 0fh dup('$')
	subStr db 0fh dup('$')
	num dw ?
	temp dw ?
	len1 dw ?
	len2 dw ?
data ends
print macro msg
	lea dx,msg
	mov ah,09h
	int 21h
endm
read macro str,len
	local l1,exit
		lea di,str
		mov cx,0000h
	l1: 	mov ah,01h
		int 21h
		cmp al,0dh
		je exit
		mov [di],al
		inc di
		inc cx
		jmp l1
	exit: 	mov len,cx
endm
printNum macro num
	local l1,l2
		mov ax,num
		mov cx,0000h
	l1: 	mov dx,0000h
		mov bx,000ah
		div bx
		push dx
		inc cx
		cmp ax,0000h
		jne l1
	l2:	pop dx
		add dx,0030h
		mov ah,02h
		int 21h
		loop l2
endm
readNum macro num
	local l1,exit
	mov ax,0000h
	mov num,ax
	l1: 	mov ah,01h
		int 21h
		cmp al,0dh
		je exit
		mov ah,00h
		sub ax,0030h
		mov temp,ax
		mov bx,000ah
		mov ax,num
		mul bx
		add ax,temp
		mov num,ax
		jmp l1
	exit: 	nop
endm
code segment
	assume cs:code,ds:data
	start:	mov ax,data
		mov ds,ax
		print msg1
		read str,len1
		print str
		printNum len1
		print msg2
		read subStr,len2
		print subStr
		printNum len2

		lea si,str
		mov ax,0000h
		mov bx,0000h
		mov cx,0000h
		mov dx,0000h
		mov dx,len1
	l1: 	mov al,[si]
		mov cx,len2
		lea di,subStr
		mov temp,si
	l2:	mov bl,[di]
		cmp al,bl
		jne notEqual
		inc di
		inc si
		mov al,[si]
		loop l2
		print affirm
		jmp exit

notEqual:	mov si,temp
		inc si
		dec dx
		cmp dx,0000h
		je notPresent
		jmp l1

notPresent: 	print deny
		;readNum num
		;printNum num
	exit: 	mov ah,4ch
		int 21h
code ends
end start