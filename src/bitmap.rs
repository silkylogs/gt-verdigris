use sdl2::pixels::Color;

#[derive(Clone)]
pub struct Bitmap {
    width: u32,
    height: u32,
    colors: Vec<Color>,
}

// todo!("Reimplement this struct to clear up the unsightly .clone() calls")
impl Bitmap {
    pub fn new(w: u32, h: u32) -> Option<Bitmap> {
        let memory = vec![Color::BLUE; (w * h).try_into().unwrap()];

        let bitmap = Bitmap {
            width: w,
            height: h,
            colors: memory,
        };

        Some(bitmap)
    }

    fn xy_to_idx(self, x: u32, y: u32) -> Option<usize> {
        if self.width < x || self.height < y {
            return None;
        }
        let idx: usize = match (y * self.width + x).try_into() {
            Ok(i) => i,
            Err(_) => {
                return None;
            }
        };
        //dbg!(x, y, idx);
        Some(idx)
    }

    fn get_color_ref(&mut self, index: usize) -> Option<&mut Color> {
        if !(index < self.colors.len()) {
            return None;
        }
        Some(&mut self.colors[index])
    }

    pub fn draw(&mut self, x: u32, y: u32, color: Color) -> Option<()> {
        let idx = self.clone().xy_to_idx(x, y).unwrap();
        let color_ref = self.get_color_ref(idx).unwrap();
        *color_ref = color;

        Some(())
    }

    pub fn get_color(self, x: u32, y: u32) -> Option<Color> {
        let cloned_self = &mut self.clone();
        let idx = self.xy_to_idx(x, y).unwrap();
        let color_ref = cloned_self.get_color_ref(idx).unwrap();
        let color = color_ref.clone();

        Some(color)
    }

    pub fn height(&self) -> u32 {
        self.height
    }
    pub fn width(&self) -> u32 {
        self.width
    }
}