// Note to people building on a mac:
// if build fails, try `source ~/.zshenv`

mod glass_16;
use raylib::prelude::*;

fn main() {
    #[rustfmt::skip]
    let mut memory: Vec<u8> = vec![
        0x2E, 0xFF, 0x00, 0x00
    ];
    let mut vm = glass_16::Vm::new();

    let (mut rl, thread) = raylib::init().size(640, 480).title("GTV").vsync().build();

    while !rl.window_should_close() {
        vm.step(&mut memory);
        println!("{}", vm.formatted_registers());

        // let mut d = rl.begin_drawing(&thread);
        // d.clear_background(Color::BLACK);

        // let width = 200;
        // let height = 100;
        // let off_x = 000;
        // let off_y = 000;
        // for i in 0..=width * height {
        //     let x = i % width + off_x;
        //     let y = i / width + off_y;
        //     d.draw_pixel(x, y, Color::new(x as u8, y as u8, 0xFF, 0xFF));
        // }

        //d.draw_text("Hell", 12, 12, 24, Color::WHITE);
    }
}
