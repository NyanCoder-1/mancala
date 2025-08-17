#ifndef __SYSTEMS_H__
#define __SYSTEMS_H__

#include "FT2.h"

typedef struct Systems *Systems_t;

Systems_t systemsCreate();
void systemsDestroy(Systems_t *systems);
FT2_t systemsGetFt2(Systems_t systems);

#endif

