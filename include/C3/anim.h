#ifndef _DOLPHIN_CP_ANIM_H_
#define _DOLPHIN_CP_ANIM_H_

typedef struct Control Control;
typedef struct KeyFrame KeyFrame;

struct ANIMAnimTrack {
    /* 0x00 */ f32 animTime;
    /* 0x04 */ struct KeyFrame * keyFrames;
    /* 0x08 */ u16 totalFrames;
    /* 0x0A */ u16 trackID;
    /* 0x0C */ u8 quantizeInfo;
    /* 0x0D */ u8 animType;
    /* 0x0E */ u8 interpolationType;
    /* 0x0F */ u8 replaceHierarchyCtrl;
};

struct ANIMPipe {
    /* 0x00 */ f32 time;
    /* 0x04 */ f32 speed;
    /* 0x08 */ struct ANIMAnimTrack * currentTrack;
    /* 0x0C */ struct Control * control;
    /* 0x10 */ u8 replaceHierarchyCtrl;
};

typedef struct ANIMSequences {
    /* 0x00 */ char *sequenceName;
    /* 0x04 */ struct ANIMAnimTrack *trackArray;
    /* 0x08 */ u16 totalTracks;
    /* 0x0A */ u16 pad16;
} ANIMSequences;

typedef struct ANIMBank {
    /* 0x00 */ u32 versionNumber;
    /* 0x04 */ ANIMSequences *animSequences;
    /* 0x08 */ u16 bankID;
    /* 0x0A */ u16 numSequences;
    /* 0x0C */ u16 numTracks;
    /* 0x0E */ u16 numKeyFrames;
    /* 0x10 */ u32 userDataSize;
    /* 0x14 */ void * userData;
} ANIMBank;

// animBank.c
void ANIMGet(ANIMBank **animBank, char *name);
void ANIMRelease(ANIMBank **animBank);
ANIMSequences *ANIMGetSequence(ANIMBank *animBank, char *sequenceName, u16 seqNum);
struct ANIMAnimTrack *ANIMGetTrackFromSeq(ANIMSequences *animSeq, u16 animTrackID);
void ANIMGetKeyFrameFromTrack(struct ANIMAnimTrack * animTrack, f32 time, KeyFrame **currentFrame, KeyFrame **nextFrame);
u32 ANIMGetUserDataSize(ANIMBank *animBank);
char *ANIMGetUserData(ANIMBank *animBank);

// animPipe.c
void ANIMBind(struct ANIMPipe *animPipe, Control *control, struct ANIMAnimTrack *animTrack, f32 time);
void ANIMSetTime(struct ANIMPipe * animPipe, f32 time);
void ANIMSetSpeed(struct ANIMPipe * animPipe, f32 speed);
void ANIMTick(struct ANIMPipe * animPipe);

#endif // _DOLPHIN_CP_ANIM_H_
