; sub with borrow

mov ax,1234h
mov cx,ax
mov bx,4520h
sub ax,bx
mov ax,cx
sbb ax,bx
hlt
