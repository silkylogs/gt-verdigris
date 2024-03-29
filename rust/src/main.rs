// Note to people building on a mac:
// if build fails, try `source ~/.zshenv`

extern crate sdl2;

mod bitmap;
mod editor;
mod game;
mod input;
mod renderer;

use bitmap::Bitmap;
use editor::{Editor, EditorWindow};
use game::WindowDetails;
use renderer::RenderData;
use sdl2::keyboard::Keycode;
use sdl2::pixels::Color;
use sdl2::ttf::FontStyle;
use sdl2::{event::Event, rect::Rect};
use std::time::{Duration, Instant};

fn main() -> Result<(), String> {
    let sdl_context = sdl2::init()?;
    let video_subsystem = sdl_context.video()?;

    let screen_width = 1024;
    let screen_height = 800;
    let game_window_details = WindowDetails::new(
        "Green Top: Verdigris".to_string(),
        screen_width,
        screen_height,
    );
    let mut game_state = game::Game {
        window: game_window_details.clone(),
        player: game::Player {
            pos_x: 200.0,
            pos_y: 200.0,
            sprite: Bitmap::new(20, 20).unwrap(),
        },
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
    let font_path = std::path::Path::new("./loaded_resources/Arial.ttf");
    let mut font = ttf_context.load_font(font_path, 128)?;
    font.set_style(FontStyle::NORMAL);
    let font = font;

    let mut game_renderer = renderer::Renderer::new(window)?;
    let mut event_pump = sdl_context.event_pump()?;

    let mut game_editor = Editor::new();
    
    // Mock the controls menu in source, for now
    let mut controls_window = EditorWindow::new(
        "Controls".to_owned(),
        Color::BLACK,
        Color::YELLOW,
        30,
        Rect::new(0, 0, screen_width, screen_height),
        5,
        Color::BLACK,
    );
    controls_window.set_draggable(false);
    game_editor.add_window(controls_window);

    // Draw the test sprite
    // let width = game_state.player.sprite.width();
    // let height = game_state.player.sprite.width();
    // for y in 0..height {
    //     for x in 0..width {
    //         let r: u8 = ((x as f32 / width as f32) * 256.0) as u8;
    //         let g: u8 = ((y as f32 / height as f32) * 256.0) as u8;
    //         let b: u8 = 50;
    //         let color = Color::RGB(r, g, b);
    //         game_state.player.sprite.draw(x, y, color);
    //     }
    // }

    let mut render_queue: Vec<RenderData> = Vec::new();

    let mut mouse_state_updated = input::MouseInput::new_default();
    'running: loop {
        let mouse_state_prev = mouse_state_updated;

        for event in event_pump.poll_iter() {
            match event {
                Event::Quit { .. } => break 'running,

                Event::MouseButtonDown { mouse_btn, .. } => {
                    match mouse_btn {
                        sdl2::mouse::MouseButton::Left => {
                            mouse_state_updated.lmb = true;
                            mouse_state_updated.lmb_held_down_instant = Instant::now();
                        }
                        sdl2::mouse::MouseButton::Right => {
                            mouse_state_updated.rmb = true;
                            mouse_state_updated.rmb_held_down_instant = Instant::now();
                        }
                        _ => {}
                    };
                }

                Event::MouseButtonUp { mouse_btn, .. } => {
                    match mouse_btn {
                        sdl2::mouse::MouseButton::Left => {
                            mouse_state_updated.lmb = false;
                            mouse_state_updated.lmb_held_down_instant = Instant::now();
                        }
                        sdl2::mouse::MouseButton::Right => {
                            mouse_state_updated.rmb = false;
                            mouse_state_updated.rmb_held_down_instant = Instant::now();
                        }
                        _ => {}
                    };
                }

                Event::MouseMotion { x, y, .. } => {
                    mouse_state_updated.poll_motion(x, y);
                }

                // TODO: to mitigate the issue of stuttering after holding the key,
                // try creating a wrapper around SDL_GetKeyboardState()
                // or create a custom key map
                Event::KeyDown {
                    keycode: Some(keycode),
                    ..
                } => {
                    // if keycode == Keycode::D {
                    //     game_state.player.pos_x += 10.0
                    // };
                    // if keycode == Keycode::A {
                    //     game_state.player.pos_x -= 10.0
                    // };
                    // if keycode == Keycode::S {
                    //     game_state.player.pos_y += 10.0
                    // };
                    // if keycode == Keycode::W {
                    //     game_state.player.pos_y -= 10.0
                    // };
                    if keycode == Keycode::Q {
                        break 'running;
                    }
                    if keycode == Keycode::E {
                        game_editor
                            .window_stack
                            .get_mut(0)
                            .unwrap()
                            .toggle_draggable();
                    }
                }

                _ => {}
            }
        }

        game_editor.apply_mouse_input(&mouse_state_updated, &mouse_state_prev);
        game_editor.draw_editor_windows(&font, &mut render_queue)?;
        render_queue.push(RenderData::Text(
            Rect::new(100, 100, 400, 400),
            "Control_window".to_owned(),
            Color::WHITE,
            &font,
        ));

        let cloned_render_queue = render_queue.clone();
        game_renderer.draw_all(cloned_render_queue)?;
        game_renderer.present();
        render_queue.clear();

        game_renderer.clear_background(Color::BLACK)?;

        std::thread::sleep(Duration::new(0, 1_000_000_000_u32 / 30));
    }

    Ok(())
}
