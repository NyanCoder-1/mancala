#ifndef __TEXTURE_H__
#define __TEXTURE_H__

#include <stdint.h>

typedef struct Texture *Texture_t;

Texture_t textureCreateFromPngFile(const char *pngPath);
Texture_t textureCreateFromPngInMemory(const char *pngData, const uint32_t pngDataSize);
Texture_t textureCreateFromRaw(const void* mem, const uint32_t width, const uint32_t height, const uint8_t colors);
void textureDestroy(Texture_t *texture);
void textureUse(Texture_t texture);

#endif

