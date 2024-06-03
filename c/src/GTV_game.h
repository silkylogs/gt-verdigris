#ifndef GTV_GAME_HEADER_DEFINED
#define GTV_GAME_HEADER_DEFINED

#include <stdint.h>
#include <stdbool.h>

#define GTV_LOCAL static            // Defined in the dll, called solely by the dll
#define GTV_EXPORT extern           // Defined in the dll, called by both dll and platform layer
#define GTV_PLATFORM_LAYER extern   // Defined in the platform layer, called by both

/* -- Utility ----------------------------------------------------------------------------------- */

typedef struct GTV_OsWindow {
    int32_t width, height;
    char *title;
} GTV_OsWindow;

#define GTV_WINDOW_TITLE "Platformer demo"

/* -- Utility ----------------------------------------------------------------------------------- */

/* -- Colors ------------------------------------------------------------------------------------ */

#define GTV_COLOR_PALETTE_COLLECTION_COUNT (0x10)
#define GTV_COLOR_PALETTE_SIZE (0x100)

typedef struct GTV_Color {
    uint8_t r, g, b, padding;
} GTV_Color;

typedef struct GTV_ColorPalette {
    GTV_Color colors[GTV_COLOR_PALETTE_SIZE];
} GTV_ColorPalette;

// typedef struct GTV_ColorPaletteCollection {
//     GTV_ColorPalette palettes[GTV_COLOR_PALETTE_COLLECTION_COUNT];
//     int32_t current;
// } GTV_ColorPaletteCollection;


/* -- Colors ------------------------------------------------------------------------------------ */

/* -- Framebuffer ------------------------------------------------------------------------------- */

#define GTV_FRAMEBUFFER_WIDTH (0x100)
#define GTV_FRAMEBUFFER_HEIGHT (GTV_FRAMEBUFFER_WIDTH)
#define GTV_FRAMEBUFFER_ELEM_COUNT (GTV_FRAMEBUFFER_WIDTH * GTV_FRAMEBUFFER_HEIGHT)

typedef struct GTV_Framebuffer {
    uint8_t *data;
    int32_t size;
} GTV_Framebuffer;

GTV_EXPORT GTV_Color
GTV_Framebuffer_get_color(uint8_t *fb, GTV_ColorPalette curr_palette, int32_t idx);

/* -- Framebuffer ------------------------------------------------------------------------------- */

/* -- Sprites ----------------------------------------------------------------------------------- */

// TODO rename to GTV_PalettizedSprite
typedef struct GTV_Sprite {
    int32_t width, height;
    uint8_t *data;
} GTV_Sprite;

/* -- Sprites ----------------------------------------------------------------------------------- */

/* -- Input ------------------------------------------------------------------------------------- */

enum GTV_KeyboardInputKey {
    GTV_KEYBOARD_KEY_UP,
    GTV_KEYBOARD_KEY_DOWN,
    GTV_KEYBOARD_KEY_LEFT,
    GTV_KEYBOARD_KEY_RIGHT,

    GTV_KEYBOARD_KEY_A,
    GTV_KEYBOARD_KEY_B,
    GTV_KEYBOARD_KEY_C,
    GTV_KEYBOARD_KEY_D,
    GTV_KEYBOARD_KEY_E,
    GTV_KEYBOARD_KEY_F,
    GTV_KEYBOARD_KEY_G,
    GTV_KEYBOARD_KEY_H,
    GTV_KEYBOARD_KEY_I,
    GTV_KEYBOARD_KEY_J,
    GTV_KEYBOARD_KEY_K,
    GTV_KEYBOARD_KEY_L,
    GTV_KEYBOARD_KEY_M,
    GTV_KEYBOARD_KEY_N,
    GTV_KEYBOARD_KEY_O,
    GTV_KEYBOARD_KEY_P,
    GTV_KEYBOARD_KEY_Q,
    GTV_KEYBOARD_KEY_R,
    GTV_KEYBOARD_KEY_S,
    GTV_KEYBOARD_KEY_T,
    GTV_KEYBOARD_KEY_U,
    GTV_KEYBOARD_KEY_V,
    GTV_KEYBOARD_KEY_W,
    GTV_KEYBOARD_KEY_X,
    GTV_KEYBOARD_KEY_Y,
    GTV_KEYBOARD_KEY_Z,
    
    GTV_KEYBOARD_KEY_ESC,

    // Note: This should always be the last variant of this enum.
    GTV_KEYBOARD_INPUT_KEY_ENUM_LENGTH,
};

typedef struct GTV_KeyboardInput {
    bool keys[GTV_KEYBOARD_INPUT_KEY_ENUM_LENGTH];
} GTV_KeyboardInput;

/* -- Input ------------------------------------------------------------------------------------- */

/* -- Game -------------------------------------------------------------------------------------- */

typedef struct GTV_PrivateGameState GTV_PrivateGameState;

typedef struct GTV_GameStateInterface {
    bool initialized;
    bool exit_requested;
    uint8_t framebuffer[GTV_FRAMEBUFFER_ELEM_COUNT];
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