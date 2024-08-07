//
// Created by Lartimes on 2024/8/7.
//
#include "bootpack.h"

void enable_mouse(struct MOUSE_DEC *mouseDec) {
    /* 激活鼠标 */
    wait_KBC_sendready();
    io_out8(PORT_KEYCMD, KEYCMD_SENDTO_MOUSE);
    wait_KBC_sendready();
    io_out8(PORT_KEYDAT, MOUSECMD_ENABLE);
    mouseDec->phase = 0;  /* 进入到等待鼠标的0xfa的状态 */
    return; /* 顺利的话，键盘控制器会返回ACK(0xfa) */
}

int mouse_decode(struct MOUSE_DEC *mdec, int data) {
    if (mdec->phase == 0) {
        if (data == 0xfa) {
            mdec->phase = 1;
        }
        return 0;
    }
    if (mdec->phase == 1) {
        if ((data & 0xc8) == 0x08) {
            mdec->buf[0] = data;
            mdec->phase = 2;
        }
        return 0;
    }
    if (mdec->phase == 2) {

        mdec->buf[1] = data;
        mdec->phase = 3;
        return 0;
    }
    if (mdec->phase == 3) {
        mdec->buf[2] = data;
        mdec->phase = 1;
        mdec->btn = mdec->buf[0] & 0x07;
        mdec->x = mdec->buf[1];
        mdec->y = mdec->buf[2];
        if ((mdec->buf[0] & 0x10) != 0) {
            mdec->x |= 0xffffff00;
        }
        if ((mdec->buf[0] & 0x20) != 0) {
            mdec->y |= 0xffffff00;
        }
        mdec->y = -mdec->y; /* 鼠标的y方向与画面符号相反 */
        return 1;
    }
    return -1;


}
