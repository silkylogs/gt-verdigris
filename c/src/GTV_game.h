#ifndef GTV_GAME_HEADER_DEFINED
#define GTV_GAME_HEADER_DEFINED

#include <stdint.h>
#include <stdbool.h>

#define GTV_LOCAL static            // Defined in the dll, called solely by the dll
#define GTV_EXPORT extern           // Defined in the dll, called by both dll and platform layer
#define GTV_PLATFORM_LAYER extern   // Defined in the platform layer, called by both

/* -- Utility ----------------------------------------------------------------------------------- */

typedef unsigned char byte;
typedef int32_t int32;
typedef uint64_t uint64;

typedef struct GTV_OsWindow {
    int32 width, height;
    char *title;
} GTV_OsWindow;

#define GTV_WINDOW_TITLE "Platformer demo"

/* -- Utility ----------------------------------------------------------------------------------- */

/* -- Colors ------------------------------------------------------------------------------------ */

#define GTV_COLOR_PALETTE_COLLECTION_COUNT (0x10)
#define GTV_COLOR_PALETTE_SIZE (0x100)

typedef struct GTV_Color {
    byte r, g, b, padding;
} GTV_Color;

typedef struct GTV_ColorPalette {
    GTV_Color colors[GTV_COLOR_PALETTE_SIZE];
} GTV_ColorPalette;

// typedef struct GTV_ColorPaletteCollection {
//     GTV_ColorPalette palettes[GTV_COLOR_PALETTE_COLLECTION_COUNT];
//     int32 current;
// } GTV_ColorPaletteCollection;


/* -- Colors ------------------------------------------------------------------------------------ */

/* -- Framebuffer ------------------------------------------------------------------------------- */

#define GTV_FRAMEBUFFER_WIDTH (0x100)
#define GTV_FRAMEBUFFER_HEIGHT (GTV_FRAMEBUFFER_WIDTH)
#define GTV_FRAMEBUFFER_ELEM_COUNT (GTV_FRAMEBUFFER_WIDTH * GTV_FRAMEBUFFER_HEIGHT)

typedef struct GTV_Framebuffer {
    byte *data;
    int32 size;
} GTV_Framebuffer;

GTV_EXPORT GTV_Color
GTV_Framebuffer_get_color(byte *fb, GTV_ColorPalette curr_palette, int32 idx);

/* -- Framebuffer ------------------------------------------------------------------------------- */

/* -- Sprites ----------------------------------------------------------------------------------- */

// TODO rename to GTV_PalettizedSprite
typedef struct GTV_Sprite {
    int32 width, height;
    byte *data;
} GTV_Sprite;

//GTV_EXPORT void GTV_Sprite_get_atlas();

/* -- Sprites ----------------------------------------------------------------------------------- */

/* -- Input ------------------------------------------------------------------------------------- */

enum GTV_KeyboardInputArrowKey {
    GTV_KEYBOARD_INPUT_ARROW_KEY_UP = 0,
    GTV_KEYBOARD_INPUT_ARROW_KEY_DOWN = 1,
    GTV_KEYBOARD_INPUT_ARROW_KEY_LEFT = 2,
    GTV_KEYBOARD_INPUT_ARROW_KEY_RIGHT = 3,
};

enum GTV_KeyboardInputLetterKey {
    GTV_KEYBOARD_INPUT_LETTER_KEY_E = 0,
};

typedef struct GTV_KeyboardInput {
    bool arrow_keys[4];
    bool letter_keys[1];
} GTV_KeyboardInput;

/* -- Input ------------------------------------------------------------------------------------- */

/* -- Game -------------------------------------------------------------------------------------- */

typedef struct GTV_PrivateGameState GTV_PrivateGameState;

typedef struct GTV_GameStateInterface {
    bool should_exit;
    byte framebuffer[GTV_FRAMEBUFFER_ELEM_COUNT];
    GTV_ColorPalette current_palette;
    GTV_KeyboardInput keyboard_input;
    GTV_Sprite palettized_sprite_atlas;

    GTV_PrivateGameState *private;
} GTV_GameStateInterface;

GTV_EXPORT void GTV_GameStateInterface_init(
    GTV_GameStateInterface *interface,
    GTV_Arena *arena,
    GTV_ColorPalette palette,
    GTV_Sprite palettized_atlas
);
GTV_EXPORT void GTV_GameStateInterface_update(GTV_GameStateInterface *);
GTV_EXPORT void GTV_GameStateInterface_cleanup(GTV_GameStateInterface *);

/* -- Game -------------------------------------------------------------------------------------- */

#endif