package fixed_point

import "core:fmt"
import "core:math"

fp64 :: distinct int

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

// -- Context -----------------------------------------------

// -- Utility -----------------------------------------------

fp64_underlying :: proc(x: fp64) -> int {
	return int(x)
}

fp64_whole :: proc(x: fp64, ctx: fp64_context) -> int { return 0 } // TODO
fp64_frac :: proc(x: fp64, ctx: fp64_context) -> int { return 0 } // TODO

// -- Utility -----------------------------------------------

// -- From types --------------------------------------------

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

@(private)
fp64_parse_whole_num :: proc(num: string) -> int {
	multiple := 1
	acc := 0

	for idx := len(num) - 1; idx >= 0; idx -= 1 {
		digit := num[idx] - '0'
		acc += multiple * int(digit)
		multiple *= 10
	}

	return acc
}

@(private)
powi :: proc(x, power: int) -> int {
	if power == 0 { return 1 }
	x := x
	for p := 0; p < power; p += 1 {
		x *= x
	}
	return x
}

@(private)
log10i :: proc(x: int) -> int {
	// repeatedly divide by 10 until zero
	return int(math.log10_f64(f64(x)))
}

@(private)
absi :: proc(x: int) -> int {
	return x & 0x7FFF_FFFF_FFFF_FFFF
}

/*
Assumes input is a valid literal
- Allowed chars: 0 through 9, period, underscore
- No spaces inbetween, trailing or preceeding
The context is then used to trim to accuracy, if needed
*/
// Note: fp64_determine_context_from_string() could be a function that sets the accuracy for you
fp64_from_string_proto1 :: proc(x: string, ctx: fp64_context) -> fp64 {
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

	if !decimal_point_set { decimal_point_idx = len(x) }
	whole_num_as_str := x[0 : decimal_point_idx]
	whole_num_as_int := fp64_parse_whole_num(whole_num_as_str)
	fixed_point_whole_component := fp64(whole_num_as_int * ctx.scaling_factor)

	// TODO: dont bother parsing fractions if need not be
	frac_as_str: string
	if decimal_point_set {
		frac_as_str = x[decimal_point_idx+1 : len(x)]
	} else {
		frac_as_str = ""
	}
	fmt.println("frac as str", frac_as_str)

	frac_as_int := fp64_parse_whole_num(frac_as_str)
	fmt.println("frac as int", frac_as_int)

	required_digit_cnt := log10i(ctx.scaling_factor) + 1
	fmt.println("required digit count", required_digit_cnt)
	
	frac_digit_cnt := int(log10i(frac_as_int) + 1)
	fmt.println("frac digit count", frac_digit_cnt)

	digit_cnt_diff := required_digit_cnt - frac_digit_cnt
	fmt.println("digit count diff", digit_cnt_diff)

	Base10_Shift :: enum { NONE, LEFT, RIGHT }
	shift: Base10_Shift
	switch {
	case digit_cnt_diff >= 0: shift = Base10_Shift.LEFT
	case digit_cnt_diff < 0: shift = Base10_Shift.RIGHT
	case: shift = Base10_Shift.NONE
	}
	fmt.println("shift status", shift)

	multiple_of_ten := powi(10, absi(digit_cnt_diff))
	fmt.println("multiple of ten", multiple_of_ten)

	fixed_point_frac_component: fp64
	switch shift {
	case .LEFT:
		fixed_point_frac_component = fp64(frac_as_int * multiple_of_ten)
	case .RIGHT:
		fixed_point_frac_component = fp64(frac_as_int / multiple_of_ten)
	case .NONE:
		fixed_point_frac_component = fp64(frac_as_int)
	}
	fmt.println("fixed point frac component", fixed_point_frac_component)

	result := fixed_point_whole_component + fixed_point_frac_component
	fmt.println("result", result)

	return result
}

@(private)
fp64_parse_frac :: proc(frac_int: int, ctx: fp64_context) -> fp64 {
	required_digit_cnt := log10i(ctx.scaling_factor) + 1
	fmt.println("required_digit_cnt", required_digit_cnt)
	
	frac_digit_cnt := int(log10i(frac_int) + 1)
	fmt.println("frac_digit_cnt", frac_digit_cnt)

	digit_cnt_diff := required_digit_cnt - frac_digit_cnt
	fmt.println("digit_cnt_diff", digit_cnt_diff)

	Base10_Shift :: enum { NONE, LEFT, RIGHT }
	shift: Base10_Shift
	switch {
	case digit_cnt_diff >= 0: shift = Base10_Shift.LEFT
	case digit_cnt_diff < 0: shift = Base10_Shift.RIGHT
	case: shift = Base10_Shift.NONE
	}
	fmt.println("shift", shift)

	multiple_of_ten := powi(10, absi(digit_cnt_diff))
	fmt.println("multiple_of_ten", multiple_of_ten)

	frac_fp: fp64
	switch shift {
	case .LEFT:
		frac_fp = fp64(frac_int * multiple_of_ten)
	case .RIGHT:
		frac_fp = fp64(frac_int / multiple_of_ten)
	case .NONE:
		frac_fp = fp64(frac_int)
	}
	fmt.println("frac_fp", frac_fp)

	return frac_fp
}

fp64_from_string_proto2 :: proc(
	x: string, ctx: fp64_context
) -> (fp64, bool) {
	NumberKind :: enum {
		ERROR, WHOLE_ONLY, FRAC_ONLY, MIXED
	}
	kind := NumberKind.ERROR

	period_cnt, period_idx, num_start_idx, num_end_idx: int
	digit_encountered, for_loop_executed: bool
	for cp, idx in x {
		switch cp {
		case '0'..='9':
			if !digit_encountered { num_start_idx = idx }
			digit_encountered = true
		case '_': /* continue */
		case '.':
			period_cnt += 1
			period_idx = idx
		case:
			num_end_idx = idx - 1
			break
		}
	}
	for_loop_executed = true

	if !digit_encountered || period_cnt > 1 || !for_loop_executed {
		kind = NumberKind.ERROR
	}
	if kind == NumberKind.ERROR {
		return fp64(0), false
	}

	if period_cnt == 0 {
		kind = NumberKind.WHOLE_ONLY
	} else if num_start_idx == period_idx {
		kind = NumberKind.FRAC_ONLY
	} else {
		kind = NumberKind.MIXED
	}
	switch kind {
	case .WHOLE_ONLY:
		whole_str := x[num_start_idx : period_idx]
		fmt.println("whole_str", whole_str)

		whole_int := fp64_parse_whole_num(whole_str)
		fmt.println("whole_int", whole_int)

		whole_fp := fp64(whole_int * ctx.scaling_factor)
		fmt.println("whole_fp", whole_fp)

		return whole_fp, true
	case .FRAC_ONLY:
		frac_str := x[period_idx + 1 : num_end_idx + 1]
		fmt.println("frac_str", frac_str)

		frac_int := fp64_parse_whole_num(frac_str)
		fmt.println("frac_int", frac_int)

		frac_fp := fp64_parse_frac(frac_int, ctx)
		fmt.println("frac_fp", frac_fp)

		return frac_fp, true
	case .MIXED:
		whole_str := x[num_start_idx : period_idx]
		fmt.println("whole_str", whole_str)

		whole_int := fp64_parse_whole_num(whole_str)
		fmt.println("whole_int", whole_int)

		whole_fp := fp64(whole_int * ctx.scaling_factor)
		fmt.println("whole_fp", whole_fp)

		frac_str := x[period_idx + 1 : num_end_idx + 1]
		fmt.println("frac_str", frac_str)

		frac_int := fp64_parse_whole_num(frac_str)
		fmt.println("frac_int", frac_int)

		frac_fp := fp64_parse_frac(frac_int, ctx)
		fmt.println("frac_fp", frac_fp)

		result := whole_fp + frac_fp
		fmt.println("result", result)

		return result, true
	case .ERROR: // "Cant happen" case
		return fp64(0), false
	}

	// "Cosmic radiation hit my chip" case
	return fp64(0), false
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
