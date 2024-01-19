//
// Created by lizhijiangjiang/Determined-to-speak on 23-12-23.
//

#ifndef x86_64_STDARG_H
#define x86_64_STDARG_H

typedef char* va_list;

#define va_start(p, count) (p = (va_list)&count + sizeof(char*))
// 这行代码做了两件事情：1、修改p_args; 2、取值
#define va_arg(p, t) (*(t*)((p += sizeof(char*)) - sizeof(char*)))
#define va_end(p) (p = 0)

#endif //x86_64_STDARG_H
