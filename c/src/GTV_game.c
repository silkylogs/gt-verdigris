#include <stdio.h>
#include <stdlib.h>
#include "GTV_game.h"

/* -- Utility ------------------------------------------------------------------------------------*/
// TODO replace malloc with an external (arena) allocator
// TODO prefix file-local functions with either "static" or a suitable define

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

int framebuffer_set_pixel_idx(byte *fb, int idx, byte pixel) {
    if ((idx >= 0) && (idx < GTV_FRAMEBUFFER_ELEM_COUNT)) {
        fb[idx] = pixel;
        return 1;
    }
    else return 0;
}

int framebuffer_set_pixel_xy(byte *fb, int x, int y, byte pixel) {
    if ((x < 0) && (x >= GTV_FRAMEBUFFER_WIDTH)) return 0;
    if ((y < 0) && (y >= GTV_FRAMEBUFFER_HEIGHT)) return 0;

    int idx = y * GTV_FRAMEBUFFER_WIDTH + x;
    if ((idx >= 0) && (idx < GTV_FRAMEBUFFER_ELEM_COUNT)) {
        framebuffer_set_pixel_idx(fb, idx, pixel);
        return 1;
    }
    else return 0;
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
    int vx, vy, px, py;
    GTV_Sprite sprite;
} GTV_Player;

struct GTV_PrivateGameState {
    GTV_Player player;
};

void GTV_PrivateGameState_init(GTV_PrivateGameState *state) {
    state->player.vx = 0;
    state->player.vy = 0;
    state->player.px = 123;
    state->player.py = 150;

    state->player.sprite.width = 16;
    state->player.sprite.height = 16;
    state->player.sprite.data = sprite_data;
}

// -- Basic platformer test ------------------------
void GTV_PrivateGameState_update(GTV_PrivateGameState *state, GTV_KeyboardInput kb_input) {
    state->player.px = int_to_fp(state->player.px, SCALING_FACTOR);
    state->player.py = int_to_fp(state->player.py, SCALING_FACTOR);

    // Axial input
    if (kb_input.arrow_keys[GTV_KEYBOARD_INPUT_ARROW_KEY_RIGHT]) {
        state->player.vx = int_to_fp(1, SCALING_FACTOR);
    } else if (kb_input.arrow_keys[GTV_KEYBOARD_INPUT_ARROW_KEY_LEFT]) {
        state->player.vx = int_to_fp(-1, SCALING_FACTOR);
    } else {
        state->player.vx = 0;
    }
    if (kb_input.arrow_keys[GTV_KEYBOARD_INPUT_ARROW_KEY_DOWN]) {
        state->player.vy = int_to_fp(1, SCALING_FACTOR);
    } else if (kb_input.arrow_keys[GTV_KEYBOARD_INPUT_ARROW_KEY_UP]) {
        state->player.vy = int_to_fp(-1, SCALING_FACTOR);
    } else {
        state->player.vy = 0;
    }

    // Rudimentary collision detection and handling
    if (state->player.px + int_to_fp(state->player.sprite.width, SCALING_FACTOR) > int_to_fp(GTV_FRAMEBUFFER_WIDTH, SCALING_FACTOR)) {
        printf("Right side collision\n");
        state->player.px = int_to_fp(GTV_FRAMEBUFFER_WIDTH, SCALING_FACTOR) - int_to_fp(state->player.sprite.width, SCALING_FACTOR);
        state->player.vx = 0;
    }
    if (state->player.px < 0) {
        printf("Left side collision\n");
        state->player.px = 0;
        state->player.vx = 0;
    }
    if (state->player.py + int_to_fp(state->player.sprite.height, SCALING_FACTOR) > int_to_fp(GTV_FRAMEBUFFER_HEIGHT, SCALING_FACTOR)) {
        printf("Bottom side collision\n");
        state->player.py = int_to_fp(GTV_FRAMEBUFFER_WIDTH, SCALING_FACTOR) - int_to_fp(state->player.sprite.height, SCALING_FACTOR);
        state->player.vy = 0;
    }
    if (state->player.py < 0) {
        printf("Top side collision\n");
        state->player.py = 0;
        state->player.vy = 0;
    }

    state->player.px += state->player.vx;
    state->player.py += state->player.vy;

    // printf("Before conversion: player.x = 0x%X (%d)\n", state->player.px, state->player.px);
    // printf("Before conversion: player.y = 0x%X (%d)\n", state->player.py, state->player.py);

    // Conversion to integer
    state->player.px = fp_to_int(state->player.px, SCALING_FACTOR);
    state->player.py = fp_to_int(state->player.py, SCALING_FACTOR);

    // printf("After conversion: player.x = 0x%X (%d)\n", state->player.px, state->player.px);
    // printf("After conversion: player.y = 0x%X (%d)\n", state->player.py, state->player.py);
}

/* -- Game -------------------------------------------------------------------------------------- */

/* -- Game state interface ---------------------------------------------------------------------- */

void GTV_GameStateInterface_init(GTV_GameStateInterface *interface) {
    interface->should_exit = 0;
    
    for (int c = 0; c < GTV_COLOR_PALETTE_SIZE; c++) {
        interface->current_palette.colors[c].r = (byte)c;
        interface->current_palette.colors[c].g = (byte)c;
        interface->current_palette.colors[c].b = (byte)c;
    }

    interface->private = malloc(sizeof (GTV_PrivateGameState));
    GTV_PrivateGameState_init(interface->private);
}

void GTV_GameStateInterface_update(GTV_GameStateInterface *interface) {
    GTV_PrivateGameState_update(interface->private, interface->keyboard_input);

    // Input test
    // if (interface->keyboard_input.arrow_keys[GTV_KEYBOARD_INPUT_ARROW_KEY_UP]) {
    //     interface->current_palette.colors[0xFF].r = 0x00;
    //     interface->current_palette.colors[0xFF].g = 0xFF;
    //     interface->current_palette.colors[0xFF].b = 0x00;
    // }
    // else {
    //     interface->current_palette.colors[0xFF].r = 0xFF;
    //     interface->current_palette.colors[0xFF].g = 0x00;
    //     interface->current_palette.colors[0xFF].b = 0x00;
    // }
    // Input test

    // Color cycling test
    // interface->current_palette.colors[0xFF].r = interface->private->player.px;
    // interface->current_palette.colors[0xFF].g = interface->private->player.py;
    // interface->current_palette.colors[0xFF].b = interface->private->player.px +
    //                                             interface->private->player.py;
    
    // Set framebuffer
    framebuffer_clear(interface->framebuffer, 0);
    for (int c = 0; c < GTV_FRAMEBUFFER_ELEM_COUNT; c++) {
        int inter = c / GTV_FRAMEBUFFER_WIDTH;
        interface->framebuffer[c] = (byte)inter;
    }
    
    GTV_Sprite_blit_to_framebuffer(
        interface->framebuffer,
        interface->private->player.sprite,
        interface->private->player.px,
        interface->private->player.py
    );
}

void GTV_GameStateInterface_cleanup(GTV_GameStateInterface *interface) {
    free(interface->private);
}

/* -- Game state interface ---------------------------------------------------------------------- */
