package platformer_2d

import "core:math/linalg"
import "core:fmt"

Player :: struct {
	rad: i32,

	pos: linalg.Vector2f32,
	vel: linalg.Vector2f32,
	grav: linalg.Vector2f32,

	vel_mult: f32,
	jmp_vel: f32,
	grounded: bool,
}

// Note: directions correspond to natural directions, not buffer position
//       (i.e. +ve y = up, not down)
player_new :: proc() -> Player {
	player: Player

	player.rad = 5
	player.pos = linalg.Vector2f32 { 400, 0 }
	player.vel = linalg.Vector2f32 { 0, 0 }
	player.grav= linalg.Vector2f32 { 0, -1.5 }

	player.grounded = false
	player.jmp_vel = 40
	player.vel_mult = 1

	return player
}

// The actions that the player can do with the character
PlayerControls :: struct {
	go_right: bool,
	go_left: bool,
	jmp_button_pressed: bool,	
}

player_update :: proc (player: ^Player, inp: PlayerControls) {
	H_SPEED :: f32(2)
	if inp.go_right {
		player.vel.x = H_SPEED
	} else if inp.go_left {
		player.vel.x = -H_SPEED
	} else {
		player.vel.x = 0
	}

	if inp.jmp_button_pressed {
		fmt.println("Jump button pressed")
		player.grounded = false
		player.vel.y = player.jmp_vel
	}

	// Apply gravity
	if !player.grounded {
		player.vel += player.grav
	} else {
	}

	VEL_MAX :: f32(10)
	player.vel.x = clamp(player.vel.x, -VEL_MAX, VEL_MAX)
	player.vel.y = clamp(player.vel.y, -VEL_MAX, VEL_MAX)

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

// -- 2d platformer ------------