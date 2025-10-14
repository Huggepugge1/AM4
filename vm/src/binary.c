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

    fread(&file->idents, sizeof(uint32_t), 1, fptr);

    fread(&file->instruction_len, sizeof(uint32_t), 1, fptr);

    file->instructions = calloc(file->instruction_len, sizeof(uint32_t));
    fread(file->instructions, sizeof(uint32_t), file->instruction_len, fptr);

    fclose(fptr);

    return file;
}

void free_binary_file(struct BinaryFile *bin) {
    free(bin->instructions);
    free(bin);
}
