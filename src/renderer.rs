use crate::editor::Editor;
use crate::game::*;
use crate::sdl2::pixels::Color;
use crate::sdl2::rect::Point;
use crate::sdl2::render::WindowCanvas;
use crate::sdl2::video::Window;
use sdl2::rect::Rect;
use sdl2::ttf::Font;

macro_rules! rect(
    ($x:expr, $y:expr, $w:expr, $h:expr) => (
        Rect::new($x as i32, $y as i32, $w as u32, $h as u32)
    )
);

pub struct Renderer {
    canvas: WindowCanvas,
}

impl Renderer {
    pub fn new(window: Window) -> Result<Renderer, String> {
        let canvas = window.into_canvas().build().map_err(|e| e.to_string())?;
        Ok(Renderer { canvas: canvas })
    }

    fn draw_background(&mut self, color: Color) -> Result<(), String> {
        self.canvas.set_draw_color(color);
        self.canvas.clear();
        Ok(())
    }

    fn draw_player(&mut self, player: &Player) -> Result<(), String> {
        let sprite = player.sprite.clone();
        let w = sprite.width();
        let h = sprite.height();
        let upper_left = (player.pos_x as u32 - w / 2, player.pos_y as u32 - h);

        for y_off in (0)..(h) {
            for x_off in (0)..(w) {
                let pixel_color = sprite.clone().get_color(x_off, y_off).unwrap();
                self.canvas.set_draw_color(pixel_color);
                self.canvas.draw_point(Point::new(
                    (upper_left.0 + x_off) as i32,
                    (upper_left.1 + y_off) as i32,
                ))?;
            }
        }

        Ok(())
    }

    fn draw_text(
        &mut self,
        text: &str,
        color: Color,
        font: &Font,
        (x, y): (u32, u32),
        (w, h): (u32, u32),
    ) -> Result<(), String> {
        let texture_creator = self.canvas.texture_creator();

        let surface = font
            .render(text)
            .blended(color)
            .map_err(|e| e.to_string())?;

        let texture = texture_creator
            .create_texture_from_surface(&surface)
            .map_err(|e| e.to_string())?;

        /*
        // Note: these are the quantities available for transforming text
        let window_dimensions = self.canvas.window().size();
        let TextureQuery { width, height, .. } = texture.query();
        let padding = 64;
        */
        let target = rect!(x, y, w, h);

        self.canvas.copy(&texture, None, Some(target))?;

        Ok(())
    }

    fn draw_editor_windows(&mut self, editor: &Editor, font: &Font) -> Result<(), String> {
        for window in &editor.window_stack {
            let window_rect = rect!(
                window.pos_x, window.pos_y,
                window.width, window.height
            );
            self.canvas.set_draw_color(window.bg_col);
            self.canvas.fill_rect(window_rect)?;

            self.draw_text(
                &window.title,
                Color::WHITE,
                font,
                (window.pos_x, window.pos_y),
                (
                    window.width / 2,
                    window.height / 32,
                ),
            )?;
        }
        Ok(())
    }

    pub fn draw_all(
        &mut self,
        game_context: &Game,
        font: &Font,
        editor: &Editor,
    ) -> Result<(), String> {
        self.draw_background(Color::BLACK)?;
        self.draw_player(&game_context.player)?;
        self.draw_editor_windows(editor, font)?;
        Ok(())
    }

    pub fn present(&mut self) {
        self.canvas.present();
    }
}
