#include "common_shaders.h"
#include "shader.h"
#include <stdlib.h>
#include <string.h>

#define COMMON_SHADERS_AMOUNT 3
static const char* const vertexShader[COMMON_SHADERS_AMOUNT] = {
	"shaders/text-vs.glsl",
	"shaders/textured-vs.glsl",
	"shaders/simple-vs.glsl",
};
static const char* const fragmentShader[COMMON_SHADERS_AMOUNT] = {
	"shaders/text-fs.glsl",
	"shaders/textured-fs.glsl",
	"shaders/simple-fs.glsl",
};

struct CommonShaders {
	Shader_t shaders[COMMON_SHADERS_AMOUNT];
};

CommonShaders_t commonShadersCreate() {
	CommonShaders_t commonShaders = (CommonShaders_t)malloc(sizeof(struct CommonShaders));
	memset(commonShaders, 0, sizeof(struct CommonShaders));
	for (int i = 0; i < COMMON_SHADERS_AMOUNT; i++) {
		commonShaders->shaders[i] = shaderCreateFromFile(vertexShader[i], fragmentShader[i]);
	}
	return commonShaders;
}
void commonShadersDestroy(CommonShaders_t* commonShadersRef) {
	if (commonShadersRef && *commonShadersRef) {
		for (int i = 0; i < COMMON_SHADERS_AMOUNT; i++) {
			shaderDestroy(&(*commonShadersRef)->shaders[i]);
		}
		free(*commonShadersRef);
		*commonShadersRef = 0;
	}
}
const Shader_t commonShadersGet(const CommonShaders_t commonShaders, uint32_t shaderId) {
	if (commonShaders && shaderId < COMMON_SHADERS_AMOUNT)
		return commonShaders->shaders[shaderId];
	return 0;
}

