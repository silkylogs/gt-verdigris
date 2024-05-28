//#include <stdio.h>
#include <raylib.h>
#include <stdlib.h>
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

GTV_Color Raylib_color_to_GTV_Color(Color raylib_color) {
    GTV_Color gtv_color;

    gtv_color.r = raylib_color.r;
    gtv_color.g = raylib_color.g;
    gtv_color.b = raylib_color.b;

    return gtv_color;
}

byte GTV_Color_to_byte(GTV_Color col, GTV_ColorPalette palette) {
    for (int32 i = 0; i < GTV_COLOR_PALETTE_SIZE; i++) {
        if (
            (col.r == palette.colors[i].r) &&
            (col.r == palette.colors[i].g) &&
            (col.r == palette.colors[i].b)
        ) {
            return i;
        }
    }

    return 0xFF; // TODO find a better color
}

/* -- Utility ------------------------------------------------------------------------------------*/

/* -- Input ------------------------------------------------------------------------------------- */

void GTV_KeyboardInput_populate(GTV_KeyboardInput *kb_input) {
    kb_input->arrow_keys[GTV_KEYBOARD_INPUT_ARROW_KEY_UP] =     IsKeyDown(KEY_UP);
    kb_input->arrow_keys[GTV_KEYBOARD_INPUT_ARROW_KEY_DOWN] =   IsKeyDown(KEY_DOWN);
    kb_input->arrow_keys[GTV_KEYBOARD_INPUT_ARROW_KEY_LEFT] =   IsKeyDown(KEY_LEFT);
    kb_input->arrow_keys[GTV_KEYBOARD_INPUT_ARROW_KEY_RIGHT] =  IsKeyDown(KEY_RIGHT);
    
    kb_input->letter_keys[GTV_KEYBOARD_INPUT_LETTER_KEY_E] =    IsKeyDown(KEY_E);
}

/* -- Input ------------------------------------------------------------------------------------- */

// TODO get atlas from filesystem
// create palette from 16x16 square in atlas
bool atlas_to_palette(Image src, GTV_ColorPalette *dst) {
    if (src.width < 16 || src.height < 16) {
        return false;
    }

    for (int32 y = 0; y < 16; y++) {
        for (int32 x = 0; x < 16; x++) {
            GTV_Color col = Raylib_color_to_GTV_Color(GetImageColor(src, x, y));
            dst->colors[y*16+x] = col;
        }
    }

    return true;
}

// use GetImageColor to convert image from color array to index array (palettization)
bool gen_palettized_atlas(Image src_img, GTV_ColorPalette src_palette, GTV_Sprite *dst) {
    if ((src_img.width != dst->width) || (src_img.height != dst->height)) return false;

    for (int32 y = 0; y < src_img.height; y++){
        for (int32 x = 0; x < src_img.width; x++) {
            Color raylib_color = GetImageColor(src_img, x, y);
            GTV_Color col = Raylib_color_to_GTV_Color(raylib_color);
            byte applied_color = GTV_Color_to_byte(col, src_palette);
            dst->data[y*dst->width+x] = applied_color;
        }
    }

    return true;
}

/* -- Main -------------------------------------------------------------------------------------- */

typedef struct GTV_OsWindow {
    int width, height;
    char *title;
} GTV_OsWindow;

#define GTV_WINDOW_TITLE "Platformer demo"

int main(void) {
    GTV_OsWindow os_window = {
        .width = 512,
        .height = 512,
        .title = GTV_WINDOW_TITLE
    };
    
    InitWindow(os_window.width, os_window.height, os_window.title);
    //SetExitKey(KEY_NULL);

    int32 backing_memory_len = 4 * sizeof (GTV_GameStateInterface);
    byte *backing_memory = malloc(backing_memory_len);
    if (!backing_memory) {
        printf("backing memory acquisition failed\n");
        return 1;
    }

    // GTV_GameStateInterface *interface = malloc(sizeof (GTV_GameStateInterface));
    GTV_Arena arena;
    GTV_Arena_init(&arena, backing_memory, backing_memory_len);
    GTV_GameStateInterface *interface = GTV_Arena_alloc(&arena, sizeof (GTV_GameStateInterface));
    if (!interface) {
        printf("allocation failed %d\n", sizeof (GTV_GameStateInterface));
        return 1;
    }
    GTV_GameStateInterface_init(interface, &arena);

    // TESTING
    // (part of gamestateinterface_init)
    interface->palettized_sprite_atlas.width = 256;
    interface->palettized_sprite_atlas.height = 256;
    interface->palettized_sprite_atlas.data = malloc(256*256);

    Image atlas = LoadImage("assets/atlas.png");
    ImageFormat(&atlas, PIXELFORMAT_UNCOMPRESSED_R8G8B8);
    if (!atlas_to_palette(atlas, &interface->current_palette)) {
        printf("atlas_to_palette failed\n");
        return 1;
    }
    if (!gen_palettized_atlas(atlas, interface->current_palette, &interface->palettized_sprite_atlas)) {
        printf("gen_palettized_atlas failed\n");
        return 1;
    }
    UnloadImage(atlas);
    // TESTING

    while (!WindowShouldClose()) {
        // if (IsKeyReleased(KEY_E)) ToggleFullscreen();

        GTV_KeyboardInput_populate(&interface->keyboard_input);
        GTV_GameStateInterface_update(interface);

        BeginDrawing();
        ClearBackground(MAGENTA);
        for (int i = 0; i < sizeof interface->framebuffer; i++) {
            GTV_Color gtv_color = interface->current_palette.colors[interface->framebuffer[i]];
            Color raylib_color = GTV_Color_to_raylib_color(gtv_color);
            int y = i / GTV_FRAMEBUFFER_WIDTH;
            int x = i % GTV_FRAMEBUFFER_WIDTH;
            int pixel_scaling_factor = os_window.width / GTV_FRAMEBUFFER_WIDTH;
            DrawRectangle(
                x*pixel_scaling_factor, y*pixel_scaling_factor,
                pixel_scaling_factor, pixel_scaling_factor,
                raylib_color
            );
        }
        EndDrawing();
    }

    CloseWindow();
    GTV_GameStateInterface_cleanup(interface);
    GTV_Arena_free_all(&arena);
    free(backing_memory);
    return 0;
}

/* -- Main -------------------------------------------------------------------------------------- */
