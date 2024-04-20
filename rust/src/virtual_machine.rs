#[rustfmt::skip]
enum Register {
    R0, R1, R2, R3, R4, R5, R6, R7,
    R8, R9, RA, RB, RC, RD, RE, RF,
}

enum ThreeRegOpCode {
    INVALID_ZERO = 0,
    ADDR = 1,
    SUBR = 2,
    MULR = 3,
    DIVR = 4,
    FIXMULR = 5,
    FIXDIVR = 6,
    CFIXSQRTR = 7,
    INVALID_RESERVED, // 0x8..0xE
    INVALID_NEXT_INSTR_PAGE = 0xF,
}

pub struct ThreeRegInstr {
    code: ThreeRegOpcode,
    reg1: Register,
    reg2: Register,
    reg3: Register,
}

fn u32_to_ThreeRegInstr(x: u32) -> ThreeRegInstr {
    todo!();
}
