#include "Unknown/File_0x800a64e0.h"
#include "Dolphin/os.h"

typedef struct
{
    OSThread t ATTRIBUTE_ALIGN(16);       // 0x000
    OSSemaphore s[2] ATTRIBUTE_ALIGN(16); // 0x320

    int BytesToRead;          // 0x338
    size_t CachedBytesToRead; // 0x33C
    int volatile ReadOffset;  // 0x340
    u32 bitBuffer;            // 0x344
    u8 * volatile DataWriteBottom;      // 0x348
    u8 *DataReadBottom;       // 0x34C
    u8 *CachedDataReadBottom; // 0x350
    void *DataReadTop;        // 0x354
    u8 * volatile DataWriteTop;         // 0x358
    int _35C;                 // 0x35C

    s8 volatile segmentCount; // 0x360
    u8 BitsInBuffer;          // 0x361
    u8 LookBackSize;          // 0x362
    u8 RepetitionSize;        // 0x363
    u8 error;                 // 0x364
    s8 compressedFlag;        // 0x365
    u8 ShouldWriteDataFlag;   // 0x366
} DataVars;

DataVars gDataDecompressorValues;

#define LZSS_FLAG_SIZE 1
#define LZSS_BYTE_SIZE 8
#define LZSS_MINIMUM_REPETITION_SIZE 2

/*================================================================================*/

#define STRUCT_OFFSET(structName, memberField) \
    (size_t)(&((structName *)NULL)->memberField)

/*================================================================================*/

void SetReadParameters(AssetLoadInstructions *instructions, void *pCompressedData, void *pOutData, BOOL readInSegments)
{
    uint segmentSize;

    gDataDecompressorValues.RepetitionSize = instructions->compressionConstants >> 8;
    gDataDecompressorValues.LookBackSize = instructions->compressionConstants & 0xff;
    gDataDecompressorValues.CachedBytesToRead = gDataDecompressorValues.BytesToRead = instructions->originalDiskSize;

    if (!readInSegments)
    {
        segmentSize = ALIGN_NEXT(instructions->compressedSize, 32);
        gDataDecompressorValues.segmentCount = -1;
    }
    else
    {
        segmentSize = 0x20000;
        gDataDecompressorValues.segmentCount = 0;
    }

    // Set the top of the and bottom of the write to the same output
    gDataDecompressorValues.DataWriteBottom = pOutData;
    gDataDecompressorValues.DataWriteTop =  pOutData;
    gDataDecompressorValues.DataReadBottom = gDataDecompressorValues.CachedDataReadBottom = pCompressedData;

    gDataDecompressorValues.DataReadTop = (u8 *)pCompressedData + segmentSize;
    gDataDecompressorValues.error = 0;
    gDataDecompressorValues.ReadOffset = 0;
    gDataDecompressorValues.BitsInBuffer = 0;
    gDataDecompressorValues.bitBuffer = 0;
    gDataDecompressorValues.compressedFlag = instructions->compressionFlag;
    gDataDecompressorValues.ShouldWriteDataFlag = TRUE;
    OSResumeThread(&gDataDecompressorValues.t);
    return;
}

/*================================================================================*/

extern u8 myStack[2][0x1000];

void StartThreadForReadingFromDisk()
{
    OSCreateThread(&gDataDecompressorValues.t, ReadDataFromDisk, NULL, myStack[1], sizeof(myStack[1]), 0x1f, 1);
    OSResumeThread(&gDataDecompressorValues.t);
}

/*================================================================================*/

void *ReadDataFromDisk(void *)
{
    OSInitSemaphore(gDataDecompressorValues.s, 3);
    while (1)
    {
        gDataDecompressorValues.ShouldWriteDataFlag = FALSE;
        OSSuspendThread(&gDataDecompressorValues.t);

        switch (gDataDecompressorValues.compressedFlag - 1)
        {
        case 0:
            DecompressDiskData();
            break;
        }
    }
}

/*================================================================================*/

#pragma opt_unroll_factor_limit 
void DecompressDiskData(void)
{
    u32 dataFlag;
    u32 register repetition;
    u32 lookBack;
    u32 i;
    u8 *tP;
    DataVars *v = &gDataDecompressorValues;
    u32 UNUSED_VALUE = 0;

    while (TRUE)
    {
        // returns 0 or 1
        dataFlag = ReadBits(LZSS_FLAG_SIZE);
        // if 1, it's original data
        if (dataFlag)
        {
            // read new byte
            *v->DataWriteTop++ = ReadBits(LZSS_BYTE_SIZE);
            v->BytesToRead--;
        }
        // else it's repeated data
        else
        {

            // how far back the repeated data starts
            // `+ 1` because a lookback of 0 means we want to read the data at the top of the DataWrite, which is DataWriteTop[-1]
            lookBack = ReadBits(v->LookBackSize) + 1;
            // how long the string of repeated data is
            repetition = ReadBits(v->RepetitionSize);
            repetition += LZSS_MINIMUM_REPETITION_SIZE;

            v->BytesToRead -= repetition;

            // someone forgot a |?
            // If the repetition algorithm requests data that is before the beginning of the array or requests more bytes than the original size, flag it
            if (v->BytesToRead < 0 | &v->DataWriteTop[-lookBack] < v->DataWriteBottom)
                break;

            // assume repetition is always > 0
            // while repetition != 0, copy the byte that was `lookback` bytes ago
            // ERROR: should load pointer instead or mr-ing previous load
            tP = v->DataWriteTop;
            // ERROR: should be mtctr, but not unroll
            asm {
                mtctr repetition
                loop:
            };
            *tP++ = tP[-lookBack];
            asm {
                bdnz loop
                li r30, 0
                li r29, 0
                li r28, 0
            };

            // update array pointer
            v->DataWriteTop = tP;
        }

        // if we read all the bytes we wanted, return
        if (!v->BytesToRead)
            return;
    }

    v->error = 1;
}

/*================================================================================*/

u32 ReadBits(u32 bits)
{
    u32 bitsNeededToRead;
    int interrupts;
    u8 bitsForNextBuffer;
    u32 toReturn;

    if (gDataDecompressorValues.BitsInBuffer >= bits)
    {
        // we have enough bits in the buffer, just cycle them out
        gDataDecompressorValues.BitsInBuffer -= bits;
        // push out the top bits, and then move back to just hold the bottom bits, worse than a mask? maybe faster for cpu
        toReturn = gDataDecompressorValues.bitBuffer << (32 - bits) >> (32 - bits);
        gDataDecompressorValues.bitBuffer >>= bits;
    }
    else
    {
        // not enough bits, figure out how many we need
        bitsNeededToRead = bits - gDataDecompressorValues.BitsInBuffer;
        // the bits left in our buffer will be the top bits of our value
        toReturn = gDataDecompressorValues.bitBuffer << bitsNeededToRead;

        // make sure we have valid data?
        while (!gDataDecompressorValues.ReadOffset && !gDataDecompressorValues.segmentCount)
        {
            // nop
        }

        // read a new u32 into our buffer
        gDataDecompressorValues.bitBuffer = *(u32 *)&gDataDecompressorValues.DataReadBottom[gDataDecompressorValues.ReadOffset];
        // how many bits our next buffer will hold after we remove the rest of the current data
        bitsForNextBuffer = 32 - bitsNeededToRead;
        // push out most of the new bits, and be left with bottom bits of the buffer
        // `or` it into the result
        toReturn |= (gDataDecompressorValues.bitBuffer << bitsForNextBuffer) >> bitsForNextBuffer;

        // update the bit variables
        gDataDecompressorValues.BitsInBuffer = bitsForNextBuffer;
        // cycle out the used bits
        gDataDecompressorValues.bitBuffer >>= bitsNeededToRead;
        // update the read offset
        gDataDecompressorValues.ReadOffset += sizeof(u32);

        // if we've read a full 32Kb
        if (gDataDecompressorValues.ReadOffset == 0x8000)
        {
            // update the read offset by moving the bottom pointer
            gDataDecompressorValues.ReadOffset = 0;
            gDataDecompressorValues.DataReadBottom += 0x8000;
            // If we've read too far, reset the pointer to the bottom of the array
            if (gDataDecompressorValues.DataReadBottom == gDataDecompressorValues.DataReadTop)
            {
                gDataDecompressorValues.DataReadBottom = gDataDecompressorValues.CachedDataReadBottom;
            }
            // signal a thread to update the cache of data to read
            interrupts = OSDisableInterrupts();
            gDataDecompressorValues.segmentCount--;
            OSSignalSemaphore(&gDataDecompressorValues.s);
            OSRestoreInterrupts(interrupts);
        }
    }
    return toReturn;
}

/*================================================================================*/
