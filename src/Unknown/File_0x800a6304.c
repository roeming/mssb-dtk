#include "Unknown/File_0x800a6304.h"

typedef struct
{
    u32 _00; // 00
    u32 _04; // 04
} lbl_803C6780_inner_struct;

typedef struct
{
    u32 tempValue;  // 000
    u32 count1;     // 004
    u32 count2;     // 008
    lbl_803C6780_inner_struct values1[0x20]; // 00C
    lbl_803C6780_inner_struct values2[0x20]; // 10C
} lbl_803C6780_struct;

static lbl_803C6780_struct data;

void fn_800A6304(void)
{
    int iVar1;

    if (data.count2 != 0)
    {
        data.tempValue += data.values2[data.count2]._04;
        data.values2[data.count2--]._04 = data.values2[data.count2]._00 = 0;
    }
}

u32 fn_800A6354(u32 param_1)
{
    u32 new_val;
    
    param_1 = ALIGN_NEXT(param_1, 32);

    new_val = data.values2[data.count2]._00 - param_1;
    
    data.count2++;

    data.values2[data.count2]._00 = new_val;
    data.values2[data.count2]._04 = param_1;

    data.tempValue = new_val - (data.values1[data.count1]._00 + data.values1[data.count1]._04);

    return data.values2[data.count2]._00;
}

void fn_800A63C8(void)
{
    if (data.count1 != 0)
    {
        data.tempValue += data.values1[data.count1]._04;

        data.values1[data.count1--]._04 = data.values1[data.count1]._00 = 0;
    }
}

u32 fn_800A6418(u32 param_1)
{
    u32 new_val;

    param_1 = ALIGN_NEXT(param_1, 32);
    new_val = data.values1[data.count1]._00 + data.values1[data.count1]._04;
    
    data.count1++;
    
    data.values1[data.count1]._00 = new_val;
    data.values1[data.count1]._04 = param_1;
    
    data.tempValue = data.values2[data.count2]._00 - (new_val + param_1);

    return data.values1[data.count1]._00;
}

void fn_800A648C()
{
    memset(&data, 0, sizeof(data));
    data.values1[0]._00 = 0x00804000;
    data.values2[0]._00 = 0x01000000;
    data.tempValue = 0x7fc000;
}