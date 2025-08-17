#include "unicode_blocks.h"
#include <stdint.h>

typedef enum BinarySearchMode {
	BSM_EXACT,
	BSM_FLOOR, // exact or less than searching value

	BSM_DEFAULT = BSM_EXACT
} BinarySearchMode;

static uint32_t binarySearch(const uint32_t value, const uint32_t* const array, const uint32_t arraySize, const BinarySearchMode mode) {
	uint32_t borderLeft = 0;
	uint32_t borderRight = arraySize - 1;
	if (array[borderRight] < value) {
		if (mode == BSM_FLOOR) {
			return borderRight;
		}
		return (uint32_t)-1;
	}
	if (value < array[borderLeft]) {
		return (uint32_t)-1;
	}
	while (borderLeft <= borderRight) {
		const uint32_t middleIndex = (uint32_t)((uint64_t)borderLeft + borderRight) / 2;
		const uint32_t currentValue = array[middleIndex];
		if (currentValue == value) {
			if (mode == BSM_EXACT) {
				return middleIndex;
			}
			borderLeft = borderRight = middleIndex;
			break;
		}
		if (currentValue < value) {
			borderLeft = middleIndex + 1;
			continue;
		}
		borderRight = middleIndex - 1;
	}
	if (mode == BSM_EXACT) {
		return (uint32_t)-1;
	}
	if (array[borderLeft] > value) {
		if (borderLeft == 0) {
			return (uint32_t)-1;
		}
		return borderLeft - 1;
	}
	return borderLeft;
}

const uint32_t unicodeGetBlockIndex(const uint32_t character, /* optional, out */ uint8_t* const ignored) {
	const uint32_t blockIndex = binarySearch(character, unicodeBlocks, unicodeBlocksCount, BSM_FLOOR);
	if (ignored) {
		if (character >= unicodeBlocks[unicodeBlocksCount - 1] || character < unicodeBlocks[0]) {
			*ignored = 1;
		}
		else {
			*ignored = unicodeBlockIsIgnored(blockIndex);
		}
	}
	return blockIndex;
}
const uint8_t unicodeBlockIsIgnored(const uint32_t blockId) {
	if (blockId >= unicodeBlocksIgnoreCount) {
		return 1;
	}
	return binarySearch(blockId, unicodeBlocksIgnore, unicodeBlocksIgnoreCount, BSM_EXACT) != (uint32_t)-1;
}

