//
// Created by lizhijiangjiang/Determined-to-speak on 23-12-22.
//

#include "../include/x86/console.h"
#include "../../x86/include/x86/myio.h"
#include "../kernel/printk.c"

//当前字符写在汇编文件中
extern char first_char;

//当前方法写在汇编文件中
extern void check_cpu();

void printk_main();

void cursor_movement();

int printk(const char *fmt, ...);

void enter_x64();

void prepare_4levelpage_table();

void load_x64_segment_descriptor();

void kernel_main(void) {
    int a = 0;

    char *video = (char *) 0xb8000;
    *video = 'z';

    cursor_movement();

    printk_main();

    check_cpu();

    enter_x64();

    return;
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
    __asm__ volatile("MOV EAX, CR4;"
                     "OR EAX, 0x20;"
                     "MOV CR4, EAX;");

    //启用IA32_EFER寄存器的LME（长模式使能）位
    __asm__ volatile("mov rax, 0xC0000080;"
                     "rdmsr;"
                     "or rax, 0x100;" //这里修改的是第8位
                     "wrmsr");

    //将CR0寄存器中的最高位（PE）置为1，从而开启分页
    __asm__ volatile("mov eax, cr0;"
                     "or eax, 0x80000000"
                     "mov cr0, eax");

    load_x64_segment_descriptor();

    //长跳
    __asm__ volatile("xchg bx, bx;"
                     "push 0x0018;"
                     "push 0x100000;"
                     "retf;");
}

void prepare_4levelpage_table() {

}