; Arithmetic operations
data segment
	opr1 db 0ah,0dh,"First number: $"
	opr2 db "Second number: $"
	addres db 0ah,0dh,"A + B: $"
	subres db 0ah,0dh,"A - B: $"
	mulres db 0ah,0dh,"A * B: $"
	divres db 0ah,0dh,"A / B: $"
	remres db " rem: $"
	rem dw ?
	num dw ?
	num1 dw ?
	num2 dw ?
	temp dw ?
	res dw ?
data ends

display macro msg
	mov dx,offset msg
	; or lea dx,msg
	mov ah,09h
	int 21h
endm

read macro num
	local l1,exit
	mov ax,0000h
	mov num,ax
	l1:
		mov ah,01h
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
	loop l1
	exit: nop
endm

numDisplay macro num
	local l1,l2
	mov ax,num
	mov cx,0000h
	l1:
		mov dx,0000h
		mov bx,000ah
		div bx
		push dx
		inc cx
		cmp ax,0000h
		jne l1
	l2:
		pop dx
		add dx,0030h
		mov ah,02h
		int 21h
	loop l2
endm

calculate macro num1,num2
	; addition
	mov ax,num1
	add ax,num2
	mov res,ax
	display addres
	numDisplay res

	; subraction
	mov ax,num1
	sub ax,num2
	mov res,ax
	display subres
	numDisplay res


	; multiplication
	mov ax,num1
	mov bx,num2
	mul bx
	mov res,ax
	display mulres
	numDisplay res

	; division
	mov ax,num1
	mov bx,num2
	mov dx,0000h
	div bx
	mov res,ax
	mov rem,dx
	display divres
	numDisplay res
	display remres
	numDisplay rem
endm

code segment
	assume ds:data, cs:code
	start:
		mov ax,data
		mov ds,ax
		display opr1
		read num1
		display opr2
		read num2
		calculate num1,num2
		mov ah,4ch
		int 21h
	code ends
end start