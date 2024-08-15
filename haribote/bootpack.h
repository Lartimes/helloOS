//
// Created by Lartimes on 2024/7/25.
//

#define ADR_BOOTINFO 0x00000ff0

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
/*asmhead.nas*/
/**
 * 启动扇区 画面扇区信息
 */
struct BOOTINFO {
    char cyls, leds, vmode, reserver;
    short scrnx, scrny;
    char *vram;
};
//fifo.c
#define FLAGS_OVERRUN 0x0001

struct FIFO32 {
    int *buf;
    int p, q, size, free, flags;
};

void fifo32_init(struct FIFO32 *fifo, int size, int *buf);

int fifo32_get(struct FIFO32 *fifo);

int fifo32_status(struct FIFO32 *fifo);

int fifo32_put(struct FIFO32 *fifo, int data);

void io_cli(void);

void io_sti(void);

void io_stihlt(void);

void io_out8(int port, int data);

int io_in8(int port);

int io_load_eflags(void);

void io_store_eflags(int eflags);

void asm_inthandler20(void);

void asm_inthandler21(void);

void asm_inthandler27(void);

void asm_inthandler2c(void);


/*dsctbl.c*/
/**
 * GDT
 */
struct SEGMENT_DESCRIPTOR {
    short limit_low, base_low;
    char base_mid, access_right;
    char limit_high, base_high;
};
/**
 * IDT
 */
struct GATE_DESCRIPTOR {
    short offset_low, selector;
    char dw_count, access_right;
    short offset_high;
};

void init_gdtidt(void);

void load_gdtr(int index, int value);

void load_idtr(int index, int value);

void set_gatedesc(struct GATE_DESCRIPTOR *gd, int offset, int selector, int ar);

void set_segmdesc(struct SEGMENT_DESCRIPTOR *sd, unsigned int limit, int base, int ar);

#define ADR_IDT            0x0026f800
#define LIMIT_IDT        0x000007ff
#define ADR_GDT            0x00270000
#define LIMIT_GDT        0x0000ffff
#define ADR_BOTPAK        0x00280000
#define LIMIT_BOTPAK    0x0007ffff
#define AR_DATA32_RW    0x4092
#define AR_CODE32_ER    0x409a
#define AR_INTGATE32    0x008e


/*graphic.c*/

void putfont8(char *vram, int xsize, int x, int y, char c, unsigned char *font);

void putfonts8_asc(char *vram, int xsize, int x, int y, char c, unsigned char *font);

void init_mouse_cursor8(char *mouse, char bc);

void putblock8_8(char *vram, int vxsize, int pxsize, int pysize, int px0, int py0, char *buf, int bxsize);

void init_palette(void);

void set_palette(int start, int end, unsigned char *rgb);

void boxfill8(unsigned char *vram, int xsize, unsigned char c, int x0, int y0, int x1, int y1);

void init_screen8(char *vram, short scrnx, short scrny);


/* int.c */

void init_pic(void);

void inthandler21(int *esp);

void inthandler27(int *esp);

void inthandler2c(int *esp);

#define PORT_KEYDAT 0x0060
#define PIC0_ICW1        0x0020

#define PIC0_OCW2        0x0020

#define PIC0_IMR        0x0021
#define PIC0_ICW2        0x0021
#define PIC0_ICW3        0x0021
#define PIC0_ICW4        0x0021
#define PIC1_ICW1        0x00a0

#define PIC1_OCW2        0x00a0

#define PIC1_IMR        0x00a1
#define PIC1_ICW2        0x00a1
#define PIC1_ICW3        0x00a1
#define PIC1_ICW4        0x00a1

//keyboard/mouse.c
#define PORT_KEYDAT                0x0060
#define PORT_KEYSTA                0x0064
#define PORT_KEYCMD                0x0064
#define KEYSTA_SEND_NOTREADY    0x02
#define KEYCMD_WRITE_MODE        0x60
#define KBC_MODE                0x47
#define KEYCMD_SENDTO_MOUSE        0xd4
#define MOUSECMD_ENABLE            0xf4

//keyboard.c
void init_keyboard(struct FIFO32 *fifo, int data0);

void wait_KBC_sendready();

//mouse
struct MOUSE_DEC {
    unsigned char buf[3], phase;
    int x, y, btn;
};


void enable_mouse(struct FIFO32 *fifo, int data0, struct MOUSE_DEC *mdec);

int mouse_decode(struct MOUSE_DEC *mdec, unsigned  char  data);

//memory
#define EFLAGS_AC_BIT 0x00040000
#define CR0_CACHE_DISABLE 0x60000000

void store_cr0(int cr0);

int load_cr0(void);

#define MEMMAN_ADDR 0x003c0000
#define MEMMAN_FREES 4090 /* 大约是32KB*/
struct FREEINFO { /* 可用信息 */
    unsigned int addr, size;
};
struct MEMMAN { /* 内存管理 */
    int frees, maxfrees, lostsize, losts;
    struct FREEINFO free[MEMMAN_FREES];
};

unsigned int memman_total(struct MEMMAN *man);

void memman_init(struct MEMMAN *man);

unsigned int memtest_sub(unsigned int start, unsigned int end);

unsigned int memtest(unsigned int start, unsigned int end);

int memman_free(struct MEMMAN *man, unsigned int addr, unsigned int size);

int memman_free_4k(struct MEMMAN *man, unsigned int addr, unsigned int size);

unsigned int memman_alloc_4k(struct MEMMAN *man, unsigned int size);



//sheet.c
#define MAX_SHEETS 256
#define SHEET_USE 1
struct SHEET {
    unsigned char *buf;
    int bxsize, bysize, vx0, vy0, col_inv, height, flags;
    struct SHTCTL *ctl;
};
struct SHTCTL {
    unsigned char *vram, *map;
    int xsize, ysize, top;
    struct SHEET *sheets[MAX_SHEETS];
    struct SHEET sheets0[MAX_SHEETS];
};

struct SHTCTL *shtctl_init(struct MEMMAN *memman, unsigned char *vram, int xsize, int ysize);

struct SHEET *sheet_alloc(struct SHTCTL *ctl);

void sheet_free(struct SHEET *sht);

void sheet_setbuf(struct SHEET *sht, unsigned char *buf, int xsize, int ysize, int col_inv);

void sheet_refresh(struct SHEET *sht, int bx0, int by0, int bx1, int by1);

void sheet_updown(struct SHEET *sht, int height);

void sheet_slide(struct SHEET *sht, int vx0, int vy0);

void sheet_refreshsub(struct SHTCTL *ctl, int vx0, int vy0, int vx1, int vy1, int h0, int h1);

void sheet_refreshmap(struct SHTCTL *ctl, int vx0, int vy0, int vx1, int vy1, int h0);

//timer.c
#define MAX_TIMER 500
#define PIT_CTRL 0x0043
#define PIT_CNT0 0x0040
#define TIMER_FLAGS_ALLOC 1 /* 已配置状态 */
#define TIMER_FLAGS_USING 2 /* 定时器运行中 */


struct TIMER {
    struct TIMER *next;
    unsigned int timeout, flags;
    struct FIFO32 *fifo;
    int data;
};
struct TIMERCTL {
    unsigned int count, next;
    struct TIMER *t0;
    struct TIMER timers0[MAX_TIMER];
};

extern struct TIMERCTL timerctl;

struct TIMER *timer_alloc(void);

void init_pit(void);

void inthandler20(int *esp);

void timer_settime(struct TIMER *timer, unsigned int timeout);

void timer_init(struct TIMER *timer, struct FIFO32 *fifo, int data);

void timer_free(struct TIMER *timer);

void putfonts8_asc_sht(struct SHEET *sht, int x, int y, int c, int b, char *s, int l);





