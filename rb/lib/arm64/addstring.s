; Add two strings together
;
; This routine assumes the destination buffer is large enough to accomodate the source strings
;
; Entry:
;   x0 - source1 pointer
;   x1 - source2 pointer
;   x2 - destination pointer
        .text
        .align  2
        .globl  addstring
addstring:
; prologue: set up the stack frame
        stp     x29,x30,[sp,-16]!
        mov     x29,sp

; copy source1
copysource1:
        ldrb    w3,[x0]
        cmp     w3,#0
        beq     copysource2
        strb    w3,[x2]
        add     x2,x2,#1
        add     x0,x0,#1
        b       copysource1

; copy source2
copysource2:
        ldrb    w3,[x1]
        cmp     w3,#0
        beq     epilogue
        strb    w3,[x2]
        add     x2,x2,#1
        add     x1,x1,#1
        b       copysource2

; epilogue: tear down stack and return
epilogue:
        mov     w3,#0
        strb    w3,[x2]
        ldp     x29,x30,[sp],16
        ret
