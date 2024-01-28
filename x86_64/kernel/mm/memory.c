//
// Created by lizhijiangjiang/Determined-to-speak on 24-1-18.
//

#include "../../include/x86_64/mm/memory.h"
#include "../../include/x86_64/printk.h"

//setup.asm 内存检测结果地址
#define  MEMORY_TEST_TIME_ADDRESS  0x7e00
#define  MEMORY_TEST_BUFFER_ADDRESS  0x7e02

//把1M以下内存称为无效内存
#define VALID_MEMORY_FROM           0x100000
//物理内存从10M的位置开始用，前面的空间用来放内核了
#define PHY_MEMORY_USE_FROM         0xa00000
//可用内存区域
#define ZONE_VALID 1
//不可用区域
#define ZONE_RESERVED 2

physics_memory_info physicsMemoryInfo;  //物理内存信息
physics_memory_map physicsMemoryMap;    //物理内存位图管理

/**
 * 初始化 physicsMemoryInfo 和 physicsMemoryMap
 */
void physics_memory_init() {

    memory_info *memoryInfo = (memory_info *) MEMORY_TEST_TIME_ADDRESS;
    ADRS *adrs = (ADRS *) MEMORY_TEST_BUFFER_ADDRESS;

    for (int i = 0; i < 4; ++i) {   //这里直接取了第4条
        ADRS *tmp = adrs + i;
        if (tmp->type == 0) {
            break;
        }
        if (tmp->base_addr_low > 0 && tmp->type == ZONE_VALID) {
            physicsMemoryInfo.valid_mem_size = tmp->length_low;
            physicsMemoryInfo.addr_start = tmp->base_addr_low;
            physicsMemoryInfo.addr_end = tmp->base_addr_low + tmp->length_low;
        }
    }

    if (physicsMemoryInfo.addr_start != VALID_MEMORY_FROM) {
        printk("no valid physics memory\n");
        printk("\t %x, %x, %x\n", physicsMemoryInfo.valid_mem_size, physicsMemoryInfo.addr_start,
               physicsMemoryInfo.addr_end);
        return;
    }

    //这里右移12位的意思是 除以4k
    physicsMemoryInfo.pages_total = physicsMemoryInfo.valid_mem_size >> 12;
    physicsMemoryInfo.pages_used = 0;
    physicsMemoryInfo.pages_free = physicsMemoryInfo.pages_total - ((PHY_MEMORY_USE_FROM - VALID_MEMORY_FROM) >> 12);

    {
        physicsMemoryMap.addr_base = PHY_MEMORY_USE_FROM;
        physicsMemoryMap.bitmap_buf = (unsigned char *) 0x500;
        physicsMemoryMap.pages_total = physicsMemoryInfo.pages_free;

        memset(physicsMemoryMap.bitmap_buf, 0, 1024);

        int bit_length = physicsMemoryMap.pages_total / 8;
        bitmap_make(&physicsMemoryMap.bitmap, physicsMemoryMap.bitmap_buf, bit_length, 0);
    }

}

/**
 * 给下一个空间（0x1000）分配数据
 * @return
 */
void *get_free_page() {
    long long ret;

    int index = bitmap_scan(&physicsMemoryMap.bitmap, 1);

    //`10M + index << 12`
    //
    //		`0`
    //
    //		`0x1000`
    //
    //		`0x2000`
    //
    //		`0x3000`
    ret = physicsMemoryMap.addr_base + (index << 12);

    return (void *) ret;
}

/**
 * 释放某一页
 * @param p 需要释放的地址
 */
void free_page(void *p) {

    //这里右移12位的意思是 除以4k
    int index = ((int) p - physicsMemoryMap.addr_base) >> 12;

    bitmap_set(&physicsMemoryMap.bitmap, index, false);

}

/**
 * 打印之前setup.asm里得到的内存信息
 */
void print_memory_info() {

    memory_info *memoryInfo = (memory_info *) MEMORY_TEST_TIME_ADDRESS;
    ADRS *adrs = (ADRS *) MEMORY_TEST_BUFFER_ADDRESS;

    unsigned short times = (unsigned short) memoryInfo->times;

    printk("======start memory check info . times = %d =====\n", times);
    for (int i = 0; i < times; ++i) {

        //这里直接移动到下一个ADRS的结构体------->这是非常重要的思想！！
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