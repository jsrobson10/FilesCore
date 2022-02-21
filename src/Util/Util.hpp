
#pragma once

#include <cstdint>
#include <cstddef>
#include <string>

namespace Util
{
	namespace Net
	{
		uint64_t get_ul(const uint8_t* data);
		uint32_t get_ui(const uint8_t* data);
		uint16_t get_us(const uint8_t* data);

		void set_ul(uint8_t* data, uint64_t value);
		void set_ui(uint8_t* data, uint32_t value);
		void set_us(uint8_t* data, uint16_t value);
	}

	namespace Text
	{
		size_t count_whitespace(const char* text);
		size_t count_whitespace(const std::string& text);
		size_t count_whitespace(const char* text, size_t len);

		uint64_t get_char_pos(char c, const char* text);
		uint64_t get_char_pos(char c, const std::string& text);
		uint64_t get_char_pos(char c, const char* text, size_t len);

		void to_upper(const char* in, char* out);
		void to_lower(const char* in, char* out);
		void to_cased(const char* in, char* out);

		void to_upper(const char* in, char* out, size_t len);
		void to_lower(const char* in, char* out, size_t len);
		void to_cased(const char* in, char* out, size_t len);

		std::string to_upper(const std::string& in);
		std::string to_lower(const std::string& in);
		std::string to_cased(const std::string& in);

		char* skip_whitespace(char* data);
		char* skip_until_whitespace(char* data);
		
		const char* skip_whitespace(const char* data);
		const char* skip_until_whitespace(const char* data);
	};
};

