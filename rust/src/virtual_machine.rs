/// This contains all opcodes to be used by this version of the virtual machine
#[rustfmt::skip]
static opcodes: [&'static str; 50] = [
	// Three reg op
	"addr", "subr", "umulr", "udivr", "smulr", "sdivr",
	"sfixmulr", "sfixdivr", "ufixmulr", "ufixdivr",

	// Two reg op
	"movr", "cmpr",
	"andr", "orr", "notr", "xorr",
	"lshlr", "lshrr", "ashlr", "ashrr", "rolr", "rorr",
	"ufixsqrtr",

	// One reg op
	"gotor", "callr",

	// One reg op with constant
	"mov", "cmp",
	"read", "write",
	"add", "sub", "umul", "udiv", "smul", "sdiv",
	"lshl", "lshr",

	// Zero reg op
	"nop", "ret",

	// Zero reg op with constant
	"call", "callge", "callg", "callle", "calll",
	"jmp", "jge", "jg", "jle", "jl",
	"goto",
];
