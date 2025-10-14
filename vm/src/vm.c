#include <stdlib.h>
#include <stdio.h>
#include "vm.h"

#define STACK_SIZE 256 
#define ARG_MASK 0x00ffffff

struct Stack {
    int32_t stack[STACK_SIZE];
    int sp;
};

void push(struct Stack *stack, int32_t value) {
    if (stack->sp >= STACK_SIZE) {
        fprintf(stderr, "Stack overflow!\n");
        exit(1);
    }
    stack->stack[stack->sp] = value;
    stack->sp++;
}

int32_t pop(struct Stack *stack) {
    if (stack->sp <= 0) {
        fprintf(stderr, "Stack underflow!\n");
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
        uint32_t instruction = bin->instructions[pc];
        uint32_t opcode = instruction >> 24;
        // Does not currently work for negative values
        int32_t op_arg = instruction & ARG_MASK;

        pc++;
        
        if (opcode == InstructionNoop) {
            // No operation
        }
        // Jump arg is currently address + number of identifiers
        else if (opcode == InstructionJmp) {
            pc = op_arg - bin->idents;
            printf("Pc %d\n", op_arg - bin->idents);
        }
        else if (opcode == InstructionJEQZ) {
            int32_t v1 = pop(&stack);
            if (v1 == 0) {
                pc = op_arg - bin->idents;
                printf("Pc %d\n", op_arg - bin->idents);
            }
        }
        else if (opcode == InstructionPush) {
            push(&stack, op_arg);
            printf("Push %d\n", op_arg);
        }
        else if (opcode == InstructionAdd) {
            int32_t v2 = pop(&stack);
            int32_t v1 = pop(&stack);
            push(&stack, v1 + v2);
            printf("Add %d, %d\n", v1, v2);
        }
        else if (opcode == InstructionSub) {
            int32_t v2 = pop(&stack);
            int32_t v1 = pop(&stack);
            push(&stack, v1 - v2);
            printf("Sub %d, %d\n", v1, v2);
        }
        else if (opcode == InstructionMul) {
            int32_t v2 = pop(&stack);
            int32_t v1 = pop(&stack);
            push(&stack, v1 * v2);
            printf("Mul %d, %d\n", v1, v2);
        }
        else if (opcode == InstructionEq) {
            int32_t v2 = pop(&stack);
            int32_t v1 = pop(&stack);
            push(&stack, v1 == v2);
            printf("Eq %d, %d\n", v1, v2);
        }
        else if (opcode == InstructionLt) {
            int32_t v2 = pop(&stack);
            int32_t v1 = pop(&stack);
            push(&stack, v1 < v2);
            printf("Lt %d, %d\n", v1, v2);
        }
        else if (opcode == InstructionLe) {
            int32_t v2 = pop(&stack);
            printf("V2 %d\n", v2);
            int32_t v1 = pop(&stack);
            printf("V1 %d\n", v1);
            push(&stack, v1 <= v2);
            printf("Le %d, %d\n", v1, v2);
        }
        else if (opcode == InstructionGt) {
            int32_t v2 = pop(&stack);
            int32_t v1 = pop(&stack);
            push(&stack, v1 > v2);
            printf("Gt %d, %d\n", v1, v2);
        }
        else if (opcode == InstructionGe) {
            int32_t v2 = pop(&stack);
            int32_t v1 = pop(&stack);
            push(&stack, v1 >= v2);
            printf("Ge %d, %d\n", v1, v2);
        }
        else if (opcode == InstructionLAnd) {
            int32_t v2 = pop(&stack);
            int32_t v1 = pop(&stack);
            push(&stack, v1 && v2);
            printf("LAnd %d, %d\n", v1, v2);
        }
        else if (opcode == InstructionLOr) {
            int32_t v2 = pop(&stack);
            int32_t v1 = pop(&stack);
            push(&stack, v1 || v2);
            printf("LOr %d, %d\n", v1, v2);
        }
        else if (opcode == InstructionLNeg) {
            int32_t v1 = pop(&stack);
            push(&stack, !v1);
            printf("LNeg %d\n", v1);
        }
        else if (opcode == InstructionFetch) {
            int32_t mem_val = memory[op_arg];
            push(&stack, mem_val);
            printf("Fetch %d from %d\n", mem_val, op_arg);
        }
        else if (opcode == InstructionStore) {
            int32_t value = pop(&stack);
            memory[op_arg] = value;
            printf("Store %d to %d\n", value, op_arg);
        }
        else {
            fprintf(stderr, "Unknown operation %02x\n", opcode);
            exit(1);
        }
    }
    return pop(&stack);
}