[section .data]
global first_char
first_char db 'l'

[section .text]
[BITS 32]
extern kernel_main

global head_start
head_start:

    xchg bx,bx
    ;TODO 这里踩到坑了，不知道为什么，这里不能使用int 0x10进行打印，具体的问题有时间抽空分析
    call kernel_main

    jmp $

jmp_to_head:
    db "success", 10, 13, 0