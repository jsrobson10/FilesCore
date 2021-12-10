
#include "Util.hpp"

uint64_t Util::get_net_ul(const uint8_t* data)
{
	return (((uint64_t)data[0] << 56) ^
			((uint64_t)data[1] << 48) ^
			((uint64_t)data[2] << 40) ^
			((uint64_t)data[3] << 32) ^
			((uint64_t)data[4] << 24) ^
			((uint64_t)data[5] << 16) ^
			((uint64_t)data[6] << 8) ^
			((uint64_t)data[7]));
}

uint32_t Util::get_net_ui(const uint8_t* data)
{
	return (((uint32_t)data[0] << 24) ^
			((uint32_t)data[1] << 16) ^
			((uint32_t)data[2] << 8) ^
			((uint32_t)data[3]));
}

uint16_t Util::get_net_us(const uint8_t* data)
{
	return (((uint16_t)data[0] << 8) ^
			((uint16_t)data[1]));
}

void Util::set_net_ul(uint8_t* data, uint64_t value)
{
	data[0] = (value >> 56) & 255;
	data[1] = (value >> 48) & 255;
	data[2] = (value >> 40) & 255;
	data[3] = (value >> 32) & 255;
	data[4] = (value >> 24) & 255;
	data[5] = (value >> 16) & 255;
	data[6] = (value >> 8) & 255;
	data[7] = value & 255;
}

void Util::set_net_ui(uint8_t* data, uint32_t value)
{
	data[0] = (value >> 24) & 255;
	data[1] = (value >> 16) & 255;
	data[2] = (value >> 8) & 255;
	data[3] = value & 255;
}

void Util::set_net_us(uint8_t* data, uint16_t value)
{
	data[0] = (value >> 8) & 255;
	data[1] = value & 255;
}

bool Util::compare(const uint8_t* data1, const uint8_t* data2, size_t len)
{
	const uint8_t* data1_end = data1 + len;

	while(data1 < data1_end)
	{
		if(data1[0] != data2[0])
		{
			return false;
		}

		else
		{
			data1 += 1;
			data2 += 1;
		}
	}

	return true;
}

