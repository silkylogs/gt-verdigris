// -- Registers ---------------------------------------------------------------

#[rustfmt::skip]
enum Register {
    R0, R1, R2, R3, R4, R5, R6, R7,
    R8, R9, RA, RB, RC, RD, RE, RF,
}

impl Register {
    #[rustfmt::skip]
    fn from_u4(x: u8) -> Register {
        match x & 0x0F {
            0x0 => Register::R0, 0x1 => Register::R1,
            0x2 => Register::R2, 0x3 => Register::R3,
            0x4 => Register::R4, 0x5 => Register::R5,
            0x6 => Register::R6, 0x7 => Register::R7,
            0x8 => Register::R8, 0x9 => Register::R9,
            0xA => Register::RA, 0xB => Register::RB,
            0xC => Register::RC, 0xD => Register::RD,
            0xE => Register::RE, 0xF => Register::RF,
            _ => unreachable!(),
        }
    }
}

// -- Registers ---------------------------------------------------------------

// -- Three reg opcodes -------------------------------------------------------


// TODO: fix "unreachable pattern" warnings by enumerating every single bit pattern
#[allow(non_camel_case_types)]
#[rustfmt::skip]
enum ThreeRegOpcode {
    INVALID_ZERO = 0,
    
    ADDR = 1,
    SUBR = 2,
    MULR = 3,
    DIVR = 4,

    FIXMULR = 5,
    FIXDIVR = 6,
    CFIXSQRTR = 7,

    INVALID_RESERVED_8,
    INVALID_RESERVED_9,
    INVALID_RESERVED_A,
    INVALID_RESERVED_B,
    INVALID_RESERVED_C,
    INVALID_RESERVED_D,
    INVALID_RESERVED_E,

    INVALID_NEXT_INSTR_PAGE = 0xF,
}

impl ThreeRegOpcode {
    fn from_u4(x: u8) -> ThreeRegOpcode {
        match x & 0x0F {
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
        let code = ((x >> 28) & 0x0f) as u8;
        let reg1 = ((x >> 24) & 0x0f) as u8;
        let reg2 = ((x >> 20) & 0x0f) as u8;
        let reg3 = ((x >> 16) & 0x0f) as u8;

        let code = ThreeRegOpcode::from_u4(code);
        let reg1 = Register::from_u4(reg1);
        let reg2 = Register::from_u4(reg2);
        let reg3 = Register::from_u4(reg3);

        ThreeRegInstr { code, reg1, reg2, reg3, }
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

        TwoRegInstr { code, reg1, reg2, }
    }
}

// -- Two reg opcodes ---------------------------------------------------------

// -- One reg opcodes ---------------------------------------------------------

#[allow(non_camel_case_types)]
#[rustfmt::skip]
enum OneRegOpcode {
    INVALID_ZERO = 0x0,
    
    READC = 0x1,
    WRITEC = 0x2,
    MOVC = 0x3,
    CALLR = 0x4,
    CMPC = 0x5,

    LSHIFTC = 0x6,
    ASHIFTC = 0x7,
    ROLLC = 0x8,

    ANDC = 0x9,
    ORC = 0xA,
    NOTR = 0xB,
    XORC = 0xC,

    INVALID_RESERVED, // 0xD .. 0xE
    INVALID_NEXT_INSTR_PAGE = 0xF,
}

impl OneRegOpcode {
    fn from_u4(x: u8) -> OneRegOpcode {
        match x & 0xF {
            0x0 => OneRegOpcode::INVALID_ZERO,
            0x1 => OneRegOpcode::READC,
            0x2 => OneRegOpcode::WRITEC,
            0x3 => OneRegOpcode::MOVC,
            0x4 => OneRegOpcode::CALLR,
            0x5 => OneRegOpcode::CMPC,
            0x6 => OneRegOpcode::LSHIFTC,
            0x7 => OneRegOpcode::ASHIFTC,
            0x8 => OneRegOpcode::ROLLC,
            0x9 => OneRegOpcode::ANDC,
            0xA => OneRegOpcode::ORC,
            0xB => OneRegOpcode::NOTR,
            0xC => OneRegOpcode::XORC,
            _ => OneRegOpcode::INVALID_RESERVED,
            0xF => OneRegOpcode::INVALID_NEXT_INSTR_PAGE,
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
            OneRegOpcode::CALLR => false,
            OneRegOpcode::NOTR => false,
            OneRegOpcode::INVALID_RESERVED => false,
            OneRegOpcode::INVALID_NEXT_INSTR_PAGE => false,
        }
    }
}

// Code:Reg1 Reg2:Reg3 _:_ _:_
pub struct OneRegInstr {
    code: OneRegOpcode,
    reg1: Register,
    optional_const_exists: bool,
    optional_const: u32,
}

impl OneRegInstr {
    fn from_u32_and_const(x: u32, optional_const: u32) -> OneRegInstr {
        let code = ((x >> 28) & 0x0f) as u8;
        let reg1 = ((x >> 24) & 0x0f) as u8;
        
        let code = OneRegOpcode::from_u4(code);
        let reg1 = Register::from_u4(reg1);
        let hasc = OneRegOpcode::has_constant(&code);

        OneRegInstr { code, reg1, optional_const_exists: hasc, optional_const }
    }
}

// -- One reg opcodes ---------------------------------------------------------

