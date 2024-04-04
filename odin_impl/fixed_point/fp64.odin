package fixed_point

import "core:fmt"

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

// This also serves as the usage example
fp64_run_tests :: proc() -> (bool, string) {
	MessageType :: enum { ERROR, SUCCESS, MESSAGE }
	msg :: proc(type: MessageType, message: string) -> string {
		return fmt.aprintf("fp64: %v: %v", type, message)
	}

	using MessageType

	test_make_ctx :: proc() -> (bool, string) {
		return false, msg(ERROR, "Test not implemented")
	}

	test_from_int :: proc() -> (bool, string) {
		return false, msg(ERROR, "Test not implemented")
	}

	test_from_f64 :: proc() -> (bool, string) {
		return false, msg(ERROR, "Test not implemented")
	}

	test_from_f32 :: proc() -> (bool, string) {
		return false, msg(ERROR, "Test not implemented")
	}

	test_add :: proc() -> (bool, string) {
		ctx := make_context_from_idiv(100)

		one_third := fp64_from_f32(1.0 / 3.0, ctx)
		two_third := fp64_add(one_third, one_third)
		nearly_one := fp64_add(one_third, two_third)
		expected_val := fp64_from_f64(0.99, ctx)

		if nearly_one != expected_val {
			return false, msg(ERROR, fmt.aprint("precision not met.", nearly_one, "!=", expected_val))
		}

		return true, msg(SUCCESS, "add")
	}

	test_sub :: proc() -> (bool, string) {
		return false, msg(ERROR, "Test not implemented")
	}

	test_mul :: proc() -> (bool, string) {
		return false, msg(ERROR, "Test not implemented")
	}

	test_div :: proc() -> (bool, string) {
		return false, msg(ERROR, "Test not implemented")
	}

	ok: bool
	status: string

	ok, status = test_make_ctx()
	fmt.println(status)
	if !ok { return false, msg(ERROR, "One test failed") }

	ok, status = test_from_int()
	fmt.println(status)
	if !ok { return false, msg(ERROR, "One test failed") }

	ok, status = test_from_f64()
	fmt.println(status)
	if !ok { return false, msg(ERROR, "One test failed") }

	ok, status = test_from_f32()
	fmt.println(status)
	if !ok { return false, msg(ERROR, "One test failed") }

	ok, status = test_add()
	fmt.println(status)
	if !ok { return false, msg(ERROR, "One test failed") }

	ok, status = test_sub()
	fmt.println(status)
	if !ok { return false, msg(ERROR, "One test failed") }

	ok, status = test_mul()
	fmt.println(status)
	if !ok { return false, msg(ERROR, "One test failed") }

	ok, status = test_div()
	fmt.println(status)
	if !ok { return false, msg(ERROR, "One test failed") }

	return true, msg(MessageType.MESSAGE, "All tests succeeded")
}

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
