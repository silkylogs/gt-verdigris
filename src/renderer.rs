use crate::sdl2::rect::Rect;
use crate::sdl2::rect::Point;
use crate::sdl2::render::WindowCanvas;
use crate::sdl2::video::Window;
use crate::sdl2::pixels::Color;

use crate::game::*;

pub struct Renderer {
    canvas: WindowCanvas
}

impl Renderer {
    pub fn new(window: Window) -> Result<Renderer, String> {
        let canvas = window
        .into_canvas()
        .build()
        .map_err(|e| e.to_string())?;
        Ok(Renderer { canvas })
    }

    fn draw_background(&mut self) -> Result<(), String> {
        self.canvas.set_draw_color(Color::BLACK);
        self.canvas.clear();
        Ok(())
    }

    fn draw_player(&mut self, player: Player) -> Result<(), String> {
        let sprite = player.sprite.clone();
        let w = sprite.width();
        let h = sprite.height();
        let upper_left = (
            player.pos_x as u32 - w / 2,
            player.pos_y as u32 - h,
        );

        for y_off in (0)..(h) {
            for x_off in (0)..(w) {
                let pixel_color = sprite.clone().get_color(x_off, y_off).unwrap();
                self.canvas.set_draw_color(pixel_color);
                self.canvas.draw_point(Point::new(
                    (upper_left.0 + x_off) as i32,
                    (upper_left.1 + y_off) as i32
                ))?;
            }
        }

        Ok(())
    }

    pub fn draw(&mut self, game_context: Game) -> Result<(), String> {
        self.draw_background()?;
        self.draw_player(game_context.player)?;
        self.canvas.present();
        Ok(())
    }
}