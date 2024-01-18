//
// Created by lizhijiangjiang/Determined-to-speak on 24-1-18.
//

#include "../../include/x86_64/memory.h"
#include "../../include/x86_64/printk.h"

#define  MEMORY_TEST_TIME_ADDRESS  0x7e00

/**
 * 打印之前setup.asm里得到的内存信息
 */
void print_memory_info() {

    __asm__ ("xchg %bx, %bx;");

    memory_info *memoryInfo = (memory_info *) MEMORY_TEST_TIME_ADDRESS;
    ADRS *adrs = (ADRS *) (MEMORY_TEST_TIME_ADDRESS + 2);

    unsigned short times = memoryInfo->times;

//    printk("======start memory check info =====\n");
    for (int i = 0; i < times; ++i) {

        __asm__ ("xchg %bx, %bx;");

        //这里直接移动到下一个ADRS的结构体
        ADRS *tmp = adrs + i;

        printk("\t %x, %x, %x, %x, %d\n", tmp->base_addr_high, tmp->base_addr_low,
               tmp->length_high, tmp->length_low, tmp->type);
    }
//    printk("======finish memory check info =====\n");

}