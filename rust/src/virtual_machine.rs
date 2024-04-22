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
    INVALID_ZERO,
    ADDR, SUBR, MULR, DIVR,
    FIXMULR, FIXDIVR, CFIXSQRTR,
    INVALID_RESERVED, INVALID_NEXT_INSTR_PAGE,
}

impl ThreeRegOpcode {
    fn from_u4(x: u8) -> ThreeRegOpcode {
        match x & 0x0F {
            0x0 => ThreeRegOpcode::INVALID_ZERO,
            0x1 => ThreeRegOpcode::ADDR,
            0x2 => ThreeRegOpcode::SUBR,
            0x3 => ThreeRegOpcode::MULR,
            0x4 => ThreeRegOpcode::DIVR,
            0x5 => ThreeRegOpcode::FIXMULR,
            0x6 => ThreeRegOpcode::FIXDIVR,
            0x7 => ThreeRegOpcode::CFIXSQRTR,
            0x8..=0xE => ThreeRegOpcode::INVALID_RESERVED,
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
            ThreeRegOpcode::INVALID_ZERO => {
                registers.FLAGS.invalid_instruction = true;
            },
            ThreeRegOpcode::ADDR => {
                let arg1 = *VmRegisters::get_gpr(registers, &instr.reg2);
                let arg2 = *VmRegisters::get_gpr(registers, &instr.reg3);
                let dest = VmRegisters::get_mut_gpr(registers, &instr.reg1);

                *dest = arg1.wrapping_add(arg2);
            },
            ThreeRegOpcode::SUBR => {
                let arg1 = *VmRegisters::get_gpr(registers, &instr.reg2);
                let arg2 = *VmRegisters::get_gpr(registers, &instr.reg3);
                let dest = VmRegisters::get_mut_gpr(registers, &instr.reg1);

                *dest = arg1.wrapping_sub(arg2);
            },
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
            },
            ThreeRegOpcode::DIVR => {
                let arg1 = *VmRegisters::get_gpr(registers, &instr.reg2);
                let arg2 = *VmRegisters::get_gpr(registers, &instr.reg3);
                let result = if arg2 == 0 { 0xffff_ffff_u32 } else { arg1 / arg2 };

                *VmRegisters::get_mut_gpr(registers, &instr.reg1) = result;
            },
            ThreeRegOpcode::FIXMULR => {
                let arg1 = *VmRegisters::get_gpr(registers, &instr.reg1);
                let arg2 = *VmRegisters::get_gpr(registers, &instr.reg2);
                let intermediate = arg1 as u64 * arg2 as u64;
                let result = intermediate / u16::MAX as u64;
                *VmRegisters::get_mut_gpr(registers, &instr.reg1) = result as u32;
            },
            ThreeRegOpcode::FIXDIVR => {
                let arg1 = *VmRegisters::get_gpr(registers, &instr.reg1);
                let arg2 = *VmRegisters::get_gpr(registers, &instr.reg2);
                let scaled_numerator = arg1 as u64 * u16::MAX as u64;
                let quotient = scaled_numerator / arg2 as u64;
                let result = quotient as u32;
                *VmRegisters::get_mut_gpr(registers, &instr.reg1) = result;
            },
            ThreeRegOpcode::CFIXSQRTR => {
                todo!();
            },
            ThreeRegOpcode::INVALID_RESERVED => {
                registers.FLAGS.reserved_instruction = true;
            },
            ThreeRegOpcode::INVALID_NEXT_INSTR_PAGE => {
                unreachable!();
            },
        }
    }
}

// -- Three reg opcodes -------------------------------------------------------

// -- Two reg opcodes ---------------------------------------------------------

#[allow(non_camel_case_types)]
#[rustfmt::skip]
#[derive(PartialEq)]
enum TwoRegOpcode {
    INVALID_ZERO, WRITER, READR, MOVR, CMPR,
    LSHIFTR, ASHIFTR, ROLLR,
    ANDR, ORR, XORR,
    INVALID_RESERVED, INVALID_NEXT_INSTR_PAGE,
}

impl TwoRegOpcode {
    fn from_u4(x: u8) -> TwoRegOpcode {
        match x & 0x0F {
            0x0 => TwoRegOpcode::INVALID_ZERO,
            0x1 => TwoRegOpcode::WRITER,
            0x2 => TwoRegOpcode::READR,
            0x3 => TwoRegOpcode::MOVR,
            0x4 => TwoRegOpcode::CMPR,
            0x5 => TwoRegOpcode::LSHIFTR,
            0x6 => TwoRegOpcode::ASHIFTR,
            0x7 => TwoRegOpcode::ROLLR,
            0x8 => TwoRegOpcode::ANDR,
            0x9 => TwoRegOpcode::ORR,
            0xA => TwoRegOpcode::XORR,
            0xB..=0xE => TwoRegOpcode::INVALID_RESERVED,
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
}

// -- Two reg opcodes ---------------------------------------------------------

// -- One reg opcodes ---------------------------------------------------------

#[allow(non_camel_case_types)]
#[rustfmt::skip]
#[derive(PartialEq)]
enum OneRegOpcode {
    INVALID_ZERO,
    READC, WRITEC, MOVC, JMPR, CMPC,
    LSHIFTC, ASHIFTC, ROLLC,
    ANDC, ORC, NOTR, XORC,
    INVALID_RESERVED, INVALID_NEXT_INSTR_PAGE,
}

impl OneRegOpcode {
    fn from_u4(x: u8) -> OneRegOpcode {
        match x & 0xF {
            0x0 => OneRegOpcode::INVALID_ZERO,
            0x1 => OneRegOpcode::READC,
            0x2 => OneRegOpcode::WRITEC,
            0x3 => OneRegOpcode::MOVC,
            0x4 => OneRegOpcode::JMPR,
            0x5 => OneRegOpcode::CMPC,
            0x6 => OneRegOpcode::LSHIFTC,
            0x7 => OneRegOpcode::ASHIFTC,
            0x8 => OneRegOpcode::ROLLC,
            0x9 => OneRegOpcode::ANDC,
            0xA => OneRegOpcode::ORC,
            0xB => OneRegOpcode::NOTR,
            0xC => OneRegOpcode::XORC,
            0xD..=0xE => OneRegOpcode::INVALID_RESERVED,
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
            OneRegOpcode::INVALID_ZERO => false,
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
}

// -- One reg opcodes ---------------------------------------------------------

// -- Zero reg opcodes --------------------------------------------------------

#[allow(non_camel_case_types)]
#[rustfmt::skip]
#[derive(PartialEq)]
enum ZeroRegOpcode {
    INVALID_ZERO,
    NOP,
    JMP, JE, JG, JL,
    INVALID_RESERVED, INVALID_NEXT_INSTR_PAGE,
}

impl ZeroRegOpcode {
    fn from_u4(x: u8) -> ZeroRegOpcode {
        match x & 0xF {
            0x0 => ZeroRegOpcode::INVALID_ZERO,
            0x1 => ZeroRegOpcode::NOP,
            0x2 => ZeroRegOpcode::JMP,
            0x3 => ZeroRegOpcode::JE,
            0x4 => ZeroRegOpcode::JG,
            0x5 => ZeroRegOpcode::JL,
            0x6..=0xE => ZeroRegOpcode::INVALID_RESERVED,
            0xF => ZeroRegOpcode::INVALID_NEXT_INSTR_PAGE,
            _ => unreachable!(),
        }
    }

    fn has_constant(code: &ZeroRegOpcode) -> bool {
        match code {
            ZeroRegOpcode::INVALID_ZERO => false,
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
        VmInstruction::ThreeRegInstr(instr) => todo!(),
        VmInstruction::TwoRegInstr(instr) => match instr.code {
            TwoRegOpcode::INVALID_ZERO => {
                todo!();
            }
            TwoRegOpcode::WRITER => {
                todo!();
            }
            TwoRegOpcode::READR => {
                todo!();
            }
            TwoRegOpcode::MOVR => {
                todo!();
            }
            TwoRegOpcode::CMPR => {
                todo!();
            }
            TwoRegOpcode::LSHIFTR => {
                todo!();
            }
            TwoRegOpcode::ASHIFTR => {
                todo!();
            }
            TwoRegOpcode::ROLLR => {
                todo!();
            }
            TwoRegOpcode::ANDR => {
                todo!();
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
                unreachable!();
            }
        },
        VmInstruction::OneRegInstr(instr) => match instr.code {
            OneRegOpcode::INVALID_ZERO => {
                todo!();
            }
            OneRegOpcode::READC => {
                todo!();
            }
            OneRegOpcode::WRITEC => {
                todo!();
            }
            OneRegOpcode::MOVC => {
                todo!();
            }
            OneRegOpcode::JMPR => {
                todo!();
            }
            OneRegOpcode::CMPC => {
                todo!();
            }
            OneRegOpcode::LSHIFTC => {
                todo!();
            }
            OneRegOpcode::ASHIFTC => {
                todo!();
            }
            OneRegOpcode::ROLLC => {
                todo!();
            }
            OneRegOpcode::ANDC => {
                todo!();
            }
            OneRegOpcode::ORC => {
                todo!();
            }
            OneRegOpcode::NOTR => {
                todo!();
            }
            OneRegOpcode::XORC => {
                todo!();
            }
            OneRegOpcode::INVALID_RESERVED => {
                todo!();
            }
            OneRegOpcode::INVALID_NEXT_INSTR_PAGE => {
                unreachable!();
            }
        },
        VmInstruction::ZeroRegInstr(instr) => match instr.code {
            ZeroRegOpcode::INVALID_ZERO => {
                todo!();
            }
            ZeroRegOpcode::NOP => {
                todo!();
            }
            ZeroRegOpcode::JMP => {
                todo!();
            }
            ZeroRegOpcode::JE => {
                todo!();
            }
            ZeroRegOpcode::JG => {
                todo!();
            }
            ZeroRegOpcode::JL => {
                todo!();
            }
            ZeroRegOpcode::INVALID_RESERVED => {
                todo!();
            }
            ZeroRegOpcode::INVALID_NEXT_INSTR_PAGE => {
                unreachable!();
            }
        },
        VmInstruction::ErrorInstructionFromFutureISA => {
            todo!();
        }
    }
}

// -- Executor ----------------------------------------------------------------
