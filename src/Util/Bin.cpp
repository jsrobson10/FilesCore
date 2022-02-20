
#include "./Util.hpp"

int Util::Bin::compare(const uint8_t* data1, const uint8_t* data2, size_t len)
{
	const uint8_t* data1_end = data1 + len;

	while(data1 < data1_end)
	{
		uint8_t c1 = data1[0];
		uint8_t c2 = data2[0];

		if(c1 > c2)
		{
			return 1;
		}
		
		else if(c1 < c2)
		{
			return -1;
		}

		else
		{
			data1 += 1;
			data2 += 1;
		}
	}

	return 0;
}

