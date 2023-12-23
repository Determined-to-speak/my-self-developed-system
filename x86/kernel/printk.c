//
// Created by lizhijiangjiang/Determined-to-speak on 23-12-23.
//

#include "../include/x86/printk.h"
#include "vsprintf.c"

static char buf[1024];

int printk(const char *fmt, ...) {
    char *args;
    int i;

    va_start(args,fmt);
    i = vsprintf(buf, fmt, args);

    va_end(args);

    console_write(buf, i);

    return i;
}