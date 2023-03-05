                mov si,1000h
                mov [si],04h
                mov [si+1],05h
                mov [si+2],08h
                mov [si+3],03h
                mov [si+4],10h
                mov [si+5],11h
                mov [si+6],0fh
                mov [si+7],0dh
                mov [si+8],0eh
                mov [si+9],0bh

		mov cx,000ah
		mov bx,0000h
		mov [1200h],bx
l1:		mov al,[si]
		mov [1200h],cl
		mov cx,000ah
		mov di,1000h
l2:		mov bl,[di]
		cmp al,bl
		jle noswap
		mov [si],bl
		mov [di],al
		mov al,bl
noswap:		inc di
		loop l2
		inc si
		mov cl,[1200h]
		loop l1
		hlt