//
// Created by lizhijiangjiang/Determined-to-speak on 23-12-23.
//

#ifndef x86_IO_H
#define x86_IO_H

extern char my_in_byte(int port);

extern short my_in_word(int port);

extern void my_out_byte(int port, int v);

extern void my_out_word(int port, int v);

#endif //x86_IO_H
