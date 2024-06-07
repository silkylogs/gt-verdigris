#include <stdio.h>
#include <stdlib.h>

#include "GTV_Arena.h"
#include "GTV_game.h"

/* -- Utility ------------------------------------------------------------------------------------*/

GTV_LOCAL float GTV_clamp(float x, float min, float max) {
    float retval;
    // printf("x=%f ", x);
    if (x < min) retval = min;
    else if (x > max) retval = max;
    else retval = x;
    // printf("retval=%f\n", retval);
    return retval;
}

// A port of C#'s Math.Sign function
GTV_LOCAL int GTV_sign(int num) {
    if (num > 0) return 1;
    else if (num < 0) return -1;
    else return 0;
}

/* -- Utility ------------------------------------------------------------------------------------*/

#include "GTV_Colors.c"
#include "GTV_Framebuffer.c"
#include "GTV_Sprites.c"
#include "sprites.h"
#include "GTV_AABB.c"
#include "GTV_Player.c"

/* -- Game -------------------------------------------------------------------------------------- */

typedef struct GTV_Level {
    char *name;
    uint32_t id;
    GTV_Player player;
    GTV_AABB_Collection solids;
    GTV_Sprite level_atlas;
} GTV_Level;

typedef struct GTV_PrivateGameState {
    GTV_Level test_level;
} GTV_PrivateGameState;

GTV_LOCAL void GTV_test_level_init(GTV_Level *lvl) {
    GTV_Player_set_position(&lvl->player, 50.0f, 75.0f);

    // Load level data from appropriate manager
    lvl->solids.elems[0].x = 0.0f;
    lvl->solids.elems[0].y = 20.0f;
    lvl->solids.elems[0].w = 50.0f;
    lvl->solids.elems[0].h = 5.0f;

    lvl->solids.elems[1].x = 30.0f;
    lvl->solids.elems[1].y = 99.0f;
    lvl->solids.elems[1].w = 99.0f;
    lvl->solids.elems[1].h = 10.0f;

    lvl->solids.elems[2].x = 70.0f;
    lvl->solids.elems[2].y = 00.0f;
    lvl->solids.elems[2].w = 10.0f;
    lvl->solids.elems[2].h = 100.0f;

    lvl->solids.elems[3].x = 128.0f;
    lvl->solids.elems[3].y = 128.0f;
    lvl->solids.elems[3].w = 10.0f;
    lvl->solids.elems[3].h = 256.0f;
}

GTV_LOCAL void GTV_PrivateGameState_init(GTV_PrivateGameState *state, GTV_Sprite player_sprite) {
    GTV_Player_init(&state->test_level.player, player_sprite);
    GTV_test_level_init(&state->test_level);
}

GTV_LOCAL void GTV_update_gameplay(GTV_GameStateInterface *interface) {
    GTV_Player *player = &interface->private->test_level.player;
    bool *keys = interface->keyboard_input.keys;
    GTV_AABB_Collection solids = interface->private->test_level.solids;

    // Reset
    if (interface->keyboard_input.keys[GTV_KEYBOARD_KEY_R]) {
        GTV_Player_init(player, player->sprite);
    }

    player->vx = GTV_clamp(player->vx, -player->vx_max, player->vx_max);
    if (keys[GTV_KEYBOARD_KEY_RIGHT]) {
        GTV_Player_move_x(player, player->vx, solids);
    } else if (keys[GTV_KEYBOARD_KEY_LEFT]) {
        GTV_Player_move_x(player, -player->vx, solids);
    } else {
        (void)0;
    }
    
    if (
        keys[GTV_KEYBOARD_KEY_UP] &&
        player->grounded
    ) {
        player->vy = -player->jmpy;
    }
    if (!player->grounded) {
        player->vy += player->gravy;
    } else {
        // player->vy = 0;
    }
    player->vy = GTV_clamp(player->vy, -player->vy_max, player->vy_max);
    GTV_Player_move_y(player, player->vy, solids);
    // printf("y=%f vy=%f\n", player->bounds.y, player->vy);
}

/* -- Game -------------------------------------------------------------------------------------- */

/* -- Drawing ----------------------------------------------------------------------------------- */

GTV_LOCAL void GTV_draw_gameplay(GTV_GameStateInterface *interface) {
    GTV_Player player = interface->private->test_level.player;
    GTV_AABB_Collection solids = interface->private->test_level.solids;

    GTV_Sprite_blit_to_framebuffer(
        interface->framebuffer,
        player.sprite,
        (int32_t)player.bounds.x,
        (int32_t)player.bounds.y
    );

    uint8_t bound_color = { 0x31 };
    GTV_AABB_draw(player.bounds, interface->framebuffer, bound_color);
    
    for (int32_t i = 0; i < GTV_AABB_COLLECTION_COUNT; i++) {
        GTV_AABB_draw(
            solids.elems[i],
            interface->framebuffer, bound_color
        );
    }
}

GTV_LOCAL void GTV_draw_all(GTV_GameStateInterface *interface) {
    // TESTING
    GTV_Sprite_blit_to_framebuffer(
        interface->framebuffer,
        interface->palettized_sprite_atlas,
        0, 0
    );
    // TESTING

    GTV_draw_gameplay(interface);
}

/* -- Drawing ----------------------------------------------------------------------------------- */

/* -- Game state interface ---------------------------------------------------------------------- */

GTV_LOCAL GTV_Sprite generate_player_sprite(GTV_Sprite atlas, GTV_Arena *arena) {
    GTV_Sprite sprite = {
        .width = 16, .height = 16, .data = NULL
    };
    sprite.data = GTV_Arena_alloc(arena, sprite.width * sprite.height);

    for (int32_t sprite_y = 0; sprite_y < sprite.height; sprite_y++) {
        for (int32_t sprite_x = 0; sprite_x < sprite.width; sprite_x++) {
            int32_t
                atlas_x = sprite_x + 16,
                atlas_y = sprite_y + 0;
            int32_t
                sprite_idx = sprite_y * sprite.width + sprite_x,
                atlas_idx = atlas_y * atlas.width + atlas_x; 
            
            sprite.data[sprite_idx] = atlas.data[atlas_idx];
        }
    }

    return sprite;
}

GTV_EXPORT void GTV_GameStateInterface_init(
    GTV_GameStateInterface *interface,
    GTV_Arena *arena,
    GTV_ColorPalette palette,
    GTV_Sprite palettized_atlas
) {
    interface->current_palette = palette;
    interface->palettized_sprite_atlas = palettized_atlas;

    interface->private = GTV_Arena_alloc(arena, sizeof (GTV_PrivateGameState));
    GTV_Sprite player_sprite = generate_player_sprite(interface->palettized_sprite_atlas, arena);
    GTV_PrivateGameState_init(interface->private, player_sprite);
}

GTV_EXPORT void GTV_GameStateInterface_update(GTV_GameStateInterface *interface) {
    uint8_t clear_color = { 0 };
    GTV_Framebuffer_clear(interface->framebuffer, clear_color);

    GTV_update_gameplay(interface);
    if (interface->keyboard_input.keys[GTV_KEYBOARD_KEY_ESC]) {
        interface->exit_requested = true;
    }

    GTV_draw_all(interface);
}

GTV_EXPORT void GTV_GameStateInterface_cleanup(GTV_GameStateInterface *interface) {
    (void)interface; // shut the fuck up
}

// -- Game state interface -------------------------------------------------------------------------

__declspec(dllexport)
void GTV_game_tick(
    GTV_GameStateInterface *interface,
    GTV_Arena *arena,
    GTV_ColorPalette palette,
    GTV_Sprite palettized_atlas
) {
    if (!interface->initialized) {
        GTV_GameStateInterface_init(interface, arena, palette, palettized_atlas);
        interface->initialized = true;
    }

    GTV_GameStateInterface_update(interface);

    if (interface->exit_requested) {
        GTV_GameStateInterface_cleanup(interface);
    }
}

bool DllMain(void *inst, int reason, void *reserved) {
    (void)inst; (void)reason; (void)reserved;
    return true;
}