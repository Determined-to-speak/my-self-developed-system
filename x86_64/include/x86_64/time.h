//
// Created by lizhijiangjiang/Determined-to-speak on 24-2-4.
//

#ifndef x86_64_TIME_H
#define x86_64_TIME_H

typedef struct tm {
    int tm_sec;
    int tm_min;
    int tm_hour;
    int tm_mday;
    int tm_mon;
    int tm_year;
    int tm_wday;
    int tm_yday;
    int tm_isdst;
} __attribute__((packed)) tm;

long kernel_mktime(struct tm * tm);
void time_init(void);

#endif //x86_64_TIME_H
