typedef struct GTV_Player {
    // TOOD replace `gravy` with `grav_y`
    float x_remainder, y_remainder, vx, vy, gravy, jmpy, input_vx;
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

// A port of C#'s Math.Sign function
GTV_LOCAL int GTV_sign(int num) {
    if (num > 0) return 1;
    else if (num < 0) return -1;
    else return 0;
}

GTV_LOCAL void
GTV_Player_move_x(
    GTV_Player *player_prev_state,
    float amount,
    GTV_AABB_Collection test_boxes
) {
    player_prev_state->x_remainder += amount;
    int move = (int)player_prev_state->x_remainder;
    if (move != 0) {
        player_prev_state->x_remainder -= move;
        int sign = GTV_sign(move);
        while (move != 0) {
            GTV_AABB next_pos = player_prev_state->bounds;
            next_pos.x += sign;
            next_pos.y += 0;
            if (!GTV_AABB_player_intersects_boxes(next_pos, test_boxes)) {
                // No solid besides player
                player_prev_state->bounds.x += sign;
                move -= sign;
            } else {
                // Solid present, do something
                printf("Wall has been hit sideways\n");
                break;
            }
        }
    }
}

GTV_LOCAL void
GTV_Player_move_y(
    GTV_Player *player_prev_state,
    float amount,
    GTV_AABB_Collection test_boxes
) {
    player_prev_state->y_remainder += amount;
    int move = (int)player_prev_state->y_remainder;
    if (move != 0) {
        player_prev_state->y_remainder -= move;
        int sign = GTV_sign(move);
        while (move != 0) {
            GTV_AABB next_pos = player_prev_state->bounds;
            next_pos.x += 0;
            next_pos.y += sign;

            if (!GTV_AABB_player_intersects_boxes(next_pos, test_boxes)) {
                // No solid below player
                player_prev_state->bounds.y += sign;
                move -= sign;
                player_prev_state->grounded = false;
            } else {
                // Solid present, do something
                player_prev_state->grounded = true;
                break;
            }
        }
    }
}
/*
GTV_LOCAL void
GTV_Player_move_y(
    GTV_Player *player_prev_state,
    GTV_KeyboardInput *input,
    GTV_AABB_Collection test_boxes
) {
    GTV_Player player = *player_prev_state;
    bool should_apply_future_movement_state = true;

    player.grounded = GTV_AABB_player_is_grounded(player.bounds, test_boxes);
    printf("player.grounded = %s\n", player.grounded ? "true" : "false");
    // printf("player.vy = %f\n", player.vy);

    if (input->arrow_keys[GTV_KEYBOARD_INPUT_ARROW_KEY_DOWN]) {
        // TODO: increase vy slightly
    } else if (input->arrow_keys[GTV_KEYBOARD_INPUT_ARROW_KEY_UP]) {
        // jump
        if (player.grounded) player.vy = -player.jmpy;
    }

    // gravity
    // if (!player.grounded) {
    //     player.vy = player.gravy;
    // } else {
    //     player.vy = 0;
    // }
    // player.bounds.y += player.vy;
    player.bounds.y += 0.1f;

    if ((player.bounds.y + player.bounds.h > GTV_FRAMEBUFFER_HEIGHT) ||
        (player.bounds.y <= 0) ||
        (GTV_AABB_player_intersects_boxes(player.bounds, test_boxes)))
    {
        player_prev_state->grounded = true;
        should_apply_future_movement_state = false;
    }

    if (should_apply_future_movement_state) {
        player_prev_state->bounds = player.bounds;
    }
    // player_prev_state->bounds.x = 0;
    // player_prev_state->bounds.y = 0;
}
*/