#include "lexer.h"
#include "arguments.h"
#include "value.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

bool char_is_white_space(char c) { return c == ' ' || c == '\n' || c == '\t'; }

void lex_string(char *str, struct TokenVec *vec);

struct Token token_get(char *string) {
    if (strcmp(string, "noop") == 0) {
        struct Token token = {.kind = TokenNoop, .value = {.kind = None}};
        return token;
    }
    if (strcmp(string, "\n") == 0) {
        struct Token token = {.kind = TokenNewLine, .value = {.kind = None}};
        return token;
    }
    fprintf(stderr, "Could not parse token `%s`\n", string);
    exit(1);
}

void token_kind_to_string(struct Token *token,
                          __attribute__((unused)) char **str) {
    switch (token->kind) {
    case TokenNoop:
        *str = "noop";
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
    while (fgets(file_contents, 65535, fptr)) {
        lex_string(file_contents, vec);
    }

    return vec;
}

void lex_string(char *str, struct TokenVec *vec) {
    char current_string[256] = {0};
    size_t pos = 0;

    while (*str) {
        while (*str && !char_is_white_space(*str)) {
            current_string[pos++] = *str++;
            if (pos > 254) {
                fprintf(stderr,
                        "Found a token more than 254 characters long\n");
                exit(1);
            }
        }
        if (strlen(current_string) > 0) {
            token_vec_push(vec, token_get(current_string));
        }
        if (*str == '\n') {
            token_vec_push(vec, token_get("\n"));
        }
        memset(current_string, 0, 256);
        pos = 0;
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
