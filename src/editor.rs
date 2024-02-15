use sdl2::{
    pixels::Color,
    rect::{Point, Rect},
};

use crate::input::{ButtonStatus, MouseInput};

// As of now, this struct is hardwired to have a fixed window layout
// Ideally, it should contain an enum which could be one of many layouts
#[derive(Debug)]
pub struct EditorWindow {
    // General settings
    overall_rect: Rect,
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

        overall_window_rect: Rect,
        client_area_padding: u32,
        bg_col: Color,
    ) -> EditorWindow {
        let title_bar_height = 20;
        EditorWindow {
            title: title,
            title_col: title_col,
            title_bar_col: title_bar_col,
            title_bar_width: overall_window_rect.width(),
            title_bar_height: title_bar_height,

            overall_rect: overall_window_rect,
            client_area_width: overall_window_rect.width() - client_area_padding * 2,
            client_area_height: overall_window_rect.height()
                - client_area_padding * 2
                - title_bar_height,
            client_area_padding: client_area_padding,
            bg_col: bg_col,
        }
    }
    //pub fn new_default() -> EditorWindow;

    pub fn title_bar_rect(&self) -> Rect {
        Rect::new(
            self.overall_rect.x(),
            self.overall_rect.y(),
            self.title_bar_width,
            self.title_bar_height,
        )
    }

    pub fn client_area_rect(&self) -> Result<Rect, String> {
        let upper_x: i32 = self.overall_rect.x() + self.client_area_padding as i32;
        let upper_y: i32 =
            self.overall_rect.y() + (self.client_area_padding + self.title_bar_height) as i32;

        Ok(Rect::new(
            upper_x,
            upper_y,
            self.client_area_width,
            self.client_area_height,
        ))
    }

    pub fn window_rect(&self) -> Rect {
        self.overall_rect
    }

    pub fn move_by(&mut self, delta: Point) {
        self.overall_rect.x += delta.x();
        self.overall_rect.y += delta.y();
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

    pub fn add_default_window(&mut self) {
        self.add_window(EditorWindow::new(
            "Default Window".to_owned(),
            Color::WHITE,
            Color::BLUE,
            Rect::new(0, 0, 800, 600),
            2,
            Color::GREY,
        ));
    }

    pub fn get_mut_topmost_window_at_coords(&mut self, coords: Point) -> Option<&mut EditorWindow> {
        self.window_stack
            .iter_mut()
            .rfind(|window| window.window_rect().contains_point(coords))
    }

    pub fn apply_mouse_input(&mut self, mouse_state: &MouseInput, mouse_pos_delta: Point) {
        let coords = mouse_state.coords();
        if mouse_state.lmb() == ButtonStatus::HeldDown {
            //todo!("Find a better name for this");
            match self.get_mut_topmost_window_at_coords(coords) {
                Some(window) => {
                    //println!("Found window \"{}\"", window.title)
                    window.move_by(mouse_pos_delta);
                }
                None => println!("Found nothing"),
            };
        }
    }
}
