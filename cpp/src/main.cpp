#include <cstdio>
#include <string>
#include <string_view>
#include <format>

#include "virtual_machine/virtual_machine.hpp"

int main() {
	virtual_machine::VM ctx {};
	std::cout << ctx.mem_hexdump() << std::endl;

	for (uint8_t i { 0 }; ctx.last_operation_succeeded; ++i)
		ctx.push(i);

	std::cout << ctx.mem_hexdump() << std::endl;

	return 0;
}