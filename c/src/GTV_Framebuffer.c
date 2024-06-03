/* -- Framebuffer ------------------------------------------------------------------------------- */

GTV_EXPORT GTV_Color
GTV_Framebuffer_get_color(uint8_t *fb, GTV_ColorPalette curr_palette, int32_t idx) {
    GTV_Color default_col = g_magenta;
    if ((idx >= 0) && (idx < GTV_FRAMEBUFFER_ELEM_COUNT))
        return GTV_Color_get_from_palette(curr_palette, fb[idx]);
    else return default_col;
}

GTV_LOCAL void GTV_Framebuffer_clear(uint8_t *fb, uint8_t pixel) {
    for (int32_t i = 0; i < GTV_FRAMEBUFFER_ELEM_COUNT; i++)
        fb[i] = pixel;
}

GTV_LOCAL bool GTV_Framebuffer_set_pixel_idx(uint8_t *fb, int32_t idx, uint8_t pixel) {
    if ((idx >= 0) && (idx < GTV_FRAMEBUFFER_ELEM_COUNT)) {
        fb[idx] = pixel;
        return true;
    }
    else return false;
}

GTV_LOCAL bool GTV_Framebuffer_set_pixel_xy(uint8_t *fb, int32_t x, int32_t y, uint8_t pixel) {
    if ((x < 0) && (x >= GTV_FRAMEBUFFER_WIDTH)) return false;
    if ((y < 0) && (y >= GTV_FRAMEBUFFER_HEIGHT)) return false;

    int32_t idx = y * GTV_FRAMEBUFFER_WIDTH + x;
    if ((idx >= 0) && (idx < GTV_FRAMEBUFFER_ELEM_COUNT)) {
        GTV_Framebuffer_set_pixel_idx(fb, idx, pixel);
        return true;
    }
    else return false;
}

/* -- Framebuffer ------------------------------------------------------------------------------- */