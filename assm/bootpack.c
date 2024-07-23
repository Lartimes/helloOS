//
// Created by Lartimes on 2024/7/23.
//
void io_hlt(void);

void HariMain(void) {
fin:
    io_hlt(); /*执行naskfunc.nas里的_io_hlt*/
    goto fin;
}
