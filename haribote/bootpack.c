#include <stdio.h>

#define COL8_000000 0
#define COL8_FF0000 1
#define COL8_00FF00 2
#define COL8_FFFF00 3
#define COL8_0000FF 4
#define COL8_FF00FF 5
#define COL8_00FFFF 6
#define COL8_FFFFFF 7
#define COL8_C6C6C6 8
#define COL8_840000 9
#define COL8_008400 10
#define COL8_848400 11
#define COL8_000084 12
#define COL8_840084 13
#define COL8_008484 14
#define COL8_848484 15

void io_hlt(void);

void io_cli(void);

void io_out8(int port, int data);

int io_load_eflags(void);

void io_store_eflags(int eflags);

void init_palette(void);

void set_palette(int start, int end, unsigned char *rgb);

void boxfill8(unsigned char *vram, int xsize, unsigned char c, int x0, int y0, int x1, int y1);

void init_screen(char *vram, short scrnx, short scrny);

void putfont8(char *vram, int xsize, int x, int y, char c, unsigned char *font);

void putfonts8_asc(char *vram, int xsize, int x, int y, char c, unsigned char *font);

void init_mouse_cursor8(char *mouse, char bc);

void putblock8_8(char *vram, int vxsize, int pxsize, int pysize, int px0, int py0, char *buf, int bxsize);

/**
 * 启动扇区 画面扇区信息
 */
struct BOOTINFO {
    char cyls, leds, vmode, reserver;
    short scrnx, scrny;
    char *vram;
};


/**
 * 主程序MAIN 入口
 */
void HariMain(void) {
    struct BOOTINFO *binfo;
    init_palette(); /* 设定调色板 */
    binfo = (struct BOOTINFO *) 0x0ff0;
    char mcursor[16 * 16];
    char s[40];
    int mx, my;
    init_screen(binfo->vram, binfo->scrnx, binfo->scrny);

    putfonts8_asc(binfo->vram, binfo->scrnx, 8, 8, COL8_FFFFFF, "ABC 123");
    putfonts8_asc(binfo->vram, binfo->scrnx, 31, 31, COL8_000000, "Haribote OS.");
    putfonts8_asc(binfo->vram, binfo->scrnx, 30, 30, COL8_FFFFFF, "Haribote OS.");

    sprintf(s, "scrnx = %d", binfo->scrnx);
    putfonts8_asc(binfo->vram, binfo->scrnx, 16, 64, COL8_FFFFFF, s);

    init_mouse_cursor8(mcursor, COL8_008484);
    /* 显示鼠标 */
    mx = (binfo->scrnx - 16) / 2; /* 计算画面的中心坐标*/
    my = (binfo->scrny - 28 - 16) / 2;
    putblock8_8(binfo->vram, binfo->scrnx, 16, 16, mx, my, mcursor, 16);

    for (;;) {
        io_hlt();
    }
}


/**
 * 放入字体包装方法
 * @param vram
 * @param xsize
 * @param x
 * @param y
 * @param c
 * @param s
 */
void putfonts8_asc(char *vram, int xsize, int x, int y, char c, unsigned char *s) {
    extern char hankaku[4096];
    for (; *s != 0x00; s++) {
        putfont8(vram, xsize, x, y, c, hankaku + *s * 16);
        x += 8;
    }
    return;
}

/**
 * 初始化鼠标指针
 * @param mouse
 * @param bc
 */
void init_mouse_cursor8(char *mouse, char bc) {

    static char cursor[16][16] = {"**************..", "*OOOOOOOOOOO*...", "*OOOOOOOOOO*....", "*OOOOOOOOO*.....",
                                  "*OOOOOOOO*......", "*OOOOOOO*.......", "*OOOOOOO*.......", "*OOOOOOOO*......",
                                  "*OOOO**OOO*.....", "*OOO*..*OOO*....", "*OO*....*OOO*...", "*O*......*OOO*..",
                                  "**........*OOO*.", "*..........*OOO*", "............*OO*", ".............***"};
    int x, y;
    for (y = 0; y < 16; y++) {
        for (x = 0; x < 16; x++) {
            if (cursor[y][x] == '*') {
                mouse[y * 16 + x] = COL8_000000;
            }
            if (cursor[y][x] == 'O') {
                mouse[y * 16 + x] = COL8_FFFFFF;
            }
            if (cursor[y][x] == '.') {
                mouse[y * 16 + x] = bc;
            }
        }
    }
    return;

}

/**
 * 渲染背景
 * @param vram 0xa0000
 * @param vxsize 320
 * @param pxsize  16
 * @param pysize  16
 * @param px0 x
 * @param py0  y
 * @param buf cursor_char_arr
 * @param bxsize  16
 */
void putblock8_8(char *vram, int vxsize, int pxsize, int pysize, int px0, int py0, char *buf, int bxsize) {
    int y , x ;
    for ( y = 0; y < pysize; ++y) {
        for ( x = 0; x < pxsize; ++x) {
            vram[(py0 + y) * vxsize + (px0 + x)] = buf[y * bxsize + x];
        }
    }
    return;

}

/**
 * 初始化屏幕画板
 * @param vram
 * @param scrnx
 * @param scrny
 */
void init_screen(char *vram, short scrnx, short scrny) {
    int xsize, ysize;
    xsize = (int) scrnx;
    ysize = (int) scrny;
    boxfill8(vram, xsize, COL8_008484, 0, 0, xsize - 1, ysize - 29);
    boxfill8(vram, xsize, COL8_C6C6C6, 0, ysize - 28, xsize - 1, ysize - 28);
    boxfill8(vram, xsize, COL8_FFFFFF, 0, ysize - 27, xsize - 1, ysize - 27);
    boxfill8(vram, xsize, COL8_C6C6C6, 0, ysize - 26, xsize - 1, ysize - 1);
    boxfill8(vram, xsize, COL8_FFFFFF, 3, ysize - 24, 59, ysize - 24);
    boxfill8(vram, xsize, COL8_FFFFFF, 2, ysize - 24, 2, ysize - 4);
    boxfill8(vram, xsize, COL8_848484, 3, ysize - 4, 59, ysize - 4);
    boxfill8(vram, xsize, COL8_848484, 59, ysize - 23, 59, ysize - 5);
    boxfill8(vram, xsize, COL8_000000, 2, ysize - 3, 59, ysize - 3);
    boxfill8(vram, xsize, COL8_000000, 60, ysize - 24, 60, ysize - 3);
    boxfill8(vram, xsize, COL8_848484, xsize - 47, ysize - 24, xsize - 4, ysize - 24);
    boxfill8(vram, xsize, COL8_848484, xsize - 47, ysize - 23, xsize - 47, ysize - 4);
    boxfill8(vram, xsize, COL8_FFFFFF, xsize - 47, ysize - 3, xsize - 4, ysize - 3);
    boxfill8(vram, xsize, COL8_FFFFFF, xsize - 3, ysize - 24, xsize - 3, ysize - 3);
    return;
}


/**
 * 8 * 16 像素字体
 */
void putfont8(char *vram, int xsize, int x, int y, char c, unsigned char *font) {
    int i;
    char *p, d /* data */;
    for (i = 0; i < 16; i++) {
        p = vram + (y + i) * xsize + x;
        d = font[i];
        if ((d & 0x80) != 0) { p[0] = c; }
        if ((d & 0x40) != 0) { p[1] = c; }
        if ((d & 0x20) != 0) { p[2] = c; }
        if ((d & 0x10) != 0) { p[3] = c; }
        if ((d & 0x08) != 0) { p[4] = c; }
        if ((d & 0x04) != 0) { p[5] = c; }
        if ((d & 0x02) != 0) { p[6] = c; }
        if ((d & 0x01) != 0) { p[7] = c; }
    }
    return;
}


/**
 * 调色板
 */
void init_palette(void) {
    static unsigned char table_rgb[16 * 3] = {0x00, 0x00, 0x00,/*  0:黑 */
                                              0xff, 0x00, 0x00,   /*  1:亮红 */
                                              0x00, 0xff, 0x00,   /*  2:亮绿 */
                                              0xff, 0xff, 0x00,   /*  3:亮黄 */
                                              0x00, 0x00, 0xff,   /*  4:亮蓝 */
                                              0xff, 0x00, 0xff,   /*  5:亮紫 */
                                              0x00, 0xff, 0xff,   /*  6:浅亮蓝 */
                                              0xff, 0xff, 0xff,   /*  7:白 */
                                              0xc6, 0xc6, 0xc6,   /*  8:亮灰 */
                                              0x84, 0x00, 0x00,   /*  9:暗红 */
                                              0x00, 0x84, 0x00,   /* 10:暗绿 */
                                              0x84, 0x84, 0x00,   /* 11:暗黄 */
                                              0x00, 0x00, 0x84,   /* 12:暗青 */
                                              0x84, 0x00, 0x84,   /* 13:暗紫 */
                                              0x00, 0x84, 0x84,   /* 14:浅暗蓝 */
                                              0x84, 0x84, 0x84    /* 15:暗灰 */    };
    set_palette(0, 15, table_rgb);
    /* C语言中的static char语句只能用于数据，相当于汇编中的DB指令 */
    return;

}

/**
 * 对16种颜色存储寄存器
 * @param start
 * @param end
 * @param rgb
 */
void set_palette(int start, int end, unsigned char *rgb) {
    int i, eflags;
    eflags = io_load_eflags();  /* 记录中断许可标志的值*/
    io_cli();                   /* 将中断许可标志置为0，禁止中断 */
    io_out8(0x03c8, start);
    for (i = start; i <= end; i++) {
        io_out8(0x03c9, rgb[0] / 4);
        io_out8(0x03c9, rgb[1] / 4);
        io_out8(0x03c9, rgb[2] / 4);
        rgb += 3;
    }
    io_store_eflags(eflags);    /* 复原中断许可标志 */\
    return;
}

void boxfill8(unsigned char *vram, int xsize, unsigned char c, int x0, int y0, int x1, int y1) {
//     boxfill8(p, LENGTH, COL8_FF0000, 20, 20, 120, 120);
//    *p = 0xa0000 + x * 320 + y ;
    int x, y;
    for (y = y0; y <= y1; y++) {
        for (x = x0; x <= x1; x++)
            vram[y * xsize + x] = c;
    }
    return;
}


