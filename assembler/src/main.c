#include <stdio.h>
#include <stdlib.h>

#include "arguments.h"
#include "code_generation.h"
#include "lexer.h"
#include "parser.h"

int main(int argc, char **argv) {
    struct Arguments args = arguments_parse(argc, argv);
    arguments_print(args);
    if (args.input == NULL) {
        fprintf(stderr, "am4asm needs a filename. See `--help` for more info");
        exit(1);
    }

    struct TokenVec *tokens = lex(args.input);
    token_vec_print(tokens);

    struct ParseResult result = parse(tokens);
    instruction_vec_print(result.instructions);
    label_map_print(result.labels);

    generate_code_and_write_to_file(result.instructions, args.output);

    token_vec_destroy(tokens);
    instruction_vec_destroy(result.instructions);
    label_map_destroy(result.labels);

    return 0;
}
