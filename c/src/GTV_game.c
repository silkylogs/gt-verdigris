#include <stdio.h>
#include <stdlib.h>

#include "GTV_game.h"

/* -- Utility ------------------------------------------------------------------------------------*/

typedef struct GTV_Arena GTV_Arena;

void GTV_Arena_init(GTV_Arena *a, byte *mem, int32 size) {
    a->backing_memory = mem;
    a->allocated = 0;
    a->capacity = size;
}

void *GTV_Arena_alloc(GTV_Arena *a, int32 size) {
    // TODO
}

void GTV_Arena_free_all(GTV_Arena *a);

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

GTV_LOCAL void GTV_PrivateGameState_init(GTV_PrivateGameState *state) {
    GTV_Player_init(&state->player);

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
    GTV_Player_move_x(
        &interface->private->player,
        &interface->keyboard_input,
        interface->private->boxes
    );

    GTV_Player_move_y(
        &interface->private->player,
        &interface->keyboard_input,
        interface->private->boxes
    );

    // TODO implement gravity and jumping
    // TODO find a way to diffrentiate being grounded to touching a wall's sides
}

// -- Drawing ------------------------------------

GTV_LOCAL bool
GTV_Sprite_draw_8x8_sprite(byte *framebuffer, uint64 digit, int32 pos_x, int32 pos_y, byte color) {
    byte rows[8] = {
        (digit >> 0x38) & 0xFF,
        (digit >> 0x30) & 0xFF,
        (digit >> 0x28) & 0xFF,
        (digit >> 0x20) & 0xFF,

        (digit >> 0x18) & 0xFF,
        (digit >> 0x10) & 0xFF,
        (digit >> 0x08) & 0xFF,
        (digit >> 0x00) & 0xFF,
    };
    bool success = true;
    int32 bits_in_byte = 8;

    for (int32 y = 0; y < sizeof rows; y++) {
        for (int32 x = 0; x < bits_in_byte; x++) {
            bool contains_pixel = (rows[y] >> (bits_in_byte - x)) & 0x1;
            if (contains_pixel) {
                success &= GTV_Framebuffer_set_pixel_xy(
                    framebuffer,
                    (x + pos_x), (y + pos_y),
                    color
                );
            }
        }
    }

    return success;
}

// Keeping this around as GTV_Sprite_draw_8x8_sprite usage tutorial
// GTV_LOCAL void GTV_draw_editor(GTV_GameStateInterface *interface) {
//     int32 x = 0, y = 0;
//     for (int32 i = 0; i < GTV_ATLAS_FONT_DIGITS_8x8_COUNT; i++) {
//         GTV_Sprite_draw_8x8_sprite(
//             interface->framebuffer,
//             g_font_atlas_digits_8x8[i],
//             x+(8*i), y,
//             0xFE
//         );
//     }
//     y += 8;
//     for (int32 i = 0; i < GTV_ATLAS_FONT_CAPITAL_LETTERS_8x8_COUNT; i++) {
//         GTV_Sprite_draw_8x8_sprite(
//             interface->framebuffer,
//             g_font_atlas_capital_letters_8x8[i],
//             x+(8*i), y,
//             0xFD
//         );
//     }
// }

GTV_LOCAL void GTV_draw_gameplay(GTV_GameStateInterface *interface) {
    GTV_Player player = interface->private->player;

    GTV_Sprite_blit_to_framebuffer(
        interface->framebuffer,
        player.sprite,
        (int32)player.bounds.x,
        (int32)player.bounds.y
    );

    GTV_AABB_draw(player.bounds, interface->framebuffer, 0xFE);
    
    for (int32 i = 0; i < GTV_AABB_COLLECTION_COUNT; i++) {
        GTV_AABB_draw(
            interface->private->boxes.elems[i],
            interface->framebuffer, 0xFE
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

    // yeah framebuffer blitting works dont worry about it
    // interface->current_palette.colors[0xFF].g = 0xFF;
    // GTV_Framebuffer_set_pixel_xy(interface->framebuffer, 5, 10, 0xFF);

    // GTV_draw_gameplay(interface);
}

/* -- Game -------------------------------------------------------------------------------------- */

/* -- Game state interface ---------------------------------------------------------------------- */

GTV_EXPORT void GTV_GameStateInterface_init(GTV_GameStateInterface *interface) {
    /* {
        for (int32 c = 0; c < GTV_COLOR_PALETTE_SIZE; c++) {
            interface->current_palette.colors[c].r = (byte)0;
            interface->current_palette.colors[c].g = (byte)0;
            interface->current_palette.colors[c].b = (byte)0;
        }
        interface->current_palette.colors[0xFF].r = 0x00;
        interface->current_palette.colors[0xFF].g = 0xFF;
        interface->current_palette.colors[0xFF].b = 0x00;

        interface->current_palette.colors[0xFE].r = 0x7F;
        interface->current_palette.colors[0xFE].g = 0x00;
        interface->current_palette.colors[0xFE].b = 0x00;

        interface->current_palette.colors[0xFD].r = 0xF0;
        interface->current_palette.colors[0xFD].g = 0x80;
        interface->current_palette.colors[0xFD].b = 0x00;
    } */

    interface->should_exit = false;

    interface->private = malloc(sizeof (GTV_PrivateGameState));
    GTV_PrivateGameState_init(interface->private);
}

GTV_EXPORT void GTV_GameStateInterface_update(GTV_GameStateInterface *interface) {
    GTV_Framebuffer_clear(interface->framebuffer, 0);
    GTV_update_gameplay(interface);
    GTV_draw_all(interface);
}

GTV_EXPORT void GTV_GameStateInterface_cleanup(GTV_GameStateInterface *interface) {
    free(interface->private);
}

/* -- Game state interface ---------------------------------------------------------------------- */
