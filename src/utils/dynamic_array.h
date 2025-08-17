#ifndef __UTILS__DYNAMIC_ARRAY_H__
#define __UTILS__DYNAMIC_ARRAY_H__

#include <stdint.h>
#include <stddef.h> // size_t

typedef struct DynamicArray *DynamicArray_t;

// ctor, dtor
DynamicArray_t daCreate(size_t elementSize);
void daDestroy(DynamicArray_t *da);
DynamicArray_t daCopy(DynamicArray_t da);

// Element access
void* daFront(DynamicArray_t da);
void* daBack(DynamicArray_t da);
void* daGet(DynamicArray_t da, size_t index);
void* daData(DynamicArray_t da);

// Capacity
uint8_t daEmpty(DynamicArray_t da);
size_t daSize(DynamicArray_t da);
size_t daCapacity(DynamicArray_t da);

// Modifiers
void daClear(DynamicArray_t da);
void daInsert(DynamicArray_t da, size_t at, void* element);
void daInsertArray(DynamicArray_t da, size_t at, void* elements, size_t amount);
void daErase(DynamicArray_t da, size_t at);
void daEraseRange(DynamicArray_t da, size_t at, size_t amount);
void daPushBack(DynamicArray_t da, void* element);
void daPushBackArray(DynamicArray_t da, void* elements, size_t amount);
void daPopBack(DynamicArray_t da, /* optional */ void* out);
void daResize(DynamicArray_t da, size_t newSize);
void daReserve(DynamicArray_t da, size_t newCapacity);
void daShrinkToFit(DynamicArray_t da);

#endif // __UTILS__DYNAMIC_ARRAY_H__

