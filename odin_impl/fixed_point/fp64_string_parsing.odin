package fixed_point

import "core:fmt"

fp64_from_string :: proc(x: string) -> (bool, fp64, fp64_context) {
	acc_b10_ok, acc_b10 := extract_b10_acc(x)
	whole_ok, whole := extract_whole(x)
	fract_ok, fract := extract_fract(x)
	if !(whole_ok && fract_ok && acc_b10_ok) {
		return false, fp64(0), fp64_context {}
	}

	ctx_ok, ctx := ctx_from_acc(acc_b10)
	if !ctx_ok {
		return false, fp64(0), fp64_context {}
	}

	result := fp64(whole * ctx.scaling_factor + fract)
	return true, result, ctx
}

extract_b10_acc :: proc(x: string) -> (bool, int) {
	frac_str_ok, frac_str := extract_frac_str(x)
	if !frac_str_ok {
		return false, 0
	}

	digit_cnt_ok, digit_cnt := count_digits_in_str(frac_str)
	if !digit_cnt_ok {
		return false, 0
	}

	/*
	.1 = 1/10 = 10
	.01 = 1/100 = 100 and so on
	*/
	acc := pow10i(digit_cnt)
	return true, acc
}

// 10 to the power x
pow10i :: proc(x: int) -> int {
	acc := 1
	for p := 0; p < x; p += 1 {
		acc *= 10
	}
	return acc
}

/*
Assumptions
- Valid runes: '0'..='9', '.', '_' exclusively
- Empty strings are considered valid
- Only one '.' may exist in the string
*/
extract_frac_str :: proc(x: string) -> (bool, string) {
	period_cnt, period_idx: int
	invalid_char_encountered := false

	for digit, idx in x {
		switch digit {
			case '0'..='9', '_': /* cont */
			case '.':
				period_cnt += 1
				period_idx = idx
			case:
				invalid_char_encountered = true
		}
	}

	frac_end_idx := len(x)
	frac_start_idx := period_idx + 1
	if	invalid_char_encountered ||
		period_cnt > 1 ||
		frac_start_idx > frac_end_idx
	{
		return false, ""
	}

	if period_cnt == 0 { return true, "" }

	result := x[frac_start_idx: frac_end_idx]
	return true, result
}

extract_fract :: proc(x: string) -> (bool, int) {
	frac_str_ok, frac_str := extract_frac_str(x)
	if !frac_str_ok { return false, 0 }

	frac_int_ok, frac_int := conv_str_to_int(frac_str)
	if !frac_int_ok { return false, 0 }

	return true, frac_int
}

extract_whole :: proc(x: string) -> (bool, int) {
	whole_str_ok, whole_str := extract_whole_str(x)
	if !whole_str_ok { return false, 0 }

	whole_int_ok, whole_int := conv_str_to_int(whole_str)
	if !whole_int_ok { return false, 0 }

	return true, whole_int
}

extract_whole_str :: proc(x: string) -> (bool, string) {
	period_idx: int
	period_encountered := false
	invalid_char_encountered := false

	for digit, idx in x {
		switch digit {
			case '0'..='9', '_': /* cont */
			case '.':
				period_encountered = true
				period_idx = idx
			case:
				invalid_char_encountered = true
		}
	}

	if invalid_char_encountered {
		return false, ""
	}

	whole_start_idx := 0
	whole_end_idx: int
	if period_encountered { whole_end_idx = period_idx }
	else { whole_end_idx = len(x) }

	result := x[whole_start_idx : whole_end_idx]
	return true, result
}

ctx_from_acc :: proc(acc_b10: int) -> (bool, fp64_context) {
	ctx := make_context_from_idiv(acc_b10)
	return true, ctx
}

conv_str_to_int :: proc(x: string) -> (bool, int) {
	multiple := 1
	acc := 0

	for idx := len(x) - 1; idx >= 0; idx -= 1 {
		digit_char := x[idx]
		switch digit_char {
		case '0'..='9':
			digit := digit_char - '0'
			acc += multiple * int(digit)
			multiple *= 10
		case '_': /* cont */
		case: return false, 0
		}
	}

	return true, acc
}

/*
- Underscores and periods ignored
- 0..=9 counted as digit
- everything else counted as error
*/
count_digits_in_str :: proc(x: string) -> (bool, int) {
	count := 0
	
	for digit in x {
		switch digit {
		case '0'..='9': count += 1
		case '_', '.': /* cont */
		case: return false, 0
		}
	}

	return true, count
}
