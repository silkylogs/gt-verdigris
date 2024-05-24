#include <stdio.h>
#include <stdlib.h>

#include "GTV_game.h"

/* -- Utility ------------------------------------------------------------------------------------*/
// TODO replace malloc with an external (arena) allocator
/* -- Utility ------------------------------------------------------------------------------------*/

/* -- Colors ------------------------------------------------------------------------------------ */

typedef struct GTV_Color GTV_Color;
typedef struct GTV_ColorPalette GTV_ColorPalette;
typedef struct GTV_ColorPaletteCollection GTV_ColorPaletteCollection;

// Debug colors
GTV_Color
    g_magenta = { 0xFF, 0x00, 0xFF },
    g_black =   { 0x00, 0x00, 0x00 };

GTV_LOCAL GTV_Color GTV_Color_get_from_palette(GTV_ColorPalette palette, byte color) {
    GTV_Color default_col = g_black;
    if ((color >= 0) && (color < GTV_FRAMEBUFFER_ELEM_COUNT))
        return palette.colors[color];
    else return default_col;
}

/* -- Colors ------------------------------------------------------------------------------------ */

/* -- Framebuffer ------------------------------------------------------------------------------- */

GTV_EXPORT GTV_Color
GTV_Framebuffer_get_color(byte *fb, GTV_ColorPalette curr_palette, int32 idx) {
    GTV_Color default_col = g_magenta;
    if ((idx >= 0) && (idx < GTV_FRAMEBUFFER_ELEM_COUNT))
        return GTV_Color_get_from_palette(curr_palette, fb[idx]);
    else return default_col;
}

GTV_LOCAL void GTV_Framebuffer_clear(byte *fb, byte pixel) {
    for (int32 i = 0; i < GTV_FRAMEBUFFER_ELEM_COUNT; i++)
        fb[i] = pixel;
}

GTV_LOCAL bool GTV_Framebuffer_set_pixel_idx(byte *fb, int32 idx, byte pixel) {
    if ((idx >= 0) && (idx < GTV_FRAMEBUFFER_ELEM_COUNT)) {
        fb[idx] = pixel;
        return true;
    }
    else return false;
}

GTV_LOCAL bool GTV_Framebuffer_set_pixel_xy(byte *fb, int32 x, int32 y, byte pixel) {
    if ((x < 0) && (x >= GTV_FRAMEBUFFER_WIDTH)) return false;
    if ((y < 0) && (y >= GTV_FRAMEBUFFER_HEIGHT)) return false;

    int32 idx = y * GTV_FRAMEBUFFER_WIDTH + x;
    if ((idx >= 0) && (idx < GTV_FRAMEBUFFER_ELEM_COUNT)) {
        GTV_Framebuffer_set_pixel_idx(fb, idx, pixel);
        return true;
    }
    else return false;
}

/* -- Framebuffer ------------------------------------------------------------------------------- */

/* -- Sprites ----------------------------------------------------------------------------------- */

typedef struct GTV_Sprite {
    int32 width, height;
    byte *data;
} GTV_Sprite;

GTV_LOCAL void 
GTV_Sprite_blit_to_framebuffer(byte *fb, GTV_Sprite sprite, int32 pos_x, int32 pos_y) {
    for (int32 y = 0; y < sprite.height; y++) {
        for (int32 x = 0; x < sprite.width; x++) {
            byte pixel = sprite.data[y * sprite.width + x];
            GTV_Framebuffer_set_pixel_xy(fb, (x + pos_x), (y + pos_y), pixel);
        }
    }
}

// TODO sprite manager
byte sprite_data[16 * 16] = {
    0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,  0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
    0xFF, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xFF,
    0xFF, 0x00, 0x00, 0xFF, 0xFF, 0x00, 0x00, 0x00,  0x00, 0x00, 0x00, 0xFF, 0xFF, 0x00, 0x00, 0xFF,
    0xFF, 0x00, 0x00, 0xFF, 0xFF, 0x00, 0x00, 0x00,  0x00, 0x00, 0x00, 0xFF, 0xFF, 0x00, 0x00, 0xFF,
    0xFF, 0x00, 0x00, 0xFF, 0xFF, 0x00, 0x00, 0x00,  0x00, 0x00, 0x00, 0xFF, 0xFF, 0x00, 0x00, 0xFF,
    0xFF, 0x00, 0x00, 0xFF, 0xFF, 0x00, 0x00, 0x00,  0x00, 0x00, 0x00, 0xFF, 0xFF, 0x00, 0x00, 0xFF,
    0xFF, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xFF,
    0xFF, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xFF,  0xFF, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xFF,

    0xFF, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xFF,
    0xFF, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xFF,
    0xFF, 0x00, 0x00, 0x00, 0xFF, 0x00, 0x00, 0x00,  0x00, 0x00, 0x00, 0xFF, 0x00, 0x00, 0x00, 0xFF,
    0xFF, 0x00, 0x00, 0x00, 0x00, 0xFF, 0x00, 0x00,  0x00, 0x00, 0xFF, 0x00, 0x00, 0x00, 0x00, 0xFF,
    0xFF, 0x00, 0x00, 0x00, 0x00, 0xFF, 0xFF, 0xFF,  0xFF, 0xFF, 0xFF, 0x00, 0x00, 0x00, 0x00, 0xFF,
    0xFF, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xFF,
    0xFF, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xFF,
    0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,  0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
};

/* -- Sprites ----------------------------------------------------------------------------------- */

/* -- AABB -------------------------------------------------------------------------------------- */

typedef struct GTV_AABB {
    float x, y, w, h;
} GTV_AABB;

#define GTV_AABB_COLLECTION_COUNT ((int32)4)
typedef struct GTV_AABB_Collection {
    GTV_AABB elems[GTV_AABB_COLLECTION_COUNT];
} GTV_AABB_Collection;

GTV_LOCAL bool GTV_AABB_intersect(GTV_AABB a, GTV_AABB b) {
    float a_min_x = a.x,
        a_max_x = a.x + a.w,
        a_min_y = a.y,
        a_max_y = a.y + a.h,

        b_min_x = b.x,
        b_max_x = b.x + b.w,
        b_min_y = b.y,
        b_max_y = b.y + b.h;

    return
        a_min_x < b_max_x &&
        a_max_x > b_min_x &&
        a_min_y < b_max_y &&
        a_max_y > b_min_y;
}

GTV_LOCAL bool
GTV_AABB_player_intersects_boxes(GTV_AABB player, GTV_AABB_Collection boxes) {
    for (int32 i = 0; i < GTV_AABB_COLLECTION_COUNT; i++) {
        if (GTV_AABB_intersect(player, boxes.elems[i])) return true;
    }
    return false;
}

GTV_LOCAL bool GTV_AABB_draw(GTV_AABB box, byte *fb, byte color) {
    bool success = true;
    int32
        box_x = (int32)box.x,
        box_y = (int32)box.y,
        box_w = (int32)box.w,
        box_h = (int32)box.h;

    for (int32 x = box_x; x < box_x + box_w; x++) {
        success &= GTV_Framebuffer_set_pixel_xy(fb, x, box_y,             color); // Top
        success &= GTV_Framebuffer_set_pixel_xy(fb, x, box_y + box_h - 1, color); // Bottom
    }

    for (int32 y = box_y; y < box_y + box_h; y++) {
        success &= GTV_Framebuffer_set_pixel_xy(fb, box_x,             y, color); // Left
        success &= GTV_Framebuffer_set_pixel_xy(fb, box_x + box_w - 1, y, color); // Right
    }

    return success;
}

/* -- AABB -------------------------------------------------------------------------------------- */

/* -- Game -------------------------------------------------------------------------------------- */

typedef struct GTV_Player {
    // TOOD replace `gravy` with `grav_y`
    float vx, vy, gravy, jmpy, input_vx;
    bool grounded;
    GTV_Sprite sprite;
    GTV_AABB bounds;
} GTV_Player;

GTV_LOCAL void GTV_Player_init(GTV_Player *player) {
    player->vx = 0.0f;
    player->vy = 1.0f;
    player->gravy = 5.0f;
    player->jmpy = 1.0f;
    player->input_vx = 2.0f;
    player->grounded = false;

    player->sprite.width = 16;
    player->sprite.height = 16;
    player->sprite.data = sprite_data;

    player->bounds.x = 0.0f;
    player->bounds.y = 0.0f;
    player->bounds.w = (float)player->sprite.width;
    player->bounds.h = (float)player->sprite.height;
}

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

// TODO third argument should be the AABB system, which means...
// TODO make an AABB system
GTV_LOCAL void
GTV_Player_move_x(
    GTV_Player *player_prev_state,
    GTV_KeyboardInput *input,
    GTV_AABB_Collection test_boxes
) {
    GTV_Player player = *player_prev_state;
    bool should_apply_future_state = true;
    
    if (input->arrow_keys[GTV_KEYBOARD_INPUT_ARROW_KEY_RIGHT]) {
        player.vx = player.input_vx;
    } else if (input->arrow_keys[GTV_KEYBOARD_INPUT_ARROW_KEY_LEFT]) {
        player.vx = -player.input_vx;
    } else {
        player.vx = 0;
    }

    player.bounds.x += player.vx;

    if ((player.bounds.x < 0) ||
        ((player.bounds.x + player.bounds.w) >= (GTV_FRAMEBUFFER_WIDTH)) ||
        (GTV_AABB_player_intersects_boxes(player.bounds, test_boxes)))
    {
        should_apply_future_state = false;
    }

    if (should_apply_future_state) *player_prev_state = player;
}

GTV_LOCAL void
GTV_Player_move_y(
    GTV_Player *player_prev_state,
    GTV_KeyboardInput *input,
    GTV_AABB_Collection test_boxes
) {
    GTV_Player player = *player_prev_state;
    bool should_apply_future_state = true;

    if (input->arrow_keys[GTV_KEYBOARD_INPUT_ARROW_KEY_DOWN]) {
        // TODO: increase vy slightly
    } else if (input->arrow_keys[GTV_KEYBOARD_INPUT_ARROW_KEY_UP]) {
        // jump
        player.vy = -player.jmpy;
    }

    // gravity
    player.bounds.y += player.vy;

    if ((player.bounds.y + player.bounds.h > GTV_FRAMEBUFFER_HEIGHT) ||
        (player.bounds.y <= 0) ||
        (GTV_AABB_player_intersects_boxes(player.bounds, test_boxes)))
    {
        should_apply_future_state = false;
    }

    if (should_apply_future_state) *player_prev_state = player;
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

GTV_LOCAL void GTV_draw_all(GTV_GameStateInterface *interface) {
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


/* -- Game -------------------------------------------------------------------------------------- */

/* -- Game state interface ---------------------------------------------------------------------- */

GTV_EXPORT void GTV_GameStateInterface_init(GTV_GameStateInterface *interface) {
    interface->should_exit = 0;
    
    for (int32 c = 0; c < GTV_COLOR_PALETTE_SIZE; c++) {
        interface->current_palette.colors[c].r = (byte)0;
        interface->current_palette.colors[c].g = (byte)0;
        interface->current_palette.colors[c].b = (byte)0;
    }

    interface->private = malloc(sizeof (GTV_PrivateGameState));
    GTV_PrivateGameState_init(interface->private);
}

GTV_EXPORT void GTV_GameStateInterface_update(GTV_GameStateInterface *interface) {
    interface->current_palette.colors[0xFF].r = 0x00;
    interface->current_palette.colors[0xFF].g = 0xFF;
    interface->current_palette.colors[0xFF].b = 0x00;

    interface->current_palette.colors[0xFE].r = 0xFF;
    interface->current_palette.colors[0xFE].g = 0x00;
    interface->current_palette.colors[0xFE].b = 0x00;

    GTV_Framebuffer_clear(interface->framebuffer, 0);
    GTV_update_gameplay(interface);
    GTV_draw_all(interface);
}

GTV_EXPORT void GTV_GameStateInterface_cleanup(GTV_GameStateInterface *interface) {
    free(interface->private);
}

/* -- Game state interface ---------------------------------------------------------------------- */
