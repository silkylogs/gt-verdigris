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

		// TODO: do bit level writes
		bool write_ptr(uint8_t ptr, uint8_t value) {
			if (ptr >= Memory_T::len_bytes) return false;
			this->data[ptr] = value;
			return true;
		}
	};
	
	struct VM {
		Memory_T memory { 0 };
		bool last_operation_succeeded { true };

		uint8_t segment_ptrs[4];

		static constexpr uint8_t stack_ptr_start { 0 };
		static constexpr uint8_t stack_ptr_end { 5 };
		uint8_t stack_ptr { 0 };

		
		std::string hexdump_bytes() {
			constexpr size_t col_width { 16 };

			std::string acc {};
			Pointer_T ptr {};
			auto &byte { ptr.byte_selector };
			auto count { this->memory.len_bytes };

			while (count != 0) {
				auto newline_condition { (count % col_width) == 0 };
				if (newline_condition) {
					acc.append("\n");
					acc.append(std::format("{}", ptr.to_string()));
				}
				acc.append(std::format(" {:02X}", this->memory.deref_ptr(ptr)));
				byte++; count--;
			}

			return acc;
		}

		uint8_t stack_used() {
			return this->stack_ptr - this->stack_ptr_start;
		}

		uint8_t stack_size() {
			return this->stack_ptr_end - this->stack_ptr_start;
		}

		std::string dump_machine_status() {
			std::string acc {};
			acc.append(std::format(
				"Stack usage: {}/{}; {}%\n",
				this->stack_used(), this->stack_size(),
				(static_cast<int32_t>(this->stack_used()) * 100) / this->stack_size()));
			acc.append(
				std::format("Last operation successful? {}\n", this->last_operation_succeeded)
			);
			acc.append(std::format("Memory dump: {}", this->hexdump_bytes()));
			return acc;
		}

		void push(uint8_t value) {
			if (this->stack_ptr >= this->stack_ptr_end){
				this->last_operation_succeeded = false;
				return;
			}

			//this->memory[this->stack_ptr] = value;
			this->memory.write_ptr(this->stack_ptr, value);
			this->stack_ptr++;
			this->last_operation_succeeded = true;
		}

		void pop() {
			if (this->stack_ptr > stack_ptr_start) {
				this->stack_ptr--;
				this->last_operation_succeeded = true;
				return;
			}
			this->last_operation_succeeded = false;
		}
		
	};
}
