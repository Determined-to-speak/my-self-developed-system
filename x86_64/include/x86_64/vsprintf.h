//
// Created by lizhijiangjiang/Determined-to-speak on 24-1-18.
//

#ifndef x86_64_VSPRINTF_H
#define x86_64_VSPRINTF_H

#include "./asm/system.h"
#include "./string.h"
#include "./stdarg.h"

int vsprintf(char *buf, const char *fmt, va_list args);

#endif //x86_64_VSPRINTF_H
