#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include "GTV_game.h"

/* -- Utility ------------------------------------------------------------------------------------*/
// TODO replace malloc with an external (arena) allocator

typedef int32_t int32;

/* -- Utility ------------------------------------------------------------------------------------*/

/* -- Colors ------------------------------------------------------------------------------------ */

typedef struct GTV_Color GTV_Color;
typedef struct GTV_ColorPalette GTV_ColorPalette;
typedef struct GTV_ColorPaletteCollection GTV_ColorPaletteCollection;

GTV_LOCAL GTV_Color get_color_from_palette(GTV_ColorPalette palette, byte color) {
    GTV_Color default_col = { 0 }; /* The void */
    if ((color >= 0) && (color < GTV_FRAMEBUFFER_ELEM_COUNT))
        return palette.colors[color];
    else return default_col;
}

/* -- Colors ------------------------------------------------------------------------------------ */

/* -- Framebuffer ------------------------------------------------------------------------------- */

// TODO get a better naming convention for functions in this section

GTV_Color GTV_get_color_from_framebuffer(byte *fb, GTV_ColorPalette curr_palette, int idx) {
    GTV_Color default_col = { 0xFF, 0x00, 0xFF }; /* Vomit-worthy magenta */
    if ((idx >= 0) && (idx < GTV_FRAMEBUFFER_ELEM_COUNT))
        return get_color_from_palette(curr_palette, fb[idx]); /* TODO ??? */
    else return default_col;
}

GTV_LOCAL void framebuffer_clear(byte *fb, byte pixel) {
    for (int i = 0; i < GTV_FRAMEBUFFER_ELEM_COUNT; i++)
        fb[i] = pixel;
}

GTV_LOCAL bool framebuffer_set_pixel_idx(byte *fb, int idx, byte pixel) {
    if ((idx >= 0) && (idx < GTV_FRAMEBUFFER_ELEM_COUNT)) {
        fb[idx] = pixel;
        return true;
    }
    else return false;
}

GTV_LOCAL bool framebuffer_set_pixel_xy(byte *fb, int x, int y, byte pixel) {
    if ((x < 0) && (x >= GTV_FRAMEBUFFER_WIDTH)) return false;
    if ((y < 0) && (y >= GTV_FRAMEBUFFER_HEIGHT)) return false;

    int idx = y * GTV_FRAMEBUFFER_WIDTH + x;
    if ((idx >= 0) && (idx < GTV_FRAMEBUFFER_ELEM_COUNT)) {
        framebuffer_set_pixel_idx(fb, idx, pixel);
        return true;
    }
    else return false;
}

/* -- Framebuffer ------------------------------------------------------------------------------- */

/* -- Sprites ----------------------------------------------------------------------------------- */

typedef struct GTV_Sprite {
    int width, height;
    byte *data;
} GTV_Sprite;

GTV_LOCAL void GTV_Sprite_blit_to_framebuffer(byte *fb, GTV_Sprite sprite, int pos_x, int pos_y) {
    for (int y = 0; y < sprite.height; y++) {
        for (int x = 0; x < sprite.width; x++) {
            byte pixel = sprite.data[y * sprite.width + x];
            framebuffer_set_pixel_xy(
                fb,
                x + pos_x,
                y + pos_y,
                pixel
            );
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

/* -- Game -------------------------------------------------------------------------------------- */

typedef struct GTV_AABB {
    float x, y, w, h;
} GTV_AABB;

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

GTV_LOCAL bool GTV_AABB_draw(GTV_AABB box, byte *fb, byte color) {
    bool success = true;
    int box_x = box.x,
        box_y = box.y,
        box_w = box.w,
        box_h = box.h;

    for (int x = box_x; x < box_x + box_w; x++) {
        success &= framebuffer_set_pixel_xy(fb, x, box_y,             color); // Top
        success &= framebuffer_set_pixel_xy(fb, x, box_y + box_h - 1, color); // Bottom
    }

    for (int y = box_y; y < box_y + box_h; y++) {
        success &= framebuffer_set_pixel_xy(fb, box_x,             y, color); // Left
        success &= framebuffer_set_pixel_xy(fb, box_x + box_w - 1, y, color); // Right
    }

    return success;
}


typedef struct GTV_Player {
    // Positional values are 16.16 fixed point
    // TODO replace `gravy` with `grav_y`
    float   vx, vy,
            px, py,
            gravy, jmpy, input_vx;
    bool grounded;
    GTV_Sprite sprite;
    GTV_AABB bounds;
} GTV_Player;

typedef struct GTV_PrivateGameState {
    GTV_Player player;
    GTV_AABB test_box;
} GTV_PrivateGameState;

GTV_LOCAL void GTV_PrivateGameState_init(GTV_PrivateGameState *state) {
    state->player.px = 000;
    state->player.py = /*(240 - 5)*/0;
    state->player.vx = 0;
    state->player.vy = 1;
    state->player.gravy = 5;
    state->player.jmpy = 1;
    state->player.input_vx = 2;
    state->player.grounded = false;

    state->player.sprite.width = 16;
    state->player.sprite.height = 16;
    state->player.sprite.data = sprite_data;

    state->player.bounds.x = state->player.px;
    state->player.bounds.y = state->player.py;
    state->player.bounds.w = state->player.sprite.width;
    state->player.bounds.h = state->player.sprite.height;

    state->test_box.x = 128;
    state->test_box.y = 128;
    state->test_box.w = 10;
    state->test_box.h = 256;
}

// TODO third argument should be the AABB system, which means...
// TODO make an AABB system
GTV_LOCAL void
GTV_Player_move_x(GTV_Player *player_prev_state, GTV_KeyboardInput *input, GTV_AABB test_box) {
    GTV_Player player = *player_prev_state;
    bool should_apply_future_state = true;
    
    if (input->arrow_keys[GTV_KEYBOARD_INPUT_ARROW_KEY_RIGHT]) {
        player.vx = player.input_vx;
    } else if (input->arrow_keys[GTV_KEYBOARD_INPUT_ARROW_KEY_LEFT]) {
        player.vx = -player.input_vx;
    } else {
        player.vx = 0;
    }

    player.px += player.vx;
    player.bounds.x = player.px;

    if ((player.px < 0) ||
        ((player.px + player.bounds.w) >= (GTV_FRAMEBUFFER_WIDTH)) ||
        (GTV_AABB_intersect(player.bounds, test_box)))
    {
        should_apply_future_state = false;
    }

    if (should_apply_future_state) *player_prev_state = player;
}

GTV_LOCAL void
GTV_Player_move_y(GTV_Player *player_prev_state, GTV_KeyboardInput *input, GTV_AABB test_box) {
    GTV_Player player = *player_prev_state;
    bool should_apply_future_state = true;

    if (input->arrow_keys[GTV_KEYBOARD_INPUT_ARROW_KEY_DOWN]) {
        // TODO: increase vy slightly
    } else if (input->arrow_keys[GTV_KEYBOARD_INPUT_ARROW_KEY_UP]) {
        // jump
        player.vy = -player.jmpy;
    }

    // gravity
    player.py += player.vy;
    player.bounds.y = player.py;

    // TODO move position information from player to bounds
    if ((player.bounds.y + player.bounds.h > GTV_FRAMEBUFFER_HEIGHT) ||
        (player.bounds.y <= 0) ||
        (GTV_AABB_intersect(player.bounds, test_box)))
    {
        should_apply_future_state = false;
    }

    if (should_apply_future_state) *player_prev_state = player;
}

GTV_LOCAL void GTV_update_gameplay(GTV_GameStateInterface *interface) {
    GTV_Player_move_x(
        &interface->private->player,
        &interface->keyboard_input,
        interface->private->test_box
    );

    GTV_Player_move_y(
        &interface->private->player,
        &interface->keyboard_input,
        interface->private->test_box
    );

    // TODO implement gravity and jumping
    // TODO find a way to diffrentiate being grounded to touching a wall's sides
}

GTV_LOCAL void GTV_draw_all(GTV_GameStateInterface *interface) {
    GTV_Player player = interface->private->player;

    GTV_Sprite_blit_to_framebuffer(
        interface->framebuffer,
        player.sprite,
        player.px,
        player.py
    );

    GTV_AABB_draw(player.bounds, interface->framebuffer, 0xFE);
    GTV_AABB_draw(interface->private->test_box, interface->framebuffer, 0xFE);
}


/* -- Game -------------------------------------------------------------------------------------- */

/* -- Game state interface ---------------------------------------------------------------------- */

GTV_EXPORT void GTV_GameStateInterface_init(GTV_GameStateInterface *interface) {
    interface->should_exit = 0;
    
    for (int c = 0; c < GTV_COLOR_PALETTE_SIZE; c++) {
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

    framebuffer_clear(interface->framebuffer, 0);
    GTV_update_gameplay(interface);
    GTV_draw_all(interface);
}

GTV_EXPORT void GTV_GameStateInterface_cleanup(GTV_GameStateInterface *interface) {
    free(interface->private);
}

/* -- Game state interface ---------------------------------------------------------------------- */
