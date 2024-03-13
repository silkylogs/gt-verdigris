#include <cstdio>
#include <string>
#include <string_view>
#include <format>

#include "virtual_machine/virtual_machine.hpp"

int main() {
	virtual_machine::VM ctx { };
	//std::cout << ctx.mem_plain_hexdump(16) << std::endl << std::endl;

	// for (uint16_t i { 0 }; i < 0xffff; ++i)
	// 	ctx.push(i);

	std::cout << ctx.mem_hexdump_as_bytes() << std::endl;

	return 0;
}