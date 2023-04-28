#include<stdio.h> // printf
#include"memory.h"
#include"memory_stack.h"
int main(void){
    #define MAX_COUNT 40
    int byte_size=1;
    int* ptr_arr[MAX_COUNT];
    int index=0;
    while(byte_size<MAX_COUNT){
        ptr_arr[index]=(int*)memoryAlloc(byte_size*sizeof(int)*1024);
        if(inMemoryStack(ptr_arr[index])){
            printf("stack memory:%x\n",ptr_arr[index]);
        }else{
            printf("heap memory:%x\n",ptr_arr[index]);
        }
        ++byte_size;
        ++index;
    }
    for(index=0;index<MAX_COUNT;++index){
        memoryFree(ptr_arr[index]);
        ptr_arr[index]=NULL;
    }
    return 0;
}
