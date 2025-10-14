#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "lexer.h"
#include "parser.h"

bool expect(enum TokenKind token_kind, struct Token *token) {
    return token->kind == token_kind;
}

void parse_error_newline(struct Token *token) {
    char str[256] = {0};
    token_to_string(token, str);
    fprintf(stderr, "error(%zu:%zu): `%s` not followed by a int or bool\n",
            token->line, token->col, str);
    exit(1);
}

void parse_instruction(struct TokenVec *token_vec, struct LabelMap *labels,
                       struct Instruction *instruction, size_t instruction_addr,
                       size_t *i) {
    struct Token token = token_vec->elements[*i];
    instruction->value = token.value;

    if (token.kind == TokenNoop) {
        if (expect(TokenNewLine, &token_vec->elements[*i + 1])) {
            *i += 2;
            instruction->kind = InstructionNoop;
            return;
        }
        parse_error_newline(&token);
    }

    if (token.kind == TokenJmp) {
        if (expect(TokenLabel, &token_vec->elements[*i + 1])) {
            if (expect(TokenNewLine, &token_vec->elements[*i + 2])) {
                struct Token token = token_vec->elements[*i + 1];
                *i += 3;
                instruction->kind = InstructionJmp;
                instruction->value = token.value;
                return;
            } else {
                fprintf(stderr,
                        "error(%zu:%zu): `jmp %s` not followed by a newline\n",
                        token.line, token.col, instruction->value.value.string);
                exit(1);
            }
        } else {
            fprintf(stderr,
                    "error(%zu:%zu): `jmp` not followed by an address\n",
                    token.line, token.col);
            exit(1);
        }
    }
    if (token.kind == TokenJEQZ) {
        if (expect(TokenLabel, &token_vec->elements[*i + 1])) {
            if (expect(TokenNewLine, &token_vec->elements[*i + 2])) {
                struct Token token = token_vec->elements[*i + 1];
                *i += 3;
                instruction->kind = InstructionJEQZ;
                instruction->value = token.value;
                return;
            } else {
                fprintf(
                    stderr,
                    "error(%zu:%zu): `jmpeqz %s` not followed by a newline\n",
                    token.line, token.col, instruction->value.value.string);
                exit(1);
            }
        } else {
            fprintf(stderr,
                    "error(%zu:%zu): `jmpeqz` not followed by an address\n",
                    token.line, token.col);
            exit(1);
        }
    }

    if (token.kind == TokenPush) {
        if (expect(TokenInt, &token_vec->elements[*i + 1]) ||
            expect(TokenBool, &token_vec->elements[*i + 1])) {
            if (expect(TokenNewLine, &token_vec->elements[*i + 2])) {
                struct Token token = token_vec->elements[*i + 1];
                *i += 3;
                instruction->kind = InstructionPush;
                instruction->value = token.value;
                return;
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
            instruction->kind = InstructionAdd;
            return;
        }
        parse_error_newline(&token);
    }
    if (token.kind == TokenSub) {
        if (expect(TokenNewLine, &token_vec->elements[*i + 1])) {
            *i += 2;
            instruction->kind = InstructionSub;
            return;
        }
        parse_error_newline(&token);
    }
    if (token.kind == TokenMul) {
        if (expect(TokenNewLine, &token_vec->elements[*i + 1])) {
            *i += 2;
            instruction->kind = InstructionMul;
            return;
        }
        parse_error_newline(&token);
    }

    if (token.kind == TokenEq) {
        if (expect(TokenNewLine, &token_vec->elements[*i + 1])) {
            *i += 2;
            instruction->kind = InstructionEq;
            return;
        }
        parse_error_newline(&token);
    }
    if (token.kind == TokenLt) {
        if (expect(TokenNewLine, &token_vec->elements[*i + 1])) {
            *i += 2;
            instruction->kind = InstructionLt;
            return;
        }
        parse_error_newline(&token);
    }
    if (token.kind == TokenLe) {
        if (expect(TokenNewLine, &token_vec->elements[*i + 1])) {
            *i += 2;
            instruction->kind = InstructionLe;
            return;
        }
        parse_error_newline(&token);
    }
    if (token.kind == TokenGt) {
        if (expect(TokenNewLine, &token_vec->elements[*i + 1])) {
            *i += 2;
            instruction->kind = InstructionGt;
            return;
        }
        parse_error_newline(&token);
    }
    if (token.kind == TokenGe) {
        if (expect(TokenNewLine, &token_vec->elements[*i + 1])) {
            *i += 2;
            instruction->kind = InstructionGe;
            return;
        }
        parse_error_newline(&token);
    }

    if (token.kind == TokenLAnd) {
        if (expect(TokenNewLine, &token_vec->elements[*i + 1])) {
            *i += 2;
            instruction->kind = InstructionLAnd;
            return;
        }
        parse_error_newline(&token);
    }
    if (token.kind == TokenLOr) {
        if (expect(TokenNewLine, &token_vec->elements[*i + 1])) {
            *i += 2;
            instruction->kind = InstructionLOr;
            return;
        }
        parse_error_newline(&token);
    }
    if (token.kind == TokenLNeg) {
        if (expect(TokenNewLine, &token_vec->elements[*i + 1])) {
            *i += 2;
            instruction->kind = InstructionLNeg;
            return;
        }
        parse_error_newline(&token);
    }

    if (token.kind == TokenLabel) {
        if (expect(TokenNewLine, &token_vec->elements[*i + 1])) {
            *i += 2;
            struct Label label = {.ident = token.value.value.string,
                                  .addr = instruction_addr};
            label_map_insert(labels, label);
            instruction->kind = InstructionLabel;
            return;
        }
        parse_error_newline(&token);
    }
    char *token_string;
    token_kind_to_string(&token, &token_string);
    fprintf(stderr, "error(%zu:%zu): unexpected token `%s`\n", token.line,
            token.col, token_string);
    exit(1);
}

struct ParseResult parse(struct TokenVec *token_vec) {
    struct InstructionVec *instructions = instruction_vec_new();
    struct LabelMap *labels = label_map_new();
    struct ParseResult result = {.instructions = instructions,
                                 .labels = labels};

    // Relative addr to the current instruction
    size_t instruction_addr = 0;

    // Position in token vector
    size_t i = 0;
    while (i < token_vec->len) {
        printf("instruction_addr: %zu\n", instruction_addr);
        if (token_vec->elements[i].kind == TokenNewLine) {
            i++;
        }
        struct Instruction instruction;
        parse_instruction(token_vec, labels, &instruction, instruction_addr,
                          &i);

        if (instruction.kind != InstructionLabel) {
            instruction_vec_push(instructions, instruction);
            instruction_addr++;
        }
    }

    return result;
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

    case InstructionJmp:
        *str = "jmp";
        return;
    case InstructionJEQZ:
        *str = "jeqz";
        return;

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

    case InstructionLabel:
        *str = instruction->value.value.string;
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
    for (size_t i = 0; i < vec->len; i++) {
        if (vec->elements[i].value.kind == StringValue) {
            free(vec->elements[i].value.value.string);
        }
    }
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

struct LabelMap *label_map_new() {
    struct LabelMap *map = calloc(1, sizeof(struct LabelMap));
    map->elements = calloc(1, sizeof(struct LabelMap));
    map->len = 0;
    map->capacity = 1;
    return map;
}

void label_map_insert(struct LabelMap *map, struct Label label) {
    if (map->len == map->capacity) {
        map->elements =
            realloc(map->elements, map->capacity * 2 * sizeof(struct Label));
        map->capacity *= 2;
    }

    map->elements[map->len++] = label;
}

int32_t label_map_get(struct LabelMap *map, char *ident) {
    for (size_t i = 0; i < map->len; i++) {
        if (strcmp(map->elements[i].ident, ident) == 0) {
            return map->elements[i].addr;
        }
    }
    return -1;
}

void label_map_destroy(struct LabelMap *map) {
    for (size_t i = 0; i < map->len; i++) {
        free(map->elements[i].ident);
    }
    free(map->elements);
    free(map);
}

void label_map_print(struct LabelMap *map) {
    printf("struct LabelMap {\n");
    for (size_t i = 0; i < map->len; i++) {
        struct Label label = map->elements[i];
        printf("  %s: %d\n", label.ident, label.addr);
    }
    printf("}\n");
}
