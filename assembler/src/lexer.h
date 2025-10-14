#pragma once

#include "value.h"

#include <stdbool.h>
#include <stddef.h>

enum TokenKind {
    TokenNoop,

    TokenJmp,
    TokenJEQZ,

    TokenPush,

    TokenAdd,
    TokenSub,
    TokenMul,

    TokenEq,
    TokenLt,
    TokenLe,
    TokenGt,
    TokenGe,

    TokenLAnd,
    TokenLOr,
    TokenLNeg,

    TokenFetch,
    TokenStore,

    TokenLabel,
    TokenIdent,

    TokenInt,
    TokenBool,

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
 * Get the string representation of a TokenKind
 *
 * @param struct Token *token
 * @param char *str
 */
void token_kind_to_string(struct Token *token, char **str);

/**
 * Get the string representation of a token
 *
 * @param struct Token *token
 * @param char *str
 */
void token_to_string(struct Token *token, char *str);

/**
 * Create a new TokenVec
 *
 * @returns empty TokenVec
 */
struct TokenVec *token_vec_new();

/**
 * Push a token onto a TokenVec
 *
 * @param TokenVec vec
 * @param Token token
 */
void token_vec_push(struct TokenVec *vec, struct Token token);

/**
 * Free a TokenVec
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
