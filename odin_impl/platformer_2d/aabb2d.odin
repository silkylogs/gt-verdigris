package platformer_2d

import "core:math/linalg"
import "core:fmt"

AABB2D :: struct {
	top_left: linalg.Vector2f32,
	bottom_right: linalg.Vector2f32
}

width :: proc(hitbox: AABB2D) -> f32 {
	diff := hitbox.bottom_right.x - hitbox.top_left.x
	return abs(diff)
}

height :: proc(hitbox: AABB2D) -> f32 {
	diff := hitbox.top_left.y - hitbox.bottom_right.y 
	return abs(diff)
}
