//
// Created by lizhijiangjiang/Determined-to-speak on 24-1-18.
//

#ifndef x86_64_MEMORY_H
#define x86_64_MEMORY_H

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

void print_memory_info();

#endif //x86_64_MEMORY_H
