#include "FT2.h"
#include "assets.h"
#include "freetype/ftmodapi.h"
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

struct FT2 {
	FT_Library ftLibrary;
	uint8_t *fontFile;
	FT_Face ftFace;
};

FT2_t ft2Create() {
	FT2_t ft2 = (FT2_t)malloc(sizeof(struct FT2));
	memset(ft2, 0, sizeof(struct FT2));

	// freetype
	FT_Error error = FT_Init_FreeType(&ft2->ftLibrary);
	if (error) {
		fprintf(stderr, "An error occurred during freetype initialization\n");
		ft2Destroy(&ft2);
		return 0;
	}
	// load default font face
	uint32_t fontFileSize = 0;
	ft2->fontFile = assetLoad("fonts/LiberationSans-Regular.ttf", &fontFileSize);
	if (!fontFileSize) {
		ft2Destroy(&ft2);
		return 0;
	}
	error = FT_New_Memory_Face(ft2->ftLibrary, ft2->fontFile, fontFileSize, 0, &ft2->ftFace);
	if (error) {
		fprintf(stderr, "An error occured during font loading, it's either unsupported or corruped\n");
		ft2Destroy(&ft2);
		return 0;
	}

	return ft2;
}
void ft2Destroy(FT2_t *ft2) {
	if (ft2 && *ft2) {
		if ((*ft2)->ftFace) {
			FT_Done_Face((*ft2)->ftFace);
		}
		if ((*ft2)->fontFile) {
			free((*ft2)->fontFile);
		}
		if ((*ft2)->ftLibrary) {
			FT_Done_Library((*ft2)->ftLibrary);
		}
		free(*ft2);
		*ft2 = 0;
	}
}
FT_Library ft2GetLibrary(FT2_t ft2) {
	if (ft2) {
		return ft2->ftLibrary;
	}
	return 0;
}
FT_Face ft2GetDefaultFace(FT2_t ft2) {
	if (ft2) {
		return ft2->ftFace;
	}
	return 0;
}

