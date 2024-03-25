package main

import rl "vendor:raylib"
import "core:math/linalg"
import "core:strings"
import "core:fmt"
import "core:time"

OsWindow :: struct { w: i32, h: i32, title: string, }

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



