#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include "GTV_game.h"

/* -- Utility ------------------------------------------------------------------------------------*/
// TODO replace malloc with an external (arena) allocator

#define GTV_LOCAL static
#define GTV_EXPORT extern
typedef int32_t int32;

#define SCALING_FACTOR ((int)10)

// TODO complete fixed point conversion
typedef struct fix { int32 val; } fix;

GTV_LOCAL fix int_to_fp(int32 integer) {
    return (fix){ integer * SCALING_FACTOR };
} 

GTV_LOCAL int32 fp_to_int(fix fixed_point) {
    return fixed_point.val / SCALING_FACTOR;
}

GTV_LOCAL fix fp_add(fix x, fix y);
GTV_LOCAL fix fp_sub(fix x, fix y);
GTV_LOCAL fix fp_mul(fix x, fix y);
GTV_LOCAL fix fp_div(fix x, fix y);
GTV_LOCAL fix fp_gt(fix x, fix y);
GTV_LOCAL fix fp_lt(fix x, fix y);
GTV_LOCAL fix fp_gte(fix x, fix y);
GTV_LOCAL fix fp_lte(fix x, fix y);

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
    fix x, y, w, h;
} GTV_AABB;

GTV_LOCAL bool GTV_AABB_intersect(GTV_AABB a, GTV_AABB b) {
    fix a_min_x = a.x,
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
    box.x = fp_to_int(box.x);
    box.y = fp_to_int(box.y);
    box.w = fp_to_int(box.w);
    box.h = fp_to_int(box.h);

    for (int x = (box.x); x < (box.x + box.w); x++) {
        success &= framebuffer_set_pixel_xy(fb, x, box.y,             color); // Top
        success &= framebuffer_set_pixel_xy(fb, x, box.y + box.h - 1, color); // Bottom
    }

    for (int y = box.y; y < box.y + box.h; y++) {
        success &= framebuffer_set_pixel_xy(fb, box.x,             y, color); // Left
        success &= framebuffer_set_pixel_xy(fb, box.x + box.w - 1, y, color); // Right
    }

    return success;
}


typedef struct GTV_Player {
    // Positional values are 16.16 fixed point
    // TODO replace `gravy` with `grav_y`
    int vx, vy, px, py, gravy, jmpy, input_vx;
    bool grounded;
    GTV_Sprite sprite;
    GTV_AABB bounds;
} GTV_Player;

typedef struct GTV_PrivateGameState {
    GTV_Player player;
    GTV_AABB test_box;
} GTV_PrivateGameState;

GTV_LOCAL void GTV_PrivateGameState_init(GTV_PrivateGameState *state) {
    state->player.vx = int_to_fp(0);
    state->player.vy = int_to_fp(0);
    state->player.px = int_to_fp(000);
    state->player.py = int_to_fp(240 - 5);
    state->player.gravy = (5); // Manually converted to FP
    state->player.jmpy = int_to_fp(3);
    state->player.input_vx = int_to_fp(2);
    state->player.grounded = false;

    state->player.sprite.width = 16;
    state->player.sprite.height = 16;
    state->player.sprite.data = sprite_data;

    state->player.bounds.x = state->player.px;
    state->player.bounds.y = state->player.py;
    state->player.bounds.w = int_to_fp(state->player.sprite.width);
    state->player.bounds.h = int_to_fp(state->player.sprite.height);

    state->test_box.x = int_to_fp(128);
    state->test_box.y = int_to_fp(0);
    state->test_box.w = int_to_fp(10);
    state->test_box.h = int_to_fp(256);
}

GTV_LOCAL void GTV_update_gameplay(GTV_GameStateInterface *interface) {
    GTV_KeyboardInput *input = &interface->keyboard_input;
    GTV_Player *player_prev_state = &interface->private->player;
    GTV_Player player = *player_prev_state;

    // TODO since any further calculations about whether to apply future state
    //      just after setting this variable to false, why not use a goto instead?
    bool should_apply_future_state = true;

    if (input->arrow_keys[GTV_KEYBOARD_INPUT_ARROW_KEY_RIGHT]) {
        player.vx = player.input_vx;
    } else if (input->arrow_keys[GTV_KEYBOARD_INPUT_ARROW_KEY_LEFT]) {
        player.vx = -player.input_vx;
    } else {
        player.vx = 0;
    }

    // TODO implement gravity and jumping

    player.px += player.vx;
    player.py += player.vy;
    player.bounds.x = player.px;
    player.bounds.y = player.py;

    // TODO find a way to diffrentiate being grounded to touching a wall's sides
    if (
        (player.px < 0) ||
        (player.px + int_to_fp(player.sprite.width) >= int_to_fp(GTV_FRAMEBUFFER_WIDTH)) ||
        (GTV_AABB_intersect(player.bounds, interface->private->test_box))
    ) {
        should_apply_future_state = false;
    }

    if (should_apply_future_state) *player_prev_state = player;
}

GTV_LOCAL void GTV_draw_all(GTV_GameStateInterface *interface) {
    GTV_Player player = interface->private->player;

    GTV_Sprite_blit_to_framebuffer(
        interface->framebuffer,
        player.sprite,
        fp_to_int(player.px),
        fp_to_int(player.py)
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
