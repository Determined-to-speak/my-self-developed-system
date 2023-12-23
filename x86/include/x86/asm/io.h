//
// Created by lizhijiangjiang/Determined-to-speak on 23-12-23.
//

#ifndef x86_IO_H
#define x86_IO_H

extern char in_byte(int port);

extern short in_word(int port);

extern void out_byte(int port, int v);

extern void out_word(int port, int v);

#endif //SYSTEM_IO_H
