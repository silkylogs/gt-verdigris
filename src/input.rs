// TODO: use std::time::Duration to represent time held down

use sdl2::rect::Point;

#[derive(Copy, Clone, Debug, PartialEq)]
pub enum ButtonStatus {
    Pressed,
    NotPressed,
    JustReleased,
    HeldDown,
}

#[derive(Copy, Clone, Debug)]
pub struct MouseInput {
    lmb: ButtonStatus,
    rmb: ButtonStatus,
    cursor_pos: Point,
}

impl MouseInput {
    pub fn new_default() -> MouseInput {
        MouseInput {
            lmb: ButtonStatus::NotPressed,
            rmb: ButtonStatus::NotPressed,
            cursor_pos: Point::new(0, 0),
        }
    }

    pub fn lmb(&self) -> ButtonStatus {
        self.lmb
    }

    pub fn rmb(&self) -> ButtonStatus {
        self.rmb
    }

    pub fn coords(&self) -> Point {
        self.cursor_pos
    }

    pub fn poll_buttons(&mut self, lmb_pressed: bool, rmb_pressed: bool) {
        self.lmb = match lmb_pressed {
            true => ButtonStatus::Pressed,
            false => ButtonStatus::NotPressed,
        };
        self.rmb = match rmb_pressed {
            true => ButtonStatus::Pressed,
            false => ButtonStatus::NotPressed,
        };
    }

    pub fn poll_motion(&mut self, x: i32, y: i32) {
        self.cursor_pos.x = x;
        self.cursor_pos.y = y;
    }

    // Call this just after polling mouse state
    // Note: some states described here are nonsense and are probably a non-issue
    pub fn update(prev_state: MouseInput, just_polled_state: MouseInput) -> MouseInput {
        let match_buttons = |p_state: ButtonStatus, jp_state: ButtonStatus| -> ButtonStatus {
            match p_state {
                ButtonStatus::Pressed => match jp_state {
                    ButtonStatus::Pressed => ButtonStatus::HeldDown,
                    ButtonStatus::NotPressed => ButtonStatus::JustReleased,
                    ButtonStatus::JustReleased => ButtonStatus::NotPressed,
                    ButtonStatus::HeldDown => ButtonStatus::HeldDown,
                },
                ButtonStatus::NotPressed => match jp_state {
                    ButtonStatus::Pressed => ButtonStatus::Pressed,
                    ButtonStatus::NotPressed => ButtonStatus::NotPressed,
                    ButtonStatus::JustReleased => ButtonStatus::NotPressed,
                    ButtonStatus::HeldDown => ButtonStatus::NotPressed,
                },
                ButtonStatus::JustReleased => match jp_state {
                    ButtonStatus::Pressed => ButtonStatus::Pressed,
                    ButtonStatus::NotPressed => ButtonStatus::NotPressed,
                    ButtonStatus::JustReleased => ButtonStatus::NotPressed,
                    ButtonStatus::HeldDown => ButtonStatus::HeldDown,
                },
                ButtonStatus::HeldDown => match jp_state {
                    ButtonStatus::Pressed => ButtonStatus::HeldDown,
                    ButtonStatus::NotPressed => ButtonStatus::JustReleased,
                    ButtonStatus::JustReleased => ButtonStatus::NotPressed,
                    ButtonStatus::HeldDown => ButtonStatus::HeldDown,
                },
            }
        };

        MouseInput {
            lmb: match_buttons(prev_state.lmb, just_polled_state.lmb),
            rmb: match_buttons(prev_state.rmb, just_polled_state.rmb),
            cursor_pos: just_polled_state.cursor_pos,
        }
    }
}
