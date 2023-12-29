[section .data]
start_chack_cpu_str: db "this is cpu_correction , let's check CPU is X64"  ,10,13,0
donot_support_x64_str: db "the cpu do not support x64" ,10,13,0
support_x64_str: db "the cpu supports x64" ,10,13,0
is_old_cpu_str: db "the cpu do not support ia32e" ,10,13,0

print_cpu_str_start: db "begin read cpu info..."
print_cpu_str: times 50 db 0
print_cpu_str_end: db 10,13,0

[section .text]
[Bits 32]

extern printk
extern printk_main
extern check_esp_value

global check_cpu
check_cpu:
    push start_chack_cpu_str
    call printk
    add  esp, 4     ;这里的意思就是删掉刚才push的数据

    mov eax, 0x80000000
    cpuid
    cmp eax, 0x80000008
    jb  .donot_support_x64

    ;检测是否支持ia32e
    mov eax, 0x80000001
    cpuid
    bt edx, 29
    jnc  .is_old_cpu

    push support_x64_str
    call printk
    add esp, 4      ;这里的意思就是删掉刚才push的数据

    mov eax, 0x80000002
    cpuid
    mov [print_cpu_str + 4 * 0], eax
    mov [print_cpu_str + 4 * 1], ebx
    mov [print_cpu_str + 4 * 2], ecx
    mov [print_cpu_str + 4 * 3], edx

    mov eax, 0x80000003
    cpuid
    mov [print_cpu_str + 4 * 4], eax
    mov [print_cpu_str + 4 * 5], ebx
    mov [print_cpu_str + 4 * 6], ecx
    mov [print_cpu_str + 4 * 7], edx

    push print_cpu_str_start
    call printk
    add esp, 4

    push print_cpu_str
    call printk
    add esp, 4

    push print_cpu_str_end
    call printk
    add esp, 4

.return:
    ret

.donot_support_x64:
    push donot_support_x64_str
    call printk
    add esp,4
    jmp $

.is_old_cpu:
    push is_old_cpu_str
    call printk
    add esp,4
    jmp $
