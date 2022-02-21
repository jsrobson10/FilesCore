
#include "./Server.hpp"
#include "../../Config/Config.hpp"
#include "../../Util/Util.hpp"
#include "../Client/Client.hpp"
#include "../Http/Http.hpp"

#include <list>
#include <mutex>
#include <thread>
#include <iostream>
#include <cstring>

#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>

namespace Web
{
	namespace Server
	{
		std::thread handle;
		int sockfd;
	
	    socklen_t addrlen;
	    struct sockaddr_in addr;
	};
};

void Web::Server::init()
{
    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    
    if(sockfd <= 0)            
    {
        sockfd = -1;           
        std::cout << "Socket creation failed\n";
        return;                
    }
    
    int opt = 1;
	int port = Config::port;
    
    if(setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, &opt, sizeof(opt)))
    {
        cleanup();             
        std::cout << "Setsockopt failed\n";
        return;                
    }

    addr.sin_family = AF_INET; 
    addr.sin_addr.s_addr = INADDR_ANY;
    addr.sin_port = htons(port);    
    addrlen = sizeof(addr);    
    
    if(bind(sockfd, (struct sockaddr*)&addr, addrlen) < 0)
    {
        cleanup();             
        std::cout << "Sock bind fained\n";
        return;
    }
    
    if(listen(sockfd, 3) < 0)  
    {
        cleanup();             
        std::cout << "Sock listen failed\n";
        return;
    }

    handle = std::thread(&Web::Server::run);

    std::cout << "HTTP server listening on http://127.0.0.1:" << port << "/" << std::endl;
}

void Web::Server::cleanup()
{
	shutdown(sockfd, SHUT_RDWR);
	sockfd = -1;
}

void Web::Server::join()
{
	handle.join();
}

void Web::Server::run()
{
	for(int i = 0; sockfd != -1; i++)
	{
		int sock_new = accept(sockfd, (struct sockaddr*)&addr, &addrlen);

        if(sock_new < 0)
        {
            continue;
        }

		std::thread th(&Web::Server::accept, sock_new);
		th.detach();
	}
}

void Web::Server::accept(int sockfd)
{
	Web::Client client;
	client.sockfd = sockfd;

	Web::Http::Request req;
	req.read(client);

	std::cout << "client connected with " << req.get_header("user-agent") << std::endl;
	std::cout << "will " << req.method << " to " << req.path << std::endl;

	Web::Http::Response res;
	res.set_header("foo", "bar");
	res.set_header("connection", "close");
	res.set_body("Hello, client!");
	res.write(client);
}

bool Web::Server::running()
{
	return (sockfd != -1);
}

