// Note to self: if build fails on mac, try `source ~/.zshenv`

extern crate sdl2;

mod bitmap;
mod game;
mod renderer;
mod editor;

use bitmap::Bitmap;
use game::WindowDetails;
use sdl2::event::Event;
use sdl2::keyboard::Keycode;
use sdl2::pixels::Color;
use sdl2::ttf::FontStyle;
use std::time::Duration;
use editor::Editor;

fn main() -> Result<(), String> {
    let sdl_context = sdl2::init()?;
    let video_subsystem = sdl_context.video()?;

    let mut game_state = game::Game {
        window: WindowDetails::new("Green Top: Verdigris".to_string(), 800, 600),
        player: game::Player {
            pos_x: 200.0,
            pos_y: 200.0,
            sprite: Bitmap::new(20, 20).unwrap(),
        },
    };

    let game_editor = Editor {
        title: "Game editor window".to_owned(),
        col_bg: Color::RGB(20, 20, 25),
    };

    let window = video_subsystem
        .window(
            &game_state.window.title,
            game_state.window.width,
            game_state.window.height,
        )
        .position_centered()
        .opengl()
        .build()
        .map_err(|e| e.to_string())?;

    let ttf_context = sdl2::ttf::init().map_err(|e| e.to_string())?;
    let font_path = std::path::Path::new("./Arial.ttf");
    let mut font = ttf_context.load_font(font_path, 128)?;
    font.set_style(FontStyle::NORMAL);
    let font = font;

    let mut renderer = renderer::Renderer::new(window)?;
    let mut event_pump = sdl_context.event_pump()?;

    // Draw the test sprite
    let width = game_state.player.sprite.width();
    let height = game_state.player.sprite.width();
    for y in 0..height {
        for x in 0..width {
            let r: u8 = ((x as f32 / width as f32) * 256.0) as u8;
            let g: u8 = ((y as f32 / height as f32) * 256.0) as u8;
            let b: u8 = 50;
            let color = Color::RGB(r, g, b);

            game_state.player.sprite.draw(x, y, color);
        }
    }

    'running: loop {
        // Misc event handling
        for event in event_pump.poll_iter() {
            match event {
                Event::Quit { .. }
                | Event::KeyDown {
                    keycode: Some(Keycode::Escape),
                    ..
                } => break 'running,

                // TODO: to mitigate the issue of stuttering after holding the key,
                // try creating a wrapper around SDL_GetKeyboardState()
                // or create a custom key map
                Event::KeyDown {
                    keycode: Some(keycode),
                    ..
                } => {
                    if keycode == Keycode::D {
                        game_state.player.pos_x += 10.0
                    };
                    if keycode == Keycode::A {
                        game_state.player.pos_x -= 10.0
                    };
                    if keycode == Keycode::S {
                        game_state.player.pos_y += 10.0
                    };
                    if keycode == Keycode::W {
                        game_state.player.pos_y -= 10.0
                    };
                }
                _ => {}
            }
        }

        renderer.draw_all(&game_state, &font, &game_editor)?;
        renderer.present();
        std::thread::sleep(Duration::new(0, 1_000_000_000_u32 / 30));
    }

    Ok(())
}
