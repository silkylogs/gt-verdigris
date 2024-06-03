#include <stdio.h>
#include <stdlib.h>

#include "GTV_Arena.h"
#include "GTV_game.h"

/* -- Utility ------------------------------------------------------------------------------------*/
/* -- Utility ------------------------------------------------------------------------------------*/

#include "GTV_Colors.c"
#include "GTV_Framebuffer.c"
#include "GTV_Sprites.c"
#include "sprites.h"
#include "GTV_AABB.c"
#include "GTV_Player.c"

/* -- Game -------------------------------------------------------------------------------------- */

typedef struct GTV_PrivateGameState {
    GTV_Player player;
    GTV_AABB_Collection boxes;
} GTV_PrivateGameState;

GTV_LOCAL void GTV_PrivateGameState_init(GTV_PrivateGameState *state, GTV_Sprite player_sprite) {
    GTV_Player_init(&state->player, player_sprite);

    state->boxes.elems[0].x = 0.0f;
    state->boxes.elems[0].y = 20.0f;
    state->boxes.elems[0].w = 50.0f;
    state->boxes.elems[0].h = 5.0f;

    state->boxes.elems[1].x = 30.0f;
    state->boxes.elems[1].y = 99.0f;
    state->boxes.elems[1].w = 99.0f;
    state->boxes.elems[1].h = 10.0f;

    state->boxes.elems[2].x = 70.0f;
    state->boxes.elems[2].y = 00.0f;
    state->boxes.elems[2].w = 10.0f;
    state->boxes.elems[2].h = 100.0f;

    state->boxes.elems[3].x = 128.0f;
    state->boxes.elems[3].y = 128.0f;
    state->boxes.elems[3].w = 10.0f;
    state->boxes.elems[3].h = 256.0f;
}

GTV_LOCAL void GTV_update_gameplay(GTV_GameStateInterface *interface) {
    if (interface->keyboard_input.arrow_keys[GTV_KEYBOARD_INPUT_ARROW_KEY_RIGHT]) {
        GTV_Player_move_x(&interface->private->player, 1.0f, interface->private->boxes);
    } else if (interface->keyboard_input.arrow_keys[GTV_KEYBOARD_INPUT_ARROW_KEY_LEFT]) {
        GTV_Player_move_x(&interface->private->player, -1.0f, interface->private->boxes);
    } else {
        (void)0;
    }
    

    GTV_Player_move_y(&interface->private->player, 1.0f, interface->private->boxes);

    // TODO implement gravity and jumping
    // TODO find a way to diffrentiate being grounded to touching a wall's sides
}

/* -- Game -------------------------------------------------------------------------------------- */

/* -- Drawing ----------------------------------------------------------------------------------- */

GTV_LOCAL void GTV_draw_gameplay(GTV_GameStateInterface *interface) {
    GTV_Player player = interface->private->player;

    GTV_Sprite_blit_to_framebuffer(
        interface->framebuffer,
        player.sprite,
        (int32)player.bounds.x,
        (int32)player.bounds.y
    );

    byte bound_color = { 0x31 };
    GTV_AABB_draw(player.bounds, interface->framebuffer, bound_color);
    
    for (int32 i = 0; i < GTV_AABB_COLLECTION_COUNT; i++) {
        GTV_AABB_draw(
            interface->private->boxes.elems[i],
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

    for (int32 sprite_y = 0; sprite_y < sprite.height; sprite_y++) {
        for (int32 sprite_x = 0; sprite_x < sprite.width; sprite_x++) {
            int32
                atlas_x = sprite_x + 16,
                atlas_y = sprite_y + 0;
            int32
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
    byte clear_color = { 0 };
    GTV_Framebuffer_clear(interface->framebuffer, clear_color);

    GTV_update_gameplay(interface);
    if (interface->keyboard_input.special_keys[GTV_KEYBOARD_INPUT_SPECIAL_KEY_ESC]) {
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