#pragma once
#include "parser.h"

/**
 * Generate am4 binary / machine code and write it to a file
 *
 * @param result The result of parsing the tokens
 * @param output_file File name of the output target
 */
void generate_binary_and_write_to_file(struct ParseResult result,
                                       char *output_file);
