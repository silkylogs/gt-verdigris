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
// - 32 bit word, though id rather not hardcode this in the impl
// - little endian
// Note: Pointer format: 
//       wwww wwww wwww wwww wwww wwww wwww wwww xxxb bbbb
//       w = word addr, b = bit addr in word, x = reserved

namespace virtual_machine {
	enum class Endian_T {
		LITTLE, BIG,
	};

	struct Word_T {
		static constexpr size_t data_size { 32 };
		uint32_t data_container;

		std::string as_hex() {
			return std::format("{:#04X}", this->data_container);
		}
	};

	struct Byte_Selector_T {
		static constexpr size_t data_size { 5 };
		uint8_t data_container;

		// TODO: find a way to make std::format print user defined types
		std::string as_hex() {
			return std::format("xxx{:#05B}", this->data_container);
		}
	};

	struct Pointer_T {
		Word_T word_selector;
		Byte_Selector_T byte_selector;
	};

	struct Memory_T {
		static constexpr size_t memory_size_in_terms_of_word_t { 0x10 };
		std::array<Word_T, memory_size_in_terms_of_word_t> data {};

		consteval size_t size_words() {
			return this->data.size();
		}

		constexpr size_t size_bytes() { 
			return this->data[0].data_size * this->data.size();
		}
	};
	
	struct VM {
		Memory_T memory { 0 };
		Word_T memory_stack_ptr { 0 };
		bool last_operation_succeeded { true };
		

		std::string mem_hexdump_as_bytes() {
			constexpr size_t col_width_bytes { 8 };
			std::string acc { "Byte dump:\n" };
			Byte_Selector_T test { 69 };
			acc.append(std::format("{}\n", test.as_hex()));

			// Print the offsets
			acc.append("OFF: ");
			for (auto i { 0 }; i < col_width_bytes; ++i) {
				auto appendee = std::format("{:>4d} ", i);
				acc.append(appendee);
			}

			// Print the data
			auto byte_arr_ptr {
				reinterpret_cast<uint8_t*>(this->memory.data.data())
			};
			auto byte_arr_size {
				sizeof(this->memory.data[0]) * this->memory.data.size()
			};
			for (auto i {0}; i < byte_arr_size; ++i) {
				auto newline_condition { (i % col_width_bytes) == 0 };
				if (newline_condition) {
					auto address { std::format("{:#04x} ", i) };
					acc.append("\n");
					acc.append(address);
				}
				acc.append(std::format("{:#04X} ", byte_arr_ptr[i]));
			}

			return acc;
		}

		// void push(Word_T what) {
		// 	if (this->memory.data.memory_size <= this->memory_stack_ptr){
		// 		this->last_operation_succeeded = false;
		// 		return;
		// 	}

		// 	this->memory[this->memory_stack_ptr] = what;
		// 	this->memory_stack_ptr++;
		// 	this->last_operation_succeeded = true;
		// }

		// void pop() {
		// 	if (this->memory_stack_ptr >= 0) {
		// 		this->memory_stack_ptr--;
		// 		this->last_operation_succeeded = true;
		// 		return;
		// 	}
		// 	this->last_operation_succeeded = false;
		// 	return;
		// }
		
	};
}
