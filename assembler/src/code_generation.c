#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "code_generation.h"
#include "parser.h"

// Only header
#define MINIMUM_BINARY_SIZE 2

struct Binary {
    uint32_t *bin;
    size_t capacity;
    size_t len;
};

void write_to_binary(struct Binary *binary, uint32_t value) {
    if (binary->len >= binary->capacity) {
        size_t new_capacity = binary->capacity * 2;
        binary->bin = realloc(binary->bin, new_capacity * sizeof(uint32_t));
        if (binary->bin == NULL) {
            fprintf(stderr, "Failed to reallocate the binary buffer\n");
            exit(1);
        }
        binary->capacity *= 2;
    }
    binary->bin[binary->len++] = value;
}

void create_header(struct Binary *binary, struct InstructionVec *instructions,
                   struct IdentMap *idents) {
    // Start of the text section
    write_to_binary(binary, idents->len);
    // Total size of the binary
    write_to_binary(binary, instructions->len + idents->len);
}

/**
 * Allocates a single integer to every variable
 * @note The data section lives **BEFORE** the text section
 */
void setup_data_section(struct Binary *binary, struct IdentMap *idents) {
    for (size_t i = 0; i < idents->len; i++) {
        write_to_binary(binary, 0);
    }
}

/**
 * Labels and identifiers must be converted to addresses
 */
int32_t get_value_as_int(struct Instruction *instruction,
                         struct LabelMap *labels, struct IdentMap *idents) {
    switch (instruction->value.kind) {
    case None:
        return 0;
    case IntValue:
        return instruction->value.value.integer;
    case BoolValue:
        // We say that false = 0, true = 1
        return instruction->value.value.boolean;
    case StringValue:
        if (instruction->kind == InstructionFetch ||
            instruction->kind == InstructionStore ||
            instruction->kind == InstructionPrintV) {
            int32_t value =
                ident_map_get(idents, instruction->value.value.string);
            if (value == -1) {
                fprintf(stderr, "%s is not a valid identifier!\n",
                        instruction->value.value.string);
                exit(1);
            }
            // The data section lives first, so no need to offset by anything
            return value;

        } else {
            // The only strings are labels && identifiers
            int32_t value =
                label_map_get(labels, instruction->value.value.string);
            if (value == -1) {
                fprintf(stderr, "%s is not a valid label!\n",
                        instruction->value.value.string);
                exit(1);
            }
            // The data section lives first, so the text section is offset
            // by the number of unique identifiers (variables)
            return value + idents->len;
        }
        break;
    default:
        __builtin_unreachable();
    }
}

void generate_text_section(struct Binary *binary,
                           struct InstructionVec *instructions,
                           struct LabelMap *labels, struct IdentMap *idents) {
    for (size_t i = 0; i < instructions->len; i++) {
        struct Instruction instruction = instructions->elements[i];

        // The opcode occupies the 8 most significant bits
        int32_t opcode = instruction.kind << 24;

        int32_t value = get_value_as_int(&instruction, labels, idents);
        // Only the least significant 24 bits are reserved for data
        value = value & 0xFFFFFF;

        write_to_binary(binary, opcode | value);
    }
}

struct Binary binary_new() {
    struct Binary binary = {
        .bin = calloc(MINIMUM_BINARY_SIZE, sizeof(uint32_t)),
        .capacity = MINIMUM_BINARY_SIZE,
        .len = 0,
    };
    if (binary.bin == NULL) {
        fprintf(stderr, "Failed to do a heap allocation\n");
        exit(1);
    }
    return binary;
}

struct Binary generate_binary(struct ParseResult result) {
    struct InstructionVec *instructions = result.instructions;
    struct LabelMap *labels = result.labels;
    struct IdentMap *idents = result.idents;
    struct Binary binary = binary_new();

    create_header(&binary, instructions, idents);
    setup_data_section(&binary, idents);

    generate_text_section(&binary, instructions, labels, idents);
    return binary;
}

void generate_binary_and_write_to_file(struct ParseResult result,
                                       char *output_file) {
    struct Binary binary = generate_binary(result);
    FILE *output = fopen(output_file, "wb");
    fwrite(binary.bin, sizeof(uint32_t), binary.len, output);
    fclose(output);
    free(binary.bin);
}
