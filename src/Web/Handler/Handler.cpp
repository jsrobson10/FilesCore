
#include "./Handler.hpp"
#include "../../Util/Util.hpp"

#include <iostream>
#include <sstream>

static void handle_root(Web::Http::Request& req)
{
	
}

void Web::Handler::handle_404(Web::Http::Request& req, std::string* path, size_t pathlen)
{
	std::stringstream buf;

	buf << "<!DOCTYPE html>";
	buf << "<html><head>";
	buf << "<title>Page not found</title>";
	buf << "</head><body>";
	buf << "<h1>404 Page not found</h1>";
	buf << "<p>";
	buf << "The page you have tried to access at /";

	for(size_t i = 0; i < pathlen - 1; i++)
	{
		buf << Util::Text::xss_safe(path[i]) << "/";
	}

	if(pathlen != 0)
	{
		buf << Util::Text::xss_safe(path[pathlen - 1]);
	}

	buf << " could not be found. ";
	buf << "Please correct the spelling and try again. ";
	buf << "</p></body></html>";
		
	Web::Http::Response res(req.client);
	res.set_header("content-type", "text/html");
	res.set_header("connection", "keep-alive");
	res.status(404, "Page not found");
	res.write(buf.str().c_str());
}

bool Web::Handler::handle(Web::Http::Request& req)
{
	std::string* path;
	size_t pathlen;

	req.process_path(path, pathlen);

	if(pathlen == 0)
	{
		handle_root(req);

		delete[] path;
		return true;
	}

	std::string first = path[0];

	if(first == "get")
	{
		
	}

	handle_404(req, path, pathlen);

	delete[] path;
	return true;
}

