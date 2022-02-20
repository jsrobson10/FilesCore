
#include "./Util.hpp"

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

		c = (data++)[0];
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

		c = (data++)[0];
	}

	return data;
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


