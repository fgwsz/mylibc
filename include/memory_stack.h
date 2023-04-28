#ifndef __HF_MEMORY_STACK_H__
#define __HF_MEMORY_STACK_H__
/* include header file */
#include<stddef.h> // size_t
#include<stdbool.h> // bool
#ifdef __cplusplus
    extern "C"{
#endif // __cplusplus
/* decl public interface */
extern void* memoryStackAlloc(size_t byte_size);
extern void  memoryStackFree(void* pointer);
extern bool  inMemoryStack(void* pointer);
#ifdef __cplusplus
    }
#endif // __cplusplus
#endif // !__HF_MEMORY_STACK_H__
