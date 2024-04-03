package main

import rl "vendor:raylib"
import p2d "platformer_2d"
import "core:math/linalg"
import "core:strings"
import "core:fmt"
import "core:time"

OsWindow :: struct { w: i32, h: i32, title: string, }

GameState :: struct {
	window: OsWindow,
	player: p2d.Player,
	last_frame_time: time.Duration,
}

init_game :: proc(state: ^GameState) {
	state.window.w = 800
	state.window.h = 600
	state.window.title = "Test"

	state.player = p2d.player_new()

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
	p2d.player_update(&state.player, player_controls, dt_ns)
}

draw_game :: proc(state: ^GameState) {
	rl.BeginDrawing()
	defer rl.EndDrawing()

	rl.ClearBackground(rl.RAYWHITE)

	// draw floor
	floor_y := i32(400)
	rect_color := rl.Color { u8(floor_y%0xff), 0, 0, 0xff }
	rl.DrawRectangle(0, floor_y, 1000, 1000, rect_color)

	// And test jump reticles
	increment := i32(50)
	floor_y -= increment
	for floor_y > 0 {
		rect_color.r = u8(floor_y%0xff)
		rect_color.g = u8(floor_y%0xff / 2)
		rect_color.b = u8(floor_y%0xff / 4)
		rl.DrawRectangle(0, floor_y, 1000, 1, rect_color)
		floor_y -= increment
	}
	
	x, y, w, h := p2d.AABB2D_render_info(state.player.hitbox)
	rl.DrawRectangle(x, y, w, h, rl.DARKBLUE)
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



