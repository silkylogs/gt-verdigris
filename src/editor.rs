use std::{mem::swap, time::Duration};

use sdl2::{
    mouse,
    pixels::Color,
    rect::{Point, Rect},
};

use crate::input::MouseInput;

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
    pub is_selected: bool,

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
            is_selected: false,
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

    pub fn get_window_rect(&self) -> Rect {
        self.overall_rect
    }

    pub fn get_mut_window_rect(&mut self) -> &mut Rect {
        &mut self.overall_rect
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
            Rect::new(0, 0, 400, 400),
            2,
            Color::GREY,
        ));
    }

    pub fn get_mut_topmost_window_at_coords(&mut self, coords: Point) -> Option<&mut EditorWindow> {
        self.window_stack
            .iter_mut()
            .rfind(|window| window.get_window_rect().contains_point(coords))
    }

    pub fn get_topmost_window_at_coords(&mut self, coords: Point) -> Option<&EditorWindow> {
        self.window_stack
            .iter()
            .rfind(|window| window.get_window_rect().contains_point(coords))
    }

    pub fn move_window_at_coords_to_top(&mut self, coords: Point) {
        let index = self
            .window_stack
            .iter_mut()
            .enumerate()
            .rfind(|(_, window)| window.get_window_rect().contains_point(coords))
            .map(|(idx, _)| idx);
        match index {
            Some(i) => {
                self.window_stack[i..].rotate_left(1);
            }
            _ => {}
        }
    }

    /*
    Note: window moving is slightly broken
    if window is dragged too fast, the cursor somehow "slips" off the window hitbox
    possible workaround: (thanks anon)
    ```
    I meant to say that the specified bounds that you check for in the while loop can be defined
    as difference between the current mouse position and the center of the window rect;
    that way during your while loop you can add a check at the end of the iteration that says,
    "if the window is (for whatever reason) slipping away from the mouse position, pull it back to the mouse"
    ```
    additional high level impl note:
    ```
    >check if cursor is in rect
    >check if win that owns rect is draggable
    >while mouse1 pressed offset rect.pos by mouse.xy delta * whatever value feels good, as well as
     checking to make sure the rect stays in specified bounds if there are any
    >when you leave the while loop final check that we are in a valid position, adjust if needed

    reference to:
    pub struct EditorWindow {
        overall_rect: sdl2::Rect,
    }

    impl EditorWindow {
        pub fn move_by(&mut self, delta: sdl2::rect::Point) {
            self.overall_rect.x += delta.x();
            self.overall_rect.y += delta.y();
    }}

    fn main() {
        // Somewhere inside the main loop...
        // mouse_prev_state is one frame behind mouse_just_polled_state
        let mouse_pos_delta = sdl2::rect::Point::new(
            mouse_just_polled_state.coords().x() - mouse_prev_state.coords().x(),
            mouse_just_polled_state.coords().y() - mouse_prev_state.coords().y(),
        );
        default_window.move_by(mouse_pos_delta);
    }
    ```
    */
    pub fn apply_mouse_input(&mut self, mouse_state: &MouseInput) {
        // Note to self: bind the window in some sort of ownership to the mouse if it is chosen

        // if mouse_state.lmb {
        //     match self.get_mut_topmost_window_at_coords(mouse_state.cursor_pos) {
        //         Some(w) => {
        //             println!("Captured window title: \"{}\"", w.title);
        //             let window_pos = w.client_area_rect().unwrap().top_left();
        //             let offset = window_pos - mouse_state.cursor_pos;
        //             let repositioned_pos = mouse_state.cursor_pos + offset + Point::new(100, 100);
        //             w.client_area_rect().unwrap().reposition(repositioned_pos);
        //         }
        //         _ => {}
        //     }
        // }

        // match self.get_mut_topmost_window_at_coords(mouse_state.coords()) {
        //     Some(window) => {
        //         window.is_selected = mouse_state.lmb;

        //         if window.is_selected {
        //             window.move_by(mouse_pos_delta);
        //         }
        //     },
        //     _ => {},
    }
}
