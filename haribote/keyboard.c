//
// Created by Lartimes on 2024/8/7.
//
#include "bootpack.h"

/**
 * 等待指令处理完毕/ 仍然要等到下一个命令能够送来为止
 */
void wait_KBC_sendready(void) {
    /* 等待键盘控制电路准备完毕 */
    for (;;) {
        if ((io_in8(PORT_KEYSTA) & KEYSTA_SEND_NOTREADY) == 0) {
            break;
        }
    }
    return;
}

void init_keyboard(void) {
    /* 初始化键盘控制电路 */
    wait_KBC_sendready();
    io_out8(PORT_KEYCMD, KEYCMD_WRITE_MODE);
    wait_KBC_sendready();
    io_out8(PORT_KEYDAT, KBC_MODE);
    return;
}
