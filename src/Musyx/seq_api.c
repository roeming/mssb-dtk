#include <musyx/seq.h>

BOOL sndSeqGetValid(s32 unk)
{
    s32 id = seqGetPrivateId(unk);
    return ((-1 - id) | (id + 1)) < 0;
}

void sndSeqStop(s32 unk)
{
    hwDisableIrq();
    seqStop(unk);
    hwEnableIrq();
}

BOOL sndSeqLoop(s32 unk, BOOL b)
{
    s32 id = seqGetPrivateId(unk);
    if (id != 0xffffffff && !(id & 0x80000000))
    {
        int i;
        for(i = 0; i < 16; i++)
        {
            seqInstance[id].section[i].loopDisable = !b;
        }
        return TRUE;
    }
    return FALSE;
}

void sndSeqSpeed(u32 unk1, u16 unk2)
{
    hwDisableIrq();
    seqSpeed(unk1, unk2);
    hwEnableIrq();
}

void sndSeqContinue(s32 unk)
{
    hwDisableIrq();
    seqContinue(unk);
    hwEnableIrq();
}

void sndSeqMute(s32 unk1, s32 unk2, s32 unk3)
{
    hwDisableIrq();
    seqMute(unk1, unk2, unk3);
    hwEnableIrq();
}

void sndSeqVolume(u8 unk1, u16 unk2, u32 unk3, u8 unk4)
{
    hwDisableIrq();
    seqVolume(unk1, unk2, unk3, unk4);
    hwEnableIrq();
}

u16 seqGetMIDIPriority(u8 set, u8 channel)
{
    return seqMIDIPriority[set][channel];
}

s32 seqGetInstancesForVoice(u32 id)
{
    int i;
    for (i = 0; i < 8; i++)
    {
        SEQ_INSTANCE *seq;

        for (seq = seqActiveRoot; seq; seq = seq->next)
        {
            int ii;
            NOTE *note;

            for (ii = 0; ii < 2; ii++)
            {
                for (note = seq->noteUsed[ii]; note; note = note->next)
                {
                    if (note->id == id)
                    {
                        return seq->publicId;
                    }
                }
            }
            
            for (note = seq->noteKeyOff; note; note = note->next)
            {
                if (note->id == id)
                {
                    return seq->publicId;
                }
            }
        }
    }
    return -1;
}