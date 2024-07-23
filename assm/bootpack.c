//
// Created by Lartimes on 2024/7/23.
//
void io_hlt(void);


void HariMain(void) {
    int i; /*变量声明：i是一个32位整数*/
    for (i = 0xa0000; i <= 0xaffff; i++) {
//        p = (char *) i ;
//        *p = i & 0x0f;
        *((char *) i) = i & 0x0f;
    }

    for (;;) {
        io_hlt();
    }
}