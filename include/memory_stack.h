#ifndef __HF_FAST_MEMORY_STACK_H__
#define __HF_FAST_MEMORY_STACK_H__
/* include header file */
#include<stddef.h> // size_t
#include<stdbool.h> // bool
#ifdef __cplusplus
    extern "C"{
#endif // __cplusplus
/* decl public interface */
extern void* memory_stack_alloc(size_t byte_size);
extern void memory_stack_free(void* pointer);
extern bool memory_stack_has(void* pointer);
extern void memory_stack_print();
#ifdef __cplusplus
    }
#endif // __cplusplus
#endif // !__HF_FAST_MEMORY_STACK_H__
