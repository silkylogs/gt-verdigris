package platformer_2d

import "core:math"
import "core:fmt"

IMPLICIT_DIVIDEND :: i32(100)

// TODO: replace with celeste style grid-like 2d voxelish hit grid
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

new_pi32 :: proc() -> Playeri32 {
	return Playeri32 {}
}