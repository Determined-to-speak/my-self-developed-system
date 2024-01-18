//
// Created by lizhijiangjiang/Determined-to-speak on 23-12-29.
//

#include "../include/x86/gdt.h"
#include "../include/x86/printk.h"

unsigned long long gdt[256] = {0};

gdtr_x64 gdtrX64;

static void create_gdt_code(int index) {
    gdt_x64_entry *item = &gdt[index];

    item->limit_low = 0xffff;
    item->base_low = 0;
    item->base_middle = 0;
    item->type = 0b1000;
    item->s = 1;
    item->dpl = 0;
    item->p = 1;
    item->limit_middle = 0xff;
    item->avl = 0;
    item->long_mode = 1;
    item->db = 0;   //难受这里踩坑了，使用的是16位的段选择，db的值为0
    item->g = 0;
    item->base_high = 0;
}

static void create_gdt_data(int index) {
    gdt_x64_entry *item = &gdt[index];

    item->limit_low = 0xffff;
    item->base_low = 0;
    item->base_middle = 0;
    item->type = 0b0010;
    item->s = 1;
    item->dpl = 0;
    item->p = 1;
    item->limit_middle = 0xff;
    item->avl = 0;
    item->long_mode = 1;
    item->db = 0;   //难受,这里踩坑了，使用的是16位的段选择，db的值为0
    item->g = 0;
    item->base_high = 0;
}

/**
 * 非常重要的代码，加载64位的代码段
 */
void load_x64_segment_descriptor() {
    __asm__ volatile("sgdt gdtrX64;");
    printk("gdt: base:0x%x, limit:0x%x\n", gdtrX64.base, gdtrX64.limit);

    //这里通过memcpy方法来给gdt赋值的方法值得深度研究
    memcpy(&gdt, (void *) gdtrX64.base, gdtrX64.limit);
    create_gdt_code(3);
    create_gdt_data(4);

    gdtrX64.base = (int) &gdt;
    gdtrX64.limit = sizeof(gdt) - 1;

    __asm__ volatile ("xchg bx, bx;"
                      "lgdt gdtrX64;");
}