#include <stdio.h>
#include <stdlib.h>
#include <raylib.h>

/* -- Windows.h ----------------------------------------------------------------------------------*/

void *LoadLibraryA(char *lpLibFileName);
void *GetProcAddress(void *hModule, char *lpProcName);
bool FreeLibrary(void *hLibModule);

/* -- Windows.h ----------------------------------------------------------------------------------*/

#include "GTV_game.h"

/* -- Hot-reloading ------------------------------------------------------------------------------*/

bool (*pGTV_Arena_init)(GTV_Arena*, byte*, int32);
void *(*pGTV_Arena_alloc)(GTV_Arena*, int32);
void (*pGTV_Arena_free_all)(GTV_Arena*);
GTV_Color (*pGTV_Framebuffer_get_color)(byte*, GTV_ColorPalette, int32);
void (*pGTV_KeyboardInput_populate)(GTV_KeyboardInput*);
void (*pGTV_GameStateInterface_init)(GTV_GameStateInterface*, GTV_Arena*, GTV_ColorPalette, GTV_Sprite);
void (*pGTV_GameStateInterface_update)(GTV_GameStateInterface*);
void (*pGTV_GameStateInterface_cleanup)(GTV_GameStateInterface*);

void *dll = NULL;
bool GTV_HotReloader_load_all(void) {
    if (dll != NULL) return false;

    pGTV_Arena_init =                   LoadLibraryA("GTV_Arena_init");
    pGTV_Arena_alloc =                  LoadLibraryA("GTV_Arena_alloc");
    pGTV_Arena_free_all =               LoadLibraryA("GTV_Arena_free_all");
    pGTV_Framebuffer_get_color =        LoadLibraryA("GTV_Framebuffer_get_color");
    pGTV_KeyboardInput_populate =       LoadLibraryA("GTV_KeyboardInput_populate");
    pGTV_GameStateInterface_init =      LoadLibraryA("GTV_GameStateInterface_init");
    pGTV_GameStateInterface_update =    LoadLibraryA("GTV_GameStateInterface_update");
    pGTV_GameStateInterface_cleanup =   LoadLibraryA("GTV_GameStateInterface_cleanup");

    return
        pGTV_Arena_init
        && pGTV_Arena_alloc
        && pGTV_Arena_free_all
        && pGTV_Framebuffer_get_color 
        && pGTV_KeyboardInput_populate
        && pGTV_GameStateInterface_init
        && pGTV_GameStateInterface_update 
        && pGTV_GameStateInterface_cleanup;
}

bool GTV_HotReloader_free_all(void) {
    if (dll == NULL) return false;

    bool ok
         = FreeLibrary(pGTV_Arena_init)
        && FreeLibrary(pGTV_Arena_alloc)
        && FreeLibrary(pGTV_Arena_free_all)
        && FreeLibrary(pGTV_Framebuffer_get_color)
        && FreeLibrary(pGTV_KeyboardInput_populate)
        && FreeLibrary(pGTV_GameStateInterface_init)
        && FreeLibrary(pGTV_GameStateInterface_update)
        && FreeLibrary(pGTV_GameStateInterface_cleanup);

    if (ok) dll = NULL;
    return ok;
}

bool GTV_HotReloader_reload(void) {
    bool ok = true;
    ok &= GTV_HotReloader_free_all();
    ok &= GTV_HotReloader_load_all();
    return ok;
}

/* -- Hot-reloading ------------------------------------------------------------------------------*/

/* -- Utility ------------------------------------------------------------------------------------*/

Color GTV_Color_to_raylib_color(GTV_Color gtv_color) {
    Color raylib_color;
    
    raylib_color.r = gtv_color.r.container;
    raylib_color.g = gtv_color.g.container;
    raylib_color.b = gtv_color.b.container;
    raylib_color.a = 0xFF;

    return raylib_color;
}

GTV_Color Raylib_color_to_GTV_Color(Color raylib_color) {
    GTV_Color gtv_color;

    gtv_color.r.container = raylib_color.r;
    gtv_color.g.container = raylib_color.g;
    gtv_color.b.container = raylib_color.b;

    return gtv_color;
}

byte GTV_Color_to_byte(GTV_Color col, GTV_ColorPalette palette) {
    for (int32 i = 0; i < GTV_COLOR_PALETTE_SIZE; i++) {
        if (
            (col.r.container == palette.colors[i].r.container) &&
            (col.g.container == palette.colors[i].g.container) &&
            (col.b.container == palette.colors[i].b.container)
        ) {
            return (byte){i};
        }
    }

    return (byte){0xFF}; // TODO find a better color
}

bool atlas_to_palette(Image src, GTV_ColorPalette *dst) {
    int32
        palette_width = 16,
        palette_height = 16;
    if (src.width < palette_width || src.height < palette_height) {
        return false;
    }

    for (int32 y = 0; y < palette_height; y++) {
        for (int32 x = 0; x < palette_width; x++) {
            Color image_pixel_color = GetImageColor(src, x, y);
            GTV_Color col = Raylib_color_to_GTV_Color(image_pixel_color);
            dst->colors[y*palette_width + x] = col;
        }
    }

    return true;
}

bool palettize_atlas(Image src_img, GTV_ColorPalette src_palette, GTV_Sprite *dst) {
    if ((src_img.width != dst->width) || (src_img.height != dst->height)) return false;

    for (int32 y = 0; y < src_img.height; y++){
        for (int32 x = 0; x < src_img.width; x++) {
            Color raylib_color = GetImageColor(src_img, x, y);
            GTV_Color col = Raylib_color_to_GTV_Color(raylib_color);
            byte applied_color = GTV_Color_to_byte(col, src_palette);
            dst->data[y*dst->width + x] = applied_color;
        }
    }

    return true;
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


/* -- Main -------------------------------------------------------------------------------------- */

int main(void) {
    // Self explanatory
    GTV_OsWindow os_window = {
        .width = 512,
        .height = 512,
        .title = GTV_WINDOW_TITLE
    };
    InitWindow(os_window.width, os_window.height, os_window.title);
    //SetExitKey(KEY_NULL);

    // Initialize memory systems
    int32 backing_memory_len = 4 * sizeof (GTV_GameStateInterface);
    printf("Allocating %d bytes as backing memory.\n", backing_memory_len);
    byte *backing_memory = malloc(backing_memory_len);
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

    // Self explanatory
    GTV_GameStateInterface *interface = GTV_Arena_alloc(&arena, sizeof (GTV_GameStateInterface));
    if (!interface) {
        printf("Interface allocation failed\n");
        return 1;
    }
    GTV_GameStateInterface_init(interface, &arena, palette, palettized_atlas);
    
    while (!WindowShouldClose()) {
        // if (IsKeyReleased(KEY_E)) ToggleFullscreen();

        GTV_KeyboardInput_populate(&interface->keyboard_input);
        GTV_GameStateInterface_update(interface);

        BeginDrawing();
        ClearBackground(MAGENTA);
        for (int i = 0; i < sizeof interface->framebuffer; i++) {
            byte palette_index = interface->framebuffer[i];
            GTV_Color gtv_color = interface->current_palette.colors[palette_index.container];
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
