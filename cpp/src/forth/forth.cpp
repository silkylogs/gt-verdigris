#include "forth.hpp"

void forth::hello() {
	auto file_name {
		std::source_location::current().file_name()
	};
	printf("Hello from %s\n", file_name);
}