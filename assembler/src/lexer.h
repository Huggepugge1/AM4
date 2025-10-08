#pragma once

#include "value.h"

#include <stdbool.h>
#include <stddef.h>

enum TokenKind {
    TokenPush,

    TokenAdd,

    TokenNoop,

    TokenInt,

    TokenNewLine,
};

struct Token {
    enum TokenKind kind;
    size_t line;
    size_t col;
    struct Value value;
};

struct TokenVec {
    size_t len;
    size_t capacity;
    struct Token *elements;
};

/**
 * Generate a vector (TokenVec) of tokens
 *
 * @param filename Name of the file to turn into tokens
 *
 * @returns All the tokens in a TokenVec
 */
struct TokenVec *lex(char *filename);

/**
 * Create a new TokenVec
 *
 * @returns empty TokenVec
 */
struct TokenVec *token_vec_new();

/**
 * Push a Token onto a TokenVec
 *
 * @param vec
 * @param token
 */
void token_vec_push(struct TokenVec *vec, struct Token token);

/**
 * Destroy a TokenVec
 *
 * @param vec
 */
void token_vec_destroy(struct TokenVec *vec);

/**
 * Print all tokens in a TokenVec
 *
 * @param vec
 */
void token_vec_print(struct TokenVec *vec);
