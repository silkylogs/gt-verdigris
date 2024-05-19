//#include <stdio.h>
#include <raylib.h>
#include "GTV_game.h"

/* -- Utility ------------------------------------------------------------------------------------*/

Color GTV_Color_to_raylib_color(GTV_Color gtv_color) {
    Color raylib_color;
    
    raylib_color.r = gtv_color.r;
    raylib_color.g = gtv_color.g;
    raylib_color.b = gtv_color.b;
    raylib_color.a = 0xFF;

    return raylib_color;
}

/* -- Utility ------------------------------------------------------------------------------------*/

/* -- Main -------------------------------------------------------------------------------------- */

int main(void) {
    GTV_ColorPalette this_palette = { 0 };
    for (int c = 0; c < sizeof this_palette.colors; c += 1) {
        this_palette.colors[c + 0].r = c * 0x2;
        this_palette.colors[c + 0].g = c * 0x4;
        this_palette.colors[c + 0].b = c * 0x8;
    }

    GTV_GameState state = { 0 };
    state.should_exit = 0;
    for (int c = 0; c < sizeof state.framebuffer; c++) {
        state.framebuffer[c] = c;
    }

    InitWindow(GTV_FRAMEBUFFER_WIDTH, GTV_FRAMEBUFFER_HEIGHT, "Color cycling demo");

    while (!WindowShouldClose()) {
        BeginDrawing();
        ClearBackground(MAGENTA);

        for (int i = 0; i < sizeof state.framebuffer; i++) {
            Color raylib_color;
            GTV_Color gtv_color;

            gtv_color = this_palette.colors[state.framebuffer[i]];
            raylib_color = GTV_Color_to_raylib_color(gtv_color);

            int y = i / GTV_FRAMEBUFFER_WIDTH;
            int x = i % GTV_FRAMEBUFFER_WIDTH;
            
            DrawRectangle(x, y, 1, 1, raylib_color);
        }
        
        EndDrawing();


        for (int c = 0; c < sizeof this_palette.colors; c += 1) {
            this_palette.colors[c + 0].r += 1;
            this_palette.colors[c + 0].g += 2;
            this_palette.colors[c + 0].b += 3;
        }
    }

    CloseWindow();

    return 0;
}

/* -- Main -------------------------------------------------------------------------------------- */
