#include "core.h"
#include "scene.h"
#include "scene_main.h"
#include "systems.h"
#include <emscripten.h>
#include <emscripten/emscripten.h>
#include <emscripten/html5.h>
#include <GLES3/gl3.h>
#include <string.h>
#include <time.h>
#include <unistd.h>

typedef struct Core {
	struct timespec timePrev;
	Scene_t scene;
	Systems_t systems;
	int width;
	int height;
} Core, *Core_t;

static void initGlContext(void);
static void mainLoop(Core_t core);
static bool clickCallback(int eventType, const EmscriptenMouseEvent *mouseEvent, void *userData);
static bool mouseDownCallback(int eventType, const EmscriptenMouseEvent *mouseEvent, void *userData);
static bool mouseUpCallback(int eventType, const EmscriptenMouseEvent *mouseEvent, void *userData);
static bool mouseMoveCallback(int eventType, const EmscriptenMouseEvent *mouseEvent, void *userData);
static bool resizeCallback(int eventType, const EmscriptenUiEvent *uiEvent, void *userData);

void coreInit(void) {
	initGlContext();
}
void coreRun(void) {
	Core core;
	memset(&core, 0, sizeof(core));
	// init core data
	clock_gettime(CLOCK_MONOTONIC, &core.timePrev);
	emscripten_get_canvas_element_size("!canvas", &core.width, &core.height);
	if (core.width <= 0) core.width = 1920;
	if (core.height <= 0) core.height = 1080;
	core.systems = systemsCreate();

	// callbacks
	emscripten_set_resize_callback(EMSCRIPTEN_EVENT_TARGET_WINDOW, &core, EM_FALSE, resizeCallback);
	emscripten_set_click_callback(EMSCRIPTEN_EVENT_TARGET_WINDOW, &core, EM_FALSE, clickCallback);
	emscripten_set_mousedown_callback(EMSCRIPTEN_EVENT_TARGET_WINDOW, &core, EM_FALSE, mouseDownCallback);
	emscripten_set_mouseup_callback(EMSCRIPTEN_EVENT_TARGET_WINDOW, &core, EM_FALSE, mouseUpCallback);
	emscripten_set_mousemove_callback(EMSCRIPTEN_EVENT_TARGET_WINDOW, &core, EM_FALSE, mouseMoveCallback);

	// init scene
	coreSetScene(&core, sceneMainCreate(&core));

	// actual main loop
	emscripten_set_main_loop_arg((void(*)(void*))mainLoop, &core, 0, EM_TRUE);

	sceneDestroy(&core.scene);
	systemsDestroy(&core.systems);
}
void coreSetScene(Core_t core, Scene_t scene) {
	if (core) {
		core->scene = scene;
		sceneOnResize(core->scene, core->width > 0 ? core->width : 1920, core->height > 0 ? core->height : 1080);
	}
}
Systems_t coreGetSystems(Core_t core) {
	if (core) {
		return core->systems;
	}
	return 0;
}

EM_JS(void, mapCanvas, (void), {
	specialHTMLTargets["!canvas"] = Module.canvas;
})

static void initGlContext(void) {
	mapCanvas();
	EmscriptenWebGLContextAttributes ctxAttrs;
	emscripten_webgl_init_context_attributes(&ctxAttrs);
	ctxAttrs.majorVersion = 2;
	ctxAttrs.minorVersion = 0;
	EMSCRIPTEN_WEBGL_CONTEXT_HANDLE ctx = emscripten_webgl_create_context("!canvas", &ctxAttrs);
	emscripten_webgl_make_context_current(ctx);
	glEnable(GL_CULL_FACE);
	glFrontFace(GL_CW);
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glDepthMask(GL_TRUE);
}

static void mainLoop(Core_t core) {
	if (core) {
		// time and dTime
		struct timespec timeCurrent;
		clock_gettime(CLOCK_MONOTONIC, &timeCurrent); // Never use `gettimeofday()` for dTime, you could use that for current time, like for clock, but not for dTime
		double deltaTime = (timeCurrent.tv_sec - core->timePrev.tv_sec) + (timeCurrent.tv_nsec - core->timePrev.tv_nsec) / 1000000000.0;
		core->timePrev = timeCurrent;

		// update scene
		sceneOnUpdate(core->scene, deltaTime);

		// clear screen
		glClearColor(0.0f, 0.5f, 0.5f, 1.0f); // windows 95 teal
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		// render scene
		sceneOnDraw(core->scene);
	}
}

static bool clickCallback(int eventType, const EmscriptenMouseEvent *mouseEvent, void *userData) {
	if (!userData)
		return false;
	if (eventType == EMSCRIPTEN_EVENT_CLICK) {
		Core_t core = (Core_t)userData;
		sceneOnClick(core->scene, mouseEvent->clientX, mouseEvent->clientY);
	}
	return true;
}

static bool mouseDownCallback(int eventType, const EmscriptenMouseEvent *mouseEvent, void *userData) {
	if (!userData)
		return false;
	if (eventType == EMSCRIPTEN_EVENT_MOUSEDOWN) {
		Core_t core = (Core_t)userData;
		sceneOnMouseDown(core->scene, (MouseButton)mouseEvent->button, mouseEvent->clientX, mouseEvent->clientY);
	}
	return true;
}

static bool mouseUpCallback(int eventType, const EmscriptenMouseEvent *mouseEvent, void *userData) {
	if (!userData)
		return false;
	if (eventType == EMSCRIPTEN_EVENT_MOUSEUP) {
		Core_t core = (Core_t)userData;
		sceneOnMouseUp(core->scene, (MouseButton)mouseEvent->button, mouseEvent->clientX, mouseEvent->clientY);
	}
	return true;
}

static bool mouseMoveCallback(int eventType, const EmscriptenMouseEvent *mouseEvent, void *userData) {
	if (!userData)
		return false;
	if (eventType == EMSCRIPTEN_EVENT_MOUSEMOVE) {
		Core_t core = (Core_t)userData;
		sceneOnMouseMove(core->scene, mouseEvent->clientX, mouseEvent->clientY);
	}
	return true;
}

static bool resizeCallback(int eventType, const EmscriptenUiEvent *uiEvent, void *userData) {
	if (!userData)
		return false;
	if (eventType == EMSCRIPTEN_EVENT_RESIZE) {
		Core_t core = (Core_t)userData;
		int width = uiEvent->documentBodyClientWidth;
		int height = uiEvent->documentBodyClientHeight;
		emscripten_get_canvas_element_size("!canvas", &width, &height);
		if ((width > 0) && (height > 0)) {
			core->width = width;
			core->height = height;
			glViewport(0, 0, width, height);
			sceneOnResize(core->scene, width, height);
		}
	}
	return true;
}

