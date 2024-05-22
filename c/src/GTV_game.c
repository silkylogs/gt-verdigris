#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include "GTV_game.h"

/* -- Utility ------------------------------------------------------------------------------------*/
// TODO replace malloc with an external (arena) allocator
// TODO prefix file-local functions with either "static" or a suitable define
// TODO replace int with int32 (and int with bool)

static int int_to_fp(int integer, int scaling_factor) {
    return integer * scaling_factor;
} 

static int fp_to_int(int fixed_point, int scaling_factor) {
    return fixed_point / scaling_factor;
}

/* -- Utility ------------------------------------------------------------------------------------*/

/* -- Colors ------------------------------------------------------------------------------------ */

typedef struct GTV_Color GTV_Color;
typedef struct GTV_ColorPalette GTV_ColorPalette;
typedef struct GTV_ColorPaletteCollection GTV_ColorPaletteCollection;

GTV_Color get_color_from_palette(GTV_ColorPalette palette, byte color) {
    GTV_Color default_col = { 0 }; /* The void */
    if ((color >= 0) && (color < GTV_FRAMEBUFFER_ELEM_COUNT))
        return palette.colors[color];
    else return default_col;
}

/* -- Colors ------------------------------------------------------------------------------------ */

/* -- Framebuffer ------------------------------------------------------------------------------- */

GTV_Color GTV_get_color_from_framebuffer(byte *fb, GTV_ColorPalette curr_palette, int idx) {
    GTV_Color default_col = { 0xFF, 0x00, 0xFF }; /* Vomit-worthy magenta */
    if ((idx >= 0) && (idx < GTV_FRAMEBUFFER_ELEM_COUNT))
        return get_color_from_palette(curr_palette, fb[idx]); /*todo*/
    else return default_col;
}

void framebuffer_clear(byte *fb, byte pixel) {
    for (int i = 0; i < GTV_FRAMEBUFFER_ELEM_COUNT; i++)
        fb[i] = pixel;
}

bool framebuffer_set_pixel_idx(byte *fb, int idx, byte pixel) {
    if ((idx >= 0) && (idx < GTV_FRAMEBUFFER_ELEM_COUNT)) {
        fb[idx] = pixel;
        return true;
    }
    else return false;
}

bool framebuffer_set_pixel_xy(byte *fb, int x, int y, byte pixel) {
    if ((x < 0) && (x >= GTV_FRAMEBUFFER_WIDTH)) return false;
    if ((y < 0) && (y >= GTV_FRAMEBUFFER_HEIGHT)) return false;

    int idx = y * GTV_FRAMEBUFFER_WIDTH + x;
    if ((idx >= 0) && (idx < GTV_FRAMEBUFFER_ELEM_COUNT)) {
        framebuffer_set_pixel_idx(fb, idx, pixel);
        return true;
    }
    else return false;
}

bool framebuffer_draw_aabb(byte *fb, byte pixel, int x, int y, int w, int h) {
    bool success = true;

    for (int i = x; i <= x + w; i++) {
        success &= framebuffer_set_pixel_xy(fb, i, y, pixel);   // Top
        success &= framebuffer_set_pixel_xy(fb, i, y+h, pixel); // Bottom
    }

    for (int i = y; i <= y + h; i++) {
        success &= framebuffer_set_pixel_xy(fb, x, i, pixel);   // Left
        success &= framebuffer_set_pixel_xy(fb, x+w, i, pixel); // Right
    }

    return success;
}

/* -- Framebuffer ------------------------------------------------------------------------------- */

/* -- Sprites ----------------------------------------------------------------------------------- */

typedef struct GTV_Sprite {
    int width, height;
    byte *data;
} GTV_Sprite;

void GTV_Sprite_blit_to_framebuffer(byte *fb, GTV_Sprite sprite, int pos_x, int pos_y) {
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

#define SCALING_FACTOR ((int)16)
typedef struct GTV_Player {
    // Positional values are 16.16 fixed point
    // TODO replace `gravy` with `grav_y`
    int vx, vy, px, py, gravy, jmpy, input_vx;
    bool grounded;
    GTV_Sprite sprite;
} GTV_Player;

struct GTV_PrivateGameState {
    GTV_Player player;
};

void GTV_PrivateGameState_init(GTV_PrivateGameState *state) {
    GTV_Player *player = &state->player;
    player->vx = 0;
    player->vy = 0;
    player->px = 240;
    player->py = 0;
    player->gravy = 1; // Manually converted to FP
    player->jmpy = int_to_fp(3, SCALING_FACTOR);
    player->input_vx = int_to_fp(2, SCALING_FACTOR);
    player->grounded = false;

    GTV_Sprite *sprite = &state->player.sprite;
    sprite->width = 16;
    sprite->height = 16;
    sprite->data = sprite_data;
}


bool AABB_intersect(
    int a_min_x, int a_min_y, int a_max_x, int a_max_y,
    int b_min_x, int b_min_y, int b_max_x, int b_max_y
) {
    return
        a_min_x <= b_max_x &&
        a_max_x >= b_min_x &&
        a_min_y <= b_max_y &&
        a_max_y >= b_min_y;
}

void GTV_PrivateGameState_update(GTV_PrivateGameState *state, GTV_KeyboardInput kb_input, byte *framebuffer) {
    GTV_Player *player = &state->player;
    GTV_Sprite *sprite = &state->player.sprite;

    GTV_Sprite_blit_to_framebuffer(
        framebuffer,
        player->sprite,
        player->px,
        player->py
    );

    player->px = int_to_fp(player->px, SCALING_FACTOR);
    player->py = int_to_fp(player->py, SCALING_FACTOR);

    // Axial input
    if (kb_input.arrow_keys[GTV_KEYBOARD_INPUT_ARROW_KEY_RIGHT]) {
        player->vx = player->input_vx;
    } else if (kb_input.arrow_keys[GTV_KEYBOARD_INPUT_ARROW_KEY_LEFT]) {
        player->vx = -player->input_vx;
    } else {
        player->vx = 0;
    }
    if (kb_input.arrow_keys[GTV_KEYBOARD_INPUT_ARROW_KEY_DOWN]) {
        // player->vy = int_to_fp(1, SCALING_FACTOR);
    } else if (kb_input.arrow_keys[GTV_KEYBOARD_INPUT_ARROW_KEY_UP]) {
        // player->vy = int_to_fp(-1, SCALING_FACTOR);
        if (player->grounded) {
            player->vy = -player->jmpy;
            player->grounded = false;
        }
    } else {
        // player->vy = 0;
    }

    int player_max_x = player->px + int_to_fp(sprite->width, SCALING_FACTOR),
        player_min_x = player->px,
        player_max_y = player->py + int_to_fp(sprite->height, SCALING_FACTOR),
        player_min_y = player->py;

    // -- AABB test ---------------------------------------------------------------
    
    int box_x = int_to_fp(128, SCALING_FACTOR),
        box_y = int_to_fp(0, SCALING_FACTOR),
        box_w = int_to_fp(10, SCALING_FACTOR),
        box_h = int_to_fp(256, SCALING_FACTOR);
    
    int box_max_x = box_x + box_w,
        box_min_x = box_x,
        box_max_y = box_y + box_h,
        box_min_y = box_y;

    framebuffer_draw_aabb(
        framebuffer, 0xFE,
        fp_to_int(box_x, SCALING_FACTOR),
        fp_to_int(box_y, SCALING_FACTOR),
        fp_to_int(box_w, SCALING_FACTOR),
        fp_to_int(box_h, SCALING_FACTOR)
    );

    if (AABB_intersect(
        player_min_x, player_min_y, player_max_x, player_max_y,
        box_min_x, box_min_y, box_max_x, box_max_y
    )) {
        printf("Intersection\n");
        // TODO resolve collision
    }

    // -- AABB test ---------------------------------------------------------------

    int boundary_right_lim = int_to_fp(GTV_FRAMEBUFFER_WIDTH, SCALING_FACTOR),
        boundary_left_lim = 0,
        boundary_down_lim = int_to_fp(GTV_FRAMEBUFFER_HEIGHT, SCALING_FACTOR),
        boundary_up_lim = 0;

    // Boundary collision detection and handling
    if (player_max_x > boundary_right_lim) {
        // printf("Right side collision\n");
        player->px = boundary_right_lim - int_to_fp(sprite->width, SCALING_FACTOR);
        player->vx = 0;
    }
    if (player_min_x < 0) {
        // printf("Left side collision\n");
        player->px = 0;
        player->vx = 0;
    }
    if (player_max_y > boundary_down_lim) {
        // printf("Bottom side collision\n");
        player->py = boundary_down_lim - int_to_fp(sprite->height, SCALING_FACTOR);
        player->vy = 0;
        player->grounded = true;
    } else {
        player->grounded = false;
    }
    if (player_min_y < boundary_up_lim) {
        // printf("Top side collision\n");
        player->py = 0;
        player->vy = 0;
    }

    // Gravity
    // printf("Player grounded? %s\n", player->grounded? "true" : "false");
    if (player->grounded) {} //player->vy = 0;
    else { player->vy += player->gravy; }

    player->px += player->vx;
    player->py += player->vy;

    // printf("Before conversion: player.x = 0x%X (%d)\n", state->player.px, state->player.px);
    // printf("Before conversion: player.y = 0x%X (%d)\n", state->player.py, state->player.py);

    // Conversion to integer
    player->px = fp_to_int(player->px, SCALING_FACTOR);
    player->py = fp_to_int(player->py, SCALING_FACTOR);

    // printf("After conversion: player.x = 0x%X (%d)\n", state->player.px, state->player.px);
    // printf("After conversion: player.y = 0x%X (%d)\n", state->player.py, state->player.py);

}

/* -- Game -------------------------------------------------------------------------------------- */

/* -- Game state interface ---------------------------------------------------------------------- */

void GTV_GameStateInterface_init(GTV_GameStateInterface *interface) {
    interface->should_exit = 0;
    
    for (int c = 0; c < GTV_COLOR_PALETTE_SIZE; c++) {
        interface->current_palette.colors[c].r = (byte)0;
        interface->current_palette.colors[c].g = (byte)0;
        interface->current_palette.colors[c].b = (byte)0;
    }

    interface->private = malloc(sizeof (GTV_PrivateGameState));
    GTV_PrivateGameState_init(interface->private);
}

void GTV_GameStateInterface_update(GTV_GameStateInterface *interface) {
    interface->current_palette.colors[0xFF].r = 0x00;
    interface->current_palette.colors[0xFF].g = 0xFF;
    interface->current_palette.colors[0xFF].b = 0x00;

    interface->current_palette.colors[0xFE].r = 0xFF;
    interface->current_palette.colors[0xFE].g = 0x00;
    interface->current_palette.colors[0xFE].b = 0x00;

    framebuffer_clear(interface->framebuffer, 0);
    GTV_PrivateGameState_update(interface->private, interface->keyboard_input, interface->framebuffer);
}

void GTV_GameStateInterface_cleanup(GTV_GameStateInterface *interface) {
    free(interface->private);
}

/* -- Game state interface ---------------------------------------------------------------------- */
