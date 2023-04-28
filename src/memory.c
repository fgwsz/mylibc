#include"memory.h"
#include"memory_heap.h"
#include"memory_stack.h"
void* memoryAlloc(size_t byte_size){
    void* ret=memoryStackAlloc(byte_size);
    if(ret==NULL){
        ret=memoryHeapAlloc(byte_size);
    }
    return ret;
}
void  memoryFree(void* pointer){
    if(inMemoryStack(pointer)){
        memoryStackFree(pointer);
    }else{
        memoryHeapFree(pointer);
    }
    pointer=NULL;
}
