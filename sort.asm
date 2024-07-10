section .text

global rand32
rand32:
    rdrand eax
    ret


%macro cswapg 4
;Usage is cswapg dstA, dstB, srcA, srcB
mov %1, %3
mov %2, %4
cmovg %1, %4
cmovg %2, %3
%endmacro

%macro cmpcswapg 4
cmp %3, %4
cswapg %1, %2, %3, %4
%endmacro


global bubbleSort8s7
bubbleSort8s7:
    push rbx
    push rdi
    push rsi
    push r12
    push r13
    push r14
    push r15


    mov r8d, [rcx + 0*4]

        bubbleLoop:
        cmp rcx, rdx
        jge bubbleBreak

        ;Load
        mov r9d,  [rcx + 1*4]
        mov r10d, [rcx + 2*4]
        mov r11d, [rcx + 3*4]
        mov r12d, [rcx + 4*4]
        mov r13d, [rcx + 5*4]
        mov r14d, [rcx + 6*4]
        mov r15d, [rcx + 7*4]


        ;Initial layout of elements (and spare registers)
        ; 8,  9, 10, 11, 12, 13, 14, 15 ( a,  b)
        ;Using the 3 register version of cswapg is 8.696% slower by my measurements

        ;Swap 0/1
        ; [a,  b], 10, 11, 12, 13, 14, 15 ( 8,  9)
        cmpcswapg eax, ebx, r8d, r9d
        ;Swap 1/2
        ; a, [ 8,  9], 11, 12, 13, 14, 15 ( b, 10)
        cmpcswapg r8d, r9d, ebx, r10d
        ;Swap 2/3
        ; a,  8, [ b, 10], 12, 13, 14, 15 ( 9, 11)
        cmpcswapg ebx, r10d, r9d, r11d
        ;Swap 3/4
        ; a,  8,  b, [ 9, 11], 13, 14, 15 (10, 12)
        cmpcswapg r9d, r11d, r10d, r12d
        ;Swap 4/5
        ; a,  8,  b,  9, [10, 12], 14, 15 (11, 13)
        cmpcswapg r10d, r12d, r11d, r13d
        ;Swap 5/6
        ; a,  8,  b,  9, 10, [11, 13], 15 (12, 14)
        cmpcswapg r11d, r13d, r12d, r14d
        ;Swap 6/7
        ; a,  8,  b,  9, 10, 11, [12, 14] (13, 15)
        cmpcswapg r12d, r14d, r13d, r15d


        ;Store
        ; a,  8,  b,  9, 10, 11, 12, 14
        mov [rcx + 0*4], eax
        mov [rcx + 1*4], r8d
        mov [rcx + 2*4], ebx
        mov [rcx + 3*4], r9d
        mov [rcx + 4*4], r10d
        mov [rcx + 5*4], r11d
        mov [rcx + 6*4], r12d
        mov r8d,         r14d

        add rcx, 7*4
        jmp bubbleLoop
    bubbleBreak:

    mov [rcx + 0*4], r8d


    pop r15
    pop r14
    pop r13
    pop r12
    pop rsi
    pop rdi
    pop rbx
    ret
;


global oddEvenModSort9s8
oddEvenModSort9s8:
    push rbx
    push rdi
    push rsi
    push r12
    push r13
    push r14
    push r15


    mov r8d, [rcx + 0*4]

        oddEvenModLoop:
        cmp ecx, edx
        jge oddEvenModBreak

        ;Load
        mov r9d,  [rcx + 1*4]
        mov r10d, [rcx + 2*4]
        mov r11d, [rcx + 3*4]
        mov r12d, [rcx + 4*4]
        mov r13d, [rcx + 5*4]
        mov r14d, [rcx + 6*4]
        mov r15d, [rcx + 7*4]


        ;Initial layout of elements (and auxilary source registers)
        ; [8,  9], [10, 11], 12, 13, 14, 15 ( a,  b, di, si)

        ;Odd
        ;[ a,  b], [di, si], 12, 13, 14, 15 ( 8,  9, 10, 11)
        cmpcswapg eax, ebx, r8d,  r9d
        cmpcswapg edi, esi, r10d, r11d
        ; a,  b, di, si, [ 8,  9], [10, 11] (12, 13, 14, 15)
        cmpcswapg r8d , r9d,  r12d, r13d
        cmpcswapg r10d, r11d, r14d, r15d

        ;Save first element
        mov [rcx + 0*4], eax
        ;Load ninth element
        mov eax, [rcx + 8*4]

        ;Even
        ;[12, 13], [14, 15],  9, 10, 11,  a ( b, di, si,  8)
        cmpcswapg r12d, r13d, ebx, edi
        cmpcswapg r14d, r15d, esi, r8d
        ;12, 13, 14, 15, [ b, di], [si,  8] ( 9, 10, 11,  a)
        cmpcswapg ebx, edi, r9d, r10d
        cmpcswapg esi, r8d, r11d, eax

        ;Store
        ;12, 13, 14, 15,  b, di, si,  8
        mov [rcx + 1*4], r12d
        mov [rcx + 2*4], r13d
        mov [rcx + 3*4], r14d
        mov [rcx + 4*4], r15d
        mov [rcx + 5*4], ebx
        mov [rcx + 6*4], edi
        mov [rcx + 7*4], esi
        ;mov r8d, r8d
        
        add rcx, 8*4
        jmp oddEvenModLoop
    oddEvenModBreak:
    
    mov [rcx + 0*4], r8d


    pop r15
    pop r14
    pop r13
    pop r12
    pop rsi
    pop rdi
    pop rbx
    ret
;
