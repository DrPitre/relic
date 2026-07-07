; Input a Boolean
;
; Entry:
;   inputboolean_p0 - path
;   inputboolean_p1 - address of string to input
        .data
        .comm   inputbooleanbuffer,7,0

        .text
        .globl  inputboolean
BadInput:   .ascii    "** Input error - BOOLEAN requires TRUE/1 or FALSE/0 - reenter **"
            .byte     0x0D,0x0A
BadInputLen =         .-BadInput
TrueTRUE:   .ascii    "true"
            .byte     0x00
True1:      .ascii    "1"
            .byte     0x00
FalseFALSE: .ascii    "false"
            .byte     0x00
False0:     .ascii    "0"
            .byte     0x00

        .align  2
inputboolean:
; prologue: set up the stack frame
        stp     x29,x30,[sp,-16]!
        mov     x29,sp

; get path (fd) into x0
again:
        adrp    x0,inputboolean_p0@PAGE
        ldrb    w0,[x0,inputboolean_p0@PAGEOFF]

; get address into x1
        adrp    x1,inputbooleanbuffer@PAGE
        add     x1,x1,inputbooleanbuffer@PAGEOFF

; get length of string in x2
        mov     x2,#6

        mov     x16,#3          ; syscall number (sys_read)
        svc     #0x80           ; call kernel

; replace CR with nul byte
        adrp    x1,inputbooleanbuffer@PAGE
        add     x1,x1,inputbooleanbuffer@PAGEOFF
        add     x1,x1,x0
        sub     x1,x1,#1
        mov     w2,#0
        strb    w2,[x1]

; determine input (0 or FALSE, 1 or TRUE)
        adrp    x0,inputbooleanbuffer@PAGE
        add     x0,x0,inputbooleanbuffer@PAGEOFF
        adrp    x1,TrueTRUE@PAGE
        add     x1,x1,TrueTRUE@PAGEOFF
        bl      comparestringnocase
        cmp     w0,#0
        beq     isTrue

        adrp    x0,inputbooleanbuffer@PAGE
        add     x0,x0,inputbooleanbuffer@PAGEOFF
        adrp    x1,True1@PAGE
        add     x1,x1,True1@PAGEOFF
        bl      comparestringnocase
        cmp     w0,#0
        beq     isTrue

        adrp    x0,inputbooleanbuffer@PAGE
        add     x0,x0,inputbooleanbuffer@PAGEOFF
        adrp    x1,FalseFALSE@PAGE
        add     x1,x1,FalseFALSE@PAGEOFF
        bl      comparestringnocase
        cmp     w0,#0
        beq     isFalse

        adrp    x0,inputbooleanbuffer@PAGE
        add     x0,x0,inputbooleanbuffer@PAGEOFF
        adrp    x1,False0@PAGE
        add     x1,x1,False0@PAGEOFF
        bl      comparestringnocase
        cmp     w0,#0
        beq     isFalse

; redo
        mov     x0,#2

        adrp    x1,BadInput@PAGE
        add     x1,x1,BadInput@PAGEOFF

        mov     x2,#BadInputLen

        mov     x16,#4
        svc     #0x80

        b       again

isTrue:
        mov     w3,#1
        b       saveit

isFalse:
        mov     w3,#0
saveit:
        adrp    x1,inputboolean_p1@PAGE
        add     x1,x1,inputboolean_p1@PAGEOFF
        strb    w3,[x1]

; epilogue: tear down stack and return
epilogue:
        ldp     x29,x30,[sp],16
        ret
