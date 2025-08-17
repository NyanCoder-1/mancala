#ifndef __CORE_H__
#define __CORE_H__

#include "scene.h"
#include "systems.h"

typedef struct Core *Core_t;

void coreInit(void);
void coreRun(void);
void coreSetScene(Core_t core, Scene_t scene);
Systems_t coreGetSystems(Core_t core);

#endif

