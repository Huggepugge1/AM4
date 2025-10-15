#include <stdio.h>
#include <stdlib.h>

#include "arguments.h"
#include "code_generation.h"
#include "lexer.h"
#include "parser.h"

int main(int argc, char **argv) {
    struct Arguments args = arguments_parse(argc, argv);
    if (args.input == NULL) {
        fprintf(stderr, "am4asm needs a filename. See `--help` for more info");
        exit(1);
    }

    struct TokenVec *tokens = lex(args.input);
    struct ParseResult parlse_result = parse(tokens);
    generate_binary_and_write_to_file(parlse_result, args.output);

    token_vec_destroy(tokens);
    instruction_vec_destroy(parlse_result.instructions);
    label_map_destroy(parlse_result.labels);
    ident_map_destroy(parlse_result.idents);

    return 0;
}
