use crate::game::*;
use crate::sdl2::pixels::Color;
use crate::sdl2::rect::Point;
use crate::sdl2::render::WindowCanvas;
use crate::sdl2::video::Window;
use sdl2::rect::Rect;
use sdl2::render::TextureQuery;
use sdl2::ttf::Font;

// handle the annoying Rect i32
macro_rules! rect(
    ($x:expr, $y:expr, $w:expr, $h:expr) => (
        Rect::new($x as i32, $y as i32, $w as u32, $h as u32)
    )
);

// Scale fonts to a reasonable size when they're too big (though they might look less smooth)
fn get_centered_rect(rect_width: u32, rect_height: u32, cons_width: u32, cons_height: u32) -> Rect {
    let wr = rect_width as f32 / cons_width as f32;
    let hr = rect_height as f32 / cons_height as f32;

    let (w, h) = if wr > 1f32 || hr > 1f32 {
        if wr > hr {
            //println!("Scaling down! The text will look worse!");
            let h = (rect_height as f32 / wr) as i32;
            (cons_width as i32, h)
        } else {
            //println!("Scaling down! The text will look worse!");
            let w = (rect_width as f32 / hr) as i32;
            (w, cons_height as i32)
        }
    } else {
        (rect_width as i32, rect_height as i32)
    };

    let cx = (800 as i32 - w) / 2;
    let cy = (600 as i32 - h) / 2;
    rect!(cx, cy, w, h)
}

pub struct Renderer {
    canvas: WindowCanvas,
}

impl Renderer {
    pub fn new(window: Window) -> Result<Renderer, String> {
        let canvas = window.into_canvas().build().map_err(|e| e.to_string())?;
        Ok(Renderer { canvas: canvas })
    }

    fn draw_background(&mut self) -> Result<(), String> {
        self.canvas.set_draw_color(Color::BLACK);
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

    // Assumes ttf_context is initialized
    pub fn draw_text(&mut self, text: &str, color: Color, font: &Font) -> Result<(), String> {
        let texture_creator = self.canvas.texture_creator();

        let surface = font
            .render(text)
            .blended(color)
            .map_err(|e| e.to_string())?;

        let texture = texture_creator
            .create_texture_from_surface(&surface)
            .map_err(|e| e.to_string())?;

        let window_dimensions = self.canvas.window().size();
        let TextureQuery { width, height, .. } = texture.query();
        let padding = 64;
        let target = get_centered_rect(
            width,
            height,
            window_dimensions.0 - padding,
            window_dimensions.1 - padding,
        );

        self.canvas.copy(&texture, None, Some(target))?;

        Ok(())
    }

    pub fn draw_all(
        &mut self,
        game_context: &Game,
        font: &Font,
    ) -> Result<(), String> {
        self.draw_background()?;
        self.draw_player(&game_context.player)?;
        self.draw_text(
            "Hello, world!",
            Color::GREY,
            font,
        )?;
        Ok(())
    }

    pub fn present(&mut self) {
        self.canvas.present();
    }
}
