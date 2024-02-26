use sdl2::{
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
        title_bar_color: Color,

        overall_window_rect: Rect,
        client_area_padding: u32,
        background_col: Color,
    ) -> EditorWindow {
        let applied_title_bar_height = 20;
        EditorWindow {
            title: title,
            title_col: title_col,
            title_bar_col: title_bar_color,
            title_bar_width: overall_window_rect.width(),
            title_bar_height: applied_title_bar_height,

            overall_rect: overall_window_rect,
            client_area_width: overall_window_rect.width() - client_area_padding * 2,
            client_area_height: overall_window_rect.height()
                - client_area_padding * 2
                - applied_title_bar_height,
            client_area_padding: client_area_padding,
            bg_col: background_col,
            is_selected: false,
        }
    }

    #[allow(dead_code)]
    pub fn new_default() -> EditorWindow {
        EditorWindow::new(
            "title".to_owned(),
            Color::WHITE,
            Color::GRAY,
            Rect::new(0, 0, 20, 20),
            0,
            Color::BLACK,
        )
    }

    #[allow(dead_code)]
    pub fn title_bar_rect(&self) -> Rect {
        Rect::new(
            self.overall_rect.x(),
            self.overall_rect.y(),
            self.title_bar_width,
            self.title_bar_height,
        )
    }

    #[allow(dead_code)]
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

    #[allow(dead_code)]
    pub fn get_window_rect(&self) -> Rect {
        self.overall_rect
    }

    #[allow(dead_code)]
    pub fn get_mut_window_rect(&mut self) -> &mut Rect {
        &mut self.overall_rect
    }

    #[allow(dead_code)]
    pub fn move_by(&mut self, delta: Point) {
        self.overall_rect.x += delta.x();
        self.overall_rect.y += delta.y();
    }
}

pub struct Editor {
    pub window_stack: Vec<EditorWindow>,
}

impl Editor {
    #[allow(dead_code)]
    pub fn new() -> Editor {
        Editor {
            window_stack: Vec::new(),
        }
    }

    #[allow(dead_code)]
    pub fn add_window(&mut self, new_window: EditorWindow) {
        self.window_stack.push(new_window);
    }

    #[allow(dead_code)]
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

    #[allow(dead_code)]
    pub fn get_mut_topmost_window_at_coords(&mut self, coords: Point) -> Option<&mut EditorWindow> {
        self.window_stack
            .iter_mut()
            .rfind(|window| window.get_window_rect().contains_point(coords))
    }

    #[allow(dead_code)]
    pub fn get_topmost_window_at_coords(&mut self, coords: Point) -> Option<&EditorWindow> {
        self.window_stack
            .iter()
            .rfind(|window| window.get_window_rect().contains_point(coords))
    }

    #[allow(dead_code)]
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

    #[allow(dead_code)]
    pub fn apply_mouse_input(
        &mut self,
        mouse_state_updated: &MouseInput,
        mouse_state_prev: &MouseInput,
    ) {
        // Ownership
        // TODO: find a way to make ownership of window to mouse unique; maybe an index?
        // This is to fix the following bug:
        // > The bug in question happens to be windows sticking to the mouse when moved around very fast;
        // > This may be due to multiple windows possessing the is_selected flag.
        // > Possible solutions to this bug may involve using an index to choose the window to be
        // > slaved to the mouse as opposed to having an is_selected flag on all windows, which allows
        // > this siutation to happen in the first place.
        // > Using references bring back the mouse slipping bug, but that may be due to a skill issue on my part.
        if mouse_state_updated.lmb {
            let window = self.get_mut_topmost_window_at_coords(mouse_state_updated.cursor_pos);
            if window.is_some() {
                let mut w = window.unwrap();
                w.is_selected = true;
                self.move_window_at_coords_to_top(mouse_state_updated.coords());
            }
        }
        if !mouse_state_updated.lmb {
            for window in self.window_stack.iter_mut() {
                window.is_selected = false;
            }
        }

        // Updating
        for window in self.window_stack.iter_mut() {
            if window.is_selected {
                let offset = mouse_state_prev.cursor_pos - window.get_window_rect().top_left();
                let applied_pos = mouse_state_updated.cursor_pos - offset;
                window.get_mut_window_rect().reposition(applied_pos);
            }
        }
    }
}
