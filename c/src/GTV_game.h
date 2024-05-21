#ifndef GTV_GAME_HEADER_DEFINED
#define GTV_GAME_HEADER_DEFINED

typedef unsigned char byte;

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
    int current;
} GTV_ColorPaletteCollection;


/* -- Colors ------------------------------------------------------------------------------------ */

/* -- Framebuffer ------------------------------------------------------------------------------- */

#define GTV_FRAMEBUFFER_WIDTH (0x100)
#define GTV_FRAMEBUFFER_HEIGHT (GTV_FRAMEBUFFER_WIDTH)
#define GTV_FRAMEBUFFER_ELEM_COUNT (GTV_FRAMEBUFFER_WIDTH * GTV_FRAMEBUFFER_HEIGHT)

GTV_Color GTV_get_color_from_framebuffer(byte *fb, GTV_ColorPalette curr_palette, int idx);

/* -- Framebuffer ------------------------------------------------------------------------------- */

/* -- Game -------------------------------------------------------------------------------------- */

typedef struct GTV_PrivateGameState GTV_PrivateGameState;

typedef struct GTV_GameStateInterface {
    int should_exit;
    byte framebuffer[GTV_FRAMEBUFFER_ELEM_COUNT];
    GTV_ColorPalette current_palette;

    GTV_PrivateGameState *private;
} GTV_GameStateInterface;

void GTV_GameStateInterface_init(GTV_GameStateInterface *);
void GTV_GameStateInterface_update(GTV_GameStateInterface *);
void GTV_GameStateInterface_cleanup(GTV_GameStateInterface *);

/* -- Game -------------------------------------------------------------------------------------- */

#endif