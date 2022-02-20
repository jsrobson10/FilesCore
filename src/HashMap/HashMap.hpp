
#pragma once

#include <mutex>
#include <fstream>
#include <string>
#include <atomic>

class HashMap;

class HashMap
{
public:
	
	struct Data
	{
		uint64_t at;
		uint64_t end;
		uint64_t file_end;
		std::mutex mtx;
		std::fstream file;
	};

	class Location
	{
	public:
	
		Location(HashMap& map, const uint8_t** ids, int idc);
		Location(HashMap& map, const uint8_t* id);
		~Location();
		
		void read(uint8_t* data, size_t len);
		void write(const uint8_t* data, size_t len);
		void seek(uint64_t pos);
		void shift(int64_t amount);
		void create(uint64_t len);
		uint64_t get_pos();
		void close();
		bool is_open();
		bool exists();

	private:

		uint64_t content_pos;
		uint64_t seek_pos;
		uint64_t table_pos;
		uint32_t file_id;
		uint8_t hash[32];
		
		int hash_index;
		bool locked;
		bool entry_exists;
		HashMap* map;
	};
	
	HashMap(std::string path_1, std::string path_2, uint32_t files, uint64_t alloc_size);
	HashMap(std::string path_1, std::string path_2, uint32_t files, uint64_t alloc_size, bool createNew);
	HashMap(const HashMap& other, const char* database);
	~HashMap();

	uint8_t tag[32];

private:

	Data* data;
	std::atomic<int>* uses;
	uint64_t alloc_size;
	uint32_t files;
};

