#include "scene.h"
#include <stdio.h>
#include <stdlib.h>

struct Scene {
	void *userdata;
	cbDtor dtor;
	cbUpdate onUpdate;
	cbDraw onDraw;
	cbClick onClick;
	cbMouseDown onMouseDown;
	cbMouseUp onMouseUp;
	cbMouseMove onMouseMove;
	cbResize onResize;
};

Scene_t sceneCreate() {
	Scene_t scene = (Scene_t)malloc(sizeof(struct Scene));
	if (!scene) {
		fprintf(stderr, "Couldn't allocate memory for Scene struct\n");
		return 0;
	}
	scene->userdata = 0;
	scene->dtor = 0;
	scene->onUpdate = 0;
	scene->onDraw = 0;
	scene->onClick = 0;
	scene->onMouseDown = 0;
	scene->onMouseUp = 0;
	scene->onMouseMove = 0;
	scene->onResize = 0;
	return scene;
}
void sceneDestroy(Scene_t *scene) {
	if (scene && *scene) {
		if ((*scene)->dtor) {
			(*scene)->dtor((*scene)->userdata);
		}
		free(*scene);
		*scene = 0;
	}
}

void sceneSetUserdata(Scene_t scene, void* userdata) {
	if (scene) {
		scene->userdata = userdata;
	}
}
void sceneSetDestructor(Scene_t scene, void (*dtor)(void* userdata)) {
	if (scene) {
		scene->dtor = dtor;
	}
}
void sceneSetCallbackUpdate(Scene_t scene, void (*callback)(void* userdata, double dTime)) {
	if (scene) {
		scene->onUpdate = callback;
	}
}
void sceneSetCallbackDraw(Scene_t scene, void (*callback)(void* userdata)) {
	if (scene) {
		scene->onDraw = callback;
	}
}
void sceneSetCallbackClick(Scene_t scene, void (*callback)(void* userdata, int x, int y)) {
	if (scene) {
		scene->onClick = callback;
	}
}
void sceneSetCallbackMouseDown(Scene_t scene, void (*callback)(void* userdata, MouseButton btn, int x, int y)) {
	if (scene) {
		scene->onMouseDown = callback;
	}
}
void sceneSetCallbackMouseUp(Scene_t scene, void (*callback)(void* userdata, MouseButton btn, int x, int y)) {
	if (scene) {
		scene->onMouseUp = callback;
	}
}
void sceneSetCallbackMouseMove(Scene_t scene, void (*callback)(void* userdata, int x, int y)) {
	if (scene) {
		scene->onMouseMove = callback;
	}
}
void sceneSetCallbackResize(Scene_t scene, void (*callback)(void* userdata, int width, int height)) {
	if (scene) {
		scene->onResize = callback;
	}
}

void sceneOnUpdate(Scene_t scene, double dTime) {
	if (scene && scene->onUpdate) {
		scene->onUpdate(scene->userdata, dTime);
	}
}
void sceneOnDraw(Scene_t scene) {
	if (scene && scene->onDraw) {
		scene->onDraw(scene->userdata);
	}
}
void sceneOnClick(Scene_t scene, int x, int y) {
	if (scene && scene->onClick) {
		scene->onClick(scene->userdata, x, y);
	}
}
void sceneOnMouseDown(Scene_t scene, MouseButton btn, int x, int y) {
	if (scene && scene->onMouseDown) {
		scene->onMouseDown(scene->userdata, btn, x, y);
	}
}
void sceneOnMouseUp(Scene_t scene, MouseButton btn, int x, int y) {
	if (scene && scene->onMouseUp) {
		scene->onMouseUp(scene->userdata, btn, x, y);
	}
}
void sceneOnMouseMove(Scene_t scene, int x, int y) {
	if (scene && scene->onMouseMove) {
		scene->onMouseMove(scene->userdata, x, y);
	}
}
void sceneOnResize(Scene_t scene, int width, int height) {
	if (scene && scene->onResize) {
		scene->onResize(scene->userdata, width, height);
	}
}

