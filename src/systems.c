#include "systems.h"
#include "FT2.h"
#include "common_shaders.h"
#include <stdlib.h>
#include <string.h>

struct Systems {
	FT2_t ft2;
	CommonShaders_t commonShaders;
};

Systems_t systemsCreate() {
	Systems_t systems = (Systems_t)malloc(sizeof(struct Systems));
	memset(systems, 0, sizeof(struct Systems));

	systems->ft2 = ft2Create();
	systems->commonShaders = commonShadersCreate();

	return systems;
}
void systemsDestroy(Systems_t *systems) {
	if (systems && *systems) {
		ft2Destroy(&(*systems)->ft2);
		commonShadersDestroy(&(*systems)->commonShaders);

		free(*systems);
		*systems = 0;
	}
}
const FT2_t systemsGetFt2(const Systems_t systems) {
	if (systems) {
		return systems->ft2;
	}
	return 0;
}
const CommonShaders_t systemsGetCommonShaders(const Systems_t systems) {
	if (systems) {
		return systems->commonShaders;
	}
	return 0;
}

