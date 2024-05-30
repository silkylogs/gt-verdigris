typedef struct GTV_Player {
    // TOOD replace `gravy` with `grav_y`
    float vx, vy, gravy, jmpy, input_vx;
    bool grounded;
    GTV_Sprite sprite;
    GTV_AABB bounds;
} GTV_Player;

GTV_LOCAL void GTV_Player_init(GTV_Player *player, GTV_Sprite player_sprite) {
    player->vx = 0.0f;
    player->vy = 1.0f;
    player->gravy = 5.0f;
    player->jmpy = 1.0f;
    player->input_vx = 2.0f;
    player->grounded = false;

    player->sprite = player_sprite;

    player->bounds.x = 0.0f;
    player->bounds.y = 0.0f;
    player->bounds.w = (float)player->sprite.width;
    player->bounds.h = (float)player->sprite.height;
}

GTV_LOCAL void
GTV_Player_move_x(
    GTV_Player *player_prev_state,
    GTV_KeyboardInput *input,
    GTV_AABB_Collection test_boxes
) {
    GTV_Player player = *player_prev_state;
    bool should_apply_future_state = true;
    
    if (input->arrow_keys[GTV_KEYBOARD_INPUT_ARROW_KEY_RIGHT]) {
        player.vx = player.input_vx;
    } else if (input->arrow_keys[GTV_KEYBOARD_INPUT_ARROW_KEY_LEFT]) {
        player.vx = -player.input_vx;
    } else {
        player.vx = 0;
    }

    player.bounds.x += player.vx;

    if ((player.bounds.x < 0) ||
        ((player.bounds.x + player.bounds.w) >= (GTV_FRAMEBUFFER_WIDTH)) ||
        (GTV_AABB_player_intersects_boxes(player.bounds, test_boxes)))
    {
        should_apply_future_state = false;
    }

    if (should_apply_future_state) *player_prev_state = player;
}

GTV_LOCAL void
GTV_Player_move_y(
    GTV_Player *player_prev_state,
    GTV_KeyboardInput *input,
    GTV_AABB_Collection test_boxes
) {
    GTV_Player player = *player_prev_state;
    bool should_apply_future_state = true;

    // player.grounded = GTV_AABB_player_is_grounded(player.bounds, test_boxes);
    // printf("player.grounded = %s\n", player.grounded ? "true" : "false");
    // printf("player.vy = %f\n", player.vy);

    if (input->arrow_keys[GTV_KEYBOARD_INPUT_ARROW_KEY_DOWN]) {
        // TODO: increase vy slightly
    } else if (input->arrow_keys[GTV_KEYBOARD_INPUT_ARROW_KEY_UP]) {
        // jump
        if (player.grounded) player.vy = -player.jmpy;
    }

    // gravity
    if (!player.grounded) {
        player.vy = player.gravy;
    } else {
        player.vy = 0;
    }
    player.bounds.y += player.vy;

    if ((player.bounds.y + player.bounds.h > GTV_FRAMEBUFFER_HEIGHT) ||
        (player.bounds.y <= 0) ||
        (GTV_AABB_player_intersects_boxes(player.bounds, test_boxes)))
    {
        player.grounded = true;
        should_apply_future_state = false;
    }

    if (should_apply_future_state) *player_prev_state = player;
}
