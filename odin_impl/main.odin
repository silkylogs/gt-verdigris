package main

import rl "vendor:raylib"
import "core:math/linalg"
import "core:strings"
import "core:fmt"
import "core:time"

OsWindow :: struct { w: i32, h: i32, title: string, }

// -- 2d platformer ------------

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
	player.grav.y = -1.5

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
		player.vel.y += player.grav.y
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


GameState :: struct {
	window: OsWindow,
	player: Player,
	last_frame_time: time.Duration,
}

init_game :: proc(state: ^GameState) {
	state.window.w = 800
	state.window.h = 450
	state.window.title = "Test"

	state.player = player_new()

	state.last_frame_time = 69 * time.Microsecond
}

update_game :: proc(state: ^GameState) {
	time_now := time.now()

	key_d := rl.IsKeyDown(rl.KeyboardKey.D)
	key_a := rl.IsKeyDown(rl.KeyboardKey.A)
	key_w := rl.IsKeyDown(rl.KeyboardKey.W)
	key_s := rl.IsKeyDown(rl.KeyboardKey.S)

	// -- Update player ----
	ref_player := &state.player

	// Game action mappings
	player_controls := PlayerControls {
		jmp_button_pressed = rl.IsKeyPressed(rl.KeyboardKey.W),
		go_right = key_d,
		go_left = key_a,
	}

	player_update(ref_player, player_controls)

	// -- Update player ----

	time_since := time.since(time_now)
	state.last_frame_time = time_since
	//fmt.println("Frame time: ", state.last_frame_time)
}

draw_game :: proc(state: ^GameState) {
	rl.BeginDrawing()
	defer rl.EndDrawing()

	rl.ClearBackground(rl.RAYWHITE)
	
	rl.DrawCircle(
		i32(state.player.pos.x), i32(state.player.pos.y),
		f32(state.player.rad), rl.DARKBLUE);
}

main :: proc() {
	game_state: GameState
	init_game(&game_state)

	rl.InitWindow(
		game_state.window.w,
		game_state.window.h,
		strings.clone_to_cstring(game_state.window.title)
	)
	defer rl.CloseWindow()

	rl.SetTargetFPS(60)

	for !rl.WindowShouldClose() {
		update_game(&game_state)
		draw_game(&game_state)
	}
}



