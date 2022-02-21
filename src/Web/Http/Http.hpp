#pragma once

#include "../Client/Client.hpp"

#include <unordered_map>
#include <string>

namespace Web::Http
{
	class Object
	{
	protected:

		std::unordered_map<std::string, std::string> headers;
		bool body_set = false;
		std::string body;
	
		bool read_main(Web::Client& client);
		void write_main(Web::Client& client);
	
	public:

		void set_header(const std::string& key, const std::string& var);
		void set_body(const std::string& key);

		const std::string& get_header(const std::string& key);
		const std::string& get_body();

		void remove_header(const std::string& key);
		void remove_body();
	};

	class Request : public Object
	{
	public:
		
		std::string path = "/";
		std::string method = "GET";

		bool read(Web::Client& client);
		void write(Web::Client& client);
	};

	class Response : public Object
	{
	public:

		uint64_t status_int = 200;
		std::string status_code = "OK";

		bool read(Web::Client& client);
		void write(Web::Client& client);

		void status(int status_int, const std::string& status_code);
	};
};

