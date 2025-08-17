#ifndef __MESH_HELPER_H__
#define __MESH_HELPER_H__

#include "shader.h"

typedef struct MeshColored {
	float position[3];
	float color[4];
} MeshColored, *MeshColored_t;
typedef struct MeshTextured {
	float position[3];
	float texcoord[2];
} MeshTextured, *MeshTextured_t;

void meshApplyAttributesColored(Shader_t shader);
void meshApplyAttributesTextured(Shader_t shader);

#endif

