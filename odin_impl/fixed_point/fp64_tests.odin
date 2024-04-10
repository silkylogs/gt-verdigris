package fixed_point

import "core:fmt"
import "core:math"
import "core:runtime"

@(private)
test_fp64_from_string :: proc() -> (ok: bool, cmt: string, proc_name: string) {
	proc_name = #procedure

	ctx := make_context_from_idiv(1_000)
	num1 := fp64_from_string("123.456", ctx)
	expected := fp64(123456)
	if num1 != expected {
		ok = false
		cmt = fmt.aprint(
			"Base case failure. underlying != expected.",
			num1, "!=", expected
		)
		return
	}

	num2 := fp64_from_string(".456", ctx)
	expected = fp64(456)
	if num2 != expected {
		ok = false
		cmt = fmt.aprint(
			"Fraction only failure. underlying != expected.",
			num2, "!=", expected
		)
		return
	}

	num3 := fp64_from_string("123", ctx)
	expected = fp64(123000)
	if num3 != expected {
		ok = false
		cmt = fmt.aprint(
			"Whole number only case failure.",
			"underlying != expected.",
			num3, "!=", expected
		)
		return
	}

	num4 := fp64_from_string("123.4", ctx)
	expected = fp64(123400)
	if num4 != expected {
		ok = false
		cmt = fmt.aprint(
			"Fractional shift left failure.",
			"underlying != expected.",
			num4, "!=", expected
		)
		return
	}

	num5 := fp64_from_string("123.4567890", ctx)
	expected = fp64(123456)
	if num5 != expected {
		ok = false
		cmt = fmt.aprint(
			"Fractional shift right failure.",
			"underlying != expected.",
			num5, "!=", expected
		)
		return
	}

	ok = true
	cmt = ""
	return
}

@(private)
test_fp64_from_int :: proc() -> (ok: bool, cmt: string, proc_name: string) {
	proc_name = #procedure

	ctx := make_context_from_idiv(1000)
	num_42 := fp64_from_int(42, ctx)

	underlying := fp64_underlying(num_42)
	expected := 42000
	if underlying != expected {
		ok = false
		cmt = fmt.aprint(
			"underlying does not match expected:",
			underlying, "!=", expected
		)
		return
	}

	ok = true
	cmt = ""
	return
}

@(private)
test_fp64_from_f64 :: proc() -> (ok: bool, cmt: string, proc_name: string) {
	proc_name = #procedure

	ctx := make_context_from_idiv(1_000_000_000_000_000)
	pi64 := fp64_from_f64(math.PI, ctx)

	underlying := fp64_underlying(pi64)
	expected := 3141592653589793
	if underlying != expected {
		ok = false
		cmt = fmt.aprint(
			"underlying does not match expected:",
			underlying, "!=", expected
		)
		return
	}

	ok = true
	cmt = ""
	return
}

@(private)
test_fp64_from_f32 :: proc() -> (ok: bool, cmt: string, proc_name: string) {
	proc_name = #procedure

	ctx := make_context_from_idiv(10_000_000)
	pi32 := fp64_from_f32(f32(math.PI), ctx)

	underlying := fp64_underlying(pi32)
	expected := 31415928
	if underlying != expected {
		ok = false
		cmt = fmt.aprint(
			"underlying does not match expected:",
			underlying, "!=", expected
		)
		return
	}

	ok = true
	cmt = ""
	return
}

@(private)
test_fp64_add :: proc() -> (ok: bool, cmt: string, proc_name: string) {
	proc_name = #procedure

	ctx := make_context_from_idiv(100)

	one_third := fp64_from_f32(1.0 / 3.0, ctx)
	two_third := fp64_add(one_third, one_third)
	nearly_one := fp64_add(one_third, two_third)
	expected_val := fp64(99)

	if nearly_one != expected_val {
		ok = false
		cmt = fmt.aprint(
			"precision not met.",
			nearly_one, "!=", expected_val
		)
		return
	}

	ok = true
	cmt = ""
	return
}

@(private)
test_fp64_sub :: proc() -> (ok: bool, cmt: string, proc_name: string) {
	proc_name = #procedure

	ctx := make_context_from_idiv(1000)

	// TODO: fp64_from_string(string, ctx)
	a := fp64(3419)
	b := fp64(0419)
	res := fp64_sub(a, b)
	expected := fp64_from_int(3, ctx)

	if res != expected {
		ok = false
		cmt = fmt.aprint(
			"precision not met:",
			res, "!=", expected
		)
		return
	}

	ok = true
	cmt = ""
	return
}

@(private)
test_fp64_mul :: proc() -> (ok: bool, cmt: string, proc_name: string) {
	proc_name = #procedure

	ctx := make_context_from_idiv(100)
	a := fp64(25)
	b := fp64_from_int(2, ctx)
	res := fp64_mul(a, b, ctx)
	expected := fp64(50)

	if res != expected {
		ok = false
		cmt = fmt.aprint(
			"precision not met:",
			res, "!=", expected
		)
		return
	}

	ok = true
	cmt = ""
	return
}

@(private)
test_fp64_div :: proc() -> (ok: bool, cmt: string, proc_name: string) {
	proc_name = #procedure

	ctx := make_context_from_idiv(100)
	a := fp64(123)
	b := fp64(625)
	res := fp64_div(a, b, ctx)
	expected := fp64(19)

	if res != expected {
		ok = false
		cmt = fmt.aprint(
			"precision not met:",
			res, "!=", expected
		)
	}

	ok = true
	cmt = ""
	return
}

fp64_run_tests :: proc() -> (all_ok: bool, log: string) {
	run_test :: proc(test_proc: proc() -> (bool, string, string)) -> (bool, string) {
		test_ok, comment, proc_name := test_proc()
		log_str := fmt.aprintf(
			"%v succeeded? %v. comment: %v\n",
			proc_name, test_ok, comment
		)
		return test_ok, log_str
	}

	concat_string :: proc(a: string, b: string) -> string {
		return fmt.aprintf("%v%v", a, b)
	}

	all_ok = true
	ok: bool
	cmt: string

	ok, cmt = run_test(test_fp64_from_string)
	log = concat_string(log, cmt)
	all_ok &= ok
	
	ok, cmt = run_test(test_fp64_from_int)
	log = concat_string(log, cmt)
	all_ok &= ok

	ok, cmt = run_test(test_fp64_from_f64)
	log = concat_string(log, cmt)
	all_ok &= ok

	ok, cmt = run_test(test_fp64_from_f32)
	log = concat_string(log, cmt)
	all_ok &= ok
	
	ok, cmt = run_test(test_fp64_add)
	log = concat_string(log, cmt)
	all_ok &= ok
	
	ok, cmt = run_test(test_fp64_sub)
	log = concat_string(log, cmt)
	all_ok &= ok
	
	ok, cmt = run_test(test_fp64_mul)
	log = concat_string(log, cmt)
	all_ok &= ok
	
	ok, cmt = run_test(test_fp64_div)
	log = concat_string(log, cmt)
	all_ok &= ok

	if all_ok {
		log = fmt.aprintf("All tests succeeded. Log:\n%v", log)
	} else {
		log = fmt.aprintf("One or more tests failed. Log:\n%v", log)
	}
	return
}
