#ifndef __FT2_H__
#define __FT2_H__

#include <ft2build.h>
#include FT_FREETYPE_H

typedef struct FT2 *FT2_t;

FT2_t ft2Create();
void ft2Destroy(FT2_t *ft2);
FT_Library ft2GetLibrary(FT2_t ft2);
FT_Face ft2GetDefaultFace(FT2_t ft2);

#endif

