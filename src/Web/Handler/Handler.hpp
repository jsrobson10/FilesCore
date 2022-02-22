
#pragma once

#include "../Client/Client.hpp"
#include "../Http/Http.hpp"

namespace Web::Handler
{
	bool handle(Web::Http::Request& req);
	void handle_404(Web::Http::Request& req, std::string* path, size_t pathlen);
};

