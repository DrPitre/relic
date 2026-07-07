; Compare a String
;
; Entry:
;   x0 - string1
;   x1 - string2
; Exit:
;   w0 = -1 (string1 < string2), 0 (string1 = string2), 1 (string1 > string2)
        .text
        .align  2
        .globl  comparestring
comparestring:
; prologue: set up the stack frame
        stp     x29,x30,[sp,-16]!
        mov     x29,sp

next:
        ldrb    w2,[x0]
        add     x0,x0,#1

        ldrb    w3,[x1]
        add     x1,x1,#1

        cmp     w2,w3
        bgt     higher
        blt     lower
        cmp     w2,#0
        bne     next

equal:
        mov     w0,#0
        b       epilogue
higher:
        mov     w0,#1
        b       epilogue
lower:
        mov     w0,#-1

; epilogue: tear down stack and return
epilogue:
        ldp     x29,x30,[sp],16
        ret


; Compare a String without respect to case
;
; Entry:
;   x0 - string1
;   x1 - string2
; Exit:
;   w0 = -1 (string1 < string2), 0 (string1 = string2), 1 (string1 > string2)
        .text
        .align  2
        .globl  comparestringnocase
comparestringnocase:
; prologue: set up the stack frame
        stp     x29,x30,[sp,-16]!
        mov     x29,sp

next1:
        ldrb    w2,[x0]
        add     x0,x0,#1
        cmp     w2,#65          ; 'A'
        blt     next2
        cmp     w2,#90          ; 'Z'
        bgt     next2
        orr     w2,w2,#0x20

next2:
        ldrb    w3,[x1]
        add     x1,x1,#1
        cmp     w3,#65          ; 'A'
        blt     compare
        cmp     w3,#90          ; 'Z'
        bgt     compare
        orr     w3,w3,#0x20

compare:
        cmp     w2,w3
        bgt     higher1
        blt     lower1
        cmp     w2,#0
        bne     next1

equal1:
        mov     w0,#0
        b       epilogue1
higher1:
        mov     w0,#1
        b       epilogue1
lower1:
        mov     w0,#-1

epilogue1:
        ldp     x29,x30,[sp],16
        ret
