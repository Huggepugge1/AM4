#include "arguments.h"
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void print_help();

bool str_starts_with(char *str, char *pattern) {
    while (*pattern && *str && *str == *pattern) {
        str++;
        pattern++;
    }

    return !*pattern;
}

struct Arguments arguments_parse(int argc, char **argv) {
    // Skip the run command
    struct Arguments args = {.input = NULL, .output = NULL};

    for (int i = 1; i < argc; i++) {
        if (str_starts_with(argv[i], "--")) {
            if (strcmp(argv[i], "--help") == 0) {
                print_help();
            } else if (strcmp(argv[i], "--out") == 0) {
                i++;
                if (i < argc) {
                    args.output = argv[i];
                } else {
                    fprintf(stderr,
                            "`--out` needs an argument, see `--help` for more "
                            "info\n");
                    exit(1);
                }
            } else {
                fprintf(stderr,
                        "`%s` is not a valid argument, see `--help` for more "
                        "info\n",
                        argv[i]);
                exit(1);
            }
        } else {
            if (args.input != NULL) {
                fprintf(stderr, "You can only assemble one file at a time");
                exit(1);
            } else {
                args.input = argv[i];
            }
        }
    }
    if (args.input == NULL) {
        fprintf(stderr, "No input files, see `--help` for more info\n");
        exit(1);
    } else if (args.output == NULL) {
        args.output = "out.bin";
    }
    return args;
}

void arguments_print(struct Arguments args) {
    printf("struct Arguments {\n");
    printf("  .input = \"%s\",\n", args.input);
    printf("  .output = \"%s\",\n", args.output);
    printf("}\n");
}

void print_help() {
    printf("Assemble an am4 assembly file\n");
    printf("\n");
    printf("Usage: am4asm [OPTIONS] <FILENAME>\n");
    printf("\n");
    printf("Arguments\n");
    printf("    --help  -- Print this message\n");
    exit(0);
}
