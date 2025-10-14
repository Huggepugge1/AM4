#pragma once

#include <stddef.h>

#include "lexer.h"
#include "value.h"

enum InstructionKind {
    InstructionNoop = 0x00,

    InstructionJmp = 0x01,
    InstructionJEQZ = 0x02,

    InstructionPush = 0x10,

    InstructionAdd = 0x20,
    InstructionSub = 0x30,
    InstructionMul = 0x40,

    InstructionEq = 0xa0,
    InstructionLt = 0xa1,
    InstructionLe = 0xa2,
    InstructionGt = 0xa3,
    InstructionGe = 0xa4,

    InstructionLAnd = 0xaa,
    InstructionLOr = 0xab,
    InstructionLNeg = 0xb0,

    InstructionFetch = 0xc0,
    InstructionStore = 0xc1,

    InstructionLabel,
    InstructionIdent,
};

struct Instruction {
    enum InstructionKind kind;
    struct Value value;
};

struct InstructionVec {
    size_t len;
    size_t capacity;
    struct Instruction *elements;
};

struct Label {
    char *ident;
    int32_t addr;
};

struct LabelMap {
    size_t len;
    size_t capacity;
    struct Label *elements;
};

struct Ident {
    char *ident;
    int32_t addr;
};

struct IdentMap {
    size_t len;
    size_t capacity;
    struct Ident *elements;
};

struct ParseResult {
    struct InstructionVec *instructions;
    struct LabelMap *labels;
    struct IdentMap *idents;
};

/**
 * Generate a vector (InstructionVec) of Instructions
 *
 * @param token_vec
 *
 * @returns InstructionVec* All the instructions
 */
struct ParseResult parse(struct TokenVec *token_vec);

/**
 * Create a new InstructionVec*
 *
 * @returns InstructionVec* empty InstructionVec
 */
struct InstructionVec *instruction_vec_new();

/**
 * Push an Instruction onto an InstructionVec
 *
 * @note If there is not enough space, space is allocated
 *
 * @param vec
 * @param instruction
 */
void instruction_vec_push(struct InstructionVec *vec,
                          struct Instruction instruction);

/**
 * Free an InstructionVec
 *
 * @param vec
 */
void instruction_vec_destroy(struct InstructionVec *vec);

/**
 * Print all instructions in a TokenVec
 *
 * @param vec
 */
void instruction_vec_print(struct InstructionVec *vec);

/**
 * Create a new LabelMap*
 *
 * @returns LabelMap* empty LabelMap
 */
struct LabelMap *label_map_new();

/**
 * Insert a Label into a LabelMap
 *
 * @note If there is not enough space, space is allocated
 *
 * @param map
 * @param label
 */
void label_map_insert(struct LabelMap *map, struct Label label);

/**
 * Get the address of a label
 *
 * @note Returns -1 on fail
 *
 * @note If there is not enough space, space is allocated
 *
 * @param map
 * @param char *ident
 *
 * @returns int32_t
 */
int32_t label_map_get(struct LabelMap *map, char *ident);

/**
 * Free a LabelMap
 *
 * @param map
 */
void label_map_destroy(struct LabelMap *map);

/**
 * Print all instructions in a TokenVec
 *
 * @param map
 */
void label_map_print(struct LabelMap *map);

/**
 * Create a new IdentMap*
 *
 * @returns IdentMap* empty IdentMap
 */
struct IdentMap *ident_map_new();

/**
 * Insert a ident into a IdentMap
 *
 * @note If there is not enough space, space is allocated
 *
 * @param map
 * @param ident
 */
void ident_map_insert(struct IdentMap *map, char *ident);

/**
 * Get the address of a ident
 *
 * @note Returns -1 on fail
 *
 * @note If there is not enough space, space is allocated
 *
 * @param map
 * @param char *ident
 *
 * @returns int32_t
 */
int32_t ident_map_get(struct IdentMap *map, char *ident);

/**
 * Free a IdentMap
 *
 * @param map
 */
void ident_map_destroy(struct IdentMap *map);

/**
 * Print all instructions in a TokenVec
 *
 * @param map
 */
void ident_map_print(struct IdentMap *map);
