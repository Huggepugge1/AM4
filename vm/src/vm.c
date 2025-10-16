#include "vm.h"
#include <stdio.h>
#include <stdlib.h>

#define STACK_SIZE 256
#define ARG_MASK 0xffffff
#define SIGN_BIT 0x800000
#define SIGN_EXTEND 0xff000000

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

    uint32_t pc = bin->start_addr;
    while (pc < bin->total_size) {
        uint32_t instruction = bin->memory[pc];
        uint32_t opcode = instruction >> 24;

        int32_t op_arg = instruction & ARG_MASK;
        op_arg = op_arg | ((op_arg & SIGN_BIT) ? SIGN_EXTEND : 0);

        pc++;

        if (opcode == InstructionNoop) {
        } else if (opcode == InstructionJmp) {
            pc = op_arg;
        } else if (opcode == InstructionJEQZ) {
            int32_t v1 = pop(&stack);
            if (v1 == 0) {
                pc = op_arg;
            }
        } else if (opcode == InstructionPush) {
            push(&stack, op_arg);
        } else if (opcode == InstructionAdd) {
            int32_t v2 = pop(&stack);
            int32_t v1 = pop(&stack);
            push(&stack, v1 + v2);
        } else if (opcode == InstructionSub) {
            int32_t v2 = pop(&stack);
            int32_t v1 = pop(&stack);
            push(&stack, v1 - v2);
        } else if (opcode == InstructionMul) {
            int32_t v2 = pop(&stack);
            int32_t v1 = pop(&stack);
            push(&stack, v1 * v2);
        } else if (opcode == InstructionEq) {
            int32_t v2 = pop(&stack);
            int32_t v1 = pop(&stack);
            push(&stack, v1 == v2);
        } else if (opcode == InstructionLt) {
            int32_t v2 = pop(&stack);
            int32_t v1 = pop(&stack);
            push(&stack, v1 < v2);
        } else if (opcode == InstructionLe) {
            int32_t v2 = pop(&stack);
            int32_t v1 = pop(&stack);
            push(&stack, v1 <= v2);
        } else if (opcode == InstructionGt) {
            int32_t v2 = pop(&stack);
            int32_t v1 = pop(&stack);
            push(&stack, v1 > v2);
        } else if (opcode == InstructionGe) {
            int32_t v2 = pop(&stack);
            int32_t v1 = pop(&stack);
            push(&stack, v1 >= v2);
        } else if (opcode == InstructionLAnd) {
            int32_t v2 = pop(&stack);
            int32_t v1 = pop(&stack);
            push(&stack, v1 && v2);
        } else if (opcode == InstructionLOr) {
            int32_t v2 = pop(&stack);
            int32_t v1 = pop(&stack);
            push(&stack, v1 || v2);
        } else if (opcode == InstructionLNeg) {
            int32_t v1 = pop(&stack);
            push(&stack, !v1);
        } else if (opcode == InstructionFetch) {
            int32_t mem_val = bin->memory[op_arg];
            push(&stack, mem_val);
        } else if (opcode == InstructionStore) {
            int32_t value = pop(&stack);
            bin->memory[op_arg] = value;
        } else {
            fprintf(stderr, "Unknown operation %02x\n", opcode);
            exit(1);
        }
    }
    return pop(&stack);
}
