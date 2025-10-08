#include <stdbool.h>

struct Arguments {
    char *input;
    char *output;
};

/**
 * Parse cli arguments
 *
 * @param int argc
 * @param char **argv
 *
 * @returns A struct Arguments, containing all options for "am4asm"
 */
struct Arguments arguments_parse(int argc, char **argv);

/**
 * Print cli arguments
 *
 * @param args
 */
void arguments_print(struct Arguments args);
