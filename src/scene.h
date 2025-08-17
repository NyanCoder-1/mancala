#ifndef __SCENE_H__
#define __SCENE_H__

#include "utils/mouse_button.h"

typedef struct Scene *Scene_t;
typedef void (*cbDtor)(void* userdata);
typedef void (*cbUpdate)(void* userdata, double dTime);
typedef void (*cbDraw)(void* userdata);
typedef void (*cbClick)(void* userdata, int x, int y);
typedef void (*cbMouseDown)(void* userdata, MouseButton btn, int x, int y);
typedef void (*cbMouseUp)(void* userdata, MouseButton btn, int x, int y);
typedef void (*cbMouseMove)(void* userdata, int x, int y);
typedef void (*cbResize)(void* userdata, int width, int height);

Scene_t sceneCreate();
void sceneDestroy(Scene_t *scene);

#define SCENE_FILL_DATA(scene, userdata, dtor, onUpdate, onDraw, onClick, onMouseDown, onMouseUp, onMouseMove, onResize)\
	sceneSetUserdata(scene, userdata);\
	sceneSetDestructor(scene, (cbDtor)dtor);\
	sceneSetCallbackUpdate(scene, (cbUpdate)onUpdate);\
	sceneSetCallbackDraw(scene, (cbDraw)onDraw);\
	sceneSetCallbackClick(scene, (cbClick)onClick);\
	sceneSetCallbackMouseDown(scene, (cbMouseDown)onMouseDown);\
	sceneSetCallbackMouseUp(scene, (cbMouseUp)onMouseUp);\
	sceneSetCallbackMouseMove(scene, (cbMouseMove)onMouseMove);\
	sceneSetCallbackResize(scene, (cbResize)onResize);
void sceneSetUserdata(Scene_t scene, void* userdata);
void sceneSetDestructor(Scene_t scene, void (*dtor)(void* userdata));
void sceneSetCallbackUpdate(Scene_t scene, void (*callback)(void* userdata, double dTime));
void sceneSetCallbackDraw(Scene_t scene, void (*callback)(void* userdata));
void sceneSetCallbackClick(Scene_t scene, void (*callback)(void* userdata, int x, int y));
void sceneSetCallbackMouseDown(Scene_t scene, void (*callback)(void* userdata, MouseButton btn, int x, int y));
void sceneSetCallbackMouseUp(Scene_t scene, void (*callback)(void* userdata, MouseButton btn, int x, int y));
void sceneSetCallbackMouseMove(Scene_t scene, void (*callback)(void* userdata, int x, int y));
void sceneSetCallbackResize(Scene_t scene, void (*callback)(void* userdata, int width, int height));

void sceneOnUpdate(Scene_t scene, double dTime);
void sceneOnDraw(Scene_t scene);
void sceneOnClick(Scene_t scene, int x, int y);
void sceneOnMouseDown(Scene_t scene, MouseButton btn, int x, int y);
void sceneOnMouseUp(Scene_t scene, MouseButton btn, int x, int y);
void sceneOnMouseMove(Scene_t scene, int x, int y);
void sceneOnResize(Scene_t scene, int width, int height);

#endif

