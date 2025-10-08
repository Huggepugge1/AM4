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

struct Code generate_code(struct InstructionVec *instructions) {
    struct Code code = {
        .bin = calloc(65535, sizeof(uint32_t)),
        .len = 0,
    };
    for (size_t i = 0; i < instructions->len; i++) {
        struct Instruction instruction = instructions->elements[i];

        // This is actually fine - Source trust me bro
        int32_t value = instruction.value.value.integer;
        code.bin[code.len] = instruction.kind << 24 | value;
        code.len++;
    }
    return code;
}

void generate_code_and_write_to_file(struct InstructionVec *instructions,
                                     char *output_file) {
    struct Code code = generate_code(instructions);
    FILE *output = fopen(output_file, "w");
    fwrite(code.bin, sizeof(uint32_t), code.len, output);
    fclose(output);
    free(code.bin);
}
