//
// Created by lizhijiangjiang/Determined-to-speak on 24-1-28.
//

#include "../include/x86_64/idt.h"
#include "../include/x86_64/printk.h"
#include "../include/x86_64/asm/myio.h"

static idt_x64_entry idt[255] = {0};

static idtr_x64 idtrx64;

#define PIC_M_CTRL  0x20    // 主片的控制端口
#define PIC_M_DATA  0x21    // 主片的数据端口
#define PIC_S_CTRL  0xa0    // 从片的控制端口
#define PIC_S_DATA  0xa1    // 从片的数据端口
#define PIC_EOI     0x20    // 通知中断控制器中断结束

void send_eoi(int idt_index) {
    if (idt_index >= 0x20 && idt_index < 0x28) {
        my_out_byte(PIC_M_CTRL, PIC_EOI);
    } else if (idt_index >= 0x28 && idt_index < 0x30) {
        my_out_byte(PIC_M_CTRL, PIC_EOI);
        my_out_byte(PIC_S_CTRL, PIC_EOI);
    }
}


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
    send_eoi(0x20);
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
        if (i < 0x1f) {
            install_idt(i, interrupt_handler_table[i], 0x18, 0, 0);
        } else {
            install_idt(i, (long long) &general_interrupt_handler, 0x18, 0, 0);
        }
    }
    install_idt(0x20, (long long) &clock_interrupt, 0x18, 0, 0);
    install_idt(0x21, (long long) &keymap_interrupt, 0x18, 0, 0);
    install_idt(0x28, (long long) &rtc_interrupt, 0x18, 0, 0);

    idtrx64.limit = sizeof(idt) - 1;
    idtrx64.base = &idt;

    __asm__ volatile ("xchg %bx, %bx;");
    __asm__ volatile ("lidt idtrx64;");
}