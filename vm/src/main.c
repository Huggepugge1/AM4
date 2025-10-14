#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

#include "arguments.h"
#include "binary.h"
#include "vm.h"

int main(int argc, char **argv) {
    struct Arguments args = arguments_parse(argc, argv);
    arguments_print(args);
    if (args.input == NULL) {
        fprintf(stderr, "am4vm needs a filename. See `--help` for more info");
        exit(1);
    }

    struct BinaryFile *bin = read_binary_file(args.input);

    run_vm(bin);

    free_binary_file(bin);
}
