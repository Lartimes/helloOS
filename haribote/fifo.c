//
// Created by Lartimes on 2024/8/6.
//
#include "bootpack.h"

/**
 * 初始化FIFO
 * @param fifo
 * @param size
 * @param buf
 */
void fifo8_init(struct FIFO8 *fifo, int size, unsigned char *buf) {
//    初始化FIFO 缓冲区
    fifo->q = 0;
    fifo->p = 0;
    fifo->size = size;
    fifo->free = size;
    fifo->buf = buf;
    fifo->flags = 0;
    return;
}

/**
 * FIFO 缓存写入data
 * @param fifo
 * @param data
 * @return
 */
int fifo8_put(struct FIFO8 *fifo, unsigned char data) {
    if (fifo->free == 0) {
        fifo->flags |= FLAGS_OVERRUN;
        //溢出
        return -1;
    }
    fifo->buf[fifo->p++] = data;
    if (fifo->p == fifo->size) {
        fifo->p = 0;
    }
    fifo->free--;
    return 0;
}

/**
 * fifo缓存读取data
 * @param fifo
 * @return
 */
int fifo8_get(struct FIFO8 *fifo) {
    if (fifo->free == fifo->size) {
        return -1;
    }
    int data = fifo->buf[fifo->q++];
    if (fifo->q == fifo->size) {
        fifo->q = 0;
    }
    fifo->free++;
    return data;
}

/**
 * fifo是否free
 * @param fifo
 * @return
 */
int fifo8_status(struct FIFO8 *fifo)
/* 报告一下到底积攒了多少数据 */
{
    return fifo->size - fifo->free;
}
