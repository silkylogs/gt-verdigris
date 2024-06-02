/* -- Colors ------------------------------------------------------------------------------------ */

typedef struct GTV_Color GTV_Color;
typedef struct GTV_ColorPalette GTV_ColorPalette;
typedef struct GTV_ColorPaletteCollection GTV_ColorPaletteCollection;

// Debug colors
GTV_Color
    g_magenta = { 0xFF, 0x00, 0xFF },
    g_black =   { 0x00, 0x00, 0x00 };

GTV_LOCAL GTV_Color GTV_Color_get_from_palette(GTV_ColorPalette palette, byte color) {
    GTV_Color default_col = g_black;
    if ((color >= 0) && (color < GTV_FRAMEBUFFER_ELEM_COUNT))
        return palette.colors[color];
    else return default_col;
}

/* -- Colors ------------------------------------------------------------------------------------ */