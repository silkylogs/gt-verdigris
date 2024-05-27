typedef struct GTV_AABB {
    float x, y, w, h;
} GTV_AABB;

#define GTV_AABB_COLLECTION_COUNT ((int32)4)
typedef struct GTV_AABB_Collection {
    GTV_AABB elems[GTV_AABB_COLLECTION_COUNT];
} GTV_AABB_Collection;

GTV_LOCAL bool GTV_AABB_intersect(GTV_AABB a, GTV_AABB b) {
    float a_min_x = a.x,
        a_max_x = a.x + a.w,
        a_min_y = a.y,
        a_max_y = a.y + a.h,

        b_min_x = b.x,
        b_max_x = b.x + b.w,
        b_min_y = b.y,
        b_max_y = b.y + b.h;

    return
        a_min_x < b_max_x &&
        a_max_x > b_min_x &&
        a_min_y < b_max_y &&
        a_max_y > b_min_y;
}

GTV_LOCAL bool
GTV_AABB_player_intersects_boxes(GTV_AABB player, GTV_AABB_Collection boxes) {
    for (int32 i = 0; i < GTV_AABB_COLLECTION_COUNT; i++) {
        if (GTV_AABB_intersect(player, boxes.elems[i])) return true;
    }
    return false;
}

GTV_LOCAL bool GTV_AABB_draw(GTV_AABB box, byte *fb, byte color) {
    bool success = true;
    int32
        box_x = (int32)box.x,
        box_y = (int32)box.y,
        box_w = (int32)box.w,
        box_h = (int32)box.h;

    for (int32 x = box_x; x < box_x + box_w; x++) {
        success &= GTV_Framebuffer_set_pixel_xy(fb, x, box_y,             color); // Top
        success &= GTV_Framebuffer_set_pixel_xy(fb, x, box_y + box_h - 1, color); // Bottom
    }

    for (int32 y = box_y; y < box_y + box_h; y++) {
        success &= GTV_Framebuffer_set_pixel_xy(fb, box_x,             y, color); // Left
        success &= GTV_Framebuffer_set_pixel_xy(fb, box_x + box_w - 1, y, color); // Right
    }

    return success;
}
