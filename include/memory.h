#ifndef __HF_MEMORY_H__
#define __HF_MEMORY_H__
/* include header file */
#include<stddef.h> // size_t
#ifdef __cplusplus
    extern "C"{
#endif // __cplusplus
/* decl public interface */
extern void* memory_alloc(size_t byte_size);
extern void  memory_free(void* pointer);
#ifdef __cplusplus
    }
#endif // __cplusplus
#endif // !__HF_MEMORY_H__
