
#include <iostream>
#include <cstring>

#include <openssl/rand.h>
#include <openssl/sha.h>

#include "HashMap.hpp"
#include "Util.hpp"

static void display_hex(const uint8_t* data, size_t len)
{
	const char HEX[] = {'0','1','2','3','4','5','6','7','8','9','a','b','c','d','e','f'};

	for(size_t i = 0; i < len; i++)
	{
		std::cout.write(&HEX[(data[i] >> 4) & 15], 1);
		std::cout.write(&HEX[data[i] & 15], 1);
	}
}

int main()
{
	HashMap db_root("database/db_", "", 256);
	HashMap db(db_root, "fib_test_0");

	for(uint64_t i = 0; i < 92; i++)
	{
		uint8_t i_c[8];
		Util::set_net_ul(i_c, i);

		uint8_t id[32];
		SHA256(i_c, 8, id);
		
		HashMap::Location loc(db, id);
		
		uint8_t v_c[8];
		loc.read(v_c, 8);
		loc.close();

		std::cout << i << ": " << Util::get_net_ul(v_c) << std::endl;
	}

	/*uint64_t a = 1;
	uint64_t b = 0;
	uint64_t c = 0;

	for(uint64_t i = 0; i < 92; i++)
	{
		c = a + b;
		a = b;
		b = c;

		uint8_t i_c[8];
		uint8_t v_c[8];
		uint8_t id[32];

		Util::set_net_ul(i_c, i);
		Util::set_net_ul(v_c, c);
		SHA256(i_c, 8, id);

		display_hex(i_c, 8); std::cout << ": "; display_hex(v_c, 8); std::cout << "\n";

		HashMap::Location loc(db, id);

		loc.create(8);
		loc.write(v_c, 8);
		loc.close();
	}*/
}

