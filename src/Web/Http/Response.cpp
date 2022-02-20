
#include "./Http.hpp"

using namespace Web::Http;

bool Response::read(Web::Client& client)
{
	return true;//TODO
}

void Response::write(Web::Client& client)
{
	std::string status_str = std::to_string(status_int);
	
	client.write("HTTP/1.1 ");
	client.write(status_str.c_str());
	client.write(" ");
	client.write(status_code.c_str());
	client.write("\r\n");
	write_main(client);
}

void Response::status(int status_int, const std::string& status_code)
{
	this->status_int = status_int;
	this->status_code = status_code;
}
