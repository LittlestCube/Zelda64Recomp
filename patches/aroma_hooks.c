#include "patches.h"
#include "ultra64.h"
#include "z64msgevent.h"

#include "apcommon.h"
#include "misc_funcs.h"

#define MADAME_AROMA_LIMB_MAX 0x1B

struct EnAl;

#define FLAGS (ACTOR_FLAG_TARGETABLE | ACTOR_FLAG_FRIENDLY | ACTOR_FLAG_10 | ACTOR_FLAG_20)

#define THIS ((EnAl*)thisx)

typedef void (*EnAlActionFunc)(struct EnAl*, PlayState*);
typedef void (*EnAlUnkFunc2)(struct EnAl*, PlayState*);

typedef struct EnAl {
    /* 0x000 */ Actor actor;
    /* 0x144 */ SkelAnime skelAnime;
    /* 0x188 */ EnAlActionFunc actionFunc;
    /* 0x18C */ EnAlUnkFunc2 unk_18C;
    /* 0x190 */ MtxF unk_190[6];
    /* 0x310 */ ColliderCylinder unk_310;
    /* 0x35C */ u8 scheduleResult;
    /* 0x360 */ MsgScript* msgScript;
    /* 0x364 */ s32 msgScriptPos;
    /* 0x368 */ Actor* unk_368;
    /* 0x36C */ Vec3f unk_36C;
    /* 0x378 */ Vec3s unk_378;
    /* 0x37E */ Vec3s jointTable[MADAME_AROMA_LIMB_MAX];
    /* 0x420 */ Vec3s morphTable[MADAME_AROMA_LIMB_MAX];
    /* 0x4C2 */ u16 unk_4C2;
    /* 0x4C4 */ u16 unk_4C4;
    /* 0x4C8 */ f32 animPlaySpeed;
    /* 0x4CC */ UNK_TYPE1 unk4CC[0x8];
    /* 0x4D4 */ f32 unk_4D4;
    /* 0x4D8 */ UNK_TYPE1 unk4D8[0x4];
    /* 0x4DC */ s16 unk_4DC;
    /* 0x4DE */ s16 unk_4DE;
    /* 0x4E0 */ s16 unk_4E0;
    /* 0x4E2 */ s16 unk_4E2;
    /* 0x4E4 */ s16 unk_4E4;
    /* 0x4E6 */ s16 unk_4E6;
    /* 0x4E8 */ s16 unk_4E8;
    /* 0x4EA */ s16 unk_4EA;
    /* 0x4EC */ MsgScriptCallback msgScriptCallback;
    /* 0x4F0 */ s32 unk_4F0;
    /* 0x4F4 */ s32 unk_4F4;
    /* 0x4F8 */ s32 animIndex;
    /* 0x4FC */ UNK_TYPE1 unk4FC[0x4];
} EnAl; // size = 0x500

typedef enum {
    /* -1 */ ENAL_ANIM_NONE = -1,
    /*  0 */ ENAL_ANIM_0,
    /*  1 */ ENAL_ANIM_1,
    /*  2 */ ENAL_ANIM_2,
    /*  3 */ ENAL_ANIM_3,
    /*  4 */ ENAL_ANIM_4,
    /*  5 */ ENAL_ANIM_5,
    /*  6 */ ENAL_ANIM_6,
    /*  7 */ ENAL_ANIM_7,
    /*  8 */ ENAL_ANIM_MAX
} EnAlAnimation;

MsgScript D_80BDFCBC[] = {
    /* 0x0000 0x03 */ MSCRIPT_CMD_CHECK_CALLBACK(0x0),
    /* 0x0003 0x01 */ MSCRIPT_CMD_FOCUS_TO_CHILD(),
    /* 0x0004 0x03 */ MSCRIPT_CMD_BEGIN_TEXT(0x27A5),
    /* 0x0007 0x01 */ MSCRIPT_CMD_AWAIT_TEXT(),
    /* 0x0008 0x03 */ MSCRIPT_CMD_CHECK_CALLBACK(0x0),
    /* 0x000B 0x01 */ MSCRIPT_CMD_FOCUS_TO_SELF(),
    /* 0x000C 0x03 */ MSCRIPT_CMD_BEGIN_TEXT(0x27A6),
    /* 0x000F 0x01 */ MSCRIPT_CMD_AWAIT_TEXT(),
    /* 0x0010 0x03 */ MSCRIPT_CMD_CHECK_CALLBACK(0x0),
    /* 0x0013 0x01 */ MSCRIPT_CMD_FOCUS_TO_CHILD(),
    /* 0x0014 0x03 */ MSCRIPT_CMD_BEGIN_TEXT(0x27A7),
    /* 0x0017 0x01 */ MSCRIPT_CMD_AWAIT_TEXT(),
    /* 0x0018 0x03 */ MSCRIPT_CMD_CHECK_CALLBACK(0x0),
    /* 0x001B 0x01 */ MSCRIPT_CMD_FOCUS_TO_SELF(),
    /* 0x001C 0x03 */ MSCRIPT_CMD_BEGIN_TEXT(0x27A8),
    /* 0x001F 0x01 */ MSCRIPT_CMD_AWAIT_TEXT(),
    /* 0x0020 0x03 */ MSCRIPT_CMD_CHECK_CALLBACK(0x0),
    /* 0x0023 0x01 */ MSCRIPT_CMD_FOCUS_TO_CHILD(),
    /* 0x0024 0x03 */ MSCRIPT_CMD_BEGIN_TEXT(0x27A9),
    /* 0x0027 0x01 */ MSCRIPT_CMD_AWAIT_TEXT(),
    /* 0x0028 0x03 */ MSCRIPT_CMD_CHECK_CALLBACK(0x0),
    /* 0x002B 0x01 */ MSCRIPT_CMD_FOCUS_TO_SELF(),
    /* 0x002C 0x03 */ MSCRIPT_CMD_BEGIN_TEXT(0x27AA),
    /* 0x002F 0x01 */ MSCRIPT_CMD_AWAIT_TEXT(),
    /* 0x0030 0x03 */ MSCRIPT_CMD_CHECK_CALLBACK(0x0),
    /* 0x0033 0x01 */ MSCRIPT_CMD_FOCUS_TO_CHILD(),
    /* 0x0034 0x03 */ MSCRIPT_CMD_BEGIN_TEXT(0x27AB),
    /* 0x0037 0x01 */ MSCRIPT_CMD_AWAIT_TEXT(),
    /* 0x0038 0x03 */ MSCRIPT_CMD_CHECK_CALLBACK(0x0),
    /* 0x003B 0x01 */ MSCRIPT_CMD_FOCUS_TO_SELF(),
    /* 0x003C 0x03 */ MSCRIPT_CMD_BEGIN_TEXT(0x27AC),
    /* 0x003F 0x01 */ MSCRIPT_CMD_AWAIT_TEXT(),
    /* 0x0040 0x03 */ MSCRIPT_CMD_CHECK_CALLBACK(0x0),
    /* 0x0043 0x01 */ MSCRIPT_CMD_FOCUS_TO_CHILD(),
    /* 0x0044 0x03 */ MSCRIPT_CMD_BEGIN_TEXT(0x27AD),
    /* 0x0047 0x03 */ MSCRIPT_CMD_NOTEBOOK_EVENT(BOMBERS_NOTEBOOK_EVENT_MET_POSTMAN),
    /* 0x004A 0x03 */ MSCRIPT_CMD_NOTEBOOK_EVENT(BOMBERS_NOTEBOOK_EVENT_MET_MADAME_AROMA),
    /* 0x004D 0x01 */ MSCRIPT_CMD_AWAIT_TEXT(),
    /* 0x004E 0x03 */ MSCRIPT_CMD_SET_WEEK_EVENT_REG(WEEKEVENTREG_89_08),
    /* 0x0051 0x03 */ MSCRIPT_CMD_CHECK_CALLBACK(0x0),
    /* 0x0054 0x01 */ MSCRIPT_CMD_DONE(),
};

MsgScript D_80BDFD14[] = {
    /* 0x0000 0x05 */ MSCRIPT_CMD_CHECK_WEEK_EVENT_REG(WEEKEVENTREG_86_20, 0x00A7 - 0x0005),
    /* 0x0005 0x03 */ MSCRIPT_CMD_CHECK_CALLBACK(0x0),
    /* 0x0008 0x01 */ MSCRIPT_CMD_FOCUS_TO_CHILD(),
    /* 0x0009 0x03 */ MSCRIPT_CMD_BEGIN_TEXT(0x2AA6),
    /* 0x000C 0x01 */ MSCRIPT_CMD_AWAIT_TEXT(),
    /* 0x000D 0x03 */ MSCRIPT_CMD_CHECK_CALLBACK(0x0),
    /* 0x0010 0x01 */ MSCRIPT_CMD_FOCUS_TO_SELF(),
    /* 0x0011 0x03 */ MSCRIPT_CMD_BEGIN_TEXT(0x2AA7),
    /* 0x0014 0x01 */ MSCRIPT_CMD_AWAIT_TEXT(),
    /* 0x0015 0x03 */ MSCRIPT_CMD_CHECK_CALLBACK(0x0),
    /* 0x0018 0x01 */ MSCRIPT_CMD_FOCUS_TO_CHILD(),
    /* 0x0019 0x03 */ MSCRIPT_CMD_BEGIN_TEXT(0x2AA8),
    /* 0x001C 0x01 */ MSCRIPT_CMD_AWAIT_TEXT(),
    /* 0x001D 0x03 */ MSCRIPT_CMD_CHECK_CALLBACK(0x0),
    /* 0x0020 0x01 */ MSCRIPT_CMD_FOCUS_TO_SELF(),
    /* 0x0021 0x03 */ MSCRIPT_CMD_BEGIN_TEXT(0x2AA9),
    /* 0x0024 0x01 */ MSCRIPT_CMD_AWAIT_TEXT(),
    /* 0x0025 0x03 */ MSCRIPT_CMD_CHECK_CALLBACK(0x0),
    /* 0x0028 0x01 */ MSCRIPT_CMD_FOCUS_TO_CHILD(),
    /* 0x0029 0x03 */ MSCRIPT_CMD_BEGIN_TEXT(0x2AAA),
    /* 0x002C 0x01 */ MSCRIPT_CMD_AWAIT_TEXT(),
    /* 0x002D 0x03 */ MSCRIPT_CMD_CHECK_CALLBACK(0x0),
    /* 0x0030 0x01 */ MSCRIPT_CMD_FOCUS_TO_SELF(),
    /* 0x0031 0x03 */ MSCRIPT_CMD_BEGIN_TEXT(0x2AAB),
    /* 0x0034 0x01 */ MSCRIPT_CMD_AWAIT_TEXT(),
    /* 0x0035 0x03 */ MSCRIPT_CMD_CHECK_CALLBACK(0x0),
    /* 0x0038 0x01 */ MSCRIPT_CMD_FOCUS_TO_CHILD(),
    /* 0x0039 0x03 */ MSCRIPT_CMD_BEGIN_TEXT(0x2AAC),
    /* 0x003C 0x01 */ MSCRIPT_CMD_AWAIT_TEXT(),
    /* 0x003D 0x03 */ MSCRIPT_CMD_CHECK_CALLBACK(0x0),
    /* 0x0040 0x01 */ MSCRIPT_CMD_FOCUS_TO_CHILD(),
    /* 0x0041 0x03 */ MSCRIPT_CMD_BEGIN_TEXT(0x2AAD),
    /* 0x0044 0x01 */ MSCRIPT_CMD_AWAIT_TEXT(),
    /* 0x0045 0x03 */ MSCRIPT_CMD_CONTINUE_TEXT(0x2AAE),
    /* 0x0048 0x01 */ MSCRIPT_CMD_AWAIT_TEXT(),
    /* 0x0049 0x03 */ MSCRIPT_CMD_CHECK_CALLBACK(0x0),
    /* 0x004C 0x01 */ MSCRIPT_CMD_FOCUS_TO_SELF(),
    /* 0x004D 0x03 */ MSCRIPT_CMD_BEGIN_TEXT(0x2AAF),
    /* 0x0050 0x01 */ MSCRIPT_CMD_AWAIT_TEXT(),
    /* 0x0051 0x03 */ MSCRIPT_CMD_CHECK_CALLBACK(0x0),
    /* 0x0054 0x01 */ MSCRIPT_CMD_FOCUS_TO_CHILD(),
    /* 0x0055 0x03 */ MSCRIPT_CMD_BEGIN_TEXT(0x2AB0),
    /* 0x0058 0x01 */ MSCRIPT_CMD_AWAIT_TEXT(),
    /* 0x0059 0x03 */ MSCRIPT_CMD_CHECK_CALLBACK(0x0),
    /* 0x005C 0x01 */ MSCRIPT_CMD_FOCUS_TO_CHILD(),
    /* 0x005D 0x03 */ MSCRIPT_CMD_BEGIN_TEXT(0x2AB1),
    /* 0x0060 0x01 */ MSCRIPT_CMD_AWAIT_TEXT(),
    /* 0x0061 0x03 */ MSCRIPT_CMD_CHECK_CALLBACK(0x0),
    /* 0x0064 0x01 */ MSCRIPT_CMD_FOCUS_TO_SELF(),
    /* 0x0065 0x03 */ MSCRIPT_CMD_BEGIN_TEXT(0x2AB2),
    /* 0x0068 0x01 */ MSCRIPT_CMD_AWAIT_TEXT(),
    /* 0x0069 0x03 */ MSCRIPT_CMD_CHECK_CALLBACK(0x0),
    /* 0x006C 0x01 */ MSCRIPT_CMD_FOCUS_TO_CHILD(),
    /* 0x006D 0x03 */ MSCRIPT_CMD_BEGIN_TEXT(0x2AB3),
    /* 0x0070 0x01 */ MSCRIPT_CMD_AWAIT_TEXT(),
    /* 0x0071 0x03 */ MSCRIPT_CMD_CHECK_CALLBACK(0x0),
    /* 0x0074 0x01 */ MSCRIPT_CMD_FOCUS_TO_SELF(),
    /* 0x0075 0x03 */ MSCRIPT_CMD_BEGIN_TEXT(0x2AB4),
    /* 0x0078 0x01 */ MSCRIPT_CMD_AWAIT_TEXT(),
    /* 0x0079 0x03 */ MSCRIPT_CMD_CHECK_CALLBACK(0x0),
    /* 0x007C 0x01 */ MSCRIPT_CMD_FOCUS_TO_CHILD(),
    /* 0x007D 0x03 */ MSCRIPT_CMD_BEGIN_TEXT(0x2AB5),
    /* 0x0080 0x01 */ MSCRIPT_CMD_AWAIT_TEXT(),
    /* 0x0081 0x03 */ MSCRIPT_CMD_CHECK_CALLBACK(0x0),
    /* 0x0084 0x01 */ MSCRIPT_CMD_FOCUS_TO_SELF(),
    /* 0x0085 0x03 */ MSCRIPT_CMD_BEGIN_TEXT(0x2AB6),
    /* 0x0088 0x01 */ MSCRIPT_CMD_AWAIT_TEXT(),
    /* 0x0089 0x03 */ MSCRIPT_CMD_CHECK_CALLBACK(0x0),
    /* 0x008C 0x01 */ MSCRIPT_CMD_FOCUS_TO_CHILD(),
    /* 0x008D 0x03 */ MSCRIPT_CMD_BEGIN_TEXT(0x2AB7),
    /* 0x0090 0x01 */ MSCRIPT_CMD_AWAIT_TEXT(),
    /* 0x0091 0x03 */ MSCRIPT_CMD_CHECK_CALLBACK(0x0),
    /* 0x0094 0x01 */ MSCRIPT_CMD_FOCUS_TO_SELF(),
    /* 0x0095 0x03 */ MSCRIPT_CMD_BEGIN_TEXT(0x2AB8),
    /* 0x0098 0x03 */ MSCRIPT_CMD_NOTEBOOK_EVENT(BOMBERS_NOTEBOOK_EVENT_MET_MADAME_AROMA),
    /* 0x009B 0x03 */ MSCRIPT_CMD_NOTEBOOK_EVENT(BOMBERS_NOTEBOOK_EVENT_MET_TOTO),
    /* 0x009E 0x03 */ MSCRIPT_CMD_NOTEBOOK_EVENT(BOMBERS_NOTEBOOK_EVENT_MET_GORMAN),
    /* 0x00A1 0x01 */ MSCRIPT_CMD_AWAIT_TEXT(),
    /* 0x00A2 0x03 */ MSCRIPT_CMD_SET_WEEK_EVENT_REG(WEEKEVENTREG_86_20),
    /* 0x00A5 0x01 */ MSCRIPT_CMD_CLOSE_TEXT(),
    /* 0x00A6 0x01 */ MSCRIPT_CMD_DONE(),

    /* 0x00A7 0x03 */ MSCRIPT_CMD_CHECK_CALLBACK(0x0),
    /* 0x00AA 0x01 */ MSCRIPT_CMD_FOCUS_TO_CHILD(),
    /* 0x00AB 0x03 */ MSCRIPT_CMD_BEGIN_TEXT(0x2AEA),
    /* 0x00AE 0x01 */ MSCRIPT_CMD_AWAIT_TEXT(),
    /* 0x00AF 0x03 */ MSCRIPT_CMD_CHECK_CALLBACK(0x0),
    /* 0x00B2 0x01 */ MSCRIPT_CMD_FOCUS_TO_SELF(),
    /* 0x00B3 0x03 */ MSCRIPT_CMD_BEGIN_TEXT(0x2AEB),
    /* 0x00B6 0x01 */ MSCRIPT_CMD_AWAIT_TEXT(),
    /* 0x00B7 0x03 */ MSCRIPT_CMD_CHECK_CALLBACK(0x0),
    /* 0x00BA 0x01 */ MSCRIPT_CMD_CLOSE_TEXT(),
    /* 0x00BB 0x01 */ MSCRIPT_CMD_DONE(),
};

MsgScript D_80BDFDD0[] = {
    /* 0x0000 0x05 */ MSCRIPT_CMD_CHECK_WEEK_EVENT_REG(WEEKEVENTREG_92_08, 0x0010 - 0x0005),
    /* 0x0005 0x03 */ MSCRIPT_CMD_BEGIN_TEXT(0x2AE5),
    /* 0x0008 0x03 */ MSCRIPT_CMD_NOTEBOOK_EVENT(BOMBERS_NOTEBOOK_EVENT_MET_MADAME_AROMA),
    /* 0x000B 0x03 */ MSCRIPT_CMD_SET_WEEK_EVENT_REG(WEEKEVENTREG_92_08),
    /* 0x000E 0x01 */ MSCRIPT_CMD_AWAIT_TEXT(),
    /* 0x000F 0x01 */ MSCRIPT_CMD_DONE(),

    /* 0x0010 0x03 */ MSCRIPT_CMD_BEGIN_TEXT(0x2AE6),
    /* 0x0013 0x03 */ MSCRIPT_CMD_NOTEBOOK_EVENT(BOMBERS_NOTEBOOK_EVENT_MET_MADAME_AROMA),
    /* 0x0016 0x01 */ MSCRIPT_CMD_AWAIT_TEXT(),
    /* 0x0017 0x01 */ MSCRIPT_CMD_DONE(),
};

MsgScript D_80BDFDE8[] = {
    /* 0x0000 0x03 */ MSCRIPT_CMD_CHECK_CALLBACK(0x0),
    /* 0x0003 0x03 */ MSCRIPT_CMD_BEGIN_TEXT(0x2B19),
    /* 0x0006 0x03 */ MSCRIPT_CMD_NOTEBOOK_EVENT(BOMBERS_NOTEBOOK_EVENT_MET_MADAME_AROMA),
    /* 0x0009 0x01 */ MSCRIPT_CMD_AWAIT_TEXT(),
    /* 0x000A 0x01 */ MSCRIPT_CMD_PAUSE(),
    /* 0x000B 0x03 */ MSCRIPT_CMD_CHECK_CALLBACK(0x0),
    /* 0x000E 0x01 */ MSCRIPT_CMD_CLOSE_TEXT(),
    /* 0x000F 0x01 */ MSCRIPT_CMD_DONE(),
};

MsgScript D_80BDFDF8[] = {
    /* 0x0000 0x03 */ MSCRIPT_CMD_CHECK_CALLBACK(0x0),
    /* 0x0003 0x05 */ MSCRIPT_CMD_CHECK_WEEK_EVENT_REG(WEEKEVENTREG_57_04, 0x0079 - 0x0008),
    /* 0x0008 0x03 */ MSCRIPT_CMD_BEGIN_TEXT(0x2B1A),
    /* 0x000B 0x01 */ MSCRIPT_CMD_AWAIT_TEXT(),
    /* 0x000C 0x03 */ MSCRIPT_CMD_CONTINUE_TEXT(0x00FF),
    /* 0x000F 0x09 */ MSCRIPT_CMD_CHECK_ITEM_ACTION(PLAYER_IA_LETTER_MAMA, 0x0030 - 0x0018, 0x0025 - 0x0018, 0x0),
    /* 0x0018 0x03 */ MSCRIPT_CMD_BEGIN_TEXT(0x2B1B),
    /* 0x001B 0x03 */ MSCRIPT_CMD_NOTEBOOK_EVENT(BOMBERS_NOTEBOOK_EVENT_MET_MADAME_AROMA),
    /* 0x001E 0x01 */ MSCRIPT_CMD_AWAIT_TEXT(),
    /* 0x001F 0x01 */ MSCRIPT_CMD_PAUSE(),
    /* 0x0020 0x03 */ MSCRIPT_CMD_CHECK_CALLBACK(0x0),
    /* 0x0023 0x01 */ MSCRIPT_CMD_CLOSE_TEXT(),
    /* 0x0024 0x01 */ MSCRIPT_CMD_DONE(),

    /* 0x0025 0x03 */ MSCRIPT_CMD_PLAYER_TALK(0x2B1C),
    /* 0x0028 0x01 */ MSCRIPT_CMD_AWAIT_TEXT(),
    /* 0x0029 0x03 */ MSCRIPT_CMD_JUMP_3(0x0),
    /* 0x002C 0x01 */ MSCRIPT_CMD_AWAIT_TEXT(),
    /* 0x002D 0x03 */ MSCRIPT_CMD_JUMP(0x000C - 0x0030),

    /* 0x0030 0x03 */ MSCRIPT_CMD_PLAYER_TALK(0x2B1D),
    /* 0x0033 0x01 */ MSCRIPT_CMD_AWAIT_TEXT(),
    /* 0x0034 0x03 */ MSCRIPT_CMD_JUMP_3(0x0),
    /* 0x0037 0x01 */ MSCRIPT_CMD_AWAIT_TEXT(),
    /* 0x0038 0x03 */ MSCRIPT_CMD_CONTINUE_TEXT(0x2B1E),
    /* 0x003B 0x01 */ MSCRIPT_CMD_AWAIT_TEXT(),
    /* 0x003C 0x03 */ MSCRIPT_CMD_CONTINUE_TEXT(0x2B1F),
    /* 0x003F 0x01 */ MSCRIPT_CMD_AWAIT_TEXT(),
    /* 0x0040 0x01 */ MSCRIPT_CMD_CLOSE_TEXT(),
    /* 0x0041 0x03 */ MSCRIPT_CMD_DELETE_ITEM(ITEM_LETTER_MAMA),
    /* 0x0044 0x05 */ MSCRIPT_CMD_CHECK_WEEK_EVENT_REG(WEEKEVENTREG_57_08, 0x005C - 0x0049),
    /* 0x0049 0x05 */ MSCRIPT_CMD_OFFER_ITEM(GI_CHATEAU_BOTTLE, 0x0),
    /* 0x004E 0x03 */ MSCRIPT_CMD_SET_COLLECTIBLE(0x006F),
    /* 0x0051 0x03 */ MSCRIPT_CMD_JUMP_3(0x0),
    /* 0x0054 0x01 */ MSCRIPT_CMD_AWAIT_TEXT_DONE(),
    /* 0x0055 0x03 */ MSCRIPT_CMD_NOTEBOOK_EVENT(BOMBERS_NOTEBOOK_EVENT_DELIVERED_PRIORITY_MAIL),
    /* 0x0058 0x01 */ MSCRIPT_CMD_AWAIT_TEXT(),
    /* 0x0059 0x03 */ MSCRIPT_CMD_AUTOTALK(0x0068 - 0x005C),
    /* 0x005C 0x05 */ MSCRIPT_CMD_OFFER_ITEM(GI_RUPEE_PURPLE, 0x0),
    /* 0x0061 0x03 */ MSCRIPT_CMD_SET_COLLECTIBLE(0x0005),
    /* 0x0064 0x01 */ MSCRIPT_CMD_AWAIT_TEXT(),
    /* 0x0065 0x03 */ MSCRIPT_CMD_AUTOTALK(0x0),
    /* 0x0068 0x03 */ MSCRIPT_CMD_BEGIN_TEXT(0x2B20),
    /* 0x006B 0x03 */ MSCRIPT_CMD_NOTEBOOK_EVENT(BOMBERS_NOTEBOOK_EVENT_MET_MADAME_AROMA),
    /* 0x006E 0x01 */ MSCRIPT_CMD_AWAIT_TEXT(),
    /* 0x006F 0x01 */ MSCRIPT_CMD_UNSET_AUTOTALK(),
    /* 0x0070 0x03 */ MSCRIPT_CMD_SET_WEEK_EVENT_REG(WEEKEVENTREG_57_04),
    /* 0x0073 0x03 */ MSCRIPT_CMD_SET_WEEK_EVENT_REG(WEEKEVENTREG_57_08),
    /* 0x0076 0x03 */ MSCRIPT_CMD_JUMP(0x001F - 0x0079),

    /* 0x0079 0x03 */ MSCRIPT_CMD_BEGIN_TEXT(0x2B3C),
    /* 0x007C 0x03 */ MSCRIPT_CMD_NOTEBOOK_EVENT(BOMBERS_NOTEBOOK_EVENT_MET_MADAME_AROMA),
    /* 0x007F 0x01 */ MSCRIPT_CMD_AWAIT_TEXT(),
    /* 0x0080 0x03 */ MSCRIPT_CMD_JUMP(0x001F - 0x0083),
};

MsgScript D_80BDFE7C[] = {
    /* 0x0000 0x03 */ MSCRIPT_CMD_BEGIN_TEXT(0x2A9C),
    /* 0x0003 0x03 */ MSCRIPT_CMD_NOTEBOOK_EVENT(BOMBERS_NOTEBOOK_EVENT_MET_MADAME_AROMA),
    /* 0x0006 0x01 */ MSCRIPT_CMD_AWAIT_TEXT(),
    /* 0x0007 0x01 */ MSCRIPT_CMD_DONE(),
};

MsgScript D_80BDFE84[] = {
    /* 0x0000 0x03 */ MSCRIPT_CMD_BEGIN_TEXT(0x2A9D),
    /* 0x0003 0x03 */ MSCRIPT_CMD_NOTEBOOK_EVENT(BOMBERS_NOTEBOOK_EVENT_MET_MADAME_AROMA),
    /* 0x0006 0x01 */ MSCRIPT_CMD_AWAIT_TEXT(),
    /* 0x0007 0x01 */ MSCRIPT_CMD_DONE(),
};

MsgScript D_80BDFE8C[] = {
    /* 0x0000 0x05 */ MSCRIPT_CMD_CHECK_WEEK_EVENT_REG(WEEKEVENTREG_57_02, 0x0014 - 0x0005),
    /* 0x0005 0x03 */ MSCRIPT_CMD_BEGIN_TEXT(0x2A9E),
    /* 0x0008 0x01 */ MSCRIPT_CMD_AWAIT_TEXT(),
    /* 0x0009 0x03 */ MSCRIPT_CMD_CONTINUE_TEXT(0x2A9F),
    /* 0x000C 0x01 */ MSCRIPT_CMD_AWAIT_TEXT(),
    /* 0x000D 0x03 */ MSCRIPT_CMD_CONTINUE_TEXT(0x2AA0),
    /* 0x0010 0x01 */ MSCRIPT_CMD_AWAIT_TEXT(),
    /* 0x0011 0x03 */ MSCRIPT_CMD_JUMP(0x0018 - 0x0014),
    /* 0x0014 0x03 */ MSCRIPT_CMD_BEGIN_TEXT(0x2AA0),
    /* 0x0017 0x01 */ MSCRIPT_CMD_AWAIT_TEXT(),
    /* 0x0018 0x07 */ MSCRIPT_CMD_CHECK_TEXT_CHOICE(0x0, 0x0039 - 0x001F, 0x0039 - 0x001F),
    /* 0x001F 0x01 */ MSCRIPT_CMD_PLAY_DECIDE(),
    /* 0x0020 0x03 */ MSCRIPT_CMD_BEGIN_TEXT(0x2AA2),
    /* 0x0023 0x01 */ MSCRIPT_CMD_AWAIT_TEXT(),
    /* 0x0024 0x01 */ MSCRIPT_CMD_CLOSE_TEXT(),
    /* 0x0025 0x05 */ MSCRIPT_CMD_OFFER_ITEM(GI_MASK_KAFEIS_MASK, 0x0),
    /* 0x002A 0x03 */ MSCRIPT_CMD_SET_COLLECTIBLE(0x008F),
    /* 0x002D 0x03 */ MSCRIPT_CMD_JUMP_3(0x0),
    /* 0x0030 0x01 */ MSCRIPT_CMD_AWAIT_TEXT_DONE(),
    /* 0x0031 0x03 */ MSCRIPT_CMD_NOTEBOOK_EVENT(BOMBERS_NOTEBOOK_EVENT_RECEIVED_KAFEIS_MASK),
    /* 0x0034 0x03 */ MSCRIPT_CMD_NOTEBOOK_EVENT(BOMBERS_NOTEBOOK_EVENT_MET_MADAME_AROMA),
    /* 0x0037 0x01 */ MSCRIPT_CMD_AWAIT_TEXT(),
    /* 0x0038 0x01 */ MSCRIPT_CMD_DONE(),

    /* 0x0039 0x01 */ MSCRIPT_CMD_PLAY_CANCEL(),
    /* 0x003A 0x03 */ MSCRIPT_CMD_BEGIN_TEXT(0x2AA1),
    /* 0x003D 0x03 */ MSCRIPT_CMD_NOTEBOOK_EVENT(BOMBERS_NOTEBOOK_EVENT_MET_MADAME_AROMA),
    /* 0x0040 0x01 */ MSCRIPT_CMD_AWAIT_TEXT(),
    /* 0x0041 0x03 */ MSCRIPT_CMD_SET_WEEK_EVENT_REG(WEEKEVENTREG_57_02),
    /* 0x0044 0x01 */ MSCRIPT_CMD_DONE(),
};

MsgScript D_80BDFED4[] = {
    /* 0x0000 0x03 */ MSCRIPT_CMD_CHECK_CALLBACK(0x0),
    /* 0x0003 0x03 */ MSCRIPT_CMD_PLAYER_TALK(0x2B1D),
    /* 0x0006 0x01 */ MSCRIPT_CMD_AWAIT_TEXT(),
    /* 0x0007 0x03 */ MSCRIPT_CMD_JUMP_3(0x0),
    /* 0x000A 0x01 */ MSCRIPT_CMD_AWAIT_TEXT(),
    /* 0x000B 0x03 */ MSCRIPT_CMD_CONTINUE_TEXT(0x2B1E),
    /* 0x000E 0x01 */ MSCRIPT_CMD_AWAIT_TEXT(),
    /* 0x000F 0x03 */ MSCRIPT_CMD_CONTINUE_TEXT(0x2B1F),
    /* 0x0012 0x01 */ MSCRIPT_CMD_AWAIT_TEXT(),
    /* 0x0013 0x01 */ MSCRIPT_CMD_CLOSE_TEXT(),
    /* 0x0014 0x03 */ MSCRIPT_CMD_DELETE_ITEM(ITEM_LETTER_MAMA),
    /* 0x0017 0x05 */ MSCRIPT_CMD_CHECK_WEEK_EVENT_REG(WEEKEVENTREG_57_08, 0x002F - 0x001C),
    /* 0x001C 0x05 */ MSCRIPT_CMD_OFFER_ITEM(GI_CHATEAU_BOTTLE, 0x0),
    /* 0x0021 0x03 */ MSCRIPT_CMD_SET_COLLECTIBLE(0x006F),
    /* 0x0024 0x03 */ MSCRIPT_CMD_JUMP_3(0x0),
    /* 0x0027 0x01 */ MSCRIPT_CMD_AWAIT_TEXT_DONE(),
    /* 0x0028 0x03 */ MSCRIPT_CMD_NOTEBOOK_EVENT(BOMBERS_NOTEBOOK_EVENT_DELIVERED_PRIORITY_MAIL),
    /* 0x002B 0x01 */ MSCRIPT_CMD_AWAIT_TEXT(),
    /* 0x002C 0x03 */ MSCRIPT_CMD_AUTOTALK(0x003B - 0x002F),
    /* 0x002F 0x05 */ MSCRIPT_CMD_OFFER_ITEM(GI_RUPEE_PURPLE, 0x0),
    /* 0x0034 0x03 */ MSCRIPT_CMD_SET_COLLECTIBLE(0x0005),
    /* 0x0037 0x01 */ MSCRIPT_CMD_AWAIT_TEXT(),
    /* 0x0038 0x03 */ MSCRIPT_CMD_AUTOTALK(0x0),
    /* 0x003B 0x03 */ MSCRIPT_CMD_BEGIN_TEXT(0x2B20),
    /* 0x003E 0x03 */ MSCRIPT_CMD_NOTEBOOK_EVENT(BOMBERS_NOTEBOOK_EVENT_MET_MADAME_AROMA),
    /* 0x0041 0x01 */ MSCRIPT_CMD_AWAIT_TEXT(),
    /* 0x0042 0x01 */ MSCRIPT_CMD_UNSET_AUTOTALK(),
    /* 0x0043 0x03 */ MSCRIPT_CMD_SET_WEEK_EVENT_REG(WEEKEVENTREG_57_04),
    /* 0x0046 0x03 */ MSCRIPT_CMD_SET_WEEK_EVENT_REG(WEEKEVENTREG_57_08),
    /* 0x0049 0x01 */ MSCRIPT_CMD_PAUSE(),
    /* 0x004A 0x03 */ MSCRIPT_CMD_CHECK_CALLBACK(0x0),
    /* 0x004D 0x01 */ MSCRIPT_CMD_CLOSE_TEXT(),
    /* 0x004E 0x01 */ MSCRIPT_CMD_DONE(),
};

MsgScript D_80BDFF24[] = {
    /* 0x0000 0x03 */ MSCRIPT_CMD_CHECK_CALLBACK(0x0),
    /* 0x0003 0x03 */ MSCRIPT_CMD_PLAYER_TALK(0x2B1C),
    /* 0x0006 0x01 */ MSCRIPT_CMD_AWAIT_TEXT(),
    /* 0x0007 0x03 */ MSCRIPT_CMD_CONTINUE_TEXT(0x00FF),
    /* 0x000A 0x09 */ MSCRIPT_CMD_CHECK_ITEM_ACTION(PLAYER_IA_LETTER_MAMA, 0x0020 - 0x0013, 0x0003 - 0x0013, 0x0),
    /* 0x0013 0x03 */ MSCRIPT_CMD_BEGIN_TEXT(0x2B1B),
    /* 0x0016 0x03 */ MSCRIPT_CMD_NOTEBOOK_EVENT(BOMBERS_NOTEBOOK_EVENT_MET_MADAME_AROMA),
    /* 0x0019 0x01 */ MSCRIPT_CMD_AWAIT_TEXT(),
    /* 0x001A 0x01 */ MSCRIPT_CMD_PAUSE(),
    /* 0x001B 0x03 */ MSCRIPT_CMD_CHECK_CALLBACK(0x0),
    /* 0x001E 0x01 */ MSCRIPT_CMD_CLOSE_TEXT(),
    /* 0x001F 0x01 */ MSCRIPT_CMD_DONE(),

    /* 0x0020 0x03 */ MSCRIPT_CMD_PLAYER_TALK(0x2B1D),
    /* 0x0023 0x01 */ MSCRIPT_CMD_AWAIT_TEXT(),
    /* 0x0024 0x03 */ MSCRIPT_CMD_JUMP_3(0x0),
    /* 0x0027 0x01 */ MSCRIPT_CMD_AWAIT_TEXT(),
    /* 0x0028 0x03 */ MSCRIPT_CMD_CONTINUE_TEXT(0x2B1E),
    /* 0x002B 0x01 */ MSCRIPT_CMD_AWAIT_TEXT(),
    /* 0x002C 0x03 */ MSCRIPT_CMD_CONTINUE_TEXT(0x2B1F),
    /* 0x002F 0x01 */ MSCRIPT_CMD_AWAIT_TEXT(),
    /* 0x0030 0x01 */ MSCRIPT_CMD_CLOSE_TEXT(),
    /* 0x0031 0x03 */ //MSCRIPT_CMD_DELETE_ITEM(ITEM_LETTER_MAMA),
    /* 0x0031 0x03 */ MSCRIPT_CMD_JUMP(0x0),
    /* 0x0034 0x05 */ MSCRIPT_CMD_CHECK_WEEK_EVENT_REG(WEEKEVENTREG_57_08, 0x004C - 0x0039),
    /* 0x0039 0x05 */ MSCRIPT_CMD_OFFER_ITEM(GI_CHATEAU_BOTTLE, 0x0),
    /* 0x003E 0x03 */ MSCRIPT_CMD_SET_COLLECTIBLE(0x006F),
    /* 0x0041 0x03 */ MSCRIPT_CMD_JUMP_3(0x0),
    /* 0x0044 0x01 */ MSCRIPT_CMD_AWAIT_TEXT_DONE(),
    /* 0x0045 0x03 */ MSCRIPT_CMD_NOTEBOOK_EVENT(BOMBERS_NOTEBOOK_EVENT_DELIVERED_PRIORITY_MAIL),
    /* 0x0048 0x01 */ MSCRIPT_CMD_AWAIT_TEXT(),
    /* 0x0049 0x03 */ MSCRIPT_CMD_AUTOTALK(0x0058 - 0x004C),
    /* 0x004C 0x05 */ MSCRIPT_CMD_OFFER_ITEM(GI_RUPEE_PURPLE, 0x0),
    /* 0x0051 0x03 */ MSCRIPT_CMD_SET_COLLECTIBLE(0x0005),
    /* 0x0054 0x01 */ MSCRIPT_CMD_AWAIT_TEXT(),
    /* 0x0055 0x03 */ MSCRIPT_CMD_AUTOTALK(0x0),
    /* 0x0058 0x03 */ MSCRIPT_CMD_BEGIN_TEXT(0x2B20),
    /* 0x005B 0x03 */ MSCRIPT_CMD_NOTEBOOK_EVENT(BOMBERS_NOTEBOOK_EVENT_MET_MADAME_AROMA),
    /* 0x005E 0x01 */ MSCRIPT_CMD_AWAIT_TEXT(),
    /* 0x005F 0x01 */ MSCRIPT_CMD_UNSET_AUTOTALK(),
    /* 0x0060 0x03 */ MSCRIPT_CMD_SET_WEEK_EVENT_REG(WEEKEVENTREG_57_04),
    /* 0x0063 0x03 */ //MSCRIPT_CMD_SET_WEEK_EVENT_REG(WEEKEVENTREG_57_08),
    /* 0x0063 0x03 */ MSCRIPT_CMD_JUMP(0x0),
    /* 0x0066 0x03 */ MSCRIPT_CMD_JUMP(0x001A - 0x0069),

    // Unreachable
    /* 0x0069 0x03 */ MSCRIPT_CMD_BEGIN_TEXT(0x2B3C),
    /* 0x006C 0x03 */ MSCRIPT_CMD_NOTEBOOK_EVENT(BOMBERS_NOTEBOOK_EVENT_MET_MADAME_AROMA),
    /* 0x006F 0x01 */ MSCRIPT_CMD_AWAIT_TEXT(),
    /* 0x0070 0x03 */ MSCRIPT_CMD_JUMP(0x001A - 0x0073),
};

static ColliderCylinderInit sCylinderInit = {
    {
        COLTYPE_HIT1,
        AT_NONE,
        AC_NONE,
        OC1_ON | OC1_TYPE_ALL,
        OC2_TYPE_1,
        COLSHAPE_CYLINDER,
    },
    {
        ELEMTYPE_UNK1,
        { 0x00000000, 0x00, 0x00 },
        { 0x00000000, 0x00, 0x00 },
        TOUCH_NONE | TOUCH_SFX_NORMAL,
        BUMP_NONE,
        OCELEM_ON,
    },
    { 14, 62, 0, { 0, 0, 0 } },
};

static CollisionCheckInfoInit2 sColChkInfoInit = { 0, 0, 0, 0, MASS_IMMOVABLE };

extern FlexSkeletonHeader gMadameAromaSkel;

s32 EnAl_ChangeAnim(EnAl* this, s32 animIndex);
void func_80BDF5E8(EnAl* this, PlayState* play);

void EnAl_Init(Actor* thisx, PlayState* play) {
    EnAl* this = THIS;

    ActorShape_Init(&this->actor.shape, 0.0f, NULL, 0.0f);
    SkelAnime_InitFlex(play, &this->skelAnime, &gMadameAromaSkel, NULL, this->jointTable, this->morphTable,
                       MADAME_AROMA_LIMB_MAX);
    this->animIndex = ENAL_ANIM_NONE;
    EnAl_ChangeAnim(this, ENAL_ANIM_1);
    Collider_InitAndSetCylinder(play, &this->unk_310, &this->actor, &sCylinderInit);
    CollisionCheck_SetInfo2(&this->actor.colChkInfo, DamageTable_Get(0x16), &sColChkInfoInit);
    Actor_SetScale(&this->actor, 0.01f);
    this->scheduleResult = 0;
    this->actionFunc = func_80BDF5E8;

    this->actionFunc(this, play);
}

s32 func_80BDE92C(EnAl* this, PlayState* play);
s32 func_80BDE7FC(Actor* thisx, PlayState* play);
s32 func_80BDEA14(EnAl* this, PlayState* play);

u8* func_80BDEABC(EnAl* this, PlayState* play) {
    Player* player = GET_PLAYER(play);

    if (this->scheduleResult == 3) {
        this->msgScriptCallback = (MsgScriptCallback) func_80BDE92C;
        return (u8*) D_80BDFD14;
    }

    switch (this->scheduleResult) {
        case 1:
            if (player->transformation == PLAYER_FORM_DEKU) {
                return (u8*) D_80BDFDD0;
            }

            //if (INV_CONTENT(ITEM_MASK_KAFEIS_MASK) != ITEM_MASK_KAFEIS_MASK) {
            if (!recomp_location_is_checked(GI_MASK_KAFEIS_MASK)) {
                return (u8*) D_80BDFE8C;
            }

            if (Player_GetMask(play) == PLAYER_MASK_KAFEIS_MASK) {
                return (u8*) D_80BDFE7C;
            }
            return (u8*) D_80BDFE84;

        case 2:
            if (!CHECK_WEEKEVENTREG(WEEKEVENTREG_89_08) && CHECK_WEEKEVENTREG(WEEKEVENTREG_85_80)) {
                this->msgScriptCallback = (MsgScriptCallback) func_80BDE7FC;
                return (u8*) D_80BDFCBC;
            }

            this->msgScriptCallback = (MsgScriptCallback) func_80BDEA14;
            if (Player_GetMask(play) != PLAYER_MASK_KAFEIS_MASK) {
                return (u8*) D_80BDFDE8;
            }

            if (this->unk_4C2 & 0x800) {
                return (u8*) D_80BDFED4;
            }

            if (this->unk_4C2 & 0x1000) {
                return (u8*) D_80BDFF24;
            }
            return (u8*) D_80BDFDF8;

        default:
            return NULL;
    }
}