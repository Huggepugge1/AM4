#include <stdlib.h>
#include <stdio.h>
#include "vm.h"

#define STACK_SIZE 256 

struct Stack {
    int32_t stack[STACK_SIZE];
    int sp;
};

void push(struct Stack *stack, int32_t value) {
    if (stack->sp >= STACK_SIZE) {
        fprintf(stderr, "Stack overflow!");
        exit(1);
    }
    stack->stack[stack->sp] = value;
    stack->sp++;
}

int32_t pop(struct Stack *stack) {
    if (stack->sp <= 0) {
        fprintf(stderr, "Stack underflow!");
        exit(1);
    }
    stack->sp--;
    return stack->stack[stack->sp];
}

int32_t run_vm(struct BinaryFile *bin) { 
    struct Stack stack = {.sp = 0};
    int32_t *memory = calloc(bin->idents, sizeof(int32_t));
    
    uint32_t pc = 0;
    while (pc < bin->instruction_len) {
        printf("Instruction %i\n", pc);
        pc++;
    }
    return pop(&stack);
}