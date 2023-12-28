[section .data]
start_chack_cpu: db "this is cpu_correction , let's check CPU is X64"  ,10,13,0


[section .text]
[Bits 32]

extern printk
extern printk_main
extern check_esp_value

global check_cpu
check_cpu:

xchg bx,bx
    push 0x3
    call printk
xchg bx,bx
    call check_esp_value
xchg bx,bx

    jmp $