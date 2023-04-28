#include"memory_stack.h"
#include<string.h> // memset
typedef unsigned char __byte_t;
typedef struct{
    __byte_t flag_;
    size_t size_;
}__block_head_t;
enum{
    __stack_capacity=(size_t)(1024*1024),
    __block_head_size=sizeof(__block_head_t)
};
static __byte_t __memory_stack[__stack_capacity]={0};
static size_t __stack_size=__stack_capacity;
static size_t __stack_index=0;
void* memoryStackAlloc(size_t byte_size){
    if(byte_size==0||__block_head_size+byte_size>__stack_size){
        return NULL;
    }
    size_t current_index=__stack_index;
    size_t block_byte_size=0;
    size_t block_begin_index=0;
    void* ret=NULL;
    __byte_t first_unused_block=1;
    while(current_index<__stack_capacity){
        // check byte size of the unused block
        if(__memory_stack[current_index]==0){
            block_begin_index=current_index;
            block_byte_size=1;
            ++current_index;
            while(current_index<__stack_capacity&&
                  __memory_stack[current_index]==0){
                ++block_byte_size;
                ++current_index;
            }
            // the block large enough
            if(block_byte_size>=__block_head_size+byte_size){
                __block_head_t* block=(__block_head_t*)(
                    (__byte_t*)__memory_stack+block_begin_index
                );
                block->flag_=1;
                block->size_=byte_size;
                __stack_size-=__block_head_size+byte_size;
                ret=(__byte_t*)block+__block_head_size;
                memset(ret,0,byte_size);
                if(first_unused_block){
                    __stack_index=block_begin_index+__block_head_size+byte_size;
                }
                return ret;
            // the block not big enough, find the next
            }else if(current_index<__stack_capacity){
                first_unused_block=0;
                continue;
            // no available block found
            }else{
                return NULL;
            }
        }
        // jump the block in use
        current_index+=current_index+*(size_t*)((__byte_t*)__memory_stack+current_index+1);
    }
    return ret;
}
static __byte_t __inMemoryStack(void* pointer) {
    return pointer!=NULL&&
        (__byte_t*)pointer>=(__byte_t*)__memory_stack+__block_head_size&&
        (__byte_t*)pointer<(__byte_t*)__memory_stack+__stack_capacity-__block_head_size;
}
void  memoryStackFree(void* pointer){
    if(__inMemoryStack(pointer)){
        __block_head_t* block=(__block_head_t*)(
            (__byte_t*)pointer-__block_head_size
        );
        __stack_size+=__block_head_size+block->size_;
        size_t current_index=(__byte_t*)pointer-__block_head_size-(__byte_t*)__memory_stack;
        __stack_index=current_index<__stack_index?current_index:__stack_index;
        memset(block,0,__block_head_size+block->size_);
    }
}
bool  inMemoryStack(void* pointer){
    return __inMemoryStack(pointer);
}
