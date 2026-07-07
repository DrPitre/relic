; Print a String
;
; Entry:
;   printstring_p0 - path
;   printstring_p1 - address of string to print
        .text
        .globl  printstring
        .align  2
printstring:
; prologue: set up the stack frame
        stp     x29,x30,[sp,-16]!
        mov     x29,sp

; get path (fd) into x0
        adrp    x0,printstring_p0@PAGE
        ldrb    w0,[x0,printstring_p0@PAGEOFF]

; get address into x1
        adrp    x1,printstring_p1@PAGE
        add     x1,x1,printstring_p1@PAGEOFF

; get length of string in x2
        mov     x2,#0           ; clear x2 (string length)
        mov     x7,x1           ; move pointer to string into x7
count:  ldrb    w6,[x7]
        cmp     w6,#0
        beq     lo
        add     x7,x7,#1
        add     x2,x2,#1
        b       count

lo:     mov     x16,#4          ; syscall number (sys_write)
        svc     #0x80           ; call kernel

; epilogue: tear down stack and return
        ldp     x29,x30,[sp],16
        ret
