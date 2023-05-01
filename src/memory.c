#include"memory.h"
#include"memory_stack.h"
#include"memory_heap.h"
extern void* memory_alloc(size_t byte_size){
    void* ret=memory_stack_alloc(byte_size);
    if(ret==NULL){
        ret=memory_heap_alloc(byte_size);
    }
    return ret;
}
extern void memory_free(void* pointer){
    if(memory_stack_has(pointer)){
        memory_stack_free(pointer);
    }else{
        memory_heap_free(pointer);
    }
    pointer=NULL;
}
