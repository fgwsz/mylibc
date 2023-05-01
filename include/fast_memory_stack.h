#ifndef __HF_FAST_MEMORY_STACK_H__
#define __HF_FAST_MEMORY_STACK_H__
/* include header file */
#include<stddef.h> // size_t
#ifdef __cplusplus
    extern "C"{
#endif // __cplusplus
/* decl public interface */
extern void* fastMemoryStackAlloc(size_t byte_size);
extern void  fastMemoryStackFree(void* pointer);
extern void  fastMemoryStackPrint();
#ifdef __cplusplus
    }
#endif // __cplusplus
#endif // !__HF_FAST_MEMORY_STACK_H__
