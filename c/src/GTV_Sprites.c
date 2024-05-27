// TODO sprite manager

typedef struct GTV_Sprite {
    int32 width, height;
    byte *data;
} GTV_Sprite;

GTV_LOCAL void 
GTV_Sprite_blit_to_framebuffer(byte *fb, GTV_Sprite sprite, int32 pos_x, int32 pos_y) {
    for (int32 y = 0; y < sprite.height; y++) {
        for (int32 x = 0; x < sprite.width; x++) {
            byte pixel = sprite.data[y * sprite.width + x];
            GTV_Framebuffer_set_pixel_xy(fb, (x + pos_x), (y + pos_y), pixel);
        }
    }
}
