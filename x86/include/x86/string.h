//
// Created by lizhijiangjiang/Determined-to-speak on 23-12-22.
//

#ifndef x86_STRING_H
#define x86_STRING_H

#include "./linux/types.h"

char* strcpy(char *dest, const char *src);

char* strcat(char *dest, const char *src);

size_t strlen(const char *str);

int strcmp(const char *lhs, const char *rhs);

char* strchr(const char *str, int ch);

char* strrchr(const char *str, int ch);

int memcmp(const void *lhs, const void *rhs, size_t count);

void* memset(void *dest, int ch, size_t count);

void* memcpy(void *dest, const void *src, size_t count);

void* memchr(const void *ptr, int ch, size_t count);

#endif //x86_STRING_H
