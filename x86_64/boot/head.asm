[SECTION .text]
[bits 64]

extern x64_main

global x64_start
x64_start:
    xchg bx, bx
    call x64_main

    jmp $
