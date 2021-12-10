
#pragma once

#include <cstdint>
#include <cstddef>

namespace Util
{
	uint64_t get_net_ul(const uint8_t* data);
	uint32_t get_net_ui(const uint8_t* data);
	uint16_t get_net_us(const uint8_t* data);

	void set_net_ul(uint8_t* data, uint64_t value);
	void set_net_ui(uint8_t* data, uint32_t value);
	void set_net_us(uint8_t* data, uint16_t value);

	bool compare(const uint8_t* data1, const uint8_t* data2, size_t len);
};

