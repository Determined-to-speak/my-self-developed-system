//
// Created by lizhijiangjiang on 23-12-17.
//


#include "stdlib.h"
#include "stdio.h"


int *test(int x, int y, int z) {
    int a = 0;
//    a += x;
    printf("end test function\r\n");
    return &a;
}

#pragma pack(1) //字节对齐
struct Student {
    int age;
    int level;
};


int main(int argc, char *argv[]) {
    unsigned char c1 = 0xff;
    char c2 = 0xff;

    printf("here,%d,%d \r\n", c1, c2);
    printf("%d \r\n", sizeof(c2));

    test(0, 0x11, 0x22);

    int i = 0x11223344;
    char arr[] = {0x44, 0x33, 0x22, 0x11};

    int *j = (int *) arr;
    // %p 是显示内存地址
    printf("%p\r\n%x\r\n", &i, *j);

    char str1[] = {'C', 'h', 'i', 'n', 'a'};
    char str2[] = "china";
    char *str3 = "china";

    printf(str1);
    printf("\r\n");
    *str2 = 'l';
    printf(str2);
    printf("\r\n");
    printf(str3);
    printf("\r\n");

    char *test1 = (char *) test;
    char *test2 = (char *) &test;
    printf("%p,%p\n", test1, test2);

    //测试函数指针
    typedef int *(*task_fun_t)(int x, int y, int z);
    task_fun_t task = test;
    task(0, 0x11, 0x22);

    Student *s = (Student *) calloc(1, sizeof(Student));
    s->age = 1;
    s->level = 10;

    return 0;
}

