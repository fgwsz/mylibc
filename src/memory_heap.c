#include"memory_heap.h"
#include<stdlib.h> // malloc free
extern void* memory_heap_alloc(size_t byte_size){
    if(byte_size==0){
        return NULL;
    }
    void* ret=NULL;
    while(1){
        ret=malloc(byte_size);
        if(ret!=NULL){
            return ret;
        }
        free(ret);
        ret=NULL;
    }
}
extern void memory_heap_free(void* pointer){
    if(pointer!=NULL){
        free(pointer);
    }
}
