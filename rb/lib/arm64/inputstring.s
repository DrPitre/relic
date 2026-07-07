; Input a String
;
; Entry:
;   inputstring_p0 - path
;   inputstring_p1 - address of string to input
;   inputstring_p2 - maximum length of string to accept
        .text
        .globl  inputstring
        .align  2
inputstring:
; prologue: set up the stack frame
        stp     x29,x30,[sp,-16]!
        mov     x29,sp

; get path (fd) into x0
        adrp    x0,inputstring_p0@PAGE
        ldrb    w0,[x0,inputstring_p0@PAGEOFF]

; get address into x1
        adrp    x1,inputstring_p1@PAGE
        add     x1,x1,inputstring_p1@PAGEOFF

; get length of string in x2
        adrp    x2,inputstring_p2@PAGE
        add     x2,x2,inputstring_p2@PAGEOFF
        ldrsh   w2,[x2]

        mov     x16,#3          ; syscall number (sys_read)
        svc     #0x80           ; call kernel

        adrp    x1,inputstring_p1@PAGE
        add     x1,x1,inputstring_p1@PAGEOFF
        add     x1,x1,x0
        sub     x1,x1,#1
        mov     w2,#0
        strb    w2,[x1]

; epilogue: tear down stack and return
        ldp     x29,x30,[sp],16
        ret
