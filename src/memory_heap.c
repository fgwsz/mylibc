#include"memory_heap.h"
#include<stdlib.h> // malloc free
#include<string.h> // memset
void* memoryHeapAlloc(size_t byte_size){
    void* ret=NULL;
    while(1){
        ret=malloc(byte_size);
        if(ret!=NULL){
            memset(ret,0,byte_size);
            return ret;
        }
        free(ret);
        ret=NULL;
    }
}
void  memoryHeapFree(void* pointer){
    if(pointer!=NULL){
        free(pointer);
    }
}
