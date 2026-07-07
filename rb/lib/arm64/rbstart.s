# Ragin' Basic Compiler Start Code for ARM64 (Darwin)
               .text
               .align   2
               .globl    _start
_start:
               bl        main                ; call main routine
               mov       x0,#0                ; exit code
               mov       x16,#1                ; syscall number (sys_exit)
               svc       #0x80                ; call kernel
