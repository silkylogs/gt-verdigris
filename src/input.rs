use sdl2::rect::Point;
use std::time::Instant;

#[derive(Copy, Clone, Debug)]
pub struct MouseInput {
    pub lmb: bool,
    pub rmb: bool,
    pub lmb_held_down_instant: Instant,
    pub rmb_held_down_instant: Instant,
    pub cursor_pos: Point,
}

impl MouseInput {
    pub fn new_default() -> MouseInput {
        MouseInput {
            lmb: false,
            rmb: false,
            lmb_held_down_instant: Instant::now(),
            rmb_held_down_instant: Instant::now(),
            cursor_pos: Point::new(0, 0),
        }
    }

    pub fn coords(&self) -> Point {
        self.cursor_pos
    }

    pub fn poll_motion(&mut self, x: i32, y: i32) {
        self.cursor_pos.x = x;
        self.cursor_pos.y = y;
    }
}
