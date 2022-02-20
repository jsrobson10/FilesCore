#pragma once

#include <thread>
#include <mutex>

namespace Web::Server
{
	bool running();
	void init();
	void run();
	void accept(int sockfd);
	void cleanup();
	void join();
};

