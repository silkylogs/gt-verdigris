#include <cstdio>
#include <string>
#include <string_view>
#include <format>

#include "virtual_machine/virtual_machine.hpp"

int main() {
	virtual_machine::VM ctx {};
	ctx.print();
	std::cout << std::endl;

	for (int i = 0; i < ctx.memory_size; ++i)
		ctx.push(i);

	ctx.print();

	virtual_machine::Success_T thing { true };
	std::cout << std::format("{}\n", thing);
	return 0;
}