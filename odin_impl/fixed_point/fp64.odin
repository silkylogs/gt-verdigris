package fixed_point


// -- Context --------------------------------------------

/*
The scaling factor (also referred to as the implicit dividend) also determines accuracy
Example: a scaling factor of 100 yeilds a maximum accuracy of 0.01
*/
fp64_context :: struct {
	scaling_factor: int,
}

make_context_from_idiv :: proc(implicit_dividend: int) -> fp64_context {
	return fp64_context { scaling_factor = implicit_dividend }
}

// -- Context --------------------------------------------


// -- From types --------------------------------------------

fp64 :: distinct int

fp64_underlying :: proc(x: fp64) -> int {
	return int(x)
}

fp64_from_int :: proc(x: int, ctx: fp64_context) -> fp64 {
	return fp64(x * ctx.scaling_factor)
}

fp64_from_f64 :: proc(x: f64, ctx: fp64_context) -> fp64 {
	as_whole := x * f64(ctx.scaling_factor)
	return fp64(as_whole)
}

fp64_from_f32 :: proc(x: f32, ctx: fp64_context) -> fp64 {
	as_whole := x * f32(ctx.scaling_factor)
	return fp64(as_whole)
}

/*
Assumes input is a valid literal
- Allowed chars: 0 through 9, period, underscore
- No spaces inbetween, trailing or preceeding
The context is then used to trim to accuracy, if needed
*/
// Note: fp64_determine_context_from_string() could be a function that sets the accuracy for you
fp64_from_string :: proc(x: string, ctx: fp64_context) -> fp64 {
	num_length, decimal_point_idx: int
	decimal_point_set := false

	// Get initial info
	for cp, idx in x {
		switch cp {
		case '0'..='9', '_': /* continue */
		case '.':
			if decimal_point_set { return fp64(0) }
			decimal_point_set = true
			decimal_point_idx = idx
		case:
			num_length = idx
			break
		}
	}

	//todo
	is_whole_number := !decimal_point_set
	if is_whole_number {

	}
	whole_num_start_idx := 0
	//whole_num_end_idx

	return fp64(0) // temporariliy shut the compiler up
}

// -- From types --------------------------------------------

// -- Arithmetic --------------------------------------------

fp64_add :: proc(a: fp64, b: fp64) -> fp64 {
	return a + b
}

fp64_sub :: proc(a: fp64, b: fp64) -> fp64 {
	return a - b
}

/*
Return type is guaranteed to have same scaling factor as that of the operands,
assuming both inputs have the same scaling factor
The following working is provided to placate me in the future
=> 2.5 * 2.0
=> (25/10) * (20/10)
=> 500/100 (== 5.0)
=> 50/10 (== 5.0)
*/
fp64_mul :: proc(a: fp64, b: fp64, ctx: fp64_context) -> fp64 {
	intermediate := i128(a) * i128(b)
	retval := intermediate / i128(ctx.scaling_factor)
	return fp64(retval)
}

/*
Return type is guaranteed to have same scaling factor as that of the operands,
assuming both inputs have the same scaling factor
The following working is provided to placate me in the future
```
=> 1.23 / 6.25
=> (123/100) / (625/100)
=> ((123/100)*(100/100)) / (625/100) (numerator multiplied by accuracy multiplier)
=> (12,300/10,000) / (625/100) 
=> (12,300/625) / (100/10,000)
=> 19/100 (== 0.19)
```
Note that having a accuracy multiplier of greater than the implicit dividend is futile if
the end result is to be fitted within the accuracy specified by the implicit dividend,
hence the implicit dividend is used as the accuracy multiplier
*/
fp64_div :: proc(numerator: fp64, denominator: fp64, ctx: fp64_context) -> fp64 {
	scaled_numerator := i128(numerator) * i128(ctx.scaling_factor)
	quotient := scaled_numerator / i128(denominator)
	return fp64(quotient)
}

// -- Arithmetic --------------------------------------------
