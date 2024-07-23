make clean
rm  haribote.img
make img
copy /B asmhead.bin+bootpack.hrb haribote.sys
make img
