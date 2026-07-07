; Print an integer
;
; Entry:
;   printinteger_p0 - path
;   printinteger_p1 - integer to print
        .data
        .comm   temp,8

        .text
        .align  2
        .globl  printinteger
printinteger:
; prologue: set up the stack frame
        stp     x29,x30,[sp,-16]!
        mov     x29,sp

; get integer to print into x0
        adrp    x0,printinteger_p1@PAGE
        add     x0,x0,printinteger_p1@PAGEOFF
        ldrsh   w0,[x0]

; get pointer to temp buffer in x1
        adrp    x1,temp@PAGE
        add     x1,x1,temp@PAGEOFF

; call decS16toStr
        bl      decS16toStr

; get path (fd) into x0
        adrp    x0,printinteger_p0@PAGE
        ldrb    w0,[x0,printinteger_p0@PAGEOFF]

; get pointer to temp buffer in x1
        adrp    x1,temp@PAGE
        add     x1,x1,temp@PAGEOFF

; get length of string in x2
        mov     x2,#0           ; clear x2 (string length)
        mov     x7,x1           ; move pointer to string into x7
count:  ldrb    w6,[x7]
        cmp     w6,#0
        beq     lo
        add     x7,x7,#1
        add     x2,x2,#1
        b       count

; call system call to write
lo:     mov     x16,#4
        svc     #0x80

; epilogue: tear down stack and return
        ldp     x29,x30,[sp],16
        ret
