use std::ops::BitOrAssign;

// Registers
struct Vm {
    registers: [u16; 0x10],
    version: [u8; 8],
    opcode_reg: u16,
    operand_reg: u16,
}

impl Vm {
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
            version: [0x47, 0x10, 0x00, 0x01, 0x00, 0x00, 0x00, 0x01],
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

    fn execute_instruction_and_flag_behaviour(&mut self, memory: &mut Vec<u8>) {
        if self.opcode_reg == 0x0000_u16 {
            // zero trap
        } 
        
        let page0 = self.opcode_reg & 0x1000_u16;
        if page0 == 0x1000_u16 {
            // addr
        } else if page0 == 0x2000_u16 {
            // subr
        } else if page0 == 0x3000_u16 {
            // mulr
        } else if page0 == 0x4000_u16 {
            // divr
        } else if page0 == 0x5000_u16 {
            // um2pr
        } else if page0 == 0x6000_u16 {
            // sm2pr
        } else if page0 == 0x7000_u16 {
            // cmov
        } else {
            // reserved
        }


    }
}
