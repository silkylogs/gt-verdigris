#include <cstdio>
#include <string>
#include <string_view>
#include <format>

#include "virtual_machine/virtual_machine.hpp"

int main() {
	virtual_machine::VM ctx { };

	std::cout << "Regular pushing";
	ctx.push(1); ctx.push(2); ctx.push(3); ctx.push(4); ctx.push(5);
	std::cout << ctx.hexdump_bytes() << "\n";

	std::cout << "Stack overflow";
	ctx.push(6);
	std::cout << ctx.hexdump_bytes() << "\n";

	std::cout << "Replacing stack values";
	ctx.pop(); ctx.pop(); ctx.pop(); ctx.pop();
	ctx.push(7); ctx.push(8); ctx.push(9); ctx.push(10);
	std::cout << ctx.hexdump_bytes() << "\n";


	return 0;
}