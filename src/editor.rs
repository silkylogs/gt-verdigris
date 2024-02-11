use sdl2::{pixels::Color, rect::{Point, Rect}};

use crate::{game::WindowDetails, input::{ButtonStatus, MouseInput}};

// As of now, this struct is hardwired to have a fixed window layout
// Ideally, it should contain an enum which could be one of many layouts
#[derive(Debug)]
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
        let title_bar_height = 20;
        EditorWindow {
            title: title,
            title_col: title_col,
            title_bar_col: title_bar_col,
            title_bar_width: overall_width,
            title_bar_height: title_bar_height,

            upper_left_x: upper_left_x,
            upper_left_y: upper_left_y,
            overall_width: overall_width,
            overall_height: overall_height,
            client_area_width: overall_width - client_area_padding * 2,
            client_area_height: overall_height - client_area_padding * 2 - title_bar_height,
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
            self.title_bar_height,
        )
    }

    pub fn client_area_rect(&self) -> Result<Rect, String> {
        let upper_x: i32 = (self.upper_left_x + self.client_area_padding)
            .try_into()
            .unwrap();
        let upper_y: i32 = (self.upper_left_y + self.client_area_padding + self.title_bar_height)
            .try_into()
            .unwrap();

        Ok(Rect::new(
            upper_x,
            upper_y,
            self.client_area_width,
            self.client_area_height,
        ))
    }

    pub fn window_rect(&self) -> Rect {
        Rect::new(
            self.upper_left_x as i32, 
            self.upper_left_y as i32, 
            self.overall_width, 
            self.overall_height
        )
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
    
    pub fn get_mut_topmost_window_at_coords(&mut self, (x, y): (u32, u32)) -> Option<&mut EditorWindow> {
        for window in self.window_stack.iter_mut().rev() {
            let hitbox = window.window_rect();
            if hitbox.contains_point(Point::new(x as i32, y as i32)) {
                
                return Some(window);
            }
        }
        None
    }

    pub fn apply_mouse_input(&mut self, mouse_state: &MouseInput) {
        let coords = mouse_state.coords();
        if mouse_state.lmb() == ButtonStatus::HeldDown {
            //todo!("Find a better name for this");
            match self.get_mut_topmost_window_at_coords(coords) {
                Some(window) => println!("Found window \"{}\"", window.title),
                None => println!("Found nothing"),
            };
        }
    }
}
