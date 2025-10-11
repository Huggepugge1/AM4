#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

#include "arguments.h"

int main(int argc, char** argv) {
	struct Arguments args = arguments_parse(argc, argv);
	arguments_print(args);
	if (args.input == NULL) {
        fprintf(stderr, "am4vm needs a filename. See `--help` for more info");
        exit(1);
    }

    char file_contents[65535];
    size_t line = 0;
    while (fgets(file_contents, 65535, fptr)) {
        printf("%s\n", file_contents);
        line++;
    }

    fclose(fptr);
}

read_file(char* filename) {
	


	
}
