
#include "./Util.hpp"

#include <sstream>

size_t Util::Text::count_whitespace(const char* text)
{
	char c = text[0];
	size_t found = 0;

	while(c != '\0')
	{
		if(c == ' ' || c == '\t' || c == '\r' || c == '\n')
		{
			found += 1;
		}

		text++;
		c = text[0];
	}

	return found;
}

size_t Util::Text::count_whitespace(const char* text, size_t len)
{
	const char* end = text + len;
	size_t found = 0;

	while(text < end)
	{
		char c = text[0];

		if(c == ' ' || c == '\t' || c == '\r' || c == '\n')
		{
			found += 1;
		}

		text++;
	}

	return found;
}

size_t Util::Text::count_whitespace(const std::string& text)
{
	return count_whitespace(text.c_str(), text.length());
}

uint64_t Util::Text::get_char_pos(char c, const char* text)
{
	char at = text[0];
	uint64_t it = 0;

	while(at != '\0')
	{
		if(at == c)
		{
			return it;
		}

		it++;
		text++;
		at = text[0];
	}

	return -1;
}

uint64_t Util::Text::get_char_pos(char c, const char* text, size_t len)
{
	const char* end = text + len;
	uint64_t it = 0;

	while(text < end)
	{
		if(text[0] == c)
		{
			return it;
		}

		text++;
		it++;
	}

	return -1;
}

uint64_t Util::Text::get_char_pos(char c, const std::string& text)
{
	return get_char_pos(c, text.c_str(), text.length());
}

void Util::Text::to_upper(const char* in, char* out)
{
	char c = in[0];

	while(c != '\0')
	{
		if(c >= 'a' && c <= 'z')
		{
			c -= 32;
		}

		(out++)[0] = c;
		c = (++in)[0];
	}
}

void Util::Text::to_lower(const char* in, char* out)
{
	char c = in[0];

	while(c != '\0')
	{
		if(c >= 'A' && c <= 'Z')
		{
			c += 32;
		}

		(out++)[0] = c;
		c = (++in)[0];
	}
}

void Util::Text::to_cased(const char* in, char* out)
{
	char c = in[0];
	bool caps = true;

	while(c != '\0')
	{
		if(caps)
		{
			if(c >= 'a' && c <= 'z')
			{
				c -= 32;
			}

			caps = false;
		}

		else
		{
			if(c >= 'A' && c <= 'Z')
			{
				c += 32;
			}
		}

		if(!((c >= 'A' && c <= 'Z') || (c >= 'a' && c <= 'z')))
		{
			caps = true;
		}

		(out++)[0] = c;
		c = (++in)[0];
	}
}

void Util::Text::to_upper(const char* in, char* out, size_t len)
{
	char c;
	const char* in_end = in + len;

	while(in < in_end)
	{
		c = (in++)[0];

		if(c >= 'a' && c <= 'z')
		{
			c -= 32;
		}

		(out++)[0] = c;
	}
}

void Util::Text::to_lower(const char* in, char* out, size_t len)
{
	char c;
	const char* in_end = in + len;

	while(in < in_end)
	{
		c = (in++)[0];

		if(c >= 'A' && c <= 'Z')
		{
			c += 32;
		}

		(out++)[0] = c;
	}
}

void Util::Text::to_cased(const char* in, char* out, size_t len)
{
	char c;
	const char* in_end = in + len;
	bool caps = true;

	while(in < in_end)
	{
		c = (in++)[0];

		if(caps)
		{
			if(c >= 'a' && c <= 'z')
			{
				c -= 32;
			}

			caps = false;
		}

		else
		{
			if(c >= 'A' && c <= 'Z')
			{
				c += 32;
			}
		}

		if(!((c >= 'A' && c <= 'Z') || (c >= 'a' && c <= 'z')))
		{
			caps = true;
		}

		(out++)[0] = c;
	}
}

std::string Util::Text::to_upper(const std::string& text)
{
	size_t text_len = text.length();
	char* text_out = new char[text_len];
	to_upper(text.c_str(), text_out, text_len);

	std::string str(text_out, text_len);

	delete[] text_out;
	return str;
}

std::string Util::Text::to_lower(const std::string& text)
{
	size_t text_len = text.length();
	char* text_out = new char[text_len];
	to_lower(text.c_str(), text_out, text_len);

	std::string str(text_out, text_len);

	delete[] text_out;
	return str;
}

std::string Util::Text::to_cased(const std::string& text)
{
	size_t text_len = text.length();
	char* text_out = new char[text_len];
	to_cased(text.c_str(), text_out, text_len);

	std::string str(text_out, text_len);

	delete[] text_out;
	return str;
}

template <class T>
T* skip_whitespace_t(T* data)
{
	char c = data[0];
	
	while(c != '\0')
	{
		if(c != ' ' && c != '\t' && c != '\n' && c != '\r')
		{
			break;
		}

		c = (++data)[0];
	}

	return data;
}

template <class T>
T* skip_until_whitespace_t(T* data)
{
	char c = data[0];
	
	while(c != '\0')
	{
		if(c == ' ' || c == '\t' || c == '\n' || c == '\r')
		{
			break;
		}

		c = (++data)[0];
	}

	return data;
}

template <class T>
T* get_between_t(char c, T* text, T*& out, size_t& len)
{
	char at = text[0];

	while(at == c && at != '\0')
	{
		at = (++text)[0];
	}

	out = text;

	while(at != c && at != '\0')
	{
		at = (++text)[0];
	}

	len = text - out;

	return text;
}
		
char* Util::Text::get_between(char c, char* text, char*& out, size_t& len)
{
	return get_between_t<char>(c, text, out, len);
}

const char* Util::Text::get_between(char c, const char* text, const char*& out, size_t& len)
{
	return get_between_t<const char>(c, text, out, len);
}

char* Util::Text::skip_whitespace(char* data)
{
	return skip_whitespace_t<char>(data);
}

const char* Util::Text::skip_whitespace(const char* data)
{
	return skip_whitespace_t<const char>(data);
}

char* Util::Text::skip_until_whitespace(char* data)
{
	return skip_until_whitespace_t<char>(data);
}

const char* Util::Text::skip_until_whitespace(const char* data)
{
	return skip_until_whitespace_t<const char>(data);
}

std::string Util::Text::xss_safe(const std::string& str)
{
	std::stringstream buf;

	for(size_t i = 0; i < str.length(); i++)
	{
		char c = str[i];

		if(c == '&')       buf << "&amp";
		else if(c == '<')  buf << "&lt";
		else if(c == '>')  buf << "&gt";
		else if(c == '"')  buf << "&quot";
		else if(c == '\'') buf << "&#x27";
		else               buf << c;
	}

	return buf.str();
}

