
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
	
	char buff[1024];
	char* at = buff;
	char* next;

	size_t i = 0;
	size_t len;
	
	client.read_line(buff, sizeof(buff));

	// get the first bit of the header, will be the request type
	at = Util::Text::skip_whitespace(at);
	next = Util::Text::skip_until_whitespace(at);
	len = next - at - 1;

	Util::Text::to_lower(at, at, len);

	char req_type[1024];
	memcpy(req_type, at, len);
	req_type[len] = '\0';

	// get the path
	at = Util::Text::skip_whitespace(next);
	next = Util::Text::skip_until_whitespace(at);
	len = next - at - 1;

	char path[1024];
	memcpy(path, at, len);
	path[len] = '\0';

	// check that this is http/1.1
	at = Util::Text::skip_whitespace(next);
	next = Util::Text::skip_until_whitespace(at);
	len = next - at - 1;

	Util::Text::to_lower(at, at, len);
	 
	if(len != 8 || memcmp(at, "http/1.1", 8) != 0)
	{
		client.cleanup();
		return;
	}

	std::cout << "will " << req_type << " to " << path << std::endl;

	for(;;)
	{
		client.read_line(buff, sizeof(buff));
		std::cout << i++ << ": " << buff << std::endl;

		if(Util::Text::count_whitespace(buff) == 0)
		{
			break;
		}
	}

	Web::Http::Response res;
	res.set_header("Foo", "Bar");
	res.write(client);
}

bool Web::Server::running()
{
	return (sockfd != -1);
}

