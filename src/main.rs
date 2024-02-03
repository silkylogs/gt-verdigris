extern crate sdl2;

use sdl2::event::Event;
use sdl2::keyboard::Keycode;
use sdl2::pixels::Color;
use sdl2::rect::Rect;
use sdl2::sys::{KeyCode, KeySym, SDL_KeyCode, SDL_Rect};
use std::time::Duration;

mod game;

fn main() -> Result<(), String> {
    let sdl_context = sdl2::init()?;
    let video_subsystem = sdl_context.video()?;

    let mut game_state = game::Game {
        window_title: "Green Top: Verdigris".to_string(),
        player: game::Player {
            pos_x: 0.0,
            pos_y: 0.0,
        },
    };

    let window = video_subsystem
        .window(&game_state.window_title, 800, 600)
        .position_centered()
        .opengl()
        .build()
        .map_err(|e| e.to_string())?;

    let mut canvas = window.into_canvas().build().map_err(|e| e.to_string())?;

    let mut event_pump = sdl_context.event_pump()?;

    'running: loop {
        for event in event_pump.poll_iter() {
            match event {
                Event::Quit { .. }
                | Event::KeyDown {
                    keycode: Some(Keycode::Escape),
                    ..
                } => break 'running,
                Event::KeyDown { keycode: Some(Keycode::D), .. } => {
                    game_state.player.pos_x += 1.0;
                }
                Event::KeyDown { keycode: Some(Keycode::S), .. } => {
                    game_state.player.pos_y += 1.0;
                }
                _ => {}
            }
        }

        canvas.clear();
        canvas.set_draw_color(Color::GREEN);
        canvas.fill_rect(Rect::new(
            game_state.player.pos_x as i32,
            game_state.player.pos_y as i32,
            5,
            5,
        ))?;
        canvas.set_draw_color(Color::BLACK);

        canvas.present();
        std::thread::sleep(Duration::new(0, 1_000_000_000_u32 / 30));
    }

    Ok(())
}
