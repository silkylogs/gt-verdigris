use sdl2::{pixels::Color, rect::Rect};

use crate::game::{self, WindowDetails};

// As of now, this struct is hardwired to have a fixed window layout
// Ideally, it should contain an enum which could be one of many layouts
pub struct EditorWindow {
    // General settings
    pub upper_left_x: u32,
    pub upper_left_y: u32,
    pub overall_width: u32,
    pub overall_height: u32,
    client_area_width: u32,
    client_area_height: u32,
    client_area_padding: u32,
    pub bg_col: Color,

    // Title
    pub title: String,
    pub title_col: Color,
    pub title_bar_col: Color,
    title_bar_width: u32,
    title_bar_height: u32,
}

impl EditorWindow {
    pub fn new(
        title: String,
        title_col: Color,
        title_bar_col: Color,

        upper_left_x: u32,
        upper_left_y: u32,
        overall_width: u32,
        overall_height: u32,
        client_area_padding: u32,
        bg_col: Color,
    ) -> EditorWindow {
        EditorWindow {
            title: title,
            title_col: title_col,
            title_bar_col: title_bar_col,
            title_bar_width: overall_width,
            title_bar_height: 20,

            upper_left_x: upper_left_x,
            upper_left_y: upper_left_y,
            overall_width: overall_width,
            overall_height: overall_height,
            client_area_width: overall_width - client_area_padding,
            client_area_height: overall_height - client_area_padding,
            client_area_padding: client_area_padding,
            bg_col: bg_col,
        }
    }
    //pub fn new_default() -> EditorWindow;

    pub fn title_bar_rect(&self) -> Rect {
        Rect::new(
            self.upper_left_x as i32,
            self.upper_left_y as i32,
            self.title_bar_width,
            self.title_bar_height
        )
    }

    pub fn client_area_rect(&self) -> Result<Rect, String> {
        let upper_x: i32 = (
            self.upper_left_x + 
            self.client_area_padding)
            .try_into().unwrap();
        let upper_y: i32 = (
            self.upper_left_x + 
            self.client_area_padding +
            self.title_bar_height)
            .try_into().unwrap();

        Ok(Rect::new(
            upper_x,
            upper_y,
            self.client_area_width,
            self.client_area_height
        ))
    }
}

pub struct Editor {
    pub window_stack: Vec<EditorWindow>,
}

impl Editor {
    pub fn new() -> Editor {
        Editor {
            window_stack: Vec::new(),
        }
    }

    pub fn add_window(&mut self, new_window: EditorWindow) {
        self.window_stack.push(new_window);
    }

    pub fn add_default_window(&mut self, game_window: WindowDetails) {
        self.add_window(EditorWindow::new(
            "Default Window".to_owned(),
            Color::WHITE,
            Color::BLUE,
            0,
            0,
            game_window.width,
            game_window.height,
            2,
            Color::GREY,
        ));
    }
}
