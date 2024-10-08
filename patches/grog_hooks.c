#include "patches.h"
#include "ultra64.h"

#include "apcommon.h"
#include "misc_funcs.h"

struct EnHs;

typedef void (*EnHsActionFunc)(struct EnHs*, PlayState*);

#define OBJECT_HS_LIMB_MAX 0x10

#define HS_GET_EXIT_INDEX(thisx) ((thisx)->params & 0xF)

#define HS_TYPE_UNK1 1

// params mystery: Vanilla Grog is 0xFE01
//   0xFE00 space is never checked in Grog code
//   at the same time, type UNK1 is only checked directly with params == 1, no &F
//   so HS_TYPE_UNK1 is never valid and is unused, as 0xFE00 is still present even if its not doing anything else

// The count of [chicks following the player]   is stored in [this->actor.home.rot.z] (incremented by 2)
// The count of [chicks transformed into adult] is stored in [this->actor.home.rot.x] (incremented by 2)

typedef struct EnHs {
    /* 0x000 */ Actor actor;
    /* 0x144 */ ColliderCylinder collider;
    /* 0x190 */ SkelAnime skelAnime;
    /* 0x1D4 */ Vec3s jointTable[OBJECT_HS_LIMB_MAX];
    /* 0x234 */ Vec3s morphTable[OBJECT_HS_LIMB_MAX];
    /* 0x294 */ Vec3s headRot;
    /* 0x29A */ Vec3s torsoRot; // Set but unused
    /* 0x2A0 */ u16 stateFlags;
    /* 0x2A2 */ s16 stateTimer; // reused for different actionFunc
    /* 0x2A4 */ Vec3f nwcPos[20]; // actual chick position are even values, odd values seem to be extra values for smoother chain
    /* 0x394 */ EnHsActionFunc actionFunc;
} EnHs; // size = 0x398

void func_8095345C(EnHs* this, PlayState* play);

void func_80953098(EnHs* this, PlayState* play) {
    if (Actor_HasParent(&this->actor, play)) {
        this->actor.parent = NULL;
        this->actionFunc = func_8095345C;
        this->actor.flags |= ACTOR_FLAG_10000;
        this->stateFlags |= 0x10;
        Actor_OfferTalkExchange(&this->actor, play, 1000.0f, 1000.0f, PLAYER_IA_MINUS1);
    } else {
        this->stateFlags |= 8;
        if (recomp_location_is_checked(GI_MASK_BUNNY)) {
            Actor_OfferGetItem(&this->actor, play, GI_RUPEE_RED, 10000.0f, 50.0f);
        } else {
            Actor_OfferGetItem(&this->actor, play, GI_MASK_BUNNY, 10000.0f, 50.0f);
        }
    }
}