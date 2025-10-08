#pragma once

#include <stdint.h>

struct Value {
    enum Kind { None, IntValue } kind;
    union value {
        int32_t integer;
    } value;
};

/**
 * Get the string value of a Value
 *
 * @note Does not allocate anything, instead, just modifies the pointer
 * in place
 *
 * @param Value* value
 * @param char** str A pointer to a string you want to point to the value
 */
void value_to_string(struct Value *value, char **str);
