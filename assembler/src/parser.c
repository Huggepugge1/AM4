#include <stdio.h>
#include <stdlib.h>

#include "lexer.h"
#include "parser.h"

bool expect(enum TokenKind token_kind, struct Token *token) {
    return token->kind == token_kind;
}

void parse_error_newline(struct Token *token) {
    char str[256];
    token_to_string(token, str);
    fprintf(stderr, "error(%zu:%zu): `%s` not followed by a int or bool\n",
            token->line, token->col, str);
    exit(1);
}

struct Instruction parse_instruction(struct TokenVec *token_vec, size_t *i) {
    struct Token token = token_vec->elements[*i];
    struct Instruction instruction;
    instruction.value = token.value;

    if (token.kind == TokenPush) {
        if (expect(TokenInt, &token_vec->elements[*i + 1]) ||
            expect(TokenBool, &token_vec->elements[*i + 1])) {
            if (expect(TokenNewLine, &token_vec->elements[*i + 2])) {
                struct Token token = token_vec->elements[*i + 1];
                *i += 3;
                struct Instruction instruction = {.kind = InstructionPush,
                                                  .value = token.value};
                return instruction;
            } else {
                fprintf(stderr,
                        "error(%zu:%zu): `push <int/bool>` not followed by a "
                        "newline\n",
                        token.line, token.col);
                exit(1);
            }
        } else {
            fprintf(stderr,
                    "error(%zu:%zu): `push` not followed by a "
                    "int / bool\n",
                    token.line, token.col);
            exit(1);
        }
    }

    if (token.kind == TokenAdd) {
        if (expect(TokenNewLine, &token_vec->elements[*i + 1])) {
            *i += 2;
            instruction.kind = InstructionAdd;
            return instruction;
        }
        parse_error_newline(&token);
    }
    if (token.kind == TokenSub) {
        if (expect(TokenNewLine, &token_vec->elements[*i + 1])) {
            *i += 2;
            instruction.kind = InstructionSub;
            return instruction;
        }
        parse_error_newline(&token);
    }
    if (token.kind == TokenMul) {
        if (expect(TokenNewLine, &token_vec->elements[*i + 1])) {
            *i += 2;
            instruction.kind = InstructionMul;
            return instruction;
        }
        parse_error_newline(&token);
    }

    if (token.kind == TokenEq) {
        if (expect(TokenNewLine, &token_vec->elements[*i + 1])) {
            *i += 2;
            instruction.kind = InstructionEq;
            return instruction;
        }
        parse_error_newline(&token);
    }
    if (token.kind == TokenLt) {
        if (expect(TokenNewLine, &token_vec->elements[*i + 1])) {
            *i += 2;
            instruction.kind = InstructionLt;
            return instruction;
        }
        parse_error_newline(&token);
    }
    if (token.kind == TokenLe) {
        if (expect(TokenNewLine, &token_vec->elements[*i + 1])) {
            *i += 2;
            instruction.kind = InstructionLe;
            return instruction;
        }
        parse_error_newline(&token);
    }
    if (token.kind == TokenGt) {
        if (expect(TokenNewLine, &token_vec->elements[*i + 1])) {
            *i += 2;
            instruction.kind = InstructionGt;
            return instruction;
        }
        parse_error_newline(&token);
    }
    if (token.kind == TokenGe) {
        if (expect(TokenNewLine, &token_vec->elements[*i + 1])) {
            *i += 2;
            instruction.kind = InstructionGe;
            return instruction;
        }
        parse_error_newline(&token);
    }

    if (token.kind == TokenLAnd) {
        if (expect(TokenNewLine, &token_vec->elements[*i + 1])) {
            *i += 2;
            instruction.kind = InstructionLAnd;
            return instruction;
        }
        parse_error_newline(&token);
    }
    if (token.kind == TokenLOr) {
        if (expect(TokenNewLine, &token_vec->elements[*i + 1])) {
            *i += 2;
            instruction.kind = InstructionLOr;
            return instruction;
        }
        parse_error_newline(&token);
    }
    if (token.kind == TokenLNeg) {
        if (expect(TokenNewLine, &token_vec->elements[*i + 1])) {
            *i += 2;
            instruction.kind = InstructionLNeg;
            return instruction;
        }
        parse_error_newline(&token);
    }

    if (token.kind == TokenNoop) {
        if (expect(TokenNewLine, &token_vec->elements[*i + 1])) {
            *i += 2;
            instruction.kind = InstructionNoop;
            return instruction;
        }
        parse_error_newline(&token);
    }
    char *token_string;
    token_kind_to_string(&token, &token_string);
    fprintf(stderr, "error(%zu:%zu): unexpected token `%s`\n", token.line,
            token.col, token_string);
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
    case InstructionPush:
        *str = "push";
        return;
    case InstructionAdd:
        *str = "add";
        return;
    case InstructionSub:
        *str = "sub";
        return;
    case InstructionMul:
        *str = "mul";
        return;

    case InstructionEq:
        *str = "eq";
        return;
    case InstructionLt:
        *str = "lt";
        return;
    case InstructionLe:
        *str = "le";
        return;
    case InstructionGt:
        *str = "gt";
        return;
    case InstructionGe:
        *str = "ge";
        return;

    case InstructionLAnd:
        *str = "land";
        return;
    case InstructionLOr:
        *str = "lor";
        return;
    case InstructionLNeg:
        *str = "lor";
        return;

    case InstructionNoop:
        *str = "noop";
        return;
    }
    fprintf(stderr, "Unreachable statement reached in token_kind_to_string\n");
    exit(1);
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
