//
// Created by lizhijiangjiang/Determined-to-speak on 24-1-19.
//

#include "../../include/x86_64/mm/bitmap.h"
#include "../../include/x86_64/string.h"

/**
 * 初始化位图
 * @param map
 * @param bits
 * @param length
 * @param offset
 * @return
 */
void bitmap_init(bitmap *map, char *bits, int length, int offset) {
    memset(bits, 0, length);
    bitmap_make(map, bits, length, offset);
}

/**
 * 构造位图
 * @param map
 * @param bits
 * @param length
 * @param offset
 */
void bitmap_make(bitmap *map, char *bits, int length, int offset) {
    map->bits = bits;
    map->length = length;
    map->offset = offset;
}

/**
 * 测试某一位上是否为 1
 *
 * 此方法需要反复阅读，揣摩内存中数据的存储
 *
 * @param map
 * @param index
 * @return
 */
bool bitmap_test(bitmap *map, int index) {
    index -= map->offset;
    int bytes = index / 8;  //在位图中的具体位置(具体是那个字节)
    char bits = index % 8;
    return (map->bits[bytes] & (1 << bits));
}

/**
 * 设置位图中的某一位
 * @param map
 * @param index
 * @param value 此位是否有值
 */
void bitmap_set(bitmap *map, int index, bool value) {
    index -= map->offset;
    int bytes = index / 8;  //在位图中的具体位置(具体是那个字节)
    char bits = index % 8;
    if (value) {
        map->bits[bytes] |= (1 << bits);
    } else {
        map->bits[bytes] &= ~(1 << bits);
    }
}

/**
 * 从位图中得到连续为空的数组下标
 * @param map
 * @param count
 * @return
 */
int bitmap_scan(bitmap *map, int count) {
    int start = -1;
    int bits_length = map->length * 8;  //转换为二进值后的长度
    int next_bit = 0;
    int counter = 0;    //计数器
    while (bits_length-- > 0) {
        if (!bitmap_test(map, map->offset + next_bit)) {
            counter++;
        } else {
            counter = 0;
        }
        next_bit++;
        if (counter == count) {
            start = next_bit - count;
            break;
        }
    }

    if (start == -1) {
        return -1;
    }
    next_bit = start;
    while (count-- > 0) {
        bitmap_set(map, next_bit, true);
        next_bit++;
    }
    return start + map->offset;
}