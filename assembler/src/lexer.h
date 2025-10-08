#pragma once

#include "value.h"

#include <stdbool.h>
#include <stddef.h>

enum TokenKind {
    TokenNoop,

    TokenNewLine,
};

struct Token {
    enum TokenKind kind;
    bool has_value;
    struct Value value;
};

struct TokenVec {
    size_t len;
    size_t capacity;
    struct Token *elements;
};

struct TokenVec *lex(char *filename);

struct TokenVec *token_vec_new();
void token_vec_push(struct TokenVec *vec, struct Token);
void token_vec_destroy(struct TokenVec *vec);

void token_vec_print(struct TokenVec *vec);
