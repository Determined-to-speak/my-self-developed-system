//
// Created by lizhijiangjiang/Determined-to-speak on 24-1-18.
//

#ifndef x86_64_MEMORY_H
#define x86_64_MEMORY_H

#include "./bitmap.h"

#define PAGE_SIZE 4096

typedef struct ADRS {
    unsigned int base_addr_low;    //内存基地址的低32位
    unsigned int base_addr_high;   //内存基地址的高32位
    unsigned int length_low;       //内存块长度的低32位
    unsigned int length_high;      //内存块长度的高32位
    unsigned int type;             //描述内存块的类型
} ADRS;

typedef struct memory_info {
    unsigned short times;
    ADRS *data;
} memory_info;

typedef struct physics_memory_info {
    unsigned int addr_start;     // 非内核真实内存起始地址 一般是1M
    unsigned int addr_end;       // 非内核真实内存起始地址
    unsigned int valid_mem_size;
    unsigned int pages_total;    // 机器物理内存共多少page
    unsigned int pages_free;     // 机器物理内存还剩多少page
    unsigned int pages_used;     // 机器物理内存用了多少page
} physics_memory_info;

typedef struct {
    unsigned int addr_base;          // 可用物理内存开始位置（内存的type=1）
    unsigned int pages_total;        // 共有多少page
    bitmap bitmap;
    unsigned char *bitmap_buf;
} physics_memory_map;

void physics_memory_init();

void *get_free_page();

void free_page(void *p);

/**
 * 打印setup.asm里得到的内存信息
 */
void print_memory_info();

#endif //x86_64_MEMORY_H
