; Convert a string to an unsigned 8-bit decimal number
;
; Valid input: XXX (where 0 <= X <= 9)
;
; Entry:
;   x0 - address of string (nul, cr or lf terminated)
; Exit:
;   x0 - 0 = success; 1 = error
;   x1 - value of string (if x0 is 0)

        .text
        .globl  StrToDecU8
        .align  2
StrToDecU8:
; prologue: set up the stack frame
        stp     x29,x30,[sp,-16]!
        mov     x29,sp

; register usage for this function:
; x0  - address of string (nul, cr or lf terminated)
; w1  - character buffer
; w3  - length
; x4  - copy of x0 for iteration
; x5  - buffer iterator (end)
; x6  - buffer iterator (start)
; w7  - power of 10 multiplier
; w8  - base (10) for multiplying
; w9  - accumulated value
        mov     w3,#0           ; holds length of number
        mov     x4,x0
decbn1:
        ldrb    w1,[x4]
        add     x4,x4,#1
decbn15:
        cmp     w1,#0x30
        blt     decbn3
        cmp     w1,#0x39
        bgt     error
        add     w3,w3,#1
        b       decbn1

decbn3:
        cmp     w1,#0
        beq     ok
        cmp     w1,#0x0A
        beq     ok
        cmp     w1,#0x0D
        bne     error

ok:
        cmp     w3,#0           ; length = 0?
        beq     error           ; yes, error
        cmp     w3,#3           ; more than 3 characters?
        bgt     error           ; yes, error

; now we convert the validated string to a real binary number
; x0 = start of string
; w3 = length of number string

        add     x5,x0,x3
        mov     x6,x0
        mov     w7,#1
        mov     w8,#10
        mov     w9,#0
loop:
        sub     x5,x5,#1
        ldrb    w2,[x5]
        sub     w2,w2,#0x30
        mul     w2,w2,w7
        mul     w7,w7,w8
        add     w9,w9,w2
        cmp     x5,x6
        bne     loop

exit:
        mov     x0,#0
        mov     x1,x9
        b       epilogue

error:
        mov     x0,#1

; epilogue: tear down stack and return
epilogue:
        ldp     x29,x30,[sp],16
        ret
