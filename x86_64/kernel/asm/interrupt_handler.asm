[section .text]
[bits 64]

extern clock_interrupt_handler

global clock_interrupt
clock_interrupt:
    call  clock_interrupt_handler
    iretq