use crate::Bitmap;

#[derive(Clone)]
pub struct Player {
    pub pos_x: f32,
    pub pos_y: f32,
    pub sprite: Bitmap,
}

#[derive(Clone)]
pub struct WindowDetails {
    pub title: String,
    pub width: u32,
    pub height: u32,
}

impl WindowDetails {
    pub fn new(title: String, width: u32, height: u32) -> WindowDetails {
        WindowDetails {
            title,
            width,
            height,
        }
    }
}

#[derive(Clone)]
pub struct Game {
    pub window: WindowDetails,
    pub player: Player,
}
