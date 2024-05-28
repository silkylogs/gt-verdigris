#include <stdio.h>
#include <stdlib.h>

#include "GTV_game.h"

/* -- Utility ------------------------------------------------------------------------------------*/

typedef struct GTV_Arena GTV_Arena;

GTV_EXPORT bool GTV_Arena_init(GTV_Arena *a, byte *mem, int32 size) {
    if (!mem) {
        printf("Provided memory is null\n");
        return false;
    }

    a->backing_memory = mem;
    a->allocated = 0;
    a->capacity = size;

    return true;
}

GTV_EXPORT void *GTV_Arena_alloc(GTV_Arena *a, int32 size) {
    int32 remaining_capacity = a->capacity - a->allocated;
    if (size > remaining_capacity || size <= 0) {
        printf("Cannot provide %d bytes of memory\n", size);
        return NULL;
    }
    
    byte *mem = &a->backing_memory[a->allocated];
    a->allocated += size;
    return mem;
}

GTV_EXPORT void GTV_Arena_free_all(GTV_Arena *a) {
    a->allocated = 0;
}

// Use this as a "checkpoint" of sorts
GTV_LOCAL int32 GTV_Arena_get_current_allocated_bytes(GTV_Arena a) {
    return a.allocated;
}

// Set the arena's internal state to the "checkpoint"
GTV_LOCAL bool GTV_Arena_shrink(GTV_Arena *a, int32 shrinked_size) {
    if (shrinked_size < 0 || shrinked_size > a->allocated) return false;
    a->allocated = shrinked_size;
    return true;
}

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

    GTV_draw_gameplay(interface);
}

/* -- Game -------------------------------------------------------------------------------------- */

/* -- Game state interface ---------------------------------------------------------------------- */

GTV_LOCAL GTV_Sprite generate_player_sprite(GTV_Sprite atlas, GTV_Arena *arena) {
    GTV_Sprite sprite = {
        .width = 16, .height = 16, .data = NULL
    };
    sprite.data = GTV_Arena_alloc(arena, sprite.width * sprite.height);

    for (int32 sprite_y = 0; sprite_y < sprite.height; sprite_y++) {
        for (int32 sprite_x = 0; sprite_x < sprite.width; sprite_x++) {
            int32
                atlas_x = sprite_x + 0,
                atlas_y = sprite_y + 0;
            int32
                sprite_idx = sprite_y * sprite.width + sprite_x,
                atlas_idx = atlas_y * atlas.width + atlas_x; 
            
            // FIXME TODO Why is the atlas empty?
            printf(
                "Copying atlas[%04d] (%02X) to sprite[%03d] (%02X)\n",
                atlas_idx,
                (int32)atlas.data[atlas_idx],
                sprite_idx,
                (int32)sprite.data[sprite_idx]
            );
            sprite.data[sprite_idx] = atlas.data[atlas_idx];
        }
    }

    return sprite;
}

GTV_EXPORT void GTV_GameStateInterface_init(GTV_GameStateInterface *interface, GTV_Arena *arena) {
    interface->should_exit = false;

    interface->palettized_sprite_atlas.width = 256;
    interface->palettized_sprite_atlas.height = 256;
    interface->palettized_sprite_atlas.data = GTV_Arena_alloc(
        arena,
        interface->palettized_sprite_atlas.width * interface->palettized_sprite_atlas.height
    );

    interface->private = GTV_Arena_alloc(arena, sizeof (GTV_PrivateGameState));
    GTV_Sprite player_sprite = generate_player_sprite(interface->palettized_sprite_atlas, arena);
    GTV_PrivateGameState_init(interface->private, player_sprite);
}

GTV_EXPORT void GTV_GameStateInterface_update(GTV_GameStateInterface *interface) {
    GTV_Framebuffer_clear(interface->framebuffer, 0);
    GTV_update_gameplay(interface);
    GTV_draw_all(interface);
}

GTV_EXPORT void GTV_GameStateInterface_cleanup(GTV_GameStateInterface *interface) {
    interface = interface; // shut the fuck up
}

/* -- Game state interface ---------------------------------------------------------------------- */
