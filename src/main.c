#include<stdio.h> // printf
#include"memory_heap.h"
#include"memory_stack.h"
int main(void){
    int* ptr=(int*)memoryStackAlloc(sizeof(int));
    printf("%d\n",(size_t)ptr);
    *ptr=100;
    printf("%d\n",*ptr);
    memoryStackFree(ptr);
    printf("%d\n",*ptr);
    ptr=NULL;
    return 0;
}
