//
// Created by lizhijiangjiang/Determined-to-speak on 24-1-18.
//

#include "../../include/x86_64/mm/memory.h"
#include "../../include/x86_64/printk.h"

//MEMORY_TEST_TIME_ADDRESS equ 0x7e00
//MEMORY_TEST_BUFFER_ADDRESS equ 0x7e02

#define  MEMORY_TEST_TIME_ADDRESS  0x7e00
#define  MEMORY_TEST_BUFFER_ADDRESS  0x7e02

/**
 * 打印之前setup.asm里得到的内存信息
 */
void print_memory_info() {

    memory_info *memoryInfo = (memory_info *) MEMORY_TEST_TIME_ADDRESS;
    ADRS *adrs = (ADRS *) MEMORY_TEST_BUFFER_ADDRESS;

    unsigned short times = (unsigned short) memoryInfo->times;

    printk("======start memory check info . times = %d =====\n", times);
    for (int i = 0; i < times; ++i) {

        //这里直接移动到下一个ADRS的结构体
        ADRS *tmp = adrs + i;
        //TODO 这里有问题，查出来times的数据量很大，下面用type判断循环是否结束
        if (tmp->base_addr_high == 0 && tmp->base_addr_low == 0 && tmp->type == 0) {
            break;
        }

        printk("\t %x, %x, %x, %x, %d\n", tmp->base_addr_high, tmp->base_addr_low,
               tmp->length_high, tmp->length_low, tmp->type);
    }
    printk("======finish memory check info =====\n");

}