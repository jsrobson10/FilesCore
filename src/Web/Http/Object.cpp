
#include "./Http.hpp"
#include "../../Util/Util.hpp"

using namespace Web::Http;

Object::Object(Web::Client& client)
{
	this->client = &client;
}

Object::Object(Web::Client* client)
{
	this->client = client;
}

bool Object::read_main()
{
	char buff[1024*1024];
	size_t total_len = 0;

	for(;;)
	{
		size_t len = client->read_line(buff, sizeof(buff));

		if(len == 0 || Util::Text::count_whitespace(buff) == len)
		{
			break;
		}

		char* at = buff;
		char* key_start;
		char* data_start;

		size_t key_len = 0;
		size_t data_len = 0;

		total_len += len;
		if(total_len > sizeof(buff))
		{
			return false;
		}

		at = Util::Text::skip_whitespace(at);
		key_start = at;

		for(;;)
		{
			char c = at[0];

			if(c == '\0' || c == ' ' || c == '\t' || c == '\r' || c == ':')
			{
				key_len = (size_t)(at - buff);

				break;
			}

			at++;
		}

		for(;;)
		{
			char c = at[0];

			if(c == '\0')
			{
				break;
			}

			if(c == ':')
			{
				at++;

				break;
			}

			at++;
		}

		at = Util::Text::skip_whitespace(at);
		data_len = len - (size_t)(at - buff);
		data_start = at;

		std::string key(key_start, key_len);
		std::string data(data_start, data_len);

		set_header(key, data);
	}


	return true;
}

void Object::write_main()
{
	for(auto& header : headers)
	{
		client->write(header.first.c_str());
		client->write(": ");
		client->write(header.second.c_str());
		client->write("\r\n");
	}

	client->write("\r\n");
}

size_t Object::get_body_len()
{
	std::string body_len_str = get_header("content-length");

	try
	{
		return std::stoul(body_len_str);
	}

	catch(std::out_of_range& e) {}
	catch(std::invalid_argument& e) {}

	return 0;
}

void Object::set_header(const std::string& key, const std::string& value)
{
	headers[Util::Text::to_cased(key)] = value;
}

const std::string& Object::get_header(const std::string& key)
{
	return headers[Util::Text::to_cased(key)];
}

void Object::remove_header(const std::string& key)
{
	headers.erase(Util::Text::to_cased(key));
}

