// -- Registers ---------------------------------------------------------------

#[rustfmt::skip]
#[derive(PartialEq)]
enum GeneralPurposeRegister {
    R0, R1, R2, R3, R4, R5, R6, R7,
    R8, R9, RA, RB, RC, RD, RE, RF,
}

impl GeneralPurposeRegister {
    fn from_u4(x: u8) -> GeneralPurposeRegister {
        match x & 0x0F {
            0x0 => GeneralPurposeRegister::R0,
            0x1 => GeneralPurposeRegister::R1,
            0x2 => GeneralPurposeRegister::R2,
            0x3 => GeneralPurposeRegister::R3,
            0x4 => GeneralPurposeRegister::R4,
            0x5 => GeneralPurposeRegister::R5,
            0x6 => GeneralPurposeRegister::R6,
            0x7 => GeneralPurposeRegister::R7,
            0x8 => GeneralPurposeRegister::R8,
            0x9 => GeneralPurposeRegister::R9,
            0xA => GeneralPurposeRegister::RA,
            0xB => GeneralPurposeRegister::RB,
            0xC => GeneralPurposeRegister::RC,
            0xD => GeneralPurposeRegister::RD,
            0xE => GeneralPurposeRegister::RE,
            0xF => GeneralPurposeRegister::RF,
            _ => unreachable!(),
        }
    }
}

struct FlagsRegister {
    equal: bool,
    greater_than: bool,
    less_than: bool,
    reserved_instruction: bool,
    invalid_instruction: bool,
    zero_div: bool,
    overflow: bool,
    underflow: bool,
}

impl FlagsRegister {
    fn from_u32(x: u32) -> FlagsRegister {
        FlagsRegister {
            equal: ((x >> 0) & 0x1) == 0x1,
            greater_than: ((x >> 1) & 0x1) == 0x1,
            less_than: ((x >> 2) & 0x1) == 0x1,
            reserved_instruction: ((x >> 3) & 0x1) == 0x1,
            invalid_instruction: ((x >> 4) & 0x1) == 0x1,
            zero_div: ((x >> 5) & 0x1) == 0x1,
            overflow: ((x >> 6) & 0x1) == 0x1,
            underflow: ((x >> 7) & 0x1) == 0x1,
        }
    }

    fn set_from_current_instr(this: &mut FlagsRegister, instr: &VmInstruction) {
        todo!();
    }
}

#[rustfmt::skip]
#[allow(non_snake_case)]
struct VmRegisters {
    R0: u32, R1: u32, R2: u32, R3: u32, R4: u32, R5: u32, R6: u32, R7: u32,
    R8: u32, R9: u32, RA: u32, RB: u32, RC: u32, RD: u32, RE: u32, RF: u32,
    RIP: u32, FLAGS: FlagsRegister,
}

impl<'a, 'b> VmRegisters {
    fn get_mut_gpr(
        registers: &'a mut VmRegisters,
        reg_enum: &'b GeneralPurposeRegister,
    ) -> &'a mut u32 {
        match reg_enum {
            GeneralPurposeRegister::R0 => &mut registers.R0,
            GeneralPurposeRegister::R1 => &mut registers.R1,
            GeneralPurposeRegister::R2 => &mut registers.R2,
            GeneralPurposeRegister::R3 => &mut registers.R3,
            GeneralPurposeRegister::R4 => &mut registers.R4,
            GeneralPurposeRegister::R5 => &mut registers.R5,
            GeneralPurposeRegister::R6 => &mut registers.R6,
            GeneralPurposeRegister::R7 => &mut registers.R7,
            GeneralPurposeRegister::R8 => &mut registers.R8,
            GeneralPurposeRegister::R9 => &mut registers.R9,
            GeneralPurposeRegister::RA => &mut registers.RA,
            GeneralPurposeRegister::RB => &mut registers.RB,
            GeneralPurposeRegister::RC => &mut registers.RC,
            GeneralPurposeRegister::RD => &mut registers.RD,
            GeneralPurposeRegister::RE => &mut registers.RE,
            GeneralPurposeRegister::RF => &mut registers.RF,
        }
    }
}

impl<'a, 'b> VmRegisters {
    fn get_gpr(registers: &'a mut VmRegisters, reg_enum: &'b GeneralPurposeRegister) -> &'a u32 {
        match reg_enum {
            GeneralPurposeRegister::R0 => &registers.R0,
            GeneralPurposeRegister::R1 => &registers.R1,
            GeneralPurposeRegister::R2 => &registers.R2,
            GeneralPurposeRegister::R3 => &registers.R3,
            GeneralPurposeRegister::R4 => &registers.R4,
            GeneralPurposeRegister::R5 => &registers.R5,
            GeneralPurposeRegister::R6 => &registers.R6,
            GeneralPurposeRegister::R7 => &registers.R7,
            GeneralPurposeRegister::R8 => &registers.R8,
            GeneralPurposeRegister::R9 => &registers.R9,
            GeneralPurposeRegister::RA => &registers.RA,
            GeneralPurposeRegister::RB => &registers.RB,
            GeneralPurposeRegister::RC => &registers.RC,
            GeneralPurposeRegister::RD => &registers.RD,
            GeneralPurposeRegister::RE => &registers.RE,
            GeneralPurposeRegister::RF => &registers.RF,
        }
    }
}

// -- Registers ---------------------------------------------------------------

// -- Three reg opcodes -------------------------------------------------------

#[allow(non_camel_case_types)]
#[rustfmt::skip]
#[derive(PartialEq)]
enum ThreeRegOpcode {
    INVALID_ZERO_TRAP,
    ADDR, SUBR, MULR, DIVR,
    FIXMULR, FIXDIVR,
    INVALID_RESERVED, INVALID_NEXT_INSTR_PAGE,
}

impl ThreeRegOpcode {
    fn from_u4(x: u8) -> ThreeRegOpcode {
        match x & 0x0F {
            0x0 => ThreeRegOpcode::INVALID_ZERO_TRAP,
            0x1 => ThreeRegOpcode::ADDR,
            0x2 => ThreeRegOpcode::SUBR,
            0x3 => ThreeRegOpcode::MULR,
            0x4 => ThreeRegOpcode::DIVR,
            0x5 => ThreeRegOpcode::FIXMULR,
            0x6 => ThreeRegOpcode::FIXDIVR,
            0x7..=0xE => ThreeRegOpcode::INVALID_RESERVED,
            0xF => ThreeRegOpcode::INVALID_NEXT_INSTR_PAGE,
            _ => unreachable!(),
        }
    }
}

pub struct ThreeRegInstr {
    code: ThreeRegOpcode,
    reg1: GeneralPurposeRegister,
    reg2: GeneralPurposeRegister,
    reg3: GeneralPurposeRegister,
}

impl ThreeRegInstr {
    fn from_u32(x: u32) -> ThreeRegInstr {
        let code = ((x >> 28) & 0x0f) as u8;
        let reg1 = ((x >> 24) & 0x0f) as u8;
        let reg2 = ((x >> 20) & 0x0f) as u8;
        let reg3 = ((x >> 16) & 0x0f) as u8;

        let code = ThreeRegOpcode::from_u4(code);
        let reg1 = GeneralPurposeRegister::from_u4(reg1);
        let reg2 = GeneralPurposeRegister::from_u4(reg2);
        let reg3 = GeneralPurposeRegister::from_u4(reg3);

        ThreeRegInstr {
            code,
            reg1,
            reg2,
            reg3,
        }
    }

    fn execute_single_instruction(instr: &ThreeRegInstr, registers: &mut VmRegisters) {
        match instr.code {
            ThreeRegOpcode::INVALID_ZERO_TRAP => {
                unreachable!("This is executed, handle this!");
            }
            ThreeRegOpcode::ADDR => {
                let arg1 = *VmRegisters::get_gpr(registers, &instr.reg2);
                let arg2 = *VmRegisters::get_gpr(registers, &instr.reg3);
                let dest = VmRegisters::get_mut_gpr(registers, &instr.reg1);

                *dest = arg1.wrapping_add(arg2);
            }
            ThreeRegOpcode::SUBR => {
                let arg1 = *VmRegisters::get_gpr(registers, &instr.reg2);
                let arg2 = *VmRegisters::get_gpr(registers, &instr.reg3);
                let dest = VmRegisters::get_mut_gpr(registers, &instr.reg1);

                *dest = arg1.wrapping_sub(arg2);
            }
            ThreeRegOpcode::MULR => {
                let arg1 = *VmRegisters::get_gpr(registers, &instr.reg1);
                let arg2 = *VmRegisters::get_gpr(registers, &instr.reg2);

                let arg1 = arg1 as u64;
                let arg2 = arg2 as u64;

                let intermediate = arg1
                    .checked_mul(arg2)
                    .expect("Overflow of u32 scaled to u64: unreachable condition");
                let dest_lo = (intermediate & 0x0000_0000_ffff_ffff) as u32;
                let dest_hi = ((intermediate & 0xffff_ffff_0000_0000) >> 32) as u32;

                let dest_hi_ref = VmRegisters::get_mut_gpr(registers, &instr.reg2);
                *dest_hi_ref = dest_hi;

                let dest_lo_ref = VmRegisters::get_mut_gpr(registers, &instr.reg3);
                *dest_lo_ref = dest_lo;
            }
            ThreeRegOpcode::DIVR => {
                let arg1 = *VmRegisters::get_gpr(registers, &instr.reg2);
                let arg2 = *VmRegisters::get_gpr(registers, &instr.reg3);
                let result = if arg2 == 0 {
                    0xffff_ffff_u32
                } else {
                    arg1 / arg2
                };

                *VmRegisters::get_mut_gpr(registers, &instr.reg1) = result;
            }
            ThreeRegOpcode::FIXMULR => {
                let arg1 = *VmRegisters::get_gpr(registers, &instr.reg1);
                let arg2 = *VmRegisters::get_gpr(registers, &instr.reg2);
                let intermediate = arg1 as u64 * arg2 as u64;
                let result = intermediate / u16::MAX as u64;
                *VmRegisters::get_mut_gpr(registers, &instr.reg1) = result as u32;
            }
            ThreeRegOpcode::FIXDIVR => {
                let arg1 = *VmRegisters::get_gpr(registers, &instr.reg1);
                let arg2 = *VmRegisters::get_gpr(registers, &instr.reg2);
                let scaled_numerator = arg1 as u64 * u16::MAX as u64;
                let quotient = scaled_numerator / arg2 as u64;
                let result = quotient as u32;
                registers.FLAGS.reserved_instruction = true;
                *VmRegisters::get_mut_gpr(registers, &instr.reg1) = result;
            }
            ThreeRegOpcode::INVALID_RESERVED => {
                todo!("This is executed, handle this!");
            }
            ThreeRegOpcode::INVALID_NEXT_INSTR_PAGE => {
                unreachable!("This is executed, handle this!");
            }
        }
    }
}

// -- Three reg opcodes -------------------------------------------------------

// -- Two reg opcodes ---------------------------------------------------------

#[allow(non_camel_case_types)]
#[rustfmt::skip]
#[derive(PartialEq)]
enum TwoRegOpcode {
    WRITER, READR, MOVR, CMPR,
    LSHIFTR, ASHIFTR, ROLLR,
    ANDR, ORR, XORR,
    INVALID_RESERVED, INVALID_NEXT_INSTR_PAGE,
}

impl TwoRegOpcode {
    fn from_u4(x: u8) -> TwoRegOpcode {
        match x & 0x0F {
            0x0 => TwoRegOpcode::READR,
            0x1 => TwoRegOpcode::WRITER,
            0x2 => TwoRegOpcode::MOVR,
            0x3 => TwoRegOpcode::CMPR,
            0x4 => TwoRegOpcode::LSHIFTR,
            0x5 => TwoRegOpcode::ASHIFTR,
            0x6 => TwoRegOpcode::ROLLR,
            0x7 => TwoRegOpcode::ANDR,
            0x8 => TwoRegOpcode::ORR,
            0x9 => TwoRegOpcode::XORR,
            0xA..=0xE => TwoRegOpcode::INVALID_RESERVED,
            0xF => TwoRegOpcode::INVALID_NEXT_INSTR_PAGE,
            _ => unreachable!(),
        }
    }
}

pub struct TwoRegInstr {
    code: TwoRegOpcode,
    reg1: GeneralPurposeRegister,
    reg2: GeneralPurposeRegister,
}

impl TwoRegInstr {
    fn from_u32(x: u32) -> TwoRegInstr {
        let code = ((x >> 24) & 0x0f) as u8;
        let reg1 = ((x >> 20) & 0x0f) as u8;
        let reg2 = ((x >> 16) & 0x0f) as u8;

        let code = TwoRegOpcode::from_u4(code);
        let reg1 = GeneralPurposeRegister::from_u4(reg1);
        let reg2 = GeneralPurposeRegister::from_u4(reg2);

        TwoRegInstr { code, reg1, reg2 }
    }

    fn execute_single_instruction(
        instr: &TwoRegInstr,
        registers: &mut VmRegisters,
        memory: &mut Vec<u32>,
    ) {
        match instr.code {
            TwoRegOpcode::READR => {
                // Check whether unaligned and get other info
                let reg_y = *VmRegisters::get_gpr(registers, &instr.reg2);
                let alignment = reg_y % 0x4;
                let is_aligned = 0 == alignment;
                if is_aligned {
                    let word_address = reg_y as usize / 4;
                    let word = memory
                        .get(word_address)
                        .expect("Not enough memory, handle this case by... doing nothing maybe??");
                    *VmRegisters::get_mut_gpr(registers, &instr.reg1) = *word;
                } else {
                    let word_addr_1 = reg_y as usize / 4;
                    let word_addr_2 = word_addr_1 + 1;
                    let word_1 = *memory
                        .get(word_addr_1)
                        .expect("Not enough memory, handle this case by... doing nothing maybe??");
                    let word_2 = *memory
                        .get(word_addr_2)
                        .expect("Not enough memory, handle this case by... doing nothing maybe??");
                    let shifted = (word_1 << alignment) | (word_2 >> (4 - alignment));
                    *VmRegisters::get_mut_gpr(registers, &instr.reg1) = shifted;
                }
            }
            TwoRegOpcode::WRITER => {
                // Check whether unaligned and get other info
                let reg_y = *VmRegisters::get_gpr(registers, &instr.reg2);
                let alignment = reg_y % 0x4;
                let is_aligned = 0 == alignment;
                if is_aligned {
                    let word_address = reg_y as usize / 4;
                    let word_ref = memory
                        .get_mut(word_address)
                        .expect("Not enough memory, handle this case by... doing nothing maybe??");
                    *word_ref = *VmRegisters::get_mut_gpr(registers, &instr.reg1);
                } else {
                    let word_addr_1 = reg_y as usize / 4;
                    let word_addr_2 = word_addr_1 + 1;
                    let to_write = *VmRegisters::get_gpr(registers, &instr.reg1);

                    let word_ref_1 = memory
                        .get_mut(word_addr_1)
                        .expect("Not enough memory, handle this case by... doing nothing maybe??");
                    let mask = u32::MAX << (4 - alignment);
                    *word_ref_1 = (*word_ref_1 & mask) | (to_write >> alignment);

                    let word_ref_2 = memory
                        .get_mut(word_addr_2)
                        .expect("Not enough memory, handle this case by... doing nothing maybe??");
                    let mask = u32::MAX >> alignment;
                    *word_ref_2 = (*word_ref_2 & mask) | (to_write << (4 - alignment));
                }
            }
            TwoRegOpcode::MOVR => {
                *VmRegisters::get_mut_gpr(registers, &instr.reg1) =
                    *VmRegisters::get_gpr(registers, &instr.reg2);
            }
            TwoRegOpcode::CMPR => { /* do nothing, flags handler will take care of this */ }
            TwoRegOpcode::LSHIFTR => {
                enum Direction { Left, Right };
                let reg_y = *VmRegisters::get_gpr(registers, &instr.reg2);
                let shift_dir = if (reg_y >> 31) & 0x1 == 1 {
                    Direction::Left
                } else {
                    Direction::Right
                };
                let shift_mag = reg_y & (u32::MAX >> 1);
                let reg_x = VmRegisters::get_mut_gpr(registers, &instr.reg1);

                match shift_dir {
                    Direction::Left => *reg_x = *reg_x << shift_mag,
                    Direction::Right => *reg_x = *reg_x >> shift_mag,
                }
            }
            TwoRegOpcode::ASHIFTR => {
                enum Direction { Left, Right }
                let reg_y = *VmRegisters::get_gpr(registers, &instr.reg2);
                let shift_dir = if (reg_y >> 31) & 0x1 == 1 {
                    Direction::Left
                } else {
                    Direction::Right
                };
                let shift_mag = reg_y & (u32::MAX >> 1);
                let reg_x = VmRegisters::get_mut_gpr(registers, &instr.reg1);

                match shift_dir {
                    Direction::Left => *reg_x = ((*reg_x as i32) << shift_mag) as u32,
                    Direction::Right => *reg_x = ((*reg_x as i32) >> shift_mag) as u32,
                }
            }
            TwoRegOpcode::ROLLR => {
                enum Direction { Left, Right }
                let reg_y = *VmRegisters::get_gpr(registers, &instr.reg2);
                let shift_dir = if (reg_y >> 31) & 0x1 == 1 {
                    Direction::Left
                } else {
                    Direction::Right
                };
                let shift_mag = reg_y & (u32::MAX >> 1);
                let reg_x = VmRegisters::get_mut_gpr(registers, &instr.reg1);
                
                fn rotate_once(what: u32, direction: &Direction) -> u32 {
                    match direction {
                        Direction::Left => {
                            // 1010
                            // 010 + 1
                            let left_most_bit = (what & 0x7FFF_FFFF) >> 31;
                            left_most_bit | (what << 1)
                        },
                        Direction::Right => {
                            let right_most_bit = what & 0x1;
                            (what >> 1) | right_most_bit
                        }
                    }
                }

                for rot_cnt in 0..(shift_mag % 4) {
                    *reg_x = rotate_once(*reg_x, &shift_dir);
                }
            }
            TwoRegOpcode::ANDR => {
                
                
            }
            TwoRegOpcode::ORR => {
                todo!();
            }
            TwoRegOpcode::XORR => {
                todo!();
            }
            TwoRegOpcode::INVALID_RESERVED => {
                todo!();
            }
            TwoRegOpcode::INVALID_NEXT_INSTR_PAGE => {
                todo!();
            }
        }
    }
}

// -- Two reg opcodes ---------------------------------------------------------

// -- One reg opcodes ---------------------------------------------------------

#[allow(non_camel_case_types)]
#[rustfmt::skip]
#[derive(PartialEq)]
enum OneRegOpcode {
    READC, WRITEC, MOVC, JMPR, CMPC,
    LSHIFTC, ASHIFTC, ROLLC,
    ANDC, ORC, NOTR, XORC,
    INVALID_RESERVED, INVALID_NEXT_INSTR_PAGE,
}

impl OneRegOpcode {
    fn from_u4(x: u8) -> OneRegOpcode {
        match x & 0xF {
            0x0 => OneRegOpcode::READC,
            0x1 => OneRegOpcode::WRITEC,
            0x2 => OneRegOpcode::MOVC,
            0x3 => OneRegOpcode::JMPR,
            0x4 => OneRegOpcode::CMPC,
            0x5 => OneRegOpcode::LSHIFTC,
            0x6 => OneRegOpcode::ASHIFTC,
            0x7 => OneRegOpcode::ROLLC,
            0x8 => OneRegOpcode::ANDC,
            0x9 => OneRegOpcode::ORC,
            0xA => OneRegOpcode::NOTR,
            0xB => OneRegOpcode::XORC,
            0xC..=0xE => OneRegOpcode::INVALID_RESERVED,
            0xF => OneRegOpcode::INVALID_NEXT_INSTR_PAGE,
            _ => unreachable!(),
        }
    }

    fn has_constant(code: &OneRegOpcode) -> bool {
        match code {
            OneRegOpcode::READC => true,
            OneRegOpcode::WRITEC => true,
            OneRegOpcode::MOVC => true,
            OneRegOpcode::CMPC => true,
            OneRegOpcode::LSHIFTC => true,
            OneRegOpcode::ASHIFTC => true,
            OneRegOpcode::ROLLC => true,
            OneRegOpcode::ANDC => true,
            OneRegOpcode::ORC => true,
            OneRegOpcode::XORC => true,
            OneRegOpcode::JMPR => false,
            OneRegOpcode::NOTR => false,
            OneRegOpcode::INVALID_RESERVED => false,
            OneRegOpcode::INVALID_NEXT_INSTR_PAGE => false,
        }
    }
}

pub struct OneRegInstr {
    code: OneRegOpcode,
    reg1: GeneralPurposeRegister,
    optional_const_exists: bool,
    optional_const: u32,
}

impl OneRegInstr {
    fn from_u32_and_const(x: u32, optional_const: u32) -> OneRegInstr {
        let code = ((x >> 20) & 0x0f) as u8;
        let reg1 = ((x >> 16) & 0x0f) as u8;

        let code = OneRegOpcode::from_u4(code);
        let reg1 = GeneralPurposeRegister::from_u4(reg1);
        let hasc = OneRegOpcode::has_constant(&code);

        OneRegInstr {
            code,
            reg1,
            optional_const_exists: hasc,
            optional_const,
        }
    }

    fn execute_single_instruction(instr: &OneRegInstr, registers: &mut VmRegisters) {
        todo!();
    }
}

// -- One reg opcodes ---------------------------------------------------------

// -- Zero reg opcodes --------------------------------------------------------

#[allow(non_camel_case_types)]
#[rustfmt::skip]
#[derive(PartialEq)]
enum ZeroRegOpcode {
    NOP,
    JMP, JE, JG, JL,
    INVALID_RESERVED, INVALID_NEXT_INSTR_PAGE,
}

impl ZeroRegOpcode {
    fn from_u4(x: u8) -> ZeroRegOpcode {
        match x & 0xF {
            0x0 => ZeroRegOpcode::NOP,
            0x1 => ZeroRegOpcode::JMP,
            0x2 => ZeroRegOpcode::JE,
            0x3 => ZeroRegOpcode::JG,
            0x4 => ZeroRegOpcode::JL,
            0x5..=0xE => ZeroRegOpcode::INVALID_RESERVED,
            0xF => ZeroRegOpcode::INVALID_NEXT_INSTR_PAGE,
            _ => unreachable!(),
        }
    }

    fn has_constant(code: &ZeroRegOpcode) -> bool {
        match code {
            ZeroRegOpcode::NOP => false,
            ZeroRegOpcode::JMP => true,
            ZeroRegOpcode::JE => true,
            ZeroRegOpcode::JG => true,
            ZeroRegOpcode::JL => true,
            ZeroRegOpcode::INVALID_RESERVED => false,
            ZeroRegOpcode::INVALID_NEXT_INSTR_PAGE => false,
        }
    }
}

pub struct ZeroRegInstr {
    code: ZeroRegOpcode,
    optional_const_exists: bool,
    optional_const: u32,
}

impl ZeroRegInstr {
    fn from_u32_and_const(x: u32, optional_const: u32) -> ZeroRegInstr {
        let code = ((x >> 16) & 0x0f) as u8;

        let code = ZeroRegOpcode::from_u4(code);
        let optional_const_exists = ZeroRegOpcode::has_constant(&code);

        ZeroRegInstr {
            code,
            optional_const_exists,
            optional_const,
        }
    }

    fn execute_single_instruction(instr: &ZeroRegInstr, registers: &mut VmRegisters) {
        todo!();
    }
}

// -- Zero reg opcodes --------------------------------------------------------

// -- Decoder -----------------------------------------------------------------

enum VmInstruction {
    ThreeRegInstr(ThreeRegInstr),
    TwoRegInstr(TwoRegInstr),
    OneRegInstr(OneRegInstr),
    ZeroRegInstr(ZeroRegInstr),
    ErrorInstructionFromFutureISA,
}

impl VmInstruction {
    fn decode(word0: u32, word1: u32) -> VmInstruction {
        let possibly_three = ThreeRegInstr::from_u32(word0);
        if possibly_three.code != ThreeRegOpcode::INVALID_NEXT_INSTR_PAGE {
            return VmInstruction::ThreeRegInstr(possibly_three);
        }

        let possibly_two = TwoRegInstr::from_u32(word0);
        if possibly_two.code != TwoRegOpcode::INVALID_NEXT_INSTR_PAGE {
            return VmInstruction::TwoRegInstr(possibly_two);
        }

        let possibly_one = OneRegInstr::from_u32_and_const(word0, word1);
        if possibly_one.code != OneRegOpcode::INVALID_NEXT_INSTR_PAGE {
            return VmInstruction::OneRegInstr(possibly_one);
        }

        let possibly_zero = ZeroRegInstr::from_u32_and_const(word0, word1);
        if possibly_zero.code != ZeroRegOpcode::INVALID_NEXT_INSTR_PAGE {
            return VmInstruction::ZeroRegInstr(possibly_zero);
        }

        VmInstruction::ErrorInstructionFromFutureISA
    }
}

// -- Decoder -----------------------------------------------------------------

// -- Executor ----------------------------------------------------------------

fn execute_single_instruction(
    registers: &mut VmRegisters,
    instruction: &VmInstruction,
    memory: &mut Vec<u32>,
) {
    match instruction {
        VmInstruction::ThreeRegInstr(instr) => {
            ThreeRegInstr::execute_single_instruction(instr, registers);
        }
        VmInstruction::TwoRegInstr(instr) => {
            TwoRegInstr::execute_single_instruction(instr, registers, memory);
        }
        VmInstruction::OneRegInstr(instr) => {
            OneRegInstr::execute_single_instruction(instr, registers);
        }
        VmInstruction::ZeroRegInstr(instr) => {
            ZeroRegInstr::execute_single_instruction(instr, registers);
        }
        VmInstruction::ErrorInstructionFromFutureISA => {
            // This will be handled by one of the FlagsRegister methods
            //todo!();
        }
    }

    FlagsRegister::set_from_current_instr(&mut registers.FLAGS, &instruction);

    // If opcode has constant, RIP += 2 else RIP += 3 (bytes)
}

// -- Executor ----------------------------------------------------------------
