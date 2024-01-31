[section .text]
[bits 64]

extern clock_interrupt_handler
extern keymap_handler

global clock_interrupt
clock_interrupt:
    call  clock_interrupt_handler
    iretq


global keymap_interrupt
keymap_interrupt:
    mov   rdi, 0x21
    call  keymap_handler
    iretq