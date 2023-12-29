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

void kernel_main(void) {
    int a = 0;

    char *video = (char *) 0xb8000;
    *video = 'z';

    cursor_movement();

    printk_main();

    check_cpu();

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