#include <stdio.h>
#include <stdlib.h>

#include "lexer.h"
#include "parser.h"

bool expect(enum TokenKind token_kind, struct TokenVec *token_vec, size_t i) {
    return token_vec->elements[i].kind == token_kind;
}

struct Instruction parse_instruction(struct TokenVec *token_vec, size_t *i) {
    if (token_vec->elements[*i].kind == TokenNoop) {
        if (expect(TokenNewLine, token_vec, *i + 1)) {
            *i += 2;
            struct Instruction instruction = {.kind = InstructionNoop,
                                              .value = {.kind = None}};
            return instruction;
        }

        fprintf(stderr, "Noop not followed by a newline\n");
        exit(1);
    }

    fprintf(stderr, "Unreachable statement reached in parse_instruction\n");
    exit(1);
}

struct InstructionVec *parse(struct TokenVec *token_vec) {
    struct InstructionVec *instructions = instruction_vec_new();

    size_t i = 0;
    while (i < token_vec->len) {
        if (token_vec->elements[i].kind == TokenNewLine) {
            i++;
        }
        instruction_vec_push(instructions, parse_instruction(token_vec, &i));
    }

    return instructions;
}

struct InstructionVec *instruction_vec_new();
void instruction_vec_push(struct InstructionVec *vec, struct Instruction);
void instruction_vec_destroy(struct InstructionVec *vec);

void instruction_vec_print(struct InstructionVec *vec);

void instruction_kind_to_string(struct Instruction *instruction,
                                __attribute__((unused)) char **str) {
    switch (instruction->kind) {
    case InstructionNoop:
        *str = "noop";
        return;
        fprintf(stderr,
                "Unreachable statement reached in token_kind_to_string\n");
        exit(1);
    }
}

void instruction_to_string(struct Instruction *instruction, char (*str)[256]) {
    char *token_kind;
    instruction_kind_to_string(instruction, &token_kind);
    char *value;
    value_to_string(&instruction->value, &value);
    sprintf(*str, "struct Instruction { .kind = %s, .value = %s }", token_kind,
            value);
}

struct InstructionVec *instruction_vec_new() {
    struct InstructionVec *vec = calloc(1, sizeof(struct InstructionVec));
    vec->elements = calloc(1, sizeof(struct Instruction));
    vec->len = 0;
    vec->capacity = 1;
    return vec;
}

void instruction_vec_push(struct InstructionVec *vec,
                          struct Instruction instruction) {
    if (vec->len == vec->capacity) {
        vec->elements = realloc(vec->elements,
                                vec->capacity * 2 * sizeof(struct Instruction));
        vec->capacity *= 2;
    }

    vec->elements[vec->len++] = instruction;
}

void instruction_vec_destroy(struct InstructionVec *vec) {
    free(vec->elements);
    free(vec);
}

void instruction_vec_print(struct InstructionVec *vec) {
    printf("struct InstructionVec {\n");
    for (size_t i = 0; i < vec->len; i++) {
        char instruction[256] = {0};
        instruction_to_string(&vec->elements[i], &instruction);
        printf("  %s\n", instruction);
    }
    printf("}\n");
}
