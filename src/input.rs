use std::time::Duration;
use sdl2::rect::Point;

// The durations are timestamps
#[derive(Copy, Clone, Debug)]
pub struct MouseInput {
    pub lmb: bool,
    pub rmb: bool,
    pub lmb_timestamp: Duration,
    pub rmb_timestamp: Duration,
    pub cursor_pos: Point,
}

impl MouseInput {
    pub fn new_default() -> MouseInput {
        let zero = std::time::Duration::from_secs(0);
        MouseInput {
            lmb: false,
            rmb: false,
            lmb_timestamp: zero,
            rmb_timestamp: zero,
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

    // lmb, rmb
    pub fn timestamp_diff(curr: MouseInput, prev: MouseInput) -> (Duration, Duration) {(
        curr.lmb_timestamp - prev.lmb_timestamp,
        curr.rmb_timestamp - prev.rmb_timestamp
    )}
}
