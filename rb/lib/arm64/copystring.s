; Copy a string
;
; Entry:
;   x0 - source size (word)
;   x1 - source pointer
;   x2 - destination size (word)
;   x3 - destination pointer
        .text
        .align  2
        .globl  copystring
copystring:
; prologue: set up the stack frame
        stp     x29,x30,[sp,-16]!
        mov     x29,sp

; compare source and destination size, pick smaller and put in x0
        cmp     w0,w2
        blt     copy
        mov     w0,w2

; for 'x0' times, copy byte from x1 to x3
copy:
        cmp     w0,#0
        beq     done
        ldrb    w4,[x1]
        add     x1,x1,#1
        strb    w4,[x3]
        add     x3,x3,#1
        sub     w0,w0,#1
        b       copy
done:
        mov     w4,#0
        strb    w4,[x3]

; epilogue: tear down stack and return
        ldp     x29,x30,[sp],16
        ret
