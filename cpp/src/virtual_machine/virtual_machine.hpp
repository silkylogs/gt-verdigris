#pragma once

#include <iostream>
#include <source_location>
#include <array>
#include <cstdint>
#include <string_view>

namespace virtual_machine {
	using Word_T = int;
	using Success_T = bool;
	
	struct VM {
		static constexpr auto memory_size { static_cast<Word_T>(0x10) };
		std::array<Word_T, memory_size> memory {};
		Word_T memory_stack_ptr { 0 };

		void print() {
			for (Word_T elem: this->memory) {
				std::cout << elem << " ";
			}
		}

		Success_T push(Word_T what) {
			if (VM::memory_size <= this->memory_stack_ptr)
				return false;

			this->memory[this->memory_stack_ptr] = what;
			this->memory_stack_ptr++;
			return true;
		}

		Success_T pop() {
			if (this->memory_stack_ptr >= 0) {
				this->memory_stack_ptr--;
				return true;
			}
			return false;
		}
		
	};
}
