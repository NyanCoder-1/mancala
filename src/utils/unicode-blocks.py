import re

# data
blocks = [0,]
blocksToIgnore = []

# regex
rexp = re.compile(r'(U\+([0-9A-Fa-f]+)) - (U\+([0-9A-Fa-f]+))\t')

# parse file
with open('unicode-blocks.txt', 'r') as inputFile:
    index = 0
    prevBlockEnd = 0
    for line in inputFile:
        m = rexp.match(line)
        if m:
            blockStart = int(m.group(2), 16)
            blockEnd = int(m.group(4), 16) + 1
            if blockStart != prevBlockEnd:
                blocksToIgnore.append(len(blocks) - 1)
                blocks.append(blockStart)
            blocks.append(blockEnd)
            prevBlockEnd = blockEnd
        index += 1
    blocksToIgnore.append(len(blocks) - 1)

# format data for C code
with open('unicode_blocks.arrays.c', 'w') as outputFile:
    rexpPattern = r'(([^,]+, ){7}[^,]+), '
    rexpReplacement = r'\1,\n    '
    outputFile.write('#include <stdint.h>\n\nconst uint32_t unicodeBlocksCount = ' + str(len(blocks)) + ';\nconst uint32_t unicodeBlocks[] = {\n    ' + re.sub(rexpPattern, rexpReplacement, ', '.join('0x' + hex(block)[2:].upper().zfill(6) for block in blocks)) + '\n};\nconst uint32_t unicodeBlocksIgnoreCount = ' + str(len(blocksToIgnore)) + ';\nconst uint32_t unicodeBlocksIgnore[] = {' + ', '.join(str(block) for block in blocksToIgnore) + '};\n')

