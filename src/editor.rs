use sdl2::pixels::{Color, PixelFormat};

use crate::game::WindowDetails;

pub struct EditorWindow {
    pub title: String,
    pub pos_x: u32,
    pub pos_y: u32,
    pub width: u32,
    pub height: u32,
    pub bg_col: Color,
}

pub struct Editor {
    pub window_stack: Vec<EditorWindow>,
}

impl Editor {
    pub fn new() -> Editor {
        Editor { window_stack: Vec::new() }
    }

    pub fn add_window(&mut self, new_window: EditorWindow) {
        self.window_stack.push(new_window);
    }

    pub fn add_default_window(&mut self, game_window: WindowDetails) {
        let default_window = EditorWindow {
            title: "Default Window".to_owned(),
            pos_x: 0,
            pos_y: 0,
            width: game_window.width,
            height: game_window.height,
            bg_col: Color::GREY,
        };

        self.add_window(default_window);
    }
}
