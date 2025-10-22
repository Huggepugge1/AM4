#include <ctype.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "arguments.h"
#include "lexer.h"
#include "value.h"

bool char_is_white_space(const char c) {
    return c == ' ' || c == '\n' || c == '\t' || c == '\r';
}

bool is_int(const char *str) {
    if (*str == '-') {
        str++;
    }
    while (*str) {
        if (!isdigit(*str++)) {
            return false;
        }
    }
    return true;
}

bool is_ident(const char *str) {
    while (*str) {
        if (!(isalnum(*str) || *str == '_')) {
            return false;
        }
        str++;
    }
    return true;
}

void lex_line(char *str, size_t line, struct TokenVec *vec);

struct Token token_get(const char *str, size_t line, size_t col) {
    struct Token token = {.line = line, .col = col, .value = {.kind = None}};
    if (strcmp(str, "noop") == 0) {
        token.kind = TokenNoop;
        return token;
    }

    if (strcmp(str, "jmp") == 0) {
        token.kind = TokenJmp;
        return token;
    }
    if (strcmp(str, "jeqz") == 0) {
        token.kind = TokenJEQZ;
        return token;
    }

    if (strcmp(str, "push") == 0) {
        token.kind = TokenPush;
        return token;
    }

    if (strcmp(str, "add") == 0) {
        token.kind = TokenAdd;
        return token;
    }
    if (strcmp(str, "sub") == 0) {
        token.kind = TokenSub;
        return token;
    }
    if (strcmp(str, "mul") == 0) {
        token.kind = TokenMul;
        return token;
    }

    if (strcmp(str, "eq") == 0) {
        token.kind = TokenEq;
        return token;
    }
    if (strcmp(str, "lt") == 0) {
        token.kind = TokenLt;
        return token;
    }
    if (strcmp(str, "le") == 0) {
        token.kind = TokenLe;
        return token;
    }
    if (strcmp(str, "gt") == 0) {
        token.kind = TokenGt;
        return token;
    }
    if (strcmp(str, "ge") == 0) {
        token.kind = TokenGe;
        return token;
    }

    if (strcmp(str, "land") == 0) {
        token.kind = TokenLAnd;
        return token;
    }
    if (strcmp(str, "lor") == 0) {
        token.kind = TokenLOr;
        return token;
    }
    if (strcmp(str, "lneg") == 0) {
        token.kind = TokenLNeg;
        return token;
    }

    if (strcmp(str, "fetch") == 0) {
        token.kind = TokenFetch;
        return token;
    }
    if (strcmp(str, "store") == 0) {
        token.kind = TokenStore;
        return token;
    }
    if (strcmp(str, "printc") == 0) {
        token.kind = TokenPrintC;
        return token;
    }
    if (strcmp(str, "printv") == 0) {
        token.kind = TokenPrintV;
        return token;
    }

    if (str[strlen(str) - 1] == ':') {
        token.kind = TokenLabel;
        token.value.kind = StringValue;
        token.value.value.string = calloc(strlen(str) + 1, sizeof(char));
        strcpy(token.value.value.string, str);
        return token;
    }

    if (is_int(str)) {
        int32_t int_value = strtol(str, NULL, 10);
        token.kind = TokenInt;
        token.value.kind = IntValue;
        token.value.value.integer = int_value;
        if (int_value > (1 << 23) - 1 || int_value < -(1 << 23)) {
            fprintf(stderr,
                    "warning(%zu:%zd): `%d` cannot fit within 24 bits and will "
                    "be truncated\n",
                    line, col, int_value);
        }
        return token;
    }

    if (strcmp(str, "false") == 0) {
        token.kind = TokenBool;
        token.value.kind = BoolValue;
        token.value.value.boolean = false;
        return token;
    }
    if (strcmp(str, "true") == 0) {
        token.kind = TokenBool;
        token.value.kind = BoolValue;
        token.value.value.boolean = true;
        return token;
    }

    if (strcmp(str, "\n") == 0) {
        token.kind = TokenNewLine;
        return token;
    }

    if (is_ident(str)) {
        token.kind = TokenIdent;
        token.value.kind = StringValue;
        token.value.value.string = calloc(strlen(str) + 1, sizeof(char));
        strcpy(token.value.value.string, str);
        return token;
    }
    fprintf(stderr, "error(%zu:%zu): Could not parse token `%s`\n", line, col,
            str);
    exit(1);
}

void token_kind_to_string(struct Token *token,
                          __attribute__((unused)) char **str) {
    switch (token->kind) {
    case TokenNoop:
        *str = "noop";
        return;

    case TokenJmp:
        *str = "jmp";
        return;
    case TokenJEQZ:
        *str = "jeqz";
        return;

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

    case TokenFetch:
        *str = "fetch";
        return;
    case TokenStore:
        *str = "store";
        return;

    case TokenPrintC:
        *str = "printc";
        return;
    case TokenPrintV:
        *str = "printv";
        return;

    case TokenLabel:
        *str = "label";
        return;
    case TokenIdent:
        *str = "ident";
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
    sprintf(str,
            "struct Token { .kind = %s, .value = %s, .line = %zu, .col = %zu }",
            token_kind, value, token->line, token->col);
}

struct TokenVec *lex(char *filename) {
    struct TokenVec *vec = token_vec_new();

    FILE *fptr;
    fptr = fopen(filename, "r");
    if (fptr == NULL) {
        fprintf(stderr, "Failed to open file: %s\n", filename);
        exit(1);
    }

    char *line = NULL;
    size_t len = 0;
    size_t chars_read;

    // One-indexed
    size_t line_num = 1;
    while ((chars_read = getline(&line, &len, fptr)) != -1) {
        lex_line(line, line_num, vec);
        line_num++;
    }

    fclose(fptr);
    free(line);

    return vec;
}

#define MAX_TOKEN_SIZE 256

void lex_line(char *str, size_t line, struct TokenVec *vec) {
    char current_token[MAX_TOKEN_SIZE] = {0};

    // One-indexed
    size_t col = 1;

    while (*str) {
        size_t pos = 0;
        while (*str && !char_is_white_space(*str)) {
            if (pos > 0 && *str == '/' && current_token[pos - 1] == '/') {
                // Push the newline
                token_vec_push(vec, token_get("\n", line, strlen(str) + col));
                return;
            }

            current_token[pos++] = *str++;

            if (pos > MAX_TOKEN_SIZE - 1) {
                fprintf(stderr,
                        "error(%zu:%zu): Found a token more than 255 "
                        "characters long\n",
                        line, col);
                exit(1);
            }
        }

        if (pos > 0) {
            token_vec_push(vec, token_get(current_token, line, col));
        }

        col += pos + 1;

        if (*str == '\n') {
            // Col - 1 because IDK, works i guess
            token_vec_push(vec, token_get("\n", line, col - 1));
            return;
        }

        memset(current_token, 0, MAX_TOKEN_SIZE);
        str++;
    }
}

struct TokenVec *token_vec_new() {
    struct TokenVec *vec = calloc(1, sizeof(struct TokenVec));
    if (vec == NULL) {
        fprintf(stderr, "Failed to allocate memory");
        exit(1);
    }
    vec->elements = calloc(1, sizeof(struct Token));
    if (vec->elements == NULL) {
        fprintf(stderr, "Failed to allocate memory");
        exit(1);
    }
    vec->len = 0;
    vec->capacity = 1;
    return vec;
}

void token_vec_push(struct TokenVec *vec, struct Token token) {
    if (vec->len == vec->capacity) {
        vec->elements =
            realloc(vec->elements, vec->capacity * 2 * sizeof(struct Token));
        if (vec->elements == NULL) {
            fprintf(stderr, "Failed to reallocate memory");
            exit(1);
        }
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
        char token[MAX_TOKEN_SIZE] = {0};
        token_to_string(&vec->elements[i], token);
        printf("  %s\n", token);
    }
    printf("}\n");
}
