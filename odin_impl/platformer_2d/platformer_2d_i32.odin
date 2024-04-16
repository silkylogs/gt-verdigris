package platformer_2d

import "core:math"
import "core:fmt"

HitGrid_int :: struct {
	top_left_x: int, top_left_y: int,
	w: int, h: int,
	solids: []bool,
}

HitGrid_int_new :: proc(x: int, y: int, w: int, h: int) -> HitGrid_int {
	size := w * h
	backing_buffer := make([]bool, size)
	return HitGrid_int {
		top_left_x = x, top_left_y = y,
		w = w, h = h,
		solids = backing_buffer
	}
}

Player_int :: struct {
	// Variables
	pos_x: int, pos_y: int,
	vel_x: int, vel_y: int,
	grounded: bool,

	// Soft constants
	grav_y: int,
	vel_max: int,
	speed_x: int,

	init_jmp_vel_y: int,

	hitbox: HitGrid_int,
}

player_int_new :: proc() -> Player_int {
	applied_pos_x: int = 10
	applied_pos_y: int = 50
	applied_speed_x: int = 2
	applied_vel_max: int = 10

	time_to_apex_scaling_factor_seconds :: f32(1e+2)
	time_to_apex_secs :: int(0.32 * time_to_apex_scaling_factor_seconds)
	applied_jump_height_px: int = 100
	applied_grav_y: int = 1
		//(2 * applied_jump_height_px) / (time_to_apex_secs * time_to_apex_secs)
	applied_init_jmp_vel_y_f32: f32 = math.sqrt(2.0 * f32(applied_grav_y) * f32(applied_jump_height_px))
	applied_init_jmp_vel_y: int = int(applied_init_jmp_vel_y_f32)

	applied_hitbox_w: int = 20
	applied_hitbox_h: int = 35
	applied_hitbox_x: int = applied_pos_x - (applied_hitbox_w / 2)
	applied_hitbox_y: int = applied_pos_y - (applied_hitbox_h)

	return Player_int {
		pos_x = applied_pos_x, pos_y = applied_pos_y,
		vel_x = 0, vel_y = 0,
		grounded = false,

		grav_y = applied_grav_y,
		vel_max = applied_vel_max,
		speed_x = applied_speed_x,

		init_jmp_vel_y = applied_init_jmp_vel_y,

		hitbox = HitGrid_int_new(
			applied_hitbox_x, applied_hitbox_y,
			applied_hitbox_w, applied_hitbox_h
		),
	}
}

player_int_update :: proc (player: ^Player_int, inp: PlayerControls, dt_ns: i64) {
	if inp.go_right {
		player.vel_x = player.speed_x
	} else if inp.go_left {
		player.vel_x = -player.speed_x
	} else {
		player.vel_x = 0
	}

	if inp.jmp_button_pressed {
		fmt.println("Jump button pressed")
		player.vel_y = player.init_jmp_vel_y
		player.grounded = false
	}

	if !player.grounded {
		player.vel_y -= player.grav_y
	} else {
		//fmt.println(player.pos)
	}

	player.vel_x = clamp(player.vel_x, -player.vel_max, player.vel_max)
	player.vel_y = clamp(player.vel_y, -player.vel_max, player.vel_max)

	applied_vel_x := (player.vel_x * +1)
	applied_vel_y := (player.vel_y * -1)

	player.pos_x += applied_vel_x
	player.pos_y += applied_vel_y

	// Handle collisions
	// TODO: offload collision logic to AABB system
	h_cutoff: int = 400
	if player.pos_y >= h_cutoff {
		player.grounded = true
		player.pos_y = h_cutoff // resolve clipping
	}

	// Update hitbox
	player.hitbox.top_left_x = player.pos_x - player.hitbox.w / 2
	player.hitbox.top_left_y = player.pos_y - player.hitbox.h
}

