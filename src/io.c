#include <io.h>

struct buffer_information
{
	char *buffer;
	uint8_t is_buffered;
	uint32_t buffer_position;
	uint32_t buffer_capacity;
};

int8_t get_base(char c)
{
	if 		(c == 'o') return 8;
	else if (c == 'u') return 10;
	else if (c == 'x') return 16; 
	return -1;
}

int printf_for_char(char c, struct buffer_information* info)
{
	if (info -> is_buffered)	
	{
/*		write_char_to_serial('?');
		write_char_to_serial(c);
		write_char_to_serial('\n');
*/
		if ((info -> buffer_position) + 1 < (info -> buffer_capacity))
		{
			(info -> buffer)[(info -> buffer_position)++] = c;
		}
	}
	else
		write_char_to_serial(c);
	return 1;
}

int printf_for_unsigned_number(uint64_t n, int8_t base, struct buffer_information* info)
{
	if (base == -1) write_char_to_serial('!');

	char buf[25];
	buf[0] = 0;
	int len = (n == 0), result = 0;	
	while (n > 0)
	{
		buf[len++] = n % base;
		n /= base;
	}

	len--;
	while (len >= 0)
	{
		// we have "digits" 0123456789abcdef		
		result += printf_for_char(buf[len] + ((buf[len] > 9) ? ('a' - 10) : '0'), info);
		len--;
	}
	return result;
}

int printf_for_signed_number(int64_t n, char base, struct buffer_information* info)
{
	int result = 0;

	if (n < 0)
	{
		printf_for_char('-', info);
		n *= -1;
		result = 1;
	}
	return result += printf_for_unsigned_number((uint64_t)n, base, info);
}

int printf_parser(const char* format, va_list args, struct buffer_information* info)
{
	int result = 0; // number of printed symbols    
    
	for (const char* now = format; *now; now++)
	{
		if (*now != '%')
		{
			result += printf_for_char(*now, info);
			continue;   
		}

		now++;
		if (*now == '%') // if we need to print '%' symbol using printf
		{
			result += printf_for_char('%', info);
			continue;
		} 

		//singlecharacter
		if (*now == 'c')
		{
			char c = (char)(va_arg(args, int));
			result += printf_for_char(c, info);
			continue;
		}
        if (*now == 's')
        {
            char* s = va_arg(args, char*);
			while (*s)
            {
                result += printf_for_char(*(s++), info);
            }
            continue;
        }
        if (*now == 'd' || *now == 'i')
        {
            int n = va_arg(args, int);
            result += printf_for_signed_number(n, 10, info);
            continue;            
        }
        if (get_base(*now) != -1)
        {
            unsigned int n = va_arg(args, unsigned int);
            result += printf_for_unsigned_number(n, get_base(*now), info);
            continue;
        }

		//multicharacter, starts with 'h'
		if (*now == 'h')
		{
			now++;
			if (*now == 'd' || *now == 'i')
			{
				short n = (short)(va_arg(args, int));
				result += printf_for_signed_number(n, 10, info);
				continue;
			}
			if (get_base(*now) != -1)
			{
				unsigned short n = (unsigned short)(va_arg(args, unsigned int));
				result += printf_for_unsigned_number(n, get_base(*now), info);
				continue;
			}   

			//multicharacter, starts with 'hh'
			if (*now == 'h')
			{
				now++;				
				if (*now == 'd' || *now == 'i')
				{
					short n = (short)(va_arg(args, int));
					result += printf_for_signed_number(n, 10, info);
					continue;
				}
				if (get_base(*now) != -1)
				{
					unsigned short n = (unsigned short)(va_arg(args, unsigned int));
					result += printf_for_unsigned_number(n, get_base(*now), info);
					continue;
				}   
			}    
		}
		//multicharacter, starts with 'l'
		if (*now == 'l')
		{
			now++;
			if (*now == 'd' || *now == 'i')
			{
				long n = (long)(va_arg(args, long));
				result += printf_for_signed_number(n, 10, info);
				continue;
			}
			if (get_base(*now) != -1)
			{
				unsigned long n = (unsigned long)(va_arg(args, unsigned long));
				result += printf_for_unsigned_number(n, get_base(*now), info);
				continue;
			}   

			//multicharacter, starts with 'll'
			if (*now == 'l')
			{
				now++;
				if (*now == 'd' || *now == 'i')
				{
					long long n = (long long)(va_arg(args, long long));
					result += printf_for_signed_number(n, 10, info);
					continue;
				}
				if (get_base(*now) != -1)
				{
					unsigned long long n = (unsigned long long)(va_arg(args, unsigned long long));
					result += printf_for_unsigned_number(n, get_base(*now), info);
					continue;
				}   
			}    
		}

	}
	return result;
}  
int vprintf(const char *format, va_list args)
{
	struct buffer_information info;
	info.is_buffered = 0;

	return printf_parser(format, args, &info);
}

int printf(const char* format, ...)
{
	va_list args;
	va_start(args, format);
	int result = vprintf(format, args);
	va_end(args);
	return result;
}      

int vsnprintf(char* s, size_t n, const char* format, va_list args)
{
	struct buffer_information info;
	info.buffer = s;
	info.is_buffered = 1;
	info.buffer_position = 0;
	info.buffer_capacity = n;

	return printf_parser(format, args, &info);

}
int snprintf(char* s, size_t n, const char* format, ...)
{
	va_list args;
	va_start(args, format);
	int result = vsnprintf(s, n, format, args);
	va_end(args);
	return result;
}

