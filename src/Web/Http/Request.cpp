
#include "./Http.hpp"
#include "../../Util/Util.hpp"

#include <cstring>

using namespace Web::Http;

bool Request::read()
{
	{
		char buff[1024*1024];
		char* at = buff;
		char* next;

		size_t i = 0;
		size_t len;
		
		size_t line_len = client->read_line(buff, sizeof(buff));

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
	
	return read_main();
}

void Request::write()
{
	client->write(method.c_str());
	client->write(" ");
	client->write(path.c_str());
	client->write(" HTTP/1.1\r\n");
	write_main();
}

void Request::write(const char* body)
{
	set_header("content-length", std::to_string(strlen(body)));

	write();
	client->write(body);
}

void Request::set(const std::string& method, const std::string& path)
{
	this->method = method;
	this->path = path;
}

void Request::process_path(std::string*& paths, size_t& pathslen)
{
	const char* path_at = path.c_str();
	const char* seg;
	size_t seglen;

	path_at = Util::Text::get_between('/', path_at, seg, seglen);

	std::list<std::string> paths_l;

	while(seglen > 0)
	{
		paths_l.push_back(std::string(seg, seglen));
		path_at = Util::Text::get_between('/', path_at, seg, seglen);
	}

	size_t it = 0;

	pathslen = paths_l.size();
	paths = new std::string[pathslen];
	
	for(std::string path : paths_l)
	{
		paths[it++] = path;
	}
}

