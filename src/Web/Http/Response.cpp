
#include "./Http.hpp"
#include "../../Util/Util.hpp"

#include <cstring>

using namespace Web::Http;

bool Response::read()
{
	{
		char buff[1024*1024];
		char* at = buff;
		char* next;

		size_t i = 0;
		size_t len;
		
		size_t line_len = client->read_line(buff, sizeof(buff));

		// check that this is HTTP/1.1
		at = Util::Text::skip_whitespace(at);
		next = Util::Text::skip_until_whitespace(at);
		len = next - at;

		Util::Text::to_upper(at, at, len);
		
		if(len != 8 || memcmp(at, "HTTP/1.1", 8) != 0)
		{
			return false;
		}

		// get the status int
		at = Util::Text::skip_whitespace(next);
		next = Util::Text::skip_until_whitespace(at);
		len = next - at;

		try
		{
			status_int = std::stoul(std::string(at, len));
		}
	
		catch(std::out_of_range& e) {return false;}
		catch(std::invalid_argument& e) {return false;}

		// get the status code
		at = Util::Text::skip_whitespace(next);
		status_code = std::string(at);
	}
	
	return read_main();
}

void Response::write()
{
	std::string status_str = std::to_string(status_int);
	
	client->write("HTTP/1.1 ");
	client->write(status_str.c_str());
	client->write(" ");
	client->write(status_code.c_str());
	client->write("\r\n");
	write_main();
}

void Response::write(const char* body)
{
	set_header("content-length", std::to_string(strlen(body)));

	write();
	client->write(body);
}

void Response::status(int status_int, const std::string& status_code)
{
	this->status_int = status_int;
	this->status_code = status_code;
}

