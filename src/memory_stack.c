#include"memory_stack.h"
#include<stdio.h> // printf
typedef unsigned char __byte_t;
#define __memory_stack_size ((size_t)(1024*1024))
static __byte_t __memory_stack[__memory_stack_size]={0};
#pragma pack(push)
#pragma pack(1)
//      block
// ----------------
// |head|body|tail|
// ----------------
typedef struct{
    bool block_is_used_;
    size_t block_body_size_;
}__block_head_t;
typedef struct{
    size_t block_body_size_;
    bool block_is_used_;
}__block_tail_t;
#pragma pack(pop)
// 
#define __block_head_size (sizeof(__block_head_t))
#define __block_tail_size (sizeof(__block_tail_t))
#define __block_head_begin ((__block_head_t*)((__byte_t*)__memory_stack))
#define __block_head_end ((__block_head_t*)( \
    (__byte_t*)__memory_stack+__memory_stack_size \
))
static __block_head_t* __block_head_first_unused=__block_head_begin;
static size_t __memory_stack_remaining_size=__memory_stack_size-__block_head_size-__block_tail_size;
static bool __block_head_getIsUsed(__block_head_t* block_head);
static void __block_head_setIsUsed(__block_head_t* block_head,bool is_used);
static size_t __block_head_getBodySize(__block_head_t* block_head);
static void __block_head_setBodySize(__block_head_t* block_head,size_t body_size);
static __block_tail_t* __block_head_getTail(__block_head_t* block_head);
static __block_tail_t* __block_head_getPrevTail(__block_head_t* block_head);
static __block_head_t* __block_head_getNextHead(__block_head_t* block_head);
static __block_head_t* __block_head_getPrevHead(__block_head_t* block_head);
static void* __block_head_getBody(__block_head_t* block_head);
static void __block_head_updateTail(__block_head_t* block_head);
static void __block_head_split(__block_head_t* block_head,size_t first_block_body_size);
static bool __block_tail_getIsUsed(__block_tail_t* block_tail);
static void __block_tail_setIsUsed(__block_tail_t* block_tail,bool is_used);
static size_t __block_tail_getBodySize(__block_tail_t* block_tail);
static void __block_tail_setBodySize(__block_tail_t* block_tail,size_t body_size);
static __block_tail_t* __block_tail_getHead(__block_tail_t* block_tail);
static __block_head_t* __block_body_getHead(void* block_body);
static bool __block_head_hasNext(__block_head_t* block_head);
static bool __block_head_hasPrev(__block_head_t* block_head);
static __block_head_t* __block_head_mergePrev(__block_head_t* block_head);
static void __block_head_mergeNext(__block_head_t* block_head);
static bool __memory_stack_init();
static void* __pointer_to_block_body(void* pointer);
// 
static bool __block_head_getIsUsed(__block_head_t* block_head){
    return block_head->block_is_used_;
}
static void __block_head_setIsUsed(__block_head_t* block_head,bool is_used){
    block_head->block_is_used_=is_used;
}
static size_t __block_head_getBodySize(__block_head_t* block_head){
    return block_head->block_body_size_;
}
static void __block_head_setBodySize(__block_head_t* block_head,size_t body_size){
    block_head->block_body_size_=body_size;
}
static __block_tail_t* __block_head_getTail(__block_head_t* block_head){
    return (__block_tail_t*)(
        ((__byte_t*)block_head)+__block_head_size+__block_head_getBodySize(block_head)
    );
}
static __block_tail_t* __block_head_getPrevTail(__block_head_t* block_head){
    return (__block_tail_t*)(
        ((__byte_t*)block_head)-__block_tail_size
    );
}
static __block_head_t* __block_head_getNextHead(__block_head_t* block_head){
    return (__block_head_t*)(
        ((__byte_t*)block_head)+
        __block_head_size+
        __block_head_getBodySize(block_head)+
        __block_tail_size
    );
}
static __block_head_t* __block_head_getPrevHead(__block_head_t* block_head){
    return (__block_head_t*)(
        ((__byte_t*)block_head)-
        __block_tail_size-
        __block_tail_getBodySize(__block_head_getPrevTail(block_head))-
        __block_head_size
    );
}
static void* __block_head_getBody(__block_head_t* block_head){
    return (void*)(
        ((__byte_t*)block_head)+__block_head_size
    );
}
static void __block_head_updateTail(__block_head_t* block_head){
    __block_tail_t* block_tail=__block_head_getTail(block_head);
    __block_tail_setIsUsed(block_tail,__block_head_getIsUsed(block_head));
    __block_tail_setBodySize(block_tail,__block_head_getBodySize(block_head));
}
static void __block_head_split(__block_head_t* block_head,size_t first_block_body_size){
    if(
        first_block_body_size+__block_tail_size+__block_head_size<
        __block_head_getBodySize(block_head)
    ){
        size_t second_block_body_size=__block_head_getBodySize(block_head)-
            first_block_body_size-
            __block_tail_size-
            __block_head_size;
        __block_head_t* second_block_head=(__block_head_t*)(
            ((__byte_t*)block_head)+
            __block_head_size+
            first_block_body_size+
            __block_tail_size
        );
        __block_head_setIsUsed(second_block_head,false);
        __block_head_setBodySize(second_block_head,second_block_body_size);
        __block_head_updateTail(second_block_head);
        __block_head_t* first_block_head=block_head;
        __block_head_setIsUsed(first_block_head,true);
        __block_head_setBodySize(first_block_head,first_block_body_size);
        __block_head_updateTail(first_block_head);
    }
}
static bool __block_tail_getIsUsed(__block_tail_t* block_tail){
    return block_tail->block_is_used_;
}
static void __block_tail_setIsUsed(__block_tail_t* block_tail,bool is_used){
    block_tail->block_is_used_=is_used;
}
static size_t __block_tail_getBodySize(__block_tail_t* block_tail){
    return block_tail->block_body_size_;
}
static void __block_tail_setBodySize(__block_tail_t* block_tail,size_t body_size){
    block_tail->block_body_size_=body_size;
}
static __block_tail_t* __block_tail_getHead(__block_tail_t* block_tail){
    return (__block_tail_t*)(
        ((__byte_t*)block_tail)-
        __block_tail_getBodySize(block_tail)-
        __block_head_size
    );
}
static __block_head_t* __block_body_getHead(void* block_body){
    return (__block_head_t*)(
        ((__byte_t*)block_body)-__block_head_size
    );
}
static bool __block_head_hasNext(__block_head_t* block_head){
    return __block_head_getNextHead(block_head)<__block_head_end;
}
static bool __block_head_hasPrev(__block_head_t* block_head){
    return __block_head_getPrevHead(block_head)>=__block_head_begin;
}
static __block_head_t* __block_head_mergePrev(__block_head_t* block_head){
    size_t new_block_body_size=
        __block_head_getBodySize(__block_head_getPrevHead(block_head))+
        __block_tail_size+
        __block_head_size+
        __block_head_getBodySize(block_head);
    __block_head_t* new_block_head=__block_head_getPrevHead(block_head);
    __block_head_setIsUsed(new_block_head,false);
    __block_head_setBodySize(new_block_head,new_block_body_size);
    __block_head_updateTail(new_block_head);
    return new_block_head;
}
static void __block_head_mergeNext(__block_head_t* block_head){
    __block_head_mergePrev(__block_head_getNextHead(block_head));
}
static bool __memory_stack_init(){
    static bool init_flag=false;
    if(init_flag==false){
        __block_head_t* block_head=__block_head_begin;
        __block_head_setIsUsed(block_head,false);
        __block_head_setBodySize(
            block_head,
            __memory_stack_size-__block_head_size-__block_tail_size
        );
        __block_head_updateTail(block_head);
        init_flag=true;
    }
    return init_flag;
}
extern void* memory_stack_alloc(size_t byte_size){
    __memory_stack_init();
    if(byte_size==0){
        return NULL;
    }
    if(byte_size>__memory_stack_remaining_size){
        return NULL;
    }
    __block_head_t* block_head=__block_head_first_unused;
    bool is_first_unused=true;
    while(block_head<__block_head_end){
        if(__block_head_getIsUsed(block_head)==false){
            if(is_first_unused==true){
                is_first_unused=false;
                __block_head_first_unused=block_head;
            }
            if(__block_head_getBodySize(block_head)>=byte_size){
                __block_head_split(block_head,byte_size);
                __memory_stack_remaining_size-=__block_head_getBodySize(block_head);
                return __block_head_getBody(block_head);
            }
        }
        block_head=__block_head_getNextHead(block_head);
    }
    return NULL;
}
extern bool memory_stack_has(void* pointer){
    if(pointer==NULL){
        return false;
    }
    if(
        ((__byte_t*)pointer)<((__byte_t*)__memory_stack)||
        ((__byte_t*)pointer)>=((__byte_t*)__memory_stack)+__memory_stack_size
    ){
        return false;
    }
    return true;
}
static void* __pointer_to_block_body(void* pointer){
    __block_head_t* block_head=__block_head_begin;
    while(block_head<__block_head_end){
        if(
            ((__byte_t*)pointer)>=((__byte_t*)block_head)&&
            ((__byte_t*)pointer)<((__byte_t*)__block_head_getNextHead(block_head))
        ){
            return __block_head_getBody(block_head);
        }
        block_head=__block_head_getNextHead(block_head);
    }
    return NULL;
}
extern void memory_stack_free(void* pointer){
    __memory_stack_init();
    if(memory_stack_has(pointer)==false){
        return;
    }
    __block_head_t* block_head=__block_body_getHead(__pointer_to_block_body(pointer));
    if(__block_head_getIsUsed(block_head)==false){
        return;
    }
    __memory_stack_remaining_size+=__block_head_getBodySize(block_head);
    if(
        __block_head_hasPrev(block_head)&&
        __block_head_getIsUsed(__block_head_getPrevHead(block_head))==false
    ){
        block_head=__block_head_mergePrev(block_head);
    }
    if(
        __block_head_hasNext(block_head)&&
        __block_head_getIsUsed(__block_head_getNextHead(block_head))==false
    ){
        __block_head_mergeNext(block_head);
    }
    __block_head_setIsUsed(block_head,false);
    __block_head_updateTail(block_head);
    if(block_head<__block_head_first_unused){
        __block_head_first_unused=block_head;
    }
}
extern void memory_stack_print(){
    __memory_stack_init();
    __block_head_t* block_head=__block_head_begin;
    size_t count=0;
    __block_tail_t* block_tail=NULL;
    printf("memory stack begin\n");
    while(block_head<__block_head_end){
        printf("\tblock [%zu] begin\n",count);
        // head
        printf("\t\thead begin\n");
        printf("\t\thead:%p\n",block_head);
        printf("\t\t\tblock_is_used_:%d\n",(int)__block_head_getIsUsed(block_head));
        printf("\t\t\tblock_body_size_:%zu\n",__block_head_getBodySize(block_head));
        printf("\t\thead end\n");
        // body
        printf("\t\tbody:%p\n",__block_head_getBody(block_head));
        // tail
        block_tail=__block_head_getTail(block_head);
        printf("\t\ttail begin\n");
        printf("\t\ttail:%p\n",block_tail);
        printf("\t\t\tblock_body_size_:%zu\n",__block_tail_getBodySize(block_tail));
        printf("\t\t\tblock_is_used_:%d\n",(int)__block_tail_getIsUsed(block_tail));
        printf("\t\ttail end\n");
        printf("\tblock [%zu] end\n",count);
        // next block
        block_head=__block_head_getNextHead(block_head);
        ++count;
    }
    printf("memory stack end\n");
}
