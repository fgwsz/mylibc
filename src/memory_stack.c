#include"memory_stack.h"
#include<string.h> // memset
enum{
    stack_capacity=(size_t)(1024*1024)
};
static unsigned char memory_stack[stack_capacity];
static size_t stack_current_size=stack_capacity;
typedef struct StackMemoryBlockHead{
    unsigned char flag_;
    size_t size_;
}stack_memory_block_head_t;
void* memoryStackAlloc(size_t byte_size){
    if(byte_size==0||byte_size+sizeof(stack_memory_block_head_t)>stack_current_size){
        return NULL;
    }
    size_t current_index=0;
    size_t block_byte_size=0;
    size_t block_begin_index=0;
    while(current_index<stack_capacity){
        // check byte size of the unused block
        if(memory_stack[current_index]==0){
            block_begin_index=current_index;
            block_byte_size=0;
            ++block_byte_size;
            ++current_index;
            while(current_index<stack_capacity&&
                  memory_stack[current_index]==0){
                ++block_byte_size;
                ++current_index;
            }
            // the block large enough
            if(block_byte_size>=sizeof(stack_memory_block_head_t)+byte_size){
                stack_memory_block_head_t* block_head=(stack_memory_block_head_t*)(
                    (unsigned char*)memory_stack+block_begin_index
                );
                block_head->flag_=1;
                block_head->size_=byte_size;
                stack_current_size-=byte_size+sizeof(stack_memory_block_head_t);
                void* ret=(unsigned char*)block_head+sizeof(stack_memory_block_head_t);
                memset(ret,0,byte_size);
                return ret;
            // the block not big enough, find the next
            }else if(current_index<stack_capacity){
                continue;
            // no available block found
            }else{
                return NULL;
            }
        }
        // jump the block in use
        current_index+=current_index+(size_t)(memory_stack[current_index+1]);
    }
}
void  memoryStackFree(void* pointer){
    if(pointer!=NULL&&
       (unsigned char*)pointer>=(unsigned char*)memory_stack+sizeof(stack_memory_block_head_t)&&
       (unsigned char*)pointer<(unsigned char*)memory_stack+stack_capacity-sizeof(stack_memory_block_head_t)){
        stack_memory_block_head_t* block_head=(stack_memory_block_head_t*)(
            (unsigned char*)pointer-sizeof(stack_memory_block_head_t)
        );
        stack_current_size+=sizeof(stack_memory_block_head_t)+block_head->size_;
        memset(block_head,0,sizeof(stack_memory_block_head_t)+block_head->size_);
    }
}
