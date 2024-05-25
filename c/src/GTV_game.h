#ifndef GTV_GAME_HEADER_DEFINED
#define GTV_GAME_HEADER_DEFINED

#include <stdint.h>
#include <stdbool.h>
typedef unsigned char byte;
typedef int32_t int32;
typedef uint64_t uint64;

#define GTV_LOCAL static
#define GTV_EXPORT extern

/* -- Colors ------------------------------------------------------------------------------------ */

#define GTV_COLOR_PALETTE_COLLECTION_COUNT (0x10)
#define GTV_COLOR_PALETTE_SIZE (0x100)

typedef struct GTV_Color {
    byte r, g, b, padding;
} GTV_Color;

typedef struct GTV_ColorPalette {
    GTV_Color colors[GTV_COLOR_PALETTE_SIZE];
} GTV_ColorPalette;

typedef struct GTV_ColorPaletteCollection {
    GTV_ColorPalette palettes[GTV_COLOR_PALETTE_COLLECTION_COUNT];
    int32 current;
} GTV_ColorPaletteCollection;


/* -- Colors ------------------------------------------------------------------------------------ */

/* -- Framebuffer ------------------------------------------------------------------------------- */

#define GTV_FRAMEBUFFER_WIDTH (0x100)
#define GTV_FRAMEBUFFER_HEIGHT (GTV_FRAMEBUFFER_WIDTH)
#define GTV_FRAMEBUFFER_ELEM_COUNT (GTV_FRAMEBUFFER_WIDTH * GTV_FRAMEBUFFER_HEIGHT)

GTV_EXPORT GTV_Color
GTV_Framebuffer_get_color(byte *fb, GTV_ColorPalette curr_palette, int32 idx);

/* -- Framebuffer ------------------------------------------------------------------------------- */

/* -- Input ------------------------------------------------------------------------------------- */

enum GTV_KeyboardInputArrowKey {
    GTV_KEYBOARD_INPUT_ARROW_KEY_UP = 0,
    GTV_KEYBOARD_INPUT_ARROW_KEY_DOWN = 1,
    GTV_KEYBOARD_INPUT_ARROW_KEY_LEFT = 2,
    GTV_KEYBOARD_INPUT_ARROW_KEY_RIGHT = 3,
};

typedef struct GTV_KeyboardInput {
    byte arrow_keys[4];
} GTV_KeyboardInput;

GTV_EXPORT void GTV_KeyboardInput_populate(GTV_KeyboardInput *kb_input);

/* -- Input ------------------------------------------------------------------------------------- */

/* -- Game -------------------------------------------------------------------------------------- */

typedef struct GTV_PrivateGameState GTV_PrivateGameState;

typedef struct GTV_GameStateInterface {
    bool should_exit;
    byte framebuffer[GTV_FRAMEBUFFER_ELEM_COUNT];
    GTV_ColorPalette current_palette;
    GTV_KeyboardInput keyboard_input;

    GTV_PrivateGameState *private;
} GTV_GameStateInterface;

GTV_EXPORT void GTV_GameStateInterface_init(GTV_GameStateInterface *);
GTV_EXPORT void GTV_GameStateInterface_update(GTV_GameStateInterface *);
GTV_EXPORT void GTV_GameStateInterface_cleanup(GTV_GameStateInterface *);

/* -- Game -------------------------------------------------------------------------------------- */

#endif