#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

#include "arguments.h"
#include "binary.h"
#include "vm.h"

int main(int argc, char **argv) {
    struct Arguments args = arguments_parse(argc, argv);
    if (args.input == NULL) {
        fprintf(stderr, "am4vm needs a filename. See `--help` for more info");
        exit(1);
    }

    struct BinaryFile *bin = read_binary_file(args.input);

    int32_t result = run_vm(bin);
    printf("Result: %d\n", result);

    free_binary_file(bin);
}
