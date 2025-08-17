#include "mesh_helper.h"
#include <GLES3/gl3.h>
#include <stddef.h>

#define ARRAY_SIZE(arr) (sizeof(arr) / sizeof((arr)[0]))
const GLint MESH_COLORED_POSITION_ARRAY_SIZE = (GLint)ARRAY_SIZE(((MeshColored_t)0)->position);
const void* MESH_COLORED_POSITION_OFFSET = (void*)offsetof(struct MeshColored, position);
const GLint MESH_COLORED_COLOR_ARRAY_SIZE = (GLint)ARRAY_SIZE(((MeshColored_t)0)->color);
const void* MESH_COLORED_COLOR_OFFSET = (void*)offsetof(struct MeshColored, color);
const GLint MESH_TEXTURED_POSITION_ARRAY_SIZE = (GLint)ARRAY_SIZE(((MeshTextured_t)0)->position);
const void* MESH_TEXTURED_POSITION_OFFSET = (void*)offsetof(struct MeshTextured, position);
const GLint MESH_TEXTURED_TEXCOORD_ARRAY_SIZE = (GLint)ARRAY_SIZE(((MeshTextured_t)0)->texcoord);
const void* MESH_TEXTURED_TEXCOORD_OFFSET = (void*)offsetof(struct MeshTextured, texcoord);

void meshApplyAttributesColored(Shader_t shader)
{
	GLint positionAttrib = glGetAttribLocation(shaderGetProgram(shader), "inPos");
	GLint colorAttrib = glGetAttribLocation(shaderGetProgram(shader), "inColor");
	glEnableVertexAttribArray(positionAttrib);
	glVertexAttribPointer(positionAttrib, MESH_COLORED_POSITION_ARRAY_SIZE, GL_FLOAT, GL_FALSE, sizeof(struct MeshColored), MESH_COLORED_POSITION_OFFSET);
	glEnableVertexAttribArray(colorAttrib);
	glVertexAttribPointer(colorAttrib, MESH_COLORED_COLOR_ARRAY_SIZE, GL_FLOAT, GL_FALSE, sizeof(struct MeshColored), MESH_COLORED_COLOR_OFFSET);
}

void meshApplyAttributesTextured(Shader_t shader)
{
	GLint positionAttrib = glGetAttribLocation(shaderGetProgram(shader), "inPos");
	GLint texcoordAttrib = glGetAttribLocation(shaderGetProgram(shader), "inUv");
	glEnableVertexAttribArray(positionAttrib);
	glVertexAttribPointer(positionAttrib, MESH_TEXTURED_POSITION_ARRAY_SIZE, GL_FLOAT, GL_FALSE, sizeof(struct MeshTextured), MESH_TEXTURED_POSITION_OFFSET);
	glEnableVertexAttribArray(texcoordAttrib);
	glVertexAttribPointer(texcoordAttrib, MESH_TEXTURED_TEXCOORD_ARRAY_SIZE, GL_FLOAT, GL_FALSE, sizeof(struct MeshTextured), MESH_TEXTURED_TEXCOORD_OFFSET);
}

