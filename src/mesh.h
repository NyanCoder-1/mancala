#ifndef __MESH_H__
#define __MESH_H__

#include "shader.h"
#include <stdint.h>

typedef struct Mesh *Mesh_t;

Mesh_t meshCreate(Shader_t shader, void (*applyAttributesFn)(Shader_t), const void *vertexData, const uint32_t vertexDataSize, const uint16_t *indicesData, const uint32_t indicesDataSize);
void meshDestroy(Mesh_t *mesh);
void meshUpdateVertices(Mesh_t mesh, const void *vertexData, const uint32_t vertexDataSize);
void meshDraw(Mesh_t mesh);

#endif

