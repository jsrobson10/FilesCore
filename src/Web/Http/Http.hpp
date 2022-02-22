#pragma once

#include "../Client/Client.hpp"

#include <unordered_map>
#include <string>
#include <list>

namespace Web::Http
{
	class Object
	{
	protected:

		std::unordered_map<std::string, std::string> headers;
	
		bool read_main();
		void write_main();
	
	public:
		
		Web::Client* client;

		Object(Web::Client& client);
		Object(Web::Client* client);

		void set_header(const std::string& key, const std::string& var);
		const std::string& get_header(const std::string& key);
		void remove_header(const std::string& key);

		size_t get_body_len();
	};

	class Request : public Object
	{
	public:

		using Object::Object;
		
		std::string path = "/";
		std::string method = "GET";

		bool read();
		void write();
		void write(const char* body);

		void set(const std::string& method, const std::string& path);
		void process_path(std::string*& path, size_t& pathlen);
	};

	class Response : public Object
	{
	public:
		
		using Object::Object;

		uint64_t status_int = 200;
		std::string status_code = "OK";

		bool read();
		void write();
		void write(const char* body);

		void status(int status_int, const std::string& status_code);
	};
};

