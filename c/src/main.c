//#include <stdio.h>
#include <raylib.h>

/* -- Colors ------------------------------------------------------------------------------------ */

#define GTV_COLOR_PALETTE_COLLECTION_COUNT (0xF)
#define GTV_COLOR_PALETTE_SIZE (0xF)

typedef struct GTV_Color {
    unsigned char r, g, b;
} GTV_Color;

typedef struct GTV_ColorPalette {
    Color colors[GTV_COLOR_PALETTE_SIZE];
} GTV_ColorPalette;

typedef struct GTV_ColorPaletteCollection {
    GTV_ColorPalette palettes[GTV_COLOR_PALETTE_COLLECTION_COUNT];
    int current;
} GTV_ColorPaletteCollection;

/* -- Colors ------------------------------------------------------------------------------------ */

/* -- Framebuffer ------------------------------------------------------------------------------- */

#define GTV_FRAMEBUFFER_WIDTH (0xFF)
#define GTV_FRAMEBUFFER_HEIGHT (GTV_FRAMEBUFFER_WIDTH)
#define GTV_FRAMEBUFFER_SIZE_BYTES (GTV_FRAMEBUFFER_WIDTH * GTV_FRAMEBUFFER_HEIGHT)
unsigned char g_framebuffer[GTV_FRAMEBUFFER_SIZE_BYTES];

void draw_framebuffer_to_raylib_window(unsigned char *fb) {
    for (int y = 0; y < GTV_FRAMEBUFFER_HEIGHT; y++) {
        for (int x = 0; x < GTV_FRAMEBUFFER_WIDTH; x++) {
            // int double_pixel_idx = y*GTV_FRAMEBUFFER_WIDTH + x;
            // byte double_pixel = g_framebuffer[double_pixel_idx * 2]
            
            // byte nib_hi_color = (double_pixel & 0xF0) >> 4;


            Color col = { x, y, 2, 0xFF };
            DrawRectangle(x, y, 1, 1, col);
        }
    }
}

/* -- Framebuffer ------------------------------------------------------------------------------- */

/* -- Main -------------------------------------------------------------------------------------- */

int main(void) {
    InitWindow(GTV_FRAMEBUFFER_WIDTH, GTV_FRAMEBUFFER_HEIGHT, "Test");

    while (!WindowShouldClose()) {
        BeginDrawing();
        ClearBackground(MAGENTA);
        draw_framebuffer_to_raylib_window(g_framebuffer);
        EndDrawing();
    }

    CloseWindow();

    return 0;
}

/* -- Main -------------------------------------------------------------------------------------- */
