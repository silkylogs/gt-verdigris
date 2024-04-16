package main

import rl "vendor:raylib"
import p2d "platformer_2d"
import rs "render_system"

import "core:math/linalg"
import "core:strings"
import "core:fmt"
import "core:time"
import "core:os"

OsWindow :: struct { w: i32, h: i32, title: string, }

GameState :: struct {
	window: OsWindow,
	world: p2d.World,
	render_system: rs.RenderCommandRecipient,
	player: p2d.Player_int,
	last_frame_time: time.Duration,
}

init_game :: proc(state: ^GameState) {
	state.window = OsWindow {
		w = 800,
		h = 600,
		title = "Test",
	}

	_, state.world = p2d.World_new(w=300, h=200)
	//state.render_target = rl.GenImageColor(300, 200, rl.MAROON)
	state.render_target = rl.GenImageChecked(300, 200, 8, 8, rl.MAROON, rl.BLUE); 

	state.player = p2d.player_int_new()

	state.last_frame_time = 69 * time.Microsecond
}

update_game :: proc(state: ^GameState) {
	key_d := rl.IsKeyDown(rl.KeyboardKey.D)
	key_a := rl.IsKeyDown(rl.KeyboardKey.A)
	key_w := rl.IsKeyDown(rl.KeyboardKey.W)
	key_s := rl.IsKeyDown(rl.KeyboardKey.S)

	player_controls := p2d.PlayerControls {
		jmp_button_pressed = rl.IsKeyPressed(rl.KeyboardKey.W),
		go_right = key_d,
		go_left = key_a,
	}

	dt_ns := time.duration_nanoseconds(state.last_frame_time)
	p2d.player_int_update(&state.player, player_controls, dt_ns)
}

draw_game :: proc(state: ^GameState) {
	rl.BeginDrawing()

	rl.ClearBackground(rl.RAYWHITE)

	rl.EndDrawing()
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

	//rl.SetTargetFPS(120)
	rl.SetTargetFPS(24)

	for !rl.WindowShouldClose() {
		time_now := time.now()

		update_game(&game_state)
		draw_game(&game_state)

		time_since := time.since(time_now)
		game_state.last_frame_time = time_since
		//fmt.println("Frame time: ", game_state.last_frame_time)
	}
}
