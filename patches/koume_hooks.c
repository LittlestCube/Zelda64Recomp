#include "patches.h"
#include "ultra64.h"
#include "z64msgevent.h"

#include "apcommon.h"
#include "misc_funcs.h"

#define LOCATION_KOUME_GIFT 0x000043

#define KOUME_KIOSK_LIMB_MAX 0x02

struct EnDnh;

#define FLAGS (ACTOR_FLAG_TARGETABLE | ACTOR_FLAG_FRIENDLY)

#define THIS ((EnDnh*)thisx)

struct EnDnh;

typedef void (*EnDnhActionFunc)(struct EnDnh*, PlayState*);

typedef struct EnDnh {
    /* 0x000 */ Actor actor;
    /* 0x144 */ EnDnhActionFunc actionFunc;
    /* 0x148 */ SkelAnime skelAnime;
    /* 0x18C */ u16 unk18C;
    /* 0x190 */ MsgScript* msgScript;
    /* 0x194 */ s32 msgScriptPos;
    /* 0x198 */ s16 unk198;
    /* 0x19A */ UNK_TYPE1 pad19A[0x2];
    /* 0x19C */ s16 blinkTimer;
    /* 0x19E */ s16 eyeTexIndex;
    /* 0x1A0 */ MsgScriptCallback msgScriptCallback;
    /* 0x1A4 */ Vec3s jointTable[KOUME_KIOSK_LIMB_MAX];
    /* 0x1B0 */ Vec3s morphTable[KOUME_KIOSK_LIMB_MAX];
} EnDnh; // size = 0x1BC

typedef enum {
    /* 0 */ ENDNH_ANIM_HEAD_MOVING,
    /* 1 */ ENDNH_ANIM_MAX
} EnDnhAnimation;

extern AnimationHeader gKoumeKioskHeadMovingAnim;
extern SkeletonHeader gKoumeKioskSkel;

static AnimationInfoS sAnimationInfo[ENDNH_ANIM_MAX] = {
    { &gKoumeKioskHeadMovingAnim, 1.0f, 0, -1, ANIMMODE_LOOP, 0 }, // ENDNH_ANIM_HEAD_MOVING
};


MsgScript D_80A51250[] = {
    /* 0x0000 0x05 */ MSCRIPT_CMD_CHECK_WEEK_EVENT_REG(WEEKEVENTREG_SAVED_KOUME, 0x000D - 0x0005),
    /* 0x0005 0x03 */ MSCRIPT_CMD_BEGIN_TEXT(0x0862),
    /* 0x0008 0x01 */ MSCRIPT_CMD_AWAIT_TEXT(),
    /* 0x0009 0x03 */ MSCRIPT_CMD_SET_WEEK_EVENT_REG(WEEKEVENTREG_TALKED_KOUME_KIOSK_EMPTY),
    /* 0x000C 0x01 */ MSCRIPT_CMD_DONE(),

    /* 0x000D 0x03 */ MSCRIPT_CMD_CHECK_HUMAN(0x0015 - 0x0010),
    /* 0x0010 0x03 */ MSCRIPT_CMD_BEGIN_TEXT(0x0867),
    /* 0x0013 0x01 */ MSCRIPT_CMD_AWAIT_TEXT(),
    /* 0x0014 0x01 */ MSCRIPT_CMD_DONE(),

    /* 0x0015 0x05 */ MSCRIPT_CMD_CHECK_WEEK_EVENT_REG(WEEKEVENTREG_CLEARED_WOODFALL_TEMPLE, 0x0094 - 0x001A),
    /* 0x001A 0x05 */ MSCRIPT_CMD_CHECK_WEEK_EVENT_REG(WEEKEVENTREG_25_10, 0x004F - 0x001F),
    /* 0x001F 0x03 */ MSCRIPT_CMD_BEGIN_TEXT(0x0859),
    /* 0x0022 0x01 */ MSCRIPT_CMD_AWAIT_TEXT(),
    /* 0x0023 0x05 */ //MSCRIPT_CMD_CHECK_ITEM(ITEM_PICTOGRAPH_BOX, 0x0040 - 0x0028),
    /* 0x0023 0x03 */ MSCRIPT_CMD_JUMP(0x2),
    /* 0x0024 0x01 */ 0x00,
    /* 0x0025 0x01 */ 0x00,
    /* 0x0028 0x03 */ MSCRIPT_CMD_CONTINUE_TEXT(0x0863),
    /* 0x002B 0x01 */ MSCRIPT_CMD_AWAIT_TEXT(),
    /* 0x002C 0x01 */ MSCRIPT_CMD_CLOSE_TEXT(),
    /* 0x002D 0x05 */ MSCRIPT_CMD_OFFER_ITEM(GI_PICTOGRAPH_BOX, 0x0),
    /* 0x0032 0x03 */ MSCRIPT_CMD_SET_COLLECTIBLE(0x0043),
    /* 0x0035 0x01 */ MSCRIPT_CMD_AWAIT_TEXT(),
    /* 0x0036 0x03 */ MSCRIPT_CMD_AUTOTALK(0x0),
    /* 0x0039 0x03 */ MSCRIPT_CMD_BEGIN_TEXT(0x088D),
    /* 0x003C 0x01 */ MSCRIPT_CMD_AWAIT_TEXT(),
    /* 0x003D 0x03 */ MSCRIPT_CMD_JUMP(0x0047 - 0x0040),
    /* 0x0040 0x03 */ MSCRIPT_CMD_CONTINUE_TEXT(0x088D),
    /* 0x0043 0x01 */ MSCRIPT_CMD_AWAIT_TEXT(),
    /* 0x0044 0x03 */ MSCRIPT_CMD_JUMP(0x0),
    /* 0x0047 0x01 */ MSCRIPT_CMD_CLOSE_TEXT(),
    /* 0x0048 0x03 */ MSCRIPT_CMD_SET_WEEK_EVENT_REG(WEEKEVENTREG_25_10),
    /* 0x004B 0x03 */ MSCRIPT_CMD_CHECK_CALLBACK(0x0),
    /* 0x004E 0x01 */ MSCRIPT_CMD_DONE(),

    /* 0x004F 0x05 */ MSCRIPT_CMD_CHECK_WEEK_EVENT_REG(WEEKEVENTREG_26_10, 0x006A - 0x0054),
    /* 0x0054 0x03 */ MSCRIPT_CMD_BEGIN_TEXT(0x085A),
    /* 0x0057 0x03 */ MSCRIPT_CMD_SET_WEEK_EVENT_REG(WEEKEVENTREG_26_10),
    /* 0x005A 0x01 */ MSCRIPT_CMD_AWAIT_TEXT(),
    /* 0x005B 0x03 */ MSCRIPT_CMD_CONTINUE_TEXT(0x085B),
    /* 0x005E 0x01 */ MSCRIPT_CMD_AWAIT_TEXT(),
    /* 0x005F 0x03 */ MSCRIPT_CMD_CONTINUE_TEXT(0x085C),
    /* 0x0062 0x01 */ MSCRIPT_CMD_AWAIT_TEXT(),
    /* 0x0063 0x07 */ MSCRIPT_CMD_CHECK_TEXT_CHOICE(0x0070 - 0x006A, 0x008E - 0x006A, 0x0070 - 0x006A),
    /* 0x006A 0x03 */ MSCRIPT_CMD_BEGIN_TEXT(0x085B),
    /* 0x006D 0x03 */ MSCRIPT_CMD_JUMP(0x005E - 0x0070),

    /* 0x0070 0x01 */ MSCRIPT_CMD_PLAY_DECIDE(),
    /* 0x0071 0x03 */ MSCRIPT_CMD_CONTINUE_TEXT(0x085E),
    /* 0x0074 0x01 */ MSCRIPT_CMD_AWAIT_TEXT(),
    /* 0x0075 0x05 */ MSCRIPT_CMD_CHECK_RUPEES(10, 0x007F - 0x007A),
    /* 0x007A 0x03 */ MSCRIPT_CMD_CONTINUE_TEXT(0x0875),
    /* 0x007D 0x01 */ MSCRIPT_CMD_AWAIT_TEXT(),
    /* 0x007E 0x01 */ MSCRIPT_CMD_DONE(),

    /* 0x007F 0x03 */ MSCRIPT_CMD_CHANGE_RUPEES(-10),
    /* 0x0082 0x03 */ MSCRIPT_CMD_CONTINUE_TEXT(0x0860),
    /* 0x0085 0x01 */ MSCRIPT_CMD_AWAIT_TEXT(),
    /* 0x0086 0x01 */ MSCRIPT_CMD_CLOSE_TEXT(),
    /* 0x0087 0x03 */ MSCRIPT_CMD_SET_WEEK_EVENT_REG(WEEKEVENTREG_25_10),
    /* 0x008A 0x03 */ MSCRIPT_CMD_CHECK_CALLBACK(0x0),
    /* 0x008D 0x01 */ MSCRIPT_CMD_DONE(),

    /* 0x008E 0x01 */ MSCRIPT_CMD_PLAY_CANCEL(),
    /* 0x008F 0x03 */ MSCRIPT_CMD_CONTINUE_TEXT(0x085F),
    /* 0x0092 0x01 */ MSCRIPT_CMD_AWAIT_TEXT(),
    /* 0x0093 0x01 */ MSCRIPT_CMD_DONE(),

    /* 0x0094 0x05 */ MSCRIPT_CMD_CHECK_WEEK_EVENT_REG(WEEKEVENTREG_25_20, 0x00E2 - 0x0099),
    /* 0x0099 0x05 */ MSCRIPT_CMD_CHECK_WEEK_EVENT_REG(WEEKEVENTREG_26_10, 0x00DB - 0x009E),
    /* 0x009E 0x03 */ MSCRIPT_CMD_BEGIN_TEXT(0x0868),
    /* 0x00A1 0x03 */ MSCRIPT_CMD_SET_WEEK_EVENT_REG(WEEKEVENTREG_26_10),
    /* 0x00A4 0x01 */ MSCRIPT_CMD_AWAIT_TEXT(),
    /* 0x00A5 0x03 */ MSCRIPT_CMD_CONTINUE_TEXT(0x0869),
    /* 0x00A8 0x01 */ MSCRIPT_CMD_AWAIT_TEXT(),
    /* 0x00A9 0x03 */ MSCRIPT_CMD_CONTINUE_TEXT(0x086A),
    /* 0x00AC 0x01 */ MSCRIPT_CMD_AWAIT_TEXT(),
    /* 0x00AD 0x03 */ MSCRIPT_CMD_CONTINUE_TEXT(0x086B),
    /* 0x00B0 0x01 */ MSCRIPT_CMD_AWAIT_TEXT(),
    /* 0x00B1 0x03 */ MSCRIPT_CMD_CONTINUE_TEXT(0x086C),
    /* 0x00B4 0x01 */ MSCRIPT_CMD_AWAIT_TEXT(),
    /* 0x00B5 0x07 */ MSCRIPT_CMD_CHECK_TEXT_CHOICE(0x0, 0x00D5 - 0x00BC, 0x0),
    /* 0x00BC 0x01 */ MSCRIPT_CMD_PLAY_DECIDE(),
    /* 0x00BD 0x03 */ MSCRIPT_CMD_BEGIN_TEXT(0x086E),
    /* 0x00C0 0x03 */ MSCRIPT_CMD_SET_WEEK_EVENT_REG(WEEKEVENTREG_25_20),
    /* 0x00C3 0x03 */ MSCRIPT_CMD_SET_EVENT_INF(EVENTINF_35),
    /* 0x00C6 0x03 */ MSCRIPT_CMD_EVENT_INF(EVENTINF_36),
    /* 0x00C9 0x03 */ MSCRIPT_CMD_EVENT_INF(EVENTINF_37),
    /* 0x00CC 0x03 */ MSCRIPT_CMD_EVENT_INF(EVENTINF_40),
    /* 0x00CF 0x01 */ MSCRIPT_CMD_AWAIT_TEXT(),
    /* 0x00D0 0x01 */ MSCRIPT_CMD_CLOSE_TEXT(),
    /* 0x00D1 0x03 */ MSCRIPT_CMD_CHECK_CALLBACK(0x0),
    /* 0x00D4 0x01 */ MSCRIPT_CMD_DONE(),

    /* 0x00D5 0x01 */ MSCRIPT_CMD_PLAY_CANCEL(),
    /* 0x00D6 0x03 */ MSCRIPT_CMD_BEGIN_TEXT(0x086D),
    /* 0x00D9 0x01 */ MSCRIPT_CMD_AWAIT_TEXT(),
    /* 0x00DA 0x01 */ MSCRIPT_CMD_DONE(),

    /* 0x00DB 0x03 */ MSCRIPT_CMD_BEGIN_TEXT(0x0870),
    /* 0x00DE 0x01 */ MSCRIPT_CMD_AWAIT_TEXT(),
    /* 0x00DF 0x03 */ MSCRIPT_CMD_JUMP(0x00AD - 0x00E2),

    /* 0x00E2 0x05 */ MSCRIPT_CMD_CHECK_WEEK_EVENT_REG(WEEKEVENTREG_25_20, 0x00EE - 0x00E7),
    /* 0x00E7 0x03 */ MSCRIPT_CMD_BEGIN_TEXT(0x0871),
    /* 0x00EA 0x01 */ MSCRIPT_CMD_AWAIT_TEXT(),
    /* 0x00EB 0x03 */ MSCRIPT_CMD_JUMP(0x00A9 - 0x00EE),

    /* 0x00EE 0x03 */ MSCRIPT_CMD_BEGIN_TEXT(0x0872),
    /* 0x00F1 0x01 */ MSCRIPT_CMD_AWAIT_TEXT(),
    /* 0x00F2 0x03 */ MSCRIPT_CMD_CONTINUE_TEXT(0x0873),
    /* 0x00F5 0x01 */ MSCRIPT_CMD_AWAIT_TEXT(),
    /* 0x00F6 0x07 */ MSCRIPT_CMD_CHECK_TEXT_CHOICE(0x0, 0x0114 - 0x00FD, 0x0),
    /* 0x00FD 0x05 */ MSCRIPT_CMD_CHECK_RUPEES(10, 0x0108 - 0x0102),
    /* 0x0102 0x01 */ MSCRIPT_CMD_PLAY_ERROR(),
    /* 0x0103 0x03 */ MSCRIPT_CMD_CONTINUE_TEXT(0x0875),
    /* 0x0106 0x01 */ MSCRIPT_CMD_AWAIT_TEXT(),
    /* 0x0107 0x01 */ MSCRIPT_CMD_DONE(),

    /* 0x0108 0x01 */ MSCRIPT_CMD_PLAY_DECIDE(),
    /* 0x0109 0x03 */ MSCRIPT_CMD_CHANGE_RUPEES(-10),
    /* 0x010C 0x05 */ MSCRIPT_CMD_CHECK_WEEK_EVENT_REG(WEEKEVENTREG_26_40, 0x011A - 0x0111),
    /* 0x0111 0x03 */ MSCRIPT_CMD_JUMP(0x00BC - 0x0114),

    /* 0x0114 0x01 */ MSCRIPT_CMD_PLAY_CANCEL(),
    /* 0x0115 0x03 */ MSCRIPT_CMD_BEGIN_TEXT(0x0874),
    /* 0x0118 0x01 */ MSCRIPT_CMD_AWAIT_TEXT(),
    /* 0x0119 0x01 */ MSCRIPT_CMD_DONE(),

    /* 0x011A 0x03 */ MSCRIPT_CMD_BEGIN_TEXT(0x0876),
    /* 0x011D 0x03 */ MSCRIPT_CMD_SET_WEEK_EVENT_REG(WEEKEVENTREG_25_20),
    /* 0x0120 0x03 */ MSCRIPT_CMD_SET_EVENT_INF(EVENTINF_35),
    /* 0x0123 0x03 */ MSCRIPT_CMD_EVENT_INF(EVENTINF_36),
    /* 0x0126 0x03 */ MSCRIPT_CMD_EVENT_INF(EVENTINF_37),
    /* 0x0129 0x03 */ MSCRIPT_CMD_EVENT_INF(EVENTINF_40),
    /* 0x012C 0x01 */ MSCRIPT_CMD_AWAIT_TEXT(),
    /* 0x012D 0x01 */ MSCRIPT_CMD_CLOSE_TEXT(),
    /* 0x012E 0x03 */ MSCRIPT_CMD_CHECK_CALLBACK(0x0),
    /* 0x0131 0x01 */ MSCRIPT_CMD_DONE(),
};

MsgScript D_80A51384[] = {
    /* 0x0000 0x03 */ MSCRIPT_CMD_BEGIN_TEXT(0x0861),
    /* 0x0003 0x01 */ MSCRIPT_CMD_AWAIT_TEXT(),
    /* 0x0004 0x01 */ MSCRIPT_CMD_UNSET_AUTOTALK(),
    /* 0x0005 0x01 */ MSCRIPT_CMD_DONE(),
};

MsgScript D_80A5138C[] = {
    /* 0x0000 0x05 */ MSCRIPT_CMD_CHECK_EVENT_INF(EVENTINF_36, 0x006C - 0x0005),
    /* 0x0005 0x05 */ MSCRIPT_CMD_CHECK_WEEK_EVENT_REG(WEEKEVENTREG_26_40, 0x0044 - 0x000A),
    /* 0x000A 0x05 */ MSCRIPT_CMD_CHECK_EVENT_INF(EVENTINF_37, 0x0029 - 0x000F),
    /* 0x000F 0x03 */ MSCRIPT_CMD_BEGIN_TEXT(0x087C),
    /* 0x0012 0x01 */ MSCRIPT_CMD_AWAIT_TEXT(),
    /* 0x0013 0x05 */ MSCRIPT_CMD_CHECK_WEEK_EVENT_REG(WEEKEVENTREG_91_20, 0x0022 - 0x0018),
    /* 0x0018 0x03 */ MSCRIPT_CMD_CONTINUE_TEXT(0x087E),
    /* 0x001B 0x01 */ MSCRIPT_CMD_AWAIT_TEXT(),
    /* 0x001C 0x03 */ MSCRIPT_CMD_SET_WEEK_EVENT_REG(WEEKEVENTREG_91_20),
    /* 0x001F 0x03 */ MSCRIPT_CMD_JUMP(0x0070 - 0x0022),
    /* 0x0022 0x03 */ MSCRIPT_CMD_CONTINUE_TEXT(0x087B),
    /* 0x0025 0x01 */ MSCRIPT_CMD_AWAIT_TEXT(),
    /* 0x0026 0x03 */ MSCRIPT_CMD_JUMP(0x0070 - 0x0029),
    /* 0x0029 0x03 */ MSCRIPT_CMD_BEGIN_TEXT(0x087D),
    /* 0x002C 0x01 */ MSCRIPT_CMD_AWAIT_TEXT(),
    /* 0x002D 0x01 */ MSCRIPT_CMD_CLOSE_TEXT(),
    /* 0x002E 0x05 */ MSCRIPT_CMD_OFFER_ITEM(GI_HEART_PIECE, 0x0),
    /* 0x0033 0x03 */ MSCRIPT_CMD_SET_COLLECTIBLE(0x000C),
    /* 0x0036 0x01 */ MSCRIPT_CMD_AWAIT_TEXT(),
    /* 0x0037 0x03 */ MSCRIPT_CMD_AUTOTALK(0x0),
    /* 0x003A 0x03 */ MSCRIPT_CMD_BEGIN_TEXT(0x087E),
    /* 0x003D 0x01 */ MSCRIPT_CMD_AWAIT_TEXT(),
    /* 0x003E 0x03 */ MSCRIPT_CMD_SET_WEEK_EVENT_REG(WEEKEVENTREG_26_40),
    /* 0x0041 0x03 */ MSCRIPT_CMD_JUMP(0x0070 - 0x0044),
    /* 0x0044 0x05 */ MSCRIPT_CMD_CHECK_EVENT_INF(EVENTINF_37, 0x0054 - 0x0049),
    /* 0x0049 0x03 */ MSCRIPT_CMD_BEGIN_TEXT(0x087A),
    /* 0x004C 0x01 */ MSCRIPT_CMD_AWAIT_TEXT(),
    /* 0x004D 0x03 */ MSCRIPT_CMD_CONTINUE_TEXT(0x087B),
    /* 0x0050 0x01 */ MSCRIPT_CMD_AWAIT_TEXT(),
    /* 0x0051 0x03 */ MSCRIPT_CMD_JUMP(0x0070 - 0x0054),
    /* 0x0054 0x03 */ MSCRIPT_CMD_BEGIN_TEXT(0x0878),
    /* 0x0057 0x01 */ MSCRIPT_CMD_AWAIT_TEXT(),
    /* 0x0058 0x01 */ MSCRIPT_CMD_CLOSE_TEXT(),
    /* 0x0059 0x05 */ MSCRIPT_CMD_OFFER_ITEM(GI_RUPEE_PURPLE, 0x0),
    /* 0x005E 0x03 */ MSCRIPT_CMD_SET_COLLECTIBLE(0x0005),
    /* 0x0061 0x01 */ MSCRIPT_CMD_AWAIT_TEXT(),
    /* 0x0062 0x03 */ MSCRIPT_CMD_AUTOTALK(0x0),
    /* 0x0065 0x03 */ MSCRIPT_CMD_BEGIN_TEXT(0x0879),
    /* 0x0068 0x01 */ MSCRIPT_CMD_AWAIT_TEXT(),
    /* 0x0069 0x03 */ MSCRIPT_CMD_JUMP(0x0070 - 0x006C),
    /* 0x006C 0x03 */ MSCRIPT_CMD_BEGIN_TEXT(0x0877),
    /* 0x006F 0x01 */ MSCRIPT_CMD_AWAIT_TEXT(),
    /* 0x0070 0x03 */ MSCRIPT_CMD_EVENT_INF(EVENTINF_35),
    /* 0x0073 0x03 */ MSCRIPT_CMD_EVENT_INF(EVENTINF_36),
    /* 0x0076 0x03 */ MSCRIPT_CMD_EVENT_INF(EVENTINF_37),
    /* 0x0079 0x03 */ MSCRIPT_CMD_EVENT_INF(EVENTINF_40),
    /* 0x007C 0x01 */ MSCRIPT_CMD_UNSET_AUTOTALK(),
    /* 0x007D 0x01 */ MSCRIPT_CMD_DONE(),
};

s32 Actor_ProcessTalkRequest(Actor* actor, GameState* gameState);
void* func_80A50DF8(EnDnh* this, PlayState* play);
void func_80A50F38(EnDnh* this, PlayState* play);
void EnDnh_DoNothing(EnDnh* this, PlayState* play);
s32 func_80A50D40(Actor* actor, PlayState* play);

void* func_80A50DF8(EnDnh* this, PlayState* play) {
    if (recomp_location_is_checked(LOCATION_KOUME_GIFT)) {
        SET_WEEKEVENTREG(WEEKEVENTREG_25_10);
    } else {
        CLEAR_WEEKEVENTREG(WEEKEVENTREG_25_10);
    }
    switch (this->unk198) {
        case 1:
            return D_80A51384;

        case 2:
            return D_80A5138C;

        default:
            return D_80A51250;
    }
}