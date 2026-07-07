; Print a boolean
;
; Entry:
;   printboolean_p0 - path
;   printboolean_p1 - boolean to print

        .text
true:   .ascii  "True"
trulen  =       .-true

false:  .ascii  "False"
fallen  =       .-false

        .align  2
        .globl  printboolean
printboolean:
; prologue: set up the stack frame
        stp     x29,x30,[sp,-16]!
        mov     x29,sp

; determine value in w4
        adrp    x4,printboolean_p1@PAGE
        ldrb    w4,[x4,printboolean_p1@PAGEOFF]
        cmp     w4,#0
        bne     gotrue

; get pointer to false in x1
gofalse:
        adrp    x1,false@PAGE
        add     x1,x1,false@PAGEOFF
        mov     x2,#fallen
        b       getpath

gotrue:
        adrp    x1,true@PAGE
        add     x1,x1,true@PAGEOFF
        mov     x2,#trulen

; get path (fd) into x0
getpath:
        adrp    x0,printboolean_p0@PAGE
        ldrb    w0,[x0,printboolean_p0@PAGEOFF]

; call system call to write
        mov     x16,#4
        svc     #0x80

; epilogue: tear down stack and return
        ldp     x29,x30,[sp],16
        ret
