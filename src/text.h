#ifndef __TEXT_H__
#define __TEXT_H__

#include "font.h"
#include "shader.h"

typedef struct Text *Text_t;

Text_t textCreate(const Font_t font, const Shader_t shader);
void textDestroy(Text_t* txt);
void textDraw(const Text_t txt);
void textSetText(Text_t txt, const char* const utf8text);

#endif

