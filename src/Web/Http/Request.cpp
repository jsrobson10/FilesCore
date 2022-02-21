
#include "./Http.hpp"
#include "../../Util/Util.hpp"

#include <cstring>

using namespace Web::Http;

bool Request::read(Web::Client& client)
{
	{
		char buff[1024*1024];
		char* at = buff;
		char* next;

		size_t i = 0;
		size_t len;
		
		size_t line_len = client.read_line(buff, sizeof(buff));

		// get the first bit of the header, will be the request type
		at = Util::Text::skip_whitespace(at);
		next = Util::Text::skip_until_whitespace(at);
		len = next - at;

		Util::Text::to_upper(at, at, len);
		method = std::string(at, len);

		// get the path
		at = Util::Text::skip_whitespace(next);
		next = Util::Text::skip_until_whitespace(at);
		len = next - at;

		path = std::string(at, len);

		// check that this is http/1.1
		at = Util::Text::skip_whitespace(next);
		len = line_len - (size_t)(at - buff);

		Util::Text::to_upper(at, at, len);
		 
		if(len != 8 || memcmp(at, "HTTP/1.1", 8) != 0)
		{
			return false;
		}
	}
	
	return read_main(client);
}

void Request::write(Web::Client& client)
{
	client.write(method.c_str());
	client.write(" ");
	client.write(path.c_str());
	client.write(" HTTP/1.1\r\n");
	write_main(client);
}

