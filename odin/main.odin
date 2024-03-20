package main

import rl "vendor:raylib"
import "core:math/linalg"
import "core:strings"
import "core:fmt"
import "core:time"

Vector2i32 :: struct { x: i32, y: i32, }
OsWindow :: struct { w: i32, h: i32, title: string, }

TestCircle :: struct { rad: i32, pos: Vector2i32, }

GameState :: struct {
	window: OsWindow,

	tcircle: TestCircle,
	vel: Vector2i32,
	vert_accel: i32,
	k_jmp_accel: i32,
	k_grav_accel: i32,
	grounded: bool,

	last_frame_time: time.Duration,
}

init_game :: proc(state: ^GameState) {
	state.window.w = 800
	state.window.h = 450
	state.window.title = "Test"

	state.tcircle.rad = 10
	state.tcircle.pos = Vector2i32 { 200, 200 }
	state.vel = Vector2i32 { 0, 0 }
	state.vert_accel = 0
	state.k_jmp_accel = 5
	state.k_grav_accel = 1
	state.grounded = false

	state.last_frame_time = 69 * time.Microsecond
}

update_game :: proc(state: ^GameState) {
	time_now := time.now()

	// Game input collection
	key_d := rl.IsKeyDown(rl.KeyboardKey.D)
	key_a := rl.IsKeyDown(rl.KeyboardKey.A)
	key_w := rl.IsKeyDown(rl.KeyboardKey.W)
	key_s := rl.IsKeyDown(rl.KeyboardKey.S)

	// Game action mappings
	jmp_button_pressed := rl.IsKeyPressed(rl.KeyboardKey.W)
	go_right := key_d
	go_left := key_a

	if go_right {
		state.vel.x = 2
	} else if go_left {
		state.vel.x = -2
	} else {
		state.vel.x = 0
	}

	if jmp_button_pressed {
		fmt.println("Jump button pressed")
		state.vert_accel = state.k_jmp_accel
		state.grounded = false
	}

	// Apply velocity
	state.tcircle.pos.x += state.vel.x

	// And gravity
	if !state.grounded {
		state.vert_accel -= state.k_grav_accel
		state.vel.y -= state.vert_accel
		state.tcircle.pos.y += state.vel.y
	} else {
		//fmt.println("Grounded")
	}

	// Temp collisions
	h_cutoff := state.window.h
	if state.tcircle.pos.y >= h_cutoff {
		state.grounded = true
		state.vert_accel = 0

		// resolve clipping
		state.tcircle.pos.y = h_cutoff
	}

	time_since := time.since(time_now)
	state.last_frame_time = time_since
	fmt.println("Frame time: ", state.last_frame_time)
}

draw_game :: proc(state: ^GameState) {
	rl.BeginDrawing()
	defer rl.EndDrawing()

	rl.ClearBackground(rl.RAYWHITE)
	
	rl.DrawCircle(state.tcircle.pos.x, state.tcircle.pos.y, f32(state.tcircle.rad), rl.DARKBLUE);
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



