The Fastest Bubble Sort in the West!
====================================

This project is a demonstration of bubble sort written in x86_64 assembly. The program will test the runtime against a couple other C implementations of the same sort. My manually written assembly performs better than Clang's output on AMD Zen 3 and Intel Airmont architectures. 

You will need NASM and a C compiler such as Clang to compile it. 

This was created because other sorting algorithms such as Radix sort are often optimised at an assembly level, so to properly compare performance we would need a similarly optimised implementation. 
