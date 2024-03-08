#pragma once

#include <iostream>
#include <source_location>
#include <array>
#include <cstdint>
#include <string_view>

namespace forth {
	template <typename Word_T>
	struct Context {
		static constexpr auto memory_size { static_cast<Word_T>(0x10) };
		std::array<Word_T, memory_size> memory {};
		Word_T memory_stack_ptr { 0 };

		void print() {
			for (Word_T elem: this->memory) {
				std::cout << elem << " ";
			}
		}

		bool push(Word_T what) {
			if (Context::memory_size <= this->memory_stack_ptr)
				return false;

			this->memory[this->memory_stack_ptr] = what;
			this->memory_stack_ptr++;
			return true;
		}

		bool pop() {
			if (this->memory_stack_ptr >= 0) {
				this->memory_stack_ptr--;
				return true;
			}
			return false;
		}
	};
}
