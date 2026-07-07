; Print carriage return
;
; Entry:
;   printcr_p0 - path
        .text
        .align  2
        .globl  printcr
printcr:
; prologue: set up the stack frame
        stp     x29,x30,[sp,-16]!
        mov     x29,sp

; get path (fd) into x0
        adrp    x0,printcr_p0@PAGE
        ldrb    w0,[x0,printcr_p0@PAGEOFF]

; get address into x1
        adrp    x1,cr@PAGE
        add     x1,x1,cr@PAGEOFF

        mov     x2,#2           ; count to write
        mov     x16,#4          ; syscall number (sys_write)
        svc     #0x80           ; call kernel

; epilogue: tear down stack and return
        ldp     x29,x30,[sp],16
        ret

        .data
cr:     .byte   13
        .byte   10
