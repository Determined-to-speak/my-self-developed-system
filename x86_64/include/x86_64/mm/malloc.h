//
// Created by lizhijiangjiang/Determined-to-speak on 24-1-22.
//

#ifndef x86_64_MALLOC_H
#define x86_64_MALLOC_H

/**
 * 分配内存
 * @param size
 * @return
 */
void *kmalloc(unsigned int size);

/**
 * 释放内存
 * @param obj
 * @param size
 */
void kfree_s(void *obj, int size);

#endif //x86_64_MALLOC_H
