package platformer_2d

PixelRGBA32 :: struct {
	R, G, B, A: u8,
}

// This world is a 2d grid of pixels
World :: struct {
	width, height: int,
	pixels: []PixelRGBA32,
}

World_new :: proc(w: int, h: int) -> (bool, World) {
	if w <= 0 || h <= 0 { return false, World {} }

	size := w * h
	backing_buffer := make([]PixelRGBA32, size)
	return true, World {
		width = w, height = h,
		pixels = backing_buffer
	}
}

World_get_pixel :: proc(world: World, x: int, y: int) -> (bool, PixelRGBA32) {
	if x < 0 || y < 0 || x >= world.width || y >= world.height {
		return false, PixelRGBA32 {}
	}

	return true, world.pixels[y * world.width + x]
}