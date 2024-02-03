[SECTION .text]
[bits 64]

extern x64_main

extern vsprintf
extern strlen
extern console_write
extern printk_buff

global x64_start
x64_start:

    xchg bx, bx

;下面开启8259a的过程一般是固定的代码
.init_8259:

    ;向主发送ICW1
    mov al, 11h
    out 20h, al

    ;向从发送ICW1
    out 0a0h, al

    ;向主发送ICW2
    mov al, 20h			;主片中断号从0x20开始
    out 21h, al

    ;向从发送ICW2
    mov al, 28h			;从片中断号从0x28开始
    out 0a1h, al

    ;向主发送ICW3
    mov al, 04h			;主片的级联位是3，其他位为0，所以是0x04
    out 21h, al

    ;向从发送ICW3
    mov al, 02h			;从片的级联位是2，其他位位0，所以是0x02
    out 0a1h , al

    ;向主发送ICW4
    mov al, 003h
    out 21h, al

    ;向从发送ICW4
    out 0a1h, al

    ;屏蔽所有中断，只接收键盘中断
.enable_8259a_main:
    ;mov al, 11111110b  ;时钟中断
    ;mov al, 11111111b   ;仅剩CPU内部中断
    ;mov al, 11111101b  ;键盘中断
    mov al, 11111001b  ;键盘中断和RTC中断
    out 21h, al

    ;将从片与主片相连
.disable_8259a_slave:
    mov al, 11111101b       ;这里需要特别注意，第0位RTC电路的标志位是在后面打开，这里先放过去
    out 0a1h, al

.enbale_rtc:
    mov al, 0x8b    ;0x8b代表使用寄存器B，并阻断NMI
    out 0x70, al

    ;mov al, 0x12    ;寄存器的第一位和第四位设置为1
    mov al, 0x52     ;寄存器的第一位，第四位，第六位设置为1
    out 0x71, al

    mov al, 0x8a      ;CMOS寄存器A
    out 0x70, al
    mov al, 0x2f      ;32kHz，500ms的周期性中断
    out 0x71, al      ;写回CMOS寄存器A

    mov al, 0x0c
    out 0x70, al
    in al, 0x71     ;这里先读上一次寄存器C，清除之前的状态

    in al, 0xa1
    and al, 0xfe    ;打开从片第0位的标志，从而可以使用RTC电路
    out 0xa1, al

.call_x64_main:
    call x64_main
    jmp $

; rsi           <---rsp
; rdx
; rcx
; r8
; r9
; rdi
global printk
printk:
    push rdi
    push r9
    push r8
    push rcx
    push rdx
    push rsi

    ; 调用vsprintf(buff, fmt, p);
    mov rsi, rdi            ; fmt
    mov rdi, printk_buff    ; buff
    mov rdx, rsp            ; 第一个参数在栈中的地址
    call vsprintf

    ; 调用console_write(buff, strlen(buff))
    mov rdi, printk_buff
    call strlen

    mov rdi, printk_buff
    mov rsi, rax
    call console_write

    ; 栈还原
    add rsp, 8 * 6

    ret