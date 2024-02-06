#[derive(Copy, Clone, Debug)]
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
    x: u32,
    y: u32,
}

impl MouseInput {
    pub fn new_default() -> MouseInput {
        MouseInput {
            lmb: ButtonStatus::NotPressed,
            rmb: ButtonStatus::NotPressed,
            x: 0,
            y: 0,
        }
    }

    pub fn lmb(&self) -> ButtonStatus {
        self.lmb
    }

    pub fn rmb(&self) -> ButtonStatus {
        self.rmb
    }

    pub fn coords(&self) -> (u32, u32) {
        (self.x, self.y)
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

    pub fn poll_motion(&mut self, x: u32, y: u32) {
        self.x = x;
        self.y = y;
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
            x: just_polled_state.x,
            y: just_polled_state.y,
        }
    }
}
