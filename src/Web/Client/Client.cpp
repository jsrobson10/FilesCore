
#include "./Client.hpp"

#include <cstring>

#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>

void Web::Client::cleanup()
{
	shutdown(sockfd, SHUT_RDWR);
	sockfd = -1;
}

size_t Web::Client::read(char* data, size_t len)
{
	return ::read(sockfd, data, len);
}

// read the next line, stopping either when the buffer is maxed out or
// at a new line, with a valid new line being "\r\n" or "\n"
size_t Web::Client::read_line(char* data, size_t bufflen)
{
	char c;
	size_t at = 0;
	bool ret_flag = false;

	mtx.lock();

	for(size_t i = 0; i < bufflen;)
	{
		::read(sockfd, &c, 1);

		if(c == '\r')
		{
			ret_flag = true;
			continue;
		}

		if(c == '\n')
		{
			mtx.unlock();
			data[i] = '\0';

			return i;
		}

		if(ret_flag && c != '\n')
		{
			data[i] = '\r';
			data[i+1] = c;
			i += 2;
		}

		else
		{
			data[i] = c;
			i += 1;
		}
	}

	mtx.unlock();
	data[bufflen - 1] = '\0';

	return bufflen - 1;
}

size_t Web::Client::write(const char* data, size_t len)
{
	return ::write(sockfd, data, len);
}

size_t Web::Client::write(const char* data)
{
	return ::write(sockfd, data, strlen(data));
}

