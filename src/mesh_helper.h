#ifndef __MESH_HELPER_H__
#define __MESH_HELPER_H__

#define MESH_ATTRIB_POS 0
#define MESH_ATTRIB_COLOR 1
#define MESH_ATTRIB_UV 2

typedef struct MeshColored {
	float position[3];
	float color[4];
} MeshColored, *MeshColored_t;
typedef struct MeshTextured {
	float position[3];
	float texcoord[2];
} MeshTextured, *MeshTextured_t;

void meshApplyAttributesColored();
void meshApplyAttributesTextured();

#endif

