//
// Created by lizhijiangjiang/Determined-to-speak on 24-1-28.
//

#ifndef x86_64_IDT_H
#define x86_64_IDT_H

#include "./linux/types.h"

typedef struct idtr_x64 {
    unsigned short limit;
    long long base;
}__attribute__((packed)) idtr_x64;


typedef struct idt_x64_entry {
    short offset0;
    short selector;
    char ist;
    char type: 4;       //任务门/中断门/陷阱门
    char segment: 1;    //segment = 0 表示系统段
    char dpl: 2;        //使用 int 指令访问的最低权限
    char present: 1;    //是否有效
    short offset1;
    int offset2;
    int reserved;
}__attribute__((packed)) idt_x64_entry;

void install_idt(int index, long long handler, short selector, char ist, char dpl);

void idt_init();

extern void clock_interrupt();

#endif //x86_64_IDT_H
