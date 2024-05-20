#include "GTV_game.h"

/* -- Utility ------------------------------------------------------------------------------------*/
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
    int width, height, x, y;
    byte *data;
} GTV_Sprite;

void GTV_Sprite_blit_to_framebuffer(byte *fb, GTV_Sprite sprite) {
    for (int y = 0; y < sprite.height; y++) {
        for (int x = 0; x < sprite.width; x++) {
            byte pixel = sprite.data[y * sprite.width + x];
            framebuffer_set_pixel_xy(
                fb,
                x + sprite.x,
                y + sprite.y,
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

void GTV_GameState_init(GTV_GameState *state) {
    state->should_exit = 0;
    
    for (int c = 0; c < GTV_COLOR_PALETTE_SIZE; c++) {
        state->current_palette.colors[c].r = (byte)c;
        state->current_palette.colors[c].g = (byte)c;
        state->current_palette.colors[c].b = (byte)c;
    }
}

// Bouncing sprite test
int vx = 2,
    vy = 1,
    px = 0,
    py = 0;
void GTV_GameState_step(GTV_GameState *state) {
    // Bouncing sprite test
    GTV_Sprite smiley = {
        .width = 16, .height = 16,
        .x = 0, .y = 0,
        .data = sprite_data
    };
    if (px + smiley.width > GTV_FRAMEBUFFER_WIDTH || px < 0) vx *= -1;
    if (py + smiley.height == GTV_FRAMEBUFFER_HEIGHT || py < 0) vy *= -1;
    px += vx;
    py += vy;
    smiley.x = px;
    smiley.y = py;

    // Color cycling test
    state->current_palette.colors[0xFF].r = smiley.x;
    state->current_palette.colors[0xFF].g = smiley.y;
    state->current_palette.colors[0xFF].b = smiley.x + smiley.y;
    
    // Set framebuffer
    framebuffer_clear(state->framebuffer, 0);
    for (int c = 0; c < GTV_FRAMEBUFFER_ELEM_COUNT; c++) {
        int inter = c / GTV_FRAMEBUFFER_WIDTH;
        state->framebuffer[c] = (byte)inter;
    }
    
    // Blit sprite
    GTV_Sprite_blit_to_framebuffer(state->framebuffer, smiley);
}


/* -- Game -------------------------------------------------------------------------------------- */
