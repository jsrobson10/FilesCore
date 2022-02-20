
#include "./Http.hpp"

using namespace Web::Http;

bool Request::read(Web::Client& client)
{
	return true;//TODO
}

void Request::write(Web::Client& client)
{
	client.write(method.c_str());
	client.write(" ");
	client.write(path.c_str());
	client.write(" HTTP/1.1\r\n");
	write_main(client);
}

