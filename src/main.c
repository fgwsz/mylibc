#include<stdio.h> // printf
#include"memory.h" // memory_alloc memory_free
#include"memory_stack.h" // memory_stack_print
int main(void){
    int* num1=memory_alloc(sizeof(int));
    int* num2=memory_alloc(sizeof(int));
    memory_stack_print();
    memory_free(num1);
    memory_stack_print();
    memory_free(num2);
    memory_stack_print();
    return 0;
}
