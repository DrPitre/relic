; Input an integer
;
; Entry:
;   inputinteger_p0 - path
;   inputinteger_p1 - address of integer to input

        .text

badmsg:      .ascii  "** Input error - reenter **"
             .byte    0x0D,0x0A
badmsglen   = .-badmsg

        .globl  inputinteger
        .align  2
inputinteger:
; prologue: set up the stack frame
        stp     x29,x30,[sp,-16]!
        mov     x29,sp

; get path (fd) into x0
tryagain:
        adrp    x0,inputinteger_p0@PAGE
        ldrb    w0,[x0,inputinteger_p0@PAGEOFF]

; get address into x1
        adrp    x1,inputinteger_p1@PAGE
        add     x1,x1,inputinteger_p1@PAGEOFF

; get length of string in x2
        mov     x2,#6           ; -XXXXX maximum

        mov     x16,#3          ; syscall number (sys_read)
        svc     #0x80           ; call kernel

; call to convert
        adrp    x0,inputinteger_p1@PAGE
        add     x0,x0,inputinteger_p1@PAGEOFF
        bl      StrToDecS16
        cmp     x0,#0
        beq     saveit
        bl      RedoInput
        b       tryagain

saveit:
        adrp    x2,inputinteger_p1@PAGE
        add     x2,x2,inputinteger_p1@PAGEOFF
        strh    w1,[x2]

; epilogue: tear down stack and return
epilogue:
        ldp     x29,x30,[sp],16
        ret

RedoInput:
; prologue: set up the stack frame
        stp     x29,x30,[sp,-16]!
        mov     x29,sp

        mov     x0,#2           ; stderr
        adrp    x1,badmsg@PAGE
        add     x1,x1,badmsg@PAGEOFF
        mov     x2,#badmsglen
        mov     x16,#4
        svc     #0x80
        ldp     x29,x30,[sp],16
        ret
