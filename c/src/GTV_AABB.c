typedef struct GTV_AABB {
    float x, y, w, h;
} GTV_AABB;

#define GTV_AABB_COLLECTION_COUNT ((int32_t)32)
typedef struct GTV_AABB_Collection {
    GTV_AABB elems[GTV_AABB_COLLECTION_COUNT];
    // [0, active_idx] are active
    // (active_idx, size) are inactive
    int32_t active_idx;
} GTV_AABB_Collection;

GTV_LOCAL bool GTV_AABB_intersect(GTV_AABB a, GTV_AABB b) {
    float
        a_min_x = a.x,
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
    for (int32_t i = 0; i < GTV_AABB_COLLECTION_COUNT; i++) {
        if (GTV_AABB_intersect(player, boxes.elems[i])) return true;
    }
    return false;
}

GTV_LOCAL bool
GTV_AABB_player_is_grounded(GTV_AABB player, GTV_AABB_Collection boxes) {
    for (int32_t i = 0; i < GTV_AABB_COLLECTION_COUNT; i++) {
        if (GTV_AABB_intersect(player, boxes.elems[i])) {
            float
                player_min_y = player.y,
                player_max_y = player.y + player.h,
                box_min_y = boxes.elems[i].y,
                box_max_y = boxes.elems[i].y + boxes.elems[i].h;

            if ((player_max_y > box_min_y) && (player_min_y < box_max_y)) return true;
        };
    }
    return false;
}

GTV_LOCAL bool GTV_AABB_draw(GTV_AABB box, uint8_t *fb, uint8_t color) {
    bool success = true;
    int32_t
        box_x = (int32_t)box.x,
        box_y = (int32_t)box.y,
        box_w = (int32_t)box.w,
        box_h = (int32_t)box.h;

    for (int32_t x = box_x; x < box_x + box_w; x++) {
        success &= GTV_Framebuffer_set_pixel_xy(fb, x, box_y,             color); // Top
        success &= GTV_Framebuffer_set_pixel_xy(fb, x, box_y + box_h - 1, color); // Bottom
    }

    for (int32_t y = box_y; y < box_y + box_h; y++) {
        success &= GTV_Framebuffer_set_pixel_xy(fb, box_x,             y, color); // Left
        success &= GTV_Framebuffer_set_pixel_xy(fb, box_x + box_w - 1, y, color); // Right
    }

    return success;
}
