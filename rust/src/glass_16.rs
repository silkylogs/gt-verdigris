use std::ops::BitOrAssign;

// Registers
pub struct Vm {
    registers: [u16; 0x10],
    opcode_reg: u16,
    operand_reg: u16,
}

impl Vm {
    const VERSION: [u16; 4] = [0x4710, 0x0001, 0x0000, 0x0001];
    const MASK_ZERO_EQUAL: u16 = 0b0000_0000_0000_0001;
    const MASK_GREATER_THAN: u16 = 0b0000_0000_0000_0010;
    const MASK_LESS_THAN: u16 = 0b0000_0000_0000_0100;
    const MASK_ZERO_DIV: u16 = 0b0000_0000_0000_1000;
    const MASK_OVERFLOW: u16 = 0b0000_0000_0001_0000;
    const MASK_UNDERFLOW: u16 = 0b0000_0000_0010_0000;
    const MASK_RESERVED_INSTRUCTION: u16 = 0b0000_0000_0100_0000;
    const MASK_INVALID_INSTRUCTION: u16 = 0b0000_0000_1000_0000;

    pub fn new() -> Vm {
        Vm {
            registers: [0_u16; 0x10],
            opcode_reg: 0_u16,
            operand_reg: 0_u16,
        }
    }

    pub fn step(&mut self, memory: &mut Vec<u8>) {
        self.opcode_reg = (*memory.get(self.registers[0xE] as usize).unwrap_or(&0_u8)) as u16;
        self.registers[0xE] = match self.registers[0xE].checked_add(2_u16) {
            Some(x) => x,
            None => {
                self.registers[0xF].bitor_assign(Vm::MASK_OVERFLOW);
                self.registers[0xE].wrapping_add(2_u16)
            }
        };

        if self.should_fetch_operand() {
            self.operand_reg = (*memory.get(self.registers[0xE] as usize).unwrap_or(&0_u8)) as u16;
            self.registers[0xE] = match self.registers[0xE].checked_add(2_u16) {
                Some(x) => x,
                None => {
                    self.registers[0xF].bitor_assign(Vm::MASK_OVERFLOW);
                    self.registers[0xE].wrapping_add(2_u16)
                }
            };
        }

        self.execute_instruction_and_flag_behaviour(memory);
    }

    // -- Utility --------------------------------------------------------------

    fn get_vm_regs_opcode_operand(&self) -> ([u16; 16], u16, u16) {
        (self.registers, self.opcode_reg, self.operand_reg)
    }

    fn should_fetch_operand(&self) -> bool {
        let page2 = self.opcode_reg & 0x00F0_u16;
        if page2 == 0x0000
            || page2 == 0x0010
            || page2 == 0x0020
            || page2 == 0x0030
            || page2 == 0x0040
            || page2 == 0x0060
        {
            return true;
        }

        let page3 = self.opcode_reg & 0x000F_u16;
        if page3 == 0x0001 || page3 == 0x0002 {
            return true;
        }

        return false;
    }

    fn flags_set_hi(&mut self, flag: u16) {
        self.registers[0xF] |= flag;
    }

    fn flags_set_lo(&mut self, flag: u16) {
        self.registers[0xF] &= !flag;
    }

    // -- Utility --------------------------------------------------------------

    // -- Instruction page 0 instructions --------------------------------------

    fn p0_zero_trap(&mut self) {
        self.flags_set_hi(Vm::MASK_INVALID_INSTRUCTION | Vm::MASK_ZERO_EQUAL);
        self.flags_set_lo(Vm::MASK_RESERVED_INSTRUCTION);
    }

    fn p0_reserved_instruction(&mut self) {
        self.flags_set_hi(Vm::MASK_INVALID_INSTRUCTION | Vm::MASK_RESERVED_INSTRUCTION);
    }

    fn get_opcode_nibbles(&self) -> (usize, usize, usize, usize) {
        (
            ((self.opcode_reg & 0xF000_u16) >> 12) as usize,
            ((self.opcode_reg & 0x0F00_u16) >> 8) as usize,
            ((self.opcode_reg & 0x00F0_u16) >> 4) as usize,
            (self.opcode_reg & 0x000F_u16) as usize,
        )
    }

    fn p0_addr(&mut self) {
        let (_, x, y, z) = self.get_opcode_nibbles();

        let res = self.registers[y] as u32 + self.registers[z] as u32;
        if res > 0xFFFF_u32 {
            self.flags_set_hi(Vm::MASK_OVERFLOW);
        } else if res == 0 {
            self.flags_set_hi(Vm::MASK_ZERO_EQUAL);
        } else {
            self.flags_set_lo(Vm::MASK_OVERFLOW);
        }

        self.registers[x] = res as u16;
        self.flags_set_lo(Vm::MASK_INVALID_INSTRUCTION | Vm::MASK_RESERVED_INSTRUCTION);
    }

    fn p0_subr(&mut self) {
        let (_, x, y, z) = self.get_opcode_nibbles();

        let res = self.registers[y] as i32 - self.registers[z] as i32;
        if res < i16::MIN as i32 {
            self.flags_set_hi(Vm::MASK_OVERFLOW);
        } else if res == 0 {
            self.flags_set_hi(Vm::MASK_ZERO_EQUAL);
        } else {
            self.flags_set_lo(Vm::MASK_UNDERFLOW);
        }

        self.registers[x] = res as u16;
        self.flags_set_lo(Vm::MASK_INVALID_INSTRUCTION | Vm::MASK_RESERVED_INSTRUCTION);
    }

    fn p0_mulr(&mut self) {
        let (_, x, y, z) = self.get_opcode_nibbles();

        let res = self.registers[x] as i32 * self.registers[y] as i32;
        if res == 0 {
            self.flags_set_hi(Vm::MASK_ZERO_EQUAL);
        }

        self.registers[z] = (res & 0x0000_FFFF_u32 as i32) as u16;
        self.registers[y] = ((res & 0xFFFF_0000_u32 as i32) >> 16) as u16;
        self.flags_set_lo(Vm::MASK_INVALID_INSTRUCTION | Vm::MASK_RESERVED_INSTRUCTION);
    }

    fn p0_divr(&mut self) {
        let (_, x, y, z) = self.get_opcode_nibbles();

        if self.registers[z] == 0 {
            self.flags_set_hi(Vm::MASK_ZERO_DIV);
            self.registers[x] = i16::MAX as u16;
        } else {
            let result = self.registers[y] as i16 / self.registers[z] as i16;
            self.registers[x] = result as u16;
        }

        self.flags_set_hi(Vm::MASK_INVALID_INSTRUCTION | Vm::MASK_RESERVED_INSTRUCTION);
    }

    fn p0_um2pr(&mut self) {
        let (_, x, y, z) = self.get_opcode_nibbles();

        let mag = self.registers[z] & 0x7FFF_u16;
        self.registers[x] = if self.registers[z] as i16 > 0 {
            let res = self.registers[y] as u32 * (1_u32 << mag);

            if res > u16::MAX as u32 {
                self.flags_set_hi(Vm::MASK_OVERFLOW);
            } else {
                self.flags_set_lo(Vm::MASK_OVERFLOW);
            }

            res as u16
        } else if (self.registers[z] as i16) < 0_i16 {
            let res = self.registers[y] as u32 / (1_u32 << mag);
            self.flags_set_lo(Vm::MASK_OVERFLOW);
            res as u16
        } else {
            self.flags_set_lo(Vm::MASK_OVERFLOW);
            self.registers[x]
        };

        self.flags_set_lo(Vm::MASK_INVALID_INSTRUCTION | Vm::MASK_RESERVED_INSTRUCTION);
    }

    fn p0_sm2pr(&mut self) {
        let (_, x, y, z) = self.get_opcode_nibbles();

        let mag = self.registers[z] & 0x7FFF_u16;
        self.registers[x] = if self.registers[z] as i16 > 0 {
            let res = self.registers[y] as i32 * (1_u32 << mag) as i32;

            if res > i16::MAX as i32 {
                self.flags_set_hi(Vm::MASK_OVERFLOW);
            } else {
                self.flags_set_lo(Vm::MASK_OVERFLOW);
            }

            res as u16
        } else if (self.registers[z] as i16) < 0_i16 {
            let res = self.registers[y] as i32 / (1_u32 << mag) as i32;
            self.flags_set_lo(Vm::MASK_OVERFLOW);
            res as u16
        } else {
            self.flags_set_lo(Vm::MASK_OVERFLOW);
            self.registers[x]
        };
        self.flags_set_lo(Vm::MASK_INVALID_INSTRUCTION | Vm::MASK_RESERVED_INSTRUCTION);
    }

    fn p0_cmov(&mut self) {
        let (_, x, y, z) = self.get_opcode_nibbles();

        if 1 == self.registers[0xF] << x {
            self.registers[y] = self.registers[z];
        }

        self.flags_set_lo(Vm::MASK_INVALID_INSTRUCTION | Vm::MASK_RESERVED_INSTRUCTION);
    }

    // -- Instruction page 0 instructions --------------------------------------

    // -- Instruction page 1 instructions --------------------------------------

    fn p1_ldr(&mut self, memory: &Vec<u8>) {
        let (_, _, x, y) = self.get_opcode_nibbles();

        let nib_hi = *memory.get(self.registers[y] as usize).unwrap_or(&0_u8);
        let nib_lo = *memory
            .get(self.registers[y].wrapping_add(1) as usize)
            .unwrap_or(&0_u8);
        if self.registers[y] == 0xFFFF {
            self.flags_set_hi(Vm::MASK_OVERFLOW);
            self.registers[x] = (nib_hi as u16) << 8u16 | nib_lo as u16;
        } else {
            self.flags_set_lo(Vm::MASK_OVERFLOW);
            self.registers[x] = (nib_hi as u16) << 8u16 | nib_lo as u16;
        }

        self.flags_set_lo(Vm::MASK_INVALID_INSTRUCTION | Vm::MASK_RESERVED_INSTRUCTION);
    }

    // TODO check whether
    // the thing loaded == thing read == thing reloaded == the thing reread
    fn p1_stor(&mut self, memory: &mut Vec<u8>) {
        let (_, _, x, y) = self.get_opcode_nibbles();

        let nib_hi = ((self.registers[x] & 0xFF00) >> 8) as u8;
        let nib_lo = (self.registers[x] & 0x00FF) as u8;

        if self.registers[y] == 0xFFFF {
            self.flags_set_hi(Vm::MASK_OVERFLOW);
        } else {
            self.flags_set_lo(Vm::MASK_OVERFLOW);
        }

        match memory.get_mut(self.registers[y] as usize) {
            Some(r) => *r = nib_hi,
            None => {}
        }
        match memory.get_mut(self.registers[y].wrapping_add(1) as usize) {
            Some(r) => *r = nib_lo,
            None => {}
        }
    }

    fn p1_movr(&mut self) {
        let (_, _, x, y) = self.get_opcode_nibbles();
        self.registers[x] = self.registers[y];
        self.flags_set_lo(Vm::MASK_INVALID_INSTRUCTION | Vm::MASK_RESERVED_INSTRUCTION);
    }

    fn p1_cmpr(&mut self) {
        let (_, _, x, y) = self.get_opcode_nibbles();
        self.flags_set_lo(
            Vm::MASK_INVALID_INSTRUCTION
                | Vm::MASK_RESERVED_INSTRUCTION
                | Vm::MASK_GREATER_THAN
                | Vm::MASK_LESS_THAN
                | Vm::MASK_ZERO_EQUAL,
        );

        if self.registers[x] > self.registers[y] {
            self.flags_set_hi(Vm::MASK_GREATER_THAN);
        } else if self.registers[x] < self.registers[y] {
            self.flags_set_hi(Vm::MASK_LESS_THAN);
        } else {
            self.flags_set_hi(Vm::MASK_ZERO_EQUAL);
        }
    }

    fn p1_lshl(&mut self) {
        let (_, _, x, y) = self.get_opcode_nibbles();
        self.registers[x] <<= y;
        self.flags_set_lo(Vm::MASK_INVALID_INSTRUCTION | Vm::MASK_RESERVED_INSTRUCTION);
    }

    fn p1_lshr(&mut self) {
        let (_, _, x, y) = self.get_opcode_nibbles();
        self.registers[x] >>= y;
        self.flags_set_lo(Vm::MASK_INVALID_INSTRUCTION | Vm::MASK_RESERVED_INSTRUCTION);
    }

    // TODO do these overflow?
    fn p1_ashl(&mut self) {
        let (_, _, x, y) = self.get_opcode_nibbles();
        self.registers[x] = ((self.registers[x] as i16) << y) as u16;
        self.flags_set_lo(Vm::MASK_INVALID_INSTRUCTION | Vm::MASK_RESERVED_INSTRUCTION);
    }
    fn p1_ashr(&mut self) {
        let (_, _, x, y) = self.get_opcode_nibbles();
        self.registers[x] = ((self.registers[x] as i16) >> y) as u16;
        self.flags_set_lo(Vm::MASK_INVALID_INSTRUCTION | Vm::MASK_RESERVED_INSTRUCTION);
    }

    fn p1_andr(&mut self) {
        let (_, _, x, y) = self.get_opcode_nibbles();
        self.registers[x] &= self.registers[y];

        if 0 == self.registers[x] {
            self.flags_set_hi(Vm::MASK_ZERO_EQUAL);
        } else {
            self.flags_set_lo(Vm::MASK_ZERO_EQUAL);
        }

        self.flags_set_lo(Vm::MASK_INVALID_INSTRUCTION | Vm::MASK_RESERVED_INSTRUCTION);
    }
    fn p1_orr(&mut self) {
        let (_, _, x, y) = self.get_opcode_nibbles();
        self.registers[x] |= self.registers[y];
        self.flags_set_lo(Vm::MASK_INVALID_INSTRUCTION | Vm::MASK_RESERVED_INSTRUCTION);
    }
    fn p1_xorr(&mut self) {
        let (_, _, x, y) = self.get_opcode_nibbles();
        self.registers[x] ^= self.registers[y];
        
        if 0 == self.registers[x] {
            self.flags_set_hi(Vm::MASK_ZERO_EQUAL);
        } else {
            self.flags_set_lo(Vm::MASK_ZERO_EQUAL);
        }

        self.flags_set_lo(Vm::MASK_INVALID_INSTRUCTION | Vm::MASK_RESERVED_INSTRUCTION);
    }

    fn p1_chkbit(&mut self) {
        let (_, _, x, y) = self.get_opcode_nibbles();
        if 1 == ((self.registers[y] >> x) & 0x1) {
            self.flags_set_hi(Vm::MASK_ZERO_EQUAL);
        } else {
            self.flags_set_lo(Vm::MASK_ZERO_EQUAL);
        }
        self.flags_set_lo(Vm::MASK_INVALID_INSTRUCTION | Vm::MASK_RESERVED_INSTRUCTION);
    }

    fn p1_setbit(&mut self) {
        let (_, _, x, y) = self.get_opcode_nibbles();
        self.registers[y] |= 1 << x;
        self.flags_set_lo(Vm::MASK_INVALID_INSTRUCTION | Vm::MASK_RESERVED_INSTRUCTION);
    }

    fn p1_reserved_instruction(&mut self) {
        self.flags_set_lo(Vm::MASK_INVALID_INSTRUCTION | Vm::MASK_RESERVED_INSTRUCTION);
    }

    // -- Instruction page 1 instructions --------------------------------------

    // -- Instruction page 2 instructions --------------------------------------
    // -- Instruction page 2 instructions --------------------------------------

    // -- Instruction page 3 instructions --------------------------------------
    // -- Instruction page 3 instructions --------------------------------------

    fn execute_instruction_and_flag_behaviour(&mut self, memory: &mut Vec<u8>) {
        if self.opcode_reg == 0x0000_u16 {
            self.p0_zero_trap();
            return;
        }

        let page0 = self.opcode_reg & 0x1000_u16;
        if page0 == 0x1000_u16 {
            self.p0_addr();
        } else if page0 == 0x2000_u16 {
            self.p0_subr();
        } else if page0 == 0x3000_u16 {
            self.p0_mulr();
        } else if page0 == 0x4000_u16 {
            self.p0_divr();
        } else if page0 == 0x5000_u16 {
            self.p0_um2pr();
        } else if page0 == 0x6000_u16 {
            self.p0_sm2pr();
        } else if page0 == 0x7000_u16 {
            self.p0_cmov();
        } else {
            self.p0_reserved_instruction();
        }

        let page1 = self.opcode_reg & 0x0100_u16;
        if page1 == 0x0000_u16 {
            self.p1_ldr(memory);
        } else if page1 == 0x0100_u16 {
            self.p1_stor(memory);
        } else if page1 == 0x0200_u16 {
            self.p1_movr();
        } else if page1 == 0x0300_u16 {
            self.p1_cmpr();
        } else if page1 == 0x0400_u16 {
            self.p1_lshl();
        } else if page1 == 0x0500_u16 {
            self.p1_lshr();
        } else if page1 == 0x0600_u16 {
            self.p1_ashl();
        } else if page1 == 0x0700_u16 {
            self.p1_ashr();
        } else if page1 == 0x0800_u16 {
            self.p1_andr();
        } else if page1 == 0x0900_u16 {
            self.p1_orr();
        } else if page1 == 0x0A00_u16 {
            self.p1_xorr();
        } else if page1 == 0x0B00_u16 {
            self.p1_chkbit();
        } else if page1 == 0x0C00_u16 {
            self.p1_setbit();
        } else {
            self.p1_reserved_instruction();
        }

        let page2 = self.opcode_reg & 0x0010_u16;
        if page2 == 0x0000_u16 {
            // ldc
        } else if page2 == 0x0010_u16 {
            // stoc
        } else if page2 == 0x0020_u16 {
            // movc
        } else if page2 == 0x0030_u16 {
            // cmpc
        } else if page2 == 0x0040_u16 {
            // andc
        } else if page2 == 0x0050_u16 {
            // orc
        } else if page2 == 0x0060_u16 {
            // xorc
        } else if page2 == 0x0070_u16 {
            // notr
        } else {
            // reserved
        }

        let page3 = self.opcode_reg & 0x0001_u16;
        if page3 == 0x0000_u16 {
            // nop
        } else if page3 == 0x0001_u16 {
            // dumpregs
        } else if page3 == 0x0002_u16 {
            // dumpversion
        } else if page3 == 0x000F_u16 {
            // reserved_future
        } else {
            // reserved
        }
    }
}