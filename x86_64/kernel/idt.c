//
// Created by lizhijiangjiang/Determined-to-speak on 24-1-28.
//

#include "../include/x86_64/idt.h"
#include "../include/x86_64/printk.h"

idt_x64_entry idt[255] = {0};

idtr_x64 idtrx64;

/**
 * 普通异常中断
 */
void general_interrupt_handler() {
    printk("start general_interrupt_handler\n");
    while (true);
}

/**
 * 时钟中断
 */
void clock_interrupt_handler() {
    printk("start clock_interrupt_handler\n");
}


void install_idt(int index, long long handler, short selector, char ist, char dpl) {
    idt_x64_entry *idtX64Entry = &idt[index];

    idtX64Entry->offset0 = handler & 0xffff;
    idtX64Entry->selector = selector;
    idtX64Entry->ist = ist;
    idtX64Entry->type = 0b1110;
    idtX64Entry->segment = 0;
    idtX64Entry->dpl = dpl;
    idtX64Entry->present = 1;
    idtX64Entry->offset1 = handler >> 16 & 0xffff;
    idtX64Entry->offset2 = handler >> 32 & 0xffffffff;
}


/**
 * 初始化idt表
 */
void idt_init() {

    memcpy(&idt, 0, 255);
    for (int i = 0; i < 255; ++i) {
        install_idt(i, &general_interrupt_handler, 0x18, 0, 0);
    }
    install_idt(0x20, &clock_interrupt, 0x18, 0, 0);

    idtrx64.limit = sizeof(idt) - 1;
    idtrx64.base = &idt;

    __asm__ volatile ("xchg %bx, %bx;");
    __asm__ volatile ("lidt idtrx64;");
}