#include "camera.h"
#include "cglm/mat4.h"
#include "shader.h"
#include <cglm/cglm.h>
#include <GLES3/gl3.h>
#include <stdlib.h>
#include <string.h>

static const uint8_t MODE_PROJECTION = 0;
static const uint8_t MODE_ORTHO = 1;

struct Camera {
	double x, y, z;
	double roll, pitch, yaw;
	int screenWidth, screenHeight;
	uint8_t mode;
};

Camera_t cameraCreate(int screenWidth, int screenHeight) {
	Camera_t cam = (Camera_t)malloc(sizeof(struct Camera));
	memset(cam, 0, sizeof(struct Camera));
	cameraSetScreenSize(cam, screenWidth, screenHeight);
	return cam;
}
void cameraDestroy(Camera_t* cam) {
	if (cam && *cam) {
		free(*cam);
		*cam = 0;
	}
}
void cameraApplyViewProjection(const Camera_t cam, const Shader_t shader) {
	if (cam && shader) {
		mat4 viewProjection = GLM_MAT4_IDENTITY;
		if (cam->mode == MODE_ORTHO) {
			mat4 position = GLM_MAT4_IDENTITY;
			mat4 rotation = GLM_MAT4_IDENTITY;
			glm_euler((vec3){-cam->roll, -cam->pitch, -cam->yaw}, rotation);
			glm_translate(position, (vec3){-cam->x, -cam->y, -cam->z});
			mat4 view = GLM_MAT4_IDENTITY;
			glm_mul(rotation, position, view);

			mat4 projection = GLM_MAT4_IDENTITY;
			glm_ortho(0.0f, cam->screenWidth, cam->screenHeight, 0.0f, 0.0f, 1000.0f, projection);

			glm_mat4_mul(projection, view, viewProjection);
		}
		else if (cam->mode == MODE_PROJECTION) {
			mat4 position = GLM_MAT4_IDENTITY;
			mat4 rotation = GLM_MAT4_IDENTITY;
			glm_euler((vec3){-cam->roll, -cam->pitch, -cam->yaw}, rotation);
			glm_translate(position, (vec3){-cam->x, -cam->y, -cam->z});
			mat4 view = GLM_MAT4_IDENTITY;
			glm_mul(rotation, position, view);

			mat4 projection = GLM_MAT4_IDENTITY;
			glm_perspective(glm_rad(90.0f), (double)cam->screenWidth / (double)cam->screenHeight, 0.1f, 100.0f, projection);

			glm_mat4_mul(projection, view, viewProjection);
		}
		glUniformMatrix4fv(shaderGetUniformLocation(shader, SHADER_UNIFORM_LOCATION_ID_MATRIX_VIEWPROJECTION), 1, GL_TRUE, (GLfloat*)viewProjection);
	}
}
void cameraApplyModel(const Camera_t cam, const Shader_t shader, double x, double y, double z, double roll, double pitch, double yaw) {
	if (cam && shader) {
		mat4 rotation;
		glm_euler((vec3){roll, pitch, yaw}, rotation);
		mat4 translation = GLM_MAT4_IDENTITY;
		glm_translate(translation, (vec3){x, y, z});
		mat4 model = GLM_MAT4_IDENTITY;
		glm_mul(translation, rotation, model);
		glUniformMatrix4fv(shaderGetUniformLocation(shader, SHADER_UNIFORM_LOCATION_ID_MATRIX_MODEL), 1, GL_TRUE, (GLfloat*)model);
	}
}
void cameraSetScreenSize(Camera_t cam, int screenWidth, int screenHeight) {
	if (cam) {
		cam->screenWidth = screenWidth;
		cam->screenHeight = screenHeight;
	}
}
void cameraSetOrtho(Camera_t cam) {
	if (cam) {
		cam->mode = MODE_ORTHO;
	}
}
void cameraSetProjection(Camera_t cam) {
	if (cam) {
		cam->mode = MODE_PROJECTION;
	}
}
void cameraSetPosition(Camera_t cam, double x, double y, double z) {
	if (cam) {
		cam->x = x;
		cam->y = y;
		cam->z = z;
	}
}
void cameraSetRotation(Camera_t cam, double roll, double pitch, double yaw) {
	if (cam) {
		cam->roll = roll;
		cam->pitch = pitch;
		cam->yaw = yaw;
	}
}

