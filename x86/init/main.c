//
// Created by lizhijiangjiang/Determined-to-speak on 23-12-22.
//

#include "../include/x86/console.h"
#include "../../x86/include/x86/myio.h"

extern char first_char;

//extern char in_byte(int port);
//
//extern short in_word(int port);
//
//extern void out_byte(int port, int v);
//
//extern void out_word(int port, int v);

void printk_main();

void kernel_main(void) {
    int a = 0;

    char *video = (char *) 0xb8000;
    *video = 'z';

    printk_main();

    return;
}


void printk_main() {

    console_init();

    char *s = "lzj";

    //region    测试光标
    ushort position = 130;
    uchar current_high_position = position >> 8;
    uchar current_low_position = position & 0xff;

    my_out_byte(0x3D4, 0x0e);  //高位
    my_out_word(0x3D5, current_high_position);

    my_out_byte(0x3D4, 0x0f);
    my_out_word(0x3D5, current_low_position);

    while (true);
    //endregion

}