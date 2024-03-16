#include <cstdio>
#include <string>
#include <string_view>
#include <format>

#include "virtual_machine/virtual_machine.hpp"

int main() {
	virtual_machine::VM ctx { };
	
	std::cout << ctx.hexdump_bytes() << "\n";

	return 0;
}