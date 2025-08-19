#ifndef __SHADER_H__
#define __SHADER_H__

#include <stdint.h>

typedef struct Shader *Shader_t;
#define SHADER_UNIFORM_LOCATION_ID_MATRIX_MODEL 0
#define SHADER_UNIFORM_LOCATION_ID_MATRIX_VIEWPROJECTION 1

Shader_t shaderCreateFromFile(const char *vertexShaderPath, const char *fragmentShaderPath);
Shader_t shaderCreateFromMemory(const char *vertexShaderSource, const uint32_t vertexShaderSourceSize, const char *fragmentShaderSource, const uint32_t fragmentShaderSourceSize);
void shaderDestroy(Shader_t *shader);
uint32_t shaderGetProgram(const Shader_t shader);
int32_t shaderGetUniformLocation(const Shader_t shader, uint32_t locationId);
int32_t shaderLoadUniformLocation(const Shader_t shader, const char* const locationName);
void shaderUse(const Shader_t shader);

#endif

