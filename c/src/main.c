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

/* -- Utility ------------------------------------------------------------------------------------*/

/* -- Input ------------------------------------------------------------------------------------- */

void GTV_KeyboardInput_populate(GTV_KeyboardInput *kb_input) {
    kb_input->arrow_keys[GTV_KEYBOARD_INPUT_ARROW_KEY_UP] =     IsKeyDown(KEY_UP);
    kb_input->arrow_keys[GTV_KEYBOARD_INPUT_ARROW_KEY_DOWN] =   IsKeyDown(KEY_DOWN);
    kb_input->arrow_keys[GTV_KEYBOARD_INPUT_ARROW_KEY_LEFT] =   IsKeyDown(KEY_LEFT);
    kb_input->arrow_keys[GTV_KEYBOARD_INPUT_ARROW_KEY_RIGHT] =  IsKeyDown(KEY_RIGHT);
}

/* -- Input ------------------------------------------------------------------------------------- */

/* -- Main -------------------------------------------------------------------------------------- */

int main(void) {
    InitWindow(GTV_FRAMEBUFFER_WIDTH, GTV_FRAMEBUFFER_HEIGHT, "Color cycling demo");
    //SetExitKey(KEY_NULL);

    GTV_GameStateInterface *interface = malloc(sizeof (GTV_GameStateInterface));
    GTV_GameStateInterface_init(interface);
    while (!WindowShouldClose()) {
        // if (IsKeyReleased(KEY_E))
        //     ToggleFullscreen();

        GTV_KeyboardInput_populate(&interface->keyboard_input);
        GTV_GameStateInterface_update(interface);

        BeginDrawing();
        ClearBackground(MAGENTA);
        for (int i = 0; i < sizeof interface->framebuffer; i++) {
            GTV_Color gtv_color = interface->current_palette.colors[interface->framebuffer[i]];
            Color raylib_color = GTV_Color_to_raylib_color(gtv_color);
            int y = i / GTV_FRAMEBUFFER_WIDTH;
            int x = i % GTV_FRAMEBUFFER_WIDTH;
            DrawRectangle(x, y, 1, 1, raylib_color);
        }
        EndDrawing();
    }

    CloseWindow();
    GTV_GameStateInterface_cleanup(interface);
    free(interface);
    return 0;
}

/* -- Main -------------------------------------------------------------------------------------- */
