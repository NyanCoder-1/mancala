#include "shader.h"
#include "assets.h"
#include "mesh_helper.h"
#include <GLES3/gl3.h>
#include <stdio.h>
#include <stdlib.h>

#define SHADER_UNIFORM_LOCATIONS_AMOUNT 2
static const char* const uniformNames[SHADER_UNIFORM_LOCATIONS_AMOUNT] = {
	"matModel",
	"matViewProjection",
};

struct Shader {
	GLuint program;
	GLint uniformLocations[SHADER_UNIFORM_LOCATIONS_AMOUNT];
};

Shader_t shaderCreateFromFile(const char *vertexShaderPath, const char *fragmentShaderPath) {
	uint32_t vertexShaderSourceSize = 0;
	char* vertexShaderSource = (char*)assetLoad(vertexShaderPath, &vertexShaderSourceSize);
	uint32_t fragmentShaderSourceSize = 0;
	char* fragmentShaderSource = (char*)assetLoad(fragmentShaderPath, &fragmentShaderSourceSize);

	Shader_t shader = shaderCreateFromMemory(vertexShaderSource, vertexShaderSourceSize, fragmentShaderSource, fragmentShaderSourceSize);
	free(vertexShaderSource);
	free(fragmentShaderSource);
	return shader;
}

GLuint createShaderObject(const char* shaderSource, int shaderType) {
	// Create shader
	GLuint shader = glCreateShader(shaderType);
	// Attach shader source
	glShaderSource(shader, 1, &shaderSource, NULL);
	// Compile shader
	glCompileShader(shader);
	// Check for errors
	GLint status;
	glGetShaderiv(shader, GL_COMPILE_STATUS, &status);
	if (status == GL_FALSE) {
		// Print error
		GLint maxLength	= 0;
		glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &maxLength);
		GLchar errorLog[maxLength];
		glGetShaderInfoLog(shader, maxLength, &maxLength, &errorLog[0]);
		glDeleteShader(shader);
		fprintf(stderr, "%s\n", &errorLog[0]);
	}
	return shader;
}

Shader_t shaderCreateFromMemory(const char *vertexShaderSource, const uint32_t vertexShaderSourceSize, const char *fragmentShaderSource, const uint32_t fragmentShaderSourceSize) {
	// Create shader program
	GLuint program = glCreateProgram();
	// Create shaders
	GLuint vShader = createShaderObject(vertexShaderSource, GL_VERTEX_SHADER);
	GLuint fShader = createShaderObject(fragmentShaderSource, GL_FRAGMENT_SHADER);
	// Attach shaders
	glAttachShader(program, vShader);
	glAttachShader(program, fShader);
	// Set attrib locations per API
	glBindAttribLocation(program, MESH_ATTRIB_POS, "inPos");
	glBindAttribLocation(program, MESH_ATTRIB_COLOR, "inColor");
	glBindAttribLocation(program, MESH_ATTRIB_UV, "inUv");
	// Link program
	glLinkProgram(program);
	// Predelete shader objects, so they automatically get deleted on glDeleteProgram
	glDeleteShader(vShader);
	glDeleteShader(fShader);

	// Create a wrapper object
	Shader_t shader = (Shader_t)malloc(sizeof(struct Shader));
	shader->program = program;
	for (int i = 0; i < SHADER_UNIFORM_LOCATIONS_AMOUNT; i++) {
		shader->uniformLocations[i] = glGetUniformLocation(program, uniformNames[i]);
	}
	return shader;
}

void shaderDestroy(Shader_t *shader) {
	if (shader && *shader) {
		if ((*shader)->program) {
			glDeleteProgram((*shader)->program);
			(*shader)->program = 0;
		}
		free(*shader);
		*shader = NULL;
	}
}

uint32_t shaderGetProgram(const Shader_t shader) {
	return shader ? shader->program : 0;
}

int32_t shaderGetUniformLocation(const Shader_t shader, uint32_t locationId) {
	if (shader) {
		return shader->uniformLocations[locationId];
	}
	return -1;
}
int32_t shaderLoadUniformLocation(const Shader_t shader, const char* const locationName) {
	if (shader) {
		return glGetUniformLocation(shader->program, locationName);
	}
	return -1;
}

void shaderUse(const Shader_t shader) {
	if (shader)
		glUseProgram(shader->program);
}


