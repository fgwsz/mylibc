#include<stdio.h> // printf
#include"fast_memory_stack.h"
int main(void){
    int* num=fastMemoryStackAlloc(sizeof(int));
    fastMemoryStackPrint();
    fastMemoryStackFree(num);
    fastMemoryStackPrint();
    return 0;
}
