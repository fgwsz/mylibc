#include<stdio.h> // printf
#include"memory_stack.h"
int main(void){
    int* num1=memory_stack_alloc(sizeof(int));
    int* num2=memory_stack_alloc(sizeof(int));
    memory_stack_print();
    memory_stack_free(num1);
    memory_stack_print();
    memory_stack_free(num2);
    memory_stack_print();
    return 0;
}
