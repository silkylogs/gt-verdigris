#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include "GTV_game.h"

/* -- Utility ------------------------------------------------------------------------------------*/
// TODO replace malloc with an external (arena) allocator
// TODO prefix file-local functions with either "static" or a suitable define
// TODO replace int with int32 (and int with bool)

#define SCALING_FACTOR ((int)10)

static int int_to_fp(int integer) {
    return integer * SCALING_FACTOR;
} 

static int fp_to_int(int fixed_point) {
    return fixed_point / SCALING_FACTOR;
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
    player->px = 200;
    player->py = 240 - 5;
    player->gravy = 1; // Manually converted to FP
    player->jmpy = int_to_fp(3);
    player->input_vx = int_to_fp(2);
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

void GTV_draw_all(GTV_GameStateInterface *interface);

/*
void GTV_update_gameplay(GTV_GameStateInterface *interface) {
    GTV_Player *player = &interface->private->player;
    GTV_Sprite *sprite = &interface->private->player.sprite;
    GTV_KeyboardInput kb_input = interface->keyboard_input;
    byte *framebuffer = interface->framebuffer;


    GTV_Player next_potential_player_state = *player;
    bool apply_next_potential_player_state = true;

    // TODO instead of converting to fixed every frame find a way to keep it fixed all the time
    //      and only convert to int for rendering
    next_potential_player_state.px = int_to_fp(next_potential_player_state.px);
    next_potential_player_state.py = int_to_fp(next_potential_player_state.py);


    // NOTE intregrate collisions with input instead of having it as a seperate stage?
    int player_max_x = next_potential_player_state.px + int_to_fp(sprite->width),
        player_min_x = next_potential_player_state.px,
        player_max_y = next_potential_player_state.py + int_to_fp(sprite->height),
        player_min_y = next_potential_player_state.py;
    

    if (kb_input.arrow_keys[GTV_KEYBOARD_INPUT_ARROW_KEY_RIGHT]) {
        next_potential_player_state.vx = next_potential_player_state.input_vx;
    } else if (kb_input.arrow_keys[GTV_KEYBOARD_INPUT_ARROW_KEY_LEFT]) {
        next_potential_player_state.vx = -next_potential_player_state.input_vx;
    } else {
        next_potential_player_state.vx = 0;
    }
    if (kb_input.arrow_keys[GTV_KEYBOARD_INPUT_ARROW_KEY_DOWN]) {
        // TODO
    } else if (kb_input.arrow_keys[GTV_KEYBOARD_INPUT_ARROW_KEY_UP]) {
        if (next_potential_player_state.grounded) {
            next_potential_player_state.vy = -next_potential_player_state.jmpy;
            next_potential_player_state.grounded = false;
        }
    } else {
        // TODO
    }
    
    // Gravity
    if (next_potential_player_state.grounded) {} //player->vy = 0;
    else { next_potential_player_state.vy += next_potential_player_state.gravy; }

    // -- AABB test ---------------------------------------------------------------

    int box_x = int_to_fp(128),
        box_y = int_to_fp(0),
        box_w = int_to_fp(10),
        box_h = int_to_fp(256);
    
    int box_max_x = box_x + box_w,
        box_min_x = box_x,
        box_max_y = box_y + box_h,
        box_min_y = box_y;

    framebuffer_draw_aabb(
        framebuffer, 0xFE,
        fp_to_int(box_x),
        fp_to_int(box_y),
        fp_to_int(box_w),
        fp_to_int(box_h)
    );

    if (AABB_intersect(
        player_min_x, player_min_y, player_max_x, player_max_y,
        box_min_x, box_min_y, box_max_x, box_max_y
    )) {
        printf("Intersection\n");
        apply_next_potential_player_state = false;
        // TODO resolve collision
    }

    // -- AABB test ---------------------------------------------------------------

    int boundary_right_lim = int_to_fp(GTV_FRAMEBUFFER_WIDTH),
        boundary_left_lim = 0,
        boundary_down_lim = int_to_fp(GTV_FRAMEBUFFER_HEIGHT),
        boundary_up_lim = 0;

    // Boundary collision detection and handling
    if (player_max_x > boundary_right_lim) {
        // printf("Right side collision\n");
        next_potential_player_state.px = boundary_right_lim - int_to_fp(sprite->width);
        next_potential_player_state.vx = 0;
        apply_next_potential_player_state = false;
    }
    if (player_min_x < 0) {
        // printf("Left side collision\n");
        next_potential_player_state.px = 0;
        next_potential_player_state.vx = 0;
        apply_next_potential_player_state = false;
    }
    if (player_max_y > boundary_down_lim) {
        // printf("Bottom side collision\n");
        next_potential_player_state.py = boundary_down_lim - int_to_fp(sprite->height);
        next_potential_player_state.vy = 0;
        next_potential_player_state.grounded = true;
        apply_next_potential_player_state = false;
    } else {
        next_potential_player_state.grounded = false;
    }
    if (player_min_y < boundary_up_lim) {
        // printf("Top side collision\n");
        next_potential_player_state.py = 0;
        next_potential_player_state.vy = 0;
        apply_next_potential_player_state = false;
    }


    next_potential_player_state.px += next_potential_player_state.vx;
    next_potential_player_state.py += next_potential_player_state.vy;

    // Conversion to integer
    next_potential_player_state.px = fp_to_int(next_potential_player_state.px);
    next_potential_player_state.py = fp_to_int(next_potential_player_state.py);

    if (apply_next_potential_player_state) {
        *player = next_potential_player_state;
    }

    GTV_Sprite_blit_to_framebuffer(
        framebuffer,
        player->sprite,
        player->px,
        player->py
    );
}*/

void GTV_update_gameplay(GTV_GameStateInterface *interface) {

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
    // GTV_PrivateGameState_update(interface->private, interface->keyboard_input, interface->framebuffer);
    GTV_update_gameplay(interface);
}

void GTV_GameStateInterface_cleanup(GTV_GameStateInterface *interface) {
    free(interface->private);
}

/* -- Game state interface ---------------------------------------------------------------------- */
