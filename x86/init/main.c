//
// Created by lizhijiangjiang/Determined-to-speak on 23-12-22.
//

char extern first_char;

void kernel_main(void) {
    int a = 0;

    char* video = (char*)0xb8000;
    *video = 'z';

    first_char;

    return;
}