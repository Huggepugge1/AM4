#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "code_generation.h"
#include "parser.h"

struct Code {
    uint32_t *bin;
    size_t len;
};

struct Code generate_code(struct InstructionVec *instructions,
                          struct LabelMap *map) {
    struct Code code = {
        .bin = calloc(65535, sizeof(uint32_t)),
        .len = 0,
    };
    for (size_t i = 0; i < instructions->len; i++) {
        struct Instruction instruction = instructions->elements[i];

        int32_t value;
        switch (instruction.value.kind) {
        case None:
            value = 0;
            break;
        case IntValue:
            value = instruction.value.value.integer;
            break;
        case BoolValue:
            value = instruction.value.value.boolean;
            break;
            // The only strings are labels
        case StringValue:
            value = label_map_get(map, instruction.value.value.string);
            if (value == -1) {
                printf("%s is not a valid label!",
                       instruction.value.value.string);
                exit(1);
            }
            break;
        }
        value = value & 0xFFFFFF;
        code.bin[code.len] = instruction.kind << 24 | value;
        code.len++;
    }
    return code;
}

void generate_code_and_write_to_file(struct InstructionVec *instructions,
                                     struct LabelMap *map, char *output_file) {
    struct Code code = generate_code(instructions, map);
    FILE *output = fopen(output_file, "w");
    fwrite(code.bin, sizeof(uint32_t), code.len, output);
    fclose(output);
    free(code.bin);
}
