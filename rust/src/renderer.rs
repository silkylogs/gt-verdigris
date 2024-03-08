use crate::sdl2::pixels::Color;
use crate::sdl2::render::WindowCanvas;
use crate::sdl2::video::Window;
use sdl2::rect::Rect;
use sdl2::render::TextureQuery;
use sdl2::ttf::Font;

#[macro_export]
macro_rules! rect(
    ($x:expr, $y:expr, $w:expr, $h:expr) => (
        Rect::new($x as i32, $y as i32, $w as u32, $h as u32)
    )
);

/*
 Todo: Experiment with RenderData
 This is a way to have libraries which depend on this library have a way of filling out
 data compatible with this crate, and sending it back to said crate for processing
 Also what on earth is a lifetime
*/
#[derive(Clone)]
pub enum RenderData<'a, 'b, 'c> {
    Rect(sdl2::rect::Rect, Color),
    FilledRect(sdl2::rect::Rect, Color),
    Text(sdl2::rect::Rect, String, Color, &'a sdl2::ttf::Font<'b, 'c>),
}

pub struct Renderer {
    pub canvas: WindowCanvas,
}

impl Renderer {
    #[allow(dead_code)]
    pub fn new(window: Window) -> Result<Renderer, String> {
        let canvas = window.into_canvas().build().map_err(|e| e.to_string())?;
        Ok(Renderer { canvas })
    }

    fn draw(&mut self, drawable: &RenderData) -> Result<(), String> {
        match drawable {
            RenderData::Rect(r, col) => self.draw_rect(r, col),
            RenderData::FilledRect(r, col) => self.draw_filled_rect(r, col),
            RenderData::Text(rect, title, color, font) => {
                self.draw_text(rect.clone(), title, color.clone(), font.clone())
            }
        }
    }

    fn draw_rect(&mut self, rect: &Rect, color: &Color) -> Result<(), String> {
        self.canvas.set_draw_color(color.clone());
        self.canvas.draw_rect(rect.clone())
    }

    fn draw_filled_rect(&mut self, rect: &Rect, color: &Color) -> Result<(), String> {
        self.canvas.set_draw_color(color.clone());
        self.canvas.fill_rect(rect.clone())
    }

    // Draws text within bounds, without stretching
    #[allow(dead_code)]
    fn draw_text(
        &mut self,
        bounds: Rect,
        text: &str,
        color: Color,
        font: &Font,
    ) -> Result<(), String> {
        let texture_creator = self.canvas.texture_creator();

        let surface = font
            .render(&text)
            .blended(color)
            .map_err(|e| e.to_string())?;

        let texture = texture_creator
            .create_texture_from_surface(&surface)
            .map_err(|e| e.to_string())?;

        let tex_width: u32;
        let tex_height: u32;
        {
            let TextureQuery { width, height, .. } = texture.query();
            tex_height = height;
            tex_width = width;
        }

        let ratio: f32 = (bounds.width() as f32 / tex_width as f32)
            .min(bounds.height() as f32 / tex_height as f32);
        let target_rect = Rect::new(
            bounds.x(),
            bounds.y(),
            (tex_width as f32 * ratio) as u32,
            (tex_height as f32 * ratio) as u32,
        );

        self.canvas.copy(&texture, None, Some(target_rect))?;

        Ok(())
    }

    #[allow(dead_code)]
    pub fn clear_background(&mut self, color: Color) -> Result<(), String> {
        self.canvas.set_draw_color(color);
        self.canvas.clear();
        Ok(())
    }

    // #[allow(dead_code)]
    // fn draw_player(&mut self, player: &Player) -> Result<(), String> {
    //     let sprite = player.sprite.clone();
    //     let w = sprite.width();
    //     let h = sprite.height();
    //     let upper_left = (player.pos_x as u32 - w / 2, player.pos_y as u32 - h);

    //     for y_off in (0)..(h) {
    //         for x_off in (0)..(w) {
    //             let pixel_color = sprite.clone().get_color(x_off, y_off).unwrap();
    //             self.canvas.set_draw_color(pixel_color);
    //             self.canvas.draw_point(Point::new(
    //                 (upper_left.0 + x_off) as i32,
    //                 (upper_left.1 + y_off) as i32,
    //             ))?;
    //         }
    //     }

    //     Ok(())
    // }

    // #[allow(dead_code)]
    // fn draw_text_stretched(
    //     &mut self,
    //     text: &str,
    //     color: Color,
    //     font: &Font,
    //     target_rect: Rect,
    // ) -> Result<(), String> {
    //     let texture_creator = self.canvas.texture_creator();

    //     let surface = font
    //         .render(text)
    //         .blended(color)
    //         .map_err(|e| e.to_string())?;

    //     let texture = texture_creator
    //         .create_texture_from_surface(&surface)
    //         .map_err(|e| e.to_string())?;

    //     self.canvas.copy(&texture, None, Some(target_rect))?;

    //     Ok(())
    // }

    // #[allow(dead_code)]
    // fn draw_editor_windows(&mut self, editor: &Editor, font: &Font) -> Result<(), String> {
    //     let draw_debug = false;

    //     for window in &editor.window_stack {
    //         let overall_window_rect = window.get_window_rect();
    //         self.canvas.set_draw_color(window.bg_col);
    //         self.canvas.fill_rect(overall_window_rect)?;

    //         let title_bar_rect = window.title_bar_rect();
    //         self.canvas.set_draw_color(window.title_bar_col);
    //         self.canvas.fill_rect(title_bar_rect)?;
    //         let title_rect = rect!(
    //             title_bar_rect.x,
    //             title_bar_rect.y,
    //             title_bar_rect.w / 2,
    //             title_bar_rect.h
    //         );
    //         self.draw_text(&window.title, window.title_col, font, title_rect)?;

    //         let client_rect = window.client_area_rect().unwrap();
    //         if draw_debug {
    //             self.canvas.set_draw_color(Color::MAGENTA);
    //         }
    //         self.canvas.draw_rect(client_rect)?;
    //     }
    //     Ok(())
    // }

    // #[allow(dead_code)]
    // pub fn draw_all(
    //     &mut self,
    //     game_context: &Game,
    //     font: &Font,
    //     editor: &Editor,
    // ) -> Result<(), String> {
    //     self.draw_background(Color::BLACK)?;
    //     self.draw_player(&game_context.player)?;
    //     self.draw_editor_windows(editor, font)?;
    //     Ok(())
    // }

    pub fn draw_all(&mut self, render_queue: Vec<RenderData>) -> Result<(), String> {
        for drawable in render_queue.into_iter() {
            self.draw(&drawable)?;
        }

        Ok(())
    }

    #[allow(dead_code)]
    pub fn present(&mut self) {
        self.canvas.present();
    }
}
