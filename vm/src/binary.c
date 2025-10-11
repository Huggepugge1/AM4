#include "binary.h"
#include <stdlib.h>
#include <stdio.h>

int32_t read_binary_file(char *filename) {
	FILE *fptr;

	fptr = fopen(filename, "r");
	if (!fptr) {
    	perror("Error opening file");
    	exit(1);
	}

	
}
