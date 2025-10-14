#include "value.h"

void value_to_string(struct Value *value, char **str) {
    switch (value->kind) {
    case None:
        *str = "none";
        return;
    case IntValue:
        *str = "int";
        return;
    case BoolValue:
        *str = "bool";
        return;
    case StringValue:
        *str = value->value.string;
        return;
    }
}
