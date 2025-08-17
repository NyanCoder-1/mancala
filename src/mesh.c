#include "mesh.h"
#include <GLES3/gl3.h>
#include <stdlib.h>

struct Mesh {
	GLuint vertexArrayObject;
	GLuint vertexBuffer;
	GLuint indexBuffer;
	uint32_t indexCount;
};

Mesh_t meshCreate(void (*applyAttributesFn)(void), const void *vertexData, const uint32_t vertexDataSize, const uint16_t *indicesData, const uint32_t indicesDataSize)
{
	GLuint vertexArrayObject;
	glGenVertexArrays(1, &vertexArrayObject);
	glBindVertexArray(vertexArrayObject);
	GLuint vertexBuffer = 0;
	glGenBuffers(1, &vertexBuffer);
	glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer);
	glBufferData(GL_ARRAY_BUFFER, vertexDataSize, vertexData, GL_STATIC_DRAW);
	GLuint indexBuffer = 0;
	glGenBuffers(1, &indexBuffer);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexBuffer);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, indicesDataSize, indicesData, GL_STATIC_DRAW);
	applyAttributesFn();
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

	glBindVertexArray(0);

	Mesh_t mesh = (Mesh_t)malloc(sizeof(struct Mesh));
	mesh->vertexArrayObject = vertexArrayObject;
	mesh->vertexBuffer = vertexBuffer;
	mesh->indexBuffer = indexBuffer;
	mesh->indexCount = indicesDataSize / sizeof(uint16_t);
	return mesh;
}

void meshDestroy(Mesh_t *mesh) {
	if (mesh && *mesh) {
		if ((*mesh)->vertexBuffer) {
			glDeleteBuffers(1, &(*mesh)->vertexBuffer);
			(*mesh)->vertexBuffer = 0;
		}
		if ((*mesh)->indexBuffer) {
			glDeleteBuffers(1, &(*mesh)->indexBuffer);
			(*mesh)->indexBuffer = 0;
		}
		if ((*mesh)->vertexArrayObject) {
			glDeleteVertexArrays(1, &(*mesh)->vertexArrayObject);
			(*mesh)->vertexArrayObject = 0;
		}
		(*mesh)->indexCount = 0;
		free(*mesh);
		*mesh = NULL;
	}
}

void meshUpdateVertices(Mesh_t mesh, const void *vertexData, const uint32_t vertexDataSize) {
	if (!mesh)
		return;
	
	glBindVertexArray(mesh->vertexArrayObject);
	glBindBuffer(GL_ARRAY_BUFFER, mesh->vertexBuffer);
	glBufferData(GL_ARRAY_BUFFER, vertexDataSize, vertexData, GL_STATIC_DRAW);
}

void meshDraw(Mesh_t mesh) {
	if (!mesh)
		return;

	glBindVertexArray(mesh->vertexArrayObject);
	glBindBuffer(GL_ARRAY_BUFFER, mesh->vertexBuffer);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mesh->indexBuffer);
	glDrawElements(GL_TRIANGLES, mesh->indexCount, GL_UNSIGNED_SHORT, 0);
}

