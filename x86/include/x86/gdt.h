//
// Created by lizhijiangjiang/Determined-to-speak on 23-12-29.
//

#ifndef x86_GDT_H
#define x86_GDT_H

typedef struct gdtr_x64 {
    short limit;
    int base;
}__attribute__((packed)) gdtr_x64;


typedef struct gdt_x64_entry {
    unsigned short limit_low;
    unsigned short base_low;
    unsigned char base_middle;
    unsigned char type: 4;  //见笔记两个表示type域的图表
    unsigned char s: 1;     //代码段，数据段 or 系统段
    unsigned char dpl: 2;   //请求特权级
    unsigned char p: 1;     //有效位
    unsigned char limit_middle: 4;
    unsigned char avl: 1;   //是否可用
    unsigned char long_mode: 1; //1 长模式
    unsigned char db: 1;    //0-16bit，1-32bit
    unsigned char g: 1;     //单位 0-1b，1-4k
    unsigned char base_high;
}__attribute__((packed)) gdt_x64_entry;

#endif //x86_GDT_H
