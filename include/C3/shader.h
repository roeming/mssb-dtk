#ifndef _DOLPHIN_CP_SHADER_H_
#define _DOLPHIN_CP_SHADER_H_

enum SHADER_TG_SRC {
    SHADER_TG_POS = 0,
    SHADER_TG_NRM = 1,
    SHADER_TG_BINRM = 2,
    SHADER_TG_TANGENT = 3,
    SHADER_TG_TEX0 = 4,
    SHADER_TG_TEX1 = 5,
    SHADER_TG_TEX2 = 6,
    SHADER_TG_TEX3 = 7,
    SHADER_TG_TEX4 = 8,
    SHADER_TG_TEX5 = 9,
    SHADER_TG_TEX6 = 10,
    SHADER_TG_TEX7 = 11,
    SHADER_TG_COLOR0 = 19,
    SHADER_TG_COLOR1 = 20,
};

enum SHADER_TG_TYPE {
    SHADER_TG_MTX3x4 = 0,
    SHADER_TG_MTX2x4 = 1,
    SHADER_TG_BUMP0 = 2,
    SHADER_TG_BUMP1 = 3,
    SHADER_TG_BUMP2 = 4,
    SHADER_TG_BUMP3 = 5,
    SHADER_TG_BUMP4 = 6,
    SHADER_TG_BUMP5 = 7,
    SHADER_TG_BUMP6 = 8,
    SHADER_TG_BUMP7 = 9,
    SHADER_TG_SRTG = 10,
};

enum SHADER_MTX {
    SHADER_MTX0 = 0,
    SHADER_MTX1 = 1,
    SHADER_MTX2 = 2,
    SHADER_MTX3 = 3,
    SHADER_MTX4 = 4,
    SHADER_MTX5 = 5,
    SHADER_MTX6 = 6,
    SHADER_MTX7 = 7,
    SHADER_IDENTITY = 8,
};

struct SHDRTexCoord {
    /* 0x00 */ s16 referenceCount;
    /* 0x04 */ enum SHADER_TG_SRC genSrc;
    /* 0x08 */ struct SHDRTexCoord *shaderSrc;
    /* 0x0C */ enum SHADER_TG_TYPE type;
    /* 0x10 */ enum SHADER_MTX mtxInput;
    /* 0x14 */ u8 texCoordExpIdx;
};

enum SHADER_TYPE {
    SHADER_TYPE_TEXTURE = 0,
    SHADER_TYPE_COLOR = 1,
    SHADER_TYPE_RASTERIZED = 2,
    SHADER_TYPE_CONSTANT = 3,
    SHADER_TYPE_COMPLEXINPUT = 4,
    SHADER_TYPE_COMPLEX = 5,
    SHADER_TYPE_EMPTY = 6,
};

enum SHADER_CHANNEL {
    SHADER_CHANNEL_RGB = 0,
    SHADER_CHANNEL_RRR = 1,
    SHADER_CHANNEL_GGG = 2,
    SHADER_CHANNEL_BBB = 3,
    SHADER_CHANNEL_A = 4,
    SHADER_CHANNEL_TRIVIAL = 5,
};

enum SHADER_COLOR_TYPE {
    SHADER_CPREV = 0,
    SHADER_C0 = 1,
    SHADER_C1 = 2,
    SHADER_C2 = 3,
    SHADER_APREV = 4,
    SHADER_A0 = 5,
    SHADER_A1 = 6,
    SHADER_A2 = 7,
    SHADER_TEXC = 8,
    SHADER_TEXA = 9,
    SHADER_RASC = 10,
    SHADER_RASA = 11,
    SHADER_TEXRRR = 12,
    SHADER_TEXGGG = 13,
    SHADER_TEXBBB = 14,
    SHADER_HALF = 15,
    SHADER_QUARTER = 16,
    SHADER_COMPLEXINPUT0_RGB = 25,
    SHADER_COMPLEXINPUT1_RGB = 26,
    SHADER_COMPLEXINPUT2_RGB = 27,
    SHADER_COMPLEXINPUT3_RGB = 28,
    SHADER_COMPLEXINPUT4_RGB = 29,
    SHADER_COMPLEXINPUT5_RGB = 30,
    SHADER_COMPLEXINPUT6_RGB = 31,
    SHADER_COMPLEXINPUT7_RGB = 32,
    SHADER_COMPLEXINPUT0_A = 35,
    SHADER_COMPLEXINPUT1_A = 36,
    SHADER_COMPLEXINPUT2_A = 37,
    SHADER_COMPLEXINPUT3_A = 38,
    SHADER_COMPLEXINPUT4_A = 39,
    SHADER_COMPLEXINPUT5_A = 40,
    SHADER_COMPLEXINPUT6_A = 41,
    SHADER_COMPLEXINPUT7_A = 42,
    SHADER_COLORINPUT0_RGB = 50,
    SHADER_COLORINPUT1_RGB = 51,
    SHADER_COLORINPUT2_RGB = 52,
    SHADER_COLORINPUT3_RGB = 53,
    SHADER_COLORINPUT0_A = 60,
    SHADER_COLORINPUT1_A = 61,
    SHADER_COLORINPUT2_A = 62,
    SHADER_COLORINPUT3_A = 63,
    SHADER_NONTRIVIAL = 100,
    SHADER_ONE = 200,
    SHADER_ZERO = 201,
};

enum SHADER_TEX {
    SHADER_TEX0 = 0,
    SHADER_TEX1 = 1,
    SHADER_TEX2 = 2,
    SHADER_TEX3 = 3,
    SHADER_TEX4 = 4,
    SHADER_TEX5 = 5,
    SHADER_TEX6 = 6,
    SHADER_TEX7 = 7,
    SHADER_TEXNONE = 8,
};

enum SHADER_COLOR {
    SHADER_COLOR0 = 0,
    SHADER_COLOR1 = 1,
    SHADER_COLOR2 = 2,
    SHADER_COLOR3 = 3,
    SHADER_COLOR_NONE = 4,
};

enum SHADER_RAS {
    SHADER_RAS0 = 0,
    SHADER_RAS1 = 1,
    SHADER_RASNONE = 2,
};

enum SHADER_OP {
    SHADER_OP_ADD = 0,
    SHADER_OP_SUB = 1,
};

enum SHADER_CLAMP {
    SHADER_CLAMP_LINEAR_1023 = 0,
    SHADER_CLAMP_LINEAR_255 = 1,
    SHADER_CLAMP_GE_255 = 2,
    SHADER_CLAMP_GE_0 = 3,
    SHADER_CLAMP_EQ_255 = 4,
    SHADER_CLAMP_EQ_0 = 5,
    SHADER_CLAMP_LE_255 = 6,
    SHADER_CLAMP_LE_0 = 7,
};

enum SHADER_BIAS {
    SHADER_BIAS_ZERO = 0,
    SHADER_BIAS_ADDHALF = 1,
    SHADER_BIAS_SUBHALF = 2,
};

enum SHADER_SCALE {
    SHADER_SCALE_1 = 0,
    SHADER_SCALE_2 = 1,
    SHADER_SCALE_4 = 2,
    SHADER_SCALE_DIVIDE_2 = 3,
};

enum SHADER_COMPLEX {
    SHADER_COMPLEX0 = 0,
    SHADER_COMPLEX1 = 1,
    SHADER_COMPLEX2 = 2,
    SHADER_COMPLEX3 = 3,
    SHADER_COMPLEX4 = 4,
    SHADER_COMPLEX5 = 5,
    SHADER_COMPLEX6 = 6,
    SHADER_COMPLEX7 = 7,
};

struct SHDRShader {
    /* 0x00 */ enum SHADER_TYPE type;
    /* 0x04 */ enum SHADER_CHANNEL channel;
    /* 0x08 */ s16 referenceCount;
    /* 0x0C */ void *TEVStage;
    union {
        struct {
            /* 0x00 */ enum SHADER_COLOR_TYPE arg;
        } constantShader;
        struct {
            /* 0x00 */ enum SHADER_TEX tex;
            /* 0x04 */ struct SHDRTexCoord *texCoordShader;
        } textureShader;
        struct {
            /* 0x00 */ enum SHADER_COLOR color;
        } colorShader;
        struct {
            /* 0x00 */ enum SHADER_RAS rasColor;
        } rasterizedShader;
        struct {
            /* 0x00 */ struct SHDRShader * input1;
            /* 0x04 */ struct SHDRShader * input2;
            /* 0x08 */ struct SHDRShader * input3;
            /* 0x0C */ struct SHDRShader * input4;
            /* 0x10 */ enum SHADER_OP op;
            /* 0x14 */ enum SHADER_CLAMP clamp;
            /* 0x18 */ enum SHADER_BIAS bias;
            /* 0x1C */ enum SHADER_SCALE scale;
        } complexShader;
        struct {
            /* 0x00 */ enum SHADER_COMPLEX input;
        } complexInputShader;
    } shaderParams;
};

struct TexCoordExp {
    /* 0x00 */ enum SHADER_TG_SRC genSrc;
    /* 0x04 */ u8 srcShaderIdx;
    /* 0x08 */ enum SHADER_TG_TYPE type;
    /* 0x0C */ enum SHADER_MTX mtxInput;
};

typedef struct {
    /* 0x00 */ struct TexCoordExp * expressionArray;
    /* 0x04 */ u8 numExpressions;
    /* 0x08 */ void * mtxArray[8];
    /* 0x28 */ u8 mtxUsed[8];
} SHDRExp;

typedef struct {
    /* 0x00 */ GXTexCoordID coord[8];
    /* 0x20 */ struct SHDRTexCoord *tcShader[8];
    /* 0x40 */ struct SHDRShader *textureShader[8];
} SHDRTCResources;

typedef struct {
    /* 0x00 */ void * colorStages;
    /* 0x04 */ void * alphaStages;
    /* 0x08 */ u8 numStages;
    /* 0x0C */ void * shaderResources;
    /* 0x10 */ void * texGen;
} SHDRInfo;

typedef struct {
    /* 0x00 */ enum SHADER_COLOR_TYPE TEVArg[4];
    /* 0x10 */ GXTevOp op;
    /* 0x14 */ GXTevBias bias;
    /* 0x18 */ GXTevScale scale;
    /* 0x1C */ u8 clamp;
    /* 0x20 */ GXTevClampMode mode;
    /* 0x24 */ enum SHADER_COLOR_TYPE out_reg;
    /* 0x28 */ enum SHADER_RAS rasInput;
    /* 0x2C */ u8 texGenIdx;
    /* 0x30 */ enum SHADER_TEX texInput;
} SHDRStage;

typedef struct {
    /* 0x00 */ enum SHADER_COLOR regColor[4];
    /* 0x10 */ enum SHADER_COLOR regAlpha[4];
    /* 0x20 */ u8 colorUsed[4];
    /* 0x24 */ GXColor colorData[4];
    /* 0x34 */ u8 textureUsed[8];
    /* 0x3C */ GXTexObj * textureData[8];
    /* 0x5C */ u8 rasUsed[2];
    /* 0x60 */ GXChannelID rasData[2];
    /* 0x68 */ u8 complexUsed[8];
    /* 0x70 */ SHDRInfo * complexData[8];
} SHDRRas;

enum SHADER_REG {
    SHADER_REG_EMPTY = 0,
    SHADER_REG_CONSTANTCOLOR = 1,
    SHADER_REG_INUSE = 2,
};

typedef struct {
    /* 0x00 */ enum SHADER_REG regColorState[4];
    /* 0x10 */ enum SHADER_COLOR regColor[4];
    /* 0x20 */ enum SHADER_COLOR_TYPE colorIdList[4];
    /* 0x30 */ struct SHDRShader * colorSrc[4];
    /* 0x40 */ u8 colorLifetime[4];
    /* 0x44 */ enum SHADER_REG regAlphaState[4];
    /* 0x54 */ enum SHADER_COLOR regAlpha[4];
    /* 0x64 */ enum SHADER_COLOR_TYPE alphaIdList[4];
    /* 0x74 */ struct SHDRShader * alphaSrc[4];
    /* 0x84 */ u8 alphaLifetime[4];
    /* 0x88 */ GXTexObj * texObj[8];
    /* 0xA8 */ GXTexMapID mapIdList[8];
} SHDRResources;

enum SHADER_TEV {
    SHADER_TEV_COLOR = 0,
    SHADER_TEV_ALPHA = 1,    
};

enum SHADER_LERP_TYPE {
    SHADER_TRIVIAL = 0,
    SHADER_SIMPLE = 1,
    SHADER_LERP = 2,
};

struct ShaderTEVStage {
    /* 0x00 */ enum SHADER_TEV channel;
    /* 0x04 */ enum SHADER_TYPE type;
    /* 0x08 */ struct ShaderTEVStage * parent[8];
    /* 0x28 */ u8 numParents;
    /* 0x29 */ u8 numNonAllocatedChildren;
    /* 0x2C */ enum SHADER_COLOR_TYPE TEVArg[4];
    /* 0x3C */ struct ShaderTEVStage * tevStage[4];
    /* 0x4C */ GXTevOp op;
    /* 0x50 */ GXTevBias bias;
    /* 0x54 */ GXTevScale scale;
    /* 0x58 */ u8 clamp;
    /* 0x5C */ GXTevClampMode mode;
    /* 0x60 */ enum SHADER_COLOR_TYPE out_reg;
    /* 0x64 */ enum SHADER_RAS rasInput;
    /* 0x68 */ u8 texGenIdx;
    /* 0x6C */ enum SHADER_TEX texInput;
    /* 0x70 */ enum SHADER_LERP_TYPE LERPType;
    /* 0x74 */ u8 outputIndex;
    /* 0x78 */ enum SHADER_COLOR_TYPE outputArg;
};

// shader.c
extern SHDRInfo *SHDRPassThruShader;
extern SHDRInfo *SHDRReplaceShader;
extern SHDRInfo *SHDRModulateShader;
extern struct SHDRShader * ShaderOne;
extern struct SHDRShader * ShaderZero;
extern struct SHDRShader * ShaderHalf;
extern struct SHDRShader * ShaderQuarter;

struct SHDRShader *SHDRCreateTexture(enum SHADER_TEX tex, struct SHDRTexCoord * texCoordShader, enum SHADER_CHANNEL channel);
struct SHDRShader *SHDRCreateRasterized(enum SHADER_RAS rasColor, enum SHADER_CHANNEL channel);
struct SHDRShader *SHDRCreateColor(enum SHADER_COLOR color, enum SHADER_CHANNEL channel);
struct SHDRShader *SHDRCreateComplexInput(enum SHADER_COMPLEX input, enum SHADER_CHANNEL channel);
struct SHDRShader *SHDRCreateComplex(struct SHDRShader * input1, struct SHDRShader * input2, struct SHDRShader * input3, struct SHDRShader * input4, enum SHADER_OP op, enum SHADER_CLAMP clamp, enum SHADER_BIAS bias, enum SHADER_SCALE scale, enum SHADER_CHANNEL channel);
void SHDRFree(struct SHDRShader * shader);
SHDRInfo *SHDRCompile(struct SHDRShader * rgbShader, struct SHDRShader * aShader);
void SHDRExecute(SHDRInfo *shader);
void SHDRBindTexture(SHDRInfo *shader, enum SHADER_TEX tex, GXTexObj *texObj);
void SHDRBindRasterized(SHDRInfo *shader, enum SHADER_RAS rasColor, GXChannelID channel);
void SHDRBindColor(SHDRInfo *shader, enum SHADER_COLOR colorInput, GXColor color);
void SHDRBindComplexInput(SHDRInfo *shader, enum SHADER_COMPLEX input, SHDRInfo *inputShader);
void SHDRBindTexGenMtx(SHDRInfo *shader, enum SHADER_MTX input, MtxPtr mtxData);

// shaderBuildTree.c
extern struct SHDRShader * RGBRoot;

int BuildTEVTree(struct SHDRShader * rgbShader, struct SHDRShader * aShader, void * compiledTexGen);

// shaderExecute.c
extern void * MtxDataArray[8];
extern struct TexCoordExp TexExpPool[8];
extern SHDRRas CurrentShaderResources;
extern enum SHADER_REG CurrentColorState[4];
extern enum SHADER_REG CurrentAlphaState[4];
extern SHDRStage CompiledTEVColorPool[16];
extern SHDRStage CompiledTEVAlphaPool[16];

extern u8 CompiledTEVCounter;
extern u8 CurrentTexExp;
extern u8 MtxUsed[8];

void CheckShaderBindings(SHDRInfo *shader);
void CombineTEVStages(SHDRInfo *shader);

// shaderFlattenTree.c
void FlattenTEVTree(void);
void CopyCompiledTEVStages(SHDRStage *compiled, struct ShaderTEVStage * instructions, unsigned long numStages, SHDRRas *resources);

// shaderPrint.c
void PrintTEVTree(SHDRInfo *shader);

// shaderPruneTree.c
extern struct ShaderTEVStage CollapsedTEVPool[33];
extern u8 OptimalTreeSize;

void PruneTEVTree(void);

// shaderResManage.c
extern SHDRResources Resources;

extern u32 TEVCounter;
extern u8 BestNumTEVStages;

extern struct ShaderTEVStage * AlphaInstructions[16];
extern struct ShaderTEVStage * ColorInstructions[16];
extern struct ShaderTEVStage TEVPool[50];
extern struct ShaderTEVStage BestAlphaInstructions[16];
extern struct ShaderTEVStage BestColorInstructions[16];

void ResetShaderResources(void);
enum SHADER_COLOR_TYPE AllocateColorResource(enum SHADER_COLOR color);
enum SHADER_COLOR_TYPE AllocateAlphaResource(enum SHADER_COLOR color);
GXTexMapID AllocateTextureResource(GXTexObj *tex);
u8 CheckResourceCollision(struct ShaderTEVStage * stage, struct ShaderTEVStage * parent);
u8 CheckResourceAllocation(void);
void InitCompiledResourceInfo(SHDRRas *res);

// shaderSetTEV.c
void SetTEV(SHDRInfo *shader);

// shaderTCInternals.c
extern SHDRTCResources TCResources;
extern u32 MtxIDArray[8];

void CompileTexGen(struct SHDRShader *shader, SHDRExp **exp);
void SetTCGenState(SHDRInfo *shader);

// shaderTexCoord.c
struct SHDRTexCoord * SHDRCreateTexCoordExpression(enum SHADER_TG_SRC src, struct SHDRTexCoord *shadSrc, enum SHADER_TG_TYPE texGenType, enum SHADER_MTX mtxInput);
struct SHDRTexCoord * SHDRCreateTCPassThrough(enum SHADER_TG_SRC src);
void SHDRFreeTC(struct SHDRTexCoord * texCoord);

// unsorted externs
extern struct SHDRShader *ShaderZero;

#endif // _DOLPHIN_CP_SHADER_H_
