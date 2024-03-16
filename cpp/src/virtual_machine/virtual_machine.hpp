#pragma once

#include <iostream>
#include <source_location>
#include <array>
#include <cstdint>
#include <string>
#include <format>
#include <map>
#include <ranges>

// TODO: start of with addressing bytes, for now
// Note: Pointer format: 
//       wwww wwww xxxb bbbb
//       w = word addr, b = bit addr in word, x = reserved

namespace virtual_machine {
	struct Bit_Selector_T {
		static constexpr size_t size_bits { 5 };
		static constexpr size_t size_bytes_aligned { 1 };
		uint8_t data_container;

		size_t to_indexable() { return static_cast<size_t>(this->data_container); }

		// std::string
		// to_string_bin_fitted_noprefix(size_t fitment_characters_least = 8, char padding_char = 'x') {
		// 	size_t padding = 0;
		// 	if (fitment_characters_least > size_bits) {
		// 		padding = fitment_characters_least - size_bits;
		// 	}
		// 	std::cout << "padding = " << padding << "\n";

		// 	std::string acc { "" };
		// 	for (auto i { 0 }; i < padding; ++i) {
		// 		acc.push_back(padding_char);
		// 	}
		// 	for (uint8_t i { this->size_bits }; i != 0; --i) {
		// 		auto x {
		// 			static_cast<uint8_t>((this->data_container >> (i - 1)) & 1)
		// 		};
		// 		char bit { '0' + x };
		// 		acc.push_back(bit);
		// 	}
		// 	return acc;
		// }
	};

	struct Pointer_T {
		uint8_t byte_selector;
		Bit_Selector_T bit_selector;
		
		std::string to_string() {
			return std::format("{:02X}:{:05B}", this->byte_selector, this->bit_selector.data_container);
		}
	};

	struct Memory_T {
		static constexpr size_t len_bytes { 0x100 };
		uint8_t data[Memory_T::len_bytes];

		uint8_t deref_ptr(Pointer_T ptr) {
			uint8_t byte1 { this->data[ptr.byte_selector] };
			
			// 01234567 89abcdef
			//   |_____ _|
			// (assuming bit selector = 2)
			// byte1 = 234567xx (i.e. left shifted n times)
			// byte2 = xxxxxx89 (i.e. right shifted (8 - n) times)
			if (ptr.bit_selector.to_indexable() > 0) {
				auto effective_next_idx { static_cast<uint8_t>(ptr.byte_selector + 1) };
				auto byte2 { static_cast<uint8_t>(this->data[effective_next_idx] >> (8 - ptr.bit_selector.to_indexable())) };
				byte1 = byte1 << ptr.bit_selector.to_indexable();
				auto effective_byte { static_cast<uint8_t>(byte1 | byte2) };
				return effective_byte;
			}

			return byte1;
		}
	};
	
	struct VM {
		Memory_T memory { 0 };
		uint8_t memory_stack_ptr { 0 };
		bool last_operation_succeeded { true };
		

		/*
		std::string mem_hexdump_as_bytes() {
			constexpr size_t col_width_bytes { 8 };
			std::string acc { "Byte dump:\n" };

			// Print the offsets
			acc.append("OFF: ");
			for (auto i { 0 }; i < col_width_bytes; ++i) {
				auto appendee = std::format("{:>4d} ", i);
				acc.append(appendee);
			}

			// Print the data
			auto byte_arr_size {
				this->memory.memory_size_bytes
			};
			for (auto i {0}; i < byte_arr_size; ++i) {
				// TODO: print actual addrs instead of an index
				auto newline_condition { (i % col_width_bytes) == 0 };
				if (newline_condition) {
					auto address { std::format("{:#04x} ", i) };
					acc.append("\n");
					acc.append(address);
				}
				acc.append(std::format("{:#04X} ", this->memory.data.as_bytes[i]));
			}

			return acc;
		}
		*/
		std::string hexdump_bytes() {
			std::string acc {};
			Pointer_T ptr {};
			auto &byte { ptr.byte_selector };
			auto count { this->memory.len_bytes };

			std::cout << "Hello, world\n";
			while (count != 0) {
				acc.append(std::format("{} {}\n", ptr.to_string(), this->memory.deref_ptr(ptr)));
				byte++;
				count--;
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
