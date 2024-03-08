use sdl2::pixels::Color;

#[derive(Clone)]
pub struct Bitmap {
    width: u32,
    height: u32,
    colors: Vec<Color>,
}

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

    // Error could be better modelled by an enum
    fn xy_to_idx(&self, x: u32, y: u32) -> Result<usize, ()> {
        if self.width < x || self.height < y {
            return Err(());
        }

        match (y * self.width + x).try_into() {
            Ok(i) => Ok(i),
            Err(_) => Err(()),
        }
    }

    fn get_mut(&mut self, idx: usize) -> Result<&mut Color, ()> {
        match self.colors.get_mut(idx) {
            Some(color) => Ok(color),
            None => Err(()),
        }
    }

    fn get(&self, idx: usize) -> Result<&Color, ()> {
        match self.colors.get(idx) {
            Some(color) => Ok(color),
            None => Err(()),
        }
    }

    pub fn draw(&mut self, x: u32, y: u32, color: Color) -> Option<()> {
        let idx = self.clone().xy_to_idx(x, y).unwrap();
        let color_ref = self.get_mut(idx).unwrap();
        *color_ref = color;

        Some(())
    }

    pub fn get_color(&self, x: u32, y: u32) -> Option<Color> {
        let idx = self.xy_to_idx(x, y).unwrap();
        let color_ref = self.get(idx).unwrap();

        Some(*color_ref)
    }

    pub fn height(&self) -> u32 {
        self.height
    }
    pub fn width(&self) -> u32 {
        self.width
    }
}
