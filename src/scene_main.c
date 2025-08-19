#include "scene_main.h"
#include "FT2.h"
#include "common_shaders.h"
#include "core.h"
#include "font.h"
#include "shader.h"
#include "systems.h"
#include "text.h"
#include <cglm/cglm.h>
#include <GLES3/gl3.h>
#include <stdlib.h>

struct SceneMain {
	Core_t core;

	int width;
	int height;

	Shader_t shaderText;
	Font_t font;
	Text_t text;

	GLuint shaderTextMatModelLocation;
	GLuint shaderTextMatViewProjectionLocation;
	GLuint shaderTextColorLocation;
};

static void dtor(SceneMain_t sceneData);
static void update(SceneMain_t sceneData, double dTime);
static void draw(SceneMain_t sceneData);
static void click(SceneMain_t sceneData, int x, int y);
static void mouseDown(SceneMain_t sceneData, MouseButton button, int x, int y);
static void mouseUp(SceneMain_t sceneData, MouseButton button, int x, int y);
static void mouseMove(SceneMain_t sceneData, int x, int y);
static void resize(SceneMain_t sceneData, int width, int height);

Scene_t sceneMainCreate(Core_t core) {
	Scene_t scene = sceneCreate();
	struct SceneMain *sceneData = (SceneMain_t)malloc(sizeof(struct SceneMain));
	sceneData->core = core;
	SCENE_FILL_DATA(scene, sceneData, dtor, update, draw, click, mouseDown, mouseUp, mouseMove, resize);

	sceneData->shaderText = commonShadersGet(systemsGetCommonShaders(coreGetSystems(core)), COMMON_SHADER_TEXT);
	{
		sceneData->shaderTextMatModelLocation = shaderGetUniformLocation(sceneData->shaderText, SHADER_UNIFORM_LOCATION_ID_MATRIX_MODEL);
		sceneData->shaderTextMatViewProjectionLocation = shaderGetUniformLocation(sceneData->shaderText, SHADER_UNIFORM_LOCATION_ID_MATRIX_VIEWPROJECTION);
		sceneData->shaderTextColorLocation = shaderLoadUniformLocation(sceneData->shaderText, "diffuseColor");
	}

	sceneData->font = fontCreate(ft2GetDefaultFace(systemsGetFt2(coreGetSystems(core))), 24);
	sceneData->text = textCreate(sceneData->font, sceneData->shaderText);
	textSetText(sceneData->text, "Test text");

	return scene;
}

static void dtor(SceneMain_t sceneData) {
	if (sceneData) {
	}
	free(sceneData);
}
static void update(SceneMain_t sceneData, double dTime) {
	if (sceneData) {
	}
}
static void draw(SceneMain_t sceneData) {
	if (sceneData) {
		shaderUse(sceneData->shaderText);
		glUniform4f(sceneData->shaderTextColorLocation, 1.0f, 1.0f, 1.0f, 1.0f);
		mat4 camOrtho = GLM_MAT4_IDENTITY;
		glm_ortho(0.0f, sceneData->width, sceneData->height, 0.0f, 0.0f, 1000.0f, camOrtho);
		glUniformMatrix4fv(sceneData->shaderTextMatViewProjectionLocation, 1, GL_TRUE, (GLfloat*)camOrtho);
		{ // text
			mat4 model = GLM_MAT4_IDENTITY;
			vec3 translate = {0.0f, 0.0f, 0.0f};
			glm_translate(model, translate);
			glUniformMatrix4fv(sceneData->shaderTextMatModelLocation, 1, GL_TRUE, (GLfloat*)model);
			textDraw(sceneData->text);
		}
	}
}
static void click(SceneMain_t sceneData, int x, int y) {
	if (sceneData) {
	}
}
static void mouseDown(SceneMain_t sceneData, MouseButton button, int x, int y) {
	if (sceneData) {
	}
}
static void mouseUp(SceneMain_t sceneData, MouseButton button, int x, int y) {
	if (sceneData) {
	}
}
static void mouseMove(SceneMain_t sceneData, int x, int y) {
	if (sceneData) {
	}
}
static void resize(SceneMain_t sceneData, int width, int height) {
	if (sceneData) {
		if (width > 0) {
			sceneData->width = width;
		}
		if (height > 0) {
			sceneData->height = height;
		}
	}
}

