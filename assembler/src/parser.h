#pragma once

#include <stddef.h>

#include "lexer.h"
#include "value.h"

enum InstructionKind {
    InstructionPush = 0x10,

    InstructionAdd = 0x20,
    InstructionSub = 0x21,
    InstructionMul = 0x22,

    InstructionNoop = 0x00,
};

struct Instruction {
    enum InstructionKind kind;
    struct Value value;
};

struct InstructionVec {
    size_t len;
    size_t capacity;
    struct Instruction *elements;
};

/**
 * Generate a vector (InstructionVec) of Instructions
 *
 * @param token_vec
 *
 * @returns InstructionVec* All the instructions
 */
struct InstructionVec *parse(struct TokenVec *token_vec);

/**
 * Create a new InstructionVec*
 *
 * @returns InstructionVec* empty InstructionVec
 */
struct InstructionVec *instruction_vec_new();

/**
 * Push an Instruction onto an InstructionVec
 *
 * @note If there is not enough space, space is allocated
 *
 * @param vec
 * @param instruction
 */
void instruction_vec_push(struct InstructionVec *vec,
                          struct Instruction instruction);

/**
 * Free an InstructionVec
 *
 * @param vec
 */
void instruction_vec_destroy(struct InstructionVec *vec);

/**
 * Print all instructions in a TokenVec
 *
 * @param vec
 */
void instruction_vec_print(struct InstructionVec *vec);
