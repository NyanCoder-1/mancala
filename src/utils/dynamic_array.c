#include "utils/dynamic_array.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct DynamicArray {
	void* mem;
	size_t size;
	size_t capacity;
	size_t elementSize;
};
static const size_t POP_SHRINK_THRESHOLD = 64;

// Internal logic
static uint8_t daTryInflate(DynamicArray_t da, size_t increaseSize) {
	if (da) {
		if (!da->capacity || !da->mem) {
			// something went wrong, probably fixable, try to fix
			if (da->mem) {
				free(da->mem);
			}
			void* newData = malloc(increaseSize * da->elementSize);
			if (!newData) {
				fprintf(stderr, "Can't allocate memory for dynamic array\n");
				return 0;
			}
			da->mem = newData;
			da->capacity = increaseSize;
			return 1;
		}
		if (da->size + increaseSize > da->capacity) {
			// need to inflate
			size_t newCapacity = da->capacity * 2;
			if (newCapacity < da->size + increaseSize) {
				newCapacity = da->size + increaseSize;
			}
			void* newData = realloc(da->mem, newCapacity * da->elementSize);
			if (!newData) {
				fprintf(stderr, "Can't reallocate memory to increased capacity\n");
				return 0;
			}
			da->mem = newData;
			da->capacity = newCapacity;
			return 1;
		}
		// it's all good, no need to inflate
		return 1;
	}
	// No array
	return 0;
}

// ctor, dtor
DynamicArray_t daCreate(size_t elementSize) {
	if (!elementSize) {
		return 0;
	}
	DynamicArray_t da = (DynamicArray_t)malloc(sizeof(struct DynamicArray));
	if (!da) {
		fprintf(stderr, "Can't allocate memory for struct\n");
		return 0;
	}
	da->mem = 0;
	da->size = 0;
	da->capacity = 0;
	da->elementSize = elementSize;
	return da;
}
void daDestroy(DynamicArray_t *daPtr) {
	if (daPtr && *daPtr) {
		const DynamicArray_t da = *daPtr;
		if (da->mem) {
			free(da->mem);
			da->mem = 0;
		}
		da->size = 0;
		da->capacity = 0;
		da->elementSize = 0;
		free(da);
		*daPtr = 0;
	}
}
DynamicArray_t daCopy(DynamicArray_t da) {
	if (da && da->elementSize) {
		DynamicArray_t newDa = (DynamicArray_t)malloc(sizeof(struct DynamicArray));
		if (!newDa) {
			fprintf(stderr, "Can't allocate memory for struct\n");
			return 0;
		}
		if (da->capacity) {
			newDa->mem = malloc(da->capacity * da->elementSize);
			if (!newDa->mem) {
				fprintf(stderr, "Can't allocate memory for dynamic array\n");
				free(newDa);
				newDa = 0;
				return 0;
			}
			memcpy(newDa->mem, da->mem, da->size * da->elementSize);
		}
		else {
			newDa->mem = 0;
		}
		newDa->size = da->size;
		newDa->capacity = da->capacity;
		newDa->elementSize = da->elementSize;
		return newDa;
	}
	return 0;
}

// Element access
void* daFront(DynamicArray_t da) {
	if (da && da->mem && da->size) {
		return da->mem;
	}
	return 0;
}
void* daBack(DynamicArray_t da) {
	if (da && da->mem && da->size) {
		return &(((uint8_t*)da->mem)[(da->size - 1) * da->elementSize]);
	}
	return 0;
}
void* daGet(DynamicArray_t da, size_t index) {
	if (da && da->mem && (index < da->size)) {
		return &(((uint8_t*)da->mem)[index * da->elementSize]);
	}
	return 0;
}
void* daData(DynamicArray_t da) {
	if (da && da->mem) {
		return da->mem;
	}
	return 0;
}

// Capacity
uint8_t daEmpty(DynamicArray_t da) {
	return (!da || !da->size);
}
size_t daSize(DynamicArray_t da) {
	return da ? da->size : 0;
}
size_t daCapacity(DynamicArray_t da) {
	return da ? da->capacity : 0;
}

// Modifiers
void daClear(DynamicArray_t da) {
	if (da && da->size) {
		da->size = 0;
	}
}
void daInsert(DynamicArray_t da, size_t at, void* element) {
	daInsertArray(da, at, element, 1);
}
void daInsertArray(DynamicArray_t da, size_t at, void* elements, size_t amount) {
	if (da && amount) {
		if (!daTryInflate(da, amount)) {
			fprintf(stderr, "Ignoring...\n");
			return;
		}
		if (da->size > at) {
			// insert in middle
			for (size_t i = da->size; i > at; i--) {
				void* dest = ((uint8_t*)da->mem) + (i + amount - 1) * da->elementSize;
				void* src  = ((uint8_t*)da->mem) + (i - 1) * da->elementSize;
				memcpy(dest, src, da->elementSize);
			}
			da->size += amount;
			memcpy(daGet(da, at), elements, amount * da->elementSize);
			return;
		}
		// insert in back
		daPushBackArray(da, elements, amount);
	}
}
void daErase(DynamicArray_t da, size_t at) {
	daEraseRange(da, at, 1);
}
void daEraseRange(DynamicArray_t da, size_t at, size_t amount) {
	if (da && da->mem && da->size && at < da->size) {
		// data is trailing
		if (at + amount >= da->size) {
			da->size = at;
			return;
		}
		// default behaviour
		size_t elementsToShift = da->size - (at + amount);
		for (size_t i = 0; i < elementsToShift; i++) {
			void* dest = ((uint8_t*)da->mem) + (at + i) * da->elementSize;
			void* src = ((uint8_t*)da->mem) + (at + amount + i) * da->elementSize;
			memcpy(dest, src, da->elementSize);
		}
		da->size -= amount;
	}
}
void daPushBack(DynamicArray_t da, void* element) {
	daPushBackArray(da, element, 1);
}
void daPushBackArray(DynamicArray_t da, void* elements, size_t amount) {
	if (da) {
		if (!daTryInflate(da, amount)) {
			fprintf(stderr, "Ignoring...\n");
			return;
		}
		void *back = ((uint8_t*)da->mem) + da->size * da->elementSize;
		memcpy(back, elements, amount * da->elementSize);
		da->size += amount;
	}
}
void daPopBack(DynamicArray_t da, void* out) {
	if (da) {
		if (out) {
			if (da->mem && da->size) {
				memcpy(out, daBack(da), da->elementSize);
			}
			else {
				memset(out, 0, da->elementSize);
			}
		}
		if (da->size) {
			da->size--;
			if (da->size < da->capacity / 4 && da->capacity > POP_SHRINK_THRESHOLD) {
				size_t newCapacity = da->capacity / 2;
				void* newPtr = realloc(da->mem, newCapacity * da->elementSize);
				if (!newPtr) {
					fprintf(stderr, "Couldn't shrink dynamic array capacity\nIgnoring...\n");
					return;
				}
				da->mem = newPtr;
				da->capacity = newCapacity;
			}
		}
	}
}
void daResize(DynamicArray_t da, size_t newSize) {
	if (da) {
		// ignore if already at target size
		if (newSize == da->size) {
			return;
		}
		// inflate
		if (newSize > da->size) {
			size_t dSize = newSize - da->size;
			if (!daTryInflate(da, dSize)) {
				fprintf(stderr, "Ignoring...\n");
				return;
			}
			void* ptr = ((uint8_t*)da->mem) + da->size * da->elementSize;
			memset(ptr, 0, dSize * da->elementSize);
		}
		// both on inflate and shrinking
		da->size = newSize;
	}
}
void daReserve(DynamicArray_t da, size_t newCapacity) {
	if (da && da->elementSize) {
		if (!da->capacity || !da->mem) {
			// need to allocate new memory
			if (!da->mem) {
				free(da->mem);
			}
			void* newData = malloc(newCapacity * da->elementSize);
			if (!newData) {
				fprintf(stderr, "Can't allocate memory for dynamic array\n");
				return;
			}
			da->mem = newData;
			da->capacity = newCapacity;
			return;
		}
		if (newCapacity > da->capacity) {
			// inflate
			void* newData = realloc(da->mem, newCapacity * da->elementSize);
			if (!newData) {
				fprintf(stderr, "Can't reallocate memory to increase dynamic array size\n");
				return;
			}
			da->mem = newData;
			da->capacity = newCapacity;
			return;
		}
		// no shrinking
	}
}
void daShrinkToFit(DynamicArray_t da) {
	if (da && da->mem) {
		if (da->size < da->capacity) {
			if (!da->size) {
				// empty
				free(da->mem);
				da->mem = 0;
				da->capacity = 0;
				return;
			}
			void* newData = realloc(da->mem, da->size);
			if (!newData) {
				fprintf(stderr, "Couldn't shrink memory\n");
				return;
			}
			da->mem = newData;
			da->capacity = da->size;
			return;
		}
		if (da->size > da->capacity) {
			fprintf(stderr, "Dynamic array in broken state\n");
			return;
		}
	}
}

