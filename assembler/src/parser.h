#pragma once

#include <stddef.h>

#include "lexer.h"
#include "value.h"

enum InstructionKind {
    InstructionPush = 0x10,

    InstructionAdd = 0x20,

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

struct InstructionVec *parse(struct TokenVec *token_vec);

struct InstructionVec *instruction_vec_new();
void instruction_vec_push(struct InstructionVec *vec, struct Instruction);
void instruction_vec_destroy(struct InstructionVec *vec);

void instruction_vec_print(struct InstructionVec *vec);
