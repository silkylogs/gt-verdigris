// Note to self: if build fails on mac, try `source ~/.zshenv`

extern crate sdl2;

mod bitmap;
mod editor;
mod game;
mod input;
mod renderer;

use bitmap::Bitmap;
use editor::{Editor, EditorWindow};
use game::WindowDetails;
use sdl2::event::Event;
use sdl2::keyboard::Keycode;
use sdl2::pixels::Color;
use sdl2::rect::{Point, Rect};
use sdl2::timer;
use sdl2::ttf::FontStyle;
use std::time::{Duration, Instant};

fn main() -> Result<(), String> {
    let sdl_context = sdl2::init()?;
    let video_subsystem = sdl_context.video()?;

    let game_window_details = WindowDetails::new("Green Top: Verdigris".to_string(), 800, 600);
    let mut game_state = game::Game {
        window: game_window_details.clone(),
        player: game::Player {
            pos_x: 200.0,
            pos_y: 200.0,
            sprite: Bitmap::new(20, 20).unwrap(),
        },
    };

    let mut game_editor = Editor::new();
    game_editor.add_default_window();
    game_editor.add_window(EditorWindow::new(
        "Window 2".to_owned(),
        Color::YELLOW,
        Color::BLUE,
        Rect::new(100, 100, 300, 200),
        2,
        Color::RGB(20, 20, 25),
    ));
    game_editor.add_window(EditorWindow::new(
        "Window 3".to_owned(),
        Color::WHITE,
        Color::RGB(77, 30, 35),
        Rect::new(150, 200, 300, 200),
        1,
        Color::YELLOW,
    ));
    game_editor.add_window(EditorWindow::new(
        "Window 4".to_owned(),
        Color::GREEN,
        Color::GREY,
        Rect::new(250, 300, 300, 200),
        2,
        Color::RGB(40, 40, 45),
    ));

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

    let mut baseline_time = Instant::now();

    let mut mouse_just_polled_state = input::MouseInput::new_default();
    let mut lmb_duration = Duration::from_secs(0);
    'running: loop {
        let mouse_prev_state = mouse_just_polled_state;

        // Misc event handling
        for event in event_pump.poll_iter() {
            match event {
                Event::Quit { .. }
                | Event::KeyDown {
                    keycode: Some(Keycode::Escape),
                    ..
                } => break 'running,

                Event::MouseButtonDown { mouse_btn, timestamp, .. } => {
                    let curr_timestamp = Duration::from_millis(timestamp as u64);

                    baseline_time = Instant::now();

                    match mouse_btn {
                        sdl2::mouse::MouseButton::Left => {
                            mouse_just_polled_state = input::MouseInput { 
                                lmb: true,
                                lmb_timestamp: curr_timestamp,
                                rmb: mouse_just_polled_state.rmb,
                                rmb_timestamp: mouse_just_polled_state.rmb_timestamp,
                                cursor_pos: mouse_just_polled_state.cursor_pos,
                            }
                        }
                        sdl2::mouse::MouseButton::Right => {
                            mouse_just_polled_state = input::MouseInput { 
                                rmb: true,
                                rmb_timestamp: curr_timestamp,
                                lmb: mouse_just_polled_state.lmb,
                                lmb_timestamp: mouse_just_polled_state.lmb_timestamp,
                                cursor_pos: mouse_just_polled_state.cursor_pos,
                            }
                        },
                        _ => {},
                    };

                    (lmb_duration, _) = input::MouseInput::timestamp_diff(
                        mouse_just_polled_state,
                        mouse_prev_state
                    );
                    println!("MB down event detected");
                }

                Event::MouseButtonUp { mouse_btn, timestamp, .. } => {
                    let curr_timestamp = Duration::from_millis(timestamp as u64);
                    match mouse_btn {
                        sdl2::mouse::MouseButton::Left => {
                            mouse_just_polled_state = input::MouseInput { 
                                lmb: false,
                                lmb_timestamp: curr_timestamp,
                                rmb: mouse_just_polled_state.rmb,
                                rmb_timestamp: mouse_just_polled_state.rmb_timestamp,
                                cursor_pos: mouse_just_polled_state.cursor_pos,
                            }
                        }
                        sdl2::mouse::MouseButton::Right => {
                            mouse_just_polled_state = input::MouseInput { 
                                rmb: false,
                                rmb_timestamp: curr_timestamp,
                                lmb: mouse_just_polled_state.lmb,
                                lmb_timestamp: mouse_just_polled_state.lmb_timestamp,
                                cursor_pos: mouse_just_polled_state.cursor_pos,
                            }
                        },
                        _ => {},
                    };

                    (lmb_duration, _) = input::MouseInput::timestamp_diff(
                        mouse_just_polled_state,
                        mouse_prev_state
                    );

                    baseline_time = Instant::now();
                    // let time_held_down = input::MouseInput::timestamp_diff(
                    //     mouse_just_polled_state,
                    //     mouse_prev_state
                    // );
                    // println!("Mouse buttons held down for: {:?}", time_held_down);
                }

                Event::MouseMotion { x, y, .. } => {
                    mouse_just_polled_state.poll_motion(x, y);
                }

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

        // let applied_mouse_state = MouseInput::update(mouse_prev_state, mouse_just_polled_state);
        let delta = Point::new(
            mouse_just_polled_state.coords().x() - mouse_prev_state.coords().x(),
            mouse_just_polled_state.coords().y() - mouse_prev_state.coords().y(),
        );
        //game_editor.apply_mouse_input(&applied_mouse_state, delta);

        if mouse_just_polled_state.lmb {
            println!("Mouse buttons held down for: {:?}", baseline_time.elapsed());
        }

        renderer.draw_all(&game_state, &font, &game_editor)?;
        renderer.present();
        std::thread::sleep(Duration::new(0, 1_000_000_000_u32 / 30));
    }

    Ok(())
}
