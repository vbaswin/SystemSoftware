; add 2 numbers

MOV [1100h],20h
MOV [1102h],5
MOV SI,1100h
MOV AX,[SI]
MOV BX,[SI+02h]
MOV CL,00
ADD AX,BX
MOV [SI+ 04h],AX
INC CL
MOV [SI+ 06h],CL
HLT