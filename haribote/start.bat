make clean
rm  -rf haribote.img
make img
copy /B asmhead.bin+bootpack.hrb haribote.sys
make img
