; search and sorch by ASWIN V B
; more cleaner :)

data segment
	msg1 db 0ah,0dh,"Enter count: $"
	msg2 db 0ah,0dh,"Enter no: $"
	msg3 db 0ah,0dh,"Enter element to search: $"
	msg4 db 0ah,0dh,"Sorted list: $"
	affirm db 0ah,0dh,"Found at index: $"
	deny db 0ah,0dh,"Not found$"
	numbers dw 0fh dup(0000h)
	space db " $"
	len dw ?
	num dw ?
	temp dw ?
data ends
display macro msg
	lea dx,msg
	mov ah,09h
	int 21h
endm

read macro num
	local l1,exit
	mov ax,0000h
	mov num,ax
	l1:	mov ah,01h
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
	exit:	nop
endm

print macro num
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
	l2: 	pop dx
		add dx,0030h
		mov ah,02h
		int 21h
		loop l2
endm

readNumbers macro numbers,len
	local l1,exit
		mov di,0000h
		mov cx,len
	l1: 	read numbers[di]
		inc di
		inc di
		loop l1
endm

printNumbers macro numbers,len
		local l1,exit
		mov cx,len
		mov di,0000h
	l1:	mov temp,cx
		print numbers[di]
		display space
		mov cx,temp
		inc di
		inc di
		loop l1
endm

search macro numbers,len
		local l1,exit
		display msg3
		read num
		mov cx,len
		mov di,0000h
		mov ax,0000h
	l1:	mov ax,numbers[di]
		cmp ax,num
		je present
		inc di
		inc di
		loop l1
		display deny
		jmp exit
present: 	display affirm
		mov ax,di
		mov bl,0002h
		div bl
		print ax
	exit: 	nop
endm

sort macro numbers,len
		local l1,l2,exit
		mov cx,len
		mov si,0000h
		mov di,0000h

	l1: 	mov ax,numbers[si]
		mov temp,cx
		mov di,0000h
		mov cx,len

	l2:	mov bx,numbers[di]
		cmp ax,bx
		jg noswap
		mov numbers[si],bx
		mov numbers[di],ax
		mov ax,bx

	noswap:	inc di
		inc di
		loop l2
		mov cx,temp
		inc si
		inc si
		loop l1
endm

code segment
	assume cs:code, ds:data
	start: 	mov ax,data
		mov ds,ax
		display msg1
		read len
		readNumbers numbers,len
		printNumbers numbers,len
		search numbers,len
		sort numbers,len
		display msg4
		printNumbers numbers,len
		mov ah,4ch
		int 21h
code ends
end start