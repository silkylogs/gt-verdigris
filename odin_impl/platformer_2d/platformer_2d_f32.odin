package platformer_2d_f32

import "core:math"
import "core:math/linalg"
import "core:fmt"

Playerf32 :: struct {
	// Variables
	pos: linalg.Vector2f32,
	vel: linalg.Vector2f32,
	grounded: bool,

	// Soft constants
	grav_y: f32,
	damp: f32,
	vel_max: f32,
	x_speed: f32,

	init_jmp_vel: f32,

	hitbox: AABB2D
}

// Note: directions correspond to natural directions, not buffer position
//       (i.e. +ve y = up, not down)
player_new :: proc() -> Playerf32 {
	time_to_apex_scaling_factor_seconds :: f32(1e+2)
	time_to_apex_secs :: 0.32 * time_to_apex_scaling_factor_seconds

	// TODO: move to player struct
	applied_jump_height_px := f32(100)
	applied_grav_y := 
		(2 * applied_jump_height_px) / (time_to_apex_secs * time_to_apex_secs)
	applied_init_jmp_vel := math.sqrt(2 * applied_grav_y * applied_jump_height_px)

	return Playerf32 {
		pos = linalg.Vector2f32 { 000, 000 },
		vel = linalg.Vector2f32 { 0, 0 },
		grounded = false,

		grav_y = applied_grav_y,
		damp = 0, // <=1e-7 to get anything moving; maybe when slowing down
		vel_max = 10,
		x_speed = 2,

		init_jmp_vel = applied_init_jmp_vel,

		hitbox = AABB2D { linalg.Vector2f32{ 0, 0 }, 20, 35 }
	}
}

// The actions that the player can do with the character
PlayerControls :: struct {
	go_right: bool,
	go_left: bool,
	jmp_button_pressed: bool,
}

player_update :: proc (player: ^Playerf32, inp: PlayerControls, dt_ns: i64) {
	if inp.go_right {
		player.vel.x = player.x_speed
	} else if inp.go_left {
		player.vel.x = -player.x_speed
	} else {
		player.vel.x = 0
	}

	if inp.jmp_button_pressed {
		fmt.println("Jump button pressed")
		player.vel.y = player.init_jmp_vel
		player.grounded = false
	}

	dt := f32(dt_ns)
	vel_scale_factor := f32(1e-7)

	if !player.grounded {
		player.vel.y -= player.grav_y * vel_scale_factor * dt
	} else {
		//fmt.println(player.pos)
	}


	// Damping
	player.vel.x = player.vel.x / (1 + player.damp * dt)
	player.vel.y = player.vel.y / (1 + player.damp * dt)

	player.vel.x = clamp(player.vel.x, -player.vel_max, player.vel_max)
	player.vel.y = clamp(player.vel.y, -player.vel_max, player.vel_max)

	player.pos.x += player.vel.x * vel_scale_factor * dt
	player.pos.y += player.vel.y * vel_scale_factor * dt * -1

	// Handle collisions
	// TODO: offload collision logic to AABB system
	h_cutoff := f32(400)
	if player.pos.y >= h_cutoff {
		player.grounded = true

		// resolve clipping
		player.pos.y = h_cutoff
	}

	// Update hitbox
	player.hitbox.top_left.x = player.pos.x - player.hitbox.width / 2
	player.hitbox.top_left.y = player.pos.y - player.hitbox.height
}
