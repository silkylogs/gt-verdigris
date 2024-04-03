package platformer_2d

import "core:math/linalg"
import "core:math"
import "core:fmt"

AABB2D :: struct {
	top_left: linalg.Vector2f32,
	width: f32,
	height: f32
}

width :: proc(hitbox: AABB2D) -> f32 {
	return math.abs(hitbox.width)
}

height :: proc(hitbox: AABB2D) -> f32 {
	return math.abs(hitbox.height)
}

AABB2D_render_info :: proc(hitbox: AABB2D) ->
(x: i32, y: i32, w: i32, h: i32)
{
	x = i32(hitbox.top_left.x)
	y = i32(hitbox.top_left.y)
	w = i32(width(hitbox))
	h = i32(height(hitbox))
	return
}

AABB2D_update :: proc(arr: []AABB2D) {
	// todo
}
