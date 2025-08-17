#include "text.h"
#include "font.h"
#include "mesh.h"
#include "mesh_helper.h"
#include "utils/dynamic_array.h"
#include "utils/unicode_blocks.h"
#include <ft2build.h>
#include FT_FREETYPE_H
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

struct TextByBlocks {
	Mesh_t mesh;
	uint32_t block;
};
struct Text {
	Font_t font;
	DynamicArray_t blocks;
	Shader_t shader;
};

static uint32_t textUtf8Step(const char* const utf8Text, uint8_t* shiftAmount);

Text_t textCreate(const Font_t font, const Shader_t shader) {
	Text_t txt = (Text_t)malloc(sizeof(struct Text));
	txt->font = font;
	txt->blocks = daCreate(sizeof(struct TextByBlocks));
	txt->shader = shader;
	return txt;
}
void textDestroy(Text_t* txt) {
	if (txt && *txt) {
		free(*txt);
		*txt = 0;
	}
}
void textDraw(const Text_t txt) {
	if (txt) {
		for (int n = 0; n < daSize(txt->blocks); n++) {
			const struct TextByBlocks *const block = (const struct TextByBlocks *const)daGet(txt->blocks, n);
			if (!block) {
				continue;
			}
			fontBlockUseTexture(txt->font, block->block);
			meshDraw(block->mesh);
		}
	}
}
void textSetText(Text_t txt, const char* const utf8text) {
	if (txt) {
		for (int n = 0; n < daSize(txt->blocks); n++) {
			meshDestroy(&((struct TextByBlocks*)daGet(txt->blocks, n))->mesh);
		}
		daClear(txt->blocks);
		if (utf8text) {
			const FT_Face face = (FT_Face)fontGetFace(txt->font);
			float penX = 0.0f;
			float penY = face->size->metrics.ascender >> 6;
			const float deltaBaseline = face->size->metrics.height >> 6;
			uint8_t shift = 0;
			const char* ptr = utf8text;
			struct {
				DynamicArray_t vertices;
				DynamicArray_t indices;
				uint32_t characters;
				uint8_t used;
			} blocks[unicodeBlocksCount];
			memset(blocks, 0, sizeof(blocks));
			DynamicArray_t blockMeshIds = daCreate(sizeof(uint32_t));
			while (*ptr) {
				const uint32_t character = textUtf8Step(ptr, &shift);
				if (!shift) {
					break;
				}
				ptr += shift;
				// do newline
				if (character == '\n') {
					penX = 0.0f;
					penY += deltaBaseline;
					continue;
				}
				// get metrics
				uint32_t blockIndex = 0;
				const FontCharacterMetrics metrics = fontGetCharacterMetrics(txt->font, character, &blockIndex);
				const float prevPenX = penX;
				penX += metrics.advance;
				// do space
				if (character == 0x20) {
					continue;
				}
				// create new block if it's still doesn't exists
				if (!blocks[blockIndex].used) {
					daPushBack(blockMeshIds, &blockIndex);
					blocks[blockIndex].vertices = daCreate(sizeof(MeshTextured));
					blocks[blockIndex].characters = 0;
					blocks[blockIndex].used = 1;
				}
				// add geometry
				const float x0 = prevPenX + metrics.bearingX;
				const float y0 = penY - metrics.bearingY;
				const float x1 = x0 + metrics.width;
				const float y1 = y0 + metrics.height;
				MeshTextured quad[] = {
					(MeshTextured){.position = {x0, y0, 0.0f}, .texcoord = {metrics.uvLeft, metrics.uvTop}},
					(MeshTextured){.position = {x1, y0, 0.0f}, .texcoord = {metrics.uvRight, metrics.uvTop}},
					(MeshTextured){.position = {x0, y1, 0.0f}, .texcoord = {metrics.uvLeft, metrics.uvBottom}},
					(MeshTextured){.position = {x1, y1, 0.0f}, .texcoord = {metrics.uvRight, metrics.uvBottom}},
				};
				daPushBackArray(blocks[blockIndex].vertices, quad, 4);
				blocks[blockIndex].characters++;
			}
			const uint32_t blockMeshIdsCount = daSize(blockMeshIds);
			daResize(txt->blocks, blockMeshIdsCount);
			for (int n = 0; n < blockMeshIdsCount; n++) {
				const uint32_t blockIndex = *(uint32_t*)daGet(blockMeshIds, n);
				blocks[blockIndex].indices = daCreate(sizeof(uint16_t) * 6);
				daResize(blocks[blockIndex].indices, blocks[blockIndex].characters);
				for (uint32_t ch = 0; ch < blocks[blockIndex].characters; ch++) {
					uint16_t indices[] = {
						ch * 4 + 0, ch * 4 + 1, ch * 4 + 2,
						ch * 4 + 2, ch * 4 + 1, ch * 4 + 3
					};
					memcpy(daGet(blocks[blockIndex].indices, ch), indices, sizeof(indices));
				}
				struct TextByBlocks *block = (struct TextByBlocks*)daGet(txt->blocks, n);
				block->mesh = meshCreate(meshApplyAttributesTextured, (uint32_t*)daData(blocks[blockIndex].vertices), daSize(blocks[blockIndex].vertices) * sizeof(MeshTextured), (uint16_t*)daData(blocks[blockIndex].indices), daSize(blocks[blockIndex].indices) * sizeof(uint16_t) * 6);
				block->block = blockIndex;
				daDestroy(&blocks[blockIndex].vertices);
				daDestroy(&blocks[blockIndex].indices);
			}
			daDestroy(&blockMeshIds);
		}
	}
}

static uint32_t textUtf8Step(const char* const utf8Text, uint8_t* shiftAmount) {
	if (!*utf8Text) {
		if (shiftAmount) {
			*shiftAmount = 0;
		}
		return 0;
	}
	if (shiftAmount) {
		*shiftAmount = 1;
	}
#define UNKNOWN return 0
#define U32(ch) ((uint32_t)(uint8_t)ch)
	// 1 byte (U+0000 ~ U+007F)
	if (!(0x80 & *utf8Text)) {
		return *utf8Text & 0x7F;
	}
	// error
	if (!(0x40 & *utf8Text)) {
		UNKNOWN;
	}
	// 2 bytes (U+0080 ~ U+07FF)
	if (!(0x20 & utf8Text[0])) {
		// validate
		if (!utf8Text[1] || (((uint8_t)utf8Text[1] & 0xC0) != 0x80)) {
			UNKNOWN;
		}
		if (shiftAmount) {
			*shiftAmount = 2;
		}
		return ((U32(utf8Text[0]) & 0x1F) << 6) | (U32(utf8Text[1]) & 0x3F);
	}
	// 3 bytes (U+0800 ~ U+FFFF)
	if (!(0x10 & utf8Text[0])) {
		// validate
		if (!utf8Text[1] || (((uint8_t)utf8Text[1] & 0xC0) != 0x80) || !utf8Text[2] || (((uint8_t)utf8Text[2] & 0xC0) != 0x80)) {
			UNKNOWN;
		}
		if (shiftAmount) {
			*shiftAmount = 3;
		}
		return ((U32(utf8Text[0]) & 0x0F) << 12) | ((U32(utf8Text[1]) & 0x3F) << 6) | (U32(utf8Text[2]) & 0x3F);
	}
	// 4 bytes (U+10000 ~ U+10FFFF)
	if (!(0x08 & utf8Text[0])) {
		// validate
		if (!utf8Text[1] || (((uint8_t)utf8Text[1] & 0xF0) != 0x80) || !utf8Text[2] || (((uint8_t)utf8Text[2] & 0xC0) != 0x80) || !utf8Text[3] || (((uint8_t)utf8Text[3] & 0xC0) != 0x80)) {
			UNKNOWN;
		}
		if (shiftAmount) {
			*shiftAmount = 4;
		}
		return ((U32(utf8Text[0]) & 0x07) << 18) | ((U32(utf8Text[1]) & 0x3F) << 12) | ((U32(utf8Text[2]) & 0x3F) << 6) | (U32(utf8Text[3]) & 0x3F);
	}
	UNKNOWN;
#undef UNKNOWN
#undef U32
}

