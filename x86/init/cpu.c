//
// Created by lizhijiangjiang/Determined-to-speak on 23-12-28.
//

int printk(const char *fmt, ...);

void check_esp_value() {
    int data = 0;

    // 内联汇编：将ESP处的数据加载到寄存器中
    // 注意！！！！这里经过测试，不可以使用"mov %0, esp;"，否则获得到的数据只是一个地址！！！！
    __asm__ volatile(
            "mov eax, [esp];"
            "mov ebx, [eax];"// 将ESP的值移到寄存器
            "mov %0, ebx;"
            : "=r"(data)       // 输出操作数，将ESP的值保存到data变量中
            );

    // 打印ESP处的数据
    printk("Data at ESP: %d\n", data);
}