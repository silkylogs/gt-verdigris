// Note to people building on a mac:
// if build fails, try `source ~/.zshenv`

mod glass_16;
use raylib::prelude::*;

fn main() {
    let mut memory = vec![0u8; 0x100];
    let mut vm = glass_16::Vm::new();
    vm.step(&mut memory);

    let (mut rl, thread) = raylib::init().size(640, 480).title("GTV").vsync().build();

    while !rl.window_should_close() {
        let mut d = rl.begin_drawing(&thread);

        //handle_input();

        d.clear_background(Color::BLACK);
        d.draw_text("Hell", 12, 12, 24, Color::WHITE);
    }
}
