del a.exe sort.obj
cls
nasm sort.asm -fwin64
clang -O3 a.c sort.obj
.\a.exe