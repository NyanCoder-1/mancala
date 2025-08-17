#include "game_states.h"
#include <stdlib.h>
#include <string.h>

struct GameStates {
	int state;
};

const int GSTATE_MENU = 0;
const int GSTATE_SETTINGS = 1;
const int GSTATE_GAME = 2;
const int GSTATE_WIN = 3;
static const int GSTATE_DEFAULT = GSTATE_MENU;

GameStates_t gsCreate(SceneMain_t sceneMain) {
	GameStates_t gs = (GameStates_t)malloc(sizeof(struct GameStates));
	memset(gs, 0, sizeof(struct GameStates));
	gs->state = GSTATE_MENU;
	return gs;
}
void gsDestroy(GameStates_t *gameStates) {
	if (gameStates && *gameStates) {
		free(*gameStates);
		*gameStates = 0;
	}
}
void gsSetState(GameStates_t gameStates, int state) {
	if (gameStates) {
		gameStates->state = state;
	}
}
int gsGetState(const GameStates_t gameStates) {
	if (gameStates) {
		return gameStates->state;
	}
	return GSTATE_DEFAULT;
}

