#pragma once

#include <iostream>
#include <source_location>
#include <array>
#include <cstdint>
#include <string>
#include <format>
#include <map>
#include <ranges>

// Machine details:
// - 16 bit word
// Note: Pointer format: wwww wwww wwww wwww bbbb
//       w = word addr, b = bit addr in word

namespace virtual_machine {
	using Word_T = uint16_t;
	
	struct VM {
		static constexpr auto memory_size { static_cast<Word_T>(0x100) };
		std::array<Word_T, memory_size> memory {};
		Word_T memory_stack_ptr { 0 };
		bool last_operation_succeeded { true };

		std::string mem_to_string() {
			std::string accumulator { "Memory dump: [ " };
			for (auto elem: this->memory) {
				accumulator.append(std::format("{:#04X} ", elem));
			}
			accumulator.push_back(']');
			return accumulator;
		}

		std::string mem_hexdump() {
			constexpr auto col_width_bytes { 4 };
			std::string acc { "" };
			
			size_t i { 0 }; i--;
			for (auto const elem: this->memory) {
				i++;
				acc.append(std::format("{:03} {:#04X} ", i, elem));
				if (((i % col_width_bytes) == 0) && (i > 0)) acc.append("\n");
			}

			return acc;
		}

		void push(Word_T what) {
			if (VM::memory_size <= this->memory_stack_ptr){
				this->last_operation_succeeded = false;
				return;
			}

			this->memory[this->memory_stack_ptr] = what;
			this->memory_stack_ptr++;
			this->last_operation_succeeded = true;
		}

		void pop() {
			if (this->memory_stack_ptr >= 0) {
				this->memory_stack_ptr--;
				this->last_operation_succeeded = true;
				return;
			}
			this->last_operation_succeeded = false;
			return;
		}
		
	};
}
