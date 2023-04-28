#ifndef __HF_MEMORY_HEAP_H__
#define __HF_MEMORY_HEAP_H__
/* include header file */
#include<stddef.h> // size_t
#ifdef __cplusplus
    extern "C"{
#endif // __cplusplus
/* decl public interface */
extern void* memoryHeapAlloc(size_t byte_size);
extern void  memoryHeapFree(void* pointer);
#ifdef __cplusplus
    }
#endif // __cplusplus
#endif // !__HF_MEMORY_HEAP_H__
