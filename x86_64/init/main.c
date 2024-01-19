//
// Created by lizhijiangjiang/Determined-to-speak on 24-1-13.
//

#include "../include/x86_64/memory.h"
#include "../include/x86_64/printk.h"

/**
 * 注意汇编调用C语言的函数，不能加static关键字修饰
 */
void x64_main() {
    console_init();
    printk("enter the x64 \n");

    print_memory_info();

    while (true) {
        __asm__ volatile("hlt;");
    }
}