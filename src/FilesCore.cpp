
#include <iostream>

#include "./Web/Server/Server.hpp"

#include <unistd.h>
#include <signal.h>

static void signal_handler(int sig)
{
	Web::Server::cleanup();
}

int main()
{
	signal(SIGPIPE, SIG_IGN);
	signal(SIGABRT, signal_handler);
	signal(SIGINT, signal_handler);
	
	Web::Server::init();
	Web::Server::join();

	return 0;
}

