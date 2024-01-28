//
// Created by lizhijiangjiang/Determined-to-speak on 24-1-28.
//

#include "../include/x86_64/idt.h"
#include "../include/x86_64/printk.h"

unsigned long long idt[255] = {0};

idtr_x64 idtrx64;

/**
 * 普通异常中断
 */
void general_interrupt_handler() {
    printk("start general_interrupt_handler\n");
    while (true);
}

/**
 * 初始化idt表
 */
void idt_init() {
    long long interrupt_handler_addr = general_interrupt_handler;

    for (int i = 0; i < 255; ++i) {
        idt_x64_entry *idtX64Entry = &idt[i];

        idtX64Entry->offset0 = interrupt_handler_addr & 0xffff;
        idtX64Entry->selector = 0x18;
        idtX64Entry->ist = 0;
        idtX64Entry->type = 0b1110;
        idtX64Entry->segment = 0;
        idtX64Entry->dpl = 0;
        idtX64Entry->present = 1;
        idtX64Entry->offset1 = interrupt_handler_addr >> 16 & 0xffff;
        idtX64Entry->offset2 = interrupt_handler_addr >> 32 & 0xffffffff;
    }

    idtrx64.base = &idt;
    idtrx64.limit = sizeof(idt) - 1;

    __asm__ volatile ("xchg %bx, %bx;");
    __asm__ volatile ("lidt idtrx64;");
}