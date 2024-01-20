//
// Created by lizhijiangjiang/Determined-to-speak on 24-1-19.
//

#ifndef x86_64_BITMAP_H
#define x86_64_BITMAP_H

#include "../linux/types.h"

/**
 * 位图       这里放到0x500的位置（以前的setup.asm的位置）
 *
 * 为了方便后续的计算，位图的指针使用 char *
 */
typedef struct bitmap {
    unsigned char *bits;    //位图开始
    unsigned int length;    //位图长度
    unsigned int offset;    //之前已经使用的偏移量
} bitmap;

/**
 * 初始化位图
 * @param map
 * @param bits
 * @param length
 * @param offset 开始计算的偏移量
 * @return
 */
void bitmap_init(bitmap *map, char *bits, int length, int offset);

/**
 * 构造位图
 * @param map
 * @param bits
 * @param length
 * @param offset 开始计算的偏移量
 */
void bitmap_make(bitmap *map, char *bits, int length, int offset);

/**
 * 测试某一位上是否为 1
 * @param map
 * @param index
 * @return
 */
bool bitmap_test(bitmap *map, int index);

/**
 * 设置位图中的某一位
 * @param map
 * @param index
 * @param value
 */
void bitmap_set(bitmap *map, int index, bool value);

/**
 * 从位图中得到连续的count位
 * @param map
 * @param count
 * @return
 */
int bitmap_scan(bitmap *map, int count);

#endif //x86_64_BITMAP_H
