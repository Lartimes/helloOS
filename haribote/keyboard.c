//
// Created by Lartimes on 2024/8/7.
//
#include "bootpack.h"

struct FIFO32 *keyfifo;
int keydata0;

/**
 * 读取键盘指令
 * @param esp
 */
void inthandler21(int *esp)
{
    int data;
    io_out8(PIC0_OCW2, 0x61); /* 把IRQ-01接收信号结束的信息通知给PIC */
    data = io_in8(PORT_KEYDAT);
    fifo32_put(keyfifo, data + keydata0);
    return;
}

void wait_KBC_sendready(void)
{
    /* 等待键盘控制电路准备完毕 */
    for (;;) {
        if ((io_in8(PORT_KEYSTA) & KEYSTA_SEND_NOTREADY) == 0) {
            break;
        }
    }
    return;
}

void init_keyboard(struct FIFO32 *fifo, int data0)
{
    /* 将FIFO缓冲区的信息保存到全局变量里 */
    keyfifo = fifo;
    keydata0 = data0;
    /* 键盘控制器的初始化 */
    wait_KBC_sendready();
    io_out8(PORT_KEYCMD, KEYCMD_WRITE_MODE);
    wait_KBC_sendready();
    io_out8(PORT_KEYDAT, KBC_MODE);
    return;
}
