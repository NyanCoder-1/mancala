#ifndef __COMMON_SHADERS_H__
#define __COMMON_SHADERS_H__

#include "shader.h"
#include <stdint.h>

typedef struct CommonShaders* CommonShaders_t;
#define COMMON_SHADER_TEXT 0
#define COMMON_SHADER_TEXTURE 1
#define COMMON_SHADER_COLORED 2

CommonShaders_t commonShadersCreate();
void commonShadersDestroy(CommonShaders_t* commonShaders);
const Shader_t commonShadersGet(const CommonShaders_t commonShaders, uint32_t shaderId);

#endif

