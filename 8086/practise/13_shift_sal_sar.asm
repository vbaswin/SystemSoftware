; shift arithmetic left (arithmetic shift left)
; for left -> move the move the no the digits we specified as count to left
; and put the vacant space at right end with 0's

; for right -> move the move the no the digits we specified as count to right
; and put the vacant space at left end with 0's

; sal [dest / src], count
mov ax,07fa4h
mov bx,ax
mov cl,02h
sal ax,cl
sar bx,cl
hlt

; logical https://www.youtube.com/watch?v=KS4xncTT3r0&list=PLEacQ4Oes1V1q-LQh9i0O71aDftqPAqzS&index=25
; I don't have time for that