#include "font.h"
#include "texture.h"
#include "utils/unicode_blocks.h"
#include <ft2build.h>
#include FT_FREETYPE_H
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct FontCharacterInfo {
	float uvTop;
	float uvBottom;
	float uvLeft;
	float uvRight;
	float width;
	float height;
	float advance;
	float bearingX;
	float bearingY;
};
struct FontBlock {
	Texture_t texture;
	struct FontCharacterInfo *charactersInfo;
	uint8_t haveMetrics;
	uint8_t ready;
	uint8_t empty;
};
struct Font {
	FT_Face face;
	struct FontBlock *blocks;
	double fontSize;
};

static void fontPrepareBlockMetrics(const FT_Face face, struct FontBlock* const fontBlock, const uint32_t blockIndex);
static void fontPrepareBlock(const FT_Face face, struct FontBlock* const fontBlock, const uint32_t blockIndex);

Font_t fontCreate(const void* ft2FontFace, double fontSize) {
	if (!ft2FontFace) {
		fprintf(stderr, "No font was passed\n");
		return 0;
	}
	Font_t font = (Font_t)malloc(sizeof(struct Font));
	if (font) {
		font->fontSize = fontSize;
		font->face = (FT_Face)ft2FontFace;
		font->blocks = (struct FontBlock*)calloc(unicodeBlocksCount, sizeof(struct FontBlock));
		if (!font->blocks) {
			free(font);
			return 0;
		}
		// preinit few blocks (Latin and Cyrillic)
		static const uint32_t preinitBlocks[] = {0x00, 0x01, 0x02, 0x03, 0x08, 0x09, 0x3f, 0x66, 0x7e, 0xa4};
		static const uint32_t preinitBlocksCount = sizeof(preinitBlocks) / sizeof(uint32_t);
		for (int i = 0; i < preinitBlocksCount; i++) {
			const uint32_t blockIndex = preinitBlocks[i];
			fontBlockInit(font, blockIndex);
		}
	}
	return font;
}
void fontDestroy(Font_t* const font) {
	if (font && *font) {
		free(*font);
		*font = 0;
	}
}
void fontBlockInit(const Font_t font, const uint32_t blockId) {
	if (font && !font->blocks[blockId].ready) {
		// set font size
		uint32_t sz64th = (uint32_t)(font->fontSize * 64);
		FT_Set_Char_Size(font->face, 0, sz64th, 0, 0);
		//FT_Set_Pixel_Sizes(font->face, 0, (FT_UInt)font->fontSize);
		//printf("%f\n", font->fontSize);

		const uint32_t blockBegin = unicodeBlocks[blockId];
		const uint32_t blockEnd = unicodeBlocks[blockId + 1];

		fontPrepareBlockMetrics(font->face, &font->blocks[blockId], blockId);
		fontPrepareBlock(font->face, &font->blocks[blockId], blockId);
	}
}
const uint8_t fontBlockUseTexture(const Font_t font, const uint32_t blockId) {
	if (font && font->blocks && font->blocks[blockId].ready && font->blocks[blockId].texture) {
		textureUse(font->blocks[blockId].texture);
		return 1;
	}
	return 0;
}
const FontCharacterMetrics fontGetCharacterMetrics(const Font_t font, const uint32_t character, uint32_t* const blockId) {
	if (font && font->blocks) {
		uint8_t ignored = 0;
		uint32_t blockIndex = unicodeGetBlockIndex(character, &ignored);
		if (blockId) {
			*blockId = blockIndex;
		}
		if (!ignored) {
			fontBlockInit(font, blockIndex);
			if (font->blocks[blockIndex].ready && !font->blocks[blockIndex].empty && font->blocks[blockIndex].charactersInfo) {
				const uint32_t blockBegin = unicodeBlocks[blockIndex];
				const uint32_t characterIndexWithinBlock = character - blockBegin;
				return (FontCharacterMetrics){
					.uvTop = font->blocks[blockIndex].charactersInfo[characterIndexWithinBlock].uvTop,
					.uvBottom = font->blocks[blockIndex].charactersInfo[characterIndexWithinBlock].uvBottom,
					.uvLeft = font->blocks[blockIndex].charactersInfo[characterIndexWithinBlock].uvLeft,
					.uvRight = font->blocks[blockIndex].charactersInfo[characterIndexWithinBlock].uvRight,
					.advance = font->blocks[blockIndex].charactersInfo[characterIndexWithinBlock].advance,
					.bearingX = font->blocks[blockIndex].charactersInfo[characterIndexWithinBlock].bearingX,
					.bearingY = font->blocks[blockIndex].charactersInfo[characterIndexWithinBlock].bearingY,
					.width = font->blocks[blockIndex].charactersInfo[characterIndexWithinBlock].width,
					.height = font->blocks[blockIndex].charactersInfo[characterIndexWithinBlock].height,
				};
			}
		}
	}
	return (FontCharacterMetrics){0, 0, 0, 0, 0, 0, 0, 0, 0};
}
const void* fontGetFace(const Font_t font) {
	if (font) {
		return font->face;
	}
	return 0;
}
static void fontPrepareBlockMetrics(const FT_Face face, struct FontBlock* const fontBlock, const uint32_t blockIndex) {
	if (!fontBlock) {
		return;
	}
	const uint32_t blockBegin = unicodeBlocks[blockIndex];
	const uint32_t blockEnd = unicodeBlocks[blockIndex + 1];
	const uint32_t blockSize = blockEnd - blockBegin;
	if (!fontBlock->charactersInfo) {
		fontBlock->charactersInfo = (struct FontCharacterInfo*)calloc(blockSize, sizeof(struct FontCharacterInfo));
	}
	uint8_t empty = 1;
	for (uint32_t i = 0; i < blockSize; i++) {
		const uint32_t character = blockBegin + i;
		const FT_UInt glyphIndex = FT_Get_Char_Index(face, character);
		if (!glyphIndex && character) {
			continue;
		}
		const FT_Error error = FT_Load_Glyph(face, glyphIndex, FT_LOAD_COMPUTE_METRICS);
		const FT_GlyphSlot slot = face->glyph;
		if (!error) {
			// fill character metrics
			fontBlock->charactersInfo[i].advance = slot->metrics.horiAdvance >> 6;
			fontBlock->charactersInfo[i].bearingX = slot->metrics.horiBearingX >> 6;
			fontBlock->charactersInfo[i].bearingY = slot->metrics.horiBearingY >> 6;
			empty = 0;
		}
		else {
			//FT_Err_Invalid_Reference;
			fprintf(stderr, "Error %d: Block %d character 0x%04X (glyph index 0x%04X) can't load glyph for compute metrics\n", error, blockIndex, character, glyphIndex);
		}
	}
	fontBlock->empty = empty;
	fontBlock->haveMetrics = 1;
}
struct Bitmap {
	uint8_t* bitmap;
	uint32_t width;
	uint32_t height;
};
static void fontPrepareBlock(const FT_Face face, struct FontBlock* const fontBlock, const uint32_t blockIndex) {
	if (!fontBlock || !fontBlock->haveMetrics) {
		return;
	}
	const uint32_t blockBegin = unicodeBlocks[blockIndex];
	const uint32_t blockEnd = unicodeBlocks[blockIndex + 1];
	const uint32_t blockSize = blockEnd - blockBegin;

	// prepare local FontBlock
	struct FontBlock fb;
	memset(&fb, 0, sizeof(fb));
	fb.charactersInfo = fontBlock->charactersInfo;
	fb.haveMetrics = fontBlock->haveMetrics;

	// prepare glyphs textures and block texture size
	struct Bitmap textures[blockSize];
	memset(&textures[0], 0, sizeof(textures));
	uint32_t height = 0;
	uint32_t width = 1;
	for (uint32_t i = 0; i < blockSize; i++) {
		const uint32_t character = blockBegin + i;
		const FT_UInt glyphIndex = FT_Get_Char_Index(face, character);
		if (!glyphIndex && character) {
			continue;
		}
		const FT_GlyphSlot slot = face->glyph;
		FT_Error error = FT_Load_Glyph(face, glyphIndex, FT_LOAD_RENDER);
		if (!error) {
			// copy glyph bitmap
			const uint32_t bmpWidth = slot->bitmap.width;
			const uint32_t bmpHeight = slot->bitmap.rows;
			const uint32_t size = bmpWidth * bmpHeight;
			if (!size) {
				continue;
			}
			uint8_t *const bmp = (uint8_t*)calloc(size, sizeof(uint8_t));
			if (!bmp) {
				continue;
			}
			memcpy(bmp, slot->bitmap.buffer, size);
			// fill the texture info
			textures[i].bitmap = bmp;
			textures[i].width = bmpWidth;
			textures[i].height = bmpHeight;
			// accumulate block texture metrics
			const uint32_t desiredHeight = bmpHeight + 2;
			height = desiredHeight > height ? desiredHeight : height;
			width += bmpWidth + 1;
			// fill texture size
			fb.charactersInfo[i].width = bmpWidth;
			fb.charactersInfo[i].height = bmpHeight;
		}
		else {
			fprintf(stderr, "Block %d character 0x%06X load error\n", blockIndex, character);
		}
	}

	// clear fontBlock
	textureDestroy(&fontBlock->texture);
	fontBlock->ready = 0;

	// no glyphs in block
	if (width == 1 || height == 0) {
		return;
	}

	// align texture size by 4
	if (height % 4) {
		height += 4 - height % 4;
	}
	if (width % 4) {
		width += 4 - width % 4;
	}

	// create texture and calculate UVs
	uint8_t bmp[width * height];
	memset(bmp, 0, sizeof(bmp));
	uint32_t penX = 1;
	const uint32_t penY = 1;
	for (uint32_t i = 0; i < blockSize; i++) {
		if (!textures[i].bitmap) {
			continue;
		}
		const uint32_t character = blockBegin + i;
		// copy glyph bitmap to bmp
		const uint32_t glyphWidth = textures[i].width;
		const uint32_t glyphHeight = textures[i].height;
		const uint8_t *currentTextureBmp = textures[i].bitmap;
		for (uint32_t y = 0; y < glyphHeight; y++) {
			for (uint32_t x = 0; x < glyphWidth; x++) {
				const uint32_t texturePixelIndex = y * glyphWidth + x;
				const uint32_t bmpPixelIndex = (penY + y) * width + (penX + x);
				bmp[bmpPixelIndex] = currentTextureBmp[texturePixelIndex];
			}
		}
		// calculate UVs
		fb.charactersInfo[i].uvTop = 1.0f / height;
		fb.charactersInfo[i].uvLeft = ((float)penX) / width;
		fb.charactersInfo[i].uvBottom = fb.charactersInfo[i].uvTop + ((float)glyphHeight) / height;
		fb.charactersInfo[i].uvRight = fb.charactersInfo[i].uvLeft + ((float)glyphWidth) / width;

		// move pen
		penX += glyphWidth + 1;
	}
	fb.texture = textureCreateFromRaw(bmp, width, height, 1);
	fb.ready = 1;

	// fill fontBlock
	fontBlock->texture = fb.texture;
	fontBlock->ready = 1;
}

