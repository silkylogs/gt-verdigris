package main

import "core:math"
import "core:math/linalg"
import "core:fmt"

Player :: struct {
	// Variables
	pos: linalg.Vector2f32,
	vel: linalg.Vector2f32,
	grounded: bool,

	// Soft constants
	grav: linalg.Vector2f32,
	vel_mult: f32,
	vel_max: f32,
	jmp_vel: f32,

	x_speed: f32,
	rad: i32,
}

// Note: directions correspond to natural directions, not buffer position
//       (i.e. +ve y = up, not down)
player_new :: proc() -> Player {
	return Player {
		pos = linalg.Vector2f32 { 400, 0 },
		vel = linalg.Vector2f32 { 0, 0 },
		grounded = false,

		grav = linalg.Vector2f32 { 0, -1.5 },
		vel_mult = 1,
		vel_max = 10,
		jmp_vel = 40,
		x_speed = 2,
		rad = 5,
	}
}

// The actions that the player can do with the character
PlayerControls :: struct {
	go_right: bool,
	go_left: bool,
	jmp_button_pressed: bool,
}

player_update :: proc (player: ^Player, inp: PlayerControls) {
	if inp.go_right {
		player.vel.x = player.x_speed
	} else if inp.go_left {
		player.vel.x = -player.x_speed
	} else {
		player.vel.x = 0
	}

	if inp.jmp_button_pressed {
		fmt.println("Jump button pressed")
		JUMP_HEIGHT :: f32(100)
		applied_jmp_vel := math.sqrt(
			math.abs(2 * player.grav.y * JUMP_HEIGHT)
		)

		player.grounded = false
		player.vel.y = applied_jmp_vel
	}

	// Apply gravity
	if !player.grounded {
		player.vel += player.grav
	} else {
	}

	player.vel.x = clamp(player.vel.x, -player.vel_max, player.vel_max)
	player.vel.y = clamp(player.vel.y, -player.vel_max, player.vel_max)

	// Apply velocity
	player.pos.x += player.vel.x
	player.pos.y += -1 * player.vel.y

	// Temp collisions
	h_cutoff := f32(180)
	if player.pos.y >= h_cutoff {
		player.grounded = true

		// resolve clipping
		player.pos.y = h_cutoff
	}
}
