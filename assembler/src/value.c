#include <stdio.h>

#include "value.h"

void value_to_string(struct Value *value, char **str) {
    switch (value->kind) {
    case None:
        *str = "none";
        return;
    case Int:
        sprintf(*str, "%d", value->value.integer);
        return;
    }
}
