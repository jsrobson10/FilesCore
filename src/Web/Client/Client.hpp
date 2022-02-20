#pragma once

#include <thread>
#include <mutex>

namespace Web
{
	class Client
	{
	public:
	
		std::thread handle;
		std::mutex mtx;
		int sockfd;
	
		size_t read(char* data, size_t len);
		size_t read_line(char* data, size_t bufflen);
		size_t write(const char* data, size_t len);
		size_t write(const char* data);
		void cleanup();
	};
};

