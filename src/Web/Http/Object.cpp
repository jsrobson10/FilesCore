
#include "./Http.hpp"
#include "../../Util/Util.hpp"

using namespace Web::Http;

bool Object::read_main(Web::Client& client)
{
	return true; //TODO
}

void Object::write_main(Web::Client& client)
{
	set_header("content-length", std::to_string(body.length()));
	
	for(auto& header : headers)
	{
		client.write(header.first.c_str());
		client.write(": ");
		client.write(header.second.c_str());
		client.write("\r\n");
	}

	client.write("\r\n");
	client.write(body.c_str(), body.length());
}

void Object::set_body(const std::string& body)
{
	this->body = body;
}

void Object::set_header(const std::string& key, const std::string& value)
{
	size_t key_len = key.length();
	char* key_data = new char[key_len];

	Util::Text::to_cased(key.c_str(), key_data, key_len);
	std::string key_cased(key_data, key_len);
	
	delete[] key_data;

	headers[key_cased] = value;
}

const std::string& Object::get_body()
{
	return body;
}

const std::string& Object::get_header(const std::string& key)
{
	size_t key_len = key.length();
	char* key_data = new char[key_len];

	Util::Text::to_cased(key.c_str(), key_data, key_len);
	std::string key_cased(key_data, key_len);
	
	delete[] key_data;
	return headers[key_cased];
}

