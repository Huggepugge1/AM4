#pragma once
#include <stdint.h>

struct BinaryFile {
    uint32_t instruction_start;
    uint32_t instruction_len;
    uint32_t *instructions;
};

struct BinaryFile *read_binary_file(char *filename);

void free_binary_file(struct BinaryFile *bin);
