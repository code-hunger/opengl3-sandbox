#include <cstdarg>
#include <cstdio>

#include "logger.h"

void Logger::log(const char* msg, ...)
{
	va_list args;
	va_start(args, msg);
	vprintf(msg, args);
	va_end(args);
}
