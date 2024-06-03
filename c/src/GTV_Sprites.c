// TODO sprite manager

typedef struct GTV_Sprite GTV_Sprite;

GTV_LOCAL void 
GTV_Sprite_blit_to_framebuffer(byte *fb, GTV_Sprite sprite, int32 pos_x, int32 pos_y) {
    for (int32 y = 0; y < sprite.height; y++) {
        for (int32 x = 0; x < sprite.width; x++) {
            byte pixel = sprite.data[y * sprite.width + x];
            GTV_Framebuffer_set_pixel_xy(fb, (x + pos_x), (y + pos_y), pixel);
        }
    }
}

GTV_LOCAL bool
GTV_Sprite_draw_8x8_sprite(byte *framebuffer, uint64 sprite_8x8, int32 pos_x, int32 pos_y, byte color) {
    byte rows[8] = {
        (sprite_8x8 >> 0x38) & 0xFF,
        (sprite_8x8 >> 0x30) & 0xFF,
        (sprite_8x8 >> 0x28) & 0xFF,
        (sprite_8x8 >> 0x20) & 0xFF,

        (sprite_8x8 >> 0x18) & 0xFF,
        (sprite_8x8 >> 0x10) & 0xFF,
        (sprite_8x8 >> 0x08) & 0xFF,
        (sprite_8x8 >> 0x00) & 0xFF,
    };
    bool success = true;
    int32 bits_in_byte = 8;

    for (int32 y = 0; y < sizeof rows; y++) {
        for (int32 x = 0; x < bits_in_byte; x++) {
            bool contains_pixel = (rows[y] >> (bits_in_byte - x)) & 0x1;
            if (contains_pixel) {
                success &= GTV_Framebuffer_set_pixel_xy(
                    framebuffer,
                    (x + pos_x), (y + pos_y),
                    color
                );
            }
        }
    }

    return success;
}


// Keeping this around as text drawing tutorial
// GTV_LOCAL void GTV_draw_editor(GTV_GameStateInterface *interface) {
//     int32 x = 0, y = 0;
//     for (int32 i = 0; i < GTV_ATLAS_FONT_DIGITS_8x8_COUNT; i++) {
//         GTV_Sprite_draw_8x8_sprite(
//             interface->framebuffer,
//             g_font_atlas_digits_8x8[i],
//             x+(8*i), y,
//             0xFE
//         );
//     }
//     y += 8;
//     for (int32 i = 0; i < GTV_ATLAS_FONT_CAPITAL_LETTERS_8x8_COUNT; i++) {
//         GTV_Sprite_draw_8x8_sprite(
//             interface->framebuffer,
//             g_font_atlas_capital_letters_8x8[i],
//             x+(8*i), y,
//             0xFD
//         );
//     }
// }