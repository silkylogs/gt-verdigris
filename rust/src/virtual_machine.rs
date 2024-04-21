// -- Registers ---------------------------------------------------------------

#[rustfmt::skip]
enum Register {
    R0, R1, R2, R3, R4, R5, R6, R7,
    R8, R9, RA, RB, RC, RD, RE, RF,
}

impl Register {
    fn from_u4(x: u8) -> Option<Register> {
        #[rustfmt::skip]
        let reg = match x {
            0x0 => Register::R0, 0x1 => Register::R1,
            0x2 => Register::R2, 0x3 => Register::R3,
            0x4 => Register::R4, 0x5 => Register::R5,
            0x6 => Register::R6, 0x7 => Register::R7,
            0x8 => Register::R8, 0x9 => Register::R9,
            0xA => Register::RA, 0xB => Register::RB,
            0xC => Register::RC, 0xD => Register::RD,
            0xE => Register::RE, 0xF => Register::RF,
            _ => { return None },
        };

        Some(reg)
    }
}

// -- Registers ---------------------------------------------------------------

// -- Three reg opcodes -------------------------------------------------------

#[allow(non_camel_case_types)]
#[rustfmt::skip]
enum ThreeRegOpcode {
    INVALID_ZERO = 0,
    ADDR = 1, SUBR = 2, MULR = 3, DIVR = 4,
    FIXMULR = 5, FIXDIVR = 6, CFIXSQRTR = 7,
    INVALID_RESERVED, // 0x8..0xE
    INVALID_NEXT_INSTR_PAGE = 0xF,
}

impl ThreeRegOpcode {
    fn from_u4(x: u8) -> ThreeRegOpcode {
        match x {
            0 => ThreeRegOpcode::INVALID_ZERO,
            1 => ThreeRegOpcode::ADDR,
            2 => ThreeRegOpcode::SUBR,
            3 => ThreeRegOpcode::MULR,
            4 => ThreeRegOpcode::DIVR,
            5 => ThreeRegOpcode::FIXMULR,
            6 => ThreeRegOpcode::FIXDIVR,
            7 => ThreeRegOpcode::CFIXSQRTR,
            _ => ThreeRegOpcode::INVALID_RESERVED,
            0xF => ThreeRegOpcode::INVALID_NEXT_INSTR_PAGE,
        }
    }
}

// Code:Reg1 Reg2:Reg3 _:_ _:_
pub struct ThreeRegInstr {
    code: ThreeRegOpcode,
    reg1: Register,
    reg2: Register,
    reg3: Register,
}

impl ThreeRegInstr {
    fn from_u32(x: u32) -> ThreeRegInstr {
        let cd = ((x >> 28) & 0x0f) as u8;
        let r1 = ((x >> 24) & 0x0f) as u8;
        let r2 = ((x >> 20) & 0x0f) as u8;
        let r3 = ((x >> 16) & 0x0f) as u8;

        let cd = ThreeRegOpcode::from_u4(cd);
        let r1 = Register::from_u4(r1);
        let r2 = Register::from_u4(r2);
        let r3 = Register::from_u4(r3);

        ThreeRegInstr {
            code: cd,
            reg1: r1.expect("Impossible program state"),
            reg2: r2.expect("Impossible program state"),
            reg3: r3.expect("Impossible program state"),
        }
    }
}

// -- Three reg opcodes -------------------------------------------------------

// -- Two reg opcodes ---------------------------------------------------------

#[allow(non_camel_case_types)]
#[rustfmt::skip]
enum TwoRegOpcode {
    INVALID_ZERO = 0,
    WRITER = 1,
    READR = 2,
    MOVR = 3,
    CMPR = 4,
    
    LSHIFTR = 5,
    ASHIFTR = 6,
    ROLLR = 7,
    
    ANDR = 8,
    ORR = 9,
    XORR = 0xA,

    INVALID_RESERVED, // 0xB .. 0xE
    INVALID_NEXT_INSTR_PAGE = 0xF,
}

impl TwoRegOpcode {
    fn from_u4(x: u8) -> TwoRegOpcode {
        match x {
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
            _ => TwoRegOpcode::INVALID_RESERVED,
            0xF => TwoRegOpcode::INVALID_NEXT_INSTR_PAGE,
        }
    }
}

// Code:Reg1 Reg2:Reg3 _:_ _:_
pub struct TwoRegInstr {
    code: TwoRegOpcode,
    reg1: Register,
    reg2: Register,
}

impl TwoRegInstr {
    fn from_u32(x: u32) -> TwoRegInstr {
        let code = ((x >> 28) & 0x0f) as u8;
        let reg1 = ((x >> 24) & 0x0f) as u8;
        let reg2 = ((x >> 20) & 0x0f) as u8;

        let code = TwoRegOpcode::from_u4(code);
        let reg1 = Register::from_u4(reg1);
        let reg2 = Register::from_u4(reg2);

        TwoRegInstr {
            code,
            reg1: reg1.expect("Impossible program state"),
            reg2: reg2.expect("Impossible program state"),
        }
    }
}

// -- Two reg opcodes ---------------------------------------------------------

// -- One reg opcodes ---------------------------------------------------------

// -- One reg opcodes ---------------------------------------------------------

