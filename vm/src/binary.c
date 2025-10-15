#include "binary.h"
#include <stdio.h>
#include <stdlib.h>

struct BinaryFile *read_binary_file(char *filename) {
    FILE *fptr;

    fptr = fopen(filename, "r");
    if (!fptr) {
        perror("Error opening file");
        exit(1);
    }

    struct BinaryFile *file = calloc(1, sizeof(struct BinaryFile));

    fread(&file->start_addr, sizeof(uint32_t), 1, fptr);

    fread(&file->total_size, sizeof(uint32_t), 1, fptr);

    file->memory = calloc(file->total_size, sizeof(uint32_t));
    fread(file->memory, sizeof(uint32_t), file->total_size, fptr);

    fclose(fptr);

    return file;
}

void free_binary_file(struct BinaryFile *bin) {
    free(bin->memory);
    free(bin);
}
