#ifndef __SHADER_H__
#define __SHADER_H__

#include <stdint.h>

typedef struct Shader *Shader_t;

Shader_t shaderCreateFromFile(const char *vertexShaderPath, const char *fragmentShaderPath);
Shader_t shaderCreateFromMemory(const char *vertexShaderSource, const uint32_t vertexShaderSourceSize, const char *fragmentShaderSource, const uint32_t fragmentShaderSourceSize);
void shaderDestroy(Shader_t *shader);
uint32_t shaderGetProgram(Shader_t shader);
void shaderUse(Shader_t shader);

#endif

