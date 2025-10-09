#include <ctype.h>
#include <math.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "arguments.h"
#include "lexer.h"
#include "value.h"

bool char_is_white_space(char c) { return c == ' ' || c == '\n' || c == '\t'; }

bool is_int(char *str) {
    while (*str) {
        if (!isdigit(*str++)) {
            return false;
        }
    }
    return true;
}

void lex_string(char *str, size_t line, struct TokenVec *vec);

struct Token token_get(char *str, size_t line, size_t col) {
    // Line has to be one indexed
    line += 1;
    col += 1;
    if (strcmp(str, "noop") == 0) {
        struct Token token = {.kind = TokenNoop,
                              .line = line,
                              .col = col,
                              .value = {.kind = None}};
        return token;
    }
    if (strcmp(str, "push") == 0) {
        struct Token token = {.kind = TokenPush,
                              .line = line,
                              .col = col,
                              .value = {.kind = None}};
        return token;
    }
    if (strcmp(str, "add") == 0) {
        struct Token token = {.kind = TokenAdd,
                              .line = line,
                              .col = col,
                              .value = {.kind = None}};
        return token;
    }
    if (strcmp(str, "sub") == 0) {
        struct Token token = {.kind = TokenSub,
                              .line = line,
                              .col = col,
                              .value = {.kind = None}};
        return token;
    }
    if (strcmp(str, "mul") == 0) {
        struct Token token = {.kind = TokenMul,
                              .line = line,
                              .col = col,
                              .value = {.kind = None}};
        return token;
    }
    if (strcmp(str, "eq") == 0) {
        struct Token token = {
            .kind = TokenEq, .line = line, .col = col, .value = {.kind = None}};
        return token;
    }
    if (strcmp(str, "lt") == 0) {
        struct Token token = {
            .kind = TokenLt, .line = line, .col = col, .value = {.kind = None}};
        return token;
    }
    if (strcmp(str, "le") == 0) {
        struct Token token = {
            .kind = TokenLe, .line = line, .col = col, .value = {.kind = None}};
        return token;
    }
    if (strcmp(str, "gt") == 0) {
        struct Token token = {
            .kind = TokenGt, .line = line, .col = col, .value = {.kind = None}};
        return token;
    }
    if (strcmp(str, "ge") == 0) {
        struct Token token = {
            .kind = TokenGe, .line = line, .col = col, .value = {.kind = None}};
        return token;
    }
    if (strcmp(str, "land") == 0) {
        struct Token token = {.kind = TokenLAnd,
                              .line = line,
                              .col = col,
                              .value = {.kind = None}};
    }

    if (strcmp(str, "lor") == 0) {
        struct Token token = {.kind = TokenLOr,
                              .line = line,
                              .col = col,
                              .value = {.kind = None}};
        return token;
    }
    if (strcmp(str, "lneg") == 0) {
        struct Token token = {.kind = TokenLNeg,
                              .line = line,
                              .col = col,
                              .value = {.kind = None}};
        return token;
    }
    if (is_int(str)) {
        int32_t int_value = strtol(str, NULL, 10);
        struct Token token = {
            .kind = TokenInt,
            .line = line,
            .col = col,
            .value = {.kind = IntValue, .value = {.integer = int_value}}};
        if (int_value > pow(2, 23) - 1) {
            fprintf(stderr,
                    "warning(%zu:%zd): `%d` cannot fit within 24 bits and will "
                    "be truncated\n",
                    line, col, int_value);
        }
        return token;
    }
    if (strcmp(str, "false") == 0) {
        struct Token token = {
            .kind = TokenBool,
            .line = line,
            .col = col,
            .value = {.kind = BoolValue, .value = {.boolean = false}}};
        return token;
    }
    if (strcmp(str, "true") == 0) {
        struct Token token = {
            .kind = TokenBool,
            .line = line,
            .col = col,
            .value = {.kind = BoolValue, .value = {.boolean = true}}};
        return token;
    }
    if (strcmp(str, "\n") == 0) {
        struct Token token = {.kind = TokenNewLine,
                              .line = line,
                              .col = col,
                              .value = {.kind = None}};
        return token;
    }
    fprintf(stderr, "error(%zu:%zu): Could not parse token `%s`\n", line + 1,
            col + 1, str);
    exit(1);
}

void token_kind_to_string(struct Token *token,
                          __attribute__((unused)) char **str) {
    switch (token->kind) {
    case TokenPush:
        *str = "push";
        return;

    case TokenAdd:
        *str = "add";
        return;
    case TokenSub:
        *str = "sub";
        return;
    case TokenMul:
        *str = "mul";
        return;

    case TokenEq:
        *str = "eq";
        return;
    case TokenLt:
        *str = "lt";
        return;
    case TokenLe:
        *str = "le";
        return;
    case TokenGt:
        *str = "gt";
        return;
    case TokenGe:
        *str = "ge";
        return;

    case TokenLAnd:
        *str = "land";
        return;
    case TokenLOr:
        *str = "lor";
        return;
    case TokenLNeg:
        *str = "lneg";
        return;

    case TokenNoop:
        *str = "noop";
        return;
    case TokenInt:
        *str = "int";
        return;
    case TokenBool:
        *str = "bool";
        return;
    case TokenNewLine:
        *str = "\\n";
        return;
    };
    fprintf(stderr, "Unreachable statement reached in token_kind_to_string\n");
    exit(1);
}

void token_to_string(struct Token *token, char *str) {
    char *token_kind;
    token_kind_to_string(token, &token_kind);
    char *value;
    value_to_string(&token->value, &value);
    sprintf(str, "struct Token { .kind = %s, .value = %s }", token_kind, value);
}

struct TokenVec *lex(char *filename) {
    struct TokenVec *vec = token_vec_new();

    FILE *fptr;
    fptr = fopen(filename, "r");
    char file_contents[65535];
    size_t line = 0;
    while (fgets(file_contents, 65535, fptr)) {
        lex_string(file_contents, line, vec);
        line++;
    }

    fclose(fptr);

    return vec;
}

void lex_string(char *str, size_t line, struct TokenVec *vec) {
    char current_string[256] = {0};
    size_t pos = 0;
    size_t col = 0;

    while (*str) {
        while (*str && !char_is_white_space(*str)) {
            if (pos > 0 && *str == '/' && current_string[pos - 1] == '/') {
                token_vec_push(vec, token_get("\n", line, strlen(str)));
                return;
            }
            current_string[pos++] = *str++;
            if (pos > 254) {
                fprintf(stderr,
                        "error(%zu:%zu): Found a token more than 254 "
                        "characters long\n",
                        line + 1, col + 1);
                exit(1);
            }
        }
        if (strlen(current_string) > 0) {
            token_vec_push(vec, token_get(current_string, line, col));
        }
        if (*str == '\n') {
            token_vec_push(vec, token_get("\n", line, col));
        }
        memset(current_string, 0, 256);
        pos = 0;
        col = pos;
        str++;
    }
}

struct TokenVec *token_vec_new() {
    struct TokenVec *vec = calloc(1, sizeof(struct TokenVec));
    vec->elements = calloc(1, sizeof(struct Token));
    vec->len = 0;
    vec->capacity = 1;
    return vec;
}

void token_vec_push(struct TokenVec *vec, struct Token token) {
    if (vec->len == vec->capacity) {
        vec->elements =
            realloc(vec->elements, vec->capacity * 2 * sizeof(struct Token));
        vec->capacity *= 2;
    }

    vec->elements[vec->len++] = token;
}

void token_vec_destroy(struct TokenVec *vec) {
    free(vec->elements);
    free(vec);
}

void token_vec_print(struct TokenVec *vec) {
    printf("struct TokenVec {\n");
    for (size_t i = 0; i < vec->len; i++) {
        char token[256] = {0};
        token_to_string(&vec->elements[i], token);
        printf("  %s\n", token);
    }
    printf("}\n");
}
