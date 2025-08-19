#ifndef __SYSTEMS_H__
#define __SYSTEMS_H__

#include "FT2.h"
#include "common_shaders.h"

typedef struct Systems *Systems_t;

Systems_t systemsCreate();
void systemsDestroy(Systems_t *systems);
const FT2_t systemsGetFt2(const Systems_t systems);
const CommonShaders_t systemsGetCommonShaders(const Systems_t systems);

#endif

