#include <stdio.h>
#include <stdlib.h>
#include <raylib.h>

// -- Windows.h ------------------------------------------------------------------------------------

bool CopyFileA(char *const, char *const, bool);
bool FreeLibrary(void *);
void *LoadLibraryA(char *);
void *GetProcAddress(void *, char *);
void Sleep(int);

// -- Windows.h ------------------------------------------------------------------------------------

#include "GTV_Arena.h"
#include "GTV_game.h"

// -- Hot-reloading --------------------------------------------------------------------------------

void reload_dll(void **dll) {
    system("echo [Reloader] Initiating DLL recompilation");
    if (0 == system("build_application.bat")) {
        system("echo [Reloader]: DLL recompilation successful");
        if (*dll != NULL) { FreeLibrary(*dll); }
        CopyFileA("GTV_game.dll", "_GTV_game.dll", false);
        *dll = LoadLibraryA("_GTV_game.dll");
    } else {
        system("echo [Reloader] DLL recompilation failed");
    }
}

void *load_proc_from_dll(void *dll, char *proc_name) {
    return GetProcAddress(dll, proc_name);
}

// -- Hot-reloading --------------------------------------------------------------------------------


// -- Utility ------------------------------------------------------------------------------------

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

uint8_t GTV_Color_to_byte(GTV_Color col, GTV_ColorPalette palette) {
    for (int32_t i = 0; i < GTV_COLOR_PALETTE_SIZE; i++) {
        if (
            (col.r == palette.colors[i].r) &&
            (col.g == palette.colors[i].g) &&
            (col.b == palette.colors[i].b)
        ) {
            return (uint8_t){i};
        }
    }

    return (uint8_t){0xFF}; // TODO find a better color
}

bool atlas_to_palette(Image src, GTV_ColorPalette *dst) {
    int32_t
        palette_width = 16,
        palette_height = 16;
    if (src.width < palette_width || src.height < palette_height) {
        return false;
    }

    for (int32_t y = 0; y < palette_height; y++) {
        for (int32_t x = 0; x < palette_width; x++) {
            Color image_pixel_color = GetImageColor(src, x, y);
            GTV_Color col = Raylib_color_to_GTV_Color(image_pixel_color);
            dst->colors[y*palette_width + x] = col;
        }
    }

    return true;
}

bool palettize_atlas(Image src_img, GTV_ColorPalette src_palette, GTV_Sprite *dst) {
    if ((src_img.width != dst->width) || (src_img.height != dst->height)) return false;

    for (int32_t y = 0; y < src_img.height; y++){
        for (int32_t x = 0; x < src_img.width; x++) {
            Color raylib_color = GetImageColor(src_img, x, y);
            GTV_Color col = Raylib_color_to_GTV_Color(raylib_color);
            uint8_t applied_color = GTV_Color_to_byte(col, src_palette);
            dst->data[y*dst->width + x] = applied_color;
        }
    }

    return true;
}

// -- Utility ------------------------------------------------------------------------------------

// -- Input ------------------------------------------------------------------------------------- 

void GTV_KeyboardInput_populate(GTV_KeyboardInput *kb_input) {
    kb_input->keys[GTV_KEYBOARD_KEY_UP]     = IsKeyDown(KEY_UP);
    kb_input->keys[GTV_KEYBOARD_KEY_DOWN]   = IsKeyDown(KEY_DOWN);
    kb_input->keys[GTV_KEYBOARD_KEY_LEFT]   = IsKeyDown(KEY_LEFT);
    kb_input->keys[GTV_KEYBOARD_KEY_RIGHT]  = IsKeyDown(KEY_RIGHT);

    kb_input->keys[GTV_KEYBOARD_KEY_A]      = IsKeyDown(KEY_A);
    kb_input->keys[GTV_KEYBOARD_KEY_B]      = IsKeyDown(KEY_B);
    kb_input->keys[GTV_KEYBOARD_KEY_C]      = IsKeyDown(KEY_C);
    kb_input->keys[GTV_KEYBOARD_KEY_D]      = IsKeyDown(KEY_D);
    kb_input->keys[GTV_KEYBOARD_KEY_E]      = IsKeyDown(KEY_E);
    kb_input->keys[GTV_KEYBOARD_KEY_F]      = IsKeyDown(KEY_F);
    kb_input->keys[GTV_KEYBOARD_KEY_G]      = IsKeyDown(KEY_G);
    kb_input->keys[GTV_KEYBOARD_KEY_H]      = IsKeyDown(KEY_H);
    kb_input->keys[GTV_KEYBOARD_KEY_I]      = IsKeyDown(KEY_I);
    kb_input->keys[GTV_KEYBOARD_KEY_J]      = IsKeyDown(KEY_J);
    kb_input->keys[GTV_KEYBOARD_KEY_K]      = IsKeyDown(KEY_K);
    kb_input->keys[GTV_KEYBOARD_KEY_L]      = IsKeyDown(KEY_L);
    kb_input->keys[GTV_KEYBOARD_KEY_M]      = IsKeyDown(KEY_M);
    kb_input->keys[GTV_KEYBOARD_KEY_N]      = IsKeyDown(KEY_N);
    kb_input->keys[GTV_KEYBOARD_KEY_O]      = IsKeyDown(KEY_O);
    kb_input->keys[GTV_KEYBOARD_KEY_P]      = IsKeyDown(KEY_P);
    kb_input->keys[GTV_KEYBOARD_KEY_Q]      = IsKeyDown(KEY_Q);
    kb_input->keys[GTV_KEYBOARD_KEY_R]      = IsKeyDown(KEY_R);
    kb_input->keys[GTV_KEYBOARD_KEY_S]      = IsKeyDown(KEY_S);
    kb_input->keys[GTV_KEYBOARD_KEY_T]      = IsKeyDown(KEY_T);
    kb_input->keys[GTV_KEYBOARD_KEY_U]      = IsKeyDown(KEY_U);
    kb_input->keys[GTV_KEYBOARD_KEY_V]      = IsKeyDown(KEY_V);
    kb_input->keys[GTV_KEYBOARD_KEY_W]      = IsKeyDown(KEY_W);
    kb_input->keys[GTV_KEYBOARD_KEY_X]      = IsKeyDown(KEY_X);
    kb_input->keys[GTV_KEYBOARD_KEY_Y]      = IsKeyDown(KEY_Y);
    kb_input->keys[GTV_KEYBOARD_KEY_Z]      = IsKeyDown(KEY_Z);

    kb_input->keys[GTV_KEYBOARD_KEY_ESC]    = IsKeyDown(KEY_ESCAPE);
}

// -- Input ----------------------------------------------------------------------------------------


// -- Main -----------------------------------------------------------------------------------------

int main() {
    void *dll = 0;
    
    GTV_OsWindow os_window = {
        .width = 512,
        .height = 512,
        .title = GTV_WINDOW_TITLE
    };
    InitWindow(os_window.width, os_window.height, os_window.title);
    SetExitKey(KEY_NULL);

    // Initialize memory systems
    int32_t backing_memory_len = 4 * sizeof (GTV_GameStateInterface);
    printf("Allocating %d bytes as backing memory.\n", backing_memory_len);
    uint8_t *backing_memory = malloc(backing_memory_len);
    if (!backing_memory) {
        printf("Backing memory acquisition failed\n");
        return 1;
    }
    GTV_Arena arena;
    if (!GTV_Arena_init(&arena, backing_memory, backing_memory_len)) {
        printf("Arena initialization failed\n");
        return 1;
    }

    // Load atlas, generate palette, palettize atlas
    GTV_ColorPalette palette = {0};
    Image atlas_from_file = LoadImage("assets/atlas.png");
    ImageFormat(&atlas_from_file, PIXELFORMAT_UNCOMPRESSED_R8G8B8);
    if (!atlas_to_palette(atlas_from_file, &palette)) {
        printf("atlas_to_palette failed\n");
        return 1;
    }
    GTV_Sprite palettized_atlas = {
        .width = atlas_from_file.width,
        .height = atlas_from_file.height,
        .data = GTV_Arena_alloc(
            &arena,
            palettized_atlas.width * palettized_atlas.height
        )
    };
    if (!palettize_atlas(atlas_from_file, palette, &palettized_atlas)) {
        printf("palettize_atlas failed\n");
        return 1;
    }
    UnloadImage(atlas_from_file);

    // GameStateInterface init
    GTV_GameStateInterface *interface = GTV_Arena_alloc(&arena, sizeof (GTV_GameStateInterface));
    if (!interface) {
        printf("Interface allocation failed\n");
        return 1;
    }
    interface->initialized = false;
    interface->exit_requested = false;

    reload_dll(&dll);
    void (*p_tick_func)(
        GTV_GameStateInterface *,
        GTV_Arena *,
        GTV_ColorPalette,
        GTV_Sprite
    ) = GetProcAddress(dll, "GTV_game_tick");
    while (!WindowShouldClose() && !interface->exit_requested) {
        GTV_KeyboardInput_populate(&interface->keyboard_input);

        if (interface->keyboard_input.keys[GTV_KEYBOARD_KEY_W]) {
            reload_dll(&dll);
            p_tick_func = GetProcAddress(dll, "GTV_game_tick");
        }
        p_tick_func(interface, &arena, palette, palettized_atlas);

        BeginDrawing();
        ClearBackground(MAGENTA);
        for (int i = 0; i < GTV_FRAMEBUFFER_ELEM_COUNT; i++) {
            uint8_t palette_index = interface->framebuffer[i];
            GTV_Color gtv_color = interface->current_palette.colors[palette_index];
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
    GTV_Arena_free_all(&arena);
    free(backing_memory);
    return 0;
}

// -- Main -----------------------------------------------------------------------------------------

