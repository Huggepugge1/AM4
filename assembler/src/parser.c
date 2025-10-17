#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "lexer.h"
#include "parser.h"

bool expect(enum TokenKind token_kind, struct Token *token) {
    return token->kind == token_kind;
}

void parse_error_newline(struct Token *token) {
    char *str;
    token_kind_to_string(token, &str);
    fprintf(stderr, "error(%zu:%zu): expected `\\n`, found `%s`\n", token->line,
            token->col, str);
    exit(1);
}

struct Token next_token(struct TokenVec *tokens, size_t *i) {
    return tokens->elements[(*i)++];
}

void parse_instruction(struct TokenVec *tokens, struct LabelMap *labels,
                       struct IdentMap *idents, struct Instruction *instruction,
                       size_t instruction_addr, size_t *i) {
    struct Token token = next_token(tokens, i);
    instruction->value = token.value;

    if (token.kind == TokenNoop) {
        struct Token newline = next_token(tokens, i);
        if (expect(TokenNewLine, &newline)) {
            instruction->kind = InstructionNoop;
            return;
        }
        parse_error_newline(&newline);
    }

    if (token.kind == TokenJmp) {
        struct Token label = next_token(tokens, i);
        if (expect(TokenLabel, &label)) {
            struct Token newline = next_token(tokens, i);
            if (expect(TokenNewLine, &newline)) {
                instruction->kind = InstructionJmp;
                instruction->value = label.value;
                return;
            } else {
                fprintf(stderr,
                        "error(%zu:%zu): `jmp %s` not followed by a newline\n",
                        newline.line, newline.col,
                        instruction->value.value.string);
                exit(1);
            }
        } else {
            fprintf(stderr, "error(%zu:%zu): `jmp` not followed by a label\n",
                    label.line, label.col);
            exit(1);
        }
    }
    if (token.kind == TokenJEQZ) {
        struct Token label = next_token(tokens, i);
        if (expect(TokenLabel, &label)) {
            struct Token newline = next_token(tokens, i);
            if (expect(TokenNewLine, &newline)) {
                instruction->kind = InstructionJEQZ;
                instruction->value = label.value;
                return;
            } else {
                fprintf(
                    stderr,
                    "error(%zu:%zu): `jmpeqz %s` not followed by a newline\n",
                    newline.line, newline.col, instruction->value.value.string);
                exit(1);
            }
        } else {
            fprintf(stderr,
                    "error(%zu:%zu): `jmpeqz` not followed by a label\n",
                    label.line, label.col);
            exit(1);
        }
    }

    if (token.kind == TokenPush) {
        struct Token value = next_token(tokens, i);
        if (expect(TokenInt, &value) || expect(TokenBool, &value)) {
            struct Token newline = next_token(tokens, i);
            if (expect(TokenNewLine, &newline)) {
                instruction->kind = InstructionPush;
                instruction->value = value.value;
                return;
            } else {
                fprintf(stderr,
                        "error(%zu:%zu): `push <int/bool>` not followed by a "
                        "newline\n",
                        newline.line, newline.col);
                exit(1);
            }
        } else {
            fprintf(stderr,
                    "error(%zu:%zu): `push` not followed by a "
                    "int or a bool\n",
                    value.line, value.col);
            exit(1);
        }
    }

    if (token.kind == TokenAdd) {
        struct Token newline = next_token(tokens, i);
        if (expect(TokenNewLine, &newline)) {
            instruction->kind = InstructionAdd;
            return;
        }
        parse_error_newline(&token);
    }
    if (token.kind == TokenSub) {
        struct Token newline = next_token(tokens, i);
        if (expect(TokenNewLine, &newline)) {
            instruction->kind = InstructionSub;
            return;
        }
        parse_error_newline(&token);
    }
    if (token.kind == TokenMul) {
        struct Token newline = next_token(tokens, i);
        if (expect(TokenNewLine, &newline)) {
            instruction->kind = InstructionMul;
            return;
        }
        parse_error_newline(&token);
    }

    if (token.kind == TokenEq) {
        struct Token newline = next_token(tokens, i);
        if (expect(TokenNewLine, &newline)) {
            instruction->kind = InstructionEq;
            return;
        }
        parse_error_newline(&token);
    }
    if (token.kind == TokenLt) {
        struct Token newline = next_token(tokens, i);
        if (expect(TokenNewLine, &newline)) {
            instruction->kind = InstructionLt;
            return;
        }
        parse_error_newline(&token);
    }
    if (token.kind == TokenLe) {
        struct Token newline = next_token(tokens, i);
        if (expect(TokenNewLine, &newline)) {
            instruction->kind = InstructionLe;
            return;
        }
        parse_error_newline(&token);
    }
    if (token.kind == TokenGt) {
        struct Token newline = next_token(tokens, i);
        if (expect(TokenNewLine, &newline)) {
            instruction->kind = InstructionGt;
            return;
        }
        parse_error_newline(&token);
    }
    if (token.kind == TokenGe) {
        struct Token newline = next_token(tokens, i);
        if (expect(TokenNewLine, &newline)) {
            instruction->kind = InstructionGe;
            return;
        }
        parse_error_newline(&token);
    }

    if (token.kind == TokenLAnd) {
        struct Token newline = next_token(tokens, i);
        if (expect(TokenNewLine, &newline)) {
            instruction->kind = InstructionLAnd;
            return;
        }
        parse_error_newline(&token);
    }
    if (token.kind == TokenLOr) {
        struct Token newline = next_token(tokens, i);
        if (expect(TokenNewLine, &newline)) {
            instruction->kind = InstructionLOr;
            return;
        }
        parse_error_newline(&token);
    }
    if (token.kind == TokenLNeg) {
        struct Token newline = next_token(tokens, i);
        if (expect(TokenNewLine, &newline)) {
            instruction->kind = InstructionLNeg;
            return;
        }
        parse_error_newline(&token);
    }

    if (token.kind == TokenFetch) {
        struct Token ident = next_token(tokens, i);
        if (expect(TokenIdent, &ident)) {
            struct Token newline = next_token(tokens, i);
            if (expect(TokenNewLine, &newline)) {
                instruction->kind = InstructionFetch;
                instruction->value = ident.value;
                return;
            } else {
                fprintf(
                    stderr,
                    "error(%zu:%zu): `fetch %s` not followed by a newline\n",
                    newline.line, newline.col, instruction->value.value.string);
                exit(1);
            }
        } else {
            fprintf(stderr,
                    "error(%zu:%zu): `fetch` not followed by an identifier\n",
                    ident.line, ident.col);
            exit(1);
        }
    }
    if (token.kind == TokenStore) {
        struct Token ident = next_token(tokens, i);
        if (expect(TokenIdent, &ident)) {
            struct Token newline = next_token(tokens, i);
            if (expect(TokenNewLine, &newline)) {
                // Ownership if ident.value.value.string is transfered from the
                // Token to the Ident here
                ident_map_insert(idents, ident.value.value.string);
                instruction->kind = InstructionStore;
                char *string_value =
                    calloc(strlen(ident.value.value.string) + 1, sizeof(char));
                strcpy(string_value, ident.value.value.string);
                instruction->value.kind = StringValue;
                instruction->value.value.string = string_value;
                return;
            } else {
                fprintf(
                    stderr,
                    "error(%zu:%zu): `store %s` not followed by a newline\n",
                    newline.line, newline.col, instruction->value.value.string);
                exit(1);
            }
        } else {
            fprintf(stderr,
                    "error(%zu:%zu): `store` not followed by an identifier\n",
                    ident.line, ident.col);
            exit(1);
        }
    }

    if (token.kind == TokenPrintC) {
        struct Token value = next_token(tokens, i);
        if (expect(TokenInt, &value) || expect(TokenBool, &value)) {
            struct Token newline = next_token(tokens, i);
            if (expect(TokenNewLine, &newline)) {
                instruction->kind = InstructionPrintC;
                instruction->value = value.value;
                return;
            } else {
                fprintf(stderr,
                        "error(%zu:%zu): `printc <int/bool>` not followed by a "
                        "newline\n",
                        newline.line, newline.col);
                exit(1);
            }
        } else {
            fprintf(stderr,
                    "error(%zu:%zu): `printc` not followed by a "
                    "int or a bool\n",
                    value.line, value.col);
            exit(1);
        }
    }
    if (token.kind == TokenPrintV) {
        struct Token ident = next_token(tokens, i);
        if (expect(TokenIdent, &ident)) {
            struct Token newline = next_token(tokens, i);
            if (expect(TokenNewLine, &newline)) {
                instruction->kind = InstructionPrintV;
                instruction->value = ident.value;
                return;
            } else {
                fprintf(
                    stderr,
                    "error(%zu:%zu): `printv %s` not followed by a newline\n",
                    newline.line, newline.col, instruction->value.value.string);
                exit(1);
            }
        } else {
            fprintf(stderr,
                    "error(%zu:%zu): `printv` not followed by an identifier\n",
                    ident.line, ident.col);
            exit(1);
        }
    }

    if (token.kind == TokenLabel) {
        struct Token newline = next_token(tokens, i);
        if (expect(TokenNewLine, &newline)) {
            struct Label label = {.ident = token.value.value.string,
                                  .addr = instruction_addr};
            label_map_insert(labels, label);
            instruction->kind = InstructionLabel;
            return;
        }
        parse_error_newline(&token);
    }
    if (token.kind == TokenIdent) {
        char *token_string;
        token_kind_to_string(&token, &token_string);
        fprintf(stderr, "error(%zu:%zu): unexpected token `%s`\n", token.line,
                token.col, token_string);
        fprintf(stderr,
                "An identifier has to be preceded by `store` or `load`");
        exit(1);
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
    struct IdentMap *idents = ident_map_new();
    struct ParseResult result = {
        .instructions = instructions, .labels = labels, .idents = idents};

    // Relative addr to the current instruction
    size_t instruction_addr = 0;

    // Position in token vector
    size_t i = 0;
    while (i < token_vec->len) {
        if (token_vec->elements[i].kind == TokenNewLine) {
            i++;
        }
        struct Instruction instruction;
        parse_instruction(token_vec, labels, idents, &instruction,
                          instruction_addr, &i);

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
        *str = "lneg";
        return;

    case InstructionFetch:
        *str = "fetch";
        return;
    case InstructionStore:
        *str = "store";
        return;

    case InstructionPrintC:
        *str = "printc";
        return;
    case InstructionPrintV:
        *str = "printv";
        return;

    case InstructionLabel:
        *str = instruction->value.value.string;
        return;
    case InstructionIdent:
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

struct IdentMap *ident_map_new() {
    struct IdentMap *map = calloc(1, sizeof(struct IdentMap));
    map->elements = calloc(1, sizeof(struct Ident));
    map->len = 0;
    map->capacity = 1;
    return map;
}

void ident_map_insert(struct IdentMap *map, char *ident_string) {
    // Ident already in map
    if (ident_map_get(map, ident_string) != -1)
        return;
    if (map->len == map->capacity) {
        map->elements =
            realloc(map->elements, map->capacity * 2 * sizeof(struct Ident));
        map->capacity *= 2;
    }

    struct Ident ident = {.ident = ident_string, .addr = map->len};
    map->elements[map->len++] = ident;
}

int32_t ident_map_get(struct IdentMap *map, char *ident) {
    for (size_t i = 0; i < map->len; i++) {
        if (strcmp(map->elements[i].ident, ident) == 0) {
            return map->elements[i].addr;
        }
    }
    return -1;
}

void ident_map_destroy(struct IdentMap *map) {
    for (size_t i = 0; i < map->len; i++) {
        free(map->elements[i].ident);
    }
    free(map->elements);
    free(map);
}

void ident_map_print(struct IdentMap *map) {
    printf("struct IdentMap {\n");
    for (size_t i = 0; i < map->len; i++) {
        struct Ident ident = map->elements[i];
        printf("  %s: %d\n", ident.ident, ident.addr);
    }
    printf("}\n");
}
