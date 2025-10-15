#pragma once
#include <stdint.h>

struct BinaryFile {
    uint32_t start_addr;
    uint32_t total_size;
    uint32_t *memory;
};

struct BinaryFile *read_binary_file(char *filename);

void free_binary_file(struct BinaryFile *bin);
