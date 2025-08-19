#ifndef __CAMERA_H__
#define __CAMERA_H__

#include "shader.h"

typedef struct Camera* Camera_t;

Camera_t cameraCreate(int screenWidth, int screenHeight);
void cameraDestroy(Camera_t* cam);
void cameraApplyViewProjection(const Camera_t cam, const Shader_t shader);
void cameraApplyModel(const Camera_t cam, const Shader_t shader, double x, double y, double z, double roll, double pitch, double yaw);
void cameraSetScreenSize(Camera_t cam, int screenWidth, int screenHeight);
void cameraSetOrtho(Camera_t cam);
void cameraSetProjection(Camera_t cam);
void cameraSetPosition(Camera_t cam, double x, double y, double z);
void cameraSetRotation(Camera_t cam, double roll, double pitch, double yaw);

#endif

