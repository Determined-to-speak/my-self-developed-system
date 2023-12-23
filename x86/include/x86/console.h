//
// Created by lizhijiangjiang/Determined-to-speak on 23-12-23.
//

#ifndef x86_CONSOLE_H
#define x86_CONSOLE_H

#include "./linux/types.h"
#include "./string.h"

void console_init(void);

void console_write(char *buf, u32 count);

#endif //x86_CONSOLE_H
