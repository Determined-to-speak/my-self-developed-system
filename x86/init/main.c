//
// Created by lizhijiangjiang/Determined-to-speak on 23-12-22.
//

#include "../include/x86/console.h"
#include "../../x86/include/x86/myio.h"
#include "../kernel/printk.c"
#include "../kernel/gdt.c"

//当前字符写在汇编文件中
extern char first_char;

//当前方法写在汇编文件中
extern void check_cpu();

void printk_main();

void cursor_movement();

void enter_x64();

void prepare_4levelpage_table();

void kernel_main(void) {
    int a = 0;

    char *video = (char *) 0xb8000;
    *video = 'z';

    cursor_movement();

    printk_main();

    //检查64位CPU是否支持
    check_cpu();
    //开始进入64位模式
    enter_x64();

    while (true);

}

void cursor_movement() {
    console_init();

    //region    测试光标
    ushort position = 130;
    uchar current_high_position = position >> 8;
    uchar current_low_position = position & 0xff;

    my_out_byte(0x3D4, 0x0e);  //高位
    my_out_word(0x3D5, current_high_position);

    my_out_byte(0x3D4, 0x0f);
    my_out_word(0x3D5, current_low_position);
    //endregion
}

void printk_main() {
    console_init();

    char *str = "lzj";
    printk("name: %s\n", str);
}

void enter_x64() {

    //准备4级页表
    prepare_4levelpage_table();

    //开启PAE位
    //MOV EAX, CR4        ; 将CR4寄存器的当前值加载到EAX寄存器
    //OR EAX, 0x20        ; 设置第5位（PAE位）
    //MOV CR4, EAX        ; 将修改后的值写回CR4寄存器
    __asm__ volatile("mov eax, cr4;"
                     "bts eax, 5;"
                     "mov cr4, eax;");

    //启用IA32_EFER寄存器的LME（长模式使能）位
    __asm__ volatile("mov ecx, 0x0c0000080;"
                     "rdmsr;"
                     "bts eax, 8;" //这里修改的是第8位
                     "wrmsr;");

    //将CR0寄存器中的最高位（PE）置为1，从而开启分页
    __asm__ volatile("mov eax, cr0;"
                     "or eax, 0x80000000;"
                     "mov cr0, eax;");

    //加载64位的段选择子
    load_x64_segment_descriptor();

    //长跳
//    __asm__ volatile("push 0x0018;"
//                     "push 0x100000;"
//                     "retf;");
    __asm__ volatile("jmp 0x0018:0x100000");

}

#define FOUR_LEVEL_HEAD_TABLE_ADDR 0x8000
#define FIRST_PDPT_ADDR 0x9000
#define FIRST_PDT_ADDR 0xa000

/**
 * 构建4级页表  重要！！！！！
 */
void prepare_4levelpage_table() {

    // 准备4级头表
    int *four_level_head_table_addr = (int *) FOUR_LEVEL_HEAD_TABLE_ADDR;
    memset(four_level_head_table_addr, 0, 4096);

    *four_level_head_table_addr = FIRST_PDPT_ADDR | 3;
    *(four_level_head_table_addr + 1) = 0;

    // 页目录指针表
    int *pdpt_addr = (int *) FIRST_PDPT_ADDR;
    memset(pdpt_addr, 0, 4096);

    *pdpt_addr = FIRST_PDT_ADDR | 3;
    *(pdpt_addr + 1) = 0;

    // 页目录表
    int *pdt_addr = (int *) FIRST_PDT_ADDR;
    memset(pdt_addr, 0, 4096);

    //TODO 这里需要搞清楚为什么这样写
    //采用2M分页,这里直接填写低端2M内存映射
    *pdt_addr = 0 | 0x83;
    *(pdt_addr + 1) = 0;

    // 2M-4M
    *(pdt_addr + 2) = 0x200000 | 0x83;
    *(pdt_addr + 3) = 0;

    // 直接映射0 - 64M
//    for (int i = 0; i < 50; ++i) {
//        *(pdt_addr + i * 2) = 0x200000 * i | 0x83;  //这里页目录表的后面直接跟了2M的物理页，这时需要将第七位置为1，所以是0x83
//        *(pdt_addr + i * 2 + 1) = 0;
//    }

    __asm__ volatile("xchg bx, bx; mov cr3, ebx"::"b"(four_level_head_table_addr));

}