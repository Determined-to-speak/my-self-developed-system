//
// Created by lizhijiangjiang/Determined-to-speak on 24-1-13.
//

#include "../include/x86_64/memory.h"
#include "../include/x86_64/printk.h"

/**
 * 注意汇编调用C语言的函数，不能加static关键字修饰
 */
void x64_main() {

    printk("hello, x64, %d\n", 1231321);
    printk("hello, %s\n", "ziya");

//    __asm__ ("xchg %bx, %bx;");

//    print_memory_info();

    while (true) {
        __asm__ volatile("hlt;");
    }
}