#include "systems.h"
#include "FT2.h"
#include <stdlib.h>
#include <string.h>

struct Systems {
	FT2_t ft2;
};

Systems_t systemsCreate() {
	Systems_t systems = (Systems_t)malloc(sizeof(struct Systems));
	memset(systems, 0, sizeof(struct Systems));

	systems->ft2 = ft2Create();

	return systems;
}
void systemsDestroy(Systems_t *systems) {
	if (systems && *systems) {
		ft2Destroy(&(*systems)->ft2);

		free(*systems);
		*systems = 0;
	}
}
FT2_t systemsGetFt2(Systems_t systems) {
	if (systems) {
		return systems->ft2;
	}
	return 0;
}

