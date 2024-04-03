package platformer_2d

import "core:math"
import "core:fmt"

IMPLICIT_DIVIDEND :: i32(100)

HitGrid_i32 :: struct {
	top_left_x: i32, top_left_y: i32,
	w: i32, h: i32,
	solids: []bool,
}

HitGrid_i32_new :: proc(x: i32, y: i32, w: i32, h: i32) -> HitGrid_i32 {
	size := w * h
	backing_buffer := make([]bool, size)
	return HitGrid_i32 {
		top_left_x = x, top_left_y = y,
		w = w, h = h,
		solids = backing_buffer
	}
}

Playeri32 :: struct {
	// Variables
	pos_x: i32, pos_y: i32,
	vel_x: i32, vel_y: i32,
	grounded: bool,

	// Soft constants
	grav_y: i32,
	vel_max: i32,
	speed_x: i32,

	init_jmp_vel_y: i32,

	hitbox: HitGrid_i32,
}

player_i32_new :: proc() -> Playeri32 {
	applied_pos_x: i32 = 0
	applied_pos_y: i32 = 0
	applied_speed_x: i32 = 2
	applied_vel_max: i32 = 10

	time_to_apex_scaling_factor_seconds :: f32(1e+2)
	time_to_apex_secs :: i32(0.32 * time_to_apex_scaling_factor_seconds)
	applied_jump_height_px: i32 = 100
	applied_grav_y :=
		(2 * applied_jump_height_px) / (time_to_apex_secs * time_to_apex_secs)
	applied_init_jmp_vel_y_f32: f32 = math.sqrt(2.0 * f32(applied_grav_y) * f32(applied_jump_height_px))
	applied_init_jmp_vel_y: i32 = i32(applied_init_jmp_vel_y_f32)

	applied_hitbox_w: i32 = 20
	applied_hitbox_h: i32 = 35
	applied_hitbox_x: i32 = applied_pos_x - (applied_hitbox_w / 2)
	applied_hitbox_y: i32 = applied_pos_y - (applied_hitbox_h)

	return Playeri32 {
		pos_x = applied_pos_x, pos_y = applied_pos_y,
		vel_x = 0, vel_y = 0,
		grounded = false,

		grav_y = applied_grav_y,
		vel_max = applied_vel_max,
		speed_x = applied_speed_x,

		init_jmp_vel_y = applied_init_jmp_vel_y,

		hitbox = HitGrid_i32_new(
			applied_hitbox_x, applied_hitbox_y,
			applied_hitbox_w, applied_hitbox_h),
	}
}

player_i32_update :: proc (player: ^Playeri32, inp: PlayerControls, dt_ns: i64) {
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

	dt := f32(dt_ns)
	vel_scale_factor := f32(1e-7)

	if !player.grounded {
		player.vel_y -= i32(f32(player.grav_y) * vel_scale_factor * dt)
	} else {
		//fmt.println(player.pos)
	}


	// Damping
	//player.vel.x = player.vel.x / (1 + player.damp * dt)
	//player.vel.y = player.vel.y / (1 + player.damp * dt)

	player.vel_x = clamp(player.vel_x, -player.vel_max, player.vel_max)
	player.vel_y = clamp(player.vel_y, -player.vel_max, player.vel_max)

	player.pos_x += i32(f32(player.vel_x) * vel_scale_factor * dt * +1)
	player.pos_y += i32(f32(player.vel_y) * vel_scale_factor * dt * -1)

	// Handle collisions
	// TODO: offload collision logic to AABB system
	h_cutoff: i32 = 400
	if player.pos_y >= h_cutoff {
		player.grounded = true
		player.pos_y = h_cutoff // resolve clipping
	}

	// Update hitbox
	player.hitbox.top_left_x = player.pos_x - player.hitbox.w / 2
	player.hitbox.top_left_y = player.pos_y - player.hitbox.h
}