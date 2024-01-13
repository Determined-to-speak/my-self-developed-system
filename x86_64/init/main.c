//
// Created by lizhijiangjiang/Determined-to-speak on 24-1-13.
//

/**
 * 注意汇编调用C语言的函数，不能加static关键字修饰
 */
void x64_main() {
    *(int *) (0x8000 + 256 * 8) = 0x9000 | 3;
    *(int *) (0x8000 + 256 * 8 + 4) = 0;

    while (1);
}