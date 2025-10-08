#pragma once

struct Value {
    enum Kind { None, IntValue } kind;
    union value {
        int integer;
    } value;
};

void value_to_string(struct Value *value, char **str);
