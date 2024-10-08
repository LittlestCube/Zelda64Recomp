#include <vector>
#include <map>
#include <string>
#include <cmath>
#include <fstream>

#include "librecomp/recomp.h"
#include "librecomp/overlays.hpp"
#include "zelda_config.h"
#include "recomp_input.h"
#include "recomp_ui.h"
#include "zelda_render.h"
#include "zelda_sound.h"
#include "../../lib/mm-decomp/include/z64item.h"
#include "librecomp/helpers.hpp"
#include "../patches/input.h"
#include "../patches/graphics.h"
#include "../patches/sound.h"
#include "../patches/misc_funcs.h"
#include "ultramodern/ultramodern.hpp"
#include "ultramodern/config.hpp"

#include "Archipelago.h"

#define GI_TRUE_SKULL_TOKEN GI_75

#define GI_AP_PROG GI_77
#define GI_AP_FILLER GI_90
#define GI_AP_USEFUL GI_B3

std::vector<int> item_i_to_player;

bool death_link;
bool pending_death_link;

bool pending_clear_items = false;

extern "C" void recomp_skulltulas_enabled(uint8_t* rdram, recomp_context* ctx) {
    _return(ctx, AP_GetSlotDataInt("skullsanity") != 2);
}

extern "C" void apRecvDeathLink() {
    pending_death_link = true;
}

extern "C" void recomp_get_death_link_pending(uint8_t* rdram, recomp_context* ctx) {
    _return(ctx, pending_death_link);
}

extern "C" void recomp_reset_death_link_pending(uint8_t* rdram, recomp_context* ctx) {
    pending_death_link = false;
    AP_DeathLinkClear();
}

const char* getStr(uint8_t* rdram, PTR(char) str_ptr) {
    u32 i = 0;
    std::string str = "";
    while (MEM_B(i, (gpr) str_ptr) != 0) {
        str += MEM_B(i, (gpr) str_ptr);
        i += 1;
    }
    return str.c_str();
}

extern "C" void recomp_get_death_link_enabled(uint8_t* rdram, recomp_context* ctx) {
    _return(ctx, AP_GetSlotDataInt("death_link") == 1);
}

extern "C" void recomp_send_death_link(uint8_t* rdram, recomp_context* ctx) {
    AP_DeathLinkSend();
}

u32 apHasItem(int64_t itemId) {
    u32 count = 0;
    u32 items_size = AP_GetReceivedItemsSize();
    for (u32 i = 0; i < items_size; ++i) {
        if (AP_GetReceivedItem(i) == itemId) {
            count += 1;
        }
    }
    return count;
}

extern "C" void apGetItemId(uint8_t* rdram, recomp_context* ctx) {
    u32 arg = _arg<0, u32>(rdram, ctx);

    if (arg == 0) {
        _return(ctx, 0);
        return;
    }

    int64_t location = 0x34769420000000 | arg;

    if (AP_GetLocationHasLocalItem(location)) {
        int64_t item = AP_GetItemAtLocation(location) & 0xFFFFFF;

        if ((item & 0xFF0000) == 0x000000) {
            u8 gi = item & 0xFF;
            if (gi == GI_SKULL_TOKEN) {
                _return(ctx, (u32) GI_TRUE_SKULL_TOKEN);
                return;
            } else if (gi == GI_SWORD_KOKIRI) {
                _return(ctx, (u32) MIN(GI_SWORD_KOKIRI + apHasItem(GI_SWORD_KOKIRI), GI_SWORD_GILDED));
                return;
            } else if (gi == GI_QUIVER_30) {
                _return(ctx, (u32) MIN(GI_QUIVER_30 + apHasItem(GI_QUIVER_30), GI_QUIVER_50));
                return;
            } else if (gi == GI_BOMB_BAG_20) {
                _return(ctx, (u32) MIN(GI_BOMB_BAG_20 + apHasItem(GI_BOMB_BAG_20), GI_BOMB_BAG_40));
                return;
            } else if (gi == GI_WALLET_ADULT) {
                _return(ctx, (u32) MIN(GI_WALLET_ADULT + apHasItem(GI_WALLET_ADULT), GI_WALLET_GIANT));
                return;
            }
            _return(ctx, (u32) gi);
            return;
        }
        switch (item & 0xFF0000) {
            case 0x010000:
                _return(ctx, (u32) GI_B2);
                return;
            case 0x020000:
                switch (item & 0xFF) {
                    case 0x00:
                        _return(ctx, (u32) GI_MAGIC_JAR_SMALL);
                        return;
                    case 0x01:
                        _return(ctx, (u32) GI_SWORD_KOKIRI);
                        return;
                }
                return;
            case 0x040000:
                switch (item & 0xFF) {
                    case ITEM_SONG_TIME:
                        _return(ctx, (u32) GI_A6);
                        return;
                    case ITEM_SONG_HEALING:
                        _return(ctx, (u32) GI_AF);
                        return;
                    case ITEM_SONG_EPONA:
                        _return(ctx, (u32) GI_A5);
                        return;
                    case ITEM_SONG_SOARING:
                        _return(ctx, (u32) GI_A3);
                        return;
                    case ITEM_SONG_STORMS:
                        _return(ctx, (u32) GI_A2);
                        return;
                    case ITEM_SONG_SONATA:
                        _return(ctx, (u32) GI_AE);
                        return;
                    case ITEM_SONG_LULLABY:
                        _return(ctx, (u32) GI_AD);
                        return;
                    case ITEM_SONG_NOVA:
                        _return(ctx, (u32) GI_AC);
                        return;
                    case ITEM_SONG_ELEGY:
                        _return(ctx, (u32) GI_A8);
                        return;
                    case ITEM_SONG_OATH:
                        _return(ctx, (u32) GI_A7);
                        return;
                }
                return;
            case 0x090000:
                switch (item & 0xFF) {
                    case ITEM_DUNGEON_MAP:
                        _return(ctx, (u32) GI_MAP);
                        return;
                    case ITEM_COMPASS:
                        _return(ctx, (u32) GI_COMPASS);
                        return;
                    case ITEM_KEY_BOSS:
                        _return(ctx, (u32) GI_KEY_BOSS);
                        return;
                    case ITEM_KEY_SMALL:
                        _return(ctx, (u32) GI_KEY_SMALL);
                        return;
                }
                return;
        }
    }

    switch (AP_GetLocationItemType(location)) {
        case ITEM_TYPE_FILLER:
            _return(ctx, (u32) GI_AP_FILLER);
            return;
        case ITEM_TYPE_USEFUL:
            _return(ctx, (u32) GI_AP_USEFUL);
            return;
        default:
            _return(ctx, (u32) GI_AP_PROG);
            return;
    }
}

extern "C" void apSay(uint8_t* rdram, recomp_context* ctx) {
    AP_Say(std::string((char*) ctx->r4));
}

extern "C" void recomp_get_items_size(uint8_t* rdram, recomp_context* ctx) {
    _return(ctx, ((u32) AP_GetReceivedItemsSize()));
}

extern "C" void recomp_get_item(uint8_t* rdram, recomp_context* ctx) {
    u32 items_i = _arg<0, u32>(rdram, ctx);
    _return(ctx, ((u32) AP_GetReceivedItem(items_i)));
}

extern "C" void recomp_get_item_foreign(uint8_t* rdram, recomp_context* ctx) {
    u32 items_i = _arg<0, u32>(rdram, ctx);
    _return(ctx, item_i_to_player[items_i] != AP_GetPlayerID());
}

extern "C" void recomp_has_item(uint8_t* rdram, recomp_context* ctx) {
    u32 arg = _arg<0, u32>(rdram, ctx);
    int64_t location_id = ((int64_t) (((int64_t) 0x34769420000000) | ((int64_t) arg)));
    _return(ctx, apHasItem(location_id));
}

extern "C" void recomp_send_location(uint8_t* rdram, recomp_context* ctx) {
    u32 arg = _arg<0, u32>(rdram, ctx);
    int64_t location_id = ((int64_t) (((int64_t) 0x34769420000000) | ((int64_t) arg)));
    if (!AP_GetLocationIsChecked(location_id)) {
        AP_SendItem(location_id);
    }
}

extern "C" void recomp_location_is_checked(uint8_t* rdram, recomp_context* ctx) {
    u32 arg = _arg<0, u32>(rdram, ctx);
    int64_t location_id = ((int64_t) (((int64_t) 0x34769420000000) | ((int64_t) arg)));
    _return(ctx, AP_GetLocationIsChecked(location_id));
}

extern "C" void recomp_complete_goal(uint8_t* rdram, recomp_context* ctx) {
    AP_StoryComplete();
}

extern "C" void recomp_update_inputs(uint8_t* rdram, recomp_context* ctx) {
    recomp::poll_inputs();
}

extern "C" void recomp_puts(uint8_t* rdram, recomp_context* ctx) {
    PTR(char) cur_str = _arg<0, PTR(char)>(rdram, ctx);
    u32 length = _arg<1, u32>(rdram, ctx);

    for (u32 i = 0; i < length; i++) {
        fputc(MEM_B(i, (gpr)cur_str), stdout);
    }
}

extern "C" void recomp_exit(uint8_t* rdram, recomp_context* ctx) {
    ultramodern::quit();
}

extern "C" void recomp_get_gyro_deltas(uint8_t* rdram, recomp_context* ctx) {
    float* x_out = _arg<0, float*>(rdram, ctx);
    float* y_out = _arg<1, float*>(rdram, ctx);

    recomp::get_gyro_deltas(x_out, y_out);
}

extern "C" void recomp_get_mouse_deltas(uint8_t* rdram, recomp_context* ctx) {
    float* x_out = _arg<0, float*>(rdram, ctx);
    float* y_out = _arg<1, float*>(rdram, ctx);

    recomp::get_mouse_deltas(x_out, y_out);
}

extern "C" void recomp_powf(uint8_t* rdram, recomp_context* ctx) {
    float a = _arg<0, float>(rdram, ctx);
    float b = ctx->f14.fl; //_arg<1, float>(rdram, ctx);

    _return(ctx, std::pow(a, b));
}

extern "C" void recomp_get_target_framerate(uint8_t* rdram, recomp_context* ctx) {
    int frame_divisor = _arg<0, u32>(rdram, ctx);

    _return(ctx, ultramodern::get_target_framerate(60 / frame_divisor));
}

extern "C" void recomp_get_aspect_ratio(uint8_t* rdram, recomp_context* ctx) {
    ultramodern::renderer::GraphicsConfig graphics_config = ultramodern::renderer::get_graphics_config();
    float original = _arg<0, float>(rdram, ctx);
    int width, height;
    recompui::get_window_size(width, height);

    switch (graphics_config.ar_option) {
        case ultramodern::renderer::AspectRatio::Original:
        default:
            _return(ctx, original);
            return;
        case ultramodern::renderer::AspectRatio::Expand:
            _return(ctx, std::max(static_cast<float>(width) / height, original));
            return;
    }
}

extern "C" void recomp_get_targeting_mode(uint8_t* rdram, recomp_context* ctx) {
    _return(ctx, static_cast<int>(zelda64::get_targeting_mode()));
}

extern "C" void recomp_get_bgm_volume(uint8_t* rdram, recomp_context* ctx) {
    _return(ctx, zelda64::get_bgm_volume() / 100.0f);
}

extern "C" void recomp_get_low_health_beeps_enabled(uint8_t* rdram, recomp_context* ctx) {
    _return(ctx, static_cast<u32>(zelda64::get_low_health_beeps_enabled()));
}

extern "C" void recomp_time_us(uint8_t* rdram, recomp_context* ctx) {
    _return(ctx, static_cast<u32>(std::chrono::duration_cast<std::chrono::microseconds>(ultramodern::time_since_start()).count()));
}

extern "C" void recomp_autosave_enabled(uint8_t* rdram, recomp_context* ctx) {
    _return(ctx, static_cast<s32>(zelda64::get_autosave_mode() == zelda64::AutosaveMode::On));
}

extern "C" void recomp_load_overlays(uint8_t * rdram, recomp_context * ctx) {
    u32 rom = _arg<0, u32>(rdram, ctx);
    PTR(void) ram = _arg<1, PTR(void)>(rdram, ctx);
    u32 size = _arg<2, u32>(rdram, ctx);

    load_overlays(rom, ram, size);
}

extern "C" void recomp_high_precision_fb_enabled(uint8_t * rdram, recomp_context * ctx) {
    _return(ctx, static_cast<s32>(zelda64::renderer::RT64HighPrecisionFBEnabled()));
}

extern "C" void recomp_get_resolution_scale(uint8_t* rdram, recomp_context* ctx) {
    _return(ctx, ultramodern::get_resolution_scale());
}

extern "C" void recomp_get_inverted_axes(uint8_t* rdram, recomp_context* ctx) {
    s32* x_out = _arg<0, s32*>(rdram, ctx);
    s32* y_out = _arg<1, s32*>(rdram, ctx);

    zelda64::CameraInvertMode mode = zelda64::get_camera_invert_mode();

    *x_out = (mode == zelda64::CameraInvertMode::InvertX || mode == zelda64::CameraInvertMode::InvertBoth);
    *y_out = (mode == zelda64::CameraInvertMode::InvertY || mode == zelda64::CameraInvertMode::InvertBoth);
}

extern "C" void recomp_get_analog_inverted_axes(uint8_t* rdram, recomp_context* ctx) {
    s32* x_out = _arg<0, s32*>(rdram, ctx);
    s32* y_out = _arg<1, s32*>(rdram, ctx);

    zelda64::CameraInvertMode mode = zelda64::get_analog_camera_invert_mode();

    *x_out = (mode == zelda64::CameraInvertMode::InvertX || mode == zelda64::CameraInvertMode::InvertBoth);
    *y_out = (mode == zelda64::CameraInvertMode::InvertY || mode == zelda64::CameraInvertMode::InvertBoth);
}

extern "C" void recomp_analog_cam_enabled(uint8_t* rdram, recomp_context* ctx) {
    _return<s32>(ctx, zelda64::get_analog_cam_mode() == zelda64::AnalogCamMode::On);
}

extern "C" void recomp_get_camera_inputs(uint8_t* rdram, recomp_context* ctx) {
    float* x_out = _arg<0, float*>(rdram, ctx);
    float* y_out = _arg<1, float*>(rdram, ctx);

    // TODO expose this in the menu
    constexpr float radial_deadzone = 0.05f;

    float x, y;

    recomp::get_right_analog(&x, &y);

    float magnitude = sqrtf(x * x + y * y);

    if (magnitude < radial_deadzone) {
        *x_out = 0.0f;
        *y_out = 0.0f;
    }
    else {
        float x_normalized = x / magnitude;
        float y_normalized = y / magnitude;

        *x_out = x_normalized * ((magnitude - radial_deadzone) / (1 - radial_deadzone));
        *y_out = y_normalized * ((magnitude - radial_deadzone) / (1 - radial_deadzone));
    }
}

extern "C" void recomp_set_right_analog_suppressed(uint8_t* rdram, recomp_context* ctx) {
    s32 suppressed = _arg<0, s32>(rdram, ctx);

    recomp::set_right_analog_suppressed(suppressed);
}
