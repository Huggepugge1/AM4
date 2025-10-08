#pragma once
#include "parser.h"

/**
 * Generate am4 binary / machine code and write it to a file
 *
 * @param instructions InstructionVec of all instructions
 * @param output_file File name of the output target
 */
void generate_code_and_write_to_file(struct InstructionVec *instructions,
                                     char *output_file);
