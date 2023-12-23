//
// Created by ziya on 23-12-17.
//


#include "../include/x86/string.h"
#include "../include/x86/linux/types.h"

/**
 * 字符串复制
 * @param dest 目标字符串
 * @param src 源字符串
 * @return
 */
char *strcpy(char *dest, const char *src) {
    char *ptr = dest;
    while (true) {
        *ptr++ = *src;
        if (*src++ == EOS)
            return dest;
    }
}

/**
 * 字符串拼接函数
 * @param dest  目标字符串
 * @param src   源字符串
 * @return
 */
char *strcat(char *dest, const char *src) {
    char *ptr = dest;
    while (*ptr != EOS) {
        ptr++;
    }
    while (true) {
        *ptr++ = *src;
        if (*src++ == EOS) {
            return dest;
        }
    }
}

/**
 * 计算字符串的长度
 * @param str 字符串
 * @return
 */
size_t strlen(const char *str) {
    char *ptr = (char *) str;
    while (*ptr != EOS) {
        ptr++;
    }
    return ptr - str;
}

/**
 * 比较字符串的大小
 * @param lhs
 * @param rhs
 * @return
 */
int strcmp(const char *lhs, const char *rhs) {
    while (*lhs == *rhs && *lhs != EOS && *rhs != EOS) {
        lhs++;
        rhs++;
    }
    return *lhs < *rhs ? -1 : *lhs > *rhs;
}

/**
 * strchr() 用于查找字符串中的一个字符，并返回该字符在字符串中第一次出现的位置。
 * @param str
 * @param ch
 * @return
 */
char *strchr(const char *str, int ch) {
    char *ptr = (char *) str;
    while (true) {
        if (*ptr == ch) {
            return ptr;
        }
        if (*ptr++ == EOS) {
            return NULL;
        }
    }
}

/**
 * 在参数 str 所指向的字符串中搜索最后一次出现字符 ch（一个无符号字符）的位置。
 * @param str
 * @param ch
 * @return
 */
char *strrchr(const char *str, int ch) {
    char *last = NULL;
    char *ptr = (char *) str;
    while (true) {
        if (*ptr == ch) {
            last = ptr;
        }
        if (*ptr++ == EOS) {
            return last;
        }
    }
}

/**
 * 把 lhs 和 rhs 的前 n 个字节进行比较。
 * @param lhs
 * @param rhs
 * @param count
 * @return
 */
int memcmp(const void *lhs, const void *rhs, size_t count) {
    char *lptr = (char *) lhs;
    char *rptr = (char *) rhs;
    while (*lptr == *rptr && count-- > 0) {
        lptr++;
        rptr++;
    }
    return *lptr < *rptr ? -1 : *lptr > *rptr;
}

/**
 * 复制字符 c（一个无符号字符）到参数 str 所指向的字符串的前 n 个字符。
 * @param dest
 * @param ch
 * @param count
 * @return
 */
void *memset(void *dest, int ch, size_t count) {
    char *ptr = dest;
    while (count--) {
        *ptr++ = ch;
    }
    return dest;
}

/**
 * 从 src 复制 count 个字符到 dest。
 * @param dest
 * @param src
 * @param count
 * @return
 */
void *memcpy(void *dest, const void *src, size_t count) {
    char *ptr = dest;
    while (count--) {
        *ptr++ = *((char *) (src++));
    }
    return dest;
}

/**
 * 在参数 str 所指向的字符串的前 n 个字节中搜索第一次出现字符 c（一个无符号字符）的位置。
 * @param str
 * @param ch
 * @param count
 * @return
 */
void *memchr(const void *str, int ch, size_t count) {
    char *ptr = (char *) str;
    while (count--) {
        if (*ptr == ch) {
            return (void *) ptr;
        }
        ptr++;
    }
}