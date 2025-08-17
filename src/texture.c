#include "texture.h"
#include "assets.h"
#include <GLES3/gl3.h>
#include <stb_image.h>

struct Texture {
	GLuint texture;
	uint32_t width;
	uint32_t height;
};

Texture_t textureCreateFromPngFile(const char *pngPath)
{
	uint32_t pngDataSize = 0;
	char* pngData = (char*)assetLoad(pngPath, &pngDataSize);
	Texture_t texture = textureCreateFromPngInMemory(pngData, pngDataSize);
	free(pngData);
	return texture;
}

Texture_t textureCreateFromPngInMemory(const char *pngData, const uint32_t pngDataSize)
{
	Texture_t texture = (Texture_t)malloc(sizeof(struct Texture));
	texture->texture = 0;
	glGenTextures(1, &texture->texture);
	glBindTexture(GL_TEXTURE_2D, texture->texture);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	int width, height, channels;
	stbi_uc *imageBuffer = stbi_load_from_memory((const stbi_uc*)pngData, pngDataSize, &width, &height, &channels, 4);
	texture->width = width;
	texture->height = height;
	if (imageBuffer != NULL) {
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, imageBuffer);
	}
	else {
		glDeleteTextures(1, &texture->texture);
		free(texture);
		return 0;
	}
	stbi_image_free(imageBuffer);

	return texture;
}

Texture_t textureCreateFromRaw(const void* mem, const uint32_t width, const uint32_t height, const uint8_t colors) {
	if (!mem || !width || !height) {
		return 0;
	}
	Texture_t texture = (Texture_t)malloc(sizeof(struct Texture));
	texture->texture = 0;
	glGenTextures(1, &texture->texture);
	glBindTexture(GL_TEXTURE_2D, texture->texture);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	texture->width = width;
	texture->height = height;
	GLenum format = (colors == 4) ? GL_RGBA : ((colors == 3) ? GL_RGB : ((colors == 2) ? GL_RG : GL_RED));
	GLint internalFormat = (colors == 4) ? GL_RGBA8 : ((colors == 3) ? GL_RGB8 : ((colors == 2) ? GL_RG8 : GL_R8));
	glTexImage2D(GL_TEXTURE_2D, 0, internalFormat, width, height, 0, format, GL_UNSIGNED_BYTE, mem);

	return texture;
}

void textureDestroy(Texture_t *texture)
{
	if (texture && *texture) {
		if ((*texture)->texture)
			glDeleteTextures(1, &(*texture)->texture);
		(*texture)->texture = 0;
		(*texture)->width = 0;
		(*texture)->height = 0;
		free(*texture);
		*texture = NULL;
	}
}

void textureUse(Texture_t texture)
{
	if (texture == NULL)
		return;
	glBindTexture(GL_TEXTURE_2D, texture->texture);
}

