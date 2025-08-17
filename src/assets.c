#include "assets.h"
#include <stdio.h>
#include <stdlib.h>

uint8_t* assetLoad(const char* path, uint32_t* size)
{
	FILE *file = fopen(path, "rb");
	if (!file) {
		fprintf(stderr, "Error opening asset '%s'\n", path);
		if (size) {
			*size = 0;
		}
		return 0;
	}
	fseek(file, 0, SEEK_END);
	uint32_t fileSize = ftell(file);
	fseek(file, 0, SEEK_SET);

	uint8_t *string = malloc(fileSize + 1);
	fread(string, fileSize, 1, file);
	fclose(file);

	string[fileSize] = 0;

	if (size) {
		*size = fileSize;
	}
	return string;
}

