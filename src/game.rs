use crate::Bitmap;

#[derive(Clone)]
pub struct Player {
    pub pos_x: f32,
    pub pos_y: f32,
    pub sprite: Bitmap,
}

#[derive(Clone)]
pub struct Game {
    pub window_title: String,
    pub player: Player,
}
