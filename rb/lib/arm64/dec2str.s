; Convert an unsigned 8-bit decimal number to a string
;
; Entry:
;   x0 - 8 bit integer to convert
;   x1 - address of 4 byte buffer
; Exit:
;   x1 - address of nul byte
        .text
        .align  2
        .globl  decU8toStr

decU8toStr:
; prologue: set up the stack frame
        stp     x29,x30,[sp,-16]!
        mov     x29,sp

        mov     x2,#0
        b       byte

; Convert a signed 16-bit decimal number to a string
;
; Entry:
;   x0 - 16 bit integer to convert
;   x1 - address of 8 byte buffer
; Exit:
;   x1 - address of nul byte

        .globl  decS16toStr
        .align  2
decS16toStr:
        stp     x29,x30,[sp,-16]!
        mov     x29,sp

; determine if integer is negative; if not, go unsigned right away
        cmp     w0,#0
        bge     decU16toStr

; add negative sign
        mov     w2,#45          ; '-'
        strb    w2,[x1]
        add     x1,x1,#1

; make twos complement of number
        neg     w0,w0

; Convert an unsigned 16-bit decimal number to a string
;
; Entry:
;   x0 - 16 bit integer to convert
;   x1 - address of 6 byte buffer
; Exit:
;   x1 - address of nul byte

        .globl  decU16toStr
        .align  2
decU16toStr:
        mov     x2,#0
        mov     w4,#10000
        bl      conv
        mov     w4,#1000
        bl      conv
byte:   mov     w4,#100
        bl      conv
        mov     w4,#10
        bl      conv
        mov     w4,#1
        bl      conv
        cmp     w2,#0
        bne     addnil
        mov     w5,#0x30
        strb    w5,[x1]
        add     x1,x1,#1

addnil: mov     w5,#0
        strb    w5,[x1]

; epilogue: tear down stack and return
        ldp     x29,x30,[sp],16
        ret

conv:   mov     w5,#0x30
l1:     sub     w0,w0,w4
        cmp     w0,#0
        blt     k1
        add     w5,w5,#1
        b       l1
k1:     add     w0,w0,w4
        cmp     w5,#0x30
        bne     l2
        cmp     w2,#0
        bne     l2
        ret
l2:     strb    w5,[x1]
        add     x1,x1,#1
        mov     w2,#1
        ret
