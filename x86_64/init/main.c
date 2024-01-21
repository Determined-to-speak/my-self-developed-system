//
// Created by lizhijiangjiang/Determined-to-speak on 24-1-13.
//

#include "../include/x86_64/mm/memory.h"
#include "../include/x86_64/printk.h"

/**
 * 注意汇编调用C语言的函数，不能加static关键字修饰
 */
void x64_main() {

    console_init();
    printk("enter the x64 \n");

    //region 内存分配与检测
    print_memory_info();

    physics_memory_init();

    int *p = get_free_page();
    printk("%x\n", p);

    p = get_free_page();
    printk("%x\n", p);

    free_page(p);

    p = get_free_page();
    printk("%x\n", p);
    //endregion

}