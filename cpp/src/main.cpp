#include <cstdio>
#include <string>
#include <string_view>

#include "forth/forth.hpp"

int main() {
	forth::Context<int32_t> ctx {};
	ctx.print();
	std::cout << std::endl;

	for (int i = 0; i < ctx.memory_size; ++i)
		ctx.push(i);

	ctx.print();
	return 0;
}