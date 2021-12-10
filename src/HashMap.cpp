
#include "HashMap.hpp"
#include "Util.hpp"

#include <cstring>

#include <openssl/sha.h>
#include <openssl/rand.h>

#define TABLE_SIZE 589824

static void create_table(std::ostream& file)
{
	const uint8_t bytes[9] = {0};

	for(int i = 0; i < 65536; i++)
	{
		file.write((const char*)bytes, sizeof(bytes));
	}
}

static std::string format_uint(uint32_t value)
{
	char str[5];

	for(int i = 4; i >= 0; i--)
	{
		uint32_t v = value % 10;
		str[i] = '0' + v;
		value /= 10;
	}

	return std::string(str, 5);
}

HashMap::Location::Location(HashMap& map, const uint8_t* id) : HashMap::Location(map, &id, 1) {}

HashMap::Location::Location(HashMap& map, const uint8_t** ids, int idc)
{
	this->map = &map;

	locked = true;
	
	// create a unique ID based on the current database
	// and all ids passed in
	SHA256_CTX ctx;
	SHA256_Init(&ctx);
	SHA256_Update(&ctx, map.tag, 32);
	
	for(int i = 0; i < idc; i++)
	{
		SHA256_Update(&ctx, ids[i], 32);
	}

	SHA256_Final(hash, &ctx);
	file_id = Util::get_net_ui(&hash[28]) % map.files;

	uint8_t* hash_at = hash;
	Data* data = &map.data[file_id];

	data->file.seekg(0, std::ios::beg);
	data->at = 0;

	for(int i = 0; i < 16; i++)
	{
		int jump = (hash_at[0] * 256 + hash_at[1]) * 9;

		data->file.seekg(jump, std::ios::cur);
		data->at += jump;

		uint8_t table_tag;
		data->file.read((char*)&table_tag, 1);

		switch(table_tag)
		{
			// empty table entry
			// mark and return early
			case 0:
			{
				entry_exists = false;
				table_pos = data->at;
				hash_index = (int)(hash_at - hash);

				return;
			}

			// table entry is another table
			// follow to the next table
			case 1:
			{
				uint8_t pos_c[8];
				data->file.read((char*)pos_c, 8);

				uint64_t pos = Util::get_net_ul(pos_c);
				data->file.seekg(pos, std::ios::beg);
				data->at = pos;

				break;
			}

			// table entry is a value
			// check to see if it is the value,
			// otherwise return early so we can decide
			// to create it or not
			default:
			{
				uint8_t pos_c[8];
				data->file.read((char*)pos_c, 8);

				uint64_t pos = Util::get_net_ul(pos_c);
				data->file.seekg(pos, std::ios::beg);

				uint8_t hash_c[32];
				data->file.read((char*)hash_c, 32);

				// return and unlock the resource if the
				// value has been found
				if(Util::compare(hash_c, hash, 32))
				{
					data->at = pos + 32;
					content_pos = data->at;
					seek_pos = data->at;
					entry_exists = true;

					return;
				}

				// return with a locked resource, things
				// still need to be done to initialize this value
				else
				{
					hash_index = (int)(hash_at - hash) + 2;
					table_pos = data->at;
					data->at = pos + 32;
					entry_exists = false;

					return;
				}
			}
		}

		hash_at += 2;
	}
}

HashMap::Location::~Location()
{
	if(locked)
	{
		map->data[file_id].mtx.unlock();
	}
}


void HashMap::Location::create(uint64_t len)
{
	if(entry_exists)
	{
		throw std::runtime_error("hash location already exists");
	}

	if(!locked)
	{
		throw std::runtime_error("hash location is already closed");
	}

	Data* data = &map->data[file_id];

	data->file.seekg(table_pos, std::ios::beg);
	data->at = table_pos;

	uint8_t table_tag;
	data->file.read((char*)&table_tag, 1);

	// another value, create a new table and
	// move the current and next pointers into it
	if(table_tag != 0)
	{
		uint8_t pointer_cur_c[8];
		uint8_t pointer_table_c[8];
		uint8_t pointer_next_c[8];

		uint64_t pointer_table = data->end;
		uint64_t pointer_next = data->end + TABLE_SIZE;

		Util::set_net_ul(pointer_table_c, pointer_table);
		Util::set_net_ul(pointer_next_c, pointer_next);

		data->file.read((char*)pointer_cur_c, 8);
		data->file.seekg(-9, std::ios::cur);
		
		// replace the current table entry with a
		// pointer to another table
		table_tag = 1;
		data->file.write((char*)&table_tag, 1);
		data->file.write((char*)pointer_table_c, 8);
		data->file.seekg(pointer_table, std::ios::beg);

		create_table(data->file);

		data->file.write((char*)hash, 32);

		// allocate the data and fill it with zeros
		if(len > 0)
		{
			data->file.seekg(len - 1, std::ios::cur);
			data->file.write("\0", 1);
		}

		// get the current table entry hash value
		// to calculate its new table position
		uint8_t hash_cur[32];
		uint64_t pointer_cur = Util::get_net_ul(pointer_cur_c);
		data->file.seekg(pointer_cur, std::ios::beg);
		data->file.read((char*)hash_cur, 32);

		int jump_cur = (hash_cur[hash_index] * 256 + hash_cur[hash_index + 1]) * 9;
		int jump_next = (hash[hash_index] * 256 + hash[hash_index + 1]) * 9;

		table_tag = 2;

		// set both table entries
		data->file.seekg(pointer_table + jump_cur, std::ios::beg);
		data->file.write((char*)&table_tag, 1);
		data->file.write((char*)pointer_cur_c, 8);
		data->file.seekg(pointer_table + jump_next, std::ios::beg);
		data->file.write((char*)&table_tag, 1);
		data->file.write((char*)pointer_next_c, 8);

		data->at = pointer_table + jump_next + 9;
		data->end += TABLE_SIZE + len + 32;

		content_pos = pointer_next + 32;
		seek_pos = content_pos;
		entry_exists = true;
	}

	// empty table entry, allocate the data
	// and set the table entry
	else
	{
		uint64_t pointer_next = data->end;
		uint8_t pointer_next_c[8];

		Util::set_net_ul(pointer_next_c, pointer_next);

		table_tag = 2;

		// set the table entry and set the hash
		data->file.seekg(-1, std::ios::cur);
		data->file.write((char*)&table_tag, 1);
		data->file.write((char*)pointer_next_c, 8);
		data->file.seekg(pointer_next, std::ios::beg);
		data->file.write((char*)hash, 32);
		
		// allocate the data and fill it with zeros
		if(len > 0)
		{
			data->file.seekg(len - 1, std::ios::cur);
			data->file.write("\0", 1);
		}

		data->end += len + 32;
		data->at = data->end;
		
		content_pos = pointer_next + 32;
		seek_pos = content_pos;
		entry_exists = true;
	}
}

void HashMap::Location::close()
{
	if(locked)
	{
		map->data[file_id].mtx.unlock();
		locked = false;
	}
}

static void check_status(bool exists, bool locked)
{
	if(!exists)
	{
		throw std::runtime_error("hash location does not exist");
	}

	if(!locked)
	{
		throw std::runtime_error("hash location is closed");
	}
}

bool HashMap::Location::exists()
{
	if(!locked)
	{
		throw std::runtime_error("hash location is closed");
	}
	
	return entry_exists;
}

bool HashMap::Location::is_open()
{
	return locked;
}

void HashMap::Location::read(uint8_t* buff, size_t len)
{
	check_status(entry_exists, locked);

	Data* data = &map->data[file_id];

	if(seek_pos != data->at)
	{
		data->at = seek_pos;
		data->file.seekg(seek_pos, std::ios::beg);
	}

	data->file.read((char*)buff, len);
}

void HashMap::Location::write(const uint8_t* buff, size_t len)
{
	check_status(entry_exists, locked);
	
	Data* data = &map->data[file_id];

	if(seek_pos != data->at)
	{
		data->at = seek_pos;
		data->file.seekg(seek_pos, std::ios::beg);
	}

	data->file.write((char*)buff, len);
}

void HashMap::Location::seek(uint64_t pos)
{
	check_status(entry_exists, locked);

	seek_pos = content_pos + pos;
}

void HashMap::Location::shift(int64_t amount)
{
	check_status(entry_exists, locked);

	seek_pos += amount;
}

uint64_t HashMap::Location::get_pos()
{
	check_status(entry_exists, locked);

	return seek_pos - content_pos;
}

HashMap::HashMap(std::string path_1, std::string path_2, uint32_t files) : HashMap(path_1, path_2, files, false) {}

HashMap::HashMap(std::string path_1, std::string path_2, uint32_t files, bool create_new)
{
	this->data = new Data[files];
	this->uses = new std::atomic<int>(1);
	this->files = files;

	if(!create_new)
	{
		std::ifstream file_seed(path_1 + "root" + path_2, std::ios::binary);

		if(file_seed.is_open())
		{
			file_seed.read((char*)tag, 32);

			if(file_seed.tellg() != 32)
			{
				create_new = true;
			}

			file_seed.close();
		}

		else
		{
			create_new = true;
		}
	}

	if(!create_new)
	{
		for(int i = 0; i < files; i++)
		{
			Data* d = &data[i];

			d->at = 0;
			d->file.open(path_1 + format_uint(i) + path_2, std::ios::binary | std::ios::in | std::ios::out);

			// create a new database if theres an issue
			// with opening the file
			if(!d->file.is_open())
			{
				create_new = true;
				d->file.close();

				break;
			}
		}
	}

	// create a new database whether it was specified
	// or it was needed
	if(create_new)
	{
		std::fstream file(path_1 + "root" + path_2, std::ios::binary | std::ios::out);

		RAND_bytes(tag, 32);
		file.write((char*)tag, 32);
		file.close();

		for(int i = 0; i < files; i++)
		{
			Data* d = &data[i];

			std::string path = path_1 + format_uint(i) + path_2;
			std::fstream file(path, std::ios::binary | std::ios::out);
			create_table(file);
			file.close();

			d->at = 0;
			d->file.open(path, std::ios::binary | std::ios::in | std::ios::out);

			// this shouldn't happen, so throw
			// an error so the person will know
			if(!d->file.is_open())
			{
				throw std::runtime_error("cannot open file");
			}
		}
	}

	for(int i = 0; i < files; i++)
	{
		Data* d = &data[i];

		d->file.seekg(0, std::ios::end);
		d->at = d->file.tellg();
		d->end = d->at;
	}
}

HashMap::HashMap(const HashMap& other, const char* database)
{
	data = other.data;
	uses = other.uses;
	files = other.files;

	// do this to remember how many copies of this
	// object there are so the data can be automatically
	// cleaned up
	*uses += 1;

	// calculate the next tag so all database ids will
	// be different and there will be no collisions
	// between databases
	SHA256_CTX ctx;
	SHA256_Init(&ctx);
	SHA256_Update(&ctx, other.tag, 32);
	SHA256_Update(&ctx, (const uint8_t*)database, strlen(database));
	SHA256_Final(tag, &ctx);
}

HashMap::~HashMap()
{
	// clean up the data if there's no copies left
	if(--*uses == 0)
	{
		delete[] data;
		delete uses;
	}
}

