#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "types.h"

u32 rand32();
u32 rand32under(u32 n);
static bool checkIfSorted(u32* arr, u32 len);
static bool checkIfCounting(u32* arr, u32 len);
void bubbleSort_C(u32* arr, u32 len);
void bubbleSort_faster_C(u32* arr, u32 len);
void bubbleSort_optimised(u32* arr, u32 len);
void bubbleSort8s7(u32* arr, u32* end); //8 elements, 7 comparisons per iteration
void oddEvenMod_optimised(u32* arr, u32 len);
void oddEvenModSort9s8(u32* arr, u32* end); //9 elements, steps 8 per iteration
void selectionSort_C(u32* arr, u32 len);

//static const u32 LEN = 20*1024*1024;
static const u32 LEN = 128 * 1024;
//static const u32 LEN = 8 * 1024;
//static const u32 LEN = 5;

int main()
{
    //setvbuf(stdout, NULL, _IOLBF, 8*1024);

    clock_t tStart, tEnd;

    //Generate sequence and shuffle
    u32* shuffled = malloc(LEN * sizeof(u32));
    printf("Generating sequence length %u...\n", LEN);
    tStart = clock();
    for (u32 i = 0; i < LEN; i += 1) { shuffled[i] = i; }
    for (u32 i = 0; i < LEN; i += 1) {
        u32 sIndex = rand32under(LEN-i) + i;
        u32 a = shuffled[i];
        u32 b = shuffled[sIndex];
        shuffled[i]      = b;
        shuffled[sIndex] = a;
    }
    tEnd = clock();
    printf("Took %u ms\n", (unsigned)(tEnd - tStart));
    //for (u32 i = 0; i < LEN; i += 1) { printf("[%2u] %10u\n", i, shuffled[i]); }


    /* Bubble sort */ if (true) {
        u32* arr = malloc(LEN * sizeof(u32));
        memcpy(arr, shuffled, LEN * sizeof(u32));
        printf("\nSorting...\n");
        tStart = clock();
        bubbleSort_C(arr, LEN);
        tEnd = clock();

        printf("Checking...\n");
        if (checkIfCounting(arr, LEN) == false) {
            printf("Failed to sort\n");
            exit(-1);
        }

        free(arr);
        printf("Bubble sort in C finished in %u ms\n", (unsigned)((tEnd-tStart)*1000/CLOCKS_PER_SEC));
    }

    /* Bubble sort but probably slightly faster */ if (true) {
        u32* arr = malloc(LEN * sizeof(u32));
        memcpy(arr, shuffled, LEN * sizeof(u32));
        printf("\nSorting...\n");
        tStart = clock();
        bubbleSort_faster_C(arr, LEN);
        tEnd = clock();

        printf("Checking...\n");
        if (checkIfCounting(arr, LEN) == false) {
            printf("Failed to sort\n");
            exit(-1);
        }

        free(arr);
        printf("Bubble sort (but faster hopefully) in C finished in %u ms\n", (unsigned)((tEnd-tStart)*1000/CLOCKS_PER_SEC));
    }

    /* Part asm optimised bubble sort */ if (true) {
        u32* arr = malloc(LEN * sizeof(u32));
        memcpy(arr, shuffled, LEN * sizeof(u32));
        printf("\nSorting...\n");
        tStart = clock();
        bubbleSort_optimised(arr, LEN);
        tEnd = clock();

        printf("Checking...\n");
        if (checkIfCounting(arr, LEN) == false) {
            printf("Failed to sort\n");
            exit(-1);
        }

        free(arr);
        printf("Bubble sort (asm) finished in %u ms\n", (unsigned)((tEnd-tStart)*1000/CLOCKS_PER_SEC));
    }

    /* Part asm modified odd even sort */ if (false) {
        u32* arr = malloc(LEN * sizeof(u32));
        memcpy(arr, shuffled, LEN * sizeof(u32));
        printf("\nSorting...\n");
        tStart = clock();
        oddEvenMod_optimised(arr, LEN);
        tEnd = clock();

        printf("Checking...\n");
        if (checkIfCounting(arr, LEN) == false) {
            printf("Failed to sort\n");
            exit(-1);
        }

        free(arr);
        printf("Modified odd/even sort (asm) finished in %u ms\n", (unsigned)((tEnd-tStart)*1000/CLOCKS_PER_SEC));
    }

    /* Selection sort */ if (true) {
        u32* arr = malloc(LEN * sizeof(u32));
        memcpy(arr, shuffled, LEN * sizeof(u32));
        printf("\nSorting...\n");
        tStart = clock();
        selectionSort_C(arr, LEN);
        tEnd = clock();

        printf("Checking...\n");
        if (checkIfCounting(arr, LEN) == false) {
            printf("Failed to sort\n");
            exit(-1);
        }

        free(arr);
        printf("Selection sort finished in %u ms\n", (unsigned)((tEnd-tStart)*1000/CLOCKS_PER_SEC));
    }

    /* Selection sort (faster) */ /*if (true) {
        u32* arr = malloc(LEN * sizeof(u32));
        memcpy(arr, shuffled, LEN * sizeof(u32));
        printf("\nSorting...\n");
        tStart = clock();
        selectionSort_C(arr, LEN);
        tEnd = clock();

        printf("Checking...\n");
        if (checkIfCounting(arr, LEN) == false) {
            printf("Failed to sort\n");
            exit(-1);
        }

        free(arr);
        printf("Selection sort (faster) finished in %u ms\n", (unsigned)((tEnd-tStart)*1000/CLOCKS_PER_SEC));
    }*/

    return 0;
}

static bool checkIfSorted(u32* arr, u32 len)
{
    for (u32 i = 0; i+1 < len; i += 1) {
        if (arr[i] > arr[i+1]) return false;
    }
    return true;
}

static bool checkIfCounting(u32* arr, u32 len)
{
    for (u32 i = 0; i+1 < len; i += 1) {
        if (arr[i] != i) return false;
    }
    return true;
}

u32 rand32under(u32 n)
{
    while (true) {
        u32 r = rand32();
        u32 cap = (0xFFFFFFFF / n) * n;
        if (r < cap) return r%n;
    }
}


void bubbleSort_C(u32* arr, u32 len)
{
    while (len > 0) {
        for (u32 i = 0; i+1 < len; i += 1) {
            if (arr[i] > arr[i+1]) {
                u32 tmp = arr[i];
                arr[i] = arr[i+1];
                arr[i+1] = tmp;
            }
        }
        len -= 1;
    }
}

void bubbleSort_faster_C(u32* arr, u32 len)
{
    while (len > 0) {
        for (u32 i = 0; i+1 < len; i += 1) {
            //Load
            u32 lLeft, lRight;
            lLeft  = arr[i];
            lRight = arr[i+1];

            u32 sLeft, sRight;
            sLeft = lLeft;
            sRight = lRight;

            //Conditional swap
            if (lLeft > lRight) {
                sLeft =  lRight;
                sRight = lLeft;
            }

            //Store
            arr[i]   = sLeft;
            arr[i+1] = sRight;
        }
        len -= 1;
    }
}

void bubbleSort_optimised(u32* arr, u32 len)
{
    while (len > 0) {
        u32 maxIndex8s7 = ((len-1)/7) * 7;
        bubbleSort8s7(arr, arr + maxIndex8s7);
        for (u32 i = maxIndex8s7; i+1 < len; i += 1) {
            //Load
            u32 lLeft, lRight;
            lLeft  = arr[i];
            lRight = arr[i+1];

            u32 sLeft, sRight;
            sLeft = lLeft;
            sRight = lRight;

            //Conditional swap
            if (lLeft > lRight) {
                sLeft =  lRight;
                sRight = lLeft;
            }

            //Store
            arr[i]   = sLeft;
            arr[i+1] = sRight;
        }

        //for (u32 i = 0; i < LEN; i += 1) { printf("[%2u] %10u\n", i, arr[i]); }

        len -= 1;
    }
}


void oddEvenMod_optimised(u32* arr, u32 len)
{
    while (checkIfSorted(arr,len) == false)
    for (u32 count = 0; count < 64; count += 1) {
        u32 maxIndex9s8 = ((len-1)/8) * 8;
        oddEvenModSort9s8(arr, arr + maxIndex9s8);
        bubbleSort_faster_C(arr+maxIndex9s8, len-maxIndex9s8);

        //for (u32 i = 0; i < LEN; i += 1) { printf("[%2u] %10u\n", i, arr[i]); }
    }    
}


void selectionSort_C(u32* arr, u32 len)
{
    for (u32 n = 0; n < len - 1; n += 1) {
        //for (u32 i = 0; i < LEN; i += 1) { printf("[%2u] %10u\n", i, arr[i]); }
        u32 min = arr[n];
        u32 minIndex = n;
        for (u32 i = n + 1; i < len; i += 1) {
            if (arr[i] < min) {
                min = arr[i];
                minIndex = i;
            }
        }
        u32 left  = arr[n];
        u32 right = arr[minIndex];
        arr[n]        = right;
        arr[minIndex] = left;
    }
}

/*
void selectionSort_faster_C(u32* arr, u32 len)
{
    for (u32 n = 0; n < len - 4; n += 1) {
        //for (u32 i = 0; i < LEN; i += 1) { printf("[%2u] %10u\n", i, arr[i]); }
        u32 min[4] = {arr[n],arr[n],arr[n],arr[n]};
        u32 minIndex[4] = {n,n,n,n};

        //Find smallest values
        for (u32 i = n + 1; i < len; i += 1) {
            if (arr[i] < min[3]) {
                min[3] = arr[i];
                minIndex[3] = i;
                if (min[3] < min[2]) {
                    u32 tmp = min[2];
                    u32 tmpi = minIndex[2];
                    min[2] = min[3];
                    minIndex[2] = minIndex[3];
                    min[3] = tmp;
                    minIndex = tmpi;
                }
                if (min[2] < min[1]) {
                    u32 tmp = min[1];
                    u32 tmpi = minIndex[1];
                    min[1] = min[2];
                    minIndex[1] = minIndex[2];
                    min[2] = tmp;
                    minIndex = tmpi;
                }
                if (min[1] < min[0]) {
                    u32 tmp = min[0];
                    u32 tmpi = minIndex[0];
                    min[0] = min[1];
                    minIndex[0] = minIndex[1];
                    min[1] = tmp;
                    minIndex = tmpi;
                }
            }
        }

        //Swaps
        {
            u32 left  = arr[n];
            u32 right = arr[minIndex];
            arr[n]        = right;
            arr[minIndex] = left;
        }
    }
}*/