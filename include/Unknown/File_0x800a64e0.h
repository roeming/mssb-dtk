#include "types.h"

typedef struct 
{
    u32 compressionConstants;

    struct {
        u8 compressionFlag:2;
        u8 unused:2;
        u32 originalDiskSize:28;
    };
    u32 diskLocation;
    u32 compressedSize;
} AssetLoadInstructions;

u32 ReadBits(u32 bits);

void SetReadParameters(AssetLoadInstructions * instructions, void* pCompressedData, void*pOutData, BOOL param_4);

void *ReadDataFromDisk(void * /* unused */);

void DecompressDiskData();