#ifndef __GAME_STATES_H__
#define __GAME_STATES_H__

#include "scene_main.h"

typedef struct GameStates *GameStates_t;

extern const int GSTATE_MENU;
extern const int GSTATE_SETTINGS;
extern const int GSTATE_GAME;
extern const int GSTATE_WIN;

GameStates_t gsCreate(SceneMain_t sceneMain);
void gsDestroy(GameStates_t *gameStates);
void gsSetState(GameStates_t gameStates, int state);
int gsGetState(const GameStates_t gameStates);

#endif

