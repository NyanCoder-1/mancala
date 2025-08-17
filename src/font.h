#ifndef __FONT_H__
#define __FONT_H__

#include <stdint.h>

typedef struct Font *Font_t;
typedef struct FontCharacterMetrics {
	// uv
	float uvTop;
	float uvBottom;
	float uvLeft;
	float uvRight;

	// dimensions
	float advance;
	float bearingX;
	float bearingY;
	float width;
	float height;
} FontCharacterMetrics;

Font_t fontCreate(const void* ft2FontFace, double fontSize);
void fontDestroy(Font_t* const font);
void fontBlockInit(const Font_t font, const uint32_t blockId);
const uint8_t fontBlockUseTexture(const Font_t font, const uint32_t blockId);
const FontCharacterMetrics fontGetCharacterMetrics(const Font_t font, const uint32_t character, uint32_t* const blockId);
const void* fontGetFace(const Font_t font);

#endif

